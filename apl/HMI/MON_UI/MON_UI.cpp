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
_eOpMode (BASE_MODES::MANUAL_MODE),
ExternalInputUpdateTimer{0},
_ArrScreenEnDs{false,false,false,
    false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false,false,false,
    false,false,false,false,false,false,false,false,false}, /* initialize with all screen disabled */
_u8LanguageIndex(0),
_u8ScreenMin(DISP_MON_HOME),
_u8ScreenMax(DISP_MON_LAST)
{
    UTILS_ResetTimer(&ExternalInputUpdateTimer);
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

/* TODO: think the need of getting _eOpMode here in MON_UI */
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

/* Shubham.Wader 13.09.2022
   TODO: Some of the times is observed that, due to mismatch in "CFGZ.smc" and "CFGZ.h", the contrast value gets set to some
   random value. Sometimes the value was < 10. At that time, one may not able to go inside config -> change contrast and then check
   what is happening. So as a precaution measure, should we check cfgz contrast value with some hard coded threshold value here below ?
   Will that affect/render customers requirement ?

   Actually this defensive type of action should be taken inside any function.
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
            /*
             * TODO: The below condition is added in SGC for handling the screen status change based on Load.
             * Need to confirm and remove if not required here in GC2111.
             */
            if(_EngineMon.GetAndClearIsLoadStatusChanged())
            {
                prvConfigureScreenEnable();
            }

            if(bRefresh)
            {
                /* TODO: need to uncomment and modify below J1939 related changes */
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
            if(_hal.actuators.GetActStatus(ACTUATOR::ACT_AUDIBLE_ALARM) == true)
            {
                _GCUAlarms.TurnOffSounderAlarm();
            }
            else
            {
                /* do nothing */
            }

            if(_stScreenNo == _u8ScreenMin)
            {
                eDisplayMode = DISP_ALARM_MODE;
                ALARM_UI::ClearAlarmScreenNum();

                if(eMonScreenType == MON_SCREEN_NORMAL)
                {
                    MON_UI::_stScreenNo = _u8ScreenMax;
                    eDisplayMode = DISP_ALARM_MODE;
                    ALARM_UI::ClearAlarmScreenNum();
                    ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM1);
                }
                else if(eMonScreenType == MON_SCREEN_J1939)
                {
                    MON_UI::_stScreenNo = DISP_MON_HOME;
                    if( _cfgz.GetEngType()==CFGZ::ENG_KUBOTA) /* TODO: properly add the engine types */
                    {
                        ALARM_UI::ChangeAlarmScreenType(ALARM_UI::NCD); /* suggestion sgc4xx: its better if we keep some meaningful name for enum.(NCD)*/
                    }
                    else
                    {
                        ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM2);
                        _j1939.RequestDM2Messages();
                    }
                }
            }
            _stScreenNo--;
            while(_ArrScreenEnDs[_stScreenNo] == false)
            {
                _stScreenNo--;
            }
        }
        break;

        case DN_SHORT_PRESS: //Down key Press
        {
            //Note while check for _stScreenNo shall be placed immediately
            /*
             * SuryaPranayTeja.BVV
             * The condition in the while and after while are same.
             * But the condition need to present as if the while condition
             * is false ever then array index goes out of bounds
             * In case of Up key: Home Screen is always enabled.(i.e Min Screen)
             * TODO: Look for such array index out of bound.
             */
            _stScreenNo++;
            while(_ArrScreenEnDs[_stScreenNo] == false)
            {
                _stScreenNo++;
                if(_stScreenNo >= _u8ScreenMax)
                {
                    _stScreenNo = _u8ScreenMin;

                    ALARM_UI::ChangeAlarmScreenType(ALARM_UI::ALARM);
                    eDisplayMode = DISP_ALARM_MODE;
                    ALARM_UI::ClearAlarmScreenNum();

                    if(eMonScreenType == MON_SCREEN_J1939)
                    {
                        MON_UI::_stScreenNo = DISP_MON_HOME;
                        ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM2);
                        _j1939.RequestDM2Messages();
                    }

                }
            }

            if(MON_UI::_stScreenNo >= _u8ScreenMax)
            {
                MON_UI::_stScreenNo = _u8ScreenMin;
                ALARM_UI::ChangeAlarmScreenType(ALARM_UI::ALARM);
                eDisplayMode = DISP_ALARM_MODE;
                ALARM_UI::ClearAlarmScreenNum();
                if(eMonScreenType == MON_SCREEN_J1939)
                {
                    MON_UI::_stScreenNo = DISP_MON_HOME;
                    ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM2);
                    _j1939.RequestDM2Messages();
                }
            }
        }
        break;

        case ACK_KEY_PRESS:
        {
            if(_hal.actuators.GetActStatus(ACTUATOR::ACT_AUDIBLE_ALARM) == true)
            {
                _GCUAlarms.TurnOffSounderAlarm();
            }
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
            if(!_manualMode.IsGenRunTimersInProcess()) /* TODO: yet to add engine type dependency. */
            {
                if(eMonScreenType == MON_SCREEN_NORMAL)
                {
                    eDisplayMode = DISP_ALARM_MODE;
                    ALARM_UI::ClearAlarmScreenNum();
                    ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM2);
                    _j1939.RequestDM2Messages();
                }
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
    /*TODO:All the Screens of CAN are hard coded as disabled */
    static uint8_t u8Screen;
    for(u8Screen = DISP_MON_HOME ; u8Screen<DISP_MON_LAST; u8Screen++)
    {
        _ArrScreenEnDs[u8Screen] = false;
        switch(u8Screen)
        {
            case DISP_MON_HOME :
            case DISP_MON_PRODUCT_ID :
                _ArrScreenEnDs[u8Screen] = true;
                break;

            case DISP_MON_CAN_COMMUNICATION_INFO :
            case DISP_MON_ENG_LINK_INFO :
                    _ArrScreenEnDs[u8Screen] = false;
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
            case DISP_MON_MAINS_LOAD_KVAR :
                /* load on mains && mains mon on &&  mains contactor configured */
                if((_EngineMon.GetContactorLoadStatus() ==ENGINE_MONITORING::LOAD_ON_MAINS) &&
                   (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE) &&
                   (_manualMode.IsMainsContactorConfigured()))
                {
                    _ArrScreenEnDs[u8Screen] = true;
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
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_LUBE_OIL_PRESSURE :
                if(((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                        ||((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) >= CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;
            case DISP_MON_FUEL :
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
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

#if ENABLE_MON_J1939
void MON_UI::prvPrintPGN65378OrPGN65367(uint8_t u8PGNReceived)
{
    char arrTemp[32];
    if( _j1939.IsCommunicationFail() || (_j1939.GetSPNErrorStatus(u8PGNReceived, 0) == J1939APP::J1939APP::NOT_AVAILABLE) )
    {
        _Disp.gotoxy(64, 32);
        prvPrintCANMsgRcvError();
    }
    else if(_j1939.GetSPNErrorStatus(u8PGNReceived, 0) == J1939APP::ERROR)
    {
        _Disp.gotoxy(64, 32);
        _Disp.printStringCenterAligned((char*)Error, FONT_ARIAL);
    }
    else
    {
        if(u8PGNReceived == RX_PGN_PROPB62_65378)
        {
            _Disp.gotoxy(80, 37);
            sprintf(arrTemp,"%7.2f", (float)_j1939.GetReadData((DATABASE_RX_PGN_LIST_t)u8PGNReceived, 0));
            _Disp.printStringRightAligned(arrTemp, FONT_ARIAL);
        }
        else if(u8PGNReceived == RX_PGN_PROPB57_65367)
        {
            _Disp.gotoxy(80, 37);
            sprintf(arrTemp,"%.0f", (float)_j1939.GetReadData((DATABASE_RX_PGN_LIST_t)u8PGNReceived, 0));
            _Disp.printStringRightAligned(arrTemp, FONT_ARIAL);
        }
        else{/*do nothing*/}
        _Disp.gotoxy(90, 37);
        _Disp.printStringLeftAligned((char *)array_units[astJ1939Display[u8PGNReceived].stpSPN[0].u8SPNUnitIndx], FONT_VERDANA);
    }
}



void MON_UI::prvPrintSPNErrorVal(uint8_t u8RowNum, uint8_t u8ColStart)
{
    _Disp.gotoxy(u8ColStart, u8RowNum);
    _Disp.printStringLeftAligned((char*)Error, FONT_ARIAL);
}

void MON_UI::prvPrintJ1939SPNVal(SPNData_t stData, uint8_t u8x, uint8_t u8y )
{
    char arrTemp[32];
    switch(stData.u8DPInResolution)
    {
        case 0:
        default:
                sprintf(arrTemp,"%.0f", stData.f64SpnVal);
                break;
        case 1:
                sprintf(arrTemp,"%.1f", stData.f64SpnVal);
                break;
        case 2:
                sprintf(arrTemp,"%.2f", stData.f64SpnVal);
                break;
        case 3:
                sprintf(arrTemp,"%.3f", stData.f64SpnVal);
                break;
        case 4:
                sprintf(arrTemp,"%.4f", stData.f64SpnVal);
                break;
    }
    if(stData.u8DPInResolution == 0)
    {
        _Disp.gotoxy(u8x, u8y);
        _Disp.printStringRightAligned(arrTemp, FONT_VERDANA);
    }
    else
    {
        char arrTemp1[32], arrTemp2[32];
        uint8_t j=0;
        bool bCopyInSecondString = false;
        uint8_t u8Length=0;;
        for(uint8_t i=0;i<strlen(arrTemp);i++)
        {
            u8Length++;
            if(arrTemp[i]=='\0')
            {
                break;
            }
        }

        for(uint8_t i=0;i<u8Length;i++)
        {
            if(arrTemp[i]=='.')
            {
                bCopyInSecondString = true;
                arrTemp1[i] = '\0';
            }

            if(bCopyInSecondString)
            {
                arrTemp2[j]=arrTemp[i];
                j++;
            }
            else
            {
                arrTemp1[i]=arrTemp[i];
            }

        }
        arrTemp2[j] = '\0';
        _Disp.gotoxy(u8x, u8y);
        _Disp.printStringRightAligned(arrTemp1, FONT_VERDANA);
        _Disp.gotoxy(u8x+1, u8y);
        _Disp.printStringLeftAligned(arrTemp2, FONT_VERDANA);
    }

}

void  MON_UI::prvPrintJ1939AfterTreatDataOnScreen(bool bIsAfterTreat1PGN, uint8_t u8RxATPGNNo)
{

    uint8_t i = 0;
    _Disp.gotoxy(64, 12);
    _Disp.printStringCenterAligned((char *)(&(gstPgnSubHeadings[_u8LanguageIndex][u8RxATPGNNo - (DISP_AT1IG1_PGN_61454 -DISP_MON_SCREEN_LAST-1)].SubHeadings)), FONT_VERDANA);

    for(i = 0; i < (astJ1939Display[u8RxATPGNNo].u8NoOfSPNs); i++)
    {
        _Disp.gotoxy(2, 23 + (i * 10));
//        if(_u8LanguageIndex == CFGZ::LANGUAGE_SPANISH)
//        {
//            _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxATPGNNo].stpSPN[i].cs8pSpanishSPNName, FONT_VERDANA);
//        }
//        else
        {
            _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxATPGNNo].stpSPN[i].cs8pSPNName, FONT_VERDANA);
        }


        if(_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA))
        {
            /* If EEC1 PGN is not received for configurable time then display
             * "NA" - Not available in front of the SPN name.*/
            if( (_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::NOT_AVAILABLE)))
            {
                prvPrintNotAvailable((uint8_t)(23 + (i * 10)), 83);
            }
            else if( _DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::ERROR )
            {
                prvPrintSPNErrorVal((uint8_t)(23 + (i * 10)), 83);
            }
        }
        else
        {
            prvPrintJ1939SPNVal(_DisplayPGNScreenData[i], 93,(uint8_t)(23 + (i * 10)));
            _Disp.gotoxy(127, 23 + (i * 10));
            _Disp.printStringRightAligned((char *)array_units[astJ1939Display[u8RxATPGNNo].stpSPN[i].u8SPNUnitIndx],FONT_VERDANA);
        }
    }
}

void MON_UI::prvJ1939MonScreens()
{

    uint8_t i = 0;
    char arrTemp[32];
    char * ps8PGNNameToPrint;
    volatile uint8_t u8RxPGNNo = _stScreenNo - DISP_MON_SCREEN_LAST-1;
    ps8PGNNameToPrint = (char*)arrPGNAbbreviations[_u8LanguageIndex][astJ1939Display[u8RxPGNNo].cu8PGNName- DISP_MON_SCREEN_LAST-1];
   // ps8PGNNameToPrint = &arrPGNAbbreviations[astJ1939Display[u8RxPGNNo].cu8PGNName- DISP_MON_SCREEN_LAST-1][0];

    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.drawHorizontalLine(0, 11, 127);
    _Disp.gotoxy(64, 1);

    _Disp.printStringCenterAligned((char *)ps8PGNNameToPrint, FONT_VERDANA);

    switch(_stScreenNo)
    {
        case DISP_PROPB62_PGN_65378 :
        case DISP_PROPB57_PGN_65367:
            {
                prvPrintPGN65378OrPGN65367(u8RxPGNNo);
            }
            break;

        case DISP_EDC4_CAN_STATUS:
            if( _j1939.IsCommunicationFail() || (_j1939.GetSPNErrorStatus(RX_PGN_EMR_PREHEAT_65284, 1) == J1939APP::J1939APP::NOT_AVAILABLE) )
            {
                _Disp.gotoxy(64, 37);
                prvPrintCANMsgRcvError();
            }
            else if( _j1939.GetSPNErrorStatus(RX_PGN_EMR_PREHEAT_65284, 1)== J1939APP::ERROR )
            {
                _Disp.gotoxy(64, 37);
                _Disp.printStringCenterAligned((char *)Error, FONT_ARIAL);
            }
            else
            {
                _Disp.gotoxy(64, 37);
                sprintf(arrTemp,"%3.0f", (float)_j1939.GetReadData(RX_PGN_EMR_PREHEAT_65284, 1));
                _Disp.printStringCenterAligned(arrTemp, FONT_ARIAL);

            }
            break;

        case DISP_WFI_PGN_65279:
            _Disp.gotoxy(3, 15 + (i * 10));
//            if(_u8LanguageIndex == CFGZ::LANGUAGE_SPANISH)
//            {
//                _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[0].cs8pSpanishSPNName, FONT_VERDANA);
//            }
//            else
            {
                _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[0].cs8pSPNName, FONT_VERDANA);
            }
            if(_j1939.IsCommunicationFail()  || (_DisplayPGNScreenData[i].SPNStatusErrorNA))
            {
                if(( _j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::NOT_AVAILABLE)))
                {
                    prvPrintNotAvailable(15, 83);
                }
                else if(_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::ERROR)
                {
                    prvPrintSPNErrorVal(15, 83);
                }
            }
            else
            {
                _Disp.gotoxy(83, 15);
                if((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 0  )
                {
                    _Disp.printStringLeftAligned((char*)No, FONT_ARIAL);
                }
                else if((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 1 )
                {
                    _Disp.printStringLeftAligned((char*)Yes[_u8LanguageIndex], FONT_ARIAL);
                }
                else if((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 2 )
                {
                    prvPrintSPNErrorVal(15, 83);
                }
                else if((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 3 )
                {
                    prvPrintNotAvailable(15, 83);
                }
            }
            break;

        case DISP_OII_PGN_64554:
        case DISP_S2_PGN_65166:
        case DISP_EFL_P2_PGN_65243:
        case DISP_PROPB5E_PGN_65374_1:
        case DISP_PROPB5E_PGN_65374_2:
        case DISP_PROPB5E_PGN_65374_3:
        case DISP_PROPB5E_PGN_65374_4:
        case DISP_PROPB5E_PGN_65374_5:
        case DISP_PROPB5E_PGN_65374_6:
        case DISP_EFG1_PGN_61450:
        case DISP_LFC_PGN_65257:
        case DISP_TCI4_PGN_65176:
        case DISP_GFC_PGN_65199:
            for(i = 0; i < (astJ1939Display[u8RxPGNNo].u8NoOfSPNs); i++)
            {
                _Disp.gotoxy(3, 15 + (i * 22));
//                if(_u8LanguageIndex == CFGZ::LANGUAGE_SPANISH)
//                {
//                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSpanishSPNName, FONT_VERDANA);
//                }
//                else
                {
                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSPNName, FONT_VERDANA);
                }


                if(_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA))
                {
                    /* If EEC1 PGN is not received for configurable time then display
                     * "NA" - Not available in front of the SPN name.*/
                    if( (_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::NOT_AVAILABLE)) )
                    {
                        prvPrintNotAvailable((uint8_t)(15+(i * 22)), 83);
                    }
                    else if(_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::ERROR)
                    {
                        prvPrintSPNErrorVal((uint8_t)(15 + (i * 22)), 83);
                    }
                }
                else
                {
                    prvPrintJ1939SPNVal(_DisplayPGNScreenData[i], 93,(uint8_t)(26 + (i * 22)));
                    _Disp.gotoxy( 127,(26 + (i * 22)));
                    _Disp.printStringRightAligned((char *)array_units[astJ1939Display[u8RxPGNNo].stpSPN[i].u8SPNUnitIndx],FONT_VERDANA  );

                }
            }
            break;

        case DISP_AI_PGN_65237:
        case DISP_ETC5_PGN_65219:
        case DISP_SHUTDOWN_PGN_65252:
            for(i = 0; i < (astJ1939Display[u8RxPGNNo].u8NoOfSPNs); i++)
            {
                _Disp.gotoxy(3, 15 + (i * 11));
//                if(_u8LanguageIndex == CFGZ::LANGUAGE_SPANISH)
//                {
//                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSpanishSPNName, FONT_VERDANA);
//                }
//                else
                {
                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSPNName, FONT_VERDANA);
                }
                if(_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA))
                {
                    /* If EEC1 PGN is not received for configurable time then display
                     * "NA" - Not available in front of the SPN name.*/
                    if(((_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::NOT_AVAILABLE))))
                    {
                        prvPrintNotAvailable((uint8_t)(15 + (i * 11)), 83);
                    }
                    else if((_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::ERROR) )
                    {
                        prvPrintSPNErrorVal((uint8_t)(15 + (i * 11)), 83);
                    }
                }
                else
                {
                    if(i == J1939APP::ENG_WAIT_TO_STRT_LAMP)
                    {
                        _Disp.gotoxy(83,15);
                        if(((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 0) )
                        {
                            if(_stScreenNo == DISP_AI_PGN_65237)
                            {
                                _Disp.printStringLeftAligned((char*)StrNotCharging, FONT_ARIAL);
                            }
                            else
                            {
                                _Disp.printStringLeftAligned((char*)StrOff[_u8LanguageIndex], FONT_ARIAL);
                            }
                        }
                        else if((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 1 )
                        {
                            if(_stScreenNo == DISP_AI_PGN_65237)
                            {
                                _Disp.printStringLeftAligned((char*)strCharging, FONT_ARIAL);
                            }
                            else
                            {
                                _Disp.printStringLeftAligned((char*)StrOn[_u8LanguageIndex], FONT_ARIAL);
                            }
                        }
                        else if(((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 2) )
                        {
                            prvPrintSPNErrorVal(15, 83);
                        }
                        else if((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 3)
                        {
                            prvPrintNotAvailable(15, 83);
                        }
                    }
                    else if((i == J1939APP::EPS_SHUTDOWN_ENG)&&(_stScreenNo == DISP_SHUTDOWN_PGN_65252))
                    {
                        _Disp.gotoxy(83,26);
                        if((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 0)
                        {
                            _Disp.printStringLeftAligned((char*)No, FONT_ARIAL);
                        }
                        else if(((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 1) )
                        {
                            _Disp.printStringLeftAligned((char*)Yes[_u8LanguageIndex], FONT_ARIAL);
                        }
                        else if(((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 2))
                        {
                            prvPrintSPNErrorVal(26, 83);
                        }
                        else if((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 3)
                        {
                            prvPrintNotAvailable(26, 83);
                        }
                    }
                    else if((i == J1939APP::EPS_APPROACHING_SHUTDOWN)&&(_stScreenNo == DISP_SHUTDOWN_PGN_65252))
                    {
                        _Disp.gotoxy(83,37);
                        if((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 0)
                        {
                            _Disp.printStringLeftAligned((char*)NotApproaching, FONT_ARIAL);
                        }
                        else if(((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 1) )
                        {
                            _Disp.printStringLeftAligned((char*)Approaching, FONT_ARIAL);
                        }
                        else if(((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 2))
                        {
                            prvPrintSPNErrorVal(37, 83);
                        }
                        else if((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 3)
                        {
                            prvPrintNotAvailable(37, 83);
                        }
                    }
                }
            }
            break;


        case DISP_EOI_PGN_64914:
            for(i = 0; i < (astJ1939Display[u8RxPGNNo].u8NoOfSPNs); i++)
            {
                _Disp.gotoxy(3, 15 + (i * 11));
//                if(_u8LanguageIndex == CFGZ::LANGUAGE_SPANISH)
//                {
//                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSpanishSPNName, FONT_VERDANA);
//                }
//                else
                {
                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSPNName, FONT_VERDANA);
                }

                if(_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA))
                {
                    /* If EEC1 PGN is not received for configurable time then display
                     * "NA" - Not available in front of the SPN name.*/
                    if( (_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::NOT_AVAILABLE)) )
                    {
                        prvPrintNotAvailable((uint8_t)(15+(i*11)), 83);
                    }
                    else if(_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::ERROR)
                    {
                        prvPrintSPNErrorVal((uint8_t)(15 + (i * 11)), 83);
                    }
                }
                else
                {
                    if(i == 0)
                    {
                        _Disp.gotoxy(95, 15 + (i * 11));
                        _Disp.printStringCenterAligned((char*)(gstEngOperatingState[_u8LanguageIndex][(uint8_t)_DisplayPGNScreenData[i].f64SpnVal].OperatingState), FONT_VERDANA);
                    }
                    else if(i == 1)
                    {
                        prvPrintJ1939SPNVal(_DisplayPGNScreenData[i], 95, (uint8_t)(15+(i*11)));

                    }
                    else if(i == 2)
                    {
                        _Disp.gotoxy(83,37);
                        if(((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 0))
                        {
                            _Disp.printStringLeftAligned((char*)StrOff[_u8LanguageIndex], FONT_ARIAL);
                        }
                        else if(((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 1) )
                        {
                            _Disp.printStringLeftAligned((char*)StrOn[_u8LanguageIndex], FONT_ARIAL);
                        }
                        else if(((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 2))
                        {
                            prvPrintSPNErrorVal(37, 83);
                        }
                        else if((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 3)
                        {
                            prvPrintNotAvailable(37, 83);
                        }
                    }
                    else{/*do nothing*/}

                    _Disp.gotoxy(127, 15 + (i * 11));
                    _Disp.printStringRightAligned((char *)array_units[astJ1939Display[u8RxPGNNo].stpSPN[i].u8SPNUnitIndx], FONT_VERDANA);
                }
            }
            break;

        case DISP_AT1IG1_PGN_61454:
        case DISP_A1DOC_PGN_64800:
        case DISP_AT1IG2_PGN_64948:
        case DISP_AT1IMG_PGN_64946:
        case DISP_AT1OG1_PGN_61455:
        case DISP_A1SCRDSI1_PGN_61475:
        case DISP_A1SCRDSI2_PGN_64833:
        case DISP_A1SCRDSR1_PGN_61476:
        case DISP_A1SCREGT1_PGN_64830:
        case DISP_AT1T1I_PGN_65110:
        case DISP_AT1OG2_PGN_64947:
        case DISP_AT1S_PGN_64891:
            prvPrintJ1939AfterTreatDataOnScreen(true,u8RxPGNNo);
            break;

        case DISP_AT2IG1_PGN_61456:
        case DISP_AT2OG1_PGN_61457:
        case DISP_A2SCRDSI2_PGN_64827:
        case DISP_A2SCRDSI1_PGN_61478:
        case DISP_A2SCRDSR1_PGN_61479:
        case DISP_A2SCREGT1_PGN_64824:
            prvPrintJ1939AfterTreatDataOnScreen(false, u8RxPGNNo);
            break;

        case DISP_AT1S2_PGN_64697:

            _Disp.gotoxy(64, 12);
            _Disp.printStringCenterAligned((char*)"Service 2", FONT_VERDANA);
            _Disp.gotoxy(3, 24);
//            if(_u8LanguageIndex == CFGZ::LANGUAGE_SPANISH)
//            {
//                _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[0].cs8pSpanishSPNName, FONT_VERDANA);
//            }
//            else
            {
                _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[0].cs8pSPNName, FONT_VERDANA);
            }

            if(_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA))
            {
                /* If EEC1 PGN is not received for configurable time then display
                 * "NA" - Not available in front of the SPN name.*/
                if( (_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::NOT_AVAILABLE)))
                {
                    prvPrintNotAvailable(24, 83);
                }
                else if( _DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::ERROR )
                {
                    _Disp.gotoxy(83, 24);
                    _Disp.printStringLeftAligned((char *)Error, FONT_ARIAL);
                }
            }
            else
            {
                prvPrintJ1939SPNVal(_DisplayPGNScreenData[0], 93,24);
                _Disp.gotoxy(127,24);
                _Disp.printStringRightAligned((char *)array_units[astJ1939Display[u8RxPGNNo].stpSPN[0].u8SPNUnitIndx], FONT_VERDANA);
            }
            break;


        case DISP_PROPB32_PGN_65330:
            {
                if(_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA))
                {
                    if(((_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::NOT_AVAILABLE))))
                    {
                        _Disp.gotoxy(64, 32);
                        prvPrintCANMsgRcvError();
                    }
                    else if((_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::ERROR) && (Error != NULL))
                    {
                        _Disp.gotoxy(1, 34);
                        _Disp.printStringCenterAligned((char*)Error,FONT_ARIAL);
                    }
                }
                else
                {
                    if(((uint8_t)_DisplayPGNScreenData[i].f64SpnVal > 5) || ((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 2))
                    {
                        _Disp.gotoxy(64, 32);
                        prvPrintCANMsgRcvError();
                    }
                    else
                    {
                        _Disp.gotoxy(2, 25);

                        _Disp.printStringLeftAligned((char*)gstNcdInduceData[_u8LanguageIndex][(uint8_t)_DisplayPGNScreenData[i].f64SpnVal].NcdInduceStrategy, FONT_VERDANA );
                    }
                }
            }
            break;


        default:
            for(i = 0; i < (astJ1939Display[u8RxPGNNo].u8NoOfSPNs); i++)
            {
                _Disp.gotoxy(2, 15 + (i * 11));
//                if(_u8LanguageIndex == CFGZ::LANGUAGE_SPANISH)
//                {
//                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSpanishSPNName, FONT_VERDANA);
//                }
//                else
                {
                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSPNName, FONT_VERDANA);
                }
                if(_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA))
                {
                    /* If EEC1 PGN is not received for configurable time then display
                     * "NA" - Not available in front of the SPN name.*/
                    if( (_j1939.IsCommunicationFail() || (_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::NOT_AVAILABLE)) )
                    {
                        prvPrintNotAvailable((uint8_t)(15 + (i*11)), 83);
                    }
                    else if(_DisplayPGNScreenData[i].SPNStatusErrorNA == J1939APP::ERROR)
                    {
                        _Disp.gotoxy(83, 15 + (i * 11));
                        _Disp.printStringLeftAligned((char *)Error, FONT_ARIAL);
                    }
                }
                else
                {
                    prvPrintJ1939SPNVal(_DisplayPGNScreenData[i], 93, (uint8_t)(15 + (i * 11)));

                    _Disp.gotoxy(127, 15 + (i * 11));
                    _Disp.printStringRightAligned((char *)array_units[astJ1939Display[u8RxPGNNo].stpSPN[i].u8SPNUnitIndx], FONT_VERDANA);
                }
            }
            break;
    }

}

void MON_UI::prvPGNScreenDataAssignment(uint8_t u8PGNEnumNo)
{
    uint8_t u8SPNEnumNo = 0;
    uint8_t u8DispPGNNumber = _stScreenNo -DISP_MON_SCREEN_LAST-1;
    (void)memset((void *)(&_DisplayPGNScreenData), 0x00, sizeof(_DisplayPGNScreenData));

    switch(_stScreenNo)
    {
        case DISP_IVECO_ENGINE_STATUS:
            for(u8SPNEnumNo = J1939APP::COOLING_WATER_TEMP_1; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_65282 ; u8SPNEnumNo++)
            {
                if(u8SPNEnumNo == J1939APP::OIL_PRESSURE_3)
                {
                    // Convert kpa to bar i.e 100kpa = 1bar.
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_IVECO_ENG_STATUS_65282, u8SPNEnumNo)/100;
                }
                else
                {
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_IVECO_ENG_STATUS_65282,u8SPNEnumNo);
                }
                prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_IVECO_ENG_STATUS_65282, u8SPNEnumNo);
            }
            break;

        case DISP_AMB_PGN_65269:
            for(u8SPNEnumNo = J1939APP::BAROMETER_PRESSURE_SPN_108; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_65269 ; u8SPNEnumNo++)
            {
                if(u8SPNEnumNo == J1939APP::BAROMETER_PRESSURE_SPN_108)
                {
                    // Convert kpa to bar i.e 100kpa = 1bar.
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_AMB_65269,u8SPNEnumNo)/100;
                }
                else
                {
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal =_j1939.GetReadData(RX_PGN_AMB_65269,u8SPNEnumNo);

                }
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_AMB_65269, u8SPNEnumNo);
            }
            break;

        case DISP_ET1_PGN_65262_1:
            for(u8SPNEnumNo = J1939APP::ENG_CLNT_TEMP_SPN_110; u8SPNEnumNo < J1939APP::ENG_TURBO_OIL_TEMP_SPN_176; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal =_j1939.GetReadData(RX_PGN_ET1_65262,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_ET1_65262, u8SPNEnumNo);
            }
            break;

        case DISP_ET1_PGN_65262_2:
            for(u8SPNEnumNo = J1939APP::ENG_TURBO_OIL_TEMP_SPN_176; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_65262; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo - J1939APP::ENG_TURBO_OIL_TEMP_SPN_176].f64SpnVal =_j1939.GetReadData(RX_PGN_ET1_65262,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo - J1939APP::ENG_TURBO_OIL_TEMP_SPN_176], RX_PGN_ET1_65262, u8SPNEnumNo);
            }
            break;

        case DISP_EEC1_PGN_61444_1:
            for(u8SPNEnumNo = J1939APP::ENGINE_TORQUE_SPN_899; u8SPNEnumNo < J1939APP::SOURCE_ADDR_SPN_1483; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal =_j1939.GetReadData(RX_PGN_EEC1_61444,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_EEC1_61444, u8SPNEnumNo);
            }
            break;

        case DISP_EEC1_PGN_61444_2:
            for(u8SPNEnumNo = J1939APP::SOURCE_ADDR_SPN_1483; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_61444; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SOURCE_ADDR_SPN_1483].f64SpnVal =_j1939.GetReadData(RX_PGN_EEC1_61444,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SOURCE_ADDR_SPN_1483], RX_PGN_EEC1_61444, u8SPNEnumNo);
            }
            break;

        case DISP_PROPB5E_PGN_65374_1:
            for(u8SPNEnumNo = J1939APP::SPN_1; u8SPNEnumNo < J1939APP::SPN_3; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal =_j1939.GetReadData(RX_PGN_PROPB5E_65374,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_PROPB5E_65374, u8SPNEnumNo);
            }
            break;

        case DISP_PROPB5E_PGN_65374_2:
            for(u8SPNEnumNo = J1939APP::SPN_3; u8SPNEnumNo < J1939APP::SPN_5; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SPN_3].f64SpnVal =_j1939.GetReadData(RX_PGN_PROPB5E_65374,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SPN_3], RX_PGN_PROPB5E_65374, u8SPNEnumNo);
            }
            break;

        case DISP_PROPB5E_PGN_65374_3:
            for(u8SPNEnumNo = J1939APP::SPN_5; u8SPNEnumNo < J1939APP::SPN_7; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SPN_5].f64SpnVal =_j1939.GetReadData(RX_PGN_PROPB5E_65374,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SPN_5], RX_PGN_PROPB5E_65374, u8SPNEnumNo);
            }
            break;

        case DISP_PROPB5E_PGN_65374_4:
            for(u8SPNEnumNo = J1939APP::SPN_7; u8SPNEnumNo < J1939APP::SPN_9; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SPN_7].f64SpnVal =_j1939.GetReadData(RX_PGN_PROPB5E_65374,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SPN_7], RX_PGN_PROPB5E_65374, u8SPNEnumNo);
            }
            break;

        case DISP_PROPB5E_PGN_65374_5:
            for(u8SPNEnumNo = J1939APP::SPN_9; u8SPNEnumNo < J1939APP::SPN_11; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SPN_9].f64SpnVal =_j1939.GetReadData(RX_PGN_PROPB5E_65374,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SPN_9], RX_PGN_PROPB5E_65374, u8SPNEnumNo);
            }
            break;

        case DISP_PROPB5E_PGN_65374_6:
            for(u8SPNEnumNo = J1939APP::SPN_11; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_65374; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SPN_11].f64SpnVal =_j1939.GetReadData(RX_PGN_PROPB5E_65374,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo - J1939APP::SPN_11], RX_PGN_PROPB5E_65374, u8SPNEnumNo);
            }
            break;

        case DISP_EFL_P1_PGN_65263_1:
            for(u8SPNEnumNo = J1939APP::ENG_FUEL_PRESS_SPN_94; u8SPNEnumNo < J1939APP::ENG_CRANCKCASE_PRESS_SPN_101; u8SPNEnumNo++)
            {
                if(u8SPNEnumNo != J1939APP::ENG_OIL_LVL_SPN_98)
                {
                    // Convert kpa to bar i.e 100kpa = 1bar.
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal =_j1939.GetReadData(RX_PGN_EFL_P1_65263,u8SPNEnumNo)/100;
                    prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_EFL_P1_65263, u8SPNEnumNo);
                }
                else
                {
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal =_j1939.GetReadData(RX_PGN_EFL_P1_65263,u8SPNEnumNo);
                    prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_EFL_P1_65263, u8SPNEnumNo);
                }
            }
            break;
        case DISP_EFL_P1_PGN_65263_2:
            for(u8SPNEnumNo = J1939APP::ENG_CRANCKCASE_PRESS_SPN_101; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_65263; u8SPNEnumNo++)
            {
                if((u8SPNEnumNo == J1939APP::ENG_CRANCKCASE_PRESS_SPN_101) || (u8SPNEnumNo == J1939APP::ENG_COOLANT_PRESS_SPN_109))
                {
                    // Convert kpa to bar i.e 100kpa = 1bar.
                    _DisplayPGNScreenData[u8SPNEnumNo -J1939APP::ENG_CRANCKCASE_PRESS_SPN_101].f64SpnVal = _j1939.GetReadData(RX_PGN_EFL_P1_65263,u8SPNEnumNo)/100;
                    prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo - J1939APP::ENG_CRANCKCASE_PRESS_SPN_101], RX_PGN_EFL_P1_65263, u8SPNEnumNo );
                }
                else
                {
                    _DisplayPGNScreenData[u8SPNEnumNo-3].f64SpnVal =_j1939.GetReadData(RX_PGN_EFL_P1_65263,u8SPNEnumNo);
                    prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo-3], RX_PGN_EFL_P1_65263, u8SPNEnumNo );
                }
            }
            break;
        case DISP_IC1_PGN_65270_1:
            for(u8SPNEnumNo = J1939APP::ENG_TRAP_INLET_PRESS_SPN_81; u8SPNEnumNo < J1939APP::ENG_AIR_FILT_DIFF_PRESS_SPN_107; u8SPNEnumNo++)
            {
                if(u8SPNEnumNo == J1939APP::ENG_INTAKE_MANIFOLD_TEMP_SPN_105)
                {
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal =_j1939.GetReadData(RX_PGN_IC1_65270,u8SPNEnumNo);
                }
                else
                {
                    // Convert kpa to bar i.e 100kpa = 1bar.
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_IC1_65270,u8SPNEnumNo)/100;

                }
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_IC1_65270, u8SPNEnumNo );
            }
            break;
        case DISP_IC1_PGN_65270_2:
            for(u8SPNEnumNo = J1939APP::ENG_AIR_FILT_DIFF_PRESS_SPN_107; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_65270; u8SPNEnumNo++)
            {
                if(u8SPNEnumNo == J1939APP::ENG_EXHAUST_GAS_TEMP_SPN_173)
                {
                    _DisplayPGNScreenData[u8SPNEnumNo -J1939APP::ENG_AIR_FILT_DIFF_PRESS_SPN_107].f64SpnVal =_j1939.GetReadData(RX_PGN_IC1_65270,u8SPNEnumNo);

                }
                else
                {
                    // Convert kpa to bar i.e 100kpa = 1bar.
                    _DisplayPGNScreenData[u8SPNEnumNo - J1939APP::ENG_AIR_FILT_DIFF_PRESS_SPN_107].f64SpnVal = _j1939.GetReadData(RX_PGN_IC1_65270,u8SPNEnumNo)/100;
                }
                prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo -J1939APP::ENG_AIR_FILT_DIFF_PRESS_SPN_107], RX_PGN_IC1_65270, u8SPNEnumNo );
            }
            break;

        case DISP_DD_PGN_65276:
            for(u8SPNEnumNo = J1939APP::ENG_FUEL_FILT_DIFF_PRESS_SPN_95; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_65276; u8SPNEnumNo++)
            {
                // Convert kpa to bar i.e 100kpa = 1bar.
                _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_DD_65276,u8SPNEnumNo)/100;
                prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_DD_65276, u8SPNEnumNo);
            }
            break;
        case DISP_IC2_PGN_64976:
            for(u8SPNEnumNo = J1939APP::ENG_AIR_FILT_PRESS_SPN_2809; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_64976; u8SPNEnumNo++)
            {
                // Convert kpa to bar i.e 100kpa = 1bar.
                _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_IC2_64976,u8SPNEnumNo)/100;
                prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_IC2_64976, u8SPNEnumNo);
            }
            break;
        case DISP_LFC_PGN_65257:
            for(u8SPNEnumNo = J1939APP::ENG_TRIP_FUEL_SPN_182; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_65257; u8SPNEnumNo++)
            {
                if(u8SPNEnumNo == J1939APP::ENG_TRIP_FUEL_SPN_182)
                {
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_LFC_65257,u8SPNEnumNo);
                    prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_LFC_65257, u8SPNEnumNo);
                }
                else
                {
                    // Convert Litre to KL.
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_LFC_65257,u8SPNEnumNo)/1000;
                    prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_LFC_65257, u8SPNEnumNo);
                }
            }
            break;
        case DISP_A1SCRDSI1_PGN_61475:
            for(u8SPNEnumNo = J1939APP::EXHAUST_FLUID_DOSING_QUANTITY_SPN_4331; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_61475; u8SPNEnumNo++)
            {
                if(u8SPNEnumNo == J1939APP::EXHAUST_FLUID_DOSING_QUANTITY_SPN_4331)
                {
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal =_j1939.GetReadData(RX_PGN_A1SCRDSI1_61475,u8SPNEnumNo);
                    prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_A1SCRDSI1_61475, u8SPNEnumNo);
                }
                else
                {
                    // Convert kpa to bar i.e 100kpa = 1bar.
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_A1SCRDSI1_61475,u8SPNEnumNo)/100;
                    prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_A1SCRDSI1_61475, u8SPNEnumNo);
                }
            }
            break;
        case DISP_A2SCRDSI1_PGN_61478:
            for(u8SPNEnumNo = J1939APP::EXAUST_FLUID_ACT_DOSING_QUANTITY_SPN_4384; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_61478; u8SPNEnumNo++)
            {
                if(u8SPNEnumNo == J1939APP::EXAUST_FLUID_ACT_DOSING_QUANTITY_SPN_4384)
                {
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_A2SCRDSI1_61478,u8SPNEnumNo);

                }
                else
                {
                    // Convert kpa to bar i.e 100kpa = 1bar.
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_A2SCRDSI1_61478,u8SPNEnumNo)/100;
                }
                prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_A2SCRDSI1_61478, u8SPNEnumNo);
            }
            break;
        case DISP_AT1S2_PGN_64697:
            // Convert seconds to hours
            _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_AT1S2_64697,u8SPNEnumNo)/3600;
            prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_AT1S2_64697, u8SPNEnumNo);
            break;

        case DISP_AT1T1I_PGN_65110:
            for(u8SPNEnumNo = J1939APP::DISEL_EXHAUST_TANK_LVL_SPN_1761; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_65110; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal= _j1939.GetReadData(RX_PGN_AT1T1I_65110,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_AT1T1I_65110, u8SPNEnumNo);
            }
            break;
        case DISP_PROSTOUT_PGN_65364_1 :
            for(u8SPNEnumNo = 0; u8SPNEnumNo < 4; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal= _j1939.GetReadData(RX_PGN_PROSTOUT_65364,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_PROSTOUT_65364, u8SPNEnumNo);
            }
            break;
        case DISP_PROSTOUT_PGN_65364_2:
            for(u8SPNEnumNo = 4; u8SPNEnumNo < 8; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo - 4].f64SpnVal= _j1939.GetReadData(RX_PGN_PROSTOUT_65364,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo - 4], RX_PGN_PROSTOUT_65364, u8SPNEnumNo);
            }
            break;

        case DISP_HATZ_PROPB_PHYS_PGN_65280:
        case DISP_AI_PGN_65237:
        case DISP_ETC5_PGN_65219:
        case DISP_OII_PGN_64554:
        case DISP_HATZ_CCVS_PGN_65265:
        case DISP_IT1_PGN_65154:
        case DISP_GFP_PGN_65163:
        case DISP_IMI1_PGN_65190:
        case DISP_FD1_PGN_65213   :
        case DISP_DLCC1_PGN_64775 :
        case DISP_GFC_PGN_65199  :
        {
            /*Amruta - 23-Apr-2020
             * Below subtraction is done in order to match display array enum variable
             * with database array enum.
             * */
            for(u8SPNEnumNo = 0; u8SPNEnumNo < astJ1939Display[u8DispPGNNumber].u8NoOfSPNs; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal= _j1939.GetReadData((DATABASE_RX_PGN_LIST_t)(u8PGNEnumNo),u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP(&_DisplayPGNScreenData[u8SPNEnumNo], u8PGNEnumNo , u8SPNEnumNo );
            }
            break;
        }
        case DISP_AT1IG1_PGN_61454   :
        case DISP_A1DOC_PGN_64800    :
        case DISP_AT1IG2_PGN_64948   :
        case DISP_AT1IMG_PGN_64946   :
        case DISP_AT1OG1_PGN_61455   :
        case DISP_A1SCRDSI2_PGN_64833:
        case DISP_A1SCRDSR1_PGN_61476:
        case DISP_A1SCREGT1_PGN_64830:
        case DISP_AT2IG1_PGN_61456   :
        case DISP_AT2OG1_PGN_61457   :
        case DISP_A2SCRDSI2_PGN_64827:
        case DISP_A2SCRDSR1_PGN_61479:
        case DISP_A2SCREGT1_PGN_64824:
        case DISP_AT1OG2_PGN_64947   :
        case DISP_AT1S_PGN_64891     :
        case DISP_PROPB32_PGN_65330  :
            {
                /*Amruta - 23-Apr-2020
                 * Below subtraction is done in order to match display array enum variable
                 * with database array enum.
                 * */
                for(u8SPNEnumNo = 0; u8SPNEnumNo < astJ1939Display[u8DispPGNNumber].u8NoOfSPNs; u8SPNEnumNo++)
                {
                    _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData((DATABASE_RX_PGN_LIST_t)(u8PGNEnumNo ),u8SPNEnumNo);
                    prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], u8PGNEnumNo, u8SPNEnumNo );
                }
                break;
            }

        case DISP_EEC3_PGN_65247_1  :
            for(u8SPNEnumNo = J1939APP::NOMINAL_FRICTION_PERCENT_TORQUE; u8SPNEnumNo < J1939APP::PARISITIC_TORQUE_LOSS; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData(RX_PGN_EEC3_65247,u8SPNEnumNo);
                 prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], RX_PGN_EEC3_65247, u8SPNEnumNo );
             }
            break;
        case DISP_EEC3_PGN_65247_2  :
           for(u8SPNEnumNo = J1939APP::PARISITIC_TORQUE_LOSS; u8SPNEnumNo < J1939APP::LAST_SPN_IN_PGN_65247; u8SPNEnumNo++)
           {
               _DisplayPGNScreenData[u8SPNEnumNo - J1939APP::PARISITIC_TORQUE_LOSS].f64SpnVal = _j1939.GetReadData(RX_PGN_EEC3_65247,(u8SPNEnumNo));
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo - J1939APP::PARISITIC_TORQUE_LOSS], RX_PGN_EEC3_65247, u8SPNEnumNo );
            }
           break;
        default:
            for(u8SPNEnumNo = 0; u8SPNEnumNo < astJ1939Display[u8DispPGNNumber].u8NoOfSPNs; u8SPNEnumNo++)
            {
                _DisplayPGNScreenData[u8SPNEnumNo].f64SpnVal = _j1939.GetReadData((DATABASE_RX_PGN_LIST_t)u8PGNEnumNo,u8SPNEnumNo);
                prvAssignNoOfDigitAfterDP( &_DisplayPGNScreenData[u8SPNEnumNo], u8PGNEnumNo, u8SPNEnumNo );
            }
            break;
    }
}
#endif


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
                        /* program should not enter here */
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
            if(0) /* TODO: in nxp code is it always true */
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
                sprintf(arrTemp,"%0.1f",abs(_hal.AcSensors.GENSET_GetDispAveragePowerFactor()));
                _Disp.gotoxy(GLCD_X(22),GLCD_Y(33));
                _Disp.printStringCenterAligned((char *)arrTemp,FONT_ARIAL);
                _Disp.gotoxy(GLCD_X(23),GLCD_Y(50));
                _Disp.printStringCenterAligned((char *)StrPF,FONT_ARIAL);
                u8Position = 22;
                for(u8Local = R_PHASE; u8Local < PHASE_END ; u8Local++)
                {
                    sprintf(arrTemp,"%s-%s    %0.1f",(char *)StrPF,
                        (char *)strPhase[u8Local],
                        abs(_hal.AcSensors.GENSET_GetDispPowerFactor((PHASE_t)u8Local)));
                    _Disp.gotoxy(GLCD_X(55),GLCD_Y(u8Position));
                    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                    u8Position = u8Position + 15;
                }
            }
            else
            {
                _Disp.printImage((uint8_t *)gau8GenPFLogo, 5, 32, 26, 7);
                sprintf(arrTemp,"%s-%s    %0.1f",(char *)StrPF,
                   (char *)strPhase[R_PHASE],
                _hal.AcSensors.GENSET_GetDispPowerFactor((PHASE_t)R_PHASE));
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
            }
            else
            {
                /* Below logo will be displayed for Gen and Mains as well */
                _Disp.printImage((uint8_t *)gau8GeneratorVoltLogo, 4, 32, 26, 7);

                _Disp.gotoxy(GLCD_X(40),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)strPhase[R_PHASE],FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(68),GLCD_Y(35));
                sprintf(arrTemp,"%0.1f",((&_hal.AcSensors)->*ArrGetCurrentVal[GENSET])((PHASE_t)R_PHASE));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(71),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)StrA, FONT_VERDANA);
            }

            _Disp.gotoxy(GLCD_X(100),GLCD_Y(35));
            sprintf(arrTemp,"%0.1f", f32GenMinFreq);
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(102),GLCD_Y(35));
            _Disp.printStringLeftAligned((char *)"Hz", FONT_VERDANA);
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

            if(_j1939.IsCommunicationFail()&& _cfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_BATTERY_MON_BY_J1939))
            {
                _Disp.printStringRightAligned((char *)"NA",FONT_ARIAL);
            }
            else
            {
                sprintf(arrTemp,"%0.1f",
                        _GCUAlarms.GetSelectedBatteryVtg());
                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                _Disp.gotoxy(GLCD_X(116),GLCD_Y(26));
                _Disp.printStringLeftAligned((char*)"V",FONT_VERDANA);
            }

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

                _Disp.gotoxy(GLCD_X(70),GLCD_Y(35));
                sprintf(arrTemp, "%ld",(_EngineMon.GetBTSRunTimeMin()/60));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                _Disp.gotoxy(GLCD_X(72),GLCD_Y(35));
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
            stTemp =_GCUAlarms.GetSelectedTempSensVal();
            _Disp.printImage((uint8_t *)u8ArrEngineTemp, 4, 32, 26, 7);

          _Disp.gotoxy(GLCD_X(115),GLCD_Y(33));

          {
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
            stTemp = _GCUAlarms.GetLOPSensorVal();

            _Disp.gotoxy(GLCD_X(115),GLCD_Y(33));

            {
                if(stTemp.eStatus == A_SENSE::SENSOR_NOT_CONFIGRUED)
                {
                   _Disp.gotoxy(GLCD_X(70),GLCD_Y(37));
                   _Disp.printStringLeftAligned((char*)StrNotConfigured,FONT_VERDANA);
                }
                else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
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
            stTemp =_hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);

            prvPrintSensorStatus(stTemp,(char*)"%", INTEGER_TYPE);
            if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
            {
                stTemp.stValAndStatus.f32InstSensorVal =
                        stTemp.stValAndStatus.f32InstSensorVal
                        * _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY)/100;

                    sprintf(arrTemp,"%d",(uint16_t)((round)(stTemp.stValAndStatus.f32InstSensorVal)));
                    _Disp.gotoxy(GLCD_X(93),GLCD_Y(42));
                    _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                    _Disp.gotoxy(GLCD_X(94),GLCD_Y(42));
                    _Disp.printStringLeftAligned((char*)"Liters",FONT_VERDANA);
            }
        }
        break;

        case DISP_MON_SHELTER_TEMP:
        {
            _Disp.printImage((uint8_t *)gau8LIShelterTemp, 4, 32, 26, 7);
            _Disp.gotoxy(GLCD_X(110),GLCD_Y(33));

            stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_SHELTER_TEMPERATURE);

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
        break;

        /* TODO: aux 1 is commented in GC2111 NXP code */
        case DISP_MON_AUX_2:
        {
             stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S2_SENSOR);
             prvPrintSensorStatus(stTemp,(char*)"", FLOAT_TYPE , 80 , 37);
        }
        break;

        case DISP_MON_ENG_SPEED:
        {
            _Disp.printImage((uint8_t *)u8ArrEngineSpeed, 4, 32, 26, 7);

            _Disp.gotoxy(GLCD_X(64),GLCD_Y(37));
            sprintf(arrTemp,"%d ",(uint16_t) _GCUAlarms.GetSpeedValue());
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);

            _Disp.gotoxy(GLCD_X(65),GLCD_Y(37));
            _Disp.printStringLeftAligned((char*)"RPM",FONT_VERDANA);

        }
        break;
        case DISP_MON_ENG_RUN_TIME:
        {
            _Disp.printImage((uint8_t *)u8ArrEngineHours, 4, 32, 26, 7);

            _Disp.gotoxy(GLCD_X(124),GLCD_Y(22));
            sprintf(arrTemp,"%ldhrs  %dmin ", ( _GCUAlarms.GetSelectedEngRunMin()/60), (uint8_t)(_GCUAlarms.GetSelectedEngRunMin()%60) );
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

            {
                _Disp.gotoxy(GLCD_X(40),GLCD_Y(37));
                _Disp.printStringLeftAligned((char *)StrStarts[_u8LanguageIndex],FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(40),GLCD_Y(52));
                _Disp.printStringLeftAligned((char *)StrTrips[_u8LanguageIndex],FONT_VERDANA);
            }

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

    char EngSrNo[12];
    _cfgz.GetEngSrNo(EngSrNo);

    sprintf(arrTemp,"Ver :");
    _Disp.gotoxy(GLCD_X(2),GLCD_Y(22));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

    sprintf(arrTemp,"%s%d-R%d.%02d",SeriesId,_cfgc.GetProductID(),_cfgc.GetProductRevision(),_cfgc.GetProductVersion());
    _Disp.gotoxy(GLCD_X(43),GLCD_Y(22));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

    sprintf(arrTemp,"Eng Sr  :");
    _Disp.gotoxy(GLCD_X(2),GLCD_Y(34));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

/*
 * SuryaPranayTeja.BVV
 * Engine Serial number does not have all the ASCII characters
 * So the below loop copy the appropriate character and display
 */
    for(int i=0;i<12;i++)
    {
        if(EngSrNo[i] == 47)
        {
            sprintf(arrTemp,"%c",35);
        }
        else if(EngSrNo[i] > 57)
        {
            sprintf(arrTemp,"%c",EngSrNo[i]+7);
        }
        else
        {
            sprintf(arrTemp,"%c",EngSrNo[i]);
        }
        _Disp.gotoxy(GLCD_X(43+6*i),GLCD_Y(34));
        _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
    }

    sprintf(arrTemp,(char*)StrInfoDate[_u8LanguageIndex]);
    _Disp.gotoxy(GLCD_X(2),GLCD_Y(47));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

    sprintf(arrTemp,"%d.%s.%d",CurrentTime.u8Day, (char*)StrMonth[_u8LanguageIndex][CurrentTime.u8Month - 1],CurrentTime.u16Year);
    _Disp.gotoxy(GLCD_X(48),GLCD_Y(47));
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
    _Disp.gotoxy(GLCD_X(102),GLCD_Y(47));
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

        _Disp.printImage((uint8_t *)gau8GeneratorVoltLogo, 4, 32, 26, 7);

        _Disp.gotoxy(GLCD_X(50),GLCD_Y(37));
        _Disp.printStringLeftAligned((char *)strPhase[R_PHASE],FONT_VERDANA);

        _Disp.gotoxy(GLCD_X(90),GLCD_Y(37));
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
        _Disp.printImage((uint8_t *)gau8GeneratorVoltLogo, 4, 32, 26, 7);

        _Disp.gotoxy(GLCD_X(50),GLCD_Y(37));
        _Disp.printStringLeftAligned((char *)strPhase[R_PHASE],FONT_VERDANA);

        sprintf(arrTemp,"%0.1f",((&_hal.AcSensors)->*ArrGetVal[eSourceType][eType])(R_PHASE)/1000);
        _Disp.gotoxy(GLCD_X(90),GLCD_Y(37));
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
    _startStop.StopKeyPressed();

    if(eDisplayMode == DISP_MON_MODE)
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
                _startStop.StopCommand();
            }
            else
            {
               _startStop.StopCommand();
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
                _startStop.StopCommand();
            }
            else
            {
                _startStop.StopCommand();
            }
        }
        else if(_eOpMode == BASE_MODES::TEST_MODE)
        {
            _manualMode.ComeOutOfTestMode();
            _startStop.StopCommand();
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
    _startStop.StartKeyPressed();
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

#if ENABLE_MON_J1939
void MON_UI::prvBuildLampIconScreen(void)
{
    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.gotoxy(GLCD_X(64),GLCD_Y(5));
    _Disp.printStringCenterAligned((char *)StrLampIcons[_u8LanguageIndex],FONT_ARIAL);
    _Disp.drawHorizontalLine(0, 19, 127);
    /* to solve SGC-225 */
    if( _j1939.IsCommunicationFail() || (!_j1939.IsDM1PGNRecieved()) )
    {
        _Disp.gotoxy(32,64);
        _Disp.printStringCenterAligned((char*)strCANMsgRcvError[_u8LanguageIndex],FONT_ARIAL);
    }
    else
    {
        if(_j1939.GetLampStatus(J1939APP::RED_LAMP) == 1)
        {
            _Disp.printImage((uint8_t *)&gau8Icon6, 6, 27, 25, 1);
        }

        if(_j1939.GetLampStatus(J1939APP::AMBER_LAMP)  == 1)
        {
            _Disp.printImage((uint8_t *)&gau8Icon7, 6, 27, 25, 43);
        }

        if(_j1939.GetLampStatus(J1939APP::MIL_LAMP) == 1)
        {
            _Disp.printImage((uint8_t *)&gau8Icon8, 6, 27, 25, 85);
        }
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

    if(UTILS_GetElapsedTimeInMs(&tmr) >= 500)
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
    _Disp.printStringCenterAligned((char *)StrExhaustAFT[_u8LanguageIndex],FONT_ARIAL);
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
        _Disp.printStringCenterAligned((char*)strCANMsgRcvError[_u8LanguageIndex],FONT_ARIAL);
    }
    else
    {
        if((uint8_t)_j1939.GetReadData(RX_PGN_DPFC1_64892,0) == 1)
        {
            _Disp.printImage((uint8_t *)&gau8Icon0, 4, 24, 13, 4);
        }

        if((uint8_t)_j1939.GetReadData(RX_PGN_DPFC1_64892,1) == 1)
        {
            _Disp.printImage((uint8_t *)&gau8Icon1, 5, 22, 13, 45);
        }
        else if((uint8_t)_j1939.GetReadData(RX_PGN_DPFC1_64892,1)== 4)
        {
            if(_blink)
            {
                _Disp.printImage((uint8_t *)&gau8Icon1, 5, 22, 13, 45);
            }
            else
            {
                // do nothing.
            }
        }
        if((uint8_t)_j1939.GetReadData(RX_PGN_DPFC1_64892,2) == 1)
        {
            _Disp.printImage((uint8_t *)&gau8Icon2, 5, 24, 13, 85);
        }

        /* AT1T1I */
        /* Displayed the  image for SPN 5245 of PGN AT1T1I depending on the received value.
             *          000             OFF
             *          001             ON-Solid
             *          100             ON-Fast blink (1 HZ)*/
        if((uint8_t)_j1939.GetReadData(RX_PGN_AT1T1I_65110,2)  == 1)
        {
            _Disp.printImage((uint8_t *)&gau8Icon3, 4, 27, 35, 28);
        }
        else if((uint8_t)_j1939.GetReadData(RX_PGN_AT1T1I_65110,2) == 4)
        {
            if(_blink)
            {
                _Disp.printImage((uint8_t *)&gau8Icon3, 4, 27, 35, 28);
            }
            else
            {
                // do nothing.
            }
        }
        /* Displayed the icon for SPN 5246 when malfunction lamp is active or the value received from
         * SPN 5246 is more than or equal to 3.
         * 30/4/20 : Mal-function Lamp value compared with Lamp ON status.*/
        if((_j1939.GetLampStatus(J1939APP::MIL_LAMP)== 1) || ((2 < _j1939.GetReadData(RX_PGN_AT1T1I_65110,3)) && (_j1939.GetReadData(RX_PGN_AT1T1I_65110,3) < 7)))
        {
            _Disp.printImage((uint8_t *)&gau8Icon4, 4, 20, 40, 72);
        }
    }
}
#endif
