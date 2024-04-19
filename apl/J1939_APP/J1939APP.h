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
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef APL_J1939APP_J1939APP_H_
#define APL_J1939APP_J1939APP_H_
#include "../mw/J1939_DRIVER/J1939DRIVER.h"
#include "J1939_Database.h"
#include "UTILS_Timer.h"
#include "CFGZ.h"
#include "CFGC/CFGC.h"
#include "config.h"
#include "HAL_Manager.h"
#include "MB_APP.h"
#include "../GENSET_CONTROL/ENGINE_MONITORING/ENGINE_MONITORING.h"

class J1939APP: public J1939DRIVER
{

public:

    #define MAX_NO_SPN_IN_PGN                    (4U)

    #define    DPFC1_PGN       (64892)
    #define    AT1T1I_PGN      (65110)
    #define    PGN_EEC1        (61444)
    #define    PGN_EEC2        (61443)
    #define    PGN_EOI         (64914)
    #define    PGN_AT1T1I1     (65110)
    #define    PGN_HOURS       (65253)
    #define    PGN_LFC1        (65257)
    #define    PGN_ET1         (65262)
    #define    PGN_EFL_P1      (65263)
    #define    PGN_AMB         (65269)
    #define    PGN_VEP1        (65271)
    #define    PGN_WFI_OI      (65279)
    #define    PGN_DEFA        (65383)
    #define    PGN_SHUTDN      (65252)
    #define    PGN_CSA         (64966)
    #define    PGN_DM11        (65235)
    #define    PGN_DM03        (65228)
    #define    PGN_IC1         (65270)
    #define    PGN_LFE1        (65266)



    enum ID_SPN_STATUS
    {
        VALID_DATA = 0,
        ERROR,
        NOT_AVAILABLE
    };

    typedef struct
    {
        uint8_t u8OC;
        uint8_t u8FMI;
        uint32_t u32SpnNo;
    }J1939_DM_MSG_DECODE;

    typedef enum
    {
        RED_LAMP,
        AMBER_LAMP,
        MIL_LAMP,
        PROTECT_LAMP
    }LAMP_st;

    typedef enum
    {
        FLASH_AT_1_HZ,
        FLASH_AT_2_HZ,
        FLASH_ALWAYS,
        NO_FLASH,
    }LAMP_FLASHING_SEQ_t;

    typedef enum
    {
        DISEL_EXHAUST_TANK_LVL_SPN_1761,
        DISEL_EXHAUST_TANK_TEMP_SPN_3031,
        SCR_OPR_INDUCEMENT_ACTIVE_SPN_5245,
        SCR_OPR_INDUCEMENT_SEVERITY_SPN_5246,
        LAST_SPN_IN_PGN_65110
    }SPN_IN_PGN_65110_t;

    typedef uint8_t* (J1939_PGNs);
    static bool bEnableEngineStart;
    static bool bExecutePreheat;

    J1939APP(HAL_Manager &hal, CFGC &cfgc, CFGZ &cfgz, ENGINE_MONITORING &_engineMonitoring,  GCU_ALARMS &gcuAlarm,
            AUTO_MODE  &Automode /*, EGOV &egov */);
    void Update(bool bDeviceInconfig);
    void InitAfterConfigChange();
    void ClearAllPGNsDataBuffs(void);

    void GenerateFrame(uint8_t u8PGNNum);
    void GetPGN(uint8_t ubyPGN, uint8_t u8RxOrTx, J1939_PGN_DB_t *pstGetPGN );

    void TransmitFrame(uint8_t u8PGNNum);
    void FillJ1939PGNDataFrame(void);
    void ClerAllPGNsDataBuffs(void);
    void Operation(void);
    void LoadData(uint8_t u8PGNNum, uint8_t au8SPNDataBuf[8]);
    void GetSPN(uint8_t ubyPGN, uint8_t ubySPN, uint8_t u8TxOrRX, J1939_SPN_DB_t * pstGetSpn);
    void ExtractReadFrame();
    DATABASE_RX_PGN_LIST_t GetRXPGNEnum(uint32_t u32ReceviedPgnNo);
    static void ClearCAN_Alarms(void);
    bool GetLampStatus(LAMP_st eLamp);

    static void TxIsr(void);
    bool IsJ1939CommEnable(void);
    bool IsCommunicationFail(void);
    bool IsDM1PGNRecieved(void);
    double GetReadData(DATABASE_RX_PGN_LIST_t ePgn, uint8_t u8SpnNum);

    uint8_t GetDm1MsgCount(void);

    uint8_t GetDm2MsgCount(void);

