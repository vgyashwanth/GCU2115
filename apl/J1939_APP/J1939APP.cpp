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
static const float F32_Null = (float)0xFFFFFFFF;
bool J1939APP::bEnableEngineStart      = false;
bool J1939APP::bExecutePreheat         = false;


J1939APP::J1939APP(HAL_Manager &hal, CFGC &cfgc, CFGZ &cfgz,ENGINE_MONITORING &engineMonitoring, GCU_ALARMS &gcuAlarm,
        MB_APP &mbApp, AUTO_MODE  &Automode /*, EGOV &egov */):
J1939DRIVER(hal),
_hal(hal),
_ObjCfgc(cfgc),
_cfgz(cfgz),
_engMon(engineMonitoring),
_gcuAlarm(gcuAlarm),
_ObjmbApp(mbApp),
_Automode(Automode),
ubypReadTxPgns
{
        (J1939_PGNs)&(gstPGNs.PGNUtilityPhABasicACQty),
        (J1939_PGNs)&(gstPGNs.PGNUtilityPhBBasicACQty),
        (J1939_PGNs)&(gstPGNs.PGNUtilityPhCACBasicQty),
        (J1939_PGNs)&(gstPGNs.PGNGenAvgBasicACQty),
        (J1939_PGNs)&(gstPGNs.PGNGenPhABasicQty),
        (J1939_PGNs)&(gstPGNs.PGNGenPhAACPower),
        (J1939_PGNs)&(gstPGNs.PGNGenPhAACReactPow),
        (J1939_PGNs)&(gstPGNs.PGNGenPhBBasicQty),
        (J1939_PGNs)&(gstPGNs.PGNGenPhBACPow),
        (J1939_PGNs)&(gstPGNs.PGNGenPhBACReactPow),
        (J1939_PGNs)&(gstPGNs.PGNGenPhCBasicQty),
        (J1939_PGNs)&(gstPGNs.PGNGenPhCACPow),
        (J1939_PGNs)&(gstPGNs.PGNGenPhCACReactPow),
        (J1939_PGNs)&(gstPGNs.PGNGenTotalACActiveEnergy),
        (J1939_PGNs)&(gstPGNs.PGNGenTotalACPow),
        (J1939_PGNs)&(gstPGNs.PGNGenTotalACReactPow),
        (J1939_PGNs)&(gstPGNs.PGNGenTotlaACPercentPow),
        (J1939_PGNs)&(gstPGNs.PGNSedemacProprietary),
        (J1939_PGNs)&(gstPGNs.PGNPropStatusCnts),
        (J1939_PGNs)&(gstPGNs.PGNRunHrs),
        (J1939_PGNs)&(gstPGNs.PGNAlarms1_4),
        (J1939_PGNs)&(gstPGNs.PGNAlarms5_8),
        (J1939_PGNs)&(gstPGNs.PGNAlarms9_12),
        (J1939_PGNs)&(gstPGNs.PGNPrcntgLdFuelCA),
        (J1939_PGNs)&(gstPGNs.PGNCumltvDGKwhKvah),
        (J1939_PGNs)&(gstPGNs.PGNCumltvDGKvarh),
        (J1939_PGNs)&(gstPGNs.PGN_RQST_59904),
},

ubypReadRxPgns{

    (J1939_PGNs)&(gstPGNs.PGN_DPFC1_64892 ),
    (J1939_PGNs)&(gstPGNs.PGN_AT1T1I_65110 ),
    (J1939_PGNs)&(gstPGNs.PGN_EEC1_61444 ),
    (J1939_PGNs)&(gstPGNs.PGN_EEC2_61443 ),
    (J1939_PGNs)&(gstPGNs.PGN_EOI_64914 ),
    (J1939_PGNs)&(gstPGNs.PGN_HOURS_65253 ),
    (J1939_PGNs)&(gstPGNs.PGN_LFC1_65257 ),
    (J1939_PGNs)&(gstPGNs.PGN_ET1_65262 ),
    (J1939_PGNs)&(gstPGNs.PGN_EFL_P1_65263 ),
    (J1939_PGNs)&(gstPGNs.PGN_AMB_65269 ),
    (J1939_PGNs)&(gstPGNs.PGN_VEP1_65271 ),
    (J1939_PGNs)&(gstPGNs.PGN_WFI_OI_65279 ),
    (J1939_PGNs)&(gstPGNs.PGN_DEFA_65383 ),
    (J1939_PGNs)&(gstPGNs.PGN_SHUTDN_65252 ),
    (J1939_PGNs)&(gstPGNs.PGN_CSA_64966),
    (J1939_PGNs)&(gstPGNs.PGN_DM11_65235 ),
    (J1939_PGNs)&(gstPGNs.PGN_DM03_65228 ),
    (J1939_PGNs)&(gstPGNs.PGN_IC1_65270  ),
    (J1939_PGNs)&(gstPGNs.PGN_LFE1_65266  )

},

    f32PGN_65014Data{0},
    f32PGN_65011Data{0},
    f32PGN_65008Data{0},
    f32PGN_65030Data{0},
    f32PGN_65027Data{0},
    f32PGN_65026Data{0},
    f32PGN_65025Data{0},
    f32PGN_65024Data{0},
    f32PGN_65023Data{0},
    f32PGN_65022Data{0},
    f32PGN_65021Data{0},
    f32PGN_65020Data{0},
    f32PGN_65019Data{0},
    f32PGN_65018Data{0},
    f32PGN_65029Data{0},
    f32PGN_65028Data{0},
    f32PGN_64911Data{0},
    f32PGN_65280Data{0},
    f32PGN_65289Data{0},
    f32PGN_65290Data{0},
    f32PGN_65291Data{0},
    f32PGN_65292Data{0},
    f32PGN_65293Data{0},
    f32PGN_65295Data{0},
    f32PGN_65296Data{0},
    f32PGN_65297Data{0},
    _au8CalcSPNData{0},
    _astPGNTxDataBuff{},
    _u32RequestedPGN(0),
    _u8PGN_Num(0),
    _u8NumOfDM1SPN(0),
    _u8NumOfDM2SPN(0),
    _u8NoOfInvalidSpnInDm1Msg(0),
    _u8NoOfInvalidSpnInDm2Msg(0),
    _ConfigParam{},
    _bDeviceInConfigMode(false),
    _Timer10ms{0, false},
    _Timer20ms{0, false},
    _Timer50ms{0, false},
    _Timer80ms{0, false},
    _Timer100ms{0, false},
    _Timer250ms{0, false},
    _Timer300ms{0, false},
    _Timer500ms{0, false},
    _Timer1s{0, false},
    _CommFailTimeout{0, false},
    _u8ArrDM1SPNData{0},
    _u8ArrDM2SPNData{0},
    _u8SPNIndexInRQSTPGN(0),
    _ArrPgnReadData{0},
    _u8ActiveDtcAmberLampStatus(0),
    _u8ActiveDtcProtectLampStatus(0),
    _u8ActiveDtcRedLampStatus(0),
    _u8ActiveDtcMILLampStatus (0),
    _bIsDM1PGNRecieved(false),
    _bIsCANJ1939CommFail(false),
    _bClearActiveDTCs(false),
    _bRequestDM2PGN(false),
    _bRequestDM11PGN(false),
    _bIsDEFLevelLow(false),
    _bIsDEFLevelSevere(false),
    _BeepOnTimer{0 , false},
    _BeepOffTimer{0,false},
    _stDm1Decode{0,0,0},
    _stDm2Decode{0,0,0}
{

    UTILS_ResetTimer(&_Timer100ms);
    UTILS_ResetTimer(&_Timer250ms);
    UTILS_ResetTimer(&_Timer300ms);
    UTILS_ResetTimer(&_Timer500ms);
    UTILS_ResetTimer(&_Timer1s);
    UTILS_ResetTimer(&_Timer20ms);
    UTILS_ResetTimer(&_Timer10ms);
    UTILS_ResetTimer(&_Timer50ms);
    UTILS_ResetTimer(&_Timer80ms);
    UTILS_ResetTimer(&_CommFailTimeout);
    InitAfterConfigChange();

}


