/*
* @file        [FLASH]
* @brief       This module is used to emulate a flash using a vector of a vector.
*              The module various function with which the user can interact with
*              the flash.
*              Functions provided by this module to interact with the flash are:
*              1. Read
*              2. Write
*              3. Erase
*              4. Check if a location is blank
*              Initialization:
*              For initialization the json file(log.json) is read and then using
*              an iterator the pMemory(flash) is filled with the contents of the
*              json file.
*              Read:
*              In read, the data of size given by the user is read from the address
*              in the flash passed by the user, this data is returned via a user
*              pointer.
*              Write:
*              In write, the data present in the user pointer is written into the
*              flash in the address given by the user. At the end of this the update
*              function is called and the change is committed to the log.
*              Erase:
*              In erase the block to start erasing from is calculated using the
*              address passed by the user, the blocks starting from this block
*              to the number of block given by the user are erased by setting
*              every entry in this block to the post erase value. After this
*              the update function is called and the change is committed to the log.
* @version     [Version number]
* @author      Naveen Ramaswamy, Sudeep Chandrasekaran
* @date        24th June 2021
* @author      None
* @date        None
* @copyright   SEDEMAC Mechatronics Pvt Ltd
*/


#ifndef _FLASH_H_
#define _FLASH_H_

#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

/*Print a buffer with print formatted as 8 bytes per line*/
#define DEBUG_PRINT_HEX(buff, len) {\
                        uint16_t u16Cnt = 0;\
                          for(; u16Cnt<len; u16Cnt++)\
                          {\
                            if(!(u16Cnt%8))\
                            {\
                              printf("%s","\n");\
                            }\
                            printf("0x%02x ", buff[u16Cnt]);\
                          }\
                          printf("%s","\n");\
                        }



using json = nlohmann::json;
using namespace std;

#define POST_ERASE_VALUE 0xFF
#define BLOCK_SIZE (64U)
#define NO_OF_BLOCKS (16U)


//class BLOCK;

class FLASH
{

    public:
        typedef enum {
            DFLASH_WRITE_SUCCESS,
            DFLASH_WRITE_FAIL,
            DFLASH_ERASE_SUCCESS,
            DFLASH_ERASE_FAIL,
            DFLASH_READ_SUCCESS,
            DFLASH_READ_FAIL,
            DFLASH_LOCATION_BLANK,
            DFLASH_LOCATION_NOT_BLANK
        } FLASH_STATUS_t;

        /* Function Pointer for callback */
        typedef void (*FLASH_EVT_CB_t)(FLASH_STATUS_t evt);

        /**
        * Constructs the class
        * @param - None
        * @return
        * None
        */
        FLASH();

        /**
        * Writes the data to the Flash.
        * @param u32Addr - Offset used to index to the location within the flash to write
        * @param pu8Data - Data to write to flash
        * @param u32DataLen - Length of data to write to flash
        * @param cb - Callback function to be called at the end of write
        * @return
        * None
        */
        void Write(uint32_t u32Addr, const uint8_t *pu8Data, uint32_t u32DataLen,
                                                              FLASH_EVT_CB_t cb);

        /**
        * Reads the data from the Flash.
        * @param u32Addr - Offset used to index to the location within the flash to read
        * @param pu8Data - Pointer of location to read data to from flash
        * @param u32DataLen - Length of data to read from flash
        * @param cb - Callback function to be called at the end of read
        * @return
        * None
        */
        void Read(uint32_t u32Addr, uint8_t *pu8Data, uint32_t u32DataLen, FLASH_EVT_CB_t cb);

        /**
        * Erases a block or a few blocks of data from the flash.
        * @param u32Addr - Offset used to index to the block within the flash to start
        *                   erasing from
        * @param u16NoOfVlocks - Number of blocks to erase within the flash.
        * @param cb - Callback function to be called at the end of erase
        * @return
        * None
        */
        void Erase(uint32_t u32Addr, uint16_t u16NoOfBlocks, FLASH_EVT_CB_t cb);

        /**
        * Checks if the location of the given address in the flash is blank.
        * @param u32Addr - The location within the flash to start checking from
        * @param u32DataLen - Number of bytes to check starting from the address given
        * @return
        * None
        */
        void IsLocationBlank(uint32_t u32Addr, uint32_t u32DataLen, FLASH_EVT_CB_t cb);

        /**
        * Writes the flash in to a json file(log.json)
        * @param - None
        * @return
        * None
        */
        void Update();

        /**
        * Prints the flash to the screen
        * @param eCtPos - None
        * @return
        * None
        */
        void PrintStatus();

    private:
        static FLASH_EVT_CB_t _cb;
        static vector<vector<uint8_t>> pMemory;

        /**
        * Initializes the flash by reading from the json file into the flash.
        * @param eCtPos - None
        * @return
        * None
        */
        void prvInit();

        bool bIsInitialized;
};

#endif

