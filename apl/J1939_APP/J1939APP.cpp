/**
 * @file        [J1939APP]
 * @brief       J1939APP
 *              Provides common API's for J1939 PGNS date filling,
 *              data extraction, data transmission and reception. *
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        13-Oct-2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt. Ltd.
 **/


#include "J1939APP.h"
#include "START_STOP.h"

//static const float F32_Null = (float)(0xFFFFFFFF);
bool J1939APP::bTXBufferisEmpty        = true;
bool J1939APP::bAvrFaultShutDn         = false;
bool J1939APP::bTransferSWFaultShutDn  = false;
bool J1939APP::bAvrVoltNotDetect       = false;
bool J1939APP::bAvrFreqNotDetect       = false;
bool J1939APP::bAvrFaultWarning        = false;
bool J1939APP::bTransferSWFaultWarning = false;
bool J1939APP::bCurrentTransNotDetect  = false;
bool J1939APP::bCANCommShutDn          = false;
bool J1939APP::bCANCommWarning         = false;
bool J1939APP::bTransferSWFault        = false;
bool J1939APP::bAvrFault               = false;
MB_APP::KEY_MB_CAN_EVENT_t J1939APP::stCANEvent={0};

J1939APP::J1939APP(HAL_Manager &hal, CFGC &cfgc, CFGZ &cfgz, ENGINE_MONITORING &engineMonitoring,
         GCU_ALARMS &gcuAlarm ,MB_APP &mbApp, AUTO_MODE  &Automode):
J1939DRIVER(hal),
_hal(hal),
_ObjCfgc(cfgc),
_cfgz(cfgz),
_ObjmbApp(mbApp),
_engMon(engineMonitoring),
_Automode(Automode),
_gcuAlarm(gcuAlarm),
ubypReadTxPgns
{
        (J1939_PGNs)&(gstPGNs.PGNGC1_64915),        //0
        (J1939_PGNs)&(gstPGNs.PGNPROPB80_65408),    //1
        (J1939_PGNs)&(gstPGNs.PGNPROPB46_65350),    //2
        (J1939_PGNs)&(gstPGNs.PGNTSC1_0),           //3
        (J1939_PGNs)&(gstPGNs.PGNPROPB55_65365),    //4
        (J1939_PGNs)&(gstPGNs.PGNPROPB01_65281),    //5
        (J1939_PGNs)&(gstPGNs.PGNPROPB16_65302),    //6
        (J1939_PGNs)&(gstPGNs.PGNPROPB00_65280),    //7
        (J1939_PGNs)&(gstPGNs.PGNPROPBF7_65527),    //8
        (J1939_PGNs)&(gstPGNs.PGNPROPB69_65385),    //9
        (J1939_PGNs)&(gstPGNs.PGNPROPB73_65395),    //9
        (J1939_PGNs)&(gstPGNs.PGNPROPB7E_65406),    //9
        (J1939_PGNs)&(gstPGNs.PGNCM1_57344),        //10
        (J1939_PGNs)&(gstPGNs.PGNETC5_65219),       //11
        (J1939_PGNs)&(gstPGNs.PGNPROPB53_65363),   //12
        (J1939_PGNs)&(gstPGNs.PGNCCVS_65265),      //13
        (J1939_PGNs)&(gstPGNs.PGNPROPB08_65288),   //14
        (J1939_PGNs)&(gstPGNs.PGNETC3_65223),      //15
        (J1939_PGNs)&(gstPGNs.PGNEBC1_61441),      //16
        (J1939_PGNs)&(gstPGNs.PGNRQST_59904),      //17
},
ubypReadRxPgns{

    (J1939_PGNs)&(gstPGNs.PGN_PROPB62_65378    ),
    (J1939_PGNs)&(gstPGNs.PGN_PROPB57_65367    ),
    (J1939_PGNs)&(gstPGNs.PGN_EMR_PREHEAT_65284),
    (J1939_PGNs)&(gstPGNs.PGN_IVECO_ENG_STATUS_65282),
    (J1939_PGNs)&(gstPGNs.PGN_LFE_65266       ),
    (J1939_PGNs)&(gstPGNs.PGN_EEC2_61443      ),
    (J1939_PGNs)&(gstPGNs.PGN_AMB_65269       ),
    (J1939_PGNs)&(gstPGNs.PGN_HOURS_65253     ),
    (J1939_PGNs)&(gstPGNs.PGN_VEP1_65271      ),
    (J1939_PGNs)&(gstPGNs.PGN_DD_65276        ),
    (J1939_PGNs)&(gstPGNs.PGN_WFI_65279       ),
    (J1939_PGNs)&(gstPGNs.PGN_LFC_65257       ),
    (J1939_PGNs)&(gstPGNs.PGN_EEC4_65214      ),
    (J1939_PGNs)&(gstPGNs.PGN_LFI_65203       ),
    (J1939_PGNs)&(gstPGNs.PGN_IC2_64976       ),
    (J1939_PGNs)&(gstPGNs.PGN_EOI_64914       ),
    (J1939_PGNs)&(gstPGNs.PGN_ET2_65188       ),
    (J1939_PGNs)&(gstPGNs.PGN_EEC3_65247      ),
    (J1939_PGNs)&(gstPGNs.PGN_S2_65166        ),
    (J1939_PGNs)&(gstPGNs.PGN_EFL_P2_65243    ),
    (J1939_PGNs)&(gstPGNs.PGN_SHUTDOWN_65252  ),
    (J1939_PGNs)&(gstPGNs.PGN_EFG1_61450      ),
    (J1939_PGNs)&(gstPGNs.PGN_DPF1S_64796     ),
    (J1939_PGNs)&(gstPGNs.PGN_ET1_65262       ),
    (J1939_PGNs)&(gstPGNs.PGN_EEC1_61444      ),
    (J1939_PGNs)&(gstPGNs.PGN_EFL_P1_65263    ),
    (J1939_PGNs)&(gstPGNs.PGN_IC1_65270       ),
    (J1939_PGNs)&(gstPGNs.PGN_AT1IG1_61454    ),
    (J1939_PGNs)&(gstPGNs.PGN_A1DOC_64800     ),
    (J1939_PGNs)&(gstPGNs.PGN_AT1IG2_64948    ),
    (J1939_PGNs)&(gstPGNs.PGN_AT1IMG_64946    ),
    (J1939_PGNs)&(gstPGNs.PGN_AT1OG1_61455    ),
    (J1939_PGNs)&(gstPGNs.PGN_A1SCRDSI1_61475 ),
    (J1939_PGNs)&(gstPGNs.PGN_A1SCRDSI2_64833 ),
    (J1939_PGNs)&(gstPGNs.PGN_A1SCRDSR1_61476 ),
    (J1939_PGNs)&(gstPGNs.PGN_A1SCREGT1_64830 ),
    (J1939_PGNs)&(gstPGNs.PGN_AT2IG1_61456    ),
    (J1939_PGNs)&(gstPGNs.PGN_AT2OG1_61457    ),
    (J1939_PGNs)&(gstPGNs.PGN_A2SCRDSI2_64827 ),
    (J1939_PGNs)&(gstPGNs.PGN_A2SCRDSI1_61478 ),
    (J1939_PGNs)&(gstPGNs.PGN_A2SCRDSR1_61479 ),
    (J1939_PGNs)&(gstPGNs.PGN_A2SCREGT1_64824 ),
    (J1939_PGNs)&(gstPGNs.PGN_AT1OG2_64947    ),
    (J1939_PGNs)&(gstPGNs.PGN_AT1S2_64697     ),
    (J1939_PGNs)&(gstPGNs.PGN_AT1S_64891      ),
    (J1939_PGNs)&(gstPGNs.PGN_AT1T1I_65110    ),
    (J1939_PGNs)&(gstPGNs.PGN_PROPB_32_65330  ),
    (J1939_PGNs)&(gstPGNs.PGN_PROPB5E_65374   ),
    (J1939_PGNs)&(gstPGNs.PGN_DPFC1_64892     ),
    (J1939_PGNs)&(gstPGNs.PGN_PROPB51_65361   ),
    (J1939_PGNs)&(gstPGNs.PGNPROPB01_65281    ),
    (J1939_PGNs)&(gstPGNs.PGN_VOLVO_PREHEAT_65351),
    (J1939_PGNs)&(gstPGNs.PGN_PROSTOUT_65364),
    (J1939_PGNs)&(gstPGNs.PGN_HATZ_PROPB_PHYS_65280   ),
    (J1939_PGNs)&(gstPGNs.PGN_AI_65237   ),
    (J1939_PGNs)&(gstPGNs.PGN_ETC5_65219   ),
    (J1939_PGNs)&(gstPGNs.PGN_OII_64554   ),
    (J1939_PGNs)&(gstPGNs.PGN_IT1_65154       ),
    (J1939_PGNs)&(gstPGNs.PGN_GFP_65163       ),
    (J1939_PGNs)&(gstPGNs.PGN_IMI1_65190       ),
    (J1939_PGNs)&(gstPGNs.PGN_FD1_65213     ),
    (J1939_PGNs)&(gstPGNs.PGN_DLCC1_64775     ),
    (J1939_PGNs)&(gstPGNs.PGN_GFC_65199       ),
    (J1939_PGNs)&(gstPGNs.PGN_EPT1_65187      ),
    (J1939_PGNs)&(gstPGNs.PGN_EPT2_65186      ),
    (J1939_PGNs)&(gstPGNs.PGN_EPT3_65185      ),
    (J1939_PGNs)&(gstPGNs.PGN_EPT4_65184      ),
    (J1939_PGNs)&(gstPGNs.PGN_EPT5_65183      ),
    (J1939_PGNs)&(gstPGNs.PGN_ET4_64870       ),
    (J1939_PGNs)&(gstPGNs.PGN_TCI4_65176      ),
    (J1939_PGNs)&(gstPGNs.PGN_EFL_P12_64735   ),
    (J1939_PGNs)&(gstPGNs.PGN_CCVS_HATZ_65265   ),
    (J1939_PGNs)&(gstPGNs.PGN_DM11_65235      )

},
    _f32Pgn64915Data{0},
    _f32Pgn65408Data{0},
    _f32Pgn65350Data{0},
    _f32Pgn0Data{0},
    _f32Pgn65365Data{0},
    _f32Pgn65281Data{0},
    _f32Pgn65302Data{0},
    _f32Pgn65280Data{0},
    _f32Pgn65527Data{0},
    _f32Pgn65385Data{0},
    _f32Pgn65395Data{0},
    _f32Pgn65406Data{0},
    _f32Pgn57344Data{0},
    _f32Pgn65219Data{0},
    _f32Pgn65363Data{0},
    _f32Pgn65265Data{0},
    _f32Pgn65288Data{0},
    _f32Pgn65223Data{0},
    _f32Pgn61441Data{0},
    _au8CalcSPNData{0},
    _astPGNTxDataBuff{0},
    _u32RequestedPGN(0),
    _u8PGN_Num(0),
    _u8NumOfDM1SPN(0),
    _u8NumOfDM2SPN(0),
    _u8J1939PIDTune_Normal(0),
    _ConfigParam{0},
    _bDeviceInConfigMode(false),
    _u16FW_CRC(0),
    _Timer100ms{0},
    _Timer200ms{0},
    _Timer500ms{0},
    _Timer1s{0},
    _Timer250ms{0},
    _Timer50ms{0},
    _Timer80ms{0},
    _Timer20ms{0},
    _Timer10ms{0},
    _Timer40ms{0},
    _CommFailTimeout{0},
    _u8ArrDM1SPNData{0},
    _u8ArrDM2SPNData{0},
    _u8ArrPropB5EPGNData{0},
    _u8NumOf500msPGN(0),
    _u8NoOfInvalidSpnInDm1Msg(0),
    _u8NoOfInvalidSpnInDm2Msg(0),
    _u32TSC1CANMessageID(0),
    _u32PropBF7CANMessageID(0),
    _u8PGN0xFFF7MessageCounter(0),
    _u8PGN0MessageCounter(0),
    _u8ActiveDtcAmberLampStatus(0),
    _u8ActiveDtcProtectLampStatus(0),
    _u8ActiveDtcRedLampStatus(0),
    _u8ActiveDtcMILLampStatus (0),
    _bIsDM1PGNRecieved(false),
    _bIsCANJ1939CommFail(false),
    _bClearActiveDTCs(false),
    _bRequestDM2PGN(false),
    _bRequestDM11PGN(false),
    _ArrPgnReadData{0},
    _bEngineStartPreheatStatus(false),
    _bPreheatFailed(false),
    _u8AccPedalCounter(0),
    _stDm1Decode{0,0,0},
    _stDm2Decode{0,0,0}
{

    UTILS_ResetTimer(&_Timer100ms);
    UTILS_ResetTimer(&_Timer250ms);
    UTILS_ResetTimer(&_Timer500ms);
    UTILS_ResetTimer(&_Timer1s);
    UTILS_ResetTimer(&_Timer20ms);
    UTILS_ResetTimer(&_Timer10ms);
    UTILS_ResetTimer(&_Timer50ms);
    UTILS_ResetTimer(&_Timer80ms);
    UTILS_ResetTimer(&_Timer40ms);
    UTILS_ResetTimer(&_Timer200ms);
    UTILS_ResetTimer(&_CommFailTimeout);
    InitAfterConfigChange();

}

void J1939APP::InitAfterConfigChange()
{
    ResetLampsStatus();
    uint8_t gu8PGNNumber, gu8SPNNumber;
    for( gu8PGNNumber = 0; gu8PGNNumber < RX_PGN_LAST; gu8PGNNumber++)
    {
        for(gu8SPNNumber = 0; gu8SPNNumber < MAX_NO_SPN_IN_PGN; gu8SPNNumber++)
        {
            _ArrSpnErrorNAStatus[gu8PGNNumber][gu8SPNNumber] = NOT_AVAILABLE;
            (void)memset((void *)(&_ArrPgnReadData[gu8PGNNumber][gu8SPNNumber]), 0xFFFFFFFF, sizeof(double));
        }
    }
}

