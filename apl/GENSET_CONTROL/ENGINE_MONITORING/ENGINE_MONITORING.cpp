/**
 * @file        [ENGINE_MONITORING]
 * @brief       ENGINE_MONITORING
 *              Provides common API's for functionality related to the engine.
 *
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        5st June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#include "ENGINE_MONITORING.h"
#include "config.h"
#include "START_STOP.h"
#include "CFGC/CFGC.h"
#include "J1939_APP/J1939APP.h"
#include "../ENGINE_START_VALIDITY/ENGINE_START_VALIDITY.h"

/* supporting private macros */
#define IS_START_STOP_SM_IN_CRANK_STATE()       (  (_u8StartStopSMState == START_STOP::ID_STATE_SS_CRANKING)   \
                                               || (_u8StartStopSMState == START_STOP::ID_STATE_SS_CRANK_REST))
/*
 * SuryaPranayTeja.BVV 10-11-2022
 * The below Macros IS_LOAD_ON_MAINS and IS_LOAD_ON_GEN
 * do not have any Meaning based on the name.
 * They are intended to use for updating BTS run Hrs and to replicate conditions
 * as per NXP.
 */
#define IS_LOAD_ON_MAINS()    (((_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring) && (_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bResultInstant))||((!_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring) && BASE_MODES::IsMainsContactorClosed()))
#define IS_LOAD_ON_GEN()      (((_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring) && (_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bResultInstant))||((!_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring) && BASE_MODES::IsGenContactorClosed()))

uint8_t ENGINE_MONITORING::_u8EngineOff = 0;
uint8_t ENGINE_MONITORING::_u8EngineOn = 0;
uint8_t ENGINE_MONITORING::_u8GenReady = 0;
uint8_t ENGINE_MONITORING::_u8GenAvailable = 0;
bool ENGINE_MONITORING::_bTripLatched = false;
bool ENGINE_MONITORING::_bLoadContactorStatusChanged = false;
ENGINE_MONITORING::LOAD_CONT_STATUS_t ENGINE_MONITORING::_eLoadStatusCurrent = LOAD_NOT_ON_GEN_MAINS;

/*  Shubham Wader 21.09.2022
    todo:  Why we are making _stCummulativeCnt static? If we want to use those outside class then why to keep it
    private ? C++ property misuse . Either need to make it public to access anywhere or restrict access. */
ENGINE_MONITORING::CUMULATIVE_t ENGINE_MONITORING::_stCummulativeCnt={};
ENGINE_MONITORING::CUMULATIVE_t ENGINE_MONITORING::_stStoredCummulativeCnt = {};

bool ENGINE_MONITORING::_bEngineCranked = false;

ENGINE_MONITORING::ENGINE_MONITORING(CFGZ &cfgz, GCU_ALARMS &GCUAlarms, HAL_Manager &hal):
_cfgz(cfgz),
_GCUAlarms(GCUAlarms),
_hal(hal),
_LLOPCrankingTimer{0, false},
_Timer50MS{0, false},
_TimerOneMin{0, false},
_MainsRunTimeBaseTimer{0, false},
_TimerGenUpdateCumulative{0, false},
_TimerMainsUpdateCumulative{0, false},
_TimerBTSUpdateCumulative{0, false},
_TimerUpdateTamperedCumulative{0, false},
_BTSRunTimeBaseTimer{0, false},
_EngWarmUpTimer{0, false},
_GenWarmUpTimer{0, false},
_LOPSensMonTimer{0, false},
_u8StartStopSMState(0),
_u8ActiveSectorForCummulative(0),
_stTampEnergyRegister{},
_stEnergyRegister{},
_stMainsEnergyRegister{},
_bCrankStateLatched(false),
_bFailedCrankStateLatched(false),
_u8OvldExtMinCnt(0),
_u8OvldExtOneHrContCnt(0)
#if (TEST_AUTOMATION == YES)
,_bFromAutomation(false)
#endif
{
    UTILS_ResetTimer(&_Timer50MS);
    prvGetCumulativeCnt();
    ReadEnergySetEnergyOffset(true);
}

void ENGINE_MONITORING::Update()
{
    if(UTILS_GetElapsedTimeInMs(&_Timer50MS) >= FIFTY_MSEC)
    {
        UTILS_ResetTimer(&_Timer50MS);

        /* Instead of creating local instances of sensor structure wherever needed, created private global class limited
           instance for LOP sensor.  */
        _stLOP = _GCUAlarms.GetLOPSensorVal();

        prvCheckTrips();
        prvCheckEngineOff();
        prvUpdateEngineOn();
        prvUpdateEngineCranked();
        prvUpdateGenReady();
        UpdateContactorLoadStatus();
        prvUpdateEngineRunHrs();
        prvUpdateMainsRunHrs();
        prvUpdateBTSRunHrs();
        prvUpdateCumCrankCnts();
    }
}

void ENGINE_MONITORING::prvUpdateCumCrankCnts()
{
    if((_u8StartStopSMState == START_STOP::ID_STATE_SS_CRANKING) && !_bCrankStateLatched)
    {
        _stCummulativeCnt.u32GenNumberOfCranks++;
        _bCrankStateLatched = true;
        StoreCummulativeCnt(CUM_STORE_CRANK_CNTS);
    }
    else if(_u8StartStopSMState != START_STOP::ID_STATE_SS_CRANKING)
    {
        _bCrankStateLatched = false;
    }

    if((_GCUAlarms.IsFailToStart() || (_u8StartStopSMState == START_STOP::ID_STATE_SS_CRANK_REST)) && !_bFailedCrankStateLatched)
    {
        _stCummulativeCnt.u32GenNumberOfFailedCranks++;
        _bFailedCrankStateLatched = true;
        StoreCummulativeCnt(CUM_STORE_CRANK_CNTS);
    }
    else if(_u8StartStopSMState == START_STOP::ID_STATE_SS_CRANKING)
    {
        _bFailedCrankStateLatched = false;
    }
}

