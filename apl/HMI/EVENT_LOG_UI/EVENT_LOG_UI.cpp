/**
 * @file        [EVENT_LOG_UI]
 * @brief       This module is used to display events logs.
 *              This module will fetch the event log from using NV log module
 *              And displays the latest events. Up and down key short press for
 *              incrementing and decrementing. Up and down long press for mode
 *              change.
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        15th July 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#include <HMI/EVENT_LOG_UI/EVENT_LOG_UI.h>
#include "../UI_DS.h"

bool EVENT_LOG_UI::_bReadNVStatus = false;
bool EVENT_LOG_UI::_bNoEventPresnet= false;

EVENT_LOG_UI::EVENT_LOG_UI(HAL_Manager &hal, Display &Disp, CFGC &cfgc , GCU_ALARMS &gcuAlarms , CFGZ &cfgz):
_hal(hal),
_Disp(Disp),
_cfgc(cfgc),
_gcuAlarms(gcuAlarms),
_cfgz(cfgz),
_u16LatestFetchedEventNum(0),
_u16NumberOfEvents(0),
_u16DispEventScreenNumber(0),
_u16DispTotalEventNumber(0),
_stEventLog{0}
{

}

void EVENT_LOG_UI::Update(bool bEntry)
{
    static uint16_t u16PrvScreenNumber = _u16LatestFetchedEventNum;

    if(bEntry == true)
    {
        _bNoEventPresnet = false;
        _u16NumberOfEvents = (uint16_t)_gcuAlarms.GetCurrentEventNumber();

        if((_u16NumberOfEvents < _cfgc.GetMaxNumberOfEvents()) && (!_gcuAlarms.GetEventRolloverBit()))
        {
            _u16DispTotalEventNumber = _u16NumberOfEvents;
        }
        else
        {
            _u16DispTotalEventNumber = _cfgc.GetMaxNumberOfEvents();
        }

        if(_u16NumberOfEvents || _gcuAlarms.GetEventRolloverBit())
        {
            _u16DispEventScreenNumber = 1;
            if(_u16NumberOfEvents)
            {
                _u16LatestFetchedEventNum = _u16NumberOfEvents-1;
            }
            else
            {
                _u16LatestFetchedEventNum = 0;
            }
            _hal.Objeeprom.RequestRead((_u16LatestFetchedEventNum *sizeof(GCU_ALARMS::EVENT_LOG_t))+ EXT_EEPROM_EVENT_LOG_START_ADDRESS,
                                       (uint8_t*)&_stEventLog, sizeof(GCU_ALARMS::EVENT_LOG_t), prvEventReadCb);
        }
        else
        {
            _u16DispEventScreenNumber = 0;
            _u16LatestFetchedEventNum = 0;
            _bNoEventPresnet = true;
            prvDisplayNoEventScreen();
            return;
        }

    }
    else if(u16PrvScreenNumber !=_u16LatestFetchedEventNum)
    //Fetch Event according to event number, passing the struct.
    {

        _hal.Objeeprom.RequestRead((_u16LatestFetchedEventNum *sizeof(GCU_ALARMS::EVENT_LOG_t))+ EXT_EEPROM_EVENT_LOG_START_ADDRESS,
                                   (uint8_t*)&_stEventLog, sizeof(GCU_ALARMS::EVENT_LOG_t), prvEventReadCb);
    }
    prvDisplayEventScreen();

    u16PrvScreenNumber = _u16LatestFetchedEventNum;
}

void EVENT_LOG_UI::prvEventReadCb(EEPROM::EVENTS_t evt)
{
    if(evt == EEPROM::READ_COMPLETE)
    {
        _bReadNVStatus = true;
    }
}


void EVENT_LOG_UI::CheckKeyPress(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent)
{
    if(!_bNoEventPresnet)
    {
        switch(_sKeyEvent)
        {
            case UP_SHORT_PRESS:  //Up key Press
           {
               _u16DispEventScreenNumber--;
               if(_u16DispEventScreenNumber == 0)
               {
                   _u16DispEventScreenNumber = _u16DispTotalEventNumber;
               }

               _u16LatestFetchedEventNum++;
              if(_u16LatestFetchedEventNum >= _u16DispTotalEventNumber)
              {
                  _u16LatestFetchedEventNum = 0;
              }

           }
           break;

           case DN_SHORT_PRESS: //Dn key Press
           {
               _u16DispEventScreenNumber++;
               if(_u16DispEventScreenNumber > _u16DispTotalEventNumber)
               {
                   _u16DispEventScreenNumber = 1;
               }

               if(_u16LatestFetchedEventNum ==  (0))
               {
                   _u16LatestFetchedEventNum = _u16DispTotalEventNumber;
               }

              _u16LatestFetchedEventNum--;
           }
           break;
           default:
               break;
        }
    }
}

void EVENT_LOG_UI::prvDisplayEventScreen()
{
    char arrTemp[32];
    uint16_t u16XPos =2;
    if(_bReadNVStatus == true)
    {
        _bReadNVStatus = false;
        _Disp.ClearScreen();
        _Disp.drawRectangle();
        _Disp.drawHorizontalLine(GLCD_X(0), GLCD_Y(18), GLCD_Y(128));
        _Disp.drawVerticalLine(GLCD_X(90), GLCD_Y(0), GLCD_Y(18));
        //Print screen name
        _Disp.gotoxy(GLCD_X(45),GLCD_Y(5));
        _Disp.printStringCenterAligned((char *)strEventLog[_cfgz.GetArrLanguageIndex()],
                   FONT_ARIAL);
        _Disp.gotoxy(GLCD_X(126),GLCD_Y(5));
        sprintf(arrTemp," %d/%d",_u16DispEventScreenNumber,
               _u16DispTotalEventNumber );
        _Disp.printStringRightAligned((char *)arrTemp, FONT_VERDANA);



        _Disp.gotoxy(GLCD_X(2),GLCD_Y(20));
        sprintf(arrTemp,"%d %s %d ",_stEventLog.u8Day, StrMonth[_cfgz.GetArrLanguageIndex()][_stEventLog.u8Month-1], _stEventLog.u16Year);
        _Disp.printStringLeftAligned((char *)arrTemp, FONT_VERDANA);
        _Disp.gotoxy(GLCD_X(61),GLCD_Y(20));
        _Disp.printStringLeftAligned((char *)"Bar  kW   RPM", FONT_VERDANA);

        _Disp.gotoxy(GLCD_X(2),GLCD_Y(31));
        sprintf(arrTemp,"%02d:%02d:%02d ",_stEventLog.u8Hour, _stEventLog.u8Minute, _stEventLog.u8Second);
        _Disp.printStringLeftAligned((char *)arrTemp, FONT_VERDANA);
        _Disp.gotoxy(GLCD_X(57),GLCD_Y(31));
        if(_stEventLog.u16Lop == EVENT_LOG_LOP_SENSOR_NA)
        {

           _Disp.printStringLeftAligned((char *)"NA", FONT_VERDANA);
        }
        else if(_stEventLog.u16Lop == EVENT_LOG_LOP_SENSOR_OC)
        {

           _Disp.printStringLeftAligned((char *)"OC", FONT_VERDANA);
        }
        else
        {

           sprintf(arrTemp,"%.2f ",(float)_stEventLog.u16Lop/100 );
           _Disp.printStringLeftAligned((char *)arrTemp, FONT_VERDANA);
        }
        _Disp.gotoxy(GLCD_X(80),GLCD_Y(31));
        sprintf(arrTemp,"%.1f",(float)_stEventLog.u16GensetTotlKW/10);
        _Disp.printStringLeftAligned((char *)arrTemp, FONT_VERDANA);
        _Disp.gotoxy(GLCD_X(105),GLCD_Y(31));
        sprintf(arrTemp,"%d",_stEventLog.u16Speed);
        _Disp.printStringLeftAligned((char *)arrTemp, FONT_VERDANA);


        _Disp.gotoxy(GLCD_X(2),GLCD_Y(41));
        _Disp.printStringLeftAligned((char*)strRunHrs[_cfgz.GetArrLanguageIndex()], FONT_VERDANA);
        _Disp.gotoxy(GLCD_X(70),GLCD_Y(41));
        sprintf(arrTemp,"%luHrs %dmin", (_stEventLog.u32EngineHrs/60),
               (uint8_t)(_stEventLog.u32EngineHrs%60));
        _Disp.gotoxy(GLCD_X(126),GLCD_Y(41));
        _Disp.printStringRightAligned(arrTemp,      FONT_VERDANA);



        _Disp.gotoxy(GLCD_X(u16XPos),GLCD_Y(51));
        _Disp.printStringLeftAligned( (char*)strAlaram[_cfgz.GetArrLanguageIndex()][_stEventLog.u8EventId],
                                       FONT_VERDANA);
        u16XPos = u16XPos +_Disp.FindStringWidth( (char*)strAlaram[_cfgz.GetArrLanguageIndex()][_stEventLog.u8EventId],FONT_VERDANA);
        _Disp.gotoxy(GLCD_X(u16XPos),GLCD_Y(51));
        _Disp.printStringLeftAligned((char*)strAlaramType[_stEventLog.u8EventType],
                                      FONT_ARIAL);

    }
}

void EVENT_LOG_UI::prvDisplayNoEventScreen()
{
    char arrTemp[32];

    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.drawHorizontalLine(GLCD_X(0), GLCD_Y(18), GLCD_Y(128));
    _Disp.drawVerticalLine(GLCD_X(80), GLCD_Y(0), GLCD_Y(18));
    //Print screen name
    _Disp.gotoxy(GLCD_X(45),GLCD_Y(5));
    _Disp.printStringCenterAligned((char *)strEventLog[_cfgz.GetArrLanguageIndex()],
                FONT_ARIAL);
    _Disp.gotoxy(GLCD_X(126),GLCD_Y(5));
    sprintf(arrTemp," %d/%d",_u16DispEventScreenNumber+1,
            _u16DispTotalEventNumber+1 );
    _Disp.printStringRightAligned((char *)arrTemp, FONT_VERDANA);


    _Disp.gotoxy(GLCD_X(64),GLCD_Y(37));
    sprintf(arrTemp,"%s", (char *)StrNoEvents[_cfgz.GetArrLanguageIndex()]);
    _Disp.printStringCenterAligned((char *)arrTemp, FONT_ARIAL);

}
