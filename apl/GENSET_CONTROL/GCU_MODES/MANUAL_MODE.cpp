/**
 * @file        [MANUAL_MODE]
 * @brief       MANUAL_MODE
 *              Provides common API's for functionality offered by manual SMD.
 *              The module is implemented in class ManualSMD which is inherited
 *              from class BASE_MODES.
 *
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        1st June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#include "MANUAL_MODE.h"

MANUAL_MODE::MANUAL_MODE(HAL_Manager &hal, ENGINE_MONITORING &EngineMon,
                            CFGZ &cfgz, GCU_ALARMS &GCUAlarms,  
                                START_STOP &StartStop, GCU_MODE_VARS_t &vars):
BASE_MODES(hal, cfgz, GCUAlarms, EngineMon, vars),
_hal(hal),
_EngineMon(EngineMon),
_cfgz(cfgz),
_GCUAlarms(GCUAlarms),
_StartStop(StartStop),
_vars(vars),
_bTestMode(false),
_TestModeTimer{0, false}
{

}

void MANUAL_MODE::Update(bool bDeviceInConfigMode)
{
    if(UTILS_GetElapsedTimeInSec(&_BaseModeUpdateTimer) >= FOUR_SEC)
    {
        UTILS_ResetTimer(&_GCUSMUpdateTimer);
        UTILS_ResetTimer(&_MainsMonUpdateTimer);
        UTILS_DisableTimer(&_BaseModeUpdateTimer);
    }

    if(((_eOperatingMode == BASE_MODES::MANUAL_MODE)|| (_eOperatingMode == BASE_MODES::TEST_MODE))
            && (UTILS_GetElapsedTimeInMs(&_GCUSMUpdateTimer) >= SM_UPDATE_TIME)
             && (!bDeviceInConfigMode))
    {
        BASE_MODES::Update();

        UTILS_ResetTimer(&_GCUSMUpdateTimer);

        /**
           _bMainsPartialLEDStatus flag is used to display mains status LED if mains three phase and mains partial healthy detection is enabled.
           Mains partial healthy does not affect for any other mode than BTS. Hence _bMainsPartialLEDStatus should be 1 for non BTS modes
           _bMainsPartialLEDStatus variable is initialised with 1 in order to ignore mains partial status for all other modes.
           But consider a case if mode is switched from AUTO BTS -> Manual. In order to achieve above condition, _bMainsPartialLEDStatus is set here
         * */
        SetMainsPartialLEDstatus();

        if((_MainsStatus == MAINS_UNHELATHY)&&(_bCloseMainsContactor)&&(!_bContactorTransferOn))
        {
            OpenMainsLoad();
        }

        switch(_eManualState)
        {
            case STATE_MANUAL_GEN_OFF:
                prvSwitchToGenStartState();
                break;

            case STATE_MANUAL_GEN_START:
                _bStartRequest = false;
                if((_bStopRequest) || (_GCUAlarms.IsCommonShutdown()) ||
                                      (_GCUAlarms.IsCommonElectricTrip())
                                      ||(IsNightModeRestrictOn()))
                {
                    _bStopRequest = false;
                    _StartStop.StopCommand();
                    _bOpenGenLoad = true;
                    _eManualState = STATE_MANUAL_ENGINE_STOP;
                    if(_GCUAlarms.IsCommonShutdown() ||(IsNightModeRestrictOn()))
                    {
                        _vars.GCUState = SHUTDOWN;

                    }
                    else if (_GCUAlarms.IsCommonElectricTrip())
                    {
                        _vars.GCUState = ELECTRIC_TRIP;
                    }
                    else
                    {
                        _vars.GCUState = ENGINE_STOPPING;
                    }
                }
                else if(_EngineMon.IsWarmUpTimeExpired() && _EngineMon.IsEngineCranked())
                {
                    if(_eOperatingMode == BASE_MODES::TEST_MODE)
                    {
                        _bTestMode = true;
                        UTILS_ResetTimer(&_TestModeTimer);
                    }
                    else if(CFGZ::CFGZ_ENABLE == _cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_AUTO_LOAD_TRANSFER))
                    {
                        SwitchLoadToGen();
                    }
                    _eManualState = STATE_MANUAL_GEN_READY;
                }
                else if(_GCUAlarms.IsCommonWarning())
                {
                    _vars.GCUState = WARNING;
                }
                else if(_GCUAlarms.IsCommonNotification())
                {
                    _vars.GCUState = NOTIFICATION;
                }
                else if(_StartStop.IsGenMonOn() == 1U)
                {
                    _vars.GCUState = ENGINE_ON_HEALTHY;
                    _vars.TimerState = NO_TIMER_RUNNING;
                }

                break;

            case STATE_MANUAL_GEN_READY:
                _bStartRequest = false;

                if(_bModeSwitchFromAuto || _GCUAlarms.IsCommonShutdown() ||(IsNightModeRestrictOn())
                        || (_bTestMode && _bStopRequest))
                {
                    _bModeSwitchFromAuto = false;
                    _bOpenGenLoad = true;


                    _vars.GCUState = (_GCUAlarms.IsCommonShutdown()|| (IsNightModeRestrictOn())) ?
                                            SHUTDOWN : ENGINE_STOPPING;


                    if(_bTestMode)
                    {
                        _bTestMode = false;
                    }
                     _StartStop.StopCommand();
                    _bStopRequest = false;
                    _eManualState = STATE_MANUAL_ENGINE_STOP;
                }
                else if((_bStopRequest) || (_GCUAlarms.IsCommonElectricTrip()))
                {
                    _bStopRequest = false;
                    _bOpenGenLoad = true;


                    _vars.GCUState = (_GCUAlarms.IsCommonElectricTrip()) ?
                                            ELECTRIC_TRIP : ENGINE_STOPPING;
                    _vars.TimerState = COOL_DOWN_TIMER;
                    UTILS_ResetTimer(&_EngCoolDownTimer);
                    _eManualState = STATE_MANUAL_ENGINE_COOLING;
                }
                else if(_GCUAlarms.IsCommonWarning())
                {
                    _vars.GCUState = WARNING;
                    _vars.TimerState = (_bTestMode) ? TEST_MODE_TIMER : _vars.TimerState;
                }
                else if(_StartStop.IsGenMonOn() == 1U)
                {
                    _vars.GCUState = (_GCUAlarms.IsCommonNotification()) ?
                                        NOTIFICATION : ENGINE_ON_HEALTHY;

                    _vars.TimerState = (_bTestMode) ? TEST_MODE_TIMER : NO_TIMER_RUNNING;
                }
                break;

            case STATE_MANUAL_ENGINE_COOLING:
                _bStartRequest = false;
                _bOpenGenLoad = true;

                //Todo: Remove this commented part if not required.
