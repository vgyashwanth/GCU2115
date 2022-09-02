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
_u8PrevMainsHealthyPhCnt(0),
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
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_RUN_DURATION)*60U*20) <= _u32BattChargingCount)
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
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_RUN_DURATION_MIN)*60U*20) <= _u32ShelterTempTimerCount)
            {
                _bShelterTempTmrExpired = 1;
            }
        }
        else
        {
            _u32ShelterTempTimerCount = 0;
        }
        /***END: GEN ON timer count in fuel saver mode for high shelter temp ***/


        if((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PARTIAL_HEALTHY_DETECT_EN) == CFGZ::CFGZ_ENABLE)
                && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) != CFGZ::CFGZ_1_PHASE_SYSTEM))
        {
             if(GetMainsHealthyPhaseCnt() > _u8PrevMainsHealthyPhCnt)
             {
                 _bIsHealthyPhCntIncr = true;
             }
             else if(GetMainsHealthyPhaseCnt() < _u8PrevMainsHealthyPhCnt)
             {
                 _bIsHealthyPhCntIncr = false;
             }
             _u8PrevMainsHealthyPhCnt = GetMainsHealthyPhaseCnt();
        }

        if((_bIsHealthyPhCntIncr)
                ||((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PARTIAL_HEALTHY_DETECT_EN) == CFGZ::CFGZ_DISABLE)&& (_MainsStatus == MAINS_HELATHY))
                ||((_MainsStatus == MAINS_HELATHY)
                        && ((!_bMainsPartialHealthy)
                                || (_bMainsPartialHealthy && (!_bGenOnDuetoBTS)&&(!_bGenOnDueToSheltTemp) &&((GetMainsHealthyPhaseCnt()==1)||(GetMainsHealthyPhaseCnt() == 2)))
                            )
                   )
                ||(_GCUAlarms.IsBTSBattHealthy() && (!_bGenOnDuetoBTS)&&(!_GCUAlarms.IsShelterTempHigh()))
