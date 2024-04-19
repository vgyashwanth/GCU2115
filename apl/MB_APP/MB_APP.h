/**
 * @file        [MB_APP]
 * @brief       This module defines the modbus registers. It internally populates
 *              the modbus registers and also provides API's to read/write these
 *              registers. The module allows to define registers whose addresses
 *              aren't continuous, ex: 10 registers in address range [0-9]
 *              and another 10 registers in address range [20-29].
 *              In case we want to define groups of registers whose addresses are
 *              non-continuous, the following things need to be done:
 *              1. Create an entry for each register in MODBUS_READ_REGISTERS_t if
 *              it is a read only register. Create an entry in  MODBUS_WRITE_REGISTERS_t
 *              if it is a write only register
 *              The enum entry corresponding the first register of an address group
 *              shall be set with it's address. The successive enums will automatically
 *              assigned the right addresses, because the compilers assigns successive
 *              values to the following enum entries.
 *              2. Change MODBUS_ADDRESS_GROUPS value to the number of register
 *              groups needed.
 *              3. Create a uint16_t buffer to hold the new group of registers.
 *              This means the size of this buffer shall be equal to the number of
 *              registers in this group.
 *              ex: _au16Grp1Registers & _au16Grp1Registers is already done for
 *              group 1 and group 2.
 *              4. An entry for the new group needs to be created in _aAddressGrp.
 *              This is done in the parametrized constructor.
 * @version     [Version number]
 * @author      Sudeep Chandrasekaran
 * @date        13th June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#ifndef _MB_APP_H_
#define _MB_APP_H_


#include "stdint.h"
#include "HAL_Manager.h"
#include "RS485.h"
#include "MODBUS.h"
#include "AC_SENSE.h"
#include "GCU_ALARMS.h"
#include "ENGINE_MONITORING.h"
#include "AUTO_MODE.h"


/*Defines the number of discontinuous address groups*/

#if (TEST_AUTOMATION == YES)
/*Defines the number of discontinuous address groups*/
#define MODBUS_ADDRESS_GROUPS   (4U)
#define MODBUS_GRP3_REG_CNT     (MB_AUTOMATION_READ_REG_LAST - MB_AUX_S1)
#define MODBUS_GRP4_REG_CNT     (MB_AUTOMATION_WRITE_REG_LAST - MB_AUTOMATION_WRITE_COMMAND)
#else
/*Defines the number of discontinuous address groups*/
#define MODBUS_ADDRESS_GROUPS   (2U)
#endif
/*Number of entries in the first address group*/
#define MODBUS_GRP1_REG_CNT     (MB_READ_REG_LAST -DIG_ALARM_1_REG)
/*Number of entries in the second address group*/
#define MODBUS_GRP2_REG_CNT     (2U)
#define MODBUS_PROTOCOL_VERSION (1U)


#define FOURTH_NIBBLE 12
#define THIRD_NIBBLE 8
#define SECOND_NIBBLE 4
#define FIRST_NIBBLE 0

