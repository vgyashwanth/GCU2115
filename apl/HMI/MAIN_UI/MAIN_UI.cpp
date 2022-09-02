/**
 * @file        [MAIN_UI]
 * @brief       This module is used to handle all display related functionality.
 *              It handles display mode switching and key handling
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        4th Aug 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/



#include <HMI/MAIN_UI/MAIN_UI.h>
#include "../UI_DS.h"
#include "MB_APP/MB_APP.h"

KEYPAD::KEYPAD_EVENTS_t MAIN_UI::_sKeyEvent = AUTO_KEY_SHORT_PRESS;
bool MAIN_UI::_sbKeyEventAvailable = false;
uint16_t MAIN_UI::_u16ScreenChangeTime = SCREEN_CHANGE_OVER_PAUSE;


MAIN_UI::MAIN_UI(HAL_Manager &hal, CFGZ &pcfgz, GCU_ALARMS &GCUAlarms,
        ENGINE_MONITORING  &EngMon, START_STOP &StartStop,
        MANUAL_MODE &ManualMode,
        Display &Disp, CFGC &CFGC, SLEEP_Handler &sleep, EGOV &Egov,
        AUTO_EXERCISE_MODE &autoExercise, J1939APP &j1939 , BTS_MODE &BTSMode , CYCLIC_MODE &CyclicMode, ENGINE_START_VALIDITY &EngineStartValidity):
_hal(hal),
_cfgz(pcfgz),
_sleep(sleep),
_ManualMode(ManualMode),
_EngMon(EngMon),
_GCUAlarms(GCUAlarms),
_StartStop(StartStop),
_Egov(Egov),
_j1939(j1939),
_BTSMode(BTSMode),
_CyclicMode(CyclicMode),
_EngineStartValidity(EngineStartValidity),
_MonUI(hal, ManualMode,autoExercise, EngMon, pcfgz, StartStop, GCUAlarms, Disp, CFGC, j1939 , BTSMode, CyclicMode),
_autoExercise(autoExercise),
_DispAlarm(GCUAlarms, Disp,pcfgz, j1939,hal),
_DispEventLog(hal, Disp, CFGC, GCUAlarms, pcfgz),
_PasswordEntry(hal, pcfgz, Disp),
_objUI(hal,_PasswordEntry,pcfgz,Disp ,EngMon),
_bPanelLockOnce(false),
_bExternalPanelLockOnce(false)
{
    UTILS_ResetTimer(&_RefreshTimer);
    UTILS_ResetTimer(&_PoweSaveModeTimer);
    UTILS_ResetTimer(&_ScreenChangeOverTimer);
    UTILS_ResetTimer(&_LEDBlinkTimer);
    UTILS_ResetTimer(&_AutoExitTimer);
    UTILS_ResetTimer(&_BootTimer);
    _hal.ObjKeypad.RegisterKeyEventCB(prvKeypad_callback);

    _u16ScreenChangeTime = _cfgz.GetCFGZ_Param(CFGZ::ID_SCREEN_CHANG_TIME);

    //Copy all Auxilary strings
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_A, strAuxAString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_B, strAuxBString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_C, strAuxCString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_D, strAuxDString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_E, strAuxEString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_F, strAuxFString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_G, strAuxGString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_H, strAuxHString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_I, strAuxIString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_J, strAuxJString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_K, strAuxKString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_L, strAuxLString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_M, strAuxMString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_N, strAuxNString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_O, strAuxOString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_P, strAuxPString);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_SENSOR_S1_NAME, strAuxS1String);
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_SENSOR_S2_NAME ,strAuxS2String );
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_SENSOR_S3_NAME ,strAuxS3String );
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_SENSOR_S4_NAME ,strAuxS4String );



    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_PROFILE, strProfile);
}

void MAIN_UI::prvExitFromConfigMode()
{
    _objUI.SaveConfigFile();
    _MonUI.Init();

    _EngineStartValidity.UpdateStartValidyParam();
    _autoExercise.Init();
    _Egov.InitEgovParameters();
    _ManualMode.InitNightModeParam();
    _GCUAlarms.InitGCUAlarms();
    for(uint8_t u8AlarmIndex = 0; u8AlarmIndex <
                  GCU_ALARMS::ALARM_LIST_LAST; u8AlarmIndex++)
    {
      _GCUAlarms.ConfigureGCUAlarms(u8AlarmIndex);
    }
    for(uint8_t u8LoggingID = 0; u8LoggingID <GCU_ALARMS::ID_ALL_ALARMS_LAST; u8LoggingID++)
    {
      _GCUAlarms.AssignAlarmsForDisplay(u8LoggingID);
    }
    _ManualMode.ChangeManualState(
          BASE_MODES::STATE_MANUAL_GEN_OFF);
    _ManualMode.ChangeAutoState(
          BASE_MODES::STATE_AMF_GEN_OFF_MAINS_ON);
    _ManualMode.SetMainsStatusHealthy();
    _GCUAlarms.ClearAllAlarms();
    _GCUAlarms.ResetMainsMonParams();
    _StartStop.Init();



    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)
    {
      _ManualMode.OpenMainsLoad();
    }
    if(CEditableItem::IsAnyConfigValueEdited())
    {
      _GCUAlarms.LogEvent(GCU_ALARMS::Config_Modified_By_User_id,GCU_ALARMS::ID_NONE);
    }
    MON_UI::eDisplayMode = DISP_MON_MODE;
    _MonUI.GoToHomeScreen();
}

bool MAIN_UI::Update()
{
    bool bRefresh = true;
    static uint8_t _u8NumberOfAlarms =0;
    static bool bEventLogEntry = false;

    // Do not execute anything when device is in sleep mode.
    if(_hal.AmIInSleep())
    {
        return false;
    }

    //Reset all timers when exited from sleep
    if(_hal.bJustExitedFromSleep)
    {
        _hal.bJustExitedFromSleep = false;
        UTILS_ResetTimer(&_SleepExitTimer);
        UTILS_ResetTimer(&_PoweSaveModeTimer);
        return false;
    }

    //Ignore key press when just exited form Sleep
    if( UTILS_GetElapsedTimeInMs(&_SleepExitTimer) <= 500)
    {
        _sbKeyEventAvailable = false;
        return false;
    }

    //If any config value changed then show saving setting screen
    // till it save the data or till 2sec
    if(CEditableItem::IsAnyConfigValueEdited() &&
            (IS_DISP_MON_MODE()|| IS_DISP_ALARM_MODE()))
    {
        if(( UTILS_GetElapsedTimeInSec(&_RefreshTimer) >= TIME_OUT_SEC) )
        {
//            if(!_cfgz.IsConfigWritten())
//            {
//                _MonUI.PrintErrorScreen();
//            }
//            else
            {
                CEditableItem::ClearConfigEditStatus();
            }
            UTILS_ResetTimer(&_RefreshTimer);
        }
        return true;
    }
    else if(UTILS_GetElapsedTimeInMs(&_RefreshTimer) >= REFRESH_TIME_MS)
    {
        UTILS_ResetTimer(&_RefreshTimer);
        bRefresh = true;
    }
    else
    {
        bRefresh = false;
    }

    //Check for number of alarms, for any new alarm chage mode to Alarm mode
    if((_GCUAlarms.GetActiveAlarmCount() > _u8NumberOfAlarms)
            && IS_DISP_MON_MODE())
    {
       _u8NumberOfAlarms = _GCUAlarms.GetActiveAlarmCount();
       MON_UI::eDisplayMode = DISP_ALARM_MODE;
       ALARM_UI::ChangeAlarmScreenType(ALARM_UI::ALARM);
    }
    else
    {
        _u8NumberOfAlarms = _GCUAlarms.GetActiveAlarmCount();
    }

    //Till boot logo do not execute LED Handling
    if(UTILS_GetElapsedTimeInSec(&_BootTimer) >= BOOT_DELAY)
    {
        prvLEDHandling();
    }
    else
    {
        //Till boot time kick sleep time , power save time
        // and screen change over time
        _sleep.KickSleepTimer();
        UTILS_ResetTimer(&_PoweSaveModeTimer);
        UTILS_ResetTimer(&_ScreenChangeOverTimer);
        _sbKeyEventAvailable = false;
    }

    //kick sleep timer for following conditions
    if(   IS_DISP_EVENT_LOG_MODE()
        ||IS_DISP_CONFIG_MODE()
        ||IS_DISP_PASSWORD_ENTRY_MODE()
        ||(!IS_GCU_MANUAL_MODE())
        ||((_StartStop.GetStartStopSMDState()
                != START_STOP::ID_STATE_SS_ENG_OFF_ERR)
            &&(_StartStop.GetStartStopSMDState()
                    != START_STOP::ID_STATE_SS_ENG_OFF_OK))
        ||(((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN))
                == CFGZ::CFGZ_ENABLE) &&
                (_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR)
                      != ACT_Manager:: ACT_NOT_CONFIGURED))
        ||(IS_GCU_AUTO_MODE()
            &&(_hal.DigitalSensors.GetDigitalSensorState(
                    DigitalSensor::DI_REMOTE_START_STOP)
                   != DigitalSensor::SENSOR_NOT_CONFIGRUED))
        ||_cfgz.GetCFGZ_Param(CFGZ::ID_COMMUNICATION_EN_MB)
      )
    {
        _sleep.KickSleepTimer();
    }

    //Turn off Back light after Power save delay over
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_POWER_SAVE_MODE_EN)== CFGZ::CFGZ_ENABLE)
                 &&(UTILS_GetElapsedTimeInSec(&_PoweSaveModeTimer)
                      >= _cfgz.GetCFGZ_Param(CFGZ::ID_POWER_SAVE_MODE_DELAY)))
    {
        _hal.ObjGlcd.TurnOffBackLight();
        UTILS_ResetTimer(&_PoweSaveModeTimer);
    }

    if(_EngMon.IsEngineOn() )
    {
        UTILS_ResetTimer(&_PoweSaveModeTimer);
    }

    if(UTILS_GetElapsedTimeInSec(&_AutoExitTimer) >= _cfgz.GetCFGZ_Param(CFGZ::ID_AUTO_EXIT_TIME))
    {
        UTILS_DisableTimer(&_AutoExitTimer);
        if(IS_DISP_CONFIG_MODE() ||IS_DISP_EVENT_LOG_MODE())
        {
            UTILS_ResetTimer(&_RefreshTimer);
           prvExitFromConfigMode();
        }
        else if(IS_DISP_PASSWORD_ENTRY_MODE())
        {
           MON_UI::eDisplayMode = DISP_MON_MODE;
           _MonUI.GoToHomeScreen();
           _MonUI.Update(bRefresh);
           _PasswordEntry.SetPasswordEntryState(PASSWORD_ENTRY_UI::ENTRY);
        }
    }

    if((_sKeyEvent ==  DN_LONG_PRESS )||(_sKeyEvent ==  UP_LONG_PRESS ))
    {
        UTILS_ResetTimer(&_AutoExitTimer);
    }


    if(_ManualMode.IsGCUStateChanged())
    {
        _MonUI.GoToHomeScreen();
        _ManualMode.ClearGCUStateChangedStatus();
    }




    MB_APP::KEY_MB_CAN_EVENT_t  stMBEvent;
    MB_APP::GetMBEventStatus(&stMBEvent);
     if(_sbKeyEventAvailable || stMBEvent.bKeyEvent)
     {
         _sbKeyEventAvailable = false;
         UTILS_ResetTimer(&_AutoExitTimer);
         _sleep.KickSleepTimer();
         //During cranking turn off the back light to reduce the GCU current.
         if(_ManualMode.GetTimerState() == BASE_MODES::CRANK_START_TIMER )
         {
             _hal.ObjGlcd.TurnOffBackLight();
         }
         else
         {
             _hal.ObjGlcd.TurnOnBackLight();
         }
         UTILS_ResetTimer(&_ScreenChangeOverTimer);
        if ( _cfgz.GetCFGZ_Param(CFGZ::ID_SCREEN_CHANG_TIME) > SCREEN_CHANGE_OVER_PAUSE)
        {
            _u16ScreenChangeTime = _cfgz.GetCFGZ_Param(CFGZ::ID_SCREEN_CHANG_TIME);
        }
        else
        {
            _u16ScreenChangeTime = SCREEN_CHANGE_OVER_PAUSE;
        }
         UTILS_ResetTimer(&_PoweSaveModeTimer);

         if((_sKeyEvent == DN_SHORT_PRESS) || (_sKeyEvent == UP_SHORT_PRESS))
         {
             _hal.ObjGlcd.AutoReInitGCLDScreen();
         }
         if(!_sbKeyEventAvailable &&  stMBEvent.bKeyEvent )
          {
              if(stMBEvent.bAutoKey)
              {
                  _sKeyEvent = AUTO_KEY_SHORT_PRESS;
              }
              else if(stMBEvent.bStartKey)
              {
                  _sKeyEvent = START_KEY_SHORT_PRESS;
              }
              else if(stMBEvent.bStopKey)
              {
                  _sKeyEvent = STOP_KEY_SHORT_PRESS;
              }

              if(stMBEvent.bAckKey)
              {
                  _sKeyEvent = ACK_KEY_PRESS;
              }
          }
        switch(_sKeyEvent)
        {
           case KEYPAD::STOP_KEY_LONG_PRESS:
           {
               if(IS_DISP_MON_MODE() || IS_DISP_ALARM_MODE())
               {
                   if(  IS_GCU_MANUAL_MODE()
                       && (!_ManualMode.IsGenRunTimersInProcess())
                       && (!_EngMon.IsEngineCranked())
                       && (!_EngMon.IsEngineOn()))
                   {
                       MON_UI::eDisplayMode = DISP_PASSWORD_EDIT_MODE;
                       bRefresh = true;
                   }
               }
               else if( IS_DISP_CONFIG_MODE() ||IS_DISP_EVENT_LOG_MODE())
               {
                   prvExitFromConfigMode();
               }
               else if(IS_DISP_PASSWORD_ENTRY_MODE())
               {
                   MON_UI::eDisplayMode = DISP_MON_MODE;
                   _MonUI.GoToHomeScreen();
                   _MonUI.Update(bRefresh);
                   _PasswordEntry.SetPasswordEntryState(PASSWORD_ENTRY_UI::ENTRY);
               }
               break;
           }
           case UP_DN_KEY_LONG_PRESS:
           {
               if(IS_DISP_CONFIG_MODE())
               {
                   MON_UI::eDisplayMode = DISP_EVENT_LOG_MODE;
                   bEventLogEntry = true;
               }
               else if(IS_DISP_EVENT_LOG_MODE())
               {
                   MON_UI::eDisplayMode = DISP_CONFIG_MODE;
                   bEventLogEntry = false;
                   _objUI.Handler(CKeyCodes::NONE);
               }
               break;
           }
           case STOP_KEY_SHORT_PRESS:  //Escape key Press
           {
               if(IS_DISP_PASSWORD_ENTRY_MODE())
               {
                   if(_PasswordEntry.GetPasswordEnteryState()
                           == PASSWORD_ENTRY_UI::ENTRY)
                   {     //Enter in Read Mode Only
                       MON_UI::eDisplayMode = DISP_CONFIG_MODE;
                       _objUI.Handler(CKeyCodes::NONE);
                   }
               }

               if(IS_DISP_ALARM_MODE())
               {
                   //Enter in Read Mode Only
                   MON_UI::eDisplayMode = DISP_MON_MODE;
                   _MonUI.GoToHomeScreen();
                   _MonUI.Update(bRefresh);
               }
               break;
           }

           case START_KEY_SHORT_PRESS:  //Start key Press
           case AUTO_KEY_LONG_PRESS:   //Auto Long press
           case AUTO_KEY_SHORT_PRESS:
           case DN_STOP_KEY_LONG_PRESS:
           {
               if(IS_DISP_ALARM_MODE())
               {
                   MON_UI::eDisplayMode = DISP_MON_MODE;
                   _MonUI.GoToHomeScreen();
                   _MonUI.Update(bRefresh);
               }
               break;
           }

//           case DN_SHORT_PRESS:
//           case UP_SHORT_PRESS:
//               _GCUAlarms.TurnOffSounderAlarm();
//            break;

           case GEN_CONT_KEY_PRESS:
           {
               if(IS_GCU_MANUAL_MODE() ||IS_GCU_TEST_MODE())
               {

                   if(1)
                   {
                       if((!_ManualMode.IsGenContactorClosed())&& (_EngMon.IsGenAvailable()))
                       {
                           _ManualMode.SwitchLoadToGen();
                       }
                       else
                       {
                           _ManualMode.OpenGenLoad();
                       }
                   }
               }
           }
           break;

           case MAINS_CONT_KEY_PRESS:
           {
               if(IS_GCU_MANUAL_MODE() || IS_GCU_TEST_MODE())
               {
                   if((_ManualMode.IsMainsContactorConfigured()))
                   {
                       if((!_ManualMode.IsMainsContactorClosed()) && (_ManualMode.GetMainsStatus() == BASE_MODES:: MAINS_HELATHY ))
                       {
                           _ManualMode.SwitchLoadToMains();
                       }
                       else
                       {
                           _ManualMode.OpenMainsLoad();
                       }
                   }
               }
           }
           break;

           default:
               break;
        }

        switch(MON_UI::eDisplayMode)
        {
            case DISP_MON_MODE:
            {
                _MonUI.CheckKeyPress(_sKeyEvent);
                break;
            }
            case DISP_ALARM_MODE:
            {
                _DispAlarm.CheckKeyPress(_sKeyEvent);
                break;
            }
            case DISP_EVENT_LOG_MODE:
            {
                _DispEventLog.CheckKeyPress(_sKeyEvent);
                break;
            }
            case DISP_PASSWORD_EDIT_MODE:
            {
               _PasswordEntry.CheckKeyPress(_sKeyEvent);
                break;
            }
            case DISP_CONFIG_MODE:
            {
                _objUI.ConfigCheckKeyPress(_sKeyEvent);
                break;
            }
        }
    }

    switch(MON_UI::eDisplayMode)
    {
       case DISP_MON_MODE:
       {
//           if(((_sKeyEvent ==  KEYPAD::BSP_KEY_4_LONG_PRESS )||(_sKeyEvent ==  KEYPAD::BSP_KEY_5_LONG_PRESS )) && bRefresh)
//           {
//               _MonUI.GroupSwitching(_sKeyEvent);
//           }
           _MonUI.Update(bRefresh);

           break;
       }
       case DISP_ALARM_MODE:
       {
           if(_GCUAlarms.IsAlarmPresent())
           {
               UTILS_ResetTimer(&_ScreenChangeOverTimer);
           }

//           if(((_sKeyEvent ==  KEYPAD::BSP_KEY_4_LONG_PRESS )||(_sKeyEvent ==  KEYPAD::BSP_KEY_5_LONG_PRESS ))&& bRefresh)
//           {
//               _MonUI.GroupSwitching(_sKeyEvent);
//               _MonUI.Update(bRefresh);
//           }
//           else
           {
               _DispAlarm.Update(bRefresh);
           }

           break;
       }
       case DISP_EVENT_LOG_MODE:
       {
           UTILS_ResetTimer(&_PoweSaveModeTimer);
           _DispEventLog.Update(bEventLogEntry);
           bEventLogEntry = false;
           break;
       }
       case DISP_PASSWORD_EDIT_MODE:
       {
           UTILS_ResetTimer(&_PoweSaveModeTimer);
          _PasswordEntry.Update(bRefresh);
          if( _PasswordEntry.CheckPasswordStatus())
          {
              MON_UI::eDisplayMode = DISP_CONFIG_MODE;
              _objUI.Handler(CKeyCodes::NONE);
          }
           break;
       }
       /* <LDRA Phase code 62S> <Switch case not terminated with break.:Resolved>
        * <Verified by: Nikhil Mhaske> <11/9/2021> */
       case DISP_CONFIG_MODE:
       {
           UTILS_ResetTimer(&_PoweSaveModeTimer);
           _objUI.ConfigUIUpdate();
          //Checking long press events
          if((_sKeyEvent ==  DN_LONG_PRESS )
          ||(_sKeyEvent ==  UP_LONG_PRESS ))
          {
              _objUI.ConfigCheckKeyPress(_sKeyEvent);
          }
          break;
       }
    }

    if((UTILS_GetElapsedTimeInSec(&_ScreenChangeOverTimer) >= _u16ScreenChangeTime)&& (_cfgz.GetCFGZ_Param(CFGZ::ID_SCREEN_CHANG_TIME)>0))
    {
        _hal.ObjGlcd.AutoReInitGCLDScreen();
       UTILS_ResetTimer(&_ScreenChangeOverTimer);
       _u16ScreenChangeTime = _cfgz.GetCFGZ_Param(CFGZ::ID_SCREEN_CHANG_TIME);
       if(IS_DISP_MON_MODE())
       {
            _MonUI.CheckKeyPress(DN_SHORT_PRESS);
       }
       else if(IS_DISP_ALARM_MODE())
       {
           _DispAlarm.CheckKeyPress(DN_SHORT_PRESS);
       }
    }

    if((!IS_DISP_ALARM_MODE()) && (!IS_DISP_MON_MODE()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

MODE_TYPE_t MAIN_UI::GetDispMode(void)
{
    return MON_UI::eDisplayMode;
}
void MAIN_UI::prvLEDHandling()
{
    #define TURN_ON      BSP_IO_LEVEL_LOW
    #define TURN_OFF     BSP_IO_LEVEL_HIGH

    static bsp_io_level_t eState = BSP_IO_LEVEL_LOW;

    if(UTILS_GetElapsedTimeInMs(&_LEDBlinkTimer) >= LED_BLINK_TIMER)
    {
        UTILS_ResetTimer(&_LEDBlinkTimer);
        eState = (eState == BSP_IO_LEVEL_LOW)? BSP_IO_LEVEL_HIGH
                                               :BSP_IO_LEVEL_LOW;

        if(_ManualMode.IsOperInProgress() == true)
        {
            switch(_ManualMode.GetGCUOperatingMode())
            {

                case BASE_MODES::MANUAL_MODE:
                {
                    if(prvIsEnginNotInONstate())
                    {
                        UpdateLEDStatus(TURN_OFF, TURN_OFF, eState);
                    }
                    else
                    {
                        UpdateLEDStatus(TURN_OFF, eState, TURN_OFF);
                    }
                break;
                }
                case BASE_MODES::TEST_MODE:
                {
                    UpdateLEDStatus(eState, eState, TURN_OFF);
                    break;
                }
                case BASE_MODES::AUTO_MODE:
                case BASE_MODES::AUTO_EXERCISE_MODE:
                case BASE_MODES::BTS_MODE:
                case BASE_MODES::CYCLIC_MODE:
                {
                    UpdateLEDStatus(eState, TURN_OFF, TURN_OFF);
                    break;
                }

                default: break;

            }
       }
       else if(IS_GCU_AUTO_MODE() || IS_GCU_AUTO_EXE_MODE() || IS_GCU_BTS_MODE()|| IS_GCU_CYCLIC_MODE())
       {
           UpdateLEDStatus(TURN_ON, TURN_OFF, TURN_OFF);
       }
       else if(IS_GCU_TEST_MODE())
       {
           UpdateLEDStatus(eState, eState, TURN_OFF);
       }
       else
       {
           if(prvIsEnginNotInONstate())
           {
               UpdateLEDStatus(TURN_OFF, TURN_OFF, TURN_ON);
           }
           else
           {
               UpdateLEDStatus(TURN_OFF, TURN_ON, TURN_OFF);
           }
       }

       if(_GCUAlarms.IsCommonWarning())
       {
           _hal.ledManager.led10.ChangeLEDState(eState);
       }
       else
       {
           _hal.ledManager.led10.TurnOff();
       }

       if((_GCUAlarms.IsCommonElectricTrip())||(_GCUAlarms.IsCommonShutdown()))
       {
           _hal.ledManager.led11.ChangeLEDState(eState);
       }
       else
       {
           _hal.ledManager.led11.TurnOff();
       }

       if(_GCUAlarms.IsCommonNotification())
       {
           _hal.ledManager.led9.ChangeLEDState(eState);
       }
       else
       {
           _hal.ledManager.led9.TurnOff();
       }

       if(_EngMon.IsEngineOn())
       {
           _hal.ledManager.led5.TurnOn();
       }
       else
       {
           _hal.ledManager.led5.TurnOff();
       }

       if(((_ManualMode.GetMainsStatus() == BASE_MODES::MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
               && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE))
       {
           _hal.ledManager.led8.TurnOn();
       }
       else
       {
           _hal.ledManager.led8.TurnOff();
       }

       if(_ManualMode.IsGenContactorConfigured()&&_ManualMode.IsGenContactorClosed())
       {
           _hal.ledManager.led6.TurnOn();
       }
       else
       {
           _hal.ledManager.led6.TurnOff();
       }

       if((_ManualMode.IsMainsContactorConfigured())&& (_ManualMode.IsMainsContactorClosed()) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE))
       {
           _hal.ledManager.led7.TurnOn();
       }
       else
       {
           _hal.ledManager.led7.TurnOff();
       }

    }
    _hal.ledManager.led1.ChangeLEDState((bsp_io_level_t)(!_hal.ObjKeypad.IsKey1PressedRaw()));
//    if(KEYPAD::BSP_KEY_1_PRESS_STARTED)
//    {
//        _hal.ledManager.led1.ChangeLEDState(IsKey1PressedRaw);
//    }
//    if( KEYPAD::BSP_KEY_1_RELEASED)
//    {
//        _hal.ledManager.led1.TurnOff();
//    }


}


void MAIN_UI::prvKeypad_callback(KEYPAD::KEYPAD_EVENTS_t Evt)
{
    if(   (Evt == KEYPAD::BSP_KEY_1_PRESS_STARTED)
        ||(Evt == KEYPAD::BSP_KEY_2_PRESS_STARTED)
        ||(Evt == KEYPAD::BSP_KEY_3_PRESS_STARTED)
        ||(Evt == KEYPAD::BSP_KEY_4_PRESS_STARTED)
        ||(Evt == KEYPAD::BSP_KEY_5_PRESS_STARTED)
        ||(Evt == KEYPAD::BSP_KEY_6_PRESS_STARTED)
        ||(Evt == KEYPAD::BSP_KEY_7_PRESS_STARTED)
        ||(Evt == KEYPAD::BSP_KEY_8_PRESS_STARTED)
        )
        {
            return;
        }
    _sKeyEvent = Evt;
    _sbKeyEventAvailable = true;
    _u16ScreenChangeTime = SCREEN_CHANGE_OVER_PAUSE;
}

bool MAIN_UI::prvIsEnginNotInONstate()
{
    if( (_StartStop.GetStartStopSMDState() == START_STOP::ID_STATE_SS_ENG_OFF_ERR)
       ||(_StartStop.GetStartStopSMDState()== START_STOP::ID_STATE_SS_ENG_OFF_OK)
       ||(_StartStop.GetStartStopSMDState()== START_STOP::ID_STATE_SS_FAIL_TO_STOP)
       ||(_StartStop.GetStartStopSMDState()== START_STOP::ID_STATE_SS_STOPPING)
       ||(_StartStop.GetStartStopSMDState()== START_STOP::ID_STATE_SS_STOP_HOLD)
       ||(_ManualMode.GetManualModeState() == BASE_MODES::STATE_MANUAL_ENGINE_COOLING)
       ||(_ManualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_ENGINE_COOLING))
        {
            return true;
        }
        else
        {
            return false;
        }
}

void MAIN_UI::UpdateLEDStatus(bsp_io_level_t stLED1, bsp_io_level_t stLED2,
        bsp_io_level_t stLED3)
{

    _hal.ledManager.led2.ChangeLEDState(stLED1);
    _hal.ledManager.led3.ChangeLEDState(stLED2);
    _hal.ledManager.led4.ChangeLEDState(stLED3);
}