ENGINE_MONITORING::LOAD_CONT_STATUS_t ENGINE_MONITORING::getAssumedLoadStatus(CONTACTOR_INFO_t ci)
{
    /*
     * This function returns:
     * LOAD_ON_GEN if load is ASSUMED to be on Genset,
     * LOAD_ON_MAINS if load is ASSUMED to be on Mains,
     * LOAD_NOT_ON_GEN_MAINS otherwise.
     */

    bool bOnMainsWhenNotInManual= (ci.bMainsContactorOutputStatus) && ( ci.bCtOnLoadCable) && (ci.bMainsContactorOutputAssigned) && (BASE_MODES::GetGCUOperatingMode() != BASE_MODES::MANUAL_MODE);
    bool bOnMainsInManualFdbckNotAssigned = (ci.bMainsContactorOutputStatus) && ( ci.bCtOnLoadCable) && (ci.bMainsContactorOutputAssigned) && (!_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring) ;
    bool bOnMainsInManualFdbckAssigned = ( ci.bCtOnLoadCable) && (_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring) && (_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bResultInstant) && (BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE);

    if( (bOnMainsWhenNotInManual) ||(bOnMainsInManualFdbckNotAssigned) || (bOnMainsInManualFdbckAssigned))
    {
        return LOAD_CONT_STATUS_t::LOAD_ON_MAINS;
    }

    //When Gen contactor not configured and only mains contactor configured and in Manual mode. If falsely Mains contactor is used then LOAD is on GEN.
    bool bOnGensetByChanceInManual = (!ci.bGensetContactorOutputAssigned) && (!_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring)
                                     && (!(((ci.bMainsContactorOutputStatus) && (ci.bMainsContactorOutputAssigned) && (!_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring))
                                             || ((_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring) && (_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bResultInstant) && (BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE))));

    bool  bOnGensetWhenNotInManual  = (ci.bGensetContactorOutputStatus) && (ci.bGensetContactorOutputAssigned) && (BASE_MODES::GetGCUOperatingMode() != BASE_MODES::MANUAL_MODE);

    bool   bOnGensetInManualFdbckNotAssigned = (ci.bGensetContactorOutputStatus) && (ci.bGensetContactorOutputAssigned) && (!_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring);

    bool  bOnGensetInManualFdbckAssigned = (_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring) && (_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bResultInstant) && (BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE);

    if ((bOnGensetByChanceInManual) || (bOnGensetWhenNotInManual) || (bOnGensetInManualFdbckNotAssigned) || (bOnGensetInManualFdbckAssigned))
    {
        return LOAD_CONT_STATUS_t::LOAD_ON_GEN;
    }

    return LOAD_CONT_STATUS_t::LOAD_NOT_ON_GEN_MAINS;
}
bool ENGINE_MONITORING::IsModeSelectInputConfigured()
{
    // Not equal to not_configured means it must be assigned!
    return (_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_MODE_SELECT) != DigitalSensor::SENSOR_NOT_CONFIGRUED);
}
bool ENGINE_MONITORING::isGensetContactorFeedbackAssigned()
{
    // Not equal to not_configured means it must be assigned!
    return (_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_GEN_CONTACTOR_LATCHED) != DigitalSensor::SENSOR_NOT_CONFIGRUED);
}
bool ENGINE_MONITORING::isMainsContactorFeedbackAssigned()
{
    // Not equal to not_configured means it must be assigned!
    return (_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_MAINS_CONTACTOR_LATCHED) != DigitalSensor::SENSOR_NOT_CONFIGRUED);
}
bool ENGINE_MONITORING::gensetContactorFeedbackInputStatus()
{
    //returns true if the feedback input indicates that contactor is latched
    return (_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_GEN_CONTACTOR_LATCHED) == DigitalSensor::SENSOR_LATCHED);
}
bool ENGINE_MONITORING::mainsContactorFeedbackInputStatus()
{
    //returns true if the feedback input indicates that contactor is latched
    return (_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_MAINS_CONTACTOR_LATCHED) == DigitalSensor::SENSOR_LATCHED);
}
bool ENGINE_MONITORING::isMainsContactorOutputAssigned()
{
    //not equal to not_configured, means it must be assigned!
    return((_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR)!=ACT_Manager::ACT_NOT_CONFIGURED)
            ||(_hal.actuators.GetActStatus(ACTUATOR::ACT_OPEN_MAINS_OUT)!=ACT_Manager::ACT_NOT_CONFIGURED));

}
bool ENGINE_MONITORING::isGensetContactorOutputAssigned()
{
    return ((_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_GEN_CONTACTOR)!=ACT_Manager::ACT_NOT_CONFIGURED)
            ||(_hal.actuators.GetActStatus(ACTUATOR::ACT_OPEN_GEN_OUT)!=ACT_Manager::ACT_NOT_CONFIGURED));
}

bool ENGINE_MONITORING::haveWeTriedToCloseGensetContactor()
{
    return BASE_MODES::IsGenContactorClosed();
}
bool ENGINE_MONITORING::haveWeTriedToCloseMainsContactor()
{
    return BASE_MODES::IsMainsContactorClosed();
}
void ENGINE_MONITORING::UpdateContactorLoadStatus()
{
    static LOAD_CONT_STATUS_t eLoadStatusPrv=LOAD_NOT_ON_GEN_MAINS;

    static bool eTampStatusPrv = false;
    bool eTampStatusCurrent = eTampStatusPrv;

    eTampStatusCurrent = !IsGenStartValid(); /* stores true if tampered start */
    if(eTampStatusCurrent != eTampStatusPrv)
    {
        ReadEnergySetEnergyOffset(false);
        _hal.AcSensors.ConfigureGenTamp(eTampStatusCurrent);
    }
    eTampStatusPrv = eTampStatusCurrent;
    CONTACTOR_INFO_t ci;
    ci.bCtOnLoadCable = (_cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_LOCATION) == CFGZ::ON_LOAD_CABLE);
    ci.bGensetContactorFeedbackInputStatus = gensetContactorFeedbackInputStatus();
    ci.bGensetContactorFeedbackIsAssigned = isGensetContactorFeedbackAssigned();
    ci.bGensetContactorOutputAssigned = isGensetContactorOutputAssigned();

    ci.bMainsContactorFeedbackInputStatus = mainsContactorFeedbackInputStatus();
    ci.bMainsContactorFeedbackIsAssigned = isMainsContactorFeedbackAssigned();
    ci.bMainsContactorOutputAssigned = isMainsContactorOutputAssigned();

    ci.bGensetContactorOutputStatus = haveWeTriedToCloseGensetContactor();
    ci.bMainsContactorOutputStatus = haveWeTriedToCloseMainsContactor();
    _eLoadStatusCurrent = getAssumedLoadStatus(ci);

    if(_eLoadStatusCurrent != eLoadStatusPrv)
    {
        _bLoadContactorStatusChanged = true;
        ReadEnergySetEnergyOffset(false);
        if( _eLoadStatusCurrent == LOAD_ON_MAINS)
        {
            _hal.AcSensors.ConfigureISensing(MAINS_CURRENT_MEASUREMENT);
        }
        else if(_eLoadStatusCurrent == LOAD_ON_GEN)
        {
            _hal.AcSensors.ConfigureISensing(GENSET_CURRENT_MEASUREMENT);

        }
        else
        {
            _hal.AcSensors.ConfigureISensing(NO_CURRENT_MEASUREMENT);
        }
    }
    eLoadStatusPrv = _eLoadStatusCurrent;
}

ENGINE_MONITORING::LOAD_CONT_STATUS_t ENGINE_MONITORING::GetContactorLoadStatus()
{
    return _eLoadStatusCurrent;
}

bool ENGINE_MONITORING::GetAndClearIsLoadStatusChanged()
{
    if(_bLoadContactorStatusChanged)
    {
        _bLoadContactorStatusChanged = false;
        return true;
    }

    return false;
}



uint16_t ENGINE_MONITORING::prvCheckTimeSlot(uint32_t u32RunTime)
{
    static uint16_t u16TimeSlot = TIME_1st_SLOT_SEC;

    if(u32RunTime < RUN_MIN_1st_SLOT )
    {
        u16TimeSlot = TIME_1st_SLOT_SEC;
    }
    else if((u32RunTime >= RUN_MIN_1st_SLOT ) && (u32RunTime <= RUN_MIN_2nd_SLOT))
    {
        u16TimeSlot = TIME_2nd_SLOT_SEC;
    }
    else if((u32RunTime >= RUN_MIN_2nd_SLOT ) && (u32RunTime <= RUN_MIN_3rd_SLOT))
    {
        u16TimeSlot = TIME_3rd_SLOT_SEC;
    }
    else
    {
        u16TimeSlot = TIME_4th_SLOT_SEC;
    }

    return u16TimeSlot;
}

