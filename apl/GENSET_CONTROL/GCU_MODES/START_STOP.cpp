/**
 * @file        [START_STOP]
 * @brief       START_STOP
 *              Provides common API's for functionality offered by start stop.
 *              state machine. The module is implemented in class START_STOP
 *              which is inherited from class BASE_MODES.
 *
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        1st June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#include "START_STOP.h"
#include "../EGOV/EGOV.h"
#include "J1939_APP/J1939APP.h"
bool START_STOP::_bChargAltStopLatched = false;
bool START_STOP::_bMonitorChargAlt = false;
bool START_STOP::_bOPStopSolenoid = false;
bool START_STOP::_bOPStartRelay = false;
bool START_STOP::_bMonitorDGIdleRun = false;
uint16_t START_STOP::_u16ConfiguredSafetyMonDelay = false;
stTimer START_STOP::_SafetyMonTimer = {0};
stTimer START_STOP::_EngStoppingTimer = {0};
bool START_STOP::_bOPPreheat = false;
bool START_STOP::_bOPFuelRelay= false;

bool START_STOP::_bStopCommand = false;
START_STOP::SS_STATE_t START_STOP::_State =ID_STATE_SS_ENG_OFF_OK;
extern J1939APP *gpJ1939;
START_STOP::START_STOP(HAL_Manager &hal, ENGINE_MONITORING &EngineMon, CFGZ &cfgz,  
                        GCU_ALARMS &GCUAlarms, BASE_MODES::GCU_MODE_VARS_t &vars, CHARGING_ALT &ChargeAlt, ENGINE_START_VALIDITY  &EngineStartValidity):
_hal(hal),
_EngineMon(EngineMon),
_cfgz(cfgz),
_GCUAlarms(GCUAlarms),
_vars(vars),
_ChargeAlt(ChargeAlt),
_EngineStartValidity(EngineStartValidity),
_bStartKeyPressed(false),
_bStopKeyPressed(false),
_u8NoOfCrankAttempts(0),
_bGenStarted(false),
_bStartCommand(false),
_bEmergencyStopLatched(false),
_bEmergencyStop(false),
_bSimStartReceived(false),
_bSimStopReceived(false),
_bSimAutoReceived(false),
_bSimAutoLatched(false),
_bSimStartLatched(false),
_bSimStopLatched(false),
_bAlarmMutePressed(false),
_bAlarmMuteReleased(false),
_bAlarmAckPressed(false),
_bAlarmAckReleased(false),
_bAckAudblAlrmRecd(false),
_bSimAckRecd(false),
_bDGIdleRunDelayRunning(false),
_PreheatTimer{0},
_EngStartTimer{0},
_EngCrankingTimer{0},
_EngCrankRestTimer{0},
_StartStopSMUpdateTimer{0},
_StopHoldTimer{0},
_PowerOnTimer{0},
_DGIDleRunTimer{0}
{
    UTILS_ResetTimer(&_PowerOnTimer);
    _u16ConfiguredSafetyMonDelay = _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SAFETY_MONITOR_DELAY);
}


void START_STOP::Init()
{
    UTILS_ResetTimer(&_StartStopSMUpdateTimer);
    _u16ConfiguredSafetyMonDelay = _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SAFETY_MONITOR_DELAY);
}

void START_STOP::Update(bool bDeviceInConfigMode)
{
    A_SENSE::SENSOR_RET_t   stEngTemp = {0};
    volatile bool                    bPreheatTempLimitReached;
    if(UTILS_GetElapsedTimeInSec(&_PowerOnTimer) >= FOUR_SEC)
    {
        UTILS_ResetTimer(&_StartStopSMUpdateTimer);
        UTILS_DisableTimer(&_PowerOnTimer);
    }

    if(UTILS_GetElapsedTimeInMs(&_StartStopSMUpdateTimer) >= FIFTY_MSEC)
    {
        prvReqStartDGIdleRunTimer();
        prvUpdateDGIDLERunStatus();
        prvUpdateSimStartStopStatus();
        UTILS_ResetTimer(&_StartStopSMUpdateTimer);
        stEngTemp = _hal.AnalogSensors.GetSensorValue(
                        AnalogSensor::A_SENSE_ENG_TEMPERATURE);
        if(_hal.DigitalSensors.GetDigitalSensorState(
                DigitalSensor::DI_EMERGENCY_STOP) == DigitalSensor::SENSOR_LATCHED)
        {
            _bEmergencyStop = true;
        }
        else
        {
            _bEmergencyStop = false;
        }

        if(!_bEmergencyStop)
        {
            _bEmergencyStopLatched = 0;
        }

        bPreheatTempLimitReached = CheckPreheatTempCondition();

        switch(_State)
        {
            case ID_STATE_SS_ENG_OFF_OK:
                prvSetOutputVariables(false, false, false, false);
                _bMonitorDGIdleRun = false;
                _bDGIdleRunDelayRunning = false;
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_WAVE_DETECTION) == CFGZ::CFGZ_ENABLE)
                {
                    _EngineStartValidity.InitInvalidDgDetectionStateMachine();
                    ENGINE_START_VALIDITY::SetEngineStartInvalidity(false);
                }

                if(_EngineMon.IsEngineOn() == 1U)
                {
                    prvEngineOnGenStartAction();
                }

                if((_bStopCommand) || (_bEmergencyStop &&
                        !_bEmergencyStopLatched))
                {
                    _bEmergencyStopLatched = true;
                    bPreheatTempLimitReached = false;
                    prvStopCommandAction();
                }
                else if(_bStartCommand)
                {
                    _bStartCommand = false;
                    _u8NoOfCrankAttempts = 0;
                    _GCUAlarms.LogEvent(GCU_ALARMS::Engine_Start_id, GCU_ALARMS::ID_NONE);
                    _bOPPreheat = false;
                    bPreheatTempLimitReached = false;
                    UTILS_DisableTimer(&_PreheatTimer);
                    UTILS_ResetTimer(&_EngStartTimer);
                    _State = ID_STATE_SS_START_WAIT;
                    _vars.TimerState = BASE_MODES::START_DELAY_TIMER;
                    _vars.GCUState = BASE_MODES::ENGINE_STARTING;
                }
                break;

            case ID_STATE_SS_ENG_OFF_ERR:
                prvSetOutputVariables(false, false, false, false);
                _State = ID_STATE_SS_ENG_OFF_OK;
                break;

            case ID_STATE_SS_PREHEAT:
                prvSetOutputVariables(false, false, false, true);
                _bStartCommand = false;

                if(_EngineMon.IsEngineOn() == 1U)
                {
                    UTILS_DisableTimer(&_PreheatTimer);
                    prvEngineOnGenStartAction();
                }
                else if((_bStopCommand)|| (_GCUAlarms.IsCommonWarning()))
                {
                    prvStopCommandAction();
                    UTILS_DisableTimer(&_PreheatTimer);
                    bPreheatTempLimitReached = false;
                }
                else if(((UTILS_GetElapsedTimeInSec(&_PreheatTimer)) >= 
                        _cfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_PREHEAT_TIMER)) ||
                        bPreheatTempLimitReached)
                {
                    bPreheatTempLimitReached = false;
                    prvTurnOffPreheatStartCranking();
                }
                break;

            case ID_STATE_SS_START_WAIT:
                prvSetOutputVariables(false, false, false, false);
                _bStartCommand = false;

                if(_EngineMon.IsEngineOn() == 1U)
                {
                    UTILS_DisableTimer(&_EngStartTimer);
                    prvEngineOnGenStartAction();
                }
                else if((_bStopCommand) || (_GCUAlarms.IsCommonWarning()))
                {
                   prvStopCommandAction();
                }
                else if((UTILS_GetElapsedTimeInSec(&_EngStartTimer)) >=
                        GetCrankStartDelay())
                {
                    UTILS_DisableTimer(&_EngStartTimer);

                    if((!bPreheatTempLimitReached) && IS_PRHEAT_RELAY_CONFIGURED())
                    {
                        _bOPPreheat = true;
                        bPreheatTempLimitReached = false;
                        UTILS_ResetTimer(&_PreheatTimer);
                        _State = ID_STATE_SS_PREHEAT;
                        _vars.GCUState = BASE_MODES::ENGINE_STARTING;
                        _vars.TimerState = BASE_MODES::PREHEAT_TIMER;
                    }
                    else
                    {
                        prvSkipPreheatStartCranking();
                    }
                }
                break;

            case ID_STATE_SS_CRANKING:
                prvSetOutputVariables(true, false, true, false);
                _bStartCommand = false;
                if((_bStopCommand)||(_GCUAlarms.IsCommonWarning()))
                {
                    UTILS_DisableTimer(&_EngCrankingTimer);
                    prvStopCommandAction();
                }
                else if(_EngineMon.IsEngineCranked())
                {
                    UTILS_DisableTimer(&_EngCrankingTimer);
                    prvHandleEngineCranked();
                    _hal.ObjGlcd.TurnOnBackLight();
                }
                else if((UTILS_GetElapsedTimeInSec(&_EngCrankingTimer)) >= 
                        _cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_HOLD_TIME))
                {
                    if(_u8NoOfCrankAttempts >= _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_START_ATTEMPTS))
                    {
                        _hal.ObjGlcd.TurnOnBackLight();
                        _GCUAlarms.UpdateFailToStart();
                        _State = ID_STATE_SS_ENG_OFF_ERR;
                        _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;
                    }
                    else
                    {
                        _hal.ObjGlcd.TurnOnBackLight();
                        UTILS_ResetTimer(&_EngCrankRestTimer);
                        _State = ID_STATE_SS_CRANK_REST;
                        _vars.GCUState = BASE_MODES::ENGINE_STARTING;
                        _vars.TimerState = BASE_MODES::CRANK_REST_TIMER;
                    }
                    _ChargeAlt.StopExcitation();
                    UTILS_DisableTimer(&_EngCrankingTimer);
                }
                else if((!_bChargAltStopLatched) &&
                        (1 >=(_cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_HOLD_TIME) -
                                UTILS_GetElapsedTimeInSec(&_EngCrankingTimer))))
                {
                    /*Todo: Need to check the correctness of above if condition.
                     * In SGC420 the Stop excitation pulse is applied after initial 2secs \
                     * however in GC2111 it is applied 1sec before cranking timer expires.
                     */
                    _ChargeAlt.StopExcitation();
                    _bChargAltStopLatched = true;
                }
                break;

            case ID_STATE_SS_CRANK_REST:
                _hal.ObjGlcd.TurnOnBackLight();
                prvSetOutputVariables(false, false, false, false);
                _bStartCommand = false;
                if((_bStopCommand)||(_GCUAlarms.IsCommonWarning()))
                {
                    UTILS_DisableTimer(&_EngCrankRestTimer);
                    prvStopCommandAction();
                }
                else if(_EngineMon.IsEngineCranked())
                {
                    UTILS_DisableTimer(&_EngCrankRestTimer);
                    prvHandleEngineCranked();
                }
                else if((UTILS_GetElapsedTimeInSec(&_EngCrankRestTimer)) >= 
                        _cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_REST_TIME))
                {
                    UTILS_DisableTimer(&_EngCrankRestTimer);
                    UTILS_ResetTimer(&_EngCrankingTimer);
                    _State = ID_STATE_SS_CRANKING;
                    _u8NoOfCrankAttempts++;
                    _ChargeAlt.StartExcitation();
                    _bChargAltStopLatched = false;
                    _vars.GCUState = BASE_MODES::ENGINE_STARTING;
                    _vars.TimerState = BASE_MODES::CRANK_START_TIMER;
                }

                //Do not monitor Chg Alt Volt for first 1 sec
                //(1 Count == 50ms) so comparing with 20 counts to count 1 sec
                if(1U <= UTILS_GetElapsedTimeInSec(&_EngCrankRestTimer))
                {
                    _bMonitorChargAlt = true;
                }
                else
                {
                    _bMonitorChargAlt = false;
                }
                break;

            case ID_STATE_SS_ENG_ON:
                prvSetOutputVariables(false, false, true, false);
                _bStartCommand = false;

                if(((UTILS_GetElapsedTimeInSec(&_SafetyMonTimer)) >= 
                        _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SAFETY_MONITOR_DELAY))
                        && (_vars.TimerState == BASE_MODES::SAFETY_MON_TIMER))
                {
                    _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;
                }
                if(_bStopCommand)
                {
                    prvStopCommandAction();
                }
                break;

            case ID_STATE_SS_STOPPING:
                _bStopCommand = false;
                prvSetOutputVariables(false, true, false, false);
                UTILS_DisableTimer(&_SafetyMonTimer);
                UTILS_DisableTimer(&_DGIDleRunTimer);
                _EngineMon.DisableEngWarmUpTimer();

                if(_EngineMon.IsEngineOn() == 0U)
                {
                    UTILS_DisableTimer(&_EngStoppingTimer);
                    UTILS_ResetTimer(&_StopHoldTimer);
                    _State = ID_STATE_SS_STOP_HOLD;
                    _vars.GCUState = BASE_MODES::ENGINE_STOPPING;
                    _vars.TimerState = BASE_MODES::ADDTIONAL_STOP_TIMER;
                }
                else if((UTILS_GetElapsedTimeInSec(&_EngStoppingTimer)) >=
                        _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_STOP_ACTION_TIME))
                {
                    _bStartCommand = false;
                    UTILS_DisableTimer(&_EngStoppingTimer);
                    _vars.GCUState = BASE_MODES::ENGINE_STOPPING;
                    _State = ID_STATE_SS_FAIL_TO_STOP;
                    _GCUAlarms.UpdateFailToStop();
                    _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;
                }
                break;

            case ID_STATE_SS_STOP_HOLD:
                _bStopCommand = false;
                prvSetOutputVariables(false, true, false, false);
                if(_bEmergencyStop)
                {
                    _bEmergencyStopLatched = true;
                }

                if(_EngineMon.IsEngineOn() == 1U)
                {
                    UTILS_DisableTimer(&_StopHoldTimer);
                    prvEngineOnGenStartAction();
                }
                else if((UTILS_GetElapsedTimeInSec(&_StopHoldTimer)) >=
                        _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ADDN_STOPPING_TIME))
                {

                    UTILS_DisableTimer(&_StopHoldTimer);
                    _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;
                    _State = ID_STATE_SS_ENG_OFF_OK;
                }
                break;

            case ID_STATE_SS_FAIL_TO_STOP:
                prvSetOutputVariables(false, false, false, false);
                _bStartCommand = false;
                if(_bStopCommand || (_bEmergencyStop &&
                        (!_bEmergencyStopLatched)))
                {
                    _bEmergencyStopLatched = true;
                    prvStopCommandAction();
                }
                if(!(_EngineMon.IsEngineOn()))
                {
                    _State = ID_STATE_SS_ENG_OFF_ERR;
                    UTILS_DisableTimer(&_StopHoldTimer);
                }
                break;

            default:
                break;
        }
        if(bPreheatTempLimitReached && _bOPPreheat)
        {
            _bOPPreheat = false;
        }
        prvTurnOnOffOutputs();
        _EngineMon.UpdateStartStopState((uint8_t)_State);
    }
}

