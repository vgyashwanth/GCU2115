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


#define TURN_ON      BSP_IO_LEVEL_LOW
#define TURN_OFF     BSP_IO_LEVEL_HIGH

KEYPAD::KEYPAD_EVENTS_t MAIN_UI::_sKeyEvent = AUTO_KEY_SHORT_PRESS;
bool MAIN_UI::_sbKeyEventAvailable = false;
bool MAIN_UI::_bEnteredPowerSave = false;

MAIN_UI::MAIN_UI(HAL_Manager &hal, CFGZ &pcfgz, GCU_ALARMS &GCUAlarms,
        ENGINE_MONITORING  &EngMon, START_STOP &StartStop,MANUAL_MODE &ManualMode,
        Display &Disp, CFGC &CFGC, SLEEP_Handler &sleep,
        J1939APP &j1939 , BTS_MODE &BTSMode , CYCLIC_MODE &CyclicMode, ENGINE_START_VALIDITY &EngineStartValidity):
_hal(hal),
_cfgz(pcfgz),
_sleep(sleep),
_ManualMode(ManualMode),
_EngMon(EngMon),
_GCUAlarms(GCUAlarms),
_StartStop(StartStop),
_j1939(j1939),
_BTSMode(BTSMode),
_CyclicMode(CyclicMode),
_EngineStartValidity(EngineStartValidity),
_MonUI(hal, ManualMode, EngMon, pcfgz, StartStop, GCUAlarms, Disp, CFGC, j1939 , BTSMode, CyclicMode),
_DispAlarm(GCUAlarms, Disp,pcfgz, j1939,hal),
_DispEventLog(hal, Disp, CFGC, GCUAlarms, pcfgz),
_PasswordEntry(hal, pcfgz, Disp),
_objUI(hal,_PasswordEntry,pcfgz,Disp ,EngMon),
_bRefresh(true),
_bEventLogEntry(false),
_u16ScreenChangeTime(_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SCRN_CHNGOVER_TIME))

{
    UTILS_ResetTimer(&_RefreshTimer);
    UTILS_ResetTimer(&_PoweSaveModeTimer);
    UTILS_ResetTimer(&_ScreenChangeOverTimer);
    UTILS_ResetTimer(&_LEDBlinkTimer);
    UTILS_ResetTimer(&_AutoExitTimer);
    UTILS_ResetTimer(&_BootTimer);
    _hal.ObjKeypad.RegisterKeyEventCB(prvKeypad_callback);

    /* Copy all Auxiliary strings */
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

    /* Copy profile name */
    _cfgz.GetCFGZ_Param(CFGZ::ID_ARR_PROFILE, strProfile);
}

void MAIN_UI::prvExitFromConfigMode()
{
    _objUI.SaveConfigFile(); /* consist saving setting screen */
    _GCUAlarms.InitGCUAlarms();
    for(uint8_t u8AlarmIndex = 0; u8AlarmIndex < GCU_ALARMS::ALARM_LIST_LAST; u8AlarmIndex++)
    {
        _GCUAlarms.ConfigureGCUAlarms(u8AlarmIndex);
    }
    for(uint8_t u8LoggingID = 0; u8LoggingID < GCU_ALARMS::ID_ALL_ALARMS_LAST; u8LoggingID++)
    {
        _GCUAlarms.AssignAlarmsForDisplay(u8LoggingID);
    }


    /* todo: Shift below prv functions to the respective classes once all other files become ready */
    prvUpadteBaseModeConfigDependency(); /* base mode related function call made with manual mode obj referenece*/
    _GCUAlarms.ClearAllAlarms();
    _GCUAlarms.ResetMainsMonParams();
    _EngineStartValidity.InitEngineStartValidityConfig();
    _StartStop.Init();
    
    _MonUI.Init(); /* MON UI need to be updated at the end */

    if(CEditableItem::IsAnyConfigValueEdited())
    {
      _GCUAlarms.LogEvent(GCU_ALARMS::Config_Modified_By_User_id,CFGZ::CFGZ_ACTION_NONE_NoWESN);
    }
    else
    {
        /* Do nothing */
    }
}

