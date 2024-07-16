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

#define CONFIG_VAL_INC_DEC_TIMER_MS (300U)
CMenuItem UI::menuItemsLowestLevel[INDEX_LAST];
CMenuItem UI::menuItemsMidLevel[NUM_OF_MENU_ITEMS_AT_MID_LEVEL];
CMenuItem UI::menuItemsTopLevel[NUM_OF_MENU_ITEMS_AT_TOP_LEVEL];
uint16_t u16IndexOfEditableItems=0,u16NumberofEditableItems=0;
//NewUI
uint8_t ParamInSubmenus[ID_SUB_MENU_LAST] =
{
 LEAFNODES_IN_GENERAL,
 LEAFNODES_IN_DISPLAY,
 LEAFNODES_IN_MODBUS_COMM,
 LEAFNODES_IN_BTS_CONFIG,
 LEAFNODES_IN_CYCLIC_CONFIG,
 LEAFNODES_IN_NIGHT_MODE_CONFG,
 LEAFNODES_IN_AUTO_MD_SWITCH,
 LEAFNODES_IN_DIG_IN_A,
 LEAFNODES_IN_DIG_IN_B,
 LEAFNODES_IN_DIG_IN_C,
 LEAFNODES_IN_DIG_IN_D,
 LEAFNODES_IN_DIG_IN_E,
 LEAFNODES_IN_DIG_IN_F,
 LEAFNODES_IN_DIG_IN_G,
 LEAFNODES_IN_DIG_IN_H,
 LEAFNODES_IN_DIG_IN_I,
 LEAFNODES_IN_LOP_RES_DIG_J,
 LEAFNODES_IN_FUEL_LVL_DIG_K,
 LEAFNODES_IN_ENG_TEMP_DIG_L,
 LEAFNODES_IN_SHEL_TEMP_DIG_M,
 LEAFNODES_IN_AUX_S2_RES_DIG_N,
 LEAFNODES_IN_AUX_S3_DIG_O,
 LEAFNODES_IN_AUX_S4_DIG_P,
 LEAFNODES_IN_OUT_A,
 LEAFNODES_IN_OUT_B,
 LEAFNODES_IN_OUT_C,
 LEAFNODES_IN_OUT_D,
 LEAFNODES_IN_OUT_E,
 LEAFNODES_IN_OUT_F,
 LEAFNODES_IN_OUT_G,
 LEAFNODES_IN_OUT_H,
 LEAFNODES_IN_OUT_I,
 LEAFNODES_IN_OUT_J,
 LEAFNODES_IN_OUT_K,
 LEAFNODES_IN_OUT_L,
 LEAFNODES_IN_OUT_M,
 LEAFNODES_IN_OUT_N,
 LEAFNODES_IN_OUT_O,
 LEAFNODES_IN_OUT_P,
 LEAFNODES_IN_CRANKING_TIMER,
 LEAFNODES_IN_GENERAL_TIMER,
 LEAFNODES_IN_ALT_CONFIG,
 LEAFNODES_IN_VOLT_MONITOR,
 LEAFNODES_IN_FREQ_MONITOR,
 LEAFNODES_IN_CURRENT_MONITOR,
 LEAFNODES_IN_FAN_CURR_MONITOR,
 LEAFNODES_IN_LOAD_MONITOR,
 LEAFNODES_IN_MAINS_CONFIG,
 LEAFNODES_IN_UNDER_VOLT_MON,
 LEAFNODES_IN_OVER_VOLT_MON,
 LEAFNODES_IN_UNDER_FREQ_MON,
 LEAFNODES_IN_OVER_FREQ_MON,
 LEAFNODES_IN_CRANK_DISCONN,
 LEAFNODES_IN_SPEED_MONITOR,
 LEAFNODES_IN_BATTERY_MONITOR,
 LEAFNODES_IN_CHARGE_ALT_MON,
 LEAFNODES_IN_PREHEAT,
 LEAFNODES_IN_ECU,
 LEAFNODES_IN_LOP_FROM_ECU,
 LEAFNODES_IN_COOL_TEMP_ECU,
 LEAFNODES_IN_LOT_ECU,
 LEAFNODES_IN_CANOPY_TEMP_ECU,
 LEAFNODES_IN_EGR_FAULT_MON,
 LEAFNODES_IN_MAINT_ALARM,
 LEAFNODES_IN_ALARM_DUE_DATE,
 LEAFNODES_IN_SR_NO_DATA,
 LEAFNODES_IN_PASSWORD_1,
 LEAFNODES_IN_PASSWORD_2,
 LEAFNODES_IN_SELECT_PROFILE,
};

uint8_t SubmenusInMenus[ID_MAIN_MENU_LAST] =
{
 SUBMENUS_IN_MODULE,
 SUBMENUS_IN_INPUTS,
 SUBMENUS_IN_OUTPUTS,
 SUBMENUS_IN_TIMERS,
 SUBMENUS_IN_GENERATOR,
 SUBMENUS_IN_MAINS,
 SUBMENUS_IN_ENGINE,
 SUBMENUS_IN_MAINTENANCE,
 SUBMENUS_IN_ID,
 SUBMENUS_IN_MISC
};

//End
//All editable option string screens
enum
{
    ID_MANUAL_AUTO,
    ID_ENABLE_DISABLE,
    ID_MODBUS,
    ID_BAUDRATE,
    ID_PARITY,
    ID_MODBUS_MAP,
    ID_YES_NO,

    ID_EVENT_OCCURENCE,
    ID_DIG_IP_POLARITY,

    ID_ACTION_NoWESN,
    ID_ACTION_NoWES,
    ID_ACTION_NoWS,
    ID_ACTION_WS,
    ID_ACTION_NoN,
    ID_ACTION_NW,

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

    ID_SPEED_SENSE_SOURCE,
    ID_SPEED_SENSE_SOURCE_ALT_F,
    ID_CT_LOCATION,
    ID_THRESHOLD_TYPE,

    ID_ENGINE_TYPE,
    ID_ECU_ALARM_ACTION,

    ID_BATT_MON_SRC,

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
    ID_MM,
    ID_UNIT_LST
};

static const char* arrUnit[ID_UNIT_LST]=
{
   " %",
   " Ohm",
   " degC",//"`C",
   " sec",
   " Mins",
   " Hz",
   " RPM",
   " V",
   " Hrs",
   " kW",
   " V Ph-N",
   " A",
   " Bar",
   " % RPM error",
   " x25 Hz",
   " mA",
   " mm"
};

static const char* strOutputSources[1][CFGZ::CFGZ_OUTPUT_LAST] =
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
  "Low Fuel LVL Switch",
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
  "DG Not In Manual",
  "Manual Mode",
  "BTS Mode",
  "Preheat Output",
  "ECU Start",
  "Malfunction Indicator Lamp",
  "Inducement Buzzer",
  "EGR Output",
  "Buzzer 2",
  "Battery Unhealthy",
  "Supercapacitor Unhealthy",
  "Canopy Temp Sensor",
  "DG On Load",
  "DG Overload",
  "LFL Sensor Shutdown"
 }
};

static const char* strInputSources[1][CFGZ::CFGZ_INPUT_LAST]=
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
  "Close Gen/Open Mains SW",
  "Close Mains/Open Gen SW",
  "Simulate Mains",
  "V-Belt Broken Switch",
  "Mains Contactor Latched",
  "Genset Contactor Latched",
  "Battery Charger Fail",
  "Smoke Fire",
  "Mode Select Switch",
  "Ambient Temp Switch",
  "EGR Digital In",
  "EB MCCB On Feedback",
  "DG MCCB On Feedback",
  "Supercapacitor Fail",
  "Canopy Door Open"
 }
};

