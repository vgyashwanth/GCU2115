/**
 * @file        [MON_UI]
 * @brief       This module is used to display Monitoring screens
 *              with the help go Display class and CONST_UI.h
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        15th July 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#include <HMI/ALARM_UI/ALARM_UI.h>
#include <HMI/MON_UI/MON_UI.h>
#include "AC_SENSE_DS.h"
#include "../CONST_UI/CONST_UI.h"
#include "../UI_DS.h"

#define FUEL_VOLT_MAX_VAL   5.5

bool    MON_UI::_bMBModeChnageCMDRcvd = false;
uint8_t MON_UI::_stScreenNo           = MON_UI::DISP_MON_HOME;
MODE_TYPE_t MON_UI::eDisplayMode      = DISP_MON_MODE;
MON_UI::MON_SCREEN_st MON_UI::eMonScreenType = MON_SCREEN_NORMAL;

MON_UI::MON_UI(HAL_Manager &hal, MANUAL_MODE &manualMode,
        ENGINE_MONITORING &_engineMonitoring, CFGZ &cfgz, START_STOP &startStop,
        GCU_ALARMS &GCUAlarms, Display &Disp, CFGC &CFGC, J1939APP &j1939 , BTS_MODE &BTSMode, CYCLIC_MODE &cyclicMode):
_hal(hal),
_manualMode(manualMode),
_EngineMon(_engineMonitoring),
_cfgz(cfgz),
_startStop(startStop),
_GCUAlarms(GCUAlarms),
_Disp(Disp),
_cfgc(CFGC),
_j1939(j1939),
_BTSMode(BTSMode),
_cyclicMode(cyclicMode),
_LogoTimer{0,false},
_eOpMode (BASE_MODES::MANUAL_MODE),
ExternalInputUpdateTimer{0, false},
AutomaticModeSwitchingTimer{0, false},
_ArrScreenEnDs{false,false,false,
    false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false}, /* initialize with all screen disabled */
_u8LanguageIndex(0),
_u8ScreenMin(DISP_MON_HOME),
_u8ScreenMax(DISP_MON_LAST)
{
    UTILS_ResetTimer(&ExternalInputUpdateTimer);
    UTILS_DisableTimer(&AutomaticModeSwitchingTimer);
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_POWER_ON_LAMP_TEST) == CFGZ::CFGZ_ENABLE)
    {
        //Turn on all LED's if power on lamp test is enabled.
        _hal.ledManager.led1.TurnOn();
        _hal.ledManager.led2.TurnOn();
        _hal.ledManager.led3.TurnOn();
        _hal.ledManager.led4.TurnOn();
        _hal.ledManager.led5.TurnOn();
        _hal.ledManager.led6.TurnOn();
        _hal.ledManager.led7.TurnOn();
        _hal.ledManager.led8.TurnOn();
        _hal.ledManager.led9.TurnOn();
        _hal.ledManager.led10.TurnOn();
        _hal.ledManager.led11.TurnOn();
    }
    UTILS_ResetTimer(&_LogoTimer);
    UTILS_ResetTimer(&_GroupSwitchingTimer); /* TODO: remove timer if not using */

    prvDisplayBootLogo();
    Init();
}

void MON_UI::Init()
{
   prvConfigureScreenEnable(); /* Enable or disable the screens */

   UTILS_DisableTimer(&AutomaticModeSwitchingTimer);
   GCU_ALARMS::_bModeSwitchAlarm = false;
   GCU_ALARMS::_bAutomaticModeSwitchStatus = false;

   /*
    * By SuryaPranayTeja.BVV
    * The if else logic below is used for setting the mode
    * on "power on" based on the CFGZ parameter
    * Mode in auto is based on below conditions
    */
   if(_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_POWER_ON_MODE) == CFGZ::CFGZ_GENSET_MODE_MANUAL)
   {
       _eOpMode = BASE_MODES::MANUAL_MODE;
   }
   else
   {
       if((_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON) == CFGZ::CFGZ_ENABLE)
               ||(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2))
       {
           _eOpMode = BASE_MODES::BTS_MODE;
       }
       else if(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_CYCLIC_MODE) == CFGZ::CFGZ_ENABLE)
       {
           _eOpMode = BASE_MODES::CYCLIC_MODE;
       }
       else
       {
           _eOpMode = BASE_MODES::AUTO_MODE;
       }
   }
   _manualMode.SetGCUOperatingMode(_eOpMode);

/*
 * FIXME:How about not allowing a Contrast value less than 10 and if less than 10 still maintain minimum as 10?
 */
   _hal.ObjGlcd.AdjustContrast(_cfgz.GetCFGZ_Param(CFGZ::ID_DISPLAY_CONTRAST));
   _u8LanguageIndex = _cfgz.GetArrLanguageIndex();
   /*default settings */
    eDisplayMode = DISP_MON_MODE;
    GoToHomeScreen();
}

void MON_UI::Update(bool bRefresh)
{
    static STARTING_STATE_t  stBootstate = WAIT_FOR_BOOT_LOGO ;

    if((_cfgz.IsC03Error() || _cfgc.IsC02Error()) && bRefresh)
    {
        prvDisplayError();
        return ;
    }

    switch(stBootstate)
    {
        case WAIT_FOR_BOOT_LOGO:
        {
            //Wait for Logo display time
            if(UTILS_GetElapsedTimeInMs(&_LogoTimer) >= LOGO_TIMER_MS)
            {
                stBootstate = RUNNING_MODE;
               UTILS_DisableTimer(&_LogoTimer);
               //Turn Off all LED's
               _hal.ledManager.led1.TurnOff();
               _hal.ledManager.led2.TurnOff();
               _hal.ledManager.led3.TurnOff();
               _hal.ledManager.led4.TurnOff();
               _hal.ledManager.led5.TurnOff();
               _hal.ledManager.led6.TurnOff();
               _hal.ledManager.led7.TurnOff();
               _hal.ledManager.led8.TurnOff();
               _hal.ledManager.led9.TurnOff();
               _hal.ledManager.led10.TurnOff();
               _hal.ledManager.led11.TurnOff();
            }
            break;
        }
        case RUNNING_MODE:
        {
            if(bRefresh)
            {
                /* Need to uncomment and modify below J1939 related changes */
                // if(eMonScreenType == MON_SCREEN_J1939)
                // {
                //     prvPGNScreenDataAssignment(u8ArrMatrixDispAndRXPGN[_stScreenNo-DISP_MON_SCREEN_LAST-1]);
                // }
                prvDisplayMonScreen();
            }

            if(UTILS_GetElapsedTimeInMs(&ExternalInputUpdateTimer) >= EXT_INPUT_UPDATE_TIME_MS)
            {
                UTILS_ResetTimer(&ExternalInputUpdateTimer);

                if(_startStop.IsSimStopReceived())
                {
                    if(_EngineMon.IsModeSelectInputConfigured())
                    {
                        if(_manualMode.GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)
                        {
                            prvStopKeyPressAction();
                        }
                    }
                    else
                    {
                        prvStopKeyPressAction();
                    }
                }
                else if(_startStop.IsSimStartReceived())
                {
                    prvStartKeyPressAction();
                }
                else
                {
                    if(_EngineMon.IsModeSelectInputConfigured() && _startStop.IsModeSwitchAutoKeyReceived())
                    {
                        _startStop.ClearModeSwitchKeyRecieved();
                        prvAutoKeyPressAction();
                    }
                    else if(!_EngineMon.IsModeSelectInputConfigured() && (_startStop.IsSimAutoReceived() || _bMBModeChnageCMDRcvd))
                    {
                        prvAutoKeyPressAction();
                    }
                }
            }

            if(_manualMode.IsTestModeTimerExpired())
            {
                _manualMode.Stop();
                _manualMode.DisableTestModeTimer();
                _manualMode.ComeOutOfTestMode();
                _eOpMode = BASE_MODES::MANUAL_MODE;
                _manualMode.SetGCUOperatingMode(_eOpMode);
            }

            if((_manualMode.GetTimerState() != BASE_MODES::NO_TIMER_RUNNING)
                    &&(_manualMode.GetTimerState() < BASE_MODES::TEST_MODE_TIMER)
                    &&(eMonScreenType == MON_SCREEN_NORMAL))
            {
                _stScreenNo = DISP_MON_HOME;
                eMonScreenType = MON_SCREEN_NORMAL;
            }

            prvHandleAutomaticModeSwitching();

        }
        break;

        default: break;
        }

    if(eMonScreenType ==MON_SCREEN_NORMAL )
    {
        _u8ScreenMin= 0;
        _u8ScreenMax = DISP_MON_LAST;
    }
#if ENABLE_MON_J1939
    else
    {
        _u8ScreenMin= DISP_PROPB62_PGN_65378;
        _u8ScreenMax = DISP_J1939_RX_LAST;
    }
#endif
}

void MON_UI::GoToHomeScreen()
{
    _stScreenNo = DISP_MON_HOME;
    eMonScreenType =MON_SCREEN_NORMAL;
}

void MON_UI::CheckKeyPress(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent)
{
    switch(_sKeyEvent)
    {
        case AUTO_KEY_SHORT_PRESS:
        {
            if(!_EngineMon.IsModeSelectInputConfigured())
            {
                prvAutoKeyPressAction();
                GoToHomeScreen();
                eDisplayMode = DISP_MON_MODE;
            }
        }
        break;

        case START_KEY_SHORT_PRESS:
        {
            prvStartKeyPressAction();
            GoToHomeScreen();
            eDisplayMode = DISP_MON_MODE;
        }
        break;

        case STOP_KEY_SHORT_PRESS:
        {
            if(_EngineMon.IsModeSelectInputConfigured())
            {
                if(_manualMode.GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)
                {
                    prvStopKeyPressAction();
                }
            }
            else
            {
                prvStopKeyPressAction();
            }
        }
        break;

        case UP_SHORT_PRESS:  //Up key Press
        {
            /*
             * SuryaPranayTeja.Bvv 13-10-2022
             * The below functionality is added as in NXP
             * by UP key press also the Sounder Alarm gets turned off
             * and we continue to support it
             */
            if(_hal.actuators.GetActStatus(ACTUATOR::ACT_AUDIBLE_ALARM) == ACT_Manager::ACT_LATCHED)
            {
                _GCUAlarms.TurnOffSounderAlarm();
            }


            if(_cfgz.GetEngType() != CFGZ::CFGZ_CONVENTIONAL)
            {
                if((_stScreenNo == DISP_LAMP_ICONS) && (_cfgz.GetEngType() == CFGZ::ECU_162))
                {
                    MON_UI::_stScreenNo = DISP_MON_LAST;
                    eDisplayMode = DISP_ALARM_MODE;
                    ALARM_UI::ClearAlarmScreenNum();
                    ALARM_UI::ChangeAlarmScreenType(ALARM_UI::ALARM);
                }
                else if(_stScreenNo == DISP_EXAFTERTREAT_ICONS)
                {
                    MON_UI::_stScreenNo = DISP_MON_LAST;
                    eDisplayMode = DISP_ALARM_MODE;
                    ALARM_UI::ClearAlarmScreenNum();
                    ALARM_UI::ChangeAlarmScreenType(ALARM_UI::ALARM);
                }
            }
            else
            {
                if(_stScreenNo == DISP_MON_HOME)
                  {
                      MON_UI::_stScreenNo = DISP_MON_LAST;
                      eDisplayMode = DISP_ALARM_MODE;
                      ALARM_UI::ClearAlarmScreenNum();
                      ALARM_UI::ChangeAlarmScreenType(ALARM_UI::ALARM);
                  }
            }



//            if(_stScreenNo == _u8ScreenMin)
//            {
//                eDisplayMode = DISP_ALARM_MODE;
//                ALARM_UI::ClearAlarmScreenNum();
//
//                if(eMonScreenType == MON_SCREEN_NORMAL)
//                {
//                    MON_UI::_stScreenNo = _u8ScreenMax;
//                    eDisplayMode = DISP_ALARM_MODE;
//                    ALARM_UI::ClearAlarmScreenNum();
//                    ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM1);
//                }
//                else if(eMonScreenType == MON_SCREEN_J1939)
//                {
//                    MON_UI::_stScreenNo = DISP_MON_HOME;
//                }
//            }
            _stScreenNo--;
            while(_ArrScreenEnDs[_stScreenNo] == false)
            {
                _stScreenNo--;
            }
        }
        break;

        case DN_SHORT_PRESS: //Down key Press
        {
            /*
             * SuryaPranayTeja.BVV
             * The condition inside the while and after execution of while are same.
             * But the conditions need to be present as if the while condition
             * is false for all the screens then array index goes out of bounds
             * In case of Up key this issue will not be there as Home Screen is always enabled.(i.e Min Screen).
             * So array index does not go out of bounds.
             */
            //Note Also the while check for _stScreenNo should be placed first unlike the UpKey press.
            _stScreenNo++;
            while(_ArrScreenEnDs[_stScreenNo] == false)
            {
                _stScreenNo++;
                if(_stScreenNo >= _u8ScreenMax)
                {

                    if(_cfgz.GetEngType() != CFGZ::CFGZ_CONVENTIONAL)
                    {
                        if(_cfgz.GetEngType() != CFGZ::ECU_162)
                        {
                            MON_UI::_stScreenNo = DISP_EXAFTERTREAT_ICONS;
                        }
                        else
                        {
                            MON_UI::_stScreenNo = DISP_LAMP_ICONS;
                        }
                    }
                    else
                    {
                         MON_UI::_stScreenNo = DISP_MON_HOME;
                    }
//                    _stScreenNo = _u8ScreenMin;

//                    ALARM_UI::ChangeAlarmScreenType(ALARM_UI::ALARM);

                    if(_cfgz.GetEngType() != CFGZ::CFGZ_CONVENTIONAL)
                     {
                         ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM1);
                     }
                     else
                     {
                         ALARM_UI::ChangeAlarmScreenType(ALARM_UI::ALARM);
                     }
                    eDisplayMode = DISP_ALARM_MODE;
                    ALARM_UI::ClearAlarmScreenNum();

//                    if(eMonScreenType == MON_SCREEN_J1939)
//                    {
//                        MON_UI::_stScreenNo = DISP_MON_HOME;
//                        ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM2);
//                        _j1939.RequestDM2Messages();
//                    }
                }
            }

            if(MON_UI::_stScreenNo >= _u8ScreenMax)
            {
//                MON_UI::_stScreenNo = _u8ScreenMin;

                if((_cfgz.GetEngType() != CFGZ::CFGZ_CONVENTIONAL))
                {
                    if(_cfgz.GetEngType() != CFGZ::ECU_162)
                    {
                        MON_UI::_stScreenNo = DISP_EXAFTERTREAT_ICONS;
                    }
                    else
                    {
                        MON_UI::_stScreenNo = DISP_LAMP_ICONS;
                    }
                 }
                 else
                 {
                      MON_UI::_stScreenNo = DISP_MON_HOME;
                 }

//                ALARM_UI::ChangeAlarmScreenType(ALARM_UI::ALARM);
                if(_cfgz.GetEngType() != CFGZ::CFGZ_CONVENTIONAL)
                {
                   ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM1);
                }
                else
                {
                     ALARM_UI::ChangeAlarmScreenType(ALARM_UI::ALARM);
                }
                eDisplayMode = DISP_ALARM_MODE;
                ALARM_UI::ClearAlarmScreenNum();
//                if(eMonScreenType == MON_SCREEN_J1939)
//                {
//                    MON_UI::_stScreenNo = DISP_MON_HOME;
//                    ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM2);
//                    _j1939.RequestDM2Messages();
//                }
            }
        }
        break;

        case ACK_KEY_PRESS:
        {
            if(_hal.actuators.GetActStatus(ACTUATOR::ACT_AUDIBLE_ALARM) == ACT_Manager::ACT_LATCHED)
            {
                _GCUAlarms.TurnOffSounderAlarm();
            }
            _GCUAlarms.ClearAutoModeSwitchAlarm();
            _GCUAlarms.ClearAllAlarms();
            if(eMonScreenType == MON_SCREEN_NORMAL)
            {
                _stScreenNo = DISP_MON_HOME;
            }
        }
        break;

        case DN_STOP_KEY_LONG_PRESS:
        {
            //software reset to go in boot loader
            NVIC_SystemReset();

        }
        break;

        case DN_LONG_PRESS:
        {
            if(!_manualMode.IsGenRunTimersInProcess())
            {
//                if(eMonScreenType == MON_SCREEN_NORMAL)
//                {
//                    eDisplayMode = DISP_ALARM_MODE;
//                    ALARM_UI::ClearAlarmScreenNum();
//                    ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM2);
//                    _j1939.RequestDM2Messages();
//                }
                eMonScreenType = MON_SCREEN_NORMAL;
                _stScreenNo = DISP_MON_HOME;

            }
        }
        break;

        case AUTO_KEY_LONG_PRESS:
            if((_eOpMode == BASE_MODES::MANUAL_MODE)
                    && (_manualMode.GetManualModeState() == BASE_MODES::STATE_MANUAL_GEN_OFF))
            {
                GoToHomeScreen();
                _eOpMode = BASE_MODES::TEST_MODE;
                _manualMode.SetGCUOperatingMode(_eOpMode);
            }
            break;

        default:
            break;
    }
}

