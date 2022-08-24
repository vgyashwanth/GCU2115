/*
* @file        [NV_LOG_Interface]
* @brief       This module acts as an interface between the NV_LOG module and 
*              the FLASH module. The NV_LOG module does not interact with the
*              flash directly and calls functions within this module to perform
*              reads, writes and erases on the flash. Other function provided by
*              this module are a funtion to check if a given location is empty 
*              and also a CRC calculation function.
* @version     [Version number]
* @author      Naveen Ramaswamy, Sudeep Chandrasekaran
* @date        24th June 2021
* @author      None
* @date        None
* @copyright   SEDEMAC Mechatronics Pvt Ltd
*/


#include "NV_LOG_Interface.h"
#include "string.h"
#include "DFLASH/DFLASH.h"
#include "CRC16.h"


static NV_LOG_Interface *pInterface;

static void FlashCb(DFLASH::FLASH_STATUS_t evt);

NV_LOG_Interface::NV_LOG_Interface(DFLASH &dflash)
:_cb(NULL),
_pDFlash(&dflash)
{
    pInterface = this;
}

NV_LOG_Interface::NV_LOG_Interface()
:_cb(NULL),
 _pDFlash(NULL)
{
    pInterface = this;
}

NV_LOG_STATUS_t NV_LOG_Interface::Write(uint32_t u32Addr, uint8_t *pu8Data,
                                    uint32_t u32DataLen, NV_LOG_EVT_CB_t cb)
{
    _cb = cb;
    _pDFlash->RequestWrite(u32Addr, pu8Data, u32DataLen, FlashCb);
    return NV_LOG_SUCCESS;
}

NV_LOG_STATUS_t NV_LOG_Interface::Read(uint32_t u32Addr, uint8_t *pu8Data, 
                                    uint32_t u32DataLen, NV_LOG_EVT_CB_t cb)
{
    _cb = cb;
    _pDFlash->Read(u32Addr, pu8Data, u32DataLen);
    _cb(NV_LOG_SUCCESS);
    return NV_LOG_SUCCESS;
}

NV_LOG_STATUS_t NV_LOG_Interface::Erase(uint32_t u32Addr, uint16_t u16NoOfBlocks,
                                                                 NV_LOG_EVT_CB_t cb)
{
    _cb = cb;
    _pDFlash->RequestErase(u32Addr, u16NoOfBlocks, FlashCb);
    return NV_LOG_SUCCESS;
}

NV_LOG_STATUS_t NV_LOG_Interface::IsLocationBlank(uint32_t u32Addr, uint16_t u16NoOfBytes,
                                                                    NV_LOG_EVT_CB_t cb)
{
    _cb = cb;
    _pDFlash->RequestBlankCheck(u32Addr, u16NoOfBytes, FlashCb);
    return NV_LOG_SUCCESS;
}

uint16_t NV_LOG_Interface::CalculateModbusCRC(const uint8_t *pu8Data, uint16_t u16Len)
{
    return CRC16::ComputeModbusCRC((uint8_t*)pu8Data, u16Len);
}

void NV_LOG_Interface::HandleFlashEvent(NV_LOG_STATUS_t eStatus)
{
    if(_cb != NULL)
    {
        _cb(eStatus);
    }
}

static void FlashCb(DFLASH::FLASH_STATUS_t evt)
{

    if(pInterface != NULL)
    {
        if( (evt == DFLASH::DFLASH_WRITE_SUCCESS) ||
            (evt == DFLASH::DFLASH_ERASE_SUCCESS) ||
            (evt == DFLASH::DFLASH_READ_SUCCESS))
        {
            pInterface->HandleFlashEvent(NV_LOG_SUCCESS);
        }
        else if(evt == DFLASH::DFLASH_BLANK)
        {
            pInterface->HandleFlashEvent(NV_LOG_LOCATION_BLANK);   
        }
        else if(evt == DFLASH::DFLASH_NOT_BLANK)
        {
            pInterface->HandleFlashEvent(NV_LOG_LOCATION_NOT_BLANK);   
        }
        else
        {
            pInterface->HandleFlashEvent(NV_LOG_FLASH_ERR); 
        }
    }   
}