static const char* strOptions[1][ID_LAST][8]=
{
 {
  {"Manual", "Auto"},
  {"Disable","Enable"},
  {"None", "MODBUS-SEDEMAC"},
  {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"},
  {"None", "Even", "Odd"},
  {"Map A", "Map B"},
  {"No", "Yes"},
  {"Daily","Weekly","Monthly"},
  {"Close To Activate", "Open To Activate"},

  {"None", "Warning", "Electrical Trip","Shutdown","Notification"},
  {"None", "Warning", "Electrical Trip","Shutdown"},
  {"None", "Warning", "Shutdown"},
  {"Warning", "Shutdown"},
  {"None", "Notification"},
  {"Notification", "Warning"},

  {"Never","From Engine Start", "From Monitoring On", "Always"},
  {"Not used","Dig In J", "LOP Sensor","LOP By J1939"},
  {"Not used","Dig In K", "Anlg In Fuel LVL"},
  {"Not used","Dig In L", "Anlg In Eng Temp","Coolant Temp by J1939"},
  {"Not used","Dig In M", "Shelter Temperature", "Canopy Temperature"},
  {"Not used","Dig In N", "Auxiliary Sensor "},
  {"Not used", "Dig In O", "LOP Curr Sensor", "LOP Vlt Sensor"},
  {"Not used", "Dig In P", "Fuel Vlt Sensor"},
  {"Engine Body","Battery Negative"},
  {"De-energise", "Energise"},
  {"2 Poles", "4 Poles", "6 Poles", "8 Poles"},
  {"1 Phase", "3 Phase"},
  {"EARTH Leakage Current","FAN Current"},
  {"Alt Freq","Magnetic pickup","W Point Frequency"},
  {"Alt Freq","Speed Mon By J1939"},
  {"On Alt Output Cable", "On Load Cable"},
  {"Less Than Threshold", "Greater Than Threshold"},
  {     "Conventional",
        "ECU 162"},
  {"None",
   "Warning",
   "Electrical Trip",
   "Shutdown",
   "Notification"},
  {"Battery", "Supercapacitor"}
 }
};

static const char* strMainMenu[1][ID_MAIN_MENU_LAST]
{
    {
        "MODULE",
        "INPUTS",
        "OUTPUTS",
        "TIMERS",
        "GENERATOR",
        "MAINS",
        "ENGINE",
        "MAINTENANCE",
        "ID",
        "MISC"
    }
};

typedef enum
{
    /*This enum is for strAlternate which provides alternate strings*/
    ID_STR_ALT_SUBMENU_ENG_TEMPERATURE,
    ID_STR_ALT_EDITITEM_ENG_TEMP_SHDN_EN,
    ID_STR_ALT_EDITITEM_ENG_TEMP_SHDN_TH,
    ID_STR_ALT_EDITITEM_ENG_TEMP_WARN_EN,
    ID_STR_ALT_EDITITEM_ENG_TEMP_WARN_TH,
    ID_STR_ALT_LAST
}STR_ALT;
static const char* strAlternate[1][ID_STR_ALT_LAST]
{
  {
    "ENG TEMPERATURE",
    "ENG TEMP SHDN EN",
    "ENG TEMP SHDN TH",
    "ENG TEMP WARN EN",
    "ENG TEMP WARN TH"
  }
};

static const char* strSubMenu[1][ID_SUB_MENU_LAST]
{
    {
        //Module
        "GENERAL",
        "DISPLAY",
        "MODBUS COMM",
        "BTS CONFIG",
        "CYCLIC CONFIG",
        "NIGHT MODE CONFG",
        "AUTO MD SWITCH",
        //Inputs
        "DIG IN A",
        "DIG IN B",
        "DIG IN C",
        "DIG IN D",
        "DIG IN E",
        "DIG IN F",
        "DIG IN G",
        "DIG IN H",
        "DIG IN I",
        "LOP RES / DIG J",
        "FUEL LVL / DIG K",
        "ENG TEMP / DIG L",
        "SHEL TEMP /DIG M",
        "AUX S2 RES /DIG N",
        "AUX S3/DIG O",
        "AUX S4/DIG P",
        //Outputs
        "OUT A",
        "OUT B",
        "OUT C",
        "OUT D",
        "OUT E",
        "OUT F",
        "OUT G",
        "OUT H",
        "OUT I",

        "OUT J",
        "OUT K",
        "OUT L",
        "OUT M",
        "OUT N",
        "OUT O",
        "OUT P",
        //Timers
        "CRANKING TIMER",
        "GENERAL TIMER",
        //Generator
        "ALT CONFIG",
        "VOLT MONITOR",
        "FREQ MONITOR",
        "CURRENT MONITOR",
        "FAN CURR MON",
        "LOAD MONITOR",
        //Mains
        "MAINS CONFIG",
        "UNDER VOLT MON",
        "OVER VOLT MON",
        "UNDER FREQ MON",
        "OVER FREQ MON",
        //Engine
        "CRANK DISCONN",
        "SPEED MONITOR",
        "BATTERY MONITOR",
        "CHARGE ALT MON",
        "PREHEAT",

        "ENG CONTROL UNIT",
        "LUBE OIL PRESSURE",
        "CLNT TEMPERATURE",
        "LUBE OIL TEMP",
        "CANOPY TEMP",

        //EGR
        "EGR TIMERS",

        //Maintenance
        "MAINT ALARM",
        "ALARM DUE DATE",
        //ID
        "SERIAL NO DATA",
        "MASTER PIN",
        "USER PIN",
        //MISC
        "SELECT PROFILE"
    }
};

static const char* strLeafNode[1][SID_LEAF_NODE_STRING]
{
    {
        //"GENERAL",
        "PROFILE NAME",
        "POWER ON MODE",
        "POWER ON LAMP TEST",
        "PASSWORD LOCK",
        //"DISPLAY",
        "CONTRAST",
        "POWER SAVE MODE",
        //"COMMUNICATION",
        "COMM MODE",
        "MODBUS SLAVE ID",
        "MODBUS BAUDRATE",
        "PARITY",
        "MODBUS MAP",
        //"BTS CONFIG",
        "BATTERY MON",
        "LOW BATT THRESHOLD",
        "LOW BATT MON DELAY",
        "DG RUN DURATION",
        //"CYCLIC CONFIG",
        "CYCLIC MODE",
        "DG OFF DURATION",
        "DG ON DURATION",
        //"NIGHT MODE CONFG",
        "NIGHT MODE",
        "START TIME",
        "OFF DURATION",
        //"AUTO MD SWITCH",
        "AUTO MD SWITCH",
        "MANUAL MODE TIME",
        "OUTPUT ON TIME",
        //"DIG IN A"
        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION  DELAY",
        //"DIG IN B"
        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION  DELAY",
        //"DIG IN C"
        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION  DELAY",
        //"DIG IN D"
        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION  DELAY",
        //"DIG IN E"
        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION  DELAY",
        //"DIG IN F"
        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION  DELAY",
        //"DIG IN G"
        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION  DELAY",
        //"DIG IN H"
        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION  DELAY",
        //"DIG IN I"
        "SOURCE",
        "POLARITY",
        "ACTION",
        "ACTIVATION",
        "ACTIVATION  DELAY",
        //"LOP RES /DIG J",
        "SENSOR SELECTION",
        "(DIG) SOURCE",
        "(DIG) POLARITY",
        "(DIG) ACTION",
        "(DIG) ACTIVATION",
        "(DIG) ACTIVATION DELAY",
        "CIRCUIT FAULT ACTION",
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
        //"FUEL LVL / DIG K"
        "SENSOR SELECTION",
        "(DIG) SOURCE",
        "(DIG) POLARITY",
        "(DIG) ACTION",
        "(DIG) ACTIVATION",
        "(DIG) ACTIVATION DELAY",
        "SHUTDOWN",
        "SHUTDOWN THRESHOLD",
        "NOTIFICATION",
        "NOTIFICATION THRESH",
        "FUEL TANK CAPACITY",
        "FUEL THEFT WARNING",
        "FUEL THEFT THRESHOLD",
        "FUEL CONSUMPTION",
        "FUEL IN PERCENT",
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
        "R11",
        "L11",
        "R12",
        "L12",
        "R13",
        "L13",
        "R14",
        "L14",
        "R15",
        "L15",
        "R16",
        "L16",
        "R17",
        "L17",
        "R18",
        "L18",
        "R19",
        "L19",
        "R20",
        "L20",
        //"ENG TEMP / DIG L",
        "SENSOR SELECTION",
        "(DIG) SOURCE",
        "(DIG) POLARITY",
        "(DIG) ACTION",
        "(DIG) ACTIVATION",
        "(DIG) ACTIVATION DELAY",
        "OPEN CKT WARNING",
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
        //"SHEL TEMP /CANOPY TEMP/ DIG M",
        "SENSOR SELECTION",
        "(DIG) SOURCE",
        "(DIG) POLARITY",
        "(DIG) ACTION",
        "(DIG) ACTIVATION",
        "(DIG) ACTIVATION DELAY",
        "HIGH TEMP THRESHOLD",
        "LOW TEMP THRESHOLD",
        "HIGH TEMP MON DELAY",
        "DG RUN DURATION",
        "OPEN CKT NOTIFICATION",
        "OPEN CKT WARNING",
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
        //"AUX S2 RES/DIG N",
        "SENSOR SELECTION",
        "(DIG) SOURCE",
        "(DIG) POLARITY",
        "(DIG) ACTION",
        "(DIG) ACTIVATION",
        "(DIG) ACTIVATION DELAY",
        "ACTION",
        "THRESHOLD",
        "THRESHOLD TYPE",
        "OPEN CKT WARNING",
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
        //"AUX S3/DIG O",
        "SENSOR SELECTION",
        "(DIG) SOURCE",
        "(DIG) POLARITY",
        "(DIG) ACTION",
        "(DIG) ACTIVATION",
        "(DIG) ACTIVATION DELAY",
        "CIRCUIT FAULT ACTION",
        " I1/V1",
        " P1",
        " I2/V2",
        " P2",
        " I3/V3",
        " P3",
        " I4/V4",
        " P4",
        " I5/V5",
        " P5",
        " I6/V6",
        " P6",
        " I7/V7",
        " P7",
        " I8/V8",
        " P8",
        " I9/V9",
        " P9",
        " I10/V10",
        " P10",
        //"AUX S4 /DIG P",
        "SENSOR SELECTION",
        "(DIG) SOURCE",
        "(DIG) POLARITY",
        "(DIG) ACTION",
        "(DIG) ACTIVATION",
        "(DIG) ACTIVATION DELAY",
        "SHUTDOWN",
        "SHUTDOWN THRESHOLD",
        "NOTIFICATION",
        "NOTIFICATION THRESH",
        "FUEL THEFT WARNING",
        "FUEL THEFT THRESHOLD",
        "FUEL CONSUMPTION",
        "FUEL IN %",
        "SENSOR LOW VTG",
        "SENSOR HIGH VTG",
        "TANK WITH STEP",
        "TANK WIDTH",
        "TANK LENGTH 1",
        "TANK HEIGHT 1",
        "TANK LENGTH 2",
        "TANK HEIGHT 2",
        //"OUT A"
        "SOURCE",
        "ON ACTIVATION",
        //"OUT B"
        "SOURCE",
        "ON ACTIVATION",
        //"OUT C"
        "SOURCE",
        "ON ACTIVATION",
        //"OUT D"
        "SOURCE",
        "ON ACTIVATION",
        //"OUT E"
        "SOURCE",
        "ON ACTIVATION",
        //"OUT F"
        "SOURCE",
        "ON ACTIVATION",
        //"OUT G"
        "SOURCE",
        "ON ACTIVATION",
        //"OUT H"
        "SOURCE",
        "ON ACTIVATION",
        //"OUT I"
        "SOURCE",
        "ON ACTIVATION",
        // "OUT J"
        "SOURCE",
        "ON ACTIVATION",
        // "OUT K"
        "SOURCE",
        "ON ACTIVATION",
        // "OUT L"
        "SOURCE",
        "ON ACTIVATION",
        // "OUT M"
        "SOURCE",
        "ON ACTIVATION",
        // "OUT N"
        "SOURCE",
        "ON ACTIVATION",
        // "OUT O"
        "SOURCE",
        "ON ACTIVATION",
        // "OUT P"
        "SOURCE",
        "ON ACTIVATION",
        //"CRANKING"
        "CRANK HOLD TIME",
        "CRANK REST TIME",
        "MANUAL START DELAY",
        "AUTO START DELAY",
        //"GENERAL"
        "SAFETY MONITOR DELAY",
        "MAINS DETECT DELAY",
        "ALT DETECT DELAY",
        "WARM-UP DELAY",
        "RETN-TO-MAINS DELAY",
        "ENG COOL TIME",
        "STOP ACTION TIME",
        "ADDN STOPPING TIME",
        "LOAD TRANSFER DELAY",
        "PWR SAVE MODE DELAY",
        "SCRN CHNGOVER TIME",
        "SOUNDER ALARM TIMER",
        "TEST MODE TIMER",
        //"ALT CONFIG"
        "ALT PRESENT",
        "NUMBER OF  POLES",
        "ALT AC SYSTEM",
        "MIN HEALTHY VOLT",
        "MIN HEALTHY FREQ",
        "PHASE REVERSAL DETECT",
        "PHASE REVERSAL ACTION",
        "AUTO LOAD TRANSFER",
        "ALT WAVE DETECTION",
        //"VOLT MON"
        "UNDER VOLT SHUTDOWN",
        "UV SHUTDOWN THRESH",
        "UNDER VOLT WARNING",
        "UV WARNING THRESHOLD",
        "OVER VOLT SHUTDOWN",
        "OV SHUTDOWN THRESH",
        "OVER VOLT WARNING",
        "OV WARNING THRESHOLD",
        //"FREQ MON"
        "UNDER FREQ SHUTDOWN",
        "UF SHUTDOWN THRESH",
        "UNDER FREQ WARNING",
        "UF WARNING THRESHOLD",
        "OVER FREQ SHUTDOWN",
        "OF SHUTDOWN THRESH",
        "OVER FREQ WARNING",
        "OF WARNING THRESHOLD",
        //"CURRENT MON"
        "LOAD CT RATIO",
        "OVER CURR ACTION",
        "OVER CURR THRESHOLD",
        "OVER CURR DELAY",
        "CT CORRECTION FACTOR",
        "CT LOCATION",
        //"FAN CURR MON"
        "FAN CURRENT MON",
        "FAN MON CT RATIO",
        "HIGH CURR THRESHOLD",
        "HIGH CURR ACTION",
        "LOW CURR THRESHOLD",
        "LOW CURR ACTION",
        "CURR MON DELAY",
        //"LOAD MON"
        "GEN  RATING",
        "FULL LOAD CURRENT",
        "OVERLOAD ACTION",
        "OVERLOAD THRESHOLD",
        "OVERLOAD MON DELAY",
        "UNBAL LOAD ACTION",
        "UNBAL LOAD THRESHOLD",
        "UNBAL LOAD DELAY",
        "UNBAL LOAD ACT THRES",
        //“MAINS CONFIG “
        "MAINS MONITORING",
        "MAINS AC SYSTEM",
        "PHASE REVERSAL DETECT",
        "PHASE REVERSAL ACTION",
        "3PH CALC EN FOR 1PH",
        //"UV MON"
        "ENABLE",
        "TRIP",
        "RETURN",
        //"OV MON"
        "ENABLE",
        "TRIP",
        "RETURN",
        //"UF MON"
        "ENABLE",
        "TRIP",
        "RETURN",
        //"OF MON"
        "ENABLE",
        "TRIP",
        "RETURN",
        //"CRANK DISCONN",
        "START ATTEMPTS",
        "DISCONN ON LOP SENS",
        "DISCONN LOP SENS",
        "MON LLOP BEFORE CRNK",
        "MON LOP BEFORE CRANK",
        "DISCONN ON LLOP SW",
        "LLOP SW TRANS TIME",
        "ALT FREQUENCY",
        "ENGINE SPEED",
        "DISCON ON CHG ALT VLT",
        "CHG ALT THRESHOLD",
        //"SPEED MON",
        "SPEED SENSE SOURCE",
        /*"RESERVED",*/ //"FLYWHEEL TEETH",
        "UNDER SPEED SHUTDOWN",
        "UNDER SPEED THRESH",
        "UNDER SPEED DELAY",
        "OVER SPEED THRESH",
        "OVER SPEED DELAY",
        "GROSS OS THRESHOLD",
        //"BATTERY MON",
        "MONITORING SOURCE",
        "LOW VOLT ACTION",
        "LOW VOLT THRESHOLD",
        "LOW VOLT DELAY",
        "HIGH VOLT ACTION",
        "HIGH VOLT THRESHOLD",
        "HIGH VOLT DELAY",
        //"CHARGE ALT MON",
        "FAIL ACTION",
        "FAIL THRESHOLD",
        "FAIL DELAY",
        //"PREHEAT",
        "PREHEAT TIMER",
        "ENG TEMPERATURE",
        "ENG TEMP THRESHOLD",
        "AMB TEMPERATURE",

        "ENGINE TYPE",
        "ENG SPEED FROM ECU",
        "LOP FROM ECU",
        "CLNT TEMP FROM ECU",
        "ENG RUN HRS FROM ECU",
        "BAT VTG FROM ECU",
        "OIL TEMP FROM ECU",
        "COMM FAIL ACTION",
        "COMM FAIL ACTIVATION",
        "COMM FAIL ACT DELAY",
        "AMBER ACTION",
        "AMBER ACTIVATION",
        "AMBER ACT DELAY",
        "RED ACTION",
        "RED ACTIVATION",
        "RED ACT DELAY",
        "MIL ACTION",
        "MIL ACTIVATION",
        "MIL ACT DELAY",
        "PROTECT ACTION",
        "PROTECT ACTIVATION",
        "PROTECT ACT DELAY",
        "GCU SOURCE ADDR",
        "ECU SOURCE ADDR",
        "SHUTDOWN",
        "SHUTDOWN THRESHOLD",
        "WARNING",
        "WARNING THRESHOLD",
        "CLNT TEMP SHDN EN",
        "CLNT TEMP SHDN TH",
        "CLNT TEMP WARN EN",
        "CLNT TEMP WARN TH",
        "OIL TEMP SHDN EN",
        "OIL TEMP SHDN TH",
        "OIL TEMP WARN EN",
        "OIL TEMP WARN TH",
        "CANOPY TEMP SHDN EN",
        "CANOPY TEMP SHDN TH",
        "CANOPY TEMP WARN EN",
        "CANOPY TEMP WARN TH",

        //EGR Timers
        "ENABLE TIMERS",
        "FAULT SHUTDOWN TIMER",
        "FAULT WARNING TIMER",
        "HEAL TIMER",

        //"MAINT ALARM",
        "ACTION",
        "DUE AT ENGINE HOURS",
        //"ALARM DUE DATE",
        "SERVICE DATE #1",
        "SERVICE DATE #2",
        "SERVICE DATE #3",

        //SR NO DATA
        "GENSET SR NO",
        "ENGINE SR NO",
        "ALTERNATOR SR NO",
        "CONTROLLER SR NO",
        "ENG CONTROLLER SR NO",
        "SITE ID",

        /*Select Profile*/
        "PROFILE"
    }
};

CMenu mainMenu;
CMenu* pCurMenu = &mainMenu;
UI::PASSWORD_EDIT_FLAGS_t UI::stPassEdit={false,false};
bool UI::bSrNosEdited = false;
PRODUCT_SR_NOS_t UI::_stSrNos = {};

/*char* arrMonth[12];*/

UI::UI(HAL_Manager &hal,  PASSWORD_ENTRY_UI &Password, CFGZ &cfgz, Display &Disp, ENGINE_MONITORING &engMon, MB_APP &MbApp):
_objHal(hal),
_objPassword(Password),
_objcfgz(cfgz),
_objDisplay(Disp),
_engMon(engMon),
_mbApp(MbApp),
_u16MenuSp(0),
_menuStack{NULL},
_MiscParam{},
_arrProfileNames{NULL},
profilename{0},
_u8LanguageArrayIndex(0),
_pCurEditableItemsScreen(NULL)
{
    UTILS_ResetTimer(&_ValIncDecTimer);
    prvFetchProfileNames();
    prvUpdateAutomationModbusMap();
    prvReadSrNos();
}

void UI::prvReadSrNos()
{
    bool bUpdateDataInEEPROM;
    /*Copy signature, version and crc into the structure*/
    _objHal.Objeeprom.BlockingRead(SERIAL_NOS_AREA_START_ADDRESS, (uint8_t*)&_stSrNos,  sizeof(PRODUCT_SR_NOS_t));
    if(_stSrNos.u32Signature == SR_NO_DATA_SIGNATURE)
    {
        if(_stSrNos.u16ProdSrNoVer == SR_NOS_LATEST_VER)
        {
            if(CRC16::ComputeCRCGeneric((uint8_t *)&_stSrNos + SR_NOS_HEADER_SIZE,
              (sizeof(PRODUCT_SR_NOS_t) - SR_NOS_HEADER_SIZE),
             CRC_MEMORY_SEED) != _stSrNos.u16Crc)
            {
                /*CRC does not match*/
                bUpdateDataInEEPROM = true;
            }
            else
            {
                bUpdateDataInEEPROM = false;
            }
        }
        else
        {
            /*Version is different. Add code for handling future versions here*/
            bUpdateDataInEEPROM = true;
        }
    }
    else
    {
        /*Signature is incorrect*/
        bUpdateDataInEEPROM = true;
    }

    if(bUpdateDataInEEPROM) /*Sr no data is invalid*/
    {
        uint16_t u16DataLen = (sizeof(PRODUCT_SR_NOS_t) - SR_NOS_HEADER_SIZE);
        uint8_t* pu8DataStart = (uint8_t *)&_stSrNos + SR_NOS_HEADER_SIZE;
        memset(pu8DataStart, 0, u16DataLen); //Ascii code for NULL
        _stSrNos.u32Signature = SR_NO_DATA_SIGNATURE;
        _stSrNos.u16ProdSrNoVer = SR_NOS_LATEST_VER;
        _stSrNos.u8GenSrNo[GEN_SRNO_LEN-1] = '\0';
        _stSrNos.u8EngSrNo[ENG_SRNO_LEN-1] = '\0';
        _stSrNos.u8AltSrNo[ALT_SRNO_LEN-1] = '\0';
        _stSrNos.u8MainContSrNo[MAIN_CONT_SRNO_LEN-1] = '\0';
        _stSrNos.u8EngContSrNo[ENG_CONT_SRNO_LEN-1] = '\0';
        _stSrNos.u8SiteId[SITE_ID_LEN-1] = '\0';
        _stSrNos.u16Crc = CRC16::ComputeCRCGeneric((uint8_t *)&_stSrNos + SR_NOS_HEADER_SIZE,
               u16DataLen, CRC_MEMORY_SEED);
        _objHal.Objeeprom.RequestWrite(SERIAL_NOS_AREA_START_ADDRESS, (uint8_t*)&_stSrNos, sizeof(PRODUCT_SR_NOS_t), NULL);
    }
}

void UI::InitEditableItems()
{
    _objcfgz.ReadMiscParam(&_MiscParam);
    CEditableItem::PASSWORD_t stPIN_1,stPIN_2;

    _u8LanguageArrayIndex =  0;

    /*for(uint8_t i=0; i<12; i++)
    {
        memcpy( &arrMonth[i], &StrMonth[_u8LanguageArrayIndex][i], 21);
    }*/

    uint8_t Customer_Code = _objcfgz.GetCustomerCodefromCFGC();

    uint16_t Mains_UV_Trip_min,Mains_UV_Trip_max,Mains_UV_return_min,Mains_UV_return_max;
    uint16_t Mains_OV_Trip_min,Mains_OV_Trip_max,Mains_OV_return_min,Mains_OV_return_max;
    uint16_t Mains_Detect_delay_min,Mains_Detect_delay_max;

    if(Customer_Code == 3)
    {
        Mains_UV_Trip_min = 120;
        Mains_UV_Trip_max = 240;
        Mains_UV_return_min = 122;
        Mains_UV_return_max = 242;

        Mains_OV_Trip_min = 202;
        Mains_OV_Trip_max = 288;
        Mains_OV_return_min = 200;
        Mains_OV_return_max = 286;

        Mains_Detect_delay_min = 1;
        Mains_Detect_delay_max = 20;
    }
    else
    {
        Mains_UV_Trip_min = 99;
        Mains_UV_Trip_max = 240;
        Mains_UV_return_min = 122;
        Mains_UV_return_max = 242;

        Mains_OV_Trip_min = 202;
        Mains_OV_Trip_max = 288;
        Mains_OV_return_min = 200;
        Mains_OV_return_max = 286;

        Mains_Detect_delay_min = 1;
        Mains_Detect_delay_max = 30;
    }

/*
 * SuryaPranayTeja.BVV 15-11-2022
 * The INDEX_OF_GENERAL_PROFILE_NAME is just for display purpose as of on 15-11-2022.
 * It should display the profile name which is editable by the select profile present in MISC.
 * The CFGZ parameter of uint8 is reserved which is not being used currently.
 */
    ArrEditableItem[INDEX_OF_GENERAL_PROFILE_NAME] = CEditableItem((uint32_t)_MiscParam.u8MiscParam[PROFILE_NO],strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_PROFILE_NAME], "", "%s",(const char**)_arrProfileNames, 1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_GENERAL_POWER_ON_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_POWER_ON_MODE), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_POWER_ON_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MANUAL_AUTO], 2  , CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_POWER_ON_LAMP_TEST] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_POWER_ON_LAMP_TEST),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_POWER_ON_LAMP_TEST], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_PASSWORD_LOCK] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_PASSWORD_LOCK),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_PASSWORD_LOCK], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_DISPLAY_CONTRAST] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISPLAY_CONTRAST), strLeafNode[_u8LanguageArrayIndex][SID_DISPLAY_CONTRAST], arrUnit[ID_PERCENT], "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DISPLAY_POWER_SAVE_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISPLAY_POWER_SAVE_MODE),strLeafNode[_u8LanguageArrayIndex][SID_DISPLAY_POWER_SAVE_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_MODBUS_COMM_COMM_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_COMM_MODE),strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_COMM_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MODBUS], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_COMM_MODBUS_SLAVE_ID] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_MODBUS_SLAVE_ID), strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_MODBUS_SLAVE_ID], "", "%u", (uint8_t)1, (uint8_t)247, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_COMM_MODBUS_BAUDRATE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_MODBUS_BAUDRATE),strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_MODBUS_BAUDRATE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_BAUDRATE], 8, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_COMM_PARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_PARITY),strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_PARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_PARITY], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_COMM_MAP] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_MAP),strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_MAP], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MODBUS_MAP], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_BTS_CONFIG_BATTERY_MON] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_BATTERY_MON], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_BTS_CONFIG_LOW_BATT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_LOW_BATT_THRESHOLD], arrUnit[ID_V], "%f", (float)40.0,(float) 55.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_BTS_CONFIG_LOW_BATT_MON_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_MON_DELAY),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_LOW_BATT_MON_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_BTS_CONFIG_DG_RUN_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_DG_RUN_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_CYCLIC_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_CYCLIC_MODE),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_CONFIG_CYCLIC_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_DG_OFF_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_CONFIG_DG_OFF_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_DG_ON_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_ON_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_CONFIG_DG_ON_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFG_NIGHT_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFG_NIGHT_MODE),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_MODE_CONFG_NIGHT_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFG_START_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFG_START_TIME),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_MODE_CONFG_START_TIME], arrUnit[ID_MINS], "%u", (uint16_t)0, (uint16_t)2359,true, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFG_OFF_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFG_OFF_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_MODE_CONFG_OFF_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)1440, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_AUTO_MD_SWITCH_AUTO_MD_SWITCH] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUTO_MD_SWITCH_AUTO_MD_SWITCH),strLeafNode[_u8LanguageArrayIndex][SID_AUTO_MD_SWITCH_AUTO_MD_SWITCH], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUTO_MD_SWITCH_MANUAL_MODE_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUTO_MD_SWITCH_MANUAL_MODE_TIME),strLeafNode[_u8LanguageArrayIndex][SID_AUTO_MD_SWITCH_MANUAL_MODE_TIME], arrUnit[ID_MINS], "%u", (uint16_t)16, (uint16_t)120, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUTO_MD_SWITCH_OUTPUT_ON_TIME] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUTO_MD_SWITCH_OUTPUT_ON_TIME), strLeafNode[_u8LanguageArrayIndex][SID_AUTO_MD_SWITCH_OUTPUT_ON_TIME], arrUnit[ID_MINS], "%u", (uint8_t)5, (uint8_t)15, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_A_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_B_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_C_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_D_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_E_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_F_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_G_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_H_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_I_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_LOP_DIG_J_SENS_SEL], 3, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_DIG_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_DIG_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_DIG_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWS], 3, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R1), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R1], arrUnit[ID_OHM], "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V1), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V1], arrUnit[ID_BAR], "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R2), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R2], arrUnit[ID_OHM], "%.0f",(float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V2), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V2], arrUnit[ID_BAR], "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R3), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R3], arrUnit[ID_OHM],  "%.0f",(float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V3), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V3], arrUnit[ID_BAR], "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R4), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R4], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V4), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V4], arrUnit[ID_BAR], "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R5), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R5], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V5), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V5],arrUnit[ID_BAR], "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R6), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R6], arrUnit[ID_OHM],  "%.0f",(float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V6), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V6], arrUnit[ID_BAR], "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R7), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R7], arrUnit[ID_OHM],  "%.0f",(float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V7), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V7], arrUnit[ID_BAR], "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R8), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R8], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V8), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V8], arrUnit[ID_BAR], "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R9), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R9], arrUnit[ID_OHM], "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V9), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V9], arrUnit[ID_BAR], "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_R10), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_R10], arrUnit[ID_OHM],  "%.0f",(float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_V10), strLeafNode[_u8LanguageArrayIndex][SID_LOP_RES_DIG_J_V10], arrUnit[ID_BAR], "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_FUEL_DIG_K_SENS_SEL], 3, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SHUTDOWN), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_SHUTDOWN], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD], arrUnit[ID_PERCENT], "%u",(uint8_t)0, (uint8_t)78, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_NOTIFICATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_NOTIFICATION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_NOTIFICATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD], arrUnit[ID_PERCENT], "%u",(uint8_t)2, (uint8_t)80,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY], "Litre", "%u", (uint16_t)2, (uint16_t)1000,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD], "%/Hour", "%u",(uint8_t)1, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_CONSUMPTION] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION], " Litre/hour", "%f", (float)0.0,(float)100.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_IN_PCT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_IN_PCT), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_IN_PCT], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R1), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R1], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L1), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L1], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R2), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R2], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L2), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L2], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R3), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R3], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L3), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L3], arrUnit[ID_PERCENT],  "%.0f",(float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R4), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R4], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L4), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L4], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R5), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R5], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L5), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L5], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R6), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R6], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L6), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L6], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R7), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R7], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L7), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L7], arrUnit[ID_PERCENT],  "%.0f",(float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R8), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R8], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L8), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L8], arrUnit[ID_PERCENT],  "%.0f",(float)0, (float)100.0,(float)1.0,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R9), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R9], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L9), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L9], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R10), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R10],arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L10), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L10], arrUnit[ID_PERCENT], "%.0f", (float)0, (float)100.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R11] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R11), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R11], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L11] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L11), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L11], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R12] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R12), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R12], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L12] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L12), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L12], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R13] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R13), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R13], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L13] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L13), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L13], arrUnit[ID_PERCENT],  "%.0f",(float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R14] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R14), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R14], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L14] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L14), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L14], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R15] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R15), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R15], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L15] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L15), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L15], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R16] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R16), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R16], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L16] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L16), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L16], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R17] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R17), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R17], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L17] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L17), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L17], arrUnit[ID_PERCENT],  "%.0f",(float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R18] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R18), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R18], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L18] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L18), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L18], arrUnit[ID_PERCENT],  "%.0f",(float)0, (float)100.0,(float)1.0,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R19] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R19), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R19], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L19] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L19), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L19], arrUnit[ID_PERCENT],  "%.0f", (float)0, (float)100.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_R20] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_R20), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_R20],arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_L20] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_L20), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_L20], arrUnit[ID_PERCENT], "%.0f", (float)0, (float)100.0,(float)1.0,  CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENG_CLNT_DIG_L_SENS_SEL], 3, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_OPEN_CKT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_OPEN_CKT_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_OPEN_CKT_WARNING],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R1), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R1], arrUnit[ID_OHM],  "%.0f", (float)0, (float)5000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T1), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T1],arrUnit[ID_DEG_C],  "%.0f",(float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R2), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R2], arrUnit[ID_OHM],  "%.0f", (float)0, (float)5000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T2), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T2], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R3), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R3],arrUnit[ID_OHM],  "%.0f", (float)0, (float)5000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T3), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T3], arrUnit[ID_DEG_C],  "%.0f",(float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R4), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R4], arrUnit[ID_OHM], "%.0f", (float)0, (float)5000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T4), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T4], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R5), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R5], arrUnit[ID_OHM],  "%.0f", (float)0, (float)5000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T5), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T5], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R6), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R6], arrUnit[ID_OHM],  "%.0f", (float)0, (float)5000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T6), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T6], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R7), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R7], arrUnit[ID_OHM],  "%.0f", (float)0, (float)5000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T7), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T7], arrUnit[ID_DEG_C],  "%.0f",(float)-5.0, (float)300.0,(float)1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R8), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R8], arrUnit[ID_OHM],  "%.0f", (float)0, (float)5000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T8), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T8], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R9), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R9], arrUnit[ID_OHM],  "%.0f", (float)0, (float)5000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T9), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T9], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_R10), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_R10], arrUnit[ID_OHM],  "%.0f", (float)0, (float)5000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_T10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_T10), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_L_T10], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M_SENS_SEL], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_HIGH_TEMP_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_HIGH_TEMP_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_HIGH_TEMP_THRESHOLD], arrUnit[ID_DEG_C], "%.0f",(float)20.0, (float)60.0,(float)1,CEditableItem::PIN1_ALLOWED );//16
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_LOW_TEMP_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_LOW_TEMP_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_LOW_TEMP_THRESHOLD], arrUnit[ID_DEG_C], "%.0f",(float)18.0, (float)55.0,(float)1, CEditableItem::PIN1_ALLOWED );//16
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_HIGH_TEMP_MON_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_HIGH_TEMP_MON_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_HIGH_TEMP_MON_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_OPEN_CKT_NOTIFICATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_OPEN_CKT_NOTIFICATION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_OPEN_CKT_NOTIFICATION],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DG_RUN_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DG_RUN_DURATION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_DG_RUN_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R1), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R1], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T1), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T1],arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R2), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R2], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T2), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T2], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R3), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R3],arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T3), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T3], arrUnit[ID_DEG_C],  "%.0f",(float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R4), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R4], arrUnit[ID_OHM], "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T4), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T4], arrUnit[ID_DEG_C],  "%.0f",(float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R5), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R5], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T5), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T5], arrUnit[ID_DEG_C],  "%.0f",(float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R6), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R6], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T6), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T6], arrUnit[ID_DEG_C],  "%.0f",(float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R7), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R7], arrUnit[ID_OHM],  "%.0f",(float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T7), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T7], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R8), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R8], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T8), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T8], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R9), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R9], arrUnit[ID_OHM],  "%.0f",(float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T9), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T9], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_R10), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_R10], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_T10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_T10), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_T10], arrUnit[ID_DEG_C],  "%.0f", (float)-5.0, (float)300.0,(float)1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N_SENS_SEL], 3, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWES], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_THRESHOLD], "", "%f", (float)0, (float)1000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_THRESHOLD_TYPE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_THRESHOLD_TYPE), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_THRESHOLD_TYPE], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_THRESHOLD_TYPE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_OPEN_CKT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_OPEN_CKT_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_OPEN_CKT_WARNING],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R1], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V1],"",  "%f", (float)0.0, (float)1000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R2], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V2], "",  "%f", (float)0.0, (float)1000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R3), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R3],arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V3), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V3], "",  "%f", (float)0.0, (float)1000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R4), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R4], arrUnit[ID_OHM], "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V4), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V4], "",  "%f", (float)0.0, (float)1000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R5), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R5], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0, (float)1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V5), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V5],"" ,  "%f", (float)0.0, (float)1000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R6), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R6], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0, (float)1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V6), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V6], "",  "%f", (float)0.0, (float)1000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R7), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R7], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V7), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V7], "",  "%f", (float)-0.0, (float)1000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R8), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R8], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V8), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V8], "",  "%f", (float)0.0, (float)1000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R9), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R9], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V9), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V9],"" ,  "%f", (float)0.0, (float)1000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_R10), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_R10], arrUnit[ID_OHM],  "%.0f", (float)0, (float)1000.0,(float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_V10), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S2_RES_DIG_N_V10], "",  "%f", (float)0.0, (float)1000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O_SENS_SEL], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWS], 3, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I1_V1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I1_V1], arrUnit[ID_MILLI_AMPERE],  "%f", (float)4.0, (float)20.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P1],arrUnit[ID_BAR],  "%f", (float)0.0, (float)10.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I2_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I2_V2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I2_V2], arrUnit[ID_MILLI_AMPERE],  "%f", (float)4.0, (float)20.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P2], arrUnit[ID_BAR],  "%f", (float)0.0, (float)10.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I3_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I3_V3), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I3_V3],arrUnit[ID_MILLI_AMPERE],  "%f", (float)4.0, (float)20.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P3), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P3], arrUnit[ID_BAR],  "%f", (float)0.0, (float)10.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I4_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I4_V4), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I4_V4], arrUnit[ID_MILLI_AMPERE], "%f", (float)4.0, (float)20.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P4), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P4], arrUnit[ID_BAR],  "%f", (float)0.0, (float)10.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I5_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I5_V5), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I5_V5], arrUnit[ID_MILLI_AMPERE],  "%f", (float)4.0, (float)20.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P5), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P5], arrUnit[ID_BAR],  "%f", (float)0.0, (float)10.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I6_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I6_V6), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I6_V6], arrUnit[ID_MILLI_AMPERE],  "%f", (float)4.0, (float)20.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P6), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P6], arrUnit[ID_BAR],  "%f", (float)0.0, (float)10.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I7_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I7_V7), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I7_V7], arrUnit[ID_MILLI_AMPERE],  "%f", (float)4.0, (float)20.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P7), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P7], arrUnit[ID_BAR],  "%f", (float)0.0, (float)10.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I8_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I8_V8), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I8_V8], arrUnit[ID_MILLI_AMPERE],  "%f", (float)4.0, (float)20.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P8), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P8], arrUnit[ID_BAR],  "%f", (float)0.0, (float)10.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I9_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I9_V9), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I9_V9], arrUnit[ID_MILLI_AMPERE],  "%f", (float)4.0, (float)20.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P9), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P9], arrUnit[ID_BAR],  "%f", (float)0.0, (float)10.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I10_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_I10_V10), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_I10_V10], arrUnit[ID_MILLI_AMPERE],  "%f", (float)4.0, (float)20.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_P10), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S3_DIG_O_P10], arrUnit[ID_BAR],  "%f", (float)0.0, (float)10.0,(float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P_SENS_SEL], 3, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_INPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)180, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SHUTDOWN), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_SHUTDOWN], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SHUTDOWN_THRESHOLD] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SHUTDOWN_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_SHUTDOWN_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint8_t)0, (uint8_t)78, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_NOTIFICATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_NOTIFICATION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_NOTIFICATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_NOTIFICATION_THRESHOLD] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_NOTIFICATION_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_NOTIFICATION_THRESHOLD], arrUnit[ID_PERCENT], "%u",(uint8_t)2, (uint8_t)80,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_FUEL_THEFT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_FUEL_THEFT_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_FUEL_THEFT_WARNING],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_FUEL_THEFT_THRESHOLD] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_FUEL_THEFT_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_FUEL_THEFT_THRESHOLD], "%/hour",  "%u", (uint8_t)1, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_FUEL_CONSUMPTION] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_FUEL_CONSUMPTION), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_FUEL_CONSUMPTION], "Litre/hour",  "%f", (float)0, (float)100.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_FUEL_IN_PCT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_FUEL_IN_PCT), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_FUEL_IN_PCT],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_LOW_VTG] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_LOW_VTG), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_SENSOR_LOW_VTG], arrUnit[ID_V],  "%.2f",(float)0, (float)5.00,(float)0.01, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_HIGH_VTG] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_HIGH_VTG), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_SENSOR_HIGH_VTG],arrUnit[ID_V],  "%.2f", (float)0, (float)5.00,(float)0.01, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_WITH_STEP] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_WITH_STEP), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_WITH_STEP],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_WIDTH] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_WIDTH), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_WIDTH], arrUnit[ID_MM], "%u", (uint16_t)0, (uint16_t)3000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_LENGTH_1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_LENGTH_1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_LENGTH_1], arrUnit[ID_MM],  "%u", (uint16_t)0, (uint16_t)3000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_HEIGHT_1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_HEIGHT_1), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_HEIGHT_1], arrUnit[ID_MM],  "%u", (uint16_t)0, (uint16_t)3000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_LENGTH_2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_LENGTH_2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_LENGTH_2], arrUnit[ID_MM],  "%u", (uint16_t)0, (uint16_t)3000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_HEIGHT_2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_HEIGHT_2), strLeafNode[_u8LanguageArrayIndex][SID_AUX_S4_DIG_P_TANK_HEIGHT_2], arrUnit[ID_MM],  "%u", (uint16_t)0, (uint16_t)3000, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_A_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_A_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_A_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_A_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_A_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_A_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_B_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_B_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_B_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_B_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_B_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_B_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_C_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_C_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_C_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_C_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_C_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_C_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_D_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_D_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_D_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_D_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_D_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_D_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_E_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_E_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_E_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_E_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_E_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_E_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_F_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_F_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_F_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_F_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_F_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_F_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_G_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_G_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_G_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_G_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_G_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_G_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_H_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_H_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_H_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_H_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_H_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_H_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_I_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_I_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_I_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_I_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_I_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_I_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_J_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_J_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_J_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_J_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_J_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_J_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_K_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_K_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_K_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_K_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_K_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_K_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_L_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_L_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_L_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_L_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_L_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_L_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_M_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_M_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_M_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_M_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_M_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_M_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_N_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_N_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_N_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_N_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_N_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_N_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_O_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_O_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_O_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_O_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_O_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_O_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_OUT_P_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_P_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_OUT_P_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], CFGZ::CFGZ_OUTPUT_LAST, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OUT_P_ON_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OUT_P_ON_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_OUT_P_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_CRANKING_TIMER_CRANK_HOLD_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_HOLD_TIME), strLeafNode[_u8LanguageArrayIndex][SID_CRANKING_TIMER_CRANK_HOLD_TIME], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)10, CEditableItem::PIN1_ALLOWED );//8
    ArrEditableItem[INDEX_OF_CRANKING_TIMER_CRANK_REST_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_REST_TIME), strLeafNode[_u8LanguageArrayIndex][SID_CRANKING_TIMER_CRANK_REST_TIME], arrUnit[ID_SEC], "%u", (uint16_t)2, (uint16_t)60, CEditableItem::PIN1_ALLOWED );//8
    ArrEditableItem[INDEX_OF_CRANKING_TIMER_MANUAL_START_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_MANUAL_START_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_CRANKING_TIMER_MANUAL_START_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)10, CEditableItem::PIN1_ALLOWED );//8
    ArrEditableItem[INDEX_OF_CRANKING_TIMER_AUTO_START_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_AUTO_START_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_CRANKING_TIMER_AUTO_START_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)10, CEditableItem::PIN1_ALLOWED );//8

    ArrEditableItem[INDEX_OF_GENERAL_TIMER_SAFETY_MONITOR_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SAFETY_MONITOR_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_SAFETY_MONITOR_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)10, (uint16_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_MAINS_DETECT_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_MAINS_DETECT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_MAINS_DETECT_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)Mains_Detect_delay_min, (uint16_t)Mains_Detect_delay_max, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_ALT_DETECT_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_ALT_DETECT_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)30, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_WARM_UP_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_WARM_UP_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_WARM_UP_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)10, (uint16_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_RETN_TO_MAINS_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_RETN_TO_MAINS_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_RETN_TO_MAINS_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)59, (uint16_t)600, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_ENG_COOL_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ENG_COOL_TIME),  strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_ENG_COOL_TIME], arrUnit[ID_SEC], "%u", (uint16_t)59, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_STOP_ACTION_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_STOP_ACTION_TIME), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_STOP_ACTION_TIME], arrUnit[ID_SEC], "%u", (uint16_t)10, (uint16_t)120, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_ADDN_STOPPING_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ADDN_STOPPING_TIME), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_ADDN_STOPPING_TIME], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)120, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_LOAD_TRANSFER_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_LOAD_TRANSFER_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_LOAD_TRANSFER_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_PWR_SAVE_MODE_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_PWR_SAVE_MODE_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_PWR_SAVE_MODE_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)120, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_SCRN_CHNGOVER_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SCRN_CHNGOVER_TIME), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_SCRN_CHNGOVER_TIME], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_SOUNDER_ALARM_TIMER] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SOUNDER_ALARM_TIMER),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_SOUNDER_ALARM_TIMER], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)300, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_TIMER_TEST_MODE_TIMER] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_TEST_MODE_TIMER),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER_TEST_MODE_TIMER], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_ALT_CONFIG_ALT_PRESENT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_ALT_PRESENT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_NUMBER_OF_POLES] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_NUMBER_OF_POLES),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_NUMBER_OF_POLES], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ALT_POLES], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_ALT_AC_SYSTEM] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_ALT_AC_SYSTEM], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_NO_OF_PHASES], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_MIN_HEALTHY_VOLT] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_MIN_HEALTHY_VOLT), strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_MIN_HEALTHY_VOLT], arrUnit[ID_V_PH_N], "%f", (uint16_t)180, (uint16_t)270, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_MIN_HEALTHY_FREQ] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_MIN_HEALTHY_FREQ), strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_MIN_HEALTHY_FREQ], arrUnit[ID_HZ], "%u", (uint8_t)44, (uint8_t)56, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_PHASE_REVERSAL_DETECT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_PHASE_REVERSAL_DETECT),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_PHASE_REVERSAL_DETECT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_PHASE_REVERSAL_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_PHASE_REVERSAL_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_PHASE_REVERSAL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWES], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_AUTO_LOAD_TRANSFER] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_AUTO_LOAD_TRANSFER),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_AUTO_LOAD_TRANSFER], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ALT_CONFIG_ALT_WAVE_DETECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_WAVE_DETECTION),strLeafNode[_u8LanguageArrayIndex][SID_ALT_CONFIG_ALT_WAVE_DETECTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD], arrUnit[ID_V_PH_N], "%u", (uint16_t)180, (uint16_t)225, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_UNDER_VOLT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_WARNING),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_UNDER_VOLT_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_UV_WARNING_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UV_WARNING_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_UV_WARNING_THRESHOLD], arrUnit[ID_V_PH_N], "%u", (uint16_t)185, (uint16_t)230, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_OVER_VOLT_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD], arrUnit[ID_V_PH_N], "%u", (uint16_t)235, (uint16_t)285, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_OVER_VOLT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_WARNING),strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_OVER_VOLT_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_VOLT_MONITOR_OV_WARNING_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OV_WARNING_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_VOLT_MONITOR_OV_WARNING_THRESHOLD], arrUnit[ID_V_PH_N], "%u", (uint16_t)230, (uint16_t)280, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_UF_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UF_SHUTDOWN_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_UF_SHUTDOWN_THRESHOLD], arrUnit[ID_HZ], "%f", (float)44.0, (float)48.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_UNDER_FREQ_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UNDER_FREQ_WARNING),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_UNDER_FREQ_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_UF_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UF_WARNING_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_UF_WARNING_THRESHOLD], arrUnit[ID_HZ], "%f", (float)45.0, (float)49.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_OVER_FREQ_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OVER_FREQ_SHUTDOWN),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_OVER_FREQ_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_OF_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OF_SHUTDOWN_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_OF_SHUTDOWN_THRESHOLD], arrUnit[ID_HZ], "%f", (float)52.0, (float)56.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_OVER_FREQ_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OVER_FREQ_WARNING),strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_OVER_FREQ_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FREQ_MONITOR_OF_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OF_WARNING_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_FREQ_MONITOR_OF_WARNING_THRESHOLD], arrUnit[ID_HZ], "%f", (float)51.0, (float)55.0,(float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_LOAD_CT_RATIO] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_LOAD_CT_RATIO),strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_LOAD_CT_RATIO], "/5", "%u", (uint16_t)0, (uint16_t)8000, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_OVER_CURR_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_OVER_CURR_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_OVER_CURR_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWES], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_OVER_CURR_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_OVER_CURR_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_OVER_CURR_THRESHOLD], "A", "%u", (uint16_t)5, (uint16_t)10000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_OVER_CURR_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_OVER_CURR_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_OVER_CURR_DELAY], arrUnit[ID_SEC], "%u",(uint16_t)1, (uint16_t)600,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_CT_CORRECTION_FACTOR] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_CORRECTION_FACTOR),strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_CT_CORRECTION_FACTOR],"", "%.3f", (float)0.900, (float)1.100,(float)0.001, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CURRENT_MONITOR_CT_LOCATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_LOCATION),strLeafNode[_u8LanguageArrayIndex][SID_CURRENT_MONITOR_CT_LOCATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_CT_LOCATION], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_FAN_CURRENT_MON] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_FAN_CURRENT_MON),strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_FAN_CURRENT_MON], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_FAN_MON_CT_RATIO] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_FAN_MON_CT_RATIO),strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_FAN_MON_CT_RATIO], "/5", "%u", (uint16_t)0, (uint16_t)8000, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_HIGH_CURR_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_HIGH_CURR_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_HIGH_CURR_THRESHOLD], "A", "%f", (float)0.1, (float)10.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_HIGH_CURR_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_HIGH_CURR_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_HIGH_CURR_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_LOW_CURR_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_LOW_CURR_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_LOW_CURR_THRESHOLD], "A", "%f", (float)0, (float)9.9,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_LOW_CURR_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_LOW_CURR_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_LOW_CURR_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_CURR_MON_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FAN_CURR_MONITOR_CURR_MON_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_FAN_CURR_MONITOR_CURR_MON_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)180, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_LOAD_MONITOR_GEN_RATING] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_GEN_RATING), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_GEN_RATING], arrUnit[ID_KW], "%u", (uint16_t)0, (uint16_t)800, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_FULL_LOAD_CURRENT] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_FULL_LOAD_CURRENT), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_FULL_LOAD_CURRENT], "Amp/phase", "%u", (uint16_t)1, (uint16_t)800, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_OVERLOAD_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_OVERLOAD_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_OVERLOAD_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWES], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_OVERLOAD_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_OVERLOAD_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_OVERLOAD_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint16_t)100, (uint16_t)150, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_OVERLOAD_MON_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_OVERLOAD_MON_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_OVERLOAD_MON_DELAY], arrUnit[ID_SEC], "%u",(uint16_t)1, (uint16_t)60,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_UNBAL_LOAD_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_UNBAL_LOAD_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWES], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_UNBAL_LOAD_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_UNBAL_LOAD_THRESHOLD], arrUnit[ID_PERCENT], "%u", (uint16_t)5, (uint16_t)100, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_UNBAL_LOAD_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_UNBAL_LOAD_DELAY], arrUnit[ID_SEC], "%u",(uint16_t)1, (uint16_t)60,CEditableItem::PIN1_ALLOWED );
    /*
     * Shibu Kumar 08-08-2023
     * As discussed with mihir , Unbalance load activation threashold range should be same as GC1114.
     * So lower limit is changed from 50 to 0.
     */
    ArrEditableItem[INDEX_OF_LOAD_MONITOR_UNBAL_LOAD_ACT_THRESH] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_ACT_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_LOAD_MONITOR_UNBAL_LOAD_ACT_THRESH], arrUnit[ID_PERCENT], "%u", (uint16_t)0, (uint16_t)100, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_MAINS_CONFIG_MAINS_MONITORING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_CONFIG_MAINS_MONITORING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_CONFIG_MAINS_AC_SYSTEM] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_CONFIG_MAINS_AC_SYSTEM], "", "%s", strOptions[_u8LanguageArrayIndex][ID_NO_OF_PHASES], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_CONFIG_PHASE_REVERSAL_DETECT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_PHASE_REVERSAL_DETECT),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_CONFIG_PHASE_REVERSAL_DETECT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_CONFIG_PHASE_REVERSAL_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_PHASE_REVERSAL_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_CONFIG_PHASE_REVERSAL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoN], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH),strLeafNode[_u8LanguageArrayIndex][SID_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_UNDER_VOLT_MON_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_ENABLE),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_VOLT_MON_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_UNDER_VOLT_MON_TRIP] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_TRIP), strLeafNode[_u8LanguageArrayIndex][SID_UNDER_VOLT_MON_TRIP], arrUnit[ID_V_PH_N], "%u", (uint16_t)Mains_UV_Trip_min, (uint16_t)Mains_UV_Trip_max, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_UNDER_VOLT_MON_RETURN] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_RETURN), strLeafNode[_u8LanguageArrayIndex][SID_UNDER_VOLT_MON_RETURN], arrUnit[ID_V_PH_N], "%u", (uint16_t)Mains_UV_return_min, (uint16_t)Mains_UV_return_max, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_VOLT_MON_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_VOLT_MON_ENABLE),strLeafNode[_u8LanguageArrayIndex][SID_OVER_VOLT_MON_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_VOLT_MON_TRIP] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_VOLT_MON_TRIP), strLeafNode[_u8LanguageArrayIndex][SID_OVER_VOLT_MON_TRIP], arrUnit[ID_V_PH_N], "%u", (uint16_t)Mains_OV_Trip_min, (uint16_t)Mains_OV_Trip_max, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_VOLT_MON_RETURN] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_VOLT_MON_RETURN), strLeafNode[_u8LanguageArrayIndex][SID_OVER_VOLT_MON_RETURN], arrUnit[ID_V_PH_N], "%u", (uint16_t)Mains_OV_return_min, (uint16_t)Mains_OV_return_max, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_UNDER_FREQ_MON_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_FREQ_MON_ENABLE),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_FREQ_MON_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_UNDER_FREQ_MON_TRIP] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_FREQ_MON_TRIP),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_FREQ_MON_TRIP],arrUnit[ID_HZ], "%f", (float)40.0, (float)48.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_UNDER_FREQ_MON_RETURN] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_UNDER_FREQ_MON_RETURN),strLeafNode[_u8LanguageArrayIndex][SID_UNDER_FREQ_MON_RETURN],arrUnit[ID_HZ], "%f", (float)41.0, (float)49.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_FREQ_MON_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_FREQ_MON_ENABLE),strLeafNode[_u8LanguageArrayIndex][SID_OVER_FREQ_MON_ENABLE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_FREQ_MON_TRIP] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_FREQ_MON_TRIP),strLeafNode[_u8LanguageArrayIndex][SID_OVER_FREQ_MON_TRIP],arrUnit[ID_HZ], "%f", (float)52.0, (float)60.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OVER_FREQ_MON_RETURN] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_OVER_FREQ_MON_RETURN),strLeafNode[_u8LanguageArrayIndex][SID_OVER_FREQ_MON_RETURN],arrUnit[ID_HZ], "%f", (float)51.0, (float)59.0,(float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_CRANK_DISCONN_START_ATTEMPTS] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_START_ATTEMPTS), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONN_START_ATTEMPTS], "", "%u", (uint8_t)1, (uint8_t)10, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONN_DISCONN_ON_LOP_SENS] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_DISCONN_ON_LOP_SENS),strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONN_DISCONN_ON_LOP_SENS], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONN_MON_LOP_BEFORE_CRANK] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_MON_LOP_BEFORE_CRANK), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONN_MON_LOP_BEFORE_CRANK], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONN_DISCONN_LOP_SENS] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_DISCONN_LOP_SENS),strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONN_DISCONN_LOP_SENS],arrUnit[ID_BAR], "%f", (float)0.5, (float)10.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONN_MON_LLOP_BEFORE_CRNK] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_MON_LLOP_BEFORE_CRNK),strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONN_MON_LLOP_BEFORE_CRNK], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONN_DISCONN_ON_LLOP_SW] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_DISCONN_ON_LLOP_SW),strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONN_DISCONN_ON_LLOP_SW], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONN_LLOP_SW_TRANS_TIME] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_LLOP_SW_TRANS_TIME),strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONN_LLOP_SW_TRANS_TIME],arrUnit[ID_SEC], "%f", (float)0.0, (float)6.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONN_ALT_FREQUENCY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_ALT_FREQUENCY), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONN_ALT_FREQUENCY], arrUnit[ID_HZ], "%u",(uint16_t)10, (uint16_t)25,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONN_ENGINE_SPEED] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_ENGINE_SPEED),strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONN_ENGINE_SPEED], arrUnit[ID_RPM], "%u", (uint16_t)150, (uint16_t)800, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONN_DISCONN_ON_CHG_ALT_VOLT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_DISCONN_ON_CHG_ALT_VOLT), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONN_DISCONN_ON_CHG_ALT_VOLT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CRANK_DISCONN_CHG_ALT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_CHG_ALT_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_CRANK_DISCONN_CHG_ALT_THRESHOLD], arrUnit[ID_V], "%f", (float)5.0, (float)30.0, (float)0.1,CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_SPEED_MONITOR_SPEED_SENSE_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_SPEED_SENSE_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_SPEED_SENSE_SOURCE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_SPEED_SENSE_SOURCE_ALT_F], 1, CEditableItem::PIN1_ALLOWED );
    /*this "SPEED_MONITOR_RESERVED" param not in smart config*/
    /*ArrEditableItem[INDEX_OF_SPEED_MONITOR_RESERVED] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_RESERVED),strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_RESERVED], "", "%u", (uint8_t)1,(uint8_t) 255, CEditableItem::PIN1_ALLOWED );*/
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_UNDER_SPEED_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD], arrUnit[ID_RPM], "%u",(uint16_t)1300, (uint16_t)1500,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_UNDER_SPEED_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_UNDER_SPEED_DELAY], arrUnit[ID_SEC], "%u",(uint16_t)1, (uint16_t)60,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_OVER_SPEED_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_OVER_SPEED_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_OVER_SPEED_THRESHOLD], arrUnit[ID_RPM], "%u",(uint16_t)1500, (uint16_t)1680,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_OVER_SPEED_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_OVER_SPEED_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_OVER_SPEED_DELAY], arrUnit[ID_SEC], "%u",(uint16_t)1, (uint16_t)20,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SPEED_MONITOR_GROSS_OS_THRESHOLD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_GROSS_OS_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SPEED_MONITOR_GROSS_OS_THRESHOLD], arrUnit[ID_PERCENT], "%u",(uint16_t)100, (uint16_t)200,CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_MON_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_MON_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_MON_SOURCE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_BATT_MON_SRC], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_LOW_VOLT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_LOW_VOLT_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_LOW_VOLT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_LOW_VOLT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_LOW_VOLT_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_LOW_VOLT_THRESHOLD], arrUnit[ID_V], "%f", (float)8.0, (float)31.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_LOW_VOLT_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_LOW_VOLT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_LOW_VOLT_DELAY], arrUnit[ID_SEC], "%u",(uint16_t)5, (uint16_t)1800,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_HIGH_VOLT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_HIGH_VOLT_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_HIGH_VOLT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_HIGH_VOLT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_HIGH_VOLT_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_HIGH_VOLT_THRESHOLD], arrUnit[ID_V], "%f", (float)9.0, (float)32.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_BATTERY_MONITOR_HIGH_VOLT_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_HIGH_VOLT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_BATTERY_MONITOR_HIGH_VOLT_DELAY], arrUnit[ID_SEC], "%u",(uint16_t)5, (uint16_t)1800,CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_CHARGE_ALT_MON_FAIL_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_CHARGE_ALT_MON_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWESN], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CHARGE_ALT_MON_FAIL_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_CHARGE_ALT_MON_FAIL_THRESHOLD], arrUnit[ID_V], "%f", (float)0.0, (float)35.0, (float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CHARGE_ALT_MON_FAIL_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_CHARGE_ALT_MON_FAIL_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)60,CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_PREHEAT_PREHEAT_TIMER] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_PREHEAT_TIMER),strLeafNode[_u8LanguageArrayIndex][SID_PREHEAT_PREHEAT_TIMER], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)900, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_PREHEAT_ENG_TEMPERATURE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_ENG_TEMPERATURE),strLeafNode[_u8LanguageArrayIndex][SID_PREHEAT_ENG_TEMPERATURE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_PREHEAT_ENG_TEMP_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_ENG_TEMP_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_PREHEAT_ENG_TEMP_THRESHOLD], arrUnit[ID_DEG_C], "%.0f", (float)-5, (float)300, (float)1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_PREHEAT_AMB_TEMPERATURE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_AMB_TEMPERATURE),strLeafNode[_u8LanguageArrayIndex][SID_PREHEAT_AMB_TEMPERATURE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );


    ArrEditableItem[INDEX_OF_ENGINE_TYPE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE),strLeafNode[_u8LanguageArrayIndex][SID_ENGINE_TYPE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENGINE_TYPE],2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG),strLeafNode[_u8LanguageArrayIndex][SID_LOP_FROM_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG),strLeafNode[_u8LanguageArrayIndex][SID_CLNT_TEMP_FROM_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_SPEED_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_SPEED_FROM_ENG),strLeafNode[_u8LanguageArrayIndex][SID_ENG_SPEED_FROM_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_RUN_HOURS_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_RUNNING_HOURS_FROM_ECU),strLeafNode[_u8LanguageArrayIndex][SID_ENG_RUN_HOURS_FROM_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_BAT_VTG_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BAT_VTG_FROM_ECU),strLeafNode[_u8LanguageArrayIndex][SID_BAT_VTG_FROM_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_OIL_TEMP_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OIL_TEMP_FROM_ECU),strLeafNode[_u8LanguageArrayIndex][SID_OIL_TEMP_FROM_ECU], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_COMM_FAILURE_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_COMM_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ECU_ALARM_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_COMM_FAILURE_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_COMM_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACT_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_COMM_FAILURE_ACT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_COMM_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)1, (uint8_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AMBER_LAMP_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_AMBER_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_AMBER_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ECU_ALARM_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_AMBER_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_AMBER_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_AMBER_ACT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_AMBER_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_RED_LAMP_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_RED_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_RED_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ECU_ALARM_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_RED_LAMP_ACT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_RED_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_RED_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_RED_LAMP_ACT_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_RED_ACT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_RED_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_MALFUNCTION_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_MALFUNC_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ECU_ALARM_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_MALFUNCTION_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_MALFUNC_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_MALFUNCTION_ACT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_MALFUNC_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_PROTECT_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_PROTECT_FAIL_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ECU_ALARM_ACTION], 5, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_PROTECT_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_PROTECT_FAIL_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ECU_PROTECT_ACT_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_PROTECT_FAIL_ACT_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ:: ID_SGC_SOURCE_ADDRESS), strLeafNode[_u8LanguageArrayIndex][SID_SGC_SOURCE_ADDR], "", "%u", (uint8_t)0, (uint8_t)253, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ECU_SOURCE_ADDRESS] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ:: ID_ECU_SOURCE_ADDRESS), strLeafNode[_u8LanguageArrayIndex][SID_ECU_SOURCE_ADDR], "", "%u", (uint8_t)0, (uint8_t)253, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_LOP_SHUTDOWN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_SHUTDOWN_EN), strLeafNode[_u8LanguageArrayIndex][SID_LOP_SENS_ECU_SHUTDOWN], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SHUTDOWN_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_SHUTDOWN_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_LOP_SENS_ECU_SHUTDOWN_THRESH], "Bar", "%f", (float)0.0, (float)9.8 , CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_WARNING_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_WARNING_EN), strLeafNode[_u8LanguageArrayIndex][SID_LOP_SENS_ECU_WARNING], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_WARNING_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_WARNING_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_LOP_SENS_ECU_WARNING_THRESH], "Bar", "%f", (float)0.2, (float)10.0 , CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_HWT_SHUTDOWN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_SHUTDOWN_EN),strLeafNode[_u8LanguageArrayIndex][SID_CLNT_TEMP_SHUTDOWN_EN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_HWT_SHUTDOWN_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CLNT_TEMP_SHUTDOWN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_CLNT_TEMP_SHUTDOWN_THRESH], arrUnit[ID_DEG_C], "%.0f", (float)26, (float)200, (float)1, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_HWT_WARNING_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_WARNING_EN),strLeafNode[_u8LanguageArrayIndex][SID_CLNT_TEMP_WARNING_EN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_HWT_WARNING_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CLNT_TEMP_WARNING_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_CLNT_TEMP_WARNING_THRESH], arrUnit[ID_DEG_C], "%.0f", (float)25, (float)199, (float)1, CEditableItem::PIN1_ALLOWED);

    ArrEditableItem[INDEX_OF_HIGH_OIL_TEMP_SHUTDOWN_EN_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OIL_TEMP_FROM_ECU_SHUTDOWN_EN),strLeafNode[_u8LanguageArrayIndex][SID_OIL_TEMP_SHUTDOWN_EN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_HIGH_OIL_TEMP_SHUTDOWN_THRESH_FROM_ECU] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_OIL_TEMP_FROM_ECU_SHUTDOWN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_OIL_TEMP_SHUTDOWN_THRESH], arrUnit[ID_DEG_C], "%.0f", (float)26, (float)200, (float)1, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_HIGH_OIL_TEMP_WARNING_EN_FROM_ECU] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_OIL_TEMP_FROM_ECU_WARNING_EN),strLeafNode[_u8LanguageArrayIndex][SID_OIL_TEMP_WARNING_EN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_HIGH_OIL_TEMP_WARNING_THRESH_FROM_ECU] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_OIL_TEMP_FROM_ECU_WARNING_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_OIL_TEMP_WARNING_THRESH], arrUnit[ID_DEG_C], "%.0f", (float)25, (float)199, (float)1, CEditableItem::PIN1_ALLOWED);

    ArrEditableItem[INDEX_OF_CANOPY_TEMP_SHUTDOWN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CANOPY_TEMP_SHUTDOWN_EN),strLeafNode[_u8LanguageArrayIndex][SID_CANOPY_TEMP_SHUTDOWN_EN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CANOPY_TEMP_SHUTDOWN_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CANOPY_TEMP_SHUTDOWN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_CANOPY_TEMP_SHUTDOWN_THRESH], arrUnit[ID_DEG_C], "%.0f", (float)-5, (float)300, (float)1, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_CANOPY_TEMP_WARNING_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CANOPY_TEMP_WARNING_EN),strLeafNode[_u8LanguageArrayIndex][SID_CANOPY_TEMP_WARNING_EN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE],2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_CANOPY_TEMP_WARNING_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CANOPY_TEMP_WARNING_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_CANOPY_TEMP_WARNING_THRESH], arrUnit[ID_DEG_C], "%.0f", (float)-5, (float)298, (float)1, CEditableItem::PIN1_ALLOWED);

    ArrEditableItem[INDEX_OF_ENABLE_EGR_TIMERS] = CEditableItem((uint32_t)_objcfgz.GetProductSpecificData(CFGZ::PS_EGR_TIMERS_ENABLE),strLeafNode[_u8LanguageArrayIndex][SID_ENABLE_EGR_TIMERS], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN3_ALLOWED );
    ArrEditableItem[INDEX_OF_FAULT_SHUTDOWN_TIMER] = CEditableItem((uint16_t)_objcfgz.GetProductSpecificData(CFGZ::PS_EGR_CONFIGURED_SHUTDOWN_TIMER), strLeafNode[_u8LanguageArrayIndex][SID_FAULT_SHUTDOWN_TIMER], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)FAULT_PRESENT_MONITORING_TIME_MINUTES, CEditableItem::PIN3_ALLOWED);
    ArrEditableItem[INDEX_OF_FAULT_WARNING_TIMER] = CEditableItem((uint16_t)_objcfgz.GetProductSpecificData(CFGZ::PS_EGR_CONFIGURED_WARNING_TIMER), strLeafNode[_u8LanguageArrayIndex][SID_FAULT_WARNING_TIMER], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)EGR_WARNING_INDUCEMENT_LEVEL_TIME, CEditableItem::PIN3_ALLOWED);
    ArrEditableItem[INDEX_OF_HEAL_TIMER] = CEditableItem((uint16_t)_objcfgz.GetProductSpecificData(CFGZ::PS_EGR_CONFIGURED_HEAL_TIMER), strLeafNode[_u8LanguageArrayIndex][SID_HEAL_TIMER], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)FAULT_RESET_MONITORING_TIME_MINUTES, CEditableItem::PIN3_ALLOWED);

    ArrEditableItem[INDEX_OF_MAINT_ALARM_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINT_ALARM_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_MAINT_ALARM_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTION_NW], 2, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_MAINT_ALARM_DUE_AT_ENGINE_HOURS] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MAINT_ALARM_DUE_AT_ENGINE_HOURS),strLeafNode[_u8LanguageArrayIndex][SID_MAINT_ALARM_DUE_AT_ENGINE_HOURS], arrUnit[ID_HRS], "%u", (uint16_t)10, (uint16_t)65000, CEditableItem::PIN1_ALLOWED );

    _stMaintenanceDt ={(uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_1),(uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_2),(uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_3)};
    ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE] = CEditableItem((CEditableItem::DATE_t)_stMaintenanceDt, "", "","%u",(CEditableItem::DATE_t){1,1,2024}, (CEditableItem::DATE_t){31,12,2080},CEditableItem::PIN1_ALLOWED );

    if(_MiscParam.u8MiscParam[PROFILE_NO] > MAX_NUMBER_OF_PROFILE)
    {
        _MiscParam.u8MiscParam[PROFILE_NO] =0;
    }

    ArrEditableItem[INDEX_OF_ACTIVE_PROFILE] = CEditableItem((uint32_t)_MiscParam.u8MiscParam[PROFILE_NO],strLeafNode[_u8LanguageArrayIndex][SID_PROFILE_NAME], "", "%s",(const char**)_arrProfileNames, MAX_NUMBER_OF_PROFILE, CEditableItem::PIN1_ALLOWED );

    stPIN_1 = {0,0,0,0};
    stPIN_2 = {0,0,0,0};

    ArrEditableItem[INDEX_OF_GENSET_SR_NO]  = CEditableItem((uint8_t*)_stSrNos.u8GenSrNo, strLeafNode[_u8LanguageArrayIndex][SID_GENSET_SR_NO], "", "%u", CEditableItem::PIN1_ALLOWED, CEditableItem::SRNO_GENSET );
    ArrEditableItem[INDEX_OF_ENGINE_SR_NO]  = CEditableItem((uint8_t*)_stSrNos.u8EngSrNo, strLeafNode[_u8LanguageArrayIndex][SID_ENGINE_SR_NO], "", "%u", CEditableItem::PIN1_ALLOWED, CEditableItem::SRNO_ENGINE );
    ArrEditableItem[INDEX_OF_ALTERNATOR_SR_NO]  = CEditableItem((uint8_t*)_stSrNos.u8AltSrNo, strLeafNode[_u8LanguageArrayIndex][SID_ALTERNATOR_SR_NO], "", "%u", CEditableItem::PIN1_ALLOWED, CEditableItem::SRNO_ALT );
    ArrEditableItem[INDEX_OF_MAIN_CONTROLLER_SR_NO]  = CEditableItem((uint8_t*)_stSrNos.u8MainContSrNo, strLeafNode[_u8LanguageArrayIndex][SID_MAIN_CONTROLLER_SR_NO], "", "%u", CEditableItem::NOT_ALLOWED, CEditableItem::SRNO_MAINCONT );
    ArrEditableItem[INDEX_OF_ENGINE_CONTROLLER_SR_NO]  = CEditableItem((uint8_t*)_stSrNos.u8EngContSrNo, strLeafNode[_u8LanguageArrayIndex][SID_ENGINE_CONTROLLER_SR_NO], "", "%u", CEditableItem::PIN1_ALLOWED, CEditableItem::SRNO_ENGCONT );
    ArrEditableItem[INDEX_OF_SITE_ID]  = CEditableItem((uint8_t*)_stSrNos.u8SiteId, strLeafNode[_u8LanguageArrayIndex][SID_SITE_ID], "", "%u", CEditableItem::PIN1_ALLOWED, CEditableItem::SRNO_SITEID );
    ArrEditableItem[INDEX_OF_PIN_1]  = CEditableItem((CEditableItem::PASSWORD_t)stPIN_1,"", "", "%u", (CEditableItem::PASSWORD_t){0,0,0,0}, (CEditableItem::PASSWORD_t){9,9,9,9}, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_PIN_2]  = CEditableItem((CEditableItem::PASSWORD_t)stPIN_2,"", "", "%u", (CEditableItem::PASSWORD_t){0,0,0,0},(CEditableItem::PASSWORD_t) {9,9,9,9}, CEditableItem::PIN1_PIN2_ALLOWED );

}

