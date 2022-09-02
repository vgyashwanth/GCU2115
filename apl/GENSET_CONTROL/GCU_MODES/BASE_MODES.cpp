/**
 * @file        [BASE_MODES]
 * @brief       BASE_MODES
 *              Provides common API's for functionality offered by Auto and
 *              Manual SMD's. This module is implemented in class BASE_MODES
 *              which is the base class for all other SMD classes.
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        1st June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#include "BASE_MODES.h"
#include "AUTO_EXERCISE_MODE.h"
bool BASE_MODES::_bOpenMainsLoad = false;
bool BASE_MODES::_bOpenGenLoad = false;
bool BASE_MODES::_bOpenMainsStatus = false;
bool BASE_MODES::_bCloseMainsStatus = false;
bool BASE_MODES::_bRemoteStartRCVD = false;
bool BASE_MODES::_bRemoteStopRCVD = false;
bool BASE_MODES::_bContactorTransferOn = false;
bool BASE_MODES::_bCloseGenContactor = false;
bool BASE_MODES::_bCloseMainsContactor = false;
bool BASE_MODES::_bModeSwitchFromAuto = false;
bool BASE_MODES::_bStopRequest = false;
bool BASE_MODES::_bStartRequest = false;
bool BASE_MODES::_bEmergencyStop = false;
bool BASE_MODES::_bSwitchLoadToMains = false;
bool BASE_MODES::_bSwitchLoadToGen = false;
bool BASE_MODES::_bMBStartCmdRequested = false;
bool BASE_MODES::_bMBStopCmdRequested = false;
bool BASE_MODES::_bMBStartCmdReceived = false;
bool BASE_MODES::_bMBStopCmdReceived = false;
bool BASE_MODES::_bOpenGenStatus = false;
bool BASE_MODES::_bCloseGenStatus = false;
bool BASE_MODES::_bOpenGenReceived = false;
bool BASE_MODES::_bOpenMainsReceived = false;

bool BASE_MODES:: _bStartPress = false;
bool BASE_MODES:: _bStopPress = false;



stTimer BASE_MODES::_ReturnToMainsTimer = {0};
stTimer BASE_MODES::_EngCoolDownTimer = {0};
stTimer BASE_MODES::_GCUSMUpdateTimer = {0};
stTimer BASE_MODES::_BaseModeUpdateTimer = {0};
stTimer BASE_MODES::_MainsMonUpdateTimer = {0};
stTimer BASE_MODES::_GenPulseTimer = {0};
stTimer BASE_MODES::_MainsPulseTimer = {0};
BASE_MODES::MANUAL_STATE_t BASE_MODES::_eManualState = STATE_MANUAL_GEN_OFF;
BASE_MODES::AMF_STATE_t BASE_MODES::_eAutoState = STATE_AMF_GEN_OFF_MAINS_OFF;
BASE_MODES::MAINS_STATUS_t BASE_MODES::_MainsStatus = BASE_MODES::MAINS_HELATHY;
BASE_MODES::AUTO_EXE_STATE_t BASE_MODES:: _eAutoExeState = ID_AUTO_EXE_DG_OFF;
BASE_MODES::BTS_STATE_t BASE_MODES::_eBTSState = STATE_BTS_GEN_OFF_MAINS_OFF;
BASE_MODES::CYCLIC_STATE_t BASE_MODES::_eCyclicState = STATE_CYCLIC_GEN_OFF_MAINS_OFF;
stTimer BASE_MODES::_ContactorTransferTimer = {0};
bool BASE_MODES::_bContTransferToMainsOn = false;
bool BASE_MODES::_bContTransferToGenOn = false;
BASE_MODES::GCU_OPERATING_MODE_t BASE_MODES::_eOperatingMode =MANUAL_MODE;
uint16_t BASE_MODES::_u16SchOnTimeMin = 0;
uint16_t BASE_MODES::_u16SchOnTimeHr= 0;
uint32_t BASE_MODES::_u32SchRemTime_sec= 0;
uint16_t BASE_MODES:: _u16NightModeStartTime = 0;
uint16_t BASE_MODES:: _u16NightModeStopTime = 0;
uint16_t BASE_MODES:: _u16NightModeDurationHrsMin = 0;
bool BASE_MODES::_bGCUStateChanged= false;
BASE_MODES::GCU_STATE_t BASE_MODES::_ePrevGCUState=EMPTY_GCU_STATE;
//BASE_MODES::TIMER_STATE_t BASE_MODES::_ePrevTimerState = NO_TIMER_RUNNING;
//BASE_MODES::GCU_OPERATING_MODE_t BASE_MODES::_ePrevOperatingMode = MANUAL_MODE;

bool BASE_MODES::_bRPhasHealthyStatus = false;
bool BASE_MODES::_bYPhasHealthyStatus = false;
bool BASE_MODES::_bBPhasHealthyStatus = false;
bool BASE_MODES::_bMainsPartialHealthy = false;
bool BASE_MODES::_bMainsPartialLEDStatus = false;
bool BASE_MODES::_bMainsHigh = false;
bool BASE_MODES::_bMainsLow  = false;
bool BASE_MODES::_bNightModeRestrict = false;
bool BASE_MODES::_bLoadTransferEn = false;
bool BASE_MODES::_bSchGenStart = false;
bool BASE_MODES::_bIsHealthyPhCntIncr = false;

//#define IS_GEN_LOAD_INHIBIT_IP_ENABLED()        _GCUAlarms.IsAlarmMonEnabled(GCU_ALARMS::GEN_LOAD_INHIBIT)
#define IS_GEN_LOAD_INHIBIT_IP_ENABLED() 1
//#define IS_GEN_LOAD_INHIBIT_IP_ACTIVATED()      _GCUAlarms.IsAlarmActive(GCU_ALARMS::GEN_LOAD_INHIBIT)
#define IS_GEN_LOAD_INHIBIT_IP_ACTIVATED() 1
#define GEN_CONTACTOR_IS_ABOUT_TO_LATCH()       ((_bContTransferToGenOn) || (_bSwitchLoadToGen))
#define IS_GEN_CONTACTOR_LATCHED()              ((_bCloseGenContactor) || GEN_CONTACTOR_IS_ABOUT_TO_LATCH())
#define GEN_CONTACTOR_NOT_LATCHED()             (!_bCloseGenContactor)
#define LOAD_XFER_TO_GEN_NOT_INITIATED()        (!_bContTransferToGenOn)

//#define IS_MAINS_LOAD_INHIBIT_IP_ENABLED()      _GCUAlarms.IsAlarmMonEnabled(GCU_ALARMS::MAINS_LOAD_INHIBIT)
#define IS_MAINS_LOAD_INHIBIT_IP_ENABLED()      1
//#define IS_MAINS_LOAD_INHIBIT_IP_ACTIVATED()    _GCUAlarms.IsAlarmActive(GCU_ALARMS::MAINS_LOAD_INHIBIT)
#define IS_MAINS_LOAD_INHIBIT_IP_ACTIVATED()  1
#define MAINS_CONTACTOR_IS_ABOUT_TO_LATCH()     ((_bContTransferToMainsOn) || (_bSwitchLoadToMains))
#define IS_MAINS_CONTACTOR_LATCHED()            ((_bCloseMainsContactor) || MAINS_CONTACTOR_IS_ABOUT_TO_LATCH())
#define MAINS_CONTACTOR_NOT_LATCHED()           (!_bCloseMainsContactor)
#define LOAD_XFER_TO_MAINS_NOT_INITIATED()      (!_bContTransferToMainsOn)

#define ENGINE_NOT_IN_COOLING_STAGE()           (prvEngineNotInCoolingStage())
#define ENGINE_STOP_IS_NOT_TRIGERRED()          (!START_STOP::IsStopCommand())
#define ENGINE_IS_RUNNING()                     ((START_STOP::GetStartStopSMDState()==START_STOP::ID_STATE_SS_ENG_ON) && ENGINE_NOT_IN_COOLING_STAGE() &&ENGINE_STOP_IS_NOT_TRIGERRED())
#define IS_GEN_AVAILABLE()                      (_EngineMon.IsGenAvailable())

#define IS_DEV_IN_MANUAL_MODE()                 (_eOperatingMode == MANUAL_MODE)
#define IS_DEV_IN_SCH_MODE()                    (_eOperatingMode == AUTO_EXERCISE_MODE)
#define IS_DEV_IN_AUTO_MODE()                   (_eOperatingMode != MANUAL_MODE)

#define IS_AUTO_LOAD_XFER_ENABLED()             (IS_DEV_IN_MANUAL_MODE() && (CFGZ::CFGZ_ENABLE == _cfgz.GetCFGZ_Param(CFGZ::ID_AUTOLOAD_TRANSFER)))
#define IS_SCH_LOAD_XFER_ENABLED()              ((IS_DEV_IN_SCH_MODE() && (_bLoadTransferEn))|| (!IS_DEV_IN_SCH_MODE()))
#define IS_SCH_LOAD_XFER_INITIATED()            ((_bLoadTransferEn) && (_bSchGenStart))

#define IS_RET_TO_MAINS_STARTED()               (UTILS_IsTimerEnabled(&_ReturnToMainsTimer))
#define IS_RET_TO_MAINS_EXPIRED()               (UTILS_GetElapsedTimeInSec(&_ReturnToMainsTimer) >= 5U)
#define CHK_RET_TO_MAINS()                      ((IS_RET_TO_MAINS_STARTED() && IS_RET_TO_MAINS_EXPIRED()) \
                                                    || !IS_RET_TO_MAINS_STARTED())

#define MAINS_MONITORING_ENABLED()              (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)
#define IS_MAINS_HEALTHY()                      (MAINS_MONITORING_ENABLED() && (_MainsStatus == MAINS_HELATHY))
#define IS_REMORE_SS_ENABLED()                  ((!MAINS_MONITORING_ENABLED()) && (_bRemoteStopRCVD))

#define IS_MAINS_PARTIAL_HEALTHY_CONFIGURED()   (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PARTIAL_HEALTHY_DETECT_EN) == CFGZ::CFGZ_ENABLE)
#define IS_MAINS_PH_RECOVERED_IN_3_PHASE()      ((_bIsHealthyPhCntIncr &&_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) != CFGZ::CFGZ_1_PHASE_SYSTEM && IS_MAINS_PARTIAL_HEALTHY_CONFIGURED()) \
                                                    || (!IS_MAINS_PARTIAL_HEALTHY_CONFIGURED()))

#define MAINS_HEALTHY_COND_FULFILLED()          (IS_MAINS_HEALTHY() || IS_REMORE_SS_ENABLED())

BASE_MODES::BASE_MODES(HAL_Manager &hal, CFGZ &cfgz,  GCU_ALARMS &GCUAlarms,
                          ENGINE_MONITORING &EngineMon, GCU_MODE_VARS_t &vars):
_hal(hal),
_cfgz(cfgz),
_GCUAlarms(GCUAlarms),
_EngineMon(EngineMon),
_vars(vars)
{
    UTILS_ResetTimer(&_BaseModeUpdateTimer);
    UTILS_DisableTimer(&_GenPulseTimer);
    UTILS_DisableTimer(&_MainsPulseTimer);
    InitNightModeParam();
}

void BASE_MODES::Update()
{
    static bool entry = false;
    if((UTILS_GetElapsedTimeInMs(&_MainsMonUpdateTimer)) >= FIFTY_MSEC)
    {
        if(entry == false)
        {
            entry = true;
            _ePrevGCUState = _vars.GCUState;
//            _ePrevTimerState = _vars.TimerState;
//            _ePrevOperatingMode = _eOperatingMode;
        }

        if((_ePrevGCUState != _vars.GCUState))
//                ||( _ePrevTimerState != _vars.TimerState)
//                ||(_ePrevOperatingMode != _eOperatingMode)
//                ||(_vars.TimerState == CRANK_START_TIMER )
//                ||(_vars.TimerState == CRANK_REST_TIMER)
//                ||(_vars.TimerState == COOL_DOWN_TIMER)
//                ||(_vars.TimerState == STOP_ACTION_TIMER))
        {
            _bGCUStateChanged = true;
            _ePrevGCUState = _vars.GCUState;
//            _ePrevTimerState = _vars.TimerState;
//            _ePrevOperatingMode = _eOperatingMode;
        }

        UTILS_ResetTimer(&_MainsMonUpdateTimer);
        prvUpdateNightModeRestrictStatus();
        UpdateMainsStatus();
        prvUpdateContactorOutputs();
//        prvUpdateBreakerPulseState();
        prvUpdateBTSBattHybrdModeStatus();
        if(_hal.DigitalSensors.GetDigitalSensorState(
                DigitalSensor::DI_EMERGENCY_STOP) == DigitalSensor::SENSOR_LATCHED)
        {
            _bEmergencyStop = true;
        }
        else
        {
            _bEmergencyStop = false;
        }

        prvOperatingModeOutputs();
    }
}

void BASE_MODES::prvUpdateBTSBattHybrdModeStatus(void)
{
    // If site is on BTS battery as it is healthy hence DG is in OFF state with no alarm condition,
    // then need to manipulate output - BTS battery hybrid mode
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON) == CFGZ::CFGZ_ENABLE)
            && (!_EngineMon.IsGenReady())
            && (!_bCloseGenContactor)
            && (!_bCloseMainsContactor)
            && (_eOperatingMode == BTS_MODE)
            && (_GCUAlarms.IsCommonAlarm()))
    {
        _hal.actuators.Activate(ACTUATOR::ACT_BTS_BATTERY_HYBRID_MODE);
    }
    else if(_bCloseGenContactor || _bCloseMainsContactor
            || (_GCUAlarms.IsCommonAlarm()) || (_EngineMon.IsGenReady()) || (_eOperatingMode != BTS_MODE))
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_BTS_BATTERY_HYBRID_MODE);
    }
}
BASE_MODES::GCU_STATE_t BASE_MODES::GetGCUState()
{
  return _vars.GCUState;
}

BASE_MODES::TIMER_STATE_t BASE_MODES::GetTimerState()
{
  return _vars.TimerState;
}

bool BASE_MODES:: GetStopPressState()
{
    return (bool)_bStopPress;
}

void BASE_MODES:: SetStopPressState(bool state)
{
    _bStopPress = state;
}

bool BASE_MODES:: GetStartPressState()
{
    return (bool)_bStartPress;
}

void BASE_MODES:: SetStartPressState(bool state)
{
    _bStartPress = state;
}

void BASE_MODES::Stop()
{
    _bStopRequest = true;
}

void BASE_MODES::Start()
{
    _bStartRequest = true;
}

void BASE_MODES::SwitchToManualMode()
{
    _bModeSwitchFromAuto = true;
}

void BASE_MODES::SwitchLoadToMains()
{
    _bSwitchLoadToMains = true;
}

void BASE_MODES::SwitchLoadToGen()
{
    _bSwitchLoadToGen = true;
}


void BASE_MODES::DisableEngCoolTimer()
{
    UTILS_DisableTimer(&_EngCoolDownTimer);
}

void BASE_MODES::DisableReturnToMains()
{
    UTILS_DisableTimer(&_ReturnToMainsTimer);
}

bool BASE_MODES::IsGCUStateChanged()
{
    return _bGCUStateChanged;
}

void BASE_MODES::ClearGCUStateChangedStatus()
{
    _bGCUStateChanged = false;
}
void BASE_MODES::prvSetGCUState()
{
    if(_GCUAlarms.IsCommonShutdown())
    {
        _vars.GCUState = SHUTDOWN;
    }                
    else if(_GCUAlarms.IsCommonElectricTrip())
    {
        _vars.GCUState = ELECTRIC_TRIP;
    }       
    else if(_GCUAlarms.IsCommonWarning())
    {
        _vars.GCUState = WARNING;
    }            
    else if(_GCUAlarms.IsCommonNotification())
    {
        _vars.GCUState = NOTIFICATION;  
    }     
}

void BASE_MODES::UpdateMainsStatus()
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)
    {
        _MainsStatus = MAINS_HELATHY;
    }
    else if(_hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_SIMULATE_MAINS) ==  DigitalSensor::SENSOR_LATCHED)
    {
        _MainsStatus = MAINS_HELATHY;
    }



          {
              _MainsStatus = MAINS_HELATHY;
              _bMainsPartialHealthy = true;
          }


}
void BASE_MODES::prvHandleInhibitInputs(void)
{
    static uint8_t u8GenToggle , u8MainsToggle;
    if(IS_GEN_LOAD_INHIBIT_IP_ACTIVATED()&& IS_GEN_CONTACTOR_LATCHED())
    {
        /* If Gen load inhibit input is activated then open Genset contactor */
        _bSwitchLoadToGen = false;
        _bOpenGenLoad= true;

        u8GenToggle = 0;
    }
    else if(IS_GEN_LOAD_INHIBIT_IP_ENABLED()
            &&(!IS_GEN_LOAD_INHIBIT_IP_ACTIVATED())
            && GEN_CONTACTOR_NOT_LATCHED()
            && ENGINE_IS_RUNNING()
            && IS_GEN_AVAILABLE()
            &&(IS_DEV_IN_AUTO_MODE()||(IS_AUTO_LOAD_XFER_ENABLED()))
            && LOAD_XFER_TO_GEN_NOT_INITIATED()
            && IS_SCH_LOAD_XFER_ENABLED()
            && (u8GenToggle == 0))
    {
        u8GenToggle = 1;
        _bSwitchLoadToGen = true;
    }

    if(IS_MAINS_LOAD_INHIBIT_IP_ACTIVATED()&& IS_MAINS_CONTACTOR_LATCHED())
    {
        /* If Mains load inhibit input is activated then open Mains contactor */
        _bSwitchLoadToMains = false;
        _bOpenMainsLoad= true;
        u8MainsToggle = 0;
    }
    else if(IS_MAINS_LOAD_INHIBIT_IP_ENABLED()
            && (!IS_MAINS_LOAD_INHIBIT_IP_ACTIVATED())
            && MAINS_CONTACTOR_NOT_LATCHED()
            && LOAD_XFER_TO_MAINS_NOT_INITIATED()
            && MAINS_HEALTHY_COND_FULFILLED()
            && (IS_MAINS_PH_RECOVERED_IN_3_PHASE())
            && IS_DEV_IN_AUTO_MODE()
            && (!IS_SCH_LOAD_XFER_INITIATED())
            && CHK_RET_TO_MAINS()
    )
    {
        _bSwitchLoadToMains = true;
        u8MainsToggle = 0;
    }
    else if(IS_MAINS_LOAD_INHIBIT_IP_ENABLED()
            && (!IS_MAINS_LOAD_INHIBIT_IP_ACTIVATED())
            && ENGINE_IS_RUNNING()
            && IS_AUTO_LOAD_XFER_ENABLED()
            && GEN_CONTACTOR_NOT_LATCHED()
            && LOAD_XFER_TO_GEN_NOT_INITIATED()
            && (0 == u8MainsToggle)
            && IS_GEN_AVAILABLE())
    {
        _bSwitchLoadToGen = 1;
        u8MainsToggle = 1;
    }

}

