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
bool START_STOP::_bEngStoppingComplete = false;
bool START_STOP::_bChargAltStopLatched = false;
bool START_STOP::_bOPStopSolenoid = false;
bool START_STOP::_bOPStartRelay = false;
uint16_t START_STOP::_u16ConfiguredSafetyMonDelay = false;
stTimer START_STOP::_SafetyMonTimer = {0};
stTimer START_STOP::_EngStoppingTimer = {0};
bool START_STOP::_bOPPreheat = false;
bool START_STOP::_bOPFuelRelay= false;
bool START_STOP::_bKeySwitchOutput = false;
bool START_STOP::_bInIdleMode = false;
bool START_STOP::_bActiveAlarms = false;
bool START_STOP::_bStopCommand = false;
bool START_STOP::_bSkipStartIdle = false;
bool START_STOP::_bSkipStopIdle = false;
bool START_STOP::_bStopIdleExec = false;
bool START_STOP::_bIsLowIdleTimerModeExec = false;

bool START_STOP:: _bStartIdleEnabled = false;
bool START_STOP:: _bStopIdleEnabled = false;

START_STOP::SS_STATE_t START_STOP::_State =ID_STATE_SS_ENG_OFF_OK;
extern J1939APP *gpJ1939;
bool START_STOP::_bJ1939PrheatFaultPresent= false;
START_STOP::START_STOP(HAL_Manager &hal, ENGINE_MONITORING &EngineMon, CFGZ &cfgz,  
                        GCU_ALARMS &GCUAlarms, BASE_MODES::GCU_MODE_VARS_t &vars, CHARGING_ALT &ChargeAlt, ENGINE_START_VALIDITY  &EngineStartValidity):
_hal(hal),
_EngineMon(EngineMon),
_cfgz(cfgz),
_GCUAlarms(GCUAlarms),
_vars(vars),
_ChargeAlt(ChargeAlt),
_EngineStartValidity(EngineStartValidity),
_PullSolenoidState(PULL_SOLENOID_OFF),
_IdleModeState(START_IDLE_OPR),
_bStopIdleExecuted(false),
_bLowIdleOp(false),
_bIdleModeOnPulse(false),
_bIdleModeOffPulse(false),
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
_bActivatePullSolenoid(false),
_bActivateHoldSolenoid(false),
_PreheatTimer{0},
_EngStartTimer{0},
_ISVPullSolenoidTimer{0},
_EngCrankingTimer{0},
_EngCrankRestTimer{0},
_StartStopSMUpdateTimer{0},
_StopHoldTimer{0},
_PowerOnTimer{0},
_LowSpeedTimer{0},
_IdleModePulseTimer{0},
_IdleToRatedTimer{0},
_IdleModeStopTimer{0}
{
    UTILS_ResetTimer(&_PowerOnTimer);
    UTILS_DisableTimer(&_LowSpeedTimer);

    UTILS_DisableTimer(&_IdleModePulseTimer);
    UTILS_DisableTimer(&_IdleToRatedTimer);
    UTILS_DisableTimer(&_IdleModeStopTimer);

    _u16ConfiguredSafetyMonDelay = _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SAFETY_MONITOR_DELAY);
}