void UI::InitEditableItemsScreens()
{
    uint16_t k =  INDEX_OF_GENERAL_PROFILE_NAME;
    for(uint8_t i=0;i<sizeof(ParamInSubmenus);i++)
    {
        for(uint8_t j=0;j<ParamInSubmenus[i];j++)
        {
            ArrEditableItemScreen[k] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][i], &ArrEditableItem[k], 10, 20);
            k++;
        }
    }
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

    for(int i=INDEX_OF_GENERAL_PROFILE_NAME; i<=INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE;i++)
    {
        menuItemsLowestLevel[i] = CMenuItem( &ArrEditableItemScreen[i]);
    }

    //menuItemsLowestLevel[INDEX_OF_ENG_SR_NO] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_ENG_SR_NO], &ArrEditableItemScreen[INDEX_OF_ENG_SR_NO]);
    menuItemsLowestLevel[INDEX_OF_GENSET_SR_NO] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_SR_NO_DATA], &ArrEditableItemScreen[INDEX_OF_GENSET_SR_NO]);
    menuItemsLowestLevel[INDEX_OF_ENGINE_SR_NO] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_SR_NO_DATA], &ArrEditableItemScreen[INDEX_OF_ENGINE_SR_NO]);
    menuItemsLowestLevel[INDEX_OF_ALTERNATOR_SR_NO] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_SR_NO_DATA], &ArrEditableItemScreen[INDEX_OF_ALTERNATOR_SR_NO]);
    menuItemsLowestLevel[INDEX_OF_MAIN_CONTROLLER_SR_NO] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_SR_NO_DATA], &ArrEditableItemScreen[INDEX_OF_MAIN_CONTROLLER_SR_NO]);
    menuItemsLowestLevel[INDEX_OF_ENGINE_CONTROLLER_SR_NO] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_SR_NO_DATA], &ArrEditableItemScreen[INDEX_OF_ENGINE_CONTROLLER_SR_NO]);
    menuItemsLowestLevel[INDEX_OF_SITE_ID] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_SR_NO_DATA], &ArrEditableItemScreen[INDEX_OF_SITE_ID]);

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

    /*Need to disable Passwords in config UI*/
    menuItemsMidLevel[ID_PASSWORD_1].isEnabled = false;
    menuItemsMidLevel[ID_PASSWORD_2].isEnabled = false;
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

        _objHal.Objpflash.Read((_objcfgz.GetFactoryProfilesStartAddress()+ (i * (sizeof(CFGZ::CFGZ_PARAMS_t) - sizeof(uint8_t)*(DUMMY_ITEMS)))), (uint8_t*)&param, sizeof(CFGZ::CFGZ_PARAMS_t) );

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
    prvInitialiseECUParam();
    prvUpdateConfigStrings();
    prvUpdateAutomationModbusMap();
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
    CFGZ::LATEST_PRODUCT_SPECIFIC_DATA_t ProductParam ={};
    _objcfgz.GetProductSpecificData(&ProductParam);
    if(CEditableItem::IsAnyConfigValueEdited())
    {
        _objDisplay.ClearScreen();
        _objDisplay.drawRectangle();
        _objDisplay.gotoxy(GLCD_X(64),GLCD_Y(25));
        _objDisplay.printStringCenterAligned((char*)strSavingSettings[_u8LanguageArrayIndex], FONT_ARIAL);

        if( _MiscParam.u8MiscParam[PROFILE_NO] != ArrEditableItem[INDEX_OF_ACTIVE_PROFILE].value.u8Val)
        {
            _objHal.Objpflash.Read((_objcfgz.GetFactoryProfilesStartAddress()+ (ArrEditableItem[INDEX_OF_ACTIVE_PROFILE].value.u8Val
                    *( sizeof(CFGZ::CFGZ_PARAMS_t)- sizeof(uint8_t)*DUMMY_ITEMS))), (uint8_t*)&AllParam, sizeof(CFGZ::CFGZ_PARAMS_t) );
            _MiscParam.u8MiscParam[PROFILE_NO] = ArrEditableItem[INDEX_OF_ACTIVE_PROFILE].value.u8Val;
        }
        else
        {
            uint16_t id_float = (CFGZ::ID_BTS_CONFIG_LOW_BATT_THRESHOLD);
            uint16_t id_uint16 = (CFGZ::ID_BTS_CONFIG_LOW_BATT_MON_DELAY);
            uint16_t id_uint8 = (CFGZ::ID_GENERAL_PROFILE_NAME);
            for (uint16_t i = 0 ; i<=INDEX_OF_MAINT_ALARM_DUE_AT_ENGINE_HOURS ; i++)
            {
                if( (i>=INDEX_OF_ENABLE_EGR_TIMERS) && (i<=INDEX_OF_HEAL_TIMER) )
                {
                    continue;
                }
                if(ArrEditableItem[i].dataType == CEditableItem::DT_FLOAT)
                {
                    AllParam.f32ArrParam[id_float] = (float)ArrEditableItem[i].value.fVal;
                    id_float++;
                }
                else if((ArrEditableItem[i].dataType == CEditableItem::DT_UINT16) ||(ArrEditableItem[i].dataType ==CEditableItem::DT_TIME_HRS_MINS))
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
                    //The conditions that comes to this point of execution are not present.
                }
            }

            ProductParam.u16ProductParam[CFGZ:: PS_EGR_CONFIGURED_SHUTDOWN_TIMER] = (uint16_t)ArrEditableItem[INDEX_OF_FAULT_SHUTDOWN_TIMER].value.u16Val;
            ProductParam.u16ProductParam[CFGZ:: PS_EGR_CONFIGURED_WARNING_TIMER] = (uint16_t)ArrEditableItem[INDEX_OF_FAULT_WARNING_TIMER].value.u16Val;
            ProductParam.u16ProductParam[CFGZ:: PS_EGR_CONFIGURED_HEAL_TIMER] = (uint16_t)ArrEditableItem[INDEX_OF_HEAL_TIMER].value.u16Val;
            ProductParam.u8ProductParam[CFGZ:: PS_EGR_TIMERS_ENABLE] = (uint8_t)ArrEditableItem[INDEX_OF_ENABLE_EGR_TIMERS].value.u8Val;

            u8Month = (uint8_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE].value.stDate.u8Month;
            u16Year = (uint16_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE].value.stDate.u16Year;
            u8Date = (uint8_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE].value.stDate.u8Date;
            if((_stMaintenanceDt.u8Date!=u8Date)||(_stMaintenanceDt.u8Month!=u8Month)||(_stMaintenanceDt.u16Year!=u16Year))
            {
                if(u8Date > prvMaxDaysInMonth( u8Month, u16Year))
                {
                    AllParam.u8ArrParam[CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_1] = (uint8_t)_stMaintenanceDt.u8Date;
                    AllParam.u8ArrParam[CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_2] = (uint8_t)_stMaintenanceDt.u8Month;
                    AllParam.u16ArrParam[CFGZ:: ID_ALARM_DUE_DATE_SERVICE_DATE_3] = (uint16_t)_stMaintenanceDt.u16Year;
                    _objDisplay.gotoxy(GLCD_X(35),GLCD_Y(50));
                    _objDisplay.printStringCenterAligned((char *)StrMaintDate[0][0], FONT_VERDANA);
                }
                else
                {
                    AllParam.u8ArrParam[CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_1] = (uint8_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE].value.stDate.u8Date;
                    AllParam.u8ArrParam[CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_2] = (uint8_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE].value.stDate.u8Month;
                    AllParam.u16ArrParam[CFGZ:: ID_ALARM_DUE_DATE_SERVICE_DATE_3] = (uint16_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE].value.stDate.u16Year;
                    _objDisplay.gotoxy(GLCD_X(35),GLCD_Y(50));
                    _objDisplay.printStringCenterAligned((char *)StrMaintDate[0][1], FONT_VERDANA);
                }
            }
            else
            {
                AllParam.u8ArrParam[CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_1] = (uint8_t)_stMaintenanceDt.u8Date;
                AllParam.u8ArrParam[CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_2] = (uint8_t)_stMaintenanceDt.u8Month;
                AllParam.u16ArrParam[CFGZ:: ID_ALARM_DUE_DATE_SERVICE_DATE_3] = (uint16_t)_stMaintenanceDt.u16Year;
                _objDisplay.gotoxy(GLCD_X(35),GLCD_Y(50));
            }


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

        _MiscParam.u16CRC = CRC16::ComputeCRCGeneric((uint8_t *)&_MiscParam, sizeof(MISC_PARAM_t) -sizeof(uint16_t), CRC_MEMORY_SEED);

        _objHal.Objeeprom.RequestWrite(EXT_EEPROM_PASWORD_START_ADDRESS,(uint8_t*)&_MiscParam, sizeof(MISC_PARAM_t) , NULL) ;
        _objcfgz.WriteActiveProfile(&AllParam);

        if(bSrNosEdited)
        {
            bSrNosEdited = false;
            /*Sr nos are edited. Copy updated values into SrNos structure, calculate CRC and write into Eeprom*/
            uint16_t u16DataLen = (sizeof(PRODUCT_SR_NOS_t) - SR_NOS_HEADER_SIZE);
            memcpy((void*)((uint8_t*)&_stSrNos + SR_NOS_HEADER_SIZE), (uint8_t*)&(CEditableItem::u8SrNoArr[0].u8Arr[0]), u16DataLen);
            _stSrNos.u16Crc = CRC16::ComputeCRCGeneric((uint8_t *)&_stSrNos + (SR_NOS_HEADER_SIZE),
               u16DataLen, CRC_MEMORY_SEED);
            _objHal.Objeeprom.RequestWrite(SERIAL_NOS_AREA_START_ADDRESS, (uint8_t*)&_stSrNos, sizeof(PRODUCT_SR_NOS_t), NULL);
        }
        prvUpdateAutomationModbusMap();
    }
    _objcfgz.WriteProductSpecificData(&ProductParam);
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
            case CFGZ::CFGZ_BATTERY_CHARGER_FAIL:
            case CFGZ::CFGZ_SMOKE_FIRE:
            case CFGZ::CFGZ_EB_MCCB_ON_FEEDBACK:
            case CFGZ::CFGZ_DG_MCCB_ON_FEEDBACK:
            {
               //Do nothing
            }
            break;

            case CFGZ::CFGZ_LOW_FUEL_LEVEL_SWITCH:
            case CFGZ::CFGZ_LOW_LUBE_OIL_PRESS_SWITCH:
            case CFGZ::CFGZ_HIGH_ENGINE_TEMP_SWITCH:
            case CFGZ::CFGZ_LOW_WATER_LVL_SWITCH:
            {
                menuItemsLowestLevel[u16SourceIndex + 3].isEnabled =false;
            }
            break;

            case CFGZ::CFGZ_EMERGENCY_STOP:
            case CFGZ::CFGZ_SIMULATE_START:
            case CFGZ::CFGZ_SIMULATE_STOP:
            case CFGZ::CFGZ_SIMULATE_AUTO:
            case CFGZ::CFGZ_CLOSE_GEN_OPEN_MAINS_SWITCH:
            case CFGZ::CFGZ_CLOSE_MAINS_OPEN_GEN_SWITCH:
            case CFGZ::CFGZ_EGR_ECU_DIGITAL_IN:
            case CFGZ::CFGZ_SIMULATE_MAINS:
            {
                menuItemsLowestLevel[u16SourceIndex + 2].isEnabled =false;
                menuItemsLowestLevel[u16SourceIndex + 3].isEnabled =false;
                menuItemsLowestLevel[u16SourceIndex + 4].isEnabled =false;
                TurnOFFActionActivation(u16SourceIndex);
            }
            break;

            case CFGZ::CFGZ_REMOTE_START_STOP:
            case CFGZ::CFGZ_MAINS_CONTACTOR_LATCHED:
            case CFGZ::CFGZ_GEN_CONTACTOR_LATCHED:
            case CFGZ::CFGZ_MODE_SELECT:
            case CFGZ::CFGZ_AMB_TEMP_SELECT:
            {
                menuItemsLowestLevel[u16SourceIndex + 2].isEnabled =false;
                menuItemsLowestLevel[u16SourceIndex + 3].isEnabled =false;
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
    for(uint16_t i = INDEX_OF_OUT_A_SOURCE ; i<=INDEX_OF_OUT_P_SOURCE; i = i+2)
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
    /*
     * SuryaPranayTeja.BVV 16-11-2022
     * TODO :Handling Menu Visibility will create a problem on the top Screen Number/Total number display
     * if we try to hide or unhide a parameter which is above the decision parameter.
     * Example lets say:
     * Actual display order is Threshold and after that Enable parameter.
     * Based on the enable parameter if we hide the threshold , it will cause the problem.
     */
    LowestLevelMenuEnDis(INDEX_OF_GENERAL_PROFILE_NAME,INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE,true);
    menuItemsMidLevel[ID_PREHEAT_S].isEnabled =true;
    /*................................Module Menu........................................................................................................*/
    if(ArrEditableItem[INDEX_OF_MODBUS_COMM_COMM_MODE].value.u8Val == CFGZ::CFGZ_DISABLE)
        LowestLevelMenuEnDis(INDEX_OF_MODBUS_COMM_MODBUS_SLAVE_ID,INDEX_OF_MODBUS_COMM_MAP,false);

    if(ArrEditableItem[INDEX_OF_BTS_CONFIG_BATTERY_MON].value.u8Val == CFGZ::CFGZ_DISABLE)
        LowestLevelMenuEnDis(INDEX_OF_BTS_CONFIG_LOW_BATT_THRESHOLD,INDEX_OF_BTS_CONFIG_DG_RUN_DURATION,false);
    if(ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_CYCLIC_MODE].value.u8Val == CFGZ::CFGZ_DISABLE)
        LowestLevelMenuEnDis(INDEX_OF_CYCLIC_CONFIG_DG_OFF_DURATION,INDEX_OF_CYCLIC_CONFIG_DG_ON_DURATION,false);
    if(ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFG_NIGHT_MODE].value.u8Val == CFGZ::CFGZ_DISABLE)
        LowestLevelMenuEnDis(INDEX_OF_NIGHT_MODE_CONFG_START_TIME,INDEX_OF_NIGHT_MODE_CONFG_OFF_DURATION,false);
    if(ArrEditableItem[INDEX_OF_AUTO_MD_SWITCH_AUTO_MD_SWITCH].value.u8Val == CFGZ::CFGZ_DISABLE)
        LowestLevelMenuEnDis(INDEX_OF_AUTO_MD_SWITCH_MANUAL_MODE_TIME,INDEX_OF_AUTO_MD_SWITCH_OUTPUT_ON_TIME,false);
    //INPUTS MENU
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_A_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_B_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_C_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_D_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_E_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_F_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_G_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_H_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_I_SOURCE);

    //Sensor J
    if(ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE,INDEX_OF_LOP_RES_DIG_J_V10,false);
    if(ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE].value.u8Val == CFGZ::CFGZ_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_LOP_RES_DIG_J_DIG_POLARITY,INDEX_OF_LOP_RES_DIG_J_V10,false);
    if(ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE].value.u8Val != CFGZ::CFGZ_SENSOR_NOT_USED)
    {
        DigitalInputMenuVisiblity(INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE);
        LowestLevelMenuEnDis(INDEX_OF_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION,INDEX_OF_LOP_RES_DIG_J_V10,false);
    }
    if(ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
        LowestLevelMenuEnDis(INDEX_OF_LOP_RES_DIG_J_DIG_SOURCE,INDEX_OF_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY,false);

    //Sensor K
    if(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_FUEL_LVL_DIG_K_DIG_SOURCE,INDEX_OF_FUEL_LVL_DIG_K_L20,false);
    if(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_SOURCE].value.u8Val == CFGZ::CFGZ_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_FUEL_LVL_DIG_K_DIG_POLARITY,INDEX_OF_FUEL_LVL_DIG_K_L20,false);
    if(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_DIG_SOURCE].value.u8Val != CFGZ::CFGZ_SENSOR_NOT_USED)
    {
        DigitalInputMenuVisiblity(INDEX_OF_FUEL_LVL_DIG_K_DIG_SOURCE);
        LowestLevelMenuEnDis(INDEX_OF_FUEL_LVL_DIG_K_SHUTDOWN,INDEX_OF_FUEL_LVL_DIG_K_L20,false);
    }
    if(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
        LowestLevelMenuEnDis(INDEX_OF_FUEL_LVL_DIG_K_DIG_SOURCE,INDEX_OF_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY,false);
    if(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1 && ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SHUTDOWN].value.u8Val == CFGZ::CFGZ_DISABLE)
        menuItemsLowestLevel[INDEX_OF_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD].isEnabled = false;
    if(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1 && ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_NOTIFICATION].value.u8Val == CFGZ::CFGZ_DISABLE)
        menuItemsLowestLevel[INDEX_OF_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD].isEnabled = false;
    if(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1 && ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING].value.u8Val == CFGZ::CFGZ_DISABLE)
        menuItemsLowestLevel[INDEX_OF_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD].isEnabled = false;

    //Sensor L
    if(ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED || ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_L_DIG_SOURCE,INDEX_OF_ENG_TEMP_DIG_L_T10,false);
    if(ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_SOURCE].value.u8Val == CFGZ::CFGZ_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_L_DIG_POLARITY,INDEX_OF_ENG_TEMP_DIG_L_T10,false);
    if(ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_DIG_SOURCE].value.u8Val != CFGZ::CFGZ_SENSOR_NOT_USED)
    {
        DigitalInputMenuVisiblity(INDEX_OF_ENG_TEMP_DIG_L_DIG_SOURCE);
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_L_OPEN_CKT_WARNING,INDEX_OF_ENG_TEMP_DIG_L_T10,false);
    }
    if(ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_L_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
        LowestLevelMenuEnDis(INDEX_OF_ENG_TEMP_DIG_L_DIG_SOURCE,INDEX_OF_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY,false);

    //Sensor M
    if(ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_SHEL_TEMP_DIG_M_DIG_SOURCE,INDEX_OF_SHEL_TEMP_DIG_M_T10,false);
    if(ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_SOURCE].value.u8Val == CFGZ::CFGZ_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_SHEL_TEMP_DIG_M_DIG_POLARITY,INDEX_OF_SHEL_TEMP_DIG_M_T10,false);
    if(ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_SOURCE].value.u8Val != CFGZ::CFGZ_SENSOR_NOT_USED)
    {
        DigitalInputMenuVisiblity(INDEX_OF_SHEL_TEMP_DIG_M_DIG_SOURCE);
        LowestLevelMenuEnDis(INDEX_OF_SHEL_TEMP_DIG_M_HIGH_TEMP_THRESHOLD,INDEX_OF_SHEL_TEMP_DIG_M_T10,false);
    }
    if(ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
        LowestLevelMenuEnDis(INDEX_OF_SHEL_TEMP_DIG_M_DIG_SOURCE,INDEX_OF_SHEL_TEMP_DIG_M_DIG_ACTIVATION_DELAY,false);
    if(ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        LowestLevelMenuEnDis(INDEX_OF_SHEL_TEMP_DIG_M_DIG_SOURCE,INDEX_OF_SHEL_TEMP_DIG_M_DG_RUN_DURATION,false);
    }

    //Sensor N
    if(ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_AUX_S2_RES_DIG_N_DIG_SOURCE,INDEX_OF_AUX_S2_RES_DIG_N_V10,false);
    if(ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_SOURCE].value.u8Val == CFGZ::CFGZ_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_AUX_S2_RES_DIG_N_DIG_POLARITY,INDEX_OF_AUX_S2_RES_DIG_N_V10,false);
    if(ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_SOURCE].value.u8Val != CFGZ::CFGZ_SENSOR_NOT_USED)
    {
        DigitalInputMenuVisiblity(INDEX_OF_AUX_S2_RES_DIG_N_DIG_SOURCE);
        LowestLevelMenuEnDis(INDEX_OF_AUX_S2_RES_DIG_N_ACTION,INDEX_OF_AUX_S2_RES_DIG_N_V10,false);
    }
    if(ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
        LowestLevelMenuEnDis(INDEX_OF_AUX_S2_RES_DIG_N_DIG_SOURCE,INDEX_OF_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY,false);
    if(ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1 && ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_ACTION].value.u8Val == CFGZ::CFGZ_ACTION_NONE)
        menuItemsLowestLevel[INDEX_OF_AUX_S2_RES_DIG_N_THRESHOLD].isEnabled = false;
    //Sensor O
    if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_AUX_S3_DIG_O_DIG_SOURCE,INDEX_OF_AUX_S3_DIG_O_P10,false);
    if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_SOURCE].value.u8Val == CFGZ::CFGZ_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_AUX_S3_DIG_O_DIG_POLARITY,INDEX_OF_AUX_S3_DIG_O_P10,false);
    if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_SOURCE].value.u8Val != CFGZ::CFGZ_SENSOR_NOT_USED)
    {
        DigitalInputMenuVisiblity(INDEX_OF_AUX_S3_DIG_O_DIG_SOURCE);
        LowestLevelMenuEnDis(INDEX_OF_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION,INDEX_OF_AUX_S3_DIG_O_P10,false);
    }
    if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1 || ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
        LowestLevelMenuEnDis(INDEX_OF_AUX_S3_DIG_O_DIG_SOURCE,INDEX_OF_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY,false);

    //Sensor P
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_AUX_S4_DIG_P_DIG_SOURCE,INDEX_OF_AUX_S4_DIG_P_TANK_HEIGHT_2,false);
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_SOURCE].value.u8Val == CFGZ::CFGZ_SENSOR_NOT_USED)
        LowestLevelMenuEnDis(INDEX_OF_AUX_S4_DIG_P_DIG_POLARITY,INDEX_OF_AUX_S4_DIG_P_TANK_HEIGHT_2,false);
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN && ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_SOURCE].value.u8Val != CFGZ::CFGZ_SENSOR_NOT_USED)
    {
        DigitalInputMenuVisiblity(INDEX_OF_AUX_S4_DIG_P_DIG_SOURCE);
        LowestLevelMenuEnDis(INDEX_OF_AUX_S4_DIG_P_SHUTDOWN,INDEX_OF_AUX_S4_DIG_P_TANK_HEIGHT_2,false);
    }
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
        LowestLevelMenuEnDis(INDEX_OF_AUX_S4_DIG_P_DIG_SOURCE,INDEX_OF_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY,false);
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SHUTDOWN].value.u8Val == CFGZ::CFGZ_DISABLE)
        menuItemsLowestLevel[INDEX_OF_AUX_S4_DIG_P_SHUTDOWN_THRESHOLD].isEnabled = false;
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_NOTIFICATION].value.u8Val == CFGZ::CFGZ_DISABLE)
        menuItemsLowestLevel[INDEX_OF_AUX_S4_DIG_P_NOTIFICATION_THRESHOLD].isEnabled = false;
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_FUEL_THEFT_WARNING].value.u8Val == CFGZ::CFGZ_DISABLE)
        menuItemsLowestLevel[INDEX_OF_AUX_S4_DIG_P_FUEL_THEFT_THRESHOLD].isEnabled = false;
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_TANK_WITH_STEP].value.u8Val == CFGZ::CFGZ_DISABLE)
        LowestLevelMenuEnDis(INDEX_OF_AUX_S4_DIG_P_TANK_LENGTH_2,INDEX_OF_AUX_S4_DIG_P_TANK_HEIGHT_2,false);
    //OUTPUTS MENU
    menuItemsLowestLevel[INDEX_OF_OUT_A_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_A_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_B_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_B_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_C_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_C_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_D_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_D_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_E_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_E_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_F_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_F_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_G_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_G_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_H_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_H_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_I_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_I_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);

    menuItemsLowestLevel[INDEX_OF_OUT_J_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_J_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_K_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_K_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_L_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_L_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_M_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_M_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_N_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_N_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_O_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_O_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);
    menuItemsLowestLevel[INDEX_OF_OUT_P_ON_ACTIVATION].isEnabled = (bool)(ArrEditableItem[INDEX_OF_OUT_P_SOURCE].value.u8Val!=CFGZ::CFGZ_NOT_CONFIGURED);

    //TIMERS MENU
    //GENERATOR MENU
    //Alt Config
    if(ArrEditableItem[INDEX_OF_ALT_CONFIG_ALT_PRESENT].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        LowestLevelMenuEnDis(INDEX_OF_ALT_CONFIG_NUMBER_OF_POLES,INDEX_OF_ALT_CONFIG_ALT_WAVE_DETECTION,false);
    }
    if(ArrEditableItem[INDEX_OF_ALT_CONFIG_ALT_AC_SYSTEM].value.u8Val == CFGZ::CFGZ_1_PHASE_SYSTEM)
    {
        LowestLevelMenuEnDis(INDEX_OF_ALT_CONFIG_PHASE_REVERSAL_DETECT,INDEX_OF_ALT_CONFIG_PHASE_REVERSAL_ACTION,false);
    }
    if(ArrEditableItem[INDEX_OF_ALT_CONFIG_ALT_AC_SYSTEM].value.u8Val == CFGZ::CFGZ_3_PHASE_SYSTEM && ArrEditableItem[INDEX_OF_ALT_CONFIG_PHASE_REVERSAL_DETECT].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_ALT_CONFIG_PHASE_REVERSAL_ACTION].isEnabled = false;
    }

    //Voltage Mon.

    if(ArrEditableItem[INDEX_OF_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD].isEnabled = false;
    }
    if(ArrEditableItem[INDEX_OF_VOLT_MONITOR_UNDER_VOLT_WARNING].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_VOLT_MONITOR_UV_WARNING_THRESHOLD].isEnabled = false;
    }
    if(ArrEditableItem[INDEX_OF_VOLT_MONITOR_OVER_VOLT_SHUTDOWN].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD].isEnabled = false;
    }
    if(ArrEditableItem[INDEX_OF_VOLT_MONITOR_OVER_VOLT_WARNING].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_VOLT_MONITOR_OV_WARNING_THRESHOLD].isEnabled = false;
    }
    //Freq Mon
    if(ArrEditableItem[INDEX_OF_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_FREQ_MONITOR_UF_SHUTDOWN_THRESHOLD].isEnabled = false;
    }
    if(ArrEditableItem[INDEX_OF_FREQ_MONITOR_UNDER_FREQ_WARNING].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_FREQ_MONITOR_UF_WARNING_THRESHOLD].isEnabled = false;
    }
    if(ArrEditableItem[INDEX_OF_FREQ_MONITOR_OVER_FREQ_SHUTDOWN].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_FREQ_MONITOR_OF_SHUTDOWN_THRESHOLD].isEnabled = false;
    }
    if(ArrEditableItem[INDEX_OF_FREQ_MONITOR_OVER_FREQ_WARNING].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_FREQ_MONITOR_OF_WARNING_THRESHOLD].isEnabled = false;
    }
    //Current Mon.
    if(ArrEditableItem[INDEX_OF_CURRENT_MONITOR_OVER_CURR_ACTION].value.u8Val == CFGZ::CFGZ_ACTION_NONE)
    {
        LowestLevelMenuEnDis(INDEX_OF_CURRENT_MONITOR_OVER_CURR_THRESHOLD,INDEX_OF_CURRENT_MONITOR_OVER_CURR_DELAY,false);
    }
    //Fan Current Mon
    if(ArrEditableItem[INDEX_OF_FAN_CURR_MONITOR_FAN_CURRENT_MON].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        LowestLevelMenuEnDis(INDEX_OF_FAN_CURR_MONITOR_FAN_MON_CT_RATIO,INDEX_OF_FAN_CURR_MONITOR_CURR_MON_DELAY,false);
    }
    //Load monitoring
    if(ArrEditableItem[INDEX_OF_LOAD_MONITOR_OVERLOAD_ACTION].value.u8Val == CFGZ::CFGZ_ACTION_NONE)
    {
        LowestLevelMenuEnDis(INDEX_OF_LOAD_MONITOR_OVERLOAD_THRESHOLD,INDEX_OF_LOAD_MONITOR_OVERLOAD_MON_DELAY,false);
    }
    if(ArrEditableItem[INDEX_OF_LOAD_MONITOR_UNBAL_LOAD_ACTION].value.u8Val == CFGZ::CFGZ_ACTION_NONE)
    {
        LowestLevelMenuEnDis(INDEX_OF_LOAD_MONITOR_UNBAL_LOAD_THRESHOLD,INDEX_OF_LOAD_MONITOR_UNBAL_LOAD_ACT_THRESH,false);
    }
    //MAINS MENU
    //Configuration
    if(ArrEditableItem[INDEX_OF_MAINS_CONFIG_MAINS_MONITORING].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        LowestLevelMenuEnDis(INDEX_OF_MAINS_CONFIG_MAINS_AC_SYSTEM,INDEX_OF_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH,false);
    }
    if(ArrEditableItem[INDEX_OF_MAINS_CONFIG_MAINS_AC_SYSTEM].value.u8Val == CFGZ::CFGZ_1_PHASE_SYSTEM)
    {
        LowestLevelMenuEnDis(INDEX_OF_MAINS_CONFIG_PHASE_REVERSAL_DETECT,INDEX_OF_MAINS_CONFIG_PHASE_REVERSAL_ACTION,false);
    }
    if(ArrEditableItem[INDEX_OF_MAINS_CONFIG_MAINS_AC_SYSTEM].value.u8Val == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        menuItemsLowestLevel[INDEX_OF_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH].isEnabled = false;
        if(ArrEditableItem[INDEX_OF_MAINS_CONFIG_PHASE_REVERSAL_DETECT].value.u8Val == CFGZ::CFGZ_DISABLE)
        {
            menuItemsLowestLevel[INDEX_OF_MAINS_CONFIG_PHASE_REVERSAL_ACTION].isEnabled = false;
        }
    }
    //Voltage Monitoring
    if(ArrEditableItem[INDEX_OF_UNDER_VOLT_MON_ENABLE].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        LowestLevelMenuEnDis(INDEX_OF_UNDER_VOLT_MON_TRIP,INDEX_OF_UNDER_VOLT_MON_RETURN,false);
    }
    if(ArrEditableItem[INDEX_OF_OVER_VOLT_MON_ENABLE].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        LowestLevelMenuEnDis(INDEX_OF_OVER_VOLT_MON_TRIP,INDEX_OF_OVER_VOLT_MON_RETURN,false);
    }
    //Freq Monitoring
    if(ArrEditableItem[INDEX_OF_UNDER_FREQ_MON_ENABLE].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        LowestLevelMenuEnDis(INDEX_OF_UNDER_FREQ_MON_TRIP,INDEX_OF_UNDER_FREQ_MON_RETURN,false);
    }
    if(ArrEditableItem[INDEX_OF_OVER_FREQ_MON_ENABLE].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        LowestLevelMenuEnDis(INDEX_OF_OVER_FREQ_MON_TRIP,INDEX_OF_OVER_FREQ_MON_RETURN,false);
    }

    //Crank Disconnect
    if(ArrEditableItem[INDEX_OF_CRANK_DISCONN_DISCONN_ON_LOP_SENS].value.u8Val == CFGZ::CFGZ_DISABLE && ArrEditableItem[INDEX_OF_CRANK_DISCONN_MON_LOP_BEFORE_CRANK].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_CRANK_DISCONN_DISCONN_LOP_SENS].isEnabled = false;
    }
    if(ArrEditableItem[INDEX_OF_CRANK_DISCONN_DISCONN_ON_LLOP_SW].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_CRANK_DISCONN_LLOP_SW_TRANS_TIME].isEnabled = false;
    }
    if(ArrEditableItem[INDEX_OF_CRANK_DISCONN_DISCONN_ON_CHG_ALT_VOLT].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        menuItemsLowestLevel[INDEX_OF_CRANK_DISCONN_CHG_ALT_THRESHOLD].isEnabled = false;
    }
    //Speed Monitoring
    if(ArrEditableItem[INDEX_OF_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        LowestLevelMenuEnDis(INDEX_OF_SPEED_MONITOR_UNDER_SPEED_THRESHOLD,INDEX_OF_SPEED_MONITOR_UNDER_SPEED_DELAY,false);
    }
    //Battery Monitoring
    if(ArrEditableItem[INDEX_OF_BATTERY_MONITOR_LOW_VOLT_ACTION].value.u8Val == CFGZ::CFGZ_ACTION_NONE)
    {
        LowestLevelMenuEnDis(INDEX_OF_BATTERY_MONITOR_LOW_VOLT_THRESHOLD,INDEX_OF_BATTERY_MONITOR_LOW_VOLT_DELAY,false);
    }
    if(ArrEditableItem[INDEX_OF_BATTERY_MONITOR_HIGH_VOLT_ACTION].value.u8Val == CFGZ::CFGZ_ACTION_NONE)
    {
        LowestLevelMenuEnDis(INDEX_OF_BATTERY_MONITOR_HIGH_VOLT_THRESHOLD,INDEX_OF_BATTERY_MONITOR_HIGH_VOLT_DELAY,false);
    }


    //Charging Alternator
    if(ArrEditableItem[INDEX_OF_CHARGE_ALT_MON_FAIL_ACTION].value.u8Val == CFGZ::CFGZ_ACTION_NONE)
    {
        LowestLevelMenuEnDis(INDEX_OF_CHARGE_ALT_MON_FAIL_THRESHOLD,INDEX_OF_CHARGE_ALT_MON_FAIL_DELAY,false);
    }

    //Preheat

    //Maintenance

    /*
     * SuryaPranayTeja.BVV 17-11-2022
     * The below statements are necessary so that while the sensor
     * is not configured as the Digital input, then we change the DIG Source to Not Configured.
     * Otherwise it takes the values which are present inside(as they get hide we cannot see)
     * and continue to take actions as if they are configured.
     */
    if(ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_SENSOR_SELECTION].value.u8Val != CFGZ::CFGZ_ANLG_DIG_IN)
    {
        ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_SOURCE].value.u8Val = CFGZ::CFGZ_NOT_CONFIGURED;
        ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_DIG_SOURCE].tempValue.u8Val = CFGZ::CFGZ_NOT_CONFIGURED;
    }
    if(ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_SENSOR_SELECTION].value.u8Val != CFGZ::CFGZ_ANLG_DIG_IN)
    {
        ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_SOURCE].value.u8Val = CFGZ::CFGZ_NOT_CONFIGURED;
        ArrEditableItem[INDEX_OF_AUX_S2_RES_DIG_N_DIG_SOURCE].tempValue.u8Val = CFGZ::CFGZ_NOT_CONFIGURED;
    }
    if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val != CFGZ::CFGZ_ANLG_DIG_IN)
    {
        ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_SOURCE].value.u8Val = CFGZ::CFGZ_NOT_CONFIGURED;
        ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_DIG_SOURCE].tempValue.u8Val = CFGZ::CFGZ_NOT_CONFIGURED;
    }
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION].value.u8Val != CFGZ::CFGZ_ANLG_DIG_IN)
    {
        ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_SOURCE].value.u8Val = CFGZ::CFGZ_NOT_CONFIGURED;
        ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_DIG_SOURCE].tempValue.u8Val = CFGZ::CFGZ_NOT_CONFIGURED;
    }

    if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION].numOfStringFixedOptions = 2;
        ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION].stringFixedOptions = strOptions[_u8LanguageArrayIndex][ID_ACTION_WS];
        for(uint8_t i = 0; i<10; i++)
        {
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].formatString = "%.0f";
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].unitOfMeasurement = arrUnit[ID_MILLI_AMPERE];
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].minVal.fVal = 4.0f;
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].maxVal.fVal = 20.0f;
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].fValLC = 1.0f;
        }
    }
    else if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION].numOfStringFixedOptions = 3;
        ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION].stringFixedOptions = strOptions[_u8LanguageArrayIndex][ID_ACTION_NoWS];
        for(uint8_t i = 0; i<10; i++)
        {
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].formatString = "%f";
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].unitOfMeasurement = arrUnit[ID_V];
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].minVal.fVal = 0.0f;
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].maxVal.fVal = 5.0f;
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].fValLC = 0.1f;
        }
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
    _objDisplay.drawHorizontalLine(GLCD_X(0), GLCD_Y(19), GLCD_X(128));

    switch (uiState)
    {
    case    UI_STATE_INITIALIZING:
        Initialize();
        pCurMenu = &mainMenu;
        u16IndexOfEditableItems =0;
        UpdateMaxNumberOfItem();
        uiState = UI_STATE_SHOWING_MENU;
        pCurMenu->show();
        UTILS_ResetTimer(&_ValIncDecTimer);
        _u16MenuSp =0;
        break;
    case    UI_STATE_SHOWING_MENU:
        switch (keyCode)
        {
        case    CKeyCodes::DOWN:
            if(_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].dataType == CEditableItem::DT_PASSWORD)
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
            _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex = 0;
            pCurMenu->show();
            break;
        case    CKeyCodes::UP:
            if(_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].dataType == CEditableItem::DT_PASSWORD)
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
            _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex = 0;
            pCurMenu->show();
            break;
        case    CKeyCodes::ENTER:
            if (pCurMenu->pMenuItems[pCurMenu->indexOfSelectedMenuItem].isLeafNode)
            {
                // we ought to show the editable items screen
                _pCurEditableItemsScreen = pCurMenu->pMenuItems[pCurMenu->indexOfSelectedMenuItem].pEditableItemsScreen;
                _pCurEditableItemsScreen->initTempValues();
                _pCurEditableItemsScreen->show(true);    // i.e. show tempValue(s) rather than Value(s)


#if(CONFIG_EDIT == YES)
                if(_objPassword.GetEnteredPassword() != 0U) //Check if config is in write mode
#else
                if(((_pCurEditableItemsScreen->pEditableItems->u8PasswordLevel & _objPassword.GetEnteredPassword()) //If accessible through entered password
                        || (_objPassword.GetEnteredPassword()== PASSWORD_ENTRY_UI::MASTER_PIN))
                      )
#endif
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
            prvUpdateEngineTypeDependency();
            InitialiseCustomSensor();
            pCurMenu->show();
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
                /* Following logic is implemented to update the indexofEditableItems on Escape key press
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
            pCurMenu->show();
            break;
        default:
            pCurMenu->show();
            break;
        }
        break;
    case   UI_STATE_SHOWING_SCREEN_ITEM_EDIT:
        switch (keyCode)
        {
        case    CKeyCodes::ENTER:
                prvUpdateEngineTypeDependency();
                if(_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].dataType > CEditableItem::DT_TIME_HRS_MINS)
                {
                    if((_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].dataType != CEditableItem::DT_SRNO)
                      || 
                      (CEditableItem::u8TempSrNoArr[(_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem])._eSrNoType]
                      .u8Arr[_pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex])
                      != 0) /*Check that the temp value doesnt contain ascii value of 0*/
                    {
                        _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex++;  
                    }
                    else
                    {
                        _pCurEditableItemsScreen->pEditableItems[_pCurEditableItemsScreen->indexOfSelectedEditableItem].u8MultiItemEditIndex = 0;
                        uiState = UI_STATE_SHOWING_MENU;  
                    }
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
                prvUpdateConfigStrings();
                HandleMenuVisibility();
                UpdateMaxNumberOfItem();
                pCurMenu->show();

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
                pCurMenu->show();
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
        pCurMenu->show();
        break;
    }
}