bool MAIN_UI::Update()
{
    _bRefresh = true; /* todo: think about it */
    static uint8_t _u8NumberOfAlarms = 0;

    /* exclude execution when in sleep mode. */
    if(_hal.AmIInSleep())
    {
        return false;
    }
    else
    {
        /* Do nothing */
    }

    /* Reset timers when exited from sleep */
    if(_hal.bJustExitedFromSleep)
    {
        _hal.bJustExitedFromSleep = false;
        UTILS_ResetTimer(&_SleepExitTimer);
        UTILS_ResetTimer(&_PoweSaveModeTimer);
        return false;
    }
    else
    {
        /* Do nothing */
    }

    /* Ignore key press when just exited form Sleep*/
    if( UTILS_GetElapsedTimeInMs(&_SleepExitTimer) <= 500)
    {
        _sbKeyEventAvailable = false;
        return false;
    }
    else
    {
        /* Do nothing */
    }

    /* 3 sec intentional timeout to save the configs */
    if(CEditableItem::IsAnyConfigValueEdited() &&  (IS_DISP_MON_MODE()|| IS_DISP_ALARM_MODE()))
    {
        if(( UTILS_GetElapsedTimeInSec(&_RefreshTimer) >= TIME_OUT_SEC) )
        {
            CEditableItem::ClearConfigEditStatus();
            UTILS_ResetTimer(&_RefreshTimer);
        }
        _sleep.KickSleepTimer();
        return true;
    }
    else if(UTILS_GetElapsedTimeInMs(&_RefreshTimer) >= REFRESH_TIME_MS)
    {
        UTILS_ResetTimer(&_RefreshTimer);
        _bRefresh = true;
    }
    else
    {
        _bRefresh = false;
    }

    /* Move mon screen to alarm screen if new alarm present */
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

    /* Hanlde LED once boot logo displayed */
    if(UTILS_GetElapsedTimeInSec(&_BootTimer) >= BOOT_DELAY)
    {
        prvLEDHandling();
    }
    else
    {
        /* execution should not enter in any mode while boot timing */
        _sleep.KickSleepTimer();
        UTILS_ResetTimer(&_PoweSaveModeTimer);
        UTILS_ResetTimer(&_ScreenChangeOverTimer);
        _sbKeyEventAvailable = false;
    }


    if(!prvIsSleepEnabled())
    {
        _sleep.KickSleepTimer();
    }
    else
    {
        /* execute sleep  */
    }

    /* Turn off back light if power save occurs */
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_DISPLAY_POWER_SAVE_MODE)== CFGZ::CFGZ_ENABLE)
       && (UTILS_GetElapsedTimeInSec(&_PoweSaveModeTimer) >= _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_PWR_SAVE_MODE_DELAY)))
    {
        _hal.ObjGlcd.TurnOffBackLight();
        _bEnteredPowerSave =  true;
        UTILS_ResetTimer(&_PoweSaveModeTimer);
    }

    if(_EngMon.IsEngineOn())
    {
        UTILS_ResetTimer(&_PoweSaveModeTimer);
        _bEnteredPowerSave = false;
    }
    else
    {
        /* Do nothing */
    }

    if(_ManualMode.IsGCUStateChanged())
    {
        _MonUI.GoToHomeScreen();
        _ManualMode.ClearGCUStateChangedStatus();
    }


    /* During cranking turn off the back light to reduce the GCU current. (Hardware specific)*/
    if(!_bEnteredPowerSave)
    {

    }

    MB_APP::KEY_MB_CAN_EVENT_t  stMBEvent;
    MB_APP::GetMBEventStatus(&stMBEvent);
     if(_sbKeyEventAvailable || stMBEvent.bKeyEvent)
     {
        _sbKeyEventAvailable = false;

        _sleep.KickSleepTimer();
        UTILS_ResetTimer(&_ScreenChangeOverTimer);
        UTILS_ResetTimer(&_PoweSaveModeTimer);

        /* The below statements have dependency from START STOP and PowerSave mode
         * Whenever a key press is found then controller exists power save and not in
         * Cranking it will turn on.
         * Other handling is done in START STOP even if the Engine starts based on other conditions.
         */

        if(_ManualMode.GetTimerState() == BASE_MODES::CRANK_START_TIMER )
        {
            _hal.ObjGlcd.TurnOffBackLight();
        }
        else
        {
            _hal.ObjGlcd.TurnOnBackLight();
        }




/* Shubham Wader 17.09.2022
   below condition is confirmed with Mihir B. (SYSE). SW will execute screen changeover delay from config only if it is
   greater that the hard code delay that user dont know.*/
        /*
         * The below conditions are added for the below reason.
         * If user has pressed any key then if the configured
         * screen time is less than 2 mins then We do not change the screen for minimum of 2 minutes.
         * After that if no key press is observed then we continue to use the configured change over time.
         */
        if ( _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SCRN_CHNGOVER_TIME) > SCREEN_CHANGE_OVER_PAUSE)
        {
            _u16ScreenChangeTime = _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SCRN_CHNGOVER_TIME);
        }
        else
        {
            _u16ScreenChangeTime = SCREEN_CHANGE_OVER_PAUSE;
        }

        if((_sKeyEvent == DN_SHORT_PRESS) || (_sKeyEvent == UP_SHORT_PRESS))
        {
            _hal.ObjGlcd.AutoReInitGCLDScreen();
        }
        else
        {
            /* do nothing */
        }

        if(stMBEvent.bKeyEvent)
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
            else
            {
              /* Do nothing */
            }
