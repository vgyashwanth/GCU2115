/*
 * J1939DRIVER.h
 *
 *  Created on: 13-Oct-2021
 *      Author: madhuri.abhang
 */

#ifndef MW_J1939_DRIVER_J1939DRIVER_H_
#define MW_J1939_DRIVER_J1939DRIVER_H_

#include "../../../utils/UTILS_Timer.h"
#include "HAL_Manager.h"
class J1939DRIVER
{
public:
    #define BYTE_SIZE               8U
    #define WORD_SIZE               16U
    #define LONG_SIZE               32U

    #define WORD                    1U
    #define LONG                    2U
    typedef union
    {
        double  f64SPNData;
        uint64_t u64SPNData;
        uint32_t u32SPNData;
        uint16_t u16SPNData;
        uint8_t u8SPNData;
    }SPNVal_t;

    typedef struct __attribute__((packed))
    {
        uint16_t u16SPN_Loc;          //Bit number in 8 byte data
        uint8_t ubySPN_DataLength;   //Number of bits in the SPN
        double ufSPN_Resolution;     //Scale factor for SPN
        double ufMinValDataRange;    //Minimum value
        double ufMaxValDataRange;    //Max value
    }J1939_SPN_DB_t;

    typedef struct __attribute__((packed))
    {
        uint16_t ubyReqOrTimeBased;     // period in ms
        uint8_t ubyPDU_Prio;            // Priority
        uint8_t ubyPDU_SA;              // Source Address.
        uint8_t ubyPDU_DA;              // Destination Address
        uint8_t ubyPDU_SPNs;            // Number of SPN's
        uint8_t u8PGNDataLength;        // PGN Data length
        uint8_t u8DP;                   // Data page
        uint8_t u8PF;                   // PDU(Protocol Data unit) format
        uint8_t u8PS;                   // PDU Specific
    }J1939_PGN_DB_t;

    SPNVal_t tSPN;
    static bool bFrameTransmited;
    J1939_PGN_DB_t tSupportPGN;
    J1939_SPN_DB_t tSupportSPN;


    typedef struct
    {
        // For testing the storage bit format, following method is used.
        // It is validated through debugging the code.
        // In ExtractReadFrame in J1939app.c, volatile uPDUIDData uPDU_ID_Data volatile Key assigned to uPDU_ID_Data,
        // so that it will not optimize the uPDU_ID_Data.
        // Break point is placed inside the  if(_hal.Objcan.ReadFrame(&CAN_RxQueueBuffer) ==BSP_SUCCESS).
        // After receiving the request PGN value in uPDU_ID_Data.ulPDUId = 0xbd3a5dee
        // And we get the values in ubyPDU_SA = 0xee, uiPDU_PGN= 0x3a5d, ubPGN_DP=1, ubPGNE_DP =0,
        // ubyPDU_Prio = 7.
        // Results are as expected.

        uint32_t ubyPDU_SA:8, uiPDU_PGN:16, ubPGN_DP:1, ubPGNE_DP:1, ubyPDU_Prio:3, ubyPDU_NU:3;
    }tPDUIDFormat;

    J1939DRIVER(HAL_Manager &hal);
    /**
     * Update function for J1939 Driver
     * @param  None
     * @return None
     */
    void Update(void);

    /**
     * This will generate the 29bit extended CAN ID
     * @param  TxFrame
     * @return None
     */
    void SetFrameID(can_frame_t* TxFrame);

    /**
     *  float J1939CalculateSPN(uint32_t ulData, uint32_t ulResolnSigni, uint8_t ubyResolnBase,
     *  uint32_t ulOffsetSigni, uint8_t ubyOffsetBase, uint8_t ubySign)
     *  This function calculates the value of SPN using its resolution & offset value.]
     *  Call this function after J1939ValidateSPN()
     *  After this function call J1939SPNValueAssign()
     * @param    1. SPN Value
     *           2. Resolution Significant
     *           3. Resolution Base
     *           4. Offset Significant of SPN
     *           5. Offset base
     *           6. Sign of Offset
     * @return  None
     *
     */
    void CalculateSPN(double f64Resolution, double f64Offset, double f64MaxValue, uint8_t u8DataDir);

    /**
     * J1939GenearateSPN-This function used to fill the SPN data
     *  value at the right place of CAN data field]
     * @param   Data array for filling the data.
     * @return  None
     */
    void GenearateSPN(uint8_t au8TransmitBuf[]);

    /**
     * J1939SwapSPNData-This function used to swap the integer value, long value
     * @param       1. u32TempTransmitData - Data to swap
     *              2. u8DataType - Data type for example
     *                  a. WORD - For integer value
     *                  b. LONg - For long value

     * @return  Returns the swapped value
     */
    uint32_t SwapSPNData(uint32_t u32TempTransmitData, uint8_t u8DataType);

private:
    #define CALC_FOR_TRANSMIT       1U
    #define CALC_FOR_RECEIVE        0U
    #define SWAP_HIGHER_AND_LOWER_BYTE_OF_WORD(x)  (((x >> 8) & 0xff) | ((x & 0xff) << 8))
    #define START_FROM_LOC_0        0U

    #define PDU_Prio_Mask_Shift     26U
    #define PDU_PF_Mask_Shift       16U
    #define PDU_PS_Mask_Shift       8U

    #define PDU_PRIO_Mask           0x1C000000u
    #define PDU_PF_Mask             0x00FF0000u
    #define PDU_PS_Mask             0x0000FF00u
    #define PDU_SA_Mask             0x000000FFu


    stTimer _CANTimer;
    HAL_Manager &_ObjHal;
    can_frame_t _CAN_TxFrame;
};

#endif /* MW_J1939_DRIVER_J1939DRIVER_H_ */