void J1939APP::GenerateFrame(uint8_t u8PGNNum)
{
    uint8_t u8Index = 0;
    uint8_t au8CalSPNData[8] = {0};
    /// get the PGN information
    GetPGN(u8PGNNum, CALC_FOR_TRANSMIT, &tSupportPGN);

    if(u8PGNNum ==  PGN_TX_RQST)
    {
        // Do not perform any action as au8CalSPNData[] buffer is already having bytes to send
        if(_bRequestDM2PGN)
        {
            UpdateDM2RequestData(au8CalSPNData);
            _bRequestDM2PGN = false;
        }
        else if(_bRequestDM11PGN)
        {
            UpdateDM11RequestData(au8CalSPNData);

            _bRequestDM11PGN = false;
        }
        else
        {
            prvUpdatePGN59904Data(au8CalSPNData);
        }
    }
    else
    {

        (((J1939APP*)this)->*J1939APP::afpUpdatePGNData[(u8PGNNum)])();

        /// calculate the SPN's data which is pointed by pu8CalcSPNData pointer
        LoadData(u8PGNNum, au8CalSPNData);
    }


    _astPGNTxDataBuff[u8PGNNum].u8LockDataBuff = 1;
    for(u8Index = 0; u8Index < 8; u8Index++)
    {
        /// load the buffer with SPN values
        _astPGNTxDataBuff[u8PGNNum].au8TxDataBuffUpdtd[u8Index] = au8CalSPNData[u8Index];
        //u8TransmitBuf[u8Index] = au8CalSPNData[u8Index];
    }
    _astPGNTxDataBuff[u8PGNNum].u8LockDataBuff = 0;

    //Copy updated buffer in old buff in case of Buffer is locked then this buffer will be used for tx
    for(u8Index = 0; u8Index < 8; u8Index++)
    {
        _astPGNTxDataBuff[u8PGNNum].au8TxDataBuffOld[u8Index] = _astPGNTxDataBuff[u8PGNNum].au8TxDataBuffUpdtd[u8Index];
    }

    /* DM1 messages should get cleared upon timeout of 2.5 seconds
    * This part of code is added here considering DM packet is received by GCU and
    * later no any frame is received then even it should get clear.
    */
   if(_bClearActiveDTCs)
   {
       _bClearActiveDTCs = 0;
       SetDm1MsgCount(0);
       (void)memset((void *)&_stDm1Decode, 0x00, sizeof(_stDm1Decode));
   }


}



void J1939APP::GetPGN(uint8_t ubyPGN, uint8_t u8RxOrTx, J1939_PGN_DB_t *pstGetPGN )
{
    if(u8RxOrTx == CALC_FOR_TRANSMIT)
    {
        (void)memcpy((void *)(pstGetPGN), (void *)(ubypReadTxPgns[ubyPGN]),10);
    }
    else
    {
        (void)memcpy((void *)(pstGetPGN), (void *)(ubypReadRxPgns[ubyPGN]),10);
    }

    /* While transmitting PGNs - Source address should refer from configuration.
    */
        if(u8RxOrTx == CALC_FOR_TRANSMIT)
        {
           // pstGetPGN->ubyPDU_SA = _cfgz.GetCFGZ_Param(CFGZ::ID_SGC_SOURCE_ADDRESS);
            pstGetPGN->ubyPDU_SA = 0x3;
           /* For HATZ Engine, the Source Address should for TSC1 should be "3" always */
           if(CFGZ::ENG_HATZ == _cfgz.GetEngType())
            {
                pstGetPGN->ubyPDU_SA = 0x3;
            }


           if(((CFGZ::ENG_YUCHAI_YCGCU == _cfgz.GetEngType())||(CFGZ::ENG_HATZ == _cfgz.GetEngType())) && (PGN_TSC1 == ubyPGN))
             {
                 pstGetPGN->ubyPDU_SA = 0x3;
             }
             else if((CFGZ::ENG_IVECO == _cfgz.GetEngType()) && (PGN_PROPB_00 == ubyPGN))
             {
                 pstGetPGN->ubyPDU_SA = 0x27;
             }
             else if(CFGZ::ENG_PERKINS_ADAM4 == _cfgz.GetEngType() && ((PGN_GC1 == ubyPGN) || (PGN_ETC3 == ubyPGN)))
             {
                 pstGetPGN->ubyPDU_SA = 0xEA;
             }
             else if((CFGZ::ENG_YUCHAI_BOSCH == _cfgz.GetEngType())&&(PGN_EBC1 == ubyPGN))
             {
                 pstGetPGN->ubyPDU_SA = 0x0B;
             }
             else if((CFGZ::ENG_YUCHAI_BOSCH == _cfgz.GetEngType())&& (PGN_TSC1 == ubyPGN))
             {
                 pstGetPGN->ubyPDU_SA = 0x03;
             }
             else if((CFGZ::ENG_YUCHAI_BOSCH == _cfgz.GetEngType())&& (PGN_TX_RQST == ubyPGN))
             {
                 pstGetPGN->ubyPDU_SA = 0x21;
             }
             else if(((CFGZ::ENG_YUCHAI_YCGCU == _cfgz.GetEngType())||(CFGZ::ENG_HATZ == _cfgz.GetEngType()))&& ((PGN_CM1 == ubyPGN)||(PGN_GC1 == ubyPGN)))
             {
                 pstGetPGN->ubyPDU_SA = 0x03;
             }

             //As per DEIF's requirement , changed the Priority of CM1 PGN to 3 for Yuchai YCGCU , Hatz & Weichai Engine Type
             if(((CFGZ::ENG_YUCHAI_YCGCU == _cfgz.GetEngType())
                     ||(CFGZ::ENG_HATZ == _cfgz.GetEngType())
                     ||(CFGZ::ENG_WECHAI == _cfgz.GetEngType())
                     ||(CFGZ::ENG_PERKINS_ADAM4 == _cfgz.GetEngType()))
                     &&(PGN_CM1 == ubyPGN))
             {
                 pstGetPGN->ubyPDU_Prio = 0x03;
             }

        }

}

void J1939APP::GetSPN(uint8_t ubyPGN, uint8_t ubySPN, uint8_t u8TxOrRX,
                      J1939_SPN_DB_t * pstGetSpn)
{
    /// load the SPN database into structure(SPN database is read from EEPROM and saved in array)
    if(u8TxOrRX == CALC_FOR_TRANSMIT)
    {
        (void)memcpy((void *)(pstGetSpn), (void *)(ubypReadTxPgns[ubyPGN] + 10 +
                (ubySPN * sizeof(J1939_SPN_DB_t))),sizeof(J1939_SPN_DB_t));
    }
    else
    {
        (void)memcpy((void *)(pstGetSpn), (void *)(ubypReadRxPgns[ubyPGN] + 10 +
                (ubySPN * sizeof(J1939_SPN_DB_t))),sizeof(J1939_SPN_DB_t));
    }

}


void J1939APP::LoadData(uint8_t u8PGNNum, uint8_t au8SPNDataBuf[8])
{
    uint8_t No_Of_SPNs = tSupportPGN.ubyPDU_SPNs;

    /// Run the for loop for number of SPN's in single PGN
    for(uint8_t u8SPNNum = 0; u8SPNNum < No_Of_SPNs; u8SPNNum++)
    {
        /// get the SPN database
        GetSPN(u8PGNNum, u8SPNNum, CALC_FOR_TRANSMIT, &tSupportSPN);
        tSPN.f64SPNData = (PGN_DataFrames[u8PGNNum][u8SPNNum]);

        /*If SPN is in No. of bits i.e length is less than a byte then we need to handle it differently
         * as functions which calculates SPN takes data in DOUBLE data type but our data is in FLOAT type
         * so while passing FLOAT data type to the function which expects DOUBLE it is converting
         * FLOAT(single precision) to DOUBLE(Double precision) effectively changing the binary representation,
         * as our SPN is boolean this change is causing data loss so here handling for both type of SPN is done
         * differently  */

        /// Calculate the SPN data to send
         CalculateSPN(tSupportSPN.ufSPN_Resolution,
                        tSupportSPN.ufMinValDataRange, tSupportSPN.ufMaxValDataRange, CALC_FOR_TRANSMIT);

        /// fill the calculated data to appropriate locations of SPN in the data field
        GenearateSPN(_au8CalcSPNData);


        /// load the transmit buffer
        for(uint8_t u8BufIndex=0; u8BufIndex<8; u8BufIndex++)
        {
            au8SPNDataBuf[u8BufIndex] |= _au8CalcSPNData[u8BufIndex];
        }

        //Clearing array for next calculation iteration
        for(uint8_t u8BufIndex=0;u8BufIndex<8;u8BufIndex++)
        {
            _au8CalcSPNData[u8BufIndex] = 0;
        }
    }
}



