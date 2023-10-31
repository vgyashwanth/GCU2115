/*
 * CONST_UI.cpp
 *
 *  Created on: 27-Jul-2021
 *      Author: madhuri.abhang
 */

#include "CONST_UI.h"


const uint8_t u8BmpLogoInfo[3][20]=           /// info
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x78,0x78,0x58,0x18,0x18,0x18,0x18,0x7E,0x7E,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

const uint8_t u8BmpLogoGen[3][20]=
{
    {0x00,0x00,0x01,0x06,0x08,0x10,0x01,0x01,0x02,0x02,0x00,0x00,0x07,0x06,0x05,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x70,0x8C,0x03,0x00,0xC0,0x20,0x20,0x10,0x18,0x08,0x04,0x04,0x03,0x00,0xC0,0x31,0x0E,0x00,0x00},
    {0x00,0x00,0x00,0x00,0xA0,0x60,0xE0,0x00,0x00,0x40,0x40,0x80,0x80,0x08,0x10,0x60,0x80,0x00,0x00,0x00}
};

const uint8_t u8BmpLogoMains[3][20]=
{
    {0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x03,0x00,0x00,0x00,0x00,0x01,0x01,0x02,0x03,0x02,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x18,0x24,0xFF,0x42,0xFF,0xA5,0xA5,0xBD,0x99,0x24,0x42,0x81,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x80,0x00,0xC0,0x00,0x00,0x00,0x00,0x80,0x80,0x40,0xC0,0x40,0x00,0x00,0x00}
};


const uint8_t u8BmpLogoEng[3][20]=
{
    {0x00,0x00,0x01,0x06,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x06,0x05,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x70,0x8C,0x03,0x00,0x00,0x7C,0x44,0x44,0x7C,0x40,0x20,0x1C,0x00,0x00,0xC0,0x31,0x0E,0x00,0x00},
    {0x00,0x00,0x00,0x00,0xA0,0x60,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x60,0x80,0x00,0x00,0x00}
};

const uint8_t u8BmpLogoshelterTemp[3][20]=
{
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x18,0x24,0x24,0x34,0x24,0x34,0x24,0x34,0x24,0x34,0x24,0x7E,0xFF,0x7E,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

const uint8_t u8BmpLogoLoad[3][20]=
{
    {0x00,0x00,0x00,0x00,0x08,0x04,0x02,0x00,0x01,0x3A,0x02,0x02,0x02,0x3A,0x01,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x24,0x24,0x24,0x00,0x7E,0x81,0x00,0x00,0x00,0x00,0x00,0x18,0x24,0xA5,0x7E,0x42,0x7E,0x00},
    {0x00,0x00,0x00,0x00,0x10,0x20,0x40,0x00,0x80,0x5C,0x40,0x40,0x40,0x5C,0x80,0x00,0x00,0x00,0x00,0x00}
};

const char *strTimerStatus[1][14]=
{
   {
    " ",
    "Preheating:",
    "Start delay:",
    "Cranking:",
    "Crank rest:",
    "Safety monitoring:",
    "Cool down:",
    "Stop action:",
    "Addn stopping:",
    "Remaining minutes :",
    "Remaining minutes :",
    "Remaining minutes :",
    "GEN ON rem min:",
    "GEN OFF rem min:"
   }
};

const char *strGCUStatus[1][10]=
{
 {
     "  ",
     "Engine off-ready",
     "Starting attempt:",
     "Engine on-healthy",
     "Engine stopping",
     "Notification > Pls clr",
     "Warning alarm > Pls clr",
     "Electrical trip > Pls clr",
     "Shutdown alarm > Pls clr",
     "Engine off"
 }
};

const char *strHystogram[]=
{
     "0-20",
     "21-40",
     "41-60",
     "61-80",
     "81-100",
     "101+",

};
char Mains_status[32] = "MAINS READING";
const char *strMonScreens[1][50]=
{
    {
        "EXHAUST AFT TREAT",
        "LAMP ICONS",
        //Status & Info
        "STATUS",
        "PRODUCT INFO",
        "ENGINE TYPE",
        "CAN BUS INFO",
        "ENGINE LINK",
        //Generator
        "EGR MON",
        "GEN  VOLTAGE",
        "GEN  kW   LOAD",
        "GEN  kVA  LOAD",
        "GEN  kVAr LOAD",
        "GEN  PWR  FACTOR",
        "GEN  OUTPUT",
        "GEN  ENERGY",
        //Mains
        Mains_status,   //"MAINS   READING",
        "MAINS  kW   LOAD",
        "MAINS  kVA  LOAD",
        "MAINS  kVAr LOAD",
        "MAINS  OUTPUT",
        "MAINS  ENERGY",
        //Engine
        "BATTERY",
        "CHRG ALT",
        "AIR INTAKE TEMP",
        "BOOST  PRESS",
        "SITE BAT RUN",
        "TMPR RUN TIME",
        "ENG  TEMP",
        "ENG LUB OIL PR",
        "ENG REM FUEL",
        //Sensor
        "SHELTER TEMP",
        //Engine
        (char *)&strAuxS2String,
        "ENG SPEED",
        "ENG RUN TIME",
        "EEC1",
        "EEC2",
        "EOI",
        "AT1T1I1",
        "HOURS",
        "LFC1",
        "ET1",
        "EFL / P1",
        "AMBIENT CONDITIONS",
        "VEP1",
        "WFI",
        "DEFA",
        "IC1",
        "SHUTDN",
        "CSA",
        "LFE1",
    }
};


const char *strGCUMode[1][5]=
{
 {
  "TEST MODE",
  "MANUAL MODE",
  "AUTO MODE",
  "Auto- BTS Battery Backup",
  "Auto - Cyclic",
 }
};

const char *strIDLMode[]=
{
    "IDLE MODE - ACTIVE   ",
    "IDLE MODE - DEACTIVE"
};

const uint8_t u8ArrContactor1[6][16] =
{
    {0x00,0x00,0x00,0x00,0x00,0x60,0x90,0x9F,0x9F,0x90,0x60,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x1F,0x10,0x10,0x12,0x12,0xF2,0xF2,0x12,0x13,0x10,0x10,0x1F,0x00,0x00},
    {0x00,0x00,0xFF,0x00,0x00,0x1C,0x22,0x22,0x22,0x22,0x9C,0x00,0x00,0xFF,0x00,0x00},
    {0x00,0x00,0xFF,0x00,0x00,0x23,0x52,0x8A,0xFA,0x8A,0x8B,0x00,0x00,0xFF,0x00,0x00},
    {0x00,0x00,0xF8,0x08,0x08,0x88,0x48,0x4F,0x4F,0x48,0x88,0x08,0x08,0xF8,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x06,0x09,0xF9,0xF9,0x09,0x06,0x00,0x00,0x00,0x00,0x00}
};
/// -------------------------------------------------------------------------------------------
const uint8_t u8ArrContactor2[3][16] =
{
    {0x00,0x04,0x0E,0x3F,0x0A,0x3F,0x1B,0x15,0x1B,0x2A,0x24,0x2A,0x51,0x60,0x40,0x00},
    {0x00,0x00,0x00,0x80,0x00,0x80,0x00,0x03,0x03,0x80,0x80,0x80,0x40,0xC0,0x40,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x06,0x09,0xF9,0xF9,0x09,0x06,0x00,0x00,0x00,0x00,0x00}
};
/// -------------------------------------------------------------------------------------------
const uint8_t u8ArrContactor3[3][16] =
{
    {0x00,0x00,0x00,0x00,0x00,0x60,0x90,0x9F,0x9F,0x90,0x60,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x01,0x02,0x02,0xC2,0xC2,0x02,0x02,0x01,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x78,0x84,0x02,0x79,0x85,0x81,0x9D,0x85,0x79,0x02,0x84,0x78,0x00,0x00}
};
/// -------------------------------------------------------------------------------------------
const uint8_t u8ArrContactor4[1][10] =
{
    {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xFF,0xFF}
};
/// -------------------------------------------------------------------------------------------

const uint8_t u8ArrDollar[4][32]=        ///  Dollar
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x04,0x04,0x1F,0x3F,0x7F,0xE4,0xC4,0xC4,0xC4,0xE4,0xF4,0x7C,0x3F,0x1F,0x0F,0x04,0x04,0x04,0x84,0xC4,0xE4,0xF4,0x7F,0x3F,0x1F,0x07,0x04,0x04,0x00,0x00},
    {0x00,0x00,0x40,0x40,0xF0,0xF8,0xFC,0x5E,0x4F,0x47,0x43,0x40,0x40,0x40,0xE0,0xF0,0xF8,0x7C,0x5E,0x4F,0x47,0x47,0x47,0x4E,0xFC,0xF8,0xF0,0xC0,0x40,0x40,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};


const uint8_t u8ArrBattery[4][32] =       // LI Battery
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x02,0x02,0x1F,0x10,0x10,0x13,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xFF,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0xFF,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x40,0x40,0xF8,0x08,0x88,0xC8,0x88,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};


const uint8_t u8ArrEngineTemp[4][32] =        // LI Engine Temp
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x03,0x07,0x7F,0x7E,0x3E,0x1C,0x1C,0x1C,0x1C,0x3E,0x7E,0x7F,0x07,0x03,0x01,0x01,0x01,0x01,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0xC3,0xFF,0xFF,0xFF,0xC3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC3,0xFF,0xFF,0xFF,0xC3,0x81,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0xC0,0xE0,0xFE,0x7E,0x7C,0x38,0x38,0x38,0x38,0x7C,0x7E,0xFE,0xE0,0xC0,0x81,0x80,0x80,0x80,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x68,0x48,0x68,0x48,0x68,0x48,0x68,0x48,0x68,0x48,0xFC,0xFE,0xFC,0x00,0x00,0x00,0x00,0x00}
};


const uint8_t u8ArrFuel[4][32] =     // LI Fuel Pump
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0xFF,0x80,0x9F,0x90,0x96,0x90,0x9F,0x80,0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFF,0xFF,0xFF,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0xFE,0x02,0xF2,0x13,0xD2,0x12,0xF2,0x02,0xFE,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x02,0xFE,0xFF,0xFF,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x60,0x60,0x60,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x80,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00}
};

const uint8_t u8ArrOilPressure[4][32] =       // LI Oil Can - Pressure
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x07,0x04,0x04,0x04,0x04,0x04,0x0C,0x14,0x24,0x24,0x24,0x24,0x14,0x0C,0x04,0x07,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x84,0x02,0x01,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x82,0x84,0x88,0x91,0xA1,0xC2,0x82,0x84,0x84,0x88,0x88,0x90,0x90,0xA0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0xA0,0x40,0x40,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x70,0x70,0xF8,0xE8,0xD8,0x70,0x00,0x00,0x00,0x00,0x00,0x00}
};

const uint8_t u8ArrEngineHours[4][32] =       /// LI hour glass
{
    {0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00},
    {0x00,0xFF,0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0x7E,0x3F,0x1F,0x0F,0x07,0x03,0x01,0x01,0x03,0x05,0x09,0x11,0x21,0x41,0x83,0x07,0x0F,0x1F,0x3F,0x7F,0xFF,0xFF,0x00},
    {0x00,0xFF,0X00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7E,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x80,0xC0,0xA0,0x90,0x88,0x84,0x82,0xC1,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0xFF,0x00},
    {0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00}
};


