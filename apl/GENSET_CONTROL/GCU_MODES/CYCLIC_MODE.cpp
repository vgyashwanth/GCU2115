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
_CyclicOnTimer{0, false},
_CyclicOffTimer{0, false},
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

        /* Ignore Remote start / stop */
        _bRemoteStartRCVD = false;
        _bRemoteStopRCVD  = false;


        switch(_eCyclicState)
        {
            case STATE_CYCLIC_GEN_OFF_MAINS_ON:
                if(_EngineMon.IsEngineOn() == 1U)
                {
                    _eCyclicState = STATE_CYCLIC_GEN_START;
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }
                else if(_MainsStatus == MAINS_UNHELATHY)
                {
                    _bOpenMainsLoad = true;
                    if( (!_GCUAlarms.IsCommonElectricTrip())
                            && (!_GCUAlarms.IsCommonShutdown())
                            && (!_bEmergencyStop)
                            &&(!_GCUAlarms.IsCommonWarning())
                            && GetPressureSensorStatusBeforeStart()
                            && (!IsNightModeRestrictOn())
                            && (START_STOP::ID_STATE_SS_STOP_HOLD != _StartStop.GetStartStopSMDState()))
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
                else if(UTILS_GetElapsedTimeInMs(&_ReturnToMainsTimer) >= (_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_RETN_TO_MAINS_DELAY)*1000U)+ 5U)
                {
                    SwitchLoadToMains();
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }

                if(UTILS_GetElapsedTimeInSec(&_CyclicOnTimer)>=(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_ON_DURATION)*60))
                {
                    /* Clear the Cyclic GEN ON timer */
                    UTILS_DisableTimer(&_CyclicOnTimer);
                    /* Start the Cyclic GEN OFF timer */
                    UTILS_ResetTimer(&_CyclicOffTimer);
                }

                if(_GCUAlarms.IsAlarmPresent())
                {
                    prvSetGCUState();
                    if(IsNightModeRestrictOn())
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                    else if(UTILS_IsTimerEnabled(&_CyclicOnTimer))
                    {
                        _vars.TimerState = CYCLIC_ON_TIMER;
                    }
                    else if(UTILS_IsTimerEnabled(&_CyclicOffTimer) && (!(UTILS_GetElapsedTimeInSec(&_CyclicOffTimer)
                                                >=(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60))))
//                    else if(UTILS_GetElapsedTimeInSec(&_CyclicOffTimer)
//                            >=(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60))
                    {
                        _vars.TimerState = CYCLIC_OFF_TIMER;
                    }
                }
                else if(START_STOP::ID_STATE_SS_ENG_OFF_OK ==
                            _StartStop.GetStartStopSMDState())
                {
                    _vars.GCUState = ENGINE_OFF_READY;
                    if(IsNightModeRestrictOn())
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                    else if(UTILS_IsTimerEnabled(&_CyclicOnTimer))
                    {
                        _vars.TimerState = CYCLIC_ON_TIMER;
                    }
                    else if(UTILS_IsTimerEnabled(&_CyclicOffTimer) && (!(UTILS_GetElapsedTimeInSec(&_CyclicOffTimer)
                                                >=(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60))))
//                    else if(UTILS_GetElapsedTimeInSec(&_CyclicOffTimer)
//                            >=(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60))
                    {
                        _vars.TimerState = CYCLIC_OFF_TIMER;
                    }
                    else
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                }
                break;

            case STATE_CYCLIC_GEN_OFF_MAINS_OFF:
                if(_EngineMon.IsEngineOn() == 1U)
                {
                    _eCyclicState = STATE_CYCLIC_GEN_START;
                }
                else if(_MainsStatus == MAINS_HELATHY)
                {
                    _eCyclicState = STATE_CYCLIC_GEN_OFF_MAINS_ON;
                    UTILS_ResetTimer(&_ReturnToMainsTimer);

                    if((UTILS_IsTimerEnabled(&_CyclicOffTimer))&&(START_STOP::ID_STATE_SS_STOP_HOLD !=
                            _StartStop.GetStartStopSMDState()))
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                    UTILS_DisableTimer(&_CyclicOffTimer);
                    UTILS_DisableTimer(&_CyclicOnTimer);
                }
                else if(((UTILS_GetElapsedTimeInSec(&_CyclicOffTimer)
                        >=(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60)) || ((!(UTILS_IsTimerEnabled(&_CyclicOffTimer))&&(_MainsStatus == MAINS_UNHELATHY))))
                        &&(!_GCUAlarms.IsCommonElectricTrip())
                        &&(!_GCUAlarms.IsCommonWarning())
                        && (!_GCUAlarms.IsCommonShutdown())
                        && (!_bEmergencyStop)
                        && GetPressureSensorStatusBeforeStart()
                        && (!IsNightModeRestrictOn())
                        && (START_STOP::ID_STATE_SS_STOP_HOLD !=
                                _StartStop.GetStartStopSMDState()))
                {
                    _StartStop.StartCommand();
                    _eCyclicState = STATE_CYCLIC_GEN_START;
                    UTILS_DisableTimer(&_CyclicOffTimer);
                }

                if(_GCUAlarms.IsAlarmPresent())
                {
                    prvSetGCUState();
                    if(IsNightModeRestrictOn())
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                    else if(UTILS_IsTimerEnabled(&_CyclicOnTimer))
                    {
                        _vars.TimerState = CYCLIC_ON_TIMER;
                    }
                    else if(UTILS_IsTimerEnabled(&_CyclicOffTimer) && (!(UTILS_GetElapsedTimeInSec(&_CyclicOffTimer)
                            >=(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60))))
                    {
                        _vars.TimerState = CYCLIC_OFF_TIMER;
                    }
                    else if(UTILS_GetElapsedTimeInSec(&_CyclicOffTimer)
                            >=(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60))
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                }
                else if(START_STOP::ID_STATE_SS_ENG_OFF_OK ==
                        _StartStop.GetStartStopSMDState())
                {
                    _vars.GCUState = ENGINE_OFF_READY;
                    if(IsNightModeRestrictOn())
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                    else if(UTILS_IsTimerEnabled(&_CyclicOnTimer))
                    {
                        _vars.TimerState = CYCLIC_ON_TIMER;
                    }
                    else if(UTILS_IsTimerEnabled(&_CyclicOffTimer) && (!(UTILS_GetElapsedTimeInSec(&_CyclicOffTimer)
                            >=(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60))))
                    {
                        _vars.TimerState = CYCLIC_OFF_TIMER;
                    }
                }
                break;

            case STATE_CYCLIC_GEN_START:
                if((_GCUAlarms.IsCommonShutdown())
                  || (_GCUAlarms.IsCommonElectricTrip())
                  || (_GCUAlarms.IsCommonWarning())
                  || (IsNightModeRestrictOn()))
                {
                    _StartStop.StopCommand();
                    if(_GCUAlarms.IsCommonShutdown())
                    {
                        _vars.GCUState = SHUTDOWN;
                    }
                    else if(_GCUAlarms.IsCommonWarning())
                    {
                        _vars.GCUState = WARNING;
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
                else if(_MainsStatus == MAINS_HELATHY)
                {
                    _StartStop.StopCommand();
                    UTILS_ResetTimer(&_ReturnToMainsTimer);
                    _eCyclicState = STATE_CYCLIC_RETURN_DELAY;
                }
                else if(_EngineMon.IsWarmUpTimeExpired())
                {
                    SwitchLoadToGen();
                    _eCyclicState = STATE_CYCLIC_GEN_ON_LOAD;

                    if(!UTILS_IsTimerEnabled(&_CyclicOnTimer))
                    {
                        UTILS_ResetTimer(&_CyclicOnTimer);
                    }
                    UTILS_DisableTimer(&_CyclicOffTimer);
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

                    if(UTILS_IsTimerEnabled(&_CyclicOnTimer))
                    {
                        UTILS_DisableTimer(&_CyclicOnTimer);
                    }
                }
                else if((_GCUAlarms.IsCommonElectricTrip())
                        ||(IsCyclicOnTimerExpired())
                        || (_MainsStatus == MAINS_HELATHY))
                {
                    if(IsCyclicOnTimerExpired())
                    {
                        _bStartOffTimer = true;
                        UTILS_DisableTimer(&_CyclicOnTimer);

                        _vars.GCUState = ENGINE_STOPPING;
                        _vars.TimerState = COOL_DOWN_TIMER;
                        _bOpenGenLoad = true;
                        UTILS_ResetTimer(&_EngCoolDownTimer);
                        _eCyclicState = STATE_CYCLIC_ENGINE_COOLING;
                    }
                    else if(_MainsStatus == MAINS_HELATHY)
                    {
                        _bStartOffTimer = false;
                        _eCyclicState = STATE_CYCLIC_RETURN_DELAY;
                        UTILS_ResetTimer(&_ReturnToMainsTimer);
                        _vars.GCUState = ENGINE_ON_HEALTHY;
                    }
                    else if(_GCUAlarms.IsCommonElectricTrip())
                    {
                        UTILS_DisableTimer(&_CyclicOnTimer);
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

                if(UTILS_IsTimerEnabled(&_CyclicOnTimer))
                {
                    _vars.TimerState = CYCLIC_ON_TIMER;
                }

                if((((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
                        || ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE)))
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
                    //Todo: In NXP code open gen load command is absent
                    // It should have been there why is it not there
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

                    if(IsNightModeRestrictOn())
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                    else if(UTILS_IsTimerEnabled(&_CyclicOnTimer))
                    {
                        _eCyclicState    = STATE_CYCLIC_GEN_ON_LOAD;
                        _vars.GCUState   = ENGINE_ON_HEALTHY;
                        _vars.TimerState = CYCLIC_ON_TIMER;
                    }
                    else
                    {
                        UTILS_ResetTimer(&_CyclicOffTimer);
                        _eCyclicState    = STATE_CYCLIC_GEN_OFF_MAINS_OFF;
                        _vars.TimerState = CYCLIC_OFF_TIMER;
                    }
                }
                else if(UTILS_GetElapsedTimeInMs(&_ReturnToMainsTimer) >= (_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_RETN_TO_MAINS_DELAY)*1000U)+5U)
                {
                    SwitchLoadToMains();
                    UTILS_DisableTimer(&_ReturnToMainsTimer);

                    if(UTILS_IsTimerEnabled(&_CyclicOnTimer))
                    {
                        _eCyclicState    = STATE_CYCLIC_ENGINE_COOLING;
                        UTILS_ResetTimer(&_EngCoolDownTimer);
                        _vars.GCUState = ENGINE_STOPPING;
                        _vars.TimerState = COOL_DOWN_TIMER;
                    }
                    else
                    {
                        UTILS_DisableTimer(&_CyclicOffTimer);
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
                        _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ENG_COOL_TIME))
                        || (_GCUAlarms.IsCommonShutdown())
                        || (IsNightModeRestrictOn()))
                {
                    if(_GCUAlarms.IsCommonShutdown())
                    {
                        _bStartOffTimer = false; // Clearing flag
                        UTILS_DisableTimer(&_CyclicOffTimer);
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

                    if(UTILS_IsTimerEnabled(&_CyclicOnTimer))
                    {
                        UTILS_DisableTimer(&_CyclicOnTimer);
                    }
                }
                else if(_GCUAlarms.IsCommonElectricTrip())
                {
                    _bStartOffTimer = false; // Clearing flag
                    _vars.GCUState = ELECTRIC_TRIP;
                    UTILS_DisableTimer(&_CyclicOffTimer); //Doubt
                }
                else if(_bStartOffTimer && (_MainsStatus == MAINS_UNHELATHY))
                {
                    // Wait for cooling down time to over
                }
                else if((_MainsStatus == MAINS_UNHELATHY) && (!_GCUAlarms.IsCommonElectricTrip()))
                {
                    //Doubt.
                    if(!UTILS_IsTimerEnabled(&_CyclicOnTimer))
                    {
                        UTILS_ResetTimer(&_CyclicOnTimer);
                    }
                    UTILS_DisableTimer(&_CyclicOffTimer);

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

                if(((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
                                               && (!_bCloseMainsContactor) && (!_bContactorTransferOn))
                {
                     SwitchLoadToMains();
                }
                else if(((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
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

                        if(!UTILS_IsTimerEnabled(&_CyclicOnTimer))
                        {
                            UTILS_ResetTimer(&_CyclicOffTimer);
                        }
                    }
                    else
                    {
                        UTILS_DisableTimer(&_CyclicOffTimer);
                        UTILS_DisableTimer(&_CyclicOnTimer);
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
                    if(((_MainsStatus == MAINS_HELATHY)&& (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
                         && (!_bCloseMainsContactor) && (!_bContactorTransferOn))
                    {
                         SwitchLoadToMains();
                    }
                    else if(((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
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
        /*
         * SuryaPranayTeja.BVV 16-11-2022
         * In the below case statements the if conditions are added not to display
         * very high values which occurs when the values go negative.
         * This situation occurs when
         * 1. DG running in Cyclic on duration.
         * 2. Cyclic remaining on time less than the configured return to mains delay.
         * 3. Make mains healthy.
         */
        case BASE_MODES::CYCLIC_ON_TIMER:
            if(UTILS_GetElapsedTimeInSec(&_CyclicOnTimer)<= _cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_ON_DURATION)*60)
            {
                RemainingTimeInSec = (uint32_t)(ceil((float)((_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_ON_DURATION)*60) - UTILS_GetElapsedTimeInSec(&_CyclicOnTimer)) /60.0f));
            }
            break;

        case BASE_MODES::CYCLIC_OFF_TIMER:
            if(UTILS_GetElapsedTimeInSec(&_CyclicOffTimer)<= _cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60)
            {
                RemainingTimeInSec = (uint32_t)(ceil((float)((_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60) - UTILS_GetElapsedTimeInSec(&_CyclicOffTimer)) /60.0f));
            }
            break;

        default:
            break;

    }
    return RemainingTimeInSec;
}

void CYCLIC_MODE::ClearCyclicModeVars(void)
{
    UTILS_DisableTimer(&_CyclicOffTimer);
    UTILS_DisableTimer(&_CyclicOnTimer);

}

void CYCLIC_MODE::SwitchFromManualToCyclic()
{
     if(_eCyclicState == STATE_CYCLIC_GEN_OFF_MAINS_OFF)
     {
         UTILS_DisableTimer(&_CyclicOffTimer);
         UTILS_DisableTimer(&_CyclicOnTimer);

     }
     else if(_eCyclicState == STATE_CYCLIC_GEN_ON_LOAD)
     {
          if(!UTILS_IsTimerEnabled(&_CyclicOnTimer))
          {
              UTILS_ResetTimer(&_CyclicOnTimer);
          }
          UTILS_DisableTimer(&_CyclicOffTimer);
     }
}

bool CYCLIC_MODE::IsCyclicOnTimerExpired()
{
    return (bool)(UTILS_GetElapsedTimeInSec(&_CyclicOnTimer)>=(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_ON_DURATION)*60));
}

bool CYCLIC_MODE::IsCyclicOffTimerExpired()
{
    return (bool)(UTILS_GetElapsedTimeInSec(&_CyclicOffTimer)>=(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION)*60));
}

void CYCLIC_MODE::ClearStartOffTimerFlag()
{
    _bStartOffTimer = false;

}
