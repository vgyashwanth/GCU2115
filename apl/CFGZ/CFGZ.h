/**
 * @file        [CFGZ]
 * @brief       CFGZ
 *              Provides API's for reading and writing the configuration
 *              parameter. Also it provides API for CFGZ verification.
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        26th Jun 2021
 * @author      Sudeep Chandrasekaran
 * @date        8th July 2021
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef APL_CFGZ_CFGZ_H_
#define APL_CFGZ_CFGZ_H_

#include <HMI/UI_DS.h>
#include "stdint.h"
#include "bsp.h"
#include "../../gc2k-bsp/bsp/DFLASH/DFLASH.h"
#include "CRC16.h"
#include "D_SENSE.h"
#include "HAL_Manager.h"
#include "MODBUS.h"
#include "RS485.h"
#include "productSelection.h"
#include "CFGC/CFGC.h"

// Max String length.
#define  MAX_AUX_STRING_SIZE      (21U)

// Number of blocks of CFGZ.
#define  NUMBER_OF_CFGZ_BLOCKS    (DFLASH_ACTIVE_PROFILE_LENGTH / DFLASH_BLOCK_SIZE )

/*Default modbus slave ID*/
#define CFGZ_DFLT_MODBUS_SLAVE_ID (2U)

#define PULSE_IP_SENSOR    A_SENSE::MPU_TYPE
#define DUMMY_ITEMS       0U

class CFGZ
{
  public:

    typedef struct
    {
        uint16_t u16Password1;
        uint16_t u16Password2;
        uint16_t u16CRC;
        uint16_t u16Dummy;
    }PASSWORD_t;