void START_STOP::Init()
{
    UTILS_ResetTimer(&_StartStopSMUpdateTimer);
    _u16ConfiguredSafetyMonDelay = _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SAFETY_MONITOR_DELAY);
}
void START_STOP::prvSMDInletShutoffValve()
{
    switch(_PullSolenoidState)
    {
        case PULL_SOLENOID_PULSE_ON:
            _bActivatePullSolenoid = true;
            _bActivateHoldSolenoid = true;
            _PullSolenoidState = PULL_SOLENOID_PULSE_STARTED;
            UTILS_ResetTimer(&_ISVPullSolenoidTimer);
        break;

//        case PULL_SOLENOID_PULSE_STARTED:
//            if((UTILS_GetElapsedTimeInMs(&_ISVPullSolenoidTimer) >=
//                    (uint64_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_ISV_PULL_SIGNAL_TIME)*1000))||(_State==ID_STATE_SS_STOPPING))
//            {
//                _bActivatePullSolenoid = false;
//                _PullSolenoidState = PULL_SOLENOID_PULSE_OVER;
//            }
//        break;

        case PULL_SOLENOID_PULSE_OVER:
        default:
            break;
    }
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
                _bJ1939PrheatFaultPresent = false;
                _bKeySwitchOutput = true;

                if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_WAVE_DETECTION) == CFGZ::CFGZ_ENABLE)
                {
                    _EngineStartValidity.InitInvalidDgDetectionStateMachine();
                    ENGINE_START_VALIDITY::SetEngineStartInvalidity(false);
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
                    _bEngStoppingComplete = false;
                    _PullSolenoidState = PULL_SOLENOID_OFF;
                    _GCUAlarms.LogEvent(GCU_ALARMS::Engine_Start_id, GCU_ALARMS::ID_NONE);
                   // _bOPFuelRelay = true;
                    _bOPPreheat = false;
                    UTILS_DisableTimer(&_PreheatTimer);
                    UTILS_ResetTimer(&_EngStartTimer);
                    _State = ID_STATE_SS_START_WAIT;
                    _vars.TimerState = BASE_MODES::START_DELAY_TIMER;
                    bPreheatTempLimitReached = false;
                    _vars.GCUState = BASE_MODES::ENGINE_STARTING;
                    if(_cfgz.GetEngType() != CFGZ::ENG_CONVENTIONAL )
                    {
                        _bOPFuelRelay= true;
                    }
                }
                break;

            case ID_STATE_SS_ENG_OFF_ERR:
                prvSetOutputVariables(false, false, false, false);

                _State = ID_STATE_SS_ENG_OFF_OK;
                break;

            case ID_STATE_SS_PREHEAT:
