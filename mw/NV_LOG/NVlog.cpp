/*
* @file        [NVlog]
* @brief       This module uses different algorithms to store and maintain logs
*              in a Flash for the user. The input parameters for this module are:
*              1. Log Size
*              2. Block Size
*              3. Number of Blocks
*              4. Block Start Address
*              5. Data length
*              The output parameter is the status of the NVlog whether the write
*              or read has succeeded, it is in progress or there was some error
*              like data corruption, log size mismatch,etc.
*              Design:
*              The module is implemented as a super state machine with other smaller
*              state machines for read, write, initialization, etc. operating
*              within it. The update() function runs through one iteration of this
*              super state machine.
*              Initialization:
*              For initialization the max and min block revision numbers(BRN) are
*              located and based of this the read pointer is set to the start
*              of the block with min BRN and the write pointer is set to the
*              location after the last written log in the block with max BRN.
*              If the flash is new and nothing has been written then the the block
*              header along with its CRC is written for the first block and the
*              read and write pointer point to the start of this block.
*              Writing:
*              When writing a log, the interface function for write is called and
*              the log along with its calculated CRC is written to the location
*              pointed to by the write pointer.
*              The write pointer is then updated by adding the size of an entry to it,
*              if the write pointer spills over to the next block then that block
*              is erased and the write pointer points to the start of that block.
*              If the read pointer was pointing to a location in this new block then
*              it is moved forward by one block.
*              Reading:
*              When reading a log, the address to read from is calculated using the
*              read, write pointer and log number(passed by user). The log in this
*              address is read and if the CRC check passes this log is passed back
*              to the user via a data pointer passed by the user.
* @version     [Version number]
* @author      Naveen Ramaswamy, Sudeep Chandrasekaran
* @date        24th June 2021
* @author      None
* @date        None
* @copyright   SEDEMAC Mechatronics Pvt Ltd
*/


#include "NVlog.h"


/// Used to invert the byte order of a U16 number. This comes handy for conversion
/// between endianess.
#define UTILS_SWAP_UINT16(x) ((uint16_t) ((x >> 8) | (x << 8)) )

/// Used to invert the byte order of a U32 number. This comes handy for conversion
/// between endianess.
#define UTILS_SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000U) >> 8) |\
                         (((x) & 0x0000FF00U) << 8) | ((x) << 24))

static NV_LOG *pNVLog;

static void FlashCb(NV_LOG_STATUS_t evt);


NV_LOG::NV_LOG(NV_LOG_CFG_t &cfg, NV_LOG_Interface &interface):
_u16LogSize(cfg.uLogSize),
_u16EntrySize((uint16_t)(cfg.uLogSize + sizeof(uint32_t))),
_u32BlockStartAddress(cfg.u32BlockStartAddress),
_u16BlkSize(cfg.u16BlkSize),
_u16NoOfBlocks(cfg.uNoOfBlocks),
_u32ReadPointer(0),
_u32WritePointer(sizeof(BLOCK_HEADER_t)),
_u16minBRN(0),
_u16maxBRN(0),
_u16maxBRNBlockIdx(0),
_u16minBRNBlockIdx(0),
_au8LogBuffer{0},
_u16EntriesPerBlock((uint16_t)((_u16BlkSize-(sizeof(BLOCK_HEADER_t))) /_u16EntrySize) ),
_eRequestQueue(_eRequestArr, (uint16_t)MAX_REQUESTS),
_eSuperSMState(INITIALIZE),
_eInitWriteSMState(FIND_MIN_MAX_BRN),
_eReadWriteEraseState(INITIATE_REQUEST),
_NvLogIface(interface)
{
    pNVLog = this;
}

NV_LOG_STATUS_t NV_LOG::LogData(uint8_t *pu8Data, uint16_t u16LogSize,
                                                            NV_LOG_EVT_CB_t cb)
{
    /*Checking if the log size in the request matches the log size entered
      during initialization of the class object*/
    if(u16LogSize != _u16LogSize)
    {
        return NV_LOG_SIZE_MISMATCH;
    }
    prvCreateRequest(WRITE, pu8Data, u16LogSize, 0, cb);
    return NV_LOG_SUCCESS;
}

