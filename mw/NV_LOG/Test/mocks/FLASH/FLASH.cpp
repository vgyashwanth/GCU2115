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


#include "FLASH.h"

vector<vector<uint8_t>> FLASH::pMemory;

FLASH::FLASH():
bIsInitialized(false)
{
    pMemory.resize(NO_OF_BLOCKS, vector<uint8_t> (BLOCK_SIZE));
    Erase(0, NO_OF_BLOCKS, NULL);
    prvInit();
}


void FLASH::Write(uint32_t u32Addr, const uint8_t *pu8Data, uint32_t u32DataLen,
                                                              FLASH_EVT_CB_t cb)
{
    uint16_t uBlockNo = (uint16_t)(u32Addr / BLOCK_SIZE);
    int16_t uOffset = u32Addr % BLOCK_SIZE;
    for(int i = 0; i < u32DataLen; i++)
    {
        /* Setting the offset + i value to zero if we move to the next block
           after reaching the end of the current block*/
        if(uOffset + i == BLOCK_SIZE)
        {
            uBlockNo++;
            uOffset-=BLOCK_SIZE;
        }
        /*Checking if the current value in that location is the POST_ERASE_VALUE*/
        if(pMemory[uBlockNo][uOffset + i] == POST_ERASE_VALUE)
        {
            pMemory[uBlockNo][uOffset + i] = pu8Data[i];
        }
        else
        {
            /* Setting the value in that location to 0 if there is a clash*/
            pMemory[uBlockNo][uOffset + i] = 0x00;
        }
    }
    Update();
    if(cb != NULL)
    {
        cb(DFLASH_WRITE_SUCCESS);
    }
}

void FLASH::Read(uint32_t u32Addr, uint8_t *pu8Data, uint32_t u32DataLen, FLASH_EVT_CB_t cb)
{
    uint16_t uBlockNo = (uint16_t)(u32Addr / BLOCK_SIZE);
    int16_t uOffset = u32Addr % BLOCK_SIZE;
    for(int i = 0; i < u32DataLen; i++)
    {
        if(uOffset + i == BLOCK_SIZE)
        {
            uBlockNo++;
            uOffset-=BLOCK_SIZE;
        }
        pu8Data[i] = pMemory[uBlockNo][uOffset + i];
    }
    if(cb != NULL)
    {
        cb(DFLASH_READ_SUCCESS);
    }
}

void FLASH::IsLocationBlank(uint32_t u32Addr, uint32_t u32DataLen, FLASH_EVT_CB_t cb)
{
    static uint8_t au8Buffer[256] = {0};
    Read(u32Addr, au8Buffer, u32DataLen, NULL);

    for(int i=0;i<u32DataLen;i++)
    {
        if(au8Buffer[i] != 0xFF)
        {
            cb(DFLASH_LOCATION_NOT_BLANK);
            return;
        }
    }
    cb(DFLASH_LOCATION_BLANK);
}

void FLASH::Erase(uint32_t u32Addr, uint16_t u16NoOfBlocks, FLASH_EVT_CB_t cb)
{
    uint16_t u16BlockNo = uint16_t(u32Addr / BLOCK_SIZE);
    for(int i = u16BlockNo; i < (u16BlockNo + u16NoOfBlocks); i++)
    {
        pMemory[i].assign(BLOCK_SIZE, POST_ERASE_VALUE);
    }
    /*Update the log with the pMemory value only after initialization of the vector
      has completed*/
    if(bIsInitialized)
    {
        Update();
    }
    if(cb != NULL)
    {
        cb(DFLASH_ERASE_SUCCESS);
    }
}

void FLASH::Update()
{
    json j;
    for(int i = 0; i < NO_OF_BLOCKS; i++)
    {
        j[to_string(i)] = pMemory[i];
    }
    string s = j.dump(0);
    fstream fPtr;
    fPtr.open("log.json", ios::out);
    fPtr << s;
    fPtr.close();
}

void FLASH::prvInit()
{
    json j;
    ifstream i("log.json");
    ifstream ifs("log.json", ios::ate);
    if(ifs.tellg() == 0)
    {
        return;
    }
    i >> j;
    for (json::iterator it = j.begin(); it != j.end(); ++it)
    {   
        int key = stoi(it.key());
        if(key < NO_OF_BLOCKS)
        {
            vector<uint8_t> tmp = it.value();
            pMemory[stoi(it.key())] = tmp;
        }
        else
        {
            cout<<"Block number mismatch b/w FLASH and JSON"<<endl;
        }

    }
    bIsInitialized = true;
}

void FLASH::PrintStatus()
{
    for(auto iter = pMemory.begin(); iter < pMemory.end(); iter++)
    {
        for(auto it = (*iter).begin(); it < (*iter).end(); it++)
        {
            cout<<(int)*it<<' ';
        }
        cout<<endl;
    }
    cout<<endl;
}
