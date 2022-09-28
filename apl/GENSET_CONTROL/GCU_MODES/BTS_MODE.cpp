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
_stBatteryChargingTimer{0 , false},
_stShelterTempTimer{0 , false}
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
                             prvUpdateGenOnReason();
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
                        &&(!_GCUAlarms.IsCommonElectricTrip())
                        && (!_GCUAlarms.IsCommonShutdown())
                        && (!_GCUAlarms.IsCommonWarning())
                        && (!_bEmergencyStop)
                        && (!IsNightModeRestrictOn())
                        && (START_STOP::ID_STATE_SS_STOP_HOLD != _StartStop.GetStartStopSMDState()))
                    {
                        if(GetPressureSensorStatusBeforeStart())
                        {
                             prvUpdateGenOnReason();
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
                    || (_GCUAlarms.IsCommonElectricTrip())
                    || (_GCUAlarms.IsCommonShutdown())
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
                        UTILS_ResetTimer(&_stBatteryChargingTimer);
                        UTILS_DisableTimer(&_stShelterTempTimer);
                    }
                    else if(_bGenOnDueToSheltTemp)
                    {
                        UTILS_ResetTimer(&_stShelterTempTimer);
                        UTILS_DisableTimer(&_stBatteryChargingTimer);
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
                if((UTILS_IsTimerEnabled(&_stBatteryChargingTimer))&&((_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION))!= 0))
                {
                    _vars.TimerState = BATT_CHARGE_TIMER;
                }
                else if(UTILS_IsTimerEnabled(&_stShelterTempTimer) && ((_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION))!= 0))
                {
                    _vars.TimerState = SHELTER_TEMP_TIMER;
                }

                if((_GCUAlarms.IsCommonShutdown()) || (IsNightModeRestrictOn()))
                {
                    _vars.GCUState = SHUTDOWN;
                    _StartStop.StopCommand();

                    _bOpenGenLoad = true;
                    _eBTSState = STATE_BTS_ENGINE_STOP;

                    UTILS_DisableTimer(&_stBatteryChargingTimer);
                    UTILS_DisableTimer(&_stShelterTempTimer);
                }
                else if((_GCUAlarms.IsCommonElectricTrip())
                        || (UTILS_GetElapsedTimeInSec(&_stBatteryChargingTimer)>=(_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION)*60U))
                        || ((UTILS_GetElapsedTimeInSec(&_stShelterTempTimer)>=(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION)*60U))
                                && _GCUAlarms.IsShelterTempLow()))
                {
                    if(UTILS_GetElapsedTimeInSec(&_stBatteryChargingTimer)>=(_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION)*60U))
                    {
                        UTILS_DisableTimer(&_stBatteryChargingTimer);
                    }
                    else if(UTILS_GetElapsedTimeInSec(&_stShelterTempTimer)>=(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION)*60U))
                    {
                        UTILS_DisableTimer(&_stShelterTempTimer);
                    }

                    _vars.GCUState = (_GCUAlarms.IsCommonElectricTrip()) ?
                                            ELECTRIC_TRIP : ENGINE_STOPPING;

                    _bOpenGenLoad = true;
                    _vars.TimerState = COOL_DOWN_TIMER;
                    UTILS_ResetTimer(&_EngCoolDownTimer);
                    _eBTSState = STATE_BTS_ENGINE_COOLING;
                }
                else if((UTILS_GetElapsedTimeInSec(&_stShelterTempTimer)>=(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION)*60U))
                        && (!_GCUAlarms.IsShelterTempLow()))
                {
                    UTILS_ResetTimer(&_stShelterTempTimer);
                    _bGenOnDueToSheltTemp = true;
                    UTILS_DisableTimer(&_stBatteryChargingTimer);
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

                    UTILS_DisableTimer(&_stBatteryChargingTimer);
                    UTILS_DisableTimer(&_stShelterTempTimer);
                }
                else if((_GCUAlarms.IsCommonElectricTrip())
                        || (UTILS_GetElapsedTimeInSec(&_stBatteryChargingTimer)>=(_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION)*60U))
                        || (UTILS_GetElapsedTimeInSec(&_stShelterTempTimer)>=(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION)*60U)))
                {

                    _vars.GCUState = (_GCUAlarms.IsCommonElectricTrip()) ?
                                            ELECTRIC_TRIP : ENGINE_STOPPING;
                    _bOpenGenLoad = true;
                    _vars.TimerState = COOL_DOWN_TIMER;
                    UTILS_ResetTimer(&_EngCoolDownTimer);
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                    _eBTSState = STATE_BTS_ENGINE_COOLING;

                    if(UTILS_IsTimerEnabled(&_stBatteryChargingTimer))
                    {
                        UTILS_DisableTimer(&_stBatteryChargingTimer);
                    }
                    if(UTILS_IsTimerEnabled(&_stShelterTempTimer))
                    {
                        UTILS_DisableTimer(&_stShelterTempTimer);
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

                    if(UTILS_IsTimerEnabled(&_stBatteryChargingTimer))
                    {
                        UTILS_DisableTimer(&_stBatteryChargingTimer);
                    }

                    if(UTILS_IsTimerEnabled(&_stShelterTempTimer))
                    {
                        UTILS_DisableTimer(&_stShelterTempTimer);
                    }
                }
                else if(_GCUAlarms.IsCommonElectricTrip())
                {
                    _vars.GCUState = ELECTRIC_TRIP;
                }
                else if((_MainsStatus == MAINS_UNHELATHY)
                        &&((UTILS_IsTimerEnabled(&_stBatteryChargingTimer)) ||  (UTILS_IsTimerEnabled(&_stShelterTempTimer))) )
                {
                    SwitchLoadToGen();
                    _eBTSState = STATE_BTS_GEN_ON_LOAD;
                    _hal.actuators.Deactivate(ACTUATOR::ACT_COOLING_ON);
                    UTILS_DisableTimer(&_EngCoolDownTimer);
                    if(UTILS_IsTimerEnabled(&_stBatteryChargingTimer))
                    {
                        _vars.TimerState = BATT_CHARGE_TIMER;
                    }
                    else if(UTILS_IsTimerEnabled(&_stShelterTempTimer))
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
           RemainingTimeInSec = (uint32_t)(ceil((float)((_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION)*60) - UTILS_GetElapsedTimeInSec(&_stBatteryChargingTimer)) /60.0f));
           break;

       case BASE_MODES::SHELTER_TEMP_TIMER:
           RemainingTimeInSec = (uint32_t)(ceil((float)((_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION)*60) - UTILS_GetElapsedTimeInSec(&_stShelterTempTimer)) /60.0f));
           break;

       default :
           break;

    }
    return RemainingTimeInSec;
}

