
/**
 * @file        [ui]
 * @brief
 * @version     [Version number]
 * @author      [Pramod Ranade]
 * @date        05th July 2021
 * @author      Madhuri A
 * @date        27 Sep 2021
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#include <HMI/MON_UI/MON_UI.h>
#include "ui.h"
#include "CKeyCodes.h"
#include "../../CFGZ/CFGZ.h"
#include "config.h"
#include "../CONST_UI/CONST_UI.h"
#include "../../CFGC/CFGC.h"
#include "../EGOV/EGOV.h"

#define CONFIG_VAL_INC_DEC_TIMER_MS (300U)
CMenuItem UI::menuItemsLowestLevel[INDEX_LAST];
CMenuItem UI::menuItemsMidLevel[NUM_OF_MENU_ITEMS_AT_MID_LEVEL];
CMenuItem UI::menuItemsTopLevel[NUM_OF_MENU_ITEMS_AT_TOP_LEVEL];
uint16_t u16IndexOfEditableItems=0,u16NumberofEditableItems=0;
//NewUI
<<<<<<< HEAD
uint8_t ParamInSubmenus[ID_SUB_MENU_LAST] = {4,2,4,4,3,3,  5,5,5,5,5,5,5,5,5,31,35,29,31,30,31,22,  2,2,2,2,2,2,2,  4,14,  9,8,8,6,7,9,  5,3,3,3,3,  11,8,7,4,4,   2,3,\
                                             6,6,6,20,35,20,36,23,32,32,4,7,11,12,12,12,5,7,7,5,8,7,7,11,13,6,3,3,4,4,9,20,4,5,3,1,3,4,10,1,3,6,4,1,2};
uint8_t SubmenusInMenus[ID_MAIN_MENU_LAST] = {6,16,7,2,6,5,5,2,  3,7,3,6,3,10,2,4,3,2,1,1};
=======
uint8_t ParamInSubmenus[ID_SUB_MENU_LAST] = {4,2,4,12,4,3,3,5,5,5,5,5,5,5,5,5,31,35,29,31,30,31,22,2,2,2,2,2,2,2,4,14,9,8,8,6,7,9,  5,3,3,3,3, 11,8,7,4,4,2,3,3,1};
uint8_t SubmenusInMenus[ID_MAIN_MENU_LAST] = {7,16,7,2,6,5,5,2,3,1};
>>>>>>> 2ba6ff6cf8a735594b10d9e5dfb4a128a8f5c654
//End
//All editable option string screens
enum
{
    ID_MANUAL_AUTO,
    ID_ENABLE_DISABLE,
    ID_MODBUS,
    ID_PARITY,
    ID_YES_NO,

    ID_EVENT_OCCURENCE,
    ID_DIG_IP_POLARITY,
    ID_DIG_IP_ACTION,
    ID_DIG_IP_ACTIVATION,

    ID_LOP_DIG_J_SENS_SEL,
    ID_FUEL_DIG_K_SENS_SEL,
    ID_ENG_CLNT_DIG_L_SENS_SEL,
    ID_AUX_S1_DIG_M_SENS_SEL,
    ID_AUX_S2_DIG_N_SENS_SEL,
    ID_AUX_S3_DIG_O_SENS_SEL,
    ID_AUX_S4_DIG_P_SENS_SEL,

    ID_FUEL_REF,
    ID_DIG_OP_POLARITY,

    ID_ALT_POLES,
    ID_NO_OF_PHASES,

    ID_EARTH_FAN_CM,

    ID_MAINS_PHASE_REVERSE_ACTION,
    ID_SPEED_SENSE_SOURCE,
    ID_CA_ACTION,
    ID_BAT_ACTION,
    ID_FLIT_MAINTAINANCE_ACTION,

    ID_ACTUATOR_APPLICATIONS,
    ID_ACT_DIRECTION,
    ID_EGOV_SET_POINT_SELECTION,
    ID_CLNT_TEMP_CTRL_OPT,
    ID_CT_LOCATION,

    ID_EVENT_DAY_OPTN,
    ID_RUN_EVERY_DAY,
    ID_THRESHOLD_TYPE,
    ID_ENG_FRQ_50HZ_60HZ,
    ID_COM_FAIL_ACTIVATION,
    ID_4_20MA_SENS_FAULT_ACTION,
    ID_NOTHING,
    ID_LAST
};
enum
{
  ID_PERCENT,
  ID_OHM,
  ID_DEG_C,
  ID_SEC,
  ID_MINS,
  ID_HZ,
  ID_RPM,
  ID_V,
  ID_HRS,
  ID_KW,
  ID_V_PH_N ,
  ID_AMPERE,
  ID_BAR,
  ID_PERCENT_RPM_ERROR,
  ID_ACT_SPEED_UNIT,
  ID_MILLI_AMPERE,
  ID_UNIT_LST
};

static const char* arrUnit[ID_UNIT_LST]=
{
   "%",
   "Ohm",
   "`C",
   "sec",
   "Mins",
   "Hz",
   "RPM",
   "V",
   "Hrs",
   "kW",
   "V Ph-N",
   "A",
   "Bar",
   "% RPM error",
   "x25 Hz",
   "mA",
};

static const char* strOutputSources[1][67] =
{
 {
  "Disable",
  "Sounder Alarm",
  "Battery Over Volt",
  "Battery Under Volt",
  "Charge Alt Shutdown",
  "Charge Alt Warning",
  "Close Gen Contactor",
  "Close Mains Contactor",

  "Mains Failure",
  "Common Alarm",
  "Common Electrical Trip",
  "Common Shutdown",
  "Common Warning",
  "Cooling Down",
  "Dig In A",
  "Dig In B",
  "Dig In C",
  "Dig In D",
  "Dig In E",
  "Dig In F",
  "Dig In G",
  "Dig In H",
  "Dig In I",
  "Dig In J (LOP Resistive)",
  "Dig In K (Anlg In Fuel LVL)",
  "Dig In L (Anlg In Eng Temp)",
  "Dig In M (Aux Sensor 1)",
  "Dig In N (Aux Sensor 2)",
  "Dig In O (Aux Sensor 3)",
  "Dig In P (Aux Sensor 4)",
  "Emergency Stop",
  "Stop Solenoid",
  "Fail To Start",
  "Fail To Stop",
  "Fuel Relay",
  "Gen Available",
  "R Phase OV Shutdown",
  "R Phase UV Shutdown",
  "Y Phase OV Shutdown",
  "Y Phase UV Shutdown",
  "B Phase OV Shutdown",
  "B Phase UV Shutdown",
  "Gen Over Current",
  "High Engine Temp",
  "Low Fuel LVL",
  "LFL Sensor Notification",
  "Low LOP",
  "Mains High Volt",
  "Mains Low Volt",
  "Pressure Open Circuit",
  "Open Gen Contactor",
  "Open Mains Contactor",
  "Over Freq Shutdown",
  "Over Speed Shutdown",
  "Gross Over Spd Shutdown",
  "Start Relay",
  "Temp Sensor Open Circuit",
  "Under Freq Shutdown",
  "Under Speed Shutdown",
  "Maintenance Due",
  "Stop Mode",
  "Auto Mode",
  "Manual Mode",
  "BTS Mode",
  "Preheat Output",
  "ECU Start",
  "Malfunction Indicator Lamp"
 }
};

static const char* strInputSources[1][21]=
{
 {
       "Not Used",
       "User Configured",
       "Low Fuel LVL Switch",
       "Low Lube Oil Press Switch",
       "High Engine Temp Switch",
       "Low Water LVL Switch",
       "Emergency Stop",
       "Remote Start / Stop",
       "Simulate Start",
       "Simulate Stop",
       "Simulate Auto",
       "Close Gen/Open Mains Swch",
       "Close Mains/Open Gen Swch",
       "Simulate Mains",
       "V-Belt Broken Switch",
       "Mains Contactor Latched",
       "Genset Contactor Latched",
       "Battery Charger Fail",
       "Smoke Fire",
       "Mode Select Switch",
       "Ambient Temp Switch"
   }

};

static const char* strBaudrateOptions[]=
{
    "1200",
    "2400",
    "4800",
    "9600",
    "19200",
    "38400",
    "57600",
    "115200"
};
static const char* strOptions[1][ID_LAST][8]=
{
 {
  //RushiStart
  {
   "Manual",
   "Auto"
  },
  {
   "Disable",
   "Enable"
  },
  {
   "None",
   "MODBUS"
  },
  {
   "None",
   "Even",
   "Odd"
  },
  {
   "No",
   "Yes"
  },
  //RushiEnd
  {
   "Daily",
   "Weekly",
   "Monthly"
  },
  {
   "Close To Activate",
   "Open To Activate"
  },
  {
   "None",
   "Notification",
   "Warning",
   "Electrical Trip",
   "Shutdown"
  },
  {
   "Never",
   "From Engine Start",
   "From Monitoring On",
   "Always"
  },

  {
   "Not used",
   "Dig In J",
   "LOP Sensor"
  },
  {
   "Not used",
   "Dig In K",
   "Fuel LVL Sensor"
  },
  {
   "Not used",
   "Dig In L",
   "Eng CLNT Temp"
  },
  {
   "Not used",
   "Dig in M",
   "S1 Sensor",
   "Shelter Temperature"
  },
  {
   "Not used",
   "Dig in N",
   "S2 Sensor"
  },
  {
   "Not used",
   "Dig in O",
   "4-20mA Sensor",
   "0-5V Sensor",
   "LOP sensor(4-20mA)"
  },
  {
   "Not used",
   "Dig in P",
   "4-20mA Sensor",
   "0-5V Sensor"
  },
  {
   "Engine Body",
   "Battery Negative"
  },
  {
   "De-Energise",
   "Energise"
  },
  {
   "2 Poles",
   "4 Poles",
   "6 Poles",
   "8 Poles"
  },
  {
   "1 Ph(L1-N)",
   "3 Ph(L1-L2-L3-N)",
   "Split Ph(L1-N-L2)"
  },
  {
   "EARTH Leakage Current",
   "FAN Current"
  },
  {
   "None",
   "Notification"
  },
  {
   "Alt Freq",
   "Magnetic pickup",
   "W Point Frequency"
  },
  {
   "None",
   "Notification",
   "Warning",
   "Electrical Trip",
   "Shutdown"
  },
  {
   "None",
   "Notification",
   "Warning",
   "Shutdown",
  },
  {
   "None",
   "Notification",
   "Warning",
  },

  {
   "As E-Governor",
   "As Start / Stop Device"
  },
  {
   "Clockwise",
   "Anti-Clockwise"
  },
  {
   "Fixed (0% Droop)",
   "Speed Bias Inp (0-5V)",
   "Load Based Droop"
  },
  {
   "Heater Control",
   "Cooler Control",
  },
  {
   "On Alt Output Cable",
   "On Load Cable"
  },
  {
   "SUNDAY",
   "MONDAY",
   "TUESDAY",
   "WEDNESDAY",
   "THURSDAY",
   "FRIDAY",
   "SATURDAY"
  },
  {
   "EVERYDAY"
  },

  {
   "Less Than Threshold",
   "Greater Than Threshold"
  },
  {
   "50HZ",
   "60HZ"
  },
  {
   "Never",
   "From Engine Start",
   "From Monitoring On",
   "Always",
   "While Fuel Relay ON"
  },
  {
   "Notification",
   "Warning",
   "Electrical Trip",
   "Shutdown"
  },
  {
   " "
  }
 }
};

static const char* strMainMenu[1][ID_MAIN_MENU_LAST]
{
    {
        //RushiStart
        "MODULE",
        "INPUTS",
        "OUTPUTS",
        "TIMERS",
        "GENERATOR",
        "MAINS",
        "ENGINE",
        "MAINTENANCE",
        //RushiEnd


        "ID",

        "SELECT PROFILE"
    }
};

static const char* strSubMenu[1][ID_SUB_MENU_LAST]
{
    {
        //RushiStart
        "GENERAL",
        "DISPLAY",
        "MODBUS_COMM",
        "CAN J1939 COMM",
        "BTS_CONFIG",
        "CYCLIC_CONFIG",
        "NIGHT_MODE_CONFIG",

        "DIG IN A",
        "DIG IN B",
        "DIG IN C",
        "DIG IN D",
        "DIG IN E",
        "DIG IN F",
        "DIG IN G",
        "DIG IN H",
        "DIG IN I",
        "LOP RES DIG J",
        "FUEL LVL DIG K",
        "ENG TEMP DIG M",
        "SHEL TEMP DIG N",
        "AUX S2_RES DIG N",
        "AUX S3 DIG O",
        "AUX S4 DIG P",

        "ID_OUT_A",
        "ID_OUT_B",
        "ID_OUT_C",
        "ID_OUT_D",
        "ID_OUT_E",
        "ID_OUT_F",
        "ID_OUT_G",

        "ID_CRANKING_TIMER",
        "ID_GENERAL_TIMER",

        "ID_ALT_CONFIG",
        "ID_VOLT_MONITOR",
        "ID_FREQ_MONITOR",
        "ID_CURRENT_MONITOR",
        "ID_FAN_CURR_MONITOR",
        "ID_LOAD_MONITOR",

        "ID_MAINS_CONFIG",
        "ID_UNDER_VOLT_MON",
        "ID_OVER_VOLT_MON",
        "ID_UNDER_FREQ_MON",
        "ID_OVER_FREQ_MON",

        "ID_CRANK_DISCONNECT",
        "ID_SPEED_MONITOR",
        "ID_BATTERY_MONITOR",
        "ID_CHARGE_ALT_MON",
        "ID_PREHEAT",

        "ID_MAINT_ALARM",
        "ID_ALARM_DUE_DATE",

        //RushiEnd

        //Password
        "ENG SR NO",
        "PASSWORD 1",
        "PASSWORD 2",

        //Select Profile
        "SELECT PROFILE",
    }
};

static const char* strLeafNode[1][SID_LEAF_NODE_STRING]
{
    {
        //RushiStart
        "PROFILE NAME" ,
        "POWER ON MODE" ,
        "POWER ON LAMP TEST" ,
        "PASSWORD LOCK" ,

        "CONTRAST",
        "POWER SAVE MODE",

        "COMM MODE",
        "MODBUS SLAVE ID",
        "BAUDRATE",
        "PARITY",

        "BATTERY MON",
        "LOW BATT THRESHOLD",
        "LOW BATT MON DELAY",
        "GEN RUN DURATION",

        "CYCLIC MODE",
        "GEN OFF DURATION",
        "GEN ON DURATION",

        "NIGHT MODE",
        "START TIME",
        "OFF DURATION",

        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION DELAY",

        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION DELAY",

        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION DELAY",

        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION DELAY",

        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION DELAY",

        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION DELAY",

        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION DELAY",

        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION DELAY",

        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION DELAY",

         "SENSOR_SELECTION",
         "DIG_SOURCE",
         "DIG_POLARITY",
         "DIG_ACTION",
         "DIG_ACTIVATION",
         "DIG_ACTIVATION_DELAY",
         "SHUTDOWN",
         "SHUTDOWN_THRESHOLD",
         "WARNING",
         "WARNING_THRESHOLD",
         "CIRCUIT_FAULT_ACTION",
         "R1",
         "V1",
         "R2",
         "V2",
         "R3",
         "V3",
         "R4",
         "V4",
         "R5",
         "V5",
         "R6",
         "V6",
         "R7",
         "V7",
         "R8",
         "V8",
         "R9",
         "V9",
         "R10",
         "V10",

         "SENSOR_SELECTION",
         "DIG_SOURCE",
         "DIG_POLARITY",
         "DIG_ACTION",
         "DIG_ACTIVATION",
         "DIG_ACTIVATION_DELAY",
         "SHUTDOWN",
         "SHUTDOWN_THRESHOLD",
         "NOTIFICATION",
         "NOTIFICATION_THRESHOLD",
         "FUEL_TANK_CAPACITY",
         "FUEL_THEFT_WARNING",
         "FUEL_THEFT_THRESHOLD",
         "FUEL_CONSUMPTION",
         "FUEL_IN_LITERS",
         "R1",
         "L1",
         "R2",
         "L2",
         "R3",
         "L3",
         "R4",
         "L4",
         "R5",
         "L5",
         "R6",
         "L6",
         "R7",
         "L7",
         "R8",
         "L8",
         "R9",
         "L9",
         "R10",
         "L10",

<<<<<<< HEAD
         "SENSOR_SELECTION",
         "DIG_SOURCE",
         "DIG_POLARITY",
         "DIG_ACTION",
         "DIG_ACTIVATION",
         "DIG_ACTIVATION_DELAY",
         "ACTION",
         "THRESHOLD",
         "OPEN_CKT_WARNING",
         "R1",
         "T1",
         "R2",
         "T2",
         "R3",
         "T3",
         "R4",
         "T4",
         "R5",
         "T5",
         "R6",
         "T6",
         "R7",
         "T7",
         "R8",
         "T8",
         "R9",
         "T9",
         "R10",
         "T10",

         "SENSOR_SELECTION",
         "DIG_SOURCE",
         "DIG_POLARITY",
         "DIG_ACTION",
         "DIG_ACTIVATION",
         "DIG_ACTIVATION_DELAY",
         "HIGH_TEMP_THRESHOLD",
         "LOW_TEMP_THRESHOLD",
         "HIGH_TEMP_MON_DELAY",
         "DG_RUN_DURATION",
         "OPEN_CKT_NOTIFICATION",
         "R1",
         "T1",
         "R2",
         "T2",
         "R3",
         "T3",
         "R4",
         "T4",
         "R5",
         "T5",
         "R6",
         "T6",
         "R7",
         "T7",
         "R8",
         "T8",
         "R9",
         "T9",
         "R10",
         "T10",
=======
         "SID_ENG_TEMP_DIG_L_SENSOR_SELECTION",
         "SID_ENG_TEMP_DIG_L_DIG_SOURCE",
         "SID_ENG_TEMP_DIG_L_DIG_POLARITY",
         "SID_ENG_TEMP_DIG_L_DIG_ACTION",
         "SID_ENG_TEMP_DIG_L_DIG_ACTIVATION",
         "SID_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY",
         "SID_ENG_TEMP_DIG_L_ACTION",
         "SID_ENG_TEMP_DIG_L_THRESHOLD",
         "SID_ENG_TEMP_DIG_L_OPEN_CKT_WARNING",
         "SID_ENG_TEMP_DIG_L_R1",
         "SID_ENG_TEMP_DIG_L_T1",
         "SID_ENG_TEMP_DIG_L_R2",
         "SID_ENG_TEMP_DIG_L_T2",
         "SID_ENG_TEMP_DIG_L_R3",
         "SID_ENG_TEMP_DIG_L_T3",
         "SID_ENG_TEMP_DIG_L_R4",
         "SID_ENG_TEMP_DIG_L_T4",
         "SID_ENG_TEMP_DIG_L_R5",
         "SID_ENG_TEMP_DIG_L_T5",
         "SID_ENG_TEMP_DIG_L_R6",
         "SID_ENG_TEMP_DIG_L_T6",
         "SID_ENG_TEMP_DIG_L_R7",
         "SID_ENG_TEMP_DIG_L_T7",
         "SID_ENG_TEMP_DIG_L_R8",
         "SID_ENG_TEMP_DIG_L_T8",
         "SID_ENG_TEMP_DIG_L_R9",
         "SID_ENG_TEMP_DIG_L_T9",
         "SID_ENG_TEMP_DIG_L_R10",
         "SID_ENG_TEMP_DIG_L_T10",

         "SID_SHEL_TEMP_DIG_M_SENSOR_SELECTION",
         "SID_SHEL_TEMP_DIG_M_DIG_SOURCE",
         "SID_SHEL_TEMP_DIG_M_DIG_POLARITY",
         "SID_SHEL_TEMP_DIG_M_DIG_ACTION",
         "SID_SHEL_TEMP_DIG_M_DIG_ACTIVATION",
         "SID_SHEL_TEMP_DIG_M_DIG_ACTIVATION_DELAY",
         "SID_SHEL_TEMP_DIG_M_HIGH_TEMP_THRESHOLD",
         "SID_SHEL_TEMP_DIG_M_LOW_TEMP_THRESHOLD",
         "SID_SHEL_TEMP_DIG_M_HIGH_TEMP_MON_DELAY",
         "SID_SHEL_TEMP_DIG_M_DG_RUN_DURATION",
         "SID_SHEL_TEMP_DIG_M_OPEN_CKT_NOTIFICATION",
         "SID_SHEL_TEMP_DIG_M_R1",
         "SID_SHEL_TEMP_DIG_M_T1",
         "SID_SHEL_TEMP_DIG_M_R2",
         "SID_SHEL_TEMP_DIG_M_T2",
         "SID_SHEL_TEMP_DIG_M_R3",
         "SID_SHEL_TEMP_DIG_M_T3",
         "SID_SHEL_TEMP_DIG_M_R4",
         "SID_SHEL_TEMP_DIG_M_T4",
         "SID_SHEL_TEMP_DIG_M_R5",
         "SID_SHEL_TEMP_DIG_M_T5",
         "SID_SHEL_TEMP_DIG_M_R6",
         "SID_SHEL_TEMP_DIG_M_T6",
         "SID_SHEL_TEMP_DIG_M_R7",
         "SID_SHEL_TEMP_DIG_M_T7",
         "SID_SHEL_TEMP_DIG_M_R8",
         "SID_SHEL_TEMP_DIG_M_T8",
         "SID_SHEL_TEMP_DIG_M_R9",
         "SID_SHEL_TEMP_DIG_M_T9",
         "SID_SHEL_TEMP_DIG_M_R10",
         "SID_SHEL_TEMP_DIG_M_T10",
>>>>>>> 2ba6ff6cf8a735594b10d9e5dfb4a128a8f5c654

         "SENSOR_SELECTION",
         "DIG_SOURCE",
         "DIG_POLARITY",
         "DIG_ACTION",
         "DIG_ACTIVATION",
         "DIG_ACTIVATION_DELAY",
         "ACTION",
         "THRESHOLD",
         "THRESHOLD_TYPE",
         "OPEN_CKT_WARNING",
         "R1",
         "V1",
         "R2",
         "V2",
         "R3",
         "V3",
         "R4",
         "V4",
         "R5",
         "V5",
         "R6",
         "V6",
         "R7",
         "V7",
         "R8",
         "V8",
         "R9",
         "V9",
         "R10",
         "V10",

         "SENSOR_SELECTION",
         "DIG_SOURCE",
         "DIG_POLARITY",
         "DIG_ACTION",
         "DIG_ACTIVATION",
         "DIG_ACTIVATION_DELAY",
         "SHUTDOWN",
         "SHUTDOWN_THRESHOLD",
         "WARNING",
         "WARNING_THRESHOLD",
         "CIRCUIT_FAULT_ACTION",
         "I1/V1",
         "P1",
         "I2/V2",
         "P2",
         "I3/V3",
         "P3",
         "I4/V4",
         "P4",
         "I5/V5",
         "P5",
         "I6/V6",
         "P6",
         "I7/V7",
         "P7",
         "I8/V8",
         "P8",
         "I9/V9",
         "P9",
         "I10/V10"
         "P10",

         "SENSOR_SELECTION",
         "DIG_SOURCE",
         "DIG_POLARITY",
         "DIG_ACTION",
         "DIG_ACTIVATION",
         "DIG_ACTIVATION_DELAY",
         "SHUTDOWN",
         "SHUTDOWN_THRESHOLD",
         "NOTIFICATION",
         "NOTIFICATION_THRESHOLD",
         "FUEL_THEFT_WARNING",
         "FUEL_THEFT_THRESHOLD",
         "FUEL_CONSUMPTION",
         "FUEL_IN_LITERS",
         "SENSOR_LOW_VTG",
         "SENSOR_HIGH_VTG",
         "TANK_WITH_STEP",
         "TANK_WIDTH",
         "TANK_LENGTH_1",
         "TANK_HEIGHT_1",
         "TANK_LENGTH_2",
         "TANK_HEIGHT_2",
         /*Outputs*/
         "SOURCE",
         "ON_ACTIVATION",

         "SOURCE",
         "ON_ACTIVATION",

         "SOURCE",
         "ON_ACTIVATION",

         "SOURCE",
         "ON_ACTIVATION",

         "SOURCE",
         "ON_ACTIVATION",

         "SOURCE",
         "ON_ACTIVATION",

         "SOURCE",
         "ON_ACTIVATION",
         /*Timers*/
         "_CRANK_HOLD_TIME",
         "_CRANK_REST_TIME",
         "_MANUAL_START_DELAY",
         "_AUTO_START_DELAY",

