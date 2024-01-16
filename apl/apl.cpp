/*
 * apl.cpp
 *
 *  Created on: 09-Apr-2021
 *      Author: sudeep.c
 */
//#include "config.h"
#include "hal_data.h"
#include "apl.h"
#include "HAL_Manager.h"
#include "MB_APP.h"
#include "APL_Manager/APL_Manager.h"
#include "config.h"

#define UNIT_TESTING 0

void APL_Main();

#if(UNIT_TESTING)
//#include "DEBUG_PRINT/DEBUG_PRINT.h"

//volatile static bool bFlashErased = false;
//volatile static DFLASH::FLASH_STATUS_t eFlashStatus;
//volatile static uint8_t su8UsbRecvBuffSz;
//volatile static uint8_t sau8UsbRecvBuff[20];
//volatile static bool    sbEEPROMTxComplete = false;
//volatile static KEYPAD::KEYPAD_EVENTS_t sKeyEvt;
//volatile static bool    sbKeyEvtAvailable = false;
//volatile static NV_LOG_STATUS_t _requestResult;
//A_SENSE::SENSOR_RET_t pressure, temperature, fuel, press4_20;
//float f32Vbat, f32ChargingAlt, f32RPMP, f32RPMalt, f32freq;
//float f32RPMPulseIp;
//float f32Pin26LowLevelShutDnThresh;
//int16_t s16Pin24HighTempShutDnThresh;
//char Arrary[20]={0};

void KeyPadTest();

void GLCD_Test();

void D_SENSE_Test();

void DFLASH_test();

void USB_CDC_Test();

void EEPROM_M95M01_Test();

void AC_SENSE_Test();

void MODBUS_Test();

void LED_Test();

void Virtual_EEPROM_Test();

void RTC_Test();

void HSD_Test();

void TestCfgz(void);

void PFLASH_Test();

void AnlgIP_Test();

void DIG_IP_Test();

void A_SENSE_Test();

void CFGZ_Test();

void flash_cb(DFLASH::FLASH_STATUS_t evt);

void usb_cb(uint8_t *pu8Data, uint16_t u16Len);

//void eeprom_cb(EEPROM::EVENTS_t evt);

void keypad_cb(KEYPAD::KEYPAD_EVENTS_t Evt);

void UpdatePID(float f32RPM);

void TestAnalog();

void StartStopKeyPress();

void TurnOnAllDigitalOutput();

void NV_LOG_Test();

#endif
void APL_Main()
{
    __enable_irq();
      APL_Manager AplManager;
//    BSP_Manager bsp;

    while(true)
    {
        AplManager.Update();
    }

#if(UNIT_TESTING)
    // StartStopKeyPress();
    // KeyPadTest();
    // D_SENSE_Test();
    // USB_CDC_Test();
    // DFLASH_test();
    // GLCD_Test();
    // AC_SENSE_Test();
    // MODBUS_Test();
    // Virtual_EEPROM_Test();
    // TestCfgz();
    // LED_Test();
    // RTC_Test();
    // HSD_Test();
    // ANLG_IP_Test();
    // AnlgIP_Test();
    // PFLASH_Test();
    // DIG_IP_Test();
    // CFGZ_Test();
    // NV_LOG_Test();
#endif

}
#if(UNIT_TESTING)
void NV_LOG_Test()
{
//    HAL_Manager hal;
//    NV_LOG_Interface NvLogInterface(hal.dflash);
//
//    stTimer tmr;
//
//    NV_LOG::NV_LOG_CFG_t NvLogCfg;
//
//    NvLogCfg.uLogSize               = 20;
//    NvLogCfg.u32BlockStartAddress   = DFLASH_EVENT_LOG_START_ADDRESS;
//    NvLogCfg.u16BlkSize             = 64;
////    NvLogCfg.uNoOfBlocks            = 96;
//    NvLogCfg.uNoOfBlocks            = 3;
//
//   eFlashStatus = DFLASH::DFLASH_NO_STATUS;
//   hal.dflash.RequestErase(DFLASH_EVENT_LOG_START_ADDRESS, 3, flash_cb);
//   while(eFlashStatus==DFLASH::DFLASH_NO_STATUS)
//   {
//       hal.Update();
//   }
//
//    NV_LOG NvLog(NvLogCfg, NvLogInterface);
//
//   uint8_t au8Data[20];
//
//   memset(au8Data,1,sizeof(au8Data));
//   _requestResult = NV_LOG_IN_PROGRESS;
//   NvLog.LogData(au8Data, sizeof(au8Data), nvlog_cb);
//   while(_requestResult != NV_LOG_SUCCESS)
//   {
//       hal.Update();
//       NvLog.Update();
//   }
//
//   memset(au8Data,2, sizeof(au8Data));
//   _requestResult = NV_LOG_IN_PROGRESS;
//   NvLog.LogData(au8Data, sizeof(au8Data), nvlog_cb);
//   while(_requestResult != NV_LOG_SUCCESS)
//   {
//       hal.Update();
//       NvLog.Update();
//   }
//
//   memset(au8Data,3,sizeof(au8Data));
//   _requestResult = NV_LOG_IN_PROGRESS;
//   NvLog.LogData(au8Data, sizeof(au8Data), nvlog_cb);
//   while(_requestResult != NV_LOG_SUCCESS)
//   {
//       hal.Update();
//       NvLog.Update();
//   }
//
//   memset(au8Data,4,sizeof(au8Data));
//   _requestResult = NV_LOG_IN_PROGRESS;
//   NvLog.LogData(au8Data, sizeof(au8Data), nvlog_cb);
//   while(_requestResult != NV_LOG_SUCCESS)
//   {
//       hal.Update();
//       NvLog.Update();
//   }
//
//   memset(au8Data,5,sizeof(au8Data));
//   _requestResult = NV_LOG_IN_PROGRESS;
//   NvLog.LogData(au8Data, sizeof(au8Data), nvlog_cb);
//   while(_requestResult != NV_LOG_SUCCESS)
//   {
//       hal.Update();
//       NvLog.Update();
//   }
//
//   memset(au8Data,6,sizeof(au8Data));
//   _requestResult = NV_LOG_IN_PROGRESS;
//   NvLog.LogData(au8Data, sizeof(au8Data), nvlog_cb);
//   while(_requestResult != NV_LOG_SUCCESS)
//   {
//       hal.Update();
//       NvLog.Update();
//   }
//
//   memset(au8Data,7,sizeof(au8Data));
//   _requestResult = NV_LOG_IN_PROGRESS;
//   NvLog.LogData(au8Data, sizeof(au8Data), nvlog_cb);
//   while(_requestResult != NV_LOG_SUCCESS)
//   {
//       hal.Update();
//       NvLog.Update();
//   }
//
//
//
//    uint8_t au8ReadData[20];
//
//    _requestResult = NV_LOG_IN_PROGRESS;
//    NvLog.Read(au8ReadData, 20, 4, nvlog_cb);
//    while(_requestResult != NV_LOG_SUCCESS)
//    {
//        hal.Update();
//        NvLog.Update();
//    }
//
//    bool bTestPass1 = true;
//    for(int i=0;i<sizeof(au8ReadData);i++)
//    {
//        if(au8ReadData[i] != 7)
//        {
//            bTestPass1 = false;
//            break;
//        }
//    }
//
//    _requestResult = NV_LOG_IN_PROGRESS;
//    NvLog.Read(au8ReadData, 20, 0, nvlog_cb);
//    while(_requestResult != NV_LOG_SUCCESS)
//    {
//        hal.Update();
//        NvLog.Update();
//    }
//    bool bTestPass2 = true;
//    for(int i=0;i<sizeof(au8ReadData);i++)
//    {
//        if(au8ReadData[i] != 3)
//        {
//            bTestPass2 = false;
//            break;
//        }
//    }
//
//    bool bTestPass3 = (NvLog.GetLogCount()==5);
//
//    /*Corrupt block 0 BRN*/
//    eFlashStatus = DFLASH::DFLASH_NO_STATUS;
//    hal.dflash.RequestErase(DFLASH_EVENT_LOG_START_ADDRESS, 1, flash_cb);
//    while(eFlashStatus==DFLASH::DFLASH_NO_STATUS)
//    {
//        hal.Update();
//    }
//
//    NV_LOG TmpNvLog(NvLogCfg, NvLogInterface);
//    _requestResult = NV_LOG_IN_PROGRESS;
//    TmpNvLog.Read(au8ReadData, 20, 0, nvlog_cb);
//    while(_requestResult == NV_LOG_IN_PROGRESS)
//    {
//        hal.Update();
//        TmpNvLog.Update();
//    }
//    bool bTestPass4 = (TmpNvLog.GetLogCount()==4);
//
//    while(true)
//    {
//        hal.Update();
//        NvLog.Update();
//        if(UTILS_GetElapsedTimeInSec(&tmr)>=1)
//        {
//            if(bTestPass1)
//            {
//                DEBUG_PRINT_RAW("Test 1 PASS\n");
//            }
//            else
//            {
//                DEBUG_PRINT_RAW("Test 1 FAIL\n");
//            }
//            if(bTestPass2)
//            {
//                DEBUG_PRINT_RAW("Test 2 PASS\n");
//            }
//            else
//            {
//                DEBUG_PRINT_RAW("Test 2 FAIL\n");
//            }
//            if(bTestPass3)
//            {
//                DEBUG_PRINT_RAW("Test 3 PASS\n");
//            }
//            else
//            {
//                DEBUG_PRINT_RAW("Test 3 FAIL\n");
//            }
//            if(bTestPass4)
//            {
//                DEBUG_PRINT_RAW("Test 4 PASS\n");
//            }
//            else
//            {
//                DEBUG_PRINT_RAW("Test 4 FAIL\n");
//            }
//            UTILS_ResetTimer(&tmr);
//        }
//    }
}