START_STOP::SS_STATE_t START_STOP::GetStartStopSMDState()
{
	return _State;
}

uint8_t START_STOP::GetCrankAttemptNumber()
{
    return _u8NoOfCrankAttempts;
}

bool START_STOP::IsGenStarted()
{
    return _bGenStarted;
}

void START_STOP::prvEngineOnGenStartAction()
{
    if(_State == ID_STATE_SS_ENG_OFF_OK)
    {
        /* Following statements are commented since in case of tamper proofing logic enabled
         * we should increase the run hours thought the key is pressed from panel
         */
        //                        if(1 == RAM_Calib_Data[ID_ALT_WAVE_DETECTION_EN][ID_CALIB_COL_VAL])
        //                        {
        //                            f_Invalid_DG_Start = 1;
        //                        }
        //                        else
        {
            _GCUAlarms.LogEvent(GCU_ALARMS::Engine_Start_id, GCU_ALARMS::ID_NONE);
        }
    }

    _bGenStarted = true;
    _ChargeAlt.StartExcitation();
    UTILS_ResetTimer(&_SafetyMonTimer);
    _State = ID_STATE_SS_ENG_ON;
    _vars.TimerState = BASE_MODES::SAFETY_MON_TIMER;
    _vars.GCUState = BASE_MODES::ENGINE_ON_HEALTHY;
}
void START_STOP::prvStopCommandAction()
{
    _bStopCommand = false;
    _bGenStarted = false;
    _ChargeAlt.StopExcitation();
    _hal.ObjGlcd.TurnOnBackLight();
    switch(_State)
    {
        case ID_STATE_SS_ENG_OFF_OK:
            UTILS_DisableTimer(&_SafetyMonTimer);
            _bStartCommand = false;
            UTILS_ResetTimer(&_EngStoppingTimer);
            _State = ID_STATE_SS_STOPPING;
            _bOPPreheat = false;
            UTILS_DisableTimer(&_PreheatTimer);
            _vars.GCUState = BASE_MODES::ENGINE_STOPPING;
            _vars.TimerState = BASE_MODES::STOP_ACTION_TIMER;
            break;

        case ID_STATE_SS_START_WAIT:
            UTILS_DisableTimer(&_EngStartTimer);
            _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;
            _vars.GCUState = BASE_MODES::ENGINE_OFF_READY;
            _State = ID_STATE_SS_ENG_OFF_OK;
            break;

        case ID_STATE_SS_PREHEAT:
            _State = ID_STATE_SS_ENG_OFF_OK;
            _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;
            break;

        case ID_STATE_SS_ENG_ON:
            _EngineMon.DisableGenWarmUpTimer();
            _EngineMon.StoreCummulativeCnt();
            _GCUAlarms.UpdateFuelTheftCalculation();
            /* no break*/
        case ID_STATE_SS_CRANKING:
        case ID_STATE_SS_CRANK_REST:
        case ID_STATE_SS_FAIL_TO_STOP:
            UTILS_DisableTimer(&_SafetyMonTimer);
            UTILS_ResetTimer(&_EngStoppingTimer);
            _State = ID_STATE_SS_STOPPING;
           _bChargAltStopLatched = false;
            _vars.GCUState = BASE_MODES::ENGINE_STOPPING;
            _vars.TimerState = BASE_MODES::STOP_ACTION_TIMER;
            break;

        default :
            break;
    }

}