    /**
     * Enum values for floating point variable.
     */
    typedef enum
    {
        //RushiStart
        ID_BTS_CONFIG_LOW_BATT_THRESHOLD ,

        ID_LOP_RES_DIG_J_SHUTDOWN_THRESHOLD,
        ID_LOP_RES_DIG_J_WARNING_THRESHOLD,
        ID_LOP_RES_DIG_J_R1,
        ID_LOP_RES_DIG_J_V1,
        ID_LOP_RES_DIG_J_R2,
        ID_LOP_RES_DIG_J_V2,
        ID_LOP_RES_DIG_J_R3,
        ID_LOP_RES_DIG_J_V3,
        ID_LOP_RES_DIG_J_R4,
        ID_LOP_RES_DIG_J_V4,
        ID_LOP_RES_DIG_J_R5,
        ID_LOP_RES_DIG_J_V5,
        ID_LOP_RES_DIG_J_R6,
        ID_LOP_RES_DIG_J_V6,
        ID_LOP_RES_DIG_J_R7,
        ID_LOP_RES_DIG_J_V7,
        ID_LOP_RES_DIG_J_R8,
        ID_LOP_RES_DIG_J_V8,
        ID_LOP_RES_DIG_J_R9,
        ID_LOP_RES_DIG_J_V9,
        ID_LOP_RES_DIG_J_R10,
        ID_LOP_RES_DIG_J_V10,

        ID_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD,
        ID_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD,
        ID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY,
        ID_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD,
        ID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION,
        ID_FUEL_LVL_DIG_K_FUEL_IN_LITERS,
        ID_FUEL_LVL_DIG_K_R1,
        ID_FUEL_LVL_DIG_K_L1,
        ID_FUEL_LVL_DIG_K_R2,
        ID_FUEL_LVL_DIG_K_L2,
        ID_FUEL_LVL_DIG_K_R3,
        ID_FUEL_LVL_DIG_K_L3,
        ID_FUEL_LVL_DIG_K_R4,
        ID_FUEL_LVL_DIG_K_L4,
        ID_FUEL_LVL_DIG_K_R5,
        ID_FUEL_LVL_DIG_K_L5,
        ID_FUEL_LVL_DIG_K_R6,
        ID_FUEL_LVL_DIG_K_L6,
        ID_FUEL_LVL_DIG_K_R7,
        ID_FUEL_LVL_DIG_K_L7,
        ID_FUEL_LVL_DIG_K_R8,
        ID_FUEL_LVL_DIG_K_L8,
        ID_FUEL_LVL_DIG_K_R9,
        ID_FUEL_LVL_DIG_K_L9,
        ID_FUEL_LVL_DIG_K_R10,
        ID_FUEL_LVL_DIG_K_L10,

        ID_ENG_TEMP_DIG_L_THRESHOLD,
        ID_ENG_TEMP_DIG_L_R1,
        ID_ENG_TEMP_DIG_L_T1,
        ID_ENG_TEMP_DIG_L_R2,
        ID_ENG_TEMP_DIG_L_T2,
        ID_ENG_TEMP_DIG_L_R3,
        ID_ENG_TEMP_DIG_L_T3,
        ID_ENG_TEMP_DIG_L_R4,
        ID_ENG_TEMP_DIG_L_T4,
        ID_ENG_TEMP_DIG_L_R5,
        ID_ENG_TEMP_DIG_L_T5,
        ID_ENG_TEMP_DIG_L_R6,
        ID_ENG_TEMP_DIG_L_T6,
        ID_ENG_TEMP_DIG_L_R7,
        ID_ENG_TEMP_DIG_L_T7,
        ID_ENG_TEMP_DIG_L_R8,
        ID_ENG_TEMP_DIG_L_T8,
        ID_ENG_TEMP_DIG_L_R9,
        ID_ENG_TEMP_DIG_L_T9,
        ID_ENG_TEMP_DIG_L_R10,
        ID_ENG_TEMP_DIG_L_T10,

        ID_SHEL_TEMP_DIG_M_HIGH_TEMP_THRESHOLD,
        ID_SHEL_TEMP_DIG_M_LOW_TEMP_THRESHOLD,
        ID_SHEL_TEMP_DIG_M_R1,
        ID_SHEL_TEMP_DIG_M_T1,
        ID_SHEL_TEMP_DIG_M_R2,
        ID_SHEL_TEMP_DIG_M_T2,
        ID_SHEL_TEMP_DIG_M_R3,
        ID_SHEL_TEMP_DIG_M_T3,
        ID_SHEL_TEMP_DIG_M_R4,
        ID_SHEL_TEMP_DIG_M_T4,
        ID_SHEL_TEMP_DIG_M_R5,
        ID_SHEL_TEMP_DIG_M_T5,
        ID_SHEL_TEMP_DIG_M_R6,
        ID_SHEL_TEMP_DIG_M_T6,
        ID_SHEL_TEMP_DIG_M_R7,
        ID_SHEL_TEMP_DIG_M_T7,
        ID_SHEL_TEMP_DIG_M_R8,
        ID_SHEL_TEMP_DIG_M_T8,
        ID_SHEL_TEMP_DIG_M_R9,
        ID_SHEL_TEMP_DIG_M_T9,
        ID_SHEL_TEMP_DIG_M_R10,
        ID_SHEL_TEMP_DIG_M_T10,

        ID_AUX_S2_RES_DIG_N_THRESHOLD,
        ID_AUX_S2_RES_DIG_N_R1,
        ID_AUX_S2_RES_DIG_N_V1,
        ID_AUX_S2_RES_DIG_N_R2,
        ID_AUX_S2_RES_DIG_N_V2,
        ID_AUX_S2_RES_DIG_N_R3,
        ID_AUX_S2_RES_DIG_N_V3,
        ID_AUX_S2_RES_DIG_N_R4,
        ID_AUX_S2_RES_DIG_N_V4,
        ID_AUX_S2_RES_DIG_N_R5,
        ID_AUX_S2_RES_DIG_N_V5,
        ID_AUX_S2_RES_DIG_N_R6,
        ID_AUX_S2_RES_DIG_N_V6,
        ID_AUX_S2_RES_DIG_N_R7,
        ID_AUX_S2_RES_DIG_N_V7,
        ID_AUX_S2_RES_DIG_N_R8,
        ID_AUX_S2_RES_DIG_N_V8,
        ID_AUX_S2_RES_DIG_N_R9,
        ID_AUX_S2_RES_DIG_N_V9,
        ID_AUX_S2_RES_DIG_N_R10,
        ID_AUX_S2_RES_DIG_N_V10,

        ID_AUX_S3_DIG_O_SHUTDOWN_THRESHOLD,
        ID_AUX_S3_DIG_O_WARNING_THRESHOLD,
        ID_AUX_S3_DIG_O_I1_V1,
        ID_AUX_S3_DIG_O_P1,
        ID_AUX_S3_DIG_O_I2_V2,
        ID_AUX_S3_DIG_O_P2,
        ID_AUX_S3_DIG_O_I3_V3,
        ID_AUX_S3_DIG_O_P3,
        ID_AUX_S3_DIG_O_I4_V4,
        ID_AUX_S3_DIG_O_P4,
        ID_AUX_S3_DIG_O_I5_V5,
        ID_AUX_S3_DIG_O_P5,
        ID_AUX_S3_DIG_O_I6_V6,
        ID_AUX_S3_DIG_O_P6,
        ID_AUX_S3_DIG_O_I7_V7,
        ID_AUX_S3_DIG_O_P7,
        ID_AUX_S3_DIG_O_I8_V8,
        ID_AUX_S3_DIG_O_P8,
        ID_AUX_S3_DIG_O_I9_V9,
        ID_AUX_S3_DIG_O_P9,
        ID_AUX_S3_DIG_O_I10_V10,
        ID_AUX_S3_DIG_O_P10,

        ID_AUX_S4_DIG_P_SHUTDOWN_THRESHOLD,
        ID_AUX_S4_DIG_P_NOTIFICATION_THRESHOLD,
        ID_AUX_S4_DIG_P_FUEL_THEFT_THRESHOLD,
        ID_AUX_S4_DIG_P_FUEL_CONSUMPTION,
        ID_AUX_S4_DIG_P_FUEL_IN_LITERS,
        ID_AUX_S4_DIG_P_SENSOR_LOW_VTG,
        ID_AUX_S4_DIG_P_SENSOR_HIGH_VTG,
        ID_AUX_S4_DIG_P_TANK_WITH_STEP,
        ID_AUX_S4_DIG_P_TANK_WIDTH,
        ID_AUX_S4_DIG_P_TANK_LENGTH_1,
        ID_AUX_S4_DIG_P_TANK_HEIGHT_1,
        ID_AUX_S4_DIG_P_TANK_LENGTH_2,
        ID_AUX_S4_DIG_P_TANK_HEIGHT_2,

        ID_FREQ_MONITOR_UF_SHUTDOWN_THRESHOLD,
        ID_FREQ_MONITOR_UF_WARNING_THRESHOLD,
        ID_FREQ_MONITOR_OF_SHUTDOWN_THRESHOLD,
        ID_FREQ_MONITOR_OF_WARNING_THRESHOLD,


        ID_CURRENT_MONITOR_OVER_CURR_DELAY,
        ID_FAN_CURR_MONITOR_CURR_MON_DELAY,
        ID_LOAD_MONITOR_OVERLOAD_MON_DELAY,
        ID_LOAD_MONITOR_UNBAL_LOAD_DELAY,

        ID_CRANK_DISCONNECT_ALT_FREQUENCY,
        ID_CRANK_DISCONNECT_CHG_ALT_THRESHOLD,
        ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD,
        ID_SPEED_MONITOR_UNDER_SPEED_DELAY,
        ID_SPEED_MONITOR_OVER_SPEED_THRESHOLD,
        ID_SPEED_MONITOR_OVER_SPEED_DELAY,
        ID_SPEED_MONITOR_GROSS_OS_THRESHOLD,
        ID_BATTERY_MONITOR_LOW_VOLT_THRESHOLD,
        ID_BATTERY_MONITOR_LOW_VOLT_DELAY,
        ID_BATTERY_MONITOR_HIGH_VOLT_THRESHOLD,
        ID_BATTERY_MONITOR_HIGH_VOLT_DELAY,
        ID_CHARGE_ALT_MON_FAIL_THRESHOLD,
        ID_CHARGE_ALT_MON_FAIL_DELAY,
        ID_PREHEAT_ENG_TEMP_THRESHOLD,

        //RushiEnd


        ID_ENGINE_GAIN,
        ID_LOP_LVL_SHUTDOWN_THRESH,
        ID_LOP_LVL_WARNING_THRESH,

        ID_FLOAT_LAST
    }FLOAT_PARAMS_t;