volatile float table_x_4_20mA_Sens[10] = {4,  6,  7,  9,  10, 12, 14, 15, 17, 20};
volatile float table_x_0to5V_Sens[10] = {0.5f ,0.9f ,1.3f ,1.7f ,2.1f ,2.5f ,2.9f ,3.3f ,3.7f ,4.5f};

volatile float table_LOP_Bar[10] = {0.0f, 1.0f,2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 10.0f};


void UI::prvUpdateAutomationModbusMap()
{
#if (TEST_AUTOMATION == YES)
    if(_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_MAP) == CFGZ::CFGZ_MODBUS_MAP_A_RJIO)
    {
        /*Changing to rjio map. Change rgister type of automation register groups to 'MODBUS_REG_ANY'*/
        _mbApp.SetAutomationRegTypeToAny();
    }
    else
    {
        /*Changing to indus map. Change rgister type of automation register groups to 'MODBUS_REG_INPUT'*/
        _mbApp.SetAutomationRegTypeToInput();
    }  
#endif
}

void UI::prvUpdateConfigStrings()
{
    if(ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU].value.u8Val == CFGZ::CFGZ_ENABLE)
    {
        ArrEditableItem[INDEX_OF_HWT_SHUTDOWN_EN].promptMessage = strLeafNode[_u8LanguageArrayIndex][SID_CLNT_TEMP_SHUTDOWN_EN]; 
        ArrEditableItem[INDEX_OF_HWT_SHUTDOWN_THRESH].promptMessage = strLeafNode[_u8LanguageArrayIndex][SID_CLNT_TEMP_SHUTDOWN_THRESH]; 
        ArrEditableItem[INDEX_OF_HWT_WARNING_EN].promptMessage = strLeafNode[_u8LanguageArrayIndex][SID_CLNT_TEMP_WARNING_EN];
        ArrEditableItem[INDEX_OF_HWT_WARNING_THRESH].promptMessage = strLeafNode[_u8LanguageArrayIndex][SID_CLNT_TEMP_WARNING_THRESH];
        menuItemsMidLevel[ID_ENG_TEMPERATURE].pItemName = strSubMenu[_u8LanguageArrayIndex][ID_ENG_TEMPERATURE];
        ArrSubMenu[ID_ENG_TEMPERATURE].pMenuTitle = strSubMenu[_u8LanguageArrayIndex][ID_ENG_TEMPERATURE];
    }
    else
    {
        ArrEditableItem[INDEX_OF_HWT_SHUTDOWN_EN].promptMessage = strAlternate[_u8LanguageArrayIndex][ID_STR_ALT_EDITITEM_ENG_TEMP_SHDN_EN];
        ArrEditableItem[INDEX_OF_HWT_SHUTDOWN_THRESH].promptMessage = strAlternate[_u8LanguageArrayIndex][ID_STR_ALT_EDITITEM_ENG_TEMP_SHDN_TH];
        ArrEditableItem[INDEX_OF_HWT_WARNING_EN].promptMessage = strAlternate[_u8LanguageArrayIndex][ID_STR_ALT_EDITITEM_ENG_TEMP_WARN_EN];
        ArrEditableItem[INDEX_OF_HWT_WARNING_THRESH].promptMessage = strAlternate[_u8LanguageArrayIndex][ID_STR_ALT_EDITITEM_ENG_TEMP_WARN_TH];
        menuItemsMidLevel[ID_ENG_TEMPERATURE].pItemName = strAlternate[_u8LanguageArrayIndex][ID_STR_ALT_SUBMENU_ENG_TEMPERATURE];
        ArrSubMenu[ID_ENG_TEMPERATURE].pMenuTitle = strAlternate[_u8LanguageArrayIndex][ID_STR_ALT_SUBMENU_ENG_TEMPERATURE];  
    }  

    if(ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_SENSOR_SELECTION].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_OPEN_CKT_NOTIFICATION].promptMessage = strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_OPEN_CKT_WARNING];
    }
    else
    {
        ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_M_OPEN_CKT_NOTIFICATION].promptMessage = strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_M_OPEN_CKT_NOTIFICATION];  
    }  
}

