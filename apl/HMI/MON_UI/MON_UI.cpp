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
uint8_t MON_UI::_stScreenNo           = MON_UI::DISP_HOME;
MODE_TYPE_t MON_UI::eDisplayMode      = DISP_MON_MODE;
MON_UI::MON_SCREEN_st MON_UI::eMonScreenType = MON_SCREEN_NORMAL;

MON_UI::MON_UI(HAL_Manager &hal, MANUAL_MODE &manualMode,AUTO_EXERCISE_MODE &autoExercise,
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
_autoExercise(autoExercise),
_j1939(j1939),
_BTSMode(BTSMode),
_cyclicMode(cyclicMode),
_eOpMode (BASE_MODES::MANUAL_MODE),
ExternalInputUpdateTimer{0},
_ArrScreenEnDs{false,false,false,
    false,false,false,false,false,false,
    false,
    false,false,false,false,false,false,false,false,false,false,false},
_f32GenMinFreq(0),
_f32MainsFreq(0),
_u8ScreenMin(0),
_u8ScreenMax(DISP_MON_SCREEN_LAST)
{
    UTILS_ResetTimer(&ExternalInputUpdateTimer);
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_POWER_ON_LAMP_TEST_EN) == CFGZ::CFGZ_ENABLE)
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
    UTILS_ResetTimer(&_GroupSwitchingTimer);

    //Print Boot logo
    prvDisplayBootLogo();
    Init();

}

void MON_UI::Init()
{
   prvConfigureScreenEnable();
   if(_cfgz.GetCFGZ_Param(CFGZ::ID_POWER_ON_MODE) ==
           CFGZ::CFGZ_GENSET_MODE_MANUAL)
   {
       _eOpMode = BASE_MODES::MANUAL_MODE;
   }
   else
   {
       if((_cfgz.GetCFGZ_Param(CFGZ::ID_BATT_MON_EN) == CFGZ::CFGZ_ENABLE)
               ||(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2))
       {
           _eOpMode = BASE_MODES::BTS_MODE;
       }
       else if(_cfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_MODE_EN) == CFGZ::CFGZ_ENABLE)
       {
           _eOpMode = BASE_MODES::CYCLIC_MODE;
       }
       else
       {
           _eOpMode = BASE_MODES::AUTO_MODE;
       }

   }
   _manualMode.SetGCUOperatingMode(_eOpMode);

   _hal.ObjGlcd.AdjustContrast(_cfgz.GetCFGZ_Param(CFGZ::ID_PERCENT_CONTRAST));
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
            if(_EngineMon.GetAndClearIsLoadStatusChanged())
            {
                prvConfigureScreenEnable();
            }

            if(bRefresh)
            {
                if(eMonScreenType == MON_SCREEN_J1939)
                {
                    prvPGNScreenDataAssignment(u8ArrMatrixDispAndRXPGN[_stScreenNo-DISP_MON_SCREEN_LAST-1]);
                }
                prvDisplayMonScreen();
            }

            if(UTILS_GetElapsedTimeInMs(&ExternalInputUpdateTimer)
                    >= EXT_INPUT_UPDATE_TIME_MS)
            {
                UTILS_ResetTimer(&ExternalInputUpdateTimer);
                if(_startStop.IsSimStopReceived())
                {
                    prvStopKeyPressAction();
                }
                else if(_startStop.IsSimStartReceived())
                {
                    prvStartKeyPressAction();
                }
                else if(_startStop.IsSimAutoReceived() || _bMBModeChnageCMDRcvd)
                {
                    prvAutoKeyPressAction();
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
//                    &&(_manualMode.GetTimerState() != BASE_MODES::START_DELAY_TIMER)
//                    &&(_manualMode.GetTimerState() != BASE_MODES::PREHEAT_TIMER)
                    &&(eMonScreenType == MON_SCREEN_NORMAL))
            {
                _stScreenNo = DISP_HOME;
                eMonScreenType = MON_SCREEN_NORMAL;
            }

            if( _eOpMode ==  BASE_MODES::AUTO_MODE)
            {
                if(_autoExercise.GetGCUOperatingMode() ==BASE_MODES::AUTO_EXERCISE_MODE )
                {
                    _eOpMode =  BASE_MODES::AUTO_EXERCISE_MODE;
                }
                else if(_autoExercise.GetGCUOperatingMode() ==BASE_MODES::MANUAL_MODE )
                {
                    _eOpMode =  BASE_MODES::MANUAL_MODE;
                }
            }
            else  if(_eOpMode ==  BASE_MODES::AUTO_EXERCISE_MODE)
            {
                if(_autoExercise.GetGCUOperatingMode() ==BASE_MODES::AUTO_MODE )
                {
                    _eOpMode =  BASE_MODES::AUTO_MODE;
                }
                else if(_autoExercise.GetGCUOperatingMode() ==BASE_MODES::MANUAL_MODE )
                {
                    _eOpMode =  BASE_MODES::MANUAL_MODE;
                }
            }
        }
        break;

        default: break;
        }

    if(eMonScreenType ==MON_SCREEN_NORMAL )
    {
        _u8ScreenMin= 0;
        _u8ScreenMax = DISP_MON_SCREEN_LAST;
    }
    else
    {
        _u8ScreenMin= DISP_PROPB62_PGN_65378;
        _u8ScreenMax = DISP_J1939_RX_LAST;
    }


}

void MON_UI::GoToHomeScreen()
{
    _stScreenNo = DISP_HOME;
    eMonScreenType =MON_SCREEN_NORMAL;
}

void MON_UI::CheckKeyPress(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent)
{
    switch(_sKeyEvent)
    {
        case AUTO_KEY_SHORT_PRESS:
        {
            prvAutoKeyPressAction();
            _stScreenNo = DISP_HOME;
            eDisplayMode = DISP_MON_MODE;
            MON_UI::eMonScreenType =MON_UI::MON_SCREEN_NORMAL;
        }
        break;

        case START_KEY_SHORT_PRESS:
        {
            _stScreenNo = DISP_HOME;
            eDisplayMode = DISP_MON_MODE;
            MON_UI::eMonScreenType =MON_UI::MON_SCREEN_NORMAL;
            prvStartKeyPressAction();
           break;
        }
        case STOP_KEY_SHORT_PRESS:
        {
            prvStopKeyPressAction();
            break;
        }

        case UP_SHORT_PRESS:  //Up key Press
        {

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
                    MON_UI::_stScreenNo = DISP_HOME;
                    if( _cfgz.GetEngType()==CFGZ::ENG_KUBOTA)
                    {
                        ALARM_UI::ChangeAlarmScreenType(ALARM_UI::NCD);
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


            break;
        }
        case DN_SHORT_PRESS: //Dn key Press
        {

           //Note while check for _stScreenNo shall be placed immediately
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
                       MON_UI::_stScreenNo = DISP_HOME;
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
                  MON_UI::_stScreenNo = DISP_HOME;
                  ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM2);
                  _j1939.RequestDM2Messages();
               }

           }
            break;
        }
        case ACK_KEY_PRESS:
        {

            if(_hal.actuators.GetActStatus(ACTUATOR::ACT_AUDIBLE_ALARM) == true)
            {
                _GCUAlarms.TurnOffSounderAlarm();
            }
            else
            {
                _GCUAlarms.ClearAllAlarms();
                if(eMonScreenType == MON_SCREEN_NORMAL)
                {
                    _stScreenNo = DISP_HOME;
                }
            }
            break;
        }
        case DN_STOP_KEY_LONG_PRESS:
        {
            //software reset to go in boot loader
           NVIC_SystemReset();

            break;
        }

        case DN_LONG_PRESS:
        {
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) && (!_manualMode.IsGenRunTimersInProcess()))
            {
                if(eMonScreenType == MON_SCREEN_NORMAL)
                {
                    eDisplayMode = DISP_ALARM_MODE;
                    ALARM_UI::ClearAlarmScreenNum();
                    ALARM_UI::ChangeAlarmScreenType(ALARM_UI::DM2);
                    _j1939.RequestDM2Messages();
                }

                    eMonScreenType = MON_SCREEN_NORMAL;
                   _stScreenNo = DISP_HOME;

            }
            break;
        }

        case AUTO_KEY_LONG_PRESS:
            if((_eOpMode == BASE_MODES::MANUAL_MODE)
                && (_manualMode.GetManualModeState() == BASE_MODES::STATE_MANUAL_GEN_OFF))
            {
                _eOpMode = BASE_MODES::TEST_MODE;
                GoToHomeScreen();
                _manualMode.SetGCUOperatingMode(_eOpMode);
            }
            break;

        default:
            break;
    }
}

void MON_UI::prvConfigureScreenEnable()
{
    uint8_t u8Screen;
    for(u8Screen = DISP_HOME ; u8Screen<DISP_J1939_RX_LAST; u8Screen++)
    {
        _ArrScreenEnDs[u8Screen] = false;
        switch(u8Screen)
        {
            case DISP_GEN_LN_VOLTAGE    :
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT)== CFGZ::CFGZ_ENABLE)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_GEN_PF       :
            case DISP_GEN_CUMU_POWER    :
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT)== CFGZ::CFGZ_ENABLE)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_HISTOGRAM:
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_HISTOGRAM)== CFGZ::CFGZ_ENABLE)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_GEN_KW       :
            case DISP_GEN_KVA      :
            case DISP_GEN_KVAR     :
            case DISP_GEN_CURRENT      :
                /*Load Screens will be enabled if Alternator present is enabled OR (when Mains Mon is enabled
                 * AND mains contactor is configured AND CT location is on load cable.)*/
                _ArrScreenEnDs[u8Screen] = ((_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT)== CFGZ::CFGZ_ENABLE)
                        ||((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)
                                && ((_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR)!= ACT_Manager:: ACT_NOT_CONFIGURED)
                                        || (_hal.actuators.GetActStatus(ACTUATOR::ACT_OPEN_MAINS_OUT)!= ACT_Manager:: ACT_NOT_CONFIGURED))
                                        && (_cfgz.GetCFGZ_Param(CFGZ::ID_CT_LOCATION) == CFGZ::ON_LOAD_CABLE)));
                break;

            case DISP_AUTO_EXERCISE_1:
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_EN) == CFGZ::CFGZ_ENABLE)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_AUTO_EXERCISE_2:
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_EN) == CFGZ::CFGZ_ENABLE)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_SHELTER_TEMP:
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_SCREEN_AUX_1:
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_SCREEN_AUX_2:
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_S2_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_SCREEN_AUX_3:
                if( (_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1) ||
                        (_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2) )
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_SCREEN_AUX_4:
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)  ||
                        (_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_SCREEN_DIG_IO_STATUS:
            case DISP_HOME            :
            case DISP_PRODUCT_INFO :
            case DISP_BATTERY_VOLTAGE  :
            case DISP_ENGINE_SPEED     :
            case DISP_ENGINE_RUN_TIME   :
                _ArrScreenEnDs[u8Screen] = true;
                break;

            case DISP_SITE_BATT :
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_BATT_MON_EN) == CFGZ::CFGZ_ENABLE))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_BTS_RUN_TIME :
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_BATT_MON_EN) == CFGZ::CFGZ_ENABLE))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_TAMPERED_RUNHRS :
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_WAVE_DETECT_EN) == CFGZ::CFGZ_ENABLE))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_CONTACTOR_STATUS :
                if((_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_GEN_CONTACTOR)!= ACT_Manager:: ACT_NOT_CONFIGURED)
                        ||(_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR)!= ACT_Manager:: ACT_NOT_CONFIGURED)
                        ||(_hal.actuators.GetActStatus(ACTUATOR::ACT_OPEN_GEN_OUT)!= ACT_Manager:: ACT_NOT_CONFIGURED)
                        ||(_hal.actuators.GetActStatus(ACTUATOR::ACT_OPEN_MAINS_OUT)!= ACT_Manager:: ACT_NOT_CONFIGURED))
                {
                    _ArrScreenEnDs[u8Screen] = false;
                }
                else
                {
                    _ArrScreenEnDs[u8Screen] = false; /* contactor screen will be always off */
                }
                break;

            case DISP_MAINS_LN_VOLTAGE  :
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN)
                        == CFGZ::CFGZ_ENABLE)
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_MAINS_RUN_TIME:
            case DISP_MAINS_CUMU_POWER:
                _ArrScreenEnDs[u8Screen] = (_EngineMon.GetContactorLoadStatus() ==ENGINE_MONITORING::LOAD_ON_MAINS);
                break;

            case DISP_COOLENT_TEMP     :
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                        || IS_ENG_TEMP_J1939_CONFIGURED())
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_OIL_PRESSURE     :
                if(((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                        ||((_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)))
                        || IS_ENG_LOP_J1939_CONFIGURED())
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_BALANCE_FUEL     :
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_ENGINE_TYPE:
                _ArrScreenEnDs[u8Screen] = true;
                break;

            case DISP_LAMP_ICONS:
            case DISP_EXAFTERTREAT_ICONS:
            case DISP_PROPB62_PGN_65378  :
            case DISP_LFE_PGN_65266      :
            case DISP_EEC2_PGN_61443     :
            case DISP_AMB_PGN_65269      :
            case DISP_HOURS_PGN_65253    :
            case DISP_VEP1_PGN_65271     :
            case DISP_DD_PGN_65276       :
            case DISP_WFI_PGN_65279      :
            case DISP_LFC_PGN_65257      :
            case DISP_EEC4_PGN_65214     :
            case DISP_LFI_PGN_65203      :
            case DISP_IC2_PGN_64976      :
            case DISP_EOI_PGN_64914      :
            case DISP_ET2_PGN_65188      :
            case DISP_EEC3_PGN_65247_1   :
            case DISP_EEC3_PGN_65247_2   :
            case DISP_S2_PGN_65166       :
            case DISP_EFL_P2_PGN_65243   :
            case DISP_SHUTDOWN_PGN_65252 :
            case DISP_EFG1_PGN_61450     :
            case DISP_DPF1S_PGN_64796    :
            case DISP_ET1_PGN_65262_1    :
            case DISP_ET1_PGN_65262_2    :
            case DISP_EEC1_PGN_61444_1   :
            case DISP_EEC1_PGN_61444_2   :
            case DISP_EFL_P1_PGN_65263_1 :
            case DISP_EFL_P1_PGN_65263_2 :
            case DISP_IC1_PGN_65270_1    :
            case DISP_IC1_PGN_65270_2    :
            case DISP_AT1IG1_PGN_61454   :
            case DISP_A1DOC_PGN_64800    :
            case DISP_AT1IG2_PGN_64948   :
            case DISP_AT1IMG_PGN_64946   :
            case DISP_AT1OG1_PGN_61455   :
            case DISP_A1SCRDSI1_PGN_61475:
            case DISP_A1SCRDSI2_PGN_64833:
            case DISP_A1SCRDSR1_PGN_61476:
            case DISP_A1SCREGT1_PGN_64830:
            case DISP_AT2IG1_PGN_61456   :
            case DISP_AT2OG1_PGN_61457   :
            case DISP_A2SCRDSI2_PGN_64827:
            case DISP_A2SCRDSI1_PGN_61478:
            case DISP_A2SCRDSR1_PGN_61479:
            case DISP_A2SCREGT1_PGN_64824:
            case DISP_AT1OG2_PGN_64947   :
            case DISP_AT1S2_PGN_64697    :
            case DISP_AT1S_PGN_64891     :
            case DISP_AT1T1I_PGN_65110   :
            case DISP_HATZ_CCVS_PGN_65265:
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) )
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;


            case DISP_PROPB57_PGN_65367  :
            case DISP_PROPB32_PGN_65330  :
            case DISP_PROPB5E_PGN_65374_1:
            case DISP_PROPB5E_PGN_65374_2:
            case DISP_PROPB5E_PGN_65374_3:
            case DISP_PROPB5E_PGN_65374_4:
            case DISP_PROPB5E_PGN_65374_5:
            case DISP_PROPB5E_PGN_65374_6:
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) == CFGZ::ENG_KUBOTA))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_IVECO_ENGINE_STATUS:
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) == CFGZ::ENG_IVECO))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_EDC4_CAN_STATUS    :
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) == CFGZ::ENG_DEUTZ_EMR))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_HATZ_PROPB_PHYS_PGN_65280:
            case DISP_AI_PGN_65237:
            case DISP_ETC5_PGN_65219:
            case DISP_OII_PGN_64554:
            case DISP_PROSTOUT_PGN_65364_1:
            case DISP_PROSTOUT_PGN_65364_2:
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) == CFGZ::ENG_HATZ))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_IT1_PGN_65154  :
            case DISP_GFP_PGN_65163  :
            case DISP_IMI1_PGN_65190 :
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) == CFGZ::ENG_YUCHAI_YCGCU))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_FD1_PGN_65213   :
            case DISP_DLCC1_PGN_64775 :
            case DISP_GFC_PGN_65199 :
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) == CFGZ::ENG_WECHAI))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            case DISP_EPT1_PGN_65187:
            case DISP_EPT2_PGN_65186:
            case DISP_EPT3_PGN_65185:
            case DISP_EPT4_PGN_65184:
            case DISP_EPT5_PGN_65183:
            case DISP_ET4_PGN_64870:
            case DISP_TCI4_PGN_65176:
            case DISP_EFL_P12_PGN_64735:
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) == CFGZ::ENG_PERKINS_ADAM4))
                {
                    _ArrScreenEnDs[u8Screen] = true;
                }
                break;

            default: break;
        }
    }
}