    /**
     * Enum values for unsigned integer type variable.
     */
    typedef enum
    {
        //RushiStart
        ID_BTS_CONFIG_LOW_BATT_MON_DELAY ,
        ID_BTS_CONFIG_DG_RUN_DURATION ,

        ID_CYCLIC_CONFIG_DG_OFF_DURATION ,
        ID_CYCLIC_CONFIG_DG_ON_DURATION ,

        ID_NIGHT_MODE_CONFIG_START_TIME ,
        ID_NIGHT_MODE_CONFIG_OFF_DURATION ,

        ID_CRANKING_TIMER_CRANK_HOLD_TIME,
        ID_CRANKING_TIMER_CRANK_REST_TIME,
        ID_CRANKING_TIMER_MANUAL_START_DELAY,
        ID_CRANKING_TIMER_AUTO_START_DELAY,
        ID_GENERAL_TIMER_SAFETY_MONITOR_DELAY,
        ID_GENERAL_TIMER_MAINS_DETECT_DELAY,
        ID_GENERAL_TIMER_ALT_DETECT_DELAY,
        ID_GENERAL_TIMER_WARM_UP_DELAY,
        ID_GENERAL_TIMER_RETN_TO_MAINS_DELAY,
        ID_GENERAL_TIMER_ENG_COOL_TIME,
        ID_GENERAL_TIMER_STOP_ACTION_TIME,
        ID_GENERAL_TIMER_ADDN_STOPPING_TIME,
        ID_GENERAL_TIMER_LOAD_TRANSFER_DELAY,
        ID_GENERAL_TIMER_PWR_SAVE_MODE_DELAY,
        ID_GENERAL_TIMER_SCRN_CHNGOVER_TIME,
        ID_GENERAL_TIMER_DEEP_SLP_MODE_DELAY,
        ID_GENERAL_TIMER_SOUNDER_ALARM_TIMER,
        ID_GENERAL_TIMER_TEST_MODE_TIMER,

        ID_ALT_CONFIG_MIN_HEALTHY_VOLT,
        ID_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD,
        ID_VOLT_MONITOR_UV_WARNING_THRESHOLD,
        ID_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD,
        ID_VOLT_MONITOR_OV_WARNING_THRESHOLD,

        ID_CURRENT_MONITOR_LOAD_CT_RATIO,
        ID_CURRENT_MONITOR_OVER_CURR_THRESHOLD,
        ID_FAN_CURR_MONITOR_FAN_MON_CT_RATIO,
        ID_FAN_CURR_MONITOR_HIGH_CURR_THRESHOLD,
        ID_FAN_CURR_MONITOR_LOW_CURR_THRESHOLD,
        ID_LOAD_MONITOR_GEN_RATING,
        ID_LOAD_MONITOR_OVERLOAD_THRESHOLD,
        ID_LOAD_MONITOR_UNBAL_LOAD_THRESHOLD,
        ID_LOAD_MONITOR_UNBAL_LOAD_ACT_THRESH,

        ID_CRANK_DISCONNECT_ENGINE_SPEED,
        ID_PREHEAT_PREHEAT_TIMER,
        ID_PREHEAT_ENG_TEMPERATURE,
        ID_PREHEAT_AMB_TEMPERATURE,
        //RushiEnd

        ID_AUTO_EXIT_TIME,


        ID_STOPPING_IDLE_TIME,

        ID_INITIAL_LOW_SPEED,
        ID_LOW_BAT_VTG_DELAY,
        ID_HIGH_BAT_VTG_DELAY,
        ID_CHARG_ALT_FAIL_DELAY,
        ID_PREHEAT_TIMER,
        ID_ENGINE_TEMP_LIMIT_THREH,
        ID_CLNT_TEMP_ON_THRESH,
        ID_CLNT_TEMP_OFF_THRESH,
        ID_TIMEOUT_AFTER_ACTIVATION,
        ID_ENG_REQUESTED_SPEED_TO_ECU,
        ID_SGC_SOURCE_ADDRESS,
        ID_ECU_SOURCE_ADDRESS,
        ID_ECU_COMM_FAILURE_ACT_DELAY,
        ID_ECU_AMBER_ACT_DELAY,
        ID_ECU_RED_ACT_DELAY,
        ID_ECU_MALFUNCTION_ACT_DELAY,
        ID_ECU_PROTECT_ACT_DELAY,
        ID_HIGH_CLNT_TEMP_SHUTDOWN_THRESH,
        ID_HIGH_CLNT_TEMP_WARNING_THRESH,
        ID_MAINTENANCE_DUE_HOURS,
        ID_FILT_MAINT_THRESH_YEAR,


        ID_UINT16_LAST
    }UINT16_PARAMS_t;
    /**
     * Enum values for unsigned char type variable.
     */
    typedef enum
    {
        //RushiStart
        ID_GENERAL_PROFILE_NAME ,
        ID_GENERAL_POWER_ON_MODE ,
        ID_GENERAL_POWER_ON_LAMP_TEST ,
        ID_GENERAL_PASSWORD_LOCK ,

        ID_DISPLAY_CONTRAST ,
        ID_DISPLAY_POWER_SAVE_MODE ,

        ID_MODBUS_COMM_COMM_MODE ,
        ID_MODBUS_COMM_MODBUS_SLAVE_ID ,
        ID_MODBUS_COMM_MODBUS_BAUDRATE ,
        ID_MODBUS_COMM_PARITY ,

        ID_BTS_CONFIG_BATTERY_MON ,

        ID_CYCLIC_CONFIG_CYCLIC_MODE ,

        ID_NIGHT_MODE_CONFIG_NIGHT_MODE ,

        ID_DIG_IN_A_SOURCE,
        ID_DIG_IN_A_POLARITY,
        ID_DIG_IN_A_ACTION,
        ID_DIG_IN_A_ACTIVATION,
        ID_DIG_IN_A_ACTIVATION_DELAY,

        ID_DIG_IN_B_SOURCE,
        ID_DIG_IN_B_POLARITY,
        ID_DIG_IN_B_ACTION,
        ID_DIG_IN_B_ACTIVATION,
        ID_DIG_IN_B_ACTIVATION_DELAY,

        ID_DIG_IN_C_SOURCE,
        ID_DIG_IN_C_POLARITY,
        ID_DIG_IN_C_ACTION,
        ID_DIG_IN_C_ACTIVATION,
        ID_DIG_IN_C_ACTIVATION_DELAY,

        ID_DIG_IN_D_SOURCE,
        ID_DIG_IN_D_POLARITY,
        ID_DIG_IN_D_ACTION,
        ID_DIG_IN_D_ACTIVATION,
        ID_DIG_IN_D_ACTIVATION_DELAY,

        ID_DIG_IN_E_SOURCE,
        ID_DIG_IN_E_POLARITY,
        ID_DIG_IN_E_ACTION,
        ID_DIG_IN_E_ACTIVATION,
        ID_DIG_IN_E_ACTIVATION_DELAY,

        ID_DIG_IN_F_SOURCE,
        ID_DIG_IN_F_POLARITY,
        ID_DIG_IN_F_ACTION,
        ID_DIG_IN_F_ACTIVATION,
        ID_DIG_IN_F_ACTIVATION_DELAY,

        ID_DIG_IN_G_SOURCE,
        ID_DIG_IN_G_POLARITY,
        ID_DIG_IN_G_ACTION,
        ID_DIG_IN_G_ACTIVATION,
        ID_DIG_IN_G_ACTIVATION_DELAY,

        ID_DIG_IN_H_SOURCE,
        ID_DIG_IN_H_POLARITY,
        ID_DIG_IN_H_ACTION,
        ID_DIG_IN_H_ACTIVATION,
        ID_DIG_IN_H_ACTIVATION_DELAY,

        ID_DIG_IN_I_SOURCE,
        ID_DIG_IN_I_POLARITY,
        ID_DIG_IN_I_ACTION,
        ID_DIG_IN_I_ACTIVATION,
        ID_DIG_IN_I_ACTIVATION_DELAY,

        ID_LOP_RES_DIG_J_SENSOR_SELECTION,
        ID_LOP_RES_DIG_J_DIG_SOURCE,
        ID_LOP_RES_DIG_J_DIG_POLARITY,
        ID_LOP_RES_DIG_J_DIG_ACTION,
        ID_LOP_RES_DIG_J_DIG_ACTIVATION,
        ID_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY,
        ID_LOP_RES_DIG_J_SHUTDOWN,
        ID_LOP_RES_DIG_J_WARNING,
        ID_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION,

        ID_FUEL_LVL_DIG_K_SENSOR_SELECTION,
        ID_FUEL_LVL_DIG_K_DIG_SOURCE,
        ID_FUEL_LVL_DIG_K_DIG_POLARITY,
        ID_FUEL_LVL_DIG_K_DIG_ACTION,
        ID_FUEL_LVL_DIG_K_DIG_ACTIVATION,
        ID_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY,
        ID_FUEL_LVL_DIG_K_SHUTDOWN,
        ID_FUEL_LVL_DIG_K_NOTIFICATION,
        ID_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING,

        ID_ENG_TEMP_DIG_L_SENSOR_SELECTION,
        ID_ENG_TEMP_DIG_L_DIG_SOURCE,
        ID_ENG_TEMP_DIG_L_DIG_POLARITY,
        ID_ENG_TEMP_DIG_L_DIG_ACTION,
        ID_ENG_TEMP_DIG_L_DIG_ACTIVATION,
        ID_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY,
        ID_ENG_TEMP_DIG_L_ACTION,
        ID_ENG_TEMP_DIG_L_OPEN_CKT_WARNING,

        ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION,
        ID_SHEL_TEMP_DIG_M_DIG_SOURCE,
        ID_SHEL_TEMP_DIG_M_DIG_POLARITY,
        ID_SHEL_TEMP_DIG_M_DIG_ACTION,
        ID_SHEL_TEMP_DIG_M_DIG_ACTIVATION,
        ID_SHEL_TEMP_DIG_M_DIG_ACTIVATION_DELAY,
        ID_SHEL_TEMP_DIG_M_HIGH_TEMP_MON_DELAY,
        ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION,
        ID_SHEL_TEMP_DIG_M_OPEN_CKT_NOTIFICATION,

        ID_AUX_S2_RES_DIG_N_SENSOR_SELECTION,
        ID_AUX_S2_RES_DIG_N_DIG_SOURCE,
        ID_AUX_S2_RES_DIG_N_DIG_POLARITY,
        ID_AUX_S2_RES_DIG_N_DIG_ACTION,
        ID_AUX_S2_RES_DIG_N_DIG_ACTIVATION,
        ID_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY,
        ID_AUX_S2_RES_DIG_N_ACTION,
        ID_AUX_S2_RES_DIG_N_THRESHOLD_TYPE,
        ID_AUX_S2_RES_DIG_N_OPEN_CKT_WARNING,

        ID_AUX_S3_DIG_O_SENSOR_SELECTION,
        ID_AUX_S3_DIG_O_DIG_SOURCE,
        ID_AUX_S3_DIG_O_DIG_POLARITY,
        ID_AUX_S3_DIG_O_DIG_ACTION,
        ID_AUX_S3_DIG_O_DIG_ACTIVATION,
        ID_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY,
        ID_AUX_S3_DIG_O_SHUTDOWN,
        ID_AUX_S3_DIG_O_WARNING,
        ID_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION,

        ID_AUX_S4_DIG_P_SENSOR_SELECTION,
        ID_AUX_S4_DIG_P_DIG_SOURCE,
        ID_AUX_S4_DIG_P_DIG_POLARITY,
        ID_AUX_S4_DIG_P_DIG_ACTION,
        ID_AUX_S4_DIG_P_DIG_ACTIVATION,
        ID_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY,
        ID_AUX_S4_DIG_P_SHUTDOWN,
        ID_AUX_S4_DIG_P_NOTIFICATION,
        ID_AUX_S4_DIG_P_FUEL_THEFT_WARNING,

        ID_OUT_A_SOURCE,
        ID_OUT_A_ON_ACTIVATION,
        ID_OUT_B_SOURCE,
        ID_OUT_B_ON_ACTIVATION,
        ID_OUT_C_SOURCE,
        ID_OUT_C_ON_ACTIVATION,
        ID_OUT_D_SOURCE,
        ID_OUT_D_ON_ACTIVATION,
        ID_OUT_E_SOURCE,
        ID_OUT_E_ON_ACTIVATION,
        ID_OUT_F_SOURCE,
        ID_OUT_F_ON_ACTIVATION,
        ID_OUT_G_SOURCE,
        ID_OUT_G_ON_ACTIVATION,

        ID_ALT_CONFIG_ALT_PRESENT,
        ID_ALT_CONFIG_NUMBER_OF_POLES,
        ID_ALT_CONFIG_ALT_AC_SYSTEM,
        ID_ALT_CONFIG_MIN_HEALTHY_FREQ,
        ID_ALT_CONFIG_PHASE_REVERSAL_DETECT,
        ID_ALT_CONFIG_PHASE_REVERSAL_ACTION,
        ID_ALT_CONFIG_AUTO_LOAD_TRANSFER,
        ID_ALT_CONFIG_ALT_WAVE_DETECTION,
        ID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN,
        ID_VOLT_MONITOR_UNDER_VOLT_WARNING,
        ID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN,
        ID_VOLT_MONITOR_OVER_VOLT_WARNING,
        ID_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN,
        ID_FREQ_MONITOR_UNDER_FREQ_WARNING,
        ID_FREQ_MONITOR_OVER_FREQ_SHUTDOWN,
        ID_FREQ_MONITOR_OVER_FREQ_WARNING,

        ID_CURRENT_MONITOR_OVER_CURR_ACTION,
        ID_CURRENT_MONITOR_CT_CORRECTION_FACTOR,
        ID_CURRENT_MONITOR_CT_LOCATION,
        ID_FAN_CURR_MONITOR_FAN_CURRENT_MON,
        ID_FAN_CURR_MONITOR_HIGH_CURR_ACTION,
        ID_FAN_CURR_MONITOR_LOW_CURR_ACTION,
        ID_LOAD_MONITOR_FULL_LOAD_CURRENT,
        ID_LOAD_MONITOR_OVERLOAD_ACTION,
        ID_LOAD_MONITOR_UNBAL_LOAD_ACTION,

        ID_MAINS_CONFIG_MAINS_MONITORING,
        ID_MAINS_CONFIG_MAINS_AC_SYSTEM,
        ID_MAINS_CONFIG_PHASE_REVERSAL_DETECT,
        ID_MAINS_CONFIG_PHASE_REVERSAL_ACTION,
        ID_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH,
        ID_UNDER_VOLT_MON_ENABLE,
        ID_UNDER_VOLT_MON_TRIP,
        ID_UNDER_VOLT_MON_RETURN,
        ID_OVER_VOLT_MON_ENABLE,
        ID_OVER_VOLT_MON_TRIP,
        ID_OVER_VOLT_MON_RETURN,
        ID_UNDER_FREQ_MON_ENABLE,
        ID_UNDER_FREQ_MON_TRIP,
        ID_UNDER_FREQ_MON_RETURN,
        ID_OVER_FREQ_MON_ENABLE,
        ID_OVER_FREQ_MON_TRIP,
        ID_OVER_FREQ_MON_RETURN,

        ID_CRANK_DISCONNECT_START_ATTEMPTS,
        ID_CRANK_DISCONNECT_DISCONN_ON_LOP_SENS,
        ID_CRANK_DISCONNECT_DISCONN_LOP_SENS,
        ID_CRANK_DISCONNECT_MON_LLOP_BEFORE_CRANK,
        ID_CRANK_DISCONNECT_MON_LOP_BEFORE_CRANK,
        ID_CRANK_DISCONNECT_DISCONN_ON_LLOP_SW,
        ID_CRANK_DISCONNECT_LLOP_SW_TRANS_TIME,
        ID_CRANK_DISCONNECT_DISCONN_ON_CHG_ALT_VOLT,
        ID_SPEED_MONITOR_SPEED_SENSE_SOURCE,
        ID_SPEED_MONITOR_RESERVED, // For Teeth the parameter is reserved.
        ID_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN,
        ID_BATTERY_MONITOR_LOW_VOLT_ACTION,
        ID_BATTERY_MONITOR_HIGH_VOLT_ACTION,
        ID_BATTERY_MONITOR_BATTERY_MON_BY_J1939,
        ID_CHARGE_ALT_MON_FAIL_ACTION,
        ID_CHARGE_ALT_MON_CHARGE_ALT_MON_BY_J1939,

        ID_MAINT_ALARM_ACTION,
        ID_MAINT_ALARM_DUE_AT_ENGINE_HOURS,
        ID_ALARM_DUE_DATE_SERVICE_DATE_1,
        ID_ALARM_DUE_DATE_SERVICE_DATE_2,
        ID_ALARM_DUE_DATE_SERVICE_DATE_3,

//RushiEnd


        ID_CHARG_ALT_FAIL_ACTION,
        ID_ENGINE_TEMP_LIMIT_EN,
        ID_CLNT_TEMP_CTRL_EN,
        ID_CLNT_TEMP_CTRL,
        ID_AFT_ACTIVATION_THRESH,
        ID_AFT_DEACTIVATION_THRESH,
        ID_MON_AFT_FROM_ENGINE_ON,
        ID_ENGINE_TYPE,
        ID_LOP_FROM_ENG,
        ID_CLNT_TEMP_FROM_ENG,
        ID_ENGINE_SPEED_FROM_ENG,
        ID_RUNNING_HOURS_FROM_ENG,
        ID_BATTERY_VOLT_FROM_ENG,
        ID_SPEED_TO_ECU,
        ID_START_STOP_TO_ECU,
        ID_PREHEAT_TO_ECU,
        ID_ENGINE_FRQ,

        ID_ECU_COMM_FAILURE_ACTION,
        ID_ECU_COMM_FAILURE_ACTIVATION,
        ID_ECU_AMBER_ACTION,
        ID_ECU_AMBER_ACTIVATION,
        ID_ECU_RED_ACTION,
        ID_ECU_RED_ACTIVATION,
        ID_ECU_MALFUNCTION_ACTION,
        ID_ECU_MALFUNCTION_ACTIVATION,
        ID_ECU_PROTECT_ACTION,
        ID_ECU_PROTECT_ACTIVATION,
        ID_LOP_LVL_SHUTDOWN_EN,
        ID_LOP_LVL_WARNING_EN,
        ID_CLNT_TEMP_THRESH_TYPE,
        ID_CLNT_TEMP_SHUTDOWN_EN,
        ID_CLNT_TEMP_WARNING_EN,
        ID_MAINTENANCE_ALARM_ACTION,
        ID_MAINT_ASH_LOAD_EN,
        ID_FILT_MAINT_THRESH_DAY,
        ID_FILT_MAINT_THRESH_MONTH,


        ID_UINT8_LAST
    }UINT8_PARAMS_t;

