/**
 * @file        [SMDAuto]
 * @brief       SMDAuto
 *              Provides common API's for functionality offered by Auto SMD.
 *              The module is implemented in class AutoSMD which is inherited
 *              from class BASE_MODES.
 *
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        1st June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#include "AUTO_MODE.h"

AUTO_MODE::AUTO_MODE(HAL_Manager &hal, ENGINE_MONITORING &EngineMon, 
                         CFGZ &cfgz,  GCU_ALARMS &GCUAlarms,
                            START_STOP &StartStop, GCU_MODE_VARS_t &vars):
BASE_MODES(hal, cfgz, GCUAlarms, EngineMon, vars),
_hal(hal),
_EngineMon(EngineMon),
_cfgz(cfgz),
_GCUAlarms(GCUAlarms),
_StartStop(StartStop),
_vars(vars)
{

}

void AUTO_MODE::Update(bool bDeviceInConfigMode)
{
    if(UTILS_GetElapsedTimeInSec(&_BaseModeUpdateTimer) >= FOUR_SEC)
    {
        UTILS_ResetTimer(&_GCUSMUpdateTimer);
        UTILS_ResetTimer(&_MainsMonUpdateTimer);
        UTILS_DisableTimer(&_BaseModeUpdateTimer);
    }

    if((_eOperatingMode == BASE_MODES::AUTO_MODE)
            && (!bDeviceInConfigMode)
            && (UTILS_GetElapsedTimeInMs(&_GCUSMUpdateTimer) >= SM_UPDATE_TIME))
    {
        BASE_MODES::Update();
        UTILS_ResetTimer(&_GCUSMUpdateTimer);
        _bRemoteStartRCVD = (_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::REMOTE_SS].bEnableMonitoring
                && _GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::REMOTE_SS].bResultInstant) ;

        _bRemoteStopRCVD = (_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::REMOTE_SS].bEnableMonitoring
                &&(!_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::REMOTE_SS].bResultInstant)) ;
        _bStartRequest = false;
        _bStopRequest = false;


        switch(_eAutoState)
        {
            case STATE_AMF_GEN_OFF_MAINS_OFF:
//                if(_bMBStopCmdReceived)
//                {
//                    _bMBStopCmdReceived = 0;
//                }
                if(_EngineMon.IsEngineOn() == 1U)
                {
                    _eAutoState = STATE_AMF_GEN_START;
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }
                else if(GenStartCondition())
                {
                    prvGensetStartAction();
                }
                else if(IsMainsHealthyOrRemoteStopRcvd())
                {
                    _eAutoState = STATE_AMF_GEN_OFF_MAINS_ON;
                    UTILS_ResetTimer(&_ReturnToMainsTimer);
                }

                if(_GCUAlarms.IsAlarmPresent())
                {
                   prvSetGCUState();
                }
                else if(START_STOP::ID_STATE_SS_ENG_OFF_OK == 
                            _StartStop.GetStartStopSMDState() )
                {
                    _vars.GCUState = ENGINE_OFF_READY;
                    _vars.TimerState = NO_TIMER_RUNNING;
                }
                break;
            case STATE_AMF_GEN_OFF_MAINS_ON:
//                if(_bMBStopCmdReceived)
//                {
//                    _bMBStopCmdReceived = 0;
//                }

                if(_EngineMon.IsEngineOn() == 1U)
                {
                    _eAutoState = STATE_AMF_GEN_START;
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }
                else if(GenStartCondition())
                {
                    prvGensetStartAction();
                }
                else if(UTILS_GetElapsedTimeInMs(&_ReturnToMainsTimer) >= (_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_RETN_TO_MAINS_DELAY)*1000U)+ 5U)
                {
                    //Todo: Decide whether we need to check _bCloseMainsContactor
                    // before giving switch load to mains command
                    if(!_bCloseMainsContactor)
                    {
                        SwitchLoadToMains();
                    }
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }

                if(_GCUAlarms.IsAlarmPresent())
                {
                   prvSetGCUState();
                }
                else if(START_STOP::ID_STATE_SS_ENG_OFF_OK == 
                            _StartStop.GetStartStopSMDState())
                {
                    _vars.GCUState = ENGINE_OFF_READY;
                    _vars.TimerState = NO_TIMER_RUNNING;
                }
                break;
            case STATE_AMF_GEN_START:
                if((IsMainsHealthyOrRemoteStopRcvd())
//                        || _bMBStopCmdReceived
                        || (_GCUAlarms.IsCommonShutdown())
                        || (_GCUAlarms.IsCommonElectricTrip())
                        || (_GCUAlarms.IsCommonWarning())
                        || IsNightModeRestrictOn())
                {
                    _StartStop.StopCommand();

//                    if(_bMBStopCmdReceived)
//                    {
//                        _bMBStopCmdReceived = false;
//                        _bMBStopCmdRequested = true;
//                    }

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
                        _vars.GCUState = WARNING;
                    }
                    else if(IsNightModeRestrictOn())
                    {
                        _vars.GCUState = EMPTY_GCU_STATE;
                    }
                    _eAutoState = STATE_AMF_ENGINE_STOP;
                }
                else if(_EngineMon.IsWarmUpTimeExpired() && _EngineMon.IsEngineCranked())
                {
                    SwitchLoadToGen();
                    _eAutoState = STATE_AMF_GEN_ON_LOAD;
//                    if((_bMBStartCmdReceived) || _bMBStartCmdRequested)
//                    {
//                        //Todo : update DG started to MODBUS status reg
//                        _bMBStartCmdRequested = false;
//                        _bMBStartCmdReceived = false;
//                    }
                }
                else if(_GCUAlarms.IsCommonNotification())
                {
                    _vars.GCUState = NOTIFICATION;
                }

                if(_StartStop.IsGenMonOn() == 1U)
                {
//                    _vars.GCUState = ENGINE_ON_HEALTHY;
                    _vars.TimerState = NO_TIMER_RUNNING;
                }

                break;

            case STATE_AMF_GEN_ON_LOAD:
                if(_GCUAlarms.IsCommonShutdown() || (IsNightModeRestrictOn()))
                {
                    prvShutdownAction();
                }
                else if(_GCUAlarms.IsCommonElectricTrip()) // || _bMBStopCmdReceived)
                {
//                    if(_bMBStopCmdReceived)
//                    {
//                        _bMBStopCmdReceived = false;
//                        _bMBStopCmdRequested = true;
//                    }

                    if(_GCUAlarms.IsCommonElectricTrip())
                    {
                        _vars.GCUState = ELECTRIC_TRIP;
                    }
                    else
                    {
                        _vars.GCUState = ENGINE_STOPPING;
                    }
                    _vars.TimerState = COOL_DOWN_TIMER;
                    _bOpenGenLoad = true;
                    UTILS_ResetTimer(&_EngCoolDownTimer);
                    _eAutoState = STATE_AMF_ENGINE_COOLING;
                }
                else if(IsMainsHealthyOrRemoteStopRcvd())
                {
                    _eAutoState = STATE_AMF_RETURN_DELAY;
                    UTILS_ResetTimer(&_ReturnToMainsTimer);
                    _vars.GCUState = ENGINE_ON_HEALTHY;
                }
                else if((_StartStop.IsGenMonOn() == 1U) || _GCUAlarms.IsCommonWarning() || _GCUAlarms.IsCommonNotification())
                {
                    if(_GCUAlarms.IsCommonWarning())
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

                    if(_StartStop.IsGenMonOn() == 1U)
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
                }

                if((IsMainsUnhealthyOrRemoteStartRcvd())
                        && (!_bCloseGenContactor) && (!_bContactorTransferOn))
                {
                    SwitchLoadToGen();
                }

                break;

            case STATE_AMF_RETURN_DELAY:
                if(_GCUAlarms.IsCommonShutdown() || (IsNightModeRestrictOn()))
                {
                    prvShutdownAction();
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                }
                else if(_GCUAlarms.IsCommonElectricTrip()) // || _bMBStopCmdReceived)
                {
//                    if(_bMBStopCmdReceived)
//                    {
//                        _bMBStopCmdReceived = false;
//                        _bMBStopCmdRequested = true;
//                    }

                    if(_GCUAlarms.IsCommonElectricTrip())
                    {
                        _vars.GCUState = ELECTRIC_TRIP;
                    }
                    else
                    {
                        _vars.GCUState = ENGINE_STOPPING;
                    }
                    _vars.TimerState = COOL_DOWN_TIMER;
                    _bOpenGenLoad = true;
                    UTILS_ResetTimer(&_EngCoolDownTimer);
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                    _eAutoState = STATE_AMF_ENGINE_COOLING;
                }
                else if(IsMainsUnhealthyOrRemoteStartRcvd())
                {
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                    _eAutoState = STATE_AMF_GEN_ON_LOAD;
                    _vars.GCUState = ENGINE_ON_HEALTHY;
                    _vars.TimerState = NO_TIMER_RUNNING;
                }
                else if(UTILS_GetElapsedTimeInMs(&_ReturnToMainsTimer) >=(_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_RETN_TO_MAINS_DELAY)*1000U)+5U)
                {
                    SwitchLoadToMains();
                    UTILS_DisableTimer(&_ReturnToMainsTimer);
                    UTILS_ResetTimer(&_EngCoolDownTimer);
                    _eAutoState = STATE_AMF_ENGINE_COOLING;
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
            case STATE_AMF_ENGINE_COOLING:
                _hal.actuators.Activate(ACTUATOR::ACT_COOLING_ON);
                if((UTILS_GetElapsedTimeInSec(&_EngCoolDownTimer) >=
                        _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ENG_COOL_TIME))
                        || (_GCUAlarms.IsCommonShutdown())
                        || (IsNightModeRestrictOn()))
                {
                    if(_GCUAlarms.IsCommonShutdown())
                    {

                        _vars.GCUState = SHUTDOWN;
                    }
                    _StartStop.StopCommand();
                    UTILS_DisableTimer(&_EngCoolDownTimer);
                    _eAutoState = STATE_AMF_ENGINE_STOP;
                    _hal.actuators.Deactivate(ACTUATOR::ACT_COOLING_ON);
                }
                else if(_GCUAlarms.IsCommonElectricTrip())
                {
                    _vars.GCUState = ELECTRIC_TRIP;
                }
                else if(IsMainsUnhealthyOrRemoteStartRcvd())
                {
                    SwitchLoadToGen();
                    _eAutoState = STATE_AMF_GEN_ON_LOAD;
                    _hal.actuators.Deactivate(ACTUATOR::ACT_COOLING_ON);
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

                if(prvIsMainsHealthyAndCloseMainsCont())
                {
                    SwitchLoadToMains();
                }
                else if(prvIsMainsFailedAndOpenMainsCont())
                {
                    _bOpenMainsLoad = true;
                }
                break;

            case STATE_AMF_ENGINE_STOP:
                if(_EngineMon.IsEngineOn() == 0U)
                {
                    _eAutoState = STATE_AMF_GEN_OFF_MAINS_OFF;
                }
                else if(prvIsMainsHealthyAndCloseMainsCont())
                {
                    SwitchLoadToMains();
                }
                else if(prvIsMainsFailedAndOpenMainsCont())
                {
                    _bOpenMainsLoad = true;
                }
                else if(_GCUAlarms.IsAlarmPresent())
                {
                   prvSetGCUState();
                }
                else
                {
                    _vars.GCUState = ENGINE_STOPPING;
                }
                break;
            default:
                break;
        }
    }
}

bool AUTO_MODE::prvIsMainsFailedAndOpenMainsCont()
{
    if((IsMainsUnhealthyOrRemoteStartRcvd())
            && (_bCloseMainsContactor) && (!_bContactorTransferOn))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool AUTO_MODE::prvIsMainsHealthyAndCloseMainsCont()
{
    if((IsMainsHealthyOrRemoteStopRcvd())
            && (!_bCloseMainsContactor) && (!_bContactorTransferOn))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void AUTO_MODE::prvGensetStartAction()
{
    //    if(_eAutoState == STATE_AMF_GEN_OFF_MAINS_ON)
    //    {
            _bOpenMainsLoad = true;
    //    }

        if(START_STOP::ID_STATE_SS_STOP_HOLD !=
                _StartStop.GetStartStopSMDState())
        {
//            if(_bMBStartCmdReceived)
//            {
//                _bMBStartCmdReceived = false;
//                _bMBStartCmdRequested = true;
//            }
            if((!_GCUAlarms.IsCommonElectricTrip())
                    && (!_GCUAlarms.IsCommonShutdown())
                    &&(!_GCUAlarms.IsCommonWarning())
                    && (!_bEmergencyStop)
                    && GetPressureSensorStatusBeforeStart()
                    && (!IsNightModeRestrictOn()))
            {
                _StartStop.StartCommand();
                _eAutoState = STATE_AMF_GEN_START;
                UTILS_DisableTimer(&_ReturnToMainsTimer);
            }
            else if(_eAutoState == STATE_AMF_GEN_OFF_MAINS_ON)
            {
                _eAutoState = STATE_AMF_GEN_OFF_MAINS_OFF;
                UTILS_DisableTimer(&_ReturnToMainsTimer);
            }

//            if(_bMBStartCmdRequested && (!_StartStop.IsStartCommand()))
//            {
//                _bMBStartCmdRequested = false;
//            }
        }
        else if(_eAutoState == STATE_AMF_GEN_OFF_MAINS_ON)
        {
            _eAutoState = STATE_AMF_GEN_OFF_MAINS_OFF;
            UTILS_DisableTimer(&_ReturnToMainsTimer);
        }

}

void AUTO_MODE::prvShutdownAction()
{
    _vars.GCUState = SHUTDOWN;
    _StartStop.StopCommand();
    _bOpenGenLoad = true;
    _eAutoState = STATE_AMF_ENGINE_STOP;
}

void AUTO_MODE::MBStartCMDReceived()
{
    _bMBStartCmdReceived = true;
}

void AUTO_MODE::MBStopCMDReceived()
{
    _bMBStopCmdReceived = true;
}

bool AUTO_MODE::IsMainsHealthyOrRemoteStopRcvd()
{
    if(((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(
            CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE)) || (_bRemoteStopRCVD &&
                (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE)))
    {
        return true;
    }

    return false;
}

bool AUTO_MODE::IsMainsUnhealthyOrRemoteStartRcvd()
{
    if(((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(
            CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE)) || (_bRemoteStartRCVD &&
                (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE)))
    {
        return true;
    }

    return false;
}
