/*
 * CONST_UI.h
 *
 *  Created on: 11-Jul-2021
 *      Author: madhuri.abhang
 */

#ifndef APL_HMI_UI_CONST_UI_H_
#define APL_HMI_UI_CONST_UI_H_

#include "stdint.h"
#include "GCU_ALARMS.h"
#include "CFGZ.h"
#include "J1939_APP/J1939_Database.h"
extern const uint8_t u8BmpLogo[8][32];
extern const uint8_t u8BmpLogoInfo[3][20];
extern const uint8_t u8BmpLogoGen[3][20];
extern const uint8_t u8BmpLogoMains[3][20];
extern const uint8_t u8BmpLogoEng[3][20];
extern const uint8_t u8BmpLogoLoad[3][20];
extern const char *strTimerStatus[2][14];
extern const char *strGCUStatus[2][10];
extern const char *strMonScreens[2][32];
extern const char *strGCUMode[2][5];
extern const char *strIDLMode[];
extern const uint8_t u8ArrContactor1[6][16];
extern const uint8_t u8ArrContactor2[3][16];
extern const uint8_t u8ArrContactor3[3][16];
extern const uint8_t u8ArrContactor4[1][10];
extern const uint8_t u8ArrDollar[4][32];
extern const uint8_t u8ArrBattery[4][32];
extern const uint8_t u8ArrEngineTemp[4][32];
extern const uint8_t u8ArrFuel[4][32];
extern const uint8_t u8ArrOilPressure[4][32];
extern const uint8_t u8ArrEngineHours[4][32];
extern const uint8_t u8ArrEngineSpeed[4][32];
extern const uint8_t u8ArrAlarmLogo[3][20];
extern const uint8_t gau8LIShelterTemp[4][32];
extern const char *StrMonth[2][12];
extern const char *strPhase[];
extern const char *strPh_Ph[];
extern const char *strPower[];
extern const char *StrPF;
extern const char *StrA;
extern const char *strEnergy[];
extern const char *StrOpenckt[2];
extern const char *StrSTB[2];
extern const char *StrSTG;
extern const char *StrNotConfigured;
extern const char *StrNightModeRestrict[2];
extern const char *StrAutoExercise1[2];
extern const char *StrAutoExercise2[2];
extern const char *StrStarts[2];
extern const char *StrTrips[2] ;
extern const char *StrAutoAMF[2];
extern const char *StrAutoRemoteCmd[2];

extern const char *StrNoEvents[2];
extern const char *StrInfoDate[2];
extern const char *StrMaintDate[2][2];

typedef enum
{
    STR_AUTO_EXERCISER_FREQ,
    STR_AUTO_EXERCISER_FOR,
    STR_AUTO_EXERCISER_AT,
    STR_AUTO_EXERCISER_NEXT_RUN,
    STR_AUTO_EXERCISER_DAILY,
    STR_AUTO_EXERCISER_WEEKLY,
    STR_AUTO_EXERCISER_MONTHLY,
    STR_AUTO_EXERCISER_LAST
}STR_AUTO_EXERCISER_t;
extern const char *StrAutoExerciser[2][STR_AUTO_EXERCISER_LAST];
typedef enum
{
    STR_MAINS_NOT_READ,
    STR_MAINS_PARTIAL_HEALTHY,
    STR_MAINS_HEALTHY,
    STR_MAINS_SEQ_FAIL,
    STR_MAINS_FAILED,
    STR_MAINS_STATUS_LAST
}MAINS_STATUS_t;

extern const char *StrMainsStatus[2][STR_MAINS_STATUS_LAST];
extern const char *StrEICViewMessage[2];
extern const char *StrOpnGndckt[2];
extern const char *StrEngineType[2][21];
extern const char *strCANMsgRcvError[2];
extern const char *strAlaram[][GCU_ALARMS::ID_ALL_ALARMS_LAST];
extern const char *strHystogram[];
extern const char *strNoActiveDTC[];
extern const char *strNoHistoricDTC[];
extern const char *strNoActivePCDDTC[];
extern const char *strNoActiveNCDDTC[];
extern const char *strEventLog[];
extern const char *strRunHrs[];
extern const char *strConfiguration[];

extern const char *strPasswordChanged[2];
extern const char *strPasswordAlreadyExist[2];
extern const char *strSavingSettings[2];
extern const char *StrFreq[2];
extern const char *StrFor[2];
extern const char *StrAt[2];
extern const char *StrNextRun[2];

extern const char *StrPassword[2];
extern const char *StrForReadMode[2];
extern const char *StrPressStop[2];
extern const char *StrForWriteMode[2];
extern const char *StrPressStart[2];

extern char strAuxAString[MAX_AUX_STRING_SIZE],strAuxBString[MAX_AUX_STRING_SIZE],
            strAuxCString[MAX_AUX_STRING_SIZE];
extern char strAuxDString[MAX_AUX_STRING_SIZE],strAuxEString[MAX_AUX_STRING_SIZE],
            strAuxFString[MAX_AUX_STRING_SIZE];