bool BASE_MODES::prvEngineNotInCoolingStage()
{
    bool bEngineNotInCoolingStage = true;
    switch(_eOperatingMode)
    {
        case TEST_MODE:
        case MANUAL_MODE:
            if(_eManualState == STATE_MANUAL_ENGINE_COOLING)
            {
                bEngineNotInCoolingStage = false;
            }
            break;

        case AUTO_MODE:
            if(_eAutoState == STATE_AMF_ENGINE_COOLING)
            {
                bEngineNotInCoolingStage = false;
            }
            break;

        case BTS_MODE:
            if(_eBTSState == STATE_BTS_ENGINE_COOLING)
            {
                bEngineNotInCoolingStage = false;
            }
            break;

        case CYCLIC_MODE:
            if(_eCyclicState == STATE_CYCLIC_ENGINE_COOLING)
            {
                bEngineNotInCoolingStage = false;
            }
            break;

        case AUTO_EXERCISE_MODE:
            if(_eAutoExeState == ID_AUTO_EXE_ENGINE_COOLING)
            {
                bEngineNotInCoolingStage = false;
            }
            break;

        default:
            break;

    }
    return bEngineNotInCoolingStage;
}

void BASE_MODES::prvUpdateContactorOutputs()
{
    prvHandleInhibitInputs();
    if(((_bSwitchLoadToMains)||((_hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_CLOSE_MAINS_OPEN_GEN_SWITCH) ==  
                DigitalSensor::SENSOR_LATCHED) && (_MainsStatus == MAINS_HELATHY) && (_eOperatingMode == BASE_MODES::MANUAL_MODE))) && (!_bContactorTransferOn))
    {
        _bSwitchLoadToMains = false;
        UTILS_ResetTimer(&_ContactorTransferTimer);
        _bContactorTransferOn = true;
        _bContTransferToMainsOn = true;
        // make gen contactor output inactive
        _bCloseGenContactor = false;
    }

    if(((_bSwitchLoadToGen) || ((_hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_CLOSE_GEN_OPEN_MAINS_SWITCH) ==  
                DigitalSensor::SENSOR_LATCHED) && _EngineMon.IsWarmUpTimeExpired())) && (!_bContactorTransferOn)
            )
    {
        _bSwitchLoadToGen = false;
        UTILS_ResetTimer(&_ContactorTransferTimer);
        _bContactorTransferOn = true;
        _bContTransferToGenOn = true;
        // make mains contactor output inactive
        _bCloseMainsContactor = false;

    }

    if(UTILS_GetElapsedTimeInSec(&_ContactorTransferTimer) >= 
            _cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_TRANS_DELAY))
    {
        _bContactorTransferOn = false;
        UTILS_DisableTimer(&_ContactorTransferTimer);

        if(_bContTransferToMainsOn)
        {
            _bContTransferToMainsOn = false;
            // make mains contactor out active
            _bCloseMainsContactor = true;
            _bCloseMainsStatus = true;
        }
        else if(_bContTransferToGenOn)
        {
            _bContTransferToGenOn = false;
            // make gen contactor active
            _bCloseGenContactor = true;
            _bCloseGenStatus = true;
        }
    }

    if(_bOpenMainsLoad)
    {
        _bOpenMainsStatus = true;
    }
    if(_bOpenGenLoad)
    {
        _bOpenGenStatus = true;

    }

    prvOpenGenMainLoad(&_bOpenGenLoad, &_bContTransferToGenOn, &_bCloseGenContactor);
    prvOpenGenMainLoad(&_bOpenMainsLoad, &_bContTransferToMainsOn, &_bCloseMainsContactor);

    if(_bCloseGenContactor)
    {
        _hal.actuators.Activate(ACTUATOR::ACT_CLOSE_GEN_CONTACTOR);
        _hal.actuators.Deactivate(ACTUATOR::ACT_OPEN_GEN_OUT);
    }
    else
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_CLOSE_GEN_CONTACTOR);
        _hal.actuators.Activate(ACTUATOR::ACT_OPEN_GEN_OUT);
    }

    if(_bCloseMainsContactor)
    {
        _hal.actuators.Activate(ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR);
        _hal.actuators.Deactivate(ACTUATOR::ACT_OPEN_MAINS_OUT);
    }
    else
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR);
        _hal.actuators.Activate(ACTUATOR::ACT_OPEN_MAINS_OUT);
    }
}