void ENGINE_MONITORING::prvUpdateEngineOn()
{
    if((_GCUAlarms.GetSpeedValue() > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_ENGINE_SPEED))
        || (_hal.AcSensors.GENSET_GetApproxFreq(R_PHASE) > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_ALT_FREQUENCY)))
    {
        prvUpdateEngineONstatus();
    }
    else
    {
//        UTILS_DisableTimer(&_TimerGenUpdateCumulative);
    }
}

void ENGINE_MONITORING::prvUpdateEngineCranked()
{
    bool bLOPCranked = false;
    bool bLLOPCranked = false;
    bool bChargAltCranked = false;

    bChargAltCranked = prvDisconnectCranckByChanrgingAlt();
    bLOPCranked = prvDisconnectCranckByLOPSensor();
    bLLOPCranked = prvDisconnectCranckByLOPSwitch();

    if (IS_START_STOP_SM_IN_CRANK_STATE() && (!IsEngineCranked()))
    {
        if((_GCUAlarms.GetSpeedValue() > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_ENGINE_SPEED))
            ||(_hal.AcSensors.GENSET_GetApproxFreq(R_PHASE) > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_ALT_FREQUENCY))
            ||(bLOPCranked) ||(bLLOPCranked) || (bChargAltCranked))
        {
            _bEngineCranked = true;
            _u8EngineOff = false;
            UTILS_ResetTimer(&_EngWarmUpTimer);
            prvUpdateEngineONstatus();
        }
        else
        {
            /* do nothing */
        }

    }
    else
    {
        if(_u8EngineOn == 0U)
        {
            _bEngineCranked = false;
            _u8EngineOff = true;
        }
        else
        {
            /* Do nothing */
        }
    }
}

void ENGINE_MONITORING::prvUpdateGenReady()
{
    if(_u8GenReady == 0U)
    {
        if((CHECK_GEN_MIN_HEALTHY_VTG()) && (CHECK_GEN_MIN_HEALTHY_FREQ())
            && (!_GCUAlarms.IsCommonElectricTrip()) && (!_GCUAlarms.IsCommonShutdown())
            && (UTILS_GetElapsedTimeInMs(&_EngWarmUpTimer) >= FIFTY_MSEC))
        {
            _u8GenReady = 1;

            if(IsGenStartValid())
            {
                _stCummulativeCnt.u32GenNumberOfStarts++;
            }
            else
            {
                /* do nothing */
            }

            if(_stCummulativeCnt.u32GenNumberOfStarts > MAX_NO_OF_STARTS)
            {
                _stCummulativeCnt.u32GenNumberOfStarts = 0;
            }
            else
            {
                /* do nothing */
            }
            StoreCummulativeCnt(CUM_STORE_GENERAL);
            UTILS_ResetTimer(&_GenWarmUpTimer);
        }
    }
    else if((_GCUAlarms.IsCommonShutdown()) || (_GCUAlarms.IsCommonElectricTrip()) || (_u8EngineOn == 0U))
    {
        _u8GenReady = 0;
        UTILS_DisableTimer(&_GenWarmUpTimer);
    }
    else
    {
        /* do nothing */
    }

    if(_u8GenReady && (UTILS_GetElapsedTimeInSec(&_GenWarmUpTimer) >= _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_WARM_UP_DELAY)))
    {
        _hal.actuators.Activate(ACTUATOR::ACT_GEN_AVLBL);
        _u8GenAvailable = 1;
    }
    else
    {
        _u8GenAvailable = 0;
        _hal.actuators.Deactivate(ACTUATOR::ACT_GEN_AVLBL);
    }
}

void ENGINE_MONITORING::prvCheckEngineOff()
{
    if(_u8EngineOn == 1U) /* todo: need to think why are we comparing with 1 here, if it is used as flag why dont we make it bool ?*/
    {
        /* Below conditional checks have matched with nxp code to maintain legacy */
        if(    (_hal.AcSensors.GENSET_GetApproxFreq(R_PHASE) <= 0)
            && (_GCUAlarms.GetSpeedValue() <= 0)
            && (_hal.AnalogSensors.GetGensetFreqThruCompartor() <= 0)
            && (   (_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_LUBE_OIL_PRESSURE_SWITCH) ==  DigitalSensor::SENSOR_NOT_CONFIGRUED)
                || (_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_LUBE_OIL_PRESSURE_SWITCH) ==  DigitalSensor::SENSOR_LATCHED)
                || (_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_DISCONN_ON_LLOP_SW) == CFGZ::CFGZ_DISABLE))
            && ((_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_DISCONN_ON_LOP_SENS) == CFGZ::CFGZ_DISABLE)
                || (_stLOP.stValAndStatus.f32InstSensorVal < _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_DISCONN_LOP_SENS))
                || ( (_stLOP.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT) &&
                     (_stLOP.eStatus != A_SENSE::SENSOR_NOT_CONFIGRUED) )
                )
            )
        {
            _u8EngineOn = 0;
            _bEngineCranked = false;
            _u8EngineOff = true;
            UTILS_DisableTimer(&_TimerOneMin);
            if((!_GCUAlarms.IsCommonShutdown()) && START_STOP::IsStopRelayON())
            {
                _GCUAlarms.LogEvent(GCU_ALARMS::Engine_Stop_id, CFGZ::CFGZ_ACTION_NONE_NoWESN);
            }
            else
            {
                /* do nothing */
            }
        }
    }
}


void ENGINE_MONITORING::prvCheckTrips()
{
    if((!_bTripLatched) && (_u8EngineOn == 1) && ((_GCUAlarms.IsCommonShutdown()) || (_GCUAlarms.IsCommonElectricTrip())))
    {
        _stCummulativeCnt.u32GenNumberOfTrips++;
        if(_stCummulativeCnt.u32GenNumberOfTrips > MAX_NO_OF_TRIPS)
        {
            _stCummulativeCnt.u32GenNumberOfTrips = 0;
        }
        else
        {
            /* do nothing */
        }
        StoreCummulativeCnt(CUM_STORE_GENERAL);  /* store cumm data if trip arises */
        _bTripLatched = true;
    }
    else
    {
        /* do nothing */
    }
}

uint8_t ENGINE_MONITORING::IsGenReady()
{
    return _u8GenReady;
}

uint8_t ENGINE_MONITORING::IsEngineOn()
{
    return _u8EngineOn;
}

uint8_t ENGINE_MONITORING::IsEngineOff()
{
    return _u8EngineOff;
}
uint8_t ENGINE_MONITORING::IsGenAvailable()
{
    return _u8GenAvailable;
}

bool ENGINE_MONITORING::IsEngineCranked()
{
    return _bEngineCranked;
}

bool ENGINE_MONITORING::IsWarmUpTimeExpired()
{
    return (UTILS_GetElapsedTimeInSec(&_GenWarmUpTimer) >= _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_WARM_UP_DELAY));
}

