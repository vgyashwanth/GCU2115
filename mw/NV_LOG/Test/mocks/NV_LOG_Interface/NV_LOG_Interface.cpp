#include "NV_LOG_Interface.h"
#include "UTILS_Crc.h"
#include "FLASH.h"


static NV_LOG_Interface *pInterface;

static FLASH flash;

static bool _bReadComplete = false;

static void FlashCb(FLASH::FLASH_STATUS_t evt);

NV_LOG_Interface::NV_LOG_Interface(DFLASH &dflash)
{

}

NV_LOG_Interface::NV_LOG_Interface()
{
    pInterface = this;
}

NV_LOG_STATUS_t NV_LOG_Interface::Write(uint32_t u32Addr, uint8_t *pu8Data,
                                    uint32_t u32DataLen,            NV_LOG_EVT_CB_t cb)
{
    _cb  = cb;
    flash.Write(u32Addr, pu8Data, u32DataLen, FlashCb);
    return NV_LOG_SUCCESS;
}

NV_LOG_STATUS_t NV_LOG_Interface::Read(uint32_t u32Addr, uint8_t *pu8Data, 
                                    uint32_t u32DataLen,            NV_LOG_EVT_CB_t cb)
{
    _cb  = cb;
    flash.Read(u32Addr, pu8Data, u32DataLen, FlashCb);
    return NV_LOG_SUCCESS;
}

NV_LOG_STATUS_t NV_LOG_Interface::Erase(uint32_t u32Addr, uint16_t u16NoOfBlocks,
                                                                    NV_LOG_EVT_CB_t cb)
{
    _cb  = cb;
    flash.Erase(u32Addr, u16NoOfBlocks, FlashCb);
    return NV_LOG_SUCCESS;
}

NV_LOG_STATUS_t NV_LOG_Interface::IsLocationBlank(uint32_t u32Addr, uint16_t u16NoOfBytes, NV_LOG_EVT_CB_t cb)
{
    _cb = cb;
    flash.IsLocationBlank(u32Addr, u16NoOfBytes, FlashCb);
    return NV_LOG_SUCCESS;
}

uint16_t NV_LOG_Interface::CalculateModbusCRC(const uint8_t *pu8Data, uint16_t u16Len)
{
    return Calculate_CRC16CCITXMODEM(pu8Data, u16Len);
}

void NV_LOG_Interface::HandleFlashEvent(NV_LOG_STATUS_t eStatus)
{
    if(_cb != NULL)
    {
        _cb(eStatus);
    }
}

static void FlashCb(FLASH::FLASH_STATUS_t evt)
{
    if(pInterface != NULL)
    {
        if( (evt == FLASH::DFLASH_WRITE_SUCCESS) ||
            (evt == FLASH::DFLASH_ERASE_SUCCESS) ||
            (evt == FLASH::DFLASH_READ_SUCCESS))
        {
            pInterface->HandleFlashEvent(NV_LOG_SUCCESS);
            _bReadComplete = true;
        }
        else if(evt == FLASH::DFLASH_LOCATION_BLANK)
        {
            pInterface->HandleFlashEvent(NV_LOG_LOCATION_BLANK);
        }
        else if(evt == FLASH::DFLASH_LOCATION_NOT_BLANK)
        {
            pInterface->HandleFlashEvent(NV_LOG_LOCATION_NOT_BLANK);
        }
        else
        {
            pInterface->HandleFlashEvent(NV_LOG_FLASH_ERR); 
        }
    }
}