void BASE_MODES::prvOpenGenMainLoad(bool *bOpenLoad, bool *bContTransferToLoadOn ,
                                                            bool *bCloseContactor)
{
    if(*bOpenLoad)
    {
        *bOpenLoad = false;
        if((_bContactorTransferOn) || ( *bContTransferToLoadOn))
        {
            _bContactorTransferOn = false;
            UTILS_DisableTimer(&_ContactorTransferTimer);
            *bContTransferToLoadOn = false;
        }
        *bCloseContactor = false;
    }
}

void BASE_MODES::SetGCUOperatingMode(GCU_OPERATING_MODE_t eGCUMode)
{
    _eOperatingMode = eGCUMode;
}

BASE_MODES::GCU_OPERATING_MODE_t BASE_MODES::GetGCUOperatingMode()
{
    return _eOperatingMode ;
}

BASE_MODES::MAINS_STATUS_t BASE_MODES::GetMainsStatus()
{
    return _MainsStatus;
}


BASE_MODES::AMF_STATE_t BASE_MODES::GetAutoModeState()
{
    return _eAutoState;
}

BASE_MODES::MANUAL_STATE_t BASE_MODES::GetManualModeState()
{
    return _eManualState;
}

BASE_MODES::AUTO_EXE_STATE_t  BASE_MODES::GetAutoExeState()
{
    return _eAutoExeState;
}
BASE_MODES::BTS_STATE_t  BASE_MODES::GetBTSModeState()
{
    return _eBTSState;
}
BASE_MODES::CYCLIC_STATE_t  BASE_MODES::GetCyclicModeState()
{
    return _eCyclicState;
}