<<<<<<< HEAD
         "SAFETY_MONITOR_DELAY",
         "MAINS_DETECT_DELAY",
         "ALT_DETECT_DELAY",
         "WARM_UP_DELAY",
         "RETN_TO_MAINS_DELAY",
         "ENG_COOL_TIME",
         "STOP_ACTION_TIME",
         "ADDN_STOPPING_TIME",
         "LOAD_TRANSFER_DELAY",
         "PWR_SAVE_MODE_DELAY",
         "SCRN_CHNGOVER_TIME",
         "DEEP_SLP_MODE_DELAY",
         "SOUNDER_ALARM_TIMER",
         "TEST_MODE_TIMER",
=======
         "SID_GENERAL_TIMER_SAFETY_MONITOR_DELAY",
         "SID_GENERAL_TIMER_MAINS_DETECT_DELAY",
         "SID_GENERAL_TIMER_ALT_DETECT_DELAY",
         "SID_GENERAL_TIMER_WARM_UP_DELAY",
         "SID_GENERAL_TIMER_RETN_TO_MAINS_DELAY",
         "SID_GENERAL_TIMER_ENG_COOL_TIME",
         "SID_GENERAL_TIMER_STOP_ACTION_TIME",
         "SID_GENERAL_TIMER_ADDN_STOPPING_TIME",
         "SID_GENERAL_TIMER_LOAD_TRANSFER_DELAY",
         "SID_GENERAL_TIMER_PWR_SAVE_MODE_DELAY",
         "SID_GENERAL_TIMER_SCRN_CHNGOVER_TIME",
         "SID_GENERAL_TIMER_DEEP_SLP_MODE_DELAY",
         "SID_GENERAL_TIMER_SOUNDER_ALARM_TIMER",
         "SID_GENERAL_TIMER_TEST_MODE_TIMER",
>>>>>>> 2ba6ff6cf8a735594b10d9e5dfb4a128a8f5c654
         /*Generator*/
         "ALT_PRESENT",
         "NUMBER_OF_POLES",
         "ALT_AC_SYSTEM",
         "MIN_HEALTHY_VOLT",
         "MIN_HEALTHY_FREQ",
         "PHASE_REVERSAL_DETECT",
         "PHASE_REVERSAL_ACTION",
         "AUTO_LOAD_TRANSFER",
         "ALT_WAVE_DETECTION",

         "UNDER_VOLT_SHUTDOWN",
         "UV_SHUTDOWN_THRESHOLD",
         "UNDER_VOLT_WARNING",
         "UV_WARNING_THRESHOLD",
         "OVER_VOLT_SHUTDOWN",
         "OV_SHUTDOWN_THRESHOLD",
         "OVER_VOLT_WARNING",
         "OV_WARNING_THRESHOLD",

         "UNDER_FREQ_SHUTDOWN",
         "UF_SHUTDOWN_THRESHOLD",
         "UNDER_FREQ_WARNING",
         "UF_WARNING_THRESHOLD",
         "OVER_FREQ_SHUTDOWN",
         "OF_SHUTDOWN_THRESHOLD",
         "OVER_FREQ_WARNING",
         "OF_WARNING_THRESHOLD",

         "LOAD_CT_RATIO",
         "OVER_CURR_ACTION",
         "OVER_CURR_THRESHOLD",
         "OVER_CURR_DELAY",
         "CT_CORRECTION_FACTOR",
         "CT_LOCATION",

         "FAN_CURRENT_MON",
         "FAN_MON_CT_RATIO",
         "HIGH_CURR_THRESHOLD",
         "HIGH_CURR_ACTION",
         "LOW_CURR_THRESHOLD",
         "LOW_CURR_ACTION",
         "CURR_MON_DELAY",

         "GEN_RATING",
         "FULL_LOAD_CURRENT",
         "OVERLOAD_ACTION",
         "OVERLOAD_THRESHOLD",
         "OVERLOAD_MON_DELAY",
         "UNBAL_LOAD_ACTION",
         "UNBAL_LOAD_THRESHOLD",
         "UNBAL_LOAD_DELAY",
         "UNBAL_LOAD_ACT_THRESH",

         /*Mains*/
         "MAINS_MONITORING",
         "MAINS_AC_SYSTEM",
         "PHASE_REVERSAL_DETECT",
         "PHASE_REVERSAL_ACTION",
         "3PH_CALC_EN_FOR_1PH",

         "UNDER_VOLT_MON_ENABLE",
         "UNDER_VOLT_MON_TRIP",
         "UNDER_VOLT_MON_RETURN",

         "OVER_VOLT_MON_ENABLE",
         "OVER_VOLT_MON_TRIP",
         "OVER_VOLT_MON_RETURN",

         "UNDER_FREQ_MON_ENABLE",
         "UNDER_FREQ_MON_TRIP",
         "UNDER_FREQ_MON_RETURN",

         "OVER_FREQ_MON_ENABLE",
         "OVER_FREQ_MON_TRIP",
         "OVER_FREQ_MON_RETURN",
         /*Engine*/
         "START_ATTEMPTS",
         "DISCONN_ON_LOP_SENS",
         "DISCONN_LOP_SENS",
         "MON_LLOP_BEFORE_CRANK",
         "MON_LOP_BEFORE_CRANK",
         "DISCONN_ON_LLOP_SW",
         "LLOP_SW_TRANS_TIME",
         "ALT_FREQUENCY",
         "ENGINE_SPEED",
         "DISCONN_ON_CHG_ALT_VOLT",
         "CHG_ALT_THRESHOLD",

         "SPEED_SENSE_SOURCE",
         "RESERVED",
         "UNDER_SPEED_SHUTDOWN",
         "UNDER_SPEED_THRESHOLD",
         "UNDER_SPEED_DELAY",
         "OVER_SPEED_THRESHOLD",
         "OVER_SPEED_DELAY",
         "GROSS_OS_THRESHOLD",

         "LOW_VOLT_ACTION",
         "LOW_VOLT_THRESHOLD",
         "LOW_VOLT_DELAY",
         "HIGH_VOLT_ACTION",
         "HIGH_VOLT_THRESHOLD",
         "HIGH_VOLT_DELAY",
         "BATTERY_MON_BY_J1939",

         "MON_FAIL_ACTION",
         "MON_FAIL_THRESHOLD",
         "MON_FAIL_DELAY",
         "MON_CHARGE_ALT_MON_BY_J1939",
         "PREHEAT_TIMER",
         "ENG_TEMPERATURE",
         "ENG_TEMP_THRESHOLD",
         "AMB_TEMPERATURE",
         /*Maintenance*/
         "ALARM_ACTION",
         "ALARM_DUE_AT_ENGINE_HOURS",
         "ALARM DUE DATE SERVICE DATE 1",
         "ALARM DUE DATE SERVICE DATE 2",
         "ALARM DUE DATE SERVICE DATE 3",

        //RushiEnd
        "PROFILE NAME",
        "POWER ON MODE",
        "POWER ON LAMP TEST",
        "DEEP SLEEP MODE",
        "HISTOGRAM",
        "WARNING AUTO CLEAR",

        "CYCLIC MODE",
        "GEN OFF DURATION",
        "GEN ON DURATION",
        "EVENT 1",
        "EVENT OCCURANCE",
        "EVENT DAY",
        "START TIME",
        "GEN ON DURATION",
        "LOAD TRANSFER",
        "EVENT 2",
        "NIGHT MODE RESTRICT",
        "GEN OFF DURATION",
        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION DELAY",
        "SENSOR SELECTION",
        "SHUTDOWN",
        "SHUTDOWN THRESHOLD",
        "WARNING",
        "WARNING THRESHOLD",
        "CKT FAULT ACTION",
        "R1",
        "V1",
        "R2",
        "V2",
        "R3",
        "V3",
        "R4",
        "V4",
        "R5",
        "V5",
        "R6",
        "V6",
        "R7",
        "V7",
        "R8",
        "V8",
        "R9",
        "V9",
        "R10",
        "V10",
        "LOW LVL SHUTDOWN",
        "SHUTDOWN THRESHOLD",
        "LOW LVL WARNING",
        "WARNING THRESHOLD",
        "FUEL TANK CAPACITY",
        "FUEL THEFT ALARM",
        "FUEL THEFT ALARM TH",
        "FUEL SENSOR REFERENCE",
        "THRESHOLD TYPE",
        "OPEN CKT ALARM",
        "SHELT TEMP THRESH",
        "SHELT TEMP HYST",
        "SHELT TEMP DELAY",
        "GEN RUN DURATION",
        "I1/V1",
        "VAL1",
        "I2/V2",
        "VAL2",
        "I3/V3",
        "VAL3",
        "I4/V4",
        "VAL4",
        "I5/V5",
        "VAL5",
        "I6/V6",
        "VAL6",
        "I7/V7",
        "VAL7",
        "I8/V8",
        "VAL8",
        "I9/V9",
        "VAL9",
        "I10/V10",
        "VAL10",
        "ON ACTIVATION",
        "CRANK HOLD TIME",
        "CRANK REST TIME",
        "MANUAL START DELAY",
        "AUTO START DELAY",
        "SAFETY MONITOR DELAY",
        "WARM-UP DELAY",
        "RETN-TO-MAINS DELAY",
        "ENG COOL TIME",
        "STOP ACTION TIME",
        "ADDN STOPPING TIME",
        "LOAD TRANSFER DELAY",
        "PWR SAVE MODE DELAY",
        "SCRN CHNGOVER TIME",
        "DEEP SLP MODE DELAY",
        "SOUNDER ALARM TIMER",
        "TEST MODE TIMER",
        "AUTO EXIT CNFG MODE",
        "ISV PULL SIGNAL TIMER",
        "GEN BRKR PULSE TMR",
        "MAINS BRKR PULSE TMR",
        "BREAKER FDBCK TIMER",
        "BREAKER CLOSE DELAY",
        "ALT PRESENT",
        "NUMBER OF POLES",
        "ALT AC SYSTEM",
        "MIN HEALTHY VOLT",
        "MIN HEALTHY FREQ",
        "PHASE REVERSAL DETECT",
        "PHASE REVERSAL ACTION",
        "AUTO LOAD TRANSFER",
        "WAVEFORM DETECT",
        "GEN PT ENABLE",
        "GEN PT PRIMARY",
        "GEN PT SECONDARY",
        "UNDER VOLT SHUTDOWN",
        "UV SHUTDOWN THRESH",
        "UV SHUTDOWN DELAY",
        "UNDER VOLT WARNING",
        "UV WARNING THRESHOLD",
        "UV WARNING DELAY",
        "OVER VOLT SHUTDOWN",
        "OV SHUTDOWN THRESH",
        "OV SHUTDOWN DELAY",
        "OVER VOLT WARNING",
        "OV WARNING THRESHOLD",
        "OV WARNING DELAY",
        "UNDER FREQ SHUTDOWN",
        "UF SHUTDOWN THRESH",
        "UF SHUTDOWN DELAY",
        "UNDER FREQ WARNING",
        "UF WARNING THRESHOLD",
        "UF WARNING DELAY",
        "OVER FREQ SHUTDOWN",
        "OF SHUTDOWN THRESH",
        "OF SHUTDOWN DELAY",
        "OVER FREQ WARNING",
        "OF WARNING THRESHOLD",
        "OF WARNING DELAY",
        "LOAD CT RATIO",
        "OVER CURR ACTION",
        "OVER CURR THRESHOLD",
        "OVER CURR DELAY",
        "CT LOCATION",
        "EARTH / FAN CURR MON",
        "CT RATIO",
        "HIGH CURR ACTION",
        "HIGH CURR THRESH",
        "CURR DELAY",
        "LOW CURR ACTION",
        "LOW CURR THRESH",
        "GEN RATING",
        "OVERLOAD ACTION",
        "OVERLOAD THRESHOLD",
        "OVERLOAD MON DELAY",
        "UNBAL LOAD ACTION",
        "UNBAL LOAD THRESHOLD",
        "UNBAL LOAD DELAY",
        "LOW LOAD EN",
        "LOW LOAD ACTION",
        "LOW LOAD TRIP",
        "LOW LOAD RETURN",
        "LOW LOAD DELAY",
        "MAINS MONITORING",
        "MAINS AC SYSTEM",
        "PHASE REVERSAL DETECT",
        "PHASE REVERSAL ACTION",
        "MAINS PARTIAL HEALTHY",

        "MAINS PT ENABLE",
        "MAINS PT PRIMARY",
        "MAINS PT SECONDARY",
        "UV ENABLE",
        "UV TRIP",
        "UV RETURN",
        "OV ENABLE",
        "OV TRIP",
        "OV RETURN",
        "MAINS VOLT TRIP DLY",
        "UF ENABLE",
        "UF TRIP",
        "UF RETURN",
        "OF ENABLE",
        "OF TRIP",
        "OF RETURN",
        "MAINS FREQ TRIP DLY",
        "START ATTEMPTS",
        "DISCONN ON LOP SENS",
        "DISCONN LOP THRESH",
        "MON LLOP BEF CRANK",
        "MON LOP BEF CRANK",
        "DISCONN ON LLOP SW",
        "LLOP SW TRANS TIME",
        "ALT FREQUENCY",
        "ENGINE SPEED",
        "DISC ON CHG ALT VOLT",
        "CHG ALT THRESHOLD",
        "SPEED SENSE SORCE",
        "FLYWHEEL TH/W PT FREQ",
        "UNDER SPEED SHUTDOWN",
        "UNDER SPEED THRESH",
        "UNDER SPEED DELAY",
        "OVER SPEED THRESH",
        "OVER SPEED DELAY",
        "GROSS OS THRESHOLD",
        "IDLE TO RATED DELAY",
        "STARTUP IDLE TIME",
        "STOPPING IDLE TIME",
        "IDLE MODE PULSE TIME",
        "INIT LOW SPEED",
        "LOW VOLT ACTION",
        "LOW VOLT THRESHOLD",
        "LOW VOLT DELAY",
        "HIGH VOLT ACTION",
        "HIGH VOLT THRESHOLD",
        "HIGH VOLT DELAY",
        "FAIL ACTION",
        "FAIL THRESHOLD",
        "FAIL DELAY",
        "PREHEAT TIMER",
        "ENG CLNT TEMP EN",
        "ENGINE CLNT TEMP LIMIT",
        "ENABLE",
        "TEMP CTRL",
        "ON THRESHOLD",
        "OFF THRESHOLD",
        "AFT ACT THRESHOLD",
        "AFT DEACT THRESHOLD",
        "TIMEOUT AFTER ACT",
        "MON AFT ON ENG ON",
        "ENGINE TYPE",
        "LOP FROM ECU",
        "CLNT TEMP FROM ECU",
        "ENG SPEED FROM ECU",
        "ENG RUN HRS FROM ECU",
        "BATT VTG FROM ECU",
        "ENG SPEED TO ECU",
        "ENG REQUESTED SPEED",
        "START/STOP CMD TO ECU",
        "PREHEAT CMD TO ECU",
        "ENGINE FREQUENCY",
        "ENGINE GAIN",
        "SGC SOURCE ADDRESS",
        "ECU SOURCE ADDRESS",
        "COMM FAILURE ACTION",
        "ACTIVATION (COMM FLR)",
        "ACT DELAY (COMM FLR)",
        "ACTION (AMBER)",
        "ACTIVATION (AMBER)",
        "ACT DELAY (AMBER)",
        "ACTION (RED)",
        "ACTIVATION (RED)",
        "ACT DELAY (RED)",
        "ACTION (MALFUN)",
        "ACTIVATION (MALFUN)",
        "ACT DELAY (MALFUN)",
        "ACTION (PROTECT)",
        "ACTIVATION (PROTECT)",
        "ACT DELAY (PROTECT)",
        "LOW LVL SHUTDOWN",
        "SHUTDOWN THRESHOLD",
        "LOW LVL WARNING",
        "WARNING THRESHOLD",
        "CLNT TEMP THRESH TYPE",
        "HIGH LVL SHUTDOWN",
        "SHUTDOWN THRESHOLD",
        "HIGH LVL WARNING",
        "WARNING THRESHOLD",
        "ACTION",
        "DUE AT ENGINE HOURS",
        "MAINT DUE ASH LOAD EN",
        "ALARM DUE DAY",
        "ALARM DUE MONTH",
        "ALARM DUE YEAR",
        "ACTUATOR APPLICATION",
        "ACTUATOR SPEED",
        "ACTUATOR DIRECTION",
        "SET SPEED SELECTION",
        "LOAD DROOP",
        "TARGET/RECOVERY SPD",
        "PROPORTIONAL GAIN",
        "INTEGRAL GAIN",
        "DERIVATIVE GAIN",
        "FRICTION SETOFF",
        "GAIN SCHEDULE TRIGGER",
        "LOADING FACTOR",
        "UNLOADING FACTOR",
        "CRANKING STEPS",
        "PID TRIGGER SPEED",
        "RAMP UP TIME",
        "PID ON TIME",
        "RUNNING STEPS",
        "RUN TIME",
        "NO OF STARTS",
        "NO OF TRIPS",
        "ENG kWh",
        "ENG kVAh",
        "ENG kVArh",
        "RUN TIME",
        "MAINS kWh",
        "MAINS kVAh",
        "MAINS kVArh",
        "FILT EN",
        "FILTER CONSTANT"
    }
};

CMenu mainMenu;
CMenu* pCurMenu = &mainMenu;
UI::PASSWORD_EDIT_FLAGS_t UI::stPassEdit={false,false};

char* arrMonth[12];

UI::UI(HAL_Manager &hal,  PASSWORD_ENTRY_UI &Password, CFGZ &cfgz, Display &Disp, ENGINE_MONITORING &engMon):
_objHal(hal),
_objPassword(Password),
_objcfgz(cfgz),
_objDisplay(Disp),
_engMon(engMon),
_u16MenuSp(0),
_menuStack{NULL},
_MiscParam{0},
_arrProfileNames{NULL},
profilename{0},
_u8LanguageArrayIndex(0),
_pCurEditableItemsScreen(NULL)
{
    UTILS_ResetTimer(&_ValIncDecTimer);
    prvFetchProfileNames();
}