void StartStopKeyPress()
{
//    APL_Manager AplManager;
//    stTimer tmr;
//    UTILS_ResetTimer(&tmr);
//    AplManager.keypad.RegisterKeyEventCB(keypad_cb);
//    while(true)
//    {
//        AplManager.Update();
//        if((UTILS_GetElapsedTimeInMs(&tmr)) >= 50)
//        {
//            UTILS_ResetTimer(&tmr);
//            if(!sbKeyEvtAvailable)
//            {
//                continue;
//            }
//            switch(sKeyEvt)
//            {
//                case KEYPAD::BSP_KEY_2_SHORT_PRESS:
//                {
//                    DEBUG_PRINT_RAW("BSP START KEY SHORT_PRESS\n");
//                    break;
//                }
//                case KEYPAD::BSP_KEY_3_SHORT_PRESS:
//                {
//                    DEBUG_PRINT_RAW("BSP STOP KEY SHORT_PRESS\n");
//                    break;
//                }
//            }
//        }
//        sbKeyEvtAvailable = false;
//    }
}

void DIG_IP_Test()
{
//    HAL_Manager hal;
//
//    stTimer tmr;
//    UTILS_ResetTimer(&tmr);
//
//    while(true)
//    {
//        hal.Update();
//        if(UTILS_GetElapsedTimeInSec(&tmr) >= 1)
//        {
//            DEBUG_PRINT_RAW("InputA: %d\n", hal.dip.InputA.GetPinState());
//            DEBUG_PRINT_RAW("InputB: %d\n", hal.dip.InputB.GetPinState());
//            DEBUG_PRINT_RAW("InputC: %d\n", hal.dip.InputC.GetPinState());
//            DEBUG_PRINT_RAW("InputD: %d\n", hal.dip.InputD.GetPinState());
//            DEBUG_PRINT_RAW("InputE: %d\n", hal.dip.InputE.GetPinState());
//            DEBUG_PRINT_RAW("InputF: %d\n", hal.dip.InputF.GetPinState());
//            DEBUG_PRINT_RAW("InputG: %d\n", hal.dip.InputG.GetPinState());
//            DEBUG_PRINT_RAW("InputH: %d\n", hal.dip.InputH.GetPinState());
//            DEBUG_PRINT_RAW("InputI: %d\n", hal.dip.InputI.GetPinState());
//            UTILS_ResetTimer(&tmr);
//        }
//    }

}

void AnlgIP_Test()
{
//    HAL_Manager hal;
//
//    stTimer tmr;
//    UTILS_ResetTimer(&tmr);
//
//    while(true)
//    {
//        hal.Update();
//        if(UTILS_GetElapsedTimeInMs(&tmr) > 1000)
//        {
//            DEBUG_PRINT_RAW("GetPin26ResistanceOhms :%f\n", hal.AnlgIp.GetPin26ResistanceOhms(ANLG_IP::REF_BATT_NEGATIVE).f32InstSensorVal);
//            DEBUG_PRINT_RAW("GetPin24ResistanceOhms :%f\n", hal.AnlgIp.GetPin24ResistanceOhms(ANLG_IP::REF_BATT_NEGATIVE).f32InstSensorVal);
//            DEBUG_PRINT_RAW("GetPin23VoltageMV      :%f\n", hal.AnlgIp.GetPin23Value(ANLG_IP::REF_BATT_NEGATIVE).f32InstSensorVal);
//            DEBUG_PRINT_RAW("GetVbat_V              :%f\n", hal.AnlgIp.GetVbat_V().f32InstSensorVal);
//            DEBUG_PRINT_RAW("GetSCP_V               :%f\n", hal.AnlgIp.GetSCP_V().f32InstSensorVal);
//            DEBUG_PRINT_RAW("GetChargingAltVtg_V    :%f\n", hal.AnlgIp.GetChargingAltVtg_V().f32InstSensorVal);
//            DEBUG_PRINT_RAW("GetChargingAltCurrent_A:%f\n", hal.AnlgIp.GetChargingAltCurrent_A().f32InstSensorVal);
//            DEBUG_PRINT_RAW("GetAnalogVcc_V         :%f\n", hal.AnlgIp.GetAnalogVcc_V().f32InstSensorVal);
//            DEBUG_PRINT_RAW("GetDCOffset_V          :%f\n", hal.AnlgIp.GetDCOffset_V().f32InstSensorVal);
//
//            UTILS_ResetTimer(&tmr);
//        }
//    }

}

