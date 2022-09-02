/*
 * AUTO_EXERCISE_MODE.cpp
 *
 *  Created on: 10-Feb-2022
 *      Author: madhuri.abhang
 */

#include <AUTO_EXERCISE_MODE.h>
#include "../HMI/MON_UI/MON_UI.h"
bool AUTO_EXERCISE_MODE::_bSchStopped =false;

bool AUTO_EXERCISE_MODE::_bAutoExerciserSkipped = false;
stTimer AUTO_EXERCISE_MODE::_ExeDnCntTmr={0};




AUTO_EXERCISE_MODE::AUTO_EXERCISE_MODE(HAL_Manager &hal, ENGINE_MONITORING &EngineMon,
        CFGZ &cfgz,  GCU_ALARMS &GCUAlarms,
           START_STOP &StartStop, GCU_MODE_VARS_t &vars):
BASE_MODES(hal, cfgz, GCUAlarms, EngineMon, vars),
_hal(hal),
_EngineMon(EngineMon),
_cfgz(cfgz),
_GCUAlarms(GCUAlarms),
_StartStop(StartStop),
_vars(vars),
_stEvent1{0},
_stEvent2{0},
_ExeTriggerUpdateTmr{0},
_bSchOnTimeOver(false)
{


    Init();
    UTILS_ResetTimer(&_ExeTriggerUpdateTmr);


    UTILS_DisableTimer(&_ExeDnCntTmr);
}