    /**
     * Enum values for String type variable.
     */
    typedef enum
    {

        ID_ARR_AUX_INPUT_A,
        ID_ARR_AUX_INPUT_B,
        ID_ARR_AUX_INPUT_C,
        ID_ARR_AUX_INPUT_D,
        ID_ARR_AUX_INPUT_E,
        ID_ARR_AUX_INPUT_F,
        ID_ARR_AUX_INPUT_G,
        ID_ARR_AUX_INPUT_H,
        ID_ARR_AUX_INPUT_I,
        ID_ARR_AUX_INPUT_J,
        ID_ARR_AUX_INPUT_K,
        ID_ARR_AUX_INPUT_L,
        ID_ARR_AUX_INPUT_M,
        ID_ARR_AUX_INPUT_N,
        ID_ARR_AUX_INPUT_O,
        ID_ARR_AUX_INPUT_P,
        ID_ARR_SENSOR_S1_NAME,
        ID_ARR_SENSOR_S2_NAME,
        ID_ARR_SENSOR_S3_NAME,
        ID_ARR_SENSOR_S4_NAME,
        ID_ARR_PROFILE,

        ID_ARRAY_20_BYTE_LAST
     }ARRAY_PARAMS_t;

     /**
      * This is the structure of all configuration parameter
      */
     typedef struct __attribute__((packed))
     {
         float    f32ArrParam[ID_FLOAT_LAST];
         uint16_t u16ArrParam[ID_UINT16_LAST];
         uint8_t  u8ArrParam[ID_UINT8_LAST];
         char     u8ArrStringParam[ID_ARRAY_20_BYTE_LAST][MAX_AUX_STRING_SIZE];
#if DUMMY_ITEMS
         uint8_t  uint8Dummy[DUMMY_ITEMS];
#endif
     }CFGZ_PARAMS_t;