void UI::InitialiseCustomSensor()
{
    if((ArrEditableItem[INDEX_OF_PREHEAT_AMB_TEMPERATURE].value.u8Val != ArrEditableItem[INDEX_OF_PREHEAT_AMB_TEMPERATURE].tempValue.u8Val))
    {
        if(ArrEditableItem[INDEX_OF_PREHEAT_AMB_TEMPERATURE].tempValue.u8Val == CFGZ::CFGZ_ENABLE)
        {
            ArrEditableItem[INDEX_OF_PREHEAT_ENG_TEMPERATURE].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_PREHEAT_ENG_TEMPERATURE].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        }
    }

    if((ArrEditableItem[INDEX_OF_PREHEAT_ENG_TEMPERATURE].value.u8Val != ArrEditableItem[INDEX_OF_PREHEAT_ENG_TEMPERATURE].tempValue.u8Val))
    {
        if(ArrEditableItem[INDEX_OF_PREHEAT_ENG_TEMPERATURE].tempValue.u8Val == CFGZ::CFGZ_ENABLE)
        {
            ArrEditableItem[INDEX_OF_PREHEAT_AMB_TEMPERATURE].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_PREHEAT_AMB_TEMPERATURE].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        }
    }
    
    if((ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val != ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].tempValue.u8Val))
    {
        if((ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].tempValue.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
        {
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION].value.u8Val = CFGZ::CFGZ_ACTION_NONE;
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION].tempValue.u8Val = CFGZ::CFGZ_ACTION_NONE;

            for(uint8_t i = 0; i<10; i++)
            {
                 ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P1 + (i*2)].value.fVal = table_LOP_Bar[i];
                 ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P1 + (i*2)].tempValue.fVal = table_LOP_Bar[i];
                 ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].value.fVal = table_x_4_20mA_Sens[i];
                 ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].tempValue.fVal = table_x_4_20mA_Sens[i];
            }
        }
        else if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].tempValue.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
        {
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION].value.u8Val = CFGZ::CFGZ_ACTION_NONE;
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION].tempValue.u8Val = CFGZ::CFGZ_ACTION_NONE;

          for(uint8_t i = 0; i<10; i++)
          {
               ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P1 + (i*2)].value.fVal = table_LOP_Bar[i];
               ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_P1 + (i*2)].tempValue.fVal = table_LOP_Bar[i];
               ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].value.fVal = table_x_0to5V_Sens[i];
               ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_I1_V1 + (i*2)].tempValue.fVal = table_x_0to5V_Sens[i];
          }
        }
    }

    //If LOP resistive is selected then disable the 4-20mA sensor or 0-5V sensor if it is selected as LOP.
    if(ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].value.u8Val != ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].tempValue.u8Val)
    {
        if((ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val>=CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                &&(ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].tempValue.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
        {
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].tempValue.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
        }
    }
    //If LOP 4-20mA or 0-5V sensor is selected then disable the resistive sensor if it is selected as LOP.
    if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].value.u8Val != ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].tempValue.u8Val)
    {
        if((ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].value.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                &&(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR_SELECTION].tempValue.u8Val>=CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
        {
            ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].value.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
            ArrEditableItem[INDEX_OF_LOP_RES_DIG_J_SENSOR_SELECTION].tempValue.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
        }
    }
    //If Fuel resistive sensor is selected then disable the 0-5V sensor if it is selected as Fuel.
    if(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val != ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].tempValue.u8Val)
    {
        if((ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION].value.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                &&(ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].tempValue.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
        {
            ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION].value.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
            ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION].tempValue.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
        }
    }
    //If Fuel 0-5V sensor is selected then disable the resistive sensor if it is selected as Fuel.
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION].value.u8Val != ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION].tempValue.u8Val)
    {
        if((ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                &&(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR_SELECTION].tempValue.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
        {
            ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].value.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
            ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_SENSOR_SELECTION].tempValue.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
        }
    }


    /*
     * SuryaPranayTeja.BVV 11-11-2022
     * When CT factor is changed then the Active energy calculations made from power on reset will get changed.
     * To avoid such scenario, when it is being edited then we are saving the calculations.
     *
     * For the CT correction factor(float parameter). So to avoid the warning of float comparison with "==" or "!="
     * In hand with its resolution of 3 digits, the values are multiplied by 1000.
     */
       if((ArrEditableItem[INDEX_OF_CURRENT_MONITOR_LOAD_CT_RATIO].tempValue.u16Val !=ArrEditableItem[INDEX_OF_CURRENT_MONITOR_LOAD_CT_RATIO].value.u16Val)
               ||((uint16_t)(1000*ArrEditableItem[INDEX_OF_CURRENT_MONITOR_CT_CORRECTION_FACTOR].tempValue.fVal)!= (uint16_t)(1000* ArrEditableItem[INDEX_OF_CURRENT_MONITOR_CT_CORRECTION_FACTOR].value.fVal)))
    {
        _engMon.ReadEnergySetEnergyOffset(false);
        _engMon.StoreCummulativeCnt(ENGINE_MONITORING::CUM_STORE_GENERAL);
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
uint16_t UI::GetEditableItemIndex()
{
    return u16IndexOfEditableItems;
}

void UI::prvInitialiseECUParam()
{
    static uint8_t u8EngineType;

     if(u8EngineType!=ArrEditableItem[INDEX_OF_ENGINE_TYPE].value.u8Val)
    {
        if(ArrEditableItem[INDEX_OF_ENGINE_TYPE].value.u8Val == CFGZ::CFGZ_CONVENTIONAL)
        {

            for(uint16_t u16Index = INDEX_OF_ENG_SPEED_FROM_ECU; u16Index <= INDEX_OF_ECU_SOURCE_ADDRESS; u16Index++)
            {
                prvSetPasswordAccessLevel(u16Index ,  (uint8_t)CEditableItem::NOT_ALLOWED);
            }
            
            for(uint16_t u16LocalIndex = INDEX_OF_HIGH_OIL_TEMP_SHUTDOWN_EN_FROM_ECU ; u16LocalIndex <= INDEX_OF_HIGH_OIL_TEMP_WARNING_THRESH_FROM_ECU; u16LocalIndex++)
            {
                prvSetPasswordAccessLevel(u16LocalIndex , (uint8_t)CEditableItem::NOT_ALLOWED);
            }
            
            ArrEditableItem[INDEX_OF_ENG_SPEED_FROM_ECU].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_LOP_FROM_ECU             ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU    ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_ENG_RUN_HOURS_FROM_ECU   ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_BAT_VTG_FROM_ECU        ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_OIL_TEMP_FROM_ECU        ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACTION   ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACT      ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACT_DELAY].value.u8Val = 1U;
            ArrEditableItem[INDEX_OF_AMBER_LAMP_ACTION        ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT           ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT_DELAY     ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_RED_LAMP_ACTION          ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_RED_LAMP_ACT             ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_RED_LAMP_ACT_DELAY       ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACTION  ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT     ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT_DELAY].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACTION      ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT         ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT_DELAY   ].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_HIGH_OIL_TEMP_SHUTDOWN_EN_FROM_ECU].value.u8Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_HIGH_OIL_TEMP_WARNING_EN_FROM_ECU].value.u8Val = CFGZ::CFGZ_DISABLE;
        }
        else
        {
            prvUpdateEngineParam();
        }
    }
    else
    {
         if(ArrEditableItem[INDEX_OF_ENGINE_TYPE].value.u8Val != CFGZ:: CFGZ_CONVENTIONAL)
         {
             prvUpdateEngineParam();
         }
    }
    u8EngineType=ArrEditableItem[INDEX_OF_ENGINE_TYPE].value.u8Val;
}