void MON_UI::prvConfigureScreenEnable()
{
    /*TODO :All the Screens of CAN are hard coded as disabled */
    static uint8_t u8Screen;
    if(CFGZ::CFGZ_CONVENTIONAL != _cfgz.GetEngType())
    {
      u8Screen = DISP_EXAFTERTREAT_ICONS;
    }
    else
    {
      u8Screen = DISP_MON_HOME;
    }
    for(; u8Screen<DISP_MON_LAST; u8Screen++)
    {
        _ArrScreenEnDs[u8Screen] = false;
        switch(u8Screen)
        {


            case DISP_ENGINE_TYPE :
                    _ArrScreenEnDs[u8Screen] = true;
                 break;

            case DISP_MON_ENG_LINK_INFO :
                if (CFGZ::CFGZ_CONVENTIONAL != _cfgz.GetEngType())
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_CAN_COMMUNICATION_INFO :
                if (CFGZ::CFGZ_CONVENTIONAL != _cfgz.GetEngType())
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_LAMP_ICONS :
                if ((CFGZ::CFGZ_CONVENTIONAL != _cfgz.GetEngType()))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_EXAFTERTREAT_ICONS :
                if ((CFGZ::CFGZ_CONVENTIONAL != _cfgz.GetEngType())
                    && (CFGZ::ECU_162 != _cfgz.GetEngType()))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_EEEC1 :
                if ((CFGZ::ECU_162 == _cfgz.GetEngType()) &&
                    (_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_SPEED_FROM_ENG) == CFGZ::CFGZ_ENABLE))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
            break;
            /* Previously below screen was developed for ECU162 and ECU_898, customer wants this to disable now.
               Later on customer may ask to enable this for some another engine types. */
            case DISP_EEEC2 :
            break;
            case DISP_EOI :
            if (CFGZ::ECU_162 == _cfgz.GetEngType())
            {
                _ArrScreenEnDs[u8Screen] = true;
            }
            break;
            case DISP_AT1T1I1 :
            {
                _ArrScreenEnDs[u8Screen] = false;
            }
            break;
            case DISP_HRS :
            if ((CFGZ::ECU_162 == _cfgz.GetEngType()) &&
                (_cfgz.GetCFGZ_Param(CFGZ::ID_RUNNING_HOURS_FROM_ECU) == CFGZ::CFGZ_ENABLE))
            {
                _ArrScreenEnDs[u8Screen] = true;
            }
            break;

            /* Previously below screen was developed for ECU162 and ECU_898, customer wants this to disable now.
               Later on customer may ask to enable this for some another engine types. */
            case DISP_LFC1 :
            {
                _ArrScreenEnDs[u8Screen] = false;
            }

            break;

            case DISP_ET1 :
            if ((CFGZ::ECU_162 == _cfgz.GetEngType()) &&
               (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG) == CFGZ::CFGZ_ENABLE))
            {
                _ArrScreenEnDs[u8Screen] = true;
            }
            break;
            case DISP_EFL :
            if ((CFGZ::ECU_162 == _cfgz.GetEngType())
                 &&
                (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG) == CFGZ::CFGZ_ENABLE))
            {
                _ArrScreenEnDs[u8Screen] = true;
            }
            break;
            case DISP_AMB :
            {
                _ArrScreenEnDs[u8Screen] = false;
            }
            break;
            case DISP_VEP1 :
            if((CFGZ::ECU_162 == _cfgz.GetEngType()) &&
                (_cfgz.GetCFGZ_Param(CFGZ::ID_BAT_VTG_FROM_ECU) == CFGZ::CFGZ_ENABLE))
            {
                _ArrScreenEnDs[u8Screen] = true;
            }
            break;
            case DISP_WFI :
            if(CFGZ::ECU_162 == _cfgz.GetEngType())
            {
                _ArrScreenEnDs[u8Screen] = true;
            }
            break;
            case DISP_DEFA :
            {
                _ArrScreenEnDs[u8Screen] = false;
            }
            break;
            case DISP_IC1 :
            {
                _ArrScreenEnDs[u8Screen] = false;
            }
            break;
            case DISP_SHUTDN:
            if(CFGZ::ECU_162 == _cfgz.GetEngType())
            {
                _ArrScreenEnDs[u8Screen] = true;
            }
            break;
            case DISP_CSA:
            if(CFGZ::ECU_162 == _cfgz.GetEngType())
            {
                _ArrScreenEnDs[u8Screen] = true;
            }
            break;
            case DISP_LFE1:
            {
                _ArrScreenEnDs[u8Screen] = false;
            }
            break;

            case DISP_MON_HOME :
            case DISP_MON_PRODUCT_ID :
                _ArrScreenEnDs[u8Screen] = true;
                break;
            case DISP_EGR_MON:
                if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_EGR_ECU_DIGITAL_IN) != DigitalSensor::SENSOR_NOT_CONFIGRUED)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
            break;
            case DISP_MON_GEN_VOLTAGE :
            case DISP_MON_GEN_LOAD_KW :
            case DISP_MON_GEN_LOAD_KVA :
            case DISP_MON_GEN_LOAD_KVAR :
            case DISP_MON_GEN_POWER_FACTOR :
            case DISP_MON_GEN_CURRENT :
            case DISP_MON_GEN_ENERGY :
                _ArrScreenEnDs[u8Screen] = true;
                break;
            case DISP_MON_MAINS_VOLTAGE :
            case DISP_MON_MAINS_CURRENT :
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_MAINS_LOAD_KW :
            case DISP_MON_MAINS_LOAD_KVA :
                /* mains mon on &&  mains contactor configured */
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE) &&
                   (_manualMode.IsMainsContactorConfigured()))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_MAINS_LOAD_KVAR :
            {
                _ArrScreenEnDs[u8Screen] = false; //Always disable as per NXP.
            }
            break;
            case DISP_MON_MAINS_ENERGY :
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE) &&
                   (_manualMode.IsMainsContactorConfigured()))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_MON_BAT_VOLTAGE :
                _ArrScreenEnDs[u8Screen] = true;
                break;
            case DISP_MON_CHRG_ALT_BAT_VOLTAGE :
                _ArrScreenEnDs[u8Screen] = false;
                break;
            case DISP_MON_AIR_INTAKE_TEMP :
            case DISP_MON_BOOST_PRESSURE :
                if(false)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_SITE_BAT_RUN_HRS :
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON) == CFGZ::CFGZ_ENABLE)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_TAMPERED_RUN_HRS :
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_WAVE_DETECTION) == CFGZ::CFGZ_ENABLE)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_ENG_TEMP :
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                    || (CFGZ::CFGZ_ENABLE == (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG))) )
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_LUBE_OIL_PRESSURE :
                if(((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                        ||((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) >= CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)))
                        ||(CFGZ::CFGZ_ENABLE == (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG))) )
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_FUEL :
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                    ||(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_SHELTER_TEMP :
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_CANOPY_TEMP :
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_AUX_2 :
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_ENG_SPEED :
            case DISP_MON_ENG_RUN_TIME :
                _ArrScreenEnDs[u8Screen] = true;
                break;

            default: break;
        }
    }
}

void MON_UI::prvDisplayMonScreen()
{
    /*
     * Every screen need this code.
     * The basic rectangle,Lines and icons, screen titles are printed.
     */
    static char* ArrImage;

    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.drawHorizontalLine( GLCD_X(0), GLCD_Y(19), GLCD_X(128));

    {
        _Disp.drawVerticalLine  ( GLCD_X(22), GLCD_Y(0), GLCD_Y(19));
        //Top corner icon for the screen
        switch(u8ArrGroupArray[_stScreenNo])
        {
            case DISP_GROUP_STATUS:
                ArrImage =(char*) &u8BmpLogoInfo[0];
                break;

            case DISP_GROUP_GENERATOR:
                ArrImage =(char*) &u8BmpLogoGen[0];
                break;

            case DISP_GROUP_MAINS:
                ArrImage =(char*) &u8BmpLogoMains[0];
                break;

            case DISP_GROUP_ENGINE:
                ArrImage =(char*) &u8BmpLogoEng[0];
                break;
            case DISP_GROUP_LOAD:
                ArrImage = (char*) &u8BmpLogoLoad[0];
                break;
            case DISP_SHELTER_TEMP:
                ArrImage = (char*) &u8BmpLogoshelterTemp[0];
                break;
            default:
                ArrImage =(char*) &u8BmpLogoInfo[0];
                break;
        }
        _Disp.printImage((uint8_t *)ArrImage, 3,20,0, 0);

        if(_stScreenNo != DISP_MON_MAINS_VOLTAGE)
        {    /* Mon screen title */
            _Disp.gotoxy(GLCD_X(75),GLCD_Y(5));
            _Disp.printStringCenterAligned((char *)strMonScreens[_u8LanguageIndex][_stScreenNo],FONT_ARIAL);
        }
    }

#if ENABLE_MON_J1939
    if((eMonScreenType == MON_SCREEN_J1939))
    {
        prvJ1939MonScreens();
    }
    else
#endif
    {
        prvNormalMonScreens();
        //In GC2111, while in test mode. T will be displayed at the left bottom corner.
        if(_eOpMode == BASE_MODES:: TEST_MODE && _stScreenNo !=DISP_MON_HOME)
        {
            _Disp.clearRectangularSection(GLCD_X(0)+1, GLCD_X(11)-1, GLCD_Y(52)+1, GLCD_Y(63)-1);
            _Disp.drawHorizontalLine(GLCD_X(0), GLCD_Y(52), GLCD_X(11));
            _Disp.drawVerticalLine(GLCD_X(11), GLCD_Y(52), GLCD_Y(63));
            _Disp.gotoxy(GLCD_X(6),GLCD_Y(53));
            _Disp.printStringCenterAligned((char *)"T", FONT_VERDANA);
        }
    }
}
void MON_UI::prvPrintCANMsgRcvError()
{
    _Disp.printStringCenterAligned((char*)strCANMsgRcvError[_u8LanguageIndex], FONT_ARIAL);
}

void MON_UI::prvPrintNotAvailable(uint8_t u8RowNum, uint8_t u8ColStart)
{
    _Disp.gotoxy(u8ColStart, u8RowNum);
    _Disp.printStringLeftAligned((char*)NotAvailable, FONT_ARIAL);
}