void UI::InitEditableItems()
{
    CEditableItem::PASSWORD_t stPIN_1,stPIN_2;

    CEditableItem::ENG_SR_NO_t stENG_SR_NO;

    _u8LanguageArrayIndex =  0;

    for(uint8_t i=0; i<12; i++)
    {
        memcpy( &arrMonth[i], &StrMonth[_u8LanguageArrayIndex][i], 21);
    }

//RushiStart
    ArrEditableItem[INDEX_OF_GENERAL_PROFILE_NAME] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_PROFILE_NAME), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_PROFILE_NAME]," ", "%u", (uint8_t)1, (uint8_t)10, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_POWER_ON_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_POWER_ON_MODE), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_POWER_ON_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MANUAL_AUTO], 2  , CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_POWER_ON_LAMP_TEST] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_POWER_ON_LAMP_TEST),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_POWER_ON_LAMP_TEST], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_PASSWORD_LOCK] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_PASSWORD_LOCK),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_PASSWORD_LOCK], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DISPLAY_CONTRAST] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISPLAY_CONTRAST), strLeafNode[_u8LanguageArrayIndex][SID_DISPLAY_CONTRAST], arrUnit[ID_PERCENT], "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DISPLAY_POWER_SAVE_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISPLAY_POWER_SAVE_MODE),strLeafNode[_u8LanguageArrayIndex][SID_DISPLAY_POWER_SAVE_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_MODBUS_COMM_COMM_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_COMM_MODE),strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_COMM_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MODBUS], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_COMM_MODBUS_SLAVE_ID] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_MODBUS_SLAVE_ID), strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_MODBUS_SLAVE_ID], "", "%u", (uint8_t)1, (uint8_t)247, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_COMM_MODBUS_BAUDRATE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_MODBUS_BAUDRATE),strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_MODBUS_BAUDRATE], "", "%s", strBaudrateOptions, 8, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_COMM_PARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_PARITY),strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_PARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_PARITY], 3, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTION_AMBER] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_AMBER),strLeafNode[_u8LanguageArrayIndex][SID_AMBER_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTIVATION_AMBER] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTIVATION_AMBER),strLeafNode[_u8LanguageArrayIndex][SID_AMBER_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACT_DELAY_AMBER] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACT_DELAY_AMBER), strLeafNode[_u8LanguageArrayIndex][SID_AMBER_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTION_RED] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_RED),strLeafNode[_u8LanguageArrayIndex][SID_RED_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTIVATION_RED] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTIVATION_RED),strLeafNode[_u8LanguageArrayIndex][SID_RED_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACT_DELAY_RED] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACT_DELAY_RED), strLeafNode[_u8LanguageArrayIndex][SID_RED_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTION_MIL] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_MIL),strLeafNode[_u8LanguageArrayIndex][SID_MALFUNC_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTIVATION_MIL] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTIVATION_MIL),strLeafNode[_u8LanguageArrayIndex][SID_MALFUNC_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACT_DELAY_MIL] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACT_DELAY_MIL), strLeafNode[_u8LanguageArrayIndex][SID_MALFUNC_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTION_PROTECT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_PROTECT),strLeafNode[_u8LanguageArrayIndex][SID_PROTECT_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTIVATION_PROTECT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTIVATION_PROTECT),strLeafNode[_u8LanguageArrayIndex][SID_PROTECT_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACT_DELAY_PROTECT] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACT_DELAY_PROTECT), strLeafNode[_u8LanguageArrayIndex][SID_PROTECT_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_BTS_CONFIG_BATTERY_MON] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_BATTERY_MON], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BTS_CONFIG_LOW_BATT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_LOW_BATT_THRESHOLD], "", "%f", (float)40.0,(float) 55.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BTS_CONFIG_LOW_BATT_MON_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_MON_DELAY),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_LOW_BATT_MON_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)300, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BTS_CONFIG_DG_RUN_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_DG_RUN_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_CYCLIC_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_CYCLIC_MODE),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_CONFIG_CYCLIC_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_DG_OFF_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_CONFIG_DG_OFF_DURATION], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_DG_ON_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_ON_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_CONFIG_DG_ON_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFIG_NIGHT_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_NIGHT_MODE),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_MODE_CONFIG_NIGHT_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFIG_START_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_START_TIME),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_MODE_CONFIG_START_TIME], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)2359, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFIG_OFF_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_OFF_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_MODE_CONFIG_OFF_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)1439, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_A_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_B_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_C_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_D_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_E_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_F_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_G_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_H_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_I_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_LOP_DIG_J_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_DIG_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_DIG_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_DIG_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_SHUTDOWN_THRESHOLD], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_WARNING_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_WARNING_THRESHOLD], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R1), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R1], arrUnit[ID_OHM], "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V1), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V1], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R2), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R2], arrUnit[ID_OHM], "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V2), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V2], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R3), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R3], arrUnit[ID_OHM],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V3), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V3], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R4), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R4], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V4), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V4], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R5), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R5], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V5), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V5],"Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R6), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R6], arrUnit[ID_OHM],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V6), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V6], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R7), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R7], arrUnit[ID_OHM],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V7), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V7], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R8), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R8], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V8), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V8], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R9), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R9], arrUnit[ID_OHM], "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V9), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V9], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R10), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R10], arrUnit[ID_OHM],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V10), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V10], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_FUEL_DIG_K_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SHUTDOWN), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_SHUTDOWN], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD], arrUnit[ID_PERCENT], "%u",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_NOTIFICATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_NOTIFICATION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_NOTIFICATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD], arrUnit[ID_PERCENT], "%u",(float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY], "Litre", "%u", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD], "%/Hour", "%u",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
<<<<<<< HEAD
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_CONSUMPTION] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION], "", "%u",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED);
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_IN_LITERS] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_IN_LITERS), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_IN_LITERS], "", "%u",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
=======
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_CONSUMPTION] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION], "Litre", "%u", (float)0,(float) 5000,(float)1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_IN_LITERS] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_IN_LITERS), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_IN_LITERS],"Litre", "%u", (float)0,(float) 5000,(float)1, CEditableItem::PIN1_PIN2_ALLOWED );
>>>>>>> 2ba6ff6cf8a735594b10d9e5dfb4a128a8f5c654
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R1), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R1], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L1), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L1], arrUnit[ID_PERCENT],  "%u", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R2), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R2], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L2), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L2], arrUnit[ID_PERCENT],  "%u", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R3), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R3], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L3), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L3], arrUnit[ID_PERCENT],  "%u",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R4), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R4], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L4), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L4], arrUnit[ID_PERCENT],  "%u", (float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R5), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R5], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L5), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L5], arrUnit[ID_PERCENT],  "%u", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R6), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R6], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L6), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L6], arrUnit[ID_PERCENT],  "%u", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R7), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R7], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L7), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L7], arrUnit[ID_PERCENT],  "%u",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R8), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R8], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L8), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L8], arrUnit[ID_PERCENT],  "%u",(float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R9), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R9], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L9), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L9], arrUnit[ID_PERCENT],  "%u", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R10), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R10],arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L10), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L10], arrUnit[ID_PERCENT],  "%u", (float)0, (float)5000.0,(float)0.1,  CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENG_CLNT_DIG_L_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_ACTION],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_THRESHOLD], "%/Hour", "%u", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_OPEN_CKT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_OPEN_CKT_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_OPEN_CKT_WARNING],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R1), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R1], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T1), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T1],arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R2), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R2], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T2), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T2], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R3), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R3],arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T3), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T3], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R4), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R4], arrUnit[ID_OHM], "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T4), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T4], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R5), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R5], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T5), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T5], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R6), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R6], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T6), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T6], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R7), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R7], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T7), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T7], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R8), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R8], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T8), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T8], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R9), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R9], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T9), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T9], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R10), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R10], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T10), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T10], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENG_CLNT_DIG_L_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_HIGH_TEMP_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_HIGH_TEMP_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_HIGH_TEMP_THRESHOLD], "%/Hour", "%u",(float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_LOW_TEMP_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_LOW_TEMP_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_LOW_TEMP_THRESHOLD], "%/Hour", "%u",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_HIGH_TEMP_MON_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_HIGH_TEMP_MON_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_HIGH_TEMP_MON_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DG_RUN_DURATION] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DG_RUN_DURATION], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_OPEN_CKT_NOTIFICATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_OPEN_CKT_NOTIFICATION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_OPEN_CKT_NOTIFICATION],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R1), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R1], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T1), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T1],arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R2), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R2], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T2), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T2], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R3), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R3],arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T3), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T3], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R4), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R4], arrUnit[ID_OHM], "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T4), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T4], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R5), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R5], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T5), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T5], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R6), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R6], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T6), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T6], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R7), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R7], arrUnit[ID_OHM],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T7), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T7], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R8), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R8], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T8), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T8], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R9), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R9], arrUnit[ID_OHM],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T9), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T9], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R10), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R10], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T10), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T10], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENG_CLNT_DIG_L_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_THRESHOLD], "%/Hour", "%u", (float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_THRESHOLD_TYPE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_THRESHOLD_TYPE), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_THRESHOLD_TYPE], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_OPEN_CKT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_OPEN_CKT_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_OPEN_CKT_WARNING],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R1], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V1],arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R2], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V2], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R3), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R3],arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V3), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V3], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R4), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R4], arrUnit[ID_OHM], "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V4), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V4], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R5), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R5], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V5), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V5], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R6), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R6], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V6), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V6], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R7), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R7], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V7), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V7], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R8), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R8], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V8), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V8], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R9), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R9], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V9), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V9], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R10), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R10], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V10), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V10], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENG_CLNT_DIG_L_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SHUTDOWN), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_SHUTDOWN], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SHUTDOWN_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_SHUTDOWN_THRESHOLD], "%/Hour", "%u", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_WARNING], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_WARNING_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_WARNING_THRESHOLD], "%/Hour", "%u",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I1_V1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I1_V1], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P1],arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I2_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I2_V2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I2_V2], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P2], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I3_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I3_V3), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I3_V3],arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P3), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P3], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I4_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I4_V4), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I4_V4], arrUnit[ID_OHM], "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P4), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P4], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I5_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I5_V5), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I5_V5], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P5), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P5], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I6_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I6_V6), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I6_V6], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P6), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P6], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I7_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I7_V7), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I7_V7], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P7), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P7], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I8_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I8_V8), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I8_V8], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P8), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P8], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I9_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I9_V9), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I9_V9], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P9), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P9], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I10_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I10_V10), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I10_V10], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P10), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P10], arrUnit[ID_DEG_C],  "%f", (float)-25.0, (float)300.0,(float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENG_CLNT_DIG_L_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SHUTDOWN), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_SHUTDOWN], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SHUTDOWN_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_SHUTDOWN_THRESHOLD], "%/Hour", "%u", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_NOTIFICATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_NOTIFICATION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_NOTIFICATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_NOTIFICATION_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_NOTIFICATION_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_NOTIFICATION_THRESHOLD], "%/Hour", "%u",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_FUEL_THEFT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_FUEL_THEFT_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_FUEL_THEFT_WARNING],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_FUEL_THEFT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_FUEL_THEFT_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_FUEL_THEFT_THRESHOLD], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_FUEL_CONSUMPTION] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_FUEL_CONSUMPTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_FUEL_CONSUMPTION],arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_FUEL_IN_LITERS] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_FUEL_IN_LITERS), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_FUEL_IN_LITERS], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_LOW_VTG] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_LOW_VTG), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_SENSOR_LOW_VTG], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_HIGH_VTG] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_HIGH_VTG), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_SENSOR_HIGH_VTG],arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_WITH_STEP] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_WITH_STEP), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_WITH_STEP], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_WIDTH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_WIDTH), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_WIDTH], arrUnit[ID_OHM], "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_LENGTH_1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_LENGTH_1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_LENGTH_1], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_HEIGHT_1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_HEIGHT_1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_HEIGHT_1], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_LENGTH_2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_LENGTH_2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_LENGTH_2], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_HEIGHT_2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_HEIGHT_2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_HEIGHT_2], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_A_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_A_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_A_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_A_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_A_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_A_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_B_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_B_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_B_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_B_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_B_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_B_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_C_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_C_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_C_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_C_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_C_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_C_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_D_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_D_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_D_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_D_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_D_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_D_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_E_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_E_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_E_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_E_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_E_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_E_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_F_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_F_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_F_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_F_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_F_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_F_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_G_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_G_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_G_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_G_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_G_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_G_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_CRANKING_TIMER_CRANK_HOLD_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_HOLD_TIME), strLeafNode[_u8LanguageArrayIndex][SID_CRANKING_TIMER_CRANK_HOLD_TIME], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)15, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANKING_TIMER_CRANK_REST_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_REST_TIME), strLeafNode[_u8LanguageArrayIndex][SID_CRANKING_TIMER_CRANK_REST_TIME], arrUnit[ID_SEC], "%u", (uint16_t)2, (uint16_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANKING_TIMER_MANUAL_START_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_MANUAL_START_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_CRANKING_TIMER_MANUAL_START_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANKING_TIMER_AUTO_START_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_AUTO_START_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_CRANKING_TIMER_AUTO_START_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)43200, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_GENERAL_TIMER_SAFETY_MONITOR_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SAFETY_MONITOR_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_SAFETY_MONITOR_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)10, (uint16_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_MAINS_DETECT_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_MAINS_DETECT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_MAINS_DETECT_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_ALT_DETECT_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_ALT_DETECT_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)30, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_WARM_UP_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_WARM_UP_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_WARM_UP_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_RETN_TO_MAINS_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_RETN_TO_MAINS_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_RETN_TO_MAINS_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)600, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_ENG_COOL_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ENG_COOL_TIME),  strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_ENG_COOL_TIME], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_STOP_ACTION_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_STOP_ACTION_TIME), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_STOP_ACTION_TIME], arrUnit[ID_SEC], "%u", (uint16_t)10, (uint16_t)120, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_ADDN_STOPPING_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ADDN_STOPPING_TIME), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_ADDN_STOPPING_TIME], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)120, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_LOAD_TRANSFER_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_LOAD_TRANSFER_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_LOAD_TRANSFER_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_PWR_SAVE_MODE_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_PWR_SAVE_MODE_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_PWR_SAVE_MODE_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)1800, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_SCRN_CHNGOVER_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SCRN_CHNGOVER_TIME), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_SCRN_CHNGOVER_TIME], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)1800, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER__DEEP_SLP_MODE_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_DEEP_SLP_MODE_DELAY),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_DEEP_SLP_MODE_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)1800, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_SOUNDER_ALARM_TIMER] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SOUNDER_ALARM_TIMER),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_SOUNDER_ALARM_TIMER], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_TEST_MODE_TIMER] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_TEST_MODE_TIMER),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_TEST_MODE_TIMER], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_ALT_CONFIG_ALT_PRESENT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_ALT_PRESENT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_NUMBER_OF_POLES] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_NUMBER_OF_POLES),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_NUMBER_OF_POLES], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ALT_POLES], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_ALT_AC_SYSTEM] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_ALT_AC_SYSTEM], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_NO_OF_PHASES], 3, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_MIN_HEALTHY_VOLT] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_MIN_HEALTHY_VOLT), strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_MIN_HEALTHY_VOLT], arrUnit[ID_V_PH_N], "%f", (uint16_t)50, (uint16_t)350, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_MIN_HEALTHY_FREQ] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_MIN_HEALTHY_FREQ), strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_MIN_HEALTHY_FREQ], arrUnit[ID_HZ], "%u", (uint8_t)10, (uint8_t)75, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_PHASE_REVERSAL_DETECT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_PHASE_REVERSAL_DETECT),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_PHASE_REVERSAL_DETECT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_PHASE_REVERSAL_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_PHASE_REVERSAL_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_PHASE_REVERSAL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_AUTO_LOAD_TRANSFER] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_AUTO_LOAD_TRANSFER),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_AUTO_LOAD_TRANSFER], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_ALT_WAVE_DETECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_WAVE_DETECTION),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_ALT_WAVE_DETECTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD], arrUnit[ID_V_PH_N], "%u", (uint16_t)50, (uint16_t)295, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_UNDER_VOLT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_WARNING),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_UNDER_VOLT_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_UV_WARNING_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UV_WARNING_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_UV_WARNING_THRESHOLD], arrUnit[ID_V_PH_N], "%u", (uint16_t)55, (uint16_t)300, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_OVER_VOLT_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD], arrUnit[ID_V_PH_N], "%u", (uint16_t)105, (uint16_t)350, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_OVER_VOLT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_WARNING),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_OVER_VOLT_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_OV_WARNING_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OV_WARNING_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_OV_WARNING_THRESHOLD], arrUnit[ID_V_PH_N], "%u", (uint16_t)100, (uint16_t)345, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_UF_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UF_SHUTDOWN_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_UF_SHUTDOWN_THRESHOLD], arrUnit[ID_HZ], "%f", (float)10.0, (float)59.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_UNDER_FREQ_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UNDER_FREQ_WARNING),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_UNDER_FREQ_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_UF_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UF_WARNING_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_UF_WARNING_THRESHOLD], arrUnit[ID_HZ], "%f", (float)11.0, (float)60.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_OVER_FREQ_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OVER_FREQ_SHUTDOWN),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_OVER_FREQ_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_OF_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OF_SHUTDOWN_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_OF_SHUTDOWN_THRESHOLD], arrUnit[ID_HZ], "%f", (float)26.0, (float)75.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_OVER_FREQ_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OVER_FREQ_WARNING),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_OVER_FREQ_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_OF_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OF_WARNING_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_OF_WARNING_THRESHOLD], arrUnit[ID_HZ], "%f", (float)25.0, (float)74.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_LOAD_CT_RATIO] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_LOAD_CT_RATIO),strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_LOAD_CT_RATIO], "/5", "%u", (uint16_t)0, (uint16_t)8000, CEditableItem::PIN1_PIN2_ALLOWED);
    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_OVER_CURR_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_OVER_CURR_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_OVER_CURR_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_OVER_CURR_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_OVER_CURR_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_OVER_CURR_THRESHOLD], "A", "%u", (uint16_t)5, (uint16_t)10000, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_OVER_CURR_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_OVER_CURR_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_OVER_CURR_DELAY], arrUnit[ID_SEC], "%u",(float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_CT_CORRECTION_FACTOR] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_CORRECTION_FACTOR),strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_CT_CORRECTION_FACTOR], "", "%s", strOptions[_u8LanguageArrayIndex][ID_CT_LOCATION], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_CT_LOCATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_LOCATION),strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_CT_LOCATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_CT_LOCATION], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_FAN_CURRENT_MON] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_FAN_CURRENT_MON),strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_FAN_CURRENT_MON], "", "%s", strOptions[_u8LanguageArrayIndex][ID_EARTH_FAN_CM], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_FAN_MON_CT_RATIO] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_FAN_MON_CT_RATIO),strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_FAN_MON_CT_RATIO], "/5", "%u", (uint16_t)0, (uint16_t)8000, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_HIGH_CURR_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_HIGH_CURR_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_HIGH_CURR_THRESHOLD], "A", "%f", (uint16_t)0, (uint16_t)10.0,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_HIGH_CURR_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_HIGH_CURR_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_HIGH_CURR_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_LOW_CURR_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_LOW_CURR_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_LOW_CURR_THRESHOLD], "A", "%f", (uint16_t)0, (uint16_t)10.0,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_LOW_CURR_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_LOW_CURR_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_LOW_CURR_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_CURR_MON_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_CURR_MON_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_CURR_MON_DELAY], arrUnit[ID_SEC], "%u", (float)1, (float)600,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_LOAD_MONITOR_GEN_RATING] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_GEN_RATING), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_GEN_RATING], arrUnit[ID_KW], "%u", (uint16_t)0, (uint16_t)8000, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_FULL_LOAD_CURRENT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_FULL_LOAD_CURRENT), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_FULL_LOAD_CURRENT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_OVERLOAD_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_OVERLOAD_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_OVERLOAD_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_OVERLOAD_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_OVERLOAD_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_OVERLOAD_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint16_t)50, (uint16_t)150, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_OVERLOAD_MON_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_OVERLOAD_MON_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_OVERLOAD_MON_DELAY], arrUnit[ID_SEC], "%u", (float)1, (float)600,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_UNBAL_LOAD_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_UNBAL_LOAD_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_UNBAL_LOAD_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_UNBAL_LOAD_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint16_t)5, (uint16_t)200, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_UNBAL_LOAD_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_UNBAL_LOAD_DELAY], arrUnit[ID_SEC], "%u", (float)1, (float)600,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_UNBAL_LOAD_ACT_THRESH] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_ACT_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_UNBAL_LOAD_ACT_THRESH], arrUnit[ID_PERCENT], "%u", (uint16_t)5, (uint16_t)200, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_MAINS_CONFIG_MAINS_MONITORING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_CONFIG_MAINS_MONITORING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_CONFIG_MAINS_AC_SYSTEM] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_CONFIG_MAINS_AC_SYSTEM], "", "%s", strOptions[_u8LanguageArrayIndex][ID_NO_OF_PHASES], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_CONFIG_PHASE_REVERSAL_DETECT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_PHASE_REVERSAL_DETECT),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_CONFIG_PHASE_REVERSAL_DETECT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_CONFIG_PHASE_REVERSAL_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_PHASE_REVERSAL_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_CONFIG_PHASE_REVERSAL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MAINS_PHASE_REVERSE_ACTION], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_UNDER_VOLT_MON_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_ENABLE),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_VOLT_MON_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_UNDER_VOLT_MON_TRIP] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_TRIP),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_VOLT_MON_TRIP], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_UNDER_VOLT_MON_RETURN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_RETURN),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_VOLT_MON_RETURN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_NO_OF_PHASES], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_VOLT_MON_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_VOLT_MON_ENABLE),strLeafNode[_u8LanguageArrayIndex][SID_OVER_VOLT_MON_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_VOLT_MON_TRIP] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_VOLT_MON_TRIP),strLeafNode[_u8LanguageArrayIndex][SID_OVER_VOLT_MON_TRIP], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MAINS_PHASE_REVERSE_ACTION], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_VOLT_MON_RETURN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_VOLT_MON_RETURN),strLeafNode[_u8LanguageArrayIndex][SID_OVER_VOLT_MON_RETURN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_UNDER_FREQ_MON_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_FREQ_MON_ENABLE),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_FREQ_MON_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_UNDER_FREQ_MON_TRIP] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_FREQ_MON_TRIP),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_FREQ_MON_TRIP], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_UNDER_FREQ_MON_RETURN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_FREQ_MON_RETURN),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_FREQ_MON_RETURN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_NO_OF_PHASES], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_FREQ_MON_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_FREQ_MON_ENABLE),strLeafNode[_u8LanguageArrayIndex][SID_OVER_FREQ_MON_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_FREQ_MON_TRIP] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_FREQ_MON_TRIP),strLeafNode[_u8LanguageArrayIndex][SID_OVER_FREQ_MON_TRIP], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MAINS_PHASE_REVERSE_ACTION], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_FREQ_MON_RETURN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_FREQ_MON_RETURN),strLeafNode[_u8LanguageArrayIndex][SID_OVER_FREQ_MON_RETURN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_CRANK_DISCONNECT_START_ATTEMPTS] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_START_ATTEMPTS), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONNECT_START_ATTEMPTS], "", "%u", (uint8_t)1, (uint8_t)9, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONNECT_DISCONN_ON_LOP_SENS] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_DISCONN_ON_LOP_SENS),strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONNECT_DISCONN_ON_LOP_SENS], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONNECT_DISCONN_LOP_SENS] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_DISCONN_LOP_SENS), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONNECT_DISCONN_LOP_SENS], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONNECT_MON_LLOP_BEFORE_CRANK] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_MON_LLOP_BEFORE_CRANK),strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONNECT_MON_LLOP_BEFORE_CRANK], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONNECT_MON_LOP_BEFORE_CRANK] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_MON_LOP_BEFORE_CRANK), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONNECT_MON_LOP_BEFORE_CRANK], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONNECT_DISCONN_ON_LLOP_SW] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_DISCONN_ON_LLOP_SW),strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONNECT_DISCONN_ON_LLOP_SW], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONNECT_LLOP_SW_TRANS_TIME] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_LLOP_SW_TRANS_TIME), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONNECT_LLOP_SW_TRANS_TIME], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONNECT_ALT_FREQUENCY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_ALT_FREQUENCY), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONNECT_ALT_FREQUENCY], "Bar", "%f", (float)0.5, (float)10.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONNECT_ENGINE_SPEED] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_ENGINE_SPEED),strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONNECT_ENGINE_SPEED], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONNECT_DISCONN_ON_CHG_ALT_VOLT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_DISCONN_ON_CHG_ALT_VOLT), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONNECT_DISCONN_ON_CHG_ALT_VOLT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONNECT_CHG_ALT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_CHG_ALT_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONNECT_CHG_ALT_THRESHOLD], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_SPEED_SENSE_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_SPEED_SENSE_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_SPEED_SENSE_SOURCE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_RESERVED] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_RESERVED),strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_RESERVED], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_UNDER_SPEED_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_UNDER_SPEED_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_UNDER_SPEED_DELAY], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_OVER_SPEED_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_OVER_SPEED_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_OVER_SPEED_THRESHOLD], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_OVER_SPEED_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_OVER_SPEED_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_OVER_SPEED_DELAY], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_GROSS_OS_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_GROSS_OS_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_GROSS_OS_THRESHOLD], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_LOW_VOLT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_LOW_VOLT_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_LOW_VOLT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_LOW_VOLT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_LOW_VOLT_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_LOW_VOLT_THRESHOLD], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_LOW_VOLT_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_LOW_VOLT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_LOW_VOLT_DELAY], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_HIGH_VOLT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_HIGH_VOLT_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_HIGH_VOLT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_HIGH_VOLT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_HIGH_VOLT_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_HIGH_VOLT_THRESHOLD], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_HIGH_VOLT_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_HIGH_VOLT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_HIGH_VOLT_DELAY], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_BATTERY_MON_BY_J1939] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_BATTERY_MON_BY_J1939),strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_BATTERY_MON_BY_J1939], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CHARGE_ALT_MON_FAIL_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_CHARGE_ALT_MON_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CHARGE_ALT_MON_FAIL_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_CHARGE_ALT_MON_FAIL_THRESHOLD], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CHARGE_ALT_MON_FAIL_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_CHARGE_ALT_MON_FAIL_DELAY], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CHARGE_ALT_MON_CHARGE_ALT_MON_BY_J1939] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_CHARGE_ALT_MON_BY_J1939),strLeafNode[_u8LanguageArrayIndex][SID_CHARGE_ALT_MON_CHARGE_ALT_MON_BY_J1939], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_PREHEAT_PREHEAT_TIMER] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_PREHEAT_TIMER),strLeafNode[_u8LanguageArrayIndex][SID_PREHEAT_PREHEAT_TIMER], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_PREHEAT_ENG_TEMPERATURE] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_ENG_TEMPERATURE),strLeafNode[_u8LanguageArrayIndex][SID_PREHEAT_ENG_TEMPERATURE], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_PREHEAT_ENG_TEMP_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_ENG_TEMP_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_PREHEAT_ENG_TEMP_THRESHOLD], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_PREHEAT_AMB_TEMPERATURE] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_AMB_TEMPERATURE),strLeafNode[_u8LanguageArrayIndex][SID_PREHEAT_AMB_TEMPERATURE], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_MAINT_ALARM_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINT_ALARM_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_MAINT_ALARM_ACTION_S], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINT_ALARM_DUE_AT_ENGINE_HOURS] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINT_ALARM_DUE_AT_ENGINE_HOURS), strLeafNode[_u8LanguageArrayIndex][SID_MAINT_ALARM_DUE_AT_ENGINE_HOURS], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE_1] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_1), strLeafNode[_u8LanguageArrayIndex][SID_ALARM_DUE_DATE_SERVICE_DATE_1], "", "%u", (uint8_t)1, (uint8_t)9, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE_2] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_2), strLeafNode[_u8LanguageArrayIndex][SID_ALARM_DUE_DATE_SERVICE_DATE_2], "", "%u", (uint8_t)1, (uint8_t)9, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE_3] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_3), strLeafNode[_u8LanguageArrayIndex][SID_ALARM_DUE_DATE_SERVICE_DATE_3], "", "%u", (uint8_t)1, (uint8_t)9, CEditableItem::PIN1_PIN2_ALLOWED );

    //RushiEnd
