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
//           if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE))
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
    {
        _Disp.drawVerticalLine(GLCD_X(22), GLCD_Y(0), GLCD_Y(19));
        _Disp.printImage((uint8_t *)u8ArrAlarmLogo, 3,20,0, 0);

        //Print screen name
        _Disp.gotoxy(GLCD_X(64),GLCD_Y(5));
        _Disp.printStringCenterAligned((char *)strAlarms[_cfgz.GetArrLanguageIndex()],
                    FONT_ARIAL);
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
        _Disp.printStringCenterAligned(
                (char *)strAlaram[_cfgz.GetArrLanguageIndex()][_alarm.GetAlarmId(u8AlarmScreenNum)],
                FONT_VERDANA);

        arrTemp[0]='\0';
        switch(_alarm.GetAlarmId(u8AlarmScreenNum))
        {
            case GCU_ALARMS::Low_Oil_Pressure_id:
            {

                break;
            }

            case GCU_ALARMS::fuel_level_id:
            {

                break;
            }

            case GCU_ALARMS::High_Water_Temperature_id:
            {

                break;
            }



            case GCU_ALARMS::High_Oil_Press_Detected_id:
            {

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
