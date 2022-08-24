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


#ifndef _NV_LOG_H_
#define _NV_LOG_H_

#include "C_QUEUE.h"
#include "NV_LOG_Interface.h"

class NV_LOG
{
    public:
        /*The maximum size of each entry in the log, this is used to determine the
          internal RAM buffer sizes*/
        #define MAX_LOG_SIZE (30)
        /* Defines the maximum number of requests(read/write) than can be queued
         */
        #define MAX_REQUESTS (5)
        
        /* This data structure the log configurations parameters.
         */
        typedef struct {
            uint16_t uLogSize;
            uint32_t u32BlockStartAddress;
            uint16_t u16BlkSize;
            uint16_t uNoOfBlocks;
        } NV_LOG_CFG_t;

        /* Represents the type of request(read/write)
         */
        typedef enum {
            WRITE,
            READ,
        } REQUEST_TYPE_t;

        /**
        * Constructs the class
        * @param cfg - Contains configuration details like log size, block size,
        *               number of blocks and block start details
        * @param interface - Interface to flash read, write and erase function
        * @return
        * None
        */
        NV_LOG(NV_LOG_CFG_t &cfg, NV_LOG_Interface &interface);

        /**
        * Goes through one cycle of the working of the super state machine
        * @param - None
        * @return
        * None
        */

        void Update();

        /**
        * Calls the function to create a write request and enqueue it.
        * @param pu8Data - Log data to write to flash
        * @param uLogSize - Length of log to write to flash
        * @param cb - Callback function to be called at the end of writing the log
        * @return
        * Status of the NV log
        */
        NV_LOG_STATUS_t LogData(uint8_t *pu8Data, uint16_t uLogSize,
                                                                NV_LOG_EVT_CB_t cb);
        
        /**
        * Calls the function to create a read request and enqueue it.
        * @param pu8Data - Pointer to location where log data from flash has
        *                   to be read to
        * @param uLogSize - Length of log to read from flash
        * @param uLogNumber - Gives the uLogNumber last log that exists in flash
        * @param cb - Callback function to be called at the end of reading the log
        * @return
        * Status of the NV log
        */
        NV_LOG_STATUS_t Read(uint8_t *pu8Data, uint16_t uLogSize, uint16_t uLogNumber,
                                                                   NV_LOG_EVT_CB_t cb);

        /**
        * Gets the total number of logs that currently exist in the flash.
        * @param - None
        * @return
        * Number of logs that currently exist in flash
        */
        uint16_t GetLogCount();

        /**
        * Used to set the _FlashTransactionPending variable to false once the flash
        *                               callback is called.
        * @param evt - Status of the NV log
        * @return
        * None
        */
        void FlashEventHandler(NV_LOG_STATUS_t evt);

    private:
        typedef struct {
            uint32_t _u32BRN;
            uint16_t _u16Reserved;
            uint16_t _u16CRC;
        } BLOCK_HEADER_t;

        typedef enum {
            INITIALIZE,
            WAIT_FOR_REQUEST,
            READ_LOG,
            WRITE_LOG
        } SUPER_SM_STATES_t;

        typedef enum {
            FIND_MIN_MAX_BRN,
            NEW_FLASH_INIT,
            INITIATE_WRITE,
            WRITE_TO_FLASH,
            UPDATE_WP,
            WRITE_BLK_EXHAUSTED
        } INITIALIZE_WRITE_SM_t;

        typedef enum {
            INITIATE_REQUEST,
            WAIT_FOR_COMPLETION
        } READ_WRITE_ERASE_SM_t;

        typedef enum {
            ERASE_BLOCK,
            WRITE_HEADER
        } WRITE_BH_STATES_t;

        typedef struct {
            REQUEST_TYPE_t _eRequestType;
            uint8_t  au8Data[MAX_LOG_SIZE];
            uint8_t  *pu8ReadData;
            uint16_t _u16DataLen;
            uint16_t _u16LogNumber;
            NV_LOG_EVT_CB_t _cb;
        } REQUEST_PARAMS_t;