NV_LOG_STATUS_t NV_LOG::Read(uint8_t *pu8Data, uint16_t u16LogSize,
                                    uint16_t u16LogNumber,  NV_LOG_EVT_CB_t cb)
{
    if(u16LogSize != _u16LogSize)
    {
        return NV_LOG_SIZE_MISMATCH;
    }
    prvCreateRequest(READ, pu8Data, u16LogSize, u16LogNumber, cb);
    return NV_LOG_SUCCESS;
}

void NV_LOG::Update()
{
    switch(_eSuperSMState)
    {
        case INITIALIZE:
        {
            NV_LOG_STATUS_t eStatus = prvInitializeWrite(NULL, 0);
            if(eStatus == NV_LOG_SUCCESS)
            {
                _eSuperSMState = WAIT_FOR_REQUEST;
            }
            /*Don't do anything if there is a flash error, keep retrying*/
            else if(eStatus == NV_LOG_FLASH_ERR)
            {

            }
            break;
        }
        case WAIT_FOR_REQUEST:
        {
            /*Exit if there are no requests*/
            REQUEST_PARAMS_t _requestParams;
            if (!(_eRequestQueue.Peek(&_requestParams)))
            {
                break;
            }
            if(_requestParams._eRequestType == WRITE)
            {
                _eInitWriteSMState = INITIATE_WRITE;
                _eSuperSMState = WRITE_LOG;
            }
            else
            {
                _eSuperSMState = READ_LOG;
            }
            break;
        }
        case READ_LOG:
        {
            REQUEST_PARAMS_t _requestParams;
            _eRequestQueue.Peek(&_requestParams);
            NV_LOG_STATUS_t eStatus = prvReadLog(_requestParams.pu8ReadData,
                        _requestParams._u16DataLen, _requestParams._u16LogNumber);
            if(  (eStatus == NV_LOG_SUCCESS)   || 
                 (eStatus == NV_LOG_CORRUPT)   ||
                 (eStatus == NV_LOG_FLASH_ERR) || 
                 (eStatus == NV_LOG_LOG_NOT_AVAILABLE))
            {
                if(_requestParams._cb != NULL)
                {
                    _requestParams._cb(eStatus);
                }
                _eRequestQueue.DeQueue(&_requestParams);
                _eSuperSMState = WAIT_FOR_REQUEST;
            }
            break;
        }
        case WRITE_LOG:
        {
            REQUEST_PARAMS_t _requestParams;
            _eRequestQueue.Peek(&_requestParams);
            NV_LOG_STATUS_t eStatus = prvInitializeWrite(_requestParams.au8Data, 
                                                  _requestParams._u16DataLen);
            if( (eStatus == NV_LOG_SUCCESS) || (eStatus == NV_LOG_FLASH_ERR))
            {
                if(_requestParams._cb != NULL)
                {
                    _requestParams._cb(eStatus);
                }
                _eRequestQueue.DeQueue(&_requestParams);
                _eSuperSMState = WAIT_FOR_REQUEST;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

uint16_t NV_LOG::GetLogCount()
{
    uint16_t u16WholeBlockEntryCount;
    uint16_t u16PartialBlockEntryCount;
    if(_u32ReadPointer <= _u32WritePointer)
    {
        /*Number of blocks between the read and write pointer*/
        u16WholeBlockEntryCount = (uint16_t)(((_u32WritePointer-_u32ReadPointer)/_u16BlkSize)*_u16EntriesPerBlock);
        /*Checks if there already is a log in max BRN block*/
        u16PartialBlockEntryCount = (uint16_t)(((_u32WritePointer-_u32ReadPointer)%_u16BlkSize)/_u16EntrySize);
    }
    else
    {
        /*Same calculations as above with changes due to roll over*/
        u16WholeBlockEntryCount = (uint16_t)((_u16NoOfBlocks - 1)*_u16EntriesPerBlock);
        u16PartialBlockEntryCount = (uint16_t)(((_u32WritePointer%_u16BlkSize)-sizeof(BLOCK_HEADER_t))/_u16EntrySize);
    }
    return (uint16_t)(u16WholeBlockEntryCount + u16PartialBlockEntryCount);
}

void NV_LOG::FlashEventHandler(NV_LOG_STATUS_t evt)
{
    _eFlashTransactionStatus = evt;
}

void NV_LOG::prvCreateRequest(REQUEST_TYPE_t eRequest, uint8_t *pu8Data,
                                        uint16_t u16LogSize, uint16_t u16LogNumber,
                                                                 NV_LOG_EVT_CB_t cb)
{
    REQUEST_PARAMS_t _requestParams;
    _requestParams._eRequestType = eRequest;
    if(eRequest == READ)
    {
        _requestParams.pu8ReadData      = pu8Data;
    }
    else
    {
        memcpy(_requestParams.au8Data, pu8Data, u16LogSize);
    }
    _requestParams._u16DataLen   = u16LogSize;
    _requestParams._u16LogNumber = u16LogNumber;
    _requestParams._cb           = cb;
    _eRequestQueue.EnQueue(&_requestParams);
}

NV_LOG_STATUS_t NV_LOG::prvInitializeWrite(uint8_t *_pu8Data, uint16_t _u16DataLength)
{
    NV_LOG_STATUS_t eStatus=NV_LOG_IN_PROGRESS;
    switch(_eInitWriteSMState)
    {
        case FIND_MIN_MAX_BRN:
        {
            NV_LOG_STATUS_t eRes = prvUpdateMinMaxBRNBlk();
            if(eRes == NV_LOG_BRN_UNAVAILABLE)
            {
                _eInitWriteSMState = NEW_FLASH_INIT;
            }
            else if(eRes == NV_LOG_SUCCESS)
            {
                _u32WritePointer = _u32BlockStartAddress + (_u16maxBRNBlockIdx*_u16BlkSize)
                                                                + sizeof(BLOCK_HEADER_t);
                _u32ReadPointer = _u32BlockStartAddress + (_u16minBRNBlockIdx*_u16BlkSize)
                                                                + sizeof(BLOCK_HEADER_t);
                _eInitWriteSMState = UPDATE_WP;
            }
            eStatus = NV_LOG_IN_PROGRESS;
            break;
        }
        case NEW_FLASH_INIT:
        {
            NV_LOG_STATUS_t eRes = prvWriteBlkHeader(0, 0);
            if(eRes == NV_LOG_SUCCESS)
            {
                _u32ReadPointer  = _u32BlockStartAddress+sizeof(BLOCK_HEADER_t);
                _u32WritePointer = _u32ReadPointer;
                _eInitWriteSMState    = FIND_MIN_MAX_BRN;
                eStatus = NV_LOG_SUCCESS;
            }
            else if(eRes == NV_LOG_FLASH_ERR)
            {
                _eInitWriteSMState    = FIND_MIN_MAX_BRN;
                eStatus = NV_LOG_FLASH_ERR;
            }
            break;
        }
        case INITIATE_WRITE:
        {
            memcpy(_au8LogBuffer, _pu8Data, _u16DataLength);
            /*CRC calculation*/
            uint16_t u16CRC = _NvLogIface.CalculateModbusCRC((uint8_t*)_au8LogBuffer,
                                                                        _u16DataLength);
            _au8LogBuffer[_u16DataLength] = (uint8_t)(u16CRC>>8);
            _u16DataLength++;
            _au8LogBuffer[_u16DataLength] = (uint8_t)(u16CRC & 0xFFU);
            _u16DataLength++;
            _au8LogBuffer[_u16DataLength] = 0xFF;
            _u16DataLength++;
            _au8LogBuffer[_u16DataLength] = 0xFF;
            _u16DataLength++;
            _eInitWriteSMState = WRITE_TO_FLASH;
            eStatus = NV_LOG_IN_PROGRESS;
            break;
        }
        case WRITE_TO_FLASH:
        {
            NV_LOG_STATUS_t eRes = prvWriteFlash(_u32WritePointer, _au8LogBuffer,
                                            _u16EntrySize);
            eStatus = NV_LOG_IN_PROGRESS;
            if(eRes == NV_LOG_SUCCESS)
            {
                _eInitWriteSMState = UPDATE_WP;
            }
            else if(eRes == NV_LOG_FLASH_ERR)
            {
                _eInitWriteSMState    = FIND_MIN_MAX_BRN;
                eStatus = NV_LOG_FLASH_ERR;
            }
            break;
        }
        case UPDATE_WP:
        {
            NV_LOG_STATUS_t eRes = prvIsLogEntryBlank(_u32WritePointer); 
            if(eRes == NV_LOG_LOCATION_BLANK)
            {
                _eInitWriteSMState = FIND_MIN_MAX_BRN;
                eStatus       = NV_LOG_SUCCESS;
            }
            else if(eRes == NV_LOG_LOCATION_NOT_BLANK)
            {
                _u32WritePointer += _u16EntrySize;
                /*Check whether the write pointer has moved to the next page*/
                uint16_t u16NextBlkIdx     = (uint16_t)(_u16maxBRNBlockIdx+1);
                uint32_t u32NextBlkAddress = _u32BlockStartAddress + (u16NextBlkIdx*_u16BlkSize);
                if((_u32WritePointer >= u32NextBlkAddress) || 
                        ((_u32WritePointer + _u16EntrySize) >= u32NextBlkAddress))
                {
                    _eInitWriteSMState = WRITE_BLK_EXHAUSTED;
                }
                eStatus = NV_LOG_IN_PROGRESS;
            }
            else if(eRes == NV_LOG_FLASH_ERR)
            {
                _eInitWriteSMState = FIND_MIN_MAX_BRN;
                eStatus            = NV_LOG_FLASH_ERR;
            }
            break;
        }
        case WRITE_BLK_EXHAUSTED:
        {
            uint16_t u16NextBlkIdx = (uint16_t)((_u16maxBRNBlockIdx+1)%_u16NoOfBlocks);
            NV_LOG_STATUS_t eRes = prvWriteBlkHeader(u16NextBlkIdx, _u16maxBRN+1);
            if(eRes == NV_LOG_SUCCESS)
            {
                _u16maxBRN++;
                _u16maxBRNBlockIdx = u16NextBlkIdx;
                _u32WritePointer = _u32BlockStartAddress+
                    (_u16maxBRNBlockIdx*_u16BlkSize)+sizeof(BLOCK_HEADER_t);
                /*If the read pointer was pointing to the block we just
                  wrote, then update the read address.
                 */
                if(_u16maxBRNBlockIdx == _u16minBRNBlockIdx)
                {
                    _u16minBRNBlockIdx = (uint16_t)((_u16minBRNBlockIdx+1)%_u16NoOfBlocks);
                    _u32ReadPointer  = _u32BlockStartAddress+
                        (_u16minBRNBlockIdx*_u16BlkSize)+sizeof(BLOCK_HEADER_t);
                }
                _eInitWriteSMState = FIND_MIN_MAX_BRN;
                eStatus = NV_LOG_SUCCESS;
            }
            else if(eRes == NV_LOG_FLASH_ERR)
            {
                _eInitWriteSMState = FIND_MIN_MAX_BRN;
                eStatus = NV_LOG_FLASH_ERR;
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return eStatus;
}


NV_LOG_STATUS_t NV_LOG::prvReadLog(uint8_t *pu8Data, uint16_t u16LogSize,
                                                           uint16_t u16LogNumber)
{
    /*Check if log exists*/
    if(GetLogCount() <= u16LogNumber)
    {
        return NV_LOG_LOG_NOT_AVAILABLE;
    }
    NV_LOG_STATUS_t eStatus;
    uint32_t u32Address = _u32ReadPointer + _u16BlkSize*(u16LogNumber/_u16EntriesPerBlock) +
                                    ((_u16EntrySize) * (u16LogNumber % _u16EntriesPerBlock));
    /*Case for roll over*/
    u32Address = _u32BlockStartAddress + 
                    ((u32Address - _u32BlockStartAddress) % (_u16BlkSize * _u16NoOfBlocks));

    eStatus = prvReadFlash(u32Address, _au8LogBuffer, _u16EntrySize);
    if(eStatus == NV_LOG_SUCCESS)
    {
        uint16_t u16ComputedCRC = _NvLogIface.CalculateModbusCRC(_au8LogBuffer, _u16LogSize);
        uint16_t u16StoredCRC = (uint16_t)(((uint16_t)_au8LogBuffer[u16LogSize]<<8) |
                                            (_au8LogBuffer[u16LogSize+1]));
        /*Checking for log corruption*/
        if(u16ComputedCRC != u16StoredCRC)
        {
            return NV_LOG_CORRUPT;
        }
        memcpy(pu8Data, _au8LogBuffer, _u16LogSize);
    }
    return eStatus;
}


NV_LOG_STATUS_t NV_LOG::prvUpdateMinMaxBRNBlk()
{
    /*Determine min & max BRN*/
    static uint8_t        u8BlkIdx=0;
    static bool           bValidBlocksPresent = false;
    static BLOCK_HEADER_t stBlkHeader;
    if(u8BlkIdx < _u16NoOfBlocks)
    {
        NV_LOG_STATUS_t eStatus = prvFetchBH(u8BlkIdx, stBlkHeader);
        if(eStatus == NV_LOG_SUCCESS)
        {
            if(!bValidBlocksPresent)
            {
                bValidBlocksPresent = true;
                _u16maxBRN         = (uint16_t)stBlkHeader._u32BRN;
                _u16minBRN         = (uint16_t)stBlkHeader._u32BRN;
                _u16maxBRNBlockIdx = u8BlkIdx;
                _u16minBRNBlockIdx = u8BlkIdx;
            }
            if(_u16maxBRN < stBlkHeader._u32BRN)
            {
                _u16maxBRN = (uint16_t)stBlkHeader._u32BRN;
                _u16maxBRNBlockIdx = u8BlkIdx;
            }
            if(_u16minBRN > stBlkHeader._u32BRN)
            {
                _u16minBRN = (uint16_t)stBlkHeader._u32BRN;
                _u16minBRNBlockIdx = u8BlkIdx;
            }
            u8BlkIdx++;
        }
        else if(eStatus == NV_LOG_BRN_CORRUPTED)
        {
            u8BlkIdx++;   
        }
        else if(eStatus == NV_LOG_FLASH_ERR)
        {
            u8BlkIdx = 0;
            bValidBlocksPresent = false;
            return NV_LOG_FLASH_ERR;
        }
        return NV_LOG_IN_PROGRESS;
    }
    u8BlkIdx = 0;
    if(bValidBlocksPresent)
    {
        bValidBlocksPresent =  false;
        return NV_LOG_SUCCESS;
    }
    bValidBlocksPresent =  false;
    return NV_LOG_BRN_UNAVAILABLE;
}

NV_LOG_STATUS_t NV_LOG::prvFetchBH(uint16_t u8BlkIdx, BLOCK_HEADER_t &header)
{
    uint32_t u32BlockAddr = _u32BlockStartAddress + (u8BlkIdx*_u16BlkSize);
    NV_LOG_STATUS_t eStatus = prvReadFlash(u32BlockAddr, (uint8_t*)&header, sizeof(BLOCK_HEADER_t)); 
    if(eStatus == NV_LOG_SUCCESS)
    {
        uint16_t u16CalculatedCRC = _NvLogIface.CalculateModbusCRC((uint8_t*)&header,
                                                    sizeof(BLOCK_HEADER_t)-sizeof(uint16_t));
        /*Change endianess*/
        header._u32BRN      = UTILS_SWAP_UINT32(header._u32BRN);
        header._u16Reserved = UTILS_SWAP_UINT16(header._u16Reserved);
        header._u16CRC      = UTILS_SWAP_UINT16(header._u16CRC);
        if(u16CalculatedCRC != header._u16CRC)
        {
            eStatus = NV_LOG_BRN_CORRUPTED;
        }
    }
    return eStatus;
}

NV_LOG_STATUS_t NV_LOG::prvWriteBlkHeader(uint16_t u16BlkIdx, uint32_t u32BRN)
{
    static WRITE_BH_STATES_t eState = ERASE_BLOCK;
    static uint8_t sau8WriteBuff[sizeof(BLOCK_HEADER_t)] = {0};
    NV_LOG_STATUS_t eStatus = NV_LOG_IN_PROGRESS;

    switch(eState)
    {
        case ERASE_BLOCK:
        {

            NV_LOG_STATUS_t eRes = prvEraseFlash(u16BlkIdx);
            if(eRes == NV_LOG_SUCCESS)
            {
                eState = WRITE_HEADER;
                sau8WriteBuff[0] = (uint8_t)(u32BRN>>24);
                sau8WriteBuff[1] = (uint8_t)(u32BRN>>16);
                sau8WriteBuff[2] = (uint8_t)(u32BRN>>8);
                sau8WriteBuff[3] = (uint8_t)(u32BRN & 0xFFU);
                sau8WriteBuff[4] = 0xFF;
                sau8WriteBuff[5] = 0xFF;
                uint16_t u16CRC = _NvLogIface.CalculateModbusCRC(sau8WriteBuff, 
                                                    sizeof(uint32_t) + sizeof(uint16_t));
                sau8WriteBuff[6] = (uint8_t)(u16CRC>>8);
                sau8WriteBuff[7] = (uint8_t)(u16CRC & 0xFFU);
            }
            else if(eRes == NV_LOG_FLASH_ERR)
            {
                eStatus = NV_LOG_FLASH_ERR;
            }
            eStatus = NV_LOG_IN_PROGRESS;
            break;
        }
        case WRITE_HEADER:
        {
            uint32_t u32Address = _u32BlockStartAddress+(u16BlkIdx*_u16BlkSize);
            NV_LOG_STATUS_t eRes = prvWriteFlash(u32Address, sau8WriteBuff,sizeof(sau8WriteBuff));
            if(eRes == NV_LOG_SUCCESS)
            {
                eStatus = NV_LOG_SUCCESS;
                eState = ERASE_BLOCK;
            }
            else if(eRes == NV_LOG_FLASH_ERR)
            {
                eStatus = NV_LOG_FLASH_ERR;
                eState = ERASE_BLOCK;
            }
            break;
        }
    }
    return eStatus;
}

/*This is a custom implementation and shall based on flash type*/
NV_LOG_STATUS_t NV_LOG::prvIsLogEntryBlank(uint32_t u32Address)
{
    NV_LOG_STATUS_t eStatus = NV_LOG_IN_PROGRESS;
    switch(_eReadWriteEraseState)
    {
        case INITIATE_REQUEST:
        {
            eStatus = NV_LOG_IN_PROGRESS;
            _eFlashTransactionStatus = NV_LOG_TRANSACTION_PENDING;
            _eReadWriteEraseState = WAIT_FOR_COMPLETION;
            if(NV_LOG_SUCCESS != _NvLogIface.IsLocationBlank(u32Address,  
                                            _u16EntrySize, FlashCb) )
            {
                _eReadWriteEraseState = INITIATE_REQUEST;
                eStatus = NV_LOG_FLASH_ERR;
            }
            break;
        }
        case WAIT_FOR_COMPLETION:
        {
            if(_eFlashTransactionStatus != NV_LOG_TRANSACTION_PENDING)
            {
                eStatus = _eFlashTransactionStatus;
                _eReadWriteEraseState = INITIATE_REQUEST;
            }
            break;
        }
        default:
        {
            _eReadWriteEraseState = INITIATE_REQUEST;
            break;
        }
    }
    return eStatus;
}

NV_LOG_STATUS_t NV_LOG::prvReadFlash(uint32_t u32Address, uint8_t *pu8Data, uint16_t u16Size)
{
    NV_LOG_STATUS_t eStatus = NV_LOG_IN_PROGRESS;
    switch(_eReadWriteEraseState)
    {
        case INITIATE_REQUEST:
        {
            eStatus = NV_LOG_IN_PROGRESS;
            _eFlashTransactionStatus = NV_LOG_TRANSACTION_PENDING;
            _eReadWriteEraseState = WAIT_FOR_COMPLETION;
            if(NV_LOG_SUCCESS != _NvLogIface.Read(u32Address, pu8Data, u16Size, FlashCb))
            {
                _eReadWriteEraseState = INITIATE_REQUEST;
                eStatus = NV_LOG_FLASH_ERR;
            }
            break;
        }
        case WAIT_FOR_COMPLETION:
        {
            if(_eFlashTransactionStatus != NV_LOG_TRANSACTION_PENDING)
            {
                eStatus = _eFlashTransactionStatus;
                _eReadWriteEraseState = INITIATE_REQUEST;
            }
            break;
        }
        default:
        {
            _eReadWriteEraseState = INITIATE_REQUEST;
            break;
        }
    }
    return eStatus;
}

NV_LOG_STATUS_t NV_LOG::prvEraseFlash(uint16_t u16BlkIdx)
{
    NV_LOG_STATUS_t eStatus = NV_LOG_IN_PROGRESS;
    switch(_eReadWriteEraseState)
    {
        case INITIATE_REQUEST:
        {
            eStatus = NV_LOG_IN_PROGRESS;
            _eFlashTransactionStatus = NV_LOG_TRANSACTION_PENDING;
            uint32_t u32Address = _u32BlockStartAddress+(u16BlkIdx*_u16BlkSize);
            _eReadWriteEraseState = WAIT_FOR_COMPLETION;
            if(NV_LOG_SUCCESS != _NvLogIface.Erase(u32Address, 1, FlashCb))
            {
                 eStatus = NV_LOG_FLASH_ERR;
                _eReadWriteEraseState = INITIATE_REQUEST;
            }
            break;
        }
        case WAIT_FOR_COMPLETION:
        {
            eStatus = NV_LOG_IN_PROGRESS;
            if(_eFlashTransactionStatus != NV_LOG_TRANSACTION_PENDING)
            {
                eStatus = _eFlashTransactionStatus;
                _eReadWriteEraseState = INITIATE_REQUEST;
            }
            break;
        }
        default:
        {
            _eReadWriteEraseState = INITIATE_REQUEST;
            break;
        }
    }
    return eStatus;
}

NV_LOG_STATUS_t NV_LOG::prvWriteFlash(uint32_t u32Address, 
                                            uint8_t *pu8Data, uint16_t u16DataLength)
{
    NV_LOG_STATUS_t eStatus = NV_LOG_IN_PROGRESS;
    switch(_eReadWriteEraseState)
    {
        case INITIATE_REQUEST:
        {
            eStatus = NV_LOG_IN_PROGRESS;
            _eFlashTransactionStatus = NV_LOG_TRANSACTION_PENDING;
            _eReadWriteEraseState = WAIT_FOR_COMPLETION;
            if(NV_LOG_SUCCESS != _NvLogIface.Write(u32Address, pu8Data, u16DataLength, FlashCb))
            {
                 eStatus = NV_LOG_FLASH_ERR;
                _eReadWriteEraseState = INITIATE_REQUEST;
            }
            break;
        }
        case WAIT_FOR_COMPLETION:
        {
            eStatus = NV_LOG_IN_PROGRESS;
            if(_eFlashTransactionStatus != NV_LOG_TRANSACTION_PENDING)
            {
                eStatus = _eFlashTransactionStatus;
                _eReadWriteEraseState = INITIATE_REQUEST;
            }
            break;
        }
        default:
        {
            _eReadWriteEraseState = INITIATE_REQUEST;
            break;
        }
    }
    return eStatus;
}

static void FlashCb(NV_LOG_STATUS_t evt)
{
    if(pNVLog != NULL)
    {
        pNVLog->FlashEventHandler(evt);
    }
}