//extern volatile flash_event_t WriteEvent, EraseEvent;
void MON_UI::PrintErrorScreen()
{
    _Disp.ClearScreen();
    _Disp.drawRectangle();
//    char arrTemp[32];  //Local variable for storing the sprintf output

    _Disp.gotoxy(GLCD_X(64),GLCD_Y(10));
    _Disp.printStringCenterAligned((char *)"Configuration Error",FONT_VERDANA);
    _Disp.gotoxy(GLCD_X(64),GLCD_Y(25));
    _Disp.printStringCenterAligned((char *)"Reset The Device",FONT_VERDANA);
    _Disp.gotoxy(GLCD_X(64),GLCD_Y(40));
    _Disp.printStringCenterAligned((char *)"And Reconfigure It",FONT_VERDANA);
//    _Disp.gotoxy(GLCD_X(5),GLCD_Y(50));
//    sprintf(arrTemp,"%s  %d     %s %d","Write:", WriteEvent, "Erase:", EraseEvent);
//    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

    while(1)
    {
        _hal.ObjGlcd.Update();
        (void)R_IWDT_Refresh(&g_wdt0_ctrl);
    }
}

void MON_UI::prvDisplayMonScreen()
{
    char* ArrImage;
    //Every screen need this code.
    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.drawHorizontalLine( GLCD_X(0),   GLCD_Y(19),  GLCD_Y(128));

    if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)
            ||(_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
    {
        _Disp.drawVerticalLine  ( GLCD_X(22),  GLCD_Y(0),   GLCD_Y(19));
        //Screen logo
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

        if((_stScreenNo != DISP_MAINS_LN_VOLTAGE)&&(eMonScreenType == MON_SCREEN_NORMAL))
        {    //Print screen name
            _Disp.gotoxy(GLCD_X(75),GLCD_Y(5));
            _Disp.printStringCenterAligned((char *)strMonScreens[_cfgz.GetArrLanguageIndex()][_stScreenNo],
                    FONT_ARIAL);
        }
    }
    else
    {
        uint8_t u8SizeX, u8SizeY, u8CordinateX, u8CordinateY;
        if((_stScreenNo == DISP_ENGINE_SPEED)||(_stScreenNo == DISP_ENGINE_RUN_TIME))
        {
            prvGetMonImageCoordicates( &u8SizeX, &u8SizeY, &u8CordinateX, &u8CordinateY);
            _Disp.printImage((uint8_t *)u8ImagesChiniMonScreen[_stScreenNo - 5], u8SizeX, u8SizeY, u8CordinateX, u8CordinateY); // 4 Substracted since Title of aux Sensor S1-S4 doesn't have chinese translation.

        }
        else if((_stScreenNo >= DISP_SCREEN_AUX_1)&&(_stScreenNo <= DISP_SCREEN_DIG_IO_STATUS))
        {
            _Disp.gotoxy(GLCD_X(75),GLCD_Y(5));
            _Disp.printStringCenterAligned((char *)strMonScreens[_cfgz.GetArrLanguageIndex()][_stScreenNo],
                    FONT_ARIAL);
        }
        else if((_stScreenNo != DISP_MAINS_LN_VOLTAGE)&&(eMonScreenType == MON_SCREEN_NORMAL))
        {
            prvGetMonImageCoordicates( &u8SizeX, &u8SizeY, &u8CordinateX, &u8CordinateY);
            _Disp.printImage((uint8_t *)u8ImagesChiniMonScreen[_stScreenNo], u8SizeX, u8SizeY, u8CordinateX, u8CordinateY);
        }
    }

    //Separate variable taken to take min value, because it has one filter inside the function.
    //So it will act when that function is called, so to call the function in update loop
    //two separate variables are taken/
    _f32GenMinFreq = _GCUAlarms.GetMinGenFreq();
    _f32MainsFreq = _GCUAlarms.GetMinMainsFreq();

    if((eMonScreenType == MON_SCREEN_J1939))
    {
        prvJ1939MonScreens();
    }
    else
    {
         prvNormalMonScreens();
    }
}
void MON_UI::prvPrintCANMsgRcvError()
{
    _Disp.printStringCenterAligned((char*)strCANMsgRcvError[_cfgz.GetArrLanguageIndex()], FONT_ARIAL);
//   GLCD_PrintStringCenterAligned((char*)strCANMsgRcvError[_cfgz.GetArrLanguageIndex()], GLCD_ROW(u8RowNum), GLCD_COLUMN(u8ColStart),
//           GLCD_COLUMN(u8ColEnd), GLCD_FONT_ARIAL_BOLD);
}

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