void PFLASH_Test()
{
//    HAL_Manager hal;
//
//    hal.pflash.EraseBlocking(PFLASH_BLOCK20_ADDRESS, 1);
//    hal.pflash.BlankCheckBlocking(PFLASH_BLOCK20_ADDRESS, 32*1024);
//
//    uint8_t au8WriteData[128] = "HelloGc\0";
//    hal.pflash.WriteBlocking(au8WriteData, sizeof(au8WriteData), PFLASH_BLOCK20_ADDRESS);
//
//
//    stTimer tmr;
//
//    UTILS_ResetTimer(&tmr);
//
//    volatile uint8_t *pReadData = (uint8_t*)PFLASH_BLOCK20_ADDRESS;
//
//    while(true)
//    {
//        hal.Update();
//        if(UTILS_GetElapsedTimeInMs(&tmr) > 1000)
//        {
//            DEBUG_PRINT_RAW("Read Data: %s\n", (char *)pReadData);
//            UTILS_ResetTimer(&tmr);
//        }
//    }

}

void TestCfgz(void)
{
   //  float f32Pin26LowLevelShutDnThresh;
   //  int16_t s16Pin24HighTempShutDnThresh;
   //  char Arrary[20]={0};

   //  CFGZ::CFGZ_PARAMS_t _allParam;
   //  A_SENSE::CFG_t analogSensorsCfg;
   //  /* ACT Configuration */
   //  ACT::CFG_t actuatorCfg;
   //  HAL_Manager hal(analogSensorsCfg, actuatorCfg);
   //  APL_MANAGER aplManager(hal.dflash);
   //  stTimer                      _LocalDelay;
   //  _allParam.f32ArrParam[0] = 7895.5;
   //  _allParam.s16ArrParam[0] = -500;
   //  _allParam.u16ArrParam[0] = 8765;
   //  _allParam.u8ArrParam[0] =  123;

   //  memcpy((void*)&_allParam.u8ArrStringParam[0], "Madhuri Abhang",
   //                  MAX_AUX_STRING_SIZE);

   //  (void)aplManager.cfgz.WriteActiveProfile(&_allParam);
   //  UTILS_ResetTimer(&_LocalDelay);
   //  while(1)
   //  {
   //      hal.Update();
   //      aplManager.cfgz.Update();
   //      if(UTILS_GetElapsedTimeInMs(&_LocalDelay) >= 1000)
   //      {
   //          break;
   //      }

   //  }

   //  f32Pin26LowLevelShutDnThresh = aplManager.cfgz.GetCFGZ_Param(CFGZ::ID_PIN_26_LOWLEVEL_SHUTDNTHRESH);
   //  s16Pin24HighTempShutDnThresh = aplManager.cfgz.GetCFGZ_Param(CFGZ::ID_PIN24_HIGH_TEMP_SHUTDN_THRESH);
   //  aplManager.cfgz.GetCFGZ_Param(CFGZ::ID_ARR_PROFILE, &Arrary[0]);
   // _allParam.f32ArrParam[0] = f32Pin26LowLevelShutDnThresh +s16Pin24HighTempShutDnThresh;


}

void KeyPadTest()
{
//    HAL_Manager hal;
//
//    hal.keypad.RegisterKeyEventCB(keypad_cb);
//
//    while(true)
//    {
//        hal.Update();
//        if(!sbKeyEvtAvailable)
//        {
//            continue;
//        }
//        switch(sKeyEvt)
//        {
//            case KEYPAD::BSP_KEY_1_SHORT_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_1_SHORT_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_2_SHORT_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_2_SHORT_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_3_SHORT_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_3_SHORT_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_4_SHORT_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_4_SHORT_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_5_SHORT_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_5_SHORT_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_1_LONG_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_1_LONG_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_2_LONG_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_2_LONG_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_3_LONG_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_3_LONG_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_4_LONG_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_4_LONG_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_5_LONG_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_5_LONG_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_2_4_SHORT_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_2_4_SHORT_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_2_4_LONG_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_2_4_LONG_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_4_7_SHORT_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_4_7_SHORT_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_4_7_LONG_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_4_7_LONG_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_7_8_SHORT_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_7_8_SHORT_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_7_8_LONG_PRESS:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_7_8_LONG_PRESS\n");
//                break;
//            }
//            case KEYPAD::BSP_KEY_3_PRESS_STARTED:
//            {
//                DEBUG_PRINT_RAW("BSP_KEY_3_PRESS_STARTED\n");
//                break;
//            }
//        }
//        sbKeyEvtAvailable = false;
//    }
}



void UpdatePID(float f32RPM)
{
 //   f32RPMPulseIp = f32RPM;
}

void TestPulseInput(void)
{
//   HAL_Manager hal;
//   hal.AnalogSensors.ConfigPulseInput(110, A_SENSE::MPU_TYPE);
//   hal.AnalogSensors.RegisterPulseInputUpdateCB(UpdatePID);
//   while(true)
//   {
//      hal.Update();
//      hal.AnalogSensors.Update();
//      //f32RPMPulseIp = hal.AnalogSensors.GetPulseInpuRPM();
//   }
}


void GLCD_Test()
{
//    APL_Manager hal;
//
//    volatile uint8_t x = 0;
//    volatile uint8_t y = 0;
//    volatile GLCD::GLCD_PIXEL_STATE_t state = GLCD::BSP_GLCD_SET;
//    volatile uint8_t u8Contrast = 80;
//    stTimer tmr1;
//    UTILS_ResetTimer(&tmr1);
//    hal.glcd.TurnOnBackLight();
//
//    while(true)
//    {
//        if(UTILS_GetElapsedTimeInMs(&tmr1) > 10)
//        {
//            x++;
//            if(x>=128)
//            {
//                x=0;
//                y++;
//            }
//            if(y>=64)
//            {
//                x=0;
//                y=0;
//                state = (state==GLCD::BSP_GLCD_SET?GLCD::BSP_GLCD_UNSET:GLCD::BSP_GLCD_SET);
//            }
//            UTILS_ResetTimer(&tmr1);
//            hal.glcd.AdjustContrast(u8Contrast);
//        }
//        hal.glcd.SetUnsetPixel(x, y, state);
//        hal.Update();
//}
}