<<<<<<< HEAD
    ArrEditableItem[INDEX_OF_POWER_ON_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_POWER_ON_MODE), strLeafNode[_u8LanguageArrayIndex][SID_POWER_ON_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MANUAL_AUTO], 2  , CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_POWER_ON_LAMP_TEST] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_POWER_ON_LAMP_TEST_EN),strLeafNode[_u8LanguageArrayIndex][SID_POWER_ON_LAMP_TEST], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DEEP_SLEEP_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DEEP_SLEEP_EN),strLeafNode[_u8LanguageArrayIndex][SID_DEEP_SLEEP_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_HISTOGRAM] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_HISTOGRAM),strLeafNode[_u8LanguageArrayIndex][SID_LOAD_HISTOGRAM], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_WARNING_AUTO_CLEAR] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_WARNING_AUTO_CLEAR_EN),strLeafNode[_u8LanguageArrayIndex][SID_AUTO_CLEAR_WARNINGS], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LANGUAGE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE), strLeafNode[_u8LanguageArrayIndex][SID_LANGUAGE_SUPPORT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_LANGUAGE], 3  , CEditableItem::PIN1_PIN2_ALLOWED );

 //   ArrEditableItem[INDEX_OF_CONTRAST] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISPLAY_CONTRAST), strLeafNode[_u8LanguageArrayIndex][SID_CONTRAST], arrUnit[ID_PERCENT], "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_POWER_SAVE_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISPLAY_POWER_SAVE_MODE),strLeafNode[_u8LanguageArrayIndex][SID_POWER_SAVE_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );

 //   ArrEditableItem[INDEX_OF_COMM_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_COMM_MODE),strLeafNode[_u8LanguageArrayIndex][SID_COMM_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MODBUS], 2, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_MODBUS_SLAVE_ID] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_MODBUS_SLAVE_ID), strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_SLAVE_ID], "", "%u", (uint8_t)1, (uint8_t)247, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_MODBUS_BAUDRATE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_MODBUS_BAUDRATE),strLeafNode[_u8LanguageArrayIndex][SID_BAUDRATE], "", "%s", strBaudrateOptions, 8, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_MODBUS_PARITYBIT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_PARITY),strLeafNode[_u8LanguageArrayIndex][SID_PARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_PARITY], 3, CEditableItem::PIN1_PIN2_ALLOWED );

 //   ArrEditableItem[INDEX_OF_BTS_CONFIG_BATTERY_MON] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON),strLeafNode[_u8LanguageArrayIndex][SID_BTS_BATTERY_MON], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_BTS_CONFIG_LOW_BATT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_BTS_LOW_BATT_THRESHOLD], "", "%f", (float)12.0,(float) 60.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_BTS_CONFIG_LOW_BATT_MON_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_MON_DELAY),strLeafNode[_u8LanguageArrayIndex][SID_BTS_LOW_BATT_MON_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)300, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_BTS_CONFIG_DG_RUN_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_BTS_GEN_RUN_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );

 //   ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_CYCLIC_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_CYCLIC_MODE),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_PIN2_ALLOWED );
//    ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_DG_OFF_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_GEN_OFF_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );
//    ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_DG_ON_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_ON_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_GEN_ON_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_EXERCISER_EVENT_1] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_EN), strLeafNode[_u8LanguageArrayIndex][SID_EVENT_1], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_1] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_OCCURENCE), strLeafNode[_u8LanguageArrayIndex][SID_EVENT_OCCURANCE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_EVENT_OCCURENCE], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_START_DAY), strLeafNode[_u8LanguageArrayIndex][SID_EVENT_DAY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_EVENT_DAY_OPTN], 7, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EXERCISER_START_TIME_1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_START_TIME), strLeafNode[_u8LanguageArrayIndex][SID_START_TIME], arrUnit[ID_HRS], "%u", (uint16_t)0, (uint16_t)2359,true, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EXERCISER_DG_ON_DURATION_1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_ON_DURATION), strLeafNode[_u8LanguageArrayIndex][SID_EVENT_GEN_ON_DURATION], arrUnit[ID_HRS], "%u", (uint16_t)1, (uint16_t)9959,true, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EXERCISER_LOAD_TRANSFER_1] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_1_LOAD_TRANSFER), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_TRANSFER], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_EXERCISER_EVENT_2] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_EN), strLeafNode[_u8LanguageArrayIndex][SID_EVENT_2], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_2] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_OCCURENCE), strLeafNode[_u8LanguageArrayIndex][SID_EVENT_OCCURANCE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_EVENT_OCCURENCE], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_START_DAY), strLeafNode[_u8LanguageArrayIndex][SID_EVENT_DAY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_EVENT_DAY_OPTN], 7, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EXERCISER_START_TIME_2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_START_TIME), strLeafNode[_u8LanguageArrayIndex][SID_START_TIME], arrUnit[ID_HRS], "%u", (uint16_t)0, (uint16_t)2359,true, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EXERCISER_DG_ON_DURATION_2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_ON_DURATION), strLeafNode[_u8LanguageArrayIndex][SID_EVENT_GEN_ON_DURATION], arrUnit[ID_HRS], "%u", (uint16_t)1, (uint16_t)9959,true, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EXERCISER_LOAD_TRANSFER_2] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EXERCISE_2_LOAD_TRANSFER), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_TRANSFER], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );

  //  ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFIG_NIGHT_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_NIGHT_MODE),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_MODE_RESTRICT], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
  //  ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFIG_START_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_START_TIME),strLeafNode[_u8LanguageArrayIndex][SID_START_TIME],arrUnit[ID_HRS], "%u", (uint16_t)0, (uint16_t)2359,true, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFIG_OFF_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_OFF_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_GEN_OFF_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)1440, CEditableItem::PIN1_PIN2_ALLOWED );

  /*  ArrEditableItem[INDEX_OF_DIG_IN_A_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_B_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_C_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_D_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_E_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_F_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_G_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_H_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_I_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
*/
 //   ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_LOP_DIG_J_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_CKT_FAULT_ACTION],"", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_R1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R1), strLeafNode[_u8LanguageArrayIndex][SID_R1], arrUnit[ID_OHM], "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V1), strLeafNode[_u8LanguageArrayIndex][SID_V1], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_R2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R2), strLeafNode[_u8LanguageArrayIndex][SID_R2], arrUnit[ID_OHM], "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V2), strLeafNode[_u8LanguageArrayIndex][SID_V2], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_R3] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R3), strLeafNode[_u8LanguageArrayIndex][SID_R3], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V3), strLeafNode[_u8LanguageArrayIndex][SID_V3], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_R4] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R4), strLeafNode[_u8LanguageArrayIndex][SID_R4], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V4), strLeafNode[_u8LanguageArrayIndex][SID_V4], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_R5] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R5), strLeafNode[_u8LanguageArrayIndex][SID_R5], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V5), strLeafNode[_u8LanguageArrayIndex][SID_V5],"Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_R6] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R6), strLeafNode[_u8LanguageArrayIndex][SID_R6], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V6), strLeafNode[_u8LanguageArrayIndex][SID_V6], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_R7] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R7), strLeafNode[_u8LanguageArrayIndex][SID_R7], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V7), strLeafNode[_u8LanguageArrayIndex][SID_V7], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_R8] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R8), strLeafNode[_u8LanguageArrayIndex][SID_R8], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V8), strLeafNode[_u8LanguageArrayIndex][SID_V8], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_R9] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R9), strLeafNode[_u8LanguageArrayIndex][SID_R9], arrUnit[ID_OHM], "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V9), strLeafNode[_u8LanguageArrayIndex][SID_V9], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_R10] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R10), strLeafNode[_u8LanguageArrayIndex][SID_R10], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_LOP_SENS_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V10), strLeafNode[_u8LanguageArrayIndex][SID_V10], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_FUEL_DIG_K_SENSOR] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_FUEL_DIG_K_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_K_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_K_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_K_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_K_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_K_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LFL_SHUTDOWN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LOW_LEVEL_SHUT_DN_EN), strLeafNode[_u8LanguageArrayIndex][SID_LOW_LVL_SHUTDOWN], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LFL_SHUTDOWN_THRESHOLD] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SHUT_DN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_LOW_SHUTDOWN_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint8_t)0, (uint8_t)78, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LFL_WARNING_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LOW_LEVEL_WARN_EN), strLeafNode[_u8LanguageArrayIndex][SID_LOW_LVL_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LFL_WARNING_THRESHOLD] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_WARN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_LOW_WARNING_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint8_t)2, (uint8_t)80, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_TANK_CAPACITY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_TANK_CAPACITY), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_TANK_CAPACITY], "Litre", "%u", (uint16_t)2, (uint16_t)1000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_THEFT_ALARM_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_THEFT_WARN_EN), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_THEFT_ALARM], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_THEFT_THRESHOLD] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_THEFT_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_THEFT_ALARM_TH], "%/Hour", "%u", (uint8_t)1, (uint8_t)100, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_CKT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_FAULT_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_CKT_FAULT_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_REFERENCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENSOR_REFERENCE), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_SENSOR_REFERENCE], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_FUEL_REF], 2, CEditableItem::PIN1_PIN2_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_R1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R1), strLeafNode[_u8LanguageArrayIndex][SID_R1], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_V1] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V1), strLeafNode[_u8LanguageArrayIndex][SID_V1], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_R2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R2), strLeafNode[_u8LanguageArrayIndex][SID_R2], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_V2] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V2), strLeafNode[_u8LanguageArrayIndex][SID_V2], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_R3] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R3), strLeafNode[_u8LanguageArrayIndex][SID_R3], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_V3] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V3), strLeafNode[_u8LanguageArrayIndex][SID_V3], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_R4] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R4), strLeafNode[_u8LanguageArrayIndex][SID_R4], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_V4] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V4), strLeafNode[_u8LanguageArrayIndex][SID_V4], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100,CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_R5] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R5), strLeafNode[_u8LanguageArrayIndex][SID_R5], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_V5] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V5), strLeafNode[_u8LanguageArrayIndex][SID_V5], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_R6] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R6), strLeafNode[_u8LanguageArrayIndex][SID_R6], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_V6] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V6), strLeafNode[_u8LanguageArrayIndex][SID_V6], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_R7] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R7), strLeafNode[_u8LanguageArrayIndex][SID_R7], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_V7] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V7), strLeafNode[_u8LanguageArrayIndex][SID_V7], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_R8] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R8), strLeafNode[_u8LanguageArrayIndex][SID_R8], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_V8] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V8), strLeafNode[_u8LanguageArrayIndex][SID_V8], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_R9] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R9), strLeafNode[_u8LanguageArrayIndex][SID_R9], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_V9] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V9), strLeafNode[_u8LanguageArrayIndex][SID_V9], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_R10] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R10), strLeafNode[_u8LanguageArrayIndex][SID_R10],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_FUEL_SENS_V10] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V10), strLeafNode[_u8LanguageArrayIndex][SID_V10], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100,  CEditableItem::PIN1_ALLOWED );

 //   ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENG_CLNT_DIG_L_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
  //  ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
  //  ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_CKT_FAULT_ACTION],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R1), strLeafNode[_u8LanguageArrayIndex][SID_R1], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V1] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V1), strLeafNode[_u8LanguageArrayIndex][SID_V1],arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R2), strLeafNode[_u8LanguageArrayIndex][SID_R2], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V2] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V2), strLeafNode[_u8LanguageArrayIndex][SID_V2], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R3] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R3), strLeafNode[_u8LanguageArrayIndex][SID_R3],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V3] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V3), strLeafNode[_u8LanguageArrayIndex][SID_V3], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R4] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R4), strLeafNode[_u8LanguageArrayIndex][SID_R4], arrUnit[ID_OHM], "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V4] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V4), strLeafNode[_u8LanguageArrayIndex][SID_V4], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R5] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R5), strLeafNode[_u8LanguageArrayIndex][SID_R5], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V5] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V5), strLeafNode[_u8LanguageArrayIndex][SID_V5], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R6] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R6), strLeafNode[_u8LanguageArrayIndex][SID_R6], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V6] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V6), strLeafNode[_u8LanguageArrayIndex][SID_V6], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R7] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R7), strLeafNode[_u8LanguageArrayIndex][SID_R7], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V7] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V7), strLeafNode[_u8LanguageArrayIndex][SID_V7], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R8] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R8), strLeafNode[_u8LanguageArrayIndex][SID_R8], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V8] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V8), strLeafNode[_u8LanguageArrayIndex][SID_V8], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R9] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R9), strLeafNode[_u8LanguageArrayIndex][SID_R9], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V9] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V9), strLeafNode[_u8LanguageArrayIndex][SID_V9], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R10] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R10), strLeafNode[_u8LanguageArrayIndex][SID_R10], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
//    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V10] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V10), strLeafNode[_u8LanguageArrayIndex][SID_V10], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION),strLeafNode[_u8LanguageArrayIndex][SID_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M_SENS_SEL], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_M_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_M_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_M_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_M_ACTIVATION] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_M_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_ACTIVATION_DLY),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S1_THRESHOLD_TYPE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_THRESH_TYPE),strLeafNode[_u8LanguageArrayIndex][SID_THRESHOLD_TYPE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_THRESHOLD_TYPE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S1_SHUTDOWN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_SHUT_DN_EN),strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN],"", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S1_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_SHUTDOWN_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN_THRESHOLD], "", "%f", (float)0, (float)1000,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S1_WARNING_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_WARN_EN), strLeafNode[_u8LanguageArrayIndex][SID_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S1_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_WARNING_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_WARNING_THRESHOLD], "", "%f", (float)0, (float)1000,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S1_CKT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_FAULT_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_CKT_FAULT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHELT_TEMP_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_SHELT_TEMP_THRESH], arrUnit[ID_DEG_C],  "%u", (uint16_t)25, (uint16_t)300, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHELT_TEMP_HYSTERISIS] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_HYST),strLeafNode[_u8LanguageArrayIndex][SID_SHELT_TEMP_HYST],arrUnit[ID_DEG_C],  "%u", (uint16_t)1, (uint16_t)100, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHELT_TEMP_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_MON_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SHELT_TEMP_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)300, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHELT_TEMP_RUN_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_RUN_DURATION_MIN),strLeafNode[_u8LanguageArrayIndex][SID_SHELT_GEN_RUN_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );
    if(ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].value.u8Val != CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I1),strLeafNode[_u8LanguageArrayIndex][SID_R1], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V1),strLeafNode[_u8LanguageArrayIndex][SID_V1], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I2),strLeafNode[_u8LanguageArrayIndex][SID_R2], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V2),strLeafNode[_u8LanguageArrayIndex][SID_V2], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R3] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I3),strLeafNode[_u8LanguageArrayIndex][SID_R3], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V3),strLeafNode[_u8LanguageArrayIndex][SID_V3], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R4] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I4),strLeafNode[_u8LanguageArrayIndex][SID_R4], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V4),strLeafNode[_u8LanguageArrayIndex][SID_V4], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R5] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I5),strLeafNode[_u8LanguageArrayIndex][SID_R5], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V5),strLeafNode[_u8LanguageArrayIndex][SID_V5], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R6] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I6),strLeafNode[_u8LanguageArrayIndex][SID_R6], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V6),strLeafNode[_u8LanguageArrayIndex][SID_V6], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R7] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I7),strLeafNode[_u8LanguageArrayIndex][SID_R7], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V7),strLeafNode[_u8LanguageArrayIndex][SID_V7], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R8] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I8),strLeafNode[_u8LanguageArrayIndex][SID_R8], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V8),strLeafNode[_u8LanguageArrayIndex][SID_V8], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R9] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I9),strLeafNode[_u8LanguageArrayIndex][SID_R9], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V9),strLeafNode[_u8LanguageArrayIndex][SID_V9], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R10] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I10),strLeafNode[_u8LanguageArrayIndex][SID_R10], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V10),strLeafNode[_u8LanguageArrayIndex][SID_V10], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    }
    else
    {
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I1),strLeafNode[_u8LanguageArrayIndex][SID_R1], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V1] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V1),strLeafNode[_u8LanguageArrayIndex][SID_V1], "",  "%f", (int16_t)-25, (int16_t)300.0,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I2),strLeafNode[_u8LanguageArrayIndex][SID_R2], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V2] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V2),strLeafNode[_u8LanguageArrayIndex][SID_V2], "",  "%f", (int16_t)-25, (int16_t)300.0,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R3] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I3),strLeafNode[_u8LanguageArrayIndex][SID_R3], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V3] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V3),strLeafNode[_u8LanguageArrayIndex][SID_V3], "",  "%f", (int16_t)-25, (int16_t)300.0,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R4] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I4),strLeafNode[_u8LanguageArrayIndex][SID_R4], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V4] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V4),strLeafNode[_u8LanguageArrayIndex][SID_V4], "",  "%f", (int16_t)-25, (int16_t)300.0,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R5] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I5),strLeafNode[_u8LanguageArrayIndex][SID_R5], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V5] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V5),strLeafNode[_u8LanguageArrayIndex][SID_V5], "",  "%f", (int16_t)-25, (int16_t)300.0,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R6] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I6),strLeafNode[_u8LanguageArrayIndex][SID_R6], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V6] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V6),strLeafNode[_u8LanguageArrayIndex][SID_V6], "",  "%f", (int16_t)-25, (int16_t)300.0,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R7] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I7),strLeafNode[_u8LanguageArrayIndex][SID_R7], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V7] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V7),strLeafNode[_u8LanguageArrayIndex][SID_V7], "",  "%f", (int16_t)-25, (int16_t)300.0,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R8] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I8),strLeafNode[_u8LanguageArrayIndex][SID_R8], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V8] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V8),strLeafNode[_u8LanguageArrayIndex][SID_V8], "",  "%f", (int16_t)-25, (int16_t)300.0,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R9] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I9),strLeafNode[_u8LanguageArrayIndex][SID_R9], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V9] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V9),strLeafNode[_u8LanguageArrayIndex][SID_V9], "",  "%f", (int16_t)-25, (int16_t)300.0,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_R10] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_I10),strLeafNode[_u8LanguageArrayIndex][SID_R10], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S1_SENS_V10] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S1_CALIB_V10),strLeafNode[_u8LanguageArrayIndex][SID_V10], "",  "%f", (int16_t)-25, (int16_t)300.0,  CEditableItem::PIN1_ALLOWED );
    }
  //  ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_SENSOR_SELECTION),strLeafNode[_u8LanguageArrayIndex][SID_SENSOR_SELECTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
  //  ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