/* todo: need to append below if in above else if. Need of discussion. */
            if(stMBEvent.bAckKey)
            {
                _sKeyEvent = ACK_KEY_PRESS;
            }
            else
            {
                /* do nothing */
            }
        }

        prvHandleKeyPressEvent(_sKeyEvent);
    }

    prvHandleDisplayModes();

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SCRN_CHNGOVER_TIME) > VALUE_ZERO)
    {
        prvHandleScreenChangeover();
    }
    else
    {
        /*todo: need to discuss, should we disable _ScreenChangeOverTimer here ? */
        /* skip changeover, if timer config value is set to 0. */
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

    /* conditions to handle leds are referred from GC2111 NXp  */
    static bsp_io_level_t eState = BSP_IO_LEVEL_LOW;

    if(UTILS_GetElapsedTimeInMs(&_LEDBlinkTimer) >= LED_BLINK_TIMER)
    {
        UTILS_ResetTimer(&_LEDBlinkTimer);
        eState = (eState == BSP_IO_LEVEL_LOW)? BSP_IO_LEVEL_HIGH :BSP_IO_LEVEL_LOW;

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
                case BASE_MODES::BTS_MODE:
                case BASE_MODES::CYCLIC_MODE:
                {
                    UpdateLEDStatus(eState, TURN_OFF, TURN_OFF);
                    break;
                }

                default: break;

            }
       }
       else if(IS_GCU_AUTO_MODE() || IS_GCU_BTS_MODE()|| IS_GCU_CYCLIC_MODE())
       {
           UpdateLEDStatus(TURN_ON, TURN_OFF, TURN_OFF);
       }
       else if(IS_GCU_TEST_MODE())
       {
           if(prvIsEnginNotInONstate())
           {
               UpdateLEDStatus(eState, eState, TURN_OFF);
           }
           else
           {
               UpdateLEDStatus(TURN_ON, TURN_ON, TURN_OFF);
           }
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

       if(((_ManualMode.GetMainsStatus() == BASE_MODES::MAINS_HELATHY)
           && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE))
           || ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE) && _GCUAlarms.RemoteStopReceived()))
       {
           _hal.ledManager.led8.TurnOn();
       }
       else
       {
           _hal.ledManager.led8.TurnOff();
       }

       if(_ManualMode.IsGenContactorConfigured() && _ManualMode.IsGenContactorClosed())
       {
           _hal.ledManager.led6.TurnOn();
       }
       else
       {
           _hal.ledManager.led6.TurnOff();
       }

       if((_ManualMode.IsMainsContactorConfigured()) && (_ManualMode.IsMainsContactorClosed())
           && ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE)|| _GCUAlarms.RemoteStartConfigured()))
       {
           _hal.ledManager.led7.TurnOn();
       }
       else
       {
           _hal.ledManager.led7.TurnOff();
       }

    }
    _hal.ledManager.led1.ChangeLEDState((bsp_io_level_t)(!_hal.ObjKeypad.IsKey1PressedRaw()));
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
}

bool MAIN_UI::prvIsEnginNotInONstate()
{
    if( (_StartStop.GetStartStopSMDState() == START_STOP::ID_STATE_SS_ENG_OFF_ERR)
       ||(_StartStop.GetStartStopSMDState()== START_STOP::ID_STATE_SS_ENG_OFF_OK)
       ||(_StartStop.GetStartStopSMDState()== START_STOP::ID_STATE_SS_FAIL_TO_STOP)
       ||(_StartStop.GetStartStopSMDState()== START_STOP::ID_STATE_SS_STOPPING)
       ||(_StartStop.GetStartStopSMDState()== START_STOP::ID_STATE_SS_STOP_HOLD)
       ||(_ManualMode.GetManualModeState() == BASE_MODES::STATE_MANUAL_ENGINE_COOLING))
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

void MAIN_UI::prvUpadteBaseModeConfigDependency()
{
    _ManualMode.InitNightModeParam();
    _ManualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
    _ManualMode.ChangeAutoState(BASE_MODES::STATE_AMF_GEN_OFF_MAINS_ON);
    _ManualMode.SetMainsStatusHealthy();  /* Todo: need to discuss the need of this */

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE)
    {
      _ManualMode.OpenMainsLoad();
    }
}