extern char strAuxGString[MAX_AUX_STRING_SIZE],strAuxHString[MAX_AUX_STRING_SIZE],
            strAuxIString[MAX_AUX_STRING_SIZE];
extern char strAuxJString[MAX_AUX_STRING_SIZE], strAuxKString[MAX_AUX_STRING_SIZE],
            strAuxLString[MAX_AUX_STRING_SIZE], strAuxMString[MAX_AUX_STRING_SIZE],
            strAuxNString[MAX_AUX_STRING_SIZE];
extern char strAuxOString[MAX_AUX_STRING_SIZE], strAuxPString[MAX_AUX_STRING_SIZE];
extern char strAuxS1String[MAX_AUX_STRING_SIZE], strAuxS2String[MAX_AUX_STRING_SIZE],
            strAuxS3String[MAX_AUX_STRING_SIZE], strAuxS4String[MAX_AUX_STRING_SIZE];
extern char strProfile[MAX_AUX_STRING_SIZE];


extern const uint8_t u8ImagesChiniMonScreen[][14][16];
extern const uint8_t u8ChiniAlarmScreen[4][14];
extern const uint8_t gau8ChineseMainsAbnormal[9][16] ;
extern const uint8_t  gau8ChineseMainsSequenceFailed[12][16];
extern const uint8_t gau8ChineseMainsHealthy[9][16];
extern const uint8_t gau8ChineseMainsFailed[9][16];
extern const uint8_t gau8ChineseMainsNotRead[10][14];

typedef struct
{
    uint8_t gau8ChAlarm_Pin24CktOpen[13][16];       /// 168 Bytes
    uint8_t gau8ChAlarm_HighEngTemp[7][16];         /// 252 Bytes
    uint8_t gau8ChAlarm_FuelLevelCktOpen[13][16];   /// 221 Bytes
    uint8_t gau8ChAlarm_Switch[6][15];              /// 090 Bytes
    uint8_t gau8ChAlarm_LOP[7][16];                 /// 209 Bytes
    uint8_t gau8ChAlarm_LowFuelLevel[10][17];       /// 170 Bytes
    uint8_t gau8ChAlarm_Pin26CktOpen[13][16];       /// 224 Bytes
    uint8_t gau8ChAlarm_MainsPhaseReversed[11][16]; /// 084 Bytes
    uint8_t gau8ChAlarm_LowWaterLevelSwitch[11][16];/// 135 Bytes
    uint8_t gau8ChAlarm_UnderSpeed[5][16];          /// 042 Bytes
    uint8_t gau8ChAlarm_OverSpeed[9][16];           /// 056 Bytes
    uint8_t gau8ChAlarm_GrossOverSpeed[9][16];      /// 112 Bytes
    uint8_t gau8ChAlarm_L1_PhaseOverVoltage[11][16]; ///126 Bytes
    uint8_t gau8ChAlarm_L1_PhaseUnderVoltage[11][16];///154 Bytes
    uint8_t gau8ChAlarm_L2_PhaseOverVoltage[11][16]; ///126 Bytes
    uint8_t gau8ChAlarm_L2_PhaseUnderVoltage[11][16];///154 Bytes
    uint8_t gau8ChAlarm_L3_PhaseOverVoltage[11][16]; ///126 Bytes
    uint8_t gau8ChAlarm_L3_PhaseUnderVoltage[11][16];///154 Bytes
    uint8_t gau8ChAlarm_BatteryOverVoltage[11][16]; /// 112 Bytes
    uint8_t gau8ChAlarm_BatteryUnderVoltage[11][16];/// 112 Bytes
    uint8_t gau8ChAlarm_UnderFrequency[5][16];      /// 060 Bytes
    uint8_t gau8ChAlarm_OverFrequency[5][16];       /// 060 Bytes
    uint8_t gau8ChAlarm_EmergencyStop[9][16];       /// 140 Bytes
    uint8_t gau8ChAlarm_ChargeFail[9][16];          /// 112 Bytes
    uint8_t gau8ChAlarm_MaintenanceDue[9][16];      /// 112 Bytes
    uint8_t gau8ChAlarm_FailToStop[9][16];          /// 153 Bytes
    uint8_t gau8ChAlarm_FailToStart[9][16];         /// 170 Bytes
    uint8_t gau8ChAlarm_FuelTheft[9][16];           /// 208 Bytes
    uint8_t gau8ChAlarm_HighOilPressure[11][16];    /// 176 Bytes
    uint8_t gau8ChAlarm_DGPhaseReversed[12][16];    /// 195 Bytes
    uint8_t gau8ChAlarm_Sensor[6][16];              /// 096 Bytes
    uint8_t gau8ChAlarm_NoAlarms[7][16];            /// 160 Bytes
    uint8_t gau8ChAlarm_OverCurrent[7][16];         /// 070 Bytes
    uint8_t gau8ChAlarm_OverLoad[5][16];            /// 060 Bytes
    uint8_t gau8ChAlarm_NoSpeedSignal[12][16];      /// 126 Bytes
    uint8_t gau8ChAlarm_UnbalancedLoad[11][16];     /// 187 Bytes
    uint8_t gau8ChAlarm_VBeltBroken[12][16];        /// 238 Bytes
    uint8_t gau8ChAlarm_Pin23CktOpen[14][16];       /// 224 Bytes
    uint8_t gau8ChAlarm_ShrtToBatt[14][14];         /// 196 Bytes
}CHINI_ALARM_st;