     typedef enum{
         LANGUAGE_ENGLISH,
         LANGUAGE_SPANISH,
         LANGUAGE_CHINSESE,
     }CFGZ_LANGUAGE_t;
    /*List of sensors that can be connected to the digital input*/
    typedef enum {
        CFGZ_SENSOR_NOT_USED         = 0,
        CFGZ_USER_CONFIGURED_SENSOR,
        CFGZ_LOW_FUEL_LEVEL_SWITCH,
        CFGZ_LOW_LUBE_OIL_PRESS_SWITCH,
        CFGZ_HIGH_ENGINE_TEMP_SWITCH,
        CFGZ_LOW_WATER_LVL_SWITCH,
        CFGZ_EMERGENCY_STOP,
        CFGZ_REMOTE_START_STOP,
        CFGZ_SIMULATE_START,
        CFGZ_SIMULATE_STOP,
        CFGZ_SIMULATE_AUTO,
        CFGZ_CLOSE_GEN_OPEN_MAINS_SWITCH ,
        CFGZ_CLOSE_MAINS_OPEN_GEN_SWITCH ,
        CFGZ_SIMULATE_MAINS,
        CFGZ_VBEL_BROKEN_SWITCH,
        CFGZ_MAINS_CONTACTOR_LATCHED,
        CFGZ_GEN_CONTACTOR_LATCHED,
        CFGZ_BATTERY_CHARGER_FAIL,
        CFGZ_SMOKE_FIRE,
        CFGZ_MODE_SELECT,
        CFGZ_AMB_TEMP_SELECT,

} CFGZ_DIGITAL_SENSORS_t;