/************************************************************************************************/
unsigned int  MON_UI::prvGetNoOfFractionalDigits(double dNumber)
{
    unsigned int uiDigits = 0;

//    while(dNumber!=((int)dNumber))
//    {
//        uiDigits++;
//        dNumber=dNumber*10;
//    }

    do{
        ++uiDigits;
        dNumber *= 10;
    }
    while((int)(dNumber) % 10 != 0 );

    return --uiDigits;
}
/*
void MON_UI::prvAssignNoOfDigitAfterDP(SPNData_t *StrArrayPtr, uint8_t u8PGNEnumNo, uint8_t u8SpnNo)
{
        J1939APP::J1939_PGN_DB_t stGetPGN;
        J1939APP::J1939_SPN_DB_t stGetSpn;
        unsigned int uiNoOfFractionalDigits, uiIndex;
        int64_t s64Multiplier;

        _j1939.GetPGN(u8PGNEnumNo, CALC_FOR_RECEIVE, &stGetPGN);
        _j1939.GetSPN(u8PGNEnumNo, u8SpnNo, CALC_FOR_RECEIVE, &stGetSpn);

        switch(u8PGNEnumNo)
        {
            case RX_PGN_EEC1_61444:
            case RX_PGN_EEC2_61443:
            case RX_PGN_EEC4_65214:
            case RX_PGN_EOI_64914:
                uiNoOfFractionalDigits = 0;
                break;

            case RX_PGN_AT1S2_64697:
            case RX_PGN_AT1S_64891:
            case RX_PGN_AT1OG2_64947:
            case RX_PGN_A2SCREGT1_64824:
            case RX_PGN_AT2OG1_61457:
            case RX_PGN_AT2IG1_61456:
            case RX_PGN_A1SCREGT1_64830:
            case RX_PGN_ET1_65262:
            case RX_PGN_LFE_65266:
            case RX_PGN_VEP1_65271:
            case RX_PGN_LFI_65203:
            case RX_PGN_ET2_65188:
                uiNoOfFractionalDigits = 1;
                break;
            case RX_PGN_EEC3_65247:
                if((u8SpnNo == J1939APP::ENG_DESIRED_SPEED)||(u8SpnNo == J1939APP::AFT1_EXHAUST_GAS_MASSS_FLOW_RATE))
                {
                    uiNoOfFractionalDigits = 1;
                }
                else
                {
                    uiNoOfFractionalDigits = 0;
                }
                break;
            case RX_PGN_HATZ_CCVS_65265:
                 if(u8SpnNo == 1)
                 {
                     uiNoOfFractionalDigits = 2;
                 }
                 else
                 {
                     uiNoOfFractionalDigits = 0;
                 }
                 break;
            case RX_PGN_DD_65276:
            case RX_PGN_EFL_P2_65243:
            case RX_PGN_A1DOC_64800:
            case RX_PGN_AT1IG2_64948:
                uiNoOfFractionalDigits = 2;
                break;

            case RX_PGN_EFL_P1_65263:
                if(u8SpnNo == J1939APP::ENG_OIL_LVL_SPN_98 || u8SpnNo == J1939APP::ENG_COOLANT_LVL_SPN_111)
                {
                    uiNoOfFractionalDigits = 1;
                }
                else
                {
                    uiNoOfFractionalDigits = 2;
                }
                break;
            case RX_PGN_IC1_65270:
                if(u8SpnNo == J1939APP::ENG_AIR_FILT_DIFF_PRESS_SPN_107)
                {
                    uiNoOfFractionalDigits = 3;
                }
                else if(u8SpnNo == J1939APP::ENG_INTAKE_MANIFOLD_TEMP_SPN_105 || u8SpnNo == J1939APP::ENG_EXHAUST_GAS_TEMP_SPN_173)
                {
                    uiNoOfFractionalDigits = 1;
                }
                else
                {
                    uiNoOfFractionalDigits = 2;
                }
                break;
            case RX_PGN_AMB_65269:
                if(u8SpnNo == J1939APP::BAROMETER_PRESSURE_SPN_108)
                {
                    uiNoOfFractionalDigits = 2;
                }
                else
                {
                    uiNoOfFractionalDigits = 0;
                }
                break;
            case RX_PGN_LFC_65257:
                if(u8SpnNo == J1939APP::ENG_TOTAL_FUEL_USED_SPN_250)
                {
                    uiNoOfFractionalDigits = 1;
                }
                else
                {
                    uiNoOfFractionalDigits = 0;
                }
                break;
            case RX_PGN_IC2_64976:
                if(u8SpnNo == J1939APP::ENG_AIR_FILT_PRESS_SPN_2809)
                {
                    uiNoOfFractionalDigits = 3;
                }
                else
                {
                    uiNoOfFractionalDigits = 2;
                }
                break;
            case RX_PGN_A2SCRDSI1_61478:
            case RX_PGN_A1SCRDSI1_61475:
            case RX_PGN_AT1OG1_61455:
            case RX_PGN_AT1IG1_61454:
                if(u8SpnNo == J1939APP::AT1_INTAKE_NOX_SPN_3216)
                {
                    uiNoOfFractionalDigits = 1;
                }
                else
                {
                    uiNoOfFractionalDigits = 2;
                }
                break;
            case RX_PGN_AT1T1I_65110:
                if(u8SpnNo == J1939APP::DISEL_EXHAUST_TANK_LVL_SPN_1761)
                {
                    uiNoOfFractionalDigits = 1;
                }
                else
                {
                    uiNoOfFractionalDigits = 0;
                }
                break;

            case RX_PGN_IVECO_ENG_STATUS_65282:
                if(u8SpnNo == J1939APP::OIL_PRESSURE_3)
                {
                    uiNoOfFractionalDigits = 2;
                }
                else
                {
                    uiNoOfFractionalDigits = 0;
                }
                break;
            case RX_PGN_IT1_65154:
                uiNoOfFractionalDigits = 2;
                break;
            case RX_PGN_EPT1_65187:
            case RX_PGN_EPT2_65186:
            case RX_PGN_EPT3_65185:
            case RX_PGN_EPT4_65184:
            case RX_PGN_EPT5_65183:
            case RX_PGN_TCI4_65176:
                uiNoOfFractionalDigits = 1;
                break;
            default:
                uiNoOfFractionalDigits = prvGetNoOfFractionalDigits(stGetSpn.ufSPN_Resolution);
                break;
        }

        s64Multiplier = 1;
        for(uiIndex = 1; uiIndex <= uiNoOfFractionalDigits; uiIndex++)
        {
            s64Multiplier = s64Multiplier * 10;
        }

        StrArrayPtr->u8DPInResolution = (uint8_t) uiNoOfFractionalDigits;
        StrArrayPtr->SPNStatusErrorNA = _j1939.GetSPNErrorStatus((DATABASE_RX_PGN_LIST_t)u8PGNEnumNo,u8SpnNo);
}
*/
void MON_UI::prvNormalMonScreens()
{
    char arrTemp[32] = {0};  //Local variable for storing the sprintf output

    A_SENSE::SENSOR_RET_t  stTemp;
    uint8_t u8Position;
    uint8_t u8Local;

    switch(_stScreenNo)
    {
        case DISP_MON_HOME:
        {
            _Disp.drawHorizontalLine(GLCD_X(0), GLCD_Y(35), GLCD_Y(128));
            _Disp.gotoxy(GLCD_X(3),GLCD_Y(21));
            if(_manualMode.GetGCUState()== MANUAL_MODE::ENGINE_STARTING)
            {
                sprintf(arrTemp,"%s %d/%d",strGCUStatus[_u8LanguageIndex][_manualMode.GetGCUState()],
                        _startStop.GetCrankAttemptNumber(), _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_START_ATTEMPTS));
            }
            else
            {
                sprintf(arrTemp,"%s", strGCUStatus[_u8LanguageIndex][_manualMode.GetGCUState()]);
            }

            _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(3),GLCD_Y(38));

            if(_manualMode.GetTimerState() != BASE_MODES::NO_TIMER_RUNNING)
            {
                if((_manualMode.GetTimerState() > BASE_MODES::TEST_MODE_TIMER) && ((_eOpMode == BASE_MODES::BTS_MODE)  || (_eOpMode == BASE_MODES::CYCLIC_MODE)))
                {
                    sprintf(arrTemp,"%s",strTimerStatus[_u8LanguageIndex][_manualMode.GetTimerState()]);
                    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                }
                else
                {
                    sprintf(arrTemp,"%s",strTimerStatus[_u8LanguageIndex][_manualMode.GetTimerState()]);
                    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                }

                _Disp.gotoxy(GLCD_X(100),GLCD_Y(38));
                if(_manualMode.GetTimerState() == BASE_MODES::COOL_DOWN_TIMER)
                {
                    sprintf(arrTemp," %d",(uint16_t)_manualMode.GetEngCoolDownRemTime());
                }
                else if(_manualMode.GetTimerState() == BASE_MODES::TEST_MODE_TIMER)
                {
                    sprintf(arrTemp," %d",(int)((_manualMode.GetTestModeRemTime()/ 60) + 1));
                }
                else if(_manualMode.GetTimerState() > BASE_MODES::TEST_MODE_TIMER)
                {
                    if(_eOpMode == BASE_MODES::BTS_MODE)
                    {
                        sprintf(arrTemp," %d",(int)(_BTSMode.GetBTSModeTime(_manualMode.GetTimerState())));
                    }
                    else if(_eOpMode == BASE_MODES::CYCLIC_MODE)
                    {
                        sprintf(arrTemp," %d", (int)(_cyclicMode.GetCyclicModeTime(_manualMode.GetTimerState())));
                    }
                    else
                    {
                        sprintf(arrTemp," %d",0);// Execution comes here for a moment at the time of mode switching.
                    }
                }
                else
                {
                    sprintf(arrTemp," %d",(uint16_t)_startStop.GetTimersRemainingTime(_manualMode.GetTimerState()));
                }
                _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
            }
            else
            {
                /* do nothing */
            }

            _Disp.gotoxy(GLCD_X(3),GLCD_Y(51));


            if(_manualMode.IsNightModeRestrictOn())
            {
                _Disp.printStringLeftAligned((char *)StrNightModeRestrict[_u8LanguageIndex],FONT_VERDANA);
            }
            else if(_eOpMode == BASE_MODES::AUTO_MODE)
            {
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING)== CFGZ::CFGZ_ENABLE)
                {
                    _Disp.printStringLeftAligned((char *)StrAutoAMF[_u8LanguageIndex],FONT_VERDANA);
                }
                else if(_GCUAlarms.IsAlarmMonEnabled(GCU_ALARMS::REMOTE_SS))
                {
                    _Disp.printStringLeftAligned((char *)StrAutoRemoteCmd[_u8LanguageIndex],FONT_VERDANA);
                }
                else
                {
                    _Disp.printStringLeftAligned((char *)strGCUMode[_u8LanguageIndex][ BASE_MODES::AUTO_MODE],FONT_VERDANA);
                }
            }
            else
            {
                _Disp.printStringLeftAligned((char *)strGCUMode[_u8LanguageIndex][_eOpMode],FONT_VERDANA);
            }

        }
        break;

        case DISP_MON_PRODUCT_ID:
        {
            prvProductInfo();
        }
        break;
        case DISP_ENGINE_TYPE :
        {
             _Disp.gotoxy(GLCD_X(64),GLCD_Y(35));
             _Disp.printStringCenterAligned((char *)EngType[_cfgz.GetEngType()],FONT_VERDANA);
        }
        break;
        case DISP_MON_CAN_COMMUNICATION_INFO:
        {
            sprintf(arrTemp,"%s","STATE : ");
            _Disp.gotoxy(GLCD_X(10),GLCD_Y(35));
            _Disp.printStringLeftAligned(arrTemp,FONT_VERDANA);
            if(_j1939.IsCommunicationFail())
            {
                sprintf(arrTemp,"%s","Bus Faulty");
            }
            else
            {
                sprintf(arrTemp,"%s","Bus Ok");
            }
            _Disp.gotoxy(GLCD_X(65),GLCD_Y(35));
            _Disp.printStringLeftAligned(arrTemp,FONT_VERDANA);
        }
        break;
        case DISP_MON_ENG_LINK_INFO:
        {
            sprintf(arrTemp,"%s","ECU Link : ");
            _Disp.gotoxy(GLCD_X(10),GLCD_Y(35));
            _Disp.printStringLeftAligned(arrTemp,FONT_VERDANA);
            if(_j1939.IsCommunicationFail())
            {
                sprintf(arrTemp,"%s","Not Ok");
            }
            else
            {
                sprintf(arrTemp,"%s","Ok");
            }
            _Disp.gotoxy(GLCD_X(75),GLCD_Y(35));
            _Disp.printStringLeftAligned(arrTemp,FONT_VERDANA);
        }
        break;

        case DISP_EXAFTERTREAT_ICONS :
            prvBuildExhaustIconScreen();
            break;

        case DISP_LAMP_ICONS:
             prvBuildLampIconScreen();
             break;


        case DISP_EEEC1:
        {
            _Disp.gotoxy(GLCD_X(2),GLCD_Y(33));
            _Disp.printStringLeftAligned((char *)"Engine speed:",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(120),GLCD_Y(33));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_EEC1_61444,0) == J1939APP::VALID_DATA))
            {
            sprintf(arrTemp, "%.2f RPM",(_j1939.GetReadData(RX_PGN_EEC1_61444,0)));
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_EEC1_61444,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_EEC1_61444,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp,StrCANErr);
                }
            }
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
        }
        break;
        case DISP_EEEC2:
        {
            _Disp.gotoxy(GLCD_X(2),GLCD_Y(33));
            _Disp.printStringLeftAligned((char *)"ACC PEDAL POS:",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(125),GLCD_Y(33));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_EEC2_61443,0) == J1939APP::VALID_DATA))
            {
            sprintf(arrTemp, "%.2f %%", (_j1939.GetReadData(RX_PGN_EEC2_61443, 0)));
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_EEC2_61443,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_EEC2_61443,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp,StrCANErr);
                }
            }
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
        }
        break;

        case DISP_EOI:
        {
            _Disp.gotoxy(GLCD_X(64),GLCD_Y(25));
            _Disp.printStringCenterAligned((char *)"ENG STATE:",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(64),GLCD_Y(40));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_EOI_64914,0) == J1939APP::VALID_DATA))
            {
                if ((uint8_t)(_j1939.GetReadData(RX_PGN_EOI_64914,0)) < MAX_NUM_OF_ENG_OPER_STATES - 1)
                {
                    sprintf(arrTemp,StrEngOperatingState[int((_j1939.GetReadData(RX_PGN_EOI_64914,0)))]); /* yet implement states */
                }
                else
                {
                    sprintf(arrTemp,StrEngOperatingState[MAX_NUM_OF_ENG_OPER_STATES - 1]);
                }

            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_EOI_64914,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_EOI_64914,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp,StrCANErr);
                }
            }
            _Disp.printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        }
        break;
        case DISP_AT1T1I1:
        {
            _Disp.gotoxy(GLCD_X(2),GLCD_Y(33));
            _Disp.printStringLeftAligned((char *)"DEF TNK VOL:",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(110),GLCD_Y(33));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_AT1T1I_65110,0) == J1939APP::VALID_DATA))
            {
            sprintf(arrTemp, "%.2f %%", (_j1939.GetReadData(RX_PGN_AT1T1I_65110,0)));
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_AT1T1I_65110,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_AT1T1I_65110,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp,StrCANErr);
                }
            }
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

        }
        break;
        case DISP_HRS:
        {
            _Disp.gotoxy(GLCD_X(64),GLCD_Y(25));
            _Disp.printStringCenterAligned((char *)"ENG TOTAL HRS:",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(64),GLCD_Y(40));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_HOURS_65253,0) == J1939APP::VALID_DATA))
            {
                sprintf(arrTemp,"%luhrs  %umin ", (uint32_t)( _GCUAlarms.GetSelectedEngRunMin()/60), (uint8_t)(_GCUAlarms.GetSelectedEngRunMin()%60) );
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_HOURS_65253,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_HOURS_65253,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp,StrCANErr);
                }
            }
            _Disp.printStringCenterAligned((char *)arrTemp,FONT_VERDANA);

        }
        break;
        case DISP_LFC1:
        {
            _Disp.gotoxy(GLCD_X(2),GLCD_Y(20));
            _Disp.printStringLeftAligned((char *)"ENG FUEL USED:",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(125),GLCD_Y(20));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_LFC1_65257,0) == J1939APP::VALID_DATA))
            {
            sprintf(arrTemp, "%.2fL",(_j1939.GetReadData(RX_PGN_LFC1_65257,0)));
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_LFC1_65257,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_LFC1_65257,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp,StrCANErr);
                }
            }
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);


            _Disp.gotoxy(GLCD_X(2),GLCD_Y(40));
            _Disp.printStringLeftAligned((char *)"ENG TRIP FUEL:",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(125),GLCD_Y(40));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_LFC1_65257,1) == J1939APP::VALID_DATA))
            {
            sprintf(arrTemp, "%.2fL",(_j1939.GetReadData(RX_PGN_LFC1_65257,1)));
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_LFC1_65257,1) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_LFC1_65257,1) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp,StrCANErr);
                }
            }
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
        }
        break;
        case DISP_ET1:
        {

            _Disp.gotoxy(GLCD_X(2),GLCD_Y(25));
            _Disp.printStringLeftAligned((char *)"ENG CLNT TEMP :",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(125),GLCD_Y(25));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_ET1_65262,0) == J1939APP::VALID_DATA))
            {
            sprintf(arrTemp, "%.2f`C",(_j1939.GetReadData(RX_PGN_ET1_65262,0)));
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_ET1_65262,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_ET1_65262,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp,StrCANErr);
                }
            }
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

        }
        break;
        case DISP_EFL:
        {
            _Disp.gotoxy(GLCD_X(64),GLCD_Y(25));
            _Disp.printStringCenterAligned((char *)"ENG OIL PRESS:",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(64),GLCD_Y(40));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_EFL_P1_65263,0) == J1939APP::VALID_DATA))
            {
            sprintf(arrTemp, "%.2f kPa", (_j1939.GetReadData(RX_PGN_EFL_P1_65263,0)));
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_EFL_P1_65263,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_EFL_P1_65263,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp,StrCANErr);
                }
            }
            _Disp.printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        }
        break;
        case DISP_VEP1:
        {
            _Disp.gotoxy(GLCD_X(64),GLCD_Y(25));
            _Disp.printStringCenterAligned((char *)"BAT POTENTIAL",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(64),GLCD_Y(40));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_VEP1_65271,0) == J1939APP::VALID_DATA))
            {
                 sprintf(arrTemp, "%.2f V", (_j1939.GetReadData(RX_PGN_VEP1_65271,0)));
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_VEP1_65271,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_VEP1_65271,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp,StrCANErr);
                }
            }
             _Disp.printStringCenterAligned((char *)arrTemp,FONT_VERDANA);

        }
        break;
        case DISP_WFI:
        {

            _Disp.gotoxy(GLCD_X(2),GLCD_Y(25));
            _Disp.printStringLeftAligned((char *)"WATER IN FUEL:",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(80),GLCD_Y(25));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_WFI_OI_65279,0) == J1939APP::VALID_DATA))
            {
                if((uint16_t)(_j1939.GetReadData(RX_PGN_WFI_OI_65279,0)) < MAX_NUM_OF_WFI_STATES)
                {
                    sprintf(arrTemp, StrWFIState[(uint16_t)(_j1939.GetReadData(RX_PGN_WFI_OI_65279,0))]);
                }
                else
                {
                    sprintf(arrTemp,"INVLD");
                }
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_WFI_OI_65279,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_WFI_OI_65279,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp, StrCANErr);
                }
            }
            _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

        }
        break;
        case DISP_DEFA:
        {

            _Disp.gotoxy(GLCD_X(2),GLCD_Y(25));
            _Disp.printStringLeftAligned((char *)"DEF QUA IND  :",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(75),GLCD_Y(25));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_DEFA_65383,0) == J1939APP::VALID_DATA))
            {
                if((uint16_t)(_j1939.GetReadData(RX_PGN_DEFA_65383,0)) < MAX_NUM_OF_DEFA_STATE)
                {
                    sprintf(arrTemp, StrDEFAStatus[(uint16_t)(_j1939.GetReadData(RX_PGN_DEFA_65383,0))]);
                }
                else
                {
                    sprintf(arrTemp,"INVLD");
                }
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_DEFA_65383,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_DEFA_65383,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp, StrCANErr);
                }
            }
            _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);


            _Disp.gotoxy(GLCD_X(2),GLCD_Y(40));
            _Disp.printStringLeftAligned((char *)"SCR TAMP IND:",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(75),GLCD_Y(40));

             if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_DEFA_65383,1) == J1939APP::VALID_DATA))
            {
                if((uint16_t)(_j1939.GetReadData(RX_PGN_DEFA_65383,1)) < MAX_NUM_OF_DRV_WARN)
                {
                    sprintf(arrTemp, StrDEFAStatus[(uint16_t)(_j1939.GetReadData(RX_PGN_DEFA_65383,1))]);
                }
                else
                {
                    sprintf(arrTemp,"INVLD");
                }
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_DEFA_65383,1) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_DEFA_65383,1) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp, StrCANErr);
                }
            }
            _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

        }
        break;
        case DISP_IC1:
        {
            _Disp.gotoxy(GLCD_X(2),GLCD_Y(25));
            _Disp.printStringLeftAligned((char *)"ENG MF PRESS :",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(125),GLCD_Y(25));
            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_IC1_65270,0) == J1939APP::VALID_DATA))
            {
                sprintf(arrTemp, "%.2f kPa", (_j1939.GetReadData(RX_PGN_IC1_65270,0)));
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_IC1_65270,0) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_IC1_65270,0) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp, StrCANErr);
                }
            }
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(2),GLCD_Y(40));
            _Disp.printStringLeftAligned((char *)"ENG EXST TEMP:",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(127),GLCD_Y(40));

            if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_IC1_65270,1) == J1939APP::VALID_DATA))
            {
             sprintf(arrTemp, "%.2f `C", (_j1939.GetReadData(RX_PGN_IC1_65270,1)));
            }
            else
            {
                if(_j1939.GetSPNErrorStatus(RX_PGN_IC1_65270,1) ==  J1939APP::NOT_AVAILABLE)
                {
                    sprintf(arrTemp,StrNotAvailable);
                }
                else if(_j1939.GetSPNErrorStatus(RX_PGN_IC1_65270,1) == J1939APP::ERROR)
                {
                    sprintf(arrTemp,StrErr);
                }
                else
                {
                    sprintf(arrTemp, StrCANErr);
                }
            }
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

        }
        break;
        case DISP_SHUTDN:
         {
             _Disp.gotoxy(GLCD_X(64),GLCD_Y(25));
             _Disp.printStringCenterAligned((char *)"GLOW LAMP STATUS",FONT_VERDANA);
             _Disp.gotoxy(GLCD_X(64),GLCD_Y(40));
             if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_SHUTDN_65252,0) == J1939APP::VALID_DATA))
             {
                if((int)(_j1939.GetReadData(RX_PGN_SHUTDN_65252,0)) < PGN_65252_STRING_OPTIONS)
                {
                    sprintf(arrTemp,StrPGN_65252_states[(int)(_j1939.GetReadData(RX_PGN_SHUTDN_65252,0))]);
                }
                else
                {
                    sprintf(arrTemp,"INVLD");
                }
             }
             else
             {
                 if(_j1939.GetSPNErrorStatus(RX_PGN_SHUTDN_65252,0) ==  J1939APP::NOT_AVAILABLE)
                 {
                     sprintf(arrTemp,StrNotAvailable);
                 }
                 else if(_j1939.GetSPNErrorStatus(RX_PGN_SHUTDN_65252,0) == J1939APP::ERROR)
                 {
                     sprintf(arrTemp,StrErr);
                 }
                 else
                 {
                     sprintf(arrTemp,StrCANErr);
                 }
             }
             _Disp.printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
         }
         break;
         case DISP_CSA:
         {
             _Disp.gotoxy(GLCD_X(64),GLCD_Y(25));
             _Disp.printStringCenterAligned((char *)"Cold Start Aids",FONT_VERDANA);
             _Disp.gotoxy(GLCD_X(64),GLCD_Y(40));
             if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_CSA_64966,0) == J1939APP::VALID_DATA))
             {
                if((int)(_j1939.GetReadData(RX_PGN_CSA_64966,0)) < PGN_64966_STRING_OPTIONS)
                {
                    sprintf(arrTemp,StrPGN_64966_states[(int)(_j1939.GetReadData(RX_PGN_CSA_64966,0))]);
                }
                else
                {
                    sprintf(arrTemp,"INVLD");
                }
             }
             else
             {
                 if(_j1939.GetSPNErrorStatus(RX_PGN_CSA_64966,0) ==  J1939APP::NOT_AVAILABLE)
                 {
                     sprintf(arrTemp,StrNotAvailable);
                 }
                 else if(_j1939.GetSPNErrorStatus(RX_PGN_CSA_64966,0) == J1939APP::ERROR)
                 {
                     sprintf(arrTemp,StrErr);
                 }
                 else
                 {
                     sprintf(arrTemp,StrCANErr);
                 }
             }
             _Disp.printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
         }
         break;

         case DISP_EGR_MON:
         {
            _Disp.gotoxy(GLCD_X(3),GLCD_Y(21));
            sprintf(arrTemp, "Status: %s", strEgrFault[(uint8_t)_GCUAlarms.GetEgrEcuFaultStatus()]);
            _Disp.printStringLeftAligned(arrTemp,FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(3),GLCD_Y(36));
            sprintf(arrTemp, "Fault Time: %dMin" , _GCUAlarms.GetFaultPreset72HrsTimeInMin());
            _Disp.printStringLeftAligned(arrTemp,FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(3),GLCD_Y(51));
            sprintf(arrTemp, "Heal Time : %dMin", _GCUAlarms.GetFaultReset40HrsTimeInMin());
            _Disp.printStringLeftAligned(arrTemp,FONT_VERDANA);

         }
         break;

        case DISP_MON_GEN_VOLTAGE:
        {
            prvPrintVoltageData(GENSET,_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM));
        }
        break;

        case DISP_MON_GEN_LOAD_KW:
        {
            {
                prvPrintPower(ACTIVE,_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM), GENSET);
            }
        }
        break;
        case DISP_MON_GEN_LOAD_KVA:
        {
            {
                prvPrintPower(APARENT,
                              _cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM),GENSET);
            }

        }
        break;
        case DISP_MON_GEN_LOAD_KVAR:
        {
            {
                prvPrintPower(REACTIVE,
                              _cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM),GENSET);
            }
        }
        break;
        case DISP_MON_GEN_POWER_FACTOR:
        {
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM) ==
                    CFGZ::CFGZ_3_PHASE_SYSTEM)
            {
                _Disp.drawVerticalLine(GLCD_X(50), GLCD_Y(19), GLCD_Y(64));
                sprintf(arrTemp,"%0.2f",abs(_hal.AcSensors.GENSET_GetDispAveragePowerFactor()));
                _Disp.gotoxy(GLCD_X(22),GLCD_Y(33));
                _Disp.printStringCenterAligned((char *)arrTemp,FONT_ARIAL);
                _Disp.gotoxy(GLCD_X(23),GLCD_Y(50));
                _Disp.printStringCenterAligned((char *)StrPF,FONT_ARIAL);
                u8Position = 22;
                for(u8Local = R_PHASE; u8Local < PHASE_END ; u8Local++)
                {
                    sprintf(arrTemp,"%s-%s    %0.2f",(char *)StrPF,
                            (char *)strPhase[u8Local],
                            abs(_hal.AcSensors.GENSET_GetDispPowerFactor((PHASE_t)u8Local)));
                    _Disp.gotoxy(GLCD_X(55),GLCD_Y(u8Position));
                    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                    u8Position = u8Position + 15;
                }
            }
            else
            {
                _Disp.printImage((uint8_t *)gau8GenPFLogo, 4, 30, 26, 7);
                sprintf(arrTemp,"%s-%s    %0.2f",(char *)StrPF,
                        (char *)"Ph-N",
                        abs(_hal.AcSensors.GENSET_GetDispPowerFactor((PHASE_t)R_PHASE)));
                _Disp.gotoxy(GLCD_X(48),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
            }
        }
        break;

        case DISP_MON_GEN_CURRENT:
        {
            static float f32GenMinFreq = 0.0f;
            f32GenMinFreq = _GCUAlarms.GetMinGenFreq();
            u8Position = 22;

            _pGET_VAL_t ArrGetCurrentVal[TYPE_LAST]=
            {
             &AC_SENSE::GENSET_GetCurrentAmps,
             &AC_SENSE::MAINS_GetCurrentAmps,
            };
            //Look Keypress_processor.c Line 1740 in NXP code. Similarly for Mains and also look into Assignment of Load on Gen and Load and Mains.
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM)== CFGZ::CFGZ_3_PHASE_SYSTEM))
            {
                for(u8Local = R_PHASE; u8Local < PHASE_END ; u8Local++)
                {
                    _Disp.gotoxy(GLCD_X(10),GLCD_Y(u8Position));
                    _Disp.printStringLeftAligned((char *)strPhase[u8Local],FONT_VERDANA);

                    _Disp.gotoxy(GLCD_X(50),GLCD_Y(u8Position));
                    sprintf(arrTemp,"%0.1f",((&_hal.AcSensors)->*ArrGetCurrentVal[GENSET])((PHASE_t)u8Local));
                    _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

                    _Disp.gotoxy(GLCD_X(52),GLCD_Y(u8Position));
                    _Disp.printStringLeftAligned((char *)StrA, FONT_VERDANA);

                    u8Position = u8Position + 15;
                }

                _Disp.gotoxy(GLCD_X(100),GLCD_Y(35));
                sprintf(arrTemp,"%0.1f", f32GenMinFreq);
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
                _Disp.gotoxy(GLCD_X(102),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)"Hz", FONT_VERDANA);
            }
            else
            {
                /* Below logo will be displayed for Gen and Mains as well */
                _Disp.printImage((uint8_t *)gau8GeneratorVoltLogo, 4, 32, 30, 7);

                _Disp.gotoxy(GLCD_X(40),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)"Ph-N",FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(83),GLCD_Y(35));
                sprintf(arrTemp,"%0.1f",((&_hal.AcSensors)->*ArrGetCurrentVal[GENSET])((PHASE_t)R_PHASE));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(85),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)StrA, FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(110),GLCD_Y(35));
                sprintf(arrTemp,"%0.1f", f32GenMinFreq);
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
                _Disp.gotoxy(GLCD_X(112),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)"Hz", FONT_VERDANA);
            }

            
        }


        break;
        case DISP_MON_GEN_ENERGY:
        {
            _Disp.printImage((uint8_t *)u8ArrDollar, 4, 32, 26, 7);
            _pGetdoubleValCommon_t ArrGetVal[POWER_LAST]=
            {
             &AC_SENSE::GENSET_GetTotalActiveEnergySinceInitWH,
             &AC_SENSE::GENSET_GetTotalApparentEnergySinceInitVAH,
             &AC_SENSE::GENSET_GetTotalReactiveEnergySinceInitVARH
            };

            u8Position = 22;
            for( u8Local = ACTIVE; u8Local < POWER_LAST ; u8Local++)
            {
                sprintf(arrTemp,"   %0.1lf",
                        ((&_hal.AcSensors)->*ArrGetVal[u8Local])() /1000);
                _Disp.gotoxy(GLCD_X(97),GLCD_Y(u8Position));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
                _Disp.gotoxy(GLCD_X(99),GLCD_Y(u8Position));
                _Disp.printStringLeftAligned((char *)strEnergy[u8Local],
                                             FONT_VERDANA);
                u8Position = u8Position + 15;
                /*todo: need to verify co ordinate adjustments */
            }
        }
        break;

        case DISP_MON_MAINS_VOLTAGE:
        {
            prvPrintVoltageData(MAINS,_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM));
        }
        break;

        case DISP_MON_MAINS_LOAD_KW :
        {
            prvPrintPower(ACTIVE, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM), MAINS);
        }
        break;

        case DISP_MON_MAINS_LOAD_KVA :
        {
            prvPrintPower(APARENT, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM),MAINS);
        }
        break;

        case DISP_MON_MAINS_LOAD_KVAR :
        {
            prvPrintPower(REACTIVE, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM),MAINS);
        }
        break;
        case DISP_MON_MAINS_CURRENT :
        {
            static float f32MainsFreq = 0.0f;
            f32MainsFreq = _GCUAlarms.GetMinMainsFreq();
            u8Position = 22;
            SOURCE_TYPE_t eSysType = MAINS;
            _pGET_VAL_t ArrGetCurrentVal[TYPE_LAST]=
            {
             &AC_SENSE::GENSET_GetCurrentAmps,
             &AC_SENSE::MAINS_GetCurrentAmps,
            };

            if((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM) || (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH) == CFGZ::CFGZ_ENABLE))
            {
                for(u8Local = R_PHASE; u8Local < PHASE_END ; u8Local++)
                {
                    _Disp.gotoxy(GLCD_X(10),GLCD_Y(u8Position));
                    _Disp.printStringLeftAligned((char *)strPhase[u8Local],FONT_VERDANA);

                    _Disp.gotoxy(GLCD_X(50),GLCD_Y(u8Position));
                    sprintf(arrTemp,"%.1f",((&_hal.AcSensors)->*ArrGetCurrentVal[eSysType])((PHASE_t)u8Local));
                    _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

                    _Disp.gotoxy(GLCD_X(52),GLCD_Y(u8Position));
                    _Disp.printStringLeftAligned((char *)StrA, FONT_VERDANA);

                    u8Position = u8Position + 15;
                }
            }
            else
            {
                _Disp.gotoxy(GLCD_X(10),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)strPhase[R_PHASE],FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(50),GLCD_Y(35));
                sprintf(arrTemp,"%0.1f",((&_hal.AcSensors)->*ArrGetCurrentVal[eSysType])((PHASE_t)R_PHASE));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(52),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)StrA, FONT_VERDANA);
            }

            _Disp.gotoxy(GLCD_X(100),GLCD_Y(35));
            sprintf(arrTemp,"%0.1f", f32MainsFreq);
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(102),GLCD_Y(35));
            _Disp.printStringLeftAligned((char *)"Hz", FONT_VERDANA);
        }
        break;

        case DISP_MON_MAINS_ENERGY:
        {
            _Disp.printImage((uint8_t *)u8ArrDollar, 4, 32, 26, 7);
            _pGetdoubleValCommon_t ArrGetVal[POWER_LAST]=
            {
             &AC_SENSE::MAINS_GetTotalActiveEnergySinceInitWH,
             &AC_SENSE::MAINS_GetTotalApparentEnergySinceInitVAH,
             &AC_SENSE::MAINS_GetTotalReactiveEnergySinceInitVARH
            };

            u8Position = 22;
            for( u8Local = ACTIVE; u8Local < REACTIVE ; u8Local++)
            {
                sprintf(arrTemp,"   %0.1lf",
                        ((&_hal.AcSensors)->*ArrGetVal[u8Local])() /1000);
                _Disp.gotoxy(GLCD_X(97),GLCD_Y(u8Position));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                _Disp.gotoxy(GLCD_X(99),GLCD_Y(u8Position));
                _Disp.printStringLeftAligned((char *)strEnergy[u8Local],
                                             FONT_VERDANA);
                u8Position = u8Position + 15;
            }
            _Disp.gotoxy(GLCD_X(124),GLCD_Y(52));

            sprintf(arrTemp,"%ldhrs  %dmin ", ( _EngineMon.GetMainsRunTimeMin()/60), (uint8_t)(_EngineMon.GetMainsRunTimeMin()%60) );
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

        }
        break;

        case DISP_MON_BAT_VOLTAGE:
        {
            /* VBAT */
            _Disp.printImage((uint8_t *)u8ArrBattery, 4, 32, 26, 2);
            {
                _Disp.gotoxy(GLCD_X(40),GLCD_Y(26));
                _Disp.printStringLeftAligned((char *)"ENG BATT",FONT_VERDANA);
            }
            _Disp.gotoxy(GLCD_X(112),GLCD_Y(26));
            if(CFGZ::CFGZ_ENABLE == _cfgz.GetCFGZ_Param(CFGZ::ID_BAT_VTG_FROM_ECU))
             {
                 if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_VEP1_65271,0) == J1939APP::VALID_DATA))
                 {
                      sprintf(arrTemp,"%0.1f", (_j1939.GetReadData(RX_PGN_VEP1_65271,0)));
                      _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                      _Disp.gotoxy(GLCD_X(116),GLCD_Y(26));
                      _Disp.printStringLeftAligned((char*)"V",FONT_VERDANA);
                 }
                 else
                 {
                     if(_j1939.GetSPNErrorStatus(RX_PGN_VEP1_65271,0) ==  J1939APP::NOT_AVAILABLE)
                     {
                         sprintf(arrTemp,StrNotAvailable);
                         _Disp.printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
                     }
                     else if(_j1939.GetSPNErrorStatus(RX_PGN_VEP1_65271,0) == J1939APP::ERROR)
                     {
                         sprintf(arrTemp,StrErr);
                         _Disp.printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
                     }
                     else
                     {
                         sprintf(arrTemp,StrCANErr);
                         _Disp.printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
                     }
                 }
             }
             else
             {
                 sprintf(arrTemp,"%0.1f", (_hal.AnalogSensors.GetFilteredVbattVolts()));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                _Disp.gotoxy(GLCD_X(116),GLCD_Y(26));
                _Disp.printStringLeftAligned((char*)"V",FONT_VERDANA);
             }
             
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON) == CFGZ::CFGZ_ENABLE)
            {
                /* V BTS */

                _Disp.gotoxy(GLCD_X(40),GLCD_Y(47));
                _Disp.printStringLeftAligned((char *)"BTS BATT",FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(112),GLCD_Y(47));
                sprintf(arrTemp,"%0.1f",
                        _hal.AnalogSensors.GetFilteredVBTSbattVolts());
                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                _Disp.gotoxy(GLCD_X(116),GLCD_Y(47));
                _Disp.printStringLeftAligned((char*)"V",FONT_VERDANA);
            }

        }
        break;

        case DISP_MON_CHRG_ALT_BAT_VOLTAGE:
        {
            _Disp.printImage((uint8_t *)u8ArrBattery, 4, 32, 26, 2);
            _Disp.gotoxy(GLCD_X(40),GLCD_Y(33));
            _Disp.printStringLeftAligned((char *)"CA VOLT",FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(112),GLCD_Y(33));
            sprintf(arrTemp,"%0.1f",
                    _hal.AnalogSensors.GetFilteredChargingAltVolts());
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
            _Disp.gotoxy(GLCD_X(116),GLCD_Y(33));
            _Disp.printStringLeftAligned((char*)"V",FONT_VERDANA);
        }
        break;

        case DISP_MON_AIR_INTAKE_TEMP :
        {
            _Disp.printImage((uint8_t *)u8ArrEngineTemp, 4, 32, 26, 7);
            /* todo: add J1939 dependency. Values will only receive on J1939 */
            _Disp.gotoxy(GLCD_X(40),GLCD_Y(33));
            _Disp.printStringLeftAligned((char *)"Under Development",FONT_VERDANA);
        }
        break;

        case DISP_MON_BOOST_PRESSURE :
        {
            _Disp.printImage((uint8_t *)u8ArrOilPressure, 4, 32, 26, 3);
            /* todo: add J1939 dependency. Values will only receive on J1939 */
            _Disp.gotoxy(GLCD_X(40),GLCD_Y(33));
            _Disp.printStringLeftAligned((char *)"Under Development",FONT_VERDANA);
        }
        break;

        case DISP_MON_SITE_BAT_RUN_HRS:
        {
            _Disp.printImage((uint8_t *)u8ArrEngineHours, 4, 32, 26, 7);

            _Disp.gotoxy(GLCD_X(68),GLCD_Y(35));
            sprintf(arrTemp, "%ld",(_EngineMon.GetBTSRunTimeMin()/60));
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
            _Disp.gotoxy(GLCD_X(70),GLCD_Y(35));
            _Disp.printStringLeftAligned((char *)"hrs",FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(100),GLCD_Y(35));
            sprintf(arrTemp, "%ld",(_EngineMon.GetBTSRunTimeMin()%60));
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
            _Disp.gotoxy(GLCD_X(102),GLCD_Y(35));
            _Disp.printStringLeftAligned((char *)"min",FONT_VERDANA);
        }
        break;
        case DISP_MON_TAMPERED_RUN_HRS:
        {
            _Disp.printImage((uint8_t *)u8ArrEngineHours, 4, 32, 26, 7);
            {
                _Disp.gotoxy(GLCD_X(120),GLCD_Y(28));
                sprintf(arrTemp,"%0.1f kWh",(_hal.AcSensors.GENSET_GetTotalTamperedActiveEnergySinceInitWH()) /1000);
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(120),GLCD_Y(42));
                sprintf(arrTemp,"%ld hrs %d min",(_EngineMon.GetTamperedRunTimeMin()/60), (uint8_t)(_EngineMon.GetTamperedRunTimeMin()%60));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
            }
        }
        break;

        case DISP_MON_ENG_TEMP:
        {

            _Disp.printImage((uint8_t *)u8ArrEngineTemp, 4, 32, 26, 7);

            if((CFGZ::CFGZ_ENABLE == (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG)))
                && (_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) != CFGZ::CFGZ_CONVENTIONAL))
            {
                _Disp.gotoxy(GLCD_X(64),GLCD_Y(32));
                if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_ET1_65262,0) == J1939APP::VALID_DATA))
                {
                    sprintf(arrTemp, "%.2f`C", (float)(_j1939.GetReadData(RX_PGN_ET1_65262,0)));
                }
                else
                {
                    if(_j1939.GetSPNErrorStatus(RX_PGN_ET1_65262,0) ==  J1939APP::NOT_AVAILABLE)
                    {
                        sprintf(arrTemp,StrNotAvailable);
                    }
                    else if(_j1939.GetSPNErrorStatus(RX_PGN_ET1_65262,0) == J1939APP::ERROR)
                    {
                        sprintf(arrTemp,StrErr);
                    }
                    else
                    {
                        sprintf(arrTemp,StrCANErr);
                    }
                }
              _Disp.printStringCenterAligned(arrTemp,FONT_VERDANA);
            }
            else
            {
                stTemp =_GCUAlarms.GetSelectedTempSensVal();
                _Disp.gotoxy(GLCD_X(115),GLCD_Y(33));
                prvPrintSensorStatus(stTemp,(char*)"`C", INTEGER_TYPE);
                if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
                {
                    sprintf(arrTemp,"%d",(int16_t)( (stTemp.stValAndStatus.f32InstSensorVal *DEG_F_FACTOR1) + DEG_F_FACTOR2 ));
                    _Disp.gotoxy(GLCD_X(93),GLCD_Y(42));
                    _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                    _Disp.gotoxy(GLCD_X(95),GLCD_Y(42));
                    _Disp.printStringLeftAligned((char*)"`F",FONT_VERDANA);
                }
            }
        }
        break;

        case DISP_MON_LUBE_OIL_PRESSURE:
        {
            _Disp.printImage((uint8_t *)u8ArrOilPressure, 4, 32, 26, 3);

            if(CFGZ::CFGZ_ENABLE == (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG)))
            {
                _Disp.gotoxy(GLCD_X(64),GLCD_Y(32));
                if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_EFL_P1_65263,0) == J1939APP::VALID_DATA))
                {
                    /* 1Kpa = 0.01 Bar. Received data in Kpa. */
                    sprintf(arrTemp, "%.2f Bar", (float)((_j1939.GetReadData(RX_PGN_EFL_P1_65263,0)) * 0.01f));
                }
                else
                {
                    if(_j1939.GetSPNErrorStatus(RX_PGN_EFL_P1_65263,0) ==  J1939APP::NOT_AVAILABLE)
                    {
                        sprintf(arrTemp,StrNotAvailable);
                    }
                    else if(_j1939.GetSPNErrorStatus(RX_PGN_EFL_P1_65263,0) == J1939APP::ERROR)
                    {
                        sprintf(arrTemp,StrErr);
                    }
                    else
                    {
                        sprintf(arrTemp,StrCANErr);
                    }
                }
                _Disp.printStringCenterAligned(arrTemp,FONT_VERDANA);
            }
            else
            {
                stTemp = _GCUAlarms.GetLOPSensorVal();
                _Disp.gotoxy(GLCD_X(115),GLCD_Y(33));

                if(stTemp.eStatus == A_SENSE::SENSOR_NOT_CONFIGRUED)
                {
                    _Disp.gotoxy(GLCD_X(70),GLCD_Y(37));
                    _Disp.printStringLeftAligned((char*)StrNotConfigured,FONT_VERDANA);
                }
                else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
                {
                    if(_GCUAlarms.AlarmResultInstat(GCU_ALARMS::LOP_CURR_SENS_OVER_CURR))
                    {
                        _Disp.gotoxy(GLCD_X(78),GLCD_Y(37));
                        _Disp.printStringCenterAligned((char *)"Over Value",FONT_ARIAL);
                    }
                    else
                    {
                        sprintf(arrTemp,"%0.1f",stTemp.stValAndStatus.f32InstSensorVal);
                        _Disp.gotoxy(GLCD_X(93),GLCD_Y(25));
                        _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                        _Disp.gotoxy(GLCD_X(94),GLCD_Y(25));
                        _Disp.printStringLeftAligned((char*)"bar",FONT_VERDANA);
                        sprintf(arrTemp,"%d",(uint16_t)( stTemp.stValAndStatus.f32InstSensorVal * PSI_CONVERSION));
                        _Disp.gotoxy(GLCD_X(93),GLCD_Y(42));
                        _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                        _Disp.gotoxy(GLCD_X(94),GLCD_Y(42));
                        _Disp.printStringLeftAligned((char*)"psi",FONT_VERDANA);
                    }
                }
                else
                {
                    prvPrintSensorFaultStatus(stTemp , 78, 37);
                }
            }
        }
        break;
        case DISP_MON_FUEL:
        {
            _Disp.printImage((uint8_t *)u8ArrFuel, 4, 32, 26, 7);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
            {

                stTemp =_hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);
                /*Converting from percentage to liters*/
                stTemp.stValAndStatus.f32InstSensorVal =
                            stTemp.stValAndStatus.f32InstSensorVal
                            * _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY)/100;
                prvPrintSensorStatus(stTemp,(char*)"Liters", FLOAT_TYPE);

                if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
                {
                    //The sensor value is type casted with uint16_t to avoid rapid fluctuations of display
                    uint32_t u16RunTimeExpect = (uint32_t) (((uint16_t)stTemp.stValAndStatus.f32InstSensorVal * 60)/_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION));

                    _Disp.gotoxy(GLCD_X(120),GLCD_Y(37));
                    sprintf(arrTemp,"%ld hrs %d min",(u16RunTimeExpect/60), (uint8_t)(u16RunTimeExpect%60));
                    _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

                    if(_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_IN_PCT) == CFGZ::CFGZ_ENABLE)
                    {
                        stTemp =_hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);
                        sprintf(arrTemp,"%d",(uint16_t)((round)(stTemp.stValAndStatus.f32InstSensorVal)));
                        _Disp.gotoxy(GLCD_X(93),GLCD_Y(50));
                        _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                        _Disp.gotoxy(GLCD_X(94),GLCD_Y(50));
                        _Disp.printStringLeftAligned((char*)"%",FONT_VERDANA);

                    }
                }
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
            {
                stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_0_TO_5V);
                float f32FuelPctValue = stTemp.stValAndStatus.f32InstSensorVal;
                /*Convert from percentage to liters*/
                stTemp.stValAndStatus.f32InstSensorVal = prvPin23ConvFuelLvlToLit(f32FuelPctValue);

                if(!((_hal.AnalogSensors.GetS4VoltVal() > FUEL_VOLT_MAX_VAL) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)))
                {
                    prvPrintSensorStatus(stTemp,(char*)"Liters", FLOAT_TYPE);
                }

                if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
                {
                    if(_hal.AnalogSensors.GetS4VoltVal() > FUEL_VOLT_MAX_VAL)
                    {
                        _Disp.gotoxy(GLCD_X(78),GLCD_Y(37));
                        _Disp.printStringCenterAligned((char *)"Over Value",FONT_ARIAL);
                    }
                    else
                    {
                        
                        //The sensor value is type casted with uint16_t to avoid rapid fluctuations of display
                        uint32_t u16RunTimeExpect = (uint32_t) (((uint16_t)stTemp.stValAndStatus.f32InstSensorVal * 60)/_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_FUEL_CONSUMPTION));

                        _Disp.gotoxy(GLCD_X(120),GLCD_Y(37));
                        sprintf(arrTemp,"%ld hrs %d min",(u16RunTimeExpect/60), (uint8_t)(u16RunTimeExpect%60));
                        _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);


                        if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_FUEL_IN_PCT) == CFGZ::CFGZ_ENABLE)
                        {
                            sprintf(arrTemp,"%.0f",f32FuelPctValue);
                            _Disp.gotoxy(GLCD_X(93),GLCD_Y(50));
                            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                            _Disp.gotoxy(GLCD_X(94),GLCD_Y(50));
                            _Disp.printStringLeftAligned((char*)"%",FONT_VERDANA);
                        }
                    }
                }
            }
        }
        break;

        case DISP_MON_SHELTER_TEMP:
        {
            _Disp.printImage((uint8_t *)gau8LIShelterTemp, 4, 32, 26, 7);
            _Disp.gotoxy(GLCD_X(110),GLCD_Y(33));

            stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_SHELTER_TEMPERATURE);
            prvPrintSensorStatus(stTemp,(char*)"`C", INTEGER_TYPE);
            if(stTemp.eStatus != A_SENSE::SENSOR_NOT_CONFIGRUED)
            {
                if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
                {
                    sprintf(arrTemp,"%d",(int16_t)( (stTemp.stValAndStatus.f32InstSensorVal *DEG_F_FACTOR1) + DEG_F_FACTOR2 ));
                    _Disp.gotoxy(GLCD_X(93),GLCD_Y(42));
                    _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                    _Disp.gotoxy(GLCD_X(95),GLCD_Y(42));
                    _Disp.printStringLeftAligned((char*)"`F",FONT_VERDANA);
                }
            }
        }
        break;

        case DISP_MON_CANOPY_TEMP:
        {
            _Disp.printImage((uint8_t *)gau8LIShelterTemp, 4, 32, 26, 7);
            _Disp.gotoxy(GLCD_X(110),GLCD_Y(33));

            stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_CANOPY_TEMPERATURE);
            prvPrintSensorStatus(stTemp,(char*)"`C", INTEGER_TYPE);
            if(stTemp.eStatus != A_SENSE::SENSOR_NOT_CONFIGRUED)
            {
                if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
                {
                    sprintf(arrTemp,"%d",(int16_t)( (stTemp.stValAndStatus.f32InstSensorVal *DEG_F_FACTOR1) + DEG_F_FACTOR2 ));
                    _Disp.gotoxy(GLCD_X(93),GLCD_Y(42));
                    _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                    _Disp.gotoxy(GLCD_X(95),GLCD_Y(42));
                    _Disp.printStringLeftAligned((char*)"`F",FONT_VERDANA);
                }  
            }
        }
        break;

        case DISP_MON_AUX_2:
        {
            stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S2_SENSOR);
            prvPrintSensorStatus(stTemp,(char*)"", FLOAT_TYPE , 80 , 37);
        }
        break;

        case DISP_MON_ENG_SPEED:
        {
            _Disp.printImage((uint8_t *)u8ArrEngineSpeed, 4, 32, 26, 7);

            _Disp.gotoxy(GLCD_X(100),GLCD_Y(37));

            if(CFGZ::CFGZ_ENABLE == (_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_SPEED_FROM_ENG)))
            {
                    if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_EEC1_61444,0) == J1939APP::VALID_DATA))
                {
                sprintf(arrTemp, "%.0fRPM",(float)(_j1939.GetReadData(RX_PGN_EEC1_61444,0)));
                }
                else
                {
                    if(_j1939.GetSPNErrorStatus(RX_PGN_EEC1_61444,0) ==  J1939APP::NOT_AVAILABLE)
                    {
                        sprintf(arrTemp,StrNotAvailable);
                    }
                    else if(_j1939.GetSPNErrorStatus(RX_PGN_EEC1_61444,0) == J1939APP::ERROR)
                    {
                        sprintf(arrTemp,StrErr);
                    }
                    else
                    {
                        sprintf(arrTemp,StrCANErr);
                    }
                }

            }
            else
            {
                sprintf(arrTemp,"%d RPM ",(uint16_t)
                                  round(_EngineMon.GetFilteredEngSpeed()));
            }


