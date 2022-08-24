/**
 * @file        [ALARM_UI]
 * @brief       This module is used to display Alarm screens
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
#include "../UI_DS.h"
uint8_t ALARM_UI::u8AlarmScreenNum = 0;
ALARM_UI::ALARM_st ALARM_UI::_eAlarmType = ALARM;
ALARM_UI::ALARM_UI( GCU_ALARMS &alarm, Display &Disp, CFGZ &cfgz, J1939APP &J1939, HAL_Manager &hal):
_alarm(alarm),
_Disp(Disp),
_cfgz(cfgz),
_J1939(J1939),
_hal(hal),
_u8NumberOfAlarms(0),
_eOldAlarmType(ALARM),
_u8ArrPCDAlarmForDisp{0},
_u8ArrNCDAlarmForDisp{0}
{

}

void ALARM_UI::Update(bool bRefresh)
{


    if(_cfgz.GetEngType()== CFGZ::ENG_CONVENTIONAL)
    {
        _eAlarmType = ALARM;
    }

    switch(_eAlarmType)
    {
        case ALARM:
            _u8NumberOfAlarms = _alarm.GetActiveAlarmCount();
        break;
        case DM1:
            _u8NumberOfAlarms = _J1939.GetDm1MsgCount();
            break;
        case DM2:
            _u8NumberOfAlarms = _J1939.GetDm2MsgCount();
            break;

        case NCD:
            _u8NumberOfAlarms = _J1939.GetNCDAlarmCount();
            break;
        case PCD:
            _u8NumberOfAlarms = _J1939.GetPCDAlarmCount();
            break;

    }

    if(_eOldAlarmType != _eAlarmType)
    {
        u8AlarmScreenNum =0;
        _eOldAlarmType = _eAlarmType;
        bRefresh = true;
    }

    if(bRefresh)
    {
        switch(_eAlarmType)
        {
            case ALARM:
                prvDisplayAlarmScreen();
            break;
            case DM1:
            case DM2:
                prvDisplayDMScreen();
                break;
            case NCD:
            case PCD:
                prvDisplayPCDNCDScreen();
                break;

        }
    }
}

void ALARM_UI::CheckKeyPress(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent)
{
    switch(_sKeyEvent)
    {
        case UP_SHORT_PRESS:  //Up key Press
       {
           if(u8AlarmScreenNum == 0)
           {
              u8AlarmScreenNum = _u8NumberOfAlarms;
              if((_eAlarmType == DM1)&& (_cfgz.GetEngType()!=CFGZ::ENG_CONVENTIONAL))
              {
                  _eAlarmType = ALARM;
              }
              else if(_eAlarmType == DM2)
              {
                  MON_UI::eDisplayMode = DISP_MON_MODE;
                  MON_UI::eMonScreenType =MON_UI::MON_SCREEN_J1939;
                  MON_UI::_stScreenNo = MON_UI::DISP_J1939_RX_LAST-1;
                  _eOldAlarmType = ALARM;
              }
              else if(_eAlarmType == PCD)
              {
                  _eAlarmType = DM2;
                  _J1939.RequestDM2Messages();
              }
              else if(_eAlarmType == NCD)
              {
                  _eAlarmType = PCD;
              }
              else
              {
                  MON_UI::eDisplayMode = DISP_MON_MODE;
              }
           }
           if(_u8NumberOfAlarms)
           {
               u8AlarmScreenNum--;
           }
           break;
       }
       case DN_SHORT_PRESS: //Dn key Press
       {

         u8AlarmScreenNum++;

          if(u8AlarmScreenNum >= _u8NumberOfAlarms)
          {
              u8AlarmScreenNum = 0;
              if((_eAlarmType == ALARM)&& (_cfgz.GetEngType()!=CFGZ::ENG_CONVENTIONAL))
              {
                  _eAlarmType = DM1;
              }
              else if(_eAlarmType == DM2)
              {
                  if( _cfgz.GetEngType()==CFGZ::ENG_KUBOTA)
                  {
                      _eAlarmType = PCD;
                  }
                  else
                  {
                      MON_UI::eDisplayMode = DISP_MON_MODE;
                      MON_UI::eMonScreenType =MON_UI::MON_SCREEN_J1939;
                      MON_UI::_stScreenNo = MON_UI::DISP_PROPB62_PGN_65378;
                      _eOldAlarmType = ALARM;
                  }
              }
              else if(_eAlarmType == PCD)
              {
                  _eAlarmType = NCD;
              }
              else if(_eAlarmType == NCD)
              {
                  MON_UI::eDisplayMode = DISP_MON_MODE;
                  MON_UI::eMonScreenType =MON_UI::MON_SCREEN_J1939;
                  MON_UI::_stScreenNo = MON_UI::DISP_PROPB62_PGN_65378;
                  _eOldAlarmType = ALARM;
              }
              else
              {
                  MON_UI::eDisplayMode = DISP_MON_MODE;
                  MON_UI::_stScreenNo = MON_UI::DISP_HOME;
                  MON_UI::eMonScreenType =MON_UI::MON_SCREEN_NORMAL;
              }

          }
           break;
       }
       case ACK_KEY_PRESS:
      {

          if(_hal.actuators.GetActStatus(ACTUATOR::ACT_AUDIBLE_ALARM) == true)
          {
              _alarm.TurnOffSounderAlarm();
          }
          else
          {
              _alarm.ClearAllAlarms();
              u8AlarmScreenNum = 0;

              MON_UI::eDisplayMode = DISP_MON_MODE;
              MON_UI::_stScreenNo = MON_UI::DISP_HOME;
              MON_UI::eMonScreenType =MON_UI::MON_SCREEN_NORMAL;
          }
          break;
      }

       case DN_LONG_PRESS:
       {
           if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE))
           {
               if(((_eAlarmType == DM2)||(_eAlarmType == PCD)||(_eAlarmType == NCD)))
               {
                   MON_UI::eDisplayMode = DISP_MON_MODE;
                   MON_UI::_stScreenNo = MON_UI::DISP_HOME;
                   MON_UI::eMonScreenType =MON_UI::MON_SCREEN_NORMAL;
               }
               else
               {
                   _eAlarmType = DM2;
                   _J1939.RequestDM2Messages();
               }
           }
           break;
       }
       default: break;
    }
}

void ALARM_UI::prvDisplayAlarmScreen()
{
    static uint8_t u8PrevAlarmCount = _u8NumberOfAlarms;
    char arrTemp[20]={'\0'}, arrTemp1[20]={'\0'};
    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.drawHorizontalLine(GLCD_X(0), GLCD_Y(19), GLCD_Y(128));

    _Disp.drawVerticalLine(GLCD_X(100),GLCD_Y(0),GLCD_Y(19));
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)
            || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
    {
        _Disp.drawVerticalLine(GLCD_X(22), GLCD_Y(0), GLCD_Y(19));
        _Disp.printImage((uint8_t *)u8ArrAlarmLogo, 3,20,0, 0);

        //Print screen name
        _Disp.gotoxy(GLCD_X(64),GLCD_Y(5));
        _Disp.printStringCenterAligned((char *)strAlarms[_cfgz.GetArrLanguageIndex()],
                    FONT_ARIAL);
    }
    else
    {
        _Disp.printImage((uint8_t *)&u8ChiniAlarmScreen[0][0], 4,14,5,35);
    }

    _Disp.gotoxy(GLCD_X(126),GLCD_Y(5));
    if(u8PrevAlarmCount != _u8NumberOfAlarms)
    {
        if(_u8NumberOfAlarms)
        {
            u8AlarmScreenNum = _u8NumberOfAlarms - 1;
        }
    }
    u8PrevAlarmCount = _u8NumberOfAlarms;

    if(_u8NumberOfAlarms)
    {
        sprintf(arrTemp," %d/%d", u8AlarmScreenNum + 1, _u8NumberOfAlarms);
    }
    else
    {
        sprintf(arrTemp," %d/%d",0, 0);
        u8AlarmScreenNum = 0;
    }
    _Disp.printStringRightAligned((char *)arrTemp, FONT_VERDANA);

    bool _bAlarminEnglish=false;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_CHINSESE)
    {
        switch(_alarm.GetAlarmId(u8AlarmScreenNum))
          {
              case GCU_ALARMS::NoAlarm_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_NoAlarms, 7, 16, 33, 36);
                  break;
              }
              case GCU_ALARMS::Low_Oil_Pressure_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_LOP, 7, 16, 24, 30);
                  if(_alarm.IsAlarmActive(GCU_ALARMS::LLOP_SWITCH))
                  {
                       _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_Switch, 6, 15, 46, 80);
                  }
                  if(_alarm.IsAlarmActive(GCU_ALARMS::LOW_OIL_PRESS_SHUTDOWN)
                   ||_alarm.IsAlarmActive(GCU_ALARMS::LOW_OIL_PRESS_WARNING))
                  {
                       _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_Sensor, 6, 16, 46, 5);
                  }
                  break;
              }
              case GCU_ALARMS::fuel_level_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_LowFuelLevel, 10, 17, 24, 27);
                  if(_alarm.IsAlarmActive(GCU_ALARMS::LOW_FUEL_LVL_SWITCH) )
                  {
                       _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_Switch, 6, 15, 46, 80);
                  }
                  if(_alarm.IsAlarmActive(GCU_ALARMS::LOW_FUEL_LEVEL_SHUTDOWN)
                   ||_alarm.IsAlarmActive(GCU_ALARMS::LOW_FUEL_LEVEL_WARNING))
                  {
                       _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_Sensor, 6, 16, 46, 5);
                  }
                  break;
              }
              case GCU_ALARMS::High_Water_Temperature_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_HighEngTemp, 7, 16, 24, 38);
                  if(_alarm.IsAlarmActive(GCU_ALARMS::HWT_SWITCH))
                  {
                       _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_Switch, 6, 15, 46, 80);
                  }
                  if(_alarm.IsAlarmActive(GCU_ALARMS::HIGH_ENG_TEMP_SHUTDOWN)
                   ||_alarm.IsAlarmActive(GCU_ALARMS::HIGH_ENG_TEMP_WARNING))
                  {
                       _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_Sensor, 6, 16, 46, 5);
                  }
                  break;
              }
              case GCU_ALARMS::Radiator_Water_Level_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_LowWaterLevelSwitch, 11, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::Over_Speed_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_OverSpeed, 9, 16, 33, 30);
                  break;
              }
              case GCU_ALARMS::Gross_Over_Speed_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_GrossOverSpeed, 9, 16, 33, 30);
                  break;
              }
              case GCU_ALARMS::Under_Speed_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_UnderSpeed, 5, 16, 33, 46);
                  break;
              }
              case GCU_ALARMS::R_Over_Voltage_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_L1_PhaseOverVoltage, 11, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::R_Under_Voltage_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_L1_PhaseUnderVoltage, 11, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::Y_Over_Voltage_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_L2_PhaseOverVoltage, 11, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::Y_Under_Voltage_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_L2_PhaseUnderVoltage, 11, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::B_Over_Voltage_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_L3_PhaseOverVoltage, 11, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::B_Under_Voltage_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_L3_PhaseUnderVoltage, 11, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::Over_Frequency_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_OverFrequency, 5, 16, 33, 42);
                  break;
              }
              case GCU_ALARMS::Under_Frequency_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_UnderFrequency, 5, 16, 33, 42);
                  break;
              }
              case GCU_ALARMS::Emergency_Stop_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_EmergencyStop, 9, 16, 33, 30);
                  break;
              }
              case GCU_ALARMS::Charge_Fail_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_ChargeFail, 9, 16, 33, 28);
                  break;
              }
              case GCU_ALARMS::Battery_Over_Voltage_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_BatteryOverVoltage, 11, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::Battery_Under_Voltage_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_BatteryUnderVoltage, 11, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::Filter_maintenance_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_MaintenanceDue, 9, 16, 33, 30);
                  break;
              }
              case GCU_ALARMS::Fail_To_Stop_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_FailToStop, 9, 16, 33, 26);
                  break;
              }
              case GCU_ALARMS::Fuel_Theft_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_FuelTheft, 9, 16, 33, 30);
                  break;
              }
              case GCU_ALARMS::Fail_To_Start_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_FailToStart, 9, 16, 33, 30);
                  break;
              }
              case GCU_ALARMS::Gen_Phase_Rotation_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_DGPhaseReversed, 12, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::Phase_EB_Rotation_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_MainsPhaseReversed, 11, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::Over_Load_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_OverLoad, 5, 16, 33, 46);
                  break;
              }
              case GCU_ALARMS::V_Belt_Broken_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_VBeltBroken, 12, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::Over_Current_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_OverCurrent, 7, 16, 33, 36);
                  break;
              }
              case GCU_ALARMS::MPU_Loss_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_NoSpeedSignal, 12, 16, 33, 20);
                  break;
              }
              case GCU_ALARMS::Oil_Press_Detected_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_HighOilPressure, 11, 16, 24, 20);
                  if(_alarm.AlarmResultLatched(GCU_ALARMS::LLOP_SWITCH_AT_ENG_OFF))
                  {
                       _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_Switch, 6, 15, 46, 80);
                  }

                  if(_alarm.AlarmResultLatched(GCU_ALARMS::OIL_PRESS_DETECTED))
                  {
                       _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_Sensor, 6, 16, 46, 5);
                  }
                  break;
              }
              case GCU_ALARMS::Engine_Temperature_Ckt_Open_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_Pin24CktOpen, 13, 16, 33, 16);
                  break;
              }
              case GCU_ALARMS::Oil_Pressure_Ckt_Open_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_Pin26CktOpen, 13, 16, 33, 16);
                  break;
              }
              case GCU_ALARMS::Fuel_Level_Ckt_Open_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_FuelLevelCktOpen, 13, 16, 33, 18);
                  break;
              }
              case GCU_ALARMS::Load_Unbalance_id:
              {    _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_UnbalancedLoad, 11, 16, 33, 26);
                  break;
              }
              case GCU_ALARMS::Lop_Curr_Sens_Open_Ckt_id:
              {    _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_Pin23CktOpen, 14, 16, 33, 9);
                  break;
              }
              case GCU_ALARMS::LOP_CURR_SENS_STB_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_ShrtToBatt, 14, 14, 33, 7);
                  break;
              }
#ifdef CAN_J1939
              case GCU_ALARMS::J1939_com_fail_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_CommFail, 8, 15, 34, 33);
                  break;
              }
              case GCU_ALARMS::J1939_Protect_Lamp_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_ProtectionLampOn, 7, 14, 35, 33);
                  break;
              }
              case GCU_ALARMS::J1939_Amber_Lamp_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_AmberLampOn, 9, 15, 35, 27);
                  break;
              }
              case GCU_ALARMS::J1939_Red_Lamp_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_RedLampOn, 7, 14, 35, 40);
                  break;
              }
              case GCU_ALARMS::J1939_Mil_Lamp_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChAlarm_MalfunctionLampOn, 7, 16, 34, 33);
                  break;
              }
              case GCU_ALARMS::J1939_Preheat_fail_id:
              {
                   _Disp.printImage((uint8_t *)stChiniAlarm.gau8ChinesePreheatFailedAlarm, 12, 14, 33, 20);
                  break;
              }
#endif
              default:
                  _bAlarminEnglish = true;
                  break;
          }
    }

    if((_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_ENGLISH)||_bAlarminEnglish
            || (_cfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)== CFGZ::LANGUAGE_SPANISH))
    {
        _Disp.gotoxy(GLCD_X(64),GLCD_Y(37));
        _Disp.printStringCenterAligned(
                (char *)strAlaram[_cfgz.GetArrLanguageIndex()][_alarm.GetAlarmId(u8AlarmScreenNum)],
                FONT_VERDANA);

        arrTemp[0]='\0';
        switch(_alarm.GetAlarmId(u8AlarmScreenNum))
        {
            case GCU_ALARMS::Low_Oil_Pressure_id:
            {
                if(_alarm.IsAlarmActive(GCU_ALARMS::LOW_OIL_PRESS_WARNING)
                   ||_alarm.IsAlarmActive(GCU_ALARMS::LOW_OIL_PRESS_SHUTDOWN))
                {
                    strcpy(arrTemp, "Sensor");
                }

                if(_alarm.IsAlarmActive(GCU_ALARMS::LLOP_SWITCH))
                {
                    strcpy(arrTemp1, "Switch");
                }
                break;
            }

            case GCU_ALARMS::fuel_level_id:
            {
                if(_alarm.IsAlarmActive(GCU_ALARMS::LOW_FUEL_LEVEL_SHUTDOWN)
                   ||_alarm.IsAlarmActive(GCU_ALARMS::LOW_FUEL_LEVEL_WARNING))
                {
                    strcpy(arrTemp, "Sensor");
                }

                if(_alarm.IsAlarmActive(GCU_ALARMS::LOW_FUEL_LVL_SWITCH))
                {
                    strcpy(arrTemp1, "Switch");
                }
                break;
            }

            case GCU_ALARMS::High_Water_Temperature_id:
            {
                if((_alarm.IsAlarmActive(GCU_ALARMS::HIGH_ENG_TEMP_SHUTDOWN)
                     ||_alarm.IsAlarmActive(GCU_ALARMS::HIGH_ENG_TEMP_WARNING))
                        && (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_THRESH_TYPE) == CFGZ::CFGZ_GREATER_THAN_THRESHOLD))
                {
                    strcpy(arrTemp, "Sensor");
                }

                if(_alarm.IsAlarmActive(GCU_ALARMS::HWT_SWITCH))
                {
                    strcpy(arrTemp1, "Switch");
                }

                break;
            }

            case GCU_ALARMS::Low_Water_Temperature_id:
             {
                 if((_alarm.IsAlarmActive(GCU_ALARMS::HIGH_ENG_TEMP_SHUTDOWN)
                      ||_alarm.IsAlarmActive(GCU_ALARMS::HIGH_ENG_TEMP_WARNING))
                         && (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_THRESH_TYPE) == CFGZ::CFGZ_LESS_THAN_THRESHOLD))
                 {
                     strcpy(arrTemp, "Sensor");
                 }
                 break;
             }

            case GCU_ALARMS::Oil_Press_Detected_id:
            {
                if(_alarm.AlarmResultLatched(GCU_ALARMS::OIL_PRESS_DETECTED))
                {
                    strcpy(arrTemp, "Sensor");
                }

                if(_alarm.AlarmResultLatched(GCU_ALARMS::LLOP_SWITCH_AT_ENG_OFF))
                {
                    strcpy(arrTemp1, "Switch");
                }
                break;
            }
        }

        _Disp.gotoxy(GLCD_X(2),GLCD_Y(50));
        _Disp.printStringLeftAligned((char *)arrTemp1, FONT_VERDANA);
        _Disp.gotoxy(GLCD_X(126),GLCD_Y(50));
        _Disp.printStringRightAligned((char *)arrTemp, FONT_VERDANA);
    }


}

void ALARM_UI::ClearAlarmScreenNum()
{
    u8AlarmScreenNum = 0;
}

void ALARM_UI::ChangeAlarmScreenType(ALARM_st eType)
{
    _eAlarmType = eType;
}

void ALARM_UI::prvDisplayDMScreen()
{
    unsigned int uiSpaces = 0;
    unsigned int uiLoopIndex;
    char arrTemp[32];
    J1939APP::J1939_DM_MSG_DECODE stDmMsg = {0};

    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.drawHorizontalLine(0, 11, 127);
    _Disp.gotoxy(64, 1);

    if( _eAlarmType == DM1)
    {
        _Disp.printStringCenterAligned((char *)"DM1",FONT_ARIAL);
        stDmMsg = _J1939.GetDM1Message(u8AlarmScreenNum) ;
    }
    else if(_eAlarmType == DM2)
    {
        _Disp.printStringCenterAligned((char *)"DM2",FONT_ARIAL);
        stDmMsg = _J1939.GetDM2Message(u8AlarmScreenNum);
    }

    if(!_u8NumberOfAlarms || _J1939.IsCommunicationFail())
    {
         sprintf(arrTemp," %d/%d", u8AlarmScreenNum, _u8NumberOfAlarms);
        _Disp.gotoxy(106, 1 );
        _Disp.printStringLeftAligned(arrTemp, FONT_VERDANA);

        if(DM1 == _eAlarmType)
        {
            _Disp.gotoxy(64, 32);
            _Disp.printStringCenterAligned((char*)strNoActiveDTC[_cfgz.GetArrLanguageIndex()], FONT_VERDANA);
        }
        else if(DM2 == _eAlarmType)
        {
            _Disp.gotoxy(64,32);
            _Disp.printStringCenterAligned((char*)strNoHistoricDTC[_cfgz.GetArrLanguageIndex()], FONT_VERDANA);
        }
    }
    else
    {
        if( (stDmMsg.u32SpnNo > 0) && (stDmMsg.u32SpnNo < 0x7FFFF) )
        {

            sprintf(arrTemp," %d/%d", u8AlarmScreenNum + 1, _u8NumberOfAlarms);
            _Disp.gotoxy(126, 1 );
            _Disp.printStringRightAligned(arrTemp, FONT_VERDANA);

            if(u8AlarmScreenNum > _u8NumberOfAlarms)
            {
                _u8NumberOfAlarms = u8AlarmScreenNum;
            }

            /* SPN string */
            _Disp.gotoxy(2 ,14 );
            sprintf(arrTemp,"SPN: %ld", stDmMsg.u32SpnNo);
            _Disp.printStringLeftAligned(arrTemp, FONT_VERDANA);

            /* FMI string */
            _Disp.gotoxy(58 ,14 );
            sprintf(arrTemp,"FMI: %d", stDmMsg.u8FMI);
            _Disp.printStringLeftAligned(arrTemp, FONT_VERDANA);

            /* OC string */
            _Disp.gotoxy(92 ,14 );
            sprintf(arrTemp,"OC: %d", stDmMsg.u8OC);
            _Disp.printStringLeftAligned(arrTemp, FONT_VERDANA);

            for(uiLoopIndex = 0; uiLoopIndex < NO_OF_SPNS_IN_DM; uiLoopIndex++)
            {
                if(gstDmNoString[uiLoopIndex].u32SpnNo == stDmMsg.u32SpnNo)
                {
                    break;
                }
            }

            if(uiLoopIndex < NO_OF_SPNS_IN_DM)
            {
                _Disp.gotoxy(2 ,25 );
                if(_cfgz.GetArrLanguageIndex() == CFGZ::LANGUAGE_SPANISH)
                {
                    _Disp.printStringLeftAligned(gstDmNoString[uiLoopIndex].pSpanishSpnName, FONT_VERDANA);
                }
                else
                {
                    _Disp.printStringLeftAligned(gstDmNoString[uiLoopIndex].pSpnName, FONT_VERDANA);
                }

            }
            if(stDmMsg.u8FMI < NO_OF_FMIS)
            {

                _Disp.gotoxy(2 ,50 );
                _Disp.printStringLeftAligned((char*)gstDM1FmiData[_cfgz.GetArrLanguageIndex()][stDmMsg.u8FMI].FmiStrings, FONT_VERDANA);
            }
            else if(stDmMsg.u8FMI == NO_TEXT_FMI)
            {
                _Disp.gotoxy(2 ,50 );
                _Disp.printStringLeftAligned((char*)" ", FONT_VERDANA);
            }
        }
        else
        {

            sprintf(arrTemp," %d/%d", 0, 0);
            _Disp.gotoxy(106, 1 );
            _Disp.printStringLeftAligned(arrTemp, FONT_VERDANA);
            _Disp.gotoxy(32, 64);
            if(DM1 == _eAlarmType)
            {
                _Disp.printStringCenterAligned((char*)strNoActiveDTC[_cfgz.GetArrLanguageIndex()], FONT_VERDANA);
            }
            else if(DM2 == _eAlarmType)
            {

                _Disp.printStringCenterAligned((char*)strNoHistoricDTC[_cfgz.GetArrLanguageIndex()], FONT_VERDANA);
            }
        }
    }
}

