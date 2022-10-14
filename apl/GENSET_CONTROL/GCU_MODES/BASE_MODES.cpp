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
#include "START_STOP.h"
bool BASE_MODES::_bOpenMainsLoad = false;
bool BASE_MODES::_bOpenGenLoad = false;
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
stTimer BASE_MODES::_ReturnToMainsTimer = {0};
stTimer BASE_MODES::_EngCoolDownTimer = {0};
stTimer BASE_MODES::_GCUSMUpdateTimer = {0};
stTimer BASE_MODES::_BaseModeUpdateTimer = {0};
stTimer BASE_MODES::_MainsMonUpdateTimer = {0};
BASE_MODES::MANUAL_STATE_t BASE_MODES::_eManualState = STATE_MANUAL_GEN_OFF;
BASE_MODES::AMF_STATE_t BASE_MODES::_eAutoState = STATE_AMF_GEN_OFF_MAINS_OFF;
BASE_MODES::MAINS_STATUS_t BASE_MODES::_MainsStatus = BASE_MODES::MAINS_HELATHY;
BASE_MODES::BTS_STATE_t BASE_MODES::_eBTSState = STATE_BTS_GEN_OFF_MAINS_OFF;
BASE_MODES::CYCLIC_STATE_t BASE_MODES::_eCyclicState = STATE_CYCLIC_GEN_OFF_MAINS_OFF;
stTimer BASE_MODES::_ContactorTransferTimer = {0};
bool BASE_MODES::_bContTransferToMainsOn = false;
bool BASE_MODES::_bContTransferToGenOn = false;
BASE_MODES::GCU_OPERATING_MODE_t BASE_MODES::_eOperatingMode =MANUAL_MODE;
uint16_t BASE_MODES:: _u16NightModeStartTime = 0;
uint16_t BASE_MODES:: _u16NightModeStopTime = 0;
uint16_t BASE_MODES:: _u16NightModeDurationHrsMin = 0;
bool BASE_MODES::_bGCUStateChanged= false;
BASE_MODES::GCU_STATE_t BASE_MODES::_ePrevGCUState=EMPTY_GCU_STATE;
//BASE_MODES::TIMER_STATE_t BASE_MODES::_ePrevTimerState = NO_TIMER_RUNNING;
//BASE_MODES::GCU_OPERATING_MODE_t BASE_MODES::_ePrevOperatingMode = MANUAL_MODE;

bool BASE_MODES::_bNightModeRestrict = false;

#define MAINS_CONTACTOR_IS_ABOUT_TO_LATCH()     ((_bContTransferToMainsOn) || (_bSwitchLoadToMains))
#define IS_MAINS_CONTACTOR_LATCHED()            ((_bCloseMainsContactor) || MAINS_CONTACTOR_IS_ABOUT_TO_LATCH())
#define MAINS_CONTACTOR_NOT_LATCHED()           (!_bCloseMainsContactor)
#define LOAD_XFER_TO_MAINS_NOT_INITIATED()      (!_bContTransferToMainsOn)

#define IS_DEV_IN_MANUAL_MODE()                 (_eOperatingMode == MANUAL_MODE)
#define IS_DEV_IN_AUTO_MODE()                   (_eOperatingMode != MANUAL_MODE)

#define IS_AUTO_LOAD_XFER_ENABLED()             (IS_DEV_IN_MANUAL_MODE() && (CFGZ::CFGZ_ENABLE == _cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_AUTO_LOAD_TRANSFER)))

#define MAINS_MONITORING_ENABLED()              (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE)
#define IS_MAINS_HEALTHY()                      (MAINS_MONITORING_ENABLED() && (_MainsStatus == MAINS_HELATHY))
#define IS_REMORE_SS_ENABLED()                  ((!MAINS_MONITORING_ENABLED()) && (_bRemoteStopRCVD))

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

BASE_MODES::GCU_STATE_t BASE_MODES::GetGCUState()
{
  return _vars.GCUState;
}