void BTS_MODE::ClearBTSVars(void)
{
    UTILS_DisableTimer(&_stBatteryChargingTimer);
    UTILS_DisableTimer(&_stShelterTempTimer);

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
    UTILS_DisableTimer(&_stBatteryChargingTimer);
}
void BTS_MODE::ClearSheltTempTimer()
{
    UTILS_DisableTimer(&_stShelterTempTimer);
}

void BTS_MODE::ManualToBTSOnLoad()
{
    if(!_GCUAlarms.IsBTSBattHealthy())
    {
        _bGenOnDuetoBTS = true;
        _bGenOnDueToSheltTemp = false;
        UTILS_ResetTimer(&_stBatteryChargingTimer);
        UTILS_DisableTimer(&_stShelterTempTimer);
        _vars.TimerState = BATT_CHARGE_TIMER;
    }
    else if(_GCUAlarms.IsShelterTempHigh())
    {
        _bGenOnDuetoBTS = false;
        _bGenOnDueToSheltTemp = true;
        UTILS_ResetTimer(&_stShelterTempTimer);
        UTILS_DisableTimer(&_stBatteryChargingTimer);
        _vars.TimerState = SHELTER_TEMP_TIMER;
    }
    else
    {
        _bGenOnDuetoBTS = false;
        _bGenOnDueToSheltTemp = false;
    }
}

void BTS_MODE::prvUpdateGenOnReason()
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