void BASE_MODES::AssignModechangeParameters()
{
    switch(_eManualState)
    {
         case BASE_MODES::STATE_MANUAL_GEN_OFF:
             if(_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON) == CFGZ::CFGZ_ENABLE)
             {
                 _eOperatingMode =BTS_MODE;
                 _eBTSState = STATE_BTS_GEN_OFF_MAINS_OFF;
             }
             else if(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_CYCLIC_MODE) == CFGZ::CFGZ_ENABLE)
             {
                 _eOperatingMode = BASE_MODES::CYCLIC_MODE;
                 _eCyclicState = STATE_CYCLIC_GEN_OFF_MAINS_OFF;
             }
             else if(AUTO_EXERCISE_MODE::IsExerciserStarted())
             {
                 _eOperatingMode = AUTO_EXERCISE_MODE;
                 _eAutoExeState = ID_AUTO_EXE_DG_OFF;
             }
             else
             {
                 _eOperatingMode =AUTO_MODE;
                 _eAutoState = STATE_AMF_GEN_OFF_MAINS_OFF;
             }

        break;
        case BASE_MODES::STATE_MANUAL_GEN_READY:
//            _GCUAlarms.ResetMainsMonParams();
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON) == CFGZ::CFGZ_ENABLE)
                || (_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2))
            {
                _eOperatingMode =BTS_MODE;
                _eBTSState = STATE_BTS_GEN_ON_LOAD;
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_CYCLIC_MODE) == CFGZ::CFGZ_ENABLE)
            {
                _eOperatingMode = BASE_MODES::CYCLIC_MODE;
                _eCyclicState = STATE_CYCLIC_GEN_ON_LOAD;
            }
            else if(AUTO_EXERCISE_MODE::IsExerciserStarted())
            {
                _eOperatingMode = AUTO_EXERCISE_MODE;
                _eAutoExeState = ID_AUTO_EXE_DG_ON_LOAD;
            }
            else
            {
                _eOperatingMode =AUTO_MODE;
                _eAutoState = STATE_AMF_GEN_ON_LOAD;
            }
        break;
        default: break;
    }
    UTILS_ResetTimer(&_MainsMonUpdateTimer);

}