void J1939APP::InitAfterConfigChange()
{
    ResetLampsStatus();
    ClearAllPGNsDataBuffs();
    uint8_t gu8PGNNumber, gu8SPNNumber;
    for( gu8PGNNumber = 0; gu8PGNNumber < RX_PGN_LAST; gu8PGNNumber++)
    {
        for(gu8SPNNumber = 0; gu8SPNNumber < MAX_NO_SPN_IN_PGN; gu8SPNNumber++)
        {
            _ArrSpnErrorNAStatus[gu8PGNNumber][gu8SPNNumber] = NOT_AVAILABLE;
            (void)memset((void *)(&_ArrPgnReadData[gu8PGNNumber][gu8SPNNumber]), (int)0xFFFFFFFFF, sizeof(double));
        }
    }
}

void J1939APP::ClearAllPGNsDataBuffs(void)
{
    uint8_t u8Local;

    for(u8Local=0; u8Local<4; u8Local++)
    {
        f32PGN_65014Data[u8Local] = F32_Null;
        f32PGN_65011Data[u8Local]= F32_Null;
        f32PGN_65008Data[u8Local] = F32_Null;
        f32PGN_65030Data[u8Local] = F32_Null;
        f32PGN_65027Data[u8Local] = F32_Null;
        f32PGN_65024Data[u8Local] = F32_Null;
        f32PGN_65021Data[u8Local] = F32_Null;
        f32PGN_65290Data[u8Local] = F32_Null;
        f32PGN_65295Data[u8Local] = F32_Null;

    }

    for(u8Local=0; u8Local<2; u8Local++)
    {
        f32PGN_65026Data[u8Local] = F32_Null;
        f32PGN_65023Data[u8Local] = F32_Null;
        f32PGN_65020Data[u8Local] = F32_Null;
        f32PGN_65018Data[u8Local] = F32_Null;
        f32PGN_65029Data[u8Local] = F32_Null;
        f32PGN_65296Data[u8Local] = F32_Null;
        f32PGN_65297Data[u8Local] = F32_Null;
    }

    for(u8Local=0; u8Local<3; u8Local++)
    {
        f32PGN_65025Data[u8Local] = F32_Null;
        f32PGN_65022Data[u8Local] = F32_Null;
        f32PGN_65019Data[u8Local] = F32_Null;
        f32PGN_65028Data[u8Local] = F32_Null;
        f32PGN_64911Data[u8Local] = F32_Null;
    }

    for(u8Local=0; u8Local<5; u8Local++)
    {
        f32PGN_65280Data[u8Local] = F32_Null;
    }

    for(u8Local=0; u8Local<16; u8Local++)
    {
        f32PGN_65291Data[u8Local] = F32_Null;
        f32PGN_65292Data[u8Local] = F32_Null;
        f32PGN_65293Data[u8Local] = F32_Null;
    }

    for(u8Local=0; u8Local<32; u8Local++)
    {
        f32PGN_65289Data[u8Local] = F32_Null;
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
            if(u8RxOrTx == CALC_FOR_TRANSMIT)
        {
            pstGetPGN->ubyPDU_SA = (uint8_t)_cfgz.GetCFGZ_Param(CFGZ::ID_SGC_SOURCE_ADDRESS);
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


void J1939APP::prvUpdatePGN65014Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING))
    {
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM)!=CFGZ::CFGZ_1_PHASE_SYSTEM)
        {
             f32PGN_65014Data[0] = _hal.AcSensors.MAINS_GetRYVolts();
        }

         f32PGN_65014Data[1]= _hal.AcSensors.MAINS_GetVoltageVolts(R_PHASE);
         f32PGN_65014Data[2]= _hal.AcSensors.MAINS_GetApproxFreq(R_PHASE);
    }
}
void J1939APP::prvUpdatePGN65011Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING))
    {
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM)!= CFGZ::CFGZ_1_PHASE_SYSTEM)
        {
            f32PGN_65011Data[1] = _hal.AcSensors.MAINS_GetVoltageVolts(Y_PHASE);
        }

        if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM)== CFGZ::CFGZ_3_PHASE_SYSTEM)
        {
             f32PGN_65011Data[2] = _hal.AcSensors.MAINS_GetApproxFreq(Y_PHASE);

            if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM)== CFGZ::CFGZ_3_PHASE_SYSTEM)
            {
                 f32PGN_65011Data[0] = _hal.AcSensors.MAINS_GetYBVolts();
            }
        }
    }
}

void J1939APP::prvUpdatePGN65008Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING))
    {
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM)== CFGZ::CFGZ_3_PHASE_SYSTEM)
        {
             f32PGN_65008Data[0] = _hal.AcSensors.MAINS_GetRBVolts();
             f32PGN_65008Data[1] = _hal.AcSensors.MAINS_GetVoltageVolts(B_PHASE);
             f32PGN_65008Data[2] = _hal.AcSensors.MAINS_GetApproxFreq(B_PHASE);
        }
    }
}
void J1939APP::prvUpdatePGN65030Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
      if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM)!=CFGZ::CFGZ_1_PHASE_SYSTEM)
      {
          f32PGN_65030Data[0]=_hal.AcSensors.GENSET_GetRawAverageL_L();
      }
      f32PGN_65030Data[1]= _hal.AcSensors.GENSET_GetRawAverageL_N();
      f32PGN_65030Data[2]= _hal.AcSensors.GENSET_GetAvgFreq();
      f32PGN_65030Data[3]= _hal.AcSensors.GENSET_GetRawAvgCurrent();
    }
}
void J1939APP::prvUpdatePGN65027Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        f32PGN_65027Data[1] = _hal.AcSensors.GENSET_GetVoltageVolts(R_PHASE);
        f32PGN_65027Data[2] = _hal.AcSensors.GENSET_GetApproxFreq(R_PHASE);
        f32PGN_65027Data[3] = _hal.AcSensors.GENSET_GetCurrentAmps(R_PHASE) ;

        if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM)!=CFGZ::CFGZ_1_PHASE_SYSTEM)
        {
            f32PGN_65027Data[0] = _hal.AcSensors.GENSET_GetRYVolts();
        }
    }
}

void J1939APP::prvUpdatePGN65026Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        f32PGN_65026Data[0] =  _hal.AcSensors.GENSET_GetActivePowerWatts(R_PHASE);
        f32PGN_65026Data[1] =  _hal.AcSensors.GENSET_GetApparentPowerVA(R_PHASE);
    }
}
void J1939APP::prvUpdatePGN65025Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        f32PGN_65025Data[0] =  _hal.AcSensors.GENSET_GetReactivePowerVAR(R_PHASE);
        f32PGN_65025Data[1] =   abs(_hal.AcSensors.GENSET_GetPowerFactor(R_PHASE));
    }
}
void J1939APP::prvUpdatePGN65024Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM)!=CFGZ::CFGZ_1_PHASE_SYSTEM)
        {
            f32PGN_65024Data[1] = _hal.AcSensors.GENSET_GetVoltageVolts(Y_PHASE);
            f32PGN_65024Data[2] = _hal.AcSensors.GENSET_GetApproxFreq(Y_PHASE);
            f32PGN_65024Data[3] = _hal.AcSensors.GENSET_GetCurrentAmps(Y_PHASE);

            if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM)== CFGZ::CFGZ_3_PHASE_SYSTEM)
            {
                f32PGN_65024Data[0] = _hal.AcSensors.GENSET_GetYBVolts();
            }
        }
    }
}
void J1939APP::prvUpdatePGN65023Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM)!=CFGZ::CFGZ_1_PHASE_SYSTEM)
        {
            f32PGN_65023Data[0] =  _hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE);
            f32PGN_65023Data[1] = _hal.AcSensors.GENSET_GetApparentPowerVA(Y_PHASE);
        }
    }
}