void ALARM_UI::prvDisplayPCDNCDScreen()
{
    char arrTemp[32];
    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.gotoxy(64, 1);
    if(_eAlarmType == PCD)
    {
        _Disp.printStringCenterAligned((char*)"PCD-DTC", FONT_ARIAL);
    }
    else if(_eAlarmType == NCD)
    {
        _Disp.printStringCenterAligned((char*)"NCD-DTC", FONT_ARIAL);
    }
    _Disp.drawHorizontalLine(0, 11, 127);

    if(!_u8NumberOfAlarms || _J1939.IsCommunicationFail())
    {
       sprintf(arrTemp," %d/%d", 0, 0);
       _Disp.gotoxy(106, 1);
       _Disp.printStringLeftAligned(arrTemp, FONT_VERDANA);

       _Disp.gotoxy(64, 32);
        if(_eAlarmType == PCD)
        {
            _Disp.printStringCenterAligned((char*)strNoActivePCDDTC[_cfgz.GetArrLanguageIndex()], FONT_VERDANA);
        }
        else if(_eAlarmType == NCD)
        {
            _Disp.printStringCenterAligned((char*)strNoActiveNCDDTC[_cfgz.GetArrLanguageIndex()], FONT_VERDANA);
        }
    }
    else
    {

        sprintf(arrTemp," %d/%d", u8AlarmScreenNum+1, _u8NumberOfAlarms);
        _Disp.gotoxy(126, 1);
        _Disp.printStringRightAligned(arrTemp, FONT_VERDANA);

        if(u8AlarmScreenNum > _u8NumberOfAlarms)
        {
            u8AlarmScreenNum = _u8NumberOfAlarms;
        }

        _Disp.gotoxy(2,25);

        if(_eAlarmType == PCD)
        {
            prvFillPCDAlarmArray();
            if(_u8ArrPCDAlarmForDisp[u8AlarmScreenNum] <  MAX_NUM_OF_PCD_NCD_ALARMS)
            {
                _Disp.printStringLeftAligned((char*)gstPcdNcdData[_cfgz.GetArrLanguageIndex()][_u8ArrPCDAlarmForDisp[u8AlarmScreenNum]].PcdNcdAlarms, FONT_VERDANA);
            }
        }
        else if(_eAlarmType == NCD)
        {
            prvFillNCDAlarmArray();
            if(TOTAL_PCD_ALARMS +_u8ArrNCDAlarmForDisp[u8AlarmScreenNum] <  MAX_NUM_OF_PCD_NCD_ALARMS)
            {
                _Disp.printStringLeftAligned((char*)gstPcdNcdData[_cfgz.GetArrLanguageIndex()][TOTAL_PCD_ALARMS +_u8ArrNCDAlarmForDisp[u8AlarmScreenNum]].PcdNcdAlarms, FONT_VERDANA);
            }
        }
    }
}


uint8_t ALARM_UI::prvFillPCDAlarmArray(void)
{
    uint8_t u8LoopIndex = 0;
    uint8_t v_array_index = 0;
    for(u8LoopIndex = 0; u8LoopIndex < TOTAL_PCD_ALARMS; u8LoopIndex++)
    {
        if((uint8_t)_J1939.GetReadData(RX_PGN_PROPB51_65361,u8LoopIndex) == 1)
        {
            _u8ArrPCDAlarmForDisp[v_array_index] = u8LoopIndex;
            v_array_index++;
        }
    }

    return(v_array_index);
}

/************************************************************************************************/

uint8_t ALARM_UI::prvFillNCDAlarmArray(void)
{
    uint8_t u8LoopIndex = 0;
    uint8_t v_array_index = 0;
    for(u8LoopIndex = 0; u8LoopIndex < TOTAL_NCD_ALARMS; u8LoopIndex++)
    {
        if((uint8_t)_J1939.GetReadData(RX_PGN_PROPB51_65361,(TOTAL_PCD_ALARMS + u8LoopIndex)) == 1)
        {
            _u8ArrNCDAlarmForDisp[v_array_index] = u8LoopIndex;
            v_array_index++;
        }
    }

    return(v_array_index);
}