const uint8_t u8ArrEngineSpeed[4][32] =       // LI RPM
{
    {0x00,0x00,0x00,0x00,0x00,0x05,0x06,0x07,0x00,0x00,0x00,0x00,0x01,0x01,0x02,0x02,0x02,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x07,0x18,0x60,0x80,0x00,0x00,0x03,0x0C,0x30,0x40,0x8C,0x12,0x12,0x21,0x21,0x00,0x00,0x00,0x00,0x80,0x40,0x30,0x0C,0x03,0x00,0x00,0x80,0x60,0x18,0x07,0x00},
    {0x00,0xE0,0x18,0x06,0x01,0x00,0x00,0xC0,0x30,0x0C,0x02,0x01,0x00,0x00,0x00,0x00,0x84,0x84,0x48,0x48,0x31,0x02,0x0C,0x30,0xC0,0x00,0x00,0x01,0x06,0x18,0xE0,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,0x40,0x40,0x40,0x80,0x80,0x00,0x00,0x00,0x00,0xE0,0x60,0xA0,0x00,0x00,0x00,0x00,0x00}
};


const uint8_t u8ArrAlarmLogo[3][20]=      //  Warning alarm
{
{0,0,0,0,0,1,2,4,4,4,4,4,4,8,8,15,0,0,0,0,},
{0,0,96,144,144,8,4,2,2,2,2,2,2,1,1,255,144,96,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
};

const uint8_t gau8LIShelterTemp[4][32] =  // shelter temp
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x06,0x0F,
     0x18,0x30,0x70,0x51,0x17,0x14,0x14,0x15,0x14,0x14,0x17,0x10,0x7F,0x00,
     0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x70,0xD8,0x8C,0x06,0x03,0xFF,
     0x00,0x00,0x00,0x8C,0xFF,0x01,0x81,0xCD,0x81,0x01,0xFF,0x00,0xFF,0x00,
     0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
     0xC0,0x60,0x70,0x50,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xF0,0x00,
     0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x24,0x24,0x34,0x24,
     0x34,0x24,0x34,0x24,0x34,0x24,0x34,0x24,0x34,0x24,0x7E,0xFF,0x7E,0x00,
     0x00,0x00,0x00,0x00}
};


const uint8_t gau8GeneratorVoltLogo[4][32] =
{
{0,0,0,3,4,8,16,32,33,34,6,72,64,64,32,32,32,16,8,4,3,0,0,0,0,0,0,0,0,0,},
{0,63,192,0,0,0,0,0,224,16,8,4,6,2,1,0,0,0,0,0,0,192,63,0,0,0,0,0,0,0,},
{0,192,48,12,2,1,0,0,0,0,0,0,1,2,4,200,48,0,1,2,12,48,192,0,0,0,0,0,0,0,},
{0,0,0,0,0,0,128,192,64,64,96,32,32,96,64,64,192,128,0,0,0,0,0,0,0,0,0,0,0,0,}
};

const uint8_t gau8GenPFLogo[4][30]=
{
{0,0,0,0,0,0,0,0,0,1,2,4,8,8,8,8,8,8,8,4,2,1,0,0,1,2,0,0,0,0,},
{0,0,0,0,0,0,0,63,192,128,0,0,0,0,1,2,4,4,8,16,32,224,224,159,0,0,0,0,0,0,},
{0,0,0,0,1,2,4,228,24,22,18,33,64,128,0,0,0,0,0,1,2,4,24,224,0,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,}
};

const char *strPhase[] = {"R","Y","B"};
const char *strPh_Ph[] = {"R-Y","Y-B","R-B"};
const char *strPower[] = {"kW", "kVA","kVAr"};

const char *StrPF={"PF"};
const char *StrA={"A"};
const char *strEnergy[]={"kWh","kVAh","kVArh"};

const char *StrOpenckt[2]={"Ckt Open","Circ. Ab"};
const char *StrOpnGndckt[2]={"Ckt Opn/Gnd","Circ. Ab/Tierra"};
const char *StrSTB[2]={"Short to battery","Corto de Bateria"};
const char *StrSTG={"Short To Gnd"};
const char *StrNotConfigured={"NA"};

const char *StrNightModeRestrict[2]={"Night Mode Restrict","Modo noct restring"};
const char *StrAutoExercise1[2]={"Exercise 1","EJERCICIO 1"};
const char *StrAutoExercise2[2]={"Exercise 2","EJERCICIO 2"};
const char *StrStarts[2]= {"STARTS" ,"Arranques"};
const char *StrTrips[2] = {"TRIPS","Disparos"};
const char *StrAutoAMF[2] = {"Auto - AMF","M Falla Red Auto"};
const char *StrAutoRemoteCmd[2]={"Auto-Remote Command","Comando Auto-remoto"};
const char *StrNoEvents[2]={"No Events","Sin eventos"};
const char *StrInvalidDate[2]={"Invalid Maintenance Date","Fecha no valida"};
const char *StrMaintenanceDateChanged[2]={"Maintenance Date Changed","Cambio de Fecha exitoso"};

const char *StrInfoDate[2]={"RT Clock:","Fecha    :"};
const char *StrMaintDate[1][2]=
{
 {"Invalid Maintenance Date","Maintenance Date Changed"}
};

const char *StrAutoExerciser[1][STR_AUTO_EXERCISER_LAST] =
{
 {
    "FREQ:",
    "FOR",
    "AT",
    "NEXT RUN:",
    "DAILY",
    "WEEKLY",
    "MONTHLY"
 }
};


const char *StrMainsStatus[1][STR_MAINS_STATUS_LAST] =
{
 {
  "MAINS NOT READ",
  "MAINS ABNORMAL",
  "MAINS HEALTHY",
  "MAINS SEQ FAIL",
  "MAINS FAILED"
 }
};

char strAuxAString[21],strAuxBString[21],strAuxCString[21];
char strAuxDString[21],strAuxEString[21],strAuxFString[21];
char strAuxGString[21],strAuxHString[21],strAuxIString[21];
char strAuxJString[21],strAuxKString[21],strAuxLString[21];
char strAuxMString[21],strAuxNString[21],strAuxOString[21],strAuxPString[21];
char strAuxS1String[21],strAuxS2String[21],strAuxS3String[21],strAuxS4String[21];
char strProfile[21];

const char *strAlaram[1][GCU_ALARMS::ID_ALL_ALARMS_LAST] =
{
 {
      (char*)"No Alarm",
      (char*)"Low Oil Pressure",
      (char*)"Low Fuel level",
      (char*)"High Eng Temp",
      (char*)"Low Water Level Switch",
      (char*)"Over Speed",
      (char*)"Gross Over Speed",
      (char*)"Under Speed",
      (char*)"R Phase Over Voltage",
      (char*)"R Phase Under Voltage",
      (char*)"Y Phase Over Voltage",
      (char*)"Y Phase Under Voltage",
      (char*)"B Phase Over Voltage",
      (char*)"B Phase Under Voltage",
      (char*)"Over Frequency",
      (char*)"Under Frequency",
      (char*)"Emergency Stop",
      (char*)"Charge Fail",
      (char*)"Battery Over Voltage",
      (char*)"Battery Under Voltage",
      (char*)"Over Current",
      (char*)"Maintenance Due",
      (char*)"Over Load",
      (char*)&strAuxAString,
      (char*)&strAuxBString,
      (char*)&strAuxCString,
      (char*)&strAuxDString,
      (char*)&strAuxEString,
      (char*)&strAuxFString,
      (char*)&strAuxGString,
      (char*)&strAuxHString,
      (char*)&strAuxIString,
      (char*)&strAuxJString,
      (char*)&strAuxKString,
      (char*)&strAuxLString,
      (char*)&strAuxMString,
      (char*)&strAuxNString,
      (char*)&strAuxOString,
      (char*)&strAuxPString,
      (char*)"Fail To Stop",
      (char*)"Fuel Theft",
      (char*)"Unbalanced Load",
      (char*)"No Speed Signal",
      (char*)"Fail To Start",
      (char*)"LOP Sens - Ckt Open/Gnd",
      (char*)"LOP Sensor - Ckt Open",
      (char*)"Eng Temp - Ckt Open",
      (char*)"DG Phase Reversed",
      (char*)"Mains Phase Reversed",
      (char*)"V Belt Broken",
      (char*)"LOP Sensor - Over Value",
      (char*)"Shelter Temp - Ckt Open",
      (char*)"Aux S2- Ckt Open",
      (char*)&strAuxS2String,
      (char*)"High Oil Pressure Detected",
      (char*)"Battery Charger Fail",
      (char*)"Smoke Fire",
      (char*)"Engine Start",
      (char*)"Engine Stop",
      (char*)"Config Modified By Master",
      (char*)"Config Modified By User",
      (char*)"High Shelter Temp",
      (char*)"High Canopy Fan Current",
      (char*)"Low Canopy Fan Current",
      (char*)"Invalid DG Run",
      (char*)"LOP Sens - Short to Batt",
      (char*)"Switching to Auto mode",
      (char*)"EGR Fault",  /* For notification */
      (char*)"EGR Fault",  /* For Shutdown */
      (char*)"Communication Failure",
      (char*)"Amber Lamp",
      (char*)"Red Lamp",
      (char*)"MIL Lamp",
      (char*)"Protect Lamp",
      (char*)"J1939 DTC",
      (char*)"EB MCCB Feedback ON",
      (char*)"DG MCCB Feedback ON",
 }
};

const char *StrMonth[1][12]=
{
 {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "July",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
 }
};


const char *strNoActiveDTC[2] = { "No Active DTC", "DTC sin activar"};
const char *strNoHistoricDTC[2] = {"No Historic DTC" ,"DTC sin historico"};
const char *strNoActivePCDDTC[2] = {"No Active PCD-DTC" ,"PCD-DTC sin activar"};
const char *strNoActiveNCDDTC[2] = {"No Active NCD-DTC" , "NCD-DTC sin activar"};

const char *strEventLog[2]= {"EVENT LOG","REG EVENTOS"};
const char *strRunHrs[2]= {"Run Hrs","Hrs op:"};
const char *strConfiguration[2] = {"CONFIGURATION","CONFIGURACION"};

const char *strPasswordChanged[2] = {"Password Changed","Contrasena Cambiada"};
const char *strPasswordAlreadyExist[2]={"Password already exists","Contrasena Existente"};
const char *strSavingSettings[2]={"Saving Settings...","Guardando config..."};


enum
{
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    WEEK_END
};
const char *StrDays[1][WEEK_END] =
{
 {
  "SUNDAY",
  "MONDAY",
  "TUESDAY",
  "WEDNESDAY",
  "THURSDAY",
  "FRIDAY",
  "SATURDAY"
 }
};