void D_SENSE_Test()
{

//    /* D_SENSE Configuration */
//    D_SENSE::CFG_t digitalSensorCfg =
//    {{{DigitalSensor::DI_LOW_FUEL_LEVEL_SWITCH         , DigitalSensor::CLOSE_TO_ACTIVATE, 2},
//      {DigitalSensor::DI_LOW_LUBE_OIL_PRESSURE_SWITCH  , DigitalSensor::OPEN_TO_ACTIVATE , 2},
//      {DigitalSensor::DI_HIGH_ENGINE_TEMP_SWITCH       , DigitalSensor::OPEN_TO_ACTIVATE , 2},
//      {DigitalSensor::DI_D_USER_CONFIGURED            , DigitalSensor::OPEN_TO_ACTIVATE , 2},
//      {DigitalSensor::DI_EMERGENCY_STOP                , DigitalSensor::OPEN_TO_ACTIVATE , 2},
//      {DigitalSensor::DI_REMOTE_START_STOP             , DigitalSensor::OPEN_TO_ACTIVATE , 2},
//      {DigitalSensor::DI_SIMULATE_START                , DigitalSensor::OPEN_TO_ACTIVATE , 2},
//      {DigitalSensor::DI_SIMULATE_STOP                 , DigitalSensor::OPEN_TO_ACTIVATE , 2}}};
//
//
//
//    HAL_Manager hal;
//    hal.DigitalSensors.ConfigureSensor(digitalSensorCfg);
//
//    DigitalSensor::STATUS_t status[9];
//
//    status[1] = hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_LUBE_OIL_PRESSURE_SWITCH);
//    status[2] = hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_HIGH_ENGINE_TEMP_SWITCH);
//    status[3] = hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_D_USER_CONFIGURED);
//    status[4] = hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_EMERGENCY_STOP);
//    status[5] = hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_REMOTE_START_STOP);
//    status[6] = hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_START);
//    status[7] = hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_STOP);
//    status[8] = hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_AUTO);
//    volatile int status_cnt = 0;
//    for (int i = 0; i < 9; ++i)
//    {
//        if(status[i] == DigitalSensor::SENSOR_UNLATCHED)
//            status_cnt++;
//    }
//
//    while(true)
//    {
//        hal.Update();
//        status[0] = hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_FUEL_LEVEL_SWITCH);
//        if(DigitalSensor::SENSOR_LATCHED == status[0])
//        {
//           R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_06, BSP_IO_LEVEL_HIGH);
//        }
//        else
//        {
//           R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_06, BSP_IO_LEVEL_LOW);
//        }
//    }
}

//void A_SENSE_Test()
//{
//    AnalogSensor::INTERPOLATE_INFO_t
//    Pressure={{0.0, 40.0, 80.0, 120.0, 160.0, 200.0, 240.0, 280.0, 320.0, 360.0},
//              {0.0, 1.0, 2.0,  3.0,   4.0,   5.0,   6.0,   7.0,   8.0,    9.0},
//                10},
//
//        Fuel= {{0.0, 40.0, 80.0, 120.0, 160.0, 200.0, 240.0, 280.0, 320.0, 360.0},
//               {0.0, 10.0, 20.0, 30.0,  40.0,  50.0,  60.0,  70.0,  80.0,  90.0},
//                10},
//
//        Temp={{0.0, 40.0, 80.0, 120.0, 160.0, 200.0, 240.0, 280.0, 320.0, 360.0},
//              {0.0, 10.0, 20.0, 30.0,  40.0,  50.0,  60.0,  70.0,  80.0,  90.0},
//                10},
//    A_SENSE::CFG_t analogSensCfg
//    =
//    {
//        {
//            {AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE, ANLG_IP::REF_BATT_NEGATIVE, Pressure},
//            {AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE, ANLG_IP::REF_BATT_NEGATIVE, Fuel},
//            {AnalogSensor::A_SENSE_ENG_TEMPERATURE, ANLG_IP::REF_BATT_NEGATIVE, Temp}
//        }
//    };
//
//    HAL_Manager hal;
//    hal.AnalogSensors.ConfigureSensor(analogSensCfg);
//    while(true)
//    {
//        A_SENSE::SENSOR_RET_t pressure = hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE);
//        A_SENSE::SENSOR_RET_t fuel = hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);
//        A_SENSE::SENSOR_RET_t temperature=  hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_ENG_TEMPERATURE);
//
//        hal.Update();
//
//        hal.AnalogSensors.Update();
//        float f32Vbat = hal.AnalogSensors.GetFilteredVbattVolts();
//        float f32ChargingAlt = hal.AnalogSensors.GetFilteredChargingAltVolts();
//    }
//
//}

void DFLASH_test()
{
//    HAL_Manager hal;
//
//    hal.dflash.RequestErase(0x40100000, 1, flash_cb);
//    while(!bFlashErased)
//    {
//        hal.Update();
//    }
//    bFlashErased= false;
//
//    uint8_t au8WriteData[16] = "GetItDone";
//    hal.dflash.RequestWrite(0x40100000, au8WriteData, sizeof(au8WriteData), flash_cb);
//    while(!bFlashErased)
//    {
//        hal.Update();
//    }
//
//    uint8_t au8ReadData[16] = {0};
//    hal.dflash.Read(0x40100000, au8ReadData, sizeof(au8WriteData));
//
//    while(true);
}

void USB_CDC_Test()
{
    /* Echos back whatever is received in virtual COM */
//
//     HAL_Manager hal;
//
//     /* Register USB receive nvlog_cb */
//     hal.usb.RegisterReceiveCb(usb_cb);
//
//     stTimer tmr1;
//     UTILS_ResetTimer(&tmr1);
//
//     while(true)
//     {
//         if(su8UsbRecvBuffSz > 0)
//         {
//            hal.usb.Send((uint8_t*)sau8UsbRecvBuff, su8UsbRecvBuffSz);
//            su8UsbRecvBuffSz = 0;
//         }
//         hal.Update();
//         if(UTILS_GetElapsedTimeInSec(&tmr1) > 5)
//         {
//             DEBUG_PRINT_RAW("USB_CDC_Test\n");
//             UTILS_ResetTimer(&tmr1);
//         }
//     }
}

void EEPROM_M95M01_Test()
{
//    HAL_Manager hal;
//
//    stTimer tmr;
//    uint8_t state = 1;
//    #define EEPROM_TEST_DATA_SZ (300)
//    volatile uint8_t au8WriteData[EEPROM_TEST_DATA_SZ];
//    volatile uint8_t au8ReadData[EEPROM_TEST_DATA_SZ] = {0};
//
//     /*The data size and address is chosen such that it spans over 3 EEPROM pages
//       (i.e page 0,1,2). This tests driver to the best extend.
//      */
//     const uint32_t u32StartAddress = 0xfb;
//
//     while(true)
//     {
//        hal.Update();
//        if(UTILS_GetElapsedTimeInMs(&tmr) > 2000)
//        {
//            UTILS_ResetTimer(&tmr);
//
//             switch(state)
//             {
//                 case 1:
//                 {
//                    for(int i=0;i<sizeof(au8WriteData);i++)
//                    {
//                        au8WriteData[i] = i%256 + 10;
//                    }
//                    sbEEPROMTxComplete = false;
//                    hal.eeprom.RequestWrite(u32StartAddress, (uint8_t*)au8WriteData,
//                                                 sizeof(au8WriteData), eeprom_cb);
//                    state = 2;
//                    break;
//                 }
//                 case 2:
//                 {
//                     if(sbEEPROMTxComplete) state=3;
//                     break;
//                 }
//                 case 3:
//                 {
//                     DEBUG_PRINT_HEX(au8ReadData, 5);
//                     sbEEPROMTxComplete = false;
//                     memset((uint8_t*)au8ReadData, 0x00, sizeof(au8ReadData));
//                     hal.eeprom.RequestRead(u32StartAddress, (uint8_t*)au8ReadData, sizeof(au8ReadData), eeprom_cb);
//                     state = 4;
//                     break;
//                 }
//                 case 4:
//                 {
//                     if(sbEEPROMTxComplete)
//                     {
//                        for(uint16_t i=0;i<sizeof(au8ReadData);i++)
//                        {
//                            if(au8ReadData[i]!=au8WriteData[i])
//                            {
//                                state=1;
//                                DEBUG_PRINT_RAW("EEPROM_TEST_FAIL\n");
//                                return;
//                            }
//                        }
//                        DEBUG_PRINT_RAW("EEPROM_TEST_PASS\n");
//                        state=1;
//                     }
//                     break;
//                 }
//                 default:
//                 {
//                     break;
//                 }
//             }
//        }
//     }
}