void ENGINE_MONITORING::prvUpdateCumulativeEnergyCounts()
{

    _stCummulativeCnt.f32GenKWH   = (float)(_hal.AcSensors.GENSET_GetTotalActiveEnergySinceInitWH()/KW_TO_WATT_CONVERSION);
    _stCummulativeCnt.f32GenKVAH  = (float)(_hal.AcSensors.GENSET_GetTotalApparentEnergySinceInitVAH()/KW_TO_WATT_CONVERSION);
    _stCummulativeCnt.f32GenKVARH = (float)(_hal.AcSensors.GENSET_GetTotalReactiveEnergySinceInitVARH()/KW_TO_WATT_CONVERSION);

   _stCummulativeCnt.f32MainsKWH   = (float)(_hal.AcSensors.MAINS_GetTotalActiveEnergySinceInitWH()/KW_TO_WATT_CONVERSION);
   _stCummulativeCnt.f32MainsKVAH  = (float)(_hal.AcSensors.MAINS_GetTotalApparentEnergySinceInitVAH()/KW_TO_WATT_CONVERSION);
   _stCummulativeCnt.f32MainsKVARH = (float)(_hal.AcSensors.MAINS_GetTotalReactiveEnergySinceInitVARH()/KW_TO_WATT_CONVERSION);
}

void ENGINE_MONITORING::prvUpdateCumulativeTamperedEnergyCounts()
{
    _stCummulativeCnt.f32TamprGenKWH  = (float)(_hal.AcSensors.GENSET_GetTotalTamperedActiveEnergySinceInitWH()/KW_TO_WATT_CONVERSION);
    _stCummulativeCnt.f32TamprGenKVAH  = (float)(_hal.AcSensors.GENSET_GetTotalTamperedApparentEnergySinceInitVAH()/KW_TO_WATT_CONVERSION);
    _stCummulativeCnt.f32TamprGenKVARH  = (float)(_hal.AcSensors.GENSET_GetTotalTamperedReactiveEnergySinceInitVARH()/KW_TO_WATT_CONVERSION);
}


void ENGINE_MONITORING::StoreCummulativeCnt(ENGINE_MONITORING::CUM_STORE_t eType)
{
    static CUMULATIVE_t stStoredCummulative0,stStoredCummulative1;
    CUMULATIVE_t stCummulativeCntToStore;

    if(eType == CUM_STORE_GENERAL)
    {
#if (TEST_AUTOMATION == YES)
        if(_bFromAutomation)
        {
            _bFromAutomation = false;
        }
        else
#endif
        {
            prvUpdateCumulativeEnergyCounts();
            prvUpdateCumulativeTamperedEnergyCounts();
        }
        stCummulativeCntToStore = _stCummulativeCnt;
    }
    else if(eType == CUM_STORE_CRANK_CNTS)
    {
        stCummulativeCntToStore = _stStoredCummulativeCnt;
        stCummulativeCntToStore.u32GenNumberOfCranks = _stCummulativeCnt.u32GenNumberOfCranks;
        stCummulativeCntToStore.u32GenNumberOfFailedCranks = _stCummulativeCnt.u32GenNumberOfFailedCranks;
    }
    else if(eType == CUM_STORE_OVLD_EXT_RUN_HRS)
    {
        stCummulativeCntToStore = _stStoredCummulativeCnt;
        stCummulativeCntToStore.ExtOvldStartTime = _stCummulativeCnt.ExtOvldStartTime;
        stCummulativeCntToStore.u32GenExtOverloadRunTime_min = _stCummulativeCnt.u32GenExtOverloadRunTime_min;
        stCummulativeCntToStore.u8ExtOvldStarted = _stCummulativeCnt.u8ExtOvldStarted;
        stCummulativeCntToStore.u8ExtOvldFault = _stCummulativeCnt.u8ExtOvldFault;
    }

    _stCummulativeCnt.u64Header++;
    stCummulativeCntToStore.u64Header++;
    stCummulativeCntToStore.u32CRC =(uint16_t) CRC16::ComputeCRCGeneric((uint8_t *)&stCummulativeCntToStore,
                                              sizeof(CUMULATIVE_t) - sizeof(uint32_t)
                                              , CRC_MEMORY_SEED);
    _stStoredCummulativeCnt = stCummulativeCntToStore;
    if(_u8ActiveSectorForCummulative == 0)
    {
        stStoredCummulative0 = stCummulativeCntToStore;
       _hal.Objeeprom.RequestWrite(EXT_EEPROM_CUMMULATIVE_START_ADD_SECT0, (uint8_t*)&stStoredCummulative0,
                                   sizeof(CUMULATIVE_t), NULL);
       _u8ActiveSectorForCummulative = 1;
    }
    else
    {
        stStoredCummulative1 = stCummulativeCntToStore;
       _hal.Objeeprom.RequestWrite(EXT_EEPROM_CUMMULATIVE_START_ADD_SECT1, (uint8_t*)&stStoredCummulative1,
                                                          sizeof(CUMULATIVE_t), NULL);
       _u8ActiveSectorForCummulative =0;
    }
}


uint32_t ENGINE_MONITORING::GetEngineRunTimeMin()
{
    return _stCummulativeCnt.u32EngineRunTime_min;
}

uint32_t ENGINE_MONITORING::GetTamperedRunTimeMin()
{
    return  _stCummulativeCnt.u32TamperedRunTime_min;
}

uint32_t ENGINE_MONITORING::GetRemoteRunTimeMin()
{
    return  _stCummulativeCnt.u32GenRemoteRunTime_min;
}

uint32_t ENGINE_MONITORING::GetManualRunTimeMin()
{
    return  _stCummulativeCnt.u32GenManualRunTime_min;
}

uint32_t ENGINE_MONITORING::GetNoLoadRunTimeMin()
{
    return  _stCummulativeCnt.u32GenNoLoadRunTime_min;
}

uint32_t ENGINE_MONITORING::GetOnLoadRunTimeMin()
{
    return  _stCummulativeCnt.u32GenOnLoadRunTime_min;
}

uint32_t ENGINE_MONITORING::GetCumCrankCnt()
{
    return  _stCummulativeCnt.u32GenNumberOfCranks;
}

uint32_t ENGINE_MONITORING::GetCumFailedCrankCnt()
{
    return  _stCummulativeCnt.u32GenNumberOfFailedCranks;
}

float ENGINE_MONITORING::GetTamprEEPromCummEnergy()
{
    return _stCummulativeCnt.f32TamprGenKWH;
}
//float ENGINE_MONITORING::GetGenEEPromCummKWH()
//{
//    return _stCummulativeCnt.f32GenKWH;
//}
//
//
//float ENGINE_MONITORING:: GetGenEEPromCummKVAH()
//{
//    return _stCummulativeCnt.f32GenKVAH;
//}
//
//float ENGINE_MONITORING:: GetGenEEPromCummKVARH()
//{
//    return  _stCummulativeCnt.f32GenKVARH;
//}

