/**
 * @file        [SMDCYCLIC]
 * @brief       SMDCYCLIC
 *              Provides common API's for functionality offered by CYCLIC SMD.
 *              The module is implemented in class CYCLIC SMD which is inherited
 *              from class BASE_MODES.
 *
 * @version     [Version number]
 * @author      Soujanya Mulik
 * @date        4nd June 2022
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#include "CYCLIC_MODE.h"

CYCLIC_MODE::CYCLIC_MODE(HAL_Manager &hal, ENGINE_MONITORING &EngineMon,
                         CFGZ &cfgz,  GCU_ALARMS &GCUAlarms,
                            START_STOP &StartStop, GCU_MODE_VARS_t &vars):
BASE_MODES(hal, cfgz, GCUAlarms, EngineMon, vars),
_hal(hal),
_EngineMon(EngineMon),
_cfgz(cfgz),
_GCUAlarms(GCUAlarms),
_StartStop(StartStop),
_vars(vars),
_u32CyclicOnCount(0),
_u32CyclicOffCount(0),
_bCyclicOffTimerExpired(false),
_bCyclicOnTimerExpired(false),
_bCyclicModeGenOnStatus(false),
_bCyclicModeGenOffStatus(false),
_bStartOffTimer(false)
{

}

void CYCLIC_MODE::Update(bool bDeviceInConfigMode)
{
    if(UTILS_GetElapsedTimeInSec(&_BaseModeUpdateTimer) >= FOUR_SEC)
    {
        UTILS_ResetTimer(&_GCUSMUpdateTimer);
        UTILS_ResetTimer(&_MainsMonUpdateTimer);
        UTILS_DisableTimer(&_BaseModeUpdateTimer);
    }

    if((_eOperatingMode == BASE_MODES::CYCLIC_MODE)
            && (UTILS_GetElapsedTimeInMs(&_GCUSMUpdateTimer) >= SM_UPDATE_TIME)
             && (!bDeviceInConfigMode))
    {
        BASE_MODES::Update();

        UTILS_ResetTimer(&_GCUSMUpdateTimer);

        /***START: GEN ON timer count in Cyclic mode ***/
        if(_bCyclicModeGenOnStatus)
        {
            _u32CyclicOnCount++;
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_ON_DURATION)*60*20) <= _u32CyclicOnCount)
            {
                _bCyclicOnTimerExpired= 1;
            }
        }

        /***END: GEN ON timer count in Cyclic mode ***/

        /***START: GEN OFF timer count in Cyclic mode ***/
        if(_bCyclicModeGenOffStatus)
        {
            _u32CyclicOffCount++;
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60*20) <= _u32CyclicOffCount)
            {
                _bCyclicOffTimerExpired= 1;
            }
        }
        else
        {
            _u32CyclicOffCount = 0;
        }
        /***END: GEN OFF timer count in Cyclic mode ***/

        /* Ignore Remote start / stop */
        _bRemoteStartRCVD = false;
        _bRemoteStopRCVD  = false;


        switch(_eCyclicState)
        {
            case STATE_CYCLIC_GEN_OFF_MAINS_ON:
                if(_MainsStatus == MAINS_UNHELATHY)
                {
                    _bOpenMainsLoad = true;
                    if( (!_GCUAlarms.IsCommonElectricTrip())
                            && (!_GCUAlarms.IsCommonShutdown()) && (!_bEmergencyStop)
                            && GetPressureSensorStatusBeforeStart() && (!IsNightModeRestrictOn()))
                    {
                        _StartStop.StartCommand();
                        _eCyclicState = STATE_CYCLIC_GEN_START;
                    }
                    else
                    {
                        _eCyclicState = STATE_CYCLIC_GEN_OFF_MAINS_OFF;
                    }
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }
                else if(_EngineMon.IsEngineOn() == 1U)
                {
                    _eCyclicState = STATE_CYCLIC_ENGINE_STOP;
                    _StartStop.StopCommand();
                }
                else if(UTILS_GetElapsedTimeInMs(&_ReturnToMainsTimer) >=  5U)
                {
                    SwitchLoadToMains();
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }

                if(_bCyclicModeGenOnStatus && _bCyclicOnTimerExpired)
                {
                    /* Clear the Cyclic GEN ON timer */
                    _bCyclicModeGenOnStatus = false;
                    _bCyclicOnTimerExpired  = false;

                    /* Start the Cyclic GEN OFF timer */
                    _bCyclicModeGenOffStatus = true;
                    _u32CyclicOffCount = 0;
                    _bCyclicOffTimerExpired  = false;
                }

                if(_GCUAlarms.IsAlarmPresent())
                {
                    prvSetGCUState();
                    if(_bCyclicModeGenOnStatus)
                    {
                        _vars.TimerState = CYCLIC_ON_TIMER;
                    }
                    else if(_bCyclicModeGenOffStatus && (!_bCyclicOffTimerExpired))
                    {
                        _vars.TimerState = CYCLIC_OFF_TIMER;
                    }
                }
                else if(START_STOP::ID_STATE_SS_ENG_OFF_OK ==
                            _StartStop.GetStartStopSMDState())
                {
                    _vars.GCUState = ENGINE_OFF_READY;
                    if(_bCyclicModeGenOnStatus)
                    {
                        _vars.TimerState = CYCLIC_ON_TIMER;
                    }
                    else if(_bCyclicModeGenOffStatus && (!_bCyclicOffTimerExpired))
                    {
                        _vars.TimerState = CYCLIC_OFF_TIMER;
                    }
                    else
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                }
                else if(START_STOP::ID_STATE_SS_STOP_HOLD ==
                        _StartStop.GetStartStopSMDState())
                {
                    _vars.GCUState = ENGINE_STOPPING;
                }
                break;

            case STATE_CYCLIC_GEN_OFF_MAINS_OFF:
                if(_EngineMon.IsEngineOn() == 1U)
                {
                    _eCyclicState = STATE_CYCLIC_ENGINE_STOP;
                    _StartStop.StopCommand();
                }
                else if((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(
                        CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                {
                    _eCyclicState = STATE_CYCLIC_GEN_OFF_MAINS_ON;
                    UTILS_ResetTimer(&_ReturnToMainsTimer);

                    if(_bCyclicModeGenOffStatus)
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }

                    _bCyclicModeGenOffStatus = false;
                    _bCyclicOffTimerExpired  = false;
                    _bCyclicModeGenOnStatus  = false;
                    _bCyclicOnTimerExpired   = false;
                }
                else if((_bCyclicOffTimerExpired || ((!_bCyclicModeGenOffStatus)&&((_MainsStatus == MAINS_UNHELATHY))))
                        &&(!_GCUAlarms.IsCommonElectricTrip())
                        && (!_GCUAlarms.IsCommonShutdown()) && (!_bEmergencyStop)
                        && GetPressureSensorStatusBeforeStart()
                        && (!IsNightModeRestrictOn()))
                {
                    _StartStop.StartCommand();
                    _eCyclicState = STATE_CYCLIC_GEN_START;
                    _bCyclicModeGenOffStatus = false;
                    _bCyclicOffTimerExpired  = false;
                }

                if(_GCUAlarms.IsAlarmPresent())
                {
                    prvSetGCUState();
                    if(_bCyclicModeGenOnStatus)
                    {
                        _vars.TimerState = CYCLIC_ON_TIMER;
                    }
                    else if(_bCyclicModeGenOffStatus && (!_bCyclicOffTimerExpired))
                    {
                        _vars.TimerState = CYCLIC_OFF_TIMER;
                    }
                    else if(_bCyclicModeGenOffStatus && _bCyclicOffTimerExpired)
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                }
                else if(START_STOP::ID_STATE_SS_ENG_OFF_OK ==
                        _StartStop.GetStartStopSMDState())
                {
                    _vars.GCUState = ENGINE_OFF_READY;
                    if(_bCyclicModeGenOnStatus)
                    {
                        _vars.TimerState = CYCLIC_ON_TIMER;
                    }
                    else if(_bCyclicModeGenOffStatus && (!_bCyclicOffTimerExpired))
                    {
                        _vars.TimerState = CYCLIC_OFF_TIMER;
                    }
                }
                break;

            case STATE_CYCLIC_GEN_START:
                if(((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(
                                        CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                  || (_GCUAlarms.IsCommonShutdown())
                  || (_GCUAlarms.IsCommonElectricTrip())
                  || (IsNightModeRestrictOn()))
                {
                    _StartStop.StopCommand();
                    if(_GCUAlarms.IsCommonShutdown())
                    {

                        _vars.GCUState = SHUTDOWN;
                    }
                    else if(_GCUAlarms.IsCommonElectricTrip())
                    {
                        _vars.GCUState = ELECTRIC_TRIP;
                    }
                    else if(IsNightModeRestrictOn())
                    {
                        _vars.GCUState = EMPTY_GCU_STATE;
                    }
                    _eCyclicState = STATE_CYCLIC_ENGINE_STOP;
                }
                else if(_EngineMon.IsWarmUpTimeExpired())
                {
                    SwitchLoadToGen();
                    _eCyclicState = STATE_CYCLIC_GEN_ON_LOAD;

                    if(!_bCyclicModeGenOnStatus)
                    {
                        _bCyclicModeGenOnStatus = true;
                        _u32CyclicOnCount = 0;
                    }
                    _bCyclicOffTimerExpired  = false;
                    _bCyclicModeGenOffStatus = false;
                    _bCyclicOnTimerExpired   = false;
                }
                else if(_GCUAlarms.IsCommonWarning())
                {
                    _vars.GCUState = WARNING;
                }
                else if(_GCUAlarms.IsCommonNotification())
                {
                    _vars.GCUState = NOTIFICATION;
                }

                if(_StartStop.IsGenMonOn() == 1U)
                {
                    _vars.TimerState = NO_TIMER_RUNNING;
                }
                break;

            case STATE_CYCLIC_GEN_ON_LOAD:
                if(_GCUAlarms.IsCommonShutdown() || IsNightModeRestrictOn())
                {
                    if(_GCUAlarms.IsCommonShutdown())
                    {

                        _vars.GCUState = SHUTDOWN;
                    }
                    else if(IsNightModeRestrictOn())
                    {
                        _vars.GCUState = EMPTY_GCU_STATE;
                    }

                    _StartStop.StopCommand();
                    _eCyclicState = STATE_CYCLIC_ENGINE_STOP;
                    _bOpenGenLoad = true;

                    if(_bCyclicModeGenOnStatus)
                    {
                        _bCyclicModeGenOnStatus = false;
                    }
                }
                else if((_GCUAlarms.IsCommonElectricTrip())
                        ||(_bCyclicOnTimerExpired && _bCyclicModeGenOnStatus)
                        || ((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(
                                        CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)))
                {
                    if(_bCyclicOnTimerExpired && _bCyclicModeGenOnStatus)
                    {
                        _bStartOffTimer = true;
                        _bCyclicModeGenOnStatus = false;
                        _bCyclicOnTimerExpired  = false;

                        _vars.GCUState = ENGINE_STOPPING;
                        _vars.TimerState = COOL_DOWN_TIMER;
                        _bOpenGenLoad = true;
                        UTILS_ResetTimer(&_EngCoolDownTimer);
                        _eCyclicState = STATE_CYCLIC_ENGINE_COOLING;
                    }
                    else if((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(
                            CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                    {
                        _bStartOffTimer = false;
                        _eCyclicState = STATE_CYCLIC_RETURN_DELAY;
                        UTILS_ResetTimer(&_ReturnToMainsTimer);
                        _vars.GCUState = ENGINE_ON_HEALTHY;
                    }
                    else if(_GCUAlarms.IsCommonElectricTrip())
                    {
                        _bCyclicModeGenOnStatus = false;
                        _bCyclicOnTimerExpired  = false;
                        _vars.GCUState = ELECTRIC_TRIP;
                        _vars.TimerState = COOL_DOWN_TIMER;
                        _bOpenGenLoad = true;
                        UTILS_ResetTimer(&_EngCoolDownTimer);
                        _eCyclicState = STATE_CYCLIC_ENGINE_COOLING;
                    }
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
                }

                if(_bCyclicModeGenOnStatus)
                {
                    _vars.TimerState = CYCLIC_ON_TIMER;
                }

                if((((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                        || ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
                        && (!_bCloseGenContactor) && (!_bContactorTransferOn))
                {
                    SwitchLoadToGen();
                }
                break;

            case STATE_CYCLIC_RETURN_DELAY:
                if(_GCUAlarms.IsCommonShutdown() || IsNightModeRestrictOn())
                {
                    _vars.GCUState = SHUTDOWN;
                    _StartStop.StopCommand();

                    _eCyclicState = STATE_CYCLIC_ENGINE_STOP;
                    _bOpenGenLoad = true;
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }
                else if(_GCUAlarms.IsCommonElectricTrip())
                {
                    _vars.GCUState = ELECTRIC_TRIP;
                    _vars.TimerState = COOL_DOWN_TIMER;
                    _bOpenGenLoad = true;
                    UTILS_ResetTimer(&_EngCoolDownTimer);
                    _eCyclicState = STATE_CYCLIC_ENGINE_COOLING;
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }
                else if(_MainsStatus == MAINS_UNHELATHY)
                {
                    UTILS_DisableTimer(&_ReturnToMainsTimer);

                    if(_bCyclicModeGenOnStatus)
                    {
                        _eCyclicState    = STATE_CYCLIC_GEN_ON_LOAD;
                        _vars.GCUState   = ENGINE_ON_HEALTHY;
                        _vars.TimerState = CYCLIC_ON_TIMER;
                    }
                    else
                    {
                        _bCyclicModeGenOffStatus = true;
                        _eCyclicState    = STATE_CYCLIC_GEN_OFF_MAINS_OFF;
                        _vars.TimerState = CYCLIC_OFF_TIMER;
                    }
                }
                else if(UTILS_GetElapsedTimeInMs(&_ReturnToMainsTimer) >=5U)
                {
                    SwitchLoadToMains();
                    OpenGenLoad(); /// Open GEN load as mains inhibit input might be active
                    UTILS_DisableTimer(&_ReturnToMainsTimer);

                    if(_bCyclicModeGenOnStatus)
                    {
                        _eCyclicState    = STATE_CYCLIC_ENGINE_COOLING;
                        UTILS_ResetTimer(&_EngCoolDownTimer);
                        _vars.GCUState = ENGINE_STOPPING;
                        _vars.TimerState = COOL_DOWN_TIMER;
                    }
                    else
                    {
                        _bCyclicModeGenOffStatus = false;
                        _bCyclicOffTimerExpired  = false;
                        _eCyclicState    = STATE_CYCLIC_GEN_OFF_MAINS_ON;
                        _vars.GCUState = ENGINE_OFF_READY;
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                }

                if(_GCUAlarms.IsCommonWarning())
                {
                    _vars.GCUState = WARNING;
                }
                else if(_GCUAlarms.IsCommonNotification())
                {
                    _vars.GCUState = NOTIFICATION;
                }
                break;

            case STATE_CYCLIC_ENGINE_COOLING:
                if((UTILS_GetElapsedTimeInSec(&_EngCoolDownTimer) >=
                        _cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_COOL_DELAY))
                        || (_GCUAlarms.IsCommonShutdown())
                        || (IsNightModeRestrictOn()))
                {
                    if(_GCUAlarms.IsCommonShutdown())
                    {
                        _bStartOffTimer = false; // Clearing flag
                        _bCyclicOffTimerExpired = true;
                        _vars.GCUState = SHUTDOWN;

                    }
                    else if(IsNightModeRestrictOn())
                    {
                        _bStartOffTimer = false;
                        _vars.GCUState = EMPTY_GCU_STATE;
                    }

                    _StartStop.StopCommand();
                    _eCyclicState = STATE_CYCLIC_ENGINE_STOP;
                    UTILS_DisableTimer(&_EngCoolDownTimer);

                    if(_bCyclicModeGenOnStatus)
                    {
                        _bCyclicModeGenOnStatus = false;
                        _bCyclicOnTimerExpired = false;
                    }
                }
                else if(_GCUAlarms.IsCommonElectricTrip())
                {
                    _bStartOffTimer = false; // Clearing flag
                    _vars.GCUState = ELECTRIC_TRIP;
                    _bCyclicOffTimerExpired = true;
                }
                else if(_bStartOffTimer && (_MainsStatus == MAINS_UNHELATHY))
                {
                    // Wait for cooling down time to over
                }
                else if((_MainsStatus == MAINS_UNHELATHY) && (!_GCUAlarms.IsCommonElectricTrip()))
                {
                    if(!_bCyclicModeGenOnStatus)
                    {
                        _bCyclicModeGenOnStatus = true;
                    }

                    _bStartOffTimer = false;
                    SwitchLoadToGen();
                    _eCyclicState = STATE_CYCLIC_GEN_ON_LOAD;
                    UTILS_DisableTimer(&_EngCoolDownTimer);
                }
                else if(_GCUAlarms.IsCommonWarning())
                {
                    _vars.GCUState = WARNING;
                }
                else if(_GCUAlarms.IsCommonNotification())
                {
                    _vars.GCUState = NOTIFICATION;
                }
                else
                {
                    _vars.GCUState = ENGINE_STOPPING;
                }

                if((((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                                               ||((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
                                               && (!_bCloseMainsContactor) && (!_bContactorTransferOn))
                {
                     SwitchLoadToMains();
                }
                else if((((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                        ||((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
                        && (_bCloseMainsContactor) && (!_bContactorTransferOn))

                {
                    OpenMainsLoad();
                }
                break;

            case STATE_CYCLIC_ENGINE_STOP:
                if(_EngineMon.IsEngineOn() == 0U)
                {
                    if(_bStartOffTimer)
                    {
                        _bStartOffTimer = false;
                        _eCyclicState = STATE_CYCLIC_GEN_OFF_MAINS_OFF;

                        if(!_bCyclicModeGenOnStatus)
                        {
                            _bCyclicModeGenOffStatus = true;
                            _u32CyclicOffCount = 0;
                        }
                    }
                    else
                    {
                        if((_GCUAlarms.IsCommonShutdown())||(_GCUAlarms.IsCommonElectricTrip()))
                        {
                            _bCyclicOffTimerExpired = true;
                        }
                        _bCyclicModeGenOffStatus = false;
                        _bCyclicOnTimerExpired = false;
                        _bCyclicModeGenOnStatus = false;
                        _u32CyclicOnCount = 0;
                        _eCyclicState = STATE_CYCLIC_GEN_OFF_MAINS_OFF;
                    }

                    if(_GCUAlarms.IsAlarmPresent())
                    {
                        prvSetGCUState();
                    }
                }
                else
                {
                    // In case of fail to stop condition if mains is healthy mains contactor will get latched
                    if((_MainsStatus == MAINS_HELATHY)
                         && (!_bCloseMainsContactor) && (!_bContactorTransferOn))
                    {
                         SwitchLoadToMains();
                    }
                    else if(((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                            && (_bCloseMainsContactor) && (!_bContactorTransferOn))

                    {
                        OpenMainsLoad();
                    }
                }
                break;
            default:
                break;
        }

        if(_eCyclicState == STATE_CYCLIC_ENGINE_COOLING)
        {
            _hal.actuators.Activate(ACTUATOR::ACT_COOLING_ON);
        }
        else
        {
            _hal.actuators.Deactivate(ACTUATOR::ACT_COOLING_ON);
        }

    }
}

uint32_t CYCLIC_MODE::GetCyclicModeTime(BASE_MODES::TIMER_STATE_t eTimer)
{
    uint32_t RemainingTimeInSec = 0;
    switch(eTimer)
    {
       case BASE_MODES::CYCLIC_ON_TIMER:
           RemainingTimeInSec = ((((_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_ON_DURATION)*1200) - _u32CyclicOnCount) /1200) + 1);
           break;

       case BASE_MODES::CYCLIC_OFF_TIMER:
           RemainingTimeInSec = ((((_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*1200) - _u32CyclicOffCount) /1200) + 1);
           break;

       default:
           break;

    }
    return RemainingTimeInSec;
}

void CYCLIC_MODE::ClearCyclicModeVars(void)
{
    _u32CyclicOnCount = 0;
    _u32CyclicOffCount = 0;
    _bCyclicOffTimerExpired = false;
    _bCyclicOnTimerExpired = false;

}

void CYCLIC_MODE::SwitchFromManualToCyclic()
{
     if(_eCyclicState == STATE_CYCLIC_GEN_OFF_MAINS_OFF)
     {
         _bCyclicOffTimerExpired = true;
         _bCyclicModeGenOffStatus = false;
         _bCyclicOnTimerExpired = false;
         _bCyclicModeGenOnStatus = false;

     }
     else if(_eCyclicState == STATE_CYCLIC_GEN_ON_LOAD)
     {
          if(!_bCyclicModeGenOnStatus)
          {
              _bCyclicModeGenOnStatus = true;
              _u32CyclicOnCount = 0;
          }

          _bCyclicOffTimerExpired =  false;
          _bCyclicModeGenOffStatus = false;

          _bCyclicOnTimerExpired = false;
     }
}