void J1939APP::prvUpdatePGN65022Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM)!=CFGZ::CFGZ_1_PHASE_SYSTEM)
        {
            f32PGN_65022Data[0] = _hal.AcSensors.GENSET_GetReactivePowerVAR(Y_PHASE);
        }

        if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM) ==  CFGZ::CFGZ_3_PHASE_SYSTEM)
        {
            f32PGN_65022Data[1] = abs( _hal.AcSensors.GENSET_GetPowerFactor(Y_PHASE)) ;
        }
    }
}
void J1939APP::prvUpdatePGN65021Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM)== CFGZ::CFGZ_3_PHASE_SYSTEM)
        {
            f32PGN_65021Data[0] = _hal.AcSensors.GENSET_GetRBVolts();
            f32PGN_65021Data[1] = _hal.AcSensors.GENSET_GetVoltageVolts(B_PHASE);
            f32PGN_65021Data[2] = _hal.AcSensors.GENSET_GetApproxFreq(B_PHASE);
            f32PGN_65021Data[3] =  _hal.AcSensors.GENSET_GetCurrentAmps(B_PHASE);
        }
    }
}

void J1939APP::prvUpdatePGN65020Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM)== CFGZ::CFGZ_3_PHASE_SYSTEM)
        {
            f32PGN_65020Data[0] = _hal.AcSensors.GENSET_GetActivePowerWatts(B_PHASE);
            f32PGN_65020Data[1] = _hal.AcSensors.GENSET_GetApparentPowerVA(B_PHASE);
        }
    }
}
void J1939APP::prvUpdatePGN65019Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM)== CFGZ::CFGZ_3_PHASE_SYSTEM)
        {
            f32PGN_65019Data[0] = _hal.AcSensors.GENSET_GetReactivePowerVAR(B_PHASE);

            f32PGN_65019Data[1] =  abs(_hal.AcSensors.GENSET_GetPowerFactor(B_PHASE));
        }
    }
}


void J1939APP::prvUpdatePGN65018Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
            f32PGN_65018Data[0] = (float)(_hal.AcSensors.GENSET_GetTotalActiveEnergySinceInitWH()/1000.0f);

            f32PGN_65018Data[1] = F32_Null;
    }
}

void J1939APP::prvUpdatePGN65029Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        f32PGN_65029Data[0] = _hal.AcSensors.GENSET_GetTotalActivePowerWatts();
        f32PGN_65029Data[1] = _hal.AcSensors.GENSET_GetTotalApparentPowerVA();
    }
}
void J1939APP::prvUpdatePGN65028Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
       f32PGN_65028Data[0] = _hal.AcSensors.GENSET_GetTotalReactivePowerVAR();
       f32PGN_65028Data[1] = _hal.AcSensors.GENSET_GetAveragePowerFactor();
    }
}

void J1939APP::prvUpdatePGN64911Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        f32PGN_64911Data[0] = _hal.AcSensors.GENSET_GetPercentPower();
    }
}
void J1939APP::prvUpdatePGN65280Data(void)
{
    if((BASE_MODES::GetGCUOperatingMode()== BASE_MODES::MANUAL_MODE)
            &&(!_engMon.IsEngineOn())&& (!_bDeviceInConfigMode))
    {
       f32PGN_65280Data[0] = 0;       // Operating Manual and Engine is OFF
    }
    else if((BASE_MODES::GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)
            &&(_engMon.IsEngineOn()))
    {
      f32PGN_65280Data[0] = 2;        // Operating Manual and Engine is ON
    }
    else if(BASE_MODES::GetGCUOperatingMode() ==  BASE_MODES::AUTO_MODE)
    {
       f32PGN_65280Data[0] = 1;       // Operating Auto mode irrespective of engine status
    }
    else
    {
        f32PGN_65280Data[0] = 3;      //Error
    }


    f32PGN_65280Data[1] = 3;  //Reserved


    f32PGN_65280Data[2] = 0xFFFF; //Reserved
    f32PGN_65280Data[3] = _cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_MIN_HEALTHY_FREQ);
    f32PGN_65280Data[4] = _cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_MIN_HEALTHY_VOLT);
}
void J1939APP::prvUpdatePGN65289Data(void)
{
    uint8_t u8DummyOne = 0x01;
    uint16_t u16GenStatus = 0;

    memset((void*)&f32PGN_65289Data, 0x00, sizeof(f32PGN_65289Data));

    /* f32PGN_65289Data[0] to f32PGN_65289Data[15] are used for Input Output
     * diagnostics*/
    for(uint8_t u8Index = 15 ,u8Local=GCU_ALARMS::DIG_IN_A; u8Local <=GCU_ALARMS::DIG_IN_H; u8Local++)
    {
        f32PGN_65289Data[u8Index] = ((uint8_t)_gcuAlarm.AlarmResultLatched((GCU_ALARMS::ALARM_LIST_t)u8Local));
        u8Index--;
    }

    for(uint8_t u8Index = 7 ,u8Local=CFGZ::ID_OUT_A_SOURCE; u8Local <=CFGZ::ID_OUT_G_SOURCE; u8Local=u8Local+2)
    {
        f32PGN_65289Data[u8Index] = ((uint8_t)(_hal.actuators.GetActStatus((ACTUATOR::ACTUATOR_TYPS_t)_cfgz.GetCFGZ_Param((CFGZ::UINT8_PARAMS_t)u8Local))
                == ACT_Manager::ACT_LATCHED));
        u8Index--;
    }


    /* f32PGN_65289Data[16] to f32PGN_65289Data[29] are used to represent Gen status*/
    u16GenStatus = _ObjmbApp.GetGenStatusRegister();
    for(uint8_t u8Indx2 = 0, u8Index = 16; u8Index < 30; u8Index++)
    {
        if(u8Indx2 == 11U)
        {
            f32PGN_65289Data[u8Index] = 0x0u;
            f32PGN_65289Data[u8Index]  = (float)(((u16GenStatus & ((uint16_t)0x01 << u8Indx2)) >> u8Indx2)
                                                     |((u16GenStatus & ((uint16_t)0x01 << (u8Indx2 + 1U))) >> u8Indx2)
                                                     |((u16GenStatus & ((uint16_t)0x01 << (u8Indx2 + 2U))) >> u8Indx2));

            /*Following explicit handling is to keep bit pattern safe*/
            u8DummyOne = (uint8_t) f32PGN_65289Data[u8Index];
            f32PGN_65289Data[u8Index]= u8DummyOne;
            u8DummyOne = 0x01u;
            /********************************************************/
            u8Indx2 += 2U;
        }
        else
        {
            uint32_t u32Temp = (u16GenStatus & ((uint16_t)0x01U << u8Indx2)) >> u8Indx2;
            f32PGN_65289Data[u8Index] = (float)u32Temp;
            if(u32Temp == 1)
            {
                (void *)memcpy((void *)&( f32PGN_65289Data[u8Index]), (void *)&u8DummyOne, 1U);
            }

        }
        u8Indx2++;
    }

    /* f32PGN_65289Data[30] to f32PGN_65289Data[31] are used to send Number
     * of Starts and Trips.*/
    f32PGN_65289Data[30] = (float)_engMon.GetEngineNoOfStarts();
    f32PGN_65289Data[31] = (float)_engMon.GetEngineNoOfTrips();
}

void J1939APP::prvUpdatePGN65290Data(void)
{
    f32PGN_65290Data[0] = (uint16_t)(_engMon.GetEngineRunTimeMin() /60);
    f32PGN_65290Data[1] = (uint16_t)(_engMon.GetEngineRunTimeMin() % 60);
}


void J1939APP::UpdateAlarmRegValue(uint8_t u8AlarmID, uint8_t u8Offset , float* f32PGN)
{
    if(_gcuAlarm.IsAlarmMonEnabled((GCU_ALARMS::ALARM_LIST_t)u8AlarmID))
    {
        if(_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8AlarmID))
        {
            UpdateAlarmRegStatus(u8AlarmID, u8Offset , (float*)&f32PGN[0]);
        }
        else
        {
            f32PGN[u8Offset] = (uint16_t)(ALARM_INACTIVE);
        }
    }
}