bool MAIN_UI::prvIsSleepEnabled()
{
/*Shubham.Wader 17.09.2022
 This function returns true if the program need to execute sleep
 action. Function returns false means, critical execution is happening
 don't take sleep at all. This means need to kick sleep timer, so that
 it should not cross the set delay. */

#if 0
    if(((!IS_GCU_MANUAL_MODE() && !IS_GCU_AUTO_MODE()) || _EngMon.IsEngineOn()) &&
        ((IS_GCU_AUTO_MODE()) || ((_StartStop.GetStartStopSMDState() != START_STOP::ID_STATE_SS_ENG_OFF_ERR) &&
            (_StartStop.GetStartStopSMDState() != START_STOP::ID_STATE_SS_ENG_OFF_OK))))
#endif
//    if(   IS_DISP_EVENT_LOG_MODE() ||IS_DISP_CONFIG_MODE() ||IS_DISP_PASSWORD_ENTRY_MODE() || (!IS_GCU_MANUAL_MODE())
//       || ((_StartStop.GetStartStopSMDState() != START_STOP::ID_STATE_SS_ENG_OFF_ERR)
//           && (_StartStop.GetStartStopSMDState() != START_STOP::ID_STATE_SS_ENG_OFF_OK))
//       || ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE)
//           && (_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR) != ACT_Manager:: ACT_NOT_CONFIGURED))
//       || (IS_GCU_AUTO_MODE()
//           && (_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_REMOTE_START_STOP) != DigitalSensor::SENSOR_NOT_CONFIGRUED))
//       || _cfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_COMM_MODE)
//      )
//    {
//        return false;
//    }
//    else
//    {
//        return true;
//    }
//        Commented by SuryaPranayTeja as GC2111 should not enter Sleep in any conditions
        return false;
}

