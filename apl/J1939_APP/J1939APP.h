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
//#include "CEditableItem.h"
#include "CFGC/CFGC.h"
#include "config.h"
#include "HAL_Manager.h"
#include "MB_APP.h"
#include "../GENSET_CONTROL/ENGINE_MONITORING/ENGINE_MONITORING.h"

class J1939APP: public J1939DRIVER
{

public:
    #define IS_START_STOP_RELAY_J1939_CONFIGURED()   0
//    ((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE)!=CFGZ::ENG_CONVENTIONAL) && 1)

//    _cfgz.GetCFGZ_Param(CFGZ::ID_START_STOP_TO_ECU))


   #define IS_PREHEAT_J1939_CONFIGURED()          0
//    ((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE)!=CFGZ::ENG_CONVENTIONAL)  && 1)

//    (_cfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_TO_ECU)))

   #define IS_ENG_TEMP_J1939_CONFIGURED()         0
//    ((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE)!=CFGZ::ENG_CONVENTIONAL) \
                                                 && (1))
//                                                 && (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG)))
   #define IS_ENG_LOP_J1939_CONFIGURED()         0
//    ((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE)!=CFGZ::ENG_CONVENTIONAL) \
		                                         && (1))
//                                                && (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG)))

    #define     DPFC1_PGN           (64892)
    #define     EEC1_PGN            (61444)
    #define     LFE_PGN             (65266)
    #define     EEC2_PGN            (61443)
    #define     EFL_P1_PGN          (65263)
    #define     IC1_PGN             (65270)
    #define     ET1_PGN             (65262)
    #define     AMB_PGN             (65269)
    #define     HOURS_PGN           (65253)
    #define     VEP1_PGN            (65271)
    #define     DD_PGN              (65276)
    #define     WFI_PGN             (65279)
    #define     LFC_PGN             (65257)
    #define     EEC4_PGN            (65214)
    #define     LFI_PGN             (65203)
    #define     S2_PGN              (65166)
    #define     IC2_PGN             (64976)
    #define     EOI_PGN             (64914)
    #define     ET2_PGN             (65188)
    #define     EEC3_PGN            (65247)
    #define     SHUTDOWN_PGN        (65252)
    #define     EFG1_PGN            (61450)
    #define     DPF1S_PGN           (64796)
    #define     A1DOC_PGN           (64800)
    #define     AT1IG2_PGN          (64948)
    #define     AT1IMG_PGN          (64946)
    #define     EFL_P2_PGN          (65243)
    #define     AT1T1I_PGN          (65110)
    #define     AT1IG1_PGN          (61454)
    #define     AT1OG1_PGN          (61455)
    #define     A1SCRDSI1_PGN       (61475)
    #define     A1SCRDSI2_PGN       (64833)
    #define     A1SCRDSR1_PGN       (61476)
    #define     A1SCREGT1_PGN       (64830)
    #define     AT2IG1_PGN          (61456)
    #define     AT2OG1_PGN          (61457)
    #define     A2SCRDSI2_PGN       (64827)
    #define     A2SCRDSI1_PGN       (61478)
    #define     A2SCRDSR1_PGN       (61479)
    #define     A2SCREGT1_PGN       (64824)
    #define     AT1OG2_PGN          (64947)
    #define     AT1S2_PGN           (64697)
    #define     AT1S_PGN            (64891)
    #define     EMR_PREHEAT_PGN     (65284)
    #define     PROPB_62_PGN        (65378)
    #define     PROPB_51_PGN        (65361)
    #define     PROPB_57_PGN        (65367)
    #define     IVECO_PREHEAT_PGN   (65281)
    #define     IVECO_ENG_STATUS_PGN (65282)
    #define     VOLVO_PREHEAT_PGN   (65351)
    #define     PROPB_5E_PGN        (65374)
    #define     DM11_RESET_DM01     (65235)
    #define     PROPB32_KUBOTA_PGN  (65330)
    #define     PROSTOUT_PGN        (65364)
    #define     PROPB00_PGN         (65280)
    #define     AI_PGN              (65237)
    #define     ETC5_PGN            (65219)
    #define     OII_PGN             (64554)
    #define     CCVS_PGN            (65265)
    #define     IT1_PGN             (65154)
    #define     GFP_PGN             (65163)
    #define     IMI1_PGN            (65190)
    #define     FD1_PGN             (65213)
    #define     DLCC1_PGN           (64775)
    #define     GFC_PGN             (65199)
    #define     EPT1_PGN            (65187)
    #define     EPT2_PGN            (65186)
    #define     EPT3_PGN            (65185)
    #define     EPT4_PGN            (65184)
    #define     EPT5_PGN            (65183)
    #define     ET4_PGN             (64870)
    #define     TCI4_PGN            (65176)
    #define     EFL_P12_PGN         (64735)
    #define     EGOV_PID_GAINS_PGN  (65285)
    #define     EGOV_PARAMS_PGN     (65286)
    #define     EGOV_APPL_MODE_PGN  (65287)

    #define MAX_NO_SPN_IN_PGN                    (12U)
    typedef void(J1939APP::*UPDATE_PGNDATA_FPTR)();
    UPDATE_PGNDATA_FPTR afpUpdatePGNData[TX_LAST_PGN] =
    {
        &J1939APP::prvUpdatePGN64915Data,
        &J1939APP::prvUpdatePGN65408Data,
        &J1939APP::prvUpdatePGN65350Data,
        &J1939APP::prvUpdatePGN0Data,
        &J1939APP::prvUpdatePGN65365Data,
        &J1939APP::prvUpdatePGN65281Data,
        &J1939APP::prvUpdatePGN65302Data,
        &J1939APP::prvUpdatePGN65280Data,
        &J1939APP::prvUpdatePGN65527Data,
        &J1939APP::prvUpdatePGN65385Data,
        &J1939APP::prvUpdatePGN65395Data,
        &J1939APP::prvUpdatePGN65406Data,
        &J1939APP::prvUpdatePGN57344Data,
        &J1939APP::prvUpdatePGN65219Data,
        &J1939APP::prvUpdatePGN65363Data,
        &J1939APP::prvUpdatePGN65265Data,
        &J1939APP::prvUpdatePGN65288Data,
        &J1939APP::prvUpdatePGN65223Data,
        &J1939APP::prvUpdatePGN61441Data,
    };
    typedef enum
    {
        PGN_64915_REQ_ENG_CONTROL_MODE = 0,
        PGN_64915_AUTO_MODE,
        PGN_64915_PARALLEL_MODE,
        PGN_64915_UNUSED_SPN_1,
        PGN_64915_UNUSED_SPN_2,
        PGN_64915_UNUSED_SPN_3,
        PGN_64915_UNUSED_SPN_4,
        PGN_64915_TOTAL_BYTE
    }PGN_64915_CMND_t;

    typedef enum
    {
        PGN_65408_HARD_CODE_VAL = 0,
        PGN_65408_UNUSED_SPN_1,
        PGN_65408_UNUSED_SPN_2,
        PGN_65408_ENG_START_BYTE,
        PGN_65408_UNUSED_SPN_3,
        PGN_65408_ENG_STOP_BYTE,
        PGN_65408_UNUSED_SPN_4,
        PGN_65408_UNUSED_SPN_5,
        PGN_65408_UNUSED_SPN_6,
        PGN_65408_TOTAL_BYTE
    }PGN_65408_CMND_t;
    typedef enum
    {
        NORMAOL_ENGINE_SHUTDOWN = 0,
        RAPID_ENGINE_SHUTDOWN,
        EMERGENCY_ENGINE_SHUTDOWN,
        NORMAL_ENGINE_START,
        NO_ACTION_DONT_CARE = 0
    }GC1_PGN_ENG_CMNDs_t;
    typedef enum
    {
        PGN_65350_ENG_START_BYTE = 0,
        PGN_65350_ENG_STOP_BYTE,
        PGN_65350_UNUSED_SPN_1,
        PGN_65350_UNUSED_SPN_2,
        PGN_65350_PREHEAT_BYTE,
        PGN_65350_UNUSED_SPN_3,
        PGN_65350_ACCELERATOR_PEDAL_POS,
        PGN_65350_FUEL_DISABLE_BYTE,
        PGN_65350_UNUSED_SPN_5,
        PGN_65350_ACCELERATOR_PEDAL_COUNTER,
        PGN_65350_ACCELERATOR_PEDAL_CHECKSUM,
        PGN_65350_UNUSED_SPN_6,
        PGN_65350_UNUSED_SPN_7,
        PGN_65350_TOTAL_BYTE
    }PGN_65350_CMND_t;
    typedef enum
    {
        PGN_65281_CAN_REQ_ENG_STOP = 0,
        PGN_65281_CAN_ENG_START,
        PGN_65281_UNUSED_SPN_1,
        PGN_65281_UNUSED_SPN_2,
        PGN_65281_UNUSED_SPN_3,
        PGN_65281_UNUSED_SPN_4,
        PGN_65281_UNUSED_SPN_5,
        PGN_65281_UNUSED_SPN_6,
        PGN_65281_UNUSED_SPN_7,
        PGN_65281_TOTAL_BYTE
    }PGN_65281_CMND_t;

    typedef enum
    {
        PGN_65302_ENG_STOP_BYTE = 0,
        PGN_65302_UNUSED_SPN_1,
        PGN_65302_UNUSED_SPN_2,
        PGN_65302_UNUSED_SPN_3,
        PGN_65302_UNUSED_SPN_4,
        PGN_65302_UNUSED_SPN_5,
        PGN_65302_TOTAL_BYTE
    }PGN_65302_CMND_t;

    typedef enum
    {
        PGN_65280_ENG_START_STOP_BYTE = 0,
        PGN_65280_UNUSED_SPN_1,
        PGN_65280_UNUSED_SPN_2,
        PGN_65280_UNUSED_SPN_3,
        PGN_65280_UNUSED_SPN_4,
        PGN_65280_TOTAL_BYTE
    }PGN_65280_CMND_t;

    /* General Control2 - for Cummins engine Protocol which uses source address:0xDC
     * PGN : 61470
     * SPN : 3938 */
    typedef enum
    {
        PGN_61470_GEN_EGOV_BIAS = 0,
        PGN_61470_UNUSED_SPN_1,
        PGN_61470_UNUSED_SPN_2,
        PGN_61470_UNUSED_SPN_3,
        PGN_61470_TOTAL_BYTE
    }PGN_61470_CMND_t;

    /* Cummins Engine governing - Similar to SPN:3938 except priority is "0"
     * PGN : 65385
     * SPN : NA */
    typedef enum
    {
        PGN_65385_CUMMINS_GOVERNING_RQST = 0,
        PGN_65385_UNUSED_SPN_1,
        PGN_65385_UNUSED_SPN_2,
        PGN_65385_UNUSED_SPN_3,
        PGN_65385_TOTAL_BYTE
    }PGN_65385_CMND_t;


    typedef enum
    {
       PGN_65406_CUMMINS_DROOP = 0,
       PGN_65406_CUMMINS_RES,
       PGN_65406_CUMMINS_FREQ_ADJUSTMENT,
       PGN_65406_CUMMINS_GAIN,
       PGN_65406_TOTAL_BYTE
    }PGN_65406_CMND_t;

    typedef enum
    {
        PGN_65395_UNUSED_SPN_1 = 0,
        PGN_65395_CUMMINS_GAIN_SELECT,
        PGN_65395_CUMMINS_IDLE_SPEED,
        PGN_65395_CUMMINS_STARTER_SELECTION,
        PGN_65395_CUMMINS_SHUTDOWN_OVERRIDE,
        PGN_65395_CUMMINS_FREQUENCY_RANGE,
        PGN_65395_UNUSED_SPN_2,
        PGN_65395_UNUSED_SPN_3,
        PGN_65395_UNUSED_SPN_4,
        PGN_65395_TOTAL_BYTE
    }PGN_65395_CMND_t;

    typedef enum
    {
        PGN_57344_UNUSED_SPN_1 = 0,
        PGN_57344_UNUSED_SPN_2,
        PGN_57344_UNUSED_SPN_3,
        PGN_57344_REGEN_SW_INHIBIT,
        PGN_57344_UNUSED_SPN_4,
        PGN_57344_ENG_AUTOMATIC_STRT_EN_SW,
        PGN_57344_UNUSED_SPN_5,
        PGN_57344_TOTAL_BYTE
    }PGN_57344_CMND_t;


    typedef enum
    {
        PGN_65219_UNUSED_SPN_1 = 0,
        PGN_65219_UNUSED_SPN_2,
        PGN_65219_TRANS_NEUTRAL_SW,
        PGN_65219_UNUSED_SPN_3,
        PGN_65219_UNUSED_SPN_4,
        PGN_65219_UNUSED_SPN_5,
        PGN_65219_UNUSED_SPN_6,
        PGN_65219_TOTAL_BYTE
    }PGN_65219_CMND_t;

    typedef enum
    {
        PGN_65265_UNUSED_SPN_1 = 0,
        PGN_65265_PARKING_BRAKE_SW,
        PGN_65265_UNUSED_SPN_2,
        PGN_65265_UNUSED_SPN_3,
        PGN_65265_UNUSED_SPN_4,
        PGN_65265_UNUSED_SPN_5,
        PGN_65265_UNUSED_SPN_6,
        PGN_65265_TOTAL_BYTE
    }PGN_65265_CMND_t;

    typedef enum
    {
        PGN_65363_TARGET_ENG_SPEED = 0,
        PGN_65363_EGOV_CHARACTER,
        PGN_65363_ENGINE_STOP_INFO,
        PGN_65363_TOTAL_BYTE
    }PGN_65363_CMND_t;

    typedef enum
    {
        PGN_65288_ENG_SPEED = 0,
        PGN_65288_UNUSED_SPN_1,
        PGN_65288_UNUSED_SPN_2,
        PGN_65288_UNUSED_SPN_3,
        PGN_65288_TOTAL_BYTE
    }PGN_65288_CMND_t;

    typedef enum
    {
        ECU_NO_COMMAND = 0,
        ECU_CMND_PREHEAT_START,
        ECU_CMND_PREHEAT_STOP,
        ECU_CMND_START_HOLD,
        ECU_CMND_START_RELEASE,
        ECU_CMND_STOP_HOLD,
        ECU_CMND_STOP_RELEASE,
        ECU_TOTAL_CMNDS
    }ECU_COMMANDS_t;

    ECU_COMMANDS_t geEcuCmnds;

    typedef enum
    {
        PGN_0_ENG_OVERRIDE_CONTROL_MODE = 0,
        PGN_0_ENG_REQ_SPEED_CONTROL,
        PGN_0_OVERRIDE_CONTROL_MODE_PRIORITY,
        PGN_0_HARDCODE_VALUE,
        PGN_0_ENG_REQ_SPEED,
        PGN_0_ENG_TORQUE,
        PGN_0_TRANSMISSION_RATE,
        PGN_0_CONTROL_PURPOSE,
        PGN_0_ENG_REQ_TORQUE_FRACTION,
        PGN_0_MESSAGE_COUNTER,
        PGN_0_MESSAGE_CHECKSUM
    }PGN_0_CMND_t;

    typedef enum
    {
        PGN_65365_ENG_START_BYTE = 0,
        PGN_65365_ENG_STOP_BYTE,
        PGN_65365_UNUSED_SPN_1,
        PGN_65365_UNUSED_SPN_2,
        PGN_65365_UNUSED_SPN_3,
        PGN_65365_UNUSED_SPN_4,
        PGN_65365_UNUSED_SPN_5,
        PGN_65365_TOTAL_BYTE
    }PGN_65365_CMND_t;

    enum ID_SPN_STATUS
    {
        VALID_DATA = 0,
        ERROR,
        NOT_AVAILABLE
    };


    typedef enum
    {
        LAMP_PROTECT = 0,
        LAMP_AMBER,
        LAMP_RED,
        LAMP_MIL,
        TOTAL_LAMPS
    }ACTIVE_DTC_LAMPS_t;

    typedef enum
    {
        PCD_REMOVAL_OF_DPF_SYSTEM = 0,
        PCD_OSS_OF_DPF_SYSTEM,
        PCD_FAILURE_OF_PCD_SYSTEM,
        NCD_REMOVAL_OF_EGR_SYSTEM,
        NCD_REMOVAL_OF_MAF_SENSOR
    }PCD_NCD_ALARMS_t;

    typedef enum
    {
        ENG_OIL_PRESSURE,
        ENG_COOLANT_TEMP,
        FUEL_LEVEL,
        TOTAL_ENG_HOURS,
        BATTERY_VOLTAGE,
        INPUT_VOLTAGE,
        ENG_SPEED
    }REALDATA_st;

    typedef enum
    {
        RED_LAMP,
        AMBER_LAMP,
        MIL_LAMP,
        PROTECT_LAMP
    }LAMP_st;

    typedef struct
    {
        uint8_t u8OC;
        uint8_t u8FMI;
        uint32_t u32SpnNo;
    }J1939_DM_MSG_DECODE;


    typedef enum
    {
        COOLING_WATER_TEMP_1,
        OIL_TEMP_2,
        OIL_PRESSURE_3,
        LAST_SPN_IN_PGN_65282
    }SPN_IN_PGN_IVECO_ENG_STATUS_65282_t;



    typedef enum
    {
        BAROMETER_PRESSURE_SPN_108,
        AMB_AIR_TEMP_SPN_171,
        ENG_AIR_INLET_TEMP_SPN_172,
        LAST_SPN_IN_PGN_65269
    }SPN_IN_PGN_65269_t;

    typedef enum
    {
        ENGINE_TORQUE_SPN_899,
        DEMAND_TORQUE_SPN_512,
        ACTUAL_TORQUE_SPN_513,
        ENGINE_SPEED_SPN_190,
        SOURCE_ADDR_SPN_1483,
        ENG_STARTER_MODE_SPN_1675,
        ENG_DEMAND_TORQUE_SPN_2432,
        LAST_SPN_IN_PGN_61444
    }SPN_IN_PGN_61444_t;

    typedef enum
    {
        ENG_FUEL_PRESS_SPN_94,
        ENG_OIL_LVL_SPN_98,
        ENG_OIL_PRESS_SPN_100,
        ENG_CRANCKCASE_PRESS_SPN_101,
        ENG_COOLANT_PRESS_SPN_109,
        ENG_COOLANT_LVL_SPN_111,
        LAST_SPN_IN_PGN_65263
    }SPN_IN_PGN_65263_t;

    typedef enum
    {
        ENG_TRAP_INLET_PRESS_SPN_81,
        ENG_INTAKE_MANIFOLD_PRESS_SPN_102,
        ENG_INTAKE_MANIFOLD_TEMP_SPN_105,
        ENG_AIR_INLET_PRESS_SPN_106,
        ENG_AIR_FILT_DIFF_PRESS_SPN_107,
        ENG_EXHAUST_GAS_TEMP_SPN_173,
        ENG_COOLANT_FILT_DIFF_PRESS_SPN_112,
        LAST_SPN_IN_PGN_65270
    }SPN_IN_PGN_65270_t;

    typedef enum
    {
        DISEL_EXHAUST_TANK_LVL_SPN_1761,
        DISEL_EXHAUST_TANK_TEMP_SPN_3031,
        SCR_OPR_INDUCEMENT_ACTIVE_SPN_5245,
        SCR_OPR_INDUCEMENT_SEVERITY_SPN_5246,
        LAST_SPN_IN_PGN_65110
    }SPN_IN_PGN_65110_t;

    typedef enum
    {
        ENG_FUEL_FILT_DIFF_PRESS_SPN_95,
        ENG_OIL_FILT_DIFF_PRESS_SPN_99,
        LAST_SPN_IN_PGN_65276
    }SPN_IN_PGN_65276_t;

    typedef enum
    {
        ENG_AIR_FILT_PRESS_SPN_2809,
        ENG_MANIFOLD_ABS_PRESS_SPN_3563,
        LAST_SPN_IN_PGN_64976
    }SPN_IN_PGN_64976_t;

    typedef enum
    {
        ENG_TRIP_FUEL_SPN_182,
        ENG_TOTAL_FUEL_USED_SPN_250,
        LAST_SPN_IN_PGN_65257
    }SPN_IN_PGN_65257_t;

    typedef enum
    {
        EXHAUST_FLUID_DOSING_QUANTITY_SPN_4331,
        EXHAUST_FLUID_ABS_PRESS_SPN_4334,
        LAST_SPN_IN_PGN_61475
    }SPN_IN_PGN_61475_t;

    typedef enum
    {
        EXAUST_FLUID_ACT_DOSING_QUANTITY_SPN_4384,
        EXAUST_FLUID_DOSING_ABS_PRESS_SPN_4387,
        LAST_SPN_IN_PGN_61478
    }SPN_IN_PGN_61478_t;

    typedef enum
    {
        AT1_INTAKE_NOX_SPN_3216,
        AT1_INTAKE_O2_SPN_3217,
        LAST_SPN_IN_PGN_61454
    }SPN_IN_PGN_61454_t;

    typedef enum
    {
        ENG_CLNT_TEMP_SPN_110,
        ENG_FUEL_TEMP_SPN_174,
        ENG_OIL_TEMP_SPN_175,
        ENG_TURBO_OIL_TEMP_SPN_176,
        ENG_INTERCOLLAR_TEMP_SPN_52,
        LAST_SPN_IN_PGN_65262
    }SPN_IN_PGN_65262_t;

    typedef enum
    {
        SPN_1,
        SPN_2,
        SPN_3,
        SPN_4,
        SPN_5,
        SPN_6,
        SPN_7,
        SPN_8,
        SPN_9,
        SPN_10,
        SPN_11,
        SPN_12,
        LAST_SPN_IN_PGN_65374
    }SPN_IN_PGN_65374_t;


    typedef enum
    {
        DPF_SOOT_LOAD_SPN_3719,
        DPF_ASH_LOAD_3720,
        LAST_SPN_IN_PGN_64891
    }SPN_IN_PGN_64891_t;

    /* IVECO's Preheat Status before Engine Start */
     typedef enum
     {
         PREHEAT_NOT_ACTIVE = 0
         ,PREHEAT_IN_PROGRESS
         ,PREHEAT_FAILED
         ,PREHEAT_COMPLETED
     }IVECO_ENG_PREHEAT_STATUS_t;

     typedef enum
     {
         PGN_61441_UNUSED_SPN_1 =0,
         PGN_61441_UNUSED_SPN_2,
         PGN_61441_UNUSED_SPN_3,
         PGN_61441_ENG_AUX_SHUTDOWN_SW,
         PGN_61441_UNUSED_SPN_4,
         PGN_61441_UNUSED_SPN_5,
         PGN_61441_UNUSED_SPN_6,
         PGN_61441_TOTAL_BYTE
     }PGN_61441_CMND_t;

     typedef enum
     {
         PGN_65223_UNUSED_SPN_1 = 0,
         PGN_65223_UNUSED_SPN_2,
         PGN_65223_UNUSED_SPN_3,
         PGN_65223_UNUSED_SPN_4,
         PGN_65223_DEFUEL_ACT,
         PGN_65223_UNUSED_SPN_5,
         PGN_65223_UNUSED_SPN_6,
         PGN_65223_TOTAL_BYTE
     }PGN_65223_CMND_t;

     enum SPN_IN_PGN_65252
     {
         ENG_WAIT_TO_STRT_LAMP,
         EPS_SHUTDOWN_ENG,
         EPS_APPROACHING_SHUTDOWN,
         LAST_SPN_IN_PGN_65252
     };
     enum SPN_IN_PGN_65247
     {
         NOMINAL_FRICTION_PERCENT_TORQUE,
         ENG_DESIRED_SPEED,
         ENG_OP_SPD_ASYM_ADJUSTMENT,
         PARISITIC_TORQUE_LOSS,
         AFT1_EXHAUST_GAS_MASSS_FLOW_RATE,
         LAST_SPN_IN_PGN_65247
     };
    typedef uint8_t* (J1939_PGNs);
    static bool bAvrFaultShutDn;
    static bool  bTXBufferisEmpty;
    static bool bTransferSWFaultShutDn;
    static bool bAvrVoltNotDetect;
    static bool bAvrFreqNotDetect;
    static bool bAvrFaultWarning;
    static bool bTransferSWFaultWarning;
    static bool bCurrentTransNotDetect;
    static bool bCANCommShutDn;
    static bool bCANCommWarning;
    static bool bTransferSWFault;
    static bool bAvrFault;
    static MB_APP::KEY_MB_CAN_EVENT_t stCANEvent;

    J1939APP(HAL_Manager &hal, CFGC &cfgc, CFGZ &cfgz, ENGINE_MONITORING &_engineMonitoring,
            GCU_ALARMS &gcuAlarm,MB_APP &mbApp,  AUTO_MODE  &Automode);
    void Update(bool bDeviceInconfig);
    void InitAfterConfigChange();

    void GenerateFrame(uint8_t u8PGNNum);
    void GetPGN(uint8_t ubyPGN, uint8_t u8RxOrTx, J1939_PGN_DB_t *pstGetPGN );

    void TransmitFrame(uint8_t u8PGNNum);
    void FillJ1939PGNDataFrame(void);
    void ClerAllPGNsDataBuffs(void);
    void Operation(void);
    void LoadData(uint8_t u8PGNNum, uint8_t au8SPNDataBuf[8]);
    void GetSPN(uint8_t ubyPGN, uint8_t ubySPN, uint8_t u8TxOrRX, J1939_SPN_DB_t * pstGetSpn);
    void ExtractReadFrame();
    static void ClearCAN_Alarms(void);
    static void GetCANEventStatus(MB_APP::KEY_MB_CAN_EVENT_t *stEvent);
    static void TxIsr(void);
    bool IsJ1939CommEnable(void);
    bool IsCommunicationFail(void);
    bool IsDM1PGNRecieved(void);
    bool GetLampStatus(LAMP_st eLamp);
    double GetReadData(DATABASE_RX_PGN_LIST_t ePgn, uint8_t u8SpnNum);
    uint8_t GetDm1MsgCount(void);
    uint8_t GetDm2MsgCount(void);
    J1939_DM_MSG_DECODE GetDM1Message(uint8_t u8MessageNum);
    J1939_DM_MSG_DECODE GetDM2Message(uint8_t u8MessageNum);
    uint8_t GetPCDAlarmCount(void);
    uint8_t GetNCDAlarmCount(void);
    uint8_t GetSPNErrorStatus(uint8_t u8PGNName, uint8_t u8SPNName);
    void ResetLampsStatus(void);
    uint8_t IsRedLampON()     ;
    uint8_t IsAmberLampON()   ;
    uint8_t IsMilLampON()     ;
    uint8_t IsProtectLampON() ;
    void RequestDM11PGN();
    void SetDm1MsgCount(uint8_t u8Count);
    void SetDm2MsgCount(uint8_t u8Count);
    void ClearDM2Messages();
    void ClearNCDandPCDAlarms();
    void RequestDM2Messages();
