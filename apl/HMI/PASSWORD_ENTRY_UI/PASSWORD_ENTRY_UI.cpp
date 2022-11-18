/**
 * @file        [PASSWORD_ENTRY_UI]
 * @brief       This module is used to password entry screens.
 *              This module will validate the entered password.
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        15th July 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#include <HMI/PASSWORD_ENTRY_UI/PASSWORD_ENTRY_UI.h>
#include "../UI/UI.h"
#include "../UI/CKeyCodes.h"

PASSWORD_ENTRY_UI::PASSWORD_ENTRY_UI(HAL_Manager &hal, CFGZ &cfgz, Display &Disp):
_hal(hal),
_cfgz(cfgz),
_Disp(Disp),
ArrChar{'#','#','#','#'},
_u16EnterdPassword(123),
_bPasswordStatus(false),
_eState(ENTRY),
_eCharNumber(CHAR1),
_u8EnteredPassword(0),
_u16PIN{0,0}
{

}

void PASSWORD_ENTRY_UI::Update(bool bRefresh)
{
   if(bRefresh)
   {
       switch(_eState)
       {
           case ENTRY:
           {
               prvDisplayEntryScreen();
               for(uint8_t i=CHAR1; i<CHAR_LAST; i++)
               {
                   ArrChar[i]='#';
               }
               _bPasswordStatus = false;
               _u8EnteredPassword = 0;
           }
           break;
           case PASSWORD_ENTRY:
           {
               prvDisplayPassEditScreen();
           }
           break;
           default:
               break;
       }
   }

}

bool PASSWORD_ENTRY_UI::CheckPasswordStatus()
{
    return _bPasswordStatus;
}

PASSWORD_ENTRY_UI::STATE_t PASSWORD_ENTRY_UI::GetPasswordEnteryState()
{
    return _eState;
}

void PASSWORD_ENTRY_UI::SetPasswordEntryState(STATE_t eState)
{
    _eState = eState;
}

void PASSWORD_ENTRY_UI::CheckKeyPress(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent)
{
   switch(_sKeyEvent )
   {
       case UP_SHORT_PRESS:  //Up key Press
      {
          //Increment Password Char Value
          ArrChar[_eCharNumber] = prvIncrementChar( ArrChar[_eCharNumber]);
      }
      break;
      case DN_SHORT_PRESS:  //Dn key Press
      {
          //Decrement password char value
          ArrChar[_eCharNumber] = prvDecrementChar( ArrChar[_eCharNumber]);
      }
      break;
      case START_KEY_SHORT_PRESS:  //Enter key Press
      {
          if(_eState != PASSWORD_ENTRY)
          {
              //Enter in Write Mode
              _eState = PASSWORD_ENTRY;
              _eCharNumber = CHAR1;
              ArrChar[CHAR1] = '0';
              _u8EnteredPassword = 0;
              _hal.Objeeprom.BlockingRead(EXT_EEPROM_PASWORD_START_ADDRESS, (uint8_t *)&_u16PIN, 4);
//              _hal.ObjDflash.Read(DFLASH_PASSWORD_START_ADDRESS , (uint8_t *)&_u16PIN, 4);
              _bPasswordStatus = false;
              break;
          }

          //Convert the number ASCII value into number.
          switch(_eCharNumber)
          {
              case CHAR1:
                  //one Thousand position
                  _u16EnterdPassword = (uint16_t)(((uint16_t)ArrChar[CHAR1] - ASCII_CONST) *1000U);
                  ArrChar[CHAR1] = '#';
                  ArrChar[CHAR2]  = '0';
                  _eCharNumber = CHAR2;
                  break;
              case CHAR2:
                  //Hundreds position
                  _u16EnterdPassword +=(uint16_t)(((uint16_t)ArrChar[CHAR2]- ASCII_CONST) *100U);
                  ArrChar[CHAR2] = '#';
                  ArrChar[CHAR3] = '0';
                  _eCharNumber = CHAR3;
                  break;
              case CHAR3:
                  // Tenth position
                  _u16EnterdPassword +=(uint16_t)(((uint16_t)ArrChar[CHAR3] - ASCII_CONST) *10U);
                  ArrChar[CHAR3]  = '#';
                  ArrChar[CHAR4]  = '0';
                  _eCharNumber = CHAR4;
                  break;
              case CHAR4:
                  //Ones position
                  _u16EnterdPassword +=(uint16_t)((uint16_t)ArrChar[CHAR4] - ASCII_CONST);
                  ArrChar[CHAR4] = '#';
                  ArrChar[CHAR1] = '0';
                  _eCharNumber = CHAR1;
                  prvCheckEnteredPassword();
                  break;

              default: break;
          }
      }
       break;
      default: break;
   }
}

uint8_t PASSWORD_ENTRY_UI::GetEnteredPassword()
{
    return _u8EnteredPassword;
}
void PASSWORD_ENTRY_UI::prvCheckEnteredPassword()
{
#ifdef CONFIG_MASTER_PASSWORD
    if(_u16EnterdPassword == _cfgz.GetCFGZ_Param(CFGZ::ID_MASTER_PASSWORD))
    {
        _u8EnteredPassword = PIN1;
    }
#else
    if(_u16EnterdPassword == _u16PIN[0])
    {
        _u8EnteredPassword = PIN1;
    }
#endif
    else if(_u16EnterdPassword == _u16PIN[1])  // Read from Password area in DFlash
    {
        _u8EnteredPassword = PIN2;
    }
//    else if(_u16EnterdPassword == 0)  // Read thirds Password
//    {
//        _u8EnteredPassword = PIN3;
//    }
//    else if(_u16EnterdPassword == MASTER_PASSWORD)  //Master Password
//    {
//        _u8EnteredPassword = MASTER_PIN;
//    }
    /*
     * Removed the Master password for GC2111 as per the requirement.
     */

    if(_u8EnteredPassword > 0)
    {
        _bPasswordStatus = true;
        _eState = ENTRY;
    }
    else
    {
        //Back to first char position.
        _bPasswordStatus = false;
        _eCharNumber = CHAR1;
        ArrChar[CHAR1] = '0';
        _u8EnteredPassword = 0;
    }

}