void UI::prvUpdateEngineParam(void)
{
    for(uint16_t u16Index = INDEX_OF_ENG_SPEED_FROM_ECU; u16Index <= INDEX_OF_ECU_SOURCE_ADDRESS; u16Index++)
    {
        prvSetPasswordAccessLevel(u16Index ,  (uint8_t)CEditableItem::PIN1_ALLOWED);
    }

    for(uint16_t u16LocalIndex = INDEX_OF_HIGH_OIL_TEMP_SHUTDOWN_EN_FROM_ECU ; u16LocalIndex <= INDEX_OF_HIGH_OIL_TEMP_WARNING_THRESH_FROM_ECU; u16LocalIndex++)
    {
        prvSetPasswordAccessLevel(u16LocalIndex , (uint8_t)CEditableItem::PIN1_ALLOWED);
    }

//    ArrEditableItem[INDEX_OF_ECU_SOURCE_ADDRESS].value.u16Val = 0;
//    ArrEditableItem[INDEX_OF_ECU_SOURCE_ADDRESS].tempValue.u16Val = 0;


        if(ArrEditableItem[INDEX_OF_AMBER_LAMP_ACTION].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        for(uint16_t u16Index = INDEX_OF_AMBER_LAMP_ACT; u16Index <= INDEX_OF_AMBER_LAMP_ACT_DELAY; u16Index++)
        {
            prvSetPasswordAccessLevel(u16Index ,  (uint8_t)CEditableItem::NOT_ALLOWED);
        }
        ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT     ].value.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT     ].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT_DELAY ].value.u8Val = 0;
        ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT_DELAY ].tempValue.u8Val = 0;
    }

    if(ArrEditableItem[INDEX_OF_RED_LAMP_ACTION].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        for(uint16_t u16Index = INDEX_OF_RED_LAMP_ACT; u16Index <= INDEX_OF_RED_LAMP_ACT_DELAY; u16Index++)
        {
            prvSetPasswordAccessLevel(u16Index ,  (uint8_t)CEditableItem::NOT_ALLOWED);
        }
        ArrEditableItem[INDEX_OF_RED_LAMP_ACT].value.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_RED_LAMP_ACT].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_RED_LAMP_ACT_DELAY ].value.u8Val = 0;
        ArrEditableItem[INDEX_OF_RED_LAMP_ACT_DELAY ].tempValue.u8Val = 0;
    }

    if(ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACTION].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        for(uint16_t u16Index = INDEX_OF_MALFUNCTION_LAMP_ACT; u16Index <= INDEX_OF_MALFUNCTION_LAMP_ACT_DELAY; u16Index++)
        {
            prvSetPasswordAccessLevel(u16Index ,  (uint8_t)CEditableItem::NOT_ALLOWED);
        }
        ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT].value.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT_DELAY ].value.u8Val = 0;
        ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT_DELAY ].tempValue.u8Val = 0;
    }


    if(ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACTION].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        for(uint16_t u16Index = INDEX_OF_PROTECT_LAMP_ACT; u16Index <= INDEX_OF_PROTECT_LAMP_ACT_DELAY; u16Index++)
        {
            prvSetPasswordAccessLevel(u16Index ,  (uint8_t)CEditableItem::NOT_ALLOWED);
        }
        ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT].value.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT_DELAY ].value.u8Val = 0;
        ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT_DELAY ].tempValue.u8Val = 0;
    }


}


