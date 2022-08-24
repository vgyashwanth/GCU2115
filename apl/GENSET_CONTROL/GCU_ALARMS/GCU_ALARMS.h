/**
 * @file        [GCU_ALARMS]
 * @brief
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        18th June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#ifndef _GCU_ALARMS_H_
#define _GCU_ALARMS_H_

#include "stdint.h"
#include "HAL_Manager.h"
#include "CFGZ.h"
#include "productSelection.h"

class GCU_ALARMS{
public:

    typedef enum{
        ID_NONE,
        ID_NOTIFICATION,
        ID_WARNING,
        ID_ELECTRICAL_TRIP,
        ID_SHUTDOWN,
        ID_ACTION_LAST
    }ALARM_ACTION_t;

    typedef enum
    {
       ID_NOTIFICATION1,
       ID_WARNING1,
       ID_ELECTRICAL_TRIP1,
       ID_SHUTDOWN1,
       ID_ACTION_LAST1
   }ALARM_ACTION1_t;

   typedef enum
   {
       ID_NONE2,
       ID_NOTIFICATION2,
       ID_WARNING2,
       ID_ELECTRICAL_TRIP2,
       ID_SHUTDOWN2,
       ID_ACTION_LAST2
   }ALARM_ACTION2_t;


    typedef enum{
        FLOAT_TYPE,
        TWO_BYTE_INT,
        ONE_BYTE_INT
    }THRESHOLD_TYPE_t;

    typedef union{
        float f32Value;
        uint16_t u16Value;
        uint8_t u8Value;
    }PARAM_VALUE_t;

    typedef union{
        float f32Value;
        uint16_t u16Value;
        int16_t s16Value;
        uint8_t u8Value;
    }PARAM_THRESHOLD_t;

    typedef struct{
        uint16_t u16Counter;
        uint16_t u16CounterMax;
        PARAM_THRESHOLD_t Threshold;
        uint8_t *LocalEnable;
        uint8_t u8LoggingID;
        PARAM_VALUE_t *pValue;
        THRESHOLD_TYPE_t ThreshDataType;
        bool bEnableMonitoring;
        bool bMonitoringPolarity;
        bool bEnableNotification;
        bool bEnableWarning;
        bool bEnableShutdown;
        bool bEnableElectricTrip;
        bool bResultInstant;
        bool bResultLatched;
        bool bNotificationLatched;
        bool bWarningLatched;
        bool bShutdownLatched;
        bool bElectricTripLatched;
        bool bAlarmActive;
    }ALARM_MONITORING_t;

    typedef enum {
        LOW_OIL_PRESS_SHUTDOWN,
        LOW_OIL_PRESS_WARNING,
        LOW_FUEL_LEVEL_SHUTDOWN,
        LOW_FUEL_LEVEL_WARNING,
        HIGH_ENG_TEMP_SHUTDOWN,
        HIGH_ENG_TEMP_WARNING,
        OVERSPEED,
        GROSS_OVERSPEED,
        UNDERSPEED,
        OVERFREQ_SHUTDOWN,
        UNDERFREQ_SHUTDOWN,
        GEN_R_OV_SHUTDOWN,
        GEN_R_UV_SHUTDOWN,
        GEN_Y_OV_SHUTDOWN,
        GEN_Y_UV_SHUTDOWN,
        GEN_B_OV_SHUTDOWN,
        GEN_B_UV_SHUTDOWN,
        CHARG_ALT_UV,
        CHARG_ALT_FAIL,
        VBAT_OV,
        VBAT_UV,
        VBTS_UV,
        OVERCURRENT,
        FILT_MAINTENANCE,
        ID_FILT_MAINTENANCE_BY_DATE,
        OVERLOAD,
        FUEL_THEFT,
        GEN_PHASE_ROTATION,

        MPU_LOSS,
        LOP_CURR_OPEN_CKT,
        OPEN_ENG_TEMP_CKT,
        GEN_R_OV_WARNING,
        GEN_R_UV_WARNING,
        GEN_Y_OV_WARNING,
        GEN_Y_UV_WARNING,
        GEN_B_OV_WARNING,
        GEN_B_UV_WARNING,
        OVERFREQ_WARNING,
        UNDERFREQ_WARNING,
        LOP_RES_SENS_OPEN_CKT,
        LOP_CURR_SENS_STB,
        OPEN_ANLG_SENS_S1_CKT,
        OPEN_ANLG_SENS_S2_CKT,
        OPEN_ANLG_SENS_S3_CKT,
        OPEN_ANLG_SENS_S4_CKT,
        STB_ANLG_SENS_S3_CKT,
        STB_ANLG_SENS_S4_CKT,
        ANLG_SENS_S1_MON_SHUTDOWN,
        ANLG_SENS_S1_MON_WARNING,
        ANLG_SENS_S2_MON_SHUTDOWN,
        ANLG_SENS_S2_MON_WARNING,
        ANLG_SENS_S3_MON_SHUTDOWN,
        ANLG_SENS_S3_MON_WARNING,
        ANLG_SENS_S4_MON_SHUTDOWN,
        ANLG_SENS_S4_MON_WARNING,
        OIL_PRESS_DETECTED,
        EARTH_LEAK_CURR,
        SHELTER_TEMP_START_GEN,
        SHELTER_TEMP_STOP_GEN,
        SHELTER_TEMP_OPEN_CKT,
//////////////Do not change below sequence
        MAINS_R_UNDERVOLT_TRIP,
        MAINS_Y_UNDERVOLT_TRIP,
        MAINS_B_UNDERVOLT_TRIP,
        MAINS_R_OVERVOLT_TRIP,
        MAINS_Y_OVERVOLT_TRIP,
        MAINS_B_OVERVOLT_TRIP,
        MAINS_R_UNDERFREQ_TRIP,
        MAINS_Y_UNDERFREQ_TRIP,
        MAINS_B_UNDERFREQ_TRIP,
        MAINS_R_OVERFREQ_TRIP,
        MAINS_Y_OVERFREQ_TRIP,
        MAINS_B_OVERFREQ_TRIP,


        MAINS_R_UNDERVOLT_RETURN,
        MAINS_Y_UNDERVOLT_RETURN,
        MAINS_B_UNDERVOLT_RETURN,
        MAINS_R_OVERVOLT_RETURN,
        MAINS_Y_OVERVOLT_RETURN,
        MAINS_B_OVERVOLT_RETURN,
        MAINS_R_UNDERFREQ_RETURN,
        MAINS_Y_UNDERFREQ_RETURN,
        MAINS_B_UNDERFREQ_RETURN,
        MAINS_R_OVERFREQ_RETURN,
        MAINS_Y_OVERFREQ_RETURN,
        MAINS_B_OVERFREQ_RETURN,
 //////////////Do not change above sequence
        MAINS_PHASE_ROTATION,

        OPEN_FUEL_CKT,
        LOADING_VOLT_UNHEALTHY,
        LOADING_FREQ_UNHEALTHY,
        ACTUATOR_FAIL_ABOVE_30,
        ACTUATOR_FAIL_BELOW_30,
        FAN_HIGH_CURR,
        FAN_LOW_CURR,

        DIGITAL_INPUT_A,
        DIGITAL_INPUT_B,
        DIGITAL_INPUT_C,
        DIGITAL_INPUT_D,
        DIGITAL_INPUT_E,
        DIG_IN_F,
        DIG_IN_G,
        DIG_IN_H,
        DIG_IN_I,
        DIG_IN_J,
        DIG_IN_K,
        DIG_IN_L,
        DIG_IN_M,
        DIG_IN_N,
        DIG_IN_O,
        DIG_IN_P,
        REMOTE_START_STOP,
        EMERGENCY_STOP,
        LOW_FUEL_LVL_SWITCH,
        LLOP_SWITCH,
        HWT_SWITCH,
        RWL_SWITCH,
        V_BELT_BROKEN_SWITCH,
        LLOP_SWITCH_AT_ENG_OFF,
        MAINS_CONTACTOR_LATCHED,
        GEN_CONTACTOR_LATCHED,
        BATT_CHG_FAIL,
        SMOKE_FIRE,
        PANEL_LOCK,
        EX_AUTO_PANEL_LOCK,
        GEN_LOAD_INHIBIT,
        MAINS_LOAD_INHIBIT,
        FAIL_TO_LATCH_GEN_CONT, /* fail to close */
        FAIL_TO_LATCH_MAINS_CONT, /* fail to close */
        FAIL_TO_OPEN_GEN_CONT,
        FAIL_TO_OPEN_MAIN_CONT,
        INVALID_DG_START,
        J1939_COM_FAIL,
        HIGH_OIL_PRESSHURE,
        FAIL_TO_STOP,
        FAIL_TO_START,
        LOAD_UNBALANCE,
        J1939_PROTECT_LAMP,
        J1939_AMBER_LAMP,
        J1939_RED_LAMP,
        J1939_MIL_LAMP,
        J1939_ECU_PREHEAT_FAIL,
        REGEN_SW_INHIBIT_SIGNAL,
        ASH_LOAD_MAINTAINANCE,
        AFT_ACTIVATION_TIMEOUT,
        LOW_LOAD_TRIP,
        LOW_LOAD_RETURN,

        ALARM_LIST_LAST
    }ALARM_LIST_t;

    typedef enum
    {
       NoAlarm_id,
       Low_Oil_Pressure_id,
       fuel_level_id,
       High_Water_Temperature_id ,
       Low_Water_Temperature_id,
       Radiator_Water_Level_id ,
       Over_Speed_id ,
       Gross_Over_Speed_id ,
       Under_Speed_id ,
       R_Over_Voltage_id ,
       R_Under_Voltage_id ,
       Y_Over_Voltage_id ,
       Y_Under_Voltage_id ,
       B_Over_Voltage_id ,
       B_Under_Voltage_id ,
       Over_Frequency_id ,
       Under_Frequency_id ,
       Emergency_Stop_id ,      //16
       Charge_Fail_id ,
       Battery_Over_Voltage_id ,
       Battery_Under_Voltage_id ,
       Over_Current_id ,
       Filter_maintenance_id ,
       Over_Load_id ,
       Auxilary_Input_A_id ,
       Auxilary_Input_B_id ,
       Auxilary_Input_C_id ,
       Auxilary_Input_D_id ,
       Auxilary_Input_E_id ,
       Auxilary_Input_F_id ,
       Auxilary_Input_G_id ,
       Auxilary_Input_H_id ,
       Auxilary_Input_I_id ,
       Auxilary_Input_J_id,
       Auxilary_Input_K_id,
       Auxilary_Input_L_id,
       Auxilary_Input_M_id,
       Auxilary_Input_N_id,
       Auxilary_Input_O_id,
       Auxilary_Input_P_id,
       Fail_To_Stop_id ,
       Fuel_Theft_id ,
       Load_Unbalance_id,
       MPU_Loss_id,
       Fail_To_Start_id ,
       Lop_Curr_Sens_Open_Ckt_id,
       Engine_Temperature_Ckt_Open_id,
       Gen_Phase_Rotation_id,
       Phase_EB_Rotation_id,
       Oil_Pressure_Ckt_Open_id,
       V_Belt_Broken_id,
       Aux_S1_Sens_Open_Ckt_id,
       Aux_S2_Sens_Open_Ckt_id,
       Aux_S3_Sens_Open_Ckt_id,
       Aux_S4_Sens_Open_Ckt_id,
       Aux_S1_id,
       Aux_S2_id,
       Aux_S3_id,
       Aux_S4_id,
       Oil_Press_Detected_id,
       High_Earth_Leak_Curr_id,
       Batt_Chg_Fail_id,
       Smoke_Fire_id,
       High_Shelter_temp_id,
       Shelt_Temp_Open_id,
       Fuel_Level_Ckt_Open_id,
       Panel_lock_id,
       Ex_Auto_Panel_lock_id,
       Gen_load_inhibit_id,
       Mains_Load_Inhibit_id,
       Fail_To_Close_Gen_Cont_id,
       Fail_To_Close_Mains_Cont_id,
       Fail_To_Open_Gen_Cont_id,
       Fail_To_Open_Mains_Cont_id,
       Loading_Volt_Unhealthy_id,
       Loading_Freq_Unhealthy_id,
       Actuator_Fail_id,
       Fan_High_Current_id,
       Fan_Low_Current_id,
       Invalid_gen_start_id,
       J1939_com_fail_id,
       J1939_Protect_Lamp_id,
       J1939_Amber_Lamp_id,
       J1939_Red_Lamp_id,
       J1939_Mil_Lamp_id,
       J1939_Preheat_fail_id,
       Ash_load_maintenance_id,
       Aft_Activation_id,
       LOP_CURR_SENS_STB_id,
       Aux_S3_STB_id,
       Aux_S4_STB_id,
       LOP_Curr_Sen_Over_Curr_id,
       Aux_S3_Ckt_Over_Current_id,
       Aux_S4_Ckt_Over_Current_id,
       Aux_S3_Ckt_Over_Voltage_id,
       Aux_S4_Ckt_Over_Voltage_id,
       Low_load_id,
       Config_Modified_id,
       Engine_Start_id,
       Engine_Stop_id,

       ID_ALL_ALARMS_LAST
    }ALARM_LOGGING_ID_t;

    typedef struct
    {
        uint32_t u32EngineHrs;
        uint16_t u16Lop;
        uint16_t u16Speed;
        uint16_t u16GensetTotlKW;
        uint16_t u16Year;
        uint8_t u8Second;
        uint8_t u8Minute;
        uint8_t u8Hour;
        uint8_t u8Day;
        uint8_t u8Month;
        uint8_t u8EventId;
        uint8_t u8EventType;
        uint8_t u8Dummy;
    }EVENT_LOG_t ;

    ALARM_MONITORING_t ArrAlarmMonitoring[ALARM_LIST_LAST];


    /**
     * Constructor of GCU_ALARMS.
     * @param : None
     * @return : None
     */
    GCU_ALARMS(HAL_Manager &hal, CFGZ &cfgz);

    /**
     * Update function to be called in super-loop. Runs the latching
     * mechanism of alarms.
     * @param bool for checking whether device is in configuration.
     * @return
     * None
     */
    void Update(bool bDeviceInConfigMode);

    /**
     * Returns if any notification alarm is present or not.
     * @param None
     * @return : returns true if any notification alarm present.
     */
    bool IsCommonNotification();

    /**
     * Returns if any warning alarm is present or not.
     * @param None
     * @return : returns true if any warning alarm present.
     */
    bool IsCommonWarning();

    /**
     * Returns if any electric trip alarm is present or not.
     * @param None
     * @return : returns true if any electric trip alarm present.
     */
    bool IsCommonElectricTrip();

    /**
     * Returns if any shutdown alarm is present or not.
     * @param None
     * @return : returns true if any shutdown alarm present.
     */
    bool IsCommonShutdown();

    /**
     * checks whether requested alarm is active or not.
     * @param AlarmID: enum of the alarm list whose alarm active condition
     * to be checked.
     * @return : returns true if requested alarm present.
     */
    bool IsAlarmActive(ALARM_LIST_t AlarmID);

    /**
     * checks whether requested alarms instataneous result.
     * @param AlarmID: enum of the alarm list whose alarm active condition
     * to be checked.
     * @return : returns true if requested alarm is currently active.
     */
    bool AlarmResultInstat(ALARM_LIST_t AlarmID);

    bool AlarmResultLatched(ALARM_LIST_t AlarmID);

    /**
     * checks whether requested alarm monitoring is enabled or not.
     * @param AlarmID: enum of the alarm list whose alarm monitoring condition
     * to be checked.
     * @return : returns true if requested alarm is monitored.
     */
    bool IsAlarmMonEnabled(ALARM_LIST_t AlarmID);

    /**
     * Clears all the alarms upon pressing acknowledge key or after coming
     * out of the configuration mode.
     * @param : None
     * @return : None
     */
    void ClearAllAlarms();

    /**
     * Clears all status of mains related alarms.
     * @param : None
     * @return : None
     */
    void ResetMainsMonParams();

    uint16_t GetMinGensetVoltage();

    /**
     * Checks that if any alarm is present or not.
     * @param None
     * @return : returns true if any alarm is present.
     */
    bool IsAlarmPresent();

    /**
     * Makes the fail to start variable true which is used to generate the fail
     * to start alarm.
     * @param : None
     * @return : None
     */
    void UpdateFailToStart();

    /**
     * Makes the fail to stop variable true which is used to generate the fail
     * to stop alarm.
     * @param : None
     * @return : None
     */
    void UpdateFailToStop();

    /**
     * Provides the status of fail to start alarm.
     * @param : None
     * @return : return true if engine fails to start. 
     */
    bool IsFailToStart();

    /**
     * Fill the array with the currently active alarms entry.
     * @param : None
     * @return : return the number of active alarms. 
     */
    void FillDisplayAlarmArray();

    /**
     * This API log the generated alarm in memory.
     * @param : u8EventID : Logging ID of the alarm.
     *          u8EventType : Alarm action.
     * @return : None. 
     */
    void LogEvent(uint8_t u8EventID, uint8_t u8EventType);

    float GetMinMainsFreq();

    float GetMinGenFreq();

    /**
     * This API configures all GCU alarms.
     * @param : u8AlarmIndex : Index of the alarm which is to be configured.
     * @return : None. 
     */
    void ConfigureGCUAlarms(uint8_t u8AlarmIndex);

    void DisableAFTTimeoutAfterActTimer();

    void AssignAlarmsForDisplay(uint8_t AlarmID);


    void InitGCUAlarms();

    uint8_t GetActiveAlarmCount();

    bool IsMainsSeqFail();

    void CheckMPULossAlarm();

    uint8_t GetAlarmId(uint8_t u8Val);

    static bool         _bEventNumberReadDone;
    static bool         _bRollOverReadDone;
    uint32_t GetCurrentEventNumber();
    bool GetEventRolloverBit();
    static bool bEventWrittenSuccessfully;
    A_SENSE::SENSOR_RET_t GetLOPSensorVal();
    void ActivateHighOilPressAlarmSens();
    void TurnOffSounderAlarm();
    float GetSpeedValue();
    bool IsCommonAlarm();
    A_SENSE::SENSOR_RET_t GetSelectedTempSensVal();
    float GetSelectedBatteryVtg();
    uint32_t GetSelectedEngRunMin();

    bool IsBTSBattHealthy();
    bool IsShelterTempHigh();

    bool IsMainsFeedbackAvailable();
    bool IsGenFeedbackAvailable();