    typedef enum {
        CFGZ_CLOSE_TO_ACTIVATE = 0,
        CFGZ_OPEN_TO_ACTIVATE
    } CFGZ_SENSOR_POLARITY_t;

    typedef enum{
        CFGZ_ACTION_NONE          = 0,
        CFGZ_ACTION_NOTIFICATION,
        CFGZ_ACTION_WARNING,
        CFGZ_ACTION_ELECTRICAL_TRIP,
        CFGZ_ACTION_SHUTDOWN,

    } CFGZ_SENSOR_ACTION_t;

    typedef enum
    {
        CFGZ_ACTION_NONE_1 =0,
        CFGZ_ACTION_NOTIFICATION_2,
        CFGZ_ACTION_WARNING_2,
    }CFGZ_MAINTENANCE_ACTION;
    typedef enum {
        CFGZ_NEVER_ACTIVATE    = 0,
        CFGZ_FROM_ENGINE_START,
        CFGZ_FROM_MONITORING_ON,
        CFGZ_ALWAYS,
        CFGZ_ACTIVATION_AFTER_FUEL_RELAY_ON
    } CFGZ_SENSOR_ACTIVATION_t;

    typedef enum {
        CFGZ_ANLG_SENSOR_NOT_USED   = 0,
        CFGZ_ANLG_DIG_IN,
        CFGZ_ANLG_CUSTOM_SENSOR1,
        CFGZ_ANLG_CUSTOM_SENSOR2,
        CFGZ_ANLG_CUSTOM_SENSOR3,
    } CFGZ_ANLG_SENSORS_t;

    typedef enum {
        CFGZ_LOP_CURR_SENS_NOT_USED   = 0,
        CFGZ_CURR_ANLG_DIG_IN,
        CFGZ_ANLG_LOP_CURR_SENSOR
    } CFGZ_LOP_CURR_SENSORS_t;

    /*An helper type to define mapping of sensors between CFGZ and A_SENSE*/
    typedef struct {
        CFGZ_ANLG_SENSORS_t  eCfgSensorTyp;
        AnalogSensor::TYPS_t eDsenseSensorTyp;
    } ASENSOR_MAP_ROW_t;

    /*List of actuators that can be connected to HSD's*/
    typedef enum {
        CFGZ_NOT_CONFIGURED = 0,
        CFGZ_AUDIBLE_ALARM,
        CFGZ_VBAT_OV,
        CFGZ_VBAT_UV,
        CFGZ_CA_SHUTDOWN,
        CFGZ_CA_WARNING,
        CFGZ_CLOSE_GEN_CONTACTOR,
        CFGZ_CLOSE_MAINS_CONTACTOR,

        CFGZ_MAINS_FAILURE,
        CFGZ_ALARM,
        CFGZ_ELEC_TRIP,
        CFGZ_SHUTDOWN,
        CFGZ_WARNING,
        CFGZ_COOLING_ON,
        CFGZ_DIG_IN_A,
        CFGZ_DIG_IN_B,
        CFGZ_DIG_IN_C,
        CFGZ_DIG_IN_D,
        CFGZ_DIG_IN_E,
        CFGZ_DIG_IN_F,
        CFGZ_DIG_IN_G,
        CFGZ_DIG_IN_H,
        CFGZ_DIG_IN_I,
        CFGZ_DIG_IN_J,
        CFGZ_DIG_IN_K,
        CFGZ_DIG_IN_L,
        CFGZ_DIG_IN_M,
        CFGZ_DIG_IN_N,
        CFGZ_DIG_IN_O,
        CFGZ_DIG_IN_P,
        CFGZ_E_STOP,
        CFGZ_STOP_SOLENOID,
        CFGZ_FAIL_TO_START,
        CFGZ_FAIL_TO_STOP,
        CFGZ_FUEL_RELAY,
        CFGZ_GEN_AVLBL,
        CFGZ_GEN_R_OV_SHUTDOWN,
        CFGZ_GEN_R_UV_SHUTDOWN,
        CFGZ_GEN_Y_OV_SHUTDOWN,
        CFGZ_GEN_Y_UV_SHUTDOWN,
        CFGZ_GEN_B_OV_SHUTDOWN,
        CFGZ_GEN_B_UV_SHUTDOWN,
        CFGZ_GEN_OC,
        CFGZ_HIGH_TEMP,
        CFGZ_LOW_FUEL,
        CFGZ_LOW_FUEL_NOTIFICATION,
        CFGZ_LOW_PRES,
        CFGZ_MAINS_HIGH,
        CFGZ_MAINS_LOW,
        CFGZ_OIL_CKT_OPEN,
        CFGZ_OPEN_GEN_OUT,
        CFGZ_OPEN_MAINS_OUT,
        CFGZ_OF_SHUTDOWN,
        CFGZ_OS_SHUTDOWN,
        CFGZ_GROSS_OS_SHUTDOWN,
        CFGZ_START_RELAY,
        CFGZ_TEMP_CKT_OPEN,
        CFGZ_UF_SHUTDOWN,
        CFGZ_US_SHUTDOWN,
        CFGZ_FILT_MAINT,
        CFGZ_MODE_STOP,
        CFGZ_MODE_AUTO,
        CFGZ_MODE_MANUAL,
        CFGZ_BTS_BATTERY_HYBRID_MODE,
        CFGZ_PREHEAT,
        CFGZ_ECU_START,
        CFGZ_MIL,

    } CFGZ_ACT_TYPS_t;