class MB_APP: public MODBUS {
public:
    /* Contains the list of registers, the enum values correspond to the register
       address.
     */
#if (TEST_AUTOMATION == 1)
    typedef enum {
        MB_AUX_S1=400,
        MB_AUX_S2,
        MB_GEN_L1_L2_VOLATGE,
        MB_GEN_L2_L3_VOLATGE,
        MB_GEN_L3_L1_VOLATGE,
        MB_MAINS_L1_L2_VOLTAGE,
        MB_MAINS_L2_L3_VOLTAGE,
        MB_MAINS_L3_L1_VOLTAGE,
        MB_MAINS_R_FREQUENCY,
        MB_MAINS_Y_FREQUENCY,
        MB_MAINS_B_FREQUENCY,
        MB_NO_OF_STARTS,
        MB_NO_OF_TRIPS,
        MB_ALARM_1,
        MB_ALARM_2,
        MB_ALARM_3,
        MB_ALARM_4,
        MB_ALARM_5,
        MB_ALARM_6,
        MB_ALARM_7,
        MB_ALARM_8,
        MB_ALARM_9,
        MB_ALARM_10,
        MB_ALARM_11,
        MB_ALARM_12,
        MB_ALARM_13,
        MB_ALARM_14,
        MB_ALARM_15,
        MB_ALARM_16,
        MB_ALARM_17,
        MB_DIG_IP_STATUS,
        MB_DIG_OP_STATUS,
        MB_GEN_STATUS,
        MB_TIME_STAMP0,
        MB_TIME_STAMP1,
        MB_TIME_STAMP2,
        MB_TIME_STAMP3,
        PASSWORD_ACCESS_STATUS, //
        MAIN_MENU_INDEX, //
        SUB_MENU_INDEX, //
        ITEM_INDEX, //
        MB_SPN_VALUE_1,
        MB_SPN_VALUE_2,
        MB_SPN_VALUE_3,
        MB_SPN_VALUE_4,
        MB_SPN_STATUS,     
        MB_AUTOMATION_READ_REG_LAST
    }MODBUS_FOR_AUTOMATION_READ;
#endif
        typedef enum {
        DIG_ALARM_1_REG = 16384,
        DIG_ALARM_2_REG,
        SOLID_STATE_OP_REG,
        MB_MAINS_L1_N_VOLTAGE,
        MB_MAINS_L2_N_VOLTAGE,
        MB_MAINS_L3_N_VOLTAGE,
        MB_GEN_L1_N_VOLATGE,
        MB_GEN_L2_N_VOLATGE,
        MB_GEN_L3_N_VOLATGE,
        MB_LOAD_L1_CURRENT,
        MB_LOAD_L2_CURRENT,
        MB_LOAD_L3_CURRENT,
        MB_MAINS_CUMM_ACTIVE_ENERGY_1,
        MB_MAINS_CUMM_ACTIVE_ENERGY_2,
        MB_GEN_CUMM_ACTIVE_ENERGY_1,
        MB_GEN_CUMM_ACTIVE_ENERGY_2,
        MB_ENG_RUN_HOURS,
        MB_ENG_RUN_MINUTES,
        MB_MAINS_RUN_HOURS,
        MB_MAINS_RUN_MINUTES,
        MB_BATTERY_VOLTAGE,
        MB_BTS_CHARGE_STATE,
        MB_FUEL_IN_LIT,
        MB_MAINS_L1_POWER,
      	MB_MAINS_L2_POWER,
      	MB_MAINS_L3_POWER,
        MB_GEN_L1_POWER,
        MB_GEN_L2_POWER,
        MB_GEN_L3_POWER,
        MB_BTS_RUN_HRS,
        MB_BTS_RUN_MIN,
        MB_TMP_RUN_HOURS,
        MB_TMP_RUN_MINUTES,
        MB_GEN_TMP_ACTIVE_ENERGY_1,
        MB_GEN_TMP_ACTIVE_ENERGY_2,
        /*
         * Addition of 0x4023 register in Modbus.
         * The reserved space is used for new addition as per the requirement from customer
         */
        MB_MCCB_FEEDBACK_ALARM,
        MB_EGR_FAULT_NOTIFICATION_INFO = 0X4024,
        MB_EGR_FAULT_NOTIFICATION_TIME,
        MB_EGR_HEAL_TIME,
        MB_EGR_ALARMS_INFO,

/* Expected start address for the first parameter over modbus is 0x4000.
This start address is subtracted while parsing the modbus packet so the
enum DIG_ALARM_1_REG starts from 0. After which all the parameters are sequentially added
but as per the requirement new parameters need be added from 0x4030 address over MODBUS.
Since we are supposed to add all the parameters from 0x4030 we have added offset of 14 in
COOLANT_TEMP_REGi.e difference between GEN_TMP_KWH_REG_2 and COOLANT_TEMP_REG .
NOTE :- If you are adding any enum between GEN_TMP_KWH_REG_2 and COOLANT_TEMP_REG register,
verify that COOLANT_TEMP_REG register is at 0x4030. So to do that you are supposed to edit
offset 14.
*/
        MB_COOLANT_TEMPERATURE = MB_GEN_TMP_ACTIVE_ENERGY_2 + 14,
        MB_OIL_PRESSURE,
        MB_ENG_SPEED,
        MB_GEN_R_FREQUENCY,
        MB_GEN_Y_FREQUENCY,
        MB_GEN_B_FREQUENCY,
        MB_GEN_L1_APPARENT_POWER,
        MB_GEN_L2_APPARENT_POWER,
        MB_GEN_L3_APPARENT_POWER,
        MB_GEN_L1_REACTIVE_POWER,
        MB_GEN_L2_REACTIVE_POWER,
        MB_GEN_L3_REACTIVE_POWER,
        MB_GEN_CUMM_APPARENT_ENERGY_1,
        MB_GEN_CUMM_APPARENT_ENERGY_2,
        MB_GEN_CUMM_REACTIVE_ENERGY_1,
        MB_GEN_CUMM_REACTIVE_ENERGY_2,
        MB_GEN_L1_PF,
        MB_GEN_L2_PF,
        MB_GEN_L3_PF,
        MB_GEN_AVERAGE_PF,
        MD_FUEL_PERCENTAGE,
       // MB_FUEL_RATE_PGN_65266,
        MB_SPN1  = 0x4045,
        MB_FMI1  = MB_SPN1 + 2,
        MB_SPN2  ,
        MB_FMI2  = MB_SPN2 + 2,
        MB_SPN3  ,
        MB_FMI3  = MB_SPN3 + 2,
        MB_SPN4  ,
        MB_FMI4  = MB_SPN4 + 2,
        MB_SPN5  ,
        MB_FMI5  = MB_SPN5 + 2,
        MB_SPN6  ,
        MB_FMI6  = MB_SPN6 + 2,
        MB_SPN7  ,
        MB_FMI7  = MB_SPN7 + 2,
        MB_SPN8  ,
        MB_FMI8  = MB_SPN8 + 2,
        MB_SPN9  ,
        MB_FMI9  = MB_SPN9 + 2,
        MB_SPN10 ,
        MB_FMI10  = MB_SPN10 + 2,
        MB_SPN11 ,
        MB_FMI11  = MB_SPN11 + 2,
        MB_SPN12 ,
        MB_FMI12  = MB_SPN12 + 2,
        MB_SPN13 ,
        MB_FMI13  = MB_SPN13 + 2,
        MB_SPN14 ,
        MB_FMI14  = MB_SPN14 + 2,
        MB_SPN15 ,
        MB_FMI15  = MB_SPN15 + 2,


        MB_READ_REG_LAST
    } MODBUS_READ_REGISTERS_t;