    void UpdateEngineStartStopDecisions(void);
    static bool IsEngineStartEnableFromPGN_65252(void);
    static bool IsExecutePreheatFromPGN_64966(void);
    LAMP_FLASHING_SEQ_t GetLampFlashingSequence(LAMP_st);
    J1939_DM_MSG_DECODE GetDM1Message(uint8_t u8MessageNum);

    J1939_DM_MSG_DECODE GetDM2Message(uint8_t u8MessageNum);
    uint8_t GetPCDAlarmCount(void);
    uint8_t GetNCDAlarmCount(void);
    uint8_t GetSPNErrorStatus(uint8_t u8PGNName, uint8_t u8SPNName);
    void ResetLampsStatus(void);
    void RequestDM11PGN();
    void ClearDM2Messages();
    void RequestDM2Messages();

   void SetDm1MsgCount(uint8_t u8Count);
   void SetDm2MsgCount(uint8_t u8Count);

    uint8_t IsRedLampON()     ;
    uint8_t IsAmberLampON()   ;
    uint8_t IsMilLampON()     ;
    uint8_t IsProtectLampON() ;
    void ResetDEFInducementStatus();
    void UpdateDEFInducementStrategy();
    void CommonAlarmBeeping();

    bool IsBeepOffTimerExpired();
    bool IsBeepOnTimerExpired();
    bool IsFaultCodeReceived(uint32_t u32SPNNo , uint8_t u8FMI);
    void UpdateInducementFlags(void);
    uint16_t GetSPNIndexFromStartBit(DATABASE_RX_PGN_LIST_t eRxPGN , uint16_t u16StartPos);
    uint16_t GetGenStatusRegister(void);
    bool IsEGRWarningPresent();
    bool IsEGRShutdownPresent();

private:
    #define DM1_DTC_SIZE    4U
    #define PGN_DM01        65226U
    #define PGN_DM02        65227U
    #define PGN_TPCM        60416U
    #define PGN_TPDT        60160U
    #define PGN_RQST        59904U
    #define PGN_TD          65254U

    #define PGN_100MS       100
    #define PGN_250MS       250
    #define PGN_500MS       500
    #define PGN_1SEC        1000
    #define PGN_50MS        50
    #define PGN_80MS        80
    #define PGN_20MS        20
    #define PGN_10MS        10

    typedef struct
    {
       uint8_t u8LockDataBuff;
       uint8_t au8TxDataBuffOld[8];
       uint8_t au8TxDataBuffUpdtd[8];
     }J1939_TXDATABUFF_SHRD_OBJCT;

    HAL_Manager       &_hal;
    CFGC              &_ObjCfgc;
    CFGZ              &_cfgz;
    ENGINE_MONITORING &_engMon;
    GCU_ALARMS        &_gcuAlarm;
    AUTO_MODE         &_Automode;

    J1939_PGNs ubypReadTxPgns[TX_LAST_PGN];
    J1939_PGNs ubypReadRxPgns[RX_PGN_LAST];

    float f32PGN_65014Data[4];
    float f32PGN_65011Data[4];
    float f32PGN_65008Data[4];
    float f32PGN_65030Data[4];
    float f32PGN_65027Data[4];
    float f32PGN_65026Data[2];
    float f32PGN_65025Data[3];
    float f32PGN_65024Data[4];
    float f32PGN_65023Data[2];
    float f32PGN_65022Data[3];
    float f32PGN_65021Data[4];
    float f32PGN_65020Data[2];
    float f32PGN_65019Data[3];
    float f32PGN_65018Data[2];
    float f32PGN_65029Data[2];
    float f32PGN_65028Data[3];
    float f32PGN_64911Data[3];
    float f32PGN_65280Data[17];
    float f32PGN_65289Data[32];
    float f32PGN_65290Data[4];
    float f32PGN_65291Data[16];
    float f32PGN_65292Data[16];
    float f32PGN_65293Data[16];
    float f32PGN_65294Data[16];
    float f32PGN_65295Data[4];
    float f32PGN_65296Data[2];
    float f32PGN_65297Data[2];

    volatile float* PGN_DataFrames[TX_LAST_PGN] =
    {
        f32PGN_65014Data,
        f32PGN_65011Data,
        f32PGN_65008Data,
        f32PGN_65030Data,
        f32PGN_65027Data,
        f32PGN_65026Data,
        f32PGN_65025Data,
        f32PGN_65024Data,
        f32PGN_65023Data,
        f32PGN_65022Data,
        f32PGN_65021Data,
        f32PGN_65020Data,
        f32PGN_65019Data,
        f32PGN_65018Data,
        f32PGN_65029Data,
        f32PGN_65028Data,
        f32PGN_64911Data,
        f32PGN_65280Data,
        f32PGN_65289Data,
        f32PGN_65290Data,
        f32PGN_65291Data,
        f32PGN_65292Data,
        f32PGN_65293Data,
        f32PGN_65294Data,
        f32PGN_65295Data,
        f32PGN_65296Data,
        f32PGN_65297Data,
    };