private:
    #define FUEL_THEFT_WAKEUP_TIMER         (4U)
    typedef enum{
        LUBE_OIL_PRESSURE,
        FUEL_LEVEL,
        ENGINE_TEMPERATURE,
        ENGINE_SPEED,
        GENSET_FREQUENCY,
        GEN_R_PHASE_VOLTAGE,
        GEN_Y_PHASE_VOLTAGE,
        GEN_B_PHASE_VOLTAGE,

        MAINS_R_PHASE_VOLTAGE,
        MAINS_Y_PHASE_VOLTAGE,
        MAINS_B_PHASE_VOLTAGE,
        MAINS_R_PHASE_FREQ,
        MAINS_Y_PHASE_FREQ,
        MAINS_B_PHASE_FREQ,

        MAINS_PH_PH_MIN_VOLTAGE,
        CHARG_ALT_VOLTAGE,
        CHARG_ALT_ERROR,
        BATTERY_VOLTAGE,
        BTS_VOLTAGE,
        GEN_MAX_CURRENT,
        ENG_RUN_HOURS,
        TOTAL_KW_PERCENT,
        FUEL_THEFT_ALARM,
        GEN_PHASE_ROTATION_STATUS,
        MAINS_PHASE_ROTATION_STATUS,
        ENG_TEMP_OPEN_CKT,
        LOP_RES_OPEN_CKT,
        LOP_CURRENT_OPEN_CKT,
        DIG_INPUT_A,    //26
        DIG_INPUT_B,
        DIG_INPUT_C,
        DIG_INPUT_D,
        DIG_INPUT_E,
        DIG_INPUT_F,
        DIG_INPUT_G,
        DIG_INPUT_H,
        DIG_INPUT_I,
        DIG_INPUT_J,
        DIG_INPUT_K,
        DIG_INPUT_L,
        DIG_INPUT_M,
        DIG_INPUT_N,
        DIG_INPUT_O,
        DIG_INPUT_P,
        AUX_SENS_S1_OPEN_CKT,
        AUX_SENS_S2_OPEN_CKT,
        AUX_SENS_S3_OPEN_CKT,
        AUX_SENS_S4_OPEN_CKT,
        AUX_SENS_S3_STB,
        AUX_SENS_S4_STB,
        ANLG_SENS_S1_VAL,
        ANLG_SENS_S2_VAL,
        ANLG_SENS_S3_VAL,
        ANLG_SENS_S4_VAL,
        SHELTER_TEMP_VAL,
        SHELT_TEMP_OPEN_CKT,
        EARTH_LEAKAGE_CURR_VAL,
        REMOTE_SS_STATUS,
        FAIL_TO_STOP_STATUS,
        FAIL_TO_START_STATUS,
        EMERGENCY_STOP_STATUS,
        SIM_START_STATUS,
        SIM_STOP_STATUS,
        SIM_AUTO_STATUS,
        SIM_MAINS_STATUS,
        CLOSE_GEN_OPEN_MAINS_STATUS,
        CLOSE_MAINS_OPEN_GEN_STATUS,
        GEN_UNBALANCED_LOAD,
        LOW_FUEL_LVL_SWITCH_STATUS,
        LLOP_SWITCH_STATUS,
        HWT_SWITCH_STATUS,
        WATER_LEVEL_SWITCH_STATUS,
        V_BELT_BROKEN_SWITCH_STATUS,
        MPU_LOSS_STATUS,
        MAINT_DATE,
        PIN23_SENSOR_CURRENT_VAL,
        FUEL_OPEN_CKT_VAL,
        GEN_MIN_VOLTAGE,
        GEN_MIN_FREQ,
        HIGH_OIL_PRESSURE,
        MAINS_CONT_LATCHED_STATUS,
        GEN_CONT_LATCHED_STATUS,
        FAIL_TO_LATCH_GEN_CONT_STATUS,
        FAIL_TO_LATCH_MAINS_CONT_STATUS,
        BATT_CHG_FAIL_STATUS,
        SMOKE_FIRE_STATUS,
        PANEL_LOCK_STATUS,
        EX_AUTO_PANEL_LOCK_STATUS,
        GEN_LOAD_INHIBIT_STATUS,
        MAINS_LOAD_INHIBIT_STATUS,
        INVALID_GEN_START_STATUS,
        AFT_ACTIVATION_TIMEOUT_STATUS,
        LOP_CURR_STB,
        J1939_COM_FAIL_STATUS,
        J1939_PROTECT_LAMP_STATUS,
        J1939_AMBER_LAMP_STATUS,
        J1939_RED_LAMP_STATUS,
        J1939_MIL_LAMP_STATUS,
        J1939_PREHEAT_FAIL_STATUS,
        J1939_ASH_LOAD_STATUS,
        LOW_LOAD_STATUS,
        REGENERATION_SW_STATUS,
        ALARM_VALUE_LAST
    }ALARM_VALUE_t;

    #define FOUR_SEC                   (4U)
    #define NO_OF_50MSEC_TICKS_FOR_1SEC (20U)
    #define FUEL_SETTLING_DELAY_MSEC     (500U)
    #define IS_SENSOR_VAL_LESS_THAN_THRESH() (((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == ONE_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u8Value) <= ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u8Value) || \
                                             ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == TWO_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u16Value) < ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u16Value) ||   \
                                             ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == FLOAT_TYPE) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->f32Value) < ArrAlarmMonitoring[_u8AlarmIndex].Threshold.f32Value))

    #define IS_SENSOR_VAL_MORE_THAN_THRESH() (((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == ONE_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u8Value) > ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u8Value) || \
                                             ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == TWO_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u16Value) >= ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u16Value) ||   \
                                             ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == FLOAT_TYPE) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->f32Value) > ArrAlarmMonitoring[_u8AlarmIndex].Threshold.f32Value))
    #define EVENT_LOG_LOP_SENSOR_NA         (0xFFDC)
    #define EVENT_LOG_LOP_SENSOR_OC         (0xFFDD)

    /*Reference to the hal*/
    HAL_Manager   &_hal;
    /*Reference to the configuration*/
    CFGZ          &_cfgz;

    bool          _bCommonNotification;
    bool          _bCommonWarning;
    bool          _bCommonElectricTrip;
    bool          _bCommonShutdown;
    bool          _bCommonAlarm;
    bool          _bOPSounderAlarm;
    bool          _bFailToStart;
    bool          _bFailToStop;
    bool          _bOpAutoFuelTransfer;
    bool          _bCLNTTempCtrl;
    bool          _bBTSBattHealthy;
    bool          _bHighShelterTemp;
    bool          _bUpdateFuelTheftCalc;
    uint8_t       _u8UnderFreqAlarm;
    uint8_t       _u8OverFreqAlarm;
    uint8_t       _u8RPhaseOverVoltAlarm;
    uint8_t       _u8YPhaseOverVoltAlarm;
    uint8_t       _u8BPhaseOverVoltAlarm;
    uint8_t       _u8RPhaseUnderVoltAlarm;
    uint8_t       _u8YPhaseUnderVoltAlarm;
    uint8_t       _u8BPhaseUnderVoltAlarm;
    uint8_t       _u8LowFuelLevelAlarm;
    uint8_t       _u8LowOilPressAlarm;
    uint8_t       _u8HighOilPressDetectedAlarm;
    uint8_t       _u8AuxSensS1Alarm;
    uint8_t       _u8AuxSensS2Alarm;
    uint8_t       _u8AuxSensS3Alarm;
    uint8_t       _u8AuxSensS4Alarm;
    uint8_t       _u8ActuatorFailAlarm;
    uint8_t       _u8AFTActivationTimeout;
    uint8_t       _u8HighEngTempAlarm;
    uint8_t       _u8HighEngTempSwitch;
    uint8_t       _u8HighCanopyTempAlarm;
    uint8_t       _u8HighLubeOilTempAlarm;
    uint8_t       _u8AlarmIndex;
    uint8_t       _u8DummyZero;
    uint8_t       _u8DummyOne;
    uint8_t       _u8Dummy255;
    uint8_t       _u8MonGenContactor;
    uint8_t       _u8MonMainsContactor;
    uint8_t       _u8MonGenContactorOpen;
    uint8_t       _u8MonMainsContactorOpen;
    uint8_t       _u8CrankMon;
    uint8_t       _u8EngineOff;
    uint8_t       _u8GenAvailable;
    uint8_t       _u8EngineOn;
    uint8_t       _u8GenReady;
    uint8_t       _u8MonOn;
    uint8_t       _u8FuelRelayOn;
    uint8_t       _u8LopSensMon;
    uint8_t       _u8FuelSensMon;
    uint8_t       _u8AuxSensS1;
    uint8_t       _u8AuxSensS2;
    uint8_t       _u8AuxSensS3;
    uint8_t       _u8AuxSensS4;
    uint8_t       _u8LopHiOilPressMon;
    uint8_t       _u8TempSensMon;
    uint8_t       _u8LowIdleSpeedMon;
    uint8_t       _u8FuelTheftAlarm;
    uint8_t       _u8MPULossAlarm;
    uint8_t       _u8AlarmArrayIndex;
    uint8_t       _u8MaintAlarm;
    float         _f32FuelLevelOldValue;
    stTimer       _FuelSettlingTimer;
    stTimer       _AFTTimeoutAfterActTimer;
    stTimer       _SounderAlarmTimer;
    stTimer       _UpdateAlarmMonTimer;
    stTimer       _AlarmUpdate;
    stTimer       _FuelTheftOneHourTimer;
    stTimer       _FuelTheftWakeUpTimer;
    uint8_t       *_ArrAlarmStatus[ID_ALL_ALARMS_LAST];
    PARAM_VALUE_t _ArrAlarmValue[ALARM_VALUE_LAST];
    uint8_t       _ArrAlarmForDisplay[ID_ALL_ALARMS_LAST];
    static uint8_t _u8PossibleMPULossCounter;
    uint32_t     _u32EventNumber;
    uint32_t     _u32RolledOverByte ;
    EVENT_LOG_t  _stEventLog;

    typedef struct
    {
        uint32_t     u32EventNo;
        EVENT_LOG_t  stEventLog;
    }EVENT_LOG_Q_t;

    EVENT_LOG_Q_t _stLog;

    #define EVENT_LOG_Q_SIZE (10)

   /** Internal Q to hold received frames */
   static CircularQueue<EVENT_LOG_Q_t> _EventQueue;
   static EVENT_LOG_Q_t  _EventQArr[EVENT_LOG_Q_SIZE];

    void prvAssignAlarmLatchedAction(uint8_t u8Index);
    void prvLogAlarmEvent(uint8_t u8Index);
    void prvGensetRelatedAlarms();

    void prvUpdateGCUAlarmsValue();

    void prvUpdateMonParams(uint8_t u8AlarmIndex, uint8_t* Pu8LocalEnable,
            bool bMonitoringPolarity, uint8_t u8LoggingID, uint8_t u8Threshold, 
                uint16_t u16CounterMax);

    void prvUpdateMonParams(uint8_t u8AlarmIndex, uint8_t* Pu8LocalEnable,
            bool bMonitoringPolarity, uint8_t u8LoggingID, 
                uint16_t u16Threshold, uint16_t u16CounterMax);

    void prvUpdateMonParams(uint8_t u8AlarmIndex, uint8_t* Pu8LocalEnable,
            bool bMonitoringPolarity, uint8_t u8LoggingID, 
                float f32Threshold, uint16_t u16CounterMax);

    void prvSetAlarmAction(uint8_t u8AlarmIndex, uint8_t u8AlarmAction);
    void prvSetAlarmActionfor4to20mASens(uint8_t u8AlarmIndex, uint8_t u8AlarmAction);

    void prvSetAlarmActivation(uint8_t u8AlarmIndex, uint8_t u8AlarmActivation);

    void prvAssignInputSettings(uint8_t u8InputIndex, uint8_t u8InputSource, 
            uint8_t u8Activation, uint8_t u8ActivationDelay, 
                uint8_t u8AlarmAction, uint8_t u8LoggingID);

    float prvGetMaxMainsFreq();

    float prvGetMaxGensetCurent();

    float prvGetGenMaxKWPercent();

    float prvGetGenMinKWPercent();

    uint16_t prvGetMinMainsVoltage();

    uint16_t prvGetMinPhToPhMainsVoltage();

    uint16_t prvGetMaxMainsVoltage();

    uint8_t prvGetUnbalancedLoadVal();

    void prvUpdateOutputs();

    void prvUpdateAlarmStatus();

    void prvActDeactOutput(bool bOutputCondition, ACTUATOR::ACTUATOR_TYPS_t eACTType);

    bool prvIsNeedToCheckSensFltAlarm();

    void prvIsFuelTheftAlarm();

    void prvClearAllAction(uint8_t u8AlarmIndex);

    void prvMainsHighLowOutputs();

    void prvAutoFuelTransferFunction();

    void prvCoolantTempCtrlFunction();

    void prvActDeactCLNTTempCtrlOutput();

    void prvActDeactAFTOutput();

    void prvCheckTripAction(uint8_t u8ReturnIndex, uint8_t u8TripIndex, bool status);
};
#endif