//                        &&_GCUAlarms.IsAlarmMonEnabled(GCU_ALARMS::SHELTER_TEMP_STOP_GEN)&&_bMainsPartialHealthy)
           )
        {
            SetMainsPartialLEDstatus();
        }
        else
        {
            ClearMainsPartialLEDstatus();
        }

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
                    _StartStop.StopCommand();
                    _eBTSState = STATE_BTS_ENGINE_STOP;
                }
                else if(prvGenStartCondition())
                {
                     if((!_GCUAlarms.IsCommonElectricTrip())
                            && (!_GCUAlarms.IsCommonShutdown())
                            && (!_bEmergencyStop)&& GetPressureSensorStatusBeforeStart()&&(!IsNightModeRestrictOn()))
                     {
                         prvGenStartAction();
                     }
                     else if(_MainsStatus == MAINS_HELATHY)
                     {
                         UTILS_ResetTimer(&_ReturnToMainsTimer);
                         _eBTSState = STATE_BTS_GEN_OFF_MAINS_ON;
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
                else if(START_STOP::ID_STATE_SS_STOP_HOLD == _StartStop.GetStartStopSMDState())
                {
                    _vars.GCUState = ENGINE_STOPPING;
                }
                break;

            case STATE_BTS_GEN_OFF_MAINS_ON:
                if(_EngineMon.IsEngineOn() == 1U)
                {
                    _StartStop.StopCommand();
                    _eBTSState = STATE_BTS_ENGINE_STOP;
                }
                else if(prvGenStartCondition())
                {
                    if((!_GCUAlarms.IsCommonElectricTrip())&& (!_GCUAlarms.IsCommonShutdown())
                       && (!_bEmergencyStop)&& GetPressureSensorStatusBeforeStart() && (!IsNightModeRestrictOn()))
                    {
                        prvGenStartAction();
                    }
                    else if((_bMainsPartialHealthy)||(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE))
                    {
                        if(!_bCloseMainsContactor)
                        {
                            SwitchLoadToMains();
                        }
                    }
                    else
                    {
                        OpenMainsLoad();
                        _eBTSState = STATE_BTS_GEN_OFF_MAINS_OFF;
                    }

                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }
                else if(_MainsStatus == MAINS_UNHELATHY)
                {
                    OpenMainsLoad();
                    _eBTSState = STATE_BTS_GEN_OFF_MAINS_OFF;
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
                if((((_MainsStatus == MAINS_HELATHY)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)
                        && (!_bMainsPartialHealthy))|| _bIsHealthyPhCntIncr)
                    || (_GCUAlarms.IsCommonElectricTrip()) || (_GCUAlarms.IsCommonShutdown())
                    || (_GCUAlarms.IsBTSBattHealthy() && (!_GCUAlarms.IsShelterTempHigh()))
                    || IsNightModeRestrictOn())
                {
                    _StartStop.StopCommand();
                    if(_GCUAlarms.IsCommonShutdown())
                    {

                        _vars.GCUState = SHUTDOWN;
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
                    }
                    else if(_bGenOnDueToSheltTemp)
                    {
                        _bShelterTempTmrOn = true;
                        _u32ShelterTempTimerCount = 0;
                        _bShelterTempTmrExpired= false;
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

                if(_StartStop.IsGenMonOn() == 1U)
                {
                    _vars.TimerState = NO_TIMER_RUNNING;
                }
                break;

            case STATE_BTS_GEN_ON_LOAD:
                if((_bBattChargingOn)&&((_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_RUN_DURATION))!= 0))
                {
                    _vars.TimerState = BATT_CHARGE_TIMER;
                }
                else if(_bShelterTempTmrOn && ((_cfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_RUN_DURATION_MIN))!= 0))
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
                        || _bShelterTempTmrExpired ||(_bGenOnDueToSheltTemp && (!_GCUAlarms.IsShelterTempHigh())))
                {
                    if(_bBattChargingOn && _bBattChargTmrExpired)
                    {
                        _bBattChargingOn = false;
                        _bGenOnDuetoBTS = false;
                    }
                    else if((_bShelterTempTmrOn && _bShelterTempTmrExpired)
                            ||(_bGenOnDueToSheltTemp && (!_GCUAlarms.IsShelterTempHigh())))
                    {
                        _bShelterTempTmrOn = false;
                        _bGenOnDueToSheltTemp = false;
                    }
                    _bBattChargTmrExpired = false;
                    _bShelterTempTmrExpired = false;

                    _vars.GCUState = (_GCUAlarms.IsCommonElectricTrip()) ?
                                            ELECTRIC_TRIP : ENGINE_STOPPING;

                    _bOpenGenLoad = true;
                    _vars.TimerState = COOL_DOWN_TIMER;
                    UTILS_ResetTimer(&_EngCoolDownTimer);
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                    _eBTSState = STATE_BTS_ENGINE_COOLING;
                }
                else if(((_MainsStatus == MAINS_HELATHY)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)
                        && (!_bMainsPartialHealthy))|| _bIsHealthyPhCntIncr)
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

                if(prvGenStartCondition()&& (!_bCloseGenContactor)&& (!_bContactorTransferOn))
                {
                    SwitchLoadToGen();
                }
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
                        || _bShelterTempTmrExpired ||(_bGenOnDueToSheltTemp && (!_GCUAlarms.IsShelterTempHigh())))
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
                        _bGenOnDuetoBTS = false;
                    }
                    else if(_bShelterTempTmrOn)
                    {
                        _bShelterTempTmrOn = false;
                        _bGenOnDueToSheltTemp = false;
                    }
                }
                else if(((_MainsStatus == MAINS_UNHELATHY)|| _bMainsPartialHealthy||(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE))
                        &&(!_bIsHealthyPhCntIncr) && (!_GCUAlarms.IsBTSBattHealthy()))
                {
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                    _eBTSState = STATE_BTS_GEN_ON_LOAD;
                    _vars.GCUState = ENGINE_ON_HEALTHY;
                    _vars.TimerState = NO_TIMER_RUNNING;
                }
                else if(UTILS_GetElapsedTimeInMs(&_ReturnToMainsTimer) >=5U)
                {
                    SwitchLoadToMains();
                    OpenGenLoad(); /// Open GEN load as mains inhibit input might be active
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
                       _cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_COOL_DELAY)) || _GCUAlarms.IsCommonShutdown())
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
                        _bBattChargTmrExpired = false;
                        _bBattChargingOn = false;
                        _bGenOnDuetoBTS = 0;
                    }
                    if(_bShelterTempTmrOn)
                    {
                        _bShelterTempTmrExpired = 0;
                        _bShelterTempTmrOn = false;
                        _bGenOnDueToSheltTemp = false;
                    }
                }
                else if(_GCUAlarms.IsCommonElectricTrip())
                {
                    _vars.GCUState = ELECTRIC_TRIP;
                }
                else if((((_MainsStatus == MAINS_UNHELATHY)||_bMainsPartialHealthy) && (!_bIsHealthyPhCntIncr))&&(_bBattChargingOn ||  _bShelterTempTmrOn) )
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

                if((((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                                               ||(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE))
                                               && (!_bCloseMainsContactor) && (!_bContactorTransferOn))
                {
                     SwitchLoadToMains();
                }
//                else if((((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
//                        ||(_bRemoteStopRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
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
                    _bIsHealthyPhCntIncr = false;
                    if(_GCUAlarms.IsAlarmPresent())
                    {
                       prvSetGCUState();
                    }
                }
                else
                {
                    if(((((_MainsStatus == MAINS_HELATHY)|| _bMainsPartialHealthy) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                                                   ||(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE))
                                                   && (!_bCloseMainsContactor) && (!_bContactorTransferOn))
                    {
                         SwitchLoadToMains();
                    }
//                    else if((((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
//                            ||(_bRemoteStopRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
//                            && (_bCloseMainsContactor) && (!_bContactorTransferOn))
//
//                    {
//                        OpenMainsLoad();
//                    }
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
    /* Start the GEN when BTS is unhealthy or shelter temperature is high even if mains monitoring is disabled.*/
    if(((((_MainsStatus == MAINS_UNHELATHY)|| _bMainsPartialHealthy)&& (!_bIsHealthyPhCntIncr))
           || (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)
        )&&(!_GCUAlarms.IsBTSBattHealthy() || (_GCUAlarms.IsShelterTempHigh())))
    {
        return true;
    }
    return false;
}

void BTS_MODE::prvGenStartAction()
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
    OpenMainsLoad();
    _eBTSState = STATE_BTS_GEN_START;
    UTILS_DisableTimer(&_ReturnToMainsTimer);

}

uint32_t BTS_MODE::GetBTSModeTime(BASE_MODES::TIMER_STATE_t eTimer)
{
    uint32_t RemainingTimeInSec = 0;
    switch(eTimer)
    {
       case BASE_MODES::BATT_CHARGE_TIMER:
           RemainingTimeInSec = ((((_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_RUN_DURATION)*1200) - _u32BattChargingCount) /1200) + 1);
           break;

       case BASE_MODES::SHELTER_TEMP_TIMER:
           RemainingTimeInSec = ((((_cfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_RUN_DURATION_MIN)*1200) - _u32ShelterTempTimerCount) /1200) + 1);
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