void MON_UI::prvPrintNotAvailable(uint8_t u8RowNum, uint8_t u8ColStart)
{
    _Disp.gotoxy(u8ColStart, u8RowNum);
    _Disp.printStringLeftAligned((char*)NotAvailable, FONT_ARIAL);
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
    _Disp.printStringCenterAligned((char *)(&(gstPgnSubHeadings[_cfgz.GetArrLanguageIndex()][u8RxATPGNNo - (DISP_AT1IG1_PGN_61454 -DISP_MON_SCREEN_LAST-1)].SubHeadings)), FONT_VERDANA);

    for(i = 0; i < (astJ1939Display[u8RxATPGNNo].u8NoOfSPNs); i++)
    {
        _Disp.gotoxy(2, 23 + (i * 10));
        if(_cfgz.GetArrLanguageIndex() == CFGZ::LANGUAGE_SPANISH)
        {
            _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxATPGNNo].stpSPN[i].cs8pSpanishSPNName, FONT_VERDANA);
        }
        else
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
    ps8PGNNameToPrint = (char*)arrPGNAbbreviations[_cfgz.GetArrLanguageIndex()][astJ1939Display[u8RxPGNNo].cu8PGNName- DISP_MON_SCREEN_LAST-1];
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
            if(_cfgz.GetArrLanguageIndex() == CFGZ::LANGUAGE_SPANISH)
            {
                _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[0].cs8pSpanishSPNName, FONT_VERDANA);
            }
            else
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
                    _Disp.printStringLeftAligned((char*)Yes[_cfgz.GetArrLanguageIndex()], FONT_ARIAL);
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
                if(_cfgz.GetArrLanguageIndex() == CFGZ::LANGUAGE_SPANISH)
                {
                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSpanishSPNName, FONT_VERDANA);
                }
                else
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
                if(_cfgz.GetArrLanguageIndex() == CFGZ::LANGUAGE_SPANISH)
                {
                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSpanishSPNName, FONT_VERDANA);
                }
                else
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
                                _Disp.printStringLeftAligned((char*)StrOff[_cfgz.GetArrLanguageIndex()], FONT_ARIAL);
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
                                _Disp.printStringLeftAligned((char*)StrOn[_cfgz.GetArrLanguageIndex()], FONT_ARIAL);
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
                            _Disp.printStringLeftAligned((char*)Yes[_cfgz.GetArrLanguageIndex()], FONT_ARIAL);
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
                if(_cfgz.GetArrLanguageIndex() == CFGZ::LANGUAGE_SPANISH)
                {
                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSpanishSPNName, FONT_VERDANA);
                }
                else
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
                        _Disp.printStringCenterAligned((char*)(gstEngOperatingState[_cfgz.GetArrLanguageIndex()][(uint8_t)_DisplayPGNScreenData[i].f64SpnVal].OperatingState), FONT_VERDANA);
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
                            _Disp.printStringLeftAligned((char*)StrOff[_cfgz.GetArrLanguageIndex()], FONT_ARIAL);
                        }
                        else if(((uint8_t)_DisplayPGNScreenData[i].f64SpnVal == 1) )
                        {
                            _Disp.printStringLeftAligned((char*)StrOn[_cfgz.GetArrLanguageIndex()], FONT_ARIAL);
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
            if(_cfgz.GetArrLanguageIndex() == CFGZ::LANGUAGE_SPANISH)
            {
                _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[0].cs8pSpanishSPNName, FONT_VERDANA);
            }
            else
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

                        _Disp.printStringLeftAligned((char*)gstNcdInduceData[_cfgz.GetArrLanguageIndex()][(uint8_t)_DisplayPGNScreenData[i].f64SpnVal].NcdInduceStrategy, FONT_VERDANA );
                    }
                }
            }
            break;


        default:
            for(i = 0; i < (astJ1939Display[u8RxPGNNo].u8NoOfSPNs); i++)
            {
                _Disp.gotoxy(2, 15 + (i * 11));
                if(_cfgz.GetArrLanguageIndex() == CFGZ::LANGUAGE_SPANISH)
                {
                    _Disp.printStringLeftAligned((char *)astJ1939Display[u8RxPGNNo].stpSPN[i].cs8pSpanishSPNName, FONT_VERDANA);
                }
                else
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
/************************************************************************************************/
unsigned int  MON_UI::prvGetNoOfFractionalDigits(double dNumber)
{
    unsigned int uiDigits = 0;

    while(dNumber!=((int)dNumber))
    {
        uiDigits++;
        dNumber=dNumber*10;
    }
    return uiDigits++;
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


        StrArrayPtr->u8DPInResolution = uiNoOfFractionalDigits;
        StrArrayPtr->SPNStatusErrorNA = _j1939.GetSPNErrorStatus((DATABASE_RX_PGN_LIST_t)u8PGNEnumNo,u8SpnNo);
}
void MON_UI::prvNormalMonScreens()
{
    bool bDisplayMainsLoad = false;
    char arrTemp[32] = {0};  //Local variable for storing the sprintf output
   A_SENSE::SENSOR_RET_t  stTemp;
    uint8_t u8Position;
    uint8_t u8Local;

   if(_manualMode.IsLoadOnMains())
   {
       bDisplayMainsLoad = true;
   }
    switch(_stScreenNo)
    {
        case DISP_HOME:
        {
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)
                    || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
            {
                _Disp.drawHorizontalLine(GLCD_X(0), GLCD_Y(35), GLCD_Y(128));
                _Disp.gotoxy(GLCD_X(3),GLCD_Y(21));
                if(_manualMode.GetGCUState()== MANUAL_MODE::ENGINE_STARTING)
                {
                    sprintf(arrTemp,"%s %d/%d",strGCUStatus[_cfgz.GetArrLanguageIndex()][_manualMode.GetGCUState()],
                            _startStop.GetCrankAttemptNumber(),
                            _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_ATTEMPTS));
                }
                else
                {
                    sprintf(arrTemp,"%s", strGCUStatus[_cfgz.GetArrLanguageIndex()][_manualMode.GetGCUState()]);
                }
                _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                _Disp.gotoxy(GLCD_X(3),GLCD_Y(38));
                if(_manualMode.GetTimerState() > BASE_MODES::TEST_MODE_TIMER)
                {
                    if(((_eOpMode == BASE_MODES::CYCLIC_MODE)
                            &&((_manualMode.GetTimerState() == BASE_MODES::CYCLIC_ON_TIMER)||(_manualMode.GetTimerState() == BASE_MODES::CYCLIC_OFF_TIMER)))
                       || ((_eOpMode == BASE_MODES::BTS_MODE)
                               &&((_manualMode.GetTimerState() == BASE_MODES::BATT_CHARGE_TIMER)||(_manualMode.GetTimerState() == BASE_MODES::SHELTER_TEMP_TIMER))))
                    {
                        sprintf(arrTemp,"%s",strTimerStatus[_cfgz.GetArrLanguageIndex()][_manualMode.GetTimerState()]);
                        _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                    }
                }
                else
                {
                       sprintf(arrTemp,"%s",strTimerStatus[_cfgz.GetArrLanguageIndex()][_manualMode.GetTimerState()]);
                       _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                }

                if(_manualMode.GetTimerState() == BASE_MODES::NO_TIMER_RUNNING)
                {
                    if((_startStop.IsStartIdleActive()) && (!_startStop.IsIdleModeInputConfigured()))
                    {
                          sprintf(arrTemp,"%s","Start Idle Time: ");
                           _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                    }
                    else if(_startStop.IsStopIdleActive())
                    {
                          sprintf(arrTemp,"%s","Stop Idle Time: ");
                           _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                    }
                }

                    if((_manualMode.GetTimerState() != BASE_MODES::NO_TIMER_RUNNING))
                    {
                        if(_manualMode.GetTimerState() == BASE_MODES::COOL_DOWN_TIMER)
                        {
                            sprintf(arrTemp," %d", (int)_manualMode.GetEngCoolDownRemTime());
                        }
                        else if(_manualMode.GetTimerState() ==
                                BASE_MODES::TEST_MODE_TIMER)
                        {
                            sprintf(arrTemp," %d",
                                    (int)((_manualMode.GetTestModeRemTime()/ 60) + 1));
                        }
                        else if((_manualMode.GetTimerState() > BASE_MODES::TEST_MODE_TIMER) &&
                                (_eOpMode == BASE_MODES::CYCLIC_MODE))
                        {
                            sprintf(arrTemp," %d",
                                    (int)(_cyclicMode.GetCyclicModeTime(_manualMode.GetTimerState())));
                        }
                        else if((_manualMode.GetTimerState() > BASE_MODES::TEST_MODE_TIMER) &&
                                (_eOpMode == BASE_MODES::BTS_MODE))
                        {
                            sprintf(arrTemp," %d",
                                    (int)(_BTSMode.GetBTSModeTime(_manualMode.GetTimerState())));
                        }
                        else
                        {
                            sprintf(arrTemp," %d",
                                    (int)_startStop.GetTimersRemainingTime(_manualMode.GetTimerState()));
                        }

                        if(((_manualMode.GetTimerState() == BASE_MODES::PREHEAT_TIMER)
                                &&(_cfgz.GetEngType()==CFGZ::ENG_VOLVO) && _cfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_TO_ECU))
                            || ((_manualMode.GetTimerState() > BASE_MODES::TEST_MODE_TIMER)   && ( _eOpMode != BASE_MODES::BTS_MODE) &&  (_eOpMode != BASE_MODES::CYCLIC_MODE)))
                        {
                            ;// Do nothing
                        }
                        else
                        {
                                _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                        }
                    }
                    else if(_startStop.IsStartIdleActive() && (!_startStop.IsIdleModeInputConfigured()))
                    {
                        sprintf(arrTemp," %d",(uint16_t)_startStop.GetStartIdleRemTime());
                        _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                    }
                    else if(_startStop.IsStopIdleActive())
                    {
                        sprintf(arrTemp," %d",(uint16_t)_startStop.GetStopIdleRemTime());
                        _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                    }



                 _Disp.gotoxy(GLCD_X(3),GLCD_Y(51));

                 if(_startStop.IsStartIdleActive() || _startStop.IsStopIdleActive())
                 {
                    // _Disp.gotoxy(GLCD_X(3),GLCD_Y(51));
                     _Disp.printStringLeftAligned((char *)strIDLMode[START_STOP::IDLE_MODE_ACTIVE],FONT_ARIAL);
                 }
                 else
                 {
                     if(_autoExercise.IsNightModeRestrictOn())
                     {
                         _Disp.printStringLeftAligned((char *)StrNightModeRestrict[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
                     }
                     else if(_eOpMode == BASE_MODES::AUTO_MODE)
                     {
                         if(START_STOP::IsIdleModeActive())
                         {
                             _Disp.printStringLeftAligned((char *)strIDLMode[START_STOP::IDLE_MODE_ACTIVE],FONT_ARIAL);

                         }
                         else
                         {
                             if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN)== CFGZ::CFGZ_ENABLE)
                             {
                                 _Disp.printStringLeftAligned((char *)StrAutoAMF[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
                             }
                             else
                             {
                                 _Disp.printStringLeftAligned((char *)strGCUMode[_cfgz.GetArrLanguageIndex()][ BASE_MODES::AUTO_MODE],FONT_ARIAL);
                             }
                         }
                     }
                     else if(_eOpMode == BASE_MODES::AUTO_EXERCISE_MODE)
                     {
                         if(_autoExercise.GetRunningExeType() ==1)
                         {
                             _Disp.printStringLeftAligned((char *)StrAutoExercise1[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
                         }
                         else if(_autoExercise.GetRunningExeType() ==2)
                         {
                             _Disp.printStringLeftAligned((char *)StrAutoExercise2[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
                         }

                         if(_autoExercise.IsExerciserStarted())
                         {
                             sprintf(arrTemp,"%02d:%02d", (uint8_t)(_autoExercise.GetExerciserTime()/60),
                                     (uint8_t)(_autoExercise.GetExerciserTime()%60));
                             _Disp.gotoxy(GLCD_X(126),GLCD_Y(51));
                             _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                         }
                     }
                     else
                     {
                         _Disp.printStringLeftAligned((char *)strGCUMode[_cfgz.GetArrLanguageIndex()][_eOpMode],FONT_ARIAL);
                     }
                 }


            }
            else
            {
                prvChiniHomScreen();
            }
        }
        break;
        case DISP_CONTACTOR_STATUS:
        {
            if(_manualMode.IsGenContactorClosed()&&((_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_GEN_CONTACTOR)
                    != ACT_Manager:: ACT_NOT_CONFIGURED) || (_hal.actuators.GetActStatus(ACTUATOR::ACT_OPEN_GEN_OUT)
                            != ACT_Manager:: ACT_NOT_CONFIGURED)))
            {
                prvPrintContactorImage(BASE_MODES::GEN_CONTACTOR_CLOSED);
            }
            else if(_manualMode.IsMainsContactorClosed()&&((_hal.actuators.GetActStatus(
                    ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR)
                    != ACT_Manager:: ACT_NOT_CONFIGURED) || (_hal.actuators.GetActStatus(ACTUATOR::ACT_OPEN_MAINS_OUT)
                            != ACT_Manager:: ACT_NOT_CONFIGURED)))
            {
                prvPrintContactorImage(BASE_MODES::MAINS_CONTACTOR_CLOSED);
            }
            else
            {
                prvPrintContactorImage(BASE_MODES::BOTH_CONTACTOR_OPEN);
            }
        }
        break;

        case DISP_PRODUCT_INFO:
        {
            prvProductInfo();
        }
        break;

        case DISP_SCREEN_DIG_IO_STATUS:
        {
            prvDigIOStatus();

        }
        break;

        case DISP_AUTO_EXERCISE_1:
        case DISP_AUTO_EXERCISE_2:
            prvPrintExerciser(_stScreenNo);
            break;

        case DISP_GEN_LN_VOLTAGE:
        {
            prvPrintVtgFreqData(GENSET,
                          _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE));
        }
        break;

        case DISP_GEN_KW:
        {
            if(bDisplayMainsLoad)
            {
                prvPrintPower(ACTIVE,
                      _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE), MAINS);
            }
            else
            {
                prvPrintPower(ACTIVE,
                          _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE), GENSET);
            }
        }
        break;
        case DISP_GEN_KVA:
        {
            if(bDisplayMainsLoad)
            {
                prvPrintPower(APARENT,
                          _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE),MAINS);
            }
            else
            {
                prvPrintPower(APARENT,
                          _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE),GENSET);
            }

        }
        break;
        case DISP_GEN_KVAR:
        {
            if(bDisplayMainsLoad)
            {
                prvPrintPower(REACTIVE,
                          _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE),MAINS);
            }
            else
            {
                prvPrintPower(REACTIVE,
                        _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE),GENSET);
            }
        }
        break;
        case DISP_GEN_PF:
        {
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) ==
                    CFGZ::CFGZ_3_PHASE_SYSTEM)
            {
                _Disp.drawVerticalLine(GLCD_X(50), GLCD_Y(19), GLCD_Y(64));
                sprintf(arrTemp,"%0.02f",
                        abs(_hal.AcSensors.GENSET_GetDispAveragePowerFactor()));
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
            }else if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) ==
                    CFGZ::CFGZ_SPLIT_PHASE)
            {
                _Disp.drawVerticalLine(GLCD_X(50), GLCD_Y(19), GLCD_Y(64));
                sprintf(arrTemp,"%0.02f",
                       abs(_hal.AcSensors.GENSET_GetDispAveragePowerFactor()));
                _Disp.gotoxy(GLCD_X(22),GLCD_Y(33));
                _Disp.printStringCenterAligned((char *)arrTemp,FONT_ARIAL);
                _Disp.gotoxy(GLCD_X(23),GLCD_Y(48));
                _Disp.printStringCenterAligned((char *)StrPF,FONT_ARIAL);
                u8Position = 28;
                for(u8Local = R_PHASE; u8Local < B_PHASE ; u8Local++)
                {
                   sprintf(arrTemp,"%s-%s    %0.2f",(char *)StrPF,(char *)strPhase[u8Local],
                       abs(_hal.AcSensors.GENSET_GetDispPowerFactor((PHASE_t)u8Local)));
                   _Disp.gotoxy(GLCD_X(55),GLCD_Y(u8Position));
                   _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
                   u8Position = u8Position + 18;
                }
            }
            else
            {
                sprintf(arrTemp,"%s-%s    %0.2f",(char *)StrPF,
                   (char *)strPhase[R_PHASE],
                _hal.AcSensors.GENSET_GetDispPowerFactor((PHASE_t)R_PHASE));
                _Disp.gotoxy(GLCD_X(45),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
            }
        }
        break;

        case DISP_HISTOGRAM:
            prvPrintHystogram();
            break;
        case DISP_GEN_CURRENT:
        {
            u8Position = 22;

            SOURCE_TYPE_t eSysType=GENSET;
            _pGET_VAL_t ArrGetCurrentVal[TYPE_LAST]=
            {

                      &AC_SENSE::GENSET_GetCurrentAmps,

                      &AC_SENSE::MAINS_GetCurrentAmps,

            };

            if(bDisplayMainsLoad)
            {
                eSysType=MAINS;
            }

            if(((_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE)
                    == CFGZ::CFGZ_3_PHASE_SYSTEM) && (!bDisplayMainsLoad))
                    ||
               ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE)
                                == CFGZ::CFGZ_3_PHASE_SYSTEM) && (bDisplayMainsLoad))
                    )
            {
                for(u8Local = R_PHASE; u8Local < PHASE_END ; u8Local++)
                {
                    _Disp.gotoxy(GLCD_X(20),GLCD_Y(u8Position));
                    _Disp.printStringLeftAligned((char *)strPhase[u8Local],
                                                 FONT_VERDANA);
                    _Disp.gotoxy(GLCD_X(90),GLCD_Y(u8Position));
                    sprintf(arrTemp,"%.0f",
                            ((&_hal.AcSensors)->*ArrGetCurrentVal[eSysType])((PHASE_t)u8Local));
                    _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                    _Disp.gotoxy(GLCD_X(92),GLCD_Y(u8Position));
                    _Disp.printStringLeftAligned((char *)StrA, FONT_VERDANA);
                    u8Position = u8Position + 15;
                }
            }
            else if(((_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE)
                    == CFGZ::CFGZ_SPLIT_PHASE) && (!bDisplayMainsLoad))
                    ||
               ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE)
                                == CFGZ::CFGZ_SPLIT_PHASE) && (bDisplayMainsLoad))
                    )
            {
                u8Position = 28;
                for(u8Local = R_PHASE; u8Local < B_PHASE ; u8Local++)
                {
                    _Disp.gotoxy(GLCD_X(25),GLCD_Y(u8Position));
                    _Disp.printStringLeftAligned((char *)strPhase[u8Local],
                                                 FONT_VERDANA);
                    _Disp.gotoxy(GLCD_X(90),GLCD_Y(u8Position));
                    sprintf(arrTemp,"%.1f",
                            ((&_hal.AcSensors)->*ArrGetCurrentVal[eSysType])((PHASE_t)u8Local));
                    _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                    _Disp.gotoxy(GLCD_X(92),GLCD_Y(u8Position));
                    _Disp.printStringLeftAligned((char *)StrA, FONT_VERDANA);
                    u8Position = u8Position + 18;
                }
            }
            else
            {
                _Disp.gotoxy(GLCD_X(20),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)strPhase[R_PHASE],
                                             FONT_VERDANA);
                _Disp.gotoxy(GLCD_X(90),GLCD_Y(35));
                sprintf(arrTemp,"%0.1f",
                        ((&_hal.AcSensors)->*ArrGetCurrentVal[eSysType])((PHASE_t)R_PHASE));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                _Disp.gotoxy(GLCD_X(92),GLCD_Y(35));
                _Disp.printStringLeftAligned((char *)StrA, FONT_VERDANA);
            }
        }
        break;
        case DISP_GEN_CUMU_POWER:
        {
            _Disp.printImage((uint8_t *)u8ArrDollar, 4, 32, 26, 7);
            _pGetdoubleValCommon_t ArrGetVal[POWER_LAST]=
            {
                &AC_SENSE::GENSET_GetTotalActiveEnergySinceInitWH,
                &AC_SENSE::GENSET_GetTotalApparentEnergySinceInitVAH,
                &AC_SENSE::GENSET_GetTotalReactiveEnergySinceInitVARH
            };

//            _pGetdoubleValCommon_t ArrGetVal1[POWER_LAST]=
//            {
//                &AC_SENSE::GetGensetTotalActiveEnergy,
//                &AC_SENSE::GetGensetTotalApparentEnergy,
//                &AC_SENSE::GetGensetTotalReactiveEnergy
//            };
//
//            _pGetdoubleValCommon_t ArrGetVal2[POWER_LAST]=
//            {
//                &AC_SENSE::GetGensetkWEnergyOffset,
//                &AC_SENSE::GetGensetkVAEnergyOffset,
//                &AC_SENSE::GetGensetkVAREnergyOffset
//            };

            u8Position = 22;
            for( u8Local = ACTIVE; u8Local < POWER_LAST ; u8Local++)
            {
//                sprintf(arrTemp,"   %0.1lf",
//                        ((&_hal.AcSensors)->*ArrGetVal1[u8Local])() /1000);
//                _Disp.gotoxy(GLCD_X(28),GLCD_Y(u8Position));
//                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
//                sprintf(arrTemp,"   %0.1lf",
//                        ((&_hal.AcSensors)->*ArrGetVal2[u8Local])() /1000);
//                _Disp.gotoxy(GLCD_X(65),GLCD_Y(u8Position));
//                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                sprintf(arrTemp,"   %0.1lf",
                        ((&_hal.AcSensors)->*ArrGetVal[u8Local])() /1000);
                _Disp.gotoxy(GLCD_X(97),GLCD_Y(u8Position));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                _Disp.gotoxy(GLCD_X(99),GLCD_Y(u8Position));
                _Disp.printStringLeftAligned((char *)strEnergy[u8Local],
                                             FONT_VERDANA);
                u8Position = u8Position + 15;
            }
        }
        break;

        case DISP_MAINS_LN_VOLTAGE:
        {
            prvPrintVtgFreqData(MAINS,
                             _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE));
        }
        break;

        case DISP_MAINS_RUN_TIME:

            _Disp.gotoxy(GLCD_X(124),GLCD_Y(35));
            sprintf(arrTemp,"%ldhrs  %dmin ",
                    ( _EngineMon.GetMainsRunTimeMin()/60),
                    (uint8_t)(_EngineMon.GetMainsRunTimeMin()%60) );
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
            _Disp.printImage((uint8_t *)u8ArrEngineHours, 4, 32, 26, 7);
            break;

        case DISP_MAINS_CUMU_POWER:
        {
            _Disp.printImage((uint8_t *)u8ArrDollar, 4, 32, 26, 7);
            _pGetdoubleValCommon_t ArrGetVal[POWER_LAST]=
            {
                &AC_SENSE::MAINS_GetTotalActiveEnergySinceInitWH,
                &AC_SENSE::MAINS_GetTotalApparentEnergySinceInitVAH,
                &AC_SENSE::MAINS_GetTotalReactiveEnergySinceInitVARH
            };

            u8Position = 22;
            for( u8Local = ACTIVE; u8Local < POWER_LAST ; u8Local++)
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
        }
        break;

        case DISP_BATTERY_VOLTAGE:
        {

            if(_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_CHINSESE)
            {
                _Disp.printImage((uint8_t *)gau8ChineseEngineBattery, 8, 14, 25, 10);
            }
            else
            {
                _Disp.printImage((uint8_t *)u8ArrBattery, 4, 32, 26, 2);
                _Disp.gotoxy(GLCD_X(40),GLCD_Y(26));
                _Disp.printStringLeftAligned((char *)"ENG BATT",FONT_VERDANA);
            }
            _Disp.gotoxy(GLCD_X(112),GLCD_Y(26));

            if(_j1939.IsCommunicationFail()&& _cfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_VOLT_FROM_ENG))
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


            _Disp.gotoxy(GLCD_X(40),GLCD_Y(47));
            _Disp.printStringLeftAligned((char *)"CA VOLT",FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(112),GLCD_Y(47));
            sprintf(arrTemp,"%0.1f",
                    _hal.AnalogSensors.GetFilteredChargingAltVolts());
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
            _Disp.gotoxy(GLCD_X(116),GLCD_Y(47));
            _Disp.printStringLeftAligned((char*)"V",FONT_VERDANA);

        }
        break;

        case DISP_SITE_BATT:
        {
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_CHINSESE)
            {
               _Disp.printImage((uint8_t *)gau8ChineseSiteBattery, 8, 13, 35, 10);
            }
            else
            {
                _Disp.gotoxy(GLCD_X(48),GLCD_Y(32));
                sprintf(arrTemp,"SITE BATT");
                _Disp.printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
            }
            _Disp.gotoxy(GLCD_X(88),GLCD_Y(32));
            if(_hal.AnalogSensors.GetFilteredVBTSbattVolts() > 0.3F) //To avoid display of non zero values if the BTS connections are open(60V is mapped to almost 1200mV,So ADC fluctuates
            {
                sprintf(arrTemp,"%0.1f V",_hal.AnalogSensors.GetFilteredVBTSbattVolts());
            }
            else
            {
                sprintf(arrTemp,"%0.1f V",0.0F);
            }
            _Disp.printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        }
        break;

        case DISP_BTS_RUN_TIME:
        {
            _Disp.printImage((uint8_t *)u8ArrEngineHours, 4, 32, 26, 7);
//            if(_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_CHINSESE)
//            {
//               // _Disp.printImage((uint8_t *)gau8ChineseEngineBattery, 8, 14, 22, 3);  //Yet to add image in const_ui
//            }
//            else
//            {
                _Disp.gotoxy(GLCD_X(120),GLCD_Y(35));
                sprintf(arrTemp,"%ld Hrs  %d min ",(_EngineMon.GetBTSRunTimeMin()/60),(uint8_t)(_EngineMon.GetBTSRunTimeMin()%60));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
//            }
        }
        break;
        case DISP_TAMPERED_RUNHRS:
        {
            _Disp.printImage((uint8_t *)u8ArrEngineHours, 4, 32, 26, 7);
//            if(_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_CHINSESE)
//            {
//               // _Disp.printImage((uint8_t *)gau8ChineseEngineBattery, 8, 14, 22, 3);  //Yet to add image in const_ui
//            }
//            else
            {
                _Disp.gotoxy(GLCD_X(120),GLCD_Y(28));
                sprintf(arrTemp,"%ld Hrs %d min",(_EngineMon.GetTamperedRunTimeMin()/60), (uint8_t)(_EngineMon.GetTamperedRunTimeMin()%60));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(120),GLCD_Y(42));

                sprintf(arrTemp,"%0.1f kWh",(_hal.AcSensors.GENSET_GetTotalTamperedActiveEnergySinceInitWH()) /1000);

                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
            }
        }
        break;

        case DISP_COOLENT_TEMP:
        {

            stTemp =_GCUAlarms.GetSelectedTempSensVal();
            _Disp.printImage((uint8_t *)u8ArrEngineTemp, 4, 32, 26, 7);

          _Disp.gotoxy(GLCD_X(115),GLCD_Y(33));

          if(_j1939.IsCommunicationFail()&& _cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG))
          {
              _Disp.printStringRightAligned((char *)"NA",FONT_ARIAL);
          }
          else
          {
              prvPrintSensorStatus(stTemp,(char*)"`C", INTEGER_TYPE);
              if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
              {

                 sprintf(arrTemp,"%d",(int16_t)(
                         (stTemp.stValAndStatus.f32InstSensorVal
                          *DEG_F_FACTOR1)
                          + DEG_F_FACTOR2 ));
                 _Disp.gotoxy(GLCD_X(93),GLCD_Y(42));
                 _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                 _Disp.gotoxy(GLCD_X(95),GLCD_Y(42));
                 _Disp.printStringLeftAligned((char*)"`F",FONT_VERDANA);
              }
          }

        }
        break;

        case DISP_OIL_PRESSURE:
        {
            _Disp.printImage((uint8_t *)u8ArrOilPressure, 4, 32, 26, 3);
            stTemp = _GCUAlarms.GetLOPSensorVal();

            _Disp.gotoxy(GLCD_X(115),GLCD_Y(33));

            if(_j1939.IsCommunicationFail() && _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG))
            {
                _Disp.printStringRightAligned((char *)"NA",FONT_ARIAL);
            }
            else
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
                    sprintf(arrTemp,"%d",(uint16_t)(
                            stTemp.stValAndStatus.f32InstSensorVal
                            * PSI_CONVERSION));
                    _Disp.gotoxy(GLCD_X(93),GLCD_Y(42));
                    _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                    _Disp.gotoxy(GLCD_X(94),GLCD_Y(42));
                    _Disp.printStringLeftAligned((char*)"psi",FONT_VERDANA);