uint32_t ENGINE_MONITORING::GetMainsRunTimeMin()
{
    return _stCummulativeCnt.u32MainsRunTime_min;
}
uint32_t ENGINE_MONITORING::GetBTSRunTimeMin()
{
    return _stCummulativeCnt.u32BTSRunTime_min;
}
uint32_t ENGINE_MONITORING::GetEngineNoOfStarts()
{
    return _stCummulativeCnt.u32GenNumberOfStarts;
}

uint32_t ENGINE_MONITORING::GetEngineNoOfTrips()
{
    return _stCummulativeCnt.u32GenNumberOfTrips;
}

void ENGINE_MONITORING::DisableEngWarmUpTimer()
{
    UTILS_DisableTimer(&_EngWarmUpTimer);
}

void ENGINE_MONITORING::DisableGenWarmUpTimer()
{
    UTILS_DisableTimer(&_GenWarmUpTimer);
}

bool ENGINE_MONITORING::IsGenWarmUpEnabledAndNotExpired()
{
    return ((_GenWarmUpTimer.bEnabled) && (UTILS_GetElapsedTimeInSec(&_GenWarmUpTimer) < _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_WARM_UP_DELAY)));
}

void ENGINE_MONITORING::UpdateStartStopState(uint8_t u8StartStopState)
{
    _u8StartStopSMState = u8StartStopState;
}


void ENGINE_MONITORING::ClearTriplatched()
{
    _bTripLatched = false;
}

void ENGINE_MONITORING::ReadEnergySetEnergyOffset(bool bFromEeprom)
{
    if(bFromEeprom)
    {
        _stTampEnergyRegister.f32InitialActiveEnergyWH = (_stCummulativeCnt.f32TamprGenKWH * KW_TO_WATT_CONVERSION);
        _stTampEnergyRegister.f32InitialApparentEnergyVA = (_stCummulativeCnt.f32TamprGenKVAH * KW_TO_WATT_CONVERSION);
        _stTampEnergyRegister.f32InitialReactiveEnergyVAR  = (_stCummulativeCnt.f32TamprGenKVARH*KW_TO_WATT_CONVERSION);

        _stEnergyRegister.f32InitialActiveEnergyWH = (_stCummulativeCnt.f32GenKWH * KW_TO_WATT_CONVERSION);
        _stEnergyRegister.f32InitialApparentEnergyVA = (_stCummulativeCnt.f32GenKVAH * KW_TO_WATT_CONVERSION);
        _stEnergyRegister.f32InitialReactiveEnergyVAR  = (_stCummulativeCnt.f32GenKVARH*KW_TO_WATT_CONVERSION);

        _stMainsEnergyRegister.f32InitialActiveEnergyWH = (_stCummulativeCnt.f32MainsKWH * KW_TO_WATT_CONVERSION);
        _stMainsEnergyRegister.f32InitialApparentEnergyVA = (_stCummulativeCnt.f32MainsKVAH * KW_TO_WATT_CONVERSION);
        _stMainsEnergyRegister.f32InitialReactiveEnergyVAR  = (_stCummulativeCnt.f32MainsKVARH*KW_TO_WATT_CONVERSION);
    }
    else
    {
        _stTampEnergyRegister.f32InitialActiveEnergyWH = (float)(_hal.AcSensors.GENSET_GetTotalTamperedActiveEnergySinceInitWH());
        _stTampEnergyRegister.f32InitialApparentEnergyVA = (float)(_hal.AcSensors.GENSET_GetTotalTamperedApparentEnergySinceInitVAH());
        _stTampEnergyRegister.f32InitialReactiveEnergyVAR  = (float)(_hal.AcSensors.GENSET_GetTotalTamperedReactiveEnergySinceInitVARH());

        _stEnergyRegister.f32InitialActiveEnergyWH = (float)(_hal.AcSensors.GENSET_GetTotalActiveEnergySinceInitWH());
        _stEnergyRegister.f32InitialApparentEnergyVA = (float)(_hal.AcSensors.GENSET_GetTotalApparentEnergySinceInitVAH());
        _stEnergyRegister.f32InitialReactiveEnergyVAR  = (float)(_hal.AcSensors.GENSET_GetTotalReactiveEnergySinceInitVARH());

        _stMainsEnergyRegister.f32InitialActiveEnergyWH = (float)( _hal.AcSensors.MAINS_GetTotalActiveEnergySinceInitWH());
        _stMainsEnergyRegister.f32InitialApparentEnergyVA = (float)(_hal.AcSensors.MAINS_GetTotalApparentEnergySinceInitVAH());
        _stMainsEnergyRegister.f32InitialReactiveEnergyVAR = (float)(_hal.AcSensors.MAINS_GetTotalReactiveEnergySinceInitVARH());
    }
        _hal.AcSensors.SetEnergyOffsets(_stTampEnergyRegister,_stEnergyRegister, _stMainsEnergyRegister);
}

float ENGINE_MONITORING::GetFilteredEngSpeed()
{
     if(_cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_SPEED_SENSE_SOURCE) == CFGZ::CFGZ_ALT_FREQUENCY)
    {
        return _hal.AnalogSensors.GetFiltRPMThruCompartor();
    }
    else
    {
        return 0.0;
    }
}