void UI::prvUpdateEngineTypeDependency(void)
{
    if(ArrEditableItem[INDEX_OF_ENGINE_TYPE].tempValue.u8Val == CFGZ::CFGZ_CONVENTIONAL)
    {

        for(uint16_t u16LocalIndex = INDEX_OF_ENG_SPEED_FROM_ECU ; u16LocalIndex <= INDEX_OF_ECU_SOURCE_ADDRESS; u16LocalIndex++)
        {
            prvSetPasswordAccessLevel(u16LocalIndex , (uint8_t)CEditableItem::NOT_ALLOWED);
        }

        for(uint16_t u16LocalIndex = INDEX_OF_HIGH_OIL_TEMP_SHUTDOWN_EN_FROM_ECU ; u16LocalIndex <= INDEX_OF_HIGH_OIL_TEMP_WARNING_THRESH_FROM_ECU; u16LocalIndex++)
        {
            prvSetPasswordAccessLevel(u16LocalIndex , (uint8_t)CEditableItem::NOT_ALLOWED);
        }

        ArrEditableItem[INDEX_OF_ENG_SPEED_FROM_ECU].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_LOP_FROM_ECU].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_ENG_RUN_HOURS_FROM_ECU].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_BAT_VTG_FROM_ECU].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_OIL_TEMP_FROM_ECU].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_HIGH_OIL_TEMP_SHUTDOWN_EN_FROM_ECU].tempValue.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_HIGH_OIL_TEMP_WARNING_EN_FROM_ECU].tempValue.u8Val = CFGZ::CFGZ_DISABLE;

        ArrEditableItem[INDEX_OF_ENG_SPEED_FROM_ECU].value.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_LOP_FROM_ECU].value.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU].value.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_ENG_RUN_HOURS_FROM_ECU].value.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_BAT_VTG_FROM_ECU].value.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_OIL_TEMP_FROM_ECU].value.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_HIGH_OIL_TEMP_SHUTDOWN_EN_FROM_ECU].value.u8Val = CFGZ::CFGZ_DISABLE;
        ArrEditableItem[INDEX_OF_HIGH_OIL_TEMP_WARNING_EN_FROM_ECU].value.u8Val = CFGZ::CFGZ_DISABLE;

        ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACTION].tempValue.u8Val = CFGZ::CFGZ_ACTION_NONE;
        ArrEditableItem[INDEX_OF_AMBER_LAMP_ACTION].tempValue.u8Val = CFGZ::CFGZ_ACTION_NONE;
        ArrEditableItem[INDEX_OF_RED_LAMP_ACTION].tempValue.u8Val = CFGZ::CFGZ_ACTION_NONE;
        ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACTION].tempValue.u8Val = CFGZ::CFGZ_ACTION_NONE;
        ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACTION].tempValue.u8Val = CFGZ::CFGZ_ACTION_NONE;
    }
    else
    {
        for(uint16_t u16LocalIndex = INDEX_OF_ENG_SPEED_FROM_ECU ; u16LocalIndex <= INDEX_OF_ECU_SOURCE_ADDRESS; u16LocalIndex++)
        {
            prvSetPasswordAccessLevel(u16LocalIndex , (uint8_t)CEditableItem::PIN1_ALLOWED);
        }
        for(uint16_t u16LocalIndex = INDEX_OF_HIGH_OIL_TEMP_SHUTDOWN_EN_FROM_ECU ; u16LocalIndex <= INDEX_OF_HIGH_OIL_TEMP_WARNING_THRESH_FROM_ECU; u16LocalIndex++)
        {
            prvSetPasswordAccessLevel(u16LocalIndex , (uint8_t)CEditableItem::PIN1_ALLOWED);
        }
    }
}

