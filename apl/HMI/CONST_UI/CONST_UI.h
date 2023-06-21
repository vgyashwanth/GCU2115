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


#define MAX_NUM_OF_ENG_OPER_STATES  (9)
#define MAX_NUM_OF_WFI_STATES       (4)
#define MAX_NUM_OF_DRV_WARN         (8)
#define MAX_NUM_OF_DEFA_STATE       (4)
#define PGN_65252_STRING_OPTIONS    (4)
#define PGN_64966_STRING_OPTIONS    (4)

extern const uint8_t u8BmpLogoInfo[3][20];
extern const uint8_t u8BmpLogoGen[3][20];
extern const uint8_t u8BmpLogoMains[3][20];
extern const uint8_t u8BmpLogoEng[3][20];
extern const uint8_t u8BmpLogoLoad[3][20];
extern const uint8_t u8BmpLogoshelterTemp[3][20];
extern const char *strTimerStatus[1][14];
extern const char *strGCUStatus[1][10];
extern const char *strMonScreens[1][49]; /*todo : MON_UI::DISP_MON_LAST should be used here for indexing */
extern const char *strGCUMode[1][5];
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

extern const uint8_t gau8GeneratorVoltLogo[4][32];
extern const uint8_t gau8GenPFLogo[4][30];

extern const char *StrMonth[1][12];
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
extern const char *StrMaintDate[1][2];

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
extern const char *StrAutoExerciser[1][STR_AUTO_EXERCISER_LAST];
typedef enum
{
    STR_MAINS_NOT_READ,
    STR_MAINS_PARTIAL_HEALTHY,
    STR_MAINS_HEALTHY,
    STR_MAINS_SEQ_FAIL,
    STR_MAINS_FAILED,
    STR_MAINS_STATUS_LAST
}MAINS_STATUS_t;

extern const char *StrMainsStatus[1][STR_MAINS_STATUS_LAST];
extern const char *StrEICViewMessage[2];
extern const char *StrOpnGndckt[2];
extern const char *StrEngineType[1][21];
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

extern const char *StrDays[1][7];

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
/*
extern const J1939_SPN_NAME_STRING gstDmNoString[NO_OF_SPNS_IN_DM];
extern const char SPNStringTable[SPN_LAST][SPN_NAME_MAX_LEN];
extern const char SpanishSPNStringTable[SPN_LAST][SPN_NAME_MAX_LEN];
extern const PGN_SUB_HEADINGS_t  gstPgnSubHeadings[1][MAX_NUM_OF_PGN_SUB_HEADINGS];
extern const  ENG_OPERATION_STATES_t   gstEngOperatingState[1][MAX_NUM_OF_ENG_OPER_STATES];
extern const DM01_FMI_STRINGS_t gstDM1FmiData[1][MAX_NUM_OF_FMIS];
extern const PCD_NCD_ALARMS_DATA_t    gstPcdNcdData[1][MAX_NUM_OF_PCD_NCD_ALARMS];
extern const NCD_INDUCE_STRATEGTY_t   gstNcdInduceData[1][MAX_NUM_OF_NCD_INDUCE_STRAT];
extern const char* arrPGNAbbreviations[1][NUMBER_OF_PGN_SCREENS];
*/

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
extern const char *StrNotAvailable;
extern const char *StrCANErr;
extern const char *StrErr;

extern const char *EngType[];

extern const char *StrWFIState[MAX_NUM_OF_WFI_STATES];
extern const char *StrDrvWarnState[MAX_NUM_OF_DRV_WARN];
extern const char *StrDEFAStatus[MAX_NUM_OF_DEFA_STATE];
extern const char *StrEngOperatingState[MAX_NUM_OF_ENG_OPER_STATES];
extern const char *StrPGN_65252_states[PGN_65252_STRING_OPTIONS];
extern const char *StrPGN_64966_states[PGN_64966_STRING_OPTIONS];
/* To store DTC data */

typedef struct
{
    uint32_t u32spn_no;
    uint8_t  u8fmi;
    char*    FaultCode;
    char*    Description;
} stCNG_15KVA_DTC;


#define PCODE_LAST_CNG_15KVA 38 //added for testing need to remove after getting exact requirement


extern const stCNG_15KVA_DTC J1939AlarmArrayStringsCNG_15KVA[PCODE_LAST_CNG_15KVA];
#endif /* APL_HMI_UI_CONST_UI_H_ */