    typedef enum {
        CFGZ_DENERGIZE_TO_ACTIVATE = 0,
        CFGZ_ENERGIZE_TO_ACTIVATE
    } CFGZ_ACT_ACTIVATION_TYP_t;

    /*Represents a row in the map between the actuator type of CFGZ & ACT*/
    typedef struct {
        CFGZ_ACT_TYPS_t           cfgzSensorType;
        ACTUATOR::ACTUATOR_TYPS_t actuatorType;
    } ACTUATOR_MAP_ROW_t;

    typedef enum {
        CFGZ_MODBUS_BAUD_1200 = 0,
        CFGZ_MODBUS_BAUD_2400,
        CFGZ_MODBUS_BAUD_4800,
        CFGZ_MODBUS_BAUD_9600,
        CFGZ_MODBUS_BAUD_19200,
        CFGZ_MODBUS_BAUD_38400,
        CFGZ_MODBUS_BAUD_57600,
        CFGZ_MODBUS_BAUD_115200
    } CFGZ_MODBUS_BAUD_t;

    typedef enum {
        CFGZ_MODBUS_PARITY_NONE = 0,
        CFGZ_MODBUS_PARITY_EVEN,
        CFGZ_MODBUS_PARITY_ODD
    } CFGZ_MODBUS_PARITY_t;

    typedef enum {
        CFGZ_1_PHASE_SYSTEM = 0,
        CFGZ_3_PHASE_SYSTEM,
        CFGZ_SPLIT_PHASE
    } CFGZ_AC_SYSTEM_TYP_t;

    typedef enum {
        CFGZ_GENSET_MODE_MANUAL = 0,
        CFGZ_GENSET_MODE_AUTO
    } CFGZ_GENSET_MODE_t;

    typedef enum {
        CFGZ_DISABLE = 0,
        CFGZ_ENABLE
    } CFGZ_ENABLE_DISABLE_t;

    typedef enum
    {
        CHECK_ERASE_SUCCESS,
        CHECK_CRC_WRITE_SUCCESS,
        CHECK_CFGZ_WRITE_SUCCESS,
        CHECK_CFGZ_INTEGRITY_STATUS
    }SM_DFLASH_STATUS;

    typedef enum
    {
        ON_ALT_OP_CABLE = 0,
        ON_LOAD_CABLE
    }CT_LOCATION_t;

    typedef enum
    {
        CFGZ_ALT_FREQUENCY,
        CFGZ_MAGNETIC_PICKUP,
        CFGZ_W_POINT_FREQ,
    }SPEED_SOURCE_t;

    typedef enum
    {
        CFGZ_HEATER_CONTROL,
        CFGZ_COOLER_CONTROL
    }CFGZ_CLNT_CTRL_OPT;


    typedef enum
    {
        CFGZ_LESS_THAN_THRESHOLD,
        CFGZ_GREATER_THAN_THRESHOLD
    }CFGZ_SENSOR_THRESHLD_TYPE;


    typedef enum
    {
        CFGZ_AS_EGOV,
        CFGZ_START_STOP
    }ACTUATOR_APPLICATION_t;
    typedef enum
    {
        CFGZ_FIXED_SPEED,
        CFGZ_LOAD_BASED_DROOP,
    }EGOV_SPEED_SELECTION_t;

    typedef enum
    {
        CFGZ_EARTH_LEAKAGE_CURR,
        CFGZ_FAN_CURRENT
    }CURRENT_MON_SELECTION_t;

    typedef enum
    {
        ENG_CONVENTIONAL,
        ENG_DEFAULT,   // Generic J1939
        ENG_SCANIA,
        ENG_VOLVO,
        ENG_IVECO,
        ENG_DEUTZ_EMR,
        ENG_MTU,
        ENG_KUBOTA,
        ENG_WECHAI,
        ENG_HATZ,
        ENG_PERKINS_ADAM4,
        ENG_YUCHAI_YCGCU,
        ENG_CUMMINS,
        ENG_CUMMINS_500,
        ENG_CUMMINS_558,
        ENG_CUMMINS_570,
        ENG_CUMMINS_850,
        ENG_CUMMINS_2150,
        ENG_CUMMINS_2250,
        ENG_DCEC_CUMMINS,
        ENG_YUCHAI_BOSCH,
        TOTAL_ENG_TYPES
    }ENGINE_TYPES_t;

    /**
     * Constructor, initiates this module.
     * @param : None
     * @return
     * None
     */
    CFGZ(HAL_Manager &hal, MODBUS &modbus, CFGC &cfgc);

    /**
     * This function to monitor the Erase and write status of D-Flash
     * Memory. It should be called in main loop.
     */
    void Update();

    /**
     * This functions is used to get the CFGZ parameter value of floating point
     * type.
     * @eparam   : Parameter enum value
     * @return  : Value of the Parameter
     */
    float    GetCFGZ_Param(FLOAT_PARAMS_t _f32Param);

    /**
     * This functions is used to get the CFGZ parameter value of uint16 type.
     * @eparam   : Parameter enum value
     * @return  : Value of the Parameter
     */
    uint16_t GetCFGZ_Param(UINT16_PARAMS_t _u16Param);

    /**
     * This functions is used to get the CFGZ parameter value of uint8 type.
     * @eparam   : Parameter enum value
     * @return   : Value of the Parameter
     */
    uint8_t  GetCFGZ_Param(UINT8_PARAMS_t _u8Param);

    /**
     * Following functions are used to get the parameter value from CFGZ.
     * @eparam   : Parameter enum value
     * @Param   : Address of the variable
     */
    void     GetCFGZ_Param(ARRAY_PARAMS_t _ArrParam, char* pu8ArrValue);

    uint16_t GetMisc_Param(MISC_PARAM_16_t ePram);
    uint8_t GetMisc_Param(MISC_PARAM_8_t ePram);

    /**
     * This function is used to request to write the parameter values in
     * D-Flash Memory .
     * @param  : CFGZ_PARAMS_t : Pointer of the data structure
     * @return
     * BSP_SUCCESS  - if the data was read successfully.
     * BSP_FAIL     - if the data was read fail.
     */
    BSP_STATUS_t WriteActiveProfile(CFGZ_PARAMS_t* _Allparams);

    /**
     * This is callback function for DFLASH write and erase operation.
     * @param:
     */
    void DFLASH_CB(DFLASH::FLASH_STATUS_t evt);

   /**
    * It will return the status stored in _CFGZ_Verification_status.
    * @return
    * BSP_SUCCESS
    * BSP_FAIL
    */
   BSP_STATUS_t GetCFGZ_VerificationStatus(void);

   void ApplyConfigChanges();