void ENGINE_MONITORING:: prvGetCumulativeCnt()
{
    /**
     * To store cumulative counts External EEprom is used,
     * Two sectors of size 256 bytes used.
     * Refer CUMULATIVE_t for storage structure.
     * 1. Read External eeprom's cumulative cnt sector0 and sector1.
     * 2. If both sector's CRC is good then compare the header cnt.
     * 3. Read the cumulative counts from good sector with larger header number.
     * 4. Set the active block number.
     * 5. If both sectors are bad then reset the counts, and set the active block number to 0.
     */

    CUMULATIVE_t stCummulative0,stCummulative1;

    _hal.Objeeprom.BlockingRead(EXT_EEPROM_CUMMULATIVE_START_ADD_SECT0, (uint8_t*)&stCummulative0, sizeof( CUMULATIVE_t));

    uint16_t u16CRC0= CRC16::ComputeCRCGeneric((uint8_t *)&stCummulative0, sizeof(CUMULATIVE_t) -sizeof(uint32_t)
                                              , CRC_MEMORY_SEED);

    _hal.Objeeprom.BlockingRead(EXT_EEPROM_CUMMULATIVE_START_ADD_SECT1, (uint8_t*)&stCummulative1, sizeof( CUMULATIVE_t));

    uint16_t u16CRC1= CRC16::ComputeCRCGeneric((uint8_t *)&stCummulative1, sizeof(CUMULATIVE_t) -sizeof(uint32_t)
                                                  , CRC_MEMORY_SEED);

    if((u16CRC0 == (uint16_t)stCummulative0.u32CRC) && (u16CRC1 == (uint16_t)stCummulative1.u32CRC))
    {
        if(stCummulative0.u64Header > stCummulative1.u64Header)
        {
            _hal.Objeeprom.BlockingRead(EXT_EEPROM_CUMMULATIVE_START_ADD_SECT0,
                                        (uint8_t*)&_stCummulativeCnt, sizeof( CUMULATIVE_t));
            _u8ActiveSectorForCummulative = 1;
        }
        else
        {
            _hal.Objeeprom.BlockingRead(EXT_EEPROM_CUMMULATIVE_START_ADD_SECT1,
                                        (uint8_t*)&_stCummulativeCnt, sizeof( CUMULATIVE_t));
            _u8ActiveSectorForCummulative = 0;
        }
    }
    else if(u16CRC0 == (uint16_t)stCummulative0.u32CRC)
    {
        _hal.Objeeprom.BlockingRead(EXT_EEPROM_CUMMULATIVE_START_ADD_SECT0,
                                                (uint8_t*)&_stCummulativeCnt, sizeof( CUMULATIVE_t));
        _u8ActiveSectorForCummulative = 1;
    }
    else if(u16CRC1 == (uint16_t)stCummulative1.u32CRC)
    {
        _hal.Objeeprom.BlockingRead(EXT_EEPROM_CUMMULATIVE_START_ADD_SECT1,
                                                (uint8_t*)&_stCummulativeCnt, sizeof( CUMULATIVE_t));
        _u8ActiveSectorForCummulative = 0;
    }
    else
    {
        _stCummulativeCnt.u64Header =0;
        _stCummulativeCnt.u32EngineRunTime_min =0;
        _stCummulativeCnt.u32TamperedRunTime_min =0;
        _stCummulativeCnt.u32MainsRunTime_min =0;
        _stCummulativeCnt.u32BTSRunTime_min =0;
        _stCummulativeCnt.u32GenRemoteRunTime_min =0;
        _stCummulativeCnt.u32GenManualRunTime_min =0;
        _stCummulativeCnt.u32GenNoLoadRunTime_min =0;
        _stCummulativeCnt.u32GenOnLoadRunTime_min =0;
        _stCummulativeCnt.u32GenExtOverloadRunTime_min =0;
        memset((void*)(&_stCummulativeCnt.ExtOvldStartTime), 0, sizeof(RTC::TIME_t));
        _stCummulativeCnt.u8ExtOvldStarted =0;
        _stCummulativeCnt.u8ExtOvldFault =0;


        _stCummulativeCnt.f32GenKWH =0.0;
        _stCummulativeCnt.f32GenKVAH =0.0;
        _stCummulativeCnt.f32GenKVARH =0.0;

        _stCummulativeCnt.f32MainsKWH =0.0;
        _stCummulativeCnt.f32MainsKVAH =0.0;
        _stCummulativeCnt.f32MainsKVARH =0.0;

        _stCummulativeCnt.f32TamprGenKWH = 0.0;
        _stCummulativeCnt.f32TamprGenKVAH = 0.0;
        _stCummulativeCnt.f32TamprGenKVARH = 0.0;
        _stCummulativeCnt.u32GenNumberOfCranks =0;
        _stCummulativeCnt.u32GenNumberOfFailedCranks =0;

        _u8ActiveSectorForCummulative =0;
    }
    _stStoredCummulativeCnt = _stCummulativeCnt;
    _GCUAlarms.SetExtOverloadFault((bool)(_stCummulativeCnt.u8ExtOvldFault != 0));
}

void ENGINE_MONITORING::prvUpdateEngineONstatus(void)
{
    if(_u8EngineOn == 0U)
    {
        _u8EngineOn = 1;
        UTILS_ResetTimer(&_TimerOneMin);
        UTILS_ResetTimer(&_TimerGenUpdateCumulative);
        UTILS_ResetTimer(&_TimerUpdateTamperedCumulative);
    }
    else
    {
        /* do nothing */
    }
}


void ENGINE_MONITORING::prvUpdateEngineRunHrs()
{
    static uint16_t u16TimeSlot = 1U; /* todo: initialization , need to think */
    if(UTILS_GetElapsedTimeInSec(&_TimerOneMin) >= ONE_MIN_CNT)
    {
        UTILS_ResetTimer(&_TimerOneMin);

        if((_u8EngineOn == 1U) && (IsGenStartValid()) &&
            (_GCUAlarms.GetSpeedValue() > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_ENGINE_SPEED)))
        {
            _stCummulativeCnt.u32EngineRunTime_min++;
            if(BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)
            {
                _stCummulativeCnt.u32GenManualRunTime_min++;
            }
            else
            {
                _stCummulativeCnt.u32GenRemoteRunTime_min++;
            }

            if(GetContactorLoadStatus() == LOAD_ON_GEN)
            {
                _stCummulativeCnt.u32GenOnLoadRunTime_min++;
            }
            else
            {
                _stCummulativeCnt.u32GenNoLoadRunTime_min++;
            }

            prvUpdateExtOvldRunHrs();

            u16TimeSlot = prvCheckTimeSlot(_stCummulativeCnt.u32EngineRunTime_min);
            if(UTILS_GetElapsedTimeInSec(&_TimerGenUpdateCumulative) >= u16TimeSlot)
            {
                StoreCummulativeCnt(CUM_STORE_GENERAL);
                UTILS_ResetTimer(&_TimerGenUpdateCumulative);
            }
        }
        else if((_u8EngineOn == 1U) && (!IsGenStartValid()) &&
                (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_WAVE_DETECTION) == CFGZ::CFGZ_ENABLE))
        {
            _stCummulativeCnt.u32TamperedRunTime_min++;
            u16TimeSlot = prvCheckTimeSlot(_stCummulativeCnt.u32TamperedRunTime_min);
            if(UTILS_GetElapsedTimeInSec(&_TimerUpdateTamperedCumulative) >= u16TimeSlot)
            {
                StoreCummulativeCnt(CUM_STORE_GENERAL);
                UTILS_ResetTimer(&_TimerUpdateTamperedCumulative);
            }
        }
        else
        {
            /* do nothing */
        }
    }
    else
    {
        /* do nothing */
    }
}

void ENGINE_MONITORING::prvUpdateExtOvldRunHrs()
{
    static uint8_t u8StoreCnt = 0U;
    /*NOTE: This function is only to be called inside prvUpdateEngineRunHrs() in the 1 min timeout*/
    if(_GCUAlarms.IsExtendedOverLoad())
    {
        if(!(_stCummulativeCnt.u8ExtOvldStarted != 0))
        {
            /*extended overload condition is met for the first time. Set the flag and storenthe current time*/
            _stCummulativeCnt.u8ExtOvldStarted = 1;
            _hal.ObjRTC.GetTime( &(_stCummulativeCnt.ExtOvldStartTime) );
            _stCummulativeCnt.u32GenExtOverloadRunTime_min = 0U;
        }
        _stCummulativeCnt.u32GenExtOverloadRunTime_min++;
    }

    if((_stCummulativeCnt.u32GenExtOverloadRunTime_min >= 60) && (_stCummulativeCnt.u8ExtOvldStarted != 0))
    {
        /*The overload run time has passed 1 hour. Set extended overload alarm*/
        _stCummulativeCnt.u32GenExtOverloadRunTime_min = 0U;
        _stCummulativeCnt.u8ExtOvldFault = 1;
        _GCUAlarms.SetExtOverloadFault((bool)(_stCummulativeCnt.u8ExtOvldFault != 0));
        StoreCummulativeCnt(CUM_STORE_OVLD_EXT_RUN_HRS);
    }

    if(IsDiffTwelveHr() && (_stCummulativeCnt.u8ExtOvldStarted != 0))
    {
        /*Reset the cycle cnt*/
        _stCummulativeCnt.u32GenExtOverloadRunTime_min = 0U;
        _stCummulativeCnt.u8ExtOvldStarted = 0;
        _stCummulativeCnt.u8ExtOvldFault = 0;
        _GCUAlarms.SetExtOverloadFault((_stCummulativeCnt.u8ExtOvldFault != 0));
        StoreCummulativeCnt(CUM_STORE_OVLD_EXT_RUN_HRS);
    }

    if(_stCummulativeCnt.u8ExtOvldStarted != 0)
    {
        u8StoreCnt++;
        if(u8StoreCnt >= 2)
        {
            StoreCummulativeCnt(CUM_STORE_OVLD_EXT_RUN_HRS);
            u8StoreCnt = 0;
        }
    }
}