    typedef enum
    {
        ALARM_BYTE_0,
        ALARM_BYTE_1,
        ALARM_BYTE_2,
        ALARM_BYTE_3,
        ALARM_BYTE_4,
        ALARM_BYTE_5,
        ALARM_BYTE_6,
        ALARM_BYTE_7,
        ALARM_BYTE_8,
        ALARM_BYTE_9,
        ALARM_BYTE_10,
        ALARM_BYTE_11,
        ALARM_BYTE_12,
        ALARM_BYTE_13,
        ALARM_BYTE_14,
        ALARM_BYTE_15
    }ALARM_BYTE_POS_t;

    uint8_t _au8CalcSPNData[8];
    J1939_TXDATABUFF_SHRD_OBJCT _astPGNTxDataBuff[TX_LAST_PGN];
    uint32_t _u32RequestedPGN;
    uint8_t _u8PGN_Num;
    uint8_t _u8NumOfDM1SPN;
    uint8_t _u8NumOfDM2SPN;
    uint8_t _u8NoOfInvalidSpnInDm1Msg;
    uint8_t _u8NoOfInvalidSpnInDm2Msg;

    CFGZ::CFGZ_PARAMS_t _ConfigParam;

    bool _bDeviceInConfigMode;


    stTimer _Timer10ms;
    stTimer _Timer20ms;
    stTimer _Timer50ms;
    stTimer _Timer80ms;
    stTimer _Timer100ms;
    stTimer _Timer250ms;
    stTimer _Timer500ms;
    stTimer _Timer1s;
    stTimer _CommFailTimeout;

    uint8_t _u8ArrDM1SPNData[250];
    uint8_t _u8ArrDM2SPNData[250];
    uint8_t _u8SPNIndexInRQSTPGN;
    uint8_t _ArrSpnErrorNAStatus[RX_PGN_LAST][MAX_NO_SPN_IN_PGN];
    double _ArrPgnReadData[RX_PGN_LAST][MAX_NO_SPN_IN_PGN];


    uint8_t _u8ActiveDtcAmberLampStatus;
    uint8_t _u8ActiveDtcProtectLampStatus;
    uint8_t _u8ActiveDtcRedLampStatus;
    uint8_t _u8ActiveDtcMILLampStatus ;
    uint8_t _u8ProtectLampFlashState;
    uint8_t _u8AmbergLampFlashState;
    uint8_t _u8RedLampFlashState;
    uint8_t _u8MalFunctionLampFlashState;
    bool    _bIsDM1PGNRecieved;
    bool    _bIsCANJ1939CommFail;
    bool    _bClearActiveDTCs;
    bool    _bRequestDM2PGN;
    bool    _bRequestDM11PGN;
    /*
     * _bIsDEFLevelLow will be true when we receive following SPN and FMI
     * combination i.e DTC , SPN = 1761 , FMI = 18 (PCode :P202F).
     * This DTC indicates that DEF level is low (2.92 % remaining).
     * These will be cleared when DTC are cleared*/
    bool    _bIsDEFLevelLow;
    /*
     * _bIsDEFLevelSevere will be true when we receive following SPN and FMI
     * combination i.e DTC , SPN = 1761 , FMI = 31 (PCode :P2BA7).
     * This DTC indicates that DEF level is severe (0.15 % remaining)
     *  These will be cleared when DTC are cleared**/
    bool    _bIsDEFLevelSevere;
    stTimer _BeepOnTimer;
    stTimer _BeepOffTimer;
    J1939_DM_MSG_DECODE _stDm1Decode[TOTAL_NO_OF_SPNS];
    J1939_DM_MSG_DECODE _stDm2Decode[TOTAL_NO_OF_SPNS];

    void prvUpdatePGN65014Data(void);
    void prvUpdatePGN65011Data(void);
    void prvUpdatePGN65008Data(void);
    void prvUpdatePGN65030Data(void);
    void prvUpdatePGN65027Data(void);
    void prvUpdatePGN65026Data(void);
    void prvUpdatePGN65025Data(void);
    void prvUpdatePGN65024Data(void);
    void prvUpdatePGN65023Data(void);
    void prvUpdatePGN65022Data(void);
    void prvUpdatePGN65021Data(void);
    void prvUpdatePGN65020Data(void);
    void prvUpdatePGN65019Data(void);
    void prvUpdatePGN65018Data(void);
    void prvUpdatePGN65029Data(void);
    void prvUpdatePGN65028Data(void);
    void prvUpdatePGN64911Data(void);
    void prvUpdatePGN65280Data(void);
    void prvUpdatePGN65289Data(void);
    void prvUpdatePGN65290Data(void);
    void prvUpdatePGN65291Data(void);
    void prvUpdatePGN65292Data(void);
    void prvUpdatePGN65293Data(void);
    void prvUpdatePGN65294Data(void);
    void prvUpdatePGN65295Data(void);
    void prvUpdatePGN65296Data(void);
    void prvUpdatePGN65297Data(void);