//    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
//    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY],arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
//    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_THRESHOLD_TYPE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_THRESHOLD_TYPE),strLeafNode[_u8LanguageArrayIndex][SID_THRESHOLD_TYPE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_THRESHOLD_TYPE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SHUTDOWN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_SHUT_DN_EN),strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_SHUTDOWN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN_THRESHOLD], "", "%f", (float)0, (float)1000,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
//    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_OPEN_CKT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_OPEN_CKT_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_WARNING_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_WARNING_THRESHOLD], "", "%f", (float)0, (float)1000,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
 //   ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_CKT_FAULT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_R1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_I1), strLeafNode[_u8LanguageArrayIndex][SID_R1],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_V1), strLeafNode[_u8LanguageArrayIndex][SID_V1], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_R2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_I2), strLeafNode[_u8LanguageArrayIndex][SID_R2],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_V2), strLeafNode[_u8LanguageArrayIndex][SID_V2], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_R3] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_I3), strLeafNode[_u8LanguageArrayIndex][SID_R3],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_V3), strLeafNode[_u8LanguageArrayIndex][SID_V3], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_R4] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_I4), strLeafNode[_u8LanguageArrayIndex][SID_R4],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_V4), strLeafNode[_u8LanguageArrayIndex][SID_V4], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_R5] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_I5), strLeafNode[_u8LanguageArrayIndex][SID_R5],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_V5), strLeafNode[_u8LanguageArrayIndex][SID_V5], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_R6] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_I6), strLeafNode[_u8LanguageArrayIndex][SID_R6],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_V6), strLeafNode[_u8LanguageArrayIndex][SID_V6], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_R7] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_I7), strLeafNode[_u8LanguageArrayIndex][SID_R7],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_V7), strLeafNode[_u8LanguageArrayIndex][SID_V7], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_R8] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_I8), strLeafNode[_u8LanguageArrayIndex][SID_R8],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_V8), strLeafNode[_u8LanguageArrayIndex][SID_V8], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_R9] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_I9), strLeafNode[_u8LanguageArrayIndex][SID_R9],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_V9), strLeafNode[_u8LanguageArrayIndex][SID_V9], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_R10] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_I10), strLeafNode[_u8LanguageArrayIndex][SID_R10],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SENS_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_CALIB_V10), strLeafNode[_u8LanguageArrayIndex][SID_V10], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_SENSOR_SELECTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O_SENS_SEL], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_O_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strInputSources[_u8LanguageArrayIndex], 26, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_O_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_O_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_O_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION],  "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_O_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_THRESHOLD_TYPE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_THRESH_TYPE), strLeafNode[_u8LanguageArrayIndex][SID_THRESHOLD_TYPE],"", "%s", strOptions[_u8LanguageArrayIndex][ID_THRESHOLD_TYPE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_SHUT_DN_EN), strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_SHUTDOWN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN_THRESHOLD], "", "%f", (float)0, (float)1000,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_WARNING_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_WARN_EN), strLeafNode[_u8LanguageArrayIndex][SID_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_WARNING_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_WARNING_THRESHOLD], "", "%f", (float)0, (float)1000,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_CKT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_FAULT_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_CKT_FAULT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
   if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
   {
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I1_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I1), strLeafNode[_u8LanguageArrayIndex][SID_I1_V1], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V1), strLeafNode[_u8LanguageArrayIndex][SID_VAL1], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I2_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I2), strLeafNode[_u8LanguageArrayIndex][SID_I2_V2], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V2), strLeafNode[_u8LanguageArrayIndex][SID_VAL2], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I3_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I3), strLeafNode[_u8LanguageArrayIndex][SID_I3_V3], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V3), strLeafNode[_u8LanguageArrayIndex][SID_VAL3], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I4_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I4), strLeafNode[_u8LanguageArrayIndex][SID_I4_V4], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V4), strLeafNode[_u8LanguageArrayIndex][SID_VAL4], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I5_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I5), strLeafNode[_u8LanguageArrayIndex][SID_I5_V5], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V5), strLeafNode[_u8LanguageArrayIndex][SID_VAL5], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I6_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I6), strLeafNode[_u8LanguageArrayIndex][SID_I6_V6], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V6), strLeafNode[_u8LanguageArrayIndex][SID_VAL6], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I7_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I7), strLeafNode[_u8LanguageArrayIndex][SID_I7_V7], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V7), strLeafNode[_u8LanguageArrayIndex][SID_VAL7], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I8_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I8), strLeafNode[_u8LanguageArrayIndex][SID_I8_V8], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V8), strLeafNode[_u8LanguageArrayIndex][SID_VAL8], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I9_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I9), strLeafNode[_u8LanguageArrayIndex][SID_I9_V9], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V9), strLeafNode[_u8LanguageArrayIndex][SID_VAL9], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I10_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I10), strLeafNode[_u8LanguageArrayIndex][SID_I10_V10], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V10), strLeafNode[_u8LanguageArrayIndex][SID_VAL10], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
   }
   else
   {
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I1_V1] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I1), strLeafNode[_u8LanguageArrayIndex][SID_I1_V1], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V1), strLeafNode[_u8LanguageArrayIndex][SID_VAL1], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I2_V2] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I2), strLeafNode[_u8LanguageArrayIndex][SID_I2_V2], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V2), strLeafNode[_u8LanguageArrayIndex][SID_VAL2], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I3_V3] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I3), strLeafNode[_u8LanguageArrayIndex][SID_I3_V3], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V3), strLeafNode[_u8LanguageArrayIndex][SID_VAL3], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I4_V4] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I4), strLeafNode[_u8LanguageArrayIndex][SID_I4_V4], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V4), strLeafNode[_u8LanguageArrayIndex][SID_VAL4], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I5_V5] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I5), strLeafNode[_u8LanguageArrayIndex][SID_I5_V5], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V5), strLeafNode[_u8LanguageArrayIndex][SID_VAL5], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I6_V6] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I6), strLeafNode[_u8LanguageArrayIndex][SID_I6_V6], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V6), strLeafNode[_u8LanguageArrayIndex][SID_VAL6], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I7_V7] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I7), strLeafNode[_u8LanguageArrayIndex][SID_I7_V7], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V7), strLeafNode[_u8LanguageArrayIndex][SID_VAL7], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I8_V8] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I8), strLeafNode[_u8LanguageArrayIndex][SID_I8_V8], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V8), strLeafNode[_u8LanguageArrayIndex][SID_VAL8], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I9_V9] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I9), strLeafNode[_u8LanguageArrayIndex][SID_I9_V9], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V9), strLeafNode[_u8LanguageArrayIndex][SID_VAL9], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_I10_V10] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_I10), strLeafNode[_u8LanguageArrayIndex][SID_I10_V10], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
       ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S3_CALIB_V10), strLeafNode[_u8LanguageArrayIndex][SID_VAL10], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
   }
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_SENSOR_SELECTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P_SENS_SEL], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_P_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strInputSources[_u8LanguageArrayIndex], 26, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_P_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_P_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_P_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_P_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_THRESHOLD_TYPE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_THRESH_TYPE), strLeafNode[_u8LanguageArrayIndex][SID_THRESHOLD_TYPE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_THRESHOLD_TYPE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_SHUTDOWN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_SHUT_DN_EN), strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN],"", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_SHUTDOWN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN_THRESHOLD], "", "%f", (float)0, (float)1000,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_WARNING_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_WARN_EN), strLeafNode[_u8LanguageArrayIndex][SID_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_WARNING_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_WARNING_THRESHOLD], "", "%f", (float)0, (float)1000,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_CKT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_FAULT_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_CKT_FAULT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I1_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I1), strLeafNode[_u8LanguageArrayIndex][SID_I1_V1], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V1), strLeafNode[_u8LanguageArrayIndex][SID_VAL1], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I2_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I2), strLeafNode[_u8LanguageArrayIndex][SID_I2_V2], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V2), strLeafNode[_u8LanguageArrayIndex][SID_VAL2], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I3_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I3), strLeafNode[_u8LanguageArrayIndex][SID_I3_V3], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V3), strLeafNode[_u8LanguageArrayIndex][SID_VAL3], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I4_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I4), strLeafNode[_u8LanguageArrayIndex][SID_I4_V4], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V4), strLeafNode[_u8LanguageArrayIndex][SID_VAL4], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I5_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I5), strLeafNode[_u8LanguageArrayIndex][SID_I5_V5], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V5), strLeafNode[_u8LanguageArrayIndex][SID_VAL5], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I6_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I6), strLeafNode[_u8LanguageArrayIndex][SID_I6_V6], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V6), strLeafNode[_u8LanguageArrayIndex][SID_VAL6], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I7_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I7), strLeafNode[_u8LanguageArrayIndex][SID_I7_V7], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V7), strLeafNode[_u8LanguageArrayIndex][SID_VAL7], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I8_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I8), strLeafNode[_u8LanguageArrayIndex][SID_I8_V8], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V8), strLeafNode[_u8LanguageArrayIndex][SID_VAL8], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I9_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I9), strLeafNode[_u8LanguageArrayIndex][SID_I9_V9], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V9), strLeafNode[_u8LanguageArrayIndex][SID_VAL9], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I10_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I10), strLeafNode[_u8LanguageArrayIndex][SID_I10_V10], "",  "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V10), strLeafNode[_u8LanguageArrayIndex][SID_VAL10], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    }
    else
    {
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I1_V1] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I1), strLeafNode[_u8LanguageArrayIndex][SID_I1_V1], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V1), strLeafNode[_u8LanguageArrayIndex][SID_VAL1], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I2_V2] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I2), strLeafNode[_u8LanguageArrayIndex][SID_I2_V2], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V2), strLeafNode[_u8LanguageArrayIndex][SID_VAL2], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I3_V3] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I3), strLeafNode[_u8LanguageArrayIndex][SID_I3_V3], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V3), strLeafNode[_u8LanguageArrayIndex][SID_VAL3], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I4_V4] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I4), strLeafNode[_u8LanguageArrayIndex][SID_I4_V4], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V4), strLeafNode[_u8LanguageArrayIndex][SID_VAL4], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I5_V5] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I5), strLeafNode[_u8LanguageArrayIndex][SID_I5_V5], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V5), strLeafNode[_u8LanguageArrayIndex][SID_VAL5], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I6_V6] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I6), strLeafNode[_u8LanguageArrayIndex][SID_I6_V6], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V6), strLeafNode[_u8LanguageArrayIndex][SID_VAL6], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I7_V7] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I7), strLeafNode[_u8LanguageArrayIndex][SID_I7_V7], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V7), strLeafNode[_u8LanguageArrayIndex][SID_VAL7], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I8_V8] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I8), strLeafNode[_u8LanguageArrayIndex][SID_I8_V8], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V8), strLeafNode[_u8LanguageArrayIndex][SID_VAL8], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I9_V9] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I9), strLeafNode[_u8LanguageArrayIndex][SID_I9_V9], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V9), strLeafNode[_u8LanguageArrayIndex][SID_VAL9], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_I10_V10] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_I10), strLeafNode[_u8LanguageArrayIndex][SID_I10_V10], "",  "%f", (uint8_t)4, (uint8_t)20,  CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S4_CALIB_V10), strLeafNode[_u8LanguageArrayIndex][SID_VAL10], "",  "%f", (float)0, (float)1000.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
        }
  /*  ArrEditableItem[INDEX_OF_OUT_A_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_A_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_A_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_A_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_B_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_B_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_B_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_B_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_C_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_C_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_C_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_C_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_D_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_D_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_D_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_D_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_E_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_E_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_E_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_E_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_F_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_F_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_F_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_F_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_G_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_G_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_G_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_G_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
*/
    ArrEditableItem[INDEX_OF_TMR_OF_CRANKING] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_HOLD_TIME), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_HOLD_TIME], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)15, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_CRANK_REST] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_REST_TIME), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_REST_TIME], arrUnit[ID_SEC], "%u", (uint16_t)2, (uint16_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_MANUAL_START] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MANUAL_CRANK_START_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_MANUAL_START_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_AUTO_START] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUTO_CRANK_START_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_AUTO_START_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)43200, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_TMR_OF_SAFETY_MON] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SAFETY_MON_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SAFETY_MONITORING_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)10, (uint16_t)60, CEditableItem::PIN1_ALLOWED );
    //ArrEditableItem[INDEX_OF_TMR_OF_MAINS_DETECT] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_DETECT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_MAINS_DETECT_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    //ArrEditableItem[INDEX_OF_TMR_OF_ALT_DETECT] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_DETECT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ALT_DETECT_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)30, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_WARM_UP] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_WARMUP_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_WARM_UP_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_RETURN_TO_MAINS] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_RETN_TO_MAINS_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)600, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_COOLING] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_COOL_DELAY),  strLeafNode[_u8LanguageArrayIndex][SID_ENG_COOL_TIME], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_STOP_ACTION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_STOP_ACTION_TIME), strLeafNode[_u8LanguageArrayIndex][SID_STOP_ACTION_TIME], arrUnit[ID_SEC], "%u", (uint16_t)10, (uint16_t)120, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_ADDITIONAL_STOP] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ADDN_STOP_TIME), strLeafNode[_u8LanguageArrayIndex][SID_ADDN_STOPPING_TIME], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)120, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_TRANSFER] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_TRANS_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_TRANSFER_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)60, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_TMR_OF_POWER_SAVE] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_POWER_SAVE_MODE_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_PWR_SAVE_MODE_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)1800, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_PAGE] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SCREEN_CHANG_TIME), strLeafNode[_u8LanguageArrayIndex][SID_SCRN_CHNGOVER_TIME], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)1800, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_DEEP_SLEEP] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DEEP_SLEEP_DELAY),strLeafNode[_u8LanguageArrayIndex][SID_DEEP_SLP_MODE_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)1800, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_AUDIBLE_ALARM] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SOUNDER_ALARM_TIME),strLeafNode[_u8LanguageArrayIndex][SID_SOUNDER_ALARM_TIMER], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_TEST_MODE] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_TEST_MODE_TIMER_MIN),strLeafNode[_u8LanguageArrayIndex][SID_TEST_MODE_TIMER], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_AUTO_EXIT_TIMER] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUTO_EXIT_TIME),strLeafNode[_u8LanguageArrayIndex][SID_AUTO_EXIT_CNFG_MODE], arrUnit[ID_SEC], "%u", (uint16_t)10, (uint16_t)1800, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_ISV_PULL_SIGNAL_TIMER] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ISV_PULL_SIGNAL_TIME),strLeafNode[_u8LanguageArrayIndex][SID_ISV_PULL_SIGNAL_TIMER], arrUnit[ID_SEC], "%f", (float)0.1, (float)2.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_GEN_BREAKER] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_BREAKER_PULS_TIMER),strLeafNode[_u8LanguageArrayIndex][SID_GEN_BREAKER_TIMER], arrUnit[ID_SEC], "%f", (float)0.1, (float)5.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_MAINS_BREAKER] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_BREAKER_PULS_TIMER),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_BREAKER_TIMER], arrUnit[ID_SEC], "%f", (float)0.1, (float)5.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_BREAKER_FEEDBACK] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BREAKER_FEEDBACK_TIMER),strLeafNode[_u8LanguageArrayIndex][SID_BREAKER_FEEDBACK_TIMER], arrUnit[ID_SEC], "%f", (float)1, (float)10.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_TMR_OF_BREAKER_CLOSE] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BREAKER_CLOSE_DELAY),strLeafNode[_u8LanguageArrayIndex][SID_BREAKER_CLOSE_DELAY], arrUnit[ID_SEC], "%f", (float)0.1, (float)30.0, (float)0.1,CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_ALT_PRESENT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT),strLeafNode[_u8LanguageArrayIndex][SID_ALT_PRESENT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_POLES] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NUMBER_OF_POLES),strLeafNode[_u8LanguageArrayIndex][SID_NUMBER_OF_POLES], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ALT_POLES], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_SYS] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE),strLeafNode[_u8LanguageArrayIndex][SID_ALT_AC_SYSTEM], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_NO_OF_PHASES], 3, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_MIN_HEALTHY_VOLT] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MIN_HEALTHY_VTG), strLeafNode[_u8LanguageArrayIndex][SID_MIN_HEALTHY_VOLT], arrUnit[ID_V_PH_N], "%f", (uint16_t)50, (uint16_t)350, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_MIN_HEALTHY_FREQ] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MIN_HEALTHY_FREQ), strLeafNode[_u8LanguageArrayIndex][SID_MIN_HEALTHY_FREQ], arrUnit[ID_HZ], "%u", (uint8_t)10, (uint8_t)75, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DG_PHASE_ROT_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PH_REVERS_DETECT_EN),strLeafNode[_u8LanguageArrayIndex][SID_PHASE_REVERSAL_DETECT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DG_PHASE_ROT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PH_REVERS_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_PHASE_REVERSAL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUTO_LOAD_TRANSFER] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUTOLOAD_TRANSFER),strLeafNode[_u8LanguageArrayIndex][SID_AUTO_LOAD_TRANSFER], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_WAVEFORM_DETECT_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_WAVE_DETECT_EN),strLeafNode[_u8LanguageArrayIndex][SID_WAVEFORM_DETECT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GEN_PT_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_PT_ENABLE),strLeafNode[_u8LanguageArrayIndex][SID_GEN_PT_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GEN_PT_PRIMARY_RATIO] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_PT_PRIMARY_RATIO),strLeafNode[_u8LanguageArrayIndex][SID_GEN_PT_PRIMARY_RATIO], "", "%u", (uint16_t)100, (uint16_t)25000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GEN_PT_SECONDARY_RATIO] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_PT_SECONDARY_RATIO),strLeafNode[_u8LanguageArrayIndex][SID_GEN_PT_SECONDARY_RATIO], "", "%u", (uint16_t)100, (uint16_t)700, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_UV_SHUT_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_SHUT_DN_EN),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_VOLT_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_UV_SHUT_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_SHUTDN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_UV_SHUTDOWN_THRESH], arrUnit[ID_V_PH_N], "%u", (uint16_t)50, (uint16_t)295, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_UV_SHUT_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UV_SHUTDOWN_DELAY_SEC),strLeafNode[_u8LanguageArrayIndex][SID_UV_SHUTDOWN_DELAY], arrUnit[ID_SEC], "%f", (float)0.1, (float)100.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_UV_WARN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_WARN_EN),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_VOLT_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_UV_WARN_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_WARN_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_UV_WARNING_THRESHOLD], arrUnit[ID_V_PH_N], "%u", (uint16_t)55, (uint16_t)300, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_UV_WARN_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UV_WARNING_DELAY_SEC),strLeafNode[_u8LanguageArrayIndex][SID_UV_WARNING_DELAY], arrUnit[ID_SEC], "%f", (float)0.1, (float)100.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OV_SHUT_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_SHUT_DN_EN),strLeafNode[_u8LanguageArrayIndex][SID_OVER_VOLT_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OV_SHUT_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_SHUTDN_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_OV_SHUTDOWN_THRESH], arrUnit[ID_V_PH_N], "%u", (uint16_t)105, (uint16_t)350, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OV_SHUT_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OV_SHUTDOWN_DELAY_SEC),strLeafNode[_u8LanguageArrayIndex][SID_OV_SHUTDOWN_DELAY], arrUnit[ID_SEC], "%f", (float)0.1, (float)100.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OV_WARN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_WARN_EN),strLeafNode[_u8LanguageArrayIndex][SID_OVER_VOLT_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OV_WARN_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_WARN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_OV_WARNING_THRESHOLD], arrUnit[ID_V_PH_N], "%u", (uint16_t)100, (uint16_t)345, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OV_WARN_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OV_WARNING_DELAY_SEC),strLeafNode[_u8LanguageArrayIndex][SID_OV_WARNING_DELAY], arrUnit[ID_SEC], "%f", (float)0.1, (float)100.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_UF_SHUT_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_FREQ_SHUT_DN_EN),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_FREQ_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_UF_SHUT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_FREQ_SHUT_DN_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_UF_SHUTDOWN_THRESH], arrUnit[ID_HZ], "%f", (float)10.0, (float)59.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_UF_SHUT_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UF_SHUTDOWN_DELAY_SEC),strLeafNode[_u8LanguageArrayIndex][SID_UF_SHUTDOWN_DELAY], arrUnit[ID_SEC], "%f", (float)0.1, (float)100.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_UF_WARN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_FREQ_WARN_EN),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_FREQ_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_UF_WARN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_FREQ_WARN_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_UF_WARNING_THRESHOLD], arrUnit[ID_HZ], "%f", (float)11.0, (float)60.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_UF_WARN_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_UF_WARNING_DELAY_SEC),strLeafNode[_u8LanguageArrayIndex][SID_UF_WARNING_DELAY], arrUnit[ID_SEC], "%f", (float)0.1, (float)100.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OF_SHUT_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_FREQ_SHUT_DN_EN),strLeafNode[_u8LanguageArrayIndex][SID_OVER_FREQ_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OF_SHUT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_FREQ_SHUT_DN_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_OF_SHUTDOWN_THRESH], arrUnit[ID_HZ], "%f", (float)26.0, (float)75.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OF_SHUT_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OF_SHUTDOWN_DELAY_SEC),strLeafNode[_u8LanguageArrayIndex][SID_OF_SHUTDOWN_DELAY], arrUnit[ID_SEC], "%f", (float)0.1, (float)100.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OF_WARN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_FREQ_WARN_EN),strLeafNode[_u8LanguageArrayIndex][SID_OVER_FREQ_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OF_WARN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_FREQ_WARN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_OF_WARNING_THRESHOLD], arrUnit[ID_HZ], "%f", (float)25.0, (float)74.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OF_WARN_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OF_WARNING_DELAY_SEC),strLeafNode[_u8LanguageArrayIndex][SID_OF_WARNING_DELAY], arrUnit[ID_SEC], "%f", (float)0.1, (float)100.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_CM_CT_RATIO] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CM_CT_RATIO),strLeafNode[_u8LanguageArrayIndex][SID_LOAD_CT_RATIO], "/5", "%u", (uint16_t)0, (uint16_t)8000, CEditableItem::PIN1_PIN2_ALLOWED);
    ArrEditableItem[INDEX_OF_CM_OC_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_CURR_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_OVER_CURR_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CM_OC_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_CURR_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_OVER_CURR_THRESHOLD], "A", "%u", (uint16_t)5, (uint16_t)10000, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CM_OC_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_CURR_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_OVER_CURR_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)600, CEditableItem::PIN1_PIN2_ALLOWED );
   // ArrEditableItem[INDEX_OF_CURR_MON_RESERVED] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CT_CORRECTION_FACTOR), strLeafNode[_u8LanguageArrayIndex][SID_RESERVED],"", "%f", (float)1.0, (float)800.0,(float)0.1, CEditableItem::NOT_ALLOWED );
    ArrEditableItem[INDEX_OF_CT_LOCATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CT_LOCATION),strLeafNode[_u8LanguageArrayIndex][SID_CT_LOCATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_CT_LOCATION], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_EFM_CURRENT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CM_SELECTION),strLeafNode[_u8LanguageArrayIndex][SID_EARTH_FAN_CURR_MON], "", "%s", strOptions[_u8LanguageArrayIndex][ID_EARTH_FAN_CM], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EFM_CT_RATIO] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FM_CT_RATIO),strLeafNode[_u8LanguageArrayIndex][SID_EFM_CT_RATIO], "/5", "%u", (uint16_t)0, (uint16_t)8000, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EFM_HIGH_CURR_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CURR_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_EFM_HIGH_CURR_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EFM_HIGH_CURR_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CURR_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_EFM_HIGH_CURR_THRESH], "A", "%f", (float)0, (float)10.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EFM_CURR_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURR_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_EFM_CURR_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)180, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EFM_LOW_CURR_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOW_CURR_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_EFM_LOW_CURR_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_EFM_LOW_CURR_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOW_CURR_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_EFM_LOW_CURR_THRESH], "A", "%f", (float)0, (float)10.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_LM_RATING] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_RATING), strLeafNode[_u8LanguageArrayIndex][SID_GEN_RATING], arrUnit[ID_KW], "%u", (uint16_t)0, (uint16_t)8000, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LM_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OL_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_OVERLOAD_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LM_THRESHOLD] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OL_THRESH_PERCENT), strLeafNode[_u8LanguageArrayIndex][SID_OVERLOAD_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint8_t)50, (uint8_t)150, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LM_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_LOAD_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_OVERLOAD_MON_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)600, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LM_UNBALANCE_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNBAL_LOAD_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_UNBAL_LOAD_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LM_UNBALANCE_TH] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNBAL_LOAD_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_UNBAL_LOAD_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint8_t)5, (uint8_t)200, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LM_UNBALANCE_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNBAL_LOAD_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_UNBAL_LOAD_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)600, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_LOW_LOAD_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_ALARM_EN),strLeafNode[_u8LanguageArrayIndex][SID_LOW_LOAD_EN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOW_LOAD_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_LOW_LOAD_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOW_LOAD_TRIP] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_TRIP),strLeafNode[_u8LanguageArrayIndex][SID_LOW_LOAD_TRIP], arrUnit[ID_PERCENT], "%u", (uint8_t)1, (uint8_t)99, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOW_LOAD_RETURN] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_RETURN),strLeafNode[_u8LanguageArrayIndex][SID_LOW_LOAD_RETURN], arrUnit[ID_PERCENT], "%u", (uint8_t)2, (uint8_t)100, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOW_LOAD_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_DELAY_SEC), strLeafNode[_u8LanguageArrayIndex][SID_LOW_LOAD_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)3600, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_MAINS_MON_ON] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_MONITORING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_SYSTEM] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_AC_SYSTEM], "", "%s", strOptions[_u8LanguageArrayIndex][ID_NO_OF_PHASES], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PH_REVERS_DETECT_EN),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_PHASE_REVERSAL_DETECT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PH_REVERS_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_PHASE_REVERSAL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MAINS_PHASE_REVERSE_ACTION], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PARTIAL_HEALTHY_DETECT_EN),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_PARTIAL_HEALTHY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_PT_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PT_ENABLE),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_PT_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_PT_PRIMARY_RATIO] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PT_PRIMARY_RATIO),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_PT_PRIMARY_RATIO], "", "%u", (uint16_t)100, (uint16_t)25000, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_PT_SECONDARY_RATIO] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PT_SECONDARY_RATIO),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_PT_SECONDARY_RATIO], "", "%u", (uint16_t)100, (uint16_t)700, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_MAINS_UV_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_MON_EN),strLeafNode[_u8LanguageArrayIndex][SID_UV_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_UV_TRIP_THRESH] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_TRIP_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_UV_TRIP], arrUnit[ID_V_PH_N], "%u", (uint16_t)50, (uint16_t)298, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_UV_RETURN_THRESH] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_RET_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_UV_RETURN], arrUnit[ID_V_PH_N], "%u", (uint16_t)52, (uint16_t)300, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_OV_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_MON_EN),strLeafNode[_u8LanguageArrayIndex][SID_OV_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_OV_TRIP_THRESH] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_TRIP_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_OV_TRIP], arrUnit[ID_V_PH_N], "%u", (uint16_t)102, (uint16_t)350, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_OV_RETURN_THRESH] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_RET_THRESH       ), strLeafNode[_u8LanguageArrayIndex][SID_OV_RETURN], arrUnit[ID_V_PH_N], "%u", (uint16_t)100, (uint16_t)348, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_VOLT_TRIP_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_VOLT_TRIP_DELAY_SEC),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_VOLT_TRIP_DELAY], arrUnit[ID_SEC], "%f", (float)0.1, (float)100.0, (float)0.1,CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_MAINS_UF_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_MON_EN),strLeafNode[_u8LanguageArrayIndex][SID_UF_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_UF_TRIP_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_TRIP_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_UF_TRIP], arrUnit[ID_HZ], "%f", (float)10.0, (float)59.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_UF_RETURN_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_RET_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_UF_RETURN], arrUnit[ID_HZ], "%f", (float)11.0, (float)60.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_OF_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_MON_EN),strLeafNode[_u8LanguageArrayIndex][SID_OF_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_OF_TRIP_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_TRIP_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_OF_TRIP], arrUnit[ID_HZ], "%f", (float)26.0, (float)75.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_OF_RETURN_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_RET_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_OF_RETURN], arrUnit[ID_HZ], "%f", (float)25.0, (float)74.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_FREQ_TRIP_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_FREQ_TRIP_DELAY_SEC),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_FREQ_TRIP_DELAY], arrUnit[ID_SEC], "%f", (float)0.1, (float)100.0, (float)0.1,CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_START_ATTEMPTS] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_ATTEMPTS), strLeafNode[_u8LanguageArrayIndex][SID_START_ATTEMPTS], "", "%u", (uint8_t)1, (uint8_t)9, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_START_USE_LOP] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISCON_ON_LOP_SENS_EN),strLeafNode[_u8LanguageArrayIndex][SID_DISCONN_ON_LOP_SENS], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_START_LOP_AT_CRANK] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MONITOR_LOP_SENSE_BEFORE_CRANK), strLeafNode[_u8LanguageArrayIndex][SID_MON_LOP_BEF_CRANK], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_START_LOP_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISCONNECT_PRESURE_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_DISCONN_LOP_SENS], "Bar", "%f", (float)0.5, (float)10.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_START_LLOP_AT_CRANK] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MONITOR_LLOP_BEFORE_CRANK), strLeafNode[_u8LanguageArrayIndex][SID_MON_LLOP_BEF_CRANK], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_START_USE_LLOP] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISCON_ON_LOP_SW_EN),strLeafNode[_u8LanguageArrayIndex][SID_DISCONN_ON_LLOP_SW], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_START_LLOP_DELAY] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_SW_TRANSIENT_TIME), strLeafNode[_u8LanguageArrayIndex][SID_LLOP_SW_TRANS_TIME], arrUnit[ID_SEC], "%f", (float)0, (float)3.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_START_DISCONN_FREQ] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCON_ALT_FREQ_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_ALT_FREQUENCY], arrUnit[ID_HZ], "%u", (uint8_t)10, (uint8_t)70, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_START_DISCONN_RPM] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCON_ENG_SPEED_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_ENGINE_SPEED], arrUnit[ID_RPM], "%u", (uint16_t)150, (uint16_t)4000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_START_USE_CA_VOLT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCON_CHARG_ALT_EN),strLeafNode[_u8LanguageArrayIndex][SID_DISC_ON_CHG_ALT_VOLT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_START_CA_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCON_CHARG_ALT_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_CHG_ALT_THRESHOLD], arrUnit[ID_V], "%f", (float)5.0, (float)30.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SPEED_SENSE_SOURCE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_SPEED_SENSE_SOURCE], 3, CEditableItem::PIN1_PIN2_PIN3_ALLOWED );
    ArrEditableItem[INDEX_OF_MPU_TEETH_W_PT_FREQ] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MPU_TEETH_W_POINT_FREQ), strLeafNode[_u8LanguageArrayIndex][SID_FLYWHL_TH_W_PT_FREQ], "", "%u", (uint16_t)1, (uint16_t)300, CEditableItem::PIN1_PIN2_PIN3_ALLOWED );
    ArrEditableItem[INDEX_OF_US_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDERSPEED_SHUT_DN_EN),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_SPEED_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_US_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_SPEED_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_UNDER_SPEED_THRESH], arrUnit[ID_RPM], "%u", (uint16_t)0, (uint16_t)3600, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_US_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_SPEED_DELAY       ),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_SPEED_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OS_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_SPEED_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_OVER_SPEED_THRESH], arrUnit[ID_RPM], "%u", (uint16_t)700, (uint16_t)4000, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_OS_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_SPEED_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_OVER_SPEED_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)20, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_GROSS_OS_THRESHOLD] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GROSS_OVER_SPEED_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_GROSS_OS_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint8_t)100, (uint8_t)200, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_IDLE_RATED_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_IDLE_TO_RATED_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_IDLE_TO_RATED_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)1200, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_STARTUP_IDLE_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_STARTUP_IDLE_TIME), strLeafNode[_u8LanguageArrayIndex][SID_STARTUP_IDLE_TIME], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)1200, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_STOPPING_IDLE_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_STOPPING_IDLE_TIME), strLeafNode[_u8LanguageArrayIndex][SID_STOPPING_IDLE_TIME], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)1200, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_IDLE_MODE_PULSE_TIME] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_IDLE_MODE_PULSE_TIME), strLeafNode[_u8LanguageArrayIndex][SID_IDLE_MODE_PULSE_TIME], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_EGOV_INIT_LOW_SPEED] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_INITIAL_LOW_SPEED), strLeafNode[_u8LanguageArrayIndex][SID_INIT_LOW_SPEED], arrUnit[ID_RPM], "%u", (uint16_t)500, (uint16_t)1800, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_LOW_VBAT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOW_BAT_VTG_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_LOW_VOLT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOW_VBAT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOW_BAT_VTG_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_LOW_VOLT_THRESHOLD], arrUnit[ID_V], "%f", (float)8.0, (float)31.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOW_VBAT_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOW_BAT_VTG_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_LOW_VOLT_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)1800, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_HIGH_VBAT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_BAT_VTG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_HIGH_VOLT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_HIGH_VBAT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_BAT_VTG_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_HIGH_VOLT_THRESHOLD], arrUnit[ID_V], "%f", (float)9.0, (float)32.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_HIGH_VBAT_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_BAT_VTG_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_HIGH_VOLT_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)1800, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_CA_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CHARG_ALT_FAIL_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_CA_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CA_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CHARG_ALT_FAIL_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_FAIL_THRESHOLD], arrUnit[ID_V], "%f", (float)0.0, (float)35.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CA_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CHARG_ALT_FAIL_DELAY),strLeafNode[_u8LanguageArrayIndex][SID_FAIL_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_PH_TIMER] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_TIMER),strLeafNode[_u8LanguageArrayIndex][SID_PREHEAT_TIMER], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)900, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_PH_TEMP_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TEMP_LIMIT_EN),strLeafNode[_u8LanguageArrayIndex][SID_ENG_CLNT_TEMP_EN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_PH_TEMP_VALUE] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TEMP_LIMIT_THREH),strLeafNode[_u8LanguageArrayIndex][SID_ENGINE_CLNT_TEMP_LIMIT], arrUnit[ID_DEG_C], "%u", (uint16_t)10, (uint16_t)300, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_CLNT_TEMP_CTRL_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_CTRL_EN),strLeafNode[_u8LanguageArrayIndex][SID_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CLNT_TEMP_CTRL] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_CTRL),strLeafNode[_u8LanguageArrayIndex][SID_TEMP_CTRL], "", "%s", strOptions[_u8LanguageArrayIndex][ID_CLNT_TEMP_CTRL_OPT], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CLNT_TEMP_ON_THRESH] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_ON_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_ON_THRESHOLD], arrUnit[ID_DEG_C], "%u", (uint16_t)0, (uint16_t)250, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CLNT_TEMP_OFF_THRESH] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_OFF_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_OFF_THRESHOLD], arrUnit[ID_DEG_C], "%u", (uint16_t)0, (uint16_t)250, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_AFT_ACTIVATION_THRESH] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AFT_ACTIVATION_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_AFT_ACT_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint8_t)0, (uint8_t)99, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AFT_DEACTIVATION_THRESH] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AFT_DEACTIVATION_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_AFT_DEACT_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint8_t)1, (uint8_t)100, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AFT_TIMEOUT_AFTER_ACTIVATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_TIMEOUT_AFTER_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_TIMEOUT_AFTER_ACT], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)1200, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AFT_MONITOR_ON_ENGINE_ON] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MON_AFT_FROM_ENGINE_ON),strLeafNode[_u8LanguageArrayIndex][SID_MON_AFT_ON_ENG_ON], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_ENGINE_TYPE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE),strLeafNode[_u8LanguageArrayIndex][SID_ENGINE_TYPE], "", "%s", StrEngineType[_u8LanguageArrayIndex], 21 , CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG),strLeafNode[_u8LanguageArrayIndex][SID_LOP_FROM_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG),strLeafNode[_u8LanguageArrayIndex][SID_CLNT_TEMP_FROM_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_SPEED_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_SPEED_FROM_ENG),strLeafNode[_u8LanguageArrayIndex][SID_ENG_SPEED_FROM_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_RUN_HOURS_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_RUNNING_HOURS_FROM_ENG),strLeafNode[_u8LanguageArrayIndex][SID_ENG_RUN_HRS_FROM_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_BATT_VTG_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_VOLT_FROM_ENG),strLeafNode[_u8LanguageArrayIndex][SID_BATT_VTG_FROM_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_ENG_SPEED_TO_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_TO_ECU),strLeafNode[_u8LanguageArrayIndex][SID_ENG_SPEED_TO_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_REQ_SPEED] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_REQUESTED_SPEED_TO_ECU), strLeafNode[_u8LanguageArrayIndex][SID_ENG_REQUESTED_SPEED], arrUnit[ID_RPM], "%u", (uint16_t)500, (uint16_t)4000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_START_STOP_ENG_CMD_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_START_STOP_TO_ECU),strLeafNode[_u8LanguageArrayIndex][SID_START_STOP_CMD_TO_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_PREHEAT_CMD_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_TO_ECU),strLeafNode[_u8LanguageArrayIndex][SID_PREHEAT_CMD_TO_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENGINE_FRQ_ECU] =  CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_FRQ),strLeafNode[_u8LanguageArrayIndex][SID_ENG_FREQ], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENG_FRQ_50HZ_60HZ], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENGINE_GAIN_ECU] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_GAIN),strLeafNode[_u8LanguageArrayIndex][SID_ENG_GAIN], "", "%f", (float)0.0, (float)10.0, (float)0.1,CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SGC_SOURCE_ADDRESS), strLeafNode[_u8LanguageArrayIndex][SID_SGC_SOURCE_ADDRESS], "", "%u", (uint16_t)0, (uint16_t)253, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ECU_SOURCE_ADDRESS] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_SOURCE_ADDRESS), strLeafNode[_u8LanguageArrayIndex][SID_ECU_SOURCE_ADDRESS], "", "%u", (uint16_t)0, (uint16_t)253, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_COMM_FAILURE_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_COMM_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_COMM_FAILURE_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_COMM_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_COM_FAIL_ACTIVATION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACT_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_COMM_FAILURE_ACT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_COMM_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)60, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_AMBER_LAMP_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_AMBER_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_AMBER_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_AMBER_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_AMBER_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_AMBER_ACT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_AMBER_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_RED_LAMP_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_RED_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_RED_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_RED_LAMP_ACT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_RED_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_RED_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_RED_LAMP_ACT_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_RED_ACT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_RED_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_MALFUNCTION_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_MALFUNC_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_MALFUNCTION_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_MALFUNC_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_MALFUNCTION_ACT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_MALFUNC_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_PROTECT_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_PROTECT_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_PROTECT_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_PROTECT_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_PROTECT_ACT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_PROTECT_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_LOP_SHUTDOWN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_LVL_SHUTDOWN_EN),strLeafNode[_u8LanguageArrayIndex][SID_LOW_LVL_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_LVL_SHUTDOWN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_LOW_SHUTDOWN_THRESHOLD], arrUnit[ID_BAR], "%f", (float)0.0, (float)9.8, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED);
    ArrEditableItem[INDEX_OF_LOP_WARNING_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_LVL_WARNING_EN),strLeafNode[_u8LanguageArrayIndex][SID_LOW_LVL_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_LVL_WARNING_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_LOW_WARNING_THRESHOLD], arrUnit[ID_BAR], "%f", (float)0.2, (float)10.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED);

    ArrEditableItem[INDEX_OF_COOLANT_TEMP_THRESH_TYPE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_THRESH_TYPE),strLeafNode[_u8LanguageArrayIndex][SID_CLNT_TEMP_THRESH_TYPE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_THRESHOLD_TYPE],2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ECT_SHUTDOWN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_SHUTDOWN_EN),strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ECT_SHUTDOWN_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CLNT_TEMP_SHUTDOWN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN_THRESHOLD], arrUnit[ID_DEG_C], "%u", (uint16_t)27, (uint16_t)300, CEditableItem::PIN1_PIN2_ALLOWED);
    ArrEditableItem[INDEX_OF_ECT_WARNING_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_WARNING_EN),strLeafNode[_u8LanguageArrayIndex][SID_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ECT_WARNING_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CLNT_TEMP_WARNING_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_WARNING_THRESHOLD], arrUnit[ID_DEG_C], "%u", (uint16_t)25, (uint16_t)298, CEditableItem::PIN1_PIN2_ALLOWED);

    ArrEditableItem[INDEX_OF_FILT_MAINT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINTENANCE_ALARM_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_FLIT_MAINTAINANCE_ACTION], 3, CEditableItem::PIN1_PIN2_PIN3_ALLOWED);
    ArrEditableItem[INDEX_OF_FILT_MAINT_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINTENANCE_DUE_HOURS), strLeafNode[_u8LanguageArrayIndex][SID_DUE_AT_ENGINE_HOURS], arrUnit[ID_HRS], "%u", (uint16_t)10, (uint16_t)65000, CEditableItem::PIN1_PIN2_PIN3_ALLOWED);

    ArrEditableItem[INDEX_OF_FILT_MAINT_ASH_LOAD_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINT_ASH_LOAD_EN),strLeafNode[_u8LanguageArrayIndex][SID_MAINT_DUE_ASH_LOAD_EN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED);

    _stMaintenanceDt ={(uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FILT_MAINT_THRESH_DAY),(uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FILT_MAINT_THRESH_MONTH),(uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FILT_MAINT_THRESH_YEAR)};
    ArrEditableItem[INDEX_OF_FILT_MAINT_DUE_DATE] = CEditableItem((CEditableItem::DATE_t)_stMaintenanceDt, "", "","%u",(CEditableItem::DATE_t){1,1,2017}, (CEditableItem::DATE_t){31,12,2075},CEditableItem::PIN1_PIN2_ALLOWED );