void J1939APP::prvUpdatePGN64915Data(void)
{
    memset((void*)&_f32Pgn64915Data, 0x00, sizeof(_f32Pgn64915Data));

    /* Nikhil - 24-Mar-2020
    * Below are states for SPN3542
    * Bit state 0000 = Normal Engine Shutdown
    * Bit state 0001 = Rapid Engine Shutdown
    * Bit state 0010 = Emergency Engine Shutdown
    * Bit state 0011 = Normal Engine Start
    * Bit state 0100 = Rapid Engine Start
    * Bit states 0101-1101 = available for SAE assignment
    * Bit state 1110 = Reserved
    * Bit state 1111 = Don't Care / Take No Action
    */

    if(IS_START_STOP_RELAY_J1939_CONFIGURED())
    {
      if(START_STOP::IsStartRelayON())
      {
          /* Normal Engine Start*/
          _f32Pgn64915Data[PGN_64915_REQ_ENG_CONTROL_MODE] = NORMAL_ENGINE_START;
      }
      else if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_EMERGENCY_STOP)== DigitalSensor::SENSOR_LATCHED)
      {
          /* Emergency Engine Shutdown*/
          _f32Pgn64915Data[PGN_64915_REQ_ENG_CONTROL_MODE] = EMERGENCY_ENGINE_SHUTDOWN;
      }
      else if(START_STOP::IsStopRelayON())
      {
          /* Normal Engine Shutdown*/
          _f32Pgn64915Data[PGN_64915_REQ_ENG_CONTROL_MODE] = NORMAOL_ENGINE_SHUTDOWN;
      }
      else
      {
          /* Take No action */
          _f32Pgn64915Data[PGN_64915_REQ_ENG_CONTROL_MODE] = NO_ACTION_DONT_CARE;
      }
    }
    else
    {
      /* Take No action */
        _f32Pgn64915Data[PGN_64915_REQ_ENG_CONTROL_MODE] = NO_ACTION_DONT_CARE;
    }

    /* Amruta - 20-Jan-2020
    * Below are states for SPN3567
    * 00 = inactive (ready to start automatically)
    * 01 = active (not ready to start automatically)
    * 10 = error
    * 11 = not available
    */
    if((!_gcuAlarm.IsCommonAlarm())
          && (!_engMon.IsEngineOn())
          && ((_Automode.GetGCUOperatingMode() == BASE_MODES::AUTO_MODE))
      )
    {
        _f32Pgn64915Data[PGN_64915_AUTO_MODE]= 0;
    }
    else
    {
        _f32Pgn64915Data[PGN_64915_AUTO_MODE]= 1;
    }

    _f32Pgn64915Data[PGN_64915_PARALLEL_MODE] = 3;


    /* Amruta - 20-Jan-2020
    * Below are states for SPN3568
    * 00 = inactive (ready to parallel)
    * 01 = active (not ready to parallel)
    * 10 = error
    * 11 = not available
    */
    /* This SPN is not transmitted in SGC1XX products as there is no paralleling mode */
    _f32Pgn64915Data[PGN_64915_PARALLEL_MODE] = 0x3;
    _f32Pgn64915Data[PGN_64915_UNUSED_SPN_1] = 0xFFFF;
    _f32Pgn64915Data[PGN_64915_UNUSED_SPN_2] = 0xFFFF;
    _f32Pgn64915Data[PGN_64915_UNUSED_SPN_3] = 0xFFFF;
    _f32Pgn64915Data[PGN_64915_UNUSED_SPN_4] = 0xFF;
}
void J1939APP::prvUpdatePGN65408Data(void)
{
    //  f32PGN_65408Data
    //As per SRS first byte should be 0xF1 so following data is extracted from it.
    memset((void*)&_f32Pgn65408Data, 0x00, sizeof(_f32Pgn65408Data));
    _f32Pgn65408Data[PGN_65408_HARD_CODE_VAL] = 0xA0;
    if(IS_START_STOP_RELAY_J1939_CONFIGURED())
    {
        _f32Pgn65408Data[PGN_65408_ENG_START_BYTE] = START_STOP::IsStartRelayON();
        _f32Pgn65408Data[PGN_65408_ENG_STOP_BYTE] = START_STOP::IsStopRelayON();   //28-Apr-20:code review comment
    }
    _f32Pgn65408Data[PGN_65408_UNUSED_SPN_1] = 0xFF;
    _f32Pgn65408Data[PGN_65408_UNUSED_SPN_2] = 0xF;
    _f32Pgn65408Data[PGN_65408_UNUSED_SPN_3] = 3;
    _f32Pgn65408Data[PGN_65408_UNUSED_SPN_4] = 63;
    _f32Pgn65408Data[PGN_65408_UNUSED_SPN_5] = 0xFFFF;
    _f32Pgn65408Data[PGN_65408_UNUSED_SPN_6] = 0xFFFF;
}
void J1939APP::prvUpdatePGN65350Data(void)
{
    memset((void*)&_f32Pgn65350Data, 0x00, sizeof(_f32Pgn65350Data));

    if(IS_PREHEAT_J1939_CONFIGURED())
    {
        _f32Pgn65350Data[PGN_65350_PREHEAT_BYTE] = ((START_STOP::GetStartStopSMDState() == START_STOP::ID_STATE_SS_START_WAIT)||(START_STOP::GetStartStopSMDState() == START_STOP::ID_STATE_SS_PREHEAT));
    }
    if(IS_START_STOP_RELAY_J1939_CONFIGURED())
    {
        _f32Pgn65350Data[PGN_65350_ENG_START_BYTE] = START_STOP::IsStartRelayON();
        _f32Pgn65350Data[PGN_65350_FUEL_DISABLE_BYTE] = !START_STOP::IsFuelRelayOn();
        _f32Pgn65350Data[PGN_65350_ENG_STOP_BYTE] = START_STOP::IsStopRelayON();         // 28-Apr-20: code review comment
    }

    _f32Pgn65350Data[PGN_65350_UNUSED_SPN_1] = 0xF;
    _f32Pgn65350Data[PGN_65350_UNUSED_SPN_2] = 0xF;
    _f32Pgn65350Data[PGN_65350_UNUSED_SPN_3] = 3;
    _f32Pgn65350Data[PGN_65350_ACCELERATOR_PEDAL_POS] = 50; //Pedal Position
    _f32Pgn65350Data[PGN_65350_UNUSED_SPN_5] = 127;
    _f32Pgn65350Data[PGN_65350_ACCELERATOR_PEDAL_COUNTER] = _u8AccPedalCounter;
     /*Accelerator Pedal Position Checksum Calculation*/
     prvCalculateAccPedalPosChecksum();
    _f32Pgn65350Data[PGN_65350_UNUSED_SPN_6] = 0xFF;
    _f32Pgn65350Data[PGN_65350_UNUSED_SPN_7] = 0xFF;
}
void J1939APP::prvUpdatePGN0Data(void)
{
    /* As per SRS first byte should be 0xF1 so F1 = 0b­11110001.
         * Hence PGN_0_ENG_OVERRIDE_CONTROL_MODE = 01
         * PGN_0_ENG_REQ_SPEED_CONTROL = 00
         * PGN_0_OVERRIDE_CONTROL_MODE_PRIORITY = 11
         * PGN_0_HARDCODE_VALUE = 11                              */

        _f32Pgn0Data[PGN_0_ENG_OVERRIDE_CONTROL_MODE] = 1;
        _f32Pgn0Data[PGN_0_ENG_REQ_SPEED_CONTROL] = 0;
        _f32Pgn0Data[PGN_0_OVERRIDE_CONTROL_MODE_PRIORITY] = 3;
        _f32Pgn0Data[PGN_0_HARDCODE_VALUE] = 3;
//        if(_cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_TO_ECU))
//        {
//            if(START_STOP::IsIdleModeActive())
//            {
//                _f32Pgn0Data[PGN_0_ENG_REQ_SPEED] =_cfgz.GetCFGZ_Param(CFGZ::ID_INITIAL_LOW_SPEED);
//            }
//            else
//            {
//                _f32Pgn0Data[PGN_0_ENG_REQ_SPEED] =_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_REQUESTED_SPEED_TO_ECU);
//            }
//        }
//        else
        {
            _f32Pgn0Data[PGN_0_ENG_REQ_SPEED] = 0xFFFF;
        }

        //Not transmitting this SPN so send max value 0xFF.
        _f32Pgn0Data[PGN_0_ENG_TORQUE] = 0xFF;
        /* Transmission rate as per SRS is 50 ms.
         *  STATE                       TRANSMISSION RATE
         *    0                             1000ms
         *    1                             750ms
         *    2                             500ms
         *    3                             250ms
         *    4                             100ms
         *    5                             50ms
         *    6                             20ms
         *    7                             Use Standard    */
        _f32Pgn0Data[PGN_0_TRANSMISSION_RATE] = 7;

        // Not transmitting this SPN so send max value 0x1F.
        _f32Pgn0Data[PGN_0_CONTROL_PURPOSE] = 0x1F;

        // Not transmitting this SPN so send 0xFFFF.
        _f32Pgn0Data[PGN_0_ENG_REQ_TORQUE_FRACTION] = 0xFFFF;
        _f32Pgn0Data[PGN_0_MESSAGE_COUNTER] = _u8PGN0MessageCounter;

        /* Check sum calculation */
        CalculateTsc1Checksum();
}
void J1939APP::prvUpdatePGN65365Data(void)
{
    if(IS_START_STOP_RELAY_J1939_CONFIGURED())
    {
        _f32Pgn65365Data[PGN_65365_ENG_START_BYTE] =START_STOP::IsStartRelayON();
        _f32Pgn65365Data[PGN_65365_ENG_STOP_BYTE] = START_STOP::IsStopRelayON();
    }
    _f32Pgn65365Data[PGN_65365_UNUSED_SPN_1] = 0xF;
    _f32Pgn65365Data[PGN_65365_UNUSED_SPN_2] = 0xFFFF;
    _f32Pgn65365Data[PGN_65365_UNUSED_SPN_3] = 0xFFFF;
    _f32Pgn65365Data[PGN_65365_UNUSED_SPN_4] = 0xFFFF;
    _f32Pgn65365Data[PGN_65365_UNUSED_SPN_5] = 0xFF;
}
void J1939APP::prvUpdatePGN65281Data(void)
{
    /* PGN-65281(PropB_01) : MTU Engine Start Stop(ESS) for Engine type "MTU"  */
    if(IS_START_STOP_RELAY_J1939_CONFIGURED())
    {
        _f32Pgn65281Data[PGN_65281_CAN_ENG_START] = START_STOP::IsStartRelayON();
        _f32Pgn65281Data[PGN_65281_CAN_REQ_ENG_STOP]= START_STOP::IsStopRelayON();
    }
    _f32Pgn65281Data[PGN_65281_UNUSED_SPN_1] = 0xF;
    _f32Pgn65281Data[PGN_65281_UNUSED_SPN_2] = 0xFFFF;
    _f32Pgn65281Data[PGN_65281_UNUSED_SPN_3] = 0xFFFF;
    _f32Pgn65281Data[PGN_65281_UNUSED_SPN_4] = 0xFF;
    _f32Pgn65281Data[PGN_65281_UNUSED_SPN_5] = 0x3;
    _f32Pgn65281Data[PGN_65281_UNUSED_SPN_6] = 0x3F;
    _f32Pgn65281Data[PGN_65281_UNUSED_SPN_7] = 0xFF;

}
void J1939APP::prvUpdatePGN65302Data(void)
{
    /* PGN-65302(PropB_16) : EMR Engine Stop Request for Engine type "DEUTZ"  */

    _f32Pgn65302Data[PGN_65302_ENG_STOP_BYTE] = START_STOP::IsStopRelayON();
    _f32Pgn65302Data[PGN_65302_UNUSED_SPN_1] = 0x3F;
    _f32Pgn65302Data[PGN_65302_UNUSED_SPN_2] = 0xFFFF;
    _f32Pgn65302Data[PGN_65302_UNUSED_SPN_3] = 0xFFFF;
    _f32Pgn65302Data[PGN_65302_UNUSED_SPN_4] = 0xFFFF;
    _f32Pgn65302Data[PGN_65302_UNUSED_SPN_5] = 0xFF;
}
void J1939APP::prvUpdatePGN65280Data(void)
{/* PGN-65280(PropB_00) : IVECO Engine Control Telegram(ENG_CNTRL) for Engine type "IVECO"  */

    /*  "No operations" = 00000000b,
        "Start request active" = 10000000b, Preheat
        "Start request by-pass cold procedure" = 10100000b,
        "Stop request active" = 00001000b.  */
   if(START_STOP::IsStartRelayON() && (!_bEngineStartPreheatStatus))
   {
       _f32Pgn65280Data[PGN_65280_ENG_START_STOP_BYTE] = 0xA0;
   }
   else if(START_STOP::IsStopRelayON())
   {
       _f32Pgn65280Data[PGN_65280_ENG_START_STOP_BYTE] = 0x08;
   }
   else if(_bEngineStartPreheatStatus && START_STOP::IsStartRelayON())
   {
       _f32Pgn65280Data[PGN_65280_ENG_START_STOP_BYTE] = 0x80;
   }
   else
   {
       _f32Pgn65280Data[PGN_65280_ENG_START_STOP_BYTE] = 0;
   }
   _f32Pgn65280Data[PGN_65280_UNUSED_SPN_1] = 0xFFFF;
   _f32Pgn65280Data[PGN_65280_UNUSED_SPN_2] = 0xFFFF;
   _f32Pgn65280Data[PGN_65280_UNUSED_SPN_3] = 0xFFFF;
   _f32Pgn65280Data[PGN_65280_UNUSED_SPN_4] = 0xFF;
}

void J1939APP::prvUpdatePGN65527Data(void)
{/* PGN-65527(same as TSC1) : SCANIA Engine */
    /* As per SRS first byte should be 0xF1 so F1 = 0b11110001.
     * Hence PGN_0_ENG_OVERRIDE_CONTROL_MODE = 01
     * PGN_0_ENG_REQ_SPEED_CONTROL = 00
     * PGN_0_OVERRIDE_CONTROL_MODE_PRIORITY = 11
     * PGN_0_HARDCODE_VALUE = 11                               */

    _f32Pgn65527Data[PGN_0_ENG_OVERRIDE_CONTROL_MODE] = 1;
    _f32Pgn65527Data[PGN_0_ENG_REQ_SPEED_CONTROL] = 0;
    _f32Pgn65527Data[PGN_0_OVERRIDE_CONTROL_MODE_PRIORITY] = 3;
    _f32Pgn65527Data[PGN_0_HARDCODE_VALUE] = 3;
//    if(_cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_TO_ECU))
//    {
//        if(START_STOP::IsIdleModeActive())
//        {
//            _f32Pgn65527Data[PGN_0_ENG_REQ_SPEED] = _cfgz.GetCFGZ_Param(CFGZ::ID_INITIAL_LOW_SPEED);
//        }
//        else
//        {
//            _f32Pgn65527Data[PGN_0_ENG_REQ_SPEED] = _cfgz.GetCFGZ_Param(CFGZ::ID_ENG_REQUESTED_SPEED_TO_ECU);
//        }
//    }
//    else
    {
        _f32Pgn65527Data[PGN_0_ENG_REQ_SPEED] = 0xFFFF;
    }

    //Not transmitting this SPN so send max value 0xFF.
    _f32Pgn65527Data[PGN_0_ENG_TORQUE] = 0xFF;

    /* Transmission rate as per SRS is 50 ms.
     *  STATE                       TRANSMISSION RATE
     *    0                             1000ms
     *    1                             750ms
     *    2                             500ms
     *    3                             250ms
     *    4                             100ms
     *    5                             50ms
     *    6                             20ms
     *    7                             Use Standard    */
    _f32Pgn65527Data[PGN_0_TRANSMISSION_RATE] = 7;

    // Not transmitting this SPN so send max value 0x1F.
    _f32Pgn65527Data[PGN_0_CONTROL_PURPOSE] = 0x1F;

    // Not transmitting this SPN so send 0xFFFF.
    _f32Pgn65527Data[PGN_0_ENG_REQ_TORQUE_FRACTION] = 0xFFFF;
    _f32Pgn65527Data[PGN_0_MESSAGE_COUNTER] = _u8PGN0xFFF7MessageCounter;

    // Checksum calculation
    CalculatePropB_F7Checksum();
}
void J1939APP::prvUpdatePGN65385Data(void)
{/* PGN-65385(CUMMINS) : CUMMINS Engine */
    /* When engine governing mode is Fixed speed (0% droop i.e ISOCHRONOUS send the speed %.
     * When engine governing mode is load based droop then send the value of percentage load.
     * If device does not have the governing mode then send the 0xFF i.e Not Available */
    /* Value 32768 sent as per DEIF's requirement . Since CUMMINS engine weren't working on earlier logic*/
    _f32Pgn65385Data[PGN_65385_CUMMINS_GOVERNING_RQST] = 32768;
    _f32Pgn65385Data[PGN_65385_UNUSED_SPN_1] = 0xFFFF;
    _f32Pgn65385Data[PGN_65385_UNUSED_SPN_2] = 0xFFFF;
    _f32Pgn65385Data[PGN_65385_UNUSED_SPN_3] = 0xFFFF;
    _f32Pgn65385Data[PGN_65385_UNUSED_SPN_3] = 32768;
}
void J1939APP::prvUpdatePGN57344Data(void)
{/* PGN-57344(CM1) : KUBOTA Engine */

    {
        _f32Pgn57344Data[PGN_57344_REGEN_SW_INHIBIT] = 3;
    }

    _f32Pgn57344Data[PGN_57344_UNUSED_SPN_1] = 0xFFFF;
    _f32Pgn57344Data[PGN_57344_UNUSED_SPN_2] = 0xFFFF;
    _f32Pgn57344Data[PGN_57344_UNUSED_SPN_3] = 0xFF;
    _f32Pgn57344Data[PGN_57344_UNUSED_SPN_4] = 0xF;
    if(CFGZ::ENG_HATZ == _cfgz.GetEngType())
    {
        if((START_STOP::GetStartStopSMDState() >=START_STOP::ID_STATE_SS_STOPPING)
                &&(START_STOP::GetStartStopSMDState() <=START_STOP::ID_STATE_SS_STOP_HOLD))
        {
            _f32Pgn57344Data[PGN_57344_ENG_AUTOMATIC_STRT_EN_SW] = 0 ;
        }
        else if((START_STOP::GetStartStopSMDState() >=START_STOP::ID_STATE_SS_PREHEAT)
                &&(START_STOP::GetStartStopSMDState() <=START_STOP::ID_STATE_SS_CRANK_REST))
        {
            _f32Pgn57344Data[PGN_57344_ENG_AUTOMATIC_STRT_EN_SW] = 1;
        }
        else
        {
            _f32Pgn57344Data[PGN_57344_ENG_AUTOMATIC_STRT_EN_SW] = 3;
        }
    }
    else
    {
        _f32Pgn57344Data[PGN_57344_ENG_AUTOMATIC_STRT_EN_SW] = 0x3;
    }

    _f32Pgn57344Data[PGN_57344_UNUSED_SPN_5] = 0xFFFF;
}
void J1939APP::prvUpdatePGN65219Data(void)
{    /* PGN-65219(ETC5) : KUBOTA Engine */

    if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor:: DI_GEN_CONTACTOR_LATCHED)
            != DigitalSensor:: SENSOR_NOT_CONFIGRUED )
    {
        _f32Pgn65219Data[PGN_65219_TRANS_NEUTRAL_SW] = 0;
    }
    else
    {
        _f32Pgn65219Data[PGN_65219_TRANS_NEUTRAL_SW] = 3;
    }

    _f32Pgn65219Data[PGN_65219_UNUSED_SPN_1] = 0xFF;
    _f32Pgn65219Data[PGN_65219_UNUSED_SPN_2] = 0x3;
    _f32Pgn65219Data[PGN_65219_UNUSED_SPN_3] = 0xF;
    _f32Pgn65219Data[PGN_65219_UNUSED_SPN_4] = 0xFFFF;
    _f32Pgn65219Data[PGN_65219_UNUSED_SPN_5] = 0xFFFF;
    _f32Pgn65219Data[PGN_65219_UNUSED_SPN_6] = 0xFFFF;
}
void J1939APP::prvUpdatePGN65363Data(void)
{
  /* PGN-65363(KBT PROPB_53 GEN4) : KUBOTA Engine */
//    if(_cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_TO_ECU))
//    {
//        if(START_STOP::IsIdleModeActive())
//        {
//            _f32Pgn65363Data[PGN_65363_TARGET_ENG_SPEED] = _cfgz.GetCFGZ_Param(CFGZ::ID_INITIAL_LOW_SPEED);;
//        }
//        else
//        {
//            _f32Pgn65363Data[PGN_65363_TARGET_ENG_SPEED] =_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_REQUESTED_SPEED_TO_ECU);
//        }
//    }
//    else
    {
        _f32Pgn65363Data[PGN_65363_TARGET_ENG_SPEED] = 0xFFFF;
    }

    _f32Pgn65363Data[PGN_65363_EGOV_CHARACTER] = 0x01;

    if(_engMon.IsEngineOn())
    {
        _f32Pgn65363Data[PGN_65363_ENGINE_STOP_INFO] = 0;
    }
    else
    {
        _f32Pgn65363Data[PGN_65363_ENGINE_STOP_INFO] = 0x1;
    }
}