//            sprintf(arrTemp,"%d ",(uint16_t) _GCUAlarms.GetSpeedValue());
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);

//            _Disp.gotoxy(GLCD_X(85),GLCD_Y(37));
//            _Disp.printStringLeftAligned((char*)"RPM",FONT_VERDANA);

        }
        break;
        case DISP_MON_ENG_RUN_TIME:
        {
            _Disp.printImage((uint8_t *)u8ArrEngineHours, 4, 32, 26, 7);

            _Disp.gotoxy(GLCD_X(124),GLCD_Y(22));

            if(CFGZ::CFGZ_ENABLE == (_cfgz.GetCFGZ_Param(CFGZ::ID_RUNNING_HOURS_FROM_ECU)))
            {
                if((!_j1939.IsCommunicationFail()) && (_j1939.GetSPNErrorStatus(RX_PGN_HOURS_65253,0) == J1939APP::VALID_DATA))
                {
/*
Abhishek   Date- 14-09-2022
if engine run hours goes beyond below mentioned value it is going out of screen so I Confirmed with Devendra D that if its value goes beyond that value then display 0.
*/
                    if((uint32_t)(_GCUAlarms.GetSelectedEngRunMin()/60) > 999999)
                    {
                        sprintf(arrTemp, "%.0f hr %.0f min",(double)0,(double)0);
                    }
                    else
                    {
                        sprintf(arrTemp,"%luhrs  %umin ", (uint32_t)( _GCUAlarms.GetSelectedEngRunMin()/60), (uint8_t)(_GCUAlarms.GetSelectedEngRunMin()%60) );
                    }
                }
                else
                {
                    if(_j1939.GetSPNErrorStatus(RX_PGN_HOURS_65253,0) ==  J1939APP::NOT_AVAILABLE)
                    {
                        sprintf(arrTemp,StrNotAvailable);
                    }
                    else if(_j1939.GetSPNErrorStatus(RX_PGN_HOURS_65253,0) == J1939APP::ERROR)
                    {
                        sprintf(arrTemp,StrErr);
                    }
                    else
                    {
                        sprintf(arrTemp,StrCANErr);
                    }
                }
            }
            else
            {
                sprintf(arrTemp,"%luhrs  %umin ", (uint32_t)( _GCUAlarms.GetSelectedEngRunMin()/60), (uint8_t)(_GCUAlarms.GetSelectedEngRunMin()%60) );
            }
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(40),GLCD_Y(37));
            _Disp.printStringLeftAligned((char *)StrStarts[_u8LanguageIndex],FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(40),GLCD_Y(52));
            _Disp.printStringLeftAligned((char *)StrTrips[_u8LanguageIndex],FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(90),GLCD_Y(37));
            sprintf(arrTemp,"%d ",  (uint16_t)_EngineMon.GetEngineNoOfStarts() );
            _Disp.printStringLeftAligned((char *)arrTemp,FONT_ARIAL);

            _Disp.gotoxy(GLCD_X(90),GLCD_Y(52));
            sprintf(arrTemp,"%d ",  (uint16_t)_EngineMon.GetEngineNoOfTrips() );
            _Disp.printStringLeftAligned((char *)arrTemp,FONT_ARIAL);
        }
        break;

        default:
            break;
    }
}

