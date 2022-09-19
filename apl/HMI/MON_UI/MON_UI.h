/**
 * @file        [MON_UI]
 * @brief       This module is used to display Monitoring screens
 *              with the help go Display class and CONST_UI.h
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        15th July 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef APL_HMI_MON_UI_MON_UI_H_
#define APL_HMI_MON_UI_MON_UI_H_

#include <HMI/UI_DS.h>

#include "../../MW/Display/Display.h"
//#include "HAL_Manager.h"
#include "../../apl/GENSET_CONTROL/GCU_MODES/MANUAL_MODE.h"
#include "../../CFGC/CFGC.h"
#include "../UI/UI.h"
#include "../../apl/GENSET_CONTROL/GCU_MODES/BTS_MODE.h"
#include "../../apl/GENSET_CONTROL/GCU_MODES/CYCLIC_MODE.h"
#include "J1939_APP/J1939APP.h"
#define LOGO_TIMER_MS       (4000U)
#define LED_BLINK_TIMER     (500U)

//Bas to PSI conversion factor
#define PSI_CONVERSION      (14.503773773f)

//Deg C to Deg F conversion factors
#define DEG_F_FACTOR1       (1.8f)
#define DEG_F_FACTOR2       (32U)

#define ASCII_SPACE  0x20

#define MAX_AVAILABLE_PIXELS     75U
#define GROUP_SWITCH_TIMER_MS    700U
#define EXT_INPUT_UPDATE_TIME_MS    50

#define CNFG_UNIT_TBL_VAL(x)        x

/*todo: macro definition*/
#define ENABLE_MON_J1939     (0)


class MON_UI
{
public:

    /**
     * Enum for for all monitoring screens.
     */
    typedef enum
    {
        //Status & Info
        DISP_MON_HOME = 0,
        DISP_MON_PRODUCT_ID, /* Product ID*/

        DISP_MON_CAN_COMMUNICATION_INFO,
        DISP_MON_ENG_LINK_INFO,

        //Generator
        DISP_MON_GEN_VOLTAGE,
        DISP_MON_GEN_LOAD_KW,
        DISP_MON_GEN_LOAD_KVA,
        DISP_MON_GEN_LOAD_KVAR,
        DISP_MON_GEN_POWER_FACTOR,
        DISP_MON_GEN_CURRENT,
        DISP_MON_GEN_ENERGY,
        //Mains
        DISP_MON_MAINS_VOLTAGE,  /*VLN*/
        DISP_MON_MAINS_LOAD_KW,  /*LKW*/
        DISP_MON_MAINS_LOAD_KVA, /*LKVA*/
        DISP_MON_MAINS_LOAD_KVAR,/*LKVAR*/
        DISP_MON_MAINS_CURRENT,  /*I*/
        DISP_MON_MAINS_ENERGY,
        //Engine
        DISP_MON_BAT_VOLTAGE,
        DISP_MON_CHRG_ALT_BAT_VOLTAGE,
        DISP_MON_AIR_INTAKE_TEMP,
        DISP_MON_BOOST_PRESSURE,
        DISP_MON_SITE_BAT_RUN_HRS,
        DISP_MON_TAMPERED_RUN_HRS,
        DISP_MON_ENG_TEMP,
        DISP_MON_LUBE_OIL_PRESSURE,
        DISP_MON_FUEL,
        DISP_MON_SHELTER_TEMP,
        DISP_MON_AUX_2,
        DISP_MON_ENG_SPEED,
        DISP_MON_ENG_RUN_TIME,
        DISP_MON_LAST

    }MON_DISPLAY_LIST_t;


    typedef struct stSPNDisplayData
    {
        const char *cs8pSPNName;
        const char *cs8pSpanishSPNName;
        uint8_t      u8SPNUnitIndx;
    }
    stSPNDisplayData;
    typedef struct stPGNDisplayData
    {
        const uint8_t    cu8PGNName;
        uint8_t          u8NoOfSPNs;
        stSPNDisplayData *stpSPN;
    } stPGNDisplayData;


    typedef enum
     {
          MON_SCREEN_NORMAL,
          MON_SCREEN_J1939
     }MON_SCREEN_st;



    /**
     * Private variable for the screen number.
     */
    static uint8_t _stScreenNo ;

    /** Variable to store Display mode type.
         */
    static MODE_TYPE_t eDisplayMode;
    static MON_SCREEN_st eMonScreenType;
    /**
     * Constructor of the class
     * @param reference of hal object
     * @param reference of startStop object
     * @param reference of engineMonitoring object
     * @param reference of cfgz object
     * @return None
     */
    MON_UI(HAL_Manager &hal, MANUAL_MODE &manualMode,ENGINE_MONITORING &_engineMonitoring, CFGZ &cfgz, START_STOP &startStop,
           GCU_ALARMS &GCUAlarms, Display &Disp, CFGC &CFGC, J1939APP &j1939, BTS_MODE &BTSMode, CYCLIC_MODE &cyclicMode);
    /**
     * Update routine for UI class
     * @param None
     * @return None
     */
    void Update(bool bRefresh);

    /**
     * Key press status check function
     * @param  None
     * @return None
     */
    void CheckKeyPress(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent);

    /** Init for Monitoring UI, this will initialize the GCU
     * operating mode and contrast.
     * @param none
     * @return none
     */
    void Init();

    /**
     * This assign the home to display.
     * @param None
     * @return None
     */
    void GoToHomeScreen();

    /**
     * To handle Mode change request from MODBUS
     * @param None
     * @return None
     */
    static void MBChangeModeRequest();

    /**
     * To handle Monitoring screen group switching
     * @param _sKeyEvent
     * @param None
     */
    void GroupSwitching(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent);


private:
    #define MAX_SPN_ON_1_SCREEN     (5U)
    /**
     * Enum for Group of screens.
     */
    typedef enum
    {
        DISP_GROUP_STATUS = 0,/**< DISP_GROUP_STATUS */
        DISP_GROUP_GENERATOR, /**< DISP_GROUP_GENERATOR */
        DISP_GROUP_MAINS,     /**< DISP_GROUP_MAINS */
        DISP_GROUP_ENGINE,     /**< DISP_GROUP_ENGINE */
        DISP_GROUP_LOAD       /**< DISP_GROUP_LOAD   */
    }MON_GROUP_t;

    /**
     * Constant array for assigning the screen logo of specific group of parameter/
     */
    const MON_GROUP_t u8ArrGroupArray[DISP_MON_LAST]=
    {
         DISP_GROUP_STATUS,
         DISP_GROUP_STATUS,
         DISP_GROUP_STATUS,
         DISP_GROUP_STATUS,

         DISP_GROUP_GENERATOR,
         DISP_GROUP_GENERATOR,
         DISP_GROUP_GENERATOR,
         DISP_GROUP_GENERATOR,
         DISP_GROUP_GENERATOR,
         DISP_GROUP_GENERATOR,
         DISP_GROUP_GENERATOR,

         DISP_GROUP_MAINS,
         DISP_GROUP_LOAD,
         DISP_GROUP_LOAD,
         DISP_GROUP_LOAD,
         DISP_GROUP_LOAD,
         DISP_GROUP_MAINS,

         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE,
         DISP_GROUP_ENGINE
    };

    /**
     * Enum for source type
     */
    typedef enum
    {
        GENSET,  /**< GENSET */
        MAINS,   /**< MAINS */
        TYPE_LAST/**< TYPE_LAST */
    }SOURCE_TYPE_t;

    typedef enum
    {
        WAIT_FOR_BOOT_LOGO,
        WAIT_FOR_PRODUCT_INFO,
        RUNNING_MODE
    }STARTING_STATE_t;

    /**
     * Enum for data type
     */
    typedef enum
    {
         INTEGER_TYPE,/**< INTEGER_TYPE */
         FLOAT_TYPE   /**< FLOAT_TYPE */
    }DATA_TYPE_t;