void AC_SENSE_Test()
{

//    HAL_Manager hal;
//    static int idx = 0;
//    stTimer tmr;
//
//    hal.AcSensors.ConfigureCTRatio(3);
//    hal.AcSensors.ConfigureISensing(GENSET_CURRENT_MEASUREMENT);
//
//    while(true)
//    {
//        hal.Update();
//        if(UTILS_GetElapsedTimeInMs(&tmr) < 2000)
//        {
//            continue;
//        }
//        UTILS_ResetTimer(&tmr);
//
//        idx++;
//        switch(idx)
//        {
//            case 1:
//            {
//                DEBUG_PRINT_RAW("********************START***********************\n");
//                DEBUG_PRINT_RAW("GENSET_GetVoltageVolts(R_PHASE)      :%f\n",hal.AcSensors.GENSET_GetVoltageVolts(R_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetVoltageVolts(Y_PHASE)      :%f\n",hal.AcSensors.GENSET_GetVoltageVolts(Y_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetVoltageVolts(B_PHASE)      :%f\n",hal.AcSensors.GENSET_GetVoltageVolts(B_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetCurrentAmps(R_PHASE)       :%f\n",hal.AcSensors.GENSET_GetCurrentAmps(R_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetCurrentAmps(Y_PHASE)       :%f\n",hal.AcSensors.GENSET_GetCurrentAmps(Y_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetCurrentAmps(B_PHASE)       :%f\n",hal.AcSensors.GENSET_GetCurrentAmps(B_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetApproxFreq(R_PHASE)        :%f\n",hal.AcSensors.GENSET_GetApproxFreq(R_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetApproxFreq(Y_PHASE)        :%f\n",hal.AcSensors.GENSET_GetApproxFreq(Y_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetApproxFreq(B_PHASE)        :%f\n",hal.AcSensors.GENSET_GetApproxFreq(B_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetTotalActivePowerWatts()    :%f\n",hal.AcSensors.GENSET_GetTotalActivePowerWatts());
//                DEBUG_PRINT_RAW("GENSET_GetTotalApparentPowerVA()     :%f\n",hal.AcSensors.GENSET_GetTotalApparentPowerVA());
//                DEBUG_PRINT_RAW("GENSET_GetTotalReactivePowerVAR()    :%f\n",hal.AcSensors.GENSET_GetTotalReactivePowerVAR());
//                DEBUG_PRINT_RAW("GENSET_GetActivePowerWatts(R_PHASE)  :%f\n",hal.AcSensors.GENSET_GetActivePowerWatts(R_PHASE));
//                break;
//            }
//            case 2:
//            {
//                DEBUG_PRINT_RAW("GENSET_GetActivePowerWatts(Y_PHASE)  :%f\n",hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetActivePowerWatts(B_PHASE)  :%f\n",hal.AcSensors.GENSET_GetActivePowerWatts(B_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetApparentPowerVA(R_PHASE)   :%f\n",hal.AcSensors.GENSET_GetApparentPowerVA(R_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetApparentPowerVA(Y_PHASE)   :%f\n",hal.AcSensors.GENSET_GetApparentPowerVA(Y_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetApparentPowerVA(B_PHASE)   :%f\n",hal.AcSensors.GENSET_GetApparentPowerVA(B_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetReactivePowerVAR(R_PHASE)  :%f\n",hal.AcSensors.GENSET_GetReactivePowerVAR(R_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetReactivePowerVAR(Y_PHASE)  :%f\n",hal.AcSensors.GENSET_GetReactivePowerVAR(Y_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetReactivePowerVAR(B_PHASE)  :%f\n",hal.AcSensors.GENSET_GetReactivePowerVAR(B_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetPowerFactor(R_PHASE)       :%f\n",hal.AcSensors.GENSET_GetPowerFactor(R_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetPowerFactor(Y_PHASE)       :%f\n",hal.AcSensors.GENSET_GetPowerFactor(Y_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetPowerFactor(B_PHASE)       :%f\n",hal.AcSensors.GENSET_GetPowerFactor(B_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetTotalActiveEnergyWH()      :%f\n",hal.AcSensors.GENSET_GetTotalActiveEnergySinceInitWH());
//                DEBUG_PRINT_RAW("GENSET_GetTotalApparentEnergyVAH()   :%f\n",hal.AcSensors.GENSET_GetTotalApparentEnergySinceInitVAH());
//                DEBUG_PRINT_RAW("GENSET_GetTotalReactiveEnergyVARH()  :%f\n",hal.AcSensors.GENSET_GetTotalReactiveEnergySinceInitVARH());
//                break;
//            }
//            case 3:
//            {
//                DEBUG_PRINT_RAW("GENSET_GetActiveEnergyWH(R_PHASE)    :%f\n",hal.AcSensors.GENSET_GetActiveEnergyWH(R_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetActiveEnergyWH(Y_PHASE)    :%f\n",hal.AcSensors.GENSET_GetActiveEnergyWH(Y_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetActiveEnergyWH(B_PHASE)    :%f\n",hal.AcSensors.GENSET_GetActiveEnergyWH(B_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetApparentEnergyVAH(R_PHASE) :%f\n",hal.AcSensors.GENSET_GetApparentEnergyVAH(R_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetApparentEnergyVAH(Y_PHASE) :%f\n",hal.AcSensors.GENSET_GetApparentEnergyVAH(Y_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetApparentEnergyVAH(B_PHASE) :%f\n",hal.AcSensors.GENSET_GetApparentEnergyVAH(B_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetReactiveEnergyVARH(R_PHASE):%f\n",hal.AcSensors.GENSET_GetReactiveEnergyVARH(R_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetReactiveEnergyVARH(Y_PHASE):%f\n",hal.AcSensors.GENSET_GetReactiveEnergyVARH(Y_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetReactiveEnergyVARH(B_PHASE):%f\n",hal.AcSensors.GENSET_GetReactiveEnergyVARH(B_PHASE));
//                DEBUG_PRINT_RAW("GENSET_GetPhaseRotStatus()           :%f\n",hal.AcSensors.GENSET_GetPhaseRotStatus());
//                DEBUG_PRINT_RAW("GENSET_GetRYVolts()                  :%f\n",hal.AcSensors.GENSET_GetRYVolts());
//                DEBUG_PRINT_RAW("GENSET_GetYBVolts()                  :%f\n",hal.AcSensors.GENSET_GetYBVolts());
//                DEBUG_PRINT_RAW("GENSET_GetRBVolts()                  :%f\n",hal.AcSensors.GENSET_GetRBVolts());
//                DEBUG_PRINT_RAW("MAINS_GetVoltageVolts(R_PHASE)       :%f\n",hal.AcSensors.MAINS_GetVoltageVolts(R_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetVoltageVolts(Y_PHASE)       :%f\n",hal.AcSensors.MAINS_GetVoltageVolts(Y_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetVoltageVolts(B_PHASE)       :%f\n",hal.AcSensors.MAINS_GetVoltageVolts(B_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetCurrentAmps(R_PHASE)        :%f\n",hal.AcSensors.MAINS_GetCurrentAmps(R_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetCurrentAmps(Y_PHASE)        :%f\n",hal.AcSensors.MAINS_GetCurrentAmps(Y_PHASE));
//                break;
//            }
//            case 4:
//            {
//                DEBUG_PRINT_RAW("MAINS_GetCurrentAmps(B_PHASE)        :%f\n",hal.AcSensors.MAINS_GetCurrentAmps(B_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetApproxFreq(R_PHASE)         :%f\n",hal.AcSensors.MAINS_GetApproxFreq(R_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetApproxFreq(Y_PHASE)         :%f\n",hal.AcSensors.MAINS_GetApproxFreq(Y_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetApproxFreq(B_PHASE)         :%f\n",hal.AcSensors.MAINS_GetApproxFreq(B_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetTotalActivePowerWatts()     :%f\n",hal.AcSensors.MAINS_GetTotalActivePowerWatts());
//                DEBUG_PRINT_RAW("MAINS_GetTotalApparentPowerVA()      :%f\n",hal.AcSensors.MAINS_GetTotalApparentPowerVA());
//                DEBUG_PRINT_RAW("MAINS_GetTotalReactivePowerVAR()     :%f\n",hal.AcSensors.MAINS_GetTotalReactivePowerVAR());
//                DEBUG_PRINT_RAW("MAINS_GetActivePowerWatts(R_PHASE)   :%f\n",hal.AcSensors.MAINS_GetActivePowerWatts(R_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetActivePowerWatts(Y_PHASE)   :%f\n",hal.AcSensors.MAINS_GetActivePowerWatts(Y_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetActivePowerWatts(B_PHASE)   :%f\n",hal.AcSensors.MAINS_GetActivePowerWatts(B_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetApparentPowerVA(R_PHASE)    :%f\n",hal.AcSensors.MAINS_GetApparentPowerVA(R_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetApparentPowerVA(Y_PHASE)    :%f\n",hal.AcSensors.MAINS_GetApparentPowerVA(Y_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetApparentPowerVA(B_PHASE)    :%f\n",hal.AcSensors.MAINS_GetApparentPowerVA(B_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetReactivePowerVAR(R_PHASE)   :%f\n",hal.AcSensors.MAINS_GetReactivePowerVAR(R_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetReactivePowerVAR(Y_PHASE)   :%f\n",hal.AcSensors.MAINS_GetReactivePowerVAR(Y_PHASE));
//                break;
//            }
//            case 5:
//            {
//                DEBUG_PRINT_RAW("MAINS_GetReactivePowerVAR(B_PHASE)   :%f\n",hal.AcSensors.MAINS_GetReactivePowerVAR(B_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetPowerFactor(R_PHASE)        :%f\n",hal.AcSensors.MAINS_GetPowerFactor(R_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetPowerFactor(Y_PHASE)        :%f\n",hal.AcSensors.MAINS_GetPowerFactor(Y_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetPowerFactor(B_PHASE)        :%f\n",hal.AcSensors.MAINS_GetPowerFactor(B_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetTotalActiveEnergyWH()       :%f\n",hal.AcSensors.MAINS_GetTotalActiveEnergyWH());
//                DEBUG_PRINT_RAW("MAINS_GetTotalApparentEnergyVAH()    :%f\n",hal.AcSensors.MAINS_GetTotalApparentEnergyVAH());
//                DEBUG_PRINT_RAW("MAINS_GetTotalReactiveEnergyVARH()   :%f\n",hal.AcSensors.MAINS_GetTotalReactiveEnergyVARH());
//                DEBUG_PRINT_RAW("MAINS_GetActiveEnergyWH(R_PHASE)     :%f\n",hal.AcSensors.MAINS_GetActiveEnergyWH(R_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetActiveEnergyWH(Y_PHASE)     :%f\n",hal.AcSensors.MAINS_GetActiveEnergyWH(Y_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetActiveEnergyWH(B_PHASE)     :%f\n",hal.AcSensors.MAINS_GetActiveEnergyWH(B_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetApparentEnergyVAH(R_PHASE)  :%f\n",hal.AcSensors.MAINS_GetApparentEnergyVAH(R_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetApparentEnergyVAH(Y_PHASE)  :%f\n",hal.AcSensors.MAINS_GetApparentEnergyVAH(Y_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetApparentEnergyVAH(B_PHASE)  :%f\n",hal.AcSensors.MAINS_GetApparentEnergyVAH(B_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetReactiveEnergyVARH(R_PHASE) :%f\n",hal.AcSensors.MAINS_GetReactiveEnergyVARH(R_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetReactiveEnergyVARH(Y_PHASE) :%f\n",hal.AcSensors.MAINS_GetReactiveEnergyVARH(Y_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetReactiveEnergyVARH(B_PHASE) :%f\n",hal.AcSensors.MAINS_GetReactiveEnergyVARH(B_PHASE));
//                DEBUG_PRINT_RAW("MAINS_GetPhaseRotStatus()            :%d\n",hal.AcSensors.MAINS_GetPhaseRotStatus());
//                DEBUG_PRINT_RAW("MAINS_GetRYVolts()                   :%f\n",hal.AcSensors.MAINS_GetRYVolts());
//                DEBUG_PRINT_RAW("MAINS_GetYBVolts()                   :%f\n",hal.AcSensors.MAINS_GetYBVolts());
//                DEBUG_PRINT_RAW("MAINS_GetRBVolts()                   :%f\n",hal.AcSensors.MAINS_GetRBVolts());
//                DEBUG_PRINT_RAW("*********************END***********************\n");
//                break;
//            }
//            default:
//            {
//                idx=0;
//            }
//        }
//    }
//
}