//                if(_cfgz.GetEngType()== CFGZ::ENG_CONVENTIONAL)
//                {
//                    _bOPFuelRelay = false;
//                }
                prvSetOutputVariables(false, false, _bOPFuelRelay, true);
                _bStartCommand = false;
                if(_bStopCommand)
                {
                    prvStopCommandAction();

                    UTILS_DisableTimer(&_PreheatTimer);
                    bPreheatTempLimitReached = false;
                }
                else if((CFGZ::ENG_VOLVO == _cfgz.GetEngType())&& IS_PREHEAT_J1939_CONFIGURED())
                {

                    if((uint8_t)gpJ1939->GetReadData(RX_PGN_VOLVO_PREHEAT_65351, 0) !=0)
                    {
                        UTILS_DisableTimer(&_PreheatTimer);
                        UpdateGcuStatusAndTimerDisplay(BASE_MODES::ENGINE_STARTING, BASE_MODES::PREHEAT_TIMER);
                        _hal.AnalogSensors.SkipPulses(PULSES_TO_IGNORE);
                    }
                    else if((uint8_t)gpJ1939->GetReadData(RX_PGN_VOLVO_PREHEAT_65351, 0) ==0)
                    {
                        prvTurnOffPreheatStartCranking();
                    }
                }
                else if((CFGZ::ENG_IVECO == _cfgz.GetEngType()) && IS_PREHEAT_J1939_CONFIGURED())
                {
                    if(((uint8_t)gpJ1939->GetReadData(RX_PGN_IVECO_PREHEAT_65281, 6) == J1939APP::PREHEAT_NOT_ACTIVE)
                            || ((uint8_t)gpJ1939->GetReadData(RX_PGN_IVECO_PREHEAT_65281, 6) == J1939APP::PREHEAT_COMPLETED))
                    {
                        prvTurnOffPreheatStartCranking();
                    }
                    else if((((uint8_t)gpJ1939->GetReadData(RX_PGN_IVECO_PREHEAT_65281, 6) == J1939APP::PREHEAT_IN_PROGRESS) &&( UTILS_GetElapsedTimeInSec(&_PreheatTimer) >= _cfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_PREHEAT_TIMER)))
                            ||((uint8_t)gpJ1939->GetReadData(RX_PGN_IVECO_PREHEAT_65281, 6) == J1939APP::PREHEAT_FAILED))
                    {
                        _bStopCommand = false;
                        _bGenStarted = false;
                        _ChargeAlt.StopExcitation();
                        _hal.ObjGlcd.TurnOnBackLight();
                        _bJ1939PrheatFaultPresent = true;
                        UTILS_DisableTimer(&_EngCrankingTimer);
                        UTILS_DisableTimer(&_EngCrankRestTimer);
                        bPreheatTempLimitReached = false;
                        _bOPPreheat = false;
                        UTILS_DisableTimer(&_PreheatTimer);
                        BASE_MODES::Stop();
                    }


                }
                else if(((UTILS_GetElapsedTimeInSec(&_PreheatTimer)) >= 
                        _cfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_PREHEAT_TIMER)) ||
                        bPreheatTempLimitReached)
                {
                    bPreheatTempLimitReached = false;
                    _bOPPreheat = false;
                    UTILS_DisableTimer(&_PreheatTimer);
                    _State = ID_STATE_PULL_SOLENOID_ON;
                    _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;

                }
                break;

            case ID_STATE_SS_START_WAIT:
                prvSetOutputVariables(false, false, _bOPFuelRelay, false);
                _bStartCommand = false;

                if((_cfgz.GetEngType()==CFGZ::ENG_CONVENTIONAL))
                {
                    if(((UTILS_GetElapsedTimeInSec(&_EngStartTimer)>= _cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_MANUAL_START_DELAY)) && (BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE))
                            ||((UTILS_GetElapsedTimeInSec(&_EngStartTimer)>= _cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_AUTO_START_DELAY)) && (BASE_MODES::GetGCUOperatingMode() > BASE_MODES::MANUAL_MODE)))
                    {
                        _bOPFuelRelay = true;
                    }
                    else
                    {
                        _bOPFuelRelay = false;
                    }
                }

                if((_bStopCommand))
                {

                   prvStopCommandAction();
                   UTILS_DisableTimer(&_EngStartTimer);

                   if((CFGZ::ENG_DEUTZ_EMR == _cfgz.GetEngType()) && (0 != (uint8_t)gpJ1939->GetReadData(RX_PGN_EMR_PREHEAT_65284, 0)) )
                   {
                       if(BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)
                       {
                           BASE_MODES::SetModeState(BASE_MODES::STATE_MANUAL_GEN_OFF);
                       }
                       else if(BASE_MODES::GetGCUOperatingMode() == BASE_MODES::AUTO_MODE )
                       {
                           BASE_MODES::SetModeState(BASE_MODES::STATE_AMF_GEN_OFF_MAINS_OFF);
                       }
                       else if(BASE_MODES::GetGCUOperatingMode() == BASE_MODES::BTS_MODE)
                       {
                           BASE_MODES::SetModeState(BASE_MODES::STATE_BTS_GEN_OFF_MAINS_OFF);
                       }
                       else if(BASE_MODES::GetGCUOperatingMode() == BASE_MODES::CYCLIC_MODE)
                       {
                           BASE_MODES::SetModeState(BASE_MODES::STATE_CYCLIC_GEN_OFF_MAINS_OFF);
                       }
                       else if(BASE_MODES::GetGCUOperatingMode() == BASE_MODES::AUTO_EXERCISE_MODE)
                       {
                           BASE_MODES::SetModeState(BASE_MODES::ID_AUTO_EXE_DG_OFF);
                       }
                       else{/* do nothing */}
                   }
                }
                else if(((UTILS_GetElapsedTimeInSec(&_EngStartTimer)) >=
                        _cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_MANUAL_START_DELAY) && (BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)) ||
                        ((UTILS_GetElapsedTimeInSec(&_EngStartTimer)) >=
                                _cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_AUTO_START_DELAY) && (BASE_MODES::GetGCUOperatingMode() != BASE_MODES::MANUAL_MODE)))
                {
                    UTILS_DisableTimer(&_EngStartTimer);

                    if((((((!bPreheatTempLimitReached && IS_PRHEAT_RELAY_CONFIGURED())|| IS_PREHEAT_J1939_CONFIGURED()) && ((CFGZ::ENG_IVECO != _cfgz.GetEngType()) && (CFGZ::ENG_DEUTZ_EMR != _cfgz.GetEngType())))||
                         (IS_PREHEAT_J1939_CONFIGURED() &&(CFGZ::ENG_IVECO == _cfgz.GetEngType())&& (J1939APP::PREHEAT_NOT_ACTIVE != (uint8_t)gpJ1939->GetReadData(RX_PGN_IVECO_PREHEAT_65281,6)))
                       ||((!IS_PREHEAT_J1939_CONFIGURED())&& IS_PRHEAT_RELAY_CONFIGURED() &&((CFGZ::ENG_IVECO == _cfgz.GetEngType()) || (CFGZ::ENG_DEUTZ_EMR == _cfgz.GetEngType()))) )
                      ))
                    {
                        UTILS_ResetTimer(&_PreheatTimer);
                        _State = ID_STATE_SS_PREHEAT;
                        _vars.TimerState = BASE_MODES::PREHEAT_TIMER;
                    }
                    else
                    {
                        _State = ID_STATE_PULL_SOLENOID_ON;
                        _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;
                    }

                }
                break;

            case ID_STATE_SS_CRANKING:
                prvSetOutputVariables(true, false, true, false);
                _bStartCommand = false;
                if(_bStopCommand)
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
                        _bChargAltStopLatched = false;
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
                        (2 <= UTILS_GetElapsedTimeInSec(&_EngCrankingTimer)))
                {
                    _ChargeAlt.StopExcitation();
                    _bChargAltStopLatched = true;
                }
                break;

            case ID_STATE_SS_CRANK_REST:
                _hal.ObjGlcd.TurnOnBackLight();
                if((_cfgz.GetEngType() == CFGZ::ENG_CONVENTIONAL))
                {
                    if(UTILS_GetElapsedTimeInSec(&_EngCrankRestTimer) >= (_cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_REST_TIME)-1U))
                    {
                        _bOPFuelRelay = true;
                    }
                    else
                    {
                        _bOPFuelRelay = false;
                    }
                }
                prvSetOutputVariables(false, false, _bOPFuelRelay, false);
                _bStartCommand = false;
                if(_bStopCommand)
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
                    _State = ID_STATE_PULL_SOLENOID_ON;
                    _vars.GCUState = BASE_MODES::ENGINE_STARTING;
                    _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;
                    _PullSolenoidState = PULL_SOLENOID_OFF;
                    if(CFGC::IsSGC421())
                    {
                        EGOV::ResetEgovVars();
                        _hal.actuators.STPR_MoveToHome();
                    }
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

                    _EngineMon.DisableEngWarmUpTimer();
                    _bEngStoppingComplete = false;
                    _bKeySwitchOutput = false;