void J1939APP::UpdateAlarmRegStatus(uint8_t u8AlarmID,  uint8_t u8Offset , float* f32PGN)
{
    if(_gcuAlarm.IsShutdownAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8AlarmID))
    {
        f32PGN[u8Offset] = (uint16_t)(ALARM_SHUTDOWN);
    }
    else if(_gcuAlarm.IsElectricTripAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8AlarmID))
    {
        f32PGN[u8Offset]  = (uint16_t)(ALARM_ELEC_TRIP );
    }
    else if(_gcuAlarm.IsWarningAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8AlarmID))
    {
        f32PGN[u8Offset]  = (uint16_t)(ALARM_WARNING);
    }
    else if(_gcuAlarm.IsNotificationAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8AlarmID))
    {
        f32PGN[u8Offset] = (uint16_t)(ALARM_NOTIFICATION);
    }

}

void  J1939APP::UpdateDGVoltAlarms( uint8_t u8WarningID, uint8_t u8ShutdownID, uint8_t u8Offset , float* f32PGN)
{
    if((_gcuAlarm.IsAlarmMonEnabled((GCU_ALARMS::ALARM_LIST_t)u8WarningID))
            || (_gcuAlarm.IsAlarmMonEnabled((GCU_ALARMS::ALARM_LIST_t)u8ShutdownID)))
    {
        if(_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8ShutdownID))
        {
            f32PGN[u8Offset] =  (uint16_t)(ALARM_SHUTDOWN);
        }
        else if(_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8WarningID))
        {
            f32PGN[u8Offset] =  (uint16_t)(ALARM_WARNING);
        }
        else
        {
            f32PGN[u8Offset] =  (uint16_t)(ALARM_INACTIVE );
        }
    }
    else
    {
        f32PGN[u8Offset] =  (uint16_t)(ALARM_DISABLED );
    }
}

void  J1939APP::prvUpdateDGVoltAlarmsInAnyPhase( uint8_t u8WarningID, uint8_t u8ShutdownID, uint8_t u8Offset , float* f32PGN)
{
    if((_gcuAlarm.IsAlarmMonEnabled((GCU_ALARMS::ALARM_LIST_t)u8WarningID)) || (_gcuAlarm.IsAlarmMonEnabled((GCU_ALARMS::ALARM_LIST_t)(u8WarningID+2)))
            ||(_gcuAlarm.IsAlarmMonEnabled((GCU_ALARMS::ALARM_LIST_t)(u8WarningID+4)))
            || (_gcuAlarm.IsAlarmMonEnabled((GCU_ALARMS::ALARM_LIST_t)u8ShutdownID)) || (_gcuAlarm.IsAlarmMonEnabled((GCU_ALARMS::ALARM_LIST_t)(u8ShutdownID+2)))
            || (_gcuAlarm.IsAlarmMonEnabled((GCU_ALARMS::ALARM_LIST_t)(u8ShutdownID+4))))
    {
        if(_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8ShutdownID) || _gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)(u8ShutdownID+2)) || _gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)(u8ShutdownID+4)) )
        {
            f32PGN[u8Offset] =  (uint16_t)(ALARM_SHUTDOWN);
        }
        else if(_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8WarningID) || _gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)(u8WarningID+2)) || _gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)(u8WarningID+4)))
        {
            f32PGN[u8Offset] =  (uint16_t)(ALARM_WARNING);
        }
        else
        {
            f32PGN[u8Offset] =  (uint16_t)(ALARM_INACTIVE );
        }
    }

    else
    {
        f32PGN[u8Offset] =  (uint16_t)(ALARM_DISABLED );
    }
}

void  J1939APP::UpdateEngSensorAlarms( uint8_t u8SensorID,
                    uint8_t u8SwitchID, uint8_t u8Offset , float* f32PGN)
{
    if((_gcuAlarm.IsAlarmMonEnabled((GCU_ALARMS::ALARM_LIST_t)u8SensorID))
        || (_gcuAlarm.IsAlarmMonEnabled((GCU_ALARMS::ALARM_LIST_t)u8SwitchID)))
    {
        if((_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8SensorID))
             &&(_gcuAlarm.IsShutdownAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8SensorID)))
        {
            f32PGN[u8Offset] = (uint16_t)(ALARM_SHUTDOWN );
        }
        else if((_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8SwitchID))
                    &&(_gcuAlarm.IsShutdownAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8SwitchID)))
        {
            f32PGN[u8Offset] = (uint16_t)(ALARM_SHUTDOWN );
        }
        else if((_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8SensorID))
                    &&(_gcuAlarm.IsElectricTripAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8SensorID)))
        {
            f32PGN[u8Offset] = (uint16_t)(ALARM_ELEC_TRIP );
        }
        else if((_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8SwitchID))
                    &&(_gcuAlarm.IsElectricTripAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8SwitchID)))
        {
            f32PGN[u8Offset] = (uint16_t)(ALARM_ELEC_TRIP );
        }
        else if((_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8SensorID))
                     &&(_gcuAlarm.IsWarningAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8SensorID)))
        {
            f32PGN[u8Offset] = (uint16_t)(ALARM_WARNING);
        }
        else if((_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8SwitchID))
                    &&(_gcuAlarm.IsWarningAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8SwitchID)))
        {
            f32PGN[u8Offset]  = (uint16_t)(ALARM_WARNING);
        }
        else if((_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8SensorID))
                     &&(_gcuAlarm.IsNotificationAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8SensorID)))
        {
            f32PGN[u8Offset] = (uint16_t)(ALARM_NOTIFICATION);
        }
        else if((_gcuAlarm.IsAlarmActive((GCU_ALARMS::ALARM_LIST_t)u8SwitchID))
                    &&(_gcuAlarm.IsNotificationAlarmEnabled((GCU_ALARMS::ALARM_LIST_t)u8SwitchID)))
        {
            f32PGN[u8Offset] = (uint16_t)(ALARM_NOTIFICATION );
        }
        else
        {
            f32PGN[u8Offset] = (uint16_t)(ALARM_INACTIVE );
        }
    }
    else
    {
        f32PGN[u8Offset] = (uint16_t)(ALARM_DISABLED );
    }
}