void MODBUS_Test()
{
//    APL_Manager hal;
//
//    hal.rs485.ConfigureBaudParity(RS485::BAUD_9600, RS485::PARITY_NONE);
//
//    MB_APP mbApp(hal);
//
//    mbApp.SetReadRegisterValue(MB_APP::MB_REG_PROTOCOL_VER, 3);
//    mbApp.SetReadRegisterValue(MB_APP::MB_REG_RSRVD_1, 4);
//
//    stTimer tmr;
//
//    while(true)
//    {
//        hal.Update();
//        mbApp.Update();
//
//        if(UTILS_GetElapsedTimeInMs(&tmr) >= 1000)
//        {
//            DEBUG_PRINT_RAW("MB_COMMAND      :%d\n", mbApp.GetRegisterValue(MB_APP::MB_COMMAND));
//            DEBUG_PRINT_RAW("MB_OPERATNG_MODE:%d\n", mbApp.GetRegisterValue(MB_APP::MB_OPERATNG_MODE));
//            if(100 == mbApp.GetRegisterValue(MB_APP::MB_COMMAND))
//            {
//                mbApp.SetWriteRegisterValue(MB_APP::MB_COMMAND, 0);
//            }
//            UTILS_ResetTimer(&tmr);
//        }
//    }
}