extern const CHINI_ALARM_st stChiniAlarm;


typedef struct
{
    uint8_t gau8ChineseEngineOffReady[14][14];
    uint8_t gau8ChineseStartingAttempts[12][16];
    uint8_t gau8ChineseEngineOnHealthy[10][14];
    uint8_t gau8ChineseEngineStopping[10][14];
    uint8_t gau8ChineseNotificationAlarm[12][14];
    uint8_t gau8ChineseWarningAlarm[11][14];
    uint8_t gau8ChineseElectricalTripAlarm[11][14];
    uint8_t gau8ChineseShutdownAlarm[15][13];
}CHINI_ENG_STATE_st;
typedef struct
{
    uint8_t gau8ChineseCranking[6][12];
    uint8_t gau8ChineseSafetyMonitoring[7][12];
    uint8_t gau8ChineseAdditionalStop[8][12];
    uint8_t gau8ChineseCrankRest[6][13];
    uint8_t gau8ChineseCoolDown[7][11];
    uint8_t gau8ChineseStopAction[4][11];
    uint8_t gau8ChineseStartDelay[6][12];
    uint8_t gau8ChinesePreheating[6][12];
}CHINI_TMR_st;

extern const uint8_t gau8ChineseNone[3][12];
extern const CHINI_ENG_STATE_st stChiniEngStatus;
extern const CHINI_TMR_st stChiniTmr;
extern const uint8_t u8bmpChiniManualMode[6][12];
extern const uint8_t  gau8ChineseAutoAMFMode[4][12];
extern const uint8_t gau8ChineseEngineBattery[8][14];
extern const uint8_t gau8ChineseSiteBattery[8][13];
extern const uint8_t  gau8ChineseNumOfTrips[8][14];
extern const uint8_t  gau8ChineseNumOfStarts[8][14];
extern const uint8_t gau8ChineseCirucitOpen[4][13];
extern const uint8_t gau8ChineseCirucitOpenOrGround[9][13];
extern const uint8_t gau8ChineseForReadMode[8][14] ;
extern const uint8_t gau8ChinesePresStop[8][14];
extern const uint8_t gau8ChineseForWriteMode[8][13];
extern const uint8_t gau8ChinesePressStart[8][14];
extern const uint8_t gau8ChinesePassword[4][13];
extern const char *StrDays[2][7];


extern uint8_t gau8Icon0[4][24] ;
extern uint8_t gau8Icon1[5][22] ;
extern uint8_t gau8Icon2[5][24] ;
extern uint8_t gau8Icon3[4][27] ;
extern uint8_t gau8Icon4[4][20] ;
extern uint8_t gau8Icon6[6][27] ;
extern uint8_t gau8Icon7[6][27] ;
extern uint8_t gau8Icon8[6][27] ;



typedef struct
{
    uint32_t u32SpnNo;
    char*    pSpnName;
    char*    pSpanishSpnName;
}J1939_SPN_NAME_STRING;
extern const J1939_SPN_NAME_STRING gstDmNoString[NO_OF_SPNS_IN_DM];
extern const char SPNStringTable[SPN_LAST][SPN_NAME_MAX_LEN];
extern const char SpanishSPNStringTable[SPN_LAST][SPN_NAME_MAX_LEN];
extern const PGN_SUB_HEADINGS_t  gstPgnSubHeadings[2][MAX_NUM_OF_PGN_SUB_HEADINGS];
extern const  ENG_OPERATION_STATES_t   gstEngOperatingState[2][MAX_NUM_OF_ENG_OPER_STATES];
extern const DM01_FMI_STRINGS_t gstDM1FmiData[2][MAX_NUM_OF_FMIS];
extern const PCD_NCD_ALARMS_DATA_t    gstPcdNcdData[2][MAX_NUM_OF_PCD_NCD_ALARMS];
extern const NCD_INDUCE_STRATEGTY_t   gstNcdInduceData[2][MAX_NUM_OF_NCD_INDUCE_STRAT];
extern const char* arrPGNAbbreviations[2][NUMBER_OF_PGN_SCREENS];

extern const char *Error;
extern const char * const array_units[];
extern const char *NotAvailable;
extern const char *strAlarms[];
extern const char *Approaching ;
 extern const char *NotApproaching;
extern const char *Yes[2];
extern const char *No ;
extern const uint8_t u8ArrMatrixDispAndRXPGN[];
extern const char *strCharging ;
extern const char *StrNotCharging ;
extern const char *StrLampIcons[2];
extern const char *StrExhaustAFT[2];
extern const char *StrOff[2];
extern const char *StrOn[2];
#endif /* APL_HMI_UI_CONST_UI_H_ */