bool ENGINE_MONITORING::IsDiffTwelveHr()
{
    uint8_t u8MonthDayCnt[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool bRet = true;
    RTC::TIME_t CurrTime;
    _hal.ObjRTC.GetTime(&CurrTime);

    #define STORED_YEAR  (_stCummulativeCnt.ExtOvldStartTime.u16Year)
    #define STORED_MONTH (_stCummulativeCnt.ExtOvldStartTime.u8Month)
    #define STORED_DAY   (_stCummulativeCnt.ExtOvldStartTime.u8Day)
    #define STORED_HOUR  (_stCummulativeCnt.ExtOvldStartTime.u8Hour)
    #define STORED_MIN   (_stCummulativeCnt.ExtOvldStartTime.u8Minute)

    #define CURR_YEAR  (CurrTime.u16Year)
    #define CURR_MONTH (CurrTime.u8Month)
    #define CURR_DAY   (CurrTime.u8Day)
    #define CURR_HOUR  (CurrTime.u8Hour)
    #define CURR_MIN   (CurrTime.u8Minute)

    if((CURR_YEAR > STORED_YEAR) && ((CURR_YEAR - STORED_YEAR) >= 2))
    {
        bRet = true;
    }
    else if((CURR_YEAR == (STORED_YEAR + 1)))
    {
        uint32_t u32StoredYearMinCnt = 0;
        if( (((STORED_YEAR)%4 == 0) && ((STORED_YEAR)%100 != 0)) || ((STORED_YEAR)%400 == 0) ) /*Check for leap years*/
        {
            u8MonthDayCnt[1] = 29;
            u32StoredYearMinCnt = 366*24*60;
        }
        uint16_t u16DayCnt = 0;
        for(uint8_t u8Idx = 0; u8Idx <= ((STORED_MONTH) - 1); u8Idx++ )
        {
            if(u8Idx == ((STORED_MONTH) - 1))
            {
                break;
            }
            u16DayCnt = u16DayCnt + u8MonthDayCnt[u8Idx];
        }
        u16DayCnt = u16DayCnt + STORED_DAY;
        u32StoredYearMinCnt = u32StoredYearMinCnt - (u16DayCnt*24*60 + STORED_HOUR + STORED_MIN);

        u8MonthDayCnt[1] = 29;
        uint32_t u32CurrYearMinCnt = 0;
        if( (((CURR_YEAR)%4 == 0) && ((CURR_YEAR)%100 != 0)) || ((CURR_YEAR)%400 == 0) ) /*Check for leap years*/
        {
            u8MonthDayCnt[1] = 29;
        }
        u16DayCnt = 0;
        for(uint8_t u8Idx = 0; u8Idx <= ((CURR_MONTH) - 1); u8Idx++ )
        {
            if(u8Idx == ((CURR_MONTH) - 1))
            {
                break;
            }
            u16DayCnt = u16DayCnt + u8MonthDayCnt[u8Idx];
        }
        u16DayCnt = u16DayCnt + CURR_DAY;
        u32CurrYearMinCnt = (u16DayCnt*24*60 + CURR_HOUR + CURR_MIN);

        if((u32CurrYearMinCnt + u32StoredYearMinCnt) >= 720) /*Difference is greater than 12 hrs*/
        {
            bRet = true;
        }
        else
        {
            bRet = false;
        }
        
    }
    else if(CURR_YEAR == (STORED_YEAR))
    {
        uint32_t u32StoredYearMinCnt = 0;
        if( (((STORED_YEAR)%4 == 0) && ((STORED_YEAR)%100 != 0)) || ((STORED_YEAR)%400 == 0) ) /*Check for leap years*/
        {
            u8MonthDayCnt[1] = 29;
        }
        uint16_t u16DayCnt = 0;
        for(uint8_t u8Idx = 0; u8Idx <= ((STORED_MONTH) - 1); u8Idx++ )
        {
            if(u8Idx == ((STORED_MONTH) - 1))
            {
                break;
            }
            u16DayCnt = u16DayCnt + u8MonthDayCnt[u8Idx];
        }
        u16DayCnt = u16DayCnt + STORED_DAY;
        u32StoredYearMinCnt = (u16DayCnt*24*60 + STORED_HOUR + STORED_MIN);

        u16DayCnt = 0;
        uint32_t u32CurrYearMinCnt = 0;
        for(uint8_t u8Idx = 0; u8Idx <= ((CURR_MONTH) - 1); u8Idx++ )
        {
            if(u8Idx == ((CURR_MONTH) - 1))
            {
                break;
            }
            u16DayCnt = u16DayCnt + u8MonthDayCnt[u8Idx];
        }
        u16DayCnt = u16DayCnt + CURR_DAY;
        u32CurrYearMinCnt = (u16DayCnt*24*60 + CURR_HOUR + CURR_MIN);

        if(u32CurrYearMinCnt > u32StoredYearMinCnt)
        {
            if((u32CurrYearMinCnt - u32StoredYearMinCnt) >= 720) /*Difference is greater than 12 hrs*/
            {
                bRet = true;
            }
            else
            {
                bRet = false;
            }
        }
        else
        {
            /*Curr time is less than stored time. \
            As there is some mismatch, condition is assumed to be met to avoid further issues*/
            bRet = true;
        }
    }

    return bRet;

}


void ENGINE_MONITORING::prvUpdateMainsRunHrs()
{
    static uint16_t u16TimeSlot = 1U;

    if((_cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_LOCATION) == CFGZ::ON_LOAD_CABLE)
            &&(((BASE_MODES::IsMainsContactorClosed())&&(isMainsContactorOutputAssigned()) && (BASE_MODES::GetGCUOperatingMode() != BASE_MODES::MANUAL_MODE))
                    || ((BASE_MODES::IsMainsContactorClosed()) && (isMainsContactorOutputAssigned()) && (!_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring))
                    || ((_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring) && (_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bResultInstant) && (BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE) && (BASE_MODES::GetMainsStatus() == BASE_MODES::MAINS_HELATHY))
            )
    )
    {
        if(!UTILS_IsTimerEnabled(&_MainsRunTimeBaseTimer))
        {
            UTILS_ResetTimer(&_MainsRunTimeBaseTimer);
            UTILS_ResetTimer(&_TimerMainsUpdateCumulative);
        }

        if(UTILS_GetElapsedTimeInSec(&_MainsRunTimeBaseTimer) >= ONE_MIN_CNT)
        {
            _stCummulativeCnt.u32MainsRunTime_min++;
            u16TimeSlot = prvCheckTimeSlot(_stCummulativeCnt.u32MainsRunTime_min);
            if(UTILS_GetElapsedTimeInSec(&_TimerMainsUpdateCumulative) >= u16TimeSlot)
            {
               StoreCummulativeCnt(CUM_STORE_GENERAL);
               UTILS_ResetTimer(&_TimerMainsUpdateCumulative);
            }
            UTILS_ResetTimer(&_MainsRunTimeBaseTimer);
        }
    }
    else
    {
         UTILS_DisableTimer(&_MainsRunTimeBaseTimer);
    }
}