void CRC_Test()
{
//    /* Open CRC module with 8 bit polynomial */
//    R_CRC_Open(&g_crc0_ctrl, &g_crc0_cfg);
//    uint8_t data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
//    uint32_t u32Crc;
//    crc_input_t example_input =
//    {
//        .num_bytes      = 2,
//        .crc_seed       = 0xFFFF,
//        .p_input_buffer = data
//    };
//    /* 8-bit CRC calculation */
//    R_CRC_Calculate(&g_crc0_ctrl, &example_input, &u32Crc);
//    example_input.num_bytes      = 3;
//    example_input.p_input_buffer = data+2;
//    example_input.crc_seed       = u32Crc;
//    R_CRC_Calculate(&g_crc0_ctrl, &example_input, &u32Crc);
//
//    uint16_t swCrc = CRC16::ComputeModbusCRC(data, sizeof(data));
//    DEBUG_PRINT_RAW("%d\n", swCrc);
//    while(true);
}

void Virtual_EEPROM_Test()
{

//    HAL_Manager hal;
//
//    uint32_t u32Record1 = 5;
//    uint32_t u32Record2 = 6;
//    uint32_t u32Record3 = 7;
//    sbEEPROMTxComplete = false;
//    hal.veeprom.RequestWrite(1, (uint8_t*)&(u32Record1), sizeof(u32Record1), veeprom_cb);
//    hal.veeprom.RequestWrite(2, (uint8_t*)&(u32Record2), sizeof(u32Record2), veeprom_cb);
//    hal.veeprom.RequestWrite(3, (uint8_t*)&(u32Record3), sizeof(u32Record3), veeprom_cb);
//    hal.dflash.RequestErase(0x40100000, 1, NULL);
//    hal.dflash.RequestErase(0x40100080, 1, NULL);
//    uint8_t au8WriteData[16] = "Commit test";
//    hal.dflash.RequestWrite(0x40100080, au8WriteData, sizeof(au8WriteData), flash_cb);
//    while(!sbEEPROMTxComplete)
//    {
//        hal.Update();
//    }
//    volatile uint32_t Res = 0;
//    uint8_t buffSize = sizeof(Res);
//    hal.veeprom.Read(2, (uint8_t*)&Res, &buffSize);
//
//    while(!bFlashErased)
//    {
//        hal.Update();
//    }
//    uint8_t au8ReadData[16] = {0};
//    hal.dflash.Read(0x40100080, au8ReadData, sizeof(au8ReadData));
//
//    while(true)
//    {
//        hal.veeprom.Read(2, (uint8_t*)&Res, &buffSize);
//        hal.Update();
//    }
}

void LED_Test()
{
//    HAL_Manager hal;
//
//    stTimer tmr1;
//
//    UTILS_ResetTimer(&tmr1);
//    uint8_t u8State = 0;
//
//    while(true)
//    {
//        if(UTILS_GetElapsedTimeInMs(&tmr1)>1000)
//        {
//            u8State++;
//            u8State %= 4;
//            UTILS_ResetTimer(&tmr1);
//        }
//        switch(u8State)
//        {
//            case 0:
//            {
//                hal.ledManager.led4.TurnOff();
//                hal.ledManager.led1.TurnOn();
//                break;
//            }
//            case 1:
//            {
//                hal.ledManager.led1.TurnOff();
//                hal.ledManager.led2.TurnOn();
//                break;
//            }
//            case 2:
//            {
//                hal.ledManager.led2.TurnOff();
//                hal.ledManager.led3.TurnOn();
//                break;
//            }
//            case 3:
//            {
//                hal.ledManager.led3.TurnOff();
//                hal.ledManager.led4.TurnOn();
//                break;
//            }
//        }
//        hal.Update();
//    }
}

void RTC_Test()
{
//    HAL_Manager hal;
//
//    stTimer tmr1;
//
//    RTC::TIME_t time;
//
//    /*Set Time*/
//     time.u8Second       = 0;
//     time.u8Minute       = 55;
//     time.u8Hour         = 15;
//     time.u8DayOfWeek    = 4;
//     time.u8Day          = 1;
//     time.u8Month        = 7;
//     time.u16Year        = 2021;
////     hal.rtc.SetTime(&time);
//
//    while(true)
//    {
//        if(UTILS_GetElapsedTimeInSec(&tmr1) >= 1)
//        {
//            hal.rtc.GetTime(&time);
//            DEBUG_PRINT_RAW("Time(HH:MM:SS)  : %d:%d:%d\n", time.u8Hour, time.u8Minute, time.u8Second);
//            DEBUG_PRINT_RAW("Date(DD/MM/YYYY): %d-%d-%d\n", time.u8Day, time.u8Month, time.u16Year);
//            DEBUG_PRINT_RAW("Day of week:%d\n", time.u8DayOfWeek);
//            UTILS_ResetTimer(&tmr1);
//        }
//        hal.Update();
//    }
}

void HSD_Test()
{
//    HAL_Manager hal;
//
//    stTimer tmr1;
//
//    UTILS_ResetTimer(&tmr1);
//    uint8_t u8State = 0;
//
//    while(true)
//    {
//        if(UTILS_GetElapsedTimeInMs(&tmr1)>1000)
//        {
//            u8State++;
//            u8State %= 6;
//            UTILS_ResetTimer(&tmr1);
//        }
//        switch(u8State)
//        {
//            case 0:
//            {
//                hal.hsdManager.HSDF.TurnOff();
//                hal.hsdManager.HSDA.TurnOn();
//                break;
//            }
//            case 1:
//            {
//                hal.hsdManager.HSDA.TurnOff();
//                hal.hsdManager.HSDB.TurnOn();
//                break;
//            }
//            case 2:
//            {
//                hal.hsdManager.HSDB.TurnOff();
//                hal.hsdManager.HSDC.TurnOn();
//                break;
//            }
//            case 3:
//            {
//                hal.hsdManager.HSDC.TurnOff();
//                hal.hsdManager.HSDD.TurnOn();
//                break;
//            }
//            case 4:
//            {
//                hal.hsdManager.HSDD.TurnOff();
//                hal.hsdManager.HSDE.TurnOn();
//                break;
//            }
//            case 5:
//            {
//                hal.hsdManager.HSDE.TurnOff();
//                hal.hsdManager.HSDF.TurnOn();
//                break;
//            }
//        }
//        hal.Update();
//    }
}