void BASE_MODES::OpenGenLoad()
{
    _bOpenGenLoad = true;
}

void BASE_MODES::OpenMainsLoad()
{
    _bOpenMainsLoad = true;
}

void BASE_MODES::prvOperatingModeOutputs()
{
    if((_eOperatingMode == AUTO_MODE) || (_eOperatingMode == AUTO_EXERCISE_MODE)
            || (_eOperatingMode == BTS_MODE) || (_eOperatingMode == CYCLIC_MODE))
    {
        _hal.actuators.Activate(ACTUATOR::ACT_MODE_AUTO);
    }
    else
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_MODE_AUTO);
    }

    if((_eOperatingMode == MANUAL_MODE) && (START_STOP::GetStartStopSMDState() > START_STOP::ID_STATE_SS_ENG_OFF_OK))
    {
        _hal.actuators.Activate(ACTUATOR::ACT_MODE_MANUAL);
    }
    else
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_MODE_MANUAL);
    }

    if((_eOperatingMode == MANUAL_MODE) &&  (START_STOP::GetStartStopSMDState() == START_STOP::ID_STATE_SS_ENG_OFF_OK))
    {
        _hal.actuators.Activate(ACTUATOR::ACT_MODE_STOP);
    }
    else
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_MODE_STOP);
    }
}