void START_STOP::prvSetOutputVariables(bool bStartRelayStatus, bool bStopSolenoidStatus, 
                                        bool bFuelRelayStatus, bool bPreheatStatus)
{
    _bOPStartRelay = bStartRelayStatus;
    _bOPStopSolenoid = bStopSolenoidStatus;
    _bOPFuelRelay = bFuelRelayStatus;
    _bOPPreheat = bPreheatStatus;
}

void START_STOP::prvTurnOnOffOutputs()
{
    if(_bOPStartRelay)
    {
       _hal.actuators.Activate(ACTUATOR::ACT_START_RELAY);
    }
    else
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_START_RELAY);
    }

    if(_bOPStopSolenoid)
    {
        _hal.actuators.Activate(ACTUATOR::ACT_STOP_SOLENOID);
    }
    else
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_STOP_SOLENOID);
    }

    if(_bOPFuelRelay)
    {
        _hal.actuators.Activate(ACTUATOR::ACT_FUEL_RELAY);
    }
    else
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_FUEL_RELAY);
    }

    if(_bOPPreheat)
    {
        _hal.actuators.Activate(ACTUATOR::ACT_PREHEAT);
    }
    else
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_PREHEAT);
    }
}

void START_STOP::StartCommand()
{
  _bStartCommand = true;
}

