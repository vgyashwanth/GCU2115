/*
 * CFGC.cpp
 *
 *  Created on: 07-Aug-2021
 *      Author: nikhil.mhaske
 */

#include "CFGC.h"
#include "config.h"
#include "CRC16.h"

void cb(DFLASH::FLASH_STATUS_t evt);
volatile static DFLASH::FLASH_STATUS_t eStatus = DFLASH::DFLASH_NO_STATUS;
 CFGC::CFGC_t CFGC::_stCfgc={};
CFGC::CFGC(HAL_Manager &Hal):
_Hal(Hal),
_bIntegrityCheckFailed(false)
{
    if(prvGetCFGCIntegrityCheck() != BSP_SUCCESS)
    {
        _bIntegrityCheckFailed = true;
    }
    else
    {
        _Hal.ObjDflash.Read(DFLASH_CFGC_START_ADDRESS , (uint8_t *)&_stCfgc, sizeof(CFGC_t));
    }
}

void CFGC::Update()
{
    static bool entry = false;
    if(entry == false)
    {
        entry = true;
        eStatus = DFLASH::DFLASH_NO_STATUS;
    }
}

void CFGC::GetEngineSerialNo(uint8_t au8EngSrNo[])
{
    memcpy(&au8EngSrNo[0], &_stCfgc.u8EngineSrNoChar01, TOTAL_CHAR_ENG_SR_NO);
}

uint32_t CFGC::GetPCBPID()
{
    return(_stCfgc.u32PCBPID);
}

uint16_t CFGC::GetProductID()
{
    return(_stCfgc.u16ProductID);
}

uint8_t CFGC::GetUseExtendedEventLog()
{
    return(_stCfgc.u8UseExtendedEventLog);
}

uint8_t CFGC::GetUseMultipleProfiles()
{
    return(_stCfgc.u8UseMultipleProfiles);
}

uint8_t CFGC::GetProductRevision()
{
    return(_stCfgc.u8ProductRevision);
}

uint8_t CFGC::GetProductVersion()
{
    return(_stCfgc.u8ProductVersion);
}

uint8_t CFGC::GetProductSuffix()
{
    return(_stCfgc.u8ProductSuffix);
}
uint8_t CFGC::GetCustomerCode()
{
    return(_stCfgc.u8CustomerCode);
}
void CFGC::GetSeriesProductID(char acGCUSeries[])
{
    acGCUSeries[0] = _stCfgc.strSeriesProductID[0];
    acGCUSeries[1] = _stCfgc.strSeriesProductID[1];
    acGCUSeries[2] = _stCfgc.strSeriesProductID[2];
}

uint16_t CFGC::GetMaxNumberOfEvents()
{
   if(_stCfgc.u8UseExtendedEventLog == 1)
  {
      return MAX_EVENT_100;
  }
  else
  {
      return MAX_EVENT_30;
  }
}
uint16_t CFGC::GetPasswords(uint8_t u8PasswordNo)
{
    switch(u8PasswordNo)
    {
        case 0:
            return (uint16_t)((_stCfgc.u8PIN1PasswordChar0 * 1000U) + (_stCfgc.u8PIN1PasswordChar1 * 100U) +(_stCfgc.u8PIN1PasswordChar2 * 10U) + (_stCfgc.u8PIN1PasswordChar3));
        break;
        case 1:
            return (uint16_t)((_stCfgc.u8PIN2PasswordChar0 * 1000U) + (_stCfgc.u8PIN2PasswordChar1 * 100U) +(_stCfgc.u8PIN2PasswordChar2 * 10U) + (_stCfgc.u8PIN2PasswordChar3));
        break;
        case 2:
            return (uint16_t)((_stCfgc.u8PIN3PasswordChar0 * 1000U) + (_stCfgc.u8PIN3PasswordChar1 * 100U) +(_stCfgc.u8PIN3PasswordChar2 * 10U) + (_stCfgc.u8PIN3PasswordChar3));
        break;
        default:
            return (uint16_t)((_stCfgc.u8PIN1PasswordChar0 * 1000U) + (_stCfgc.u8PIN1PasswordChar1 * 100U) +(_stCfgc.u8PIN1PasswordChar2 * 10U) + (_stCfgc.u8PIN1PasswordChar3));
            break;
    }

}

BSP_STATUS_t CFGC::prvGetCFGCIntegrityCheck()
{
    CFGC_t stCFGC;

    _Hal.ObjDflash.Read(DFLASH_CFGC_START_ADDRESS, (uint8_t*)&stCFGC,
                sizeof(CFGC_t));

    CFGC_METADATA_t stCFGCMetadata;
    _Hal.ObjDflash.Read(DFLASH_CFGC_START_ADDRESS + DFLASH_CFGC_LENGTH -  sizeof(CFGC_METADATA_t), (uint8_t *)&stCFGCMetadata, sizeof(CFGC_METADATA_t));

    if(CRC16::ComputeCRCGeneric((uint8_t*)&stCFGC, sizeof(CFGC_t), CRC_MEMORY_SEED)
              == stCFGCMetadata.u16CFGCCrc)
    {
        return BSP_SUCCESS;
    }
    else
    {
        return BSP_FAIL;
    }
}

bool CFGC::IsC02Error()
{
    return _bIntegrityCheckFailed;
}

void cb(DFLASH::FLASH_STATUS_t evt)
{
    eStatus = evt;
}