float MON_UI::prvPin23ConvFuelLvlToLit(float f32FuelLvlPct)
{
    float f32FuelLvlLit = 0.0F;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_WITH_STEP) == CFGZ::CFGZ_ENABLE)
    {
        f32FuelLvlLit =
                f32FuelLvlPct
                * (float)((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_HEIGHT_1)* _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_LENGTH_1))
                        + (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_HEIGHT_2)* _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_LENGTH_2)))
                        *(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_WIDTH))/(1000000 * 100);
    }
    else
    {
        f32FuelLvlLit =
                f32FuelLvlPct
                * (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_HEIGHT_1)* _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_LENGTH_1))
                *(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_WIDTH))/(1000000 * 100);
    }
    return f32FuelLvlLit;
}

void MON_UI::prvDisplayBootLogo()
{
    char arrTemp[32];
    sprintf(arrTemp,"Loading...");
    _Disp.gotoxy(GLCD_X(64),GLCD_Y(32));
    _Disp.printStringCenterAligned((char *)arrTemp,FONT_ARIAL);
}

void MON_UI::prvProductInfo()
{
    char arrTemp[32];

    RTC::TIME_t CurrentTime;
    _hal.ObjRTC.GetTime(&CurrentTime);

    char SeriesId[3];
    _cfgc.GetSeriesProductID(SeriesId);

    sprintf(arrTemp,"Ver :");
    _Disp.gotoxy(GLCD_X(2),GLCD_Y(22));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

    sprintf(arrTemp,"%s%d-R%02d.%02d",SeriesId,_cfgc.GetProductID(),_cfgc.GetProductRevision(),_cfgc.GetProductVersion());
    _Disp.gotoxy(GLCD_X(43),GLCD_Y(22));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

/*    sprintf(arrTemp,"Eng Sr  :");
    _Disp.gotoxy(GLCD_X(2),GLCD_Y(34));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);*/

/*
 * SuryaPranayTeja.BVV
 * Engine Serial number does not have all the ASCII characters
 * So the below loop copy the appropriate character and display
 *
 * Shubham Wader 02.03.2023
 * To address the issue "intersection of characters in engine seril number",
 * firstly the characters from engine serial number is pushed in an array (arrTemp)
 * and then sent the same for display.
 */
/*    UI::GetSrNoByIndex(CEditableItem::SRNO_ENGINE, (uint8_t*)arrTemp);
    arrTemp[SR_NOS_MAX_SIZE - 1] = '\0';

    _Disp.gotoxy(GLCD_X(43),GLCD_Y(34));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
*/
    sprintf(arrTemp,(char*)StrInfoDate[_u8LanguageIndex]);
    _Disp.gotoxy(GLCD_X(2),GLCD_Y(34)); //47 is for the third line. Eng Sr no is removed, and RTC time is moved to 2nd row
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

    sprintf(arrTemp,"%d.%s.%d",CurrentTime.u8Day, (char*)StrMonth[_u8LanguageIndex][CurrentTime.u8Month - 1],CurrentTime.u16Year);
    _Disp.gotoxy(GLCD_X(48),GLCD_Y(34));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

    //Blinking ":" for every second
    if(CurrentTime.u8Second % 2)
    {
        sprintf(arrTemp,"%d:%02d",CurrentTime.u8Hour, CurrentTime.u8Minute );
    }
    else
    {
        sprintf(arrTemp,"%d %02d",CurrentTime.u8Hour, CurrentTime.u8Minute );
    }
    _Disp.gotoxy(GLCD_X(102),GLCD_Y(34));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
}