void J1939APP::prvUpdatePGN65265Data(void)
{

    if(CFGZ::ENG_CUMMINS == _cfgz.GetEngType())
    {
        _f32Pgn65265Data[PGN_65265_PARKING_BRAKE_SW] = 0x03;
        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_1] = 0x03;
        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_2] = 0x0F;
        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_3] = 0xFF;
        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_4] = 0xFFFF;
        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_5] = 0xFFFF;
        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_6] = 0x00FF;
    }
    else
    {
        /* PGN-65265(CCVS) : KUBOTA Engine */
        if(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor:: DI_GEN_CONTACTOR_LATCHED)
                != DigitalSensor:: SENSOR_NOT_CONFIGRUED )
        {
            if(1)
            {
                _f32Pgn65265Data[PGN_65265_PARKING_BRAKE_SW] = 1;
            }
            else
            {
                _f32Pgn65265Data[PGN_65265_PARKING_BRAKE_SW] = 0;
            }

        }
        else
        {
            _f32Pgn65265Data[PGN_65265_PARKING_BRAKE_SW] = 3;
        }

        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_1] = 0x3;
        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_2] = 0xF;
        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_3] = 0xFF;
        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_4] = 0xFFFF;
        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_5] = 0xFFFF;
        _f32Pgn65265Data[PGN_65265_UNUSED_SPN_6] = 0xFFFF;
    }





}
void J1939APP::prvUpdatePGN65288Data(void)
{  /* PGN-65288 : Engine Speed*/

    _f32Pgn65288Data[PGN_65288_ENG_SPEED] = _engMon.GetFilteredEngSpeed();
    _f32Pgn65288Data[PGN_65288_UNUSED_SPN_1] = 0xFFFF;
    _f32Pgn65288Data[PGN_65288_UNUSED_SPN_2] = 0xFFFF;
    _f32Pgn65288Data[PGN_65288_UNUSED_SPN_3] = 0xFFFF;
}