void BASE_MODES::ChangeAutoState(AMF_STATE_t eAutoSMDState)
{
    _eAutoState = eAutoSMDState;
}

void BASE_MODES::SetMainsStatusHealthy()
{
    _MainsStatus = MAINS_HELATHY;
}

bool BASE_MODES::GenStartCondition()
{
    if((_bRemoteStartRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE))
        || ((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BASE_MODES::SetTimerState(TIMER_STATE_t state)
{
    _vars.TimerState = state;
}

bool BASE_MODES::IsGenContactorClosed(void)
{
    return _bCloseGenContactor;
}

bool BASE_MODES::IsMainsContactorClosed(void)
{
    return _bCloseMainsContactor;
}

bool BASE_MODES::IsGenContactorOpen(void)
{
    return _bOpenGenReceived;
}

bool BASE_MODES::IsMainsContactorOpen(void)
{
    return _bOpenMainsReceived;
}


bool BASE_MODES::GetPressureSensorStatusBeforeStart()
{
    A_SENSE::SENSOR_RET_t stVal;
    stVal = _GCUAlarms.GetLOPSensorVal();

    if(1)
    {
        _GCUAlarms.ActivateHighOilPressAlarmSens();
        return false;
    }

    return true;
}

bool BASE_MODES::IsMainsContactorConfigured()
{
    if((_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR)!=ACT_Manager::ACT_NOT_CONFIGURED)
                       ||(_hal.actuators.GetActStatus(ACTUATOR::ACT_OPEN_MAINS_OUT)!=ACT_Manager::ACT_NOT_CONFIGURED))
    {
        return true;
    }
    return false;
}

bool BASE_MODES::IsGenContactorConfigured()
{
    if((_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_GEN_CONTACTOR)!=ACT_Manager::ACT_NOT_CONFIGURED)
                          ||(_hal.actuators.GetActStatus(ACTUATOR::ACT_OPEN_GEN_OUT)!=ACT_Manager::ACT_NOT_CONFIGURED))
   {
       return true;
   }
   return false;
}

bool BASE_MODES:: GetPartialHealthyStatus()
{
    return _bMainsPartialHealthy;
}


bool BASE_MODES::GetMainsLowStatus()
{
   return _bMainsLow;
}

bool BASE_MODES::GetMainsHighStatus()
{
    return _bMainsHigh;
}

bool BASE_MODES::IsLoadOnMains()
{
    if(_EngineMon.GetContactorLoadStatus() == ENGINE_MONITORING::LOAD_ON_MAINS)
    {
        return true;
    }
    return false;
}

void BASE_MODES::SetModeState(MANUAL_STATE_t eState)
{
    _eManualState =eState;
}

void BASE_MODES::SetModeState(AMF_STATE_t eState)
{
    _eAutoState =eState;
}

void BASE_MODES::SetModeState(AUTO_EXE_STATE_t eState)
{
    _eAutoExeState=eState;
}

void BASE_MODES::SetModeState(BTS_STATE_t eState)
{
    _eBTSState=eState;
}

void BASE_MODES::SetModeState(CYCLIC_STATE_t eState)
{
    _eCyclicState=eState;
}
bool BASE_MODES::GetIndividualPhaseStatus(PHASE_t ePhase)
{
    bool status= false;
    switch(ePhase)
    {
        case R_PHASE:
            status = _bRPhasHealthyStatus;
            break;
        case Y_PHASE:
           status = _bYPhasHealthyStatus;
           break;
        case B_PHASE:
           status = _bBPhasHealthyStatus;
           break;
        default: break;
    }
    return status;

}

uint8_t BASE_MODES::GetMainsHealthyPhaseCnt(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_1_PHASE_SYSTEM)
    {
        return 0;
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        return ((uint8_t)(_bRPhasHealthyStatus)
                + (uint8_t)(_bYPhasHealthyStatus) + (uint8_t)(_bBPhasHealthyStatus));
    }
    else
    {
        return ((uint8_t)(_bRPhasHealthyStatus) + (uint8_t)(_bYPhasHealthyStatus));
    }
}