bool MON_UI::prvIsValDigitsGreaterThan3(uint16_t u16VoltageVal)
{
    /*TODO: Need to upgrade to a hysteresis kind of logic
     * So that even if the values are around the border they will not keep toggling */
     uint8_t u8DigitCount = 0;
     do
     {
         u16VoltageVal/=10;
         ++u8DigitCount;
     }while(u16VoltageVal!=0);

     if(u8DigitCount > 3)
     {
         return true;
     }
     return false;
}

void MON_UI::prvPrintVoltageData(SOURCE_TYPE_t eSource , uint8_t u8AcSystemType)
{
    char arrTemp[32];

    uint8_t u8Position;
    uint8_t u8Local;

    _pGET_VAL_t ArrGetVtgVal[TYPE_LAST]=
    {
     &AC_SENSE::GENSET_GetDispVoltageVolts,
     &AC_SENSE::MAINS_GetDispVoltageVolts
    };

    _pGET_FLOAT_VAL_COMMON_t ArrGetPhToPhVal[TYPE_LAST][PH_PH_VTG_LAST]=
    {
     {
      &AC_SENSE::GENSET_GetDispRYVolts,
      &AC_SENSE::GENSET_GetDispYBVolts,
      &AC_SENSE::GENSET_GetDispRBVolts
     },
     {
      &AC_SENSE::MAINS_GetDispRYVolts,
      &AC_SENSE::MAINS_GetDispYBVolts,
      &AC_SENSE::MAINS_GetDispRBVolts
     }
    };


    //Mains screen Title (Healthy,Unhealthy,..)
    if(eSource == MAINS)
    {
        _Disp.gotoxy(GLCD_X(75),GLCD_Y(5));
        {
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_DISABLE)
            {
                _Disp.printStringCenterAligned((char *)StrMainsStatus[_u8LanguageIndex][STR_MAINS_NOT_READ],FONT_ARIAL);
            }
            else if(_manualMode.GetMainsStatus() == BASE_MODES::MAINS_HELATHY)
            {
                _Disp.printStringCenterAligned((char *)StrMainsStatus[_u8LanguageIndex][STR_MAINS_HEALTHY],FONT_ARIAL);
            }
            else
            {
                _Disp.printStringCenterAligned((char *)StrMainsStatus[_u8LanguageIndex][STR_MAINS_FAILED],FONT_ARIAL);
            }
        }
    }

    if((u8AcSystemType == CFGZ::CFGZ_3_PHASE_SYSTEM) || ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH) == CFGZ::CFGZ_ENABLE) && eSource == MAINS))
    {
        _Disp.drawVerticalLine(GLCD_X(65), GLCD_Y(19), GLCD_Y(64));
        u8Position = 22;
        for(u8Local = R_PHASE; u8Local < PHASE_END ; u8Local++)
        {
            _Disp.gotoxy(GLCD_X(2),GLCD_Y(u8Position));
            _Disp.printStringLeftAligned((char *)strPhase[u8Local],FONT_VERDANA);

            if(!prvIsValDigitsGreaterThan3((uint16_t)
                                           ((&_hal.AcSensors)->*ArrGetVtgVal[eSource])((PHASE_t)u8Local)))
            {
                 sprintf(arrTemp,"%d",(uint16_t)((&_hal.AcSensors)->*ArrGetVtgVal[eSource])((PHASE_t)u8Local));
//                sprintf(arrTemp,"%.2f",(float)((&_hal.AcSensors)->*ArrGetVtgVal[eSource])((PHASE_t)u8Local));
                _Disp.gotoxy(GLCD_X(50),GLCD_Y(u8Position));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
                _Disp.printStringLeftAligned((char *)" V",FONT_VERDANA);
            }
            else
            {
                sprintf(arrTemp,"%.1f",(float)(((&_hal.AcSensors)->*ArrGetVtgVal[eSource])((PHASE_t)u8Local))/1000.0f);
                _Disp.gotoxy(GLCD_X(50),GLCD_Y(u8Position));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
                _Disp.printStringLeftAligned((char *)"kV",FONT_VERDANA);
            }

            u8Position = u8Position + 15;
        }

        uint8_t ePhPh;
        u8Position = 22;
        for(ePhPh = R_TO_Y; ePhPh < PH_PH_VTG_LAST ; ePhPh++)
        {
            _Disp.gotoxy(GLCD_X(68),GLCD_Y(u8Position));
            _Disp.printStringLeftAligned((char *)strPh_Ph[ePhPh],FONT_VERDANA);
            if(!prvIsValDigitsGreaterThan3((uint16_t)
                                           ((&_hal.AcSensors)->*ArrGetPhToPhVal[eSource][ePhPh])()))
            {
                sprintf(arrTemp,"%d",(uint16_t)((&_hal.AcSensors)->*ArrGetPhToPhVal[eSource][ePhPh])());
                _Disp.gotoxy(GLCD_X(116),GLCD_Y(u8Position));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
                _Disp.printStringLeftAligned((char *)" V",FONT_VERDANA);
                u8Position = u8Position + 15;
            }
            else
            {
                sprintf(arrTemp,"%.1f",(float)(((&_hal.AcSensors)->*ArrGetPhToPhVal[eSource][ePhPh])()/1000.0f));
                _Disp.gotoxy(GLCD_X(115),GLCD_Y(u8Position));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(116),GLCD_Y(u8Position));
                _Disp.printStringLeftAligned((char *)"kV",FONT_VERDANA);
                u8Position = u8Position + 15;
            }

        }
    }
    else
    {
        /* Below logo will be displayed for Gen and Mains as well */

        _Disp.printImage((uint8_t *)gau8GeneratorVoltLogo, 4, 32, 30, 7);

        _Disp.gotoxy(GLCD_X(50),GLCD_Y(37));
        if(eSource == MAINS)
        {
            _Disp.printStringLeftAligned((char *)strPhase[R_PHASE],FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(90),GLCD_Y(37));
        }
        else
        {
            _Disp.printStringLeftAligned((char *)"Ph-N",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(104),GLCD_Y(37));
        }
        

        
        if(!prvIsValDigitsGreaterThan3((uint16_t)((&_hal.AcSensors)->*ArrGetVtgVal[eSource])(R_PHASE)))
        {
            sprintf(arrTemp,"%d",(uint16_t)((&_hal.AcSensors)->*ArrGetVtgVal[eSource])(R_PHASE));
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
            _Disp.printChar('V',FONT_VERDANA);
        }
        else
        {
            sprintf(arrTemp,"%.1f",(float)(((&_hal.AcSensors)->*ArrGetVtgVal[eSource])(R_PHASE)/1000.0f));
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
            _Disp.printStringLeftAligned((char *)"kV",FONT_VERDANA);
        }
    }
}