//                    if(_GCUAlarms.IsAlarmActive(GCU_ALARMS::OVERSPEED)||
//                            _GCUAlarms.IsAlarmActive(GCU_ALARMS::GROSS_OVERSPEED)||_GCUAlarms.IsAlarmActive(GCU_ALARMS::EMERGENCY_STOP))
                    if(0)
                    {
                        _bActivateHoldSolenoid = false;
                    }
                    if(_bInIdleMode && _IdleModeStopTimer.bEnabled)
                    {
                        UTILS_ResetTimer(&_EngStoppingTimer);
                    }
                    else
                    {
                        _bSkipStopIdle = false;
                        if(_EngineMon.IsEngineOn() == 0U)
                        {
                            _bActivateHoldSolenoid = false;
                            UTILS_DisableTimer(&_EngStoppingTimer);
                            UTILS_ResetTimer(&_StopHoldTimer);
                            _State = ID_STATE_SS_STOP_HOLD;
                            _vars.TimerState = BASE_MODES::ADDTIONAL_STOP_TIMER;
                        }
                        if((UTILS_GetElapsedTimeInSec(&_EngStoppingTimer)) >=
                                _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_STOP_ACTION_TIME))
                        {
                            _bStartCommand = false;
                            _bEngStoppingComplete = true;
                            _bKeySwitchOutput = true;
                            _bActivateHoldSolenoid = false;
                            UTILS_DisableTimer(&_EngStoppingTimer);
                            UTILS_ResetTimer(&_StopHoldTimer);
                            _State = ID_STATE_SS_FAIL_TO_STOP;
                            _GCUAlarms.UpdateFailToStop();
                            _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;
                        }
                    }

                break;

            case ID_STATE_SS_STOP_HOLD:
                _bStopCommand = false;
                prvSetOutputVariables(false, true, false, false);