BASE_MODES::TIMER_STATE_t BASE_MODES::GetTimerState()
{
  return _vars.TimerState;
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
    //Todo: In NXP GC2111 for BTS and cyclic mode when mains mon is disable
    //Mains is considered unhealthy. And in auto mode it is considered healthy
    //for the same condition.
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE)
    {
        if((_eOperatingMode == BTS_MODE) || (_eOperatingMode == CYCLIC_MODE))
        {
           _MainsStatus = MAINS_UNHELATHY;
        }
        else
        {
            _MainsStatus = MAINS_HELATHY;
        }
    }
    else if(_hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_SIMULATE_MAINS) ==  DigitalSensor::SENSOR_LATCHED)
    {
        _MainsStatus = MAINS_HELATHY;
    }
    else
    {
        if(_MainsStatus == MAINS_UNHELATHY)
        {
            _MainsStatus = (MAINS_STATUS_t) ((!_GCUAlarms.AlarmResultLatched(GCU_ALARMS::MAINS_UNDERVOLT_TRIP))
                                  && (!_GCUAlarms.AlarmResultLatched(GCU_ALARMS::MAINS_OVERVOLT_TRIP))
                                  && (!_GCUAlarms.AlarmResultLatched(GCU_ALARMS::MAINS_UNDERFREQ_TRIP))
                                  && (!_GCUAlarms.AlarmResultLatched(GCU_ALARMS::MAINS_OVERFREQ_TRIP))
                                  && (!_GCUAlarms.AlarmResultLatched(GCU_ALARMS::MAINS_LL_UNDERVOLT_TRIP))
                                  && ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_PHASE_REVERSAL_DETECT) == CFGZ::CFGZ_DISABLE) ||
                                          ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_PHASE_REVERSAL_DETECT) == CFGZ::CFGZ_ENABLE) && (!_hal.AcSensors.MAINS_GetPhaseRotStatus()))));
        }
        else
        {
            _MainsStatus = (MAINS_STATUS_t) !(
                                        ( _GCUAlarms.AlarmResultLatched(GCU_ALARMS::MAINS_UNDERVOLT_TRIP))
                                    || (_GCUAlarms.AlarmResultLatched(GCU_ALARMS::MAINS_OVERVOLT_TRIP))
                                    || ( _GCUAlarms.AlarmResultLatched(GCU_ALARMS::MAINS_UNDERFREQ_TRIP))
                                    || ( _GCUAlarms.AlarmResultLatched(GCU_ALARMS::MAINS_OVERFREQ_TRIP))
                                    || (_GCUAlarms.AlarmResultLatched(GCU_ALARMS::MAINS_LL_UNDERVOLT_TRIP))
                                    || ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_PHASE_REVERSAL_DETECT) == CFGZ::CFGZ_ENABLE) && (_hal.AcSensors.MAINS_GetPhaseRotStatus())));
        }
    }
}