void MON_UI::prvPrintPower(POWER_TYPE_t eType, uint8_t u8AcSystemType, SOURCE_TYPE_t eSourceType )
{
    char arrTemp[32];  //Local variable for storing the sprintf output
    uint8_t u8Position;
    uint8_t u8Local;
    uint8_t u8XPosition = 53, u8XPositionVal=100;
    _pGET_VAL_t ArrGetVal[2][POWER_LAST]=
    {
     {
      &AC_SENSE::GENSET_GetDispActivePowerWatts,
      &AC_SENSE::GENSET_GetDispApparentPowerVA,
      &AC_SENSE::GENSET_GetDispReactivePowerVAR,
     },
     {
      &AC_SENSE::MAINS_GetDispActivePowerWatts,
      &AC_SENSE::MAINS_GetDispApparentPowerVA,
      &AC_SENSE::MAINS_GetDispReactivePowerVAR,
     }
    };

    _pGET_FLOAT_VAL_COMMON_t ArrGetTotalPower[2][POWER_LAST]=
    {
     {
      &AC_SENSE::GENSET_GetTotalFiltActivePowerWatts,
      &AC_SENSE::GENSET_GetTotalFiltApparentPowerVA,
      &AC_SENSE::GENSET_GetTotalFiltReactivePowerVAR
     },
     {
      &AC_SENSE::MAINS_GetTotalFiltActivePowerWatts,
      &AC_SENSE::MAINS_GetTotalFiltApparentPowerVA,
      &AC_SENSE::MAINS_GetTotalFiltReactivePowerVAR
     }
    };

    if((u8AcSystemType == CFGZ::CFGZ_3_PHASE_SYSTEM) || ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH) == CFGZ::CFGZ_ENABLE) && eSourceType == MAINS))
    {
        sprintf(arrTemp,"  %0.1f",((&_hal.AcSensors)->*ArrGetTotalPower[eSourceType][eType])()/1000);

        _Disp.gotoxy(GLCD_X(7),GLCD_Y(33));
        _Disp.printStringLeftAligned((char *)arrTemp,FONT_ARIAL);

        _Disp.drawVerticalLine(GLCD_X(50), GLCD_Y(19), GLCD_Y(64));
        _Disp.gotoxy(GLCD_X(23),GLCD_Y(50));
        _Disp.printStringCenterAligned((char *)strPower[eType],FONT_ARIAL);

        u8Position = 22;
        for(u8Local = R_PHASE; u8Local < PHASE_END ; u8Local++)
        {
            _Disp.gotoxy(GLCD_X(u8XPosition),GLCD_Y(u8Position));
            _Disp.printStringLeftAligned((char *)strPhase[u8Local],FONT_VERDANA);

            sprintf(arrTemp,"  %0.1f",((&_hal.AcSensors)->*ArrGetVal[eSourceType][eType])((PHASE_t)u8Local)/1000);
            _Disp.gotoxy(GLCD_X(u8XPositionVal),GLCD_Y(u8Position));
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(126),GLCD_Y(u8Position));
            _Disp.printStringRightAligned((char *)strPower[eType],FONT_VERDANA);

            u8Position = u8Position +15;
        }
    }
    else
    {

        /* Below logo will be displayed for Gen and Mains as well */
        _Disp.printImage((uint8_t *)gau8GeneratorVoltLogo, 4, 32, 30, 7);

        _Disp.gotoxy(GLCD_X(50),GLCD_Y(37));
        if(eSourceType == MAINS)
        {
            _Disp.printStringLeftAligned((char *)strPhase[R_PHASE],FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(90),GLCD_Y(37));
        }
        else
        {
            _Disp.printStringLeftAligned((char *)"Ph-N",FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(104),GLCD_Y(37));
        }

        sprintf(arrTemp,"%0.1f",((&_hal.AcSensors)->*ArrGetVal[eSourceType][eType])(R_PHASE)/1000);
        
        _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
        _Disp.printStringLeftAligned((char *)strPower[eType],FONT_VERDANA);
    }
}

void MON_UI::prvPrintSensorFaultStatus(A_SENSE::SENSOR_RET_t stTemp , uint16_t Xpos , uint16_t Ypos)
{
    if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT)
    {
        {
          _Disp.gotoxy(GLCD_X(Xpos),GLCD_Y(Ypos));
          _Disp.printStringCenterAligned((char*)StrOpnGndckt[_u8LanguageIndex],FONT_ARIAL);
        }
    }
    else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_SHORT_TO_BAT)
    {

        {
            _Disp.gotoxy(GLCD_X(Xpos),GLCD_Y(Ypos));
            _Disp.printStringCenterAligned((char*)StrSTB[_u8LanguageIndex],FONT_ARIAL);
        }
    }
}
void MON_UI::prvPrintSensorStatus(A_SENSE::SENSOR_RET_t stTemp,char *str,
                              DATA_TYPE_t etype)
{
    char arrTemp[32];  //Local variable for storing the sprintf output

    if(stTemp.eStatus == A_SENSE::SENSOR_NOT_CONFIGRUED)
    {
       _Disp.gotoxy(GLCD_X(70),GLCD_Y(37));
       _Disp.printStringLeftAligned((char*)StrNotConfigured,FONT_VERDANA);
    }
    else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
    {
        if(etype == FLOAT_TYPE)
        {
            sprintf(arrTemp,"%0.1f",stTemp.stValAndStatus.f32InstSensorVal);
        }
        else
        {
            if(strcmp(str, "`C")==0)
            {
                sprintf(arrTemp,"%d",
                         (int16_t)round(stTemp.stValAndStatus.f32InstSensorVal));
            }
            else
            {
                sprintf(arrTemp,"%d",
                        (uint16_t)round(stTemp.stValAndStatus.f32InstSensorVal));
            }

        }
        _Disp.gotoxy(GLCD_X(94),GLCD_Y(25));
        _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
        _Disp.gotoxy(GLCD_X(96),GLCD_Y(25));
        _Disp.printStringLeftAligned((char*)str,FONT_VERDANA);
    }
    else
    {
        if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT)
        {
            {
              _Disp.gotoxy(GLCD_X(83),GLCD_Y(37));
              _Disp.printStringCenterAligned((char*)StrOpenckt[_u8LanguageIndex],FONT_ARIAL);
            }
        }
        else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_SHORT_TO_BAT)
        {
            {
                _Disp.gotoxy(GLCD_X(83),GLCD_Y(37));
                _Disp.printStringCenterAligned((char*)StrSTB[_u8LanguageIndex],FONT_ARIAL);
            }

        }
    }


}

void MON_UI::prvPrintSensorStatus(A_SENSE::SENSOR_RET_t stTemp,char *str,
                              DATA_TYPE_t etype , uint8_t u8x , uint8_t u8y )
{
    char arrTemp[32];  //Local variable for storing the sprintf output

    if(stTemp.eStatus == A_SENSE::SENSOR_NOT_CONFIGRUED)
    {
       _Disp.gotoxy(GLCD_X(70),GLCD_Y(37));
       _Disp.printStringLeftAligned((char*)StrNotConfigured,FONT_VERDANA);
    }
    else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
    {
        if(etype == FLOAT_TYPE)
        {
            sprintf(arrTemp,"%0.1f",stTemp.stValAndStatus.f32InstSensorVal);
        }
        else
        {
            if(strcmp(str, "`C")==0)
            {
                sprintf(arrTemp,"%d",
                         (int16_t)round(stTemp.stValAndStatus.f32InstSensorVal));
            }
            else
            {
                sprintf(arrTemp,"%d",
                        (uint16_t)round(stTemp.stValAndStatus.f32InstSensorVal));
            }

        }
        _Disp.gotoxy(GLCD_X(u8x),GLCD_Y(u8y));
        _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
        _Disp.gotoxy(GLCD_X(u8x+2),GLCD_Y(u8y));
        _Disp.printStringLeftAligned((char*)str,FONT_VERDANA);
    }
    else
    {
        if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT)
        {
            {
              _Disp.gotoxy(GLCD_X(83),GLCD_Y(37));
              _Disp.printStringCenterAligned((char*)StrOpenckt[_u8LanguageIndex],FONT_ARIAL);
            }
        }
        else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_SHORT_TO_BAT)
        {
            {
                _Disp.gotoxy(GLCD_X(83),GLCD_Y(37));
                _Disp.printStringCenterAligned((char*)StrSTB[_u8LanguageIndex],FONT_ARIAL);
            }
        }
    }
}

void MON_UI::prvStopKeyPressAction()
{

    if((eDisplayMode == DISP_MON_MODE)
            || (eDisplayMode == DISP_ALARM_MODE))
    {
        _bMBModeChnageCMDRcvd = false;
        _startStop.ClearSimAutoPulse();
        _startStop.ClearSimStopPulse();
        _startStop.ClearSimStartPulse();
        if(_eOpMode == BASE_MODES::MANUAL_MODE)
        {
            _manualMode.Stop();
        }
        else
        {
            /* do nothing */
        }

        if(_eOpMode == BASE_MODES::AUTO_MODE)
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);
            _manualMode.DisableEngCoolTimer();
            _manualMode.DisableReturnToMains();
            if(   (_manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_OFF_MAINS_OFF)
               || (_manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_OFF_MAINS_ON)  )
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
            }
            else if((_manualMode.GetAutoModeState() != BASE_MODES::STATE_AMF_GEN_ON_LOAD)
                    &&(_manualMode.GetAutoModeState() != BASE_MODES::STATE_AMF_RETURN_DELAY))
            {
                _manualMode.SwitchToManualMode();
                _manualMode.Stop();
            }
            else
            {
                _manualMode.Stop();
            }
        }
        else if(_eOpMode == BASE_MODES::BTS_MODE)
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);
            _manualMode.DisableEngCoolTimer();
            _manualMode.DisableReturnToMains();
            _BTSMode.ClearBTSVars();
            if(   (_manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_GEN_OFF_MAINS_OFF)
                ||(_manualMode.GetBTSModeState() == BASE_MODES:: STATE_BTS_GEN_OFF_MAINS_ON) )
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
                _manualMode.OpenGenLoad();
                _manualMode.OpenMainsLoad();
            }
            else if((_manualMode.GetBTSModeState()!= BASE_MODES::STATE_BTS_GEN_ON_LOAD)
                    &&(_manualMode.GetBTSModeState()!= BASE_MODES::STATE_BTS_RETURN_DELAY))
            {
                _manualMode.SwitchToManualMode();
//                _startStop.StopCommand();
            }
            else
            {
//               _startStop.StopCommand();//On a s
            }
        }
        else if(_eOpMode == BASE_MODES::CYCLIC_MODE)
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);
            _manualMode.DisableEngCoolTimer();
            _manualMode.DisableReturnToMains();
            _cyclicMode.ClearCyclicModeVars();

            if(  (_manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_GEN_OFF_MAINS_OFF)
               ||(_manualMode.GetCyclicModeState() == BASE_MODES:: STATE_CYCLIC_GEN_OFF_MAINS_ON) )
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
                _manualMode.OpenGenLoad();
                _manualMode.OpenMainsLoad();
                _manualMode.SetTimerState(BASE_MODES::NO_TIMER_RUNNING);
            }
            else if(_manualMode.GetCyclicModeState() != BASE_MODES::STATE_CYCLIC_GEN_ON_LOAD)
            {
                _manualMode.SwitchToManualMode();
//                _startStop.StopCommand();
            }
            else
            {
//                _startStop.StopCommand();
            }
        }
        else if(_eOpMode == BASE_MODES::TEST_MODE)
        {
            _manualMode.ComeOutOfTestMode();
            _manualMode.Stop();
            _manualMode.DisableTestModeTimer();
            _eOpMode = BASE_MODES::MANUAL_MODE;
        }
        _hal.ledManager.led2.TurnOff();
        _hal.ledManager.led3.TurnOff();
        _hal.ledManager.led4.TurnOn();
        _manualMode.SetGCUOperatingMode(_eOpMode);
    }
    else
    {
        /* shubham wader 23.09.2022
        todo: do nothing.
        There are 2 possibilities for eDisplayMode, 1. Normal Monitoring mode 2. Alarm mode.
        We are accepting stop key command and executing mode specific actions only if the eDisplayMode
        is in Normal monitoring mode. Ref from MAIN_UI: if the program is in Alarm mode, on pressing stop key
        it will change the mode to normal monitoring mode. hence, whenever execution comes to this specific
        function call, we are getting eDisplayMode as Normal monitoring mode.
        So, need to think, is it necessary to check this here ? */
    }
}

void MON_UI::prvStartKeyPressAction()
{
    _bMBModeChnageCMDRcvd = false;
    _startStop.ClearSimStartPulse();
    _startStop.ClearSimAutoPulse();

    if(((_eOpMode == BASE_MODES::MANUAL_MODE)
          ||(_eOpMode == BASE_MODES::TEST_MODE))
          && (_startStop.GetStartStopSMDState() != START_STOP::ID_STATE_SS_STOPPING)
          && (_manualMode.GetManualModeState() == BASE_MODES::STATE_MANUAL_GEN_OFF)
          && (!_manualMode.IsNightModeRestrictOn())
          )
    {
        _manualMode.Start();
    }
}