void CFGZTestHelper1(DigitalSensor::D_SENSOR_TYPS_t eTyp, char *pcSensorName,  APL_Manager &apl)
{
//    DigitalSensor::STATUS_t status = apl.DigitalSensors.GetDigitalSensorState(eTyp);
//    switch(status)
//    {
//        case DigitalSensor::SENSOR_LATCHED:
//        {
//            DEBUG_PRINT_RAW("%s:%s\n", pcSensorName, "SENSOR_LATCHED");
//            break;
//        }
//        case DigitalSensor::SENSOR_UNLATCHED:
//        {
//            DEBUG_PRINT_RAW("%s:%s\n", pcSensorName, "SENSOR_UNLATCHED");
//            break;
//        }
//        case DigitalSensor::SENSOR_NOT_CONFIGRUED:
//        {
//            DEBUG_PRINT_RAW("%s:%s\n", pcSensorName, "SENSOR_NOT_CONFIGRUED");
//            break;
//        }
//        default:
//        {
//            break;
//        }
//    }
}

void CFGZTestHelper2(AnalogSensor::TYPS_t eTyp, char *pcSensorName,  APL_Manager &apl)
{
//    A_SENSE::SENSOR_RET_t status = apl.AnalogSensors.GetSensorValue(eTyp);
//    switch(status.eStatus)
//    {
//        case A_SENSE::SENSOR_NOT_CONFIGRUED:
//        {
//            DEBUG_PRINT_RAW("%s:%s\n", pcSensorName, "SENSOR_NOT_CONFIGRUED");
//            break;
//        }
//        case A_SENSE::SENSOR_READ_SUCCESS:
//        {
//            DEBUG_PRINT_RAW("%s:%s:%f\n", pcSensorName,
//                                  "SENSOR_READ_SUCCESS", status.stValAndStatus.f32InstSensorVal);
//            break;
//        }
//        default:
//        {
//            break;
//        }
//    }
}

void CFGZTestHelper3(ACTUATOR::ACTUATOR_TYPS_t eTyp, char *pcSensorName,  APL_Manager &apl)
{
//    ACT_Manager::ACT_STATUS_t status = apl.actuators.Activate(eTyp);
//    switch(status)
//    {
//        case ACT_Manager::ACT_SUCCESS:
//        {
//            DEBUG_PRINT_RAW("%s:%s\n", pcSensorName, "ACT_SUCCESS");
//            break;
//        }
//        case ACT_Manager::ACT_NOT_CONFIGURED:
//        {
//            DEBUG_PRINT_RAW("%s:%s\n", pcSensorName, "ACT_NOT_CONFIGURED");
//            break;
//        }
//        default:
//        {
//            break;
//        }
//    }
}

void CFGZ_Test()
{
//    APL_Manager apl;
//
//    stTimer tmr1;
//
//    UTILS_ResetTimer(&tmr1);
//
//    while(true)
//    {
//        apl.Update();
//        static uint8_t u8Cnt = 0;
//        if(UTILS_GetElapsedTimeInMs(&tmr1) >= 500)
//        {
//            u8Cnt++;
//            UTILS_ResetTimer(&tmr1);
//
//            switch(u8Cnt)
//            {
//                case 1:
//                {
//                    CFGZTestHelper1(DigitalSensor::DI_A_USER_CONFIGURED, (char*)"DI_A_USER_CONFIGURED", apl);
//                    break;
//                }
//                case 2:
//                {
//                    CFGZTestHelper1(DigitalSensor::DI_B_USER_CONFIGURED, (char*)"DI_B_USER_CONFIGURED", apl);
//                    break;
//                }
//                case 3:
//                {
//                    CFGZTestHelper1(DigitalSensor::DI_C_USER_CONFIGURED, (char*)"DI_C_USER_CONFIGURED", apl);
//                    break;
//                }
//                case 4:
//                {
//                    CFGZTestHelper1(DigitalSensor::DI_D_USER_CONFIGURED, (char*)"DI_D_USER_CONFIGURED", apl);
//                    break;
//                }
//                case 5:
//                {
//                    CFGZTestHelper1(DigitalSensor::DI_E_USER_CONFIGURED, (char*)"DI_E_USER_CONFIGURED", apl);
//                    break;
//                }
//                case 6:
//                {
//                    CFGZTestHelper1(DigitalSensor::DI_F_USER_CONFIGURED, (char*)"DI_F_USER_CONFIGURED", apl);
//                    break;
//                }
//                case 7:
//                {
//                    CFGZTestHelper1(DigitalSensor::DI_G_USER_CONFIGURED, (char*)"DI_G_USER_CONFIGURED", apl);
//                    break;
//                }
//                case 8:
//                {
//                    CFGZTestHelper1(DigitalSensor::DI_LOW_FUEL_LEVEL_SWITCH, (char*)"DI_LOW_FUEL_LEVEL_SWITCH", apl);
//                    break;
//                }
//                case 9:
//                {
//                    CFGZTestHelper2(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE, (char*)"A_SENSE_FUEL_LEVEL_RESISTIVE", apl);
//                    break;
//                }
//                case 10:
//                {
//                    CFGZTestHelper2(AnalogSensor::A_SENSE_ENG_TEMPERATURE, (char*)"A_SENSE_ENG_TEMPERATURE", apl);
//                    break;
//                }
//                case 11:
//                {
//                    CFGZTestHelper3(ACTUATOR::ACT_MAINS_LOW, (char*)"ACT_MAINS_LOW", apl);
//                    break;
//                }
//                case 12:
//                {
//                    CFGZTestHelper3(ACTUATOR::ACT_START_RELAY, (char*)"ACT_START_RELAY", apl);
//                    break;
//                }
//                case 13:
//                {
//                    CFGZTestHelper3(ACTUATOR::ACT_PREHEAT, (char*)"ACT_PREHEAT", apl);
//                    break;
//                }
//                case 14:
//                {
//                    CFGZTestHelper3(ACTUATOR::ACT_FAIL_TO_START, (char*)"ACT_FAIL_TO_START", apl);
//                    break;
//                }
//                case 15:
//                {
//                    CFGZTestHelper3(ACTUATOR::ACT_DIG_IN_A, (char*)"ACT_DIG_IN_A", apl);
//                    break;
//                }
//                case 16:
//                {
//                    CFGZTestHelper3(ACTUATOR::ACT_AUDIBLE_ALARM, (char*)"ACT_AUDIBLE_ALARM", apl);
//                    DEBUG_PRINT_RAW("***************************************************************\n")
//                    u8Cnt = 0;
//                    break;
//                }
//            }
//        }
//    }
}

void flash_cb(DFLASH::FLASH_STATUS_t evt)
{
// eFlashStatus = evt;
}

void usb_cb(uint8_t *pu8Data, uint16_t u16Len)
{
//    memcpy((void*)(sau8UsbRecvBuff+su8UsbRecvBuffSz), pu8Data, u16Len);
//    su8UsbRecvBuffSz = (uint8_t)(su8UsbRecvBuffSz + (uint8_t)u16Len);
}

//void eeprom_cb(EEPROM::EVENTS_t evt)
//{
////    sbEEPROMTxComplete = true;
//}

void keypad_cb(KEYPAD::KEYPAD_EVENTS_t Evt)
{
//    sKeyEvt = Evt;
//    sbKeyEvtAvailable = true;
}

//void nvlog_cb(NV_LOG_STATUS_t evt)
//{
// //   _requestResult = evt;
//}
#endif