bool START_STOP::IsStartCommand()
{
  return _bStartCommand;
}

void START_STOP::StopCommand()
{
  _bStopCommand = true;
}


bool START_STOP::IsStopCommand()
{
  return _bStopCommand ;
}

void START_STOP::StartKeyPressed()
{
    _bStartKeyPressed = true;
}
void START_STOP::StopKeyPressed()
{
    _bStopKeyPressed = true;
}

bool START_STOP::IsEngStoppingTimerEnabled()
{
    return _EngStoppingTimer.bEnabled;
}

bool START_STOP::IsFuelRelayOn()
{
    return _bOPFuelRelay;
}

uint32_t START_STOP::GetTimersRemainingTime(BASE_MODES::TIMER_STATE_t eTimer)
{
    uint32_t RemainingTimeInSec = 0;
    switch(eTimer)
    {
        case BASE_MODES::PREHEAT_TIMER:
            RemainingTimeInSec = (_cfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_PREHEAT_TIMER) -
                                    UTILS_GetElapsedTimeInSec(&_PreheatTimer));
            break;

        case BASE_MODES::START_DELAY_TIMER:
                RemainingTimeInSec = (GetCrankStartDelay() -
                                                    UTILS_GetElapsedTimeInSec(&_EngStartTimer));
            break;

        case BASE_MODES::CRANK_START_TIMER:
            RemainingTimeInSec = (_cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_HOLD_TIME) -
                                    UTILS_GetElapsedTimeInSec(&_EngCrankingTimer));
            break;

        case BASE_MODES::CRANK_REST_TIMER:
            RemainingTimeInSec = (_cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_REST_TIME) -
                                    UTILS_GetElapsedTimeInSec(&_EngCrankRestTimer));
            break;

        case BASE_MODES::STOP_ACTION_TIMER:
            RemainingTimeInSec = (_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_STOP_ACTION_TIME) -
                                    UTILS_GetElapsedTimeInSec(&_EngStoppingTimer));
            break;

        case BASE_MODES::ADDTIONAL_STOP_TIMER:
            RemainingTimeInSec = (_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ADDN_STOPPING_TIME) -
                                    UTILS_GetElapsedTimeInSec(&_StopHoldTimer));
            break;

        case BASE_MODES::SAFETY_MON_TIMER:
            RemainingTimeInSec = (_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SAFETY_MONITOR_DELAY) -
                                    UTILS_GetElapsedTimeInSec(&_SafetyMonTimer));
            break;

        default:
            break;
    }
    return RemainingTimeInSec;
}