void J1939APP::prvUpdatePGN65291Data(void)
{
    memset((void*)&f32PGN_65291Data, 0x00, sizeof(f32PGN_65291Data));

    UpdateDGVoltAlarms( GCU_ALARMS::LOW_OIL_PRESS_WARNING  ,
                   GCU_ALARMS::LOW_OIL_PRESS_SHUTDOWN, ALARM_BYTE_0,(float*)&f32PGN_65291Data[0]);

    UpdateEngSensorAlarms( GCU_ALARMS::HIGH_WATER_TEMP, GCU_ALARMS::HWT_SWITCH, ALARM_BYTE_1 , (float*)&f32PGN_65291Data[0]);
    if(_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::LOW_FUEL_LEVEL_SHUTDOWN) ||
            _gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::LOW_FUEL_LEVEL_NOTIFICATION)
            ||  _gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::LFL_SWITCH))
    {
        if(_gcuAlarm.IsAlarmActive(GCU_ALARMS::LOW_FUEL_LEVEL_SHUTDOWN))
        {
            f32PGN_65291Data[ALARM_BYTE_2] = ALARM_SHUTDOWN ;
        }
        else if (_gcuAlarm.IsAlarmActive(GCU_ALARMS::LFL_SWITCH)
                && _gcuAlarm.IsShutdownAlarmEnabled(GCU_ALARMS::LFL_SWITCH))
        {
            f32PGN_65291Data[ALARM_BYTE_2] = ALARM_SHUTDOWN ;
        }
        else if (_gcuAlarm.IsAlarmActive(GCU_ALARMS::LFL_SWITCH)
                && _gcuAlarm.IsElectricTripAlarmEnabled(GCU_ALARMS::LFL_SWITCH))
        {
            f32PGN_65291Data[ALARM_BYTE_2] = ALARM_ELEC_TRIP ;
        }
        else if (_gcuAlarm.IsAlarmActive(GCU_ALARMS::LFL_SWITCH)
                && _gcuAlarm.IsWarningAlarmEnabled(GCU_ALARMS::LFL_SWITCH))
        {
            f32PGN_65291Data[ALARM_BYTE_2] = ALARM_WARNING ;
        }
        else if(_gcuAlarm.IsAlarmActive(GCU_ALARMS::LOW_FUEL_LEVEL_NOTIFICATION))
        {
            f32PGN_65291Data[ALARM_BYTE_2] =  ALARM_NOTIFICATION;
        }
        else if (_gcuAlarm.IsAlarmActive(GCU_ALARMS::LFL_SWITCH)
                && _gcuAlarm.IsNotificationAlarmEnabled(GCU_ALARMS::LFL_SWITCH))
        {
            f32PGN_65291Data[ALARM_BYTE_2] =  ALARM_NOTIFICATION;
        }
        else
        {
            f32PGN_65291Data[ALARM_BYTE_2] =  ALARM_INACTIVE;
        }
    }
    else
    {
        f32PGN_65291Data[ALARM_BYTE_2] =  ALARM_DISABLED;
    }
    UpdateAlarmRegValue(GCU_ALARMS::RWL_SWITCH, ALARM_BYTE_3 , (float*)&f32PGN_65291Data[0]);

    UpdateAlarmRegValue(GCU_ALARMS::UNDERSPEED, ALARM_BYTE_4 , (float*)&f32PGN_65291Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::OVERSPEED_L1, ALARM_BYTE_5 , (float*)&f32PGN_65291Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::FAIL_TO_START, ALARM_BYTE_6 , (float*)&f32PGN_65291Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::FAIL_TO_STOP, ALARM_BYTE_7 , (float*)&f32PGN_65291Data[0]);

    if((_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_R_UV_WARNING)) || (_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_Y_UV_WARNING)) || (_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_B_UV_WARNING))
            || (_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_R_UV_SHUTDOWN)) || (_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_Y_UV_SHUTDOWN))|| (_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_B_UV_SHUTDOWN)))
    {
        if(_gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_R_UV_WARNING) || _gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_Y_UV_WARNING) || _gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_B_UV_WARNING) )
        {
            f32PGN_65291Data[ALARM_BYTE_8] =  (uint16_t)(ALARM_WARNING);
        }
        else if(_gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_R_UV_SHUTDOWN) || _gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_Y_UV_SHUTDOWN) || _gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_B_UV_SHUTDOWN) )
        {
            f32PGN_65291Data[ALARM_BYTE_8] =  (uint16_t)(ALARM_SHUTDOWN);
        }
        else
        {
            f32PGN_65291Data[ALARM_BYTE_8] =  (uint16_t)(ALARM_INACTIVE );
        }
    }
    else
    {
        f32PGN_65291Data[ALARM_BYTE_8] =  (uint16_t)(ALARM_DISABLED );
    }

    if((_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_R_OV_WARNING)) || (_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_Y_OV_WARNING)) || (_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_B_OV_WARNING))
            || (_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_R_OV_SHUTDOWN)) || (_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_Y_OV_SHUTDOWN))|| (_gcuAlarm.IsAlarmMonEnabled(GCU_ALARMS::DG_B_OV_SHUTDOWN)))
    {
        if(_gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_R_OV_WARNING) || _gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_Y_OV_WARNING) || _gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_B_OV_WARNING) )
        {
            f32PGN_65291Data[ALARM_BYTE_9] =  (uint16_t)(ALARM_WARNING);
        }
        else if(_gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_R_OV_SHUTDOWN) || _gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_Y_OV_SHUTDOWN) || _gcuAlarm.IsAlarmActive(GCU_ALARMS::DG_B_OV_SHUTDOWN) )
        {
            f32PGN_65291Data[ALARM_BYTE_9] =  (uint16_t)(ALARM_SHUTDOWN);
        }
        else
        {
            f32PGN_65291Data[ALARM_BYTE_9] =  (uint16_t)(ALARM_INACTIVE );
        }
    }
    else
    {
        f32PGN_65291Data[ALARM_BYTE_9] =  (uint16_t)(ALARM_DISABLED );
    }


    UpdateDGVoltAlarms( GCU_ALARMS::UNDERFREQ_WARNING  ,
               GCU_ALARMS::UNDERFREQ_SHUTDOWN, ALARM_BYTE_10,(float*)&f32PGN_65291Data[0]);
    UpdateDGVoltAlarms( GCU_ALARMS::OVERFREQ_WARNING  ,
               GCU_ALARMS::OVERFREQ_SHUTDOWN, ALARM_BYTE_11,(float*)&f32PGN_65291Data[0]);

    UpdateAlarmRegValue(GCU_ALARMS::OVERCURRENT, ALARM_BYTE_12 , (float*)&f32PGN_65291Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::OVERLOAD, ALARM_BYTE_13 , (float*)&f32PGN_65291Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::LOAD_UNBALANCE, ALARM_BYTE_14 , (float*)&f32PGN_65291Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::ESTOP, ALARM_BYTE_15 , (float*)&f32PGN_65291Data[0]);

}
void J1939APP::prvUpdatePGN65292Data(void)
{
    memset((void*)&f32PGN_65292Data, 0x00, sizeof(f32PGN_65292Data));

    f32PGN_65292Data[ALARM_BYTE_0] = F32_Null;
    UpdateAlarmRegValue(GCU_ALARMS::FILT_MAINTENANCE, ALARM_BYTE_1 , (float*)&f32PGN_65292Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::FILT_MAINTENANCE, ALARM_BYTE_1 , (float*)&f32PGN_65292Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::CA_FAIL, ALARM_BYTE_2 , (float*)&f32PGN_65292Data[0]);
    f32PGN_65292Data[ALARM_BYTE_3] = F32_Null;

    UpdateAlarmRegValue(GCU_ALARMS::VBAT_UV, ALARM_BYTE_4, (float*)&f32PGN_65292Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::VBAT_OV, ALARM_BYTE_5 , (float*)&f32PGN_65292Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::OPEN_ENG_TEMP_CKT, ALARM_BYTE_5 , (float*)&f32PGN_65292Data[0]);
    f32PGN_65292Data[ALARM_BYTE_7] = F32_Null;

    UpdateAlarmRegValue(GCU_ALARMS::FUEL_THEFT, ALARM_BYTE_8 , (float*)&f32PGN_65292Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::MPU_LOSS, ALARM_BYTE_9 , (float*)&f32PGN_65292Data[0]);
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                    ||
       (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2))
    {
        if(_gcuAlarm.IsAlarmActive(GCU_ALARMS::OPEN_LOP_SENS_CKT))
        {
            UpdateAlarmRegValue(GCU_ALARMS::OPEN_LOP_SENS_CKT, ALARM_BYTE_10,(float*)&f32PGN_65292Data[0] );
        }
        else if(_gcuAlarm.IsAlarmActive(GCU_ALARMS::LOP_SENS_SHORT_TO_BATTERY))
        {
            UpdateAlarmRegValue(GCU_ALARMS::LOP_SENS_SHORT_TO_BATTERY, ALARM_BYTE_10 , (float*)&f32PGN_65292Data[0]);
        }
        else if (_gcuAlarm.IsAlarmActive(GCU_ALARMS::OPEN_LOP_CURR_SENS_CKT))
        {
            UpdateAlarmRegValue(GCU_ALARMS::OPEN_LOP_CURR_SENS_CKT, ALARM_BYTE_10 , (float*)&f32PGN_65292Data[0]);
        }
        else
        {
            f32PGN_65292Data[ALARM_BYTE_10]= (uint16_t)(0);
        }
    }
    else
    {
        UpdateAlarmRegValue(GCU_ALARMS::OPEN_LOP_SENS_CKT, ALARM_BYTE_10 , (float*)&f32PGN_65292Data[0]);
    }
    UpdateAlarmRegValue(GCU_ALARMS::DIG_IN_I, ALARM_BYTE_11, (float*)&f32PGN_65292Data[0]);

    UpdateAlarmRegValue(GCU_ALARMS::DIG_IN_A, ALARM_BYTE_12, (float*)&f32PGN_65292Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::DIG_IN_B, ALARM_BYTE_13, (float*)&f32PGN_65292Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::DIG_IN_C, ALARM_BYTE_14, (float*)&f32PGN_65292Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::DIG_IN_D, ALARM_BYTE_15, (float*)&f32PGN_65292Data[0]);

}
void J1939APP::prvUpdatePGN65293Data(void)
{
    memset((void*)&f32PGN_65293Data, 0x00, sizeof(f32PGN_65293Data));

    UpdateAlarmRegValue(GCU_ALARMS::DIG_IN_E, ALARM_BYTE_0, (float*)&f32PGN_65293Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::DIG_IN_F, ALARM_BYTE_1, (float*)&f32PGN_65293Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::DIG_IN_G, ALARM_BYTE_2, (float*)&f32PGN_65293Data[0]);
    UpdateAlarmRegValue(GCU_ALARMS::DIG_IN_H, ALARM_BYTE_3, (float*)&f32PGN_65293Data[0]);

    f32PGN_65293Data[ALARM_BYTE_4] = F32_Null;
    f32PGN_65293Data[ALARM_BYTE_5] = F32_Null;
    f32PGN_65293Data[ALARM_BYTE_6] = F32_Null;
    f32PGN_65293Data[ALARM_BYTE_7] = F32_Null;

    f32PGN_65293Data[ALARM_BYTE_8] = F32_Null;
    f32PGN_65293Data[ALARM_BYTE_9] = F32_Null;
    f32PGN_65293Data[ALARM_BYTE_10] = F32_Null;
    f32PGN_65293Data[ALARM_BYTE_11] = F32_Null;

    f32PGN_65293Data[ALARM_BYTE_12] = F32_Null;
    f32PGN_65293Data[ALARM_BYTE_13] = F32_Null;
    f32PGN_65293Data[ALARM_BYTE_14] = F32_Null;
    f32PGN_65293Data[ALARM_BYTE_15] = F32_Null;
}


