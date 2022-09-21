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
#include "../EGOV/EGOV.h"
#include "CFGC/CFGC.h"
#include "J1939_APP/J1939APP.h"
#include "../ENGINE_START_VALIDITY/ENGINE_START_VALIDITY.h"

/* supporting private macros */
#define IS_START_STOP_SM_IN_CRANK_STATE()       (  (_u8StartStopSMState == START_STOP::ID_STATE_SS_CRANKING)   \
                                               || (_u8StartStopSMState == START_STOP::ID_STATE_SS_CRANK_REST))

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
ENGINE_MONITORING::CUMULATIVE_t ENGINE_MONITORING::_stCummulativeCnt={0};

bool ENGINE_MONITORING::_bEngineCranked = false;
extern J1939APP *gpJ1939;
ENGINE_MONITORING::ENGINE_MONITORING(CFGZ &cfgz, GCU_ALARMS &GCUAlarms, HAL_Manager &hal):
_cfgz(cfgz),
_GCUAlarms(GCUAlarms),
_hal(hal),
_u8StartStopSMState(0),
_LLOPCrankingTimer{0},
_Timer50MS{0},
_TimerOneMin{0},
_MainsRunTimeBaseTimer{0},
_TimerGenUpdateCumulative{0},
_TimerMainsUpdateCumulative{0},
_TimerBTSUpdateCumulative{0},
_TimerUpdateTamperedCumulative{0},
_BTSRunTimeBaseTimer{0},
_EngWarmUpTimer{0},
_GenWarmUpTimer{0},
_stTampEnergyRegister{0},
_stEnergyRegister{0},
_stMainsEnergyRegister{0},
_u8ActiveSectorForCummulative(0)
{
    UTILS_ResetTimer(&_Timer50MS);
    prvGetCumulativeCnt();
    ReadEnergySetEnergyOffset(true);
}