uint8_t START_STOP::IsGenMonOn()
{
    if(UTILS_GetElapsedTimeInSec(&_SafetyMonTimer) >= _u16ConfiguredSafetyMonDelay)
    {
        return 1u;
    }
    else
    {
        return 0;
    }
}

void START_STOP::prvHandleEngineCranked()
{
    _bGenStarted = true;
    _ChargeAlt.StartExcitation();
    UTILS_ResetTimer(&_SafetyMonTimer);
    _State = ID_STATE_SS_ENG_ON;
    _vars.GCUState = BASE_MODES::ENGINE_ON_HEALTHY;
    _vars.TimerState = BASE_MODES::SAFETY_MON_TIMER;
}


bool START_STOP::IsChargAltStopLtached()
{
    return _bChargAltStopLatched;
}

bool START_STOP::IsMonitorChargAltTrue()
{
    return _bMonitorChargAlt;
}

void START_STOP::ClearChargAltStopLtached()
{
    _bChargAltStopLatched = false;
}

bool START_STOP::IsTimerRunning()
{
     return (_EngStartTimer.bEnabled || _EngCrankingTimer.bEnabled ||
             _EngCrankRestTimer.bEnabled || _EngStoppingTimer.bEnabled );
}

bool START_STOP::IsAdditionalStopTimerRunnint()
{
    return _StopHoldTimer.bEnabled;
}