void J1939APP::prvUpdatePGN65295Data(void)
{
    A_SENSE::SENSOR_RET_t stval = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED} ;

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION)
                           == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        stval = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION)
                           == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        stval =_hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE_4_20);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        stval = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE_0_TO_5V);
    }


    if((stval.eStatus!=A_SENSE::SENSOR_NOT_CONFIGRUED)
            && (stval.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT ))
    {
         f32PGN_65295Data[0] =stval.stValAndStatus.f32InstSensorVal;
    }
    else if(stval.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT )/* If sensor is open circuit then invalid value*/
    {
        f32PGN_65295Data[0] = F32_Null;
    }
    else
    {
        ;
    }

    stval = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);


    if((stval.eStatus!=A_SENSE::SENSOR_NOT_CONFIGRUED)
               && (stval.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT ))
    {
        f32PGN_65295Data[2] =stval.stValAndStatus.f32InstSensorVal
                * _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY)/100;;
    }
    else if(stval.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT )/* If sensor is open circuit then invalid value*/
    {
       f32PGN_65295Data[2] = F32_Null;
    }
    else
    {
       ;
    }

    f32PGN_65295Data[1] = 0xFFFF;
    f32PGN_65295Data[3] = _cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_GEN_RATING);
}


void J1939APP::prvUpdatePGN65296Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
         f32PGN_65296Data[0] =(float)( _hal.AcSensors.GENSET_GetTotalActiveEnergySinceInitWH()/1000);
         f32PGN_65296Data[1] =(float) (_hal.AcSensors.GENSET_GetTotalApparentEnergySinceInitVAH()/1000);
    }
}
void J1939APP::prvUpdatePGN65297Data(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT))
    {
        f32PGN_65297Data[0] = (float)(_hal.AcSensors.GENSET_GetTotalReactiveEnergySinceInitVARH()/1000);
    }

}

