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
#include "../EGOV/EGOV.h"

#define AUTOMATION 0
/*Defines the number of discontinuous address groups*/
#define MODBUS_ADDRESS_GROUPS   (2U)
/*Number of entries in the first address group*/
#define MODBUS_GRP1_REG_CNT     (MB_READ_REG_LAST)
/*Number of entries in the second address group*/
#if (AUTOMATION)
#define MODBUS_GRP2_REG_CNT     (7U)
#else
#define MODBUS_GRP2_REG_CNT     (2U)
#endif
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
    typedef enum {
        MB_REG_PROTOCOL_VER = 0,
        MB_MAINS_L1_N_VOLTAGE,
        MB_MAINS_L2_N_VOLTAGE,
        MB_MAINS_L3_N_VOLTAGE,
        MB_GEN_L1_N_VOLATGE,
        MB_GEN_L2_N_VOLATGE,
        MB_GEN_L3_N_VOLATGE,
        MB_GEN_L1_CURRENT,
        MB_GEN_L2_CURRENT,
        MB_GEN_L3_CURRENT,

        MB_MAINS_CUMM_ACTIVE_ENERGY_1,
        MB_MAINS_CUMM_ACTIVE_ENERGY_2,

        MB_GEN_CUMM_ACTIVE_ENERGY_1,
        MB_GEN_CUMM_ACTIVE_ENERGY_2,

        MB_GEN_L1_L2_VOLATGE,
        MB_GEN_L2_L3_VOLATGE,
        MB_GEN_L3_L1_VOLATGE,
        MB_GEN_R_FREQUENCY,
        MB_GEN_Y_FREQUENCY,
        MB_GEN_B_FREQUENCY,
        MB_GEN_L1_PF,
        MB_GEN_L2_PF,
        MB_GEN_L3_PF,
        MB_GEN_AVERAGE_PF, /* gu16GenTotalPF */

        MB_MAINS_L1_L2_VOLTAGE,
        MB_MAINS_L2_L3_VOLTAGE,
        MB_MAINS_L3_L1_VOLTAGE,
        MB_MAINS_R_FREQUENCY,
        MB_MAINS_Y_FREQUENCY,
        MB_MAINS_B_FREQUENCY,

        MB_GEN_L1_POWER,
        MB_GEN_L2_POWER,
        MB_GEN_L3_POWER,
        MB_GEN_TOTAL_POWER,
        MB_GEN_PERCENT_LOAD,

        MB_GEN_L1_APPARENT_POWER,
        MB_GEN_L2_APPARENT_POWER,
        MB_GEN_L3_APPARENT_POWER,
        MB_GEN_TOTAL_APPARENT_POWER,
        MB_GEN_L1_REACTIVE_POWER,
        MB_GEN_L2_REACTIVE_POWER,
        MB_GEN_L3_REACTIVE_POWER,
        MB_GEN_TOTAL_REACTIVE_POWER,


        MB_GEN_CUMM_APPARENT_ENERGY_1,
        MB_GEN_CUMM_APPARENT_ENERGY_2,
        MB_GEN_CUMM_REACTIVE_ENERGY_1,
        MB_GEN_CUMM_REACTIVE_ENERGY_2,



        MB_MAINS_CUMM_APPARENT_ENERGY_1,
        MB_MAINS_CUMM_APPARENT_ENERGY_2,
        MB_MAINS_CUMM_REACTIVE_ENERGY_1,
        MB_MAINS_CUMM_REACTIVE_ENERGY_2,


        MB_OIL_PRESSURE,
        MB_COOLANT_TEMPERATURE,
        MB_FUEL_LEVEL,
        MB_FUEL_IN_LIT,

        MB_CHARGING_ALT_VOLTAGE, /* extra */
        MB_BATTERY_VOLTAGE,
        MB_ENG_SPEED,
        MB_NO_OF_STARTS,
        MB_NO_OF_TRIPS,
        MB_ENG_RUN_HOURS_1, /* extra */
        MB_ENG_RUN_HOURS_2, /* extra */
        MB_ENG_RUN_MINUTES,

        MB_MAINS_RUN_HOURS, /* extra */
        MB_MAINS_RUN_MINUTES,

        MB_AUX_S1,  /*gu16AuxS1Value */
        MB_AUX_S2,  /*gu16AuxS2Value */
        MB_AUX_S3,  /*gu16AuxS3Value */
        MB_AUX_S4,  /*gu16AuxS4Value */

        MB_BTS_CHARGE_STATE, /* gu16BTSChargeState */
        MB_BTS_RUN_HRS,
        MB_BTS_RUN_MIN,  /* gu16BtsRunMins */

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

        MB_ALARM_13,   /* gu16AlarmReg13 */
        MB_ALARM_14,   /* gu16AlarmReg14 */
        MB_ALARM_15,   /* gu16AlarmReg15 */
        MB_ALARM_16,   /* gu16AlarmReg16 */
        MB_ALARM_17,   /* gu16AlarmReg17 */

        MB_DIG_IP_STATUS,  /* gu16DigIPStatus */
        MB_DIG_OP_STATUS,  /* gu16DigOPStatus */


//       MB_IP_OP_DIAGNOSTICS,  /*Doubt: This is additional parameter. Should this be added ?*/


        MB_GEN_STATUS,
        MB_TIME_STAMP0,
        MB_TIME_STAMP1,
        MB_TIME_STAMP2,
        MB_TIME_STAMP3,
#if (AUTOMATION ==1)
        MB_CA_VTG,
#endif
        MB_READ_REG_LAST
    } MODBUS_READ_REGISTERS_t;

    typedef enum name {
        MB_COMMAND       = 0,
        MB_DATE_TIME1,
        MB_DATE_TIME2,
        MB_DATE_TIME3,
        MB_DATE_TIME4,
        MB_DATE_TIME5,
    }MODBUS_WRITE_REGISTERS_t;



    typedef struct
    {
        bool    bStartKey;
        bool    bStopKey ;
        bool    bAutoKey ;
        bool    bAckKey  ;
        bool    bKeyEvent ;
    }KEY_MB_CAN_EVENT_t;
    /**
     * Constructs this class. This internally initiates the modbus module.
     * @param hal - Reference to hardware abstraction layer.
     * @return
     * None
     */
    MB_APP(HAL_Manager &hal,  CFGZ &cfgz, GCU_ALARMS &gcuAlarm,
            ENGINE_MONITORING &engineMonitoring, AUTO_MODE &Automode, EGOV &EGOV);

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
    EGOV                &_Egov;
    uint16_t             _u16MODBUSCommand;
    uint16_t             _u16MODBUSOperModeCMD;
  //  static MODBUS_CMD_STATUS_t _eMBCmdStatus;
    static KEY_MB_CAN_EVENT_t stMBEvent ;
    /*Address group 1 buffer*/
    uint16_t _au16Grp1Registers[MODBUS_GRP1_REG_CNT];

    /*Address group 2 buffer*/
    uint16_t _au16Grp2Registers[MODBUS_GRP2_REG_CNT];

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

    /**
     * Updates the appropriate registers with response to the MODBUS commands.
     * @param - None
     * @return
     * None
     */
    void prvUpdateGCUAlarms();

    /** Updates the appropriate registers with Time Stamp
     * */
    void prvUpdateTimeStamp();

    void UpdateEngSensorAlarms(uint8_t u8AlarmReg, uint8_t u8ShutdownID, uint8_t u8WarningID,
                        uint8_t u8SwitchID, uint8_t u8Offset);
    void UpdateAlarmRegValue(uint8_t u8AlarmID, uint8_t u8AlarmReg, uint8_t u8Offset);

    void UpdateAlarmRegStatus(uint8_t u8AlarmID, uint8_t u8AlarmReg, uint8_t u8Offset);

    void UpdateDGVoltAlarms(uint8_t u8AlarmReg, uint8_t u8WarningID, uint8_t u8ShutdownID, uint8_t u8Offset);
    void prvUpdateAUXReg(uint16_t u16AlarmID1, uint16_t u16AlarmID2, uint16_t u16Offset);

    void prvUpadateDIGInOut();
    void prvUpdateBtsParams();
    void prvUpdateAUXSensorVal();
};

#endif