void UI::prvSetPasswordAccessLevel(uint16_t u16Index,uint8_t u8PasswordLevel)
{
    if((u8PasswordLevel >= CEditableItem::NOT_ALLOWED)&&(u8PasswordLevel <=CEditableItem::PIN1_PIN2_PIN3_ALLOWED ))
    {
        ArrEditableItem[u16Index].u8PasswordLevel = u8PasswordLevel;
    }
}

void UI::GetSrNoByIndex(CEditableItem::SRNO_TYPES_t eSrNoType, uint8_t* pu8Srno)
{
    uint8_t* pu8Src;
    uint8_t u8Size;
    if((eSrNoType < CEditableItem::SRNO_TYPE_LAST) && (pu8Srno != 0))
    {
        switch(eSrNoType)
        {
        case(CEditableItem::SRNO_GENSET):
        {
            pu8Src = &_stSrNos.u8GenSrNo[0];
            u8Size = sizeof(_stSrNos.u8GenSrNo);
        }
        break;
        case(CEditableItem::SRNO_ENGINE):
        {
            pu8Src = &_stSrNos.u8EngSrNo[0];
            u8Size = sizeof(_stSrNos.u8EngSrNo);  
        }
        break;
        case(CEditableItem::SRNO_ALT):
        {
            pu8Src = &_stSrNos.u8AltSrNo[0];
            u8Size = sizeof(_stSrNos.u8AltSrNo);  
        }
        break;
        case(CEditableItem::SRNO_MAINCONT):
        {
            pu8Src = &_stSrNos.u8MainContSrNo[0];
            u8Size = sizeof(_stSrNos.u8MainContSrNo);
        }
        break;
        case(CEditableItem::SRNO_ENGCONT):
        {
            pu8Src = &_stSrNos.u8EngContSrNo[0];
            u8Size = sizeof(_stSrNos.u8EngContSrNo);  
        }
        break;
        case(CEditableItem::SRNO_SITEID):
        {
            pu8Src = &_stSrNos.u8SiteId[0];
            u8Size = sizeof(_stSrNos.u8SiteId);  
        }
        break;
        default:
            pu8Src = &_stSrNos.u8EngSrNo[0];
            u8Size = sizeof(_stSrNos.u8EngSrNo);
        break;
        }
        memcpy((void*)pu8Srno, (void*)pu8Src, u8Size);
    }  
}

void UI::StoreSrNo()
{
    bSrNosEdited = true;  
}