    /**
     * Enum for power type
     */
    typedef enum
    {
        ACTIVE,   /**< ACTIVE */
        APARENT,  /**< APARENT */
        REACTIVE, /**< REACTIVE */
        POWER_LAST/**< POWER_LAST */
    }POWER_TYPE_t;

    /**
     * Enum for voltage and frequency
     */
    typedef enum
    {
        VTG,        /**< VTG */
        FRQ,        /**< FRQ */
        LAST_VTG_FRQ/**< LAST_VTG_FRQ */
    }VTG_FREQ_t;

    /**
     * Enum for phase to phase voltages
     */
    typedef enum
    {
       R_TO_Y,       /**< R_TO_Y */
       Y_TO_B,       /**< Y_TO_B */
       R_TO_B,       /**< R_TO_B */
       PH_PH_VTG_LAST/**< PH_PH_VTG_LAST */
    }PH_PH_T;


#if ENABLE_MON_J1939
    stSPNDisplayData astPropb57SPNList[1] =
    {
            {NULL, NULL, CNFG_UNIT_TBL_VAL(6)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astPropb62SPNList[1] =
    {
            {NULL, NULL, CNFG_UNIT_TBL_VAL(35)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astIVECOEngStatusList[3] =
    {
            {&SPNStringTable[SPN_COOL_WAT_TEMP][0],&SpanishSPNStringTable[SPN_COOL_WAT_TEMP][0], CNFG_UNIT_TBL_VAL(27)},
            {&SPNStringTable[SPN_OIL_TEMP][0],     &SpanishSPNStringTable[SPN_OIL_TEMP][0],      CNFG_UNIT_TBL_VAL(27)},
            {&SPNStringTable[SPN_OIL_PRESSURE][0], &SpanishSPNStringTable[SPN_OIL_PRESSURE][0],  CNFG_UNIT_TBL_VAL(34)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astLFESPNList[2] =
    {
        {&SPNStringTable[SPN_FUEL_RATE][0],   &SpanishSPNStringTable[SPN_FUEL_RATE][0],    CNFG_UNIT_TBL_VAL(32)},
        {&SPNStringTable[SPN_THROTTLE_POS][0],&SpanishSPNStringTable[SPN_THROTTLE_POS][0], CNFG_UNIT_TBL_VAL(1)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astEEC2SPNList[3] =
    {
        {&SPNStringTable[SPN_POS_ACC][0],    &SpanishSPNStringTable[SPN_POS_ACC][0],     CNFG_UNIT_TBL_VAL(1)},
        {&SPNStringTable[SPN_LOAD_SPEED][0], &SpanishSPNStringTable[SPN_LOAD_SPEED][0],  CNFG_UNIT_TBL_VAL(1)},
        {&SPNStringTable[SPN_ACT_MAX_TRQ][0],&SpanishSPNStringTable[SPN_ACT_MAX_TRQ][0], CNFG_UNIT_TBL_VAL(1)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astET1SPNList1[3] =
    {
        {&SPNStringTable[SPN_T_CCOLANT][0],&SpanishSPNStringTable[SPN_T_CCOLANT][0], CNFG_UNIT_TBL_VAL(27)},
        {&SPNStringTable[SPN_T_FUEL][0],   &SpanishSPNStringTable[SPN_T_FUEL][0],    CNFG_UNIT_TBL_VAL(27)},
        {&SPNStringTable[SPN_T_OIL][0],    &SpanishSPNStringTable[SPN_T_OIL][0],     CNFG_UNIT_TBL_VAL(27)},
    };

    stSPNDisplayData astET1SPNList2[2] =
    {
        {&SPNStringTable[SPN_T_TUR_OIL][0], &SpanishSPNStringTable[SPN_T_TUR_OIL][0], CNFG_UNIT_TBL_VAL(27)},
        {&SPNStringTable[SPN_T_INT_CO][0],  &SpanishSPNStringTable[SPN_T_INT_CO][0],  CNFG_UNIT_TBL_VAL(27)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astHOURSSPNList[1] =
    {
        {&SPNStringTable[SPN_HOURS][0],&SpanishSPNStringTable[SPN_HOURS][0], CNFG_UNIT_TBL_VAL(35)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astAMBSPNList[3] =
    {
        {&SPNStringTable[SPN_P_ATMOS][0],  &SpanishSPNStringTable[SPN_P_ATMOS][0],   CNFG_UNIT_TBL_VAL(34)},
        {&SPNStringTable[SPN_T_AMBIENT][0],&SpanishSPNStringTable[SPN_T_AMBIENT][0], CNFG_UNIT_TBL_VAL(27)},
        {&SPNStringTable[SPN_T_AIR_INL][0],&SpanishSPNStringTable[SPN_T_AIR_INL][0], CNFG_UNIT_TBL_VAL(27)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astVEP1SPNList[2] =
    {
        {&SPNStringTable[SPN_BATT_POT][0],&SpanishSPNStringTable[SPN_BATT_POT][0], CNFG_UNIT_TBL_VAL(2)},
        {&SPNStringTable[SPN_BATTERY][0], &SpanishSPNStringTable[SPN_BATTERY][0],  CNFG_UNIT_TBL_VAL(2)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astDDSPNList[2] =
    {
        {&SPNStringTable[SPN_P_FI_FUEL][0], &SpanishSPNStringTable[SPN_P_FI_FUEL][0], CNFG_UNIT_TBL_VAL(34)},
        {&SPNStringTable[SPN_P_FI_OIL][0],  &SpanishSPNStringTable[SPN_P_FI_OIL][0],  CNFG_UNIT_TBL_VAL(34)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astWF1SPNList[1] =
    {
        {&SPNStringTable[SPN_WATER_IN_FUEL][0], &SpanishSPNStringTable[SPN_WATER_IN_FUEL][0], CNFG_UNIT_TBL_VAL(0)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astLFCSPNList[2] =
    {
        {&SPNStringTable[SPN_TRIP_FUEL][0], &SpanishSPNStringTable[SPN_TRIP_FUEL][0],  CNFG_UNIT_TBL_VAL(36)},
        {&SPNStringTable[SPN_TOTAL_FUEL][0],&SpanishSPNStringTable[SPN_TOTAL_FUEL][0], CNFG_UNIT_TBL_VAL(28)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astEEC4SPNList[2] =
    {
        {&SPNStringTable[SPN_NOM_POWER][0],   &SpanishSPNStringTable[SPN_NOM_POWER][0],  CNFG_UNIT_TBL_VAL(11)},
        {&SPNStringTable[SPN_RATED_SPEED][0], &SpanishSPNStringTable[SPN_RATED_SPEED][0],CNFG_UNIT_TBL_VAL(33)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astLFISPNList[1] =
    {
        {&SPNStringTable[SPN_MEAN_T_FUEL][0],&SpanishSPNStringTable[SPN_MEAN_T_FUEL][0], CNFG_UNIT_TBL_VAL(32)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astIC2SPNList[2] =
    {
        {&SPNStringTable[SPN_A_FIL_DIFF2][0], &SpanishSPNStringTable[SPN_A_FIL_DIFF2][0],  CNFG_UNIT_TBL_VAL(34)},
        {&SPNStringTable[SPN_IN_T_MAN_ABS][0],&SpanishSPNStringTable[SPN_IN_T_MAN_ABS][0], CNFG_UNIT_TBL_VAL(34)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astEOISPNList[3] =
    {
        {&SPNStringTable[SPN_ENG_OPERATING_STATE][0],&SpanishSPNStringTable[SPN_ENG_OPERATING_STATE][0],  CNFG_UNIT_TBL_VAL(0)},
        {&SPNStringTable[SPN_ENG_DERATE_REQ][0],     &SpanishSPNStringTable[SPN_ENG_DERATE_REQ][0],       CNFG_UNIT_TBL_VAL(1)},
        {&SPNStringTable[SPN_ENG_EMERG_SD][0],       &SpanishSPNStringTable[SPN_ENG_EMERG_SD][0],         CNFG_UNIT_TBL_VAL(0)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astET2SPNList[1] =
    {
        {&SPNStringTable[SPN_T_ECU][0], &SpanishSPNStringTable[SPN_T_ECU][0], CNFG_UNIT_TBL_VAL(27)},
    };


    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astEEC3SPNList1[3] =
    {
        {&SPNStringTable[SPN_NOM_FRICTION][0],      &SpanishSPNStringTable[SPN_NOM_FRICTION][0],        CNFG_UNIT_TBL_VAL(1)},
        {&SPNStringTable[SPN_DES_ENG_SPEED][0],     &SpanishSPNStringTable[SPN_DES_ENG_SPEED][0],       CNFG_UNIT_TBL_VAL(33)},
        {&SPNStringTable[SPN_DES_SPEED_ASYM_ADJ][0],&SpanishSPNStringTable[SPN_DES_SPEED_ASYM_ADJ][0],  CNFG_UNIT_TBL_VAL(0)},
    };

    stSPNDisplayData astEEC3SPNList2[2] =
    {
        {&SPNStringTable[SPN_ESTIMATED_TRQ_LOSS][0], &SpanishSPNStringTable[SPN_ESTIMATED_TRQ_LOSS][0], CNFG_UNIT_TBL_VAL(1)},
        {&SPNStringTable[SPM_A_E_GAS_M_FR][0],       &SpanishSPNStringTable[SPM_A_E_GAS_M_FR][0],       CNFG_UNIT_TBL_VAL(39)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astS2SPNList[1] =
    {
            {&SPNStringTable[SPN_LAST_SERVICE_TIME][0], &SpanishSPNStringTable[SPN_LAST_SERVICE_TIME][0], CNFG_UNIT_TBL_VAL(31)}
    };


    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astEFLP2SPNList[1] =
    {
            {&SPNStringTable[SPN_INJ_METERING_PRES][0], &SpanishSPNStringTable[SPN_INJ_METERING_PRES][0], CNFG_UNIT_TBL_VAL(38)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astSDSPNList[3] =
    {
            {&SPNStringTable[SPN_WAIT_TO_START][0],         &SpanishSPNStringTable[SPN_WAIT_TO_START][0],          CNFG_UNIT_TBL_VAL(0)},
            {&SPNStringTable[SPN_EPS_SHUTDOEN_ENG][0],      &SpanishSPNStringTable[SPN_EPS_SHUTDOEN_ENG][0],       CNFG_UNIT_TBL_VAL(0)},
            {&SPNStringTable[SPN_EPS_APPROCHING_SHUT_DN][0],&SpanishSPNStringTable[SPN_EPS_APPROCHING_SHUT_DN][0], CNFG_UNIT_TBL_VAL(0)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astEFG1SPNList[1] =
    {
            {&SPNStringTable[SPN_ENG_INTAKE_AIR_FR][0],&SpanishSPNStringTable[SPN_ENG_INTAKE_AIR_FR][0],  CNFG_UNIT_TBL_VAL(39)},
    };


    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astDPF1SSPNList[1] =
    {
            {&SPNStringTable[SPN_SOOT_MASS][0],&SpanishSPNStringTable[SPN_SOOT_MASS][0], CNFG_UNIT_TBL_VAL(40)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astAT1IG1SPNList[2] =
    {
        {&SPNStringTable[SPN_AT1_INT_T_NOX][0], &SpanishSPNStringTable[SPN_AT1_INT_T_NOX][0], CNFG_UNIT_TBL_VAL(29)},
        {&SPNStringTable[SPN_AT1_INT_O2][0],    &SpanishSPNStringTable[SPN_AT1_INT_O2][0],    CNFG_UNIT_TBL_VAL(1)},
    };


    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astA1DOCSPNList[1] =
    {
        {&SPNStringTable[SPN_AT1_DOX_INT_GAS_TEMP][0], &SpanishSPNStringTable[SPN_AT1_DOX_INT_GAS_TEMP][0],  CNFG_UNIT_TBL_VAL(5)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astAT1IG2SPNList[1] =
    {
        {&SPNStringTable[SPN_AT1_IG2_DPF_GAS_TEMP][0], &SpanishSPNStringTable[SPN_AT1_IG2_DPF_GAS_TEMP][0], CNFG_UNIT_TBL_VAL(5)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astAT1IMGSPNList[1] =
    {
        {&SPNStringTable[SPN_AT1_IMG_DPF_DIFF_PRESS][0],&SpanishSPNStringTable[SPN_AT1_IMG_DPF_DIFF_PRESS][0], CNFG_UNIT_TBL_VAL(41)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astAT1OG1SPNList[2] =
    {
            {&SPNStringTable[SPN_AT1_OUTL_NOX][0],&SpanishSPNStringTable[SPN_AT1_OUTL_NOX][0], CNFG_UNIT_TBL_VAL(29)},
            {&SPNStringTable[SPN_AT1_OUT_O2][0],  &SpanishSPNStringTable[SPN_AT1_OUT_O2][0],   CNFG_UNIT_TBL_VAL(1)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astA1SCRDSI1SPNList[2] =
    {
        {&SPNStringTable[SPN_AT1_EXH_FA_DQ][0],  &SpanishSPNStringTable[SPN_AT1_EXH_FA_DQ][0],   CNFG_UNIT_TBL_VAL(30)},
        {&SPNStringTable[SPN_AT1_EXH_FLU_DAB][0],&SpanishSPNStringTable[SPN_AT1_EXH_FLU_DAB][0], CNFG_UNIT_TBL_VAL(34)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astA1SCRDSI2SPNList[1] =
    {
        {&SPNStringTable[SPN_AT1_EXH_FLU_DT][0], &SpanishSPNStringTable[SPN_AT1_EXH_FLU_DT][0], CNFG_UNIT_TBL_VAL(27)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astA1SCRDSR1SPNList[1] =
    {
        {&SPNStringTable[SPN_AT1_EXH_FLU_DRQ][0], &SpanishSPNStringTable[SPN_AT1_EXH_FLU_DRQ][0],  CNFG_UNIT_TBL_VAL(30)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astA1SCREGT1SPNList[2] =
    {
        {&SPNStringTable[SPN_AT1_SCR_IN_G][0], &SpanishSPNStringTable[SPN_AT1_SCR_IN_G][0], CNFG_UNIT_TBL_VAL(27)},
        {&SPNStringTable[SPN_AT1_SCR_OU_G][0], &SpanishSPNStringTable[SPN_AT1_SCR_OU_G][0], CNFG_UNIT_TBL_VAL(27)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astAT2IG1SPNList[1] =
    {
        {&SPNStringTable[SPN_AT2_INTT_NOX][0],&SpanishSPNStringTable[SPN_AT2_INTT_NOX][0], CNFG_UNIT_TBL_VAL(29)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astAT2OG1SPNList[1] =
    {
        {&SPNStringTable[SPN_AT2_OUTL_NOX][0],&SpanishSPNStringTable[SPN_AT2_OUTL_NOX][0], CNFG_UNIT_TBL_VAL(29)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astA2SCRDSI2SPNList[1] =
    {
        {&SPNStringTable[SPN_AT2_EXH_FLU_DT][0],&SpanishSPNStringTable[SPN_AT2_EXH_FLU_DT][0], CNFG_UNIT_TBL_VAL(27)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astA2SCRDSI1SPNList[2] =
    {
        {&SPNStringTable[SPN_AT2_EXH_FA_DQ][0],  &SpanishSPNStringTable[SPN_AT2_EXH_FA_DQ][0],   CNFG_UNIT_TBL_VAL(30)},
        {&SPNStringTable[SPN_AT2_EXH_FLU_DAB][0],&SpanishSPNStringTable[SPN_AT2_EXH_FLU_DAB][0], CNFG_UNIT_TBL_VAL(34)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astA2SCRDSR1SPNList[1] =
    {
        {&SPNStringTable[SPN_AT2_EXH_FLU_DRQ][0], &SpanishSPNStringTable[SPN_AT2_EXH_FLU_DRQ][0], CNFG_UNIT_TBL_VAL(30)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astA2SCREGT1SPNList[2] =
    {
        {&SPNStringTable[SPN_AT2_SCR_IN_G][0],&SpanishSPNStringTable[SPN_AT2_SCR_IN_G][0], CNFG_UNIT_TBL_VAL(27)},
        {&SPNStringTable[SPN_AT2_SCR_OU_G][0],&SpanishSPNStringTable[SPN_AT2_SCR_OU_G][0], CNFG_UNIT_TBL_VAL(27)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astAT1OG2SPNList[1] =
    {
        {&SPNStringTable[SPN_DPF_OUTL_T][0],&SpanishSPNStringTable[SPN_DPF_OUTL_T][0], CNFG_UNIT_TBL_VAL(27)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astAT1S2SPNList[1] =
    {
        {&SPNStringTable[SPN_NEXT_REGEN][0],&SpanishSPNStringTable[SPN_NEXT_REGEN][0], CNFG_UNIT_TBL_VAL(31)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astAT1SSPNList[2] =
    {
        {&SPNStringTable[SPN_DPF_SOOT_LOAD][0],&SpanishSPNStringTable[SPN_DPF_SOOT_LOAD][0], CNFG_UNIT_TBL_VAL(1)},
        {&SPNStringTable[SPN_DPF_ASH_LOAD][0], &SpanishSPNStringTable[SPN_DPF_ASH_LOAD][0], CNFG_UNIT_TBL_VAL(1)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astEEC1SPN1List[4] =
    {
        {&SPNStringTable[SPN_ENG_TORQUE_MODE][0], &SpanishSPNStringTable[SPN_ENG_TORQUE_MODE][0],CNFG_UNIT_TBL_VAL(0)},
        {&SPNStringTable[SPN_DDE_TORQUE][0],      &SpanishSPNStringTable[SPN_DDE_TORQUE][0],     CNFG_UNIT_TBL_VAL(1)},
        {&SPNStringTable[SPN_AC_TORQUE][0],       &SpanishSPNStringTable[SPN_AC_TORQUE][0],      CNFG_UNIT_TBL_VAL(1)},
        {&SPNStringTable[SPN_SPEED][0],           &SpanishSPNStringTable[SPN_SPEED][0],          CNFG_UNIT_TBL_VAL(33)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astEEC1SPN2List[3] =
    {
        {&SPNStringTable[SPN_SA_CONT_DEVICE][0],&SpanishSPNStringTable[SPN_SA_CONT_DEVICE][0], CNFG_UNIT_TBL_VAL(0)},
        {&SPNStringTable[SPN_START_MODE][0],    &SpanishSPNStringTable[SPN_START_MODE][0],     CNFG_UNIT_TBL_VAL(0)},
        {&SPNStringTable[SPN_EDE_TORQUE][0],    &SpanishSPNStringTable[SPN_EDE_TORQUE][0],     CNFG_UNIT_TBL_VAL(1)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astEFLP1SPN1List[3] =
    {
        {&SPNStringTable[SPN_P_FUEL][0], &SpanishSPNStringTable[SPN_P_FUEL][0], CNFG_UNIT_TBL_VAL(34)},
        {&SPNStringTable[SPN_L_OIL][0],  &SpanishSPNStringTable[SPN_L_OIL][0],  CNFG_UNIT_TBL_VAL(1)},
        {&SPNStringTable[SPN_P_OIL][0],  &SpanishSPNStringTable[SPN_P_OIL][0],  CNFG_UNIT_TBL_VAL(34)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astEFLP1SPN2List[3] =
    {
        {&SPNStringTable[SPN_P_CRANKC][0],  &SpanishSPNStringTable[SPN_P_CRANKC][0],  CNFG_UNIT_TBL_VAL(34)},
        {&SPNStringTable[SPN_P_COOLANT][0], &SpanishSPNStringTable[SPN_P_COOLANT][0], CNFG_UNIT_TBL_VAL(34)},
        {&SPNStringTable[SPN_L_COOLANT][0], &SpanishSPNStringTable[SPN_L_COOLANT][0], CNFG_UNIT_TBL_VAL(1)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astIC1SPN1List[4] =
    {
        {&SPNStringTable[SPN_TRAP_INLET][0], &SpanishSPNStringTable[SPN_TRAP_INLET][0],CNFG_UNIT_TBL_VAL(34)},
        {&SPNStringTable[SPN_P_BOOST][0],    &SpanishSPNStringTable[SPN_P_BOOST][0],   CNFG_UNIT_TBL_VAL(34)},
        {&SPNStringTable[SPN_T_INT_MAN][0],  &SpanishSPNStringTable[SPN_T_INT_MAN][0], CNFG_UNIT_TBL_VAL(27)},
        {&SPNStringTable[SPN_P_IN_AIR][0],   &SpanishSPNStringTable[SPN_P_IN_AIR][0],  CNFG_UNIT_TBL_VAL(34)},
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astIC1SPN2List[3] =
    {
        {&SPNStringTable[SPN_A_FIL_DIFF1][0], &SpanishSPNStringTable[SPN_A_FIL_DIFF1][0],CNFG_UNIT_TBL_VAL(34)},
        {&SPNStringTable[SPN_T_EX_GAS][0],    &SpanishSPNStringTable[SPN_T_EX_GAS][0],   CNFG_UNIT_TBL_VAL(27)},
        {&SPNStringTable[SPN_C_FIL_DIFF][0],  &SpanishSPNStringTable[SPN_C_FIL_DIFF][0], CNFG_UNIT_TBL_VAL(34)},
    };

    /// -----------------------------------------------------------------------------------------------
    //// astAT1T1ISPN1List is having 4 SPNs out of which 2 SPNs are having SPN strings which needs to be display,
    //// other two are having ICONS, hence they need not to add here.
    stSPNDisplayData astAT1T1ISPN1List[4] =
    {
        {&SPNStringTable[SPN_DEF_LEVEL][0],       &SpanishSPNStringTable[SPN_DEF_LEVEL][0],         CNFG_UNIT_TBL_VAL(1)},
        {&SPNStringTable[SPN_AT1_EXH_FLU_TANK][0],&SpanishSPNStringTable[SPN_AT1_EXH_FLU_TANK][0],  CNFG_UNIT_TBL_VAL(27)},
        {&SPNStringTable[SPN_SCR_IND_ACT][0],     &SpanishSPNStringTable[SPN_SCR_IND_ACT][0],       CNFG_UNIT_TBL_VAL(0)},
        {&SPNStringTable[SPN_SCR_IND_SEV][0],     &SpanishSPNStringTable[SPN_SCR_IND_SEV][0],       CNFG_UNIT_TBL_VAL(0)}
    };


    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astPROPB5ESPNList1[2] =
    {
        {&SPNStringTable[SPN_ACC_TIME_DPF][0],  &SpanishSPNStringTable[SPN_ACC_TIME_DPF][0],   CNFG_UNIT_TBL_VAL(31)},
        {&SPNStringTable[SPN_REMOVAL_OF_DPF][0],&SpanishSPNStringTable[SPN_REMOVAL_OF_DPF][0], CNFG_UNIT_TBL_VAL(0)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astPROPB5ESPNList2[2] =
    {
        {&SPNStringTable[SPN_OT_DPF_REMOVAL][0],  &SpanishSPNStringTable[SPN_OT_DPF_REMOVAL][0],  CNFG_UNIT_TBL_VAL(31)},
        {&SPNStringTable[SPN_DPF_REMOVAL_TIME][0],&SpanishSPNStringTable[SPN_DPF_REMOVAL_TIME][0],CNFG_UNIT_TBL_VAL(31)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astPROPB5ESPNList3[2] =
    {
        {&SPNStringTable[SPN_LOSS_OF_DPF][0],       &SpanishSPNStringTable[SPN_LOSS_OF_DPF][0],       CNFG_UNIT_TBL_VAL(31)},
        {&SPNStringTable[SPN_NO_OF_LOSS_OF_DPF][0], &SpanishSPNStringTable[SPN_NO_OF_LOSS_OF_DPF][0], CNFG_UNIT_TBL_VAL(0)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astPROPB5ESPNList4[2] =
    {
        {&SPNStringTable[SPN_OC_FOR_DPF_LOSS][0], &SpanishSPNStringTable[SPN_OC_FOR_DPF_LOSS][0],CNFG_UNIT_TBL_VAL(31)},
        {&SPNStringTable[SPN_DPF_LOSS_TIME][0],   &SpanishSPNStringTable[SPN_DPF_LOSS_TIME][0],  CNFG_UNIT_TBL_VAL(31)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astPROPB5ESPNList5[2] =
    {
        {&SPNStringTable[SPN_ACC_TIME_PCD_FAIL][0],&SpanishSPNStringTable[SPN_ACC_TIME_PCD_FAIL][0], CNFG_UNIT_TBL_VAL(31)},
        {&SPNStringTable[SPN_NO_OF_PCD_FAIL][0],   &SpanishSPNStringTable[SPN_NO_OF_PCD_FAIL][0],    CNFG_UNIT_TBL_VAL(0)}
    };

    /// -----------------------------------------------------------------------------------------------
    stSPNDisplayData astPROPB5ESPNList6[2] =
    {
        {&SPNStringTable[SPN_OC_TIME_PCD_FAIL][0],&SpanishSPNStringTable[SPN_OC_TIME_PCD_FAIL][0], CNFG_UNIT_TBL_VAL(31)},
        {&SPNStringTable[SPN_PCD_FAIL_TIME][0],   &SpanishSPNStringTable[SPN_PCD_FAIL_TIME][0],    CNFG_UNIT_TBL_VAL(31)}
    };

    stSPNDisplayData astPROSTOUTSPNList1[4] =
    {
       {&SPNStringTable[SPN_REM_SRVC_HR][0],    &SpanishSPNStringTable[SPN_REM_SRVC_HR][0],     CNFG_UNIT_TBL_VAL(31)},
       {&SPNStringTable[SPN_ENGINE_RUNNING][0], &SpanishSPNStringTable[SPN_ENGINE_RUNNING][0],  CNFG_UNIT_TBL_VAL(0)},
       {&SPNStringTable[SPN_SERVICE_DELAY][0],  &SpanishSPNStringTable[SPN_SERVICE_DELAY][0],   CNFG_UNIT_TBL_VAL(0)},
       {&SPNStringTable[SPN_PRE_GLOW_ACTIVE][0],&SpanishSPNStringTable[SPN_PRE_GLOW_ACTIVE][0], CNFG_UNIT_TBL_VAL(0)}
    };

    stSPNDisplayData astPROSTOUTSPNList2[4] =
    {
       {&SPNStringTable[SPN_P_OIL_MISSING][0],   &SpanishSPNStringTable[SPN_P_OIL_MISSING][0],   CNFG_UNIT_TBL_VAL(0)},
       {&SPNStringTable[SPN_ENG_DIAGN_LAMP][0],  &SpanishSPNStringTable[SPN_ENG_DIAGN_LAMP][0],  CNFG_UNIT_TBL_VAL(0)},
       {&SPNStringTable[SPN_ENG_OVER_TEMP][0],   &SpanishSPNStringTable[SPN_ENG_OVER_TEMP][0],   CNFG_UNIT_TBL_VAL(0)},
       {&SPNStringTable[SPN_AIR_FILT_SWITCH][0], &SpanishSPNStringTable[SPN_AIR_FILT_SWITCH][0], CNFG_UNIT_TBL_VAL(0)}
    };

    stSPNDisplayData astHATZPROPBSPNList[2] =
    {
       {&SPNStringTable[SPN_PHYS_INJ_QTY][0],&SpanishSPNStringTable[SPN_PHYS_INJ_QTY][0], CNFG_UNIT_TBL_VAL(0)},
       {&SPNStringTable[SPN_PHYS_TORQ][0],   &SpanishSPNStringTable[SPN_PHYS_TORQ][0],    CNFG_UNIT_TBL_VAL(0)}
    };

    stSPNDisplayData astAISPNList[1] =
    {
       {&SPNStringTable[SPN_ALT_1_STAT][0],&SpanishSPNStringTable[SPN_ALT_1_STAT][0], CNFG_UNIT_TBL_VAL(0)}
    };

    stSPNDisplayData astETC5SPNList[1] =
    {
       {&SPNStringTable[SPN_TX_NEUTRAL_SW][0], &SpanishSPNStringTable[SPN_TX_NEUTRAL_SW][0], CNFG_UNIT_TBL_VAL(0)}
    };
    stSPNDisplayData astOIISPNList[2] =
    {
       {&SPNStringTable[SPN_TRQ_DERATE_TIME1][0],&SpanishSPNStringTable[SPN_TRQ_DERATE_TIME1][0], CNFG_UNIT_TBL_VAL(23)},
       {&SPNStringTable[SPN_TRQ_DERATE_TIME2][0],&SpanishSPNStringTable[SPN_TRQ_DERATE_TIME2][0], CNFG_UNIT_TBL_VAL(23)}
    };

    stSPNDisplayData astIT1SPNList[1] =
     {
        {&SPNStringTable[SPN_CYLENDER1_ING_TIME][0],&SpanishSPNStringTable[SPN_CYLENDER1_ING_TIME][0], CNFG_UNIT_TBL_VAL(27)},
     };

    stSPNDisplayData astGFPSPNList[1] =
     {
        {&SPNStringTable[SPN_INTAKE_ABS_PRESSURE][0], &SpanishSPNStringTable[SPN_INTAKE_ABS_PRESSURE][0], CNFG_UNIT_TBL_VAL(41)},
     };

    stSPNDisplayData astIMISPNList[2] =
    {
       {&SPNStringTable[SPN_TURBO1_PRESS_BOOST][0],&SpanishSPNStringTable[SPN_TURBO1_PRESS_BOOST][0], CNFG_UNIT_TBL_VAL(41)},
       {&SPNStringTable[SPN_TURBO2_PRESS_BOOST][0],&SpanishSPNStringTable[SPN_TURBO2_PRESS_BOOST][0], CNFG_UNIT_TBL_VAL(41)},
    };

    stSPNDisplayData astHATZCCVSSPNList[3] =
    {
       {&SPNStringTable[SPN_PARKING_BRAKE_SW][0],&SpanishSPNStringTable[SPN_PARKING_BRAKE_SW][0], CNFG_UNIT_TBL_VAL(0)},
       {&SPNStringTable[SPN_VEHICLE_SPD][0],     &SpanishSPNStringTable[SPN_VEHICLE_SPD][0],      CNFG_UNIT_TBL_VAL(42)},
       {&SPNStringTable[SPN_BRAKE_SW][0],        &SpanishSPNStringTable[SPN_BRAKE_SW][0],         CNFG_UNIT_TBL_VAL(0)}
    };

    stSPNDisplayData astFanDrive[3]
     {
        {&SPNStringTable[SPN_EST_PERCENT_SPEED][0],&SpanishSPNStringTable[SPN_EST_PERCENT_SPEED][0], CNFG_UNIT_TBL_VAL(1)},
        {&SPNStringTable[SPN_FAN_DRIVE_STATE][0],  &SpanishSPNStringTable[SPN_FAN_DRIVE_STATE][0],   CNFG_UNIT_TBL_VAL(0)},
        {&SPNStringTable[SPN_FAN_SPEED][0],        &SpanishSPNStringTable[SPN_FAN_SPEED][0],         CNFG_UNIT_TBL_VAL(33)}
      };

    stSPNDisplayData astDirectLampCtr1[2]
     {
        {&SPNStringTable[SPN_AMBER_LAMP_CMD][0],&SpanishSPNStringTable[SPN_AMBER_LAMP_CMD][0], CNFG_UNIT_TBL_VAL(0)},
        {&SPNStringTable[SPN_RED_LAMP_CMD][0],  &SpanishSPNStringTable[SPN_RED_LAMP_CMD][0],   CNFG_UNIT_TBL_VAL(0)},
      };

    stSPNDisplayData astGFC[2]
        {
           {&SPNStringTable[SPN_TRIP_FUEL_GAS][0],&SpanishSPNStringTable[SPN_TRIP_FUEL_GAS][0], CNFG_UNIT_TBL_VAL(43)},
           {&SPNStringTable[SPN_FUEL_GAS][0],     &SpanishSPNStringTable[SPN_FUEL_GAS][0],      CNFG_UNIT_TBL_VAL(43)},
         };

    stSPNDisplayData astEP1[4]
       {
          {&SPNStringTable[SPN_GAS_PORT1][0], &SpanishSPNStringTable[SPN_GAS_PORT1][0],CNFG_UNIT_TBL_VAL(27)},
          {&SPNStringTable[SPN_GAS_PORT2][0], &SpanishSPNStringTable[SPN_GAS_PORT2][0],CNFG_UNIT_TBL_VAL(27)},
          {&SPNStringTable[SPN_GAS_PORT3][0], &SpanishSPNStringTable[SPN_GAS_PORT3][0],CNFG_UNIT_TBL_VAL(27)},
          {&SPNStringTable[SPN_GAS_PORT4][0], &SpanishSPNStringTable[SPN_GAS_PORT4][0],CNFG_UNIT_TBL_VAL(27)},

        };
    stSPNDisplayData astEP2[4]
          {
             {&SPNStringTable[SPN_GAS_PORT5][0], &SpanishSPNStringTable[SPN_GAS_PORT5][0],CNFG_UNIT_TBL_VAL(27)},
             {&SPNStringTable[SPN_GAS_PORT6][0], &SpanishSPNStringTable[SPN_GAS_PORT6][0],CNFG_UNIT_TBL_VAL(27)},
             {&SPNStringTable[SPN_GAS_PORT7][0], &SpanishSPNStringTable[SPN_GAS_PORT7][0],CNFG_UNIT_TBL_VAL(27)},
             {&SPNStringTable[SPN_GAS_PORT8][0], &SpanishSPNStringTable[SPN_GAS_PORT8][0],CNFG_UNIT_TBL_VAL(27)},

           };
    stSPNDisplayData astEP3[4]
          {
             {&SPNStringTable[SPN_GAS_PORT9][0],  &SpanishSPNStringTable[SPN_GAS_PORT9][0],  CNFG_UNIT_TBL_VAL(27)},
             {&SPNStringTable[SPN_GAS_PORT10][0], &SpanishSPNStringTable[SPN_GAS_PORT10][0], CNFG_UNIT_TBL_VAL(27)},
             {&SPNStringTable[SPN_GAS_PORT11][0], &SpanishSPNStringTable[SPN_GAS_PORT11][0], CNFG_UNIT_TBL_VAL(27)},
             {&SPNStringTable[SPN_GAS_PORT12][0], &SpanishSPNStringTable[SPN_GAS_PORT12][0], CNFG_UNIT_TBL_VAL(27)},

           };
    stSPNDisplayData astEP4[4]
  {
     {&SPNStringTable[SPN_GAS_PORT13][0], &SpanishSPNStringTable[SPN_GAS_PORT13][0],CNFG_UNIT_TBL_VAL(27)},
     {&SPNStringTable[SPN_GAS_PORT14][0], &SpanishSPNStringTable[SPN_GAS_PORT14][0],CNFG_UNIT_TBL_VAL(27)},
     {&SPNStringTable[SPN_GAS_PORT15][0], &SpanishSPNStringTable[SPN_GAS_PORT15][0],CNFG_UNIT_TBL_VAL(27)},
     {&SPNStringTable[SPN_GAS_PORT16][0], &SpanishSPNStringTable[SPN_GAS_PORT16][0],CNFG_UNIT_TBL_VAL(27)},

   };
    stSPNDisplayData astEP5[4]
  {
     {&SPNStringTable[SPN_GAS_PORT17][0], &SpanishSPNStringTable[SPN_GAS_PORT17][0], CNFG_UNIT_TBL_VAL(27)},
     {&SPNStringTable[SPN_GAS_PORT18][0], &SpanishSPNStringTable[SPN_GAS_PORT18][0], CNFG_UNIT_TBL_VAL(27)},
     {&SPNStringTable[SPN_GAS_PORT19][0], &SpanishSPNStringTable[SPN_GAS_PORT19][0], CNFG_UNIT_TBL_VAL(27)},
     {&SPNStringTable[SPN_GAS_PORT20][0], &SpanishSPNStringTable[SPN_GAS_PORT20][0], CNFG_UNIT_TBL_VAL(27)},

   };

    stSPNDisplayData astET4[3]
    {
       {&SPNStringTable[SPN_COOL_TEMP_2   ][0], &SpanishSPNStringTable[SPN_COOL_TEMP_2   ][0],CNFG_UNIT_TBL_VAL(27)},
       {&SPNStringTable[SPN_PUMP_OUT_TEMP ][0], &SpanishSPNStringTable[SPN_PUMP_OUT_TEMP ][0],CNFG_UNIT_TBL_VAL(27)},
       {&SPNStringTable[SPN_COOL_TEMP_3   ][0], &SpanishSPNStringTable[SPN_COOL_TEMP_3   ][0],CNFG_UNIT_TBL_VAL(27)},
     };

    stSPNDisplayData astTI4[2]
    {
       {&SPNStringTable[SPN_TURB_IN_TEMP_1][0], &SpanishSPNStringTable[SPN_TURB_IN_TEMP_1][0],CNFG_UNIT_TBL_VAL(27)},
       {&SPNStringTable[SPN_TUNB_IN_TMEP_2][0], &SpanishSPNStringTable[SPN_TUNB_IN_TMEP_2][0],CNFG_UNIT_TBL_VAL(27)},
     };

    stSPNDisplayData astEFL_P12[1]
   {
      {&SPNStringTable[SPN_FILT_FUEL_PRESS][0], &SpanishSPNStringTable[SPN_FILT_FUEL_PRESS][0],CNFG_UNIT_TBL_VAL(41)},
    };

    stPGNDisplayData astJ1939Display[NUMBER_OF_PGN_SCREENS] =
    {
        /* Following 2 entries were added to compensate for DISP_EDC4_CAN_STATUS and DISP_IVECO_ENGINE_STATUS screens */
        {DISP_PROPB62_PGN_65378, 0, astPropb62SPNList},
        {DISP_PROPB57_PGN_65367, 0, astPropb57SPNList},
        {DISP_EDC4_CAN_STATUS,   0, NULL},
        {DISP_IVECO_ENGINE_STATUS, NO_OF_SPN_IVECO_ENG_STATUS_65282, astIVECOEngStatusList},
        {DISP_LFE_PGN_65266, NO_OF_SPN_LFE_PGN_65266, astLFESPNList},
        {DISP_EEC2_PGN_61443, NO_OF_SPN_EEC2_PGN_61443, astEEC2SPNList},
        {DISP_AMB_PGN_65269, NO_OF_SPN_AMB_PGN_65269, astAMBSPNList},
        {DISP_HOURS_PGN_65253, NO_OF_SPN_HOURS_PGN_65253, astHOURSSPNList},
        {DISP_VEP1_PGN_65271, NO_OF_SPN_VEP1_PGN_65271, astVEP1SPNList},
        {DISP_DD_PGN_65276, NO_OF_SPN_DD_PGN_65276, astDDSPNList},
        {DISP_WFI_PGN_65279, NO_OF_SPN_WFI_PGN_65279, astWF1SPNList},
        {DISP_LFC_PGN_65257, NO_OF_SPN_LFC_PGN_65257, astLFCSPNList},
        {DISP_EEC4_PGN_65214, NO_OF_SPN_EEC4_PGN_65214, astEEC4SPNList},
        {DISP_LFI_PGN_65203, NO_OF_SPN_LFI_PGN_65203, astLFISPNList},
        {DISP_IC2_PGN_64976, NO_OF_SPN_IC2_PGN_64976, astIC2SPNList},
        {DISP_EOI_PGN_64914, NO_OF_SPN_EOI_PGN_64914, astEOISPNList},
        {DISP_ET2_PGN_65188, NO_OF_SPN_ET2_PGN_65188, astET2SPNList},
        {DISP_EEC3_PGN_65247_1, NO_OF_SPN_EEC3_PGN_65247_1, astEEC3SPNList1},
        {DISP_EEC3_PGN_65247_2, NO_OF_SPN_EEC3_PGN_65247_2, astEEC3SPNList2},
        {DISP_S2_PGN_65166, NO_OF_SPN_S2_PGN_65166, astS2SPNList},
        {DISP_EFL_P2_PGN_65243, NO_OF_SPN_EFLP2_PGN_65243, astEFLP2SPNList},
        {DISP_SHUTDOWN_PGN_65252, NO_OF_SPN_SD_PGN_65252, astSDSPNList},
        {DISP_EFG1_PGN_61450, NO_OF_SPN_EFG1_PGN_61450, astEFG1SPNList},
        {DISP_DPF1S_PGN_64796, NO_OF_SPN_DPF1S_PGN_64796, astDPF1SSPNList},
        {DISP_ET1_PGN_65262_1, NO_OF_SPN_ET1_PGN_65262_1, astET1SPNList1},
        {DISP_ET1_PGN_65262_2, NO_OF_SPN_ET1_PGN_65262_2, astET1SPNList2},
        {DISP_EEC1_PGN_61444_1, NO_OF_SPN_EEC1_PGN_61444_1, astEEC1SPN1List},
        {DISP_EEC1_PGN_61444_2, NO_OF_SPN_EEC1_PGN_61444_2, astEEC1SPN2List},
        {DISP_EFL_P1_PGN_65263_1, NO_OF_SPN_EFL_P1_PGN_65263_1, astEFLP1SPN1List},
        {DISP_EFL_P1_PGN_65263_2, NO_OF_SPN_EFL_P1_PGN_65263_2, astEFLP1SPN2List},
        {DISP_IC1_PGN_65270_1, NO_OF_SPN_IC1_PGN_65270_1, astIC1SPN1List},
        {DISP_IC1_PGN_65270_2, NO_OF_SPN_IC1_PGN_65270_2, astIC1SPN2List},
        {DISP_AT1IG1_PGN_61454, NO_OF_SPN_AT1IG1_PGN_61454, astAT1IG1SPNList},
        {DISP_A1DOC_PGN_64800, NO_OF_SPN_A1DOC_PGN_64800, astA1DOCSPNList},
        {DISP_AT1IG2_PGN_64948, NO_OF_SPN_AT1IG2_PGN_64948, astAT1IG2SPNList},
        {DISP_AT1IMG_PGN_64946, NO_OF_SPN_AT1IMG_PGN_64946, astAT1IMGSPNList},
        {DISP_AT1OG1_PGN_61455, NO_OF_SPN_AT1OG1_PGN_61455, astAT1OG1SPNList},
        {DISP_A1SCRDSI1_PGN_61475, NO_OF_SPN_A1SCRDSI1_PGN_61475, astA1SCRDSI1SPNList},
        {DISP_A1SCRDSI2_PGN_64833, NO_OF_SPN_A1SCRDSI2_PGN_64833, astA1SCRDSI2SPNList},
        {DISP_A1SCRDSR1_PGN_61476, NO_OF_SPN_A1SCRDSR1_PGN_61476, astA1SCRDSR1SPNList},
        {DISP_A1SCREGT1_PGN_64830, NO_OF_SPN_A1SCREGT1_PGN_64830, astA1SCREGT1SPNList},
        {DISP_AT2IG1_PGN_61456, NO_OF_SPN_AT2IG1_PGN_61456, astAT2IG1SPNList},
        {DISP_AT2OG1_PGN_61457, NO_OF_SPN_AT2OG1_PGN_61457, astAT2OG1SPNList},
        {DISP_A2SCRDSI2_PGN_64827, NO_OF_SPN_A2SCRDSI2_PGN_64827, astA2SCRDSI2SPNList},
        {DISP_A2SCRDSI1_PGN_61478, NO_OF_SPN_A2SCRDSI1_PGN_61478, astA2SCRDSI1SPNList},
        {DISP_A2SCRDSR1_PGN_61479, NO_OF_SPN_A2SCRDSR1_PGN_61479, astA2SCRDSR1SPNList},
        {DISP_A2SCREGT1_PGN_64824, NO_OF_SPN_A2SCREGT1_PGN_64824, astA2SCREGT1SPNList},
        {DISP_AT1OG2_PGN_64947, NO_OF_SPN_AT1OG2_PGN_64947, astAT1OG2SPNList},
        {DISP_AT1S2_PGN_64697, NO_OF_SPN_AT1S2_PGN_64697, astAT1S2SPNList},
        {DISP_AT1S_PGN_64891, NO_OF_SPN_AT1S_PGN_64891, astAT1SSPNList},
        {DISP_AT1T1I_PGN_65110, NO_OF_SPN_AT1T1I_PGN_65110_1+NO_OF_SPN_AT1T1I_PGN_65110_2, astAT1T1ISPN1List},
        {DISP_PROPB32_PGN_65330, 1, NULL},
        {DISP_PROPB5E_PGN_65374_1, NO_OF_SPN_PROPB5E_PGN_65373_1, astPROPB5ESPNList1},
        {DISP_PROPB5E_PGN_65374_2, NO_OF_SPN_PROPB5E_PGN_65373_2, astPROPB5ESPNList2},
        {DISP_PROPB5E_PGN_65374_3, NO_OF_SPN_PROPB5E_PGN_65373_3, astPROPB5ESPNList3},
        {DISP_PROPB5E_PGN_65374_4, NO_OF_SPN_PROPB5E_PGN_65373_4, astPROPB5ESPNList4},
        {DISP_PROPB5E_PGN_65374_5, NO_OF_SPN_PROPB5E_PGN_65373_5, astPROPB5ESPNList5},
        {DISP_PROPB5E_PGN_65374_6, NO_OF_SPN_PROPB5E_PGN_65373_6, astPROPB5ESPNList6},
        {DISP_PROSTOUT_PGN_65364_1, NO_OF_SPN_PROSTOUT_PGN_65364_1, astPROSTOUTSPNList1},
        {DISP_PROSTOUT_PGN_65364_2, NO_OF_SPN_PROSTOUT_PGN_65364_2, astPROSTOUTSPNList2},
        {DISP_HATZ_PROPB_PHYS_PGN_65280, NO_OF_SPN_HATZ_PROPB_PHYS_PGN_65280, astHATZPROPBSPNList},
        {DISP_AI_PGN_65237, NO_OF_SPN_AI_PGN_65237, astAISPNList},
        {DISP_ETC5_PGN_65219, NO_OF_SPN_ETC5_PGN_65219, astETC5SPNList},
        {DISP_OII_PGN_64554, NO_OF_SPN_OII_PGN_64554, astOIISPNList},
        {DISP_IT1_PGN_65154, NO_OF_SPN_IT1_PGN_65154, astIT1SPNList},
        {DISP_GFP_PGN_65163, NO_OF_SPN_GFP_PGN_65163, astGFPSPNList},
        {DISP_IMI1_PGN_65190, NO_OF_SPN_IMI1_PGN_65190, astIMISPNList},
        {DISP_FD1_PGN_65213  ,  NO_OF_SPN_FANDRV_PGN_65213,astFanDrive},
        {DISP_DLCC1_PGN_64775 , NO_OF_SPN_DLCC_PGN_64775  ,astDirectLampCtr1 },
        {DISP_GFC_PGN_65199, NO_OF_SPN_GFC_PGN_65199,astGFC},
        { DISP_EPT1_PGN_65187,  NO_OF_SPN_EPT1 ,astEP1 },
        { DISP_EPT2_PGN_65186,  NO_OF_SPN_EPT2 ,astEP2 },
        { DISP_EPT3_PGN_65185,  NO_OF_SPN_EPT3 ,astEP3 },
        { DISP_EPT4_PGN_65184,  NO_OF_SPN_EPT4 ,astEP4 },
        { DISP_EPT5_PGN_65183,  NO_OF_SPN_EPT5 ,astEP5 },
        { DISP_ET4_PGN_64870,   NO_OF_SPN_ET4  ,astET4 },
        { DISP_TCI4_PGN_65176,   NO_OF_SPN_TI4  ,astTI4 },
        { DISP_EFL_P12_PGN_64735,   NO_OF_EFLP12  ,astEFL_P12 },
        {DISP_HATZ_CCVS_PGN_65265, NO_OF_HATZ_CCVS_PGN_65265, astHATZCCVSSPNList},
    };
#endif

    /**
     * typedef function pointer for accessing the monitoring parameter
     * values from A_SENSE class of a perticular phase
     * @param ePhase
     * @return float value
     */
    typedef float(AC_SENSE::*_pGET_VAL_t)(PHASE_t ePhase);

    /**
     * typedef function pointer for accessing the monitoring parameter
     * values from A_SENSE class.
     * @param None
     * @return double value
     */
    typedef double(AC_SENSE::*_pGetdoubleValCommon_t)();

    /**
     * typedef function pointer for accessing the monitoring parameter
     * values from A_SENSE class.
     * @param None
     * @return float value
     */
    typedef float(AC_SENSE::*_pGET_FLOAT_VAL_COMMON_t)();

    //typedef float(ENGINE_MONITORING::*_pGET_FLOAT_VAL_t)();

    //Reference of objects
    HAL_Manager         &_hal;
    MANUAL_MODE         &_manualMode;
    ENGINE_MONITORING   &_EngineMon;
    CFGZ                &_cfgz;
    START_STOP          &_startStop;
    GCU_ALARMS          &_GCUAlarms;
    Display             &_Disp;
    CFGC                &_cfgc;
    J1939APP            &_j1939;
    BTS_MODE            &_BTSMode;
    CYCLIC_MODE         &_cyclicMode;
    /**
     * Private timer logo display for boot screens
     */
    stTimer _LogoTimer;

    /** Timer for LED blinking.
     */
    stTimer _LEDBlinkTimer;

    /** Timer for monitoring mode screens group switching.
     */
    stTimer _GroupSwitchingTimer;

    /** Variable to store the current operating mode.
     */
    BASE_MODES::GCU_OPERATING_MODE_t _eOpMode;

    /** Timer for
     */
    stTimer ExternalInputUpdateTimer;
    /**
     * Array of the screens which contains its enable / disable status.
     */
    bool _ArrScreenEnDs[DISP_MON_LAST];


    static bool _bMBModeChnageCMDRcvd;
    uint8_t _u8LanguageIndex;

    uint8_t _u8ScreenMin, _u8ScreenMax;
    typedef struct
    {
        uint8_t u8DPInResolution;
        double f64SpnVal;      // This array will have ASCII converted string for SPN data
        uint8_t SPNStatusErrorNA;  // SPN status Not available or Error.
    }SPNData_t;

    SPNData_t _DisplayPGNScreenData[MAX_SPN_ON_1_SCREEN];
    /**
     * This used to enable and disable the screen.
     * @param None
     * @return None
     */
    void prvConfigureScreenEnable();

    /**
     * It is used to display product logo.
     * @param None
     * @return None
     */
    void prvDisplayBootLogo();
    /**
     * It is used to display product info.
     * @param None
     * @return None
     */
    void prvProductInfo();
    /**
     * It is used to display all monitoring screens.
     * @param None
     * @return None
     */
    void prvDisplayMonScreen();

    /**
     * It is used to display for voltage and frequency data of
     * passed source type.
     * @param Source
     * @param AC system type
     * @return None
     */
    void prvPrintVtgFreqData(SOURCE_TYPE_t Source, uint8_t u8AcSystemType);

    /**
     * It is used to display power of passed type
     * @param eType
     * @param AC system type
     * @return None
     */
    void prvPrintPower(POWER_TYPE_t eType, uint8_t u8AcSystemType, SOURCE_TYPE_t eSourceType);

    /**
     * It is used to display sensor status.
     * @param stTemp - Sensor value and status
     * @param str    -  Unit string
     * @param etype  - data type
     * @return None
     */
    void prvPrintSensorStatus(A_SENSE::SENSOR_RET_t stTemp,char *str, DATA_TYPE_t etype);

    void prvPrintSensorStatus(A_SENSE::SENSOR_RET_t stTemp,char *str,
                                  DATA_TYPE_t etype , uint8_t u8x , uint8_t u8y );
    /** This is used to handle the mode switching action on stop key
     * @param none
     * @return none
     */
    void prvStopKeyPressAction();
    /** This is used to handle the mode switching action on start key
     * @param none
     * @return none
     */
    void prvStartKeyPressAction();
    /** This is used to handle the mode switching action on Auto key
     * @param none
     * @return none
     */
    void prvAutoKeyPressAction();

    /** This is used to Display error messages.
     * @param none
     * @return none
     */
    void prvDisplayError();


    void prvPrintExerciser(uint8_t _ScreenNo);
    void prvNormalMonScreens();
    void prvPrintCANMsgRcvError();
    void prvAssignNoOfDigitAfterDP(SPNData_t *StrArrayPtr, uint8_t u8PGNEnumNo, uint8_t u8SpnNo);
    unsigned int prvGetNoOfFractionalDigits(double dNumber);
    void prvPrintNotAvailable(uint8_t u8RowNum, uint8_t u8ColStart);

#if ENABLE_MON_J1939
    void prvJ1939MonScreens();
    void prvBuildLampIconScreen(void);
    void prvBuildExhaustIconScreen(void);
    void prvPrintSPNErrorVal(uint8_t u8RowNum, uint8_t u8ColStart);
    void prvPrintJ1939AfterTreatDataOnScreen(bool bIsAfterTreat1PGN, uint8_t u8RxATPGNNo);
    void prvPrintJ1939SPNVal(SPNData_t stData, uint8_t u8x, uint8_t u8y );
    void prvPrintPGN65378OrPGN65367(uint8_t u8PGNReceived);
    void prvPGNScreenDataAssignment(uint8_t u8PGNEnumNo);
    void prvGetMonImageCoordicates( uint8_t *pu8SizeX, uint8_t *pu8SizeY, uint8_t *pu8CordinateX, uint8_t *pu8CordinateY);
#endif

    void prvPrintSensorFaultStatus(A_SENSE::SENSOR_RET_t stTemp , uint16_t Xpos , uint16_t Ypos);

    /** This function returns true when Voltage Value has more than 3 digits.
     * @param u16VoltageVal
     * @return bool
     */
    bool prvIsValDigitsGreaterThan3(uint16_t u16VoltageVal);

};



#endif /* APL_HMI_MON_UI_MON_UI_H_ */