void MAIN_UI::prvHandleKeyPressEvent(KEYPAD::KEYPAD_EVENTS_t sKeyEvent)
{

/* Immediate actions on key press */
    switch(sKeyEvent)
    {
       case KEYPAD::STOP_KEY_LONG_PRESS: /* enter/exit config mode */
       {
           if(IS_DISP_MON_MODE() || IS_DISP_ALARM_MODE())
           {
               if(  IS_GCU_MANUAL_MODE()
                   && (!_ManualMode.IsGenRunTimersInProcess())
                   && (!_EngMon.IsEngineCranked())
                   && (!_EngMon.IsEngineOn()))
               {
                   MON_UI::eDisplayMode = DISP_PASSWORD_EDIT_MODE;
                   _bRefresh = true;
               }
               else
               {
                  /* do nothing */
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
               _MonUI.Update(_bRefresh);
               _PasswordEntry.SetPasswordEntryState(PASSWORD_ENTRY_UI::ENTRY);
           }
           else
           {
              /* do nothing */
           }
       }
       break;

       case UP_DN_KEY_LONG_PRESS: /* enter/exit event log mode */
       {
           if(IS_DISP_CONFIG_MODE())
           {
               MON_UI::eDisplayMode = DISP_EVENT_LOG_MODE;
               _bEventLogEntry = true;
           }
           else if(IS_DISP_EVENT_LOG_MODE())
           {
               MON_UI::eDisplayMode = DISP_CONFIG_MODE;
               _bEventLogEntry = false;
               _objUI.Handler(CKeyCodes::NONE);
           }
           else
           {
              /* do nothing */
           }
       }
       break;
       case STOP_KEY_SHORT_PRESS:  /* Escape key Press, engine stop(manual mode) */
       {
           if(IS_DISP_PASSWORD_ENTRY_MODE())
           {
               if(_PasswordEntry.GetPasswordEnteryState() == PASSWORD_ENTRY_UI::ENTRY)
               {    /* config read mode */
                    MON_UI::eDisplayMode = DISP_CONFIG_MODE;
                   _objUI.Handler(CKeyCodes::NONE);
               }
               else
               {
                  /* do nothing */
               }
           }

           if(IS_DISP_ALARM_MODE())
           {   /* Move to home screen */
               MON_UI::eDisplayMode = DISP_MON_MODE;
               _MonUI.GoToHomeScreen();
               _MonUI.Update(_bRefresh);
           }
           else
           {
               /* Do nothing */
           }
       }
       break;

       case START_KEY_SHORT_PRESS:
       case AUTO_KEY_LONG_PRESS:
       case AUTO_KEY_SHORT_PRESS:
       case DN_STOP_KEY_LONG_PRESS:
       {
           if(IS_DISP_ALARM_MODE())
           {
               MON_UI::eDisplayMode = DISP_MON_MODE;
               _MonUI.GoToHomeScreen();
               _MonUI.Update(_bRefresh);
           }
           else
           {
               /* do nothing */
           }
       }
       break;

       case GEN_CONT_KEY_PRESS:
       {
           if(IS_GCU_MANUAL_MODE() || IS_GCU_TEST_MODE())
           {
                if(_ManualMode.IsGenContactorConfigured())
                {
                    if((!_ManualMode.IsGenContactorClosed()) && (_EngMon.IsGenAvailable()))
                    {
                        _ManualMode.SwitchLoadToGen();
                    }
                    else
                    {
                        _ManualMode.OpenGenLoad();
                    }
                }
                else
                {
                    /* do nothing */
                }
           }
           else
           {
               /* do nothing */
           }
       }
       break;

       case MAINS_CONT_KEY_PRESS:
       {
           if(IS_GCU_MANUAL_MODE() || IS_GCU_TEST_MODE())
           {
               if((_ManualMode.IsMainsContactorConfigured()))
               {
                   if((!_ManualMode.IsMainsContactorClosed()) && (_ManualMode.GetMainsStatus() == BASE_MODES::MAINS_HELATHY))
                   {
                       _ManualMode.SwitchLoadToMains();
                   }
                   else
                   {
                       _ManualMode.OpenMainsLoad();
                   }
               }
               else
               {
                   /* do nothing */
               }
           }
           else
           {
               /* do nothing */
           }
       }
       break;

       default:
           break;
    }

/* Pass keypress status to respective execuctions */
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

void MAIN_UI::prvHandleDisplayModes()
{
    /* This private function handles update functions of all display modes */
    switch(MON_UI::eDisplayMode)
    {
       case DISP_MON_MODE:
       {
           _MonUI.Update(_bRefresh);
       }
        break;
       case DISP_ALARM_MODE:
       {
           if(_GCUAlarms.IsAlarmPresent())
           {
               UTILS_ResetTimer(&_ScreenChangeOverTimer);
           }
           else
           {
               /* do nothing */
           }
           /*
            * SuryaPranayTeja.BVV 14-11-2022
            * Added the below MON UI Update function so that the Auto or Mode Select input switched will work
            * Even if the input is given in Alarm Screen
            */
           _MonUI.Update(false);
           _DispAlarm.Update(_bRefresh);
       }
        break;

       case DISP_EVENT_LOG_MODE:
       {
           UTILS_ResetTimer(&_PoweSaveModeTimer);
           _DispEventLog.Update(_bEventLogEntry);
           _bEventLogEntry = false;
       }
        break;

       case DISP_PASSWORD_EDIT_MODE:
       {
           UTILS_ResetTimer(&_PoweSaveModeTimer);
          _PasswordEntry.Update(_bRefresh);
          if( _PasswordEntry.CheckPasswordStatus())
          {
              MON_UI::eDisplayMode = DISP_CONFIG_MODE;
              _objUI.Handler(CKeyCodes::NONE);
          }
          else
          {
            /* do nothing */
          }
       }
        break;
       case DISP_CONFIG_MODE:
       {
           UTILS_ResetTimer(&_PoweSaveModeTimer);
           _objUI.ConfigUIUpdate();
          if((_sKeyEvent ==  DN_LONG_PRESS ) || (_sKeyEvent ==  UP_LONG_PRESS ))
          {
              _objUI.ConfigCheckKeyPress(_sKeyEvent);
          }
          else
          {
            /* do nothing */
          }
       }
        break;

        default:
        break;
    }
}

void MAIN_UI::prvHandleScreenChangeover()
{
    /* This function functionally move the monitering/ alarm screen to the "next" screen */
    if(UTILS_GetElapsedTimeInSec(&_ScreenChangeOverTimer) >= _u16ScreenChangeTime)
    {
        _hal.ObjGlcd.AutoReInitGCLDScreen();
        UTILS_ResetTimer(&_ScreenChangeOverTimer);
       _u16ScreenChangeTime = _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SCRN_CHNGOVER_TIME);
       if(IS_DISP_MON_MODE())
       {
            _MonUI.CheckKeyPress(DN_SHORT_PRESS);
       }
       else if(IS_DISP_ALARM_MODE())
       {
           _DispAlarm.CheckKeyPress(DN_SHORT_PRESS);
       }
       else
       {
            /* Do nothing */
        }
    }
    else
    {
        /* do nothing */
    }
}