//                if(_cfgz.GetCFGZ_Param(CFGZ::ID_MON_AFT_FROM_ENGINE_ON) == CFGZ::CFGZ_ENABLE)
//                {
//                    _GCUAlarms.DisableAFTTimeoutAfterActTimer();
//                }
                if(_bEmergencyStop)
                {
                    _bEmergencyStopLatched = true;
                }
                if((UTILS_GetElapsedTimeInSec(&_StopHoldTimer)) >=
                        _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ADDN_STOPPING_TIME))
                {

                    UTILS_DisableTimer(&_StopHoldTimer);
                    _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;
                    _State = ID_STATE_SS_ENG_OFF_OK;
                    _hal.AnalogSensors.SkipPulses(10);
                }
                else if(_bStartCommand)
                {
                    _bStartCommand = false;
                    _u8NoOfCrankAttempts = 0;
                    _bKeySwitchOutput = true;
                    UTILS_DisableTimer(&_StopHoldTimer);
                    _bOPFuelRelay = true;
                    _GCUAlarms.LogEvent(GCU_ALARMS::Engine_Start_id, GCU_ALARMS::ID_NONE);
                    _PullSolenoidState = PULL_SOLENOID_OFF;
                    UTILS_ResetTimer(&_EngStartTimer);
                    _vars.TimerState = BASE_MODES::NO_TIMER_RUNNING;
                    _State = ID_STATE_SS_START_WAIT;

                    _bIsLowIdleTimerModeExec = false;
                    _bStopIdleExec = false;
                    _bInIdleMode = false;

                    _vars.TimerState = BASE_MODES::START_DELAY_TIMER;
                    _vars.GCUState = BASE_MODES::ENGINE_STARTING;
                    if( _cfgz.GetEngType() != CFGZ::ENG_CONVENTIONAL )
                    {
                        _bOPFuelRelay  = 1;
                    }
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
                    _hal.AnalogSensors.SkipPulses(10);
                }
                break;

            case ID_STATE_PULL_SOLENOID_ON:
                if(_bStopCommand)
                {
                    _bStopCommand = false;
                    _ChargeAlt.StopExcitation();
                    _bOPPreheat = false;
                    bPreheatTempLimitReached = false;
                    UTILS_DisableTimer(&_PreheatTimer);

                    _State = ID_STATE_SS_STOPPING;
                    _vars.GCUState = BASE_MODES::ENGINE_STOPPING;
                    _vars.TimerState = BASE_MODES::STOP_ACTION_TIMER;
                }
