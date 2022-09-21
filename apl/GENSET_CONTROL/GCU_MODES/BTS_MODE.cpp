/**
 * @file        [SMDBTS]
 * @brief       SMDBTS
 *              Provides common API's for functionality offered by BTS SMD.
 *              The module is implemented in class BTSSMD which is inherited
 *              from class BASE_MODES.
 *
 * @version     [Version number]
 * @author      Soujanya Mulik
 * @date        2nd June 2022
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#include "BTS_MODE.h"

BTS_MODE::BTS_MODE(HAL_Manager &hal, ENGINE_MONITORING &EngineMon,
                         CFGZ &cfgz,  GCU_ALARMS &GCUAlarms,
                            START_STOP &StartStop, GCU_MODE_VARS_t &vars):
BASE_MODES(hal, cfgz, GCUAlarms, EngineMon, vars),
_hal(hal),
_EngineMon(EngineMon),
_cfgz(cfgz),
_GCUAlarms(GCUAlarms),
_StartStop(StartStop),
_vars(vars),
_bGenOnDuetoBTS(false),
_bGenOnDueToSheltTemp(false),
_bBattChargingOn(false),
_bShelterTempTmrOn(false),
_bBattChargTmrExpired(false),
_bShelterTempTmrExpired(false),
_u32BattChargingCount(0),
_u32ShelterTempTimerCount(0)
{

}

void BTS_MODE::Update(bool bDeviceInConfigMode)
{
    if(UTILS_GetElapsedTimeInSec(&_BaseModeUpdateTimer) >= FOUR_SEC)
    {
        UTILS_ResetTimer(&_GCUSMUpdateTimer);
        UTILS_ResetTimer(&_MainsMonUpdateTimer);
        UTILS_DisableTimer(&_BaseModeUpdateTimer);
    }

    if((_eOperatingMode == BASE_MODES::BTS_MODE)
            && (UTILS_GetElapsedTimeInMs(&_GCUSMUpdateTimer) >= SM_UPDATE_TIME)
             && (!bDeviceInConfigMode))
    {
        BASE_MODES::Update();

        UTILS_ResetTimer(&_GCUSMUpdateTimer);

        /***START: GEN ON timer count in fuel saver mode for BTS bat low volt ***/
        if(_bBattChargingOn)
        {
            _u32BattChargingCount++;
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION)*60U*20) <= _u32BattChargingCount)
            {
                _bBattChargTmrExpired = 1;
            }
        }
        else
        {
            _u32BattChargingCount = 0;
        }
        /***END: GEN ON timer count in fuel saver mode for BTS bat low volt ***/

        /***START: GEN ON timer count in fuel saver mode for high shelter temp ***/
        if(_bShelterTempTmrOn)
        {
            _u32ShelterTempTimerCount++;
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION)*60U*20) <= _u32ShelterTempTimerCount)
            {
                _bShelterTempTmrExpired = 1;
            }
        }
        else
        {
            _u32ShelterTempTimerCount = 0;
        }
        /***END: GEN ON timer count in fuel saver mode for high shelter temp ***/


        /* Ignore Remote start / stop */
        _bRemoteStartRCVD = false;
        _bRemoteStopRCVD  = false;

        /*Todo : refer to NXP code.*/
        if((_MainsStatus == MAINS_UNHELATHY)&&(_bCloseMainsContactor)&&(!_bContactorTransferOn))
        {
            OpenMainsLoad();
        }

        switch(_eBTSState)
        {
            case STATE_BTS_GEN_OFF_MAINS_OFF:
                if(_EngineMon.IsEngineOn() == 1U)
                {
//                    _StartStop.StopCommand();
//                    _eBTSState = STATE_BTS_ENGINE_STOP;
                    _eBTSState = STATE_BTS_GEN_START;
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }
                else if(prvGenStartCondition())
                {
                     if((!_GCUAlarms.IsCommonElectricTrip())
                            && (!_GCUAlarms.IsCommonShutdown())
                            &&(!_GCUAlarms.IsCommonWarning())
                            && (!_bEmergencyStop)
                            && (START_STOP::ID_STATE_SS_STOP_HOLD != _StartStop.GetStartStopSMDState()))
                     {
                         if(GetPressureSensorStatusBeforeStart())
                         {
                             if(!_GCUAlarms.IsBTSBattHealthy())
                             {
                                 _bGenOnDuetoBTS = true;
                                 _bGenOnDueToSheltTemp = false;
                             }
                             else if(_GCUAlarms.IsShelterTempHigh())
                             {
                                 _bGenOnDuetoBTS = false;
                                 _bGenOnDueToSheltTemp = true;
                             }
                             else
                             {
                                 _bGenOnDuetoBTS = false;
                                 _bGenOnDueToSheltTemp = false;
                             }
                         }

                         if(!IsNightModeRestrictOn())
                         {
                             _StartStop.StartCommand();
                             _eBTSState = STATE_BTS_GEN_START;
                         }
                         UTILS_DisableTimer(&_ReturnToMainsTimer);
                     }
                }
                else if(_MainsStatus == MAINS_HELATHY)
                {
                    UTILS_ResetTimer(&_ReturnToMainsTimer);
                    _eBTSState = STATE_BTS_GEN_OFF_MAINS_ON;
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
                break;

            case STATE_BTS_GEN_OFF_MAINS_ON:
                if(_EngineMon.IsEngineOn() == 1U)
                {
//                    _StartStop.StopCommand();
//                    _eBTSState = STATE_BTS_ENGINE_STOP;
                    _eBTSState = STATE_BTS_GEN_START;
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }
                else if(_MainsStatus == MAINS_UNHELATHY)
                {
                    OpenMainsLoad();
                    if(((!_GCUAlarms.IsBTSBattHealthy()) || (_GCUAlarms.IsShelterTempHigh()))
                        &&(!_GCUAlarms.IsCommonElectricTrip())&& (!_GCUAlarms.IsCommonShutdown()) && (!_GCUAlarms.IsCommonWarning())
                       && (!_bEmergencyStop) && (!IsNightModeRestrictOn()) && (START_STOP::ID_STATE_SS_STOP_HOLD != _StartStop.GetStartStopSMDState()))
                    {
                        if(GetPressureSensorStatusBeforeStart())
                        {
                            if(!_GCUAlarms.IsBTSBattHealthy())
                            {
                                _bGenOnDuetoBTS = true;
                                _bGenOnDueToSheltTemp = false;
                            }
                            else if(_GCUAlarms.IsShelterTempHigh())
                            {
                                _bGenOnDuetoBTS = false;
                                _bGenOnDueToSheltTemp = true;
                            }
                            else
                            {
                                _bGenOnDuetoBTS = false;
                                _bGenOnDueToSheltTemp = false;
                            }

                            _StartStop.StartCommand();
                            _eBTSState = STATE_BTS_GEN_START;
                        }
                    }
                    else
                    {
                        _eBTSState = STATE_BTS_GEN_OFF_MAINS_OFF;
                    }
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }
                else if(UTILS_GetElapsedTimeInMs(&_ReturnToMainsTimer) >=  5U)
                {
                    SwitchLoadToMains();
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
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
                break;

            case STATE_BTS_GEN_START:
                if((_MainsStatus == MAINS_HELATHY)
                    || (_GCUAlarms.IsCommonElectricTrip()) || (_GCUAlarms.IsCommonShutdown())
                    || (_GCUAlarms.IsCommonWarning())
                    || ((_GCUAlarms.IsBTSBattHealthy() && _bGenOnDuetoBTS)
                            || (_GCUAlarms.IsShelterTempLow() && _bGenOnDueToSheltTemp))
                    || IsNightModeRestrictOn())
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
                    else if (_GCUAlarms.IsCommonElectricTrip())
                    {
                        _vars.GCUState = ELECTRIC_TRIP;
                    }

                    _bGenOnDuetoBTS = false;
                    _bGenOnDueToSheltTemp = false;

                    _eBTSState = STATE_BTS_ENGINE_STOP;
                }
                else if(_EngineMon.IsWarmUpTimeExpired())
                {
                    SwitchLoadToGen();
                    _eBTSState = STATE_BTS_GEN_ON_LOAD;

                    if(_bGenOnDuetoBTS)
                    {
                        _bBattChargingOn = true;
                        _u32BattChargingCount = 0;
                        _bBattChargTmrExpired= false;
                        _bShelterTempTmrOn = false;
                    }
                    else if(_bGenOnDueToSheltTemp)
                    {
                        _bShelterTempTmrOn = true;
                        _u32ShelterTempTimerCount = 0;
                        _bShelterTempTmrExpired= false;
                        _bBattChargingOn = false;
                    }
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

            case STATE_BTS_GEN_ON_LOAD:
                if((_bBattChargingOn)&&((_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION))!= 0))
                {
                    _vars.TimerState = BATT_CHARGE_TIMER;
                }
                else if(_bShelterTempTmrOn && ((_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION))!= 0))
                {
                    _vars.TimerState = SHELTER_TEMP_TIMER;
                }

                if((_GCUAlarms.IsCommonShutdown()) || (IsNightModeRestrictOn()))
                {
                    _vars.GCUState = SHUTDOWN;
                    _StartStop.StopCommand();

                    _bOpenGenLoad = true;
                    _eBTSState = STATE_BTS_ENGINE_STOP;

                    if(_bBattChargingOn)
                    {
                        _bBattChargingOn = false;
                    }
                    if(_bShelterTempTmrOn)
                    {
                        _bShelterTempTmrOn = false;
                    }
                }
                else if((_GCUAlarms.IsCommonElectricTrip())|| _bBattChargTmrExpired
                        || (_bShelterTempTmrExpired && _GCUAlarms.IsShelterTempLow()))
                {
                    if(_bBattChargingOn && _bBattChargTmrExpired)
                    {
                        _bBattChargingOn = false;
//                        _bGenOnDuetoBTS = false;
                    }
                    else if(_bShelterTempTmrOn && _bShelterTempTmrExpired)
                    {
                        _bShelterTempTmrOn = false;
//                        _bGenOnDueToSheltTemp = false;
                    }
                    _bBattChargTmrExpired = false;
                    _bShelterTempTmrExpired = false;

                    _vars.GCUState = (_GCUAlarms.IsCommonElectricTrip()) ?
                                            ELECTRIC_TRIP : ENGINE_STOPPING;

                    _bOpenGenLoad = true;
                    _vars.TimerState = COOL_DOWN_TIMER;
                    UTILS_ResetTimer(&_EngCoolDownTimer);
                    _eBTSState = STATE_BTS_ENGINE_COOLING;
                }
                else if(_bShelterTempTmrExpired && (!_GCUAlarms.IsShelterTempLow()))
                {
                    _bShelterTempTmrOn = true;
                    _bGenOnDueToSheltTemp = true;
                    _u32ShelterTempTimerCount = 0;
                    _bShelterTempTmrExpired= false;
                    _bBattChargingOn = false;
                }
                else if(_MainsStatus == MAINS_HELATHY)
                {
                    _eBTSState = STATE_BTS_RETURN_DELAY;
                    UTILS_ResetTimer(&_ReturnToMainsTimer);
                    _vars.GCUState = ENGINE_ON_HEALTHY;
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

                //Todo : To check if following commented code is required
                // for GC2111 . There is some comment in SGC4XX NXP code as to why
                //this part of code has been added.
//                if(prvGenStartCondition()&& (!_bCloseGenContactor)&& (!_bContactorTransferOn))
//                {
//                    SwitchLoadToGen();
//                }
                break;

            case STATE_BTS_RETURN_DELAY:
                if((_GCUAlarms.IsCommonShutdown()) || (IsNightModeRestrictOn()))
                {
                    _vars.GCUState = SHUTDOWN;
                    _StartStop.StopCommand();

                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                    _bOpenGenLoad = true;
                    _eBTSState = STATE_BTS_ENGINE_STOP;

                    if(_bBattChargingOn)
                    {
                        _bBattChargingOn = false;
                    }
                    if(_bShelterTempTmrOn)
                    {
                        _bShelterTempTmrOn = false;
                    }
                }
                else if((_GCUAlarms.IsCommonElectricTrip())|| _bBattChargTmrExpired
                        || _bShelterTempTmrExpired)
                {
                    _bBattChargTmrExpired = false;
                    _bShelterTempTmrExpired = false;

                    _vars.GCUState = (_GCUAlarms.IsCommonElectricTrip()) ?
                                            ELECTRIC_TRIP : ENGINE_STOPPING;
                    _bOpenGenLoad = true;
                    _vars.TimerState = COOL_DOWN_TIMER;
                    UTILS_ResetTimer(&_EngCoolDownTimer);
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                    _eBTSState = STATE_BTS_ENGINE_COOLING;

                    if(_bBattChargingOn)
                    {
                        _bBattChargingOn = false;
//                        _bGenOnDuetoBTS = false;
                    }
                    else if(_bShelterTempTmrOn)
                    {
                        _bShelterTempTmrOn = false;
//                        _bGenOnDueToSheltTemp = false;
                    }
                }
                else if(_MainsStatus == MAINS_UNHELATHY)
                {
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                    _eBTSState = STATE_BTS_GEN_ON_LOAD;
                    _vars.GCUState = ENGINE_ON_HEALTHY;
                    _vars.TimerState = NO_TIMER_RUNNING;
                }
                else if(UTILS_GetElapsedTimeInMs(&_ReturnToMainsTimer) >=5U)
                {
                    SwitchLoadToMains();
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                    UTILS_ResetTimer(&_EngCoolDownTimer);
                    _eBTSState = STATE_BTS_ENGINE_COOLING;
                    _vars.GCUState = ENGINE_STOPPING;
                    _vars.TimerState = COOL_DOWN_TIMER;
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

            case STATE_BTS_ENGINE_COOLING:
                _hal.actuators.Activate(ACTUATOR::ACT_COOLING_ON);
                if((UTILS_GetElapsedTimeInSec(&_EngCoolDownTimer) >=
                       _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ENG_COOL_TIME)) || _GCUAlarms.IsCommonShutdown())
                {
                    if(_GCUAlarms.IsCommonShutdown())
                    {
                        _vars.GCUState = SHUTDOWN;
                    }
                    _StartStop.StopCommand();
                    UTILS_DisableTimer(&_EngCoolDownTimer);
                    _eBTSState = STATE_BTS_ENGINE_STOP;
                    _hal.actuators.Deactivate(ACTUATOR::ACT_COOLING_ON);

                    if(_bBattChargingOn)
                    {
//                        _bBattChargTmrExpired = false;
                        _bBattChargingOn = false;
//                        _bGenOnDuetoBTS = 0;
                    }
                    if(_bShelterTempTmrOn)
                    {
//                        _bShelterTempTmrExpired = 0;
                        _bShelterTempTmrOn = false;
//                        _bGenOnDueToSheltTemp = false;
                    }
                }
                else if(_GCUAlarms.IsCommonElectricTrip())
                {
                    _vars.GCUState = ELECTRIC_TRIP;
                }
                else if((_MainsStatus == MAINS_UNHELATHY)&&(_bBattChargingOn ||  _bShelterTempTmrOn) )
                {
                    SwitchLoadToGen();
                    _eBTSState = STATE_BTS_GEN_ON_LOAD;
                    _hal.actuators.Deactivate(ACTUATOR::ACT_COOLING_ON);
                    UTILS_DisableTimer(&_EngCoolDownTimer);
                    if(_bBattChargingOn)
                    {
                        _vars.TimerState = BATT_CHARGE_TIMER;
                    }
                    else if(_bShelterTempTmrOn)
                    {
                        _vars.TimerState = SHELTER_TEMP_TIMER;
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
                else
                {
                    _vars.GCUState = ENGINE_STOPPING;
                }

                if((((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
                                               ||(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE))
                                               && (!_bCloseMainsContactor) && (!_bContactorTransferOn))
                {
                     SwitchLoadToMains();
                }
//                else if((((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
//                        ||(_bRemoteStopRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE)))
//                        && (_bCloseMainsContactor) && (!_bContactorTransferOn))
//
//                {
//                    OpenMainsLoad();
//                }
                break;

            case STATE_BTS_ENGINE_STOP:
                if(_EngineMon.IsEngineOn() == 0U)
                {
                    _eBTSState = STATE_BTS_GEN_OFF_MAINS_OFF;
                    if(_GCUAlarms.IsAlarmPresent())
                    {
                       prvSetGCUState();
                    }
                }
                else
                {
                    if(((_MainsStatus == MAINS_HELATHY)&& (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
                                                   && (!_bCloseMainsContactor) && (!_bContactorTransferOn))
                    {
                         SwitchLoadToMains();
                    }
//                    else if((((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
//                            ||(_bRemoteStopRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE)))
//                            && (_bCloseMainsContactor) && (!_bContactorTransferOn))
//
//                    {
//                        OpenMainsLoad();
//                    }
                }

                if(_bGenOnDuetoBTS)
                {
                    _bGenOnDuetoBTS = false;
                }

                if(_bGenOnDueToSheltTemp)
                {
                    _bGenOnDueToSheltTemp = false;
                }
                break;

            default :
                break;
        }

        if(_eBTSState == STATE_BTS_ENGINE_COOLING)
        {
            _hal.actuators.Activate(ACTUATOR::ACT_COOLING_ON);
        }
        else
        {
            _hal.actuators.Deactivate(ACTUATOR::ACT_COOLING_ON);
        }
    }
}
bool BTS_MODE::prvGenStartCondition()
{
    /* Start the GEN when BTS is unhealthy or shelter temperature is high.*/
    if((_MainsStatus == MAINS_UNHELATHY)
        &&((!_GCUAlarms.IsBTSBattHealthy()) || (_GCUAlarms.IsShelterTempHigh())))
    {
        return true;
    }
    return false;
}


uint32_t BTS_MODE::GetBTSModeTime(BASE_MODES::TIMER_STATE_t eTimer)
{
    uint32_t RemainingTimeInSec = 0;
    switch(eTimer)
    {
       case BASE_MODES::BATT_CHARGE_TIMER:
           RemainingTimeInSec = ((((_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION)*1200) - _u32BattChargingCount) /1200) + 1);
           break;

       case BASE_MODES::SHELTER_TEMP_TIMER:
           RemainingTimeInSec = ((((_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION)*1200) - _u32ShelterTempTimerCount) /1200) + 1);
           break;

    }
    return RemainingTimeInSec;
}

void BTS_MODE::ClearBTSVars(void)
{
    _bBattChargingOn = false;
    _bShelterTempTmrOn = false;
    _u32BattChargingCount = 0;
    _u32ShelterTempTimerCount = 0;

}
bool BTS_MODE::IsGenOnDuetoBTS()
{
    return _bGenOnDuetoBTS;
}
bool BTS_MODE::IsGenOnDueToSheltTemp(void)
{
    return _bGenOnDueToSheltTemp;
}
void BTS_MODE::TurnOffBattCharging()
{
    _bBattChargingOn = false;
}
void BTS_MODE::ClearSheltTempTimer()
{
    _bShelterTempTmrOn = false;
}

void BTS_MODE::ManualToBTSOnLoad()
{
    if(!_GCUAlarms.IsBTSBattHealthy())
    {
        _bGenOnDuetoBTS = true;
        _bBattChargingOn = true;
        _u32BattChargingCount = 0;
        _bBattChargTmrExpired = false;
        _vars.TimerState = BATT_CHARGE_TIMER;
    }
    else if(_GCUAlarms.IsShelterTempHigh())
    {
        _bGenOnDuetoBTS = false;
        _bBattChargingOn = false;
        _bGenOnDueToSheltTemp = true;
        _bShelterTempTmrOn = true;
        _u32ShelterTempTimerCount = 0;
        _bShelterTempTmrExpired = false;
        _vars.TimerState = SHELTER_TEMP_TIMER;
    }
    else
    {
        _bGenOnDuetoBTS = false;
        _bGenOnDueToSheltTemp = false;
        _bBattChargTmrExpired = true;
    }
}

