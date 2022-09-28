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
        int16_t s16Value; //Todo: If param Threshold has s16 values, then Param values also should have s16 and accordingly Threshold type adjusted to support it.
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
        LOW_FUEL_LEVEL_NOTIFICATION,
        HIGH_WATER_TEMP,
        OVERSPEED_L1,
        OVERSPEED_L2,
        UNDERSPEED,
        OVERFREQ_SHUTDOWN,
        UNDERFREQ_SHUTDOWN,
        DG_R_OV_SHUTDOWN,
        DG_R_UV_SHUTDOWN,
        DG_Y_OV_SHUTDOWN,
        DG_Y_UV_SHUTDOWN,
        DG_B_OV_SHUTDOWN,
        DG_B_UV_SHUTDOWN,
        MAINS_UNDERVOLT_TRIP,
        MAINS_OVERVOLT_TRIP,
        MAINS_UNDERVOLT_RETURN,
        MAINS_OVERVOLT_RETURN,
        MAINS_UNDERFREQ_TRIP,
        MAINS_OVERFREQ_TRIP,
        MAINS_UNDERFREQ_RETURN,
        MAINS_OVERFREQ_RETURN,
        CA_UV,
        CA_FAIL,
        VBAT_OV,
        VBAT_UV,
        VBTS_UV,
        OVERCURRENT,
        FILT_MAINTENANCE,
        OVERLOAD,
        FUEL_THEFT,
        DG_PHASE_ROTATION,
        EB_PHASE_ROTATION,
        MPU_LOSS,
        OPEN_LOP_CURR_SENS_CKT,
        OPEN_LOP_SENS_CKT,
        OPEN_ENG_TEMP_CKT,
        DG_R_OV_WARNING,
        DG_R_UV_WARNING,
        DG_Y_OV_WARNING,
        DG_Y_UV_WARNING,
        DG_B_OV_WARNING,
        DG_B_UV_WARNING,
        OVERFREQ_WARNING,
        UNDERFREQ_WARNING,
        LOP_CURR_SENS_OVER_CURR,
        OPEN_AN_SEN_S1_CKT,
        OPEN_AN_SEN_S2_CKT,
        AN_SEN_S2_MON,
        OIL_PRESS_MON,
        SHELTER_TEMP_START_DG,
        SHELTER_TEMP_STOP_DG,
        MAINS_LL_UNDERVOLT_TRIP,
        MAINS_LL_UNDERVOLT_RETURN,
        FAN_HIGH_CURR,
        FAN_LOW_CURR,
        LOP_SENS_SHORT_TO_BATTERY,

        DIG_IN_A,
        DIG_IN_B,
        DIG_IN_C,
        DIG_IN_D,
        DIG_IN_E,
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
        REMOTE_SS,
        FAIL_TO_STOP,
        FAIL_TO_START,
        ESTOP,
        LOAD_UNBALANCE,
        LFL_SWITCH,
        LLOP_SWITCH,
        HWT_SWITCH,
        RWL_SWITCH,
        FILT_MAINTENANCE_BY_DATE,
        V_BELT_BROKEN_SWITCH,
        LLOP_MON,
        HIGH_OIL_PRESS_DETECTED,
        MAINS_CONTACTOR_LATCHED,
        DG_CONTACTOR_LATCHED,
        BATT_CHG_FAIL,
        SMOKE_FIRE,
        MODE_SELECT,
        INVALID_DG_START,
        AMB_TEMP_SWITCH,
        ALARM_AMBER_LAMP,
        ALARM_RED_LAMP,
        ALARM_MIL_LAMP,
        ALARM_PROTECT_LAMP,
        ALARM_P0031,
        ALARM_P0032,
        ALARM_P0107,
        ALARM_P0108,
        ALARM_P0112,
        ALARM_P0113,
        ALARM_P0117,
        ALARM_P0118,
        ALARM_P0121,
        ALARM_P0122,
        ALARM_P0123,
        ALARM_P0131,
        ALARM_P0132,
        ALARM_P0133,
        ALARM_P0134,
        ALARM_P0217,
        ALARM_P0236,
        ALARM_P0237,
        ALARM_P0238,
        ALARM_P0335,
        ALARM_P0340,
        ALARM_P0522,
        ALARM_P0523,
        ALARM_P0524,
        ALARM_P0562,
        ALARM_P0563,
        ALARM_P0638,
        ALARM_P0657,
        ALARM_P0658,
        ALARM_P0659,
        ALARM_P1518,
        ALARM_P1630,
        ALARM_P1631,
        ALARM_P1632,
        ALARM_P1633,
        ALARM_P2100,
        ALARM_P2102,
        ALARM_P2103,
        ALARM_P2111,
        ALARM_P2112,
        ALARM_P2177,
        ALARM_P2178,
        ALARM_P2191,
        ALARM_P2192,
        ALARM_P2231,
        ALARM_P2300,
        ALARM_P2301,
        ALARM_P2302,
        ALARM_P2303,
        ALARM_P2304,
        ALARM_P2305,
        ALARM_P2306,
        ALARM_P2307,
        ALARM_P2308,
        ALARM_P2309,
        ALARM_P2310,
        ALARM_P2311,
        ALARM_P2312,
        ALARM_P2313,
        ALARM_P2314,
        ALARM_P2315,
        ALARM_P2316,
        ALARM_P2317,
        ALARM_LIST_LAST
    }ALARM_LIST_t;

    typedef enum
    {
        NoAlarm_id,
        Low_Oil_Pressure_id,
        fuel_level_id,
        High_Water_Temperature_id ,
        Radiator_Water_Level_id ,
        Over_Speed_l1_id ,
        Over_Speed_l2_id ,
        Under_Speed_id ,
        R_Over_Voltage_id ,
        R_Under_Voltage_id ,
        Y_Over_Voltage_id ,
        Y_Under_Voltage_id ,
        B_Over_Voltage_id ,
        B_Under_Voltage_id ,
        Over_Frequency_id ,
        Under_Frequency_id ,
        Emergency_Stop_id ,
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
        Lop_Sen_Ckt_Open_id,
        Engine_Temperature_Ckt_Open_id,
        Phase_DG_Rotation_id,
        Phase_EB_Rotation_id,
        V_Belt_Broken_id,
        LOP_Curr_Sen_Over_Curr_id,
        Aux_S1_Ckt_Open_id,
        Aux_S2_Ckt_Open_id,
        Aux_S2_id,
        High_Oil_Press_Detected_id,
        Batt_Chg_Fail_id,
        Smoke_Fire_id,
        Engine_Start_id,
        Engine_Stop_id,
        Config_Modified_By_Master_id  ,
        Config_Modified_By_User_id ,
        High_Shelter_temp_id,
        Fan_High_Current_id,
        Fan_Low_Current_id,
        Invalid_gen_start_id,
        Lop_Short_To_Battery_id,
        Alarm_Amber_Lamp_id,
        Alarm_Red_Lamp_id,
        Alarm_Mil_Lamp_id,
        Alarm_Protect_Lamp_id,
        Alarm_P0031_id,
        Alarm_P0032_id,
        Alarm_P0107_id,
        Alarm_P0108_id,
        Alarm_P0112_id,
        Alarm_P0113_id,
        Alarm_P0117_id,
        Alarm_P0118_id,
        Alarm_P0121_id,
        Alarm_P0122_id,
        Alarm_P0123_id,
        Alarm_P0131_id,
        Alarm_P0132_id,
        Alarm_P0133_id,
        Alarm_P0134_id,
        Alarm_P0217_id,
        Alarm_P0236_id,
        Alarm_P0237_id,
        Alarm_P0238_id,
        Alarm_P0335_id,
        Alarm_P0340_id,
        Alarm_P0522_id,
        Alarm_P0523_id,
        Alarm_P0524_id,
        Alarm_P0562_id,
        Alarm_P0563_id,
        Alarm_P0638_id,
        Alarm_P0657_id,
        Alarm_P0658_id,
        Alarm_P0659_id,
        Alarm_P1518_id,
        Alarm_P1630_id,
        Alarm_P1631_id,
        Alarm_P1632_id,
        Alarm_P1633_id,
        Alarm_P2100_id,
        Alarm_P2102_id,
        Alarm_P2103_id,
        Alarm_P2111_id,
        Alarm_P2112_id,
        Alarm_P2177_id,
        Alarm_P2178_id,
        Alarm_P2191_id,
        Alarm_P2192_id,
        Alarm_P2231_id,
        Alarm_P2300_id,
        Alarm_P2301_id,
        Alarm_P2302_id,
        Alarm_P2303_id,
        Alarm_P2304_id,
        Alarm_P2305_id,
        Alarm_P2306_id,
        Alarm_P2307_id,
        Alarm_P2308_id,
        Alarm_P2309_id,
        Alarm_P2310_id,
        Alarm_P2311_id,
        Alarm_P2312_id,
        Alarm_P2313_id,
        Alarm_P2314_id,
        Alarm_P2315_id,
        Alarm_P2316_id,
        Alarm_P2317_id,
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

    void ResetMainsTrip(uint8_t u8Index);
    void ResetMainsReturn(uint8_t u8Index);

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

    void AssignAlarmsForDisplay(uint8_t AlarmID);

    void InitGCUAlarms();

    uint8_t GetActiveAlarmCount();

    bool IsMainsSeqFail();

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
    bool IsShelterTempLow();
    bool IsSounderAlarmOn();

    bool IsLowFuelLevelAlarmActive();
    bool IsHighEngTempAlarmActive();
    bool IsLowOilPresAlarmActive();
    bool IsRPhaseOverVoltAlarmActive();
    bool IsYPhaseOverVoltAlarmActive();
    bool IsBPhaseOverVoltAlarmActive();
    bool IsRPhaseUnderVoltAlarmActive();
    bool IsYPhaseUnderVoltAlarmActive();
    bool IsBPhaseUnderVoltAlarmActive();

/* Public functions accessible to objects, return true if event monitoring enabled and event occured.*/
    bool RemoteStartReceived();
    bool RemoteStopReceived();

    void UpdateFuelTheftCalculation();
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
        MAINS_MIN_PHASE_VOLTAGE,
        MAINS_MAX_PHASE_VOLTAGE,
        MAINS_MIN_PHASE_FREQ,
        MAINS_MAX_PHASE_FREQ,
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
        MPU_LOSS_STATUS,
        ENG_TEMP_OPEN_CKT,
        LOP_RES_OPEN_CKT,
        LOP_CURRENT_OPEN_CKT,
        DIG_INPUT_A,
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
        LOW_FUEL_LVL_SWITCH_STATUS,
        LLOP_SWITCH_STATUS,
        HWT_SWITCH_STATUS,
        WATER_LEVEL_SWITCH_STATUS,
        EMERGENCY_STOP_STATUS,
        REMOTE_SS_STATUS,
        SIM_START_STATUS,
        SIM_STOP_STATUS,
        SIM_AUTO_STATUS,
        CLOSE_GEN_OPEN_MAINS_STATUS,
        CLOSE_MAINS_OPEN_GEN_STATUS,
        SIM_MAINS_STATUS,
        V_BELT_BROKEN_SWITCH_STATUS,
        MAINS_CONT_LATCHED_STATUS,
        GEN_CONT_LATCHED_STATUS,
        BATT_CHG_FAIL_STATUS,
        SMOKE_FIRE_STATUS,
        MODE_SELECT_STATUS,
        AMB_TEMP_SELECT_STATUS,
        FAIL_TO_STOP_STATUS,
        FAIL_TO_START_STATUS,
        GEN_UNBALANCED_LOAD,
        MAINT_DATE,
        HIGH_OIL_PRESSURE,
        INVALID_GEN_START_STATUS,
        AUX_SENS_S1_OPEN_CKT,
        AUX_SENS_S2_OPEN_CKT,
        ANLG_SENS_S2_VAL,
        SHELTER_TEMP_VAL,
        SHELT_TEMP_OPEN_CKT,
        EARTH_LEAKAGE_CURR_VAL,
        PIN23_SENSOR_CURRENT_VAL,
        FUEL_OPEN_CKT_VAL,
        LOP_CURR_STB,
        J1939_AMBER_LAMP_STATUS,
        J1939_RED_LAMP_STATUS,
        J1939_MIL_LAMP_STATUS,
        J1939_PROTECT_LAMP_STATUS,
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

    bool          _bCommonAlarm;
    bool          _bCommonNotification;
    bool          _bCommonWarning;
    bool          _bCommonElectricTrip;
    bool          _bCommonShutdown;
    bool          _bOPSounderAlarm;
    bool          _bFailToStart;
    bool          _bFailToStop;
    bool          _bCLNTTempCtrl;
    bool          _bBTSBattHealthy;
    bool          _bHighShelterTemp;
    bool          _bLowShelterTemp;
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
    stTimer       _SounderAlarmTimer;
    stTimer       _UpdateAlarmMonTimer;
    stTimer       _AlarmUpdate;
    stTimer       _FuelTheftOneHourTimer;
    stTimer       _FuelTheftWakeUpTimer;
    uint8_t       *_ArrAlarmStatus[ID_ALL_ALARMS_LAST];
    PARAM_VALUE_t _ArrAlarmValue[ALARM_VALUE_LAST];
    uint8_t       _ArrAlarmForDisplay[ID_ALL_ALARMS_LAST];
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

    void prvSetAlarmAction_NoWESN(uint8_t u8AlarmIndex, uint8_t u8AlarmAction);
    void prvSetAlarmAction_NoWS(uint8_t u8AlarmIndex, uint8_t u8AlarmAction);
    void prvSetAlarmAction_WS(uint8_t u8AlarmIndex, uint8_t u8AlarmAction);
    void prvSetAlarmAction_NW(uint8_t u8AlarmIndex, uint8_t u8AlarmAction);
    void prvSetAlarmAction_NoN(uint8_t u8AlarmIndex, uint8_t u8AlarmAction);

    void prvSetAlarmActivation(uint8_t u8AlarmIndex, uint8_t u8AlarmActivation);

    void prvAssignInputSettings(uint8_t u8InputIndex, uint8_t u8InputSource,
            uint8_t u8Activation, uint8_t u8ActivationDelay,
                uint8_t u8AlarmAction, uint8_t u8LoggingID);

    float prvGetMinMainsFreq();
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

    void prvCoolantTempCtrlFunction();

    void prvActDeactCLNTTempCtrlOutput();

    void prvCheckTripAction(uint8_t u8ReturnIndex, uint8_t u8TripIndex, bool status);

};
#endif