void J1939APP::prvUpdatePGN59904Data(uint8_t *au8CalSPNData)
{
    //Following cases for sending request for these 6 PGN's.
    switch(_u8NumOf500msPGN)
    {
     /* The value of u8NumOf1sPGN is never used with 0. Hence there wont be case that expression
     * switch(u8NumOf1sPGN-1) results into -1 value */
        case 0: //RX_PGN_HOURS_65253
            *au8CalSPNData = 0xE5;
            au8CalSPNData++;
            *au8CalSPNData   = 0xFE;
            break;
        case 1://RX_PGN_LFC_65257
            *au8CalSPNData = 0xE9;
            au8CalSPNData++;
            *au8CalSPNData   = 0xFE;
            break;
        case 2://PGN_EEC4_65214
            *au8CalSPNData = 0xBE;
            au8CalSPNData++;
            *au8CalSPNData   = 0xFE;
            break;
        case 3://RX_PGN_LFI_65203
            *au8CalSPNData = 0xB3;
             au8CalSPNData++;
            *au8CalSPNData   = 0xFE;
            break;
        case 4://RX_PGN_AT1S2_64697
            *au8CalSPNData = 0xB9;
            au8CalSPNData++;
            *au8CalSPNData   = 0xFC;
            break;
        case 5://RX_PGN_AT1S_64891
            *au8CalSPNData = 0x7B;
            au8CalSPNData++;
            *au8CalSPNData   = 0xFD;
            break;
        case 6://RX_PGN_S2_65166
            *au8CalSPNData = 0x8E;
            au8CalSPNData++;
            *au8CalSPNData   = 0xFE;
            break;
        case 7://RX_PGN_PROPB57_65367
            *au8CalSPNData = 0x57;
            au8CalSPNData++;
            *au8CalSPNData   = 0xFF;
            break;
        case 8://RX_PGN_PROPB5E_65374
            *au8CalSPNData = 0x5E;
            au8CalSPNData++;
            *au8CalSPNData   = 0xFF;
            break;
        case 9://RX_PGN_IT1_65154
            *au8CalSPNData = 0x82;
            /* <LDRA Phase code 9S> <Assignment operation in expression.: Resolved.>
             * <Verified by: Nikhil Mhaske> <04/02/2020> */
            au8CalSPNData++;
            *au8CalSPNData   = 0xFE;
            break;
        case 10://RX_PGN_GFP_65163
            *au8CalSPNData = 0x8B;
            /* <LDRA Phase code 9S> <Assignment operation in expression.: Resolved.>
             * <Verified by: Nikhil Mhaske> <04/02/2020> */
            au8CalSPNData++;
            *au8CalSPNData   = 0xFE;
            break;
        case 11://RX_PGN_GFC_65199
            *au8CalSPNData = 0xAF;
            /* <LDRA Phase code 9S> <Assignment operation in expression.: Resolved.>
             * <Verified by: Nikhil Mhaske> <04/02/2020> */
            au8CalSPNData++;
            *au8CalSPNData   = 0xFE;
            break;
        default:
            break;
    }
}
void J1939APP::ExtractReadFrame(void)
{
    uint8_t u8Local;
    static uint16_t  u16LocalPGN;
    static uint8_t u8CntDM1Index=0;
    static uint8_t u8CntDM2Index=0;
    static uint8_t u8CntPropB5EIndex = 0;
    static uint8_t u8NumberOfPacket = 0,u8ReadData=0;

    uint8_t u8PS, u8PF;
//    uint8_t u8SourceAddress;
    bool bECUSourceAddressMatched = false;
    static uint8_t u8PrevPackateNo =0;
    uint8_t u8ReceivedPgnNo = RX_PGN_LAST;
    can_frame_t CAN_RxQueueBuffer;

     typedef union
     {
         tPDUIDFormat tPDUIdFrame;
         uint32_t ulPDUId;
     }uPDUIDData;

     uPDUIDData uPDU_ID_Data;

     if(_hal.Objcan.ReadFrame(&CAN_RxQueueBuffer) == BSP_SUCCESS)
     {
        /// read the CAN frame id
        uPDU_ID_Data.ulPDUId =  CAN_RxQueueBuffer.id;

        u8PS =(uint8_t)( uPDU_ID_Data.tPDUIdFrame.uiPDU_PGN & 0x00FF);    // PDU Specific
        u8PF =(uint8_t) ((uPDU_ID_Data.tPDUIdFrame.uiPDU_PGN >>8)& 0x00FF); //PDU Format

//        u8SourceAddress = (uint8_t) (uPDU_ID_Data.tPDUIdFrame.ubyPDU_SA);
        /* If the received source address is same as the configured ECU Source address then only received the data.  */
//        if(u8SourceAddress == _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_SOURCE_ADDRESS))
        {
            bECUSourceAddressMatched = true;
        }
        //  when 0 <= PF(PDU Format) <= 239 defines peer-peer communication. This is PDU1 format.
        //  In this case, PS defines a 8 bit destination node address.
        //  PGN = (DP<<9 + PF<<8 + 0)
        //  In this case, PS is not considered a part of the PGN.
        //  when 240 <= PF <= 255 defines broadcast communication. This is PDU2 format.
        //  In this case, PS defines a 8 bit Group extension. A group extension
        //  identifies a group of nodes on the J1939 bus.
        //  PGN = (DP<<9 + PF<<8 + PS)  PGN-Parameter Group Number

        if((u8PF <=255)&&(u8PF >=240))
        {
            uPDU_ID_Data.tPDUIdFrame.uiPDU_PGN = (uint16_t)((((uint16_t)uPDU_ID_Data.tPDUIdFrame.ubPGN_DP)<<9U)
                                                   + (((uint16_t)u8PF)<<8U)+ u8PS);
        }
        else if((u8PF <=239)&&(u8PF >=0))
        {
            uPDU_ID_Data.tPDUIdFrame.uiPDU_PGN = (uint16_t)((((uint16_t)uPDU_ID_Data.tPDUIdFrame.ubPGN_DP)<<9U)
                                               + (((uint16_t)u8PF)<<8U));
        }
        if(bECUSourceAddressMatched)
        {
            bECUSourceAddressMatched = false;
            switch(uPDU_ID_Data.tPDUIdFrame.uiPDU_PGN)
            {

                case PGN_TPCM: //TPCM Transfer protocol - connection management ---> BAM-Braudcast Announce message
                    u16LocalPGN = (uint16_t)((((uint16_t)CAN_RxQueueBuffer.data[6]) << 8U) + CAN_RxQueueBuffer.data[5]);
                    if((u16LocalPGN == PGN_DM01)|| (u16LocalPGN == PGN_DM02) || (u16LocalPGN == PROPB_5E_PGN))
                    {
                        u8ReadData = 1;
                        u8NumberOfPacket =CAN_RxQueueBuffer.data[3];

                        //Number of SPN(Suspect Parameter Number)

                        if((u16LocalPGN == PGN_DM01))
                        {
                            /* If DM1 packet is received then clear DM1 log reset timer, in order not to clear received log */
                            u8CntDM1Index = 0;
                            _u8NumOfDM1SPN= (uint8_t)(((((uint16_t)CAN_RxQueueBuffer.data[2] << 8U) | CAN_RxQueueBuffer.data[1]) - 2) / DM1_DTC_SIZE);
                        }
                        else if(u16LocalPGN == PGN_DM02)
                        {
                            u8CntDM2Index = 0;
                            _u8NumOfDM2SPN= (uint8_t)(((((uint16_t)CAN_RxQueueBuffer.data[2] << 8U) | CAN_RxQueueBuffer.data[1]) - 2) / DM1_DTC_SIZE);
                        }
                        else if(u16LocalPGN == PROPB_5E_PGN)
                        {
                            u8CntPropB5EIndex = 0;
                        }
                    }
                    break;

                case PGN_TPDT://TPDT - Transfer Protocol Data Transfer
                    if(u8ReadData)
                    {
                        if(u16LocalPGN == PGN_DM01)
                        {
                            for(u8Local = 1; u8Local < 8; u8Local++)
                            {
                                _u8ArrDM1SPNData[u8CntDM1Index++]= CAN_RxQueueBuffer.data[u8Local];
                            }
                        }
                        else if(u16LocalPGN == PGN_DM02)
                        {
                            for(u8Local = 1; u8Local < 8; u8Local++)
                            {
                                _u8ArrDM2SPNData[u8CntDM2Index++]= CAN_RxQueueBuffer.data[u8Local];
                            }
                        }
                        else if(u16LocalPGN == PROPB_5E_PGN)
                        {
                            for(u8Local = 1; u8Local < 8; u8Local++)
                            {
                                _u8ArrPropB5EPGNData[u8CntPropB5EIndex++]= CAN_RxQueueBuffer.data[u8Local];
                            }
                        }

                        if((CAN_RxQueueBuffer.data[0] > u8PrevPackateNo)||(u8ReadData==1))
                        {
                            if(u8ReadData == 1)
                            {
                                u8PrevPackateNo = CAN_RxQueueBuffer.data[0];
                                u8ReadData = 2;
                            }
                            u8PrevPackateNo = CAN_RxQueueBuffer.data[0];
                            u8NumberOfPacket--;

                            if(u8NumberOfPacket == 0) // Compare to 1 as first packate is TPCM
                            {
                                if(u16LocalPGN == PGN_DM01)
                                {
                                    prvExtractDmMsg(PGN_DM01, _u8NumOfDM1SPN);       // Extract all SPN data.
                                    u8CntDM1Index = 0;
                                }
                                else if(u16LocalPGN == PGN_DM02)
                                {
                                    prvExtractDmMsg(PGN_DM02, _u8NumOfDM2SPN);
                                    u8CntDM2Index = 0;
                                }
                                else if(u16LocalPGN == PROPB_5E_PGN)
                                {
                                    GetPGN(RX_PGN_PROPB5E_65374, CALC_FOR_RECEIVE, &tSupportPGN);
                                    for(uint8_t u8RxSPNNum = 0; u8RxSPNNum < tSupportPGN.ubyPDU_SPNs; u8RxSPNNum++)
                                    {
                                        GetSPN(RX_PGN_PROPB5E_65374, u8RxSPNNum, CALC_FOR_RECEIVE, &tSupportSPN);
                                        tSPN.u64SPNData = prvExtractSPN((uint8_t*)_u8ArrPropB5EPGNData, RX_PGN_PROPB5E_65374, u8RxSPNNum, tSupportSPN.u16SPN_Loc, tSupportSPN.ubySPN_DataLength);

                                        CalculateSPN(tSupportSPN.ufSPN_Resolution,
                                                tSupportSPN.ufMinValDataRange, tSupportSPN.ufMaxValDataRange, CALC_FOR_RECEIVE);

                                        if(_ArrSpnErrorNAStatus[RX_PGN_PROPB5E_65374][u8RxSPNNum] == VALID_DATA)
                                        {
                                            _ArrPgnReadData[RX_PGN_PROPB5E_65374][u8RxSPNNum] = tSPN.f64SPNData;
                                        }
                                        else
                                        {
                                            _ArrPgnReadData[RX_PGN_PROPB5E_65374][u8RxSPNNum] = 0;
                                        }
                                    }
                                    u8CntPropB5EIndex = 0;
                                }
                                u8ReadData = 0;
                            }
                        }
                        else
                        {
                            u8ReadData=0;
                        }
                    }
                    break;
                case PGN_DM01://DM01- Diagnostic messages
                        for(u8Local=0;u8Local<8; u8Local++  )
                        {
                            _u8ArrDM1SPNData[u8CntDM1Index++] = CAN_RxQueueBuffer.data[u8Local];
                        }
                        _u8NumOfDM1SPN =1;
                        prvExtractDmMsg(PGN_DM01, _u8NumOfDM1SPN);
                        u8CntDM1Index=0;
                    break;
                case PGN_DM02://DM02- Diagnostic messages
                        for(u8Local = 0; u8Local < 8; u8Local++)
                        {
                            _u8ArrDM2SPNData[u8CntDM2Index++] = CAN_RxQueueBuffer.data[u8Local];
                        }
                        _u8NumOfDM2SPN = 1;
                        prvExtractDmMsg(PGN_DM02, _u8NumOfDM2SPN);
                        u8CntDM2Index = 0;
                        break;
                case PGN_RQST: //RQST- Request message
                       _u32RequestedPGN = (((uint32_t)CAN_RxQueueBuffer.data[2]) << 16) + ((uint16_t)CAN_RxQueueBuffer.data[1] << 8) + CAN_RxQueueBuffer.data[0];
                       u8ReceivedPgnNo = RX_PGN_LAST;
                       break;

               case DPFC1_PGN:
                   u8ReceivedPgnNo = RX_PGN_DPFC1_64892  ;
                   break;

               case EEC1_PGN:
                   UTILS_ResetTimer(&_CommFailTimeout);
                   u8ReceivedPgnNo = RX_PGN_EEC1_61444    ;
                   _bIsCANJ1939CommFail = false;
                   break;

               case LFE_PGN:
                   u8ReceivedPgnNo = RX_PGN_LFE_65266     ;
                   break;

               case EEC2_PGN:
                   u8ReceivedPgnNo = RX_PGN_EEC2_61443    ;
                   break;

               case EFL_P1_PGN:
                   u8ReceivedPgnNo = RX_PGN_EFL_P1_65263  ;
                   break;

               case IC1_PGN:
                   u8ReceivedPgnNo = RX_PGN_IC1_65270     ;
                   break;

               case ET1_PGN:
                   u8ReceivedPgnNo = RX_PGN_ET1_65262     ;
                   break;

               case AMB_PGN:
                   u8ReceivedPgnNo = RX_PGN_AMB_65269     ;
                   break;

               case HOURS_PGN:
                   u8ReceivedPgnNo = RX_PGN_HOURS_65253   ;
                   break;

               case VEP1_PGN:
                   u8ReceivedPgnNo = RX_PGN_VEP1_65271    ;
                   break;

               case DD_PGN:
                   u8ReceivedPgnNo = RX_PGN_DD_65276      ;
                   break;

               case WFI_PGN:
                   u8ReceivedPgnNo = RX_PGN_WFI_65279     ;
                   break;

               case LFC_PGN:
                   u8ReceivedPgnNo = RX_PGN_LFC_65257     ;
                   break;

               case EEC4_PGN:
                   u8ReceivedPgnNo = RX_PGN_EEC4_65214    ;
                   break;

               case LFI_PGN:
                   u8ReceivedPgnNo = RX_PGN_LFI_65203     ;
                   break;

               case S2_PGN:
                   u8ReceivedPgnNo = RX_PGN_S2_65166     ;
                   break;

               case IC2_PGN:
                   u8ReceivedPgnNo = RX_PGN_IC2_64976     ;
                   break;

               case EOI_PGN:
                   u8ReceivedPgnNo = RX_PGN_EOI_64914     ;
                   break;

               case ET2_PGN:
                   u8ReceivedPgnNo = RX_PGN_ET2_65188     ;
                   break;

               case EEC3_PGN:
                   u8ReceivedPgnNo = RX_PGN_EEC3_65247     ;
                   break;

               case EFL_P2_PGN:
                   u8ReceivedPgnNo = RX_PGN_EFL_P2_65243     ;
                   break;

               case SHUTDOWN_PGN:
                   u8ReceivedPgnNo = RX_PGN_SHUTDOWN_65252     ;
                   break;

               case EFG1_PGN:
                   u8ReceivedPgnNo = RX_PGN_EFG1_61450     ;
                   break;

               case DPF1S_PGN:
                   u8ReceivedPgnNo = RX_PGN_DPF1S_64796     ;
                   break;

               case A1DOC_PGN:
                   u8ReceivedPgnNo = RX_PGN_A1DOC_64800     ;
                   break;

               case AT1IG2_PGN:
                   u8ReceivedPgnNo = RX_PGN_AT1IG2_64948     ;
                   break;

               case AT1IMG_PGN:
                   u8ReceivedPgnNo = RX_PGN_AT1IMG_64946     ;
                   break;

               case AT1T1I_PGN:
                   u8ReceivedPgnNo = RX_PGN_AT1T1I_65110  ;
                   break;

               case PROPB32_KUBOTA_PGN:
                   u8ReceivedPgnNo = RX_PGN_PROPB_32_KUBOTA_65330  ;
                   break;

               case EMR_PREHEAT_PGN:
                   u8ReceivedPgnNo = RX_PGN_EMR_PREHEAT_65284  ;
                   break;

               case PROPB_51_PGN:
                   u8ReceivedPgnNo = RX_PGN_PROPB51_65361;
                   break;

               case PROPB_62_PGN:
                   u8ReceivedPgnNo = RX_PGN_PROPB62_65378;
                   break;

               case PROPB_57_PGN:
                   u8ReceivedPgnNo = RX_PGN_PROPB57_65367;
                   break;

               case IVECO_PREHEAT_PGN:
                   u8ReceivedPgnNo = RX_PGN_IVECO_PREHEAT_65281;
                   break;

               case VOLVO_PREHEAT_PGN:
                   u8ReceivedPgnNo = RX_PGN_VOLVO_PREHEAT_65351  ;
                   break;

               case IVECO_ENG_STATUS_PGN:
                   u8ReceivedPgnNo = RX_PGN_IVECO_ENG_STATUS_65282;
                   break;

               case AT1IG1_PGN:
                   u8ReceivedPgnNo = RX_PGN_AT1IG1_61454  ;
                   break;

               case AT1OG1_PGN:
                   u8ReceivedPgnNo = RX_PGN_AT1OG1_61455   ;
                   break;

               case A1SCRDSI1_PGN:
                   u8ReceivedPgnNo = RX_PGN_A1SCRDSI1_61475;
                   break;

               case A1SCRDSI2_PGN:
                   u8ReceivedPgnNo = RX_PGN_A1SCRDSI2_64833;
                   break;

               case A1SCRDSR1_PGN:
                   u8ReceivedPgnNo = RX_PGN_A1SCRDSR1_61476;
                   break;

               case A1SCREGT1_PGN:
                   u8ReceivedPgnNo = RX_PGN_A1SCREGT1_64830;
                   break;

               case AT2IG1_PGN:
                   u8ReceivedPgnNo = RX_PGN_AT2IG1_61456  ;
                   break;

               case AT2OG1_PGN:
                   u8ReceivedPgnNo = RX_PGN_AT2OG1_61457   ;
                   break;

               case A2SCRDSI2_PGN:
                   u8ReceivedPgnNo = RX_PGN_A2SCRDSI2_64827;
                   break;

               case A2SCRDSI1_PGN:
                   u8ReceivedPgnNo = RX_PGN_A2SCRDSI1_61478;
                   break;

               case A2SCRDSR1_PGN:
                   u8ReceivedPgnNo = RX_PGN_A2SCRDSR1_61479;
                   break;

               case A2SCREGT1_PGN:
                   u8ReceivedPgnNo = RX_PGN_A2SCREGT1_64824;
                   break;

               case AT1OG2_PGN:
                   u8ReceivedPgnNo = RX_PGN_AT1OG2_64947   ;
                   break;

               case AT1S2_PGN:
                   u8ReceivedPgnNo = RX_PGN_AT1S2_64697    ;
                   break;

               case AT1S_PGN:
                   u8ReceivedPgnNo = RX_PGN_AT1S_64891     ;
                   break;
               case PROSTOUT_PGN:
                   u8ReceivedPgnNo = RX_PGN_PROSTOUT_65364     ;
                   break;

               case PROPB00_PGN:
                   u8ReceivedPgnNo = RX_PGN_HATZ_PROPB_PHYS_65280     ;
                   break;

               case AI_PGN:
                   u8ReceivedPgnNo = RX_PGN_AI_65237     ;
                   break;

               case ETC5_PGN:
                   u8ReceivedPgnNo = RX_PGN_ETC5_65219     ;
                   break;

               case OII_PGN:
                   u8ReceivedPgnNo = RX_PGN_OII_64554     ;
                   break;
               case IT1_PGN:
                   u8ReceivedPgnNo = RX_PGN_IT1_65154     ;
               break;
               case GFP_PGN:
                   u8ReceivedPgnNo = RX_PGN_GFP_65163     ;
               break;
               case IMI1_PGN:
                   u8ReceivedPgnNo = RX_PGN_IMI1_65190     ;
               break;
              case FD1_PGN  :
                  u8ReceivedPgnNo = RX_PGN_FD1_65213     ;
                  break;
              case DLCC1_PGN:
                  u8ReceivedPgnNo = RX_PGN_DLCC1_64775     ;
              break;
               case GFC_PGN:
                   u8ReceivedPgnNo = RX_PGN_GFC_65199     ;
               break;
               case EPT1_PGN:
                   u8ReceivedPgnNo = RX_PGN_EPT1_65187;
                   break;
               case EPT2_PGN:
                   u8ReceivedPgnNo = RX_PGN_EPT2_65186;
                   break;
               case EPT3_PGN:
                   u8ReceivedPgnNo = RX_PGN_EPT3_65185;
                   break;
               case EPT4_PGN:
                   u8ReceivedPgnNo = RX_PGN_EPT4_65184;
                   break;
               case EPT5_PGN:
                   u8ReceivedPgnNo = RX_PGN_EPT5_65183;
                   break;
               case ET4_PGN:
                   u8ReceivedPgnNo = RX_PGN_ET4_64870;
                   break;
               case TCI4_PGN:
                   u8ReceivedPgnNo = RX_PGN_TCI4_65176;
                   break;
               case EFL_P12_PGN:
                   u8ReceivedPgnNo = RX_PGN_EFL_P12_64735;
                   break;
     //          case EGOV_PID_GAINS_PGN:
//               {
//                   ArrEditableItem[INDEX_OF_EGOV_GEN_SET_SPEED].value.u16Val = (uint16_t)((CAN_RxQueueBuffer.data[1] << 8) + CAN_RxQueueBuffer.data[0]);
//                   ArrEditableItem[INDEX_OF_EGOV_GEN_KP].value.u16Val = (uint16_t)((CAN_RxQueueBuffer.data[3] << 8) + CAN_RxQueueBuffer.data[2]);
//                   ArrEditableItem[INDEX_OF_EGOV_GEN_KI].value.u16Val = (uint16_t)((CAN_RxQueueBuffer.data[5] << 8) + CAN_RxQueueBuffer.data[4]);
//                   ArrEditableItem[INDEX_OF_EGOV_GEN_KD].value.u16Val = (uint16_t)((CAN_RxQueueBuffer.data[7] << 8) + CAN_RxQueueBuffer.data[6]);
//
//                   _egov.InitEgovParameters();
//                   u8ReceivedPgnNo = RX_PGN_LAST;
//               }
   //                break;
   //            case EGOV_PARAMS_PGN:
//               {
//                   ArrEditableItem[INDEX_OF_EGOV_GEN_DITHER].value.u16Val = (uint16_t)((CAN_RxQueueBuffer.data[1] << 8) + CAN_RxQueueBuffer.data[0]);
//                   ArrEditableItem[INDEX_OF_EGOV_GEN_GAIN_SCHEDULE].value.fVal = (float)((CAN_RxQueueBuffer.data[3] << 8) + CAN_RxQueueBuffer.data[2]);
//                   ArrEditableItem[INDEX_OF_EGOV_GEN_LOADING_FAC].value.u16Val = (uint16_t)((CAN_RxQueueBuffer.data[5] << 8) + CAN_RxQueueBuffer.data[4]);
//                   ArrEditableItem[INDEX_OF_EGOV_GEN_UNLOADING_FAC].value.u16Val = (uint16_t)((CAN_RxQueueBuffer.data[7] << 8) + CAN_RxQueueBuffer.data[6]);
//
//                   _egov.InitEgovParameters();
//                   u8ReceivedPgnNo = RX_PGN_LAST;
//               }
    //               break;
   //            case EGOV_APPL_MODE_PGN:
//               {
//                   _u8J1939PIDTune_Normal = CAN_RxQueueBuffer.data[1];
//
//                   if(CAN_RxQueueBuffer.data[1] == 1) // Write to Flash if 1st bit is one.
//                   {
//                       _ConfigParam.u16ArrParam[CFGZ::ID_EGOV_GEN_SET_SPEED] = ArrEditableItem[INDEX_OF_EGOV_GEN_SET_SPEED].value.u16Val;
//                       _ConfigParam.u16ArrParam[CFGZ::ID_EGOV_GEN_KP] = ArrEditableItem[INDEX_OF_EGOV_GEN_KP].value.u16Val;
//                       _ConfigParam.u16ArrParam[CFGZ::ID_EGOV_GEN_KI] = ArrEditableItem[INDEX_OF_EGOV_GEN_KI].value.u16Val;
//                       _ConfigParam.u16ArrParam[CFGZ::ID_EGOV_GEN_KD] = ArrEditableItem[INDEX_OF_EGOV_GEN_KD].value.u16Val;
//
//                       _ConfigParam.u16ArrParam[CFGZ::ID_EGOV_GEN_DITHER] = ArrEditableItem[INDEX_OF_EGOV_GEN_DITHER].value.u16Val;
//                       _ConfigParam.f32ArrParam[CFGZ::ID_EGOV_GEN_GAIN_SCHEDULE] = ArrEditableItem[INDEX_OF_EGOV_GEN_GAIN_SCHEDULE].value.fVal;
//                       _ConfigParam.u16ArrParam[CFGZ::ID_EGOV_GEN_LOADING_FAC] = ArrEditableItem[INDEX_OF_EGOV_GEN_LOADING_FAC].value.u16Val;
//                       _ConfigParam.u16ArrParam[CFGZ::ID_EGOV_GEN_UNLOADING_FAC] = ArrEditableItem[INDEX_OF_EGOV_GEN_UNLOADING_FAC].value.u16Val;
//
//                       _cfgz.WriteActiveProfile(&_ConfigParam);
//                       _egov.InitEgovParameters();
//                   }
//                   u8ReceivedPgnNo = RX_PGN_LAST;
//               }
    //               break;
               case CCVS_PGN:
                   u8ReceivedPgnNo = RX_PGN_HATZ_CCVS_65265     ;
                 break;
               case DM11_RESET_DM01:
                   _bClearActiveDTCs = true;
                   u8ReceivedPgnNo = RX_PGN_LAST;
                   break;


                default:
                    break;
            }

            if(RX_PGN_LAST > u8ReceivedPgnNo)
            {
                GetPGN(u8ReceivedPgnNo, CALC_FOR_RECEIVE, &tSupportPGN);
                // Run the for loop for number of SPN's in single PGN
                for(uint8_t u8RxSPNNum = 0; u8RxSPNNum < tSupportPGN.ubyPDU_SPNs; u8RxSPNNum++)
                {
                    GetSPN(u8ReceivedPgnNo, u8RxSPNNum, CALC_FOR_RECEIVE, &tSupportSPN);
                    tSPN.u64SPNData = prvExtractSPN(CAN_RxQueueBuffer.data, u8ReceivedPgnNo, u8RxSPNNum, tSupportSPN.u16SPN_Loc, tSupportSPN.ubySPN_DataLength);

                    CalculateSPN(tSupportSPN.ufSPN_Resolution,
                            tSupportSPN.ufMinValDataRange, tSupportSPN.ufMaxValDataRange, CALC_FOR_RECEIVE);
                    if(_ArrSpnErrorNAStatus[u8ReceivedPgnNo][u8RxSPNNum] == VALID_DATA)
                    {
                        _ArrPgnReadData[u8ReceivedPgnNo][u8RxSPNNum] = tSPN.f64SPNData;
                    }
                    else
                    {
                        _ArrPgnReadData[u8ReceivedPgnNo][u8RxSPNNum] = 0;
                    }
                }
            }
//             Todo: Use the DPF Ash Load percentage as a pvalue for Ash Load Maintenance Alarm
//            if(u8ReceivedPgnNo == RX_PGN_AT1S_64891)
//            {
//                gstAftData.u8DPFAshLoadPercentage = _ArrPgnReadData[u8ReceivedPgnNo][1];
//            }
        }
    }
}