//                if((_cfgz.GetCFGZ_Param(CFGZ::ID_COLD_START_EN) == CFGZ::CFGZ_ENABLE)
//                        && (_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TEMP_LIMIT_EN) == CFGZ::CFGZ_DISABLE))
//                {
//                    START_STOP::ClearPreheatOutput();
//                }


                if(((UTILS_GetElapsedTimeInSec(&_EngCoolDownTimer)) >=
                        _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ENG_COOL_TIME)) ||
                        (_bStopRequest) || (_GCUAlarms.IsCommonShutdown())
                        || (IsNightModeRestrictOn()))
                {
                    _bStopRequest = false;
                    _StartStop.StopCommand();

                    UTILS_DisableTimer(&_EngCoolDownTimer);
                    _eManualState = STATE_MANUAL_ENGINE_STOP;
                }

                break;
            case STATE_MANUAL_ENGINE_STOP:
                _bStartRequest = false;
                if((_EngineMon.IsEngineOn() == 0U))
                {
                    _eManualState = STATE_MANUAL_GEN_OFF;
                }
                else if(_GCUAlarms.IsAlarmPresent())
                {
                   prvSetGCUState();
                }
                else
                {
                    _vars.GCUState = ENGINE_STOPPING;
                }

                if(_bStopRequest)
                {
                    _bStopRequest = false;
                    _StartStop.StopCommand();
                }
                break;
            default:
                //TODO:: define the error code value for this situation.
                break;
        }

        if(_eManualState == STATE_MANUAL_ENGINE_COOLING)
        {
            _hal.actuators.Activate(ACTUATOR::ACT_COOLING_ON);
        }
        else
        {
            _hal.actuators.Deactivate(ACTUATOR::ACT_COOLING_ON);
        }

        _bModeSwitchFromAuto = false;
    }
}

void MANUAL_MODE::prvSwitchToGenStartState()
{
    if(_bStopRequest)
    {
        _bStopRequest = false;
        _bStartRequest = false;
    }
    else if(_bStartRequest)
    {
        _bStartRequest = false;
        if((!_bEmergencyStop) && (!_GCUAlarms.IsCommonElectricTrip())
             && (!_GCUAlarms.IsCommonShutdown())
            && GetPressureSensorStatusBeforeStart())
        {
            _StartStop.StartCommand();
            _eManualState = STATE_MANUAL_GEN_START;
        }
        UTILS_DisableTimer(&_ReturnToMainsTimer);
    }
    else if(_EngineMon.IsEngineOn() == 1U)
    {
        _StartStop.StopCommand();
        _eManualState = STATE_MANUAL_ENGINE_STOP;
    }

    if(_GCUAlarms.IsAlarmPresent())
    {
       prvSetGCUState();
    }
    else if(START_STOP::ID_STATE_SS_ENG_OFF_OK == _StartStop.GetStartStopSMDState())
    {
        _vars.GCUState = ENGINE_OFF_READY;
        _vars.TimerState = NO_TIMER_RUNNING;
    }
}

bool MANUAL_MODE::IsOperInProgress()
{
    return (_StartStop.IsTimerRunning() ||_StartStop.IsAdditionalStopTimerRunnint() || _EngCoolDownTimer.bEnabled
             || _ReturnToMainsTimer.bEnabled  || _EngineMon.IsGenWarmUpEnabledAndNotExpired());
}

bool MANUAL_MODE::IsGenRunTimersInProcess()
{
    return (_StartStop.IsTimerRunning() || _EngCoolDownTimer.bEnabled
                  || _EngineMon.IsGenWarmUpEnabledAndNotExpired());
}

uint32_t MANUAL_MODE::GetEngCoolDownRemTime()
{
    return (_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ENG_COOL_TIME) -
                    UTILS_GetElapsedTimeInSec(&_EngCoolDownTimer));
}

void MANUAL_MODE::ChangeManualState(MANUAL_STATE_t estate)
{
    _eManualState = estate;
}
void MANUAL_MODE::ComeOutOfTestMode()
{
    _bTestMode = false;
}

void MANUAL_MODE::DisableTestModeTimer()
{
    UTILS_DisableTimer(&_TestModeTimer);
}

bool MANUAL_MODE::IsTestModeTimerExpired()
{
    return ((UTILS_GetElapsedTimeInSec(&_TestModeTimer) >=
                (_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_TEST_MODE_TIMER)*60)));
}

uint32_t MANUAL_MODE::GetTestModeRemTime()
{
    return ((_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_TEST_MODE_TIMER)*60) -
                    UTILS_GetElapsedTimeInSec(&_TestModeTimer));
}
