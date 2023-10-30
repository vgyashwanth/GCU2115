/*
 * CFGC.h
 *
 *  Created on: 07-Aug-2021
 *      Author: nikhil.mhaske
 */

#ifndef APL_CFGC_CFGC_H_
#define APL_CFGC_CFGC_H_

#include "stdio.h"
#include "HAL_Manager.h"




class CFGC{
public:
    typedef struct __attribute__((packed))
    {
        uint32_t u32PCBPID;
        uint16_t u16ProductID;
        uint8_t u8UseLanguage;
        uint8_t u8UseExtendedEventLog;
        uint8_t u8PIN1PasswordChar0;
        uint8_t u8PIN1PasswordChar1;
        uint8_t u8PIN1PasswordChar2;
        uint8_t u8PIN1PasswordChar3;
        uint8_t u8PIN2PasswordChar0;
        uint8_t u8PIN2PasswordChar1;
        uint8_t u8PIN2PasswordChar2;
        uint8_t u8PIN2PasswordChar3;
        uint8_t u8CFGZxxxRevision;
        uint8_t u8CFGCxxxRevision;
        uint8_t u8UseMultipleProfiles;
        uint8_t u8PIN3PasswordChar0;
        uint8_t u8PIN3PasswordChar1;
        uint8_t u8PIN3PasswordChar2;
        uint8_t u8PIN3PasswordChar3;
        uint8_t u8ParameterSetRevision;
        uint8_t u8ProductRevision;
        uint8_t u8PCBPIDRevision;
        uint8_t u8ProductVersion;
        uint8_t u8ProductSuffix;
        uint8_t u8ReservedParameter2;
        uint8_t u8EngineSrNoChar01;
        uint8_t u8EngineSrNoChar02;
        uint8_t u8EngineSrNoChar03;
        uint8_t u8EngineSrNoChar04;
        uint8_t u8EngineSrNoChar05;
        uint8_t u8EngineSrNoChar06;
        uint8_t u8EngineSrNoChar07;
        uint8_t u8EngineSrNoChar08;
        uint8_t u8EngineSrNoChar09;
        uint8_t u8EngineSrNoChar10;
        uint8_t u8EngineSrNoChar11;
        uint8_t u8EngineSrNoChar12;
        uint8_t u8EngineSrNoChar13;
        uint8_t u8EngineSrNoChar14;
        uint8_t u8EngineSrNoChar15;
        uint8_t u8EngineSrNoChar16;
        uint8_t u8EngineSrNoChar17;
        uint8_t u8EngineSrNoChar18;
        uint8_t u8EngineSrNoChar19;
        uint8_t u8EngineSrNoChar20;
        uint8_t u8EngineSrNoChar21;
        uint8_t u8EngineSrNoChar22;
        uint8_t u8EngineSrNoChar23;
        uint8_t u8EngineSrNoChar24;
        uint8_t u8EngineSrNoChar25;
        uint8_t u8CustomerCode;
        char strSeriesProductID[3];
    } CFGC_t;

    CFGC(HAL_Manager &Hal);

    void Update();

    static uint16_t GetProductID();

    uint32_t GetPCBPID();

    uint8_t GetUseExtendedEventLog();

    uint8_t GetUseMultipleProfiles();

    uint8_t GetProductRevision();

    uint8_t GetProductVersion();

    uint16_t GetPasswords(uint8_t u8PasswordNo);

    uint8_t GetProductSuffix();

    uint8_t GetCustomerCode();

    void GetEngineSerialNo(uint8_t au8EngSerNo[]);

    void GetSeriesProductID(char acGCUSeries[]);

    static uint16_t GetMaxNumberOfEvents();

    bool IsC02Error();

private:
    #define MAX_EVENT_100           100
    #define MAX_EVENT_30            30
    #define TOTAL_CHAR_ENG_SR_NO    25

    HAL_Manager &_Hal;
    static CFGC_t      _stCfgc;
    bool        _bIntegrityCheckFailed;


    /**
    * This function is used to check the CRC of all CFGC area.
    * @return
    * BSP_SUCCESS  - if the CRC matches.
    * BSP_FAIL     - if the CRC not matches.
    */
    BSP_STATUS_t prvGetCFGCIntegrityCheck();
    /*
     * The following structure is "CFGC metadata".
     * This will be stored as the last few bytes in the CFGC area.
     */
    typedef struct
    {
        uint16_t u16CFGCCrc;  // CFGC CRC
        uint16_t u16Reserved1;  // should be 0xffff for now
        uint32_t u32HighestAddrUsedByCFGC;
        RTC::TIME_t timeOfFlashing;
    } CFGC_METADATA_t;

};
#endif /* APL_CFGC_CFGC_H_ */