void J1939APP::Update(bool bDeviceInconfig)
{
#define GAIN 0.2f

    uint16_t u16PGN_State ;

    _bDeviceInConfigMode =bDeviceInconfig;

    if(_bRequestDM11PGN || _bRequestDM2PGN )
    {
        GenerateFrame(PGN_TX_RQST);
        TransmitFrame(PGN_TX_RQST);
    }

     GenerateFrame(_u8PGN_Num);
    _u8PGN_Num++;//change name according to guidelines
    if(_u8PGN_Num >= TX_LAST_PGN)
    {
       _u8PGN_Num =0;
    }

    ExtractReadFrame();


    CFGZ::ENGINE_TYPES_t eEngineType = _cfgz.GetEngType();
    if((!_bDeviceInConfigMode)&&(eEngineType != CFGZ::ENG_CONVENTIONAL))
    {
        prvHandleEngineStartStop();

        if(UTILS_GetElapsedTimeInMs(&_Timer10ms)>=10)
        {
            for(uint8_t u8PGN=0; u8PGN < TX_LAST_PGN; u8PGN++)
            {
                u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
                if(u16PGN_State == PGN_10MS)
                {
                    if(eEngineType == CFGZ::ENG_KUBOTA)
                    {
                        if(u8PGN == PGN_PROPB_53_KBT)
                        {
                            TransmitFrame(u8PGN);
                        }
                        if(u8PGN == PGN_TSC1)
                        {
                            CalculateTsc1Checksum();
                            TransmitFrame(u8PGN);
                            _u8PGN0MessageCounter++;
                            if(_u8PGN0MessageCounter > 7)
                            {
                                _u8PGN0MessageCounter = 0;
                            }
                        }
                    }
                    else if( (u8PGN == PGN_TSC1)
                          && ((((CFGZ::ENG_CUMMINS != eEngineType)&&(eEngineType != CFGZ::ENG_YUCHAI_YCGCU)&& (eEngineType > (uint8_t)CFGZ::ENG_VOLVO )) || (CFGZ::ENG_DEFAULT == eEngineType))))
                    {
                        CalculateTsc1Checksum();
                        TransmitFrame(u8PGN);

                        _u8PGN0MessageCounter++;
                        if(_u8PGN0MessageCounter > 7)
                        {
                            _u8PGN0MessageCounter = 0;
                        }
                    }
                    else if((u8PGN == PGN_PROPB_F7) && (CFGZ::ENG_SCANIA == eEngineType))
                    {

                        CalculatePropB_F7Checksum();
                        TransmitFrame(u8PGN);

                        _u8PGN0xFFF7MessageCounter++;
                        if(_u8PGN0xFFF7MessageCounter > 7)
                        {
                            _u8PGN0xFFF7MessageCounter = 0;
                        }
                    }

                }

                (void)R_IWDT_Refresh(&g_wdt0_ctrl);
            }
            UTILS_ResetTimer(&_Timer10ms);
        }

        if(( UTILS_GetElapsedTimeInMs(&_Timer20ms) >= 20))
        {
            for(uint8_t u8PGN=0; u8PGN < TX_LAST_PGN; u8PGN++)
            {
               u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
               if(u16PGN_State == PGN_20MS)
               {
                   if( (u8PGN == PGN_PROPB_80) && (CFGZ::ENG_SCANIA == eEngineType) )
                   {
                       TransmitFrame(u8PGN);
                   }
                   if( (u8PGN == PGN_PROPB_46) && (CFGZ::ENG_VOLVO == eEngineType) )
                   {
                       TransmitFrame(u8PGN);
                       _u8AccPedalCounter++;
                       if(_u8AccPedalCounter > 7)
                       {
                           _u8AccPedalCounter =0;
                       }
                   }
               }
               (void)R_IWDT_Refresh(&g_wdt0_ctrl);
            }

            UTILS_ResetTimer(&_Timer20ms);
        }

        if(( UTILS_GetElapsedTimeInMs(&_Timer40ms) >= 40))
        {
            if(CFGZ::ENG_YUCHAI_YCGCU == eEngineType)
            {
                for(uint8_t u8PGN=0; u8PGN < TX_LAST_PGN; u8PGN++)
                {
                  u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
                   if(u8PGN == PGN_TSC1 )
                  {
                      TransmitFrame(u8PGN);
                  }

                  (void)R_IWDT_Refresh(&g_wdt0_ctrl);
                }
            }

           UTILS_ResetTimer(&_Timer40ms);
        }


        if(( UTILS_GetElapsedTimeInMs(&_Timer50ms) >= 50))
        {
           for(uint8_t u8PGN=0; u8PGN < TX_LAST_PGN; u8PGN++)
           {
              u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
              if(u16PGN_State == PGN_50MS)
              {
                  if((u8PGN == PGN_PROPB_00) && (CFGZ::ENG_IVECO == eEngineType))
                  {
                      TransmitFrame(u8PGN);
                  }
                  else if(u8PGN == PGN_PROPB_08_ENG_SPEED)
                  {
                      if(_u8J1939PIDTune_Normal == 1)
                      {
                          TransmitFrame(u8PGN);
                      }
                  }
              }
              (void)R_IWDT_Refresh(&g_wdt0_ctrl);
           }

           UTILS_ResetTimer(&_Timer50ms);
        }

        if(( UTILS_GetElapsedTimeInMs(&_Timer80ms) >= 80))
        {
           for(uint8_t u8PGN=0; u8PGN < TX_LAST_PGN; u8PGN++)
           {
               if((u8PGN == PGN_GC1)&&((CFGZ::ENG_HATZ == _cfgz.GetEngType())
                       ||(CFGZ::ENG_YUCHAI_YCGCU == _cfgz.GetEngType())
                       ||(CFGZ::ENG_WECHAI == _cfgz.GetEngType())
                       ||(CFGZ::ENG_PERKINS_ADAM4 == _cfgz.GetEngType()))
                  )
               {
                   u16PGN_State = PGN_80MS;
               }
               else if(CFGZ::ENG_CUMMINS == _cfgz.GetEngType())
               {
                   u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
               }
               else
               {
                   u16PGN_State = 0;
               }
              if(u16PGN_State == PGN_80MS)
              {
                   TransmitFrame(u8PGN);
              }
              (void)R_IWDT_Refresh(&g_wdt0_ctrl);
           }

           UTILS_ResetTimer(&_Timer80ms);
        }

        if(( UTILS_GetElapsedTimeInMs(&_Timer100ms) >= 100) )
        {
            for(uint8_t u8PGN=0; u8PGN<TX_LAST_PGN; u8PGN++ )
            {
                if((u8PGN == PGN_CM1)&&((CFGZ::ENG_HATZ == _cfgz.GetEngType())
                        ||(CFGZ::ENG_YUCHAI_YCGCU == _cfgz.GetEngType())
                        ||(CFGZ::ENG_PERKINS_ADAM4 == _cfgz.GetEngType())))
                {
                    u16PGN_State = PGN_100MS;
                }
                else
                {
                    u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
                }
                if(u16PGN_State == PGN_100MS)
                {
                    if( (u8PGN == PGN_GC1) && ((CFGZ::ENG_DEFAULT == eEngineType) || (CFGZ::ENG_CUMMINS == eEngineType)|| (CFGZ::ENG_MTU ==eEngineType)) )
                    {
                        TransmitFrame(u8PGN);
                    }
                    if( (u8PGN == PGN_PROPB_55) && (CFGZ::ENG_MTU == eEngineType) )
                    {
                        TransmitFrame(u8PGN);
                    }
                    if( (u8PGN == PGN_PROPB_01) && (CFGZ::ENG_MTU == eEngineType) )
                    {
                        TransmitFrame(u8PGN);
                    }
                    if( (u8PGN == PGN_PROPB_16) && (CFGZ::ENG_DEUTZ_EMR == eEngineType) )
                    {
                        TransmitFrame(u8PGN);
                    }
                    if((u8PGN == PGN_ETC5) && (CFGZ::ENG_KUBOTA == eEngineType))
                    {
                        TransmitFrame(u8PGN);
                    }
                    if((u8PGN == PGN_CCVS) && ((CFGZ::ENG_KUBOTA == eEngineType) || (CFGZ::ENG_CUMMINS == eEngineType)))
                    {
                        TransmitFrame(u8PGN);
                    }
                    if((u8PGN == PGN_CM1) &&((CFGZ::ENG_HATZ == _cfgz.GetEngType())||(CFGZ::ENG_YUCHAI_YCGCU == _cfgz.GetEngType())||(CFGZ::ENG_PERKINS_ADAM4 == _cfgz.GetEngType())))
                    {
                        TransmitFrame(u8PGN);
                    }
                    if((u8PGN == PGN_EBC1) &&((CFGZ::ENG_DCEC_CUMMINS == _cfgz.GetEngType())||(CFGZ::ENG_YUCHAI_BOSCH == _cfgz.GetEngType())))
                    {
                        TransmitFrame(u8PGN);
                    }
                }
                (void)R_IWDT_Refresh(&g_wdt0_ctrl);
            }

            UTILS_ResetTimer(&_Timer100ms);
        }

        if(( UTILS_GetElapsedTimeInMs(&_Timer200ms) >= 200))
        {

            for(uint8_t u8PGN=0; u8PGN<TX_LAST_PGN; u8PGN++)
            {
               u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
               if((u16PGN_State == PGN_200MS)&&(u8PGN == PGN_ETC3) && (CFGZ::ENG_PERKINS_ADAM4 == _cfgz.GetEngType()))
               {
                   TransmitFrame(u8PGN);
               }
               (void)R_IWDT_Refresh(&g_wdt0_ctrl);
            }
            UTILS_ResetTimer(&_Timer200ms);
        }

        if(( UTILS_GetElapsedTimeInMs(&_Timer500ms) >= 500))
        {
            for(uint8_t u8PGN=0; u8PGN<TX_LAST_PGN; u8PGN++)
            {
                u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
                if(u16PGN_State == PGN_500MS)
                {
                    TransmitFrame(u8PGN);
                    _u8NumOf500msPGN++;

                    if(eEngineType == CFGZ::ENG_KUBOTA)
                    {
                        if(_u8NumOf500msPGN == 9)
                        {
                            _u8NumOf500msPGN = 0;
                        }
                    }
                    else if(eEngineType == CFGZ::ENG_YUCHAI_YCGCU)
                    {
                        if((_u8NumOf500msPGN >= 7) &&(_u8NumOf500msPGN <= 8))
                        {
                            _u8NumOf500msPGN = 9;
                        }
                        if(_u8NumOf500msPGN > 10)
                        {
                            _u8NumOf500msPGN = 0;
                        }
                    }
                    else if(eEngineType == CFGZ::ENG_WECHAI)
                    {
                        if((_u8NumOf500msPGN >= 7)&&(_u8NumOf500msPGN <11))
                        {
                            _u8NumOf500msPGN = 11;
                        }
                        else if(_u8NumOf500msPGN > 11)
                        {
                            _u8NumOf500msPGN = 0;
                        }

                    }
                    else
                    {
                        if(_u8NumOf500msPGN >= 7)
                        {
                            _u8NumOf500msPGN = 0;
                        }
                    }
                }
                (void)R_IWDT_Refresh(&g_wdt0_ctrl);
            }
            UTILS_ResetTimer(&_Timer500ms);
        }

        if(( UTILS_GetElapsedTimeInMs(&_Timer1s) >= 1000))
        {
            for(uint8_t u8PGN=0; u8PGN < TX_LAST_PGN; u8PGN++)
            {
                u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
                if(u16PGN_State == PGN_1SEC)
                {
                    if((u8PGN == PGN_CM1) && ((CFGZ::ENG_KUBOTA == eEngineType) || (CFGZ::ENG_WECHAI == eEngineType)))
                    {
                        TransmitFrame(u8PGN);
                    }
                }
                (void)R_IWDT_Refresh(&g_wdt0_ctrl);
            }
            UTILS_ResetTimer(&_Timer1s);
        }

    }

//    if(UTILS_GetElapsedTimeInSec(&_CommFailTimeout) > _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_COMM_FAILURE_ACT_DELAY))
//    {
//        _bIsCANJ1939CommFail = true;
//        ResetLampsStatus();
//        _ArrPgnReadData[RX_PGN_EFL_P1_65263][2] = 0;
//        _ArrPgnReadData[RX_PGN_ET1_65262][0] = 0;
//        _ArrPgnReadData[RX_PGN_LFE_65266][0] = 0;
//        _ArrPgnReadData[RX_PGN_HOURS_65253][0] = 0;
//        _ArrPgnReadData[RX_PGN_VEP1_65271][0] = 0;
//        _ArrPgnReadData[RX_PGN_VEP1_65271][1] = 0;
//        _ArrPgnReadData[RX_PGN_EEC1_61444][2] = 0;
//
//    }

}

void J1939APP::TransmitFrame(uint8_t u8PGNNum)
{
    can_frame_t TxFrame;
    /// get the PGN information
    GetPGN(u8PGNNum, CALC_FOR_TRANSMIT, &tSupportPGN);

   if(_astPGNTxDataBuff[u8PGNNum].u8LockDataBuff)
    {
        /// load the transmit buffer with calculated data
        for(uint8_t i=0;i<8;i++)
        {
            TxFrame.data[i] = _astPGNTxDataBuff[u8PGNNum].au8TxDataBuffOld[i] ;
        }
    }
    else
    {
        /// load the transmit buffer with calculated data
        for(uint8_t i=0;i<8;i++)
        {
            TxFrame.data[i] =  _astPGNTxDataBuff[u8PGNNum].au8TxDataBuffUpdtd[i];
        }
    }

    /// load the frame id contents in CAN frame
    SetFrameID(&TxFrame);
    bTXBufferisEmpty = false;
    _hal.Objcan.SendData(&TxFrame);

}