//                    _Disp.printImage((uint8_t *)u8ArrOilPressure, 4, 32, 26, 7);
                }
                else
                {
                    prvPrintSensorFaultStatus(stTemp , 78, 37);
                }
            }
        }
        break;
        case DISP_BALANCE_FUEL:
        {

            stTemp =_hal.AnalogSensors.GetSensorValue(
                            AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);

            _Disp.printImage((uint8_t *)u8ArrFuel, 4, 32, 26, 7);
            prvPrintSensorStatus(stTemp,(char*)"%", INTEGER_TYPE);
            if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
            {
                stTemp.stValAndStatus.f32InstSensorVal =
                        stTemp.stValAndStatus.f32InstSensorVal
                        * _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_TANK_CAPACITY)/100;


                    sprintf(arrTemp,"%d",
                            (uint16_t)((round)(stTemp.stValAndStatus.f32InstSensorVal)));
                    _Disp.gotoxy(GLCD_X(93),GLCD_Y(42));
                    _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                    _Disp.gotoxy(GLCD_X(94),GLCD_Y(42));
                    _Disp.printStringLeftAligned((char*)"Liters",FONT_VERDANA);

                //_Disp.printImage((uint8_t *)u8ArrFuel, 4, 32, 26, 7);

            }
        }
        break;

        case DISP_SHELTER_TEMP:
        {
            _Disp.printImage((uint8_t *)gau8LIShelterTemp, 4, 32, 26, 7);
            _Disp.gotoxy(GLCD_X(110),GLCD_Y(33));

            stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_SHELTER_TEMPERATURE);

            prvPrintSensorStatus(stTemp,(char*)"`C", INTEGER_TYPE);
            if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
            {
               sprintf(arrTemp,"%d",(int16_t)(
                       (stTemp.stValAndStatus.f32InstSensorVal
                        *DEG_F_FACTOR1)
                        + DEG_F_FACTOR2 ));
               _Disp.gotoxy(GLCD_X(93),GLCD_Y(42));
               _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
               _Disp.gotoxy(GLCD_X(95),GLCD_Y(42));
               _Disp.printStringLeftAligned((char*)"`F",FONT_VERDANA);
            }
        }
        break;

        case DISP_SCREEN_AUX_1:
        {
            // S1 as s1 Analog sensor
            stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S1_SENSOR);
            prvPrintSensorStatus(stTemp,(char*)"", FLOAT_TYPE , 80 , 37);

        }
        break;
        case DISP_SCREEN_AUX_2:
        {

             stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S2_SENSOR);
             prvPrintSensorStatus(stTemp,(char*)"", FLOAT_TYPE , 80 , 37);
        }
        break;
        case DISP_SCREEN_AUX_3:
        {
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
            {
                // S3 as 4-20 mA sensor
                stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S3_4_20_SENSOR);
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
            {
                // S3 as 0-5 V Sensor
                stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S3_0_5_SENSOR);
            }

            if(stTemp.eStatus == A_SENSE::SENSOR_NOT_CONFIGRUED)
            {
               _Disp.gotoxy(GLCD_X(80),GLCD_Y(37));
               _Disp.printStringLeftAligned((char*)StrNotConfigured,FONT_VERDANA);
            }
            else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
            {
                sprintf(arrTemp,"%0.1f",stTemp.stValAndStatus.f32InstSensorVal);
                _Disp.gotoxy(GLCD_X(80),GLCD_Y(37));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
            }
            else
            {
                prvPrintSensorFaultStatus(stTemp , 80 ,37);
            }
        }
        break;
        case DISP_SCREEN_AUX_4:
        {
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
            {
                // S4 as 4-20 mA sensor
                stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S4_4_20_SENSOR);
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
            {
                // S4 as 0-5 V Sensor
                stTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S4_0_5_SENSOR);
            }

            if(stTemp.eStatus == A_SENSE::SENSOR_NOT_CONFIGRUED)
            {
               _Disp.gotoxy(GLCD_X(80),GLCD_Y(37));
               _Disp.printStringLeftAligned((char*)StrNotConfigured,FONT_VERDANA);
            }
            else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
            {
                sprintf(arrTemp,"%0.1f",stTemp.stValAndStatus.f32InstSensorVal);
                _Disp.gotoxy(GLCD_X(80),GLCD_Y(37));
                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
            }
            else
            {
                prvPrintSensorFaultStatus(stTemp, 70, 37);
            }
        }
        break;

        case DISP_ENGINE_SPEED:
        {
            _Disp.printImage((uint8_t *)u8ArrEngineSpeed, 4, 32, 26, 7);
            _Disp.gotoxy(GLCD_X(94),GLCD_Y(37));
            if(_j1939.IsCommunicationFail() && _cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_SPEED_FROM_ENG))
            {
                _Disp.printStringRightAligned((char *)"NA",FONT_ARIAL);
            }
            else
            {
                sprintf(arrTemp,"%d ",(uint16_t) _GCUAlarms.GetSpeedValue());
                _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                _Disp.gotoxy(GLCD_X(95),GLCD_Y(37));
                _Disp.printStringLeftAligned((char*)"RPM",FONT_VERDANA);
            }
        }
        break;
        case DISP_ENGINE_RUN_TIME:
        {
            _Disp.gotoxy(GLCD_X(124),GLCD_Y(22));
            if(_j1939.IsCommunicationFail() && _cfgz.GetCFGZ_Param(CFGZ::ID_RUNNING_HOURS_FROM_ENG))
            {
                _Disp.printStringRightAligned((char *)"NA",FONT_ARIAL);
            }
            else
            {
                sprintf(arrTemp,"%ldhrs  %dmin ",
                        ( _GCUAlarms.GetSelectedEngRunMin()/60),
                        (uint8_t)(_GCUAlarms.GetSelectedEngRunMin()%60) );
                _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
            }

            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)
                    || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
            {
                _Disp.printImage((uint8_t *)u8ArrEngineHours, 4, 32, 26, 7);

                _Disp.gotoxy(GLCD_X(40),GLCD_Y(37));
                _Disp.printStringLeftAligned((char *)StrStarts[_cfgz.GetArrLanguageIndex()],FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(40),GLCD_Y(52));
                _Disp.printStringLeftAligned((char *)StrTrips[_cfgz.GetArrLanguageIndex()],FONT_VERDANA);
            }
            else
            {
                _Disp.printImage((uint8_t *)gau8ChineseNumOfStarts, 8, 14, 32, 12);
                _Disp.printImage((uint8_t *)gau8ChineseNumOfTrips, 8, 14, 48, 12);
            }

            _Disp.gotoxy(GLCD_X(90),GLCD_Y(37));
            sprintf(arrTemp,"%d ",  (uint16_t)_EngineMon.GetEngineNoOfStarts() );
            _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(90),GLCD_Y(52));
            sprintf(arrTemp,"%d ",  (uint16_t)_EngineMon.GetEngineNoOfTrips() );
            _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
        }
        break;
        case DISP_LAMP_ICONS:
            prvBuildLampIconScreen();
            break;
        case DISP_EXAFTERTREAT_ICONS:
            prvBuildExhaustIconScreen();
            break;
        case DISP_ENGINE_TYPE:
        {
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE))
            {
                _Disp.gotoxy(GLCD_X(4),GLCD_Y(40));
                _Disp.printStringLeftAligned((char *)StrEICViewMessage[_cfgz.GetArrLanguageIndex()],FONT_VERDANA);

                _Disp.gotoxy(GLCD_X(65),GLCD_Y(25));
                sprintf(arrTemp,"%s ",(char *)StrEngineType[_cfgz.GetArrLanguageIndex()][_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE)]);
                _Disp.printStringCenterAligned((char *)arrTemp,FONT_ARIAL);
            }
            else
            {
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)
                        || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
                {
                    _Disp.gotoxy(GLCD_X(65),GLCD_Y(30));
                    sprintf(arrTemp,"%s ",(char *)StrEngineType[_cfgz.GetArrLanguageIndex()][CFGZ::ENG_CONVENTIONAL]);
                    _Disp.printStringCenterAligned((char *)arrTemp,FONT_ARIAL);
                }
                else
                {
                    _Disp.printImage((uint8_t *)gau8ChineseNone,  3, 12, 30, 50);
                }

            }

        }
        break;

        default: break;
    }
}