   bool IsC03Error();

   bool IsConfigWritten();

   void ReadFactoryProfile();

   SM_DFLASH_STATUS ReturnCFGZFlashState();

   void EnableDisableMainsParam();

   /**
   * This function is used to read the whole CFGZ parameters from D-Flash
   * memory and store the values in _All_Param structure.
   * @return
   * On Successful read it will return BSP_SUCCESS, otherwise BSP_FAIL,
   */
   BSP_STATUS_t ReadCFGZFromDflash(CFGZ_PARAMS_t* Param);

   BSP_STATUS_t ReadCFGZFromEEprom(CFGZ_PARAMS_t* Param);

   void ReadMiscParam(MISC_PARAM_t *stParam);

   void GetExpInterpolationTable(float *af32TableX, float *af32TableY, FLOAT_PARAMS_t eTableStart);

   bool IsDigOutputConfigured(UINT8_PARAMS_t eDigitalOutput);

   static void  EEpromWritCB(EEPROM::EVENTS_t eEvent);

   ENGINE_TYPES_t GetEngType(void);

   void GetEngSrNo(char EngSrNo[]);

   uint8_t GetArrLanguageIndex();
private:

    /**
    * Reference object of DFLASH class.
    */
    HAL_Manager &_hal;

    /*Holds reference to modbus*/
    MODBUS      &_modbus;

    CFGC        &_cfgc;
    /**
    * This structure will contain current copy of D-Flash CFGZ data.
    */
    CFGZ_PARAMS_t _All_Param;

    /**
    * This structure will contain CFGZ data to be written in DFLASH.
    */
    CFGZ_PARAMS_t _All_Param_Write;

    /**
    * This variable is used to indicate that D-Flash Erase and write operation
    *  is initiated.
    */
    bool _EraseAndWriteInitiated;

    /**
    * This will store the CFGZ verification status.
    */
    BSP_STATUS_t _CFGZ_Verification_status;

    static EEPROM::EVENTS_t _eEvent;

    /**
    * This function is used to check the CRC of all CFGZ area.
    * @return
    * BSP_SUCCESS  - if the CRC matches.
    * BSP_FAIL     - if the CRC not matches.
    */
    BSP_STATUS_t prvGetIntegrityStatus(void);


    /**
    * Reads configuration from the RAM copy of CFGZ and configures D_SENSE
    * @param: None
    * @return
    * None
    */
    void prvConfigureDSENSE();

    /**
    * Reads configuration from the RAM copy of CFGZ and configures A_SENSE
    * @param: None
    * @return
    * None
    */
    void prvConfigureASENSE();

    /**
    * The name(i.e enum value) of digital sensors defined in CFGZ is different
    * from those defined in D_SENSE. This function derives the sensor name as in
    * D_SENSE from the sensor name defined in this module.
    * @param u8CfgSensorType: The index in CFGZ_PARAMS_t.u8ArrParam where the sensor
    *                         type is present.
    * @param eInput         : The digital input for which the sensor type need
    *                         to be determined.
    * @return
    * the digital sensor type.
    */
    DigitalSensor::D_SENSOR_TYPS_t prvGetDigitalSensor(uint8_t u8CfgSensorType,
                                                         D_SENSE::INPUTS_t eInput);

    /**
    * The name(i.e enum value) of analog sensors defined in CFGZ is different
    * from those defined in A_SENSE. This function derives the sensor name as in
    * A_SENSE from the sensor name defined in this module.
    * @param u8CfgSensorType: The index in CFGZ_PARAMS_t.u8ArrParam where the sensor
    *                         type is present.
    * @param pMap           : The map between the sensor name in CFGZ and A_SENSE.
    * @param u8MapSize      : Number of rows in the map.
    * @return
    * the analog sensor type.
    */
    AnalogSensor::TYPS_t prGetAnalogSensor(uint8_t u8CfgSensorIdx,
                                        const ASENSOR_MAP_ROW_t *pMap, uint8_t u8MapSize);


    /**
    * Helper function to copy the interpolation table from CFGZ_PARAMS_t to
    * interpolation data structure defined in A_SENSE.
    * @param eTableStart   : Start index of interpolation table in CFGZ_PARAMS_t.
    * @param stDestination : The destination data structure
    * @return
    * None
    */
    void prvCpyInterpolationTable(FLOAT_PARAMS_t eTableStart,
                                    AnalogSensor::INTERPOLATE_INFO_t &stDestination);

    /**
    * Configures the actuators
    * @param: None
    * @return
    * None
    */
    void prvConfigureACT();

    /**
    * The name of actuators defined in CFGZ is different from those defined in
    * ACT. This function provides a translation from the CFGZ class name to the
    * ACTUATOR class name for an actuator.
    * @param u8CfgzActuatorTypeIdx: Index in CFGZ_PARAMS_t.u8ArrParam where the
    *                               CFGZ name for the actuator is present.
    * @return
    * The name of actuator in ACTUATOR class
    */
    ACTUATOR::ACTUATOR_TYPS_t prvGetACTType(uint8_t u8CfgzActuatorTypeIdx);

    /**
    * The function configures RS485 and modbus.
    * @param : None
    * @return
    * None
    */
    void prvConfigureMODBUS();

    /**
    * Translates CFGZ baud rate name to RS485 baud rate name.
    * @param : None
    * @return
    * RS485 baud rate
    */
    RS485::BAUD_t prvGetRS485Baud();

    /**
    * Translates CFGZ parity rate name to RS485 parity rate name.
    * @param : None
    * @return
    * RS485 parity rate
    */
    RS485::PARITY_t prvGetRS485Parity();

    /**
    * Configures AC_SENSE module
    * @param: None
    * @return
    * None
    */
    void prvConfigureACSense();

    /**
    * Loads default configuration into the RAM data structure holding CFGZ.
    * @param: None
    * @return
    * None
    */
    void prvLoadDefault();

    void prvSetHardcodedActDelay();
    void prvSetPassword();

    /*
     * The following structure is "firmware metadata".
     * This will be stored as the last few bytes in the firmware area.
     * i.e. firmware cannot actually extend all the way up to FW_END_ADDR;
     * it must end at least sizeof(FW_METADATA_t) before that.
     */
    typedef struct
    {
        uint16_t u16CFGZCrc;  // firmware CRC
        uint16_t u16Reserved1;  // should be 0xffff for now
        uint32_t u32HighestAddrUsedByCFGZ;
        RTC::TIME_t timeOfFlashing;
    } CFGZ_METADATA_t;

    CFGZ_METADATA_t strCFGZMetadata;
    bool _bDflashCallbackRcvd;
    bool _bConfigWritten;
    MISC_PARAM_t _stMiscParam;
};

#endif /* APL_CFGZ_CFGZ_H_ */