double J1939APP::GetParamsFromJ1939(J1939APP::REALDATA_st edata)
{
    double f64Data=0;
    switch(edata)
    {
        case ENG_OIL_PRESSURE : f64Data=_ArrPgnReadData[RX_PGN_EFL_P1_65263][2];
         break;
        case ENG_COOLANT_TEMP : f64Data=_ArrPgnReadData[RX_PGN_ET1_65262][0];
         break;
        case FUEL_LEVEL       : f64Data=_ArrPgnReadData[RX_PGN_LFE_65266][0];
         break;
        case TOTAL_ENG_HOURS  : f64Data=_ArrPgnReadData[RX_PGN_HOURS_65253][0];
         break;
        case BATTERY_VOLTAGE  : f64Data=_ArrPgnReadData[RX_PGN_VEP1_65271][0];
         break;
        case INPUT_VOLTAGE    : f64Data=_ArrPgnReadData[RX_PGN_VEP1_65271][1];
         break;
        case ENG_SPEED        : f64Data=_ArrPgnReadData[RX_PGN_EEC1_61444][2];
         break;
    }

    return f64Data;
}


bool J1939APP::IsJ1939CommEnable(void)
{
    bool bStatus = false;

    if(_cfgz.GetEngType() != CFGZ::ENG_CONVENTIONAL)
    {
        bStatus = true;
    }
    return bStatus;
}

void J1939APP::SetDm1MsgCount(uint8_t u8Count)
{
    _u8NumOfDM1SPN = u8Count;
    _u8NoOfInvalidSpnInDm1Msg = u8Count;
}

uint8_t J1939APP::GetDm1MsgCount(void)
{
    /* return only valid SPNs */
    return (_u8NumOfDM1SPN - _u8NoOfInvalidSpnInDm1Msg);
}

void J1939APP::SetDm2MsgCount(uint8_t u8Count)
{
    _u8NumOfDM2SPN = u8Count;
    _u8NoOfInvalidSpnInDm2Msg = u8Count;
}

uint8_t J1939APP::GetDm2MsgCount(void)
{
    /* return only valid SPNs */
    return (_u8NumOfDM2SPN - _u8NoOfInvalidSpnInDm2Msg);
}

uint8_t J1939APP::GetPCDAlarmCount(void)
{
    uint8_t u8index= 0;
    uint8_t u8NoOfPCDAlarms = 0;
    for(u8index = PCD_REMOVAL_OF_DPF_SYSTEM; u8index <= PCD_FAILURE_OF_PCD_SYSTEM; u8index++)
    {
        if((uint8_t)_ArrPgnReadData[RX_PGN_PROPB51_65361][u8index] == 1)
        {
            u8NoOfPCDAlarms++;
        }
    }
    return u8NoOfPCDAlarms;
}


uint8_t J1939APP::GetNCDAlarmCount(void)
{
    uint8_t u8index= 0;
    uint8_t u8NoOfNCDAlarms = 0;
    for(u8index = NCD_REMOVAL_OF_EGR_SYSTEM; u8index <= NCD_REMOVAL_OF_MAF_SENSOR; u8index++)
    {
        if((uint8_t)_ArrPgnReadData[RX_PGN_PROPB51_65361][u8index] == 1)
        {
            u8NoOfNCDAlarms++;
        }
    }
    return u8NoOfNCDAlarms;
}


 void J1939APP::UpdateDM2RequestData(uint8_t *au8CalSPNData)
{
    *au8CalSPNData = 0xCB;
    /* <LDRA Phase code 9S> <Assignment operation in expression.: Resolved.>
     * <Verified by: Nikhil Mhaske> <26/03/2020> */
    au8CalSPNData++;
    *au8CalSPNData   = 0xFE;
}

 void J1939APP::UpdateDM11RequestData(uint8_t *au8CalSPNData)
{
    *au8CalSPNData = 0xD3;
    /* <LDRA Phase code 9S> <Assignment operation in expression.: Resolved.>
     * <Verified by: Nikhil Mhaske> <26/03/2020> */
    au8CalSPNData++;
    *au8CalSPNData   = 0xFE;
}


void J1939APP::CalculateTsc1Checksum(void)
{
    unsigned int uiLocal = 0;
    uint8_t *pTxBuff = NULL;
    uint8_t u8Checksum=0;

    if(_astPGNTxDataBuff[PGN_TSC1].u8LockDataBuff)
    {
        pTxBuff = (uint8_t*)&_astPGNTxDataBuff[PGN_TSC1].au8TxDataBuffOld;
    }
    else
    {
        pTxBuff = (uint8_t*)&_astPGNTxDataBuff[PGN_TSC1].au8TxDataBuffUpdtd;
    }

    for (uiLocal = 0 ; uiLocal < 7 ; uiLocal++)
    {
        u8Checksum += *pTxBuff;
        pTxBuff++;
    }

    _u32TSC1CANMessageID = (uint32_t)((tSupportPGN.ubyPDU_Prio << PDU_Prio_Mask_Shift)& PDU_PRIO_Mask);
    _u32TSC1CANMessageID |= ((((uint32_t)tSupportPGN.u8PF )<< PDU_PF_Mask_Shift)& PDU_PF_Mask);
    _u32TSC1CANMessageID |= ((((uint32_t) tSupportPGN.u8PS) << PDU_PS_Mask_Shift) & PDU_PS_Mask);
    _u32TSC1CANMessageID |= (uint32_t) ((tSupportPGN.ubyPDU_SA & PDU_SA_Mask));

    /* Message checksum is calculated using first 7 bytes, ,message counter and message identifier.
     * Checksum = ( byte_1 + byte_2 + byte_3 + byte_4 + byte_5 + byte_6+ byte_7 + (message_counter & 0x0F)
     *             + Msg_ID_low_byte + Msg_ID_mid_low_byte + Msg_ID_mid_high_byte + Msg_ID_high_byte)
     * Message Checksum = (((Checksum >> 6) & 0x03) + (Checksum >> 3) + Checksum) & 0x07             */

    u8Checksum = (uint8_t)(u8Checksum + (_u8PGN0MessageCounter & 0x0F) + (uint8_t)(_u32TSC1CANMessageID & 0x000000FF) +
                  (uint8_t)((_u32TSC1CANMessageID & 0x0000FF00) >> 8) + (uint8_t)((_u32TSC1CANMessageID & 0x00FF0000) >> 16)
                  + (uint8_t)((_u32TSC1CANMessageID & 0xFF000000) >> 24));

    _f32Pgn0Data[PGN_0_MESSAGE_CHECKSUM] =(float)((((u8Checksum >> 6) & 0x03) + (u8Checksum >> 3) + u8Checksum) & 0x07);
}



void J1939APP::CalculatePropB_F7Checksum()
{
    unsigned int uiLocal = 0;
    uint8_t *pTxBuff = NULL;
    uint8_t u8ChecksumPGNFFF7 = 0;

    GetPGN(PGN_PROPB_F7, CALC_FOR_TRANSMIT, &tSupportPGN);

    if(_astPGNTxDataBuff[PGN_PROPB_F7].u8LockDataBuff)
    {
        pTxBuff = (uint8_t*)&_astPGNTxDataBuff[PGN_PROPB_F7].au8TxDataBuffOld;
    }
    else
    {
        pTxBuff = (uint8_t*)&_astPGNTxDataBuff[PGN_PROPB_F7].au8TxDataBuffUpdtd;
    }

    for (uiLocal = 0 ; uiLocal < 7 ; uiLocal++)
    {
        u8ChecksumPGNFFF7  += *pTxBuff;
        pTxBuff++;
    }

    _u32PropBF7CANMessageID = (uint32_t)((tSupportPGN.ubyPDU_Prio << PDU_Prio_Mask_Shift)& PDU_PRIO_Mask);
    _u32PropBF7CANMessageID |= ((((uint32_t)tSupportPGN.u8PF )<< PDU_PF_Mask_Shift)& PDU_PF_Mask);
    _u32PropBF7CANMessageID |= ((((uint32_t) tSupportPGN.u8PS) << PDU_PS_Mask_Shift) & PDU_PS_Mask);
    _u32PropBF7CANMessageID |= (uint32_t) ((tSupportPGN.ubyPDU_SA & PDU_SA_Mask));

    /* Message checksum is calculated using first 7 bytes, ,message counter and message identifier.
     * Checksum = ( byte_1 + byte_2 + byte_3 + byte_4 + byte_5 + byte_6+ byte_7 + (message_counter & 0x0F)
     *             + Msg_ID_low_byte + Msg_ID_mid_low_byte + Msg_ID_mid_high_byte + Msg_ID_high_byte)
     * Message Checksum = (((Checksum >> 6) & 0x03) + (Checksum >> 3) + Checksum) & 0x07             */

    u8ChecksumPGNFFF7 = (uint8_t)(u8ChecksumPGNFFF7 + (_u8PGN0xFFF7MessageCounter & 0x0F) + (uint8_t)(_u32PropBF7CANMessageID & 0x000000FF) +
            (uint8_t)((_u32PropBF7CANMessageID & 0x0000FF00) >> 8) + (uint8_t)((_u32PropBF7CANMessageID & 0x00FF0000) >> 16)
            + (uint8_t)((_u32PropBF7CANMessageID & 0xFF000000) >> 24));

    _f32Pgn65527Data[PGN_0_MESSAGE_CHECKSUM] = (float)((((u8ChecksumPGNFFF7 >> 6) & 0x03) + (u8ChecksumPGNFFF7 >> 3) + u8ChecksumPGNFFF7) & 0x07);
}

void  J1939APP::prvExtractDmMsg(uint16_t u16DmMsgNo, uint8_t u8NoOfSpnInDmMsg)
{
    uint8_t u8Local1 = 0;
    uint8_t u8Local2 = 0;
    uint32_t u32LocalDTC = 0;
    uint8_t u8Cnt = 0;
    uint8_t u8LocalCounterDM1 = 0;
    uint8_t u8LocalCounterDM2 = 0;

    typedef struct
    {
        uint16_t ubitProtectLamp:2,
        ubitAmbarLamp:2,
        ubitRedLamp:2,
        ubitMalFunctionLamp:2,
        ubitunused:8;
    }TagDM1LampStatus;
     union
    {
        TagDM1LampStatus DM1LampStatus;
        uint8_t u8Data[2];
    } DM1Lamp;
    typedef struct
    {
        uint32_t u16SPN:16,
        u8FMI:5,
        ubitSPN:3,
        u8Ocuurenece:7,
        uReserved:1;
    }TagDM1DTC;

    union
    {
        TagDM1DTC DM1DTC;
        uint8_t u8Data[4];
    } DTCSPN;

    DM1Lamp.u8Data[0]= _u8ArrDM1SPNData[u8Cnt++];
    DM1Lamp.u8Data[1]= _u8ArrDM1SPNData[u8Cnt++];

    if(PGN_DM01 == u16DmMsgNo)
    {
        // Amber lamp
        if(DM1Lamp.DM1LampStatus.ubitAmbarLamp == 1)
        {
            _u8ActiveDtcAmberLampStatus = true;
        }
        else
        {
            _u8ActiveDtcAmberLampStatus = false;
        }

        // Red Lamp
        if(DM1Lamp.DM1LampStatus.ubitRedLamp == 1)
        {
            _u8ActiveDtcRedLampStatus = true;
        }
        else
        {
            _u8ActiveDtcRedLampStatus = false;
        }

        // MIL Lamp
        if(DM1Lamp.DM1LampStatus.ubitMalFunctionLamp == 1)
        {
            _u8ActiveDtcMILLampStatus = true;
        }
        else
        {
            _u8ActiveDtcMILLampStatus = false;
        }

        // Protect Lamp
        if(DM1Lamp.DM1LampStatus.ubitProtectLamp == 1)
        {
            _u8ActiveDtcProtectLampStatus = true;
        }
        else
        {
            _u8ActiveDtcProtectLampStatus = false;
        }

        _bIsDM1PGNRecieved = true;
        _u8NoOfInvalidSpnInDm1Msg = 0;
    }
    else    // if PGN_DM02
    {
        _u8NoOfInvalidSpnInDm2Msg = 0;
    }




    /* DM screen is from 0th index */
    for(u8Local1 = 0; u8Local1 < u8NoOfSpnInDmMsg; u8Local1++)
    {
        if(PGN_DM01 == u16DmMsgNo)
        {
            for(u8Local2=0; u8Local2 < 4; u8Local2++)
            {
                DTCSPN.u8Data[u8Local2] = _u8ArrDM1SPNData[u8Cnt++];
            }
            u32LocalDTC = (((uint16_t)DTCSPN.DM1DTC.ubitSPN) << 16) | DTCSPN.DM1DTC.u16SPN;
            /* Assign the DM value only if DTC is greater than "0" and less than or equal to "5,24,287(0x7FFFF)" */
            if((u32LocalDTC > 0) && (u32LocalDTC < 0x7FFFF))
            {
                prvAssignDmMsgVal(&_stDm1Decode[u8LocalCounterDM1], u32LocalDTC,(uint8_t)DTCSPN.DM1DTC.u8FMI, (uint8_t)DTCSPN.DM1DTC.u8Ocuurenece);
                u8LocalCounterDM1++;
            }
            else
            {
                /* u8NoOfInvalidSpnInDm1Msg is the number of SPN's in the DM1 packet whose SPN
                 * number is 0 or greater than or equal to 0x7FFFF.*/
                _u8NoOfInvalidSpnInDm1Msg++;
            }
        }
        else if(PGN_DM02 == u16DmMsgNo)
        {
            for(u8Local2=0; u8Local2 < 4; u8Local2++)
            {
                DTCSPN.u8Data[u8Local2] = _u8ArrDM2SPNData[u8Cnt++];
            }
            u32LocalDTC = (((uint16_t)DTCSPN.DM1DTC.ubitSPN) << 16) | DTCSPN.DM1DTC.u16SPN;
            if((u32LocalDTC > 0) && (u32LocalDTC < 0x7FFFF))
            {
                prvAssignDmMsgVal(&_stDm2Decode[u8LocalCounterDM2], u32LocalDTC,(uint8_t)DTCSPN.DM1DTC.u8FMI, (uint8_t)DTCSPN.DM1DTC.u8Ocuurenece);
                u8LocalCounterDM2++;
            }
            else
            {
                /* u8NoOfInvalidSpnInDm2Msg is the number of SPN's in the DM2 packet whose SPN
                 * number is 0 or greater than or equal to 0x7FFFF.*/
                _u8NoOfInvalidSpnInDm2Msg++;
            }
        }
    }
}