void MON_UI::prvDisplayBootLogo()
{
    _Disp.printImage((uint8_t *)&u8BmpLogo, 8, 32,16, 32);
}


void MON_UI::prvProductInfo()
{

    RTC::TIME_t CurrentTime;

    char SeriesId[3];
    char arrTemp[32];

    char EngSrNo[12];
    _cfgz.GetEngSrNo(EngSrNo);

    sprintf(arrTemp,"Prod ID:");
    _Disp.gotoxy(GLCD_X(2),GLCD_Y(22));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
    _cfgc.GetSeriesProductID(SeriesId);
    _Disp.gotoxy(GLCD_X(43),GLCD_Y(22));
    sprintf(arrTemp,"S%s%d-R%d.%02d",SeriesId,_cfgc.GetProductID(),
            _cfgc.GetProductRevision(),_cfgc.GetProductVersion());
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
    _Disp.gotoxy(GLCD_X(110),GLCD_Y(22));
    sprintf(arrTemp,"D");
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);


    sprintf(arrTemp,"Eng Sr  :");
    _Disp.gotoxy(GLCD_X(2),GLCD_Y(34));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
    //sprintf(arrTemp, "0000000000");
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


    sprintf(arrTemp,(char*)StrInfoDate[_cfgz.GetArrLanguageIndex()]);
    _Disp.gotoxy(GLCD_X(2),GLCD_Y(47));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
    _hal.ObjRTC.GetTime(&CurrentTime);
    sprintf(arrTemp,"%d %s %d",CurrentTime.u8Day, (char*)StrMonth[_cfgz.GetArrLanguageIndex()][CurrentTime.u8Month - 1],
        CurrentTime.u16Year);
    _Disp.gotoxy(GLCD_X(43),GLCD_Y(47));
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

    _Disp.gotoxy(GLCD_X(102),GLCD_Y(47));
    if(CurrentTime.u8Second % 2)
    {
        sprintf(arrTemp,"%d:%02d",CurrentTime.u8Hour, CurrentTime.u8Minute );
    }
    else
    {
        sprintf(arrTemp,"%d %02d",CurrentTime.u8Hour, CurrentTime.u8Minute );
    }
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
}

void MON_UI::prvDigIOStatus()
{
    char arrTemp[32];

    _Disp.gotoxy(GLCD_X(3),GLCD_Y(26));
    sprintf(arrTemp,"DI:");
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

    for(uint8_t u8Local = _hal.DigitalSensors.DI_A, x = 0; u8Local<= _hal.DigitalSensors.DI_P; u8Local++, x++)
    {
        uint8_t InputState = _hal.DigitalSensors.GetDigInputState(u8Local);
        if(InputState!= DigitalSensor::SENSOR_NOT_CONFIGRUED)
        {
            sprintf(arrTemp , "%d", (uint8_t)(_hal.DigitalSensors.GetDigInputState(u8Local)));
        }
        else
        {
            sprintf(arrTemp , "N");
        }
       _Disp.gotoxy(GLCD_X(17+(7*x)),GLCD_Y(26));
       _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
    }

    _Disp.gotoxy(GLCD_X(3),GLCD_Y(46));
    sprintf(arrTemp,"DO:");
    _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);

    for(uint8_t u8Local =CFGZ::ID_DIG_OP_SOURCE_A , x =0; u8Local<= CFGZ::ID_DIG_OP_SOURCE_G; u8Local=u8Local+2, x++)
    {
       sprintf(arrTemp , "%d", (uint8_t)(_hal.actuators.GetActStatus((ACTUATOR::ACTUATOR_TYPS_t)_cfgz.GetCFGZ_Param((CFGZ::UINT8_PARAMS_t)u8Local))
               == ACT_Manager::ACT_LATCHED));
       _Disp.gotoxy(GLCD_X(24+(7*x)),GLCD_Y(46));
       _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
    }

}

void MON_UI::prvPrintContactorImage(BASE_MODES::CONTACTOR_STATUS_t stContactorStatus)
{
    if(_manualMode.IsMainsContactorConfigured())
    {
        _Disp.printImage((uint8_t *)u8ArrContactor2,3, 16, 40, 9);
        _Disp.printImage((uint8_t *)u8ArrContactor1,6, 16, 40, 39);
        _Disp.printImage((uint8_t *)u8ArrContactor3,3, 16, 40, 93);

        switch(stContactorStatus)
        {
            case BASE_MODES::BOTH_CONTACTOR_OPEN:
            {
                _Disp.printImage((uint8_t *)u8ArrContactor4,1, 10,29, 32);
                _Disp.printImage((uint8_t *)u8ArrContactor4,1, 10,29, 86);
                break;
            }

            case BASE_MODES::GEN_CONTACTOR_CLOSED:
            {
                _Disp.printImage((uint8_t *)u8ArrContactor4,1, 10,29, 32);
                _Disp.printImage((uint8_t *)u8ArrContactor4,1, 10,35, 86);
                break;
            }

            case BASE_MODES::MAINS_CONTACTOR_CLOSED:
            {
                _Disp.printImage((uint8_t *)u8ArrContactor4,1, 10,35, 32);
                _Disp.printImage((uint8_t *)u8ArrContactor4,1, 10,29, 86);
            }
                break;

            default:
                break;
        }
    }
    else
    {

        _Disp.printImage((uint8_t *)u8ArrContactor1,6, 16, 40, 25);
        _Disp.printImage((uint8_t *)u8ArrContactor3,3, 16, 40, 79);


        switch(stContactorStatus)
        {
            case BASE_MODES::BOTH_CONTACTOR_OPEN:
           {
               _Disp.printImage((uint8_t *)u8ArrContactor4,1, 10,29, 72);
               break;
           }

           case BASE_MODES::GEN_CONTACTOR_CLOSED:
           {
               _Disp.printImage((uint8_t *)u8ArrContactor4,1, 10,35, 72);
               break;
           }

          default:
           {
               _Disp.printImage((uint8_t *)u8ArrContactor4,1, 10,29, 72);
           }
               break;
        }
    }

}