//                else if(ACT_Manager::ACT_NOT_CONFIGURED !=
//                        _hal.actuators. GetActStatus(ACTUATOR::ACT_ISV_PULL_SIGNAL))
//                {
//                    if(_PullSolenoidState == PULL_SOLENOID_OFF)
//                    {
//                        _PullSolenoidState = PULL_SOLENOID_PULSE_ON ;
//                        UTILS_ResetTimer(&_ISVPullSolenoidTimer);
//                    }
//                    else if(_PullSolenoidState == PULL_SOLENOID_PULSE_OVER)
//                    {
//                        UTILS_ResetTimer(&_EngCrankingTimer);
//                        _State = ID_STATE_SS_CRANKING;
//                        _hal.ObjGlcd.TurnOffBackLight();
//                        _u8NoOfCrankAttempts++;
//                        _ChargeAlt.StartExcitation();
//                        _bChargAltStopLatched = false;
//                        _vars.GCUState = BASE_MODES::ENGINE_STARTING;
//                        _vars.TimerState = BASE_MODES::CRANK_START_TIMER;
//                        /* Reset the value here, so to ignore the first 10 pulses at Engine Cranking state */
//                        _hal.AnalogSensors.SkipPulses(PULSES_TO_IGNORE);
//
//                    }
//                }
                else
                {
                    UTILS_ResetTimer(&_EngCrankingTimer);
                    _State = ID_STATE_SS_CRANKING;

                    _hal.ObjGlcd.TurnOffBackLight();
                    _u8NoOfCrankAttempts++;
                    _ChargeAlt.StartExcitation();
                    _bChargAltStopLatched = false;
                    _vars.GCUState = BASE_MODES::ENGINE_STARTING;
                    _vars.TimerState = BASE_MODES::CRANK_START_TIMER;
                    /* Reset the value here, so to ignore the first 10 pulses at Engine Cranking state */
                    _hal.AnalogSensors.SkipPulses(PULSES_TO_IGNORE);

                }

                break;

            default:
                break;
        }
        if(bPreheatTempLimitReached && _bOPPreheat)
        {
            if((CFGZ::ENG_VOLVO == _cfgz.GetEngType())&& (IS_PREHEAT_J1939_CONFIGURED()))
            {
                //Do nothing
            }
            else
            {
                _bOPPreheat = false;
            }
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

void START_STOP::prvStopCommandAction()
{

    if(true)
    {
        _bStopIdleExecuted = false;
        /*LDRA_INSPECTED 62 S */
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
                UTILS_DisableTimer(&_EngStartTimer);
                _bActivateHoldSolenoid = false;
                UTILS_DisableTimer(&_EngStoppingTimer);
                UTILS_ResetTimer(&_StopHoldTimer);
                _State = ID_STATE_SS_STOP_HOLD;
                _vars.TimerState = BASE_MODES::ADDTIONAL_STOP_TIMER;
                break;

            case ID_STATE_SS_ENG_ON:
                _EngineMon.DisableGenWarmUpTimer();
                _EngineMon.StoreCummulativeCnt();
                /* no break*/
            case ID_STATE_SS_CRANKING:
            case ID_STATE_SS_CRANK_REST:
            case ID_STATE_SS_FAIL_TO_STOP:
                _bOPPreheat = false;
                UTILS_DisableTimer(&_PreheatTimer);
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

void START_STOP::SkipStopIdleCommand()
{
    _bSkipStopIdle = true;
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
bool START_STOP::IsIdleModeActive()
{
    return (_bStartIdleEnabled || _bStopIdleEnabled);
}
uint16_t START_STOP::GetStartIdleRemTime()
{

    {
        return 0;
    }
}
uint16_t START_STOP::GetStopIdleRemTime()
{

    {
        return 0;
    }
}
 uint16_t START_STOP::GetIdleToRatedRemTime()
{

    {
        return 0;
    }
}

bool START_STOP::IsStartIdleActive()
{
    if(IsIdleModeInputConfigured())
    {
        return _bStartIdleEnabled;
    }
    else
    {
        return  _LowSpeedTimer.bEnabled;
    }
}
bool START_STOP::IsStopIdleActive()
{
   return  _bStopIdleEnabled;
}
bool START_STOP:: IsLowIdleRatedDelay()
{
    return  _IdleToRatedTimer.bEnabled;
}
bool START_STOP::IsIdleToRatedDelayActive()
{
    return _IdleToRatedTimer.bEnabled;
}


uint32_t START_STOP::GetStopLowIdleTime()
{
     uint32_t RemainingTimeInSec =  0;

    return RemainingTimeInSec;
}
bool START_STOP::IsKeySwitchOutputActive()
{
    return _bKeySwitchOutput;
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
            if(BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)
            {
                RemainingTimeInSec = (_cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_MANUAL_START_DELAY) -
                                                    UTILS_GetElapsedTimeInSec(&_EngStartTimer));
            }
            else if((BASE_MODES::GetGCUOperatingMode() == BASE_MODES::AUTO_MODE)||
                    (BASE_MODES::GetGCUOperatingMode() == BASE_MODES::AUTO_EXERCISE_MODE)||
                    (BASE_MODES::GetGCUOperatingMode() == BASE_MODES::BTS_MODE)||
                    (BASE_MODES::GetGCUOperatingMode() == BASE_MODES::CYCLIC_MODE))
            {
                RemainingTimeInSec = (_cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_AUTO_START_DELAY) -
                                                    UTILS_GetElapsedTimeInSec(&_EngStartTimer));
            }
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
    UTILS_DisableTimer(&_EngCrankingTimer);
    _State = ID_STATE_SS_ENG_ON;
    _vars.GCUState = BASE_MODES::ENGINE_ON_HEALTHY;
    _vars.TimerState = BASE_MODES::SAFETY_MON_TIMER;
    _bChargAltStopLatched = false;
}

bool START_STOP::IsEngStoppingComplete()
{
    return _bEngStoppingComplete;
}

bool START_STOP::IsChargAltStopLtached()
{
    return _bChargAltStopLatched;
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

bool  START_STOP::IsIdleToRatedRampOver()
{
    return (_bActiveAlarms); /* This flag diables UV,OV,UF alarms */
}

void START_STOP::UpdateGcuStatusAndTimerDisplay(BASE_MODES::GCU_STATE_t eGcuState, BASE_MODES::TIMER_STATE_t eTimerDisplay)
{
    _vars.GCUState = eGcuState;
    _vars.TimerState = eTimerDisplay;
}

void START_STOP::prvTurnOffPreheatStartCranking()
{
    UTILS_ResetTimer(&_EngCrankingTimer);
    _State = ID_STATE_SS_CRANKING;
    _u8NoOfCrankAttempts++;
    _ChargeAlt.StartExcitation();
    _bChargAltStopLatched = false;
    UpdateGcuStatusAndTimerDisplay(BASE_MODES::ENGINE_STARTING, BASE_MODES::CRANK_START_TIMER);
    _bOPPreheat = false;
    /* Reset the value here, so to ignore the first 10 pulses at Engine Cranking state */
    _hal.AnalogSensors.SkipPulses(PULSES_TO_IGNORE);

}

bool START_STOP::IsJ1939PreheatFaultPresent()
{
    return _bJ1939PrheatFaultPresent;
}




bool START_STOP::IsIdleModeInputConfigured()
{
    return false;
}

void START_STOP::prvDisableStartIdle()
{}
void START_STOP::prvDisableStopIdle()
{}




bool START_STOP::CheckPreheatTempCondition()
{

    A_SENSE::SENSOR_RET_t sensVal = _GCUAlarms.GetSelectedTempSensVal() ;

    if ( IS_GCU_PREHEAT_TEMP_ENABLED()
         &&
         (  IS_GCU_TEMP_CONFIGURED()
              ||
            IS_ENG_TEMP_J1939_CONFIGURED()
         )        )
    {
        return true;
    }

    return false;
}
