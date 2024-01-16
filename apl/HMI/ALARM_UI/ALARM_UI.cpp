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
_eOldAlarmType(ALARM)
{

}

void ALARM_UI::Update(bool bRefresh)
{
    if(_cfgz.GetEngType()== CFGZ::CFGZ_CONVENTIONAL)
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
        default:
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
            default:
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
           {/*SuryaPranayTeja.Bvv 13-10-2022
              The below functionality is added as in NXP
               by UP key press also the Sounder Alarm gets turned off
               and we need to support it*/
               if(_hal.actuators.GetActStatus(ACTUATOR::ACT_AUDIBLE_ALARM) == ACT_Manager::ACT_LATCHED)
               {
                   _alarm.TurnOffSounderAlarm();
               }
           }
           if(u8AlarmScreenNum == 0)
           {
              u8AlarmScreenNum = _u8NumberOfAlarms;

              if(_eAlarmType == ALARM)
              {
                  if((_cfgz.GetEngType() != CFGZ::CFGZ_CONVENTIONAL))
                  {

                      _eAlarmType = DM1;
                  }
                  else
                  {
                      MON_UI::eDisplayMode = DISP_MON_MODE;
                      _eOldAlarmType = ALARM;
                  }
              }
              else if((_eAlarmType == DM1) && (_cfgz.GetEngType() != CFGZ::CFGZ_CONVENTIONAL))
              {
                 MON_UI::eDisplayMode = DISP_MON_MODE;
                 _eOldAlarmType = DM1;
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

              if(_eAlarmType == ALARM)
              {
                MON_UI::eDisplayMode = DISP_MON_MODE;
                if((_cfgz.GetEngType() != CFGZ::CFGZ_CONVENTIONAL))
                {
                  if(CFGZ::ECU_162 == _cfgz.GetEngType())
                  {
                      MON_UI::_stScreenNo = MON_UI::DISP_LAMP_ICONS;
                  }
                  else
                  {
                      MON_UI::_stScreenNo = MON_UI::DISP_EXAFTERTREAT_ICONS;
                  }
                }
                else
                {
                  MON_UI::_stScreenNo = MON_UI::DISP_MON_HOME;
                }
                _eOldAlarmType = ALARM;
              }
              else if((_eAlarmType == DM1) && (_cfgz.GetEngType() != CFGZ::CFGZ_CONVENTIONAL))
              {
                  _eAlarmType = ALARM;
              }
              else if (_eAlarmType == DM2)
              {
                _eAlarmType = ALARM;
              }

          }
           break;
       }
       case ACK_KEY_PRESS:
      {

          if(_hal.actuators.GetActStatus(ACTUATOR::ACT_AUDIBLE_ALARM) == ACT_Manager::ACT_LATCHED)
          {
              _alarm.TurnOffSounderAlarm();
          }
          _alarm.ClearAutoModeSwitchAlarm();
          _alarm.ClearAllAlarms();
          u8AlarmScreenNum = 0;

          MON_UI::eDisplayMode = DISP_MON_MODE;
          MON_UI::_stScreenNo = MON_UI::DISP_MON_HOME;
          MON_UI::eMonScreenType =MON_UI::MON_SCREEN_NORMAL;

          break;
      }

       case DN_LONG_PRESS:
       {
//           if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE))
           {
               if(_eAlarmType == DM2)
               {
                   MON_UI::eDisplayMode = DISP_MON_MODE;
                   MON_UI::_stScreenNo = MON_UI::DISP_MON_HOME;
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
    _Disp.drawHorizontalLine(GLCD_X(0), GLCD_Y(19), GLCD_X(128));

    {
        _Disp.drawVerticalLine(GLCD_X(22), GLCD_Y(0), GLCD_Y(19));
        _Disp.printImage((uint8_t *)u8ArrAlarmLogo, 3,20,0, 0);

        //Print screen name
        _Disp.gotoxy(GLCD_X(60),GLCD_Y(5));
        _Disp.printStringCenterAligned((char *)strAlarms[_cfgz.GetArrLanguageIndex()],FONT_ARIAL);
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

    {
        _Disp.gotoxy(GLCD_X(64),GLCD_Y(37));
        _Disp.printStringCenterAligned((char *)strAlaram[_cfgz.GetArrLanguageIndex()][_alarm.GetAlarmId(u8AlarmScreenNum)],FONT_VERDANA);

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
                        ||_alarm.IsAlarmActive(GCU_ALARMS::LOW_FUEL_LEVEL_NOTIFICATION))
                {
                    strcpy(arrTemp, "Sensor");
                }

                if(_alarm.IsAlarmActive(GCU_ALARMS::LFL_SWITCH))
                {
                    strcpy(arrTemp1, "Switch");
                }
                break;
            }

            case GCU_ALARMS::High_Water_Temperature_id:
            {
                if(_alarm.IsAlarmActive(GCU_ALARMS::HIGH_WATER_TEMP))
                {
                    strcpy(arrTemp, "Sensor");
                }
                if(_alarm.IsAlarmActive(GCU_ALARMS::HWT_SWITCH))
                {
                    strcpy(arrTemp1, "Switch");
                }
                break;
            }
            case GCU_ALARMS::High_Oil_Press_Detected_id:
            {
                if(_alarm.AlarmResultLatched(GCU_ALARMS::OIL_PRESS_MON))
                {
                    strcpy(arrTemp, "Sensor");
                }

                if(_alarm.AlarmResultLatched(GCU_ALARMS::LLOP_MON))
                {
                    strcpy(arrTemp1, "Switch");
                }
                break;
            }
        }

        _Disp.gotoxy(GLCD_X(2),GLCD_Y(50));
        _Disp.printStringLeftAligned((char *)arrTemp, FONT_VERDANA);
        _Disp.gotoxy(GLCD_X(126),GLCD_Y(50));
        _Disp.printStringRightAligned((char *)arrTemp1, FONT_VERDANA);
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

    char arrTemp[32];
    J1939APP::J1939_DM_MSG_DECODE stDmMsg = {};

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
    else
    {
        /* nothing */
    }

    if(!_u8NumberOfAlarms)
    {

         sprintf(arrTemp," %d/%d", u8AlarmScreenNum, _u8NumberOfAlarms);
        _Disp.gotoxy(106, 1 );
        _Disp.printStringLeftAligned(arrTemp, FONT_VERDANA);

        if(DM1 == _eAlarmType)
        {
            _Disp.gotoxy(64, 32);
            _Disp.printStringCenterAligned((char*)"No Active DTC", FONT_VERDANA);
        }
        else if (DM2 == _eAlarmType)
        {
            _Disp.gotoxy(64, 32);
            _Disp.printStringCenterAligned((char*)"No Previous DTC", FONT_VERDANA);
        }
        else
        {
            /* nothing */
        }
    }
    else
    {
        if( (stDmMsg.u32SpnNo > 0) && (stDmMsg.u32SpnNo < 0x7FFFF) )
        {

            sprintf(arrTemp," %d/%d", u8AlarmScreenNum + 1, _u8NumberOfAlarms);
            _Disp.gotoxy(102, 1 );
            _Disp.printStringLeftAligned(arrTemp, FONT_VERDANA);

            if(u8AlarmScreenNum > _u8NumberOfAlarms)
            {
                _u8NumberOfAlarms = u8AlarmScreenNum;
            }

            /* SPN string */
            _Disp.gotoxy(2 ,14 );
            sprintf(arrTemp,"SPN: %ld", stDmMsg.u32SpnNo);
            _Disp.printStringLeftAligned(arrTemp, FONT_VERDANA);

            /* FMI string */
            _Disp.gotoxy(60 ,14 );
            sprintf(arrTemp,"FMI: %d", stDmMsg.u8FMI);
            _Disp.printStringLeftAligned(arrTemp, FONT_VERDANA);

            static uint16_t uiLoopIndex = 0;
            if(CFGZ::ECU_162 == _cfgz.GetEngType()){
                for(uiLoopIndex = 0; uiLoopIndex < PCODE_LAST_ECU162; uiLoopIndex++)
                {
                    if(J1939AlarmArrayStringsECU162[uiLoopIndex].u32spn_no == stDmMsg.u32SpnNo  && (J1939AlarmArrayStringsECU162[uiLoopIndex].u8fmi == stDmMsg.u8FMI))
                    {
                        break;
                    }
                }

                if(uiLoopIndex < PCODE_LAST_ECU162)
                {
                    _Disp.gotoxy(2 ,25 );
                    sprintf(arrTemp,"PCODE: %s",((char*)J1939AlarmArrayStringsECU162[uiLoopIndex].FaultCode));
                    _Disp.printStringLeftAligned(arrTemp, FONT_VERDANA);
                    _Disp.gotoxy(2, 36);
                    _Disp.printStringLeftAligned((char *)J1939AlarmArrayStringsECU162[uiLoopIndex].Description, FONT_VERDANA);
                }
            }
            else{
                /*Do Nothing*/
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
                _Disp.printStringCenterAligned((char*)"No Active DTC", FONT_VERDANA);
            }
            else if (DM2 == _eAlarmType)
            {
                _Disp.gotoxy(64, 32);
                _Disp.printStringCenterAligned((char*)"No Previous DTC", FONT_VERDANA);
            }
            else
            {
                /* nothing */
            }
        }
    }
}