void BASE_MODES::SetMainsPartialLEDstatus()
{
    _bMainsPartialLEDStatus = true;
}
void BASE_MODES::ClearMainsPartialLEDstatus()
{
    _bMainsPartialLEDStatus = false;
}
void BASE_MODES::ClearMainsPartialHealthyStatus(void)
{
    _bMainsPartialHealthy = false;
}
void BASE_MODES::InitNightModeParam()
{
    uint16_t u16RequiredHours = 0, u16RequiredMins = 0, u16IncrementReqHours = 0;

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_NIGHT_MODE) == CFGZ::CFGZ_ENABLE)
    {
        _u16NightModeStartTime = _cfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_START_TIME);

        _u16NightModeDurationHrsMin = (uint16_t)(((_cfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_OFF_DURATION))/60)*100)
                + (uint16_t)((_cfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_OFF_DURATION))%60);

        u16RequiredMins = (_u16NightModeStartTime%100 ) + (_u16NightModeDurationHrsMin%100);

        if(u16RequiredMins > 59)
        {
            u16IncrementReqHours = 1;
            u16RequiredMins = u16RequiredMins - 60;
        }
        else
        {
            u16IncrementReqHours = 0;
        }
        u16RequiredHours = (_u16NightModeStartTime/100 ) + (_u16NightModeDurationHrsMin/100) + u16IncrementReqHours;
        if(u16RequiredHours >= 24)
        {
            u16RequiredHours = u16RequiredHours - 24;
        }
        _u16NightModeStopTime = (u16RequiredHours*100) + u16RequiredMins;
    }
    else
    {
        _bNightModeRestrict = false;
    }

}