    typedef enum name {
        MB_COMMAND = 0,
        MB_MODE_REG
//        MB_DATE_TIME1,
//        MB_DATE_TIME2,
//        MB_DATE_TIME3,
//        MB_DATE_TIME4,
//        MB_DATE_TIME5,
    }MODBUS_WRITE_REGISTERS_t;

#if (TEST_AUTOMATION == YES)
    typedef enum {
        MB_AUTOMATION_WRITE_COMMAND = 350,
        MB_AUTOMATION_ENGINE_RUN_HOURS_1,                    /* RUN_HOUR_1 will hold hour value */
        MB_AUTOMATION_ENGINE_RUN_HOURS_2,                    /* RUN_HOUR_2 will hold minutes value */
        MB_AUTOMATION_MAINS_RUN_HOURS_1,
        MB_AUTOMATION_MAINS_RUN_HOURS_2,
        MB_AUTOMATION_BTS_RUN_HOUR_1,
        MB_AUTOMATION_BTS_RUN_HOUR_2,
        MB_AUTOMATION_DATE_TIME1,
        MB_AUTOMATION_DATE_TIME2,
        MB_AUTOMATION_DATE_TIME3,
        MB_AUTOMATION_DATE_TIME4,
        MB_AUTOMATION_GEN_ACTIVE_ENERGY_1,
        MB_AUTOMATION_GEN_ACTIVE_ENERGY_2,
        MB_AUTOMATION_GEN_APPARENT_ENERGY_1,
        MB_AUTOMATION_GEN_APPARENT_ENERGY_2,
        MB_AUTOMATION_GEN_REACTIVE_ENERGY_1,
        MB_AUTOMATION_GEN_REACTIVE_ENERGY_2,
        MB_AUTOMATION_NUMBER_OF_STARTS,
        MB_AUTOMATION_NUMBER_OF_TRIPS,
        MB_PGN_LOW_WORD,
        MB_PGN_HIGH_WORD,
        MB_SPN_BIT_POSITION,
        MB_AUTOMATION_WRITE_REG_LAST
    }MODBUS_FOR_AUTOMATION_WRITE;