void J1939APP::prvAssignDmMsgVal(J1939APP::J1939_DM_MSG_DECODE* pstDmMsgArray, uint32_t u32LocalDM1SPN, uint8_t u8LocalFMI, uint8_t u8LocalOC)
{
    pstDmMsgArray->u32SpnNo = u32LocalDM1SPN;
    pstDmMsgArray->u8FMI = u8LocalFMI;
    pstDmMsgArray->u8OC = u8LocalOC;
}

uint32_t J1939APP::prvExtractSPN(uint8_t *pCANData, uint8_t ubyPGN, uint8_t ubySPN, uint16_t ubyStartBit,uint8_t ubyNumberOfBit)
{
    uint8_t ubyStartByte = 0;
    uint8_t ubyTemp1 = 0;
    uint8_t ubyTemp2 = 0;
    uint32_t ulData = 0;
    uint32_t ulMask = 0;
    uint32_t ulErrorCheckValue = 0xFB;
    uint32_t ulNotAvailableCheckVal = 0xFF;

    ubyStartByte = (uint8_t) (ubyStartBit / 8);                          // Calculate the byte number in which data is present
    ubyTemp1 = (ubyStartBit % 8);

    // Remove the bits from start byte which are not is that SPN
    ubyTemp2 = pCANData[ubyStartByte] >> ubyTemp1;

    ulMask = (1U << ubyNumberOfBit) - 1U;

    if( (ubyNumberOfBit <= 8)&& ((ubyNumberOfBit + ubyTemp1) < 16) )
    {
        ulData = (uint32_t)pCANData[ubyStartByte+1] << (8-ubyTemp1);
        if( !((ubyPGN == RX_PGN_EEC1_61444) && (ubySPN == 3)) )  // EEC1 Source address SPN
        {
            if(ulData == 0xFF)
            {
                _ArrSpnErrorNAStatus[ubyPGN][ubySPN] = NOT_AVAILABLE;
            }
            else if(ulData >= 0xFB)
            {
                _ArrSpnErrorNAStatus[ubyPGN][ubySPN] = ERROR;
            }
            else
            {
                _ArrSpnErrorNAStatus[ubyPGN][ubySPN] = VALID_DATA;
            }
        }
        else
        {
            _ArrSpnErrorNAStatus[ubyPGN][ubySPN] = VALID_DATA;
        }
    }
    else if( (ubyNumberOfBit <= 16) && ((ubyNumberOfBit + ubyTemp1) < 24) )
    {
        ulData = (uint32_t )pCANData[ubyStartByte+2] << (16-ubyTemp1);
        ulErrorCheckValue = ulErrorCheckValue << 8;
        ulNotAvailableCheckVal= ulNotAvailableCheckVal << 8;
    }
    else if( (ubyNumberOfBit <= 24) && ((ubyNumberOfBit + ubyTemp1) < 32) )
    {
        ulData = (uint32_t )pCANData[ubyStartByte+3] << (24-ubyTemp1);
        ulData = ulData | (uint32_t )pCANData[ubyStartByte+2] << (16-ubyTemp1);
        ulErrorCheckValue = ulErrorCheckValue << 16;
        ulNotAvailableCheckVal= ulNotAvailableCheckVal << 16;
    }
    else if((ubyNumberOfBit <= 32)&& (ubyNumberOfBit+ ubyTemp1) < 64)
    {
        ulData = (uint32_t )pCANData[ubyStartByte+2]<<(32-ubyTemp1);
        ulData = ulData|(uint32_t )pCANData[ubyStartByte+3] << (24-ubyTemp1);
        ulData = ulData|(uint32_t )pCANData[ubyStartByte+2] << (16-ubyTemp1);
        ulErrorCheckValue = ulErrorCheckValue << 24;
        ulNotAvailableCheckVal = ulNotAvailableCheckVal << 24;
    }

    ulData = ulData|((uint32_t )pCANData[ubyStartByte+1] << (8-ubyTemp1) | ubyTemp2);
    ulData = ulMask & ulData;

    if(ulData >= ulNotAvailableCheckVal)
    {
        _ArrSpnErrorNAStatus[ubyPGN][ubySPN] = NOT_AVAILABLE;
    }
    else if(ulData >= ulErrorCheckValue)
    {
        _ArrSpnErrorNAStatus[ubyPGN][ubySPN] = ERROR;
    }
    else
    {
        _ArrSpnErrorNAStatus[ubyPGN][ubySPN] = VALID_DATA;
    }
    return(ulData);
}

bool J1939APP::IsCommunicationFail(void)
{
    return _bIsCANJ1939CommFail;
}

bool J1939APP::IsDM1PGNRecieved(void)
{
    return _bIsDM1PGNRecieved;
}

bool J1939APP::GetLampStatus(LAMP_st eLamp)
{

    bool bStatus =false;
    switch(eLamp)
    {
        case RED_LAMP    :
            if(_u8ActiveDtcRedLampStatus)
            {
                bStatus = true;
            }
            break;
        case AMBER_LAMP  :
            if(_u8ActiveDtcAmberLampStatus)
             {
                 bStatus = true;
             }
            break;
        case MIL_LAMP    :
            if(_u8ActiveDtcMILLampStatus)
             {
                 bStatus = true;
             }
            break;
        case PROTECT_LAMP:
            if(_u8ActiveDtcProtectLampStatus)
             {
                 bStatus = true;
             }
            break;
        default: break;
    }

    return bStatus;
}

double J1939APP::GetReadData(DATABASE_RX_PGN_LIST_t ePgn, uint8_t u8SpnNum)
{
    return _ArrPgnReadData[(uint8_t)ePgn][u8SpnNum];
}

J1939APP::J1939_DM_MSG_DECODE J1939APP::GetDM1Message(uint8_t u8MessageNum)
{
    return _stDm1Decode[u8MessageNum];
}

J1939APP::J1939_DM_MSG_DECODE J1939APP::GetDM2Message(uint8_t u8MessageNum)
{
    return _stDm2Decode[u8MessageNum];
}

uint8_t J1939APP::GetSPNErrorStatus(uint8_t u8PGNName, uint8_t u8SPNName)
{
    return _ArrSpnErrorNAStatus[u8PGNName][u8SPNName];
}

void J1939APP::ResetLampsStatus(void)
{
    _u8ActiveDtcAmberLampStatus = 0;
    _u8ActiveDtcRedLampStatus = 0;
    _u8ActiveDtcMILLampStatus = 0;
    _u8ActiveDtcProtectLampStatus = 0;
}


/* PGN-65395(CUMMINS) : CUMMINS Engine Genset Control parameters */
 void J1939APP::prvUpdatePGN65395Data(void)
{

   _f32Pgn65395Data[PGN_65395_UNUSED_SPN_1] = 0;

   _f32Pgn65395Data[PGN_65395_CUMMINS_GAIN_SELECT] = START_STOP::IsFuelRelayOn();

//   _f32Pgn65395Data[PGN_65395_CUMMINS_IDLE_SPEED] = START_STOP::IsIdleModeActive();
   _f32Pgn65395Data[PGN_65395_CUMMINS_STARTER_SELECTION] = 3;
   _f32Pgn65395Data[PGN_65395_CUMMINS_SHUTDOWN_OVERRIDE] = 0;
   /*As per DEIF's requirement we should have drop down menu in ECU submenu to select the frequency(50Hz or 60Hz) Since we dont have it.
    * Here we consider if Eng requested speed is more than 1650 then 60Hz else 50Hz. Needs to be reviewed.*/

   _f32Pgn65395Data[PGN_65395_CUMMINS_FREQUENCY_RANGE] = 0;//_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_FRQ);


   _f32Pgn65395Data[PGN_65395_UNUSED_SPN_2] = 32768;
   _f32Pgn65395Data[PGN_65395_UNUSED_SPN_3] = 0;
   _f32Pgn65395Data[PGN_65395_UNUSED_SPN_4] = 0;

}
/* PGN-65406(CUMMINS) : CUMMINS Governing Adjustment Parameters */
 void J1939APP::prvUpdatePGN65406Data(void)
{
   _f32Pgn65406Data[PGN_65406_CUMMINS_DROOP] = 0;
   _f32Pgn65406Data[PGN_65406_CUMMINS_RES] = 0;
   _f32Pgn65406Data[PGN_65406_CUMMINS_FREQ_ADJUSTMENT] = 0;
   _f32Pgn65406Data[PGN_65406_CUMMINS_GAIN] = 0;//_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_GAIN);
}

 /* PGN-61441 : EBC1 Engine Brake Controller*/
 void J1939APP::prvUpdatePGN61441Data(void)
 {
     _f32Pgn61441Data[PGN_61441_UNUSED_SPN_1] = 0xFF;
     _f32Pgn61441Data[PGN_61441_UNUSED_SPN_2] = 0xFFFF;
     _f32Pgn61441Data[PGN_61441_UNUSED_SPN_3] = 0xF;

     if(START_STOP::IsFuelRelayOn())
     {
         _f32Pgn61441Data[PGN_61441_ENG_AUX_SHUTDOWN_SW] = 0;
     }
     else
     {
         _f32Pgn61441Data[PGN_61441_ENG_AUX_SHUTDOWN_SW] = 1;
     }

     _f32Pgn61441Data[PGN_61441_UNUSED_SPN_4] = 0x3;
     _f32Pgn61441Data[PGN_61441_UNUSED_SPN_5] = 0xFFFF;
     _f32Pgn61441Data[PGN_61441_UNUSED_SPN_6] = 0xFFFF;

 }
 /* PGN 65223 ETC3 - PERKINS Engine */
 void J1939APP::prvUpdatePGN65223Data(void)
 {
     _f32Pgn65223Data[PGN_65223_DEFUEL_ACT] = !START_STOP::IsFuelRelayOn();
     _f32Pgn65223Data[PGN_65223_UNUSED_SPN_1] = 0xFFFF;
     _f32Pgn65223Data[PGN_65223_UNUSED_SPN_2] = 0xFFFF;
     _f32Pgn65223Data[PGN_65223_UNUSED_SPN_3] = 0xFF;
     _f32Pgn65223Data[PGN_65223_UNUSED_SPN_4] = 0xF;
     _f32Pgn65223Data[PGN_65223_UNUSED_SPN_5] = 3;
     _f32Pgn65223Data[PGN_65223_UNUSED_SPN_6] = 0xFFFF;
 }

void J1939APP::prvHandleEngineStartStop()
{
    static START_STOP::SS_STATE_t ePrvState = START_STOP::ID_STATE_SS_ENG_OFF_OK;
    bool bStateChanged =false;
    if(ePrvState !=START_STOP::GetStartStopSMDState())
    {
        bStateChanged =true;
        ePrvState = START_STOP::GetStartStopSMDState();
    }

    switch(START_STOP::GetStartStopSMDState())
    {
        case START_STOP::ID_STATE_SS_ENG_OFF_OK:
            _bEngineStartPreheatStatus = true;
            if(_bPreheatFailed && (_cfgz.GetEngType()== CFGZ::ENG_IVECO))
            {
                _bPreheatFailed = false;
            }
            break;
        case START_STOP::ID_STATE_SS_ENG_OFF_ERR:
            break;
        case START_STOP::ID_STATE_SS_PREHEAT:
            if((_cfgz.GetEngType() ==CFGZ::ENG_IVECO) && ((uint8_t)_ArrPgnReadData[RX_PGN_IVECO_PREHEAT_65281][6] == 0))
            {
                _bEngineStartPreheatStatus = false;
            }
            break;
        case START_STOP::ID_STATE_SS_START_WAIT:
            if(bStateChanged)
            {
                bStateChanged = false;
                if( (CFGZ::ENG_VOLVO == _cfgz.GetEngType()) && (IS_PREHEAT_J1939_CONFIGURED()) )
                {
                    _ArrPgnReadData[RX_PGN_VOLVO_PREHEAT_65351][0] = PREHEAT_COMPLETED;
                }
                else if( (CFGZ::ENG_DEUTZ_EMR == _cfgz.GetEngType()) && (IS_PREHEAT_J1939_CONFIGURED()) )
                {
                    _ArrPgnReadData[RX_PGN_EMR_PREHEAT_65284][0] = PREHEAT_NA;
                }
                else if( (CFGZ::ENG_IVECO == _cfgz.GetEngType()) && (IS_PREHEAT_J1939_CONFIGURED()) )
                {
                    _ArrPgnReadData[RX_PGN_IVECO_PREHEAT_65281][6] = PREHEAT_IN_PROGRESS;
                }
            }
            break;
        case START_STOP::ID_STATE_SS_CRANKING:
            break;
        case START_STOP::ID_STATE_SS_CRANK_REST:
            break;
        case START_STOP::ID_STATE_SS_ENG_ON:
            break;
        case START_STOP::ID_STATE_SS_STOPPING:
            _bEngineStartPreheatStatus = true;
            if(bStateChanged)
            {

            }
            break;
        case START_STOP::ID_STATE_SS_STOP_HOLD:

            break;
        case START_STOP::ID_STATE_SS_FAIL_TO_STOP:
            break;

    }
}

void J1939APP::prvCalculateAccPedalPosChecksum(void)
{
   uint16_t u16AccPedalPosChecksum = 0;
   const uint16_t  u16ConstPedalPos=0x1ff; //ie. 50%
   u16AccPedalPosChecksum = (uint8_t)(u16ConstPedalPos & 0x00FF) + (uint8_t)((u16ConstPedalPos & 0xFF00)>>8) + (_u8AccPedalCounter & 0x0F);
   _f32Pgn65350Data[PGN_65350_ACCELERATOR_PEDAL_CHECKSUM] = (float)((((u16AccPedalPosChecksum >> 6) & 0x03) + (u16AccPedalPosChecksum >> 3) + u16AccPedalPosChecksum) & 0x07);

}

uint8_t J1939APP::IsRedLampON()
{
    return _u8ActiveDtcRedLampStatus;
}

uint8_t J1939APP::IsAmberLampON()
{
    return _u8ActiveDtcAmberLampStatus;
}

uint8_t J1939APP::IsMilLampON()
{
    return _u8ActiveDtcMILLampStatus;
}

uint8_t J1939APP::IsProtectLampON()
{
    return _u8ActiveDtcProtectLampStatus;
}

void J1939APP::RequestDM11PGN()
{
    _bRequestDM11PGN = true;
}

void J1939APP::RequestDM2Messages()
{
    _bRequestDM2PGN = true;
}
void J1939APP::ClearDM2Messages()
{
    memset((void *)&_stDm2Decode,0, sizeof(_stDm2Decode));
}

void J1939APP::ClearNCDandPCDAlarms()
{
    for(uint8_t u8index = PCD_REMOVAL_OF_DPF_SYSTEM; u8index <= NCD_REMOVAL_OF_MAF_SENSOR; u8index++)
      {
         _ArrPgnReadData[RX_PGN_PROPB51_65361][u8index] =0;

      }
}