const char *strCANMsgRcvError[2] = {"Not Available" , "No disponible"};

 /* Icons for DPFC1 */
 uint8_t gau8Icon0[4][24]
 {
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x7E, 0x00, 0x00, 0x7F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00},
     {0x30, 0x3F, 0x3F, 0x30, 0x3F, 0x3F, 0x30, 0x3F, 0x3F, 0x30, 0x30, 0x33, 0x7B, 0x78, 0x30, 0x00, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00},
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x1F, 0x03, 0x01, 0x60, 0xE1, 0x83, 0x07, 0x00, 0x00, 0x07, 0x03, 0x81, 0xE0, 0x61, 0x03, 0x1F, 0x1E},
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xA0, 0xB0, 0x98, 0x1C, 0x0C, 0x0C, 0x1C, 0x98, 0xB0, 0xA0, 0x80, 0x80, 0x00, 0x00},
 };
 uint8_t gau8Icon1[5][22]
 {
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x7C, 0x00, 0x7C, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
     {0xFF, 0xFF, 0x00, 0x00, 0x63, 0x63, 0x00, 0x00, 0x63, 0x63, 0x00, 0x00, 0x63, 0x63, 0x00, 0x00, 0x63, 0x63, 0x00, 0x00, 0xFF, 0xFF},
     {0x80, 0x83, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x7E, 0x70, 0x01, 0x00, 0x00, 0x01, 0x70, 0x7E, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x83, 0x80},
     {0x00, 0xE0, 0x70, 0x38, 0x18, 0x08, 0x1A, 0x3B, 0x79, 0xF1, 0x00, 0x00, 0xF1, 0x79, 0x3B, 0x1A, 0x08, 0x18, 0x38, 0x70, 0xE0, 0x00},
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
 };
 uint8_t gau8Icon2[5][24]
 {
     {0x00, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00, 0x00, 0x3E, 0x7C, 0x00, 0x7C, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
     {0xFF, 0xFF, 0x00, 0x00, 0x63, 0xE3, 0xC0, 0x60, 0x73, 0x7B, 0x0C, 0x06, 0x63, 0x63, 0x00, 0x00, 0x63, 0x63, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00},
     {0x80, 0x83, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x7E, 0x70, 0x01, 0x00, 0x00, 0x01, 0xF0, 0x7E, 0x3E, 0x18, 0x0C, 0x06, 0x03, 0x83, 0x80, 0x00, 0x00},
     {0x00, 0xE0, 0x70, 0x38, 0x18, 0x08, 0x1A, 0x3B, 0x79, 0xF1, 0x00, 0x00, 0xF1, 0x79, 0x3B, 0x1A, 0x08, 0x18, 0x38, 0x70, 0xE0, 0x60, 0x30, 0x18},
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
 };

 /* Icons for AT1T1I */
 uint8_t gau8Icon3[4][27]
 {
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x03, 0x39, 0x70, 0x00, 0x00, 0x7F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x33, 0x1E},
     {0x08, 0x18, 0x18, 0x1C, 0x3E, 0x3E, 0x98, 0x80, 0x80, 0xC0, 0xE7, 0xEF, 0x80, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x66, 0x3C},
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x1F, 0x03, 0x01, 0x60, 0xE1, 0x83, 0x07, 0x00, 0x00, 0x07, 0x03, 0x81, 0xE0, 0x61, 0x03, 0x1F, 0x1E, 0x00, 0x87, 0xCD, 0x78},
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xA0, 0xB0, 0x98, 0x1C, 0x0C, 0x0C, 0x1C, 0x98, 0xB0, 0xA0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x08, 0x98, 0xF0}
 };
 uint8_t gau8Icon4[4][20]
 {
     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1E, 0x00, 0x1E, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
     {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x19, 0x19, 0x18, 0x18, 0x18, 0x18, 0x01, 0x01, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
     {0x0F, 0x01, 0x00, 0x00, 0x00, 0x38, 0xF8, 0xC1, 0x07, 0x00, 0x00, 0x07, 0xC1, 0xF8, 0x38, 0x00, 0x00, 0x00, 0x01, 0x0F},
     {0x80, 0xC0, 0xE0, 0x60, 0x20, 0x68, 0xEC, 0xE6, 0xC7, 0x03, 0x03, 0xC7, 0xE6, 0xEC, 0x68, 0x20, 0x60, 0xE0, 0xC0, 0x80}
 };

 /* Icons for DM1 */
 uint8_t gau8Icon6[6][27]
 {
     {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x03 ,0x03 ,0x63 ,0x63 ,0x63 ,0x63 ,0x7F ,0x7F ,0x63 ,0x63 ,0x63 ,0x03 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00},
     {0x1F ,0x1F ,0x00 ,0x00 ,0x3F ,0x3F ,0x30 ,0x30 ,0x30 ,0xF0 ,0xF0 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xF8 ,0xFC ,0x0E ,0x07 ,0x03 ,0x01 ,0x00 ,0x00},
     {0xFF ,0xFF ,0x60 ,0x60 ,0xFF ,0xFF ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x80 ,0xC0 ,0xFF ,0xFF},
     {0x80 ,0x80 ,0x00 ,0x00 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xFF ,0xFF ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0x00 ,0x00 ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0xFF ,0xFF},
     {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x3F ,0x3F ,0x31 ,0x31 ,0xF1 ,0xF1 ,0x01 ,0x01 ,0xF1 ,0xF1 ,0x31 ,0x31 ,0x31 ,0x3F ,0x3F ,0x00},
     {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x00}
 };
uint8_t gau8Icon7[6][27]
 {
     {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x03 ,0x03 ,0x63 ,0x63 ,0x63 ,0x63 ,0x7F ,0x7F ,0x63 ,0x63 ,0x63 ,0x03 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00},
     {0x1F ,0x1F ,0x00 ,0x00 ,0x3F ,0x3F ,0x30 ,0x30 ,0x30 ,0xF0 ,0xF0 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xF8 ,0xFC ,0x0E ,0x07 ,0x03 ,0x01 ,0x00 ,0x00},
     {0xFF ,0xFF ,0x60 ,0x60 ,0xFF ,0xFF ,0x00 ,0x00 ,0x00 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x30 ,0x00 ,0x30 ,0x30 ,0x00 ,0x00 ,0x00 ,0x80 ,0xC0 ,0xFF ,0xFF},
     {0x80 ,0x80 ,0x00 ,0x00 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xFF ,0xFF ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0x00 ,0x00 ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0xFF ,0xFF},
     {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x3F ,0x3F ,0x31 ,0x31 ,0xF1 ,0xF1 ,0x01 ,0x01 ,0xF1 ,0xF1 ,0x31 ,0x31 ,0x31 ,0x3F ,0x3F ,0x00},
     {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x00}
 };

 uint8_t gau8Icon8[6][27]
 {
     {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x03 ,0x03 ,0x63 ,0x63 ,0x63 ,0x63 ,0x7F ,0x7F ,0x63 ,0x63 ,0x63 ,0x03 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00},
     {0x1F ,0x1F ,0x00 ,0x00 ,0x3F ,0x3F ,0x30 ,0x30 ,0x30 ,0xF0 ,0xF0 ,0x00 ,0x0C ,0x0E ,0x0B ,0x09 ,0x09 ,0x08 ,0x00 ,0xF8 ,0xFC ,0x0E ,0x07 ,0x03 ,0x01 ,0x00 ,0x00},
     {0xFF ,0xFF ,0x60 ,0x60 ,0xFF ,0xFF ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x6A ,0xEA ,0xAA ,0x2A ,0x2A ,0x2B ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x80 ,0xC0 ,0xFF ,0xFF},
     {0x80 ,0x80 ,0x00 ,0x00 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xFF ,0xFF ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0xC0 ,0x00 ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0x03 ,0xFF ,0xFF},
     {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x3F ,0x3F ,0x31 ,0x31 ,0xF1 ,0xF1 ,0x01 ,0x01 ,0xF1 ,0xF1 ,0x31 ,0x31 ,0x31 ,0x3F ,0x3F ,0x00},
     {0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x80 ,0x00}
 };

 const uint8_t u8RLSImage[6][28] =
 {

 {   0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x03,    0x03,    0x42,    0x43,    0x43,    0x43,    0x63,    0x7F,    0x43,    0x43,    0x43,    0x43,    0x02,    0x03,    0x03,    0x00,    0x00,    0x00,    0x00,    0x00    },
 {   0x03,    0x03,    0x00,    0x00,    0x00,    0x1F,    0x1F,    0x18,    0xF8,    0xF0,    0x00,    0x00,    0x00,    0x07,    0x07,    0x04,    0x07,    0x07,    0x05,    0x05,    0x00,    0xF0,    0xFC,    0x1E,    0x07,    0x01,    0x00,    0x00    },
 {   0xFF,    0xFE,    0x20,    0x20,    0x20,    0xFF,    0xFF,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x08,    0x9C,    0x90,    0x18,    0x04,    0x34,    0x9C,    0x00,    0x00,    0x00,    0x00,    0x80,    0xE0,    0xFF,    0x3F    },
 {   0x00,    0x00,    0x00,    0x00,    0x00,    0xE0,    0xE0,    0x20,    0x20,    0x30,    0x3F,    0x3F,    0x03,    0x83,    0x83,    0x83,    0x83,    0x83,    0x80,    0xE0,    0x00,    0x03,    0x03,    0x03,    0x03,    0x03,    0xFF,    0xFF    },
 {   0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x0F,    0x0F,    0x08,    0x08,    0xF8,    0xF8,    0x00,    0x00,    0x00,    0xF8,    0x18,    0x08,    0x08,    0x0F,    0x0F,    0x00    },
 {   0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0x00,    0xC0,    0xC0,    0xC0,    0xC0,    0xC0,    0xC0,    0xC0,    0xC0,    0xC0,    0xC0,    0xC0,    0xC0,    0xC0,    0xC0,    0xC0,    0x00    }

 };


// const J1939_SPN_NAME_STRING gstDmNoString[NO_OF_SPNS_IN_DM] =
// {
//   { 16   , (char*)"Engine Fuel Filter Differen   tial Pressure"               , (char*)"Filtro Diferencial de Presion de Combustible de Motor"          }, // Engine Fuel Filter Differential Pressure
//   { 21   , (char*)"Engine ECU Temperature"                                    , (char*)"Temperatura de ECU de Motor"				                       },
//   { 22   , (char*)"Engine Extended Crankcase Blow-by Pressure"                , (char*)"Marcha Extendida por Presion de Aire"			               },
//   { 46   , (char*)"Pneumatic Supply Pressure"                                 , (char*)"Alimentacion de Presion Pneumatica"						       },
//   { 51   , (char*)"Engine Throttle Position"                                  , (char*)"Posicion de Ascelerador"							               },
//   { 52   , (char*)"Engine Intercooler Tempera ture"                           , (char*)"Temperatura de Intercooler"                                     }, // Engine Intercooler Temperature
//   { 72   , (char*)"Engine Blower Bypass Valve Position"                       , (char*)"Posicion de la valula Bypass de Soplador"                       },
//   { 73   , (char*)"Auxiliary Pump Pressure "                                  , (char*)"Bomba Auxiliar de Presion"			                           },
//   { 81   , (char*)"Engine  Particulate  Trap     Inlet Pressure"              , (char*)"Presion en la entrada de Trampa de particulas"                  }, // Engine Particulate Trap Inlet Pressure
//   { 82   , (char*)"Engine Air Start Pressure"                                 , (char*)"Presion de Aire de Arranque"			                           },
//   { 90   , (char*)"Power Takeoff Oil Tempera ture"                            , (char*)"Temperatura del aceite de la toma de fuerza"                    }, // Power Takeoff Oil Temperature
//   { 91   , (char*)"Accelerator Pedal Position 1"                              , (char*)"Posicion Pedal de Aceeracion 1"                                 },
//   { 92   , (char*)"Engine Percent Load At Cu rrent Speed"                     , (char*)"Porcentaje de carga a velocidad Actual"                         }, // Engine Percent Load At Current Speed
//   { 94   , (char*)"Engine Fuel Delivery Pressu re"                            , (char*)"Presion de Combustible de Salida"                               }, // Engine Fuel Delivery Pressure
//   { 95   , (char*)"Engine Fuel Filter  Differen   tial Pressure"              , (char*)"Presion diferencial en filtro de combustible"                    }, // Engine Fuel Filter Differential Pressure
//   { 96   , (char*)"Fuel Level"                                                , (char*)"Nivel de Comb" 								                       },
//   { 97   , (char*)"Water In Fuel Indicator"                                   , (char*)"Aguan en Indicador de Agua"                                     },
//   { 98   , (char*)"Engine Oil Level"                                          , (char*)"Nivel de Aceite de Motor"                                       },
//   { 99   , (char*)"Engine Oil Filter Differenti  al Pressure"                 , (char*)"Presion Diferencial en Filtr  o de Aceite"                        }, // Engine Oil Filter  Differential Pressure
//   { 100  , (char*)"Engine Oil Pressure"                                       , (char*)"Presion de Aceite de Motor"                                     },
//   { 101  , (char*)"Engine Crankcase Pressure"                                 , (char*)"Presion de Aceite Ciguenal"                                     },
//   { 102  , (char*)"Engine Turbocharger Boost Pressure"                        , (char*)"Presion de aumento  Turbocargador"                              },
//   { 103  , (char*)"Engine Turbocharger 1 Spee d"                              , (char*)"Velocidad de Turbocargador 1"                                   }, // Engine Turbocharger 1 Speed
//   { 104  , (char*)"Engine Turbocharger Lube   Oil Pressure 1"                 , (char*)"Presion de Aceite de TurboCargador 1"                           },
//   { 105  , (char*)"Engine Intake Manifold 1 Te mperature"                     , (char*)"Temperatura de Entrada de Manifold 1"                           }, // Engine Intake Manifold 1 Temperature
//   { 106  , (char*)"Engine Air Inlet Pressure"                                 , (char*)"Presion de Admision de Aire"                                    },
//   { 107  , (char*)"Engine Air Filter 1 Differen tial Pressure"                , (char*)"Presion Diferencial de Filtr  o de Aire 1"                        }, // Engine Air Filter 1 Differential Pressure
//   { 108  , (char*)"Barometric Pressure"                                       , (char*)"Presion Barometrica"                                            },
//   { 109  , (char*)"Engine Coolant Pressure"                                   , (char*)"Presion de Refrigerante de Motor"                               },
//   { 110  , (char*)"Engine Coolant Temperatur e"                               , (char*)"Temperatura de Refrigerante de Motor"                           }, // Engine Coolant Temperature
//   { 111  , (char*)"Engine Coolant Level"                                      , (char*)"Nivel de Refrigerante de Motor"                                 },
//   { 112  , (char*)"Engine Coolant Filter Diffe rential Pressure"              , (char*)"Presion Diferencial de Filtr  o de Refrigerante"                  }, // Engine Coolant Filter  Differential Pressure
//   { 114  , (char*)"Net Battery Current"                                       , (char*)"Corriente Neta de Bateria"                                      },
//   { 115  , (char*)"Alternator Current"                                        , (char*)"Corriente de Alternador"                                        },
//   { 124  , (char*)"Transmission Oil Level"                                    , (char*)"Nivel de Aceite de Transmision"                                 },
//   { 126  , (char*)"Transmission Filter Differe ntial Pressure"                , (char*)"Presion Diferencial de Filtr  o de Transmicion"                   }, // Transmission Filter Differential Pressure
//   { 127  , (char*)"Transmission Oil Pressure"                                 , (char*)"Presion de Aceite de Transmision"                               },
//   { 129  , (char*)"Engine Injector Metering    Rail 2 Pressure"               , (char*)"Pres. rail dosif 2 inyector motor"             		           }, // Engine Injector Metering Rail 2 Pressure
//   { 132  , (char*)"Engine Inlet Air Mass Flow  Rate"                          , (char*)"Tasa de Flujo de entrada de Masa de aire"                       }, // Engine Intake Air Mass Flow Rate
//   { 136  , (char*)"Auxiliary Vaccum Pressure  Reading"                        , (char*)"Lectura Presion de Vacio Auxiliar"                              }, // Auxiliary Vaccum Pressure Reading
//   { 137  , (char*)"Auxiliary Gage Pressure      Reading 1"                    , (char*)"Lectura de presion manometrica auxiliar 1"                      }, // Auxiliary Gage Pressure Reading 1
//   { 138  , (char*)"Auxiliary Absolute Pressur e Reading"                      , (char*)"Lectura de presion absoluta auxiliar"                           }, // Auxiliary Absolute Pressure Reading
//   { 156  , (char*)"Engine Injector Timing Rail 1 Pressure"                    , (char*)"Presion rail sinc 1 iny motor"                                  },
//   { 157  , (char*)"Engine Injector Metering    Rail 1 Pressure"               , (char*)"Pres. rail dosif 1 inyector motor"                              }, // Engine Injector Metering Rail 1 Pressure
//   { 158  , (char*)"Battery Potential(Voltage) , Switched"                     , (char*)"Potencial de la bateria (voltaje), conmutado"                   }, // Battery Potential (Voltage),Switched
//   { 159  , (char*)"Engine Gas Supply Pressure"                                , (char*)"Presion de Entrada de Gas"                                      },
//   { 164  , (char*)"Engine Injection Control Pr essure"                        , (char*)"Presion de control de inyeccion del motor"                      }, // Engine Injection Control Pressure
//   { 166  , (char*)"Engine Rated Power"                                        , (char*)"Potencia nominal del motor"                                     },
//   { 167  , (char*)"Charging System Potential  (Voltage)"                      , (char*)"Potencial del sistema de carga (voltaje)"                       },
//   { 168  , (char*)"Electrical Potential (Voltag e)"                           , (char*)"Potencial electrico (voltaje)"                                  }, // Electrical Potential (Voltage)
//   { 171  , (char*)"Ambient Air Temperature"                                   , (char*)"Temperatura Ambiente"                                           },
//   { 172  , (char*)"Engine Air Inlet Temperatur e"                             , (char*)"Temperatura de entrada de aire del motor"                       }, // Engine Air Intake Temperature
//   { 173  , (char*)"Engine Exhaust Gas Temper ature"                           , (char*)"Temperatura de los gases de escape del motor"                   }, // Engine Exhaust Gas Temperature
//   { 174  , (char*)"Engine Fuel Temperature 1"                                 , (char*)"Temperatura del combustible del motor 1"                        },
//   { 175  , (char*)"Engine Oil Temperature 1"                                  , (char*)"Temperatura del aceite del   motor 1"                             },
//   { 176  , (char*)"Engine Turbocharger Oil Te mperature"                      , (char*)"Temperatura del aceite del turbocompresor del motor"            }, // Engine Turbocharger Oil Temperature
//   { 177  , (char*)"Transmission Oil Temperatu re"                             , (char*)"Temperatura del aceite de transmision"                          }, // Transmission Oil Temperature
//   { 182  , (char*)"Engine Trip Fuel"                                          , (char*)"Combustible de viaje del motor"                                 },
//   { 183  , (char*)"Engine Fuel Rate"                                          , (char*)"Tasa de combustible del motor"                                  },
//   { 184  , (char*)"Engine Instantaneous Fuel   Economy"                       , (char*)"Economia de combustible instantanea del motor"                  }, //  Engine Instantaneous Fuel Economy
//   { 185  , (char*)"Engine  Average  Fuel  Econ omy"                           , (char*)"Economia de combustible promedio del motor"                     }, //  Engine Average Fuel Economy
//   { 188  , (char*)"Engine Speed At Idle, Point 1 (Engine Config.)"            , (char*)"Vel mot ralenti, punto 1 (config motor)"                        },
//   { 189  , (char*)"Engine Rated Speed"                                        , (char*)"Velocidad Nominal de Motor"                                     },
//   { 190  , (char*)"Engine Speed"                                              , (char*)"Velocidad de Motor"                                             },
//   { 235  , (char*)"Engine Total Idle Hours"                                   , (char*)"Horas totales de inactividad del motor"                         },
//   { 236  , (char*)"Engine Total Idle Fuel Used"                               , (char*)"Combustible total en ralenti usado del motor"                   },
//   { 247  , (char*)"Engine Total Hours of Oper ation"                          , (char*)"Horas totales de funcionamiento del motor"                      }, //  Engine Total Hours of Operation
//   { 250  , (char*)"Engine Total Fuel Used"                                    , (char*)"Combustible total usado del motor"                              },
//   { 411  , (char*)"Engine Exhaust Gas  Recirc ulation Diff. Pressure"         , (char*)"Pres dif recirc gases esc motor"                                }, //  Engine Exhaust Gas  Recirculation Diff. Pressure
//   { 412  , (char*)"Engine Exhaust Gas  Recirc ulation Temperature"            , (char*)"Temp rec de gas esc  motor"                                     }, //  Engine Exhaust Gas  Recirculation Temperature
//   { 441  , (char*)"Auxiliary Temperature 1"                                   , (char*)"Temperatura Auxiliar 1"                                         },
//   { 442  , (char*)"Auxiliary Temperature 2"                                   , (char*)"Temperatura Auxiliar 2"                                         },
//   { 444  , (char*)"Battery 2 Potential (Voltag e)"                            , (char*)"Potencial de la bateria 2     (voltaje)"                            }, //  Battery 2 Potential (Voltag e)
//   { 515  , (char*)"Engine's Desired Operating  Speed"                         , (char*)"Velocidad de funcionamiento deseada del motor"                  }, //  Engine's Desired Operating  Speed
//   { 518  , (char*)"Engine Requested Torque/T orque Limit"                     , (char*)"Par torsion solic mot / Lim torsion"                            }, //  Engine Requested Torque/Torque Limit
//   { 522  , (char*)"Percent Clutch Slip"                                       , (char*)"Porcentaje de deslizamiento del embrague"                       },
//   { 528  , (char*)"Engine Speed At Point 2 (En gine Config.)"                 , (char*)"Vel mot punto 2 (config motor)"                                 }, //  Engine Speed At Point 2 (Engine Config.)
//   { 529  , (char*)"Engine Speed At Point 3 (En gine Config.)"                 , (char*)"Vel mot punto 3 (config motor)"                                 }, //  Engine Speed At Point 3 (Engine Config.)
//   { 530  , (char*)"Engine Speed At Point 4 (E ngine Config.)"                 , (char*)"Vel mot punto 4 (config motor)"                                 }, //  Engine Speed At Point 4 (Engine Config.)
//   { 531  , (char*)"Engine Speed At Point 5 (En gine Config.)"                 , (char*)"Vel mot punto 5 (config motor)"                                 }, //  Engine Speed At Point 5 (Engine Config.)
//   { 532  , (char*)"Engine Speed At High Idle, P oint 6 (Engine Config.)"      , (char*)"Vel mot punto 6 (config motor)"                                 }, //  Engine Speed At High Idle, Point 6 (Engine Config.)
//   { 539  , (char*)"Engine Percent Torque At I dle,Point 1 (Engine Config.)"   , (char*)"Porcen  par motor ralenti, punto 1 (config mot)"                }, //  Engine Percent Torque At Idle,Point 1 (Engine Config.)
//   { 540  , (char*)"Engine Percent Torque At   Point 2 (Engine Config.)"       , (char*)"Porcen  par motor ralenti, punto 2 (config mot)"                },
//   { 541  , (char*)"Engine Percent Torque At   Point 3 (Engine Config.)"       , (char*)"Porcen  par motor ralenti, punto 3 (config mot)"                },
//   { 542  , (char*)"Engine Percent Torque At   Point 4 (Engine Config.)"       , (char*)"Porcen  par motor ralenti, punto 4 (config mot)"                },
//   { 543  , (char*)"Engine Percent Torque At   Point 5 (Engine Config.)"       , (char*)"Porcen  par motor ralenti, punto 5 (config mot)"                },
//   { 544  , (char*)"Engine Reference Torque (E ngine Config.)"                 , (char*)"Par de referencia del motor (configuracion del motor)"          }, //  Engine Reference Torque (Engine Config.)
//   { 1109 , (char*)"Engine Protection System   Approaching Shutdown"           , (char*)"sistema proteccion motor cerca de parar"                        },
//   { 1110 , (char*)"Engine Protection System    has Shutdown Engine"           , (char*)"sistema proteccion motor ha apagado motor"                      },
//   { 1119 , (char*)"Engine Actual Exhaust Oxy gen"                             , (char*)"Oxigeno de escape real del motor"                               }, //  Engine Actual Exhaust Oxygen
//   { 1122 , (char*)"Engine Alternator Bearing 1 Temperature"                   , (char*)"Temperatura del cojinete 1 del alternador del motor"            },
//   { 1123 , (char*)"Engine Alternator Bearing 2 Temperature"                   , (char*)"Temperatura del cojinete 2 del alternador del motor"            },
//   { 1124 , (char*)"Engine Alternator Winding 1 Temperature"                   , (char*)"Temperatura del devanado 1 del alternador del motor"            },
//   { 1125 , (char*)"Engine Alternator Winding 2 Temperature"                   , (char*)"Temperatura del devanado 2 del alternador del motor"            },
//   { 1126 , (char*)"Engine Alternator Winding 3 Temperature"                   , (char*)"Temperatura del devanado 3 del alternador del motor"            },
//   { 1127 , (char*)"Engine Turbocharger 1 Boo st Pressure"                     , (char*)"Turbocompresor del motor 1 Presion de refuerzo"                 }, //  Engine Turbocharger 1 Boost Pressure
//   { 1128 , (char*)"Engine Turbocharger 2 Boo st Pressure"                     , (char*)"Turbocompresor del motor 2 Presion de refuerzo"                 }, //  Engine Turbocharger 2 Boost Pressure
//   { 1129 , (char*)"Engine Turbocharger 3 Boo st Pressure"                     , (char*)"Turbocompresor del motor 3 Presion de refuerzo"                 }, //  Engine Turbocharger 3 Boost Pressure
//   { 1130 , (char*)"Engine Turbocharger 4 Boo st Pressure"                     , (char*)"Turbocompresor del motor 4 Presion de refuerzo"                 }, //  Engine Turbocharger 4 Boost Pressure
//   { 1131 , (char*)"Engine Intake Manifold 2 Te mperature"                     , (char*)"Temperatura del colector de admision del motor 2"               }, //  Engine Intake Manifold 2 Temperature
//   { 1132 , (char*)"Engine Intake Manifold 3 Te mperature"                     , (char*)"Temperatura del colector de admision del motor 3"               }, //  Engine Intake Manifold 3 Temperature
//   { 1133 , (char*)"Engine Intake Manifold 4 Te mperature"                     , (char*)"Temperatura del colector de admision del motor 4"               }, //  Engine Intake Manifold 4 Temperature
//   { 1135 , (char*)"Engine Oil Temperature 2"                                  , (char*)"Temperatura del aceite del motor 2"                             },
//   { 1136 , (char*)"Engine ECU Temperature"                                    , (char*)"Temperatura ECU Motor"                                          },
//   { 1137 , (char*)"Engine Exhaust Gas Port 1  Temperature"                    , (char*)"Temp puerto gases esc 1 motor"                                  },
//   { 1138 , (char*)"Engine Exhaust Gas Port 2  Temperature"                    , (char*)"Temp puerto gases esc 2 motor"                                  },
//   { 1139 , (char*)"Engine Exhaust Gas Port 3  Temperature"                    , (char*)"Temp puerto gases esc 3 motor"                                  },
//   { 1140 , (char*)"Engine Exhaust Gas Port 4 Temperature"                     , (char*)"Temp puerto gases esc 4   motor"                                  },
//   { 1141 , (char*)"Engine Exhaust Gas Port 5  Temperature"                    , (char*)"Temp puerto gases esc 5 motor"                                  },
//   { 1142 , (char*)"Engine Exhaust Gas Port 6  Temperature"                    , (char*)"Temp puerto gases esc 6 motor"                                  },
//   { 1143 , (char*)"Engine Exhaust Gas Port 7  Temperature"                    , (char*)"Temp puerto gases esc 7 motor"                                  },
//   { 1144 , (char*)"Engine Exhaust Gas Port 8  Temperature"                    , (char*)"Temp puerto gases esc 8 motor"                                  },
//   { 1145 , (char*)"Engine Exhaust Gas Port 9  Temperature"                    , (char*)"Temp puerto gases esc 9 motor"                                  },
//   { 1146 , (char*)"Engine Exhaust Gas Port 10 Temperature"                    , (char*)"Temp puerto gases esc 10 motor"                                 },
//   { 1147 , (char*)"Engine Exhaust Gas Port 11 Temperature"                    , (char*)"Temp puerto gases esc 11 motor"                                 },
//   { 1148 , (char*)"Engine Exhaust Gas Port 12 Temperature"                    , (char*)"Temp puerto gases esc 12 motor"                                 },
//   { 1149 , (char*)"Engine Exhaust Gas Port 13 Temperature"                    , (char*)"Temp puerto gases esc 13 motor"                                 },
//   { 1150 , (char*)"Engine Exhaust Gas Port 14 Temperature"                    , (char*)"Temp puerto gases esc 14 motor"                                 },
//   { 1151 , (char*)"Engine Exhaust Gas Port 15 Temperature"                    , (char*)"Temp puerto gases esc 15 motor"                                 },
//   { 1152 , (char*)"Engine Exhaust Gas Port 16 Temperature"                    , (char*)"Temp puerto gases esc 16 motor"                                 },
//   { 1153 , (char*)"Engine Exhaust Gas Port 17 Temperature"                    , (char*)"Temp puerto gases esc 17 motor"                                 },
//   { 1154 , (char*)"Engine Exhaust Gas Port 18 Temperature"                    , (char*)"Temp puerto gases esc 18 motor"                                 },
//   { 1155 , (char*)"Engine Exhaust Gas Port 19 Temperature"                    , (char*)"Temp puerto gases esc 19 motor"                                 },
//   { 1156 , (char*)"Engine Exhaust Gas Port 20 Temperature"                    , (char*)"Temp puerto gases esc 20 motor"                                 },
//   { 1157 , (char*)"Engine Main Bearing 1 Tempe rature"                        , (char*)"Temperatura del cojinete principal del motor 1"                 }, //  Engine Main Bearing 1 Temperature
//   { 1158 , (char*)"Engine Main Bearing 2 Tempe rature"                        , (char*)"Temperatura del cojinete principal del motor 2"                 }, //  Engine Main Bearing 2 Temperature
//   { 1159 , (char*)"Engine Main Bearing 3 Tempe rature"                        , (char*)"Temperatura del cojinete principal del motor 3"                 }, //  Engine Main Bearing 3 Temperature
//   { 1160 , (char*)"Engine Main Bearing 4 Temp erature"                        , (char*)"Temperatura del cojinete principal del motor 4"                 }, //  Engine Main Bearing 4 Temperature
//   { 1161 , (char*)"Engine Main Bearing 5 Tempe rature"                        , (char*)"Temperatura del cojinete principal del motor 5"                 }, //  Engine Main Bearing 5 Temperature
//   { 1162 , (char*)"Engine Main Bearing 6 Tempe rature"                        , (char*)"Temperatura del cojinete principal del motor 6"                 }, //  Engine Main Bearing 6 Temperature
//   { 1163 , (char*)"Engine Main Bearing 7 Tempe rature"                        , (char*)"Temperatura del cojinete principal del motor 7"                 }, //  Engine Main Bearing 7 Temperature
//   { 1164 , (char*)"Engine Main Bearing 8 Tempe rature"                        , (char*)"Temperatura del cojinete principal del motor 8"                 }, //  Engine Main Bearing 8 Temperature
//   { 1165 , (char*)"Engine Main Bearing 9 Tempe rature"                        , (char*)"Temperatura del cojinete principal del motor 9"                 }, //  Engine Main Bearing 9 Temperature
//   { 1166 , (char*)"Engine Main Bearing 10 Temp erature"                       , (char*)"Temperatura del cojinete principal del motor 10"                }, //  Engine Main Bearing 10 Temperature
//   { 1167 , (char*)"Engine Main Bearing 11 Temp erature"                       , (char*)"Temperatura del cojinete principal del motor 11"                }, //  Engine Main Bearing 11 Temperature
//   { 1168 , (char*)"Engine Turbocharger Lube   Oil Pressure 2"                 , (char*)"Pres aceite lub turbocompresor motor 2"                         },
//   { 1169 , (char*)"Engine Turbocharger 2 Spee d"                              , (char*)"Turbocompresor de motor de 2 velocidades"                       }, //  Engine Turbocharger 2 Speed
//   { 1170 , (char*)"Engine Turbocharger 3 Spee d"                              , (char*)"Turbocompresor de motor de 3 velocidades"                       }, //  Engine Turbocharger 3 Speed
//   { 1171 , (char*)"Engine Turbocharger 4 Spe ed"                              , (char*)"Turbocompresor de motor de 4 velocidades"                       }, //  Engine Turbocharger 4 Speed
//   { 1382 , (char*)"Engine Fuel Filter(suction si de) Differential Pressure"   , (char*)"Filt comb mot (lado succion) Pres dif"                          },  //  Engine Fuel Filter(suction side) Differential Pressure
//   { 514  , (char*)"Nominal Friction - Percent   Torque"                       , (char*)"Friccion Nominal - porcentaje de torque"                        },
//   { 519  , (char*)"Engine Des. Operating Speed Asymmetry Adjustment"          , (char*)"Diseno motor Ajuste de asim vel func"                           },
//   { 2978  ,  (char*)" Estimated Engine Parasitic Losses"                      , (char*)"Perdidas parasitarias estimadas del motor"                      },
//   { 3236 , (char*)"Aftertreat Exhaust Gas Mass Flow Rate"                     , (char*)"Caudal masico del gas de escape del tratamiento post"           },
//   { 1081 , (char*) "Engine Wait to Start Lamp"                                , (char*) "Lampara de espera para arrancar del motor"                     },
//   { 3357 , (char*) "Actual Maximum Percent Torque"                            , (char*) "Porcentaje de Torque Maximo Actual"                            },
//   { 1413  , (char*)"Cylinder 1 Ignition Timing"                               , (char*)"Tiempo de Ignicion Cilindro 1"                                  },
//   { 1390 ,  (char*)"Engine Fuel Valve 1 Intake Absolute Pressure"             , (char*)"Presion absoluta de Valvula de combustible 1"                   },
//   { 3353 , (char*)"Alternator 1 Status"                                       , (char*)"Estado de alternador 1"                                         },
//   { 7426,  (char*)"Operator Inducement Time to Torque Derate Level 1"         , (char*)"Tiempo de induccion operador reducir nivel de torsion 1"        },
//   { 7428, (char*)"Operator Inducement Time to Torque Derate Level 2"          , (char*)"Tiempo de induccion operador reducir nivel de torsion 2"        },
//   { 70 , (char*)"Parking Brake Switch"                                        , (char*)"Interruptor freno estacionamiento"                              },
//   { 84 ,(char*)"Wheel-Based Vehicle Speed"                                    , (char*)"Velocidad del vehiculo basado en ruedas"                        },
//   { 597,(char*)"Brake Switch"                                                 , (char*)"Switch de Freno"                                                },
//   { 899 ,(char*)"Engine Torque Mode"                                          , (char*)"Motor en Modo Torque"                                           },
//   { 975 ,(char*)"Engine Fan 1 Estimated Percent Speed"                        , (char*)"Vel porcentual estimada del ventilador del motor 1"             },
//   { 977 ,(char*)"Fan Drive State"                                             , (char*)"Estado  Vent"                                                   },
//   { 1639, (char*)"Fan Speed"                                                  , (char*)"Vel Ven"                                                        },
//   { 5078, (char*)"Engine Amber Warning Lamp Command"                          , (char*)"Comando al lamp advertencia ambar del motor"                    },
//   { 5079,(char*)"Engine Red Stop Lamp Command"                                , (char*)"Comando paro de la lampara roja del motor"                      },
//   { 1039 ,(char*)"Trip Fuel (Gaseous)"                                        , (char*)"Caida Comb (Gaseouso)"                                            },
//   { 1040, (char*)"Total Fuel Used (Gaseous)"                                  , (char*)"Comb total (Gaseouso)"                                      }
//};


// const char* arrPGNAbbreviations[1][NUMBER_OF_PGN_SCREENS]=
// {
//  {
//  (char*)"Ash Accumulation Time",
//  (char*)"KBT Message for HMI3",             //  PGN 65367 PROPB_57
//  (char*)"EMR EDC4 CAN STATUS",              //1 - If Engine type is EMR , PGN 65284, 1 SPN
//  (char*)"IVECO Engine Status 2",            //1 - If Engine type is IVECO ,PGN 65282, 3 SPNs
//  (char*)"Fuel Economy(Liquid)",             //2 - Fuel Economy(Liquid), PGN 65266, 1 SPN
//  (char*)"Electronic Eng Controller 2",      //3 - Electronic Engine Controller 2, PGN 61443, 2 SPN's
//  (char*)"Ambient Conditions",               //4 - Ambient Conditions, PGN 65269, 3 SPN's
//  (char*)"Engine Hours, Revolutions",        //5 - Engine Hours, Revolutions, PGN 65253, 1 SPN
//  (char*)"Vehicle Electrical Power 1",       //6 - Vehicle Electrical Power 1, PGN 65271, 2 SPN's
//  (char*)"Dash Display 1",                   //7 - Dash Display 1, PGN 65276, 2 SPN's
//  (char*)"Operator Indicators",              //8 - Operator Indicators, PGN 65279, 1 SPN
//  (char*)"Fuel Consumption(Liquid) 1",       //9 - Fuel Consumption(Liquid) 1, PGN 65257, 2 SPN's
//  (char*)"Elect Eng Controller 4",           //10 - Electronic Engine Controller 4, PGN 65214, 2 SPN's
//  (char*)"Fuel Information 1 (Liquid)",      //11 - Fuel Information 1 (Liquid), PGN 65203, 1 SPN
//  (char*)"Inlet/Exhaust Condition 2",        //12 - Inlet/Exhaust Conditions 2, PGN 64976, 2 SPN's
//  (char*)"Engine Operating Info",            //13 - Engine Operating Information, PGN 64914, 2 SPN's
//  (char*)"Engine Temperature 2",             //14 - Engine Temperature 2, PGN 65188, 1 SPN
//  (char*)"Electronic Eng Controller 3",      // Electronic engine controller 3.
//  (char*)"Electronic Eng Controller 3",      // Electronic engine controller 3.
//  (char*)"Service 2",                        // S2_65166
//  (char*)"Eng Fluid Level/Pressure 2",       // EFL_P2_65243
//  (char*)"Shutdown",                         // PGN_SHUTDOWN_65252
//  (char*)"Engine Gas Flow Rate",             // PGN_EGF1_61450
//  (char*)"DPF 1 Soot",                       // PGN_DPF1S_64796
//  (char*)"Engine Temperature 1",             //15 - Engine Temperature 1, PGN 65262, 3 SPN's
//  (char*)"Engine Temperature 1",             //16 - Engine Temperature 1, PGN 65262, 2 SPN's
//  (char*)"Electronic Eng Controller 1",      //17 - Electronic Engine Controller 1 (Screen 1), PGN 61444, 3 SPN's
//  (char*)"Electronic Eng Controller 1",      //18 - Electronic Engine Controller 1 (Screen 2), PGN 61444, 3 SPN's
//  (char*)"Eng Fluid Level/Pressure 1",       //19 - Engine Fluid Level/Pressure 1 (Screen 1), PGN 65263, 3 SPN's
//  (char*)"Eng Fluid Level/Pressure 1",       //20 - Engine Fluid Level/Pressure 1 (Screen 2), PGN 65263, 3 SPN's
//  (char*)"Inlet/Exhaust Condition 1",        //21 - Inlet/Exhaust Conditions 1 (Screen 1), PGN 65270, 4 SPN's
//  (char*)"Inlet/Exhaust Condition 1",        //22 - Inlet/Exhaust Conditions 1 (Screen 2), PGN 65270, 3 SPN's
//  (char*)"After Treatment 1",                //AT1IG1 - 23 - Aftertreatment 1 Intake Gas 1, PGN 61454, 2 SPN's
//  (char*)"After Treatment 1",                // PGN_A1DOC_64800
//  (char*)"After Treatment 1",                // PGN_AT1IG2_64948
//  (char*)"After Treatment 1",                // PGN_AT1IMG_64946
//  (char*)"After Treatment 1",                //AT1OG1 - 24 - Aftertreatment 1 Outlet Gas 1, PGN 61455, 2 SPN's
//  (char*)"After Treatment 1",                //A1SCRDSI1 - 25 - Aftertreatment 1 SCR Dosing System Information 1, PGN 61475,
//  (char*)"After Treatment 1",                //A1SCRDSI2 - 26 - Aftertreatment 1 SCR Dosing System Information 2, PGN 64833
//  (char*)"After Treatment 1",                //A1SCRDSR1 - 27 - Aftertreatment 1 SCR Dosing System Requests 1, PGN 61476
//  (char*)"After Treatment 1",                //A1SCREGT1 - 28 - Aftertreatment 1 SCR Exhaust Gas Temperature 1, PGN 64830
//  (char*)"After Treatment 2",                //AT2IG1 - 29 - Aftertreatment 2 Intake Gas 1, PGN 61456
//  (char*)"After Treatment 2",                //AT2OG1 - 30 - Aftertreatment 2 Outlet Gas 1, PGN 61457
//  (char*)"After Treatment 2",                //A2SCRDSI2 - 31 - Aftertreatment 2 SCR Dosing System Information 2, PGN 61478
//  (char*)"After Treatment 2",                //A2SCRDSI1 32 - Aftertreatment 2 SCR Dosing System Information 1, PGN 64827
//  (char*)"After Treatment 2",                //A2SCRDSR1 - 33 - Aftertreatment 2 SCR Dosing System Requests 1, PGN 61479
//  (char*)"After Treatment 2",                //A2SCREGTI - 34 - Aftertreatment 2 SCR Exhaust Gas Temperature 1, PGN 64824
//  (char*)"After Treatment 1",                //AT1OG2 - 35 - Aftertreatment 1 Outlet Gas 2, PGN 64947
//  (char*)"After Treatment 1",                //AT1S2 - 36 - Aftertreatment 1 Service 2, PGN 64697
//  (char*)"After Treatment 1",                //AT1S - 37 - Aftertreatment 1 Service, PGN 64891
//  (char*)"After Treatment 1",                //AT1T1I - 38 - Aftertreatment 1 Diesel Exhaust Fluid Tank 1 Information.
//  (char*)"NCD Inducement Strategy",
//  (char*)"KBT Message for PCD",              // PGN 65374 PropB_5E_1
//  (char*)"KBT Message for PCD",              // PGN 65374 PropB_5E_2
//  (char*)"KBT Message for PCD",              // PGN 65374 PropB_5E_3
//  (char*)"KBT Message for PCD",              // PGN 65374 PropB_5E_4
//  (char*)"KBT Message for PCD",              // PGN 65374 PropB_5E_5
//  (char*)"KBT Message for PCD",              // PGN 65374 PropB_5E_6
//  (char*)"Hatz ProStOut",                    //8 - Hatz ProStout Screen 1 , PGN 65364 , 4 SPNs
//  (char*)"Hatz ProStOut",                    //9 - Hatz ProStout Screen 2 , PGN 65364, 4 SPNs
//  (char*)"Hatz Proprietary Phys",            //10 - Hatz Proprietary Phys , PGN 65280, 2 SPNs
//  (char*)"Alternator Information",           //11 - Alternator information , PGN 65237, 1 SPN
//  (char*)"Elect Transmission Cntrl. 5",      //12 - Electronic Transmission Controller 5 , PGN 65219, 1 SPN
//  (char*)"Operator Inducement Info",         //13 - Operator Inducement Information , PGN 64554 , 2 SPNs
//  (char*)"Ignition Timing 1",                //5 - Ignition Timing 1 , PGN 65154 , 1 SPN
//  (char*)"Gaseous Fuel Pressure",            //6 - Gaseous Fuel Pressure , PGN 65163 , 1 SPN
//  (char*)"Intake Manifold Info 1",           //7 - Intake manifold Information 1 , PGN 65190 , 2 SPNs
//  (char*)"Fan Drive #1",                      //18 - Fan Drive #1, PGN 65213,3 SPN
//  (char*)"Direct Lamp Ctrl Cmd 1",           //19 - Direct Lamp Control Command 1, PGN 64775, 2 SPN
//  (char*)"Fuel Consumption(Gaseous)",        //5 - Fuel Consumption(Gaseous), PGN 65199, 2 SPN's
//  (char*)"Exhaust Port Temp 1",              //DISP_EPT1_PGN_65187,
//  (char*)"Exhaust Port Temp 2",              //DISP_EPT2_PGN_65186,
//  (char*)"Exhaust Port Temp 3",              //DISP_EPT3_PGN_65185,
//  (char*)"Exhaust Port Temp 4",              //DISP_EPT4_PGN_65184,
//  (char*)"Exhaust Port Temp 5",              //DISP_EPT5_PGN_65183,
//  (char*)"Engine Temperature 4",             //DISP_ET4_PGN_64870,
//  (char*)"Turbocharger Info 4",              //DISP_TCI4_PGN_65176,
//  (char*)"Eng Fluid Level/Pressure 12",      //DISP_EFL_P12_PGN_64735,
//  (char*)"Cruise Ctrl Vehicle Speed 1",      //14 - Cruise Ctrl Vehicle Speed 1, PGN 65265 , 3 SPNs
//  }
// };

 const char * const array_units[] =
 {
     ""  ,   //0
     "%" ,   //1
     "V" ,   //2
     "A" ,   //3
     "Bar" , //4
     "`C" ,  //5
     "sec" , //6
     "Ohm" , //7
     "Hz" ,  //8
     "RPM" , //9
     "Ltr" , //10
     "kW" ,  //11
     "Hrs",  //12
     "%/hour" , //13
     "RPM/sec" ,//14
     "Amp/ph" , //15
     "x25 Hz" , //16
     "% RPM error" ,//17
     "/5" ,     //18
     "V Ph-Ph" ,//19
     "V Ph-N",  //20
     "mA",      //21
     "Ltrs/hour",//22
     "Mins",     //23
     "kWh",      //24
     "kVAh",     //25
     "kVArh"    //26
     ,"deg"      //27
     ,"kL"       //28
     ,"ppm"      //29
     ,"g/h"      //30
     ,"h"        //31
     ,"L/h"      //32
 /* New unit string added by Aishwarya Bhandari on 4/2/2020 to solve JIRA Bug: SGC-78 */
     ,"rpm"      //33
     ,"bar"      //34
     ,"hrs"      //35
     ,"L"        //36
     ,"kWm"      //37
     ,"MPa"      // 38
     ,"kg/h"     // 39
     ,"g"        // 40
     ,"kPa"      // 41
     ,"km/h"    //42
     , "kg"        //43

 };

 const char *Approaching = {"Appr"};
 const char *NotApproaching = {"Nt Appr"};
 const char *Error = {"Error"};
 const char *Yes[2] ={"Yes","Si"};
 const char *No ={"No"};
 const char *NotAvailable = {"NA"};
 const char *strAlarms[2] = {"ALARMS" , "ALARMAS"};
 const char *strCharging = {"Chrging"};
 const char *StrNotCharging = {"Nt Chrg"};
 const char *StrLampIcons[2]= {"LAMP ICONS","Iconos de lamparas"};
 const char *StrExhaustAFT[2]={"EXHAUST AFT TREAT","Escape tratamiento"};
 const char *StrOff[2] = {"Off","Apagar"};
 const char *StrOn[2] ={"On","Ecender"};


 const char *StrPassword[2]    ={"PASSWORD","Contrasena"};
 const char *StrForReadMode[2] = {"For Read Mode","Modo Leer"};
 const char *StrPressStop[2]   ={"Press STOP","Presionar Paro"};
 const char *StrForWriteMode[2]={"For Write Mode","Modo Escribir"};
 const char *StrPressStart[2]  ={"Press START","Presionar Arranque"};

 const char *StrNotAvailable={"NA"};
 const char *StrCANErr={"CAN err"};
 const char *StrErr={"Error"};


 const char *StrEngOperatingState[MAX_NUM_OF_ENG_OPER_STATES]=
 {

     "Engine stopped",
     "Pre start",
     "Starting",
     "Warm-up",
     "Running",
     "Cool down",
     "Engine stopping",
     "Post run",
     "NA"
 };

 const char *StrPGN_65252_states[PGN_65252_STRING_OPTIONS]=
 {
     "OFF",
     "ON",
     "ERROR",
     "NOT AVAILABLE"

 };

 const char *StrPGN_64966_states[PGN_64966_STRING_OPTIONS]=
 {
     "Start Enable OFF",
     "Start Enable ON",
     "Reserved",
     "Not Available"

 };

 const char *StrWFIState[MAX_NUM_OF_WFI_STATES]=
 {
     "No",
     "Yes",
     "Error",
     "NA"
 };

 const char *StrDrvWarnState[MAX_NUM_OF_DRV_WARN]=
 {
     "off",
     "on-solid",
     "res SAE",
     "res SAE",
     "on-fast",
     "res SAE",
     "res SAE",
     "NA"
 };

 const char *StrDEFAStatus[MAX_NUM_OF_DEFA_STATE]=
 {
     "NO W/L",
     "W/L ON",
     "W/L blink",
     "W/L blink"
 };


 const char *EngType[]={"CONVENTIONAL", "ECU162"};

 const st_DTC J1939AlarmArrayStringsECU162[PCODE_LAST_ECU162] =
 {
     {   97  , 5 ,  (char *)"P2264",   (char *)"WATER IN FUEL Sens ERR DURING Sens TEST" },
     {   97  , 2 ,  (char *)"P2269",   (char *)"Water in fuel detected" },
     {   100  , 11 ,  (char *)"P0524",   (char *)"Min. Oil press error in plausibility" },
     {   100  , 3 ,  (char *)"P0523",   (char *)"Oil press sen shrt to Bat positive" },
     {   100  , 4 ,  (char *)"P0522",   (char *)"Oil pres sen shrt to Bat ground" },
     {   102  , 3 ,  (char *)"P0238",   (char *)"Signal RNG chk high in manifold press sen" },
     {   102  , 4 ,  (char *)"P0237",   (char *)"Signal RNG chk low in manifold press sen" },
     {   105  , 5 ,  (char *)"P0110",   (char *)"BOOST TEMP Sens volt High" },
     {   105  , 6 ,  (char *)"P0110",   (char *)"BOOST TEMP Sens volt Lo" },
     {   108  , 15 ,  (char *)"P2229",   (char *)"Amb. Air Press. Range check max" },
     {   108  , 17 ,  (char *)"P2228",   (char *)"Amb. Air Press. Range check min" },
     {   110  , 3 ,  (char *)"P0118",   (char *)"COOLANT TEMP Sens SIGNAL RNG CHK Hi" },
     {   110  , 4 ,  (char *)"P0117",   (char *)"COOLANT TEMP Sens SIGNAL RNG CHK Lo" },
     {   157  , 13 ,  (char *)"P0089",   (char *)"PRES. rlf vlv rchd max allowed opening cnt" },
     {   157  , 16 ,  (char *)"P0089",   (char *)"Pres rlf vlv is frcd to opn, perform press increase" },
     {   157  , 21 ,  (char *)"P0089",   (char *)"Rail press lmt vlv frcd to opn by press shocks" },
     {   157  , 14 ,  (char *)"P000F",   (char *)"Rail PRES. lmt valve opened" },
     {   157  , 31 ,  (char *)"P0089",   (char *)"Press rlf vlv reached max allowed open time" },
     {   157  , 3 ,  (char *)"P0193",   (char *)"RAIL PRES. Sens volt ABOVE UPPER LMT" },
     {   157  , 4 ,  (char *)"P0192",   (char *)"RAIL PRES. Sens volt BELo LoER LMT" },
     {   158  , 3 ,  (char *)"P0563",   (char *)"BATTERY volt Sens SIGNAL RNG CHK Hi" },
     {   158  , 4 ,  (char *)"P0562",   (char *)"BATTERY volt Sens SIGNAL RNG CHK Lo" },
     {   190  , 11 ,  (char *)"P0219",   (char *)"Over speed detect wrt eng rpm(hi idl LMT exceed)" },
     {   633  , 3 ,  (char *)"P0251",   (char *)"RAIL PRES. POSITIVE DEVIATION ERR" },
     {   633  , 4 ,  (char *)"P0251",   (char *)"RAIL PRES. NEGATIVE DEVIATION ERR" },
     {   633  , 5 ,  (char *)"P0088",   (char *)"Max Rail press LMT exceed" },
     {   633  , 14 ,  (char *)"P0088",   (char *)"Max Rail press LMT exceed" },
     {   636  , 8 ,  (char *)"P0341",   (char *)"CAMSHAFT Sens SIGNAL ERR" },
     {   636  , 5 ,  (char *)"P0340",   (char *)"CAMSHAFT NO SIGNAL" },
     {   637  , 2 ,  (char *)"P0336",   (char *)"CRANKSHAFT ERR SIGNAL" },
     {   637  , 5 ,  (char *)"P0335",   (char *)"CRANKSHAFT NO SIGNAL" },
     {   651  , 5 ,  (char *)"P0201",   (char *)"1st CYL. INJECT. OPEN LOAD ERR" },
     {   651  , 3 ,  (char *)"P0262",   (char *)"INJECT. 1 SHRT ckt FOR CYL." },
     {   651  , 4 ,  (char *)"P0261",   (char *)"CYL. 1 SHRT ckt FROM Lo SIDE TO Hi SIDE" },
     {   652  , 5 ,  (char *)"P0203",   (char *)"3rd CYL. INJECT. OPEN LOAD ERR" },
     {   652  , 3 ,  (char *)"P0268",   (char *)"INJECT. 3 SHRT ckt FOR CYL." },
     {   652  , 4 ,  (char *)"P0267",   (char *)"CYL. 3 SHRT ckt FROM Lo SIDE TO Hi SIDE" },
     {   653  , 5 ,  (char *)"P0204",   (char *)"4th CYL. INJECT. OPEN LOAD ERR" },
     {   653  , 3 ,  (char *)"P0271",   (char *)"INJECT. 4 SHRT ckt FOR CYL." },
     {   653  , 4 ,  (char *)"P0270",   (char *)"CYL. 4 SHRT ckt FROM Lo SIDE TO Hi SIDE" },
     {   654  , 5 ,  (char *)"P0202",   (char *)"2nd CYL. INJECT. OPEN LOAD ERR" },
     {   654  , 3 ,  (char *)"P1202",   (char *)"INJECT. 2 SHRT ckt FOR CYL." },
     {   654  , 4 ,  (char *)"P0265",   (char *)"INJECT. 2 SHRT ckt FOR CYL." },
     {   898  , 31 ,  (char *)"P062B",   (char *)"ECU mon err for injection cutoff demand" },
     {   1136  , 3 ,  (char *)"P0666",   (char *)"Signal RNG chk high-ECU temp sen" },
     {   1136  , 4 ,  (char *)"P0666",   (char *)"Signal RNG chk low-ECU temp sen" },
     {   1687  , 3 ,  (char *)"P0699",   (char *)"Error Sensor supplies 3" },
     {   2432  , 2 ,  (char *)"P0611",   (char *)"DFC torq. Integral value" },
     {   2432  , 3 ,  (char *)"P0611",   (char *)"DFC drive limitation" },
     {   2791  , 5 ,  (char *)"P0403",   (char *)"Opn load err for EGR pwr stage" },
     {   2791  , 6 ,  (char *)"P0488",   (char *)"Ovr current err for EGR vlv H-bri" },
     {   2791  , 12 ,  (char *)"P0488",   (char *)"Ovr current err for EGR vlv H-bri" },
     {   2791  , 3 ,  (char *)"P0490",   (char *)"Motor pos shrt ckt to Bat err of EGR vlv H-Bri" },
     {   2791  , 4 ,  (char *)"P0489",   (char *)"Motor pos shrt ckt to Gnd err of EGR vlv H-Bri" },
     {   2791  , 17 ,  (char *)"P0488",   (char *)"Shrt ckt ovr load err for EGR vlv H-Bri" },
     {   2791  , 18 ,  (char *)"P0488",   (char *)"Temp depend ovr current err for EGR vlv H-Bri" },
     {   2791  , 14 ,  (char *)"P0488",   (char *)"Under vlt err fr EGR vlv H-Bri" },
     {   2791  , 7 ,  (char *)"P042F",   (char *)"EGR vlv jammed at closd pos" },
     {   2791  , 31 ,  (char *)"P042E",   (char *)"EGR vlv jammed at open position" },
     {   2791  , 1 ,  (char *)"P0406",   (char *)"Signl RNG chk hi- sens vltg for EGR vlv pos sens" },
     {   2791  , 2 ,  (char *)"P0405",   (char *)"Signl RNG chk lo- sens vltg for EGR vlv pos sens" },
     {   2802  , 3 ,  (char *)"P0563",   (char *)"BATTERY volt Sens SIGNAL RNG CHK Hi" },
     {   2802  , 4 ,  (char *)"P0562",   (char *)"BATTERY volt Sens SIGNAL RNG CHK Lo" },
     {   3509  , 3 ,  (char *)"P0643",   (char *)"Vltg flt at Sens spply 1" },
     {   3510  , 3 ,  (char *)"P0653",   (char *)"Vltg flt at Sens spply 2" },
     {   5314  , 15 ,  (char *)"P062B",   (char *)"ECU mon err for injection cutoff demand" },
     {   5358  , 2 ,  (char *)"P268C",   (char *)"Inj 1- IQA missing" },
     {   5359  , 2 ,  (char *)"P268E",   (char *)"Inj 3- IQA missing" },
     {   5360  , 2 ,  (char *)"P268F",   (char *)"Inj 4- IQA missing" },
     {   5361  , 2 ,  (char *)"P268F",   (char *)"Inj 4- IQA missing" },
     {   5838  , 2 ,  (char *)"P2BAF",   (char *)"EGR vlv blk grp Inducement lvl err 1" },
     {   5838  , 3 ,  (char *)"P2BAF",   (char *)"EGR vlv blk grp Inducement lvl err 3" },
     {   5838  , 15 ,  (char *)"P2BAC",   (char *)"EGR vlv blk grp Inducement lvl err 1" },
     {   5928  , 3 ,  (char *)"P045D",   (char *)"Motor neg shrt ckt to Bat err of EGR vlv H-Bri" },
     {   5928  , 4 ,  (char *)"P045C",   (char *)"Motor neg shrt ckt to GND fr EGR vlv H-Bri neg" },
     {   7026  , 5 ,  (char *)"P0251",   (char *)"FUEL METERING UNIT OPEN LOAD ERR" },
     {   7026  , 8 ,  (char *)"P0252",   (char *)"Over Temp. Of metering unit power stage" },
     {   7026  , 3 ,  (char *)"P0254",   (char *)"Shrt ckt to Bat positive of metering unit o/p" },
     {   7026  , 4 ,  (char *)"P0253",   (char *)"Shrt ckt to Gnd of metering unit o/p" },
     {   8518  , 2 ,  (char *)"P062D",   (char *)"shrt ckt HDEV pwr stage high side" },
     {   516098  , 11 ,  (char *)"P060B",   (char *)"Err chk no load test pulse oprerat." },
     {   516099  , 11 ,  (char *)"P060B",   (char *)"Err in plausibility of test vltg" },
     {   516100  , 11 ,  (char *)"P060B",   (char *)"Injection cutoff mon err" },
     {   516101  , 12 ,  (char *)"P060D",   (char *)"ACCELERATOR PEDAL MONITORING ERR" },
     {   516105  , 11 ,  (char *)"P061C",   (char *)"Engine speed mon err" },
     {   516106  , 11 ,  (char *)"P060A",   (char *)"INJECT. ENERGIZING TIME MONITORING ERR" },
     {   516107  , 11 ,  (char *)"P060A",   (char *)"DFC to report err due to plaus bet inj begin vs inj typ" },
     {   516110  , 11 ,  (char *)"P060A",   (char *)"DFC ICO error in the PoI2 shut-off" },
     {   516111  , 11 ,  (char *)"P060A",   (char *)"DFC ICO error in the PoI3 efficiency factor" },
     {   516113  , 11 ,  (char *)"P060A",   (char *)"DFC ICO error in total toruque relevant quantity" },
     {   516115  , 11 ,  (char *)"P060A",   (char *)"DFC error in rail pres monitor" },
     {   516117  , 11 ,  (char *)"P060A",   (char *)"Fault of ECU pwr train active" },
     {   516118  , 11 ,  (char *)"P061B",   (char *)"DFC energizing time comparison" },
     {   516120  , 11 ,  (char *)"P061B",   (char *)"DFC torque comparison error" },
     {   516121  , 11 ,  (char *)"P060A",   (char *)"error in the post-build slct monitor" },
     {   516122  , 14 ,  (char *)"P1003",   (char *)"EMM alarm FCCU0 status" },
     {   516123  , 14 ,  (char *)"P1006",   (char *)"DFC FCCU channel 103" },
     {   516124  , 14 ,  (char *)"P1005",   (char *)"EMM alarm FCCU10 status" },
     {   516125  , 14 ,  (char *)"P1007",   (char *)"DFC FCCU channel 111" },
     {   516126  , 14 ,  (char *)"P1008",   (char *)"DFC FCCU channel 115" },
     {   516127  , 14 ,  (char *)"P1009",   (char *)"EMM alarm FCCU12 status" },
     {   516128  , 14 ,  (char *)"P1010",   (char *)"EMM alarm FCCU18 status" },
     {   516129  , 14 ,  (char *)"P1011",   (char *)"EMM alarm FCCU19 status" },
     {   516130  , 14 ,  (char *)"P1004",   (char *)"EMM alarm FCCU1 status" },
     {   516131  , 14 ,  (char *)"P1013",   (char *)"EMM alarm FCCU20 status" },
     {   516132  , 14 ,  (char *)"P1014",   (char *)"EMM alarm FCCU22 status" },
     {   516133  , 14 ,  (char *)"P1015",   (char *)"EMM alarm FCCU25 status" },
     {   516134  , 14 ,  (char *)"P1016",   (char *)"EMM alarm FCCU28 status" },
     {   516135  , 14 ,  (char *)"P1012",   (char *)"EMM alarm FCCU2 status" },
     {   516136  , 14 ,  (char *)"P1018",   (char *)"EMM alarm FCCU31 status" },
     {   516137  , 14 ,  (char *)"P1019",   (char *)"EMM alarm FCCU32 status" },
     {   516138  , 14 ,  (char *)"P1020",   (char *)"EMM alarm FCCU33 status" },
     {   516139  , 14 ,  (char *)"P1021",   (char *)"EMM alarm FCCU34 status" },
     {   516140  , 14 ,  (char *)"P1022",   (char *)"EMM alarm FCCU35 status" },
     {   516141  , 14 ,  (char *)"P1023",   (char *)"EMM alarm FCCU36 status" },
     {   516142  , 14 ,  (char *)"P1024",   (char *)"EMM alarm FCCU37 status" },
     {   516143  , 14 ,  (char *)"P1025",   (char *)"EMM alarm FCCU38 status" },
     {   516144  , 14 ,  (char *)"P1026",   (char *)"EMM alarm FCCU39 status" },
     {   516145  , 14 ,  (char *)"P1017",   (char *)"EMM alarm FCCU3 status" },
     {   516146  , 14 ,  (char *)"P1027",   (char *)"EMM alarm FCCU40 status" },
     {   516147  , 14 ,  (char *)"P1028",   (char *)"EMM alarm FCCU41 status" },
     {   516148  , 14 ,  (char *)"P1029",   (char *)"EMM alarm FCCU42 status" },
     {   516149  , 14 ,  (char *)"P1030",   (char *)"EMM alarm FCCU43 status" },
     {   516150  , 14 ,  (char *)"P1031",   (char *)"EMM alarm FCCU44 status" },
     {   516151  , 14 ,  (char *)"P1032",   (char *)"EMM alarm FCCU45 status" },
     {   516152  , 14 ,  (char *)"P1033",   (char *)"EMM alarm FCCU46 status" },
     {   516153  , 14 ,  (char *)"P1034",   (char *)"EMM alarm FCCU49 status" },
     {   516154  , 14 ,  (char *)"P1036",   (char *)"EMM alarm FCCU50 status" },
     {   516155  , 14 ,  (char *)"P1037",   (char *)"EMM alarm FCCU51 status" },
     {   516156  , 14 ,  (char *)"P1038",   (char *)"EMM alarm FCCU52 status" },
     {   516157  , 14 ,  (char *)"P1039",   (char *)"EMM alarm FCCU53 status" },
     {   516158  , 14 ,  (char *)"P1040",   (char *)"EMM alarm FCCU54 status" },
     {   516159  , 14 ,  (char *)"P1041",   (char *)"EMM alarm FCCU55 status" },
     {   516159  , 31 ,  (char *)"P2505",   (char *)"Cj950, Cy327 and L9959 failure in drive" },
     {   516160  , 14 ,  (char *)"P1042",   (char *)"EMM alarm FCCU56 status" },
     {   516160  , 31 ,  (char *)"P2505",   (char *)"Cj950, Cy327 and L9959 failure in init." },
     {   516161  , 14 ,  (char *)"P1043",   (char *)"EMM alarm FCCU57 status" },
     {   516161  , 31 ,  (char *)"P2505",   (char *)"stop counter or bit flip failure" },
     {   516162  , 14 ,  (char *)"P1044",   (char *)"EMM alarm FCCU58 status" },
     {   516162  , 31 ,  (char *)"P2505",   (char *)"EEP read error" },
     {   516163  , 14 ,  (char *)"P1045",   (char *)"EMM alarm FCCU59 status" },
     {   516163  , 31 ,  (char *)"P2505",   (char *)"EEP write error" },
     {   516164  , 14 ,  (char *)"P1035",   (char *)"EMM alarm FCCU5 status" },
     {   516164  , 31 ,  (char *)"P2505",   (char *)"Read diag. For not red. Data" },
     {   516165  , 14 ,  (char *)"P1047",   (char *)"DFC FCCU channel 63" },
     {   516165  , 31 ,  (char *)"P2505",   (char *)"Stack memory threshold overrun" },
     {   516166  , 14 ,  (char *)"P1048",   (char *)"DFC FCCU channel 64" },
     {   516166  , 31 ,  (char *)"P2505",   (char *)"Irregular Switch off" },
     {   516167  , 14 ,  (char *)"P1049",   (char *)"DFC FCCU channel 65" },
     {   516168  , 14 ,  (char *)"P1046",   (char *)"DFC FCCU channel 6" },
     {   516169  , 14 ,  (char *)"P1050",   (char *)"DFC FCCU channel 70" },
     {   516170  , 14 ,  (char *)"P1051",   (char *)"DFC FCCU channel 71" },
     {   516171  , 14 ,  (char *)"P1052",   (char *)"DFC FCCU channel 74" },
     {   516172  , 14 ,  (char *)"P1053",   (char *)"DFC FCCU channel 75" },
     {   516173  , 14 ,  (char *)"P1054",   (char *)"DFC FCCU channel 78" },
     {   516174  , 14 ,  (char *)"P1055",   (char *)"DFC FCCU channel 79" },
     {   516175  , 14 ,  (char *)"P1056",   (char *)"DFC FCCU channel 80" },
     {   516176  , 14 ,  (char *)"P1057",   (char *)"DFC FCCU channel 81" },
     {   516177  , 14 ,  (char *)"P1058",   (char *)"EMM Alarm FCCU85 status" },
     {   516178  , 31 ,  (char *)"P060C",   (char *)"ABE active state DFC" },
     {   516179  , 11 ,  (char *)"P060C",   (char *)"WDA active by inquiry" },
     {   516180  , 31 ,  (char *)"P060C",   (char *)"WDA active by HW fault" },
     {   516181  , 3 ,  (char *)"P060C",   (char *)"WDA active by overvolt" },
     {   516186  , 31 ,  (char *)"P2505",   (char *)"Physical failure of Mrly" }

  };

 char *strEgrFault[]
 {
     (char*)"Egr Ecu Healthy",
     (char*)"Egr Ecu Faulty",
     (char*)"Vlv nt closing",
     (char*)"Vlv NT-Lift/SC",
     (char*)"Vlv snsr flt",
     (char*)"Vlv wire Open",
     (char*)"Temp snsr out",
     (char*)"Temp snsr flt"
 };