void J1939APP::prvUpdatePGN59904Data(uint8_t *au8CalSPNData)
{
    //Following cases for sending request for these 6 PGN's.
    switch(_u8SPNIndexInRQSTPGN)
    {
        case 0: // RX_PGN_HOURS_65253
            *au8CalSPNData = 0xE5;
            au8CalSPNData++;
            *au8CalSPNData   = 0xFE;
            break;

        case 1: // RX_PGN_LFC1_65257
            *au8CalSPNData = 0xE9;
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
    bool bECUSourceAddressMatched = false;
    static uint8_t u8NumberOfPacket = 0,u8ReadData=0;
    static uint8_t u8PrevPackateNo =0;


    uint8_t u8PS, u8PF,u8SourceAddress;
    uint8_t u8ReceivedPgnNo = RX_PGN_LAST;
    can_frame_t CAN_RxQueueBuffer;

     typedef union
     {
         tPDUIDFormat tPDUIdFrame;
         uint32_t ulPDUId;
     }uPDUIDData;

     uPDUIDData uPDU_ID_Data;

     if(_hal.Objcan.ReadFrame(&CAN_RxQueueBuffer) ==BSP_SUCCESS)
     {
        /// read the CAN frame id
        uPDU_ID_Data.ulPDUId =  CAN_RxQueueBuffer.id;


        u8PS =(uint8_t)( uPDU_ID_Data.tPDUIdFrame.uiPDU_PGN & 0x00FF);    // PDU Specific
        u8PF =(uint8_t) ((uPDU_ID_Data.tPDUIdFrame.uiPDU_PGN >>8)& 0x00FF); //PDU Format

        u8SourceAddress = (uint8_t) (uPDU_ID_Data.tPDUIdFrame.ubyPDU_SA);
        /* If the received source address is same as the configured ECU Source address then only received the data.  */

        if(u8SourceAddress == _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_SOURCE_ADDRESS))
        {
            bECUSourceAddressMatched = true;
        }
        //bECUSourceAddressMatched = true;
        //  when 0 <= PF(PDU Format) <= 239 defines peer-peer communication. This is PDU1 format.
        //  In this case, PS defines a 8 bit destination node address.
        //  PGN = (DP<<9 + PF<<8 + 0)
        //  In this case, PS is not considered a part of the PGN.
        //  when 240 <= PF <= 255 defines broadcast communication. This is PDU2 format.
        //  In this case, PS defines a 8 bit Group extension. A group extension
        //  identifies a group of nodes on the J1939 bus.
        //  PGN = (DP<<9 + PF<<8 + PS)  PGN-Parameter Group Number

        if(u8PF >=240)
        {
            uPDU_ID_Data.tPDUIdFrame.uiPDU_PGN = (uint16_t)((((uint16_t)uPDU_ID_Data.tPDUIdFrame.ubPGN_DP)<<9U)
                                                   + (((uint16_t)u8PF)<<8U)+ u8PS);
        }
        else if(u8PF <=239)
        {
            uPDU_ID_Data.tPDUIdFrame.uiPDU_PGN = (uint16_t)((((uint16_t)uPDU_ID_Data.tPDUIdFrame.ubPGN_DP)<<9U)
                                               + (((uint16_t)u8PF)<<8U)) ;
        }
        if(bECUSourceAddressMatched)
        {
            bECUSourceAddressMatched = false;
            switch(uPDU_ID_Data.tPDUIdFrame.uiPDU_PGN)
            {

                case PGN_TPCM: //TPCM Transfer protocol - connection management ---> BAM-Braudcast Announce message
                    u16LocalPGN = (uint16_t)((((uint16_t)CAN_RxQueueBuffer.data[6]) << 8U) + CAN_RxQueueBuffer.data[5]);
                    if((u16LocalPGN == PGN_DM01)|| (u16LocalPGN == PGN_DM02) /*|| (u16LocalPGN == PROPB_5E_PGN*/)
                    {
                        u8ReadData = 1;
                        u8NumberOfPacket =CAN_RxQueueBuffer.data[3];

                        _u8NumOfDM1SPN= (uint8_t)(((((uint16_t)CAN_RxQueueBuffer.data[2] << 8U)
                                          |CAN_RxQueueBuffer.data[1]) - 2U) / DM1_DTC_SIZE);
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
                        else
                        {
                            /* Do nothing */
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
                case AT1T1I_PGN:
                u8ReceivedPgnNo = RX_PGN_AT1T1I_65110  ;
                break;

                case PGN_EEC1   :
                UTILS_ResetTimer(&_CommFailTimeout);
                u8ReceivedPgnNo = RX_PGN_EEC1_61444    ;
                _bIsCANJ1939CommFail = false;
                break;
                case PGN_EEC2   :
                u8ReceivedPgnNo = RX_PGN_EEC2_61443 ;
                break;
                case PGN_EOI    :
                u8ReceivedPgnNo = RX_PGN_EOI_64914 ;
                break;
                case PGN_HOURS  :
                u8ReceivedPgnNo = RX_PGN_HOURS_65253 ;
                break;
                case PGN_LFC1   :
                u8ReceivedPgnNo = RX_PGN_LFC1_65257 ;
                break;
                case PGN_ET1    :
                u8ReceivedPgnNo = RX_PGN_ET1_65262 ;
                break;
                case PGN_EFL_P1 :
                u8ReceivedPgnNo = RX_PGN_EFL_P1_65263 ;
                break;
                case PGN_AMB    :
                u8ReceivedPgnNo = RX_PGN_AMB_65269 ;
                break;
                case PGN_VEP1   :
                u8ReceivedPgnNo = RX_PGN_VEP1_65271 ;
                break;
                case PGN_WFI_OI :
                u8ReceivedPgnNo = RX_PGN_WFI_OI_65279 ;
                break;
                case PGN_DEFA   :
                u8ReceivedPgnNo = RX_PGN_DEFA_65383 ;
                break;
                case PGN_SHUTDN:
                u8ReceivedPgnNo =  RX_PGN_SHUTDN_65252;
                break;
                case PGN_CSA:
                u8ReceivedPgnNo = RX_PGN_CSA_64966;
                break;
                case PGN_DM03   :
                u8ReceivedPgnNo = RX_PGN_DM03_65228;
                break;
                case PGN_IC1   :
                u8ReceivedPgnNo = RX_PGN_IC1_65270;
                break;
                case PGN_LFE1   :
                u8ReceivedPgnNo = RX_PGN_LFE1_65266;
                break;

               case PGN_DM11:
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
        }
    }
}


void J1939APP::Update(bool bDeviceInconfig)
{
#define GAIN 0.2f

    uint16_t u16PGN_State ;

    _bDeviceInConfigMode =bDeviceInconfig;
    if(_bRequestDM11PGN || _bRequestDM2PGN)
    {
        GenerateFrame(PGN_TX_RQST);
        TransmitFrame(PGN_TX_RQST);
    }
    else
    {
        /* nothing */
    }
    GenerateFrame(_u8PGN_Num);
    _u8PGN_Num++;//change name according to guidelines
    if(_u8PGN_Num >= TX_LAST_PGN)
    {
       _u8PGN_Num =0;
    }

    ExtractReadFrame();
    UpdateEngineStartStopDecisions();

    if((!_bDeviceInConfigMode)&&(_cfgz.GetEngType() != CFGZ::CFGZ_CONVENTIONAL))
    {

        if((UTILS_GetElapsedTimeInMs(&_Timer250ms) >= 250))
        {
            for(uint8_t u8PGN=0; u8PGN<TX_LAST_PGN; u8PGN++ )
            {
                u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
                if(u16PGN_State == PGN_250MS)
                {
                    TransmitFrame(u8PGN);
                }
                (void)R_IWDT_Refresh(&g_wdt0_ctrl);
            }
            UTILS_ResetTimer(&_Timer250ms);
        }

        if(( UTILS_GetElapsedTimeInMs(&_Timer100ms) >= 100))
        {
            for(uint8_t u8PGN=0; u8PGN<TX_LAST_PGN; u8PGN++ )
            {
                u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
                if(u16PGN_State == PGN_100MS)
                {
                    TransmitFrame(u8PGN);
                }
                (void)R_IWDT_Refresh(&g_wdt0_ctrl);
            }
            UTILS_ResetTimer(&_Timer100ms);
        }

        if(( UTILS_GetElapsedTimeInMs(&_Timer300ms) >= 300))
        {
            u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[PGN_TX_RQST]) ;
            if(u16PGN_State == PGN_300MS)
            {
                TransmitFrame(PGN_TX_RQST);
                _u8SPNIndexInRQSTPGN++;
                if(_u8SPNIndexInRQSTPGN >= 2)
                {
                    _u8SPNIndexInRQSTPGN = 0;
                }
            }
            (void)R_IWDT_Refresh(&g_wdt0_ctrl);
            UTILS_ResetTimer(&_Timer300ms);
        }

        if(( UTILS_GetElapsedTimeInMs(&_Timer500ms) >= 500))
        {
            for(uint8_t u8PGN=0; u8PGN<TX_LAST_PGN; u8PGN++ )
            {
                u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
                if(u16PGN_State == PGN_500MS)
                {
                    TransmitFrame(u8PGN);
                }
                (void)R_IWDT_Refresh(&g_wdt0_ctrl);
            }
            UTILS_ResetTimer(&_Timer500ms);
        }

        if(( UTILS_GetElapsedTimeInMs(&_Timer1s) >= 1000))
        {
            for(uint8_t u8PGN=0; u8PGN<TX_LAST_PGN; u8PGN++ )
            {
                u16PGN_State =  *(uint16_t*)(ubypReadTxPgns[u8PGN]) ;
                if(u16PGN_State == PGN_1SEC)
                {
                    TransmitFrame(u8PGN);
                }
                (void)R_IWDT_Refresh(&g_wdt0_ctrl);
            }
            UTILS_ResetTimer(&_Timer1s);
        }
    }

    if(UTILS_GetElapsedTimeInSec(&_CommFailTimeout) > _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_COMM_FAILURE_ACT_DELAY))
    {
        _bIsCANJ1939CommFail = true;
        ResetLampsStatus();
        _ArrPgnReadData[RX_PGN_EFL_P1_65263][2] = 0;
        _ArrPgnReadData[RX_PGN_ET1_65262][0] = 0;
        _ArrPgnReadData[RX_PGN_HOURS_65253][0] = 0;
        _ArrPgnReadData[RX_PGN_VEP1_65271][0] = 0;
        _ArrPgnReadData[RX_PGN_VEP1_65271][1] = 0;
        _ArrPgnReadData[RX_PGN_EEC1_61444][2] = 0;

    }

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
    _hal.Objcan.SendData(&TxFrame);

}




bool J1939APP::IsJ1939CommEnable(void)
{
    bool bStatus = false;

    if(_cfgz.GetEngType() != CFGZ::CFGZ_CONVENTIONAL)
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

    if(u8Count == 0)
    {
        memset((void *)&_stDm2Decode,0, sizeof(_stDm2Decode));
    }
    else
    {
        /* Shubham Wader
           Currently the function is being called only to cleat the DM02 messeges by sending
           0 as a function parameter. By assuming 0 as clear alarms, cleared memory as well.
           for parameter other that 0, it will just update counters. */
    }
}

uint8_t J1939APP::GetDm2MsgCount(void)
{
    /* return only valid SPNs */
    return (_u8NumOfDM2SPN - _u8NoOfInvalidSpnInDm2Msg);
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
        ubitFlashProtectLAmp:2,
        ubitFlashAmbarLamp:2,
        ubitFlashRedLamp:2,
        ubitFlashMalFuctionLamp:2;
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

    /* Flashig sequence for each lamp */
        if (DM1Lamp.DM1LampStatus.ubitFlashProtectLAmp == 0)
        {
            _u8ProtectLampFlashState = FLASH_AT_1_HZ;
        }
        else if(DM1Lamp.DM1LampStatus.ubitFlashProtectLAmp == 1)
        {
            _u8ProtectLampFlashState = FLASH_AT_2_HZ;
        }
        else
        {
            _u8ProtectLampFlashState = NO_FLASH;
        }

        if (DM1Lamp.DM1LampStatus.ubitFlashAmbarLamp == 0)
        {
            _u8AmbergLampFlashState = FLASH_AT_1_HZ;
        }
        else if(DM1Lamp.DM1LampStatus.ubitFlashAmbarLamp == 1)
        {
            _u8AmbergLampFlashState = FLASH_AT_2_HZ;
        }
        else
        {
            _u8AmbergLampFlashState = NO_FLASH;
        }

        if (DM1Lamp.DM1LampStatus.ubitFlashRedLamp == 0)
        {
            _u8RedLampFlashState = FLASH_AT_1_HZ;
        }
        else if(DM1Lamp.DM1LampStatus.ubitFlashRedLamp == 1)
        {
            _u8RedLampFlashState = FLASH_AT_2_HZ;
        }
        else
        {
            _u8RedLampFlashState = NO_FLASH;
        }

        if (DM1Lamp.DM1LampStatus.ubitFlashMalFuctionLamp == 0)
        {
            _u8MalFunctionLampFlashState = FLASH_AT_1_HZ;
        }
        else if(DM1Lamp.DM1LampStatus.ubitFlashMalFuctionLamp == 1)
        {
            _u8MalFunctionLampFlashState = FLASH_AT_2_HZ;
        }
        else
        {
            _u8MalFunctionLampFlashState = NO_FLASH;
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

    ubyStartByte = (uint8_t)(ubyStartBit / 8);                          // Calculate the byte number in which data is present
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

J1939APP::LAMP_FLASHING_SEQ_t J1939APP::GetLampFlashingSequence(LAMP_st eLamp)
{
    LAMP_FLASHING_SEQ_t eFlashingState = NO_FLASH;
    switch(eLamp)
    {
        case RED_LAMP    :
                   eFlashingState = (LAMP_FLASHING_SEQ_t)_u8RedLampFlashState;
                   break;
        case AMBER_LAMP  :
                   eFlashingState = (LAMP_FLASHING_SEQ_t)_u8AmbergLampFlashState;
                   break;
        case MIL_LAMP    :
                   eFlashingState = (LAMP_FLASHING_SEQ_t)_u8MalFunctionLampFlashState;
                   break;
        case PROTECT_LAMP:
                   eFlashingState = (LAMP_FLASHING_SEQ_t)_u8ProtectLampFlashState;
                   break;
        default: break;
    }
    return eFlashingState;
}

void J1939APP::UpdateEngineStartStopDecisions(void)
{
    if ((uint8_t)GetReadData(RX_PGN_SHUTDN_65252,0) == 0)
    {
        bEnableEngineStart = true;
    }
    else if((uint8_t)GetReadData(RX_PGN_SHUTDN_65252,0) == 1)
    {
        bEnableEngineStart = false;
    }
    else
    {
        bEnableEngineStart = false;
    }

    if ((uint8_t)GetReadData(RX_PGN_CSA_64966, 0) == 0)
    {
        bExecutePreheat = false;
    }
    else if((uint8_t)GetReadData(RX_PGN_CSA_64966, 0) == 1)
    {
        bExecutePreheat = true;
    }
    else
    {
        bExecutePreheat = false;
    }
}
bool J1939APP::IsEngineStartEnableFromPGN_65252(void)
{
    return bEnableEngineStart;
}
bool J1939APP::IsExecutePreheatFromPGN_64966(void)
{
    return bExecutePreheat;
}


void J1939APP::ResetLampsStatus(void)
{
    _u8ActiveDtcAmberLampStatus = 0;
    _u8ActiveDtcRedLampStatus = 0;
    _u8ActiveDtcMILLampStatus = 0;
    _u8ActiveDtcProtectLampStatus = 0;
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
void J1939APP::RequestDM2Messages()
{
    _bRequestDM2PGN = true;
}
void J1939APP::RequestDM11PGN()
{
    _bRequestDM11PGN = true;
}

void J1939APP::ClearDM2Messages()
{
    _u8NumOfDM2SPN = 0;
    memset((void *)&_stDm2Decode,0, sizeof(_stDm2Decode));
}

void J1939APP::CommonAlarmBeeping()
{

    if(UTILS_IsTimerEnabled(&_BeepOnTimer))
    {
        _hal.actuators.Activate(ACTUATOR::ACT_INDUCEMENT_BUZZER);
        if(IsBeepOnTimerExpired())
        {
            UTILS_DisableTimer(&_BeepOnTimer);
            UTILS_ResetTimer(&_BeepOffTimer);
        }

    }
    else if(UTILS_IsTimerEnabled(&_BeepOffTimer))
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_INDUCEMENT_BUZZER);
        if(IsBeepOffTimerExpired())
        {
            UTILS_DisableTimer(&_BeepOffTimer);
            UTILS_ResetTimer(&_BeepOnTimer);
        }

    }
}

bool J1939APP::IsBeepOnTimerExpired()
{
    if(_bIsDEFLevelLow)
    {
        return (UTILS_GetElapsedTimeInSec(&_BeepOnTimer)>=2);
    }
    else if(_bIsDEFLevelSevere)
    {
        return (UTILS_GetElapsedTimeInSec(&_BeepOnTimer)>=2);
    }

    return false;
}


bool J1939APP::IsBeepOffTimerExpired()
{
    if(_bIsDEFLevelLow)
    {
        return (UTILS_GetElapsedTimeInSec(&_BeepOffTimer)>=5);
    }
    else if(_bIsDEFLevelSevere)
    {
        return (UTILS_GetElapsedTimeInSec(&_BeepOffTimer)>=2);
    }

    return false;
}

void J1939APP::UpdateDEFInducementStrategy()
{
    if((_bIsDEFLevelLow || _bIsDEFLevelSevere))
     {
         CommonAlarmBeeping();
     }
     else
     {
        _hal.actuators.Deactivate(ACTUATOR::ACT_INDUCEMENT_BUZZER);
         UTILS_DisableTimer(&_BeepOnTimer);
         UTILS_DisableTimer(&_BeepOffTimer);
     }
}

void J1939APP::ResetDEFInducementStatus()
{
    _bIsDEFLevelLow = false;
    _bIsDEFLevelSevere = false;
}

bool J1939APP::IsFaultCodeReceived(uint32_t u32SPNNo , uint8_t u8FMI)
{
    /* We will chaeck the Fault code in received list of DTC's only */
    for(uint8_t i=0; i< (_u8NumOfDM1SPN - _u8NoOfInvalidSpnInDm1Msg) ; i++)
    {
        if((_stDm1Decode[i].u32SpnNo == u32SPNNo) && (_stDm1Decode[i].u8FMI == u8FMI))
        {
            return true;
        }
    }

    return false;
}

void J1939APP::UpdateInducementFlags(void)
{
    /* Shubham  18.05.2023
     added dependencies of DTC
     on customer request. [SYSE: Devendra D] */

    switch(_cfgz.GetEngType())
    {
        case CFGZ::CFGZ_CONVENTIONAL:
            /* Not applicable */
            break;
        case CFGZ::ECU_162:
        {
            if(
                IsFaultCodeReceived(1761 , 31)  ||
                IsFaultCodeReceived(5838 , 3)
                )
            {
                _bIsDEFLevelLow = false;
                _bIsDEFLevelSevere = true;
            }
            else if( IsFaultCodeReceived(1761 , 18)  ||
                     IsFaultCodeReceived(5838 , 2)       )
            {
                _bIsDEFLevelLow = true;
                _bIsDEFLevelSevere = false;
            }
            else
            {
                _bIsDEFLevelLow = false;
                _bIsDEFLevelSevere = false;
            }
        }
        break;

        default:
            _bIsDEFLevelLow = false;
            _bIsDEFLevelSevere = false;
        break;
    }


    if(_bIsDEFLevelLow || _bIsDEFLevelSevere)
    {
        if((!_BeepOnTimer.bEnabled) &&  (!_BeepOffTimer.bEnabled))
        {
            UTILS_ResetTimer(&_BeepOnTimer);
            UTILS_DisableTimer(&_BeepOffTimer);
        }
        else
        {
            /* Timers are already in use */
        }
    }
    else
    {
        /* Timers will be in default state that is in OFF state */
    }

}