/**************************************************************************************************
@Name           -   StartDGIDLERunDelay
@brief          -   This function starts the DG IDLE Run delay timer only if Safety monitoring
                    and warm up delay timers are over.
@param          -   None
@return         -   None
**************************************************************************************************/
void START_STOP::prvReqStartDGIdleRunTimer()
{
    if((!_bDGIdleRunDelayRunning)&& (IsGenMonOn()) && (_EngineMon.IsWarmUpTimeExpired()))
    {
        _bDGIdleRunDelayRunning = true;
        UTILS_ResetTimer(&_DGIDleRunTimer);
    }
}

/**************************************************************************************************
@Name           -   UpdateDGIDLERunStatus
@brief          -   This function tells when to start updating the DG IDLE Run status bit.
                    True(start updating the DG IDLE Run status)
@param          -   None
@return         -   None
**************************************************************************************************/
void START_STOP::prvUpdateDGIDLERunStatus(void)
{
    if((UTILS_GetElapsedTimeInSec(&_DGIDleRunTimer) >= DG_IDLE_RUN_DELAY_IN_SEC) && _bDGIdleRunDelayRunning)
    {
        _bMonitorDGIdleRun = true;
    }
    else
    {
        _bMonitorDGIdleRun = false;
    }
}

bool START_STOP::IsMonitorDGIdleRunTrue()
{
    return _bMonitorDGIdleRun;
}
void START_STOP::prvUpdateSimStartStopStatus()
{
    if(!_bSimStartLatched)
    {
        if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_START)
                == DigitalSensor::SENSOR_LATCHED)
        {
            _bSimStartLatched = true;
            _bSimStartReceived = true;
        }
    }
    else
    {
        if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_START)
                == DigitalSensor::SENSOR_UNLATCHED)
        {
            _bSimStartLatched = false;
        }
    }

    if(!_bSimStopLatched)
    {
        if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_STOP)
                == DigitalSensor::SENSOR_LATCHED)
        {
            _bSimStopLatched = true;
            _bSimStopReceived = true;
        }
    }
    else
    {
        if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_STOP)
                == DigitalSensor::SENSOR_UNLATCHED)
        {
            _bSimStopLatched = false;
        }
    }

    if(!_bSimAutoLatched)
    {
        if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_AUTO)
                == DigitalSensor::SENSOR_LATCHED)
        {
            _bSimAutoLatched = true;
            _bSimAutoReceived = true;
        }
    }
    else
    {
        if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_AUTO)
                == DigitalSensor::SENSOR_UNLATCHED)
        {
            _bSimAutoLatched = false;
        }
    }

}