void PASSWORD_ENTRY_UI::prvDisplayPassEditScreen()
{
   char arrTemp[32];
   _Disp.ClearScreen();
   _Disp.drawRectangle();
   _Disp.drawHorizontalLine(GLCD_X(0), GLCD_Y(19), GLCD_Y(128));
   _Disp.gotoxy(GLCD_X(64),GLCD_Y(5));

   {
       _Disp.printStringCenterAligned((char *)StrPassword[_cfgz.GetArrLanguageIndex()],
                                  FONT_VERDANA);
   }

   _Disp.gotoxy(GLCD_X(64),GLCD_Y(35));
   sprintf(arrTemp,"%c %c %c %c",ArrChar[CHAR1],
           ArrChar[CHAR2], ArrChar[CHAR3], ArrChar[CHAR4]);
   _Disp.printStringCenterAligned((char *)arrTemp,
           FONT_VERDANA);
}

void PASSWORD_ENTRY_UI::prvDisplayEntryScreen()
{
    _Disp.ClearScreen();
    _Disp.drawRectangle();
    _Disp.gotoxy(GLCD_X(64),GLCD_Y(5));

    {
        _Disp.printStringCenterAligned((char *)StrForReadMode[_cfgz.GetArrLanguageIndex()],
                FONT_VERDANA);
        _Disp.gotoxy(GLCD_X(64),GLCD_Y(20));
        _Disp.printStringCenterAligned((char *)StrPressStop[_cfgz.GetArrLanguageIndex()],
                FONT_VERDANA);
        _Disp.gotoxy(GLCD_X(64),GLCD_Y(35));
        _Disp.printStringCenterAligned((char *)StrForWriteMode[_cfgz.GetArrLanguageIndex()],
                FONT_VERDANA);
        _Disp.gotoxy(GLCD_X(64),GLCD_Y(50));
        _Disp.printStringCenterAligned((char *)StrPressStart[_cfgz.GetArrLanguageIndex()],
                FONT_VERDANA);
    }
}

char PASSWORD_ENTRY_UI::prvIncrementChar(char ch)
{
    uint8_t u8Ch = ch;
    u8Ch++;
    if(u8Ch > '9')
    {
        u8Ch = '0';
    }
    return (char)u8Ch;
}
char PASSWORD_ENTRY_UI::prvDecrementChar(char ch)
{
    uint8_t u8Ch = ch;
    u8Ch--;
    if(u8Ch < '0')
    {
        u8Ch = '9';
    }
    return (char)u8Ch;
}