=======
>>>>>>> 2ba6ff6cf8a735594b10d9e5dfb4a128a8f5c654




    _stMaintenanceDt ={(uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_1),(uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_2),(uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_3)};
    _objcfgz.ReadMiscParam(&_MiscParam);

    if(_MiscParam.u8MiscParam[PROFILE_NO] > MAX_NUMBER_OF_PROFILE)
    {
        _MiscParam.u8MiscParam[PROFILE_NO] =0;
    }
    //_objHal.ObjDflash.Read(DFLASH_PASSWORD_START_ADDRESS , (uint8_t *)&u16PIN, 4);
    ArrEditableItem[INDEX_OF_ACTIVE_PROFILE] = CEditableItem((uint32_t)_MiscParam.u8MiscParam[PROFILE_NO],strLeafNode[_u8LanguageArrayIndex][SID_PROFILE_NAME], "", "%s",(const char**)_arrProfileNames, MAX_NUMBER_OF_PROFILE, CEditableItem::PIN1_PIN2_ALLOWED );

    stPIN_1 = {0,0,0,0};
    stPIN_2 = {0,0,0,0};

    stENG_SR_NO.u8EngSrNoChar1 = _MiscParam.u8EngId[ID_ENG_CHAR0];
    stENG_SR_NO.u8EngSrNoChar2 = _MiscParam.u8EngId[ID_ENG_CHAR1];
    stENG_SR_NO.u8EngSrNoChar3 = _MiscParam.u8EngId[ID_ENG_CHAR2];
    stENG_SR_NO.u8EngSrNoChar4 = _MiscParam.u8EngId[ID_ENG_CHAR3];
    stENG_SR_NO.u8EngSrNoChar5 = _MiscParam.u8EngId[ID_ENG_CHAR4];
    stENG_SR_NO.u8EngSrNoChar6 = _MiscParam.u8EngId[ID_ENG_CHAR5];
    stENG_SR_NO.u8EngSrNoChar7 = _MiscParam.u8EngId[ID_ENG_CHAR6];
    stENG_SR_NO.u8EngSrNoChar8 = _MiscParam.u8EngId[ID_ENG_CHAR7];
    stENG_SR_NO.u8EngSrNoChar9 = _MiscParam.u8EngId[ID_ENG_CHAR8];
    stENG_SR_NO.u8EngSrNoChar10 = _MiscParam.u8EngId[ID_ENG_CHAR9];
    stENG_SR_NO.u8EngSrNoChar11 = _MiscParam.u8EngId[ID_ENG_CHAR10];
    stENG_SR_NO.u8EngSrNoChar12 = _MiscParam.u8EngId[ID_ENG_CHAR11];

    ArrEditableItem[INDEX_OF_ENG_SR_NO]  = CEditableItem((CEditableItem::ENG_SR_NO_t)stENG_SR_NO,"", "", "%u", (CEditableItem::ENG_SR_NO_t){47,47,47,47,47,47,47,47,47,47,47,47}, (CEditableItem::ENG_SR_NO_t) {83,83,83,83,83,83,83,83,83,83,83,83}, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_PIN_1]  = CEditableItem((CEditableItem::PASSWORD_t)stPIN_1,"", "", "%u", (CEditableItem::PASSWORD_t){0,0,0,0}, (CEditableItem::PASSWORD_t){9,9,9,9}, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_PIN_2]  = CEditableItem((CEditableItem::PASSWORD_t)stPIN_2,"", "", "%u", (CEditableItem::PASSWORD_t){0,0,0,0},(CEditableItem::PASSWORD_t) {9,9,9,9}, CEditableItem::PIN1_PIN2_ALLOWED );

}