bool MON_UI::prvIsValDigitsGreaterThan3(uint16_t u16VoltageVal)
{
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

void MON_UI::prvPrintVtgFreqData(SOURCE_TYPE_t eSource , uint8_t u8AcSystemType)
{
    uint8_t u8Position;
    uint8_t u8Local;

    char arrTemp[32];  //Local variable for storing the sprintf output

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

    //This code is for mains only.
    if(eSource == MAINS)
    {
        _Disp.gotoxy(GLCD_X(75),GLCD_Y(5));

        if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE) ==  CFGZ::LANGUAGE_ENGLISH)
                || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
        {
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)
            {
                _Disp.printStringCenterAligned((char *)StrMainsStatus[_cfgz.GetArrLanguageIndex()][STR_MAINS_NOT_READ],FONT_ARIAL);
            }
            else if(_manualMode.GetPartialHealthyStatus())
            {
                _Disp.printStringCenterAligned((char *)StrMainsStatus[_cfgz.GetArrLanguageIndex()][STR_MAINS_PARTIAL_HEALTHY],FONT_ARIAL);
            }
            else if(_manualMode.GetMainsStatus() == BASE_MODES::MAINS_HELATHY)
            {
                _Disp.printStringCenterAligned((char *)StrMainsStatus[_cfgz.GetArrLanguageIndex()][STR_MAINS_HEALTHY],FONT_ARIAL);
            }
            else if(_GCUAlarms.IsMainsSeqFail() &&
                    (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PH_REVERS_DETECT_EN)
                            == CFGZ::CFGZ_ENABLE)
                    && (_manualMode.GetMainsStatus() == BASE_MODES::MAINS_UNHELATHY))
            {
                _Disp.printStringCenterAligned((char *)StrMainsStatus[_cfgz.GetArrLanguageIndex()][STR_MAINS_SEQ_FAIL],FONT_ARIAL);
            }
            else
            {
                _Disp.printStringCenterAligned((char *)StrMainsStatus[_cfgz.GetArrLanguageIndex()][STR_MAINS_FAILED],FONT_ARIAL);
            }
        }
        else
        {
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_DISABLE)
            {
                _Disp.printImage((uint8_t *)gau8ChineseMainsNotRead, 10, 14, 2, 24);
            }
            else if(_manualMode.GetPartialHealthyStatus())
            {
                _Disp.printImage((uint8_t *)gau8ChineseMainsAbnormal, 9, 16, 2, 30);
            }
            else if(_manualMode.GetMainsStatus() == BASE_MODES::MAINS_HELATHY)
            {
                _Disp.printImage((uint8_t *)gau8ChineseMainsHealthy, 9, 16, 2, 30);
            }
            else if(_GCUAlarms.IsMainsSeqFail() &&
                    (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PH_REVERS_DETECT_EN)
                            == CFGZ::CFGZ_ENABLE)
                    && (_manualMode.GetMainsStatus() == BASE_MODES::MAINS_UNHELATHY))
            {
                _Disp.printImage((uint8_t *)gau8ChineseMainsSequenceFailed, 12, 16, 2, 28);
            }

            else
            {
                _Disp.printImage((uint8_t *)gau8ChineseMainsFailed, 9, 16, 2, 26);
            }
        }
    }

    if(u8AcSystemType == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        _Disp.drawVerticalLine(GLCD_X(70), GLCD_Y(19), GLCD_Y(64));
        u8Position = 22;
        for(u8Local = R_PHASE; u8Local < PHASE_END ; u8Local++)
        {
             _Disp.gotoxy(GLCD_X(2),GLCD_Y(u8Position));
             _Disp.printStringLeftAligned((char *)strPhase[u8Local],FONT_VERDANA);

             if(!prvIsValDigitsGreaterThan3((uint16_t)
                                            ((&_hal.AcSensors)->*ArrGetVtgVal[eSource])((PHASE_t)u8Local)))
             {
                 sprintf(arrTemp,"%d",(uint16_t)
                        ((&_hal.AcSensors)->*ArrGetVtgVal[eSource])((PHASE_t)u8Local));
                 _Disp.gotoxy(GLCD_X(35),GLCD_Y(u8Position));
                 _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                 _Disp.printChar('V',FONT_VERDANA);
             }
             else
             {
                 sprintf(arrTemp,"%.1f",(float)
                        (((&_hal.AcSensors)->*ArrGetVtgVal[eSource])((PHASE_t)u8Local))/1000.0f);
                 _Disp.gotoxy(GLCD_X(35),GLCD_Y(u8Position));
                 _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                 _Disp.printStringLeftAligned((char *)"kV",FONT_VERDANA);
             }

             u8Position = u8Position + 15;
        }
        _Disp.gotoxy(GLCD_X(68),GLCD_Y(37));

        if(eSource == MAINS)
        {
           if(_eOpMode == BASE_MODES::MANUAL_MODE)
           {
                sprintf(arrTemp,"%0.1f", _f32MainsFreq);
           }
           else
           {
               if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PARTIAL_HEALTHY_DETECT_EN))
               {
                   sprintf(arrTemp,"%0.1f", _hal.AcSensors.MAINS_GetMaxFrq());
               }
               else
               {
                   sprintf(arrTemp,"%0.1f", _f32MainsFreq);
               }
           }
        }
        else
        {
          sprintf(arrTemp,"%0.1f",
                  _f32GenMinFreq);
        }

        _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
        _Disp.gotoxy(GLCD_X(52),GLCD_Y(52));
        _Disp.printStringLeftAligned((char *)"Hz",FONT_ARIAL);

        uint8_t ePhPh;
        u8Position = 22;
        for(ePhPh = R_TO_Y; ePhPh < PH_PH_VTG_LAST ; ePhPh++)
        {
             _Disp.gotoxy(GLCD_X(73),GLCD_Y(u8Position));
             _Disp.printStringLeftAligned((char *)strPh_Ph[ePhPh],FONT_VERDANA);
             if(!prvIsValDigitsGreaterThan3((uint16_t)
                                            ((&_hal.AcSensors)->*ArrGetPhToPhVal[eSource][ePhPh])()))
             {
                 sprintf(arrTemp,"%d",(uint16_t)
                      ((&_hal.AcSensors)->*ArrGetPhToPhVal[eSource][ePhPh])());
                 _Disp.gotoxy(GLCD_X(120),GLCD_Y(u8Position));
                 _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                 _Disp.printChar('V',FONT_VERDANA);
                 u8Position = u8Position + 15;
             }
             else
             {
                 sprintf(arrTemp,"%.1f",(float)
                      (((&_hal.AcSensors)->*ArrGetPhToPhVal[eSource][ePhPh])()/1000.0f));
                 _Disp.gotoxy(GLCD_X(115),GLCD_Y(u8Position));
                 _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
                 _Disp.gotoxy(GLCD_X(116),GLCD_Y(u8Position));
                 _Disp.printStringLeftAligned((char *)"kV",FONT_VERDANA);
                 u8Position = u8Position + 15;
             }

        }
    }
    else if(u8AcSystemType ==  AC_SENSE::SPLIT_PHASE_SYSTEM)
   {
       _Disp.drawVerticalLine(GLCD_X(54), GLCD_Y(19), GLCD_Y(64));
       u8Position = 22 ;
       for(u8Local = R_PHASE; u8Local < B_PHASE ; u8Local++)
       {
          _Disp.gotoxy(GLCD_X(2),GLCD_Y(u8Position));
          _Disp.printStringLeftAligned((char *)strPhase[u8Local],FONT_VERDANA);

          if(!prvIsValDigitsGreaterThan3((uint16_t)
                                         ((&_hal.AcSensors)->*ArrGetVtgVal[eSource])((PHASE_t)u8Local)))
          {
              sprintf(arrTemp,"%d",(uint16_t)
                     ((&_hal.AcSensors)->*ArrGetVtgVal[eSource])((PHASE_t)u8Local));
              _Disp.gotoxy(GLCD_X(37),GLCD_Y(u8Position));
              _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
              _Disp.printChar('V',FONT_VERDANA);
              u8Position = u8Position + 15;
          }
          else
          {
              sprintf(arrTemp,"%.1f",(float)
                     (((&_hal.AcSensors)->*ArrGetVtgVal[eSource])((PHASE_t)u8Local)/1000.0f));
              _Disp.gotoxy(GLCD_X(37),GLCD_Y(u8Position));
              _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
              _Disp.printStringLeftAligned((char *)"kV",FONT_VERDANA);
              u8Position = u8Position + 15;
          }

       }

       _Disp.gotoxy(GLCD_X(35),GLCD_Y(52));

       if(eSource == MAINS)
       {
           sprintf(arrTemp,"%0.1f ",_f32MainsFreq);
       }
       else
       {
           sprintf(arrTemp,"%0.1f ",_f32GenMinFreq);
       }

       _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
       _Disp.printStringLeftAligned((char *)"Hz",FONT_VERDANA);
       _Disp.gotoxy(GLCD_X(57),GLCD_Y(35));
       _Disp.printStringLeftAligned((char *)strPh_Ph[R_TO_Y],FONT_VERDANA);
       if(!prvIsValDigitsGreaterThan3((uint16_t)
                                      ((&_hal.AcSensors)->*ArrGetPhToPhVal[eSource][R_TO_Y])()))
       {
           sprintf(arrTemp,"%d ",(uint16_t)
           ((&_hal.AcSensors)->*ArrGetPhToPhVal[eSource][R_TO_Y])());
           _Disp.gotoxy(GLCD_X(110),GLCD_Y(35));
           _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
           _Disp.printChar('V',FONT_VERDANA);
       }
       else
       {
           sprintf(arrTemp,"%.1f ",(float)
           (((&_hal.AcSensors)->*ArrGetPhToPhVal[eSource][R_TO_Y])()/1000.0f));
           _Disp.gotoxy(GLCD_X(110),GLCD_Y(35));
           _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
           _Disp.gotoxy(GLCD_X(112),GLCD_Y(35));
           _Disp.printStringLeftAligned((char *)"kV",FONT_VERDANA);
       }


   }
    else
    {
        _Disp.gotoxy(GLCD_X(30),GLCD_Y(37));
        _Disp.printStringLeftAligned((char *)strPhase[R_PHASE],FONT_VERDANA);
        _Disp.gotoxy(GLCD_X(70),GLCD_Y(37));
        if(!prvIsValDigitsGreaterThan3((uint16_t)
                    ((&_hal.AcSensors)->*ArrGetVtgVal[eSource])(R_PHASE)))
        {
            sprintf(arrTemp,"%d",(uint16_t)
                    ((&_hal.AcSensors)->*ArrGetVtgVal[eSource])(R_PHASE));
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
            _Disp.printChar('V',FONT_VERDANA);
        }
        else
        {
            sprintf(arrTemp,"%.1f",(float)
                    (((&_hal.AcSensors)->*ArrGetVtgVal[eSource])(R_PHASE)/1000.0f));
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
            _Disp.printStringLeftAligned((char *)"kV",FONT_VERDANA);
        }


        _Disp.gotoxy(GLCD_X(110),GLCD_Y(37));

        if(eSource == MAINS)
        {
            sprintf(arrTemp,"%0.1f ",_f32MainsFreq);
        }
        else
        {
            sprintf(arrTemp,"%0.1f ",_f32GenMinFreq);
        }

        _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
        _Disp.printStringLeftAligned((char *)"Hz",FONT_VERDANA);
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

    if(u8AcSystemType == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        sprintf(arrTemp,"  %0.1f",
                             ((&_hal.AcSensors)->*ArrGetTotalPower[eSourceType][eType])()/1000);

        if((eType == ACTIVE) && (eSourceType != MAINS))
        {
         _Disp.gotoxy(GLCD_X(3),GLCD_Y(33));
         _Disp.printStringLeftAligned((char *)arrTemp,FONT_ARIAL);

          _Disp.drawVerticalLine(GLCD_X(56), GLCD_Y(19), GLCD_Y(64));
          _Disp.gotoxy(GLCD_X(40),GLCD_Y(32));
          _Disp.printStringLeftAligned((char *)strPower[eType],FONT_ARIAL);
          sprintf(arrTemp,"  %d",
                  (uint16_t)round(_hal.AcSensors.GENSET_GetDispPercentPower()));
          _Disp.gotoxy(GLCD_X(3),GLCD_Y(48));
          _Disp.printStringLeftAligned((char *)arrTemp,FONT_ARIAL);
          _Disp.gotoxy(GLCD_X(40),GLCD_Y(48));
          _Disp.printStringLeftAligned((char *)"%",FONT_ARIAL);
          u8XPosition =58;
          u8XPositionVal = 110;
        }
        else
        {
            _Disp.gotoxy(GLCD_X(7),GLCD_Y(33));
            _Disp.printStringLeftAligned((char *)arrTemp,FONT_ARIAL);

            _Disp.drawVerticalLine(GLCD_X(50), GLCD_Y(19), GLCD_Y(64));
            _Disp.gotoxy(GLCD_X(23),GLCD_Y(50));
            _Disp.printStringCenterAligned((char *)strPower[eType],FONT_ARIAL);
        }

        u8Position = 22;
        for(u8Local = R_PHASE; u8Local < PHASE_END ; u8Local++)
        {
            _Disp.gotoxy(GLCD_X(u8XPosition),GLCD_Y(u8Position));
            _Disp.printStringLeftAligned((char *)strPhase[u8Local],FONT_VERDANA);
            sprintf(arrTemp,"  %0.1f",
                      ((&_hal.AcSensors)->*ArrGetVal[eSourceType][eType])((PHASE_t)u8Local)/1000);
            _Disp.gotoxy(GLCD_X(u8XPositionVal),GLCD_Y(u8Position));
            _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(126),GLCD_Y(u8Position));
            _Disp.printStringRightAligned((char *)strPower[eType],FONT_VERDANA);
            u8Position = u8Position +15;
        }
    }
    else if(u8AcSystemType ==  AC_SENSE::SPLIT_PHASE_SYSTEM)
    {
       _Disp.drawVerticalLine(GLCD_X(56), GLCD_Y(19), GLCD_Y(64));
       sprintf(arrTemp,"  %0.1f",
               ((&_hal.AcSensors)->*ArrGetTotalPower[eSourceType][eType])()/1000);
       _Disp.gotoxy(GLCD_X(20),GLCD_Y(30));
       _Disp.printStringCenterAligned((char *)arrTemp,FONT_ARIAL);


       if((eType == ACTIVE)&& (eSourceType != MAINS))
       {
           _Disp.gotoxy(GLCD_X(37),GLCD_Y(30));
           _Disp.printStringLeftAligned((char *)strPower[eType],FONT_ARIAL);
           sprintf(arrTemp,"  %d",
                   (uint8_t)round(_hal.AcSensors.GENSET_GetDispPercentPower()));
           _Disp.gotoxy(GLCD_X(15),GLCD_Y(48));
           _Disp.printStringCenterAligned((char *)arrTemp,FONT_ARIAL);
           _Disp.gotoxy(GLCD_X(26),GLCD_Y(48));
           _Disp.printStringLeftAligned((char *)"%",FONT_ARIAL);
       }
       else
       {
           _Disp.gotoxy(GLCD_X(23),GLCD_Y(48));
           _Disp.printStringCenterAligned((char *)strPower[eType],FONT_ARIAL);
       }
       u8Position = 28;
       for(u8Local = R_PHASE; u8Local < B_PHASE ; u8Local++)
       {
          _Disp.gotoxy(GLCD_X(60),GLCD_Y(u8Position));
          _Disp.printStringLeftAligned((char *)strPhase[u8Local],FONT_VERDANA);
          sprintf(arrTemp,"  %0.1f",
                  ((&_hal.AcSensors)->*ArrGetVal[eSourceType][eType])((PHASE_t)u8Local)/1000);
          _Disp.gotoxy(GLCD_X(100),GLCD_Y(u8Position));
          _Disp.printStringRightAligned((char *)arrTemp,FONT_VERDANA);
          _Disp.gotoxy(GLCD_X(104),GLCD_Y(u8Position));
          _Disp.printStringLeftAligned((char *)strPower[eType],FONT_VERDANA);
          u8Position = u8Position +18;
       }
    }
    else
    {
        if((eType == ACTIVE)&& (eSourceType != MAINS))
        {
            _Disp.gotoxy(GLCD_X(35),GLCD_Y(37));
            _Disp.printStringLeftAligned((char *)strPhase[R_PHASE],FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(95),GLCD_Y(25));
            sprintf(arrTemp,"%0.1f",
                    ((&_hal.AcSensors)->*ArrGetVal[eSourceType][eType])(R_PHASE)/1000);
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
            _Disp.printStringLeftAligned((char *)strPower[eType],FONT_VERDANA);

            _Disp.gotoxy(GLCD_X(95),GLCD_Y(47));
            sprintf(arrTemp,"%d",
                    (uint16_t)round(_hal.AcSensors.GENSET_GetDispPercentPower()));
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
            _Disp.printStringLeftAligned((char *)"%",FONT_VERDANA);

        }
        else
        {
            _Disp.gotoxy(GLCD_X(35),GLCD_Y(37));
            _Disp.printStringLeftAligned((char *)strPhase[R_PHASE],FONT_VERDANA);
            _Disp.gotoxy(GLCD_X(95),GLCD_Y(37));
            sprintf(arrTemp,"%0.1f",
                    ((&_hal.AcSensors)->*ArrGetVal[eSourceType][eType])(R_PHASE)/1000);
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
            _Disp.printStringLeftAligned((char *)strPower[eType],FONT_VERDANA);
        }

    }
}

