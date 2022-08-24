/*
 * J1939DRIVER.cpp
 *
 *  Created on: 13-Oct-2021
 *      Author: madhuri.abhang
 */

#include "J1939DRIVER.h"

bool J1939DRIVER::bFrameTransmited= true;
J1939DRIVER::J1939DRIVER(HAL_Manager &hal):
_ObjHal(hal),
_CAN_TxFrame{0}
{

}


void J1939DRIVER::Update(void)
{

}

void J1939DRIVER::CalculateSPN(double f64Resolution, double f64Offset, double f64MaxValue, uint8_t u8DataDir)
{
    if(u8DataDir == CALC_FOR_TRANSMIT)
    {
        /* Added check to resolve 127D LDRA resolution, Denominator checked for not equal to zero*/
        if((0 != f64Resolution) && (tSPN.f64SPNData <= f64MaxValue))
        {
            /* calculation logic has been updated for -ve values of offset */
            tSPN.u64SPNData  =(uint64_t) ( (tSPN.f64SPNData- f64Offset) / f64Resolution );
        }
        else /* If calculated f64Resolution is zero, always consider it to be constant 1*/
        {
            tSPN.u64SPNData = 0xFFFFFFFFFFFFFFFF;
        }
    }
    else if(u8DataDir == CALC_FOR_RECEIVE)
    {
        tSPN.f64SPNData  = (tSPN.u64SPNData * f64Resolution) + f64Offset;
    }
    else
    {
        ;
    }
}


void J1939DRIVER::GenearateSPN(uint8_t au8TransmitBuf[])
{

    uint32_t u32TempTransmitData = 0, u32MaskDword = 0;

    //1. Assumption: according database of J1939 standard, for data of 8bit/16bit/32bit start location % 8 is always zero.
    // Assumption is verified for all PGN's which are used for Genset control application(SAE J1939-75-2015) and which are used in all other GCU's
    //2. Assumption any SPN value will NOT spill beyond byte boundary. i.e. if SPN length is < 8: then the entire SPN value will be accommodated
    //in a single byte of the transmit array.
    u32TempTransmitData =  tSPN.u32SPNData;

    /// calculate the mask for SPN data
    u32MaskDword = (1U << tSupportSPN.ubySPN_DataLength) - 1U;

    /// Mask the value
    u32TempTransmitData = (u32TempTransmitData & u32MaskDword);

    if(tSupportSPN.ubySPN_DataLength <= 8)         // Single byte data
    {
        if((tSupportSPN.u16SPN_Loc % BYTE_SIZE) == START_FROM_LOC_0)
        {
            /// do nothing as the data is of 8 bit and we have already filled array above
            au8TransmitBuf[(tSupportSPN.u16SPN_Loc / BYTE_SIZE)] = (uint8_t)(u32TempTransmitData);
        }
        else
        {
            /// Fill data byte field
            au8TransmitBuf[(tSupportSPN.u16SPN_Loc / BYTE_SIZE)] = (uint8_t)(u32TempTransmitData
                                                            << (tSupportSPN.u16SPN_Loc % BYTE_SIZE));
        }

    }
    else if(tSupportSPN.ubySPN_DataLength <=16)   // For 2 byte data
    {
        u32TempTransmitData = SwapSPNData(u32TempTransmitData, WORD);  //Swap data
        /// load the higher byte
        au8TransmitBuf[(tSupportSPN.u16SPN_Loc / BYTE_SIZE)] = (uint8_t)((u32TempTransmitData & 0xFF00) >> 8);
        /// load the lower byte
        au8TransmitBuf[(tSupportSPN.u16SPN_Loc / BYTE_SIZE) + 1] = (uint8_t)(u32TempTransmitData & 0x00FF);
    }
    else if(tSupportSPN.ubySPN_DataLength <= 32)   // For 4 byte data
    {
        u32TempTransmitData = SwapSPNData(u32TempTransmitData, LONG);  // swap data

        au8TransmitBuf[(tSupportSPN.u16SPN_Loc / BYTE_SIZE)] = (uint8_t)((u32TempTransmitData & 0xFF000000) >> 24);

        au8TransmitBuf[(tSupportSPN.u16SPN_Loc / BYTE_SIZE) + 1] = (uint8_t)((u32TempTransmitData & 0x00FF0000)>> 16);

        au8TransmitBuf[(tSupportSPN.u16SPN_Loc / BYTE_SIZE) + 2] = (uint8_t)((u32TempTransmitData & 0x0000FF00)>> 8);

        au8TransmitBuf[(tSupportSPN.u16SPN_Loc / BYTE_SIZE) + 3] = (uint8_t)(u32TempTransmitData & 0x000000FF);
    }

}

uint32_t J1939DRIVER::SwapSPNData(uint32_t u32TempTransmitData, uint8_t u8DataType)
{
    uint32_t u32SwapSPNData;
    uint16_t u16LowerWordOfLong, u16HigherWordOfLong ;


    if(u8DataType == WORD)
    {
        /* Swap byte of word*/
        u32SwapSPNData = SWAP_HIGHER_AND_LOWER_BYTE_OF_WORD(u32TempTransmitData);

    }
    else if(u8DataType == LONG)
    {
        /* Swap words of long*/
        u16LowerWordOfLong = (uint16_t)(u32TempTransmitData & 0xFFFFu);
        u16HigherWordOfLong = (uint16_t)((uint32_t)(u32TempTransmitData & 0xFFFF0000u) >> 16);
        u16LowerWordOfLong =  SWAP_HIGHER_AND_LOWER_BYTE_OF_WORD(u16LowerWordOfLong);
        u16HigherWordOfLong = SWAP_HIGHER_AND_LOWER_BYTE_OF_WORD(u16HigherWordOfLong);
        u32SwapSPNData = ( ((uint32_t)u16LowerWordOfLong << 16)) | ((uint32_t)u16HigherWordOfLong);
    }
    else
    {
        return(u32TempTransmitData);
    }
    return(u32SwapSPNData);
}

void J1939DRIVER::SetFrameID(can_frame_t* TxFrame)
{
    /// set the CAN frame type
    TxFrame->type= CAN_FRAME_TYPE_DATA;
    /// load the CAN frame data length
    TxFrame->data_length_code = tSupportPGN.u8PGNDataLength ;
    //CAN_Tx_Buffer.LocPriority = 0;

    TxFrame->id_mode = CAN_ID_MODE_EXTENDED;

    /// load the CAN frame identifier
    TxFrame->id = 0;
    TxFrame->id |=
            (uint32_t)((tSupportPGN.ubyPDU_Prio << PDU_Prio_Mask_Shift)& PDU_PRIO_Mask);
    TxFrame->id |= ((((uint32_t)tSupportPGN.u8PF )<< PDU_PF_Mask_Shift)& PDU_PF_Mask);

    TxFrame->id|= ((((uint32_t) tSupportPGN.u8PS) << PDU_PS_Mask_Shift) & PDU_PS_Mask);
    TxFrame->id|= (uint32_t) ((tSupportPGN.ubyPDU_SA & PDU_SA_Mask));
}