void ENGINE_MONITORING::Update(bool bDeviceInConfigMode)
{
    if(UTILS_GetElapsedTimeInMs(&_Timer50MS) >= FIFTY_MSEC)
    {
        UTILS_ResetTimer(&_Timer50MS);

        /* Instead of creating local instances of sensor structure wherever needed, craeted private global class limited
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
    bool bCertainlyNotOnGenset = false;
    bool bCertainlyNotOnMains = false;
    if (!ci.bCtOnLoadCable)
    {
        //i.e. CT is on alternator output cable
        //hence,
        return LOAD_CONT_STATUS_t::LOAD_ON_GEN;
    }
    if (ci.bGensetContactorFeedbackIsAssigned)
    {
        if (ci.bGensetContactorFeedbackInputStatus)
        {
            return LOAD_CONT_STATUS_t::LOAD_ON_GEN;
        }
        //else is not necessary
        bCertainlyNotOnGenset = true;
    }
    if (ci.bMainsContactorFeedbackIsAssigned)
    {
        if (ci.bMainsContactorFeedbackInputStatus)
        {
            return LOAD_CONT_STATUS_t::LOAD_ON_MAINS;
        }
        //else is not necessary
        bCertainlyNotOnMains = true;
    }
    if ((ci.bGensetContactorFeedbackIsAssigned)
        && (ci.bMainsContactorFeedbackIsAssigned)
        && (!ci.bGensetContactorFeedbackInputStatus)
        && (!ci.bMainsContactorFeedbackInputStatus))
    {
        //if both (Genset and Mains) feedback inputs are assigned
        //and both those inputs are off, means load is certainly
        //not on Genset nor on Mains. Hence
        return LOAD_CONT_STATUS_t::LOAD_NOT_ON_GEN_MAINS;
    }
    if (!ci.bGensetContactorFeedbackIsAssigned)
    {
        //feedback not available:
        if (ci.bGensetContactorOutputAssigned)
        {
            //output is assigned and is not pulse type
            if (ci.bGensetContactorOutputStatus)
            {
                return LOAD_CONT_STATUS_t::LOAD_ON_GEN;
            }
            //else is not necessary
            bCertainlyNotOnGenset = true;
        }
        else
        {
            //output is not assigned at all
            //do nothing here, we will anyway default to Genset
        }
    }
    //now check mains
    if (!ci.bMainsContactorFeedbackIsAssigned)
    {
        //feedback not available:
        if (ci.bMainsContactorOutputAssigned)
        {
                //output is assigned and is not pulse type
                if (ci.bMainsContactorOutputStatus)
                {
                    return LOAD_CONT_STATUS_t::LOAD_ON_MAINS;
                }
                //else is not necessary
                bCertainlyNotOnMains = true;
        }
        else
        {
            //output is not assigned at all
            //do nothing here, we will anyway default to Genset
        }
    }
    if ((bCertainlyNotOnGenset) || (bCertainlyNotOnMains))
    {
        return LOAD_CONT_STATUS_t::LOAD_NOT_ON_GEN_MAINS;
    }
    return LOAD_CONT_STATUS_t::LOAD_ON_GEN;
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
        return(_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR)!=ACT_Manager::ACT_NOT_CONFIGURED);
}
bool ENGINE_MONITORING::isGensetContactorOutputAssigned()
{

    return (_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_GEN_CONTACTOR)!=ACT_Manager::ACT_NOT_CONFIGURED);
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

    eTampStatusCurrent = IsGenStartValid(); /* stores true if tampered start */
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

//    if((_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR)==ACT_Manager::ACT_NOT_CONFIGURED)
//        && (!_GCUAlarms.IsAlarmMonEnabled(GCU_ALARMS::MAINS_CONTACTOR_LATCHED))
//        && (_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_GEN_CONTACTOR)==ACT_Manager::ACT_NOT_CONFIGURED)
//        && (!_GCUAlarms.IsAlarmMonEnabled(GCU_ALARMS::GEN_CONTACTOR_LATCHED)))
//    {
//        eLoadStatusCurrent = LOAD_ON_GEN;
//    }
//    else if((_cfgz.GetCFGZ_Param(CFGZ::ID_CT_LOCATION) == 1) // On Load Cable
//           && (BASE_MODES::IsMainsContactorClosed()) && (!_GCUAlarms.IsAlarmMonEnabled(GCU_ALARMS::MAINS_CONTACTOR_LATCHED)))
//    {
//        eLoadStatusCurrent = LOAD_ON_MAINS;
//    }
//    else if((_cfgz.GetCFGZ_Param(CFGZ::ID_CT_LOCATION) == 1) // On Load Cable
//            && (_GCUAlarms.AlarmResultInstat(GCU_ALARMS::MAINS_CONTACTOR_LATCHED)))
//    {
//        eLoadStatusCurrent = LOAD_ON_MAINS;
//    }
//    else if((_cfgz.GetCFGZ_Param(CFGZ::ID_CT_LOCATION) == 0)// On Alt Cable
//            || ((_cfgz.GetCFGZ_Param(CFGZ::ID_CT_LOCATION) == 1) // On Load Cable
//                  && (BASE_MODES::IsGenContactorClosed()) && (!_GCUAlarms.IsAlarmMonEnabled(GCU_ALARMS::GEN_CONTACTOR_LATCHED))))
//    {
//        eLoadStatusCurrent = LOAD_ON_GEN;
//    }
//    else if((_cfgz.GetCFGZ_Param(CFGZ::ID_CT_LOCATION) == 1) // On Load Cable
//            && (_GCUAlarms.AlarmResultInstat(GCU_ALARMS::GEN_CONTACTOR_LATCHED)))
//    {
//        eLoadStatusCurrent = LOAD_ON_GEN;
//    }
//    else if((!_GCUAlarms.IsAlarmMonEnabled(GCU_ALARMS::GEN_CONTACTOR_LATCHED))
//            &&((_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_GEN_BREAKER_PULSE)!=ACT_Manager::ACT_NOT_CONFIGURED)
//            ||(_hal.actuators.GetActStatus(ACTUATOR::ACT_OPEN_GEN_BREAKER_PULSE)!=ACT_Manager::ACT_NOT_CONFIGURED)))
//    {
//        eLoadStatusCurrent = LOAD_ON_GEN;
//    }
//    else
//    {
//        eLoadStatusCurrent = LOAD_NOT_ON_GEN_MAINS;
//    }

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
    else if(u32RunTime > RUN_MIN_2nd_SLOT)
    {
        u16TimeSlot = TIME_3rd_SLOT_SEC;
    }
    else
    {
        /* do nothing */
    }

    return u16TimeSlot;
}

void ENGINE_MONITORING::prvUpdateEngineOn()
{
    if((_GCUAlarms.GetSpeedValue() > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_ENGINE_SPEED))
        || (_hal.AcSensors.GENSET_GetApproxFreq(R_PHASE) > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_ALT_FREQUENCY)))
    {
        prvUpdateEngineONstatus();
    }
    else
    {
        UTILS_DisableTimer(&_TimerGenUpdateCumulative);
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
        if((_GCUAlarms.GetSpeedValue() > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_ENGINE_SPEED))
            ||(_hal.AcSensors.GENSET_GetApproxFreq(R_PHASE) > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_ALT_FREQUENCY))
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
            _u8GenReady = true;

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
            StoreCummulativeCnt();
            UTILS_ResetTimer(&_GenWarmUpTimer);
        }
    }
    else if((_GCUAlarms.IsCommonShutdown()) || (_GCUAlarms.IsCommonElectricTrip()) || (_u8EngineOn == 0U))
    {
        _u8GenReady = false;
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
                || (_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_DISCONN_ON_LLOP_SW) == CFGZ::CFGZ_DISABLE))
            && ((_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_DISCONN_ON_LOP_SENS) == CFGZ::CFGZ_DISABLE)
                || (_stLOP.stValAndStatus.f32InstSensorVal < _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_DISCONN_LOP_SENS))
                || ( (_stLOP.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT) ||
                     (_stLOP.eStatus != A_SENSE::SENSOR_NOT_CONFIGRUED) )
                )
            )
        {
            _u8EngineOn = false;
            _bEngineCranked = false;
            _u8EngineOff = true;
            UTILS_DisableTimer(&_TimerOneMin);
            if((!_GCUAlarms.IsCommonShutdown()) && START_STOP::IsStopRelayON())
            {
                _GCUAlarms.LogEvent(GCU_ALARMS::Engine_Stop_id, GCU_ALARMS::ID_NONE);
            }
            else
            {
                /* do nothing */
            }

            if(CFGC::IsSGC421())
            {
                EGOV::ResetEgovVars();
            }
            else
            {
                /* do nothing */
            }
        }
    }
    else
    {
        /* todo: need to confirm abount EGOVE functionality. IF it is not there then need to remove
                 below function call */
        if(CFGC::IsSGC421())
        {
            EGOV::ResetEgovVars();
        }
        else
        {
            /* do nothing */
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
        StoreCummulativeCnt();  /* store cumm data if trip arises */
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


void ENGINE_MONITORING::StoreCummulativeCnt()
{
    static CUMULATIVE_t stStoredCummulative0,stStoredCummulative1;

    prvUpdateCumulativeEnergyCounts();
    prvUpdateCumulativeTamperedEnergyCounts();

    _stCummulativeCnt.u64Header++;
    _stCummulativeCnt.u32CRC =(uint16_t) CRC16::ComputeCRCGeneric((uint8_t *)&_stCummulativeCnt,
                                              sizeof(CUMULATIVE_t) - sizeof(uint32_t)
                                              , CRC_MEMORY_SEED);
    if(_u8ActiveSectorForCummulative == 0)
    {
        stStoredCummulative0 = _stCummulativeCnt;
       _hal.Objeeprom.RequestWrite(EXT_EEPROM_CUMMULATIVE_START_ADD_SECT0, (uint8_t*)&stStoredCummulative0,
                                   sizeof(CUMULATIVE_t), NULL);
       _u8ActiveSectorForCummulative = 1;
    }
    else
    {
        stStoredCummulative1 = _stCummulativeCnt;
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
//float ENGINE_MONITORING::GetTamprEEPromCummEnergy()
//{
//    return _stCummulativeCnt.f32TamprGenKWH;
//}
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
        _stTampEnergyRegister.f32InitialActiveEnergyWH = _hal.AcSensors.GENSET_GetTotalTamperedActiveEnergySinceInitWH();
        _stTampEnergyRegister.f32InitialApparentEnergyVA = _hal.AcSensors.GENSET_GetTotalTamperedApparentEnergySinceInitVAH();
        _stTampEnergyRegister.f32InitialReactiveEnergyVAR  = _hal.AcSensors.GENSET_GetTotalTamperedReactiveEnergySinceInitVARH();

        _stEnergyRegister.f32InitialActiveEnergyWH = _hal.AcSensors.GENSET_GetTotalActiveEnergySinceInitWH();
        _stEnergyRegister.f32InitialApparentEnergyVA = _hal.AcSensors.GENSET_GetTotalApparentEnergySinceInitVAH();
        _stEnergyRegister.f32InitialReactiveEnergyVAR  = _hal.AcSensors.GENSET_GetTotalReactiveEnergySinceInitVARH();

        _stMainsEnergyRegister.f32InitialActiveEnergyWH = _hal.AcSensors.MAINS_GetTotalActiveEnergySinceInitWH();
        _stMainsEnergyRegister.f32InitialApparentEnergyVA = _hal.AcSensors.MAINS_GetTotalApparentEnergySinceInitVAH();
        _stMainsEnergyRegister.f32InitialReactiveEnergyVAR = _hal.AcSensors.MAINS_GetTotalReactiveEnergySinceInitVARH();
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
        _stCummulativeCnt.f32GenKWH =0.0;
        _stCummulativeCnt.f32GenKVAH =0.0;
        _stCummulativeCnt.f32GenKVARH =0.0;

        _stCummulativeCnt.f32MainsKWH =0.0;
        _stCummulativeCnt.f32MainsKVAH =0.0;
        _stCummulativeCnt.f32MainsKVARH =0.0;

        _stCummulativeCnt.f32TamprGenKWH = 0.0;
        _stCummulativeCnt.f32TamprGenKVAH = 0.0;
        _stCummulativeCnt.f32TamprGenKVARH = 0.0;

        _u8ActiveSectorForCummulative =0;
    }
}

void ENGINE_MONITORING::prvUpdateEngineONstatus(void)
{
    if(_u8EngineOn == 0U)
    {
        _u8EngineOn = 1;
        UTILS_ResetTimer(&_TimerOneMin);
        UTILS_ResetTimer(&_TimerGenUpdateCumulative);
    }
    else
    {
        /* do nothing */
    }
}

void ENGINE_MONITORING::StoreEngRnCnt(uint32_t u32cnt)
{
    _stCummulativeCnt.u32EngineRunTime_min = u32cnt;
}

void ENGINE_MONITORING::StoreMainsRnCnt(uint32_t u32cnt)
{
    _stCummulativeCnt.u32MainsRunTime_min = u32cnt;
}
void ENGINE_MONITORING::StoreTripCnt(uint32_t u32cnt)
{
    if(u32cnt > MAX_NO_OF_TRIPS)
    {
        _stCummulativeCnt.u32GenNumberOfTrips = 0;
    }
    else
    {
        _stCummulativeCnt.u32GenNumberOfTrips = u32cnt;
    }
}
void ENGINE_MONITORING::StoreStartCnt(uint32_t u32cnt)
{
    if(u32cnt > MAX_NO_OF_STARTS)
    {
        _stCummulativeCnt.u32GenNumberOfStarts = 0;
    }
    else
    {
        _stCummulativeCnt.u32GenNumberOfStarts = u32cnt;
    }
}

void ENGINE_MONITORING::StoreGenEnery(POWER_TP_t eTyep, uint32_t u32cnt)
{
    switch(eTyep)
    {
        case ACTIVE_POWER:
            _stCummulativeCnt.f32GenKWH = (float)u32cnt;
            break;
        case APARENT_POWER:
            _stCummulativeCnt.f32GenKVAH = (float)u32cnt;
            break;
        case REACTIVE_POWER:
            _stCummulativeCnt.f32GenKVARH = (float)u32cnt;
            break;
        default: break;
    }
}

void ENGINE_MONITORING::StoreMainsEnery(POWER_TP_t eTyep, uint32_t u32cnt)
{
    switch(eTyep)
    {
        case ACTIVE_POWER:
            _stCummulativeCnt.f32MainsKWH = u32cnt;
            break;
        case APARENT_POWER:
            _stCummulativeCnt.f32MainsKVAH = u32cnt;
            break;
        case REACTIVE_POWER:
            _stCummulativeCnt.f32MainsKVARH = u32cnt;
            break;
        default: break;
    }
}

void ENGINE_MONITORING::prvLoadHistogram(void)
{
    uint8_t u8LocalHistogramCnt =0;
    uint8_t u8HistogramMax = HISTO_RANGE_MAX;
    uint8_t u8HistogramMin = HISTO_RANGE_MIN;
    float f32TotalPercentKw =0;

    f32TotalPercentKw = (uint8_t)_hal.AcSensors.GENSET_GetPercentPower();

    for(u8LocalHistogramCnt = ID_HIST_1 ;u8LocalHistogramCnt <= ID_HIST_6; u8LocalHistogramCnt++)
    {
      if((f32TotalPercentKw >= u8HistogramMin) && ((f32TotalPercentKw <= u8HistogramMax)|| (u8LocalHistogramCnt==5)))
        {
          _stCummulativeCnt.u16ArrHistogram[u8LocalHistogramCnt]+=1;
          break;
        }
      u8HistogramMin = u8HistogramMax + 1U;
      u8HistogramMax +=HISTO_RANGE_MAX;
    }
}


void ENGINE_MONITORING::prvUpdateEngineRunHrs()
{
    static uint16_t u16TimeSlot = 1U; /* todo: initialization , need to think */
    if(UTILS_GetElapsedTimeInSec(&_TimerOneMin) >= ONE_MIN_CNT)
    {
        UTILS_ResetTimer(&_TimerOneMin);

        if((_u8EngineOn == 1U) && (IsGenStartValid()) &&
            (_GCUAlarms.GetSpeedValue() > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_ENGINE_SPEED)))
        {
            /* todo : If required, need to update J1939 dependency on run hours */
            _stCummulativeCnt.u32EngineRunTime_min++;

            u16TimeSlot = prvCheckTimeSlot(_stCummulativeCnt.u32EngineRunTime_min);
            if(UTILS_GetElapsedTimeInSec(&_TimerGenUpdateCumulative) >= u16TimeSlot)
            {
                StoreCummulativeCnt();
                UTILS_ResetTimer(&_TimerGenUpdateCumulative);
            }
        }
        else if((_u8EngineOn == 1U) && (!IsGenStartValid()) &&
                (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_WAVE_DETECTION) == CFGZ::CFGZ_ENABLE))
        {
            _stCummulativeCnt.u32TamperedRunTime_min++;
            if(_TimerUpdateTamperedCumulative.bEnabled == false)
            {
                UTILS_ResetTimer(&_TimerUpdateTamperedCumulative);
            }
            u16TimeSlot = prvCheckTimeSlot(_stCummulativeCnt.u32TamperedRunTime_min);
            if(UTILS_GetElapsedTimeInSec(&_TimerUpdateTamperedCumulative) >= u16TimeSlot)
            {
                StoreCummulativeCnt();
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

void ENGINE_MONITORING::prvUpdateMainsRunHrs()
{
    static uint16_t u16TimeSlot = 1U;

    if(((_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR) != ACT_Manager::ACT_NOT_CONFIGURED)
           ||(_hal.actuators.GetActStatus(ACTUATOR::ACT_OPEN_MAINS_OUT) != ACT_Manager::ACT_NOT_CONFIGURED))
        && (_cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_LOCATION) == CFGZ::ON_LOAD_CABLE)
        && BASE_MODES::IsMainsContactorClosed())
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
               StoreCummulativeCnt();
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
        && (!BASE_MODES::IsGenContactorClosed())
        && (((!BASE_MODES::IsMainsContactorClosed()) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
                    || ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE) && (_u8EngineOn == 0U))))
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
               StoreCummulativeCnt();
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
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_DISCONN_ON_CHG_ALT_VOLT) == CFGZ::CFGZ_ENABLE)
        && (_hal.AnalogSensors.GetFilteredChargingAltVolts() > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_CHG_ALT_THRESHOLD))
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
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_DISCONN_ON_LOP_SENS)  == CFGZ::CFGZ_ENABLE) &&
        ((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
        ||((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_LOP_CURR_SENSOR))) &&
        (_stLOP.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT) &&
        (_stLOP.stValAndStatus.f32InstSensorVal > _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_DISCONN_LOP_SENS)))
    {
        /* wait for 1 sec to make a decision */
        if(UTILS_GetElapsedTimeInSec(&_LOPSensMonTimer) >= 1U)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
       UTILS_ResetTimer(&_LOPSensMonTimer);
       return false;
    }
}

bool ENGINE_MONITORING::prvDisconnectCranckByLOPSwitch()
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_DISCONN_ON_LLOP_SW) == CFGZ::CFGZ_ENABLE)
    {
        if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_LUBE_OIL_PRESSURE_SWITCH) != DigitalSensor::SENSOR_NOT_CONFIGRUED)
        {
            if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_LUBE_OIL_PRESSURE_SWITCH) == DigitalSensor::SENSOR_LATCHED)
            {
                /* todo: test LLOP functionality for cranck disconnet specifically  */
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

            if(UTILS_GetElapsedTimeInMs(&_LLOPCrankingTimer) >= (uint64_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_LLOP_SW_TRANS_TIME)*1000))
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
    else
    {
        return  false;
    }

}


bool ENGINE_MONITORING::IsGenStartValid()
{
    /* function returns true if valid start found */
    /* (invalidity enabled & valid start found & wave detection enabled) || (wave detection disabled) */
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_WAVE_DETECTION) == CFGZ::CFGZ_ENABLE)
    {
        return ((true == ENGINE_START_VALIDITY::GetStartWaveDetectionStatus())
                && (false == ENGINE_START_VALIDITY::GetEngineStartInvalidity()));
    }
    else
    {
        return true;
    }
}