private:
    #define DM1_DTC_SIZE    4U
    #define PGN_DM01        65226U
    #define PGN_DM02        65227U
    #define PGN_TPCM        60416U
    #define PGN_TPDT        60160U
    #define PGN_RQST        59904U
    #define PGN_TD          65254U

    #define PGN_200MS       200
    #define PGN_100MS       100
    #define PGN_250MS       250
    #define PGN_500MS       500
    #define PGN_1SEC        1000
    #define PGN_50MS        50
    #define PGN_80MS        80
    #define PGN_20MS        20
    #define PGN_10MS        10

    #define SPN_520195      520195U
    #define SPN_520196      520196U
    #define SPN_520197      520197U
    #define SPN_520198      520198U
    #define SPN_520199      520199U

    typedef struct
    {
       uint8_t u8LockDataBuff;
       uint8_t au8TxDataBuffOld[8];
       uint8_t au8TxDataBuffUpdtd[8];
     }J1939_TXDATABUFF_SHRD_OBJCT;

    HAL_Manager       &_hal;
    CFGC              &_ObjCfgc;
    CFGZ              &_cfgz;
    MB_APP            &_ObjmbApp;
    ENGINE_MONITORING &_engMon;
    AUTO_MODE         &_Automode;
    GCU_ALARMS        &_gcuAlarm;



    J1939_PGNs ubypReadTxPgns[TX_LAST_PGN];
    J1939_PGNs ubypReadRxPgns[RX_PGN_LAST];
    float _f32Pgn64915Data[7];
    float _f32Pgn65408Data[9];
    float _f32Pgn65350Data[13];
    float _f32Pgn0Data[11];
    float _f32Pgn65365Data[7];
    float _f32Pgn65281Data[9];
    float _f32Pgn65302Data[6];
    float _f32Pgn65280Data[5];
    float _f32Pgn65527Data[11];
    float _f32Pgn65385Data[4];
    float _f32Pgn65395Data[9];
    float _f32Pgn65406Data[4];
    float _f32Pgn57344Data[7];
    float _f32Pgn65219Data[7];
    float _f32Pgn65363Data[3];
    float _f32Pgn65265Data[7];
    float _f32Pgn65288Data[4];
    float _f32Pgn65223Data[7];
    float _f32Pgn61441Data[7];

    volatile float* PGN_DataFrames[TX_LAST_PGN] =
    {
         _f32Pgn64915Data,
         _f32Pgn65408Data,
         _f32Pgn65350Data,
         _f32Pgn0Data,
         _f32Pgn65365Data,
         _f32Pgn65281Data,
         _f32Pgn65302Data,
         _f32Pgn65280Data,
         _f32Pgn65527Data,
         _f32Pgn65385Data,
         _f32Pgn65395Data,
         _f32Pgn65406Data,
         _f32Pgn57344Data,
         _f32Pgn65219Data,
         _f32Pgn65363Data,
         _f32Pgn65265Data,
         _f32Pgn65288Data,
         _f32Pgn65223Data,
         _f32Pgn61441Data
    };


    /* EMR's Preheat Status before Engine Start */
    typedef enum
    {
        PREHEAT_OFF = 0
        ,PREHEAT_ON
        ,PREHEAT_ERROR
        ,PREHEAT_NA
    }EMR_ENG_PREHEAT_STATUS_t;


    uint8_t _au8CalcSPNData[8];
    J1939_TXDATABUFF_SHRD_OBJCT _astPGNTxDataBuff[TX_LAST_PGN];
    uint32_t _u32RequestedPGN;
    uint8_t _u8PGN_Num;
    uint8_t _u8NumOfDM1SPN;
    uint8_t _u8NumOfDM2SPN;
    uint8_t _u8J1939PIDTune_Normal;

    CFGZ::CFGZ_PARAMS_t _ConfigParam;
   // CEditableItem ArrEditableItem[INDEX_LAST];

    bool _bDeviceInConfigMode;
    uint16_t _u16FW_CRC;
    stTimer _Timer100ms;
    stTimer _Timer200ms;
    stTimer _Timer500ms;
    stTimer _Timer1s;
    stTimer _Timer250ms;
    stTimer _Timer50ms;
    stTimer _Timer80ms;
    stTimer _Timer20ms;
    stTimer _Timer10ms;
    stTimer _Timer40ms;
    stTimer _CommFailTimeout;
    uint8_t _u8ArrDM1SPNData[250];
    uint8_t _u8ArrDM2SPNData[250];
    uint8_t _u8ArrPropB5EPGNData[250];
    uint8_t _u8NumOf500msPGN;
    uint8_t _u8NoOfInvalidSpnInDm1Msg;
    uint8_t _u8NoOfInvalidSpnInDm2Msg;

    uint32_t _u32TSC1CANMessageID, _u32PropBF7CANMessageID ;
    uint8_t _u8PGN0xFFF7MessageCounter, _u8PGN0MessageCounter;

    uint8_t _ArrSpnErrorNAStatus[RX_PGN_LAST][12];

    uint8_t _u8ActiveDtcAmberLampStatus;
    uint8_t _u8ActiveDtcProtectLampStatus;
    uint8_t _u8ActiveDtcRedLampStatus;
    uint8_t _u8ActiveDtcMILLampStatus ;
    bool    _bIsDM1PGNRecieved;
    bool    _bIsCANJ1939CommFail;
    bool    _bClearActiveDTCs;
    bool    _bRequestDM2PGN;
    bool    _bRequestDM11PGN;
    double _ArrPgnReadData[RX_PGN_LAST][12];
    bool    _bEngineStartPreheatStatus;
    bool    _bPreheatFailed;
    uint8_t _u8AccPedalCounter;
    J1939_DM_MSG_DECODE _stDm1Decode[TOTAL_NO_OF_SPNS];
    J1939_DM_MSG_DECODE _stDm2Decode[TOTAL_NO_OF_SPNS];

   void prvUpdatePGN64915Data(void);
   void prvUpdatePGN65408Data(void);
   void prvUpdatePGN65350Data(void);
   void prvUpdatePGN0Data(void);
   void prvUpdatePGN65365Data(void);
   void prvUpdatePGN65281Data(void);
   void prvUpdatePGN65302Data(void);
   void prvUpdatePGN65280Data(void);
   void prvUpdatePGN65527Data(void);
   void prvUpdatePGN65385Data(void);
   void prvUpdatePGN65395Data(void);
   void prvUpdatePGN65406Data(void);
   void prvUpdatePGN57344Data(void);
   void prvUpdatePGN65219Data(void);
   void prvUpdatePGN65363Data(void);
   void prvUpdatePGN65265Data(void);
   void prvUpdatePGN65288Data(void);
   void prvUpdatePGN61441Data(void);
   void prvUpdatePGN65223Data(void);

   void prvUpdatePGN59904Data(uint8_t *au8CalSPNData);
   void prvFillAlarmValues(MB_APP::MODBUS_READ_REGISTERS_t eReg, float* f32PGN);
   void prvExractDM1SPN(void);
   void prvAssignDM1SPNVal(uint32_t u32LocalDM1SPN, uint8_t u8LocalFMI);

   double GetParamsFromJ1939(J1939APP::REALDATA_st edata);

   static void UpdateDM2RequestData(uint8_t *au8CalSPNData);
   static void UpdateDM11RequestData(uint8_t *au8CalSPNData);
   void CalculateTsc1Checksum(void);
   void CalculatePropB_F7Checksum();
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
   void prvHandleEngineStartStop();
   void prvCalculateAccPedalPosChecksum(void);

};

#endif /* APL_J1939APP_J1939APP_H_ */