    typedef struct{
            uint16_t EngineRunTime:1;
            uint16_t MainsRunTime:1;
            uint16_t BTSRunTime:1;
            uint16_t RTC:1;
            uint16_t ActiveEnergy:1;
            uint16_t ApparentEnergy:1;
            uint16_t ReactiveEnergy:1;
            uint16_t NumberOfStarts:1;
            uint16_t NumberOfTrips:1;
            uint16_t Reserved1:1;
            uint16_t Reserved2:1;
            uint16_t Reserved3:1;
            uint16_t Reserved4:1;
            uint16_t Reserved5:1;
            uint16_t Reserved6:1;
            uint16_t Reserved7:1;
    }stMBAutomationSetCommand;

    typedef union
    {
        stMBAutomationSetCommand stMBWriteCommandForAutomation;
        uint16_t u16CommandSet;
    }MBWriteCommand;
#endif

    typedef struct
    {
        bool    bStartKey;
        bool    bStopKey ;
        bool    bAutoKey ;
        bool    bAckKey  ;
        bool    bKeyEvent ;
    }KEY_MB_CAN_EVENT_t;

    typedef struct
    {
        uint16_t u16Mbcount;
        uint16_t u16Dummy[1];
        uint32_t u32CRC;
    }MISC_EEPROM_t ;

    /**
     * Constructs this class. This internally initiates the modbus module.
     * @param hal - Reference to hardware abstraction layer.
     * @return
     * None
     */
    MB_APP(HAL_Manager &hal,  CFGZ &cfgz, GCU_ALARMS &gcuAlarm,
            ENGINE_MONITORING &engineMonitoring, AUTO_MODE &Automode);

    /**
     * Fetches the value of a modbus register with write access(address group 2).
     * @param eRegister - The register whose value needs to be fetched.
     * @return
     * Value of modbus register
     */
    uint16_t GetRegisterValue(MODBUS_WRITE_REGISTERS_t eRegister);

    /**
     * Sets the value of a modbus register with read access(address group 1).
     * @param eRegister - The register whose value needs to be set.
     * @param u16Value  - The value to be set
     * @return
     * None
     */
    void SetReadRegisterValue(MODBUS_READ_REGISTERS_t eRegister, uint16_t u16Value);


#if (TEST_AUTOMATION == YES)


    /**
     * Gets the value of a modbus automation support registers with write access(address group 3).
     * @param eRegister
     * @return value in the register
     */
    uint16_t GetRegisterValue(MODBUS_FOR_AUTOMATION_WRITE eRegister);

    /**
     * Sets the value of a modbus automation support registers with write access(address group 3).
     * @param eRegister - modbus register whose value is to be updated.
     * @param u16Value - value to be written in register.
     */
    void SetWriteRegisterValue(MODBUS_FOR_AUTOMATION_WRITE eRegister, uint16_t u16Value);

    /**
     * Gets the value of a modbus automation support registers with write access(address group 4).
     * @param eRegister
     * @return value in the register
     */
    uint16_t GetRegisterValue(MODBUS_FOR_AUTOMATION_READ eRegister);

    /**
     * Sets the value of a modbus automation support registers with write access(address group 4).
     * @param eRegister - modbus register whose value is to be updated.
     * @param u16Value - value to be written in register.
     */
    void SetReadRegisterValue(MODBUS_FOR_AUTOMATION_READ eRegister, uint16_t u16Value);
#endif


    /**
     * Sets the value of a modbus register with write access(address group 2).
     * @param eRegister - The register whose value needs to be set.
     * @param u16Value  - The value to be set
     * @return
     * None
     */
    void SetWriteRegisterValue(MODBUS_WRITE_REGISTERS_t eRegister, uint16_t u16Value);

    /**
     * Handles modbus transactions and keeps refreshing the modbus registers
     * with the latest data from other modules. This is expected to be called in
     * the super while loop.
     * @param - None
     * @return
     * None
     */
    void     Update();

    static void GetMBEventStatus(KEY_MB_CAN_EVENT_t *stEvent);

    static uint16_t MB_Count;

    static MISC_EEPROM_t stEepromMisc;

private:
    #define MODBUS_GEN_START_CMD        (0x01)
    #define MODBUS_GEN_STOP_CMD         (0x02)
    #define MODBUS_GEN_MODE_CHANGE_CMD  (0x01)
    #define ALARM_DISABLED               0U
    #define ALARM_INACTIVE               1U
    #define ALARM_WARNING                2U
    #define ALARM_SHUTDOWN               3U
    #define ALARM_ELEC_TRIP              4U
    #define ALARM_NOTIFICATION           5U
    #define ALARM_UNIMPLEMENTED          15U