        uint16_t                        _u16LogSize;
        uint16_t                        _u16EntrySize;
        uint32_t                        _u32BlockStartAddress;
        uint16_t                        _u16BlkSize;
        uint16_t                        _u16NoOfBlocks;
        volatile NV_LOG_STATUS_t        _eFlashTransactionStatus;
        uint32_t                        _u32ReadPointer;
        uint32_t                        _u32WritePointer;
        uint16_t                        _u16minBRN;
        uint16_t                        _u16maxBRN;
        uint16_t                        _u16maxBRNBlockIdx;
        uint16_t                        _u16minBRNBlockIdx;
        uint8_t                         _au8LogBuffer[MAX_LOG_SIZE + sizeof(uint32_t)];
        uint16_t                        _u16EntriesPerBlock;
        CircularQueue<REQUEST_PARAMS_t> _eRequestQueue;
        REQUEST_PARAMS_t                _eRequestArr[MAX_REQUESTS];
        SUPER_SM_STATES_t               _eSuperSMState;
        INITIALIZE_WRITE_SM_t           _eInitWriteSMState;
        READ_WRITE_ERASE_SM_t           _eReadWriteEraseState;
        NV_LOG_Interface                &_NvLogIface;


        /**
        * Calls the function to either initialize the flash or write to it
        * @param _pu8Data - Log data to write to flash
        * @param _u16DataLength - Length of log to write to the flash
        * @return
        * Status of the NV log
        */
        NV_LOG_STATUS_t prvInitializeWrite(uint8_t *_pu8Data, uint16_t _u16DataLength);

        /**
        * Updates the min max BRN and the blocks with the min and max BRN.
        * @param - None
        * @return
        * Status of the NV log
        */
        NV_LOG_STATUS_t prvUpdateMinMaxBRNBlk();

        /**
        * Writes the passed in BRN to the header of the block passed as argument.
        * @param u16BlkIdx - Current block index
        * @param u32BRN - Latest BRN to write to the block header
        * @return
        * Status of the NV log
        */
        NV_LOG_STATUS_t prvWriteBlkHeader(uint16_t u16BlkIdx, uint32_t u32BRN);

        /**
        * Reads the block header from a specified block.
        * @param u16BlkIdx - The block index from where BH needs to be read
        * @param header    - Destination where the block header needs to be copied.
        * @return
        * Status of the NV log
        */
        NV_LOG_STATUS_t prvFetchBH(uint16_t u8BlkIdx, BLOCK_HEADER_t &header);

        /**
        * Reads data from the flash.
        * @param u32Address - Offset used to index to the location within the flash to read
        * @param pu8Data - Pointer to location where log data from flash has
        *                   to be read to
        * @param u16Size - Length of data to be read from flash.
        * @return
        * Status of the NV log
        */
        NV_LOG_STATUS_t prvReadFlash(uint32_t u32Address, uint8_t *pu8Data,
                                                                 uint16_t u16Size);

        /**
        * Erases a block from the flash.
        * @param u16BlkIdx - Index of block to be erased
        * @return
        * Status of the NV log
        */
        NV_LOG_STATUS_t prvEraseFlash(uint16_t u16BlkIdx);

        /**
        * Writes data to the flash.
        * @param u32Address - Offset used to index to the location within the
        *                       flash to write
        * @param pu8Data - Data to write to flash
        * @param u16DataLength - Length of data to write to flash
        * @return
        * Status of the NV log
        */
        NV_LOG_STATUS_t prvWriteFlash(uint32_t u32Address, 
                                    uint8_t *pu8Data, uint16_t u16DataLength);

        /**
        * Checks whether there is space within the block to write a log.
        * @param u32Addr - Offset used to index to the location within the flash to read
        * @return
        * Status of the NV log
        */
        NV_LOG_STATUS_t prvIsLogEntryBlank(uint32_t u32Address);

        /**
        * Function to create a read or write request and enqueue it.
        * @param eRequest - Enum to specify whether it is a read or write request
        * @param pu8Data - Pointer to location where log data from flash has
        *                   to be read to
        * @param u16LogSize - Length of log to read from flash or write to flash
        * @param u16LogNumber - Gives the u16LogNumber last log that exists in flash
        * @param cb - Callback function to be called at the end of reading/writing the log
        * @return
        * Status of the NV log
        */
        void            prvCreateRequest(REQUEST_TYPE_t eRequest, uint8_t *pu8Data,
                                    uint16_t u16LogSize, uint16_t u16LogNumber,
                                                          NV_LOG_EVT_CB_t cb);

        /**
        * Calls the function to read logs from the flash.
        * @param pu8Data - Pointer to location where log data from flash has
        *                   to be read to
        * @param u16LogSize - Length of log to read from flash
        * @param u16LogNumber - Gives the u16LogNumber last log that exists in flash
        * @return
        * Status of the NV log
        */
        NV_LOG_STATUS_t prvReadLog(uint8_t *pu8Data, uint16_t u16LogSize, 
                                                           uint16_t u16LogNumber);
};

#endif