bool START_STOP::IsSimAutoReceived()
{
    return _bSimAutoReceived;
}

bool START_STOP::IsSimStartReceived()
{
    return _bSimStartReceived;
}

bool START_STOP::IsSimStopReceived()
{
    return _bSimStopReceived;
}

void START_STOP::ClearSimAutoPulse()
{
    _bSimAutoReceived = false;
}

void START_STOP::ClearSimStopPulse()
{
    _bSimStopReceived = false;
}

void START_STOP::ClearSimStartPulse()
{
    _bSimStartReceived = false;
}

void START_STOP::ClearPreheatOutput()
{
    _bOPPreheat = false;
}

bool START_STOP::IsStopRelayON()
{
    return _bOPStopSolenoid;
}

bool START_STOP::IsStartRelayON()
{
    return _bOPStartRelay;
}

bool START_STOP::IsStartPreheatON()
{
    return _bOPPreheat;
}

void START_STOP::UpdateGcuStatusAndTimerDisplay(BASE_MODES::GCU_STATE_t eGcuState, BASE_MODES::TIMER_STATE_t eTimerDisplay)
{
    _vars.GCUState = eGcuState;
    _vars.TimerState = eTimerDisplay;
}

void START_STOP::prvTurnOffPreheatStartCranking()
{
    _bOPPreheat = false;
    UTILS_DisableTimer(&_PreheatTimer);
    UTILS_ResetTimer(&_EngCrankingTimer);
    _State = ID_STATE_SS_CRANKING;
    _u8NoOfCrankAttempts++;
    _ChargeAlt.StartExcitation();
    _bChargAltStopLatched = false;
    _vars.GCUState = BASE_MODES::ENGINE_STARTING;
    _vars.TimerState = BASE_MODES::CRANK_START_TIMER;
}

void START_STOP::prvSkipPreheatStartCranking()
{
    UTILS_ResetTimer(&_EngCrankingTimer);
    _State = ID_STATE_SS_CRANKING;
    _u8NoOfCrankAttempts++;
    _ChargeAlt.StartExcitation();
    _bChargAltStopLatched = false;
    _vars.GCUState = BASE_MODES::ENGINE_STARTING;
    _vars.TimerState = BASE_MODES::CRANK_START_TIMER;
}


bool START_STOP::CheckPreheatTempCondition()
{
    //Todo : If User selects Eng Temp en in preheat menu then Ambient Temp
    //selection should get displayed and vice versa
    // Need to implement this in UI.cpp
    A_SENSE::SENSOR_RET_t sensVal = _GCUAlarms.GetSelectedTempSensVal() ;

    //Todo: In NXP the eng temp value is considered from
    // j1939 when ENg temp sensor is configured and
    // Can communication is enabled in the config.
    //Need to add can communication check
    if (( IS_GCU_PREHEAT_TEMP_ENABLED()
         &&
         (  IS_GCU_TEMP_CONFIGURED())
         &&
         ((sensVal.stValAndStatus.f32InstSensorVal > _cfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_ENG_TEMP_THRESHOLD))
                 && (sensVal.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT))
        )
        ||
        (( IS_PRHEAT_RELAY_CONFIGURED()) // Preheat output configured
          && (IS_AMB_TEMP_ENABLED())     // Amb Temp enable
          && (_GCUAlarms.IsAlarmMonEnabled(GCU_ALARMS::AMB_TEMP_SWITCH)) // Amb Temp switch enable
          && (!_GCUAlarms.AlarmResultInstat(GCU_ALARMS::AMB_TEMP_SWITCH)))) // Amb temp switch de-activated
    {
        return true;
    }

    return false;
}

uint32_t START_STOP::GetCrankStartDelay()
{
    if((BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)||(BASE_MODES::GetGCUOperatingMode() == BASE_MODES::TEST_MODE))
    {
       return (uint32_t)_cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_MANUAL_START_DELAY);
    }

    return (uint32_t)_cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_AUTO_START_DELAY);
}