void MON_UI::prvAutoKeyPressAction()
{
    /* Shubham.Wader 23.09.2022
        Mode switching logic rearranged referencing GC2111 NXP
    */
    _bMBModeChnageCMDRcvd = false;
    _startStop.ClearSimAutoPulse();

    _GCUAlarms.ClearAllAlarms(); //As per NXP.

    if(_eOpMode == BASE_MODES::MANUAL_MODE)
    {
        if( (!_GCUAlarms.IsCommonShutdown())
                && (!_GCUAlarms.IsCommonElectricTrip())
                && (!_GCUAlarms.IsCommonWarning()))
        {
            _manualMode.AssignModechangeParameters();
            if(_manualMode.GetGCUOperatingMode()==BASE_MODES::BTS_MODE)
            {
                _eOpMode = BASE_MODES::BTS_MODE;

                if(_BTSMode.GetBTSModeState() == BASE_MODES::STATE_BTS_GEN_ON_LOAD)
                {
                    _BTSMode.ManualToBTSOnLoad();
                }
            }
            else if(_manualMode.GetGCUOperatingMode()==BASE_MODES::CYCLIC_MODE)
            {
                _eOpMode = BASE_MODES::CYCLIC_MODE;
                _cyclicMode.SwitchFromManualToCyclic();

            }
            else if(_manualMode.GetGCUOperatingMode()==BASE_MODES::AUTO_MODE)
            {
                _eOpMode = BASE_MODES::AUTO_MODE;
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            /* ignore command if shutdown/trip present */
        }
    }
    else if(_eOpMode == BASE_MODES::AUTO_MODE)
    {
        _eOpMode = BASE_MODES::MANUAL_MODE;

        if(   (_manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_OFF_MAINS_ON)
           || (_manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_OFF_MAINS_OFF) )
        {
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
            _manualMode.DisableReturnToMains();
            _manualMode.OpenGenLoad();
            _manualMode.OpenMainsLoad();
        }
        else if( _manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_ON_LOAD ||
                 _manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_START   ||
                 _manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_RETURN_DELAY  )
        {
            if(_manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_START)
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_START);
            }
            else
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);

                if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_AUTO_LOAD_TRANSFER) == CFGZ::CFGZ_ENABLE)
                {
                    _manualMode.SwitchLoadToGen();
                }
                else
                {
                    _manualMode.OpenGenLoad();
                    _manualMode.OpenMainsLoad();
                }
            }
            _manualMode.DisableReturnToMains();
        }
        else if( _manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_ENGINE_COOLING)
        {
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_ENGINE_COOLING);
        }
        else if (_manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_ENGINE_STOP)
        {
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_ENGINE_STOP);
        }
        else
        {
            /* do nothing */
        }
    }
    else if(_eOpMode == BASE_MODES::BTS_MODE)
    {
//        _BTSMode.ClearBTSVars();
        _eOpMode = BASE_MODES::MANUAL_MODE;
        if(  (_manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_GEN_OFF_MAINS_OFF)
           ||(_manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_GEN_OFF_MAINS_ON))
        {
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
            _manualMode.DisableReturnToMains();
        }
        else if( _manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_GEN_START     ||
                 _manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_GEN_ON_LOAD   ||
                 _manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_RETURN_DELAY  )
        {
            if(_manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_GEN_START)
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_START);
            }
            else
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);

                if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_AUTO_LOAD_TRANSFER) == CFGZ::CFGZ_ENABLE)
                {
                    _manualMode.SwitchLoadToGen();
                }
                else
                {
                    _manualMode.OpenGenLoad();
                    _manualMode.OpenMainsLoad();
                }
            }
            _manualMode.DisableReturnToMains();
        }
        else if( _manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_ENGINE_COOLING)
        {
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_ENGINE_COOLING);
        }
        else if (_manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_ENGINE_STOP)
        {
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_ENGINE_STOP);
        }
        else
        {
            /* do nothing */
        }
    }
    else if(_eOpMode == BASE_MODES::CYCLIC_MODE)
    {
        _cyclicMode.ClearCyclicModeVars();
        _eOpMode = BASE_MODES::MANUAL_MODE;
        if(  (_manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_GEN_OFF_MAINS_OFF)
           ||(_manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_GEN_OFF_MAINS_ON))
        {

            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
            _manualMode.DisableReturnToMains();
            _manualMode.OpenGenLoad();
            _manualMode.OpenMainsLoad();
        }
        else if( _manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_GEN_START     ||
                 _manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_GEN_ON_LOAD   ||
                 _manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_RETURN_DELAY  )
        {
            if(_manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_GEN_START)
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_START);
            }
            else
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);

                if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_AUTO_LOAD_TRANSFER) == CFGZ::CFGZ_ENABLE)
                {
                    _manualMode.SwitchLoadToGen();
                }
                else
                {
                    _manualMode.OpenGenLoad();
                    _manualMode.OpenMainsLoad();
                }
            }
            _manualMode.DisableReturnToMains();
        }
        else if( _manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_ENGINE_COOLING)
        {
            _cyclicMode.ClearStartOffTimerFlag();
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_ENGINE_COOLING);
        }
        else if (_manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_ENGINE_STOP)
        {
            _cyclicMode.ClearStartOffTimerFlag();
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_ENGINE_STOP);
        }
        else
        {
            /* do nothing */
        }
    }
    else
    {
        /* no more modes to switch */
    }

    _manualMode.SetGCUOperatingMode(_eOpMode);
}
void MON_UI::prvHandleAutomaticModeSwitching()
{
    static uint32_t prevAutomaticModeSwitchTime = 0;

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUTO_MD_SWITCH_AUTO_MD_SWITCH) == CFGZ::CFGZ_ENABLE)
    {
        if(_eOpMode != BASE_MODES::MANUAL_MODE)
        {
            UTILS_DisableTimer(&AutomaticModeSwitchingTimer);
            GCU_ALARMS::_bModeSwitchAlarm = false;
            GCU_ALARMS::_bAutomaticModeSwitchStatus = false;
        }
        else if((_eOpMode == BASE_MODES::MANUAL_MODE) && !UTILS_IsTimerEnabled(&AutomaticModeSwitchingTimer))
        {
            UTILS_ResetTimer(&AutomaticModeSwitchingTimer);
        }
        else if(UTILS_GetElapsedTimeInSec(&AutomaticModeSwitchingTimer) >= _cfgz.GetCFGZ_Param(CFGZ::ID_AUTO_MD_SWITCH_MANUAL_MODE_TIME)*60)
        {
            prvAutoKeyPressAction();
            GCU_ALARMS::_bModeSwitchAlarm = false;
        }

        uint16_t ModeSwitchDispTime = _cfgz.GetCFGZ_Param(CFGZ::ID_AUTO_MD_SWITCH_MANUAL_MODE_TIME) - _cfgz.GetCFGZ_Param(CFGZ::ID_AUTO_MD_SWITCH_OUTPUT_ON_TIME);
        if(UTILS_GetElapsedTimeInSec(&AutomaticModeSwitchingTimer) >= ModeSwitchDispTime*60 && prevAutomaticModeSwitchTime < ModeSwitchDispTime*60)
        {
            GCU_ALARMS::_bModeSwitchAlarm = true;
        }
        if(UTILS_GetElapsedTimeInSec(&AutomaticModeSwitchingTimer) >= ModeSwitchDispTime*60)
        {
            GCU_ALARMS:: _bAutomaticModeSwitchStatus = true;
        }
        else
        {
            GCU_ALARMS:: _bAutomaticModeSwitchStatus = false;
        }
    }
    else
    {
        UTILS_DisableTimer(&AutomaticModeSwitchingTimer);
        GCU_ALARMS::_bModeSwitchAlarm = false;
        GCU_ALARMS::_bAutomaticModeSwitchStatus = false;
    }
    prevAutomaticModeSwitchTime = UTILS_GetElapsedTimeInSec(&AutomaticModeSwitchingTimer);
}

void MON_UI::MBChangeModeRequest()
{
    _bMBModeChnageCMDRcvd = true;
}

void MON_UI::GroupSwitching(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent)
{
    if(UTILS_GetElapsedTimeInMs(&_GroupSwitchingTimer) > GROUP_SWITCH_TIMER_MS)
    {
        UTILS_ResetTimer(&_GroupSwitchingTimer);
        switch(_sKeyEvent)
        {
            case DN_LONG_PRESS:
            {
                uint8_t u8GroupNumber = u8ArrGroupArray[_stScreenNo];
                u8GroupNumber++;
                if(u8GroupNumber > DISP_GROUP_ENGINE)
                {
                    u8GroupNumber = DISP_GROUP_STATUS;
                }

                for(uint8_t i=_u8ScreenMin; i<_u8ScreenMax ; i++ )
                {
                    if(_ArrScreenEnDs[i])
                    {
                        if((u8ArrGroupArray[i] == u8GroupNumber))
                        {
                            _stScreenNo = i;
                            break;
                        }
                    }
                    else
                    {
                        u8GroupNumber++;
                    }
                }
                break;
            }

            case UP_LONG_PRESS:
            {
               uint8_t u8GroupNumber = u8ArrGroupArray[_stScreenNo];
               if(u8GroupNumber != 0U)
               {
                   u8GroupNumber --;
               }
               else
               {
                   u8GroupNumber = DISP_GROUP_ENGINE;
               }

               for(uint8_t i=_u8ScreenMin; i<_u8ScreenMax ; i++ )
               {
                   if(u8ArrGroupArray[i] == u8GroupNumber)
                   {
                       if(_ArrScreenEnDs[i])
                       {
                           _stScreenNo = i;
                           break;
                       }
                       else
                       {
                           _stScreenNo = i - 1;
                           break;
                       }
                   }
               }
                break;
            }

            default: break;
        }
    }
}

void MON_UI:: prvDisplayError()
{
    _Disp.ClearScreen();
    _Disp.drawRectangle();

    if(_cfgz.IsC03Error() && _cfgc.IsC02Error())
    {
        _Disp.gotoxy(GLCD_X(64),GLCD_Y(25));
        _Disp.printStringCenterAligned((char*)"Error: C02", FONT_ARIAL);
        _Disp.gotoxy(GLCD_X(64),GLCD_Y(35) );
        _Disp.printStringCenterAligned((char*)"Error: C03", FONT_ARIAL);
    }
    else if(_cfgz.IsC03Error())
    {
        _Disp.gotoxy(GLCD_X(64),GLCD_Y(32));
        _Disp.printStringCenterAligned((char*)"Error: C03", FONT_ARIAL);
    }
    else if(_cfgc.IsC02Error())
   {
       _Disp.gotoxy(GLCD_X(64),GLCD_Y(32));
       _Disp.printStringCenterAligned((char*)"Error: C02", FONT_ARIAL);
   }
}

void MON_UI::prvBuildExhaustIconScreen(void)
{
    static stTimer tmr;
    static bool _blink;
    if(!tmr.bEnabled)
    {
        UTILS_ResetTimer(&tmr);
    }

    if(UTILS_GetElapsedTimeInSec(&tmr) >= 1)
    {
        UTILS_ResetTimer(&tmr);
        if (_blink)
        {
            _blink= false;
        }
        else
        {
            _blink= true;
        }
    }

    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.gotoxy(GLCD_X(64),GLCD_Y(1));
    _Disp.printStringCenterAligned((char *)"EXHAUST AFT TREAT",FONT_ARIAL);
    _Disp.drawHorizontalLine(0, 11, 127);

    /* If communication failure alarm occurs then display "Not available" on screen
     * 1. ArrPgnReadData[RX_PGN_DPFC1_64892][0] = SPN 3698
     *     display the icon only when on solid = 1 received else don not display the icon.
     * 2. ArrPgnReadData[RX_PGN_DPFC1_64892][1] = SPN 3697
     *      000     OFF
     *      001     Solid On
     *      100     Blink with 1Hz
     * 3. ArrPgnReadData[RX_PGN_DPFC1_64892][2] = SPN 3703
     *     display the icon only when on solid = 1 received else don not display the icon
     */
    if( _j1939.IsCommunicationFail() )
    {
        _Disp.gotoxy(32,64);
        _Disp.printStringCenterAligned((char*)StrCANErr,FONT_ARIAL);
    }
    else
    {
        if((uint8_t)_j1939.GetReadData(RX_PGN_DPFC1_64892,0) == 1)
        {
            _Disp.printImage((uint8_t *)&gau8Icon0, 4, 24, 13, 6);
        }

        if((uint8_t)_j1939.GetReadData(RX_PGN_DPFC1_64892,1) == 1)
        {
            _Disp.printImage((uint8_t *)&gau8Icon1, 5, 22, 13, 45);
        }
        else if((uint8_t)_j1939.GetReadData(RX_PGN_DPFC1_64892,1)== 4)
        {
            if(_blink)
            {
                _Disp.printImage((uint8_t *)&gau8Icon1, 5, 22, 13, 47);
            }
            else
            {
                // do nothing.
            }
        }
        if((uint8_t)_j1939.GetReadData(RX_PGN_DPFC1_64892,2) == 1)
        {
            _Disp.printImage((uint8_t *)&gau8Icon2, 5, 24, 13, 87);
        }

        /* AT1T1I */
        /* Displayed the  image for SPN 5245 of PGN AT1T1I depending on the received value.
             *          000             OFF
             *          001             ON-Solid
             *          100             ON-Fast blink (1 HZ)*/
        if((uint8_t)_j1939.GetReadData(RX_PGN_AT1T1I_65110,2)  == 1)
        {
            _Disp.printImage((uint8_t *)&gau8Icon3, 4, 27, 35, 30);
        }
        else if((uint8_t)_j1939.GetReadData(RX_PGN_AT1T1I_65110,2) == 4)
        {
            if(_blink)
            {
                _Disp.printImage((uint8_t *)&gau8Icon3, 4, 27, 35, 30);
            }
            else
            {
                // do nothing.
            }
        }
        /* Displayed the icon for SPN 5246 when malfunction lamp is active or the value received from
         * SPN 5246 is more than or equal to 3.
         * 30/4/20 : Mal-function Lamp value compared with Lamp ON status.*/
        if((_j1939.GetLampStatus(J1939APP::MIL_LAMP)== 1) || ((2 < (uint8_t)_j1939.GetReadData(RX_PGN_AT1T1I_65110,3)) && ((uint8_t)_j1939.GetReadData(RX_PGN_AT1T1I_65110,3) < 7)))
        {
            _Disp.printImage((uint8_t *)&gau8Icon4, 4, 20, 40, 74);
        }
    }
}

void MON_UI::prvBuildLampIconScreen(void)
{

    static bool _bBlinkAt2Hz = false;
    static bool _bBlinkAt1Hz = false;
    static stTimer LampFlashTimer_1_Hz;
    static stTimer LampFlashTimer_2_Hz;

    if (!LampFlashTimer_1_Hz.bEnabled)
    {
        UTILS_ResetTimer(&LampFlashTimer_1_Hz);
    }
    if(!LampFlashTimer_2_Hz.bEnabled)
    {
        UTILS_ResetTimer(&LampFlashTimer_2_Hz);
    }

    if (UTILS_GetElapsedTimeInMs(&LampFlashTimer_1_Hz) >= 1000)
    {
        UTILS_ResetTimer(&LampFlashTimer_1_Hz);
        if (_bBlinkAt1Hz)
        {
            _bBlinkAt1Hz = false;
        }
        else
        {
            _bBlinkAt1Hz = true;
        }
    }

    if (UTILS_GetElapsedTimeInMs(&LampFlashTimer_2_Hz) >= 500)
    {
        UTILS_ResetTimer(&LampFlashTimer_2_Hz);
        if (_bBlinkAt2Hz)
        {
            _bBlinkAt2Hz = false;
        }
        else
        {
            _bBlinkAt2Hz = true;
        }
    }

    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.gotoxy(GLCD_X(64),GLCD_Y(5));
    _Disp.printStringCenterAligned((char *)"LAMP ICONS",FONT_ARIAL);
    _Disp.drawHorizontalLine(0, 19, 127);

    /* to solve SGC-225 */
    if( _j1939.IsCommunicationFail() || (!_j1939.IsDM1PGNRecieved()) )
    {
        _Disp.gotoxy(32,64);
        _Disp.printStringCenterAligned((char *)StrCANErr,FONT_ARIAL);
    }
    else
    {

        if(_j1939.GetLampStatus(J1939APP::RED_LAMP) && (_j1939.GetLampFlashingSequence(J1939APP::RED_LAMP) == J1939APP::FLASH_ALWAYS))
        {
            _Disp.printImage((uint8_t *)&u8RLSImage, 6, 28, 25, 1);
        }
        else if(_j1939.GetLampStatus(J1939APP::RED_LAMP) && (_j1939.GetLampFlashingSequence(J1939APP::RED_LAMP) == J1939APP::FLASH_AT_1_HZ))
        {
            if(_bBlinkAt1Hz)
            {
                _Disp.printImage((uint8_t *)&u8RLSImage, 6, 28, 25, 1);
            }
        }
        else if(_j1939.GetLampStatus(J1939APP::RED_LAMP) && (_j1939.GetLampFlashingSequence(J1939APP::RED_LAMP) == J1939APP::FLASH_AT_2_HZ))

        {
            if(_bBlinkAt2Hz)
            {
                _Disp.printImage((uint8_t *)&u8RLSImage, 6, 28, 25, 1);
            }
        }
        else
        {
            /* no instrction to display lamp */
        }


        if(_j1939.GetLampStatus(J1939APP::PROTECT_LAMP) && (_j1939.GetLampFlashingSequence(J1939APP::PROTECT_LAMP) == J1939APP::FLASH_ALWAYS))
        {
            _Disp.printImage((uint8_t *)&gau8Icon7, 6, 27, 25, 43);
        }
        else if(_j1939.GetLampStatus(J1939APP::PROTECT_LAMP) && (_j1939.GetLampFlashingSequence(J1939APP::PROTECT_LAMP) == J1939APP::FLASH_AT_1_HZ))
        {
            if(_bBlinkAt1Hz)
            {
                _Disp.printImage((uint8_t *)&gau8Icon7, 6, 27, 25, 43);
            }
        }
        else if(_j1939.GetLampStatus(J1939APP::PROTECT_LAMP) && (_j1939.GetLampFlashingSequence(J1939APP::PROTECT_LAMP) == J1939APP::FLASH_AT_2_HZ))

        {
            if(_bBlinkAt2Hz)
            {
                _Disp.printImage((uint8_t *)&gau8Icon7, 6, 27, 25, 43);
            }
        }
        else
        {
            /* no instrction to display lamp */
        }


        if(_j1939.GetLampStatus(J1939APP::MIL_LAMP) && (_j1939.GetLampFlashingSequence(J1939APP::MIL_LAMP) == J1939APP::FLASH_ALWAYS))
        {
            _Disp.printImage((uint8_t *)&gau8Icon8, 6, 27, 25,85);
        }
        else if(_j1939.GetLampStatus(J1939APP::MIL_LAMP) && (_j1939.GetLampFlashingSequence(J1939APP::MIL_LAMP) == J1939APP::FLASH_AT_1_HZ))
        {
            if(_bBlinkAt1Hz)
            {
                _Disp.printImage((uint8_t *)&gau8Icon8, 6, 27, 25,85);
            }
        }
        else if(_j1939.GetLampStatus(J1939APP::MIL_LAMP) && (_j1939.GetLampFlashingSequence(J1939APP::MIL_LAMP) == J1939APP::FLASH_AT_2_HZ))

        {
            if(_bBlinkAt2Hz)
            {
                _Disp.printImage((uint8_t *)&gau8Icon8, 6, 27, 25,85);
            }
        }
        else
        {
            /* no instrction to display lamp */
        }
    }
}