    HAL_Manager         &_hal;
    CFGZ                &_cfgz;
    GCU_ALARMS          &_gcuAlarm;
    ENGINE_MONITORING   &_engineMonitoring;
    AUTO_MODE           &_Automode;
    uint16_t             _u16MODBUSCommand;
    uint16_t             _u16MODBUSOperModeCMD;

  //  static MODBUS_CMD_STATUS_t _eMBCmdStatus;
    static KEY_MB_CAN_EVENT_t stMBEvent ;



    static uint64_t Curr_MB_Valid_Count;

    /*Address group 1 buffer*/
    uint16_t _au16Grp1Registers[MODBUS_GRP1_REG_CNT];

    /*Address group 2 buffer*/
    uint16_t _au16Grp2Registers[MODBUS_GRP2_REG_CNT];

#if (TEST_AUTOMATION == YES)
    /*Address group 4 buffer*/
    uint16_t _au16Grp3Registers[MODBUS_GRP3_REG_CNT];
    uint16_t _au16Grp4Registers[MODBUS_GRP4_REG_CNT];
#endif

    /*List to store address groups*/
    MODBUS::ADDRESS_GROUP_t   _aAddressGrp[MODBUS_ADDRESS_GROUPS];
    MODBUS::ADDRESS_GRP_LST_t _AddressGrpLst;
    uint16_t _u16TempAlarmVal;
    /**
     * Determines the register group index from the register enum. The returned
     * value is used to index _aAddressGrp.
     * @param u16RegisterAddress - Register address
     * @param bReadAccess        - Should the group have read access
     * @param bWriteAccess       - Should the group have write access
     * @return
     * index of the register group.
     */
    uint8_t prvIdentifyRegisterGroup(uint16_t u16RegisterAddress,
                                                bool bReadAccess, bool bWriteAccess);

    /**
     * Updates the appropriate registers with the value of electrical parameters.
     * @param - None
     * @return
     * None
     */
    void prvUpdateElectricalParams();

    /**
     * Updates the appropriate registers with the value of analog sensors.
     * @param - None
     * @return
     * None
     */
    void prvUpdateAnalogParams();

    /**
     * Updates the appropriate registers with cumulative start and stop counts.
     * @param - None
     * @return
     * None
     */
    void prvUpdateStartTripsRunHours();

    /**
     * Updates the appropriate registers with response to the MODBUS commands.
     * @param - None
     * @return
     * None
     */
    void prvUpdateMBCommandStatus();

    /** Updates the appropriate registers with Time Stamp
     * */
    void prvUpdateTimeStamp();

    void prvUpdateEngSensorAlarms(uint8_t u8AlarmID1, uint8_t u8AlarmID2, uint8_t u8AlarmID3, uint8_t u8Offset);
    void prvUpdateEngSensorAlarms(uint8_t u8AlarmID1, uint8_t u8AlarmID2, uint8_t u8Offset);
    void prvUpdateEngSensorAlarms(uint8_t u8AlarmID1, uint8_t u8Offset);
    void prvUpdateAlarmRegValue(uint8_t u8AlarmID, uint8_t u8Offset);
    void prvUpdateAlarmRegStatus(uint8_t u8AlarmID, uint8_t u8Offset);


    void prvUpadateDIGInOut();
    void prvUpdateBtsParams();
    void prvUpdateTmpParams();
    void prvUpdateAUXSensorVal();

    void prvUpdateModbusParamInEventLog();

    /**
     * Updates the appropriate registers with response to the MODBUS commands.
     * @param - None
     * @return
     * None
     */
    void prvUpdateGCUAlarms();

    void prvGetMiscParams();

    void prvUpdateLatestDM1Messages(void);
    void prvUpdateEGRrelatedRegisters(void);
    void prvUpdateDm01FaultCodesOnModbus(void);
#if (TEST_AUTOMATION == YES)
    /**
     * This function updates the automation test support related modbus read registers.
     * @param bDeviceInConfigMode
     */
    void prvUpdateMBReadRegisterForAutomation(bool bDeviceInConfigMode);

    /**
     * This function updates the PGN number for automation test support related modbus read registers.
     */
    void prvUpdatePGNNumber(void);


    /**
     * This function updates the automation test support related modbus write registers.
     */
    void prvUpdateMBWriteRegisterForAutomation();
#endif

};

#endif