void MON_UI::prvPrintSensorFaultStatus(A_SENSE::SENSOR_RET_t stTemp , uint16_t Xpos , uint16_t Ypos)
{
    if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT)
    {
        if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)
                || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
        {
          _Disp.gotoxy(GLCD_X(Xpos),GLCD_Y(Ypos));
          _Disp.printStringCenterAligned((char*)StrOpnGndckt[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
        }
        else
        {
          _Disp.printImage((uint8_t *)gau8ChineseCirucitOpenOrGround, 9, 13, 37, 30);
        }
    }
    else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_SHORT_TO_BAT)
    {
        if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)
                || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
        {
            _Disp.gotoxy(GLCD_X(Xpos),GLCD_Y(Ypos));
            _Disp.printStringCenterAligned((char*)StrSTB[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
        }
        else
        {
           _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_ShrtToBatt, 4, 13, 37, 30);
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
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)
                    || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
            {
              _Disp.gotoxy(GLCD_X(83),GLCD_Y(37));
              _Disp.printStringCenterAligned((char*)StrOpenckt[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
            }
            else
            {
              _Disp.printImage((uint8_t *)gau8ChineseCirucitOpen, 4, 13, 37, 60);
            }
        }
        else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_SHORT_TO_BAT)
        {
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)
                    || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
            {
                _Disp.gotoxy(GLCD_X(83),GLCD_Y(37));
                _Disp.printStringCenterAligned((char*)StrSTB[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
            }
            else
            {
               _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_ShrtToBatt, 4, 13, 37, 60);
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
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)
                    || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
            {
              _Disp.gotoxy(GLCD_X(83),GLCD_Y(37));
              _Disp.printStringCenterAligned((char*)StrOpenckt[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
            }
            else
            {
              _Disp.printImage((uint8_t *)gau8ChineseCirucitOpen, 4, 13, 37, 60);
            }
        }
        else if(stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_SHORT_TO_BAT)
        {
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)
                    || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
            {
                _Disp.gotoxy(GLCD_X(83),GLCD_Y(37));
                _Disp.printStringCenterAligned((char*)StrSTB[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
            }
            else
            {
               _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_ShrtToBatt, 4, 13, 37, 60);
            }
        }
    }


}

void MON_UI::prvStopKeyPressAction()
{

    _startStop.StopKeyPressed();

    if(_eOpMode == BASE_MODES::MANUAL_MODE)
    {
        BASE_MODES:: SetStopPressState(true);
    }
    if(eDisplayMode == DISP_MON_MODE)
    {
        _bMBModeChnageCMDRcvd = false;
        _startStop.ClearSimAutoPulse();
        _startStop.ClearSimStopPulse();
        _startStop.ClearSimStartPulse();
        _manualMode.Stop();

        if(_eOpMode == BASE_MODES::AUTO_MODE)
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);
            _manualMode.DisableEngCoolTimer();
            _manualMode.DisableReturnToMains();
            if((_manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_OFF_MAINS_OFF)
                    ||(_manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_OFF_MAINS_ON))
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
            }
            else if((_manualMode.GetAutoModeState() != BASE_MODES::STATE_AMF_GEN_ON_LOAD)
                    &&(_manualMode.GetAutoModeState() != BASE_MODES::STATE_AMF_RETURN_DELAY))
            {
                _manualMode.SwitchToManualMode();
            }
        }
        else if(_eOpMode == BASE_MODES::AUTO_EXERCISE_MODE)
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);
            _manualMode.DisableEngCoolTimer();
            if(_manualMode.GetAutoExeState() != BASE_MODES::ID_AUTO_EXE_DG_ON_LOAD)
            {
                _manualMode.SwitchToManualMode();
            }
            _autoExercise.StopAutoExe();
        }
        else if(_eOpMode == BASE_MODES::BTS_MODE)
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);
            _manualMode.DisableEngCoolTimer();
            _manualMode.DisableReturnToMains();
            _BTSMode.ClearBTSVars();
            if((_manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_GEN_OFF_MAINS_OFF)
                    ||(_manualMode.GetBTSModeState() == BASE_MODES:: STATE_BTS_GEN_OFF_MAINS_ON))
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
                _manualMode.OpenGenLoad();
            }
            else if((_manualMode.GetBTSModeState()!= BASE_MODES::STATE_BTS_GEN_ON_LOAD)
                    &&(_manualMode.GetBTSModeState()!= BASE_MODES::STATE_BTS_RETURN_DELAY))
            {
                _manualMode.SwitchToManualMode();
                //_startStop.StopCommand();
            }
            else
            {
               // _startStop.StopCommand();
            }
            // After coming out of Auto Mode MainsPartialHealthy should be always false & MainsPartialLEDStatus should be true.
            _manualMode.ClearMainsPartialHealthyStatus();
            _manualMode.SetMainsPartialLEDstatus();
        }
        else if(_eOpMode == BASE_MODES::CYCLIC_MODE)
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);
            _manualMode.DisableEngCoolTimer();
            _manualMode.DisableReturnToMains();
            _cyclicMode.ClearCyclicModeVars();

            if((_manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_GEN_OFF_MAINS_OFF)
                    ||(_manualMode.GetCyclicModeState() == BASE_MODES:: STATE_CYCLIC_GEN_OFF_MAINS_ON))
            {
                _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
                _manualMode.OpenGenLoad();
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
            _manualMode.DisableTestModeTimer();
            _eOpMode = BASE_MODES::MANUAL_MODE;
        }
        _hal.ledManager.led2.TurnOff();
        _hal.ledManager.led3.TurnOff();
        _hal.ledManager.led4.TurnOn();
        _manualMode.SetGCUOperatingMode(_eOpMode);
    }
}

void MON_UI::prvStartKeyPressAction()
{
    _bMBModeChnageCMDRcvd = false;
    _startStop.ClearSimStartPulse();
    _startStop.ClearSimAutoPulse();

    if(_eOpMode == BASE_MODES::MANUAL_MODE)
    {
        BASE_MODES::SetStartPressState(true);
    }

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
    _bMBModeChnageCMDRcvd = false;
    _startStop.ClearSimAutoPulse();
    if(_eOpMode == BASE_MODES::MANUAL_MODE)
    {
        if( (!_GCUAlarms.IsCommonShutdown()) && (!_GCUAlarms.IsCommonElectricTrip()))
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
            else if(_manualMode.GetGCUOperatingMode()==BASE_MODES::AUTO_EXERCISE_MODE)
            {
                _eOpMode = BASE_MODES::AUTO_EXERCISE_MODE;
            }
            else if(_manualMode.GetGCUOperatingMode()==BASE_MODES::AUTO_MODE)
            {
                _eOpMode = BASE_MODES::AUTO_MODE;
            }
        }
    }
    else if(_eOpMode == BASE_MODES::AUTO_MODE)
    {
        if((_manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_OFF_MAINS_ON)
                ||(_manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_OFF_MAINS_OFF))
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
            _manualMode.DisableReturnToMains();
        }
        else if(_manualMode.GetAutoModeState() == BASE_MODES::STATE_AMF_GEN_ON_LOAD)
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUTOLOAD_TRANSFER) == CFGZ::CFGZ_ENABLE)
            {
                _manualMode.SwitchLoadToGen();
            }
            else
            {
                _manualMode.OpenGenLoad();
                _manualMode.OpenMainsLoad();
            }
            _manualMode.DisableReturnToMains();
        }
    }
    else if(_eOpMode == BASE_MODES::AUTO_EXERCISE_MODE)
    {
        if(_manualMode.GetAutoExeState() == BASE_MODES::ID_AUTO_EXE_DG_ON_LOAD)
        {
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUTOLOAD_TRANSFER) == CFGZ::CFGZ_DISABLE)
            {
               _manualMode.SwitchLoadToGen();
            }
            else
            {
               _manualMode.OpenGenLoad();
               _manualMode.OpenMainsLoad();
            }
            _autoExercise.StopAutoExe();
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.DisableReturnToMains();
        }
        else if(_manualMode.GetAutoExeState() == BASE_MODES::ID_AUTO_EXE_FAULT)
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
            _autoExercise.StopAutoExe();
        }
    }
    else if(_eOpMode == BASE_MODES::BTS_MODE)
    {
        if((_manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_GEN_OFF_MAINS_OFF)
                ||(_manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_GEN_OFF_MAINS_ON))
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
            _manualMode.DisableReturnToMains();
        }
        else if(_manualMode.GetBTSModeState() == BASE_MODES::STATE_BTS_GEN_ON_LOAD)
        {
             if(_BTSMode.IsGenOnDuetoBTS())
             {
                 _BTSMode.TurnOffBattCharging(); // Clear BTS run time.
             }
             else if(_BTSMode.IsGenOnDueToSheltTemp())
             {
                 _BTSMode.TurnOffBattCharging(); // Clear BTS run time.
             }

             _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);
             if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUTOLOAD_TRANSFER) == CFGZ::CFGZ_ENABLE)
             {
                 _manualMode.SwitchLoadToGen();
             }
             else
             {
                 _manualMode.OpenGenLoad();
                 _manualMode.OpenMainsLoad();
             }
             _eOpMode = BASE_MODES::MANUAL_MODE;
             _manualMode.DisableReturnToMains();
        }
        // After coming out of Auto Mode MainsPartialHealthy should be always false & MainsPartialLEDStatus should be true.
        _manualMode.ClearMainsPartialHealthyStatus();
        _manualMode.SetMainsPartialLEDstatus();
    }
    else if(_eOpMode == BASE_MODES::CYCLIC_MODE)
    {
        _cyclicMode.ClearCyclicModeVars();
        if((_manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_GEN_OFF_MAINS_OFF)
                ||(_manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_GEN_OFF_MAINS_ON))
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_OFF);
            _manualMode.DisableReturnToMains();
        }
        else if((_manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_GEN_ON_LOAD)
                ||(_manualMode.GetCyclicModeState() == BASE_MODES::STATE_CYCLIC_RETURN_DELAY))
        {
            _eOpMode = BASE_MODES::MANUAL_MODE;
            _manualMode.ChangeManualState(BASE_MODES::STATE_MANUAL_GEN_READY);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUTOLOAD_TRANSFER) == CFGZ::CFGZ_ENABLE)
            {
                _manualMode.SwitchLoadToGen();
            }
            else
            {
                _manualMode.OpenGenLoad();
                _manualMode.OpenMainsLoad();
            }
            _manualMode.DisableReturnToMains();

        }
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


void MON_UI::prvGetMonImageCoordicates( uint8_t *pu8SizeX, uint8_t *pu8SizeY, uint8_t *pu8CordinateX, uint8_t *pu8CordinateY)
{
    switch(_stScreenNo)
    {
        case DISP_AUTO_EXERCISE_1:
        case DISP_AUTO_EXERCISE_2:
        case DISP_HOME:
            *pu8SizeX= 11;
            *pu8SizeY= 16;
            *pu8CordinateX = 2;
            *pu8CordinateY = 20;
            break;


        case DISP_ENGINE_TYPE:
            *pu8SizeX= 9;
            *pu8SizeY= 16;
            *pu8CordinateX = 3;
            *pu8CordinateY = 30;
            break;

        case DISP_CONTACTOR_STATUS:
            *pu8SizeX= 9;
            *pu8SizeY= 16;
            *pu8CordinateX = 2;
            *pu8CordinateY = 27;
            break;
        case DISP_PRODUCT_INFO:
            *pu8SizeX= 9;
            *pu8SizeY= 16;
            *pu8CordinateX = 2;
            *pu8CordinateY = 30;
            break;
        case DISP_GEN_LN_VOLTAGE:
            *pu8SizeX= 11;
            *pu8SizeY= 16;
            *pu8CordinateX = 2;
            *pu8CordinateY = 20;
            break;

        case DISP_GEN_PF:
            *pu8SizeX= 9;
            *pu8SizeY= 16;
            *pu8CordinateX = 2;
            *pu8CordinateY = 24;
            break;

        case DISP_HISTOGRAM:
            *pu8SizeX= 7;
            *pu8SizeY= 16;
            *pu8CordinateX = 2;
            *pu8CordinateY = 38;
            break;

        case DISP_MAINS_LN_VOLTAGE:
            *pu8SizeX= 12;
            *pu8SizeY= 16;
            *pu8CordinateX = 2;
            *pu8CordinateY = 28;
            break;
        case DISP_MAINS_RUN_TIME:
            *pu8SizeX= 11;
            *pu8SizeY= 16;
            *pu8CordinateX = 2;
            *pu8CordinateY = 24;
            break;




       case DISP_GEN_CURRENT:
           *pu8SizeX= 9;
           *pu8SizeY= 16;
           *pu8CordinateX = 2;
           *pu8CordinateY = 28;
           break;
       case DISP_GEN_KW:
       case DISP_GEN_KVA:
       case DISP_GEN_KVAR:
           *pu8SizeX= 12;
           *pu8SizeY= 16;
           *pu8CordinateX = 2;
           *pu8CordinateY = 20;
           break;
       case DISP_GEN_CUMU_POWER:
           *pu8SizeX= 11;
           *pu8SizeY= 16;
           *pu8CordinateX = 2;
           *pu8CordinateY = 22;
           break;



      case DISP_MAINS_CUMU_POWER:
          *pu8SizeX= 11;
          *pu8SizeY= 16;
          *pu8CordinateX = 2;
          *pu8CordinateY = 22;
          break;

      case DISP_SITE_BATT:
      case DISP_BATTERY_VOLTAGE :
          *pu8SizeX= 11;
          *pu8SizeY= 16;
          *pu8CordinateX = 2;
          *pu8CordinateY = 20;
          break;


      case DISP_BTS_RUN_TIME:
          *pu8SizeX= 9;
          *pu8SizeY= 16;
          *pu8CordinateX = 2;
          *pu8CordinateY = 27;
          break;
      case DISP_COOLENT_TEMP    :
          *pu8SizeX= 9;
          *pu8SizeY= 16;
          *pu8CordinateX = 2;
          *pu8CordinateY = 27;
          break;
      case DISP_OIL_PRESSURE    :
          *pu8SizeX= 9;
          *pu8SizeY= 16;
          *pu8CordinateX = 2;
          *pu8CordinateY = 30;
          break;
      case DISP_BALANCE_FUEL    :
          *pu8SizeX= 9;
          *pu8SizeY= 16;
          *pu8CordinateX = 2;
          *pu8CordinateY = 28;
          break;

      case DISP_SHELTER_TEMP    :
          *pu8SizeX= 14;
          *pu8SizeY= 16;
          *pu8CordinateX = 2;
          *pu8CordinateY = 12;
          break;

      case DISP_ENGINE_SPEED    :
          *pu8SizeX= 11;
          *pu8SizeY= 16;
          *pu8CordinateX = 2;
          *pu8CordinateY = 20;
          break;

          //Todo:Change Tampered Run Hours Co-ordinates as per new translation
      case DISP_TAMPERED_RUNHRS:
      case DISP_ENGINE_RUN_TIME :
          *pu8SizeX= 13;
          *pu8SizeY= 16;
          *pu8CordinateX = 2;
          *pu8CordinateY = 12;
          break;
      default: break;
    }
}