    typedef void(J1939APP::*UPDATE_PGNDATA_FPTR)();
    UPDATE_PGNDATA_FPTR afpUpdatePGNData[TX_LAST_PGN] =
    {
        &J1939APP::prvUpdatePGN65014Data,
        &J1939APP::prvUpdatePGN65011Data,
        &J1939APP::prvUpdatePGN65008Data,
        &J1939APP::prvUpdatePGN65030Data,
        &J1939APP::prvUpdatePGN65027Data,
        &J1939APP::prvUpdatePGN65026Data,
        &J1939APP::prvUpdatePGN65025Data,
        &J1939APP::prvUpdatePGN65024Data,
        &J1939APP::prvUpdatePGN65023Data,
        &J1939APP::prvUpdatePGN65022Data,
        &J1939APP::prvUpdatePGN65021Data,
        &J1939APP::prvUpdatePGN65020Data,
        &J1939APP::prvUpdatePGN65019Data,
        &J1939APP::prvUpdatePGN65018Data,
        &J1939APP::prvUpdatePGN65029Data,
        &J1939APP::prvUpdatePGN65028Data,
        &J1939APP::prvUpdatePGN64911Data,
        &J1939APP::prvUpdatePGN65280Data,
        &J1939APP::prvUpdatePGN65289Data,
        &J1939APP::prvUpdatePGN65290Data,
        &J1939APP::prvUpdatePGN65291Data,
        &J1939APP::prvUpdatePGN65292Data,
        &J1939APP::prvUpdatePGN65293Data,
        &J1939APP::prvUpdatePGN65294Data,
        &J1939APP::prvUpdatePGN65295Data,
        &J1939APP::prvUpdatePGN65296Data,
        &J1939APP::prvUpdatePGN65297Data,
    };

   void prvUpdatePGN59904Data(uint8_t *au8CalSPNData);
   void prvExractDM1SPN(void);
   void prvAssignDM1SPNVal(uint32_t u32LocalDM1SPN, uint8_t u8LocalFMI);
   static void UpdateDM2RequestData(uint8_t *au8CalSPNData);
   static void UpdateDM11RequestData(uint8_t *au8CalSPNData);
   void prvExtractDmMsg(uint16_t u16DmMsgNo, uint8_t u8NoOfSpnInDmMsg);
   void prvAssignDmMsgVal(J1939_DM_MSG_DECODE* pstDmMsgArray, uint32_t u32LocalDM1SPN, uint8_t u8LocalFMI, uint8_t u8LocalOC);
   /*
    * @Name    [uint32_t ExtractSPN(uint8_t *pCANData,uint8_t ubyStartBit,
                uint8_t ubyNumberOfBit)]
    * @brief   [This function will extract data of particular SPN from received can frame data.]
    * @pre     [Call this function after data receive]
    * @param   [1. Received CAN data buffer pointer
    *           2. Start Bit of that particular SPN
    *           3. Number of bits of that particular SPN]
    * @post    [After this function call J1939ValidateSPN function]
    * @return  [Extracted SPN data value]
    * @Author  [Madhuri Abhang]
    *
    */
   uint32_t prvExtractSPN(uint8_t *pCANData, uint8_t ubyPGN, uint8_t ubySPN, uint16_t ubyStartBit,uint8_t ubyNumberOfBit);

   void UpdateAlarmRegValue(uint8_t u8AlarmID, uint8_t u8Offset , float* f32PGN);
   void UpdateAlarmRegStatus(uint8_t u8AlarmID,  uint8_t u8Offset , float* f32PGN);
   void UpdateDGVoltAlarms( uint8_t u8WarningID, uint8_t u8ShutdownID, uint8_t u8Offset , float* f32PGN);
   void UpdateEngSensorAlarms( uint8_t u8SensorID,uint8_t u8SwitchID, uint8_t u8Offset , float* f32PGN);
   void prvUpdateDGVoltAlarmsInAnyPhase( uint8_t u8WarningID, uint8_t u8ShutdownID, uint8_t u8Offset , float* f32PGN);

   bool _bIsEGRInducementWarning;
   bool _bIsEGRInducementShutdown;
};

#endif /* APL_J1939APP_J1939APP_H_ */