void UI::InitEditableItemsScreens()
{
//RushiStart
    //NewUI
        uint16_t k =  INDEX_OF_GENERAL_PROFILE_NAME;
        for(uint8_t i=0;i<sizeof(ParamInSubmenus);i++)
        {
            for(uint8_t j=0;j<ParamInSubmenus[i];j++)
            {
                ArrEditableItemScreen[k] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][i], &ArrEditableItem[k], 10, 20);
                k++;
            }
        }
    //End
    //RushiEnd

}
void UI::LowestLevelMenuEnDis(uint16_t u16StartIndex , uint16_t u16EndIndex , bool bEn_Ds)
{
    for(uint16_t i=u16StartIndex; i<=u16EndIndex ; i++)
    {
        menuItemsLowestLevel[i].isEnabled =bEn_Ds;
    }

}
void UI::InitMenuItemsAndMenus()
{
    //first, initialize the lowest level menu items:

          for(int i=INDEX_OF_GENERAL_PROFILE_NAME; i<=INDEX_OF_CAN_J1939_COMM_ACT_DELAY_PROTECT;i++)
          {
              menuItemsLowestLevel[i] = CMenuItem( &ArrEditableItemScreen[i]);
          }

          menuItemsLowestLevel[INDEX_OF_ENG_SR_NO] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_ENG_SR_NO], &ArrEditableItemScreen[INDEX_OF_ENG_SR_NO]);
          menuItemsLowestLevel[INDEX_OF_PIN_1] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_PASSWORD_1], &ArrEditableItemScreen[INDEX_OF_PIN_1]);
          menuItemsLowestLevel[INDEX_OF_PIN_2] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_PASSWORD_2], &ArrEditableItemScreen[INDEX_OF_PIN_2]);


          menuItemsLowestLevel[INDEX_OF_ACTIVE_PROFILE] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_SELECT_PROFILE], &ArrEditableItemScreen[INDEX_OF_ACTIVE_PROFILE]);


      //Now create sub-menus and then initialize mid level menu items:
  //NewUI
          uint16_t j =  INDEX_OF_GENERAL_PROFILE_NAME;
          for(uint8_t i=0;i<sizeof(ParamInSubmenus);i++)
          {
              ArrSubMenu[i] = CMenu(strSubMenu[_u8LanguageArrayIndex][i],ParamInSubmenus[i], &(menuItemsLowestLevel[j]));
              j = j+ParamInSubmenus[i];
          }
  //End
          // Submenu
  //NewUI
              for(uint8_t i=ID_GENERAL_S;i<ID_SUB_MENU_LAST;i++)
              {
                      menuItemsMidLevel[i] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][i], &ArrSubMenu[i]);
              }
  //End


          //MainMenu
  //NewUI
          uint8_t k = 0;
          for(uint8_t i=0;i<sizeof(SubmenusInMenus);i++)
          {

              ArrMenu[i] = CMenu(strMainMenu[_u8LanguageArrayIndex][i],SubmenusInMenus[i], &(menuItemsMidLevel[k]));
              menuItemsTopLevel[i] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][i], &ArrMenu[i]);

              k = k+SubmenusInMenus[i];
          }
  //End

          mainMenu = CMenu((char*)strConfiguration[_u8LanguageArrayIndex], ID_MAIN_MENU_LAST, menuItemsTopLevel);
}

void UI::prvFetchProfileNames()
{
    CFGZ::CFGZ_PARAMS_t param;
    for(uint8_t i=0; i<MAX_NUMBER_OF_PROFILE; i++)
    {

        _objHal.Objpflash.Read((FACTORY_CFGZ_ADDRESS+ (i * (sizeof(CFGZ::CFGZ_PARAMS_t) - sizeof(uint8_t)*(DUMMY_ITEMS)))), (uint8_t*)&param, sizeof(CFGZ::CFGZ_PARAMS_t) );

       memcpy( profilename[i], &param.u8ArrStringParam[CFGZ::ID_ARR_PROFILE], 21);
       _arrProfileNames[i]=profilename[i];
    }
}

void UI::Initialize()
{
    stPassEdit = { false , false };
    InitEditableItems();
    InitEditableItemsScreens();
    InitMenuItemsAndMenus();
    InitialiseCustomSensor();
    HandleMenuVisibility();
}

UI_STATES_t uiState = UI_STATE_INITIALIZING;
void UI::SaveConfigFile()
{
    uint8_t u8Date;
    uint8_t u8Month;
    uint16_t u16Year;
    uint16_t u16PIN1;
    uint16_t u16PIN2;

    CFGZ::CFGZ_PARAMS_t AllParam;
    if(CEditableItem::IsAnyConfigValueEdited())
    {
        _objDisplay.ClearScreen();
        _objDisplay.drawRectangle();
        _objDisplay.gotoxy(GLCD_X(64),GLCD_Y(25));
        _objDisplay.printStringCenterAligned((char*)strSavingSettings[_u8LanguageArrayIndex], FONT_ARIAL);

        if( _MiscParam.u8MiscParam[PROFILE_NO] != ArrEditableItem[INDEX_OF_ACTIVE_PROFILE].value.u8Val)
        {
            _objHal.Objpflash.Read((FACTORY_CFGZ_ADDRESS+ (ArrEditableItem[INDEX_OF_ACTIVE_PROFILE].value.u8Val
                    *( sizeof(CFGZ::CFGZ_PARAMS_t)- sizeof(uint8_t)*DUMMY_ITEMS))), (uint8_t*)&AllParam, sizeof(CFGZ::CFGZ_PARAMS_t) );
        }
        else
        {
<<<<<<< HEAD
            AllParam.f32ArrParam[CFGZ::ID_BTS_CONFIG_LOW_BATT_THRESHOLD] = (float)ArrEditableItem[INDEX_OF_BTS_CONFIG_LOW_BATT_THRESHOLD].value.fVal;

//            for(int i = CFGZ::ID_ENG_CLNT_CALIB_R1, j = INDEX_OF_ENG_CLNT_SENS_R1; i <= CFGZ::ID_ENG_CLNT_CALIB_R10;i=i+2 ,j=j+2)
//            {
//                AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.u16Val;
//                AllParam.f32ArrParam[i+1] = (float)ArrEditableItem[j+1].value.i16Val;
//
//            }


//            for(int i = CFGZ::ID_FUEL_CALIB_R1, j = INDEX_OF_FUEL_SENS_R1; i <= CFGZ::ID_FUEL_CALIB_R10;i=i+2 ,j=j+2)
//            {
//                AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.u16Val;
//                AllParam.f32ArrParam[i+1] = (float)ArrEditableItem[j+1].value.u8Val;
//            }

//            for(int i = CFGZ::ID_LOP_CALIB_R1, j = INDEX_OF_LOP_SENS_R1; i <= CFGZ::ID_LOP_CALIB_R10;i=i+2 ,j=j+2)
//            {
//                AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.u16Val;
//                AllParam.f32ArrParam[i+1] = (float)ArrEditableItem[j+1].value.fVal;
//
//            }


            AllParam.f32ArrParam[CFGZ::ID_S1_SHUTDOWN_THRESH] = (float)ArrEditableItem[INDEX_OF_AUX_S1_SHUTDOWN_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_S1_WARNING_THRESH] = (float)ArrEditableItem[INDEX_OF_AUX_S1_WARNING_THRESHOLD].value.fVal;

            if(ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
            {
                for(int i = CFGZ::ID_S1_CALIB_I1, j = INDEX_OF_AUX_S1_SENS_R1; i <= CFGZ::ID_S1_CALIB_I10;i=i+2 ,j=j+2)
                {
                    AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.u16Val;
                    AllParam.f32ArrParam[i+1] = (float)ArrEditableItem[j+1].value.i16Val;
                }
            }
            else
            {
                for(int i = CFGZ::ID_S1_CALIB_I1, j = INDEX_OF_AUX_S1_SENS_R1; i <= CFGZ::ID_S1_CALIB_I10;i=i+2 ,j=j+2)
                {
                    AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.u16Val;
                    AllParam.f32ArrParam[i+1] = (float)ArrEditableItem[j+1].value.fVal;
                }
            }

            AllParam.f32ArrParam[CFGZ::ID_S2_SHUTDOWN_THRESH] = (float)ArrEditableItem[INDEX_OF_AUX_S2_SHUTDOWN_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_S2_WARNING_THRESH] = (float)ArrEditableItem[INDEX_OF_AUX_S2_WARNING_THRESHOLD].value.fVal;

            for(int i = CFGZ::ID_S2_CALIB_I1, j = INDEX_OF_AUX_S2_SENS_R1; i <= CFGZ::ID_S2_CALIB_I10;i=i+2 ,j=j+2)
            {
                AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.u16Val;
                AllParam.f32ArrParam[i+1] = (float)ArrEditableItem[j+1].value.fVal;
            }

            AllParam.f32ArrParam[CFGZ::ID_S3_SHUTDOWN_THRESH] = (float)ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_S3_WARNING_THRESH] = (float)ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD].value.fVal;
            if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val != CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
            {
                for(int i = CFGZ::ID_S3_CALIB_I1, j = INDEX_OF_AUX_S3_SENS_I1_V1; i <= CFGZ::ID_S3_CALIB_I10;i=i+2 ,j=j+2)
                {
                    AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.u8Val;

                }

            }
            else
            {
                for(int i = CFGZ::ID_S3_CALIB_I1, j = INDEX_OF_AUX_S3_SENS_I1_V1; i <= CFGZ::ID_S3_CALIB_I10;i=i+2 ,j=j+2)
                {
                    AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.fVal;

                }

            }

            for(int i = CFGZ::ID_S3_CALIB_V1, j = INDEX_OF_AUX_S3_SENS_VAL1; i <= CFGZ::ID_S3_CALIB_V10;i=i+2 ,j=j+2)
            {
                AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.fVal;

            }

            AllParam.f32ArrParam[CFGZ::ID_S4_SHUTDOWN_THRESH] = (float)ArrEditableItem[INDEX_OF_AUX_S4_SHUTDOWN_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_S4_WARNING_THRESH] = (float)ArrEditableItem[INDEX_OF_AUX_S4_WARNING_THRESHOLD].value.fVal;

            if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR].value.u8Val != CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
            {
                for(int i = CFGZ::ID_S4_CALIB_I1, j = INDEX_OF_AUX_S4_SENS_I1_V1; i <= CFGZ::ID_S4_CALIB_I10;i=i+2 ,j=j+2)
                {
                    AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.u8Val;

                }

            }
            else
            {
                for(int i = CFGZ::ID_S4_CALIB_I1, j = INDEX_OF_AUX_S4_SENS_I1_V1; i <= CFGZ::ID_S4_CALIB_I10;i=i+2 ,j=j+2)
                {
                    AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.fVal;

                }
            }

            for(int i = CFGZ::ID_S4_CALIB_V1, j = INDEX_OF_AUX_S4_SENS_VAL1; i <= CFGZ::ID_S4_CALIB_V10;i=i+2 ,j=j+2)
            {
                AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.fVal;

            }

=======
>>>>>>> 2ba6ff6cf8a735594b10d9e5dfb4a128a8f5c654
//RushiStart
            uint16_t id_float = (CFGZ::ID_BTS_CONFIG_LOW_BATT_THRESHOLD);
            uint16_t id_uint16 = (CFGZ::ID_CAN_J1939_COMM_ACT_DELAY_AMBER);
            uint16_t id_uint8 = (CFGZ::ID_GENERAL_PROFILE_NAME);
            for (uint16_t i = 0 ; i<=INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE_3 ; i++)
            {
                if(ArrEditableItem[i].dataType == CEditableItem::DT_FLOAT)
                {
                    AllParam.f32ArrParam[id_float] = (float)ArrEditableItem[i].value.fVal;
                    id_float++;
                }
                else if(ArrEditableItem[i].dataType == CEditableItem::DT_UINT16)
                {
                    AllParam.u16ArrParam[id_uint16] = (uint16_t)ArrEditableItem[i].value.u16Val;
                    id_uint16++;
                }
                else if(ArrEditableItem[i].dataType == (CEditableItem::DT_UINT8)||(ArrEditableItem[i].dataType ==CEditableItem::DT_STRING_FIXED))
                {
                    AllParam.u8ArrParam[id_uint8] = (uint8_t)ArrEditableItem[i].value.u8Val;
                    id_uint8++;
                }
                else
                {

                }
            }
//RushiEnd


            AllParam.u8ArrParam[CFGZ::ID_CAN_J1939_COMM_ACTION_AMBER] = (uint8_t)ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTION_AMBER].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CAN_J1939_COMM_ACTIVATION_AMBER] = (uint8_t)ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTIVATION_AMBER].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CAN_J1939_COMM_ACTION_RED] = (uint8_t)ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTION_RED].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CAN_J1939_COMM_ACTIVATION_RED] = (uint8_t)ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTIVATION_RED].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CAN_J1939_COMM_ACTION_MIL] = (uint8_t)ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTION_MIL].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CAN_J1939_COMM_ACTIVATION_MIL] = (uint8_t)ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTIVATION_MIL].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CAN_J1939_COMM_ACTION_PROTECT] = (uint8_t)ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTION_PROTECT].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CAN_J1939_COMM_ACTIVATION_PROTECT] = (uint8_t)ArrEditableItem[INDEX_OF_CAN_J1939_COMM_ACTIVATION_PROTECT].value.u8Val;


            for(uint16_t i = CFGZ::ID_ARR_AUX_INPUT_A;i<=CFGZ::ID_ARR_PROFILE;i++)
            {
                _objcfgz.GetCFGZ_Param((CFGZ::ARRAY_PARAMS_t)i, AllParam.u8ArrStringParam[i]);
            }

        }

        _MiscParam.u8MiscParam[PROFILE_NO] = (uint8_t)ArrEditableItem[INDEX_OF_ACTIVE_PROFILE].value.u8Val;


        if(stPassEdit.bPIN1Changed == true)
        {
            u16PIN1 =(uint16_t)((ArrEditableItem[INDEX_OF_PIN_1].value.stPassword.u8PassChar1*1000) + (ArrEditableItem[INDEX_OF_PIN_1].value.stPassword.u8PassChar2*100) + (ArrEditableItem[INDEX_OF_PIN_1].value.stPassword.u8PassChar3*10)+
                    (ArrEditableItem[INDEX_OF_PIN_1].value.stPassword.u8PassChar4));
        }
        else
        {
            u16PIN1 = _MiscParam.u16MiscParam[PASSWORD1];
        }

        if(stPassEdit.bPIN2Changed == true)
        {
            u16PIN2 =(uint16_t)((ArrEditableItem[INDEX_OF_PIN_2].value.stPassword.u8PassChar1*1000) + (ArrEditableItem[INDEX_OF_PIN_2].value.stPassword.u8PassChar2*100) + (ArrEditableItem[INDEX_OF_PIN_2].value.stPassword.u8PassChar3*10)+
                    (ArrEditableItem[INDEX_OF_PIN_2].value.stPassword.u8PassChar4));
        }
        else
        {
            u16PIN2 = _MiscParam.u16MiscParam[PASSWORD2];
        }

        if((stPassEdit.bPIN1Changed)||(stPassEdit.bPIN2Changed))
        {
            if((u16PIN1 != u16PIN2))
            {
                _MiscParam.u16MiscParam[PASSWORD1] = u16PIN1;
                _MiscParam.u16MiscParam[PASSWORD2]=  u16PIN2;
                _objDisplay.gotoxy(GLCD_X(64),GLCD_Y( 40));
                _objDisplay.printStringCenterAligned((char*)strPasswordChanged[_u8LanguageArrayIndex], FONT_VERDANA);
            }
            else
            {
                _objDisplay.gotoxy(GLCD_X(64),GLCD_Y( 40));
                _objDisplay.printStringCenterAligned((char*)strPasswordAlreadyExist[_u8LanguageArrayIndex], FONT_VERDANA);
            }
        }

        _MiscParam.u8EngId[0] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar1;
        _MiscParam.u8EngId[1] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar2;
        _MiscParam.u8EngId[2] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar3;
        _MiscParam.u8EngId[3] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar4;
        _MiscParam.u8EngId[4] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar5;
        _MiscParam.u8EngId[5] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar6;
        _MiscParam.u8EngId[6] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar7;
        _MiscParam.u8EngId[7] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar8;
        _MiscParam.u8EngId[8] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar9;
        _MiscParam.u8EngId[9] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar10;
        _MiscParam.u8EngId[10] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar11;
        _MiscParam.u8EngId[11] = ArrEditableItem[INDEX_OF_ENG_SR_NO].value.stEngSrNo.u8EngSrNoChar12;

        // _MiscParam.u8MiscParam[DISP_VOLT_EN] = (uint8_t)ArrEditableItem[INDEX_OF_DISP_VOLT_FILT_EN].value.u8Val;
        // _MiscParam.u8MiscParam[DISP_VOLT_CONST] = (uint8_t)ArrEditableItem[INDEX_OF_DISP_VOLT_FILT_CONST].value.u8Val;


        _MiscParam.u16CRC = CRC16::ComputeCRCGeneric((uint8_t *)&_MiscParam, sizeof(MISC_PARAM_t) -sizeof(uint16_t), CRC_MEMORY_SEED);

        _objHal.Objeeprom.RequestWrite(EXT_EEPROM_PASWORD_START_ADDRESS,(uint8_t*)&_MiscParam, sizeof(MISC_PARAM_t) , NULL) ;
        _objcfgz.WriteActiveProfile(&AllParam);

    }

    _objcfgz.ApplyConfigChanges();
    uiState = UI_STATE_INITIALIZING;
}

void UI::DigitalInputMenuVisiblity(uint16_t u16SourceIndex)
{

    if(ArrEditableItem[u16SourceIndex].value.u8Val==CFGZ::CFGZ_SENSOR_NOT_USED)
    {
        menuItemsLowestLevel[u16SourceIndex + 1].isEnabled =false;
        menuItemsLowestLevel[u16SourceIndex + 2].isEnabled =false;
        menuItemsLowestLevel[u16SourceIndex + 3].isEnabled =false;
        menuItemsLowestLevel[u16SourceIndex + 4].isEnabled =false;
    }
    else
    {
        menuItemsLowestLevel[u16SourceIndex + 1].isEnabled =true;
        menuItemsLowestLevel[u16SourceIndex + 2].isEnabled =true;
        menuItemsLowestLevel[u16SourceIndex + 3].isEnabled =true;
        menuItemsLowestLevel[u16SourceIndex + 4].isEnabled =true;
        switch(ArrEditableItem[u16SourceIndex].value.u8Val)
        {
            case CFGZ::CFGZ_USER_CONFIGURED_SENSOR:
            case CFGZ::CFGZ_VBEL_BROKEN_SWITCH:
            case CFGZ::CFGZ_LOW_WATER_LVL_SWITCH:
            case CFGZ::CFGZ_BATTERY_CHARGER_FAIL:
            case CFGZ::CFGZ_SMOKE_FIRE:
            case CFGZ::CFGZ_MODE_SELECT:
            case CFGZ::CFGZ_AMB_TEMP_SELECT:
            {
               //Do nothing
            }
            break;
            case CFGZ::CFGZ_LOW_FUEL_LEVEL_SWITCH:
            case CFGZ::CFGZ_LOW_LUBE_OIL_PRESS_SWITCH:
            case CFGZ::CFGZ_HIGH_ENGINE_TEMP_SWITCH:
            case CFGZ::CFGZ_MAINS_CONTACTOR_LATCHED:
            case CFGZ::CFGZ_GEN_CONTACTOR_LATCHED:
            {
                menuItemsLowestLevel[u16SourceIndex + 3].isEnabled =false;
            }
            break;

            case CFGZ::CFGZ_SIMULATE_START:
            case CFGZ::CFGZ_SIMULATE_STOP:
            case CFGZ::CFGZ_SIMULATE_AUTO:
            case CFGZ::CFGZ_EMERGENCY_STOP:
            case CFGZ::CFGZ_CLOSE_GEN_OPEN_MAINS_SWITCH:
            case CFGZ::CFGZ_CLOSE_MAINS_OPEN_GEN_SWITCH:
            case CFGZ::CFGZ_SIMULATE_MAINS:
           // case CFGZ::CFGZ_REMOTE_ALARM_MUTE:
         //   case CFGZ::CFGZ_REMOTE_ALARM_ACK:
            {
                menuItemsLowestLevel[u16SourceIndex + 2].isEnabled =false;
                menuItemsLowestLevel[u16SourceIndex + 3].isEnabled =false;
                menuItemsLowestLevel[u16SourceIndex + 4].isEnabled =false;
                TurnOFFActionActivation(u16SourceIndex);
            }
            break;

            case CFGZ::CFGZ_REMOTE_START_STOP:
            {
                menuItemsLowestLevel[u16SourceIndex + 2].isEnabled =false;
                menuItemsLowestLevel[u16SourceIndex + 3].isEnabled =false;
               // TurnOFFActionActivation(u16SourceIndex);
            }
            break;
            default: break;
        }
    }

}

void UI::TurnOFFActionActivation(uint16_t u16Index)
{
   ArrEditableItem[u16Index +2].value.u8Val = 0;
   ArrEditableItem[u16Index +3].value.u8Val = 0;
   ArrEditableItem[u16Index +4].value.u8Val = 1;
   ArrEditableItem[u16Index +2].tempValue.u8Val = 0;
   ArrEditableItem[u16Index +3].tempValue.u8Val = 0;
   ArrEditableItem[u16Index +4].tempValue.u8Val = 1;
}
bool UI::IsOutputConfigured(uint8_t u8Source)
{
    for(uint16_t i = INDEX_OF_OUT_A_SOURCE ; i<=INDEX_OF_OUT_G_SOURCE; i = i+2)
    {
        if(ArrEditableItem[i].value.u8Val == u8Source)
        {
            return true;
        }
    }

    return false;
}
void UI::ResetDigitalInputSource(uint16_t u16SourceIndex)
{
    if((ArrEditableItem[u16SourceIndex].value.u8Val!=CFGZ::CFGZ_ANLG_DIG_IN))
    {
        ArrEditableItem[u16SourceIndex+1].value.u8Val = CFGZ::CFGZ_SENSOR_NOT_USED;
        ArrEditableItem[u16SourceIndex+1].tempValue.u8Val = CFGZ::CFGZ_SENSOR_NOT_USED;
    }
}