void ENGINE_MONITORING::prvUpdateBTSRunHrs()
{
    static uint16_t u16TimeSlot = 1U;

    if((_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON) == CFGZ::CFGZ_ENABLE)
            &&(((BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE) && (!IS_LOAD_ON_MAINS()) && (!IS_LOAD_ON_GEN()) && (_GCUAlarms.IsBTSBattHealthy()))
                    || ((BASE_MODES::GetGCUOperatingMode() != BASE_MODES::MANUAL_MODE)
                            && (!BASE_MODES::IsGenContactorClosed())
                            && (!BASE_MODES::IsMainsContactorClosed()))))
    {
        if(_BTSRunTimeBaseTimer.bEnabled == false)
        {
            UTILS_ResetTimer(&_BTSRunTimeBaseTimer);
            UTILS_ResetTimer(&_TimerBTSUpdateCumulative);
        }
        if(UTILS_GetElapsedTimeInSec(&_BTSRunTimeBaseTimer)>= ONE_MIN_CNT)
        {
            _stCummulativeCnt.u32BTSRunTime_min++;
            u16TimeSlot = prvCheckTimeSlot(_stCummulativeCnt.u32BTSRunTime_min);
            if(UTILS_GetElapsedTimeInSec(&_TimerBTSUpdateCumulative) >= u16TimeSlot)
            {
               StoreCummulativeCnt(CUM_STORE_GENERAL);
               UTILS_ResetTimer(&_TimerBTSUpdateCumulative);
            }
            UTILS_ResetTimer(&_BTSRunTimeBaseTimer);
        }
    }
    else
    {
         UTILS_DisableTimer(&_BTSRunTimeBaseTimer);
    }
}


bool ENGINE_MONITORING::prvDisconnectCranckByChanrgingAlt()
{
    /* prvUpdateEngineCranked() function is called every 50ms hence the _u8ChargAltMonCount
     * corresponds to 2 seconds will be 2000ms/50ms = 40*/
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_DISCONN_ON_CHG_ALT_VOLT) == CFGZ::CFGZ_ENABLE)
        && (_hal.AnalogSensors.GetFilteredChargingAltVolts() > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_CHG_ALT_THRESHOLD))
        && (START_STOP::IsMonitorChargAltTrue()))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool ENGINE_MONITORING::prvDisconnectCranckByLOPSensor()
{
    /* todo: have confusion about sensors */
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_DISCONN_ON_LOP_SENS)  == CFGZ::CFGZ_ENABLE) &&
        IsLOPSensorConfigured() && (_stLOP.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT) &&
        (_stLOP.stValAndStatus.f32InstSensorVal > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_DISCONN_LOP_SENS)))
    {
        /* wait for 1 sec to make a decision */
        return (UTILS_GetElapsedTimeInSec(&_LOPSensMonTimer) >= 1U);
    }
    else
    {
       UTILS_ResetTimer(&_LOPSensMonTimer);
       return false;
    }
}

bool ENGINE_MONITORING::prvDisconnectCranckByLOPSwitch()
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_DISCONN_ON_LLOP_SW) == CFGZ::CFGZ_ENABLE)
    {
        if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_LUBE_OIL_PRESSURE_SWITCH) != DigitalSensor::SENSOR_NOT_CONFIGRUED)
        {
            if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_LUBE_OIL_PRESSURE_SWITCH) == DigitalSensor::SENSOR_LATCHED)
            {
                /* todo: test LLOP functionality for crank disconnect specifically  */
                UTILS_DisableTimer(&_LLOPCrankingTimer);
            }
            else
            {
                if(!UTILS_IsTimerEnabled(&_LLOPCrankingTimer))
                {
                    UTILS_ResetTimer(&_LLOPCrankingTimer);
                }
                else
                {
                    /* do nothing */
                }
            }

            if(UTILS_GetElapsedTimeInMs(&_LLOPCrankingTimer) >= (uint64_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_LLOP_SW_TRANS_TIME)*1000))
            {
                UTILS_DisableTimer(&_LLOPCrankingTimer);
                return  true;
            }
            else
            {
                return false;
            }
        }
    }
        return  false;
}

bool ENGINE_MONITORING::IsGenStartValid()
{
    /* function returns true if valid start found */
    /* (invalidity enabled & valid start found & wave detection enabled) || (wave detection disabled) */
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_WAVE_DETECTION) == CFGZ::CFGZ_ENABLE)
    {
        return ((true == ENGINE_START_VALIDITY::IsEngineStartValidityDetectionEnabled())
                && (true == ENGINE_START_VALIDITY::IsValidEngineStartFound()));
    }
    else
    {
        return true;
    }
}

bool ENGINE_MONITORING::IsLOPSensorConfigured()
{
    return ((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
        ||((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) >= CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
        ||(_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG) == CFGZ::CFGZ_ENABLE));
}


#if (TEST_AUTOMATION == YES)
void ENGINE_MONITORING::SetEngineRunTime(uint32_t u32EngineRunTimeInmin)
{
    _stCummulativeCnt.u32EngineRunTime_min=u32EngineRunTimeInmin;
}

void ENGINE_MONITORING::SetMainsRunTime(uint32_t u32MainsRunTimeInmin)
{
    _stCummulativeCnt.u32MainsRunTime_min = u32MainsRunTimeInmin;
}

void ENGINE_MONITORING::SetBTSRunTime(uint32_t u32BTSRunTimeInmin)
{
    _stCummulativeCnt.u32BTSRunTime_min = u32BTSRunTimeInmin;
}

void ENGINE_MONITORING::SetGenActiveEnergy(uint32_t u32GenActiveEnergy)
{
    _stCummulativeCnt.f32GenKWH=(float)u32GenActiveEnergy;
    _bFromAutomation = true;
}

void ENGINE_MONITORING::SetGenApparentEnergy(uint32_t u32GenApparentEnergy)
{
    _stCummulativeCnt.f32GenKVAH=(float)u32GenApparentEnergy;
    _bFromAutomation = true;
}

void ENGINE_MONITORING::SetGenReactiveEnergy(uint32_t u32GenReactiveEnergy)
{
    _stCummulativeCnt.f32GenKVARH=(float)u32GenReactiveEnergy;
    _bFromAutomation = true;
}

void ENGINE_MONITORING::SetGenNumberOfStarts(uint32_t u32NumberOfStarts)
{
    _stCummulativeCnt.u32GenNumberOfStarts=u32NumberOfStarts;
}

void ENGINE_MONITORING::SetGenNumberOfTrips(uint32_t u32NumberOfTrips)
{
    _stCummulativeCnt.u32GenNumberOfTrips=u32NumberOfTrips;
}
#endif