void BASE_MODES::prvUpdateNightModeRestrictStatus()
{
      RTC::TIME_t stCurrentTime;

      if(_cfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_NIGHT_MODE) == CFGZ::CFGZ_ENABLE)
      {
          _hal.ObjRTC.GetTime(&stCurrentTime);
           if(_u16NightModeStopTime < _u16NightModeStartTime)
           {
               if((((stCurrentTime.u8Hour == (_u16NightModeStartTime/100))&&(stCurrentTime.u8Minute >= (_u16NightModeStartTime%100)))
                       || (stCurrentTime.u8Hour > (_u16NightModeStartTime/100)))
                   ||
                   (((stCurrentTime.u8Hour == (_u16NightModeStopTime/100))&&(stCurrentTime.u8Minute < (_u16NightModeStopTime%100)))
                           || (stCurrentTime.u8Hour < (_u16NightModeStopTime/100))))

               {
                   _bNightModeRestrict = true;
               }
               else
               {
                   _bNightModeRestrict = false;
               }
           }
           else
           {

               if((((stCurrentTime.u8Hour == (_u16NightModeStartTime/100))&&(stCurrentTime.u8Minute >= (_u16NightModeStartTime%100)))
                       || (stCurrentTime.u8Hour > (_u16NightModeStartTime/100)))
                   &&
                   (((stCurrentTime.u8Hour == (_u16NightModeStopTime/100))&&(stCurrentTime.u8Minute < (_u16NightModeStopTime%100)))
                           || (stCurrentTime.u8Hour < (_u16NightModeStopTime/100))))
               {
                   _bNightModeRestrict = true;
               }
               else
               {
                   _bNightModeRestrict = false;
               }
           }
      }
      else
      {
          _bNightModeRestrict = false;
      }
}
bool BASE_MODES::IsNightModeRestrictOn()
{
    return _bNightModeRestrict;
}

//void BASE_MODES::prvUpdateBreakerPulseState()
//{}