void UI::DigitalOutputMenuVisiblity(uint16_t u16SourceIndex)
{
    menuItemsLowestLevel[u16SourceIndex + 1].isEnabled = (bool)(ArrEditableItem[u16SourceIndex].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);

}
void UI::MenuVisibilityOfGenThreshAndDly(uint16_t u16Index)
{
    if(ArrEditableItem[u16Index].value.u8Val == CFGZ::CFGZ_DISABLE )
    {
        menuItemsLowestLevel[u16Index + 1].isEnabled =false;
        menuItemsLowestLevel[u16Index + 2].isEnabled =false;
    }
    else
    {
        menuItemsLowestLevel[u16Index + 1].isEnabled =true;
        menuItemsLowestLevel[u16Index + 2].isEnabled =true;
    }
}
void UI::MenuVisiblituyOfMainsThresh(uint16_t u16Index)
{
    if(ArrEditableItem[u16Index].value.u8Val == CFGZ::CFGZ_DISABLE )
    {
        menuItemsLowestLevel[u16Index + 1].isEnabled =false;
        menuItemsLowestLevel[u16Index + 2].isEnabled =false;
    }
    else
    {
        menuItemsLowestLevel[u16Index + 1].isEnabled =true;
        menuItemsLowestLevel[u16Index + 2].isEnabled =true;
    }
}
void UI::HandleMenuVisibility(void)
{
    bool bEn_Ds=true;

    bEn_Ds = (ArrEditableItem[INDEX_OF_MODBUS_COMM_COMM_MODE].value.u8Val == CFGZ::CFGZ_ENABLE );
    LowestLevelMenuEnDis(INDEX_OF_MODBUS_COMM_MODBUS_SLAVE_ID,INDEX_OF_MODBUS_COMM_PARITY,bEn_Ds);

    bEn_Ds  = (ArrEditableItem[INDEX_OF_BTS_CONFIG_BATTERY_MON].value.u8Val ==  CFGZ::CFGZ_ENABLE );
    LowestLevelMenuEnDis(INDEX_OF_BTS_CONFIG_LOW_BATT_THRESHOLD,INDEX_OF_BTS_CONFIG_DG_RUN_DURATION,bEn_Ds);

    bEn_Ds  = (ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_CYCLIC_MODE].value.u8Val ==  CFGZ::CFGZ_ENABLE );
    LowestLevelMenuEnDis(INDEX_OF_CYCLIC_CONFIG_DG_OFF_DURATION,INDEX_OF_CYCLIC_CONFIG_DG_ON_DURATION,bEn_Ds);

    bEn_Ds  = (ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFIG_NIGHT_MODE].value.u8Val ==  CFGZ::CFGZ_ENABLE );
    LowestLevelMenuEnDis(INDEX_OF_NIGHT_MODE_CONFIG_START_TIME,INDEX_OF_NIGHT_MODE_CONFIG_OFF_DURATION,bEn_Ds);

    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_A_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_B_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_C_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_D_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_E_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_F_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_G_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_H_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_I_SOURCE);



    if(ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
    {
        LowestLevelMenuEnDis(INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE,INDEX_OF_LOP_RES_DIG_J_V10,false);
    }
    else if(ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN)
    {
        LowestLevelMenuEnDis(INDEX_OF_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION,INDEX_OF_LOP_RES_DIG_J_V10,false);
        menuItemsLowestLevel[INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE     ].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_LOP_RES_DIG_J_DIG_POLARITY,INDEX_OF_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY,bEn_Ds);
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION,INDEX_OF_LOP_RES_DIG_J_V10,true);
        LowestLevelMenuEnDis(INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE,INDEX_OF_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY, false);
    }

    if(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
    {
<<<<<<< HEAD
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_K_SOURCE,INDEX_OF_FUEL_LVL_DIG_K_L10,false);
=======
        LowestLevelMenuEnDis(INDEX_OF_FUEL_LVL_DIG_K_DIG_SOURCE,INDEX_OF_FUEL_LVL_DIG_K_L10,false);
>>>>>>> 2ba6ff6cf8a735594b10d9e5dfb4a128a8f5c654
    }
    else if(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN)
    {
<<<<<<< HEAD
        LowestLevelMenuEnDis(INDEX_OF_LFL_SHUTDOWN_EN,INDEX_OF_FUEL_LVL_DIG_K_L10,false);
        menuItemsLowestLevel[INDEX_OF_DIG_IN_K_SOURCE].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_DIG_IN_K_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_K_POLARITY,INDEX_OF_DIG_IN_K_DELAY,bEn_Ds);
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_LFL_SHUTDOWN_EN,INDEX_OF_FUEL_LVL_DIG_K_L10,true);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_K_SOURCE,INDEX_OF_DIG_IN_K_DELAY, false);
        menuItemsLowestLevel[INDEX_OF_LFL_SHUTDOWN_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_LFL_SHUTDOWN_EN].value.u8Val == CFGZ::CFGZ_ENABLE );
        menuItemsLowestLevel[INDEX_OF_LFL_WARNING_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_LFL_WARNING_EN].value.u8Val == CFGZ::CFGZ_ENABLE );
        menuItemsLowestLevel[INDEX_OF_FUEL_THEFT_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_FUEL_THEFT_ALARM_EN].value.u8Val == CFGZ::CFGZ_ENABLE );
=======
        LowestLevelMenuEnDis(INDEX_OF_FUEL_LVL_DIG_K_SHUTDOWN,INDEX_OF_FUEL_LVL_DIG_K_L10,false);
        menuItemsLowestLevel[INDEX_OF_FUEL_LVL_DIG_K_DIG_SOURCE].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_FUEL_LVL_DIG_K_DIG_POLARITY,INDEX_OF_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY,bEn_Ds);
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_FUEL_LVL_DIG_K_SHUTDOWN,INDEX_OF_FUEL_LVL_DIG_K_L10,true);
        LowestLevelMenuEnDis(INDEX_OF_FUEL_LVL_DIG_K_DIG_SOURCE,INDEX_OF_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY, false);
        menuItemsLowestLevel[INDEX_OF_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SHUTDOWN].value.u8Val == CFGZ::CFGZ_ENABLE );
        menuItemsLowestLevel[INDEX_OF_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_NOTIFICATION].value.u8Val == CFGZ::CFGZ_ENABLE );
        menuItemsLowestLevel[INDEX_OF_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING].value.u8Val == CFGZ::CFGZ_ENABLE );
>>>>>>> 2ba6ff6cf8a735594b10d9e5dfb4a128a8f5c654
    }

    if(ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
    {
<<<<<<< HEAD
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_M_DIG_SOURCE,INDEX_OF_ENG_TEMP_DIG_M_T10,false);
=======
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_L_DIG_SOURCE,INDEX_OF_ENG_TEMP_DIG_L_T10,false);
>>>>>>> 2ba6ff6cf8a735594b10d9e5dfb4a128a8f5c654
    }
    else if(ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN)
    {
<<<<<<< HEAD
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_M_ACTION,INDEX_OF_ENG_TEMP_DIG_M_T10,false);
        menuItemsLowestLevel[INDEX_OF_ENG_TEMP_DIG_M_DIG_SOURCE].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_DIG_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_M_DIG_POLARITY,INDEX_OF_ENG_TEMP_DIG_M_DIG_ACTIVATION_DELAY,bEn_Ds);
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_M_ACTION,INDEX_OF_ENG_TEMP_DIG_M_T10,true);
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_M_DIG_SOURCE,INDEX_OF_ENG_TEMP_DIG_M_DIG_ACTIVATION_DELAY, false);
=======
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_L_ACTION,INDEX_OF_ENG_TEMP_DIG_L_T10,false);
        menuItemsLowestLevel[INDEX_OF_ENG_TEMP_DIG_L_DIG_SOURCE].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_L_DIG_POLARITY,INDEX_OF_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY,bEn_Ds);
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_L_ACTION,INDEX_OF_ENG_TEMP_DIG_L_T10,true);
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_L_DIG_SOURCE,INDEX_OF_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY, false);
>>>>>>> 2ba6ff6cf8a735594b10d9e5dfb4a128a8f5c654
    }


    ResetDigitalInputSource(INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION);
    ResetDigitalInputSource(INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION);
    ResetDigitalInputSource(INDEX_OF_ENG_TEMP_DIG_L_SENSOR_SELECTION);
    ResetDigitalInputSource(INDEX_OF_SHEL_TEMP_DIG_M_SENSOR_SELECTION);
    ResetDigitalInputSource(INDEX_OF_AUX_S2_RES_DIG_N_SENSOR_SELECTION);
    ResetDigitalInputSource(INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION);
    ResetDigitalInputSource(INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION);

    //Digital O/P
    for(uint16_t i = INDEX_OF_OUT_A_SOURCE;i<=INDEX_OF_OUT_G_SOURCE;i=i+2)
    {
        DigitalOutputMenuVisiblity(i);
    }
}

void UI::ConfigCheckKeyPress(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent)
{
    uint8_t u8KeyCode = CKeyCodes::NONE;
    switch(_sKeyEvent)
    {
      case START_KEY_SHORT_PRESS:
          u8KeyCode = CKeyCodes::ENTER;
          break;
      case STOP_KEY_SHORT_PRESS:
          u8KeyCode = CKeyCodes::ESCAPE;
          break;
      case DN_SHORT_PRESS:
          u8KeyCode = CKeyCodes::DOWN;
          break;
      case UP_SHORT_PRESS:
          u8KeyCode = CKeyCodes::UP;
          break;
      default:
          u8KeyCode = CKeyCodes::NONE;
          break;
    }

    if(u8KeyCode !=CKeyCodes::NONE)
    {
        Handler(u8KeyCode);
    }

    if(UTILS_GetElapsedTimeInMs(&_ValIncDecTimer) >= CONFIG_VAL_INC_DEC_TIMER_MS)
    {
        UTILS_ResetTimer(&_ValIncDecTimer);

        if(_sKeyEvent ==  DN_LONG_PRESS )
        {
            u8KeyCode = CKeyCodes::DN_LONG;
            Handler(u8KeyCode);
        }
        else if(_sKeyEvent ==  UP_LONG_PRESS )
        {
            u8KeyCode = CKeyCodes::UP_LONG;
            Handler(u8KeyCode);
        }
    }
}

void UI::ConfigUIUpdate()
{
    if(uiState == UI_STATE_SHOWING_SCREEN_ITEM_EDIT) // For blinking leaf node
    {
        _pCurEditableItemsScreen->show(true);
    }
}

void UI::UpdateMaxNumberOfItem()
{
   u16NumberofEditableItems = 0;

  for(uint8_t i =0; i<pCurMenu->numOfMenuItems; i++)
  {
      if(pCurMenu->pMenuItems[i].isEnabled)
      {
          u16NumberofEditableItems++;
      }
  }
}
void UI::Handler(int keyCode)
{
/*
* This function should be called repeatedly - but not necessarily
* periodically. It is okay to call this from the main loop.
* A state machine is implemented here.
*/
    _objDisplay.ClearScreen();
    _objDisplay.drawRectangle();
    _objDisplay.drawHorizontalLine(GLCD_X(0), GLCD_Y(19), GLCD_Y(128));
    _objDisplay.drawVerticalLine(GLCD_X(98), GLCD_Y(0), GLCD_Y(19));
    bool bChineseSelected = (_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_POWER_ON_MODE)==CFGZ::LANGUAGE_CHINSESE);
    switch (uiState)
    {
    case    UI_STATE_INITIALIZING:
        Initialize();
        pCurMenu = &mainMenu;
        u16IndexOfEditableItems =0;
        UpdateMaxNumberOfItem();
        uiState = UI_STATE_SHOWING_MENU;
        pCurMenu->show(bChineseSelected);
        UTILS_ResetTimer(&_ValIncDecTimer);
        _u16MenuSp =0;
        break;
    case    UI_STATE_SHOWING_MENU:
        switch (keyCode)
        {
        case    CKeyCodes::DOWN:
            if(_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].dataType == CEditableItem::DT_PASSWORD
                    ||_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].dataType == CEditableItem::DT_ENG_SR_NO)
            {

                if(_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex <
                                       _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MaxOneScreenEditItems-1)
               {
                    _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex++;
               }
                else
                {
                    _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex =0;
                }

            }
            if (pCurMenu->indexOfSelectedMenuItem < (pCurMenu->numOfMenuItems - 1))
            {
                (pCurMenu->indexOfSelectedMenuItem)++;
                u16IndexOfEditableItems++;
            }
            else
            {
                u16IndexOfEditableItems =0;
                pCurMenu->indexOfSelectedMenuItem = 0;

            }

            while(pCurMenu->pMenuItems[pCurMenu->indexOfSelectedMenuItem].isEnabled == false)
            {
                if (pCurMenu->indexOfSelectedMenuItem < (pCurMenu->numOfMenuItems - 1))
                {
                    (pCurMenu->indexOfSelectedMenuItem)++;
                }
                else
                {
                    u16IndexOfEditableItems =0;
                    pCurMenu->indexOfSelectedMenuItem = 0;
                }
            }
            pCurMenu->show(bChineseSelected);
            break;
        case    CKeyCodes::UP:
            if(_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].dataType == CEditableItem::DT_PASSWORD
                   || _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].dataType == CEditableItem::DT_ENG_SR_NO)
            {

                if(_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex >  0)
               {
                    _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex--;
               }
                else
                {
                    _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex =
                            _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MaxOneScreenEditItems-1;
                }
            }
            if (pCurMenu->indexOfSelectedMenuItem > 0)
            {
                (pCurMenu->indexOfSelectedMenuItem)--;
                u16IndexOfEditableItems--;

            }
            else
            {
                pCurMenu->indexOfSelectedMenuItem = (pCurMenu->numOfMenuItems - 1);
                u16IndexOfEditableItems = u16NumberofEditableItems-1;
            }

            while(pCurMenu->pMenuItems[pCurMenu->indexOfSelectedMenuItem].isEnabled == false)
            {
                if (pCurMenu->indexOfSelectedMenuItem > 0)
                {
                    (pCurMenu->indexOfSelectedMenuItem)--;
                }
                else
                {
                    pCurMenu->indexOfSelectedMenuItem = (pCurMenu->numOfMenuItems - 1);
                    u16IndexOfEditableItems = u16NumberofEditableItems;
                }
            }
            pCurMenu->show(bChineseSelected);
            break;
        case    CKeyCodes::ENTER:
            if (pCurMenu->pMenuItems[pCurMenu->indexOfSelectedMenuItem].isLeafNode)
            {
                // we ought to show the editable items screen
                _pCurEditableItemsScreen = pCurMenu->pMenuItems[pCurMenu->indexOfSelectedMenuItem].pEditableItemsScreen;
                _pCurEditableItemsScreen->initTempValues();
                _pCurEditableItemsScreen->show(true);    // i.e. show tempValue(s) rather than Value(s)
                if(((_pCurEditableItemsScreen->pEditableItems->u8PasswordLevel & _objPassword.GetEnteredPassword()) //If accessible through entered password
                        || (_objPassword.GetEnteredPassword()== PASSWORD_ENTRY_UI::MASTER_PIN))
                      )
                {
                    if (_pCurEditableItemsScreen->numOfEditableItems > 1)
                    {
                        // if multiple items on the screen
                        uiState = UI_STATE_SHOWING_SCREEN_ITEM_SELECTION;
                    }
                    else
                    {
                        // if only one item on the screen
                        uiState = UI_STATE_SHOWING_SCREEN_ITEM_EDIT;
                    }
                }
            }
            else
            {
                // we ought to show the next menu
                // push the cure menu on stack
                if (_u16MenuSp < MENU_STACK_DEPTH)
                {
                    _menuStack[_u16MenuSp] = pCurMenu;
                    _u16MenuSp++;
                }
                pCurMenu = pCurMenu->pMenuItems[pCurMenu->indexOfSelectedMenuItem].pMenu;
                _pCurEditableItemsScreen = pCurMenu->pMenuItems[pCurMenu->indexOfSelectedMenuItem].pEditableItemsScreen;
                _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex=0;
                u16IndexOfEditableItems = 0;
            }
            HandleMenuVisibility();
            UpdateMaxNumberOfItem();
            InitialiseCustomSensor();
            pCurMenu->show(bChineseSelected);
            break;
        case    CKeyCodes::ESCAPE:
            // go back one level

            if (_u16MenuSp > 0)
            {
                pCurMenu->indexOfSelectedMenuItem = 0;
                _u16MenuSp--;
                pCurMenu = _menuStack[_u16MenuSp];
                u16IndexOfEditableItems = (uint16_t) pCurMenu->indexOfSelectedMenuItem;
                uint16_t i = (uint16_t)pCurMenu->indexOfSelectedMenuItem;
                uint16_t j = 1;
                /* Following logic is implemented to update the indexofEditableItems on Escape keypress
                 * This logic checks if any of the menus before the current menu are disabled by traversing backwards till the first menu item.
                 * Then depending on the number of menus disabled , 1 is decremented from the indexofEditableItem(Menu index)*/
                while(i>0)
                {
                    if((pCurMenu->pMenuItems[pCurMenu->indexOfSelectedMenuItem - j].isEnabled == false))
                    {
                        u16IndexOfEditableItems--;
                    }
                    i--;
                    j++;
                }
            }
            UpdateMaxNumberOfItem();
            pCurMenu->show(bChineseSelected);
            break;
        default:
            pCurMenu->show(bChineseSelected);
            break;
        }
        break;
    case   UI_STATE_SHOWING_SCREEN_ITEM_EDIT:
        switch (keyCode)
        {
        case    CKeyCodes::ENTER:
                if(_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].dataType > CEditableItem::DT_TIME_HRS_MINS)
                {
                    _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex++;
                    if(_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex >=
                            _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MaxOneScreenEditItems)
                    {
                        _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex =0;

                         uiState = UI_STATE_SHOWING_MENU;
                    }
                }
                else
                {
                   uiState = UI_STATE_SHOWING_MENU;
                }
                InitialiseCustomSensor();
                _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].saveTempValue();
                HandleMenuVisibility();
                UpdateMaxNumberOfItem();
                pCurMenu->show(bChineseSelected);

                break;
        case    CKeyCodes::ESCAPE:
            _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].CopyPrevValue();
            if (_pCurEditableItemsScreen->numOfEditableItems > 1)
            {
                // if multiple items on the screen
                uiState = UI_STATE_SHOWING_SCREEN_ITEM_SELECTION;
                _pCurEditableItemsScreen->show(true);
            }
            else
            {
                uiState = UI_STATE_SHOWING_MENU;
                pCurMenu->show(bChineseSelected);
            }
            break;
        case    CKeyCodes::DOWN:
            // now decrement the value being displayed
            _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].decrementValue(true, false);
            _pCurEditableItemsScreen->show(true);
            break;
        case    CKeyCodes::UP:
            // now decrement the value being displayed
            _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].incrementValue(true, false);
            _pCurEditableItemsScreen->show(true);
            break;

        case CKeyCodes::UP_LONG:
            _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].incrementValue(true, true);
            _pCurEditableItemsScreen->show(true);
            break;
        case CKeyCodes::DN_LONG:
            _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].decrementValue(true, true);
            _pCurEditableItemsScreen->show(true);
            break;
        default:
            _pCurEditableItemsScreen->show(true);
           break;
        }
        break;
    default:
        pCurMenu->show(bChineseSelected);
        break;
    }
}

volatile uint16_t table_x_S1_sens[10] = {100,   200,   300,   400,   500,   600,   700,   800,   900,   1000};
volatile float    table_y_S1_sens[10] = {100.0f,200.0f,300.0f,400.0f,500.0f,600.0f,700.0f,800.0f,900.0f,1000.0f};

volatile uint16_t table_x_Shelt_Temp[10] = {400, 800, 1200, 1600, 2000, 2400, 2800, 3200, 3600, 4000};
volatile int16_t  table_y_Shelt_Temp[10] = {85,  75,  65,   55,   45,   35,   25,   15,   5,    -5};

volatile uint8_t table_x_4_20mA_Sens[10] = {4,  6,  7,  9,  10, 12, 14, 15, 17, 20};
volatile float   table_y_4_20mA_Sens[10] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};
volatile float table_y_LOP_Curr_sens[10] = {1.0f,  2.0f,  3.0f,  4.0f,  5.0f,  6.0f,  7.0f,  8.0f,  9.0f,  10.0f};


volatile float table_x_0to5V_Sens[10] = {0.5f ,1.0f ,1.5f ,2.0f ,2.5f ,3.0f ,3.5f ,4.0f ,4.5f ,5.0f};
volatile float table_y_0to5V_Sens[10] = {10.0f,20.0f,30.0f,40.0f,50.0f,60.0f,70.0f,80.0f,90.0f,100.0f};

void UI::InitialiseCustomSensor()
{

//    if((ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].value.u8Val != ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].tempValue.u8Val))
//    {
//        if((ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].tempValue.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
//        {
//            ArrEditableItem[INDEX_OF_AUX_S1_CKT_FAULT_ACTION].value.u8Val = CFGZ::CFGZ_ACTION_NONE;
//            ArrEditableItem[INDEX_OF_AUX_S1_CKT_FAULT_ACTION].tempValue.u8Val = CFGZ::CFGZ_ACTION_NONE;
//
//            for(uint8_t i = 0; i<10; i++)
//            {
//                 ArrEditableItem[INDEX_OF_AUX_S1_SENS_R1 + (i*2)].value.u16Val = table_x_S1_sens[i];
//                 ArrEditableItem[INDEX_OF_AUX_S1_SENS_R1 + (i*2)].tempValue.u16Val = table_x_S1_sens[i];
//                 ArrEditableItem[INDEX_OF_AUX_S1_SENS_V1 + (i*2)].value.fVal = table_y_S1_sens[i];
//                 ArrEditableItem[INDEX_OF_AUX_S1_SENS_V1 + (i*2)].tempValue.fVal = table_y_S1_sens[i];
//            }
//        }
//        else if(ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].tempValue.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
//        {
//          for(uint8_t i = 0; i<10; i++)
//          {
//               ArrEditableItem[INDEX_OF_AUX_S1_SENS_R1 + (i*2)].value.u16Val = table_x_Shelt_Temp[i];
//               ArrEditableItem[INDEX_OF_AUX_S1_SENS_R1 + (i*2)].tempValue.u16Val = table_x_Shelt_Temp[i];
//               ArrEditableItem[INDEX_OF_AUX_S1_SENS_V1 + (i*2)].value.i16Val = table_y_Shelt_Temp[i];
//               ArrEditableItem[INDEX_OF_AUX_S1_SENS_V1 + (i*2)].tempValue.i16Val = table_y_Shelt_Temp[i];
//          }
//        }
//    }

       if((ArrEditableItem[INDEX_OF_CURRENT_MONITOR_LOAD_CT_RATIO].tempValue.u16Val !=ArrEditableItem[INDEX_OF_CURRENT_MONITOR_LOAD_CT_RATIO].value.u16Val))
    {
        _engMon.ReadEnergySetEnergyOffset(false);
        _engMon.StoreCummulativeCnt();
    }
}



uint16_t UI::prvMaxDaysInMonth(uint8_t u8Month ,uint16_t u16Year)
{
    if(u8Month == 2)
    {
        return IsLeapYear(u16Year)?(DaysInMonth[u8Month - 1]) : (DaysInMonth[u8Month - 1] - 1);
    }

    return (DaysInMonth[u8Month - 1]);
}