void BASE_MODES::prvUpdateContactorOutputs()
{
    if(((_bSwitchLoadToMains)
            ||((_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_CLOSE_MAINS_OPEN_GEN_SWITCH) ==  DigitalSensor::SENSOR_LATCHED) && (_MainsStatus == MAINS_HELATHY) && (_eOperatingMode == BASE_MODES::MANUAL_MODE)))
            && (!_bContactorTransferOn))
    {
        _bSwitchLoadToMains = false;
        UTILS_ResetTimer(&_ContactorTransferTimer);
        _bContactorTransferOn = true;
        _bContTransferToMainsOn = true;
        // make gen contactor output inactive
        _bCloseGenContactor = false;
    }

    if(((_bSwitchLoadToGen)
            || ((_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_CLOSE_GEN_OPEN_MAINS_SWITCH) ==  DigitalSensor::SENSOR_LATCHED) && _EngineMon.IsWarmUpTimeExpired() && (_eOperatingMode == BASE_MODES::MANUAL_MODE)))
            && (!_bContactorTransferOn)
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
            _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_LOAD_TRANSFER_DELAY))
    {
        _bContactorTransferOn = false;
        UTILS_DisableTimer(&_ContactorTransferTimer);

        if(_bContTransferToMainsOn)
        {
            _bContTransferToMainsOn = false;
            // make mains contactor out active
            _bCloseMainsContactor = true;
        }
        else if(_bContTransferToGenOn)
        {
            _bContTransferToGenOn = false;
            // make gen contactor active
            _bCloseGenContactor = true;
        }
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
        if( *bContTransferToLoadOn)
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
BASE_MODES::BTS_STATE_t  BASE_MODES::GetBTSModeState()
{
    return _eBTSState;
}
BASE_MODES::CYCLIC_STATE_t  BASE_MODES::GetCyclicModeState()
{
    return _eCyclicState;
}

/* Shubham Wader: making supportive macros for better understanding of the logic */
#define IS_BTS_MODE_CONFIG_ENABLED()          ( (_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON) == CFGZ::CFGZ_ENABLE))

#define IS_CYCLIC_MODE_CONFIG_ENABLED()        (_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_CYCLIC_MODE) == CFGZ::CFGZ_ENABLE)

#define SET_GCU_OPERATING_MODE(mode_)          (_eOperatingMode = mode_)
#define SET_BTS_MODE_STATE(state_)             (_eBTSState = state_)
#define SET_CYCLIC_MODE_STATE(state_)          (_eCyclicState = state_)
#define SET_AUTO_MODE_STATE(state_)            (_eAutoState = state_)

void BASE_MODES::AssignModechangeParameters()
{
/* Shubham Wader 23.09.2022
Below mode and state related assignments done by referencing the GC2111 NXP code.
*/
    switch(_eManualState)
    {
        case STATE_MANUAL_GEN_OFF:
        {
            if(IS_BTS_MODE_CONFIG_ENABLED())
            {
                SET_GCU_OPERATING_MODE(BTS_MODE);
                SET_BTS_MODE_STATE(STATE_BTS_GEN_OFF_MAINS_ON);
            }
            else if(IS_CYCLIC_MODE_CONFIG_ENABLED())
            {
                SET_GCU_OPERATING_MODE(CYCLIC_MODE);
                SET_CYCLIC_MODE_STATE(STATE_CYCLIC_GEN_OFF_MAINS_ON);
            }
            else
            {
                SET_GCU_OPERATING_MODE(AUTO_MODE);
                SET_AUTO_MODE_STATE(STATE_AMF_GEN_OFF_MAINS_ON);
            }
        }
        break;

        case STATE_MANUAL_GEN_START:
        {
            if(IS_BTS_MODE_CONFIG_ENABLED())
            {
                SET_GCU_OPERATING_MODE(BTS_MODE);
                SET_BTS_MODE_STATE(STATE_BTS_GEN_START);
            }
            else if(IS_CYCLIC_MODE_CONFIG_ENABLED())
            {
                SET_GCU_OPERATING_MODE(CYCLIC_MODE);
                SET_CYCLIC_MODE_STATE(STATE_CYCLIC_GEN_START);
            }
            else
            {
                SET_GCU_OPERATING_MODE(AUTO_MODE);
                SET_AUTO_MODE_STATE(STATE_AMF_GEN_START);
            }
        }
        break;

        case STATE_MANUAL_GEN_READY:
        {
            if(IS_BTS_MODE_CONFIG_ENABLED())
            {
                SET_GCU_OPERATING_MODE(BTS_MODE);
                SET_BTS_MODE_STATE(STATE_BTS_GEN_ON_LOAD);
            }
            else if(IS_CYCLIC_MODE_CONFIG_ENABLED())
            {
                SET_GCU_OPERATING_MODE(CYCLIC_MODE);
                SET_CYCLIC_MODE_STATE(STATE_CYCLIC_GEN_ON_LOAD);
            }
            else
            {
                SET_GCU_OPERATING_MODE(AUTO_MODE);
                SET_AUTO_MODE_STATE(STATE_AMF_GEN_ON_LOAD);
            }
        }
        break;

        case STATE_MANUAL_ENGINE_COOLING:
        {
            if(IS_BTS_MODE_CONFIG_ENABLED())
            {
                SET_GCU_OPERATING_MODE(BTS_MODE);
                SET_BTS_MODE_STATE(STATE_BTS_ENGINE_COOLING);
            }
            else if(IS_CYCLIC_MODE_CONFIG_ENABLED())
            {
                SET_GCU_OPERATING_MODE(CYCLIC_MODE);
                SET_CYCLIC_MODE_STATE(STATE_CYCLIC_ENGINE_COOLING);
            }
            else
            {
                SET_GCU_OPERATING_MODE(AUTO_MODE);
                SET_AUTO_MODE_STATE(STATE_AMF_ENGINE_COOLING);
            }
        }
        break;

        case STATE_MANUAL_ENGINE_STOP:
        {
            if(IS_BTS_MODE_CONFIG_ENABLED())
            {
                SET_GCU_OPERATING_MODE(BTS_MODE);
                SET_BTS_MODE_STATE(STATE_BTS_ENGINE_STOP);
            }
            else if(IS_CYCLIC_MODE_CONFIG_ENABLED())
            {
                SET_GCU_OPERATING_MODE(CYCLIC_MODE);
                SET_CYCLIC_MODE_STATE(STATE_CYCLIC_ENGINE_STOP);
            }
            else
            {
                SET_GCU_OPERATING_MODE(AUTO_MODE);
                SET_AUTO_MODE_STATE(STATE_AMF_ENGINE_STOP);
            }
        }
        break;

        default:
        {
            SET_GCU_OPERATING_MODE(MANUAL_MODE);
        }
        break;
    }
    /* todo: need to test the whole functionality with below function call. */
    _GCUAlarms.ResetMainsMonParams();
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
    if((_eOperatingMode == AUTO_MODE)
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
    if((_bRemoteStartRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE))
        || ((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
//        || _bMBStartCmdReceived
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

bool BASE_MODES::GetPressureSensorStatusBeforeStart()
{
    A_SENSE::SENSOR_RET_t stVal;
    stVal = _GCUAlarms.GetLOPSensorVal();

    if((_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::OIL_PRESS_MON].bResultInstant
               &&( stVal.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT))
        ||_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::LLOP_MON].bResultInstant
        )
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

void BASE_MODES::SetModeState(BTS_STATE_t eState)
{
    _eBTSState=eState;
}

void BASE_MODES::SetModeState(CYCLIC_STATE_t eState)
{
    _eCyclicState=eState;
}

void BASE_MODES::InitNightModeParam()
{
    uint16_t u16RequiredHours = 0, u16RequiredMins = 0, u16IncrementReqHours = 0;

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFG_NIGHT_MODE) == CFGZ::CFGZ_ENABLE)
    {
        _u16NightModeStartTime = _cfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFG_START_TIME);

        _u16NightModeDurationHrsMin = (uint16_t)(((_cfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFG_OFF_DURATION))/60)*100)
                + (uint16_t)((_cfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFG_OFF_DURATION))%60);

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
        u16RequiredHours = (uint16_t)((_u16NightModeStartTime/100 ) + (_u16NightModeDurationHrsMin/100) + u16IncrementReqHours);
        if(u16RequiredHours >= 24)
        {
            u16RequiredHours = u16RequiredHours - 24;
        }
        _u16NightModeStopTime = (uint16_t)((u16RequiredHours*100) + u16RequiredMins);
    }
    else
    {
        _bNightModeRestrict = false;
    }

}

void BASE_MODES::prvUpdateNightModeRestrictStatus()
{
      RTC::TIME_t stCurrentTime;

      if(_cfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFG_NIGHT_MODE) == CFGZ::CFGZ_ENABLE)
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

bool BASE_MODES::EngineNotInCoolingStage()
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

        default:
            break;

    }
    return bEngineNotInCoolingStage;
}