void MON_UI::prvChiniHomScreen()
{
    char arrTemp[32];
    _Disp.drawHorizontalLine(0, 35, 128);
    switch(_manualMode.GetGCUState())
    {

        case BASE_MODES::ENGINE_OFF_READY:
            {
                _Disp.printImage((uint8_t*)stChiniEngStatus.gau8ChineseEngineOffReady, 14, 14, 21, 3);
            }
            break;

        case BASE_MODES::ENGINE_STARTING:
            {
                _Disp.printImage((uint8_t *)stChiniEngStatus.gau8ChineseStartingAttempts, 12, 16, 21, 3);
                _Disp.gotoxy(105,24);
                sprintf(arrTemp,"%d/%d", _startStop.GetCrankAttemptNumber(),
                        _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_ATTEMPTS) );
            }
            break;

        case BASE_MODES::ENGINE_ON_HEALTHY:
            {
                _Disp.printImage((uint8_t *)stChiniEngStatus.gau8ChineseEngineOnHealthy, 10, 14, 21, 3);
            }
            break;

        case BASE_MODES::ENGINE_STOPPING:
            {
                _Disp.printImage((uint8_t *)stChiniEngStatus.gau8ChineseEngineStopping, 10, 14, 21, 3);
            }
            break;

        case BASE_MODES::NOTIFICATION:
            {
                _Disp.printImage((uint8_t *)stChiniEngStatus.gau8ChineseNotificationAlarm, 12, 14, 21, 3);
            }
            break;

        case BASE_MODES::WARNING:
            {
                _Disp.printImage((uint8_t *)stChiniEngStatus.gau8ChineseWarningAlarm, 11, 14, 21, 3);
            }
            break;

        case BASE_MODES::ELECTRIC_TRIP:
            {
                _Disp.printImage((uint8_t *)stChiniEngStatus.gau8ChineseElectricalTripAlarm, 11, 14, 21, 3);
            }
            break;

        case BASE_MODES::SHUTDOWN:
            {
                _Disp.printImage((uint8_t *)stChiniEngStatus.gau8ChineseShutdownAlarm, 15, 13, 21, 1);
            }
            break;

        default:
            {
                _Disp.gotoxy(GLCD_X(3),GLCD_Y(21));
                _Disp.printStringLeftAligned((char *)strGCUStatus[_cfgz.GetArrLanguageIndex()][_manualMode.GetGCUState()],FONT_VERDANA);
            }
            break;
    }

    switch(_manualMode.GetTimerState())
    {
        case  BASE_MODES::CRANK_START_TIMER:
            {
                _Disp.printImage((uint8_t *)stChiniTmr.gau8ChineseCranking, 6, 12, 38, 3);
            }
            break;

        case  BASE_MODES::CRANK_REST_TIMER:
            {
                _Disp.printImage((uint8_t *)stChiniTmr.gau8ChineseCrankRest, 6, 13, 37, 3);
            }
            break;

        case  BASE_MODES::COOL_DOWN_TIMER:
            {
                _Disp.printImage((uint8_t *)stChiniTmr.gau8ChineseCoolDown, 7, 11, 38, 3);
            }
            break;

        case  BASE_MODES::STOP_ACTION_TIMER:
            {
                _Disp.printImage((uint8_t *)stChiniTmr.gau8ChineseStopAction, 4, 11, 38, 3);
            }
            break;

        case  BASE_MODES::ADDTIONAL_STOP_TIMER:
            {
                _Disp.printImage((uint8_t *)stChiniTmr.gau8ChineseAdditionalStop, 8, 12, 38, 3);
            }
            break;

        case  BASE_MODES::SAFETY_MON_TIMER:
            {
                _Disp.printImage((uint8_t *)stChiniTmr.gau8ChineseSafetyMonitoring, 7, 12, 38, 3);
            }
            break;

        case  BASE_MODES::START_DELAY_TIMER:
            {
                _Disp.printImage((uint8_t *)stChiniTmr.gau8ChineseStartDelay, 6, 12, 38, 3);
            }
            break;

        case  BASE_MODES::PREHEAT_TIMER:
            {
                _Disp.printImage((uint8_t *)stChiniTmr.gau8ChinesePreheating, 6, 12, 38, 3);
            }
            break;

        default:
            {
                _Disp.gotoxy(GLCD_X(5),GLCD_Y(37));
                sprintf(arrTemp,"%s",strTimerStatus[_cfgz.GetArrLanguageIndex()][_manualMode.GetTimerState()]);
                _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
            }
            break;
    }

    if(_startStop.GetTimersRemainingTime(_manualMode.GetTimerState()) > 0)
    {
        _Disp.gotoxy(GLCD_X(70),GLCD_Y(37));
        if((_manualMode.GetTimerState() == BASE_MODES::PREHEAT_TIMER)
               &&(_cfgz.GetEngType()==CFGZ::ENG_VOLVO) && _cfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_TO_ECU))
       {
           ;// Do nothing
       }
       else
       {
           sprintf(arrTemp," %d", (int)_startStop.GetTimersRemainingTime(_manualMode.GetTimerState()));
       }
        _Disp.printStringLeftAligned((char *)arrTemp,FONT_VERDANA);
    }

    if(_eOpMode == BASE_MODES::MANUAL_MODE)
    {
        _Disp.printImage((uint8_t *)u8bmpChiniManualMode, 6, 12, 51, 3);
    }
    else if(_eOpMode == BASE_MODES::AUTO_MODE)
    {

        _Disp.printImage((uint8_t *)gau8ChineseAutoAMFMode, 4, 12, 51, 3);
        _Disp.gotoxy(GLCD_X(33),GLCD_Y(53));
        _Disp.printStringLeftAligned((char*)" - AMF");
    }
    else if(_eOpMode == BASE_MODES::AUTO_EXERCISE_MODE)
    {
        if(_autoExercise.GetRunningExeType() ==1)
        {
            _Disp.printStringLeftAligned((char *)"Exercise 1",FONT_ARIAL);
        }
        else if(_autoExercise.GetRunningExeType() ==2)
        {
            _Disp.printStringLeftAligned((char *)"Exercise 2",FONT_ARIAL);
        }

        if(_autoExercise.IsExerciserStarted())
        {
            sprintf(arrTemp,"%02d:%02d", (uint8_t)(_autoExercise.GetExerciserTime()/60),
                    (uint8_t)(_autoExercise.GetExerciserTime()%60));
            _Disp.gotoxy(GLCD_X(126),GLCD_Y(51));
            _Disp.printStringRightAligned((char *)arrTemp,FONT_ARIAL);
        }
    }

}


void MON_UI:: prvPrintExerciser(uint8_t _ScreenNo)
{
    AUTO_EXERCISE_MODE::EXERCISE_t stTempExercise;
    char arrTemp[32];
    _Disp.gotoxy(GLCD_X(3),GLCD_Y(21));
    _Disp.printStringLeftAligned((char*)StrAutoExerciser[_cfgz.GetArrLanguageIndex()][STR_AUTO_EXERCISER_FREQ], FONT_VERDANA);

    _Disp.gotoxy(GLCD_X(3),GLCD_Y(31));
    _Disp.printStringLeftAligned((char*)StrAutoExerciser[_cfgz.GetArrLanguageIndex()][STR_AUTO_EXERCISER_NEXT_RUN], FONT_VERDANA);

    _Disp.gotoxy(GLCD_X(51),GLCD_Y(42));
    _Disp.printStringLeftAligned((char*)StrAutoExerciser[_cfgz.GetArrLanguageIndex()][STR_AUTO_EXERCISER_AT], FONT_VERDANA);
    _Disp.gotoxy(GLCD_X(51),GLCD_Y(52));
    _Disp.printStringLeftAligned((char*)StrAutoExerciser[_cfgz.GetArrLanguageIndex()][STR_AUTO_EXERCISER_FOR], FONT_VERDANA);

     _autoExercise.GetExerciserInfo(&stTempExercise, _ScreenNo );
//    stTempExercise.u8Occurence = 1;
//     stTempExercise.u16Year = 2021;
//     stTempExercise.u8Day = 3;
//     stTempExercise.u8DayOfWeek = 1;
//     stTempExercise.u8Month = 2;
//     stTempExercise.u32OnDuration = 4502;
//     stTempExercise.u32StartTime = 1410;
     _Disp.gotoxy(GLCD_X(50),GLCD_Y(21));
     switch(stTempExercise.u8Occurence)
     {
         case 0:
         {
             _Disp.printStringLeftAligned((char*)StrAutoExerciser[_cfgz.GetArrLanguageIndex()][STR_AUTO_EXERCISER_DAILY], FONT_VERDANA);

         }
         break;

         case 1:
         {
             _Disp.printStringLeftAligned((char*)StrAutoExerciser[_cfgz.GetArrLanguageIndex()][STR_AUTO_EXERCISER_WEEKLY], FONT_VERDANA);
             _Disp.gotoxy(GLCD_X(65),GLCD_Y(31));
             _Disp.printStringLeftAligned((char*)StrDays[_cfgz.GetArrLanguageIndex()][stTempExercise.u8DayOfWeek], FONT_VERDANA);
         }
         break;

         case 2:
         {
             _Disp.printStringLeftAligned((char*)StrAutoExerciser[_cfgz.GetArrLanguageIndex()][STR_AUTO_EXERCISER_MONTHLY], FONT_VERDANA);
             _Disp.gotoxy(GLCD_X(65),GLCD_Y(31));
             sprintf(arrTemp,"%02d %s %d",  stTempExercise.u8Day, (char*)StrMonth[_cfgz.GetArrLanguageIndex()][stTempExercise.u8Month - 1]
                                                                             , stTempExercise.u16Year );
             _Disp.printStringLeftAligned((char*)arrTemp, FONT_VERDANA);
         }
         break;
     }
     _Disp.gotoxy(GLCD_X(75),GLCD_Y(42));
     sprintf(arrTemp,"%02d:%02d",  (stTempExercise.u32StartTime /100),(stTempExercise.u32StartTime%100) );
     _Disp.printStringLeftAligned((char*)arrTemp, FONT_ARIAL);
     _Disp.gotoxy(GLCD_X(105),GLCD_Y(42));
     _Disp.printStringLeftAligned((char*)"Hrs", FONT_VERDANA);
     _Disp.gotoxy(GLCD_X(75),GLCD_Y(52));
     sprintf(arrTemp,"%02d:%02d",  (stTempExercise.u32OnDuration/100),(stTempExercise.u32OnDuration%100) );
     _Disp.printStringLeftAligned((char*)arrTemp, FONT_ARIAL );
     _Disp.gotoxy(GLCD_X(105),GLCD_Y(52));
     _Disp.printStringLeftAligned((char*)"Hrs", FONT_VERDANA);
}


void MON_UI::prvBuildLampIconScreen(void)
{
    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.gotoxy(GLCD_X(64),GLCD_Y(5));
    _Disp.printStringCenterAligned((char *)StrLampIcons[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
    _Disp.drawHorizontalLine(0, 19, 127);
    /* to solve SGC-225 */
    if( _j1939.IsCommunicationFail() || (!_j1939.IsDM1PGNRecieved()) )
    {
        _Disp.gotoxy(32,64);
        _Disp.printStringCenterAligned((char*)strCANMsgRcvError[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
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
    _Disp.printStringCenterAligned((char *)StrExhaustAFT[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
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
        _Disp.printStringCenterAligned((char*)strCANMsgRcvError[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
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


void MON_UI:: prvPrintHystogram()
{
    _Disp.drawVerticalLine(64, 19, 64);
    char arrTemp[32];
    uint32_t u32TempVal_1, u32TempVal_2 = 0;


    uint8_t u8Ylocation=23;
    for(uint8_t i=0;i<6;i++)
    {
        if(i>=3)
        {
            if(i==3)
            {
                u8Ylocation=23;
            }
            _Disp.gotoxy(GLCD_X(66),GLCD_Y(u8Ylocation));

        }
        else
        {
            _Disp.gotoxy(GLCD_X(3),GLCD_Y(u8Ylocation));
        }
        _Disp.printStringLeftAligned((char*)strHystogram[i], FONT_VERDANA);
        u8Ylocation = u8Ylocation+14;
    }


    u8Ylocation = 23;

    for(uint8_t i=0;i<6;i++)
    {
        if(i>=3)
        {
            if(i==3)
            {
                u8Ylocation=23;
            }
            _Disp.gotoxy(GLCD_X(102),GLCD_Y(u8Ylocation));

        }
        else
        {
            _Disp.gotoxy(GLCD_X(38),GLCD_Y(u8Ylocation));
        }

        u32TempVal_1 = ((_EngineMon.GetHistogramData(i))/ 60);
        u32TempVal_2 = ((_EngineMon.GetHistogramData(i)) % 60);

        if(u32TempVal_1 < 100)
        {
            sprintf(arrTemp,"%d:%02d",u32TempVal_1,u32TempVal_2);
        }
        else
        {
            sprintf(arrTemp,"%d",u32TempVal_1);
        }

        _Disp.printStringLeftAligned((char*)arrTemp, FONT_VERDANA);
        u8Ylocation = u8Ylocation+14;
    }

}

