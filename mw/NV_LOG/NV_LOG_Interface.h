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


#ifndef _NV_LOG_INTERFACE_H_
#define _NV_LOG_INTERFACE_H_

#include "stdint.h"
#include "NV_LOG_DS.h"

class DFLASH;

class NV_LOG_Interface {

public:

    /**
    * Constructs the class
    * @param dflash - Reference to the DFLASH object to use.
    * @return
    * None
    */
    NV_LOG_Interface(DFLASH &dflash);

    /**
    * Constructs the class
    * @param - None
    * @return
    * None
    */
    NV_LOG_Interface();

    /**
    * Writes the data to the Flash.
    * @param u32Addr - Offset used to index to the location within the flash to write
    * @param pu8Data - Data to write to flash
    * @param u32DataLen - Length of data to write to flash
    * @param cb - Callback function to be called at the end of write
    * @return
    * Status of the NV log
    */
    NV_LOG_STATUS_t Write(uint32_t u32Addr, uint8_t *pu8Data,
                                        uint32_t u32DataLen, NV_LOG_EVT_CB_t cb);

    /**
    * Reads the data from the Flash.
    * @param u32Addr - Offset used to index to the location within the flash to read
    * @param pu8Data - Pointer of location to read data to from flash
    * @param u32DataLen - Length of data to read from flash
    * @param cb - Callback function to be called at the end of read
    * @return
    * Status of the NV log
    */
    NV_LOG_STATUS_t Read(uint32_t u32Addr, uint8_t *pu8Data, 
                                        uint32_t u32DataLen, NV_LOG_EVT_CB_t cb);

    /**
    * Erases a block or a few blocks of data from the flash.
    * @param u32Addr - Offset used to index to the block within the flash to start
    *                   erasing from
    * @param u16NoOfBlocks - Number of blocks to erase within the flash.
    * @param cb - Callback function to be called at the end of erase
    * @return
    * Status of the NV log
    */
    NV_LOG_STATUS_t Erase(uint32_t u32Addr, uint16_t u16NoOfBlocks,
                                                             NV_LOG_EVT_CB_t cb);

    /**
    * Checks if the location of the given address in the flash is blank.
    * @param u32Addr - The location within the flash to start checking from
    * @param u16NoOfBytes - Number of bytes to check starting from the address given
    * @param cb - Callback function to be called at the end of the check
    * @return
    * None
    */
    NV_LOG_STATUS_t IsLocationBlank(uint32_t u32Addr, uint16_t u16NoOfBytes,
                                                            NV_LOG_EVT_CB_t cb);

    /**
    * Calculates the CRC for the data
    * @param pu8Data - Data for which CRC has to be calculated
    * @param u16Len - Length of the data
    * @return
    * Calculated CRC
    */
    uint16_t CalculateModbusCRC(const uint8_t *pu8Data, uint16_t u16Len);

    /**
    * Calls the call back function with passing the status as an argument
    * @param eStatus - Status of the NV log
    * @return
    * None
    */
    void HandleFlashEvent(NV_LOG_STATUS_t eStatus);

private:
    NV_LOG_EVT_CB_t _cb;
    DFLASH *_pDFlash;
};

#endif