void AUTO_EXERCISE_MODE::Update(bool bDeviceInConfigMode)
{
    static bool bPrevCoolState = false;
    if(UTILS_GetElapsedTimeInSec(&_BaseModeUpdateTimer) >= FOUR_SEC)
    {
       UTILS_ResetTimer(&_GCUSMUpdateTimer);
       UTILS_ResetTimer(&_MainsMonUpdateTimer);
       UTILS_DisableTimer(&_BaseModeUpdateTimer);
    }

    if(UTILS_GetElapsedTimeInSec(&_ExeTriggerUpdateTmr) >= 1)
    {
       UTILS_ResetTimer(&_ExeTriggerUpdateTmr);
       prvScheduleExercise();
    }

    prvMonitorExcerciserTimer();
    prvUpdateCallingForSchedulerRunOp();

    if((_eOperatingMode == BASE_MODES::AUTO_EXERCISE_MODE)
               && (!bDeviceInConfigMode)
               && (UTILS_GetElapsedTimeInMs(&_GCUSMUpdateTimer) >= SM_UPDATE_TIME))
    {
       BASE_MODES::Update();
       UTILS_ResetTimer(&_GCUSMUpdateTimer);

       _bRemoteStartRCVD = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE);
       _bRemoteStopRCVD = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE);

       _bStartRequest = false;
       _bStopRequest = false;

       switch(_eAutoExeState)
       {
           case  ID_AUTO_EXE_DG_OFF:

                prvGensetStartAction();

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
           case ID_AUTO_EXE_DG_START:
               if(_EngineMon.IsWarmUpTimeExpired() && _EngineMon.IsEngineCranked())
               {
                   if(_bLoadTransferEn)
                   {
                       SwitchLoadToGen();
                   }
                   if(!_ExeDnCntTmr.bEnabled)
                   {
                       UTILS_ResetTimer(&_ExeDnCntTmr);
                   }
                   _eAutoExeState = ID_AUTO_EXE_DG_ON_LOAD;
               }
               else if(_GCUAlarms.IsCommonShutdown()||(IsNightModeRestrictOn()))
               {
                   _StartStop.StopCommand();

                   _bOpenGenLoad = true;
                   _vars.GCUState = SHUTDOWN;
                   _eAutoExeState = ID_AUTO_EXE_FAULT;
               }
               else if(_GCUAlarms.IsCommonElectricTrip())
               {
                   _vars.GCUState = ELECTRIC_TRIP;
                   _vars.TimerState = NO_TIMER_RUNNING;
                   _bOpenGenLoad = true;
                   _StartStop.StopCommand();
                   _eAutoExeState = ID_AUTO_EXE_FAULT;
               }
               else  if(UTILS_GetElapsedTimeInSec(&_ExeDnCntTmr) >= _u32SchRemTime_sec)
               {
                   _bOpenGenLoad = true;
                   UTILS_ResetTimer(&_EngCoolDownTimer);
                   _eAutoExeState = ID_AUTO_EXE_ENGINE_COOLING;
               }
               else if(_GCUAlarms.IsCommonWarning() || _GCUAlarms.IsCommonNotification())
               {
                   if(_GCUAlarms.IsCommonWarning())
                   {
                       _vars.GCUState = WARNING;
                       _vars.TimerState = NO_TIMER_RUNNING;
                   }
                   else if(_GCUAlarms.IsCommonNotification())
                   {
                       _vars.GCUState = NOTIFICATION;
                   }
               }
               else if(_StartStop.IsGenMonOn())
               {
                   _vars.GCUState = ENGINE_ON_HEALTHY;
                   _vars.TimerState = NO_TIMER_RUNNING;
               }
               break;
           case ID_AUTO_EXE_DG_ON_LOAD:
               if(_GCUAlarms.IsCommonShutdown() || (IsNightModeRestrictOn()))
               {

                   prvShutdownAction();
               }
               else if(_GCUAlarms.IsCommonElectricTrip())
               {
                   _vars.GCUState = ELECTRIC_TRIP;
                   _vars.TimerState = COOL_DOWN_TIMER;
                   _bOpenGenLoad = true;
                   UTILS_ResetTimer(&_EngCoolDownTimer);
                   _eAutoExeState = ID_AUTO_EXE_ENGINE_COOLING;
                   bPrevCoolState = true;
               }
               else if((UTILS_GetElapsedTimeInSec(&_ExeDnCntTmr) >= _u32SchRemTime_sec) ||(!UTILS_IsTimerEnabled(&_ExeDnCntTmr)))
               {
                   if(((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)) ||
                           (_bRemoteStartRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
                   {
                       SwitchLoadToGen();
                       _eAutoExeState = ID_AUTO_EXE_DG_OFF;
                       _eAutoState = STATE_AMF_GEN_ON_LOAD;
                       _eOperatingMode = BASE_MODES::AUTO_MODE;
                       prvSetExercise();
                   }
                   else
                   {
                       SwitchLoadToMains();
                       _vars.TimerState = COOL_DOWN_TIMER;
                       UTILS_ResetTimer(&_EngCoolDownTimer);
                       _eAutoExeState = ID_AUTO_EXE_ENGINE_COOLING;
                   }
               }
               else if(_GCUAlarms.IsCommonWarning() ||_GCUAlarms.IsCommonNotification())
               {
                   if(_GCUAlarms.IsCommonWarning())
                   {
                       _vars.GCUState = WARNING;
                   }
                   else if(_GCUAlarms.IsCommonNotification())
                   {
                       _vars.GCUState = NOTIFICATION;
                   }
                   else if(_StartStop.IsGenMonOn())
                   {
                       _vars.GCUState = ENGINE_ON_HEALTHY;
                   }

                   if(_StartStop.IsGenMonOn())
                    {
                        _vars.TimerState = NO_TIMER_RUNNING;
                    }
               }
               else if(_StartStop.IsGenMonOn())
               {
                   _vars.GCUState = ENGINE_ON_HEALTHY;
                   _vars.TimerState = NO_TIMER_RUNNING;
               }

               if(!_bLoadTransferEn)
               {
                   if((((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)) ||
                      (_bRemoteStartRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
                      && (!_bContactorTransferOn) &&((!_bCloseGenContactor) ||_bCloseMainsContactor)
                    )
                   {
                       SwitchLoadToGen();
                   }
                   else if ((((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                           ||(_bRemoteStopRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
                           && (!_bCloseMainsContactor) && (!_bContactorTransferOn))
                   {
                       SwitchLoadToMains();
                   }
               }
               else if(_bLoadTransferEn && (!_bCloseGenContactor))
               {
                   SwitchLoadToGen();
               }

               break;
           case ID_AUTO_EXE_ENGINE_COOLING:
               _hal.actuators.Activate(ACTUATOR::ACT_COOLING_ON);
               if((UTILS_GetElapsedTimeInSec(&_EngCoolDownTimer) >=
                      _cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_COOL_DELAY)) || _GCUAlarms.IsCommonShutdown()
                       || (IsNightModeRestrictOn()))
              {
                   UTILS_DisableTimer(&_EngCoolDownTimer);
                   _StartStop.StopCommand();
                   if(_GCUAlarms.IsCommonShutdown() || _GCUAlarms.IsCommonElectricTrip() || bPrevCoolState)
                   {
                       if(_GCUAlarms.IsCommonShutdown() )
                       {

                           _vars.GCUState = SHUTDOWN;
                       }
                       else if( _GCUAlarms.IsCommonElectricTrip())
                       {
                           _vars.GCUState = ELECTRIC_TRIP;
                       }
                       _eAutoExeState = ID_AUTO_EXE_FAULT;
                       bPrevCoolState = false;
                   }
                   else
                   {
                       _eAutoExeState = ID_AUTO_EXE_ENGINE_STOP;
                   }
                   _hal.actuators.Deactivate(ACTUATOR::ACT_COOLING_ON);
              }
              else if(_GCUAlarms.IsCommonElectricTrip())
              {
                  _vars.GCUState = ELECTRIC_TRIP;
              }
              else if(((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)) ||
                      (_bRemoteStartRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
              {
                  SwitchLoadToGen();
                  UTILS_DisableTimer(&_EngCoolDownTimer);
                 _eAutoExeState = ID_AUTO_EXE_DG_OFF;
                 _eAutoState = STATE_AMF_GEN_ON_LOAD;
                 _eOperatingMode = BASE_MODES::AUTO_MODE;
                 prvSetExercise();
                 bPrevCoolState = false;

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

               break;
           case ID_AUTO_EXE_FAULT:

               if ((((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                                              ||(_bRemoteStopRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
                                              && (!_bCloseMainsContactor) && (!_bContactorTransferOn))
               {
                    SwitchLoadToMains();
               }
               else if ((((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                       ||(_bRemoteStartRCVD  && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
                       && (_bCloseMainsContactor) && (!_bContactorTransferOn))

               {
                   OpenMainsLoad();
               }
               else
               {
                   prvSetGCUState();
               }

               if(UTILS_GetElapsedTimeInSec(&_ExeDnCntTmr) >= _u32SchRemTime_sec)
               {
                   UTILS_DisableTimer(&_ExeDnCntTmr);
                   _eAutoState = STATE_AMF_GEN_OFF_MAINS_ON;
                   _eOperatingMode = BASE_MODES::AUTO_MODE;
                   prvSetExercise();
               }
               else
               {
                   _eAutoExeState = ID_AUTO_EXE_DG_OFF;
               }


               break;
           case ID_AUTO_EXE_ENGINE_STOP:
               if(_EngineMon.IsEngineOn() == 0U)
               {
                   _eOperatingMode = BASE_MODES::AUTO_MODE;
                   _eAutoState = STATE_AMF_GEN_OFF_MAINS_ON;
                   prvSetExercise();
                   bPrevCoolState = false;
               }
               else if((((_MainsStatus == MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)) ||
                   (_bRemoteStopRCVD && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)))
                   && (!_bContactorTransferOn) &&(!_bCloseMainsContactor)
                 )
                {
                   SwitchLoadToMains();
                }
                else if (((_MainsStatus == MAINS_UNHELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
                        && (_bCloseMainsContactor) && (!_bContactorTransferOn))
                {
                   OpenMainsLoad();
                }

               if((!_GCUAlarms.IsCommonShutdown()) || (!_GCUAlarms.IsCommonElectricTrip()) || (!_GCUAlarms.IsCommonWarning())
                       ||(!_GCUAlarms.IsCommonNotification()))
               {
                   _vars.GCUState = ENGINE_OFF_READY;
               }

               break;

           default: break;
       }

    }
}

void  AUTO_EXERCISE_MODE::Init()
{
    _stEvent1.u8Enable = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_EN);
    _stEvent1.u8Occurence =_cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_OCCURENCE);
    if(WEEKLY == (_stEvent1.u8Occurence))
    {
        (_stEvent1.u8DayOfWeek) = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_START_DAY) ;
    }
    else if (MONTHLY == (_stEvent1.u8Occurence))
    {
        (_stEvent1.u8Day) = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_START_DAY);
    }
    _stEvent1.u32StartTime = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_START_TIME);
    _stEvent1.u32OnDuration = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_ON_DURATION);
    _stEvent1.u8LoadTrans = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_LOAD_TRANSFER);

    _stEvent2.u8Enable = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_EN);
    _stEvent2.u8Occurence =_cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_OCCURENCE);

    if(WEEKLY == (_stEvent2.u8Occurence))
    {
      (_stEvent2.u8DayOfWeek) = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_START_DAY) ;
    }
    else if (MONTHLY == (_stEvent2.u8Occurence))
    {
      (_stEvent2.u8Day) = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_START_DAY);
    }
    _stEvent2.u32StartTime = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_START_TIME);
    _stEvent2.u32OnDuration = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_ON_DURATION);
    _stEvent2.u8LoadTrans = _cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_LOAD_TRANSFER);


    if(1 == _stEvent1.u8Enable)
    {
        prvSetValidDateAndTimeExercise(&_stEvent1);
    }

    if(1 == _stEvent2.u8Enable)
    {
        prvSetValidDateAndTimeExercise(&_stEvent2);
    }

    prvSetExercise();
}


void AUTO_EXERCISE_MODE::EnableAutoExerciser(void)
{
    _eAutoExeState = ID_AUTO_EXE_DG_OFF;
    _eOperatingMode = BASE_MODES::AUTO_EXERCISE_MODE;

    if((_eAutoState == STATE_AMF_GEN_ON_LOAD) || (_eAutoState == STATE_AMF_RETURN_DELAY)
            ||(_eAutoState == STATE_AMF_ENGINE_COOLING))
    {
        _eAutoExeState = ID_AUTO_EXE_DG_ON_LOAD;
        UTILS_DisableTimer(&_ReturnToMainsTimer);
        UTILS_DisableTimer(&_EngCoolDownTimer);
        if(!_ExeDnCntTmr.bEnabled)
        {
            UTILS_ResetTimer(&_ExeDnCntTmr);
        }
    }
    else
    {
        _eAutoExeState = ID_AUTO_EXE_DG_OFF;
    }
}

void AUTO_EXERCISE_MODE::prvGensetStartAction()
{
    if((!_GCUAlarms.IsCommonElectricTrip())
            && (!_GCUAlarms.IsCommonShutdown()) && (!_bEmergencyStop)
            && GetPressureSensorStatusBeforeStart()
            && (!IsNightModeRestrictOn()))
    {
        _StartStop.StartCommand();
        _eAutoExeState = ID_AUTO_EXE_DG_START;

    }
    else
    {
        _eAutoExeState = ID_AUTO_EXE_FAULT;
    }


}


void AUTO_EXERCISE_MODE::prvShutdownAction()
{
    _vars.GCUState = SHUTDOWN;
    _StartStop.StopCommand();
    _bOpenGenLoad = true;
    _eAutoExeState = ID_AUTO_EXE_FAULT;
}

void AUTO_EXERCISE_MODE::prvSetNextExercise(EXERCISE_t *pExercise)
{
    uint32_t u32Seconds;
    RTC::TIME_t stExerciserDateTime = {0};

    (pExercise -> u8Set) = false;
    (pExercise -> u8Status) = UNSERVED;

    stExerciserDateTime.u8Hour   = pExercise -> u32StartTime /100;
    stExerciserDateTime.u8Minute = ((pExercise -> u32StartTime) % 100U);
    stExerciserDateTime.u8Day    = (pExercise -> u8Day);
    stExerciserDateTime.u8Month  = (pExercise -> u8Month);
    stExerciserDateTime.u16Year   = (pExercise -> u16Year);

    /* Get number of seconds till exercise date and time*/
    prvConvertTimeAndDateToSeconds(&stExerciserDateTime, &u32Seconds);

    if((pExercise -> u8Occurence)!= MONTHLY)
    {
        if((pExercise -> u8Occurence)== DAILY)
        {
            u32Seconds += SECONDS_IN_DAY;
        }
        else
        {
            u32Seconds += SECONDS_IN_WEEK;
        }
        prvConvertSecondsToDateAndTime(&stExerciserDateTime, u32Seconds);
        (pExercise -> u8Day)       = stExerciserDateTime.u8Day;
        (pExercise -> u8Month)     = stExerciserDateTime.u8Month;
        (pExercise -> u16Year)     = stExerciserDateTime.u16Year;
        (pExercise -> u8DayOfWeek) = stExerciserDateTime.u8DayOfWeek;
    }
    else
    {
        (pExercise -> u8Month)++;
        if(MAX_NUMBER_OF_MONTHS_IN_YEAR < (pExercise -> u8Month))
        {
            (pExercise -> u8Month) = 1U;
            (pExercise -> u16Year)++;
        }
    }
}

uint8_t AUTO_EXERCISE_MODE::prvCmpExercises(EXERCISE_t *pstExercise1, EXERCISE_t *pstExercise2)
{
  uint8_t u8RetVal = 0;

  if((pstExercise1 -> u16Year) == (pstExercise2 -> u16Year))
  {
      if(pstExercise1 -> u8Month == (pstExercise2 -> u8Month))
      {
          if((pstExercise1 -> u8Day) == (pstExercise2 -> u8Day))
          {
              if((pstExercise1 -> u32StartTime) == (pstExercise2 -> u32StartTime))
              {
                  u8RetVal = 0;
              }
              else if((pstExercise1 -> u32StartTime) < (pstExercise2 -> u32StartTime))
              {
                  u8RetVal = 1;
              }
              else
              {
                  u8RetVal = 2;
              }
          }
          else if((pstExercise1 -> u8Day) < (pstExercise2 -> u8Day))
          {
              u8RetVal = 1;
          }
          else
          {
              u8RetVal = 2;
          }
      }
      else if((pstExercise1 -> u8Month) < (pstExercise2 -> u8Month))
      {
          u8RetVal = 1;
      }
      else
      {
          u8RetVal = 2;
      }
  }
  else if((pstExercise1 -> u16Year) < (pstExercise2 -> u16Year))
  {
      u8RetVal = 1;
  }
  else
  {
      u8RetVal = 2;
  }

  return(u8RetVal);
}


void AUTO_EXERCISE_MODE::prvAssignNextExerToRun(uint8_t u8CmprsnVal, EXERCISE_t *pstEvent1, EXERCISE_t *pstEvent2)
{
  if(1U == u8CmprsnVal)
  {
      pstNextExerciseToRun = pstEvent1;
  }
  else if(2U == u8CmprsnVal)
  {
      pstNextExerciseToRun = pstEvent2;
  }
  else
  {
      /* This block executes when both events are scheduled on same time,
       * event 1 is served by default. */
      pstNextExerciseToRun = pstEvent1;
      (pstEvent2 -> u8Status) = SERVED;
  }
}


void AUTO_EXERCISE_MODE::prvSetExercise(void)
{
  uint8_t u8RetVal;

    /* Initialise exercisers for next schedule*/
    if(SERVED == _stEvent1.u8Status)
    {
        prvSetNextExercise(&_stEvent1);
    }

    if(SERVED == _stEvent2.u8Status)
    {
      prvSetNextExercise(&_stEvent2);
    }

    /* If both events are enabled, compare both the scheduled events and set latest */
    if((1U == _stEvent1.u8Enable) && (1U == _stEvent2.u8Enable))
    {
        /* SGC420-19 */
        if(prvIsSetTimeLessThanCurrentTime(&_stEvent1))
        {
            prvSetNextExercise(&_stEvent1);
        }
        if(prvIsSetTimeLessThanCurrentTime(&_stEvent2))
        {
          prvSetNextExercise(&_stEvent2);
        }

      u8RetVal = prvCmpExercises(&_stEvent1, &_stEvent2);
      prvAssignNextExerToRun(u8RetVal, &_stEvent1, &_stEvent2);
    }
  else if(1U == _stEvent1.u8Enable)
  {
      pstNextExerciseToRun = &_stEvent1;
  }
  else if(1U == _stEvent2.u8Enable)
  {
      pstNextExerciseToRun = &_stEvent2;
  }

    if((1U == _stEvent1.u8Enable) || (1U == _stEvent2.u8Enable))
   {
        /* <LDRA Phase code 128 D> <Global pointer not checked within this procedure: Resolved.>
         * <Verified by: Nidhi Alugu> <04/03/2020> */
        if(pstNextExerciseToRun != NULL)
        {
            (pstNextExerciseToRun -> u8Set) = 1;
        }

        if(pstNextExerciseToRun == &_stEvent1)
        {
            _stEvent2.u8Set = 0;
        }
        else if(pstNextExerciseToRun == &_stEvent2)
        {
            _stEvent1.u8Set = 0;
        }

        if(pstNextExerciseToRun -> u8Set)
        {
           _u16SchOnTimeHr = (uint16_t)((pstNextExerciseToRun -> u32OnDuration)/100);
           _u16SchOnTimeMin = (uint16_t)((pstNextExerciseToRun -> u32OnDuration) % 100U);
           /* <LDRA Phase code 332 S> <Widening cast on complex integer expression (MR): Resolved.>
            * <Verified by: Nidhi Alugu> <11/03/2020> */
           _u32SchRemTime_sec = (uint32_t)((((uint32_t)_u16SchOnTimeHr * 60) + _u16SchOnTimeMin)*60);
           _bLoadTransferEn = (bool)(pstNextExerciseToRun -> u8LoadTrans);
        }
   }


}


void AUTO_EXERCISE_MODE::prvScheduleExercise(void)
{
    RTC::TIME_t stCurrentTime;
    _hal.ObjRTC.GetTime(&stCurrentTime);

    if((((1U == _stEvent1.u8Set)&&((1U == _stEvent1.u8Enable)))
            || ((1U == _stEvent2.u8Set)&&((1U == _stEvent2.u8Enable))))
     && (_bSchGenStart == false))
    {
        if((stCurrentTime.u8Day    == (pstNextExerciseToRun -> u8Day))
         &&(stCurrentTime.u8Month  == (pstNextExerciseToRun -> u8Month))
         &&(stCurrentTime.u16Year   == (pstNextExerciseToRun -> u16Year))
         &&(stCurrentTime.u8Hour   == (pstNextExerciseToRun -> u32StartTime)/100)
         &&(stCurrentTime.u8Minute == ((pstNextExerciseToRun -> u32StartTime % 100))
         &&(UNSERVED == (pstNextExerciseToRun -> u8Status))))
        {
            _bSchStopped = false;
            _bSchGenStart = true;
            _bSchOnTimeOver = false;
            (pstNextExerciseToRun -> u8Status) = SERVED;
        }
    }
}

void AUTO_EXERCISE_MODE::prvDayOfWeekOnDate(RTC::TIME_t *pTimeStamp)
{
    uint32_t u32Second = 0;

    prvConvertTimeAndDateToSeconds(pTimeStamp, &u32Second);
    prvConvertSecondsToDateAndTime(pTimeStamp, u32Second);
}

void AUTO_EXERCISE_MODE::prvDateOnDayOfWeek(RTC::TIME_t *pstCurrentTime, uint8_t u8DayOfWeek)
{
    uint32_t u32Second = 0;
    uint8_t u8DaysOffset;

    _hal.ObjRTC.GetTime(pstCurrentTime);

    if(u8DayOfWeek == (pstCurrentTime -> u8DayOfWeek))
    {
        u8DaysOffset = 0;
    }
    else if(u8DayOfWeek < (pstCurrentTime -> u8DayOfWeek))
    {
        u8DaysOffset = (7U - ((uint8_t)(pstCurrentTime -> u8DayOfWeek))) + u8DayOfWeek;
    }
    else
    {
        u8DaysOffset = u8DayOfWeek - (pstCurrentTime -> u8DayOfWeek);
    }

    prvConvertTimeAndDateToSeconds(pstCurrentTime, &u32Second);
    u32Second = u32Second + NO_OF_SECOND_IN_DAYS(u8DaysOffset);
    prvConvertSecondsToDateAndTime(pstCurrentTime, u32Second);
}

void AUTO_EXERCISE_MODE::prvIncrementDateByWeek(RTC::TIME_t *pstTimeStamp)
{
    uint32_t u32Second = 0;

    prvConvertTimeAndDateToSeconds(pstTimeStamp, &u32Second);
    u32Second = u32Second + SECONDS_IN_WEEK;
    prvConvertSecondsToDateAndTime(pstTimeStamp, u32Second);
}


void  AUTO_EXERCISE_MODE::prvIncrementTimeStampMonth(RTC::TIME_t *pstTimeStamp)
{
    (pstTimeStamp -> u8Month)++;
    if(MAX_NUMBER_OF_MONTHS_IN_YEAR < (pstTimeStamp -> u8Month))
    {
        (pstTimeStamp -> u8Month) = JANUARY;
        (pstTimeStamp -> u16Year)++;
    }
}

void AUTO_EXERCISE_MODE::prvSetValidDateAndTimeExercise(EXERCISE_t *pstExercise)
{
    uint8_t u8StartHr, u8StartMin;
    RTC::TIME_t stCurrentTime;
    uint8_t *pu8MonthArray;

      _hal.ObjRTC.GetTime(&stCurrentTime);
    u8StartHr = (uint8_t)(pstExercise -> u32StartTime /100);
    u8StartMin = (uint8_t)(pstExercise -> u32StartTime % 100);

    if(DAILY == (pstExercise -> u8Occurence))
    {
        /* If current time passed the exercise's time, increment day*/
        if(((u8StartHr == stCurrentTime.u8Hour) && (u8StartMin <= stCurrentTime.u8Minute))
         || (u8StartHr < stCurrentTime.u8Hour))
        {
            /* Just increment day */
            stCurrentTime.u8Day++;

            if(stCurrentTime.u16Year & 3U)
            {
                /* For non-leap year */
                pu8MonthArray = (uint8_t *)&ULY[0];
            }
            else
            {
                /* For leap year */
                pu8MonthArray = (uint8_t *)&LY[0];
            }

            if(stCurrentTime.u8Day > *(pu8MonthArray + (stCurrentTime.u8Month)))
            {
                stCurrentTime.u8Month++;
                stCurrentTime.u8Day = 1;
                if(stCurrentTime.u8Month > MAX_NUMBER_OF_MONTHS_IN_YEAR)
                {
                    stCurrentTime.u8Month = JANUARY;
                    stCurrentTime.u16Year++;
                }
            }
        }
        else
        {
            ;
        }
        (pstExercise -> u8Day) = stCurrentTime.u8Day;
        /* Update both month and year according to new calculations*/
        (pstExercise -> u8Month) = stCurrentTime.u8Month;
        (pstExercise -> u16Year) = stCurrentTime.u16Year;
    }
    else if(WEEKLY == (pstExercise -> u8Occurence))
    {
        if((pstExercise -> u8DayOfWeek) == stCurrentTime.u8DayOfWeek)
        {
            if(((u8StartHr == stCurrentTime.u8Hour) && (u8StartMin <= stCurrentTime.u8Minute))
             || (u8StartHr < stCurrentTime.u8Hour))
            {
                /* If current time has passed the exercisers time */
                /* then increment the date by a week */
                prvIncrementDateByWeek(&stCurrentTime);
                (pstExercise -> u8Day) = stCurrentTime.u8Day;
            }
            else
            {
                /* If current time has not passed the exercise time, use current date*/
                (pstExercise -> u8Day) = stCurrentTime.u8Day;
            }
        }
        else
        {
            /* If current day of week is not equal to the exercise day of week, get date on next same day of week*/
            prvDateOnDayOfWeek(&stCurrentTime, (pstExercise -> u8DayOfWeek));
            (pstExercise -> u8Day) = stCurrentTime.u8Day;
        }

        /* Update both month and year according to new calculations*/
        (pstExercise -> u8Month) = stCurrentTime.u8Month;
        (pstExercise -> u16Year) = stCurrentTime.u16Year;
    }
    /* If exercise is occurring monthly, execute following block*/
    else if(MONTHLY == (pstExercise -> u8Occurence))
    {
        /* If current date has passed the exercise date*/
        if((pstExercise -> u8Day) < stCurrentTime.u8Day)
        {
            /* Increment date by month*/
            prvIncrementTimeStampMonth(&stCurrentTime);
        }
        /* else if current date and exercise date are equal, check for time condition*/
        else if((pstExercise -> u8Day) == stCurrentTime.u8Day)
        {
            /* If current time has passed the exercise starti time*/
            if(((u8StartHr == stCurrentTime.u8Hour) && (u8StartMin <= stCurrentTime.u8Minute))
             || (u8StartHr < stCurrentTime.u8Hour))
            {
                /* Increment time stamp by month*/
                prvIncrementTimeStampMonth(&stCurrentTime);
            }
            else
            {
                ;
            }
        }
        else
        {
            ;
        }
        /* Following code it to get the day of week on the day of month, it  is just to fill the day of week member,
         * it is not taken in consideration, but to keep it accurate and in sync with day of month*/
        stCurrentTime.u8Day = (pstExercise -> u8Day);
        prvDayOfWeekOnDate(&stCurrentTime);
        (pstExercise -> u8DayOfWeek) = stCurrentTime.u8DayOfWeek;

        /* Update both month and year according to new calculations*/
        (pstExercise -> u8Month) = stCurrentTime.u8Month;
        (pstExercise -> u16Year) = stCurrentTime.u16Year;
    }
}

void AUTO_EXERCISE_MODE::prvConvertSecondsToDateAndTime(RTC::TIME_t *TimePtr, uint32_t Seconds)
{
    uint32_t x;
    uint32_t Days;

    Days = Seconds / SECONDS_IN_DAY;                    /* Days */

    Seconds = Seconds % SECONDS_IN_DAY;                 /* Seconds left */
    (TimePtr -> u8Hour) =(uint8_t) (Seconds / SECONDS_IN_HOUR);      /* Hours */

    Seconds = Seconds % SECONDS_IN_HOUR;                /* Seconds left */
    (TimePtr -> u8Minute) =(uint8_t) (Seconds / SECONDS_IN_MINUTE);  /* Minutes */

    (TimePtr -> u8Second) =(uint8_t)( Seconds % SECONDS_IN_MINUTE);  /* Seconds */

    (TimePtr -> u8DayOfWeek) = (uint8_t)((Days + 6U) % NO_OF_DAYS_PER_WEEK); /* u8Day of week */
    (TimePtr -> u16Year) = (4U * (Days / ((4U * DAYS_IN_NONLEAPYEAR) + 1U))) + VALID_YEAR_RANGE_LOWER_LIMIT; /* u16Year */

    Days = Days % ((4U * DAYS_IN_NONLEAPYEAR) + 1U);
    if(Days == ((0U * DAYS_IN_NONLEAPYEAR) + MAX_NUMBER_OF_MINUTES_IN_HOUR))
    {
        (TimePtr -> u8Day) = DAYS_IN_FEB_LEAP;
        (TimePtr -> u8Month) = MONTH_FEB;
        return;
    }
    else if(Days > ((0U * DAYS_IN_NONLEAPYEAR) + MAX_NUMBER_OF_MINUTES_IN_HOUR))
    {
        Days--;
    }
    else
    {}

    x =  Days / DAYS_IN_NONLEAPYEAR;
    (TimePtr -> u16Year) += (uint16_t)x;
    Days -= x * DAYS_IN_NONLEAPYEAR;

    for(x = 1U; x <= MAX_NUMBER_OF_MONTHS_IN_YEAR; x++)
    {
        if(Days < ULY[x])
        {
            (TimePtr -> u8Month) =(uint8_t) x;
            break;
        }
        else
        {
            Days -= ULY[x];
        }
    }
    (TimePtr -> u8Day) =(uint8_t)( Days + 1U);
}


 bool AUTO_EXERCISE_MODE::prvConvertTimeAndDateToSeconds(RTC::TIME_t *TimePtr, uint32_t *SecondsPtr)
{
    uint32_t Seconds;                    /* Number of seconds till 1.1.2000 */

    if(((TimePtr -> u16Year) < VALID_YEAR_RANGE_LOWER_LIMIT) || ((TimePtr -> u16Year) > VALID_YEAR_RANGE_HIGHER_LIMIT)
    || ((TimePtr -> u8Month) > MAX_NUMBER_OF_MONTHS_IN_YEAR) || ((TimePtr -> u8Month) == 0U)
    || ((TimePtr -> u8Day) > MAX_NUMBER_OF_DAYS_IN_MONTHS) || ((TimePtr -> u8Day) == 0U))
    {
        /* Test the correctness of 3 input parameters */
        return false;
    }

    if((TimePtr -> u16Year) & 3U)
    {
        /* Non-leap u16Year */
        if(ULY[(TimePtr -> u8Month)] < (TimePtr -> u8Day))
        {
            return false;
        }
    }
    else
    {
        /* Leap u16Year */
        if(LY[(TimePtr -> u8Month)] < (TimePtr -> u8Day))
        {
            return false;
        }
    }

    /* Compute number of days from 2000 till given year */
    Seconds = (((TimePtr -> u16Year) - VALID_YEAR_RANGE_LOWER_LIMIT) * DAYS_IN_NONLEAPYEAR)
            + ((((TimePtr -> u16Year) - VALID_YEAR_RANGE_LOWER_LIMIT) + 3U) / 4U);

    Seconds += MONTH_DAYS[(TimePtr -> u8Month)];  /* Add number of days till given month */
    Seconds += (TimePtr -> u8Day);                /* Add days in given month */

    if(((TimePtr -> u16Year) & 3U) || ((TimePtr -> u8Month) <= MONTH_FEB))
    {
        /* For non-leap year or month <= 2, decrement day counter */
        Seconds--;
    }
    Seconds = (Seconds * SECONDS_IN_DAY) + ((TimePtr -> u8Hour) * SECONDS_IN_HOUR) +
              ((TimePtr -> u8Minute) * SECONDS_IN_MINUTE) + (TimePtr -> u8Second);
    *SecondsPtr = Seconds;

    return true;
}

uint8_t AUTO_EXERCISE_MODE::prvIsSetTimeLessThanCurrentTime(EXERCISE_t *pstExercise)
{
    uint8_t u8Return = 0;
    RTC::TIME_t stExerciserDateTime = {0};
    uint32_t u32EventStartTimeSec, u32CurrentTimeSec;
    RTC::TIME_t stCurrentTime;

    _hal.ObjRTC.GetTime(&stCurrentTime);

    //Calculate seconds for first event
    stExerciserDateTime.u8Hour   = (uint8_t)(pstExercise->u32StartTime / 100U);
    stExerciserDateTime.u8Minute  = (uint8_t)(pstExercise->u32StartTime % 100U);
    stExerciserDateTime.u8Day     = (pstExercise->u8Day);
    stExerciserDateTime.u8Month   = (pstExercise->u8Month);
    stExerciserDateTime.u16Year   = (pstExercise->u16Year);
    prvConvertTimeAndDateToSeconds(&stExerciserDateTime, &u32EventStartTimeSec);

    //Calculate seconds for current time
    prvConvertTimeAndDateToSeconds(&stCurrentTime, &u32CurrentTimeSec);

    if(u32CurrentTimeSec > u32EventStartTimeSec)
    {
        u8Return = 1;
    }

    return u8Return;
}
/*************************************************************************************************/


bool AUTO_EXERCISE_MODE::IsExerciserStarted()
{
    return _bSchGenStart;
}

void AUTO_EXERCISE_MODE::ChageAutoExeSMDState(BASE_MODES::AUTO_EXE_STATE_t estate)
{
    _eAutoExeState = estate;
}

void AUTO_EXERCISE_MODE::StopAutoExe()
{
    _bSchStopped = true;
}

void AUTO_EXERCISE_MODE::GetExerciserInfo(EXERCISE_t* pstTemp, uint8_t u8TempExeType)
{
    switch(u8TempExeType)
    {
        case  MON_UI::DISP_AUTO_EXERCISE_1:
            memcpy(pstTemp, &_stEvent1, sizeof(EXERCISE_t));
            break;
        case MON_UI::DISP_AUTO_EXERCISE_2:
            memcpy(pstTemp, &_stEvent2, sizeof(EXERCISE_t));
            break;
        default: break;
    }
}

void AUTO_EXERCISE_MODE::prvMonitorExcerciserTimer()
{
    if(_bSchGenStart)
    {
        if(_eOperatingMode == AUTO_MODE)
        {
            EnableAutoExerciser();
            uint32_t u32TempTime      =   (uint32_t)(((_u16SchOnTimeHr * 60) + _u16SchOnTimeMin)*60);
            if(_ExeDnCntTmr.bEnabled )
            {
                _u32SchRemTime_sec =  u32TempTime - UTILS_GetElapsedTimeInSec(&_ExeDnCntTmr) ;
            }
            else
            {
                _u32SchRemTime_sec =u32TempTime;
            }
            if(_eAutoExeState != ID_AUTO_EXE_DG_ON_LOAD)
            {
                UTILS_DisableTimer(&_ExeDnCntTmr);
            }
        }
        else if(_eOperatingMode == MANUAL_MODE)
        {
            if(!_ExeDnCntTmr.bEnabled)
            {
                UTILS_ResetTimer(&_ExeDnCntTmr);
            }
        }

        if((UTILS_GetElapsedTimeInSec(&_ExeDnCntTmr) >= _u32SchRemTime_sec) || _bSchStopped)
        {
            UTILS_DisableTimer(&_ExeDnCntTmr);
            _bSchGenStart = false;
            _u32SchRemTime_sec = 0;
            if(_bSchStopped)
            {
                prvSetExercise();
            }
        }
    }
}


uint8_t  AUTO_EXERCISE_MODE::GetRunningExeType(void)
{
    if(_stEvent1.u8Set)
    {
       return 1;
    }
    else if(_stEvent2.u8Set)
    {
        return 2;
    }
    return 0;
}

void AUTO_EXERCISE_MODE::prvUpdateCallingForSchedulerRunOp()
{

}


uint32_t AUTO_EXERCISE_MODE::GetExerciserTime(void)
{
    return (uint32_t)(ceil((float)(_u32SchRemTime_sec - UTILS_GetElapsedTimeInSec(&_ExeDnCntTmr))/60.0f));
}
