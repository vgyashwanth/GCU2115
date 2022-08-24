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

//All editable option string screens
enum
{
   ID_MANUAL_AUTO,
   ID_LANGUAGE,
   ID_ENABLE_DISABLE,
   ID_MODBUS,
   ID_PARITY,
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

   ID_YES_NO,
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

static const char* strOutputSources[2][84] =
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
        "Dig In J(LOP Resistive)",
        "Dig In K(Anlg In Fuel LVL)",
        "Dig In L(Anlg In Eng Temp)",
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
        "L1 Phase OV Shutdown",
        "L1 Phase UV Shutdown",
        "L2 Phase OV Shutdown",
        "L2 Phase UV Shutdown",
        "L3 Phase OV Shutdown",
        "L3 Phase UV Shutdown",
        "Gen Over Current",
        "High Engine CLNT Temp",
        "Low Fuel LVL",
        "Low LOP",
        "Mains High Volt",
        "Mains Low Volt",
        "Oil Pressure Open Circuit",
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
        "Preheat Output",
        "Calling For Scheduler Run",
        "Stop and Panel Lock",
        "External Panel Lock",
        "Fail To Close Generator",
        "Fail To Close Mains",
        "Loading Volt Not Reached",
        "Loading Freq Not Reached",
        "MPU Loss",
        "BTS Battery Hybrid Mode",
        "Automatic Fuel Transfer",
        "ISV Pull Signal",
        "ISV Hold Signal",
        "Idle Mode On",

        "CLNT Temp Ctrl",
        "Key Switch",

        "Open Gen Contactor Pulse",
        "Close Gen Contactor Pulse",
        "Open Mains Cntctr Pulse",
        "Close Mains Cntctr Pulse",

        "Idle Mode On Pulse",
        "Idle Mode Off Pulse"
   },
   {
        "Desactivado",
        "Alarma Sonora",
        "Alto Voltaje de Bateria",
        "Bajo Voltaje de Bateria",
        "Paro Carga de Alternador",
        "Alarm Carg Alt",
        "Cerrar Interruptor Gen",
        "Cerrar Interruptor Red",

        "Falla de Red",
        "Alarma Comun",
        "Disparo Electrico Comun",
        "Paro Comun",
        "Advertencia Comun",
        "Enfriamiento",
        "Entrada Dig A",
        "Entrada Dig B",
        "Entrada Dig C",
        "Entrada Dig D",
        "Entrada Dig E",
        "Entrada Dig F",
        "Entrada Dig G",
        "Entrada Dig H",
        "Entrada Dig I",
        "Dig In J (LOP Resistive) ",
        "Dig In K (Anlg In Fuel LVL)",
        "Dig In L (Anlg In Eng Temp)",
        "Dig In M (Aux Sensor 1)",
        "Dig In N (Aux Sensor 2)",
        "Dig In O (Aux Sensor 3)",
        "Dig In P (Aux Sensor 4)",
        "Paro de Eergencia",
        "Solenoide de Paro",
        "Falla de arranque",
        "Falla de Paro",
        "Rele de Combustible",
        "Generador Disponible",
        "Sobre Voltaje L1 Paro",
        "Bajo Voltaje L1 Paro",
        "Sobre Voltaje L2 Paro",
        "Bajo Voltaje L2 Paro",
        "Sobre Voltaje L3 Paro",
        "Bajo Voltaje L3 Paro",
        "SobreCorr Gen",
        "Alta Temp de Refrigerante",
        "Bajo Nivel de Combustible",
        "Bajo LOP",
        "Alto Voltaje de Red",
        "Bajo Voltaje de Red",
        "Circ. Ab. Sen Pres",
        "Abrir Interruptor de Gen",
        "Abrir Interruptor de Red",
        "Sobre Frecuencia Paro",
        "Sobre Velocidad Paro",
        "Paro Alta Sobrevel",
        "Rele de Marcha",
        "Circ. Ab Sen Temp.",
        "Baja Frecuencia Paro",
        "Baja Velocidad Paro",
        "Mantenimiento en",
        "Modo Paro",
        "Modo Auto",
        "Modo Manual",
        "Salida de Precalentador",
        "Arran. program.",
        "Paro y Bloque de Panel",
        "Bloqueo de Panel Externo",
        "Falla de cierre de Int de Gen",
        "Falla de cierre de Int de Red",
        "Voltaje no Alcanzado",
        "Frecuencia no alcanzada",
        "Circuito MPU Abierto",
        "BTS Modo Hibrido Baterias",
        "Tranf Auto Comb",
        "ISV Senal de Jalar",
        "ISV Senal de Mantener",
        "Velocidad de Ralenti",

        "Ctrl de temp de ref",
        "Switch de llave",
        "Pulso  apertura cont GEN",
        "Pulso cierre cont GEN",
        "Pulso apertura cont RED",
        "Pulso cierre cont RED",
        "Pulso On Modo Bajas Rpm",
        "Pulso Off Modo Bajas Rpm",
     }
};

static const char* strInputSources[2][27]=
{
 {
       "Not Used",
       "User Configured",
       "Low Fuel LVL Switch",
       "Low Lube Oil Press Switch",
       "High Eng CLNT Temp SW",
       "Low Water LVL Switch",
       "Emergency Stop",
       "Remote Start / Stop",
       "Simulate Start",
       "Simulate Stop",
       "Simulate Auto",
       "Close Gen/Opn Mains Swch",
       "Close Mains/Opn Gen Swch",
       "Simulate Mains",
       "V-Belt Broken Switch",
       "Mains Contactor Latched",
       "Genset Contactor Latched",
       "Battery Charger Fail",
       "Smoke Fire",
       "Remote Alarm Mute",
       "Remote Alarm Acknowledge",
       "Stop and Panel Lock",
       "External Panel Lock",
       "Generator Load Inhibit",
       "Mains Load Inhibit",
      // "Neutral SW Signal",
       "Regen SW Inhibit Signal",
       "Idle Mode Enable",
   },
   {
       "Sin Uso",
       "Configuracion Usuario",
       "Bajo Nivel de Comb. Switch",
       "Baja P. de Aceite. Sw",
       "Alta Temp Ref. Sw",
       "Bajo N. de Ref. Sw",
       "Paro de Emergencia",
       "Arranque/Paro Remoto",
       "Simular Arranque",
       "Simular Paro",
       "Simular Auto",
       "C Gen/Abrir Red Sw",
       "C Red/Abrir Gen Sw",
       "Simular Red",
       "Banda Rota Switch",
       "Contactor de Red Cerrado",
       "Contactor de Red Abierto",
       "Falla Carg Bat",
       "Humo Incendio Detectado",
       "Silenciar Alarma Externo",
       "Res Alarm Ext",
       "Paro y Bloque de Panel",
       "Bloqueo de Panel Externo",
       "Inhibir Carga Generador",
       "Inhibir Carga Red",
      // "Neutral SW Signal",
       "Senal de inhibicion regen SW",
       "Modo bajas RPM Habilitar",
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
static const char* strOptions[2][ID_LAST][8]=
{
 {
  {
      "Manual",
      "Auto"
  },
  {
       "English",
       "Spanish",
       "Chinese"
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
      "No",
      "Yes"
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
 },
 {
  {
      "Manual",
      "Auto"
  },
  {
      "Ingles",
      "Spanish",
      "Chino"
  },
  {    "Desactivado",
       "Activado"
  },
  {
      "Nunca",
      "MODBUS"
  },
  {
      "Nunca",
      "Even",
      "Odd"
  },
  {
      "Diario",
      "Semanal",
      "Mensual"
  },
  {
     "Cerrar para activar",
     "Abrir para activar"
  },
  {
      "Nunca",
      "Aviso",
      "Advertencia",
      "Disparo Electrico",
      "Paro"
  },
  {
      "Nunca ",
      "De Arranque",
      "Dese Monitoreo Encendido",
      "Siempre"
  },
  {
       "Sin Uso",
       "Entrada Dig J",
       "Sensor LOP"
  },
  {
       "Sin Uso",
       "Entrada Dig K",
       "Fuel LVL Sensor"
  },
  {
       "Sin Uso",
       "Entrada Dig L",
       "Temp Ref Motor"
  },

  {
       "Sin Uso",
       "Entrada Dig M",
       "Sensor S1",
       "Temp Caseta"
  },
  {
       "Sin Uso",
       "Entrada Dig N",
       "Sensor S2"
  },
  {
       "Sin Uso",
       "Entrada Dig O",
       "Sensor 4-20mA",
       "Sensor 0-5 V",
       "Presion de Aceite(4-20mA)"
  },
  {
       "Sin Uso",
       "Entrada Dig P",
       "Sensor 4-20mA",
       "Sensor 0-5V"
  },
  {
      "Cuerpo de Motor",
      "Negativo de Bteria"
  },
  {
      "Desenergizado",
      "Energizado"
  },
  {
      "No",
      "Si"
  },
  {
      "2 Polos",
      "4 Polos",
      "6 Polos",
      "8 Polos"
  },
  {
      "1 Fase(L1-N)",
      "3 Fase(L1-L2-L3-N)",
      "Split Fase(L1-N-L2)"
  },
  {
      "Corriente de Falla a Tierra",
      "Corriente de Ventilador"
  },
  {
      "Nunca",
      "Aviso"
  },
  {
      "Frecuencia Alterna",
      "Magnetic pickup",
      "W Alternador"
  },
  {
      "Nunca",
      "Aviso",
      "Advertencia",
      "Disparo Electrico",
      "Paro"
  },
  {
      "Nunca",
      "Aviso",
      "Advertencia",
      "Paro"
  },
  {
      "Nunca",
      "Aviso",
      "Advertencia"
  },

  {
      "Como E Gobernador",
      "Ctrl de Arran/Paro"
  },
  {
       "Manesillas del Reloj",
       "Anti Manesillas del Reloj"
  },
  {
       "Caida  0%",
       "Speed Bias Inp (0-5V)",
       "Carga basada en caida"
  },
  {
       "Control de precalentador",
       "Control de enfriamiento"
  },
  {
       "TC en Generador ",
       "TC en Carga"
   },
   {
       "Domingo",
       "Lunes",
       "Martes",
       "Miercoles",
       "Jueves",
       "Viernes",
       "Sabado"
   },
   {
      "Todos los dias"
   },
   {
      "Menor que Umbral",
      "Mayor que Umbral"
   },
   {
     "50HZ",
     "60HZ"
   },
   {
      "Nunca",
      "De Arranque",
      "Dese Monitoreo Encendido",
      "Siempre",
      "en enc Rele Comb"
   },
   {
       "Aviso",
       "Advertencia",
       "Disparo Electrico",
       "Paro"
   },
   {
        " "
   }
 }
};

static const char* strMainMenu[2][ID_MAIN_MENU_LAST]
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
        "ROTARY ACTUATOR",
        "ID",
        "RESET PARAMETERS",
        "SELECT PROFILE",
        "DISP VOLT FILT"
    },
    {
       	"MODULO",
       	"ENTRADAS",
       	"SALIDAS",
       	"TEMPORIZADORES",
        "GENERADOR",
        "RED",
       	"MOTOR",
       	"MANTENIMIENTO",
       	"ACTUADOR",
       	"ID",
       	"RESET PARAM",
        "SELEC PERFIL",
        "VER FLTR VOLT"
    }
};

static const char* strSubMenu[2][ID_SUB_MENU_LAST]
{
    {
        //Module
         "GENERAL",
         "DISPLAY",
         "COMMUNICATION",
         "SITE BAT CONFIG",
         "CYCLIC CONFIG",
         "AUTO EXERCISE 1",
         "AUTO EXERCISE 2",
         "NIGHT MODE",
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
         "LOP/DIG J",
         "FUEL LVL/DIG K",
         "ENG CLNT T/DIG L",
         "AUX S1 RES/DIG M",
         "AUX S2 RES/DIG N",
         "AUX S3/DIG O",
         "AUX S4/DIG P",
       //Outputs
         "OUTPUT A",
         "OUTPUT B",
         "OUTPUT C",
         "OUTPUT D",
         "OUTPUT E",
         "OUTPUT F",
         "OUTPUT G",
       //Timers
         "CRANKING TIMER",
         "START/STOP TIMER",
         "GENERAL TIMER",
       //Generator
         "ALT CONFIG",
         "VOLT MONITOR",
         "FREQ MONITOR",
         "CURRENT MONITOR",
         "EARTH/FAN MON",
         "LOAD MONITOR",
       //Mains
         "MAINS CONFIG",
         "VOLTAGE MON",
         "FREQUENCY MON",
       //Engine
         "CRANK DISCONN",
         "SPEED MONITOR",
         "BATTERY MONITOR",
         "CHARGE ALT MON",
         "PREHEAT",
         "CLNT TEMP CTRL",
         "AFT FUNCTION",
         "ENG CONTROL UNIT",
         "LUBE OIL PRESSURE",
         "ENG CLNT TMP",
       //Maintenance
         "MAINT ALARM",
         "ALARM DUE DATE",

       //Rotary Actuator
         "GENERAL",
         "ENG START STRGY",
         "GEN EGOV CNFG",
         "STR/STP CNFG",

       //Password
         "ENG SR NO",
         "PASSWORD 1",
         "PASSWORD 2",

       //Reset Parameters
         "GENSET",
         "MAINS",

       //Select Profile
         "SELECT PROFILE",

        //Disp Volt Filt
         "DISP VOLT FILT"
    },
    {
     //Module
     "GENERAL",
     "PANTALLA",
     "COMUNICACION",
     "CONFIG BATERIA",
     "CONFIG CICLICA",
     "ARRAN PROGRAM 1",
     "ARRAN PROGRAM 2",
     "MODO NOCTURNO",
     //Inputs
     "ENT DIG A",
     "ENT DIG B",
     "ENT DIG C",
     "ENT DIG D",
     "ENT DIG E",
     "ENT DIG F",
     "ENT DIG G",
     "ENT DIG H",
     "ENT DIG I",
     "BPA RES/DIG J",
     "NIV COMB/DIG K",
     "TEMP REF/DIG L",
     "AUX S1 RES/DIG M",
     "AUX S2 RES/DIG N",
     "AUX S3 RES/DIG O",
     "AUX S4 RES/DIG P",
     //Outputs
     "SALIDA A",
     "SALIDA B",
     "SALIDA C",
     "SALIDA D",
     "SALIDA E",
     "SALIDA F",
     "SALIDA G",
     //Timers
     "TEMPO MARCHA",
     "TEMP ARRAN/PARO",
     "TEMPO GENERAL",
     //Generator
     "CONFIG ALT",
     "MONITOR VOLT",
     "MONITOR FREC",
     "MONITOR CORR",
     "MONITOR TIERRA",
     "MONITOR CARGA",
     //Mains
     "CONFIG RED",
     "MON VOLTAJE",
     "MON FREC",
     //Engine
     "DESCON MARCHA",
     "MONITOR VEL",
     "MON BATERIA",
     "MON CARGA ALT",
     "PRECALENTADOR",
     "CTRL TEMP REF",
     "FUNCION AFT",
     "UNI CTRL MOT",
     "PRESION ACEITE",
     "TEMP DE REF",
     //Maintenance
     "ALARM MANTEN",
     "ALARM FECHA",

     //Rotary Actuator
     "GENERAL",
     "ESTR  ARRAN MOT",
     "CONFIG GOB E",
     "DISP ARRA/PARO",

     //Password
     "ENG SR NO",
     "CONTRASENA 1",
     "CONTRASENA 2",

     //Reset Parameters
     "GENERADOR",
     "RED",

     //Select Profile
     "SELEC PERFIL",

     //Disp Volt Filt
     "VER FLTR VOLT"
    }

};

static const char* strLeafNode[2][SID_LEAF_NODE_STRING]
{
    {
        "PROFILE NAME",
        "POWER ON MODE",
        "POWER ON LAMP TEST",
        "DEEP SLEEP MODE",
        "HISTOGRAM",
        "WARNING AUTO CLEAR",
        "LANGUAGE",
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
    },
    {
     "PERFIL",
     "MODO ENCENDIDO",
     "TEST LAMPARAS IND",
     "MODO DESCANSO",
     "CARGA HISTOGRAMA",
     "AVISO DE BORRAR",
     "AYUDA LENGUAJE",
     "CONTRASTE",
     "MODO AHORRO",
     "MODO COMUNIC",
     "ID MODBUS ESC.",
     "BAUDRATE",
     "PARIDAD",
     "MON BATERIA",
     "BATERIA BAJA",
     "MON BATERIA BAJA T",
     "DURACION GEN ARR",
     "MODO CICLICO",
     "DURAICION GEN APAG",
     "DURACION GEN ENC",
     "EVENTO 1",
     "RECURRENCIA EVENTO",
     "EVENTO DIA",
     "HORA DE ARRANQUE",
     "DURACION GEN ENC",
     "TRANSFERENCIA CARGA",
     "EVENTO 2",
     "RESTRINGIR MODO NOCT",
     "DURAICION GEN APAG",

     //Inputs
     "FUENTE",
     "POLARIDAD",
     "ACCION",
     "ACTIVACION",
     "RETARDO ACTIVACION",
     "SELECCION SENSOR",
     "PARO",
     "UMBRAL PARO",
     "AVISO",
     "UMBRAL DE AVISO",
     "ACC FALLO CIRCUITO",
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
     "PARO BAJO NIVEL" ,
     "UMBRAL DISPARO",
     "AVISO BAJO NIVEL",
     "UMBRAL AVISO",
     "CAPACIDAD TANQ COMB",
     "ALARM ROBO COMB",
     "UMBRAL ROBO COMB",
     "REF SENSOR COMB",
     "TIPO DE UMBRAL",
     "ALARM CIRC ABIERTO",
     "UMBRAL TEMP CASETA",
     "TEMP CASETA HISTE",
     "TEMP CASETA RET",
     "DURACION GEN ARR",
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
     "SOB ACTIVACION",

     //Timers
     "MARCHA T ACTIVACION",
     "DESCANSO MARCHA",
     "RET ARRAN MANUAL",
     "RET ARRAN AUTO",
     "RET MON SEGUR",
     "RET CALENTAMIENTO",
     "RET REGRESO DE RED",
     "T DE ENFRIAMIENTO",
     "T DE PARO",
     "T ADICIONAL DE PARO",
     "RET DE TRANSF CARGA",
     "RET MODO AHORRO",
     "T CAMBIO PANTALLA",
     "RET MODO DESCANSO",
     "T DE ALARMA SONORA",
     "T MODO TEST",
     "MOD AUTO SAL CNFG",
     "VALV PARO TEMPO.",
     "T.PULSO INTERRUPTOR GEN",
     "T.PULSO INTERRUPTOR RED",
     "T. RETROALIMENTACION INT",
     "RETARDO CIERRE INTERRUPTOR",
     "ALTERNADOR PRESENTE",
     "NUMERO DE POLOS",
     "ALT SISTEMA CA",
     "VOLT OPERATIVO MIN",
     "FREC OPERATIVA MIN",
     "SEC FASES INVERSA",
     "ACC SEC DE FASES INV",
     "TRANSF. AUTO DE CARGA",
     "FORMA DE ONDA DETECT",
     "TP GEN HABILITADO",
     "TP PRIMARIO GEN",
     "TP SECUNDARIO GEN",
     "PARO BAJO VOLTAJE",
     "PARO BAJO VOLT",
     "BV RETARDO DE PARO",
     "AVISO BAJO VOLTAJE",
     "AVISO BAJO VOLT",
     "BV RETARDO DE ALARMA",
     "PARO SOBRE VOLTAJE",
     "PARO SOBREVOLT",
     "SV RETARDO DE PARO",
     "AVISO SOBRE VOLTAJE",
     "AVISO SOBREVOLT",
     "SV RETARDO DE ALARMA",
     "PARO BAJA FREC",
     "UMBRAL BAJA FREC",
     "BF RETARDO DE PARO",
     "AVISO BAJA FREC",
     " AVISO BAJA FREC",
     "BF RETARDO DE ALARMA",
     "PARO SOBREFREC",
     "PARO SOBREFREC",
     "SF RETARDO DE PARO",
     "AVISO SOBREFREC",
     " AVISO SOBREFREC",
     "SF RETARDO DE ALARMA",
     "REL TRANF CORRIENTE",
     "ACC SOBRECORRIENTE",
     " ACCION SOBRECORR",
     "RET SOBRECORRIENTE",
     "POSICION TC",
     "MON CORR TIERRA",
     "REL TRANF CORR",
     "ACC SOBRECORRIENTE",
     "SOBRECORR",
     "RET SOBRECORRIENTE",
     "ACC BAJACORRIENTE",
     "UMBRAL BAJA CORRIENTE",
     "CAPACIDAD GEN",
     "ACCION SOBRECARGA",
     "UMBRAL SOBRECARGA",
     "RET MON SOBRECAR",
     "ACC DESBAL CARGA",
     "DESBAL DE CARGA",
     "RET DESBAL DE CARGA",
     "BAJA CARGA HABILITAR",
     "ACCION BAJA CARGA",
     "DISPARO POR BAJA CARGA",
     "REGRESO BAJA CARGA",
     "RETARDO BAJA CARGA",
     "MONITOREO DE RED",
     "SISTEMA CA RED",
     "SEC INVERSA FASES",
     "ACC SEC INVERSA",
     "RED CASI OPERATIVA",

     "TP RED HABILITADO",
     "TP PRIMARIO RED",
     "TP SECUNDARIO RED",
     "BV HABILITADO",
     "BJ DISPARO",
     "BJ REGRESO",
     "AB HABILITADO",
     "AB DISPARO",
     "AV REGRESO",
     "RET. DISPARO VOLT RED",
     "BF HABILITADO",
     "BF DISPARO",
     "BF REGRESO",
     "AF HABILITADO",
     "AF DISPARO",
     "AF REGRESO",
     "RET. DIPARO FREC RED",
     "INTENTOS DE ARRANQUE",
     "DESCON SENS BPA",
     "DESCON BPA UMBRA",
     "MON BPA ANT MARCHA",
     "MON BPA ANTES ARR",
     "DESCON CONT BPA",
     "BPA  CONT T. TRANS",
     "FRECUENCIA ALT",
     "VELOCIDAD MOTOR",
     "DESC CRGA VOLT ALT",
     "UMBRAL CRGA ALT ",
     "SENSOR VELOCIDAD",
     "CREMALL PT FREC",
     "BAJA VELOCIDAD PARO",
     "UMBRAL DE BAJA VEL",
     "RET BAJA VELOCIDAD",
     "UMBRAL SOBREVEL",
     "RET SOBREVEL",
     "UMBRAL BRUTO SV",
     "RET. BAJA RPM A NOM",
     "T. ARRANQUE BAJA RPM",
     "T. PARO BAJA RPM",
     "MODO BAJA RPM T PULSO",
     "INICIAL VEL RALENTI",
     "ACCION BAJO VOLTAJE",
     "UMBRAL BAJO VOLTAJE",
     "RET BAJO VOLTAJE",
     "ACCION ALTO VOLTAJE",
     "UMBRAL ALTO VOLTAJE",
     "RET ALTO VOLTAJE",
     "ACCION FALLO",
     "UMBRAL FALLO",
     "RESTRASO FALLO",
     "TEMPO PRECA",
     "TEMP MOTOR HABILITAR",
     "LIMITE REF TEMP MOTOR",
     "ACTIVADO",
     "TEMP CTRL",
     "UMBRAL ENCENDIDO",
     "UMBRAL APAGADO",
     "TCA UMBRAL ACT",
     "TCA UMBRAL DEACT",
     "T FIN DESPUES ACT",
     "MON TCA MOT RODA",
     "TIPO DE MOTOR",
     "BPA ECU",
     "TEMP REF ECU",
     "VEL MOTOR ECU",
     "HRS OPER ECU",
     "V BATT ECU",
     "VEL MOT A ECU",
     "VEL REQUERIDA",
     "COMAN ARR/PARO",
     "COMAN PRECAL",
     "FREC. MOTOR",
     "GANANCIA MOTOR",
     "SGC DIRECCION FUENTE",
     "ECU DIRECCION FUENTE",
     "ACCION FALLO COM",
     "ACTIVACION(COMM FLR)",
     "RET. ACT(COMM FLR)",
     "ACCION(AMBAR)",
     "ACTIVACION(AMBAR)",
     "RETARDO ACT(AMBAR)",
     "ACCION(ROJO)",
     "ACTIVACION(ROJO)",
     "RET ACT(ROJO)",
     "ACCION (FALLO)",
     "ACTIVACION(FALLO)",
     "RET (FALLO)",
     "ACCION (PROTEC)",
     "ACT (PROTEC)",
     "ACT RET (PROTECT)",
     "BAJO  PARO",
     "UMBRAL BAJO  PARO",
     "BAJO  AVISO",
     "UMBRAL BAJO  AVISO",
     "TEMP REFR. TIPO UMBRAL",
     "A TEMP REF PARO",
     "UMBRAL PARO",
     "A TEMP REF AVISO",
     "UMBRAL DE AVISO",
     "ACCION",
     "POR HORAS MOTOR",
     "MAINT POR CENIZA",
     "ALARM DUE DAY",
     "ALARM DUE MONTH",
     "ALARM DUE YEAR",
     "APL. ACTUADOR",
     "VEL ACTUADOR",
     "DIRECCION ACT",
     "SEL VEL",
     "CAIDA CARGA",
     "OBJ/RECUP VEL",
     "GAN PROP",
     "GAN INTEGRAL",
     "GAN DERIV",
     "APAG FRICCION",
     "DISP GAN PROG",
     "FACTOR CARGA",
     "FACTOR DESCARGA",
     "PASOS MARCHA",
     "PID VEL",
     "TIEMPO RAMPA SUBIDA",
     "TIEMPO PID ACTIVO",
     "PASO DE ARRANQUE",
     "HORAS DE OPERACION",
     "NO DE ARRANQUES",
     "NO DE DISPAROS",
     "ENG kWh",
     "ENG kVAh",
     "ENG kVArh",
     "RUN TIME",
     "MAINS kWh",
     "MAINS kVAh",
     "MAINS kVArh",
     "FILTRO ACTIVO",
     "FILTRO CONSTANTE"
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

    _u8LanguageArrayIndex =  _objcfgz.GetArrLanguageIndex();

    for(uint8_t i=0; i<12; i++)
    {
        memcpy( &arrMonth[i], &StrMonth[_u8LanguageArrayIndex][i], 21);
    }


    ArrEditableItem[INDEX_OF_POWER_ON_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_POWER_ON_MODE), strLeafNode[_u8LanguageArrayIndex][SID_POWER_ON_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MANUAL_AUTO], 2  , CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_POWER_ON_LAMP_TEST] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_POWER_ON_LAMP_TEST_EN),strLeafNode[_u8LanguageArrayIndex][SID_POWER_ON_LAMP_TEST], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DEEP_SLEEP_ENABLE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DEEP_SLEEP_EN),strLeafNode[_u8LanguageArrayIndex][SID_DEEP_SLEEP_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOAD_HISTOGRAM] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOAD_HISTOGRAM),strLeafNode[_u8LanguageArrayIndex][SID_LOAD_HISTOGRAM], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_WARNING_AUTO_CLEAR] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_WARNING_AUTO_CLEAR_EN),strLeafNode[_u8LanguageArrayIndex][SID_AUTO_CLEAR_WARNINGS], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LANGUAGE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE), strLeafNode[_u8LanguageArrayIndex][SID_LANGUAGE_SUPPORT], "", "%s", strOptions[_u8LanguageArrayIndex][ID_LANGUAGE], 3  , CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_CONTRAST] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PERCENT_CONTRAST), strLeafNode[_u8LanguageArrayIndex][SID_CONTRAST], arrUnit[ID_PERCENT], "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_POWER_SAVE_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_POWER_SAVE_MODE_EN),strLeafNode[_u8LanguageArrayIndex][SID_POWER_SAVE_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_COMM_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_COMMUNICATION_EN_MB),strLeafNode[_u8LanguageArrayIndex][SID_COMM_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MODBUS], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_SLAVE_ID] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SLAVEID_MB), strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_SLAVE_ID], "", "%u", (uint8_t)1, (uint8_t)247, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_BAUDRATE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BAUDRATE_MB),strLeafNode[_u8LanguageArrayIndex][SID_BAUDRATE], "", "%s", strBaudrateOptions, 8, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_PARITYBIT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_PARITYBIT_MB),strLeafNode[_u8LanguageArrayIndex][SID_PARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_PARITY], 3, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_SITE_BATTERY_MON] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATT_MON_EN),strLeafNode[_u8LanguageArrayIndex][SID_BTS_BATTERY_MON], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SITE_LOW_BATT_THRESH] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOW_VOLT_THRESH),strLeafNode[_u8LanguageArrayIndex][SID_BTS_LOW_BATT_THRESHOLD], "", "%f", (float)12.0,(float) 60.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SITE_LOW_BATT_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BATT_MON_DELAY),strLeafNode[_u8LanguageArrayIndex][SID_BTS_LOW_BATT_MON_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)300, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SITE_GEN_RUN_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_RUN_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_BTS_GEN_RUN_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_CYCLIC_MODE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_MODE_EN),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CYCLIC_GEN_OFF_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_OFF_TIME),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_GEN_OFF_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CYCLIC_GEN_ON_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GEN_ON_TIME),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_GEN_ON_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );

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

    ArrEditableItem[INDEX_OF_NIGHT_MODE_RESTRICT] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_EN),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_MODE_RESTRICT], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_NIGHT_START_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_START_TIME),strLeafNode[_u8LanguageArrayIndex][SID_START_TIME],arrUnit[ID_HRS], "%u", (uint16_t)0, (uint16_t)2359,true, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_NIGHT_GEN_OFF_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_OFF_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_GEN_OFF_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)1440, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_A_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTA_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTA_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTA_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTA_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTA_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_B_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTB_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTB_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTB_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTB_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTB_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_C_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTC_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTC_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTC_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTC_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTC_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_D_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTD_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTD_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTD_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTD_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTD_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_E_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTE_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTE_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTE_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTE_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTE_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_F_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTF_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTF_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTF_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTF_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTF_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_G_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTG_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTG_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTG_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTG_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTG_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_H_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTH_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTH_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTH_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTH_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTH_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_I_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTI_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTI_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTI_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTI_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTI_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_SENS_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_LOP_DIG_J_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_J_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTJ_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_J_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTJ_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_J_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTJ_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_J_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTJ_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_J_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTJ_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_CKT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_SENS_FAULT_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_CKT_FAULT_ACTION],"", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_R1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R1), strLeafNode[_u8LanguageArrayIndex][SID_R1], arrUnit[ID_OHM], "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_V1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V1), strLeafNode[_u8LanguageArrayIndex][SID_V1], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_R2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R2), strLeafNode[_u8LanguageArrayIndex][SID_R2], arrUnit[ID_OHM], "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_V2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V2), strLeafNode[_u8LanguageArrayIndex][SID_V2], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_R3] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R3), strLeafNode[_u8LanguageArrayIndex][SID_R3], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_V3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V3), strLeafNode[_u8LanguageArrayIndex][SID_V3], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_R4] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R4), strLeafNode[_u8LanguageArrayIndex][SID_R4], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_V4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V4), strLeafNode[_u8LanguageArrayIndex][SID_V4], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_R5] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R5), strLeafNode[_u8LanguageArrayIndex][SID_R5], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_V5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V5), strLeafNode[_u8LanguageArrayIndex][SID_V5],"Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_R6] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R6), strLeafNode[_u8LanguageArrayIndex][SID_R6], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_V6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V6), strLeafNode[_u8LanguageArrayIndex][SID_V6], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_R7] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R7), strLeafNode[_u8LanguageArrayIndex][SID_R7], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_V7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V7), strLeafNode[_u8LanguageArrayIndex][SID_V7], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_R8] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R8), strLeafNode[_u8LanguageArrayIndex][SID_R8], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_V8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V8), strLeafNode[_u8LanguageArrayIndex][SID_V8], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_R9] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R9), strLeafNode[_u8LanguageArrayIndex][SID_R9], arrUnit[ID_OHM], "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_V9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V9), strLeafNode[_u8LanguageArrayIndex][SID_V9], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_R10] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_R10), strLeafNode[_u8LanguageArrayIndex][SID_R10], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_LOP_SENS_V10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_LOP_CALIB_V10), strLeafNode[_u8LanguageArrayIndex][SID_V10], "Bar", "%f", (float)0, (float)10.0, (float)0.1, CEditableItem::PIN1_ALLOWED );

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
    ArrEditableItem[INDEX_OF_FUEL_SENS_R1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R1), strLeafNode[_u8LanguageArrayIndex][SID_R1], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_V1] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V1), strLeafNode[_u8LanguageArrayIndex][SID_V1], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_R2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R2), strLeafNode[_u8LanguageArrayIndex][SID_R2], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_V2] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V2), strLeafNode[_u8LanguageArrayIndex][SID_V2], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_R3] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R3), strLeafNode[_u8LanguageArrayIndex][SID_R3], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_V3] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V3), strLeafNode[_u8LanguageArrayIndex][SID_V3], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_R4] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R4), strLeafNode[_u8LanguageArrayIndex][SID_R4], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_V4] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V4), strLeafNode[_u8LanguageArrayIndex][SID_V4], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_R5] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R5), strLeafNode[_u8LanguageArrayIndex][SID_R5], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_V5] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V5), strLeafNode[_u8LanguageArrayIndex][SID_V5], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_R6] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R6), strLeafNode[_u8LanguageArrayIndex][SID_R6], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_V6] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V6), strLeafNode[_u8LanguageArrayIndex][SID_V6], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_R7] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R7), strLeafNode[_u8LanguageArrayIndex][SID_R7], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_V7] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V7), strLeafNode[_u8LanguageArrayIndex][SID_V7], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_R8] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R8), strLeafNode[_u8LanguageArrayIndex][SID_R8], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_V8] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V8), strLeafNode[_u8LanguageArrayIndex][SID_V8], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_R9] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R9), strLeafNode[_u8LanguageArrayIndex][SID_R9], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_V9] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V9), strLeafNode[_u8LanguageArrayIndex][SID_V9], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_R10] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_R10), strLeafNode[_u8LanguageArrayIndex][SID_R10],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0,  CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_SENS_V10] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_CALIB_V10), strLeafNode[_u8LanguageArrayIndex][SID_V10], arrUnit[ID_PERCENT],  "%u", (uint8_t)0, (uint8_t)100,  CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENG_CLNT_DIG_L_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_L_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_L_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_L_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_L_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_L_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_ACTIVATION_DLY), strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_CKT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_FAULT_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_CKT_FAULT_ACTION],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R1] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R1), strLeafNode[_u8LanguageArrayIndex][SID_R1], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V1] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V1), strLeafNode[_u8LanguageArrayIndex][SID_V1],arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R2] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R2), strLeafNode[_u8LanguageArrayIndex][SID_R2], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V2] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V2), strLeafNode[_u8LanguageArrayIndex][SID_V2], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R3] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R3), strLeafNode[_u8LanguageArrayIndex][SID_R3],arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V3] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V3), strLeafNode[_u8LanguageArrayIndex][SID_V3], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R4] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R4), strLeafNode[_u8LanguageArrayIndex][SID_R4], arrUnit[ID_OHM], "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V4] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V4), strLeafNode[_u8LanguageArrayIndex][SID_V4], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R5] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R5), strLeafNode[_u8LanguageArrayIndex][SID_R5], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V5] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V5), strLeafNode[_u8LanguageArrayIndex][SID_V5], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R6] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R6), strLeafNode[_u8LanguageArrayIndex][SID_R6], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V6] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V6), strLeafNode[_u8LanguageArrayIndex][SID_V6], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R7] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R7), strLeafNode[_u8LanguageArrayIndex][SID_R7], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V7] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V7), strLeafNode[_u8LanguageArrayIndex][SID_V7], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R8] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R8), strLeafNode[_u8LanguageArrayIndex][SID_R8], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V8] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V8), strLeafNode[_u8LanguageArrayIndex][SID_V8], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R9] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R9), strLeafNode[_u8LanguageArrayIndex][SID_R9], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V9] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V9), strLeafNode[_u8LanguageArrayIndex][SID_V9], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R10] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_R10), strLeafNode[_u8LanguageArrayIndex][SID_R10], arrUnit[ID_OHM],  "%f", (uint16_t)0, (uint16_t)5000.0, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V10] = CEditableItem((int16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_CLNT_CALIB_V10), strLeafNode[_u8LanguageArrayIndex][SID_V10], arrUnit[ID_DEG_C],  "%f", (int16_t)-25.0, (int16_t)300.0, CEditableItem::PIN1_ALLOWED );

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
    ArrEditableItem[INDEX_OF_AUX_S2_DIG_N_SENSOR] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_SENS_SELECTION),strLeafNode[_u8LanguageArrayIndex][SID_SENSOR_SELECTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_N_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_N_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_N_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_N_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_N_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_ACTIVATION_DLY),strLeafNode[_u8LanguageArrayIndex][SID_ACTIVATION_DELAY],arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_THRESHOLD_TYPE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_THRESH_TYPE),strLeafNode[_u8LanguageArrayIndex][SID_THRESHOLD_TYPE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_THRESHOLD_TYPE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SHUTDOWN_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_SHUT_DN_EN),strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_SHUTDOWN_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_SHUTDOWN_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_SHUTDOWN_THRESHOLD], "", "%f", (float)0, (float)1000,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_WARNING_EN] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_WARN_EN), strLeafNode[_u8LanguageArrayIndex][SID_WARNING], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_WARNING_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_WARNING_THRESH), strLeafNode[_u8LanguageArrayIndex][SID_WARNING_THRESHOLD], "", "%f", (float)0, (float)1000,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_AUX_S2_CKT_FAULT_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_S2_SENS_FAULT_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_CKT_FAULT_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
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
    ArrEditableItem[INDEX_OF_DIG_OUT_A_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_SOURCE_A),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_OUT_A_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_ACTIVATION_A),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_OUT_B_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_SOURCE_B),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_OUT_B_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_ACTIVATION_B),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_OUT_C_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_SOURCE_C),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_OUT_C_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_ACTIVATION_C),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_OUT_D_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_SOURCE_D),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_OUT_D_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_ACTIVATION_D),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_OUT_E_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_SOURCE_E),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_OUT_E_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_ACTIVATION_E),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_OUT_F_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_SOURCE_F),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_OUT_F_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_ACTIVATION_F),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_OUT_G_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_SOURCE_G),strLeafNode[_u8LanguageArrayIndex][SID_SOURCE], "", "%s", strOutputSources[_u8LanguageArrayIndex], 84, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_OUT_G_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_OP_ACTIVATION_G),strLeafNode[_u8LanguageArrayIndex][SID_ON_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_OP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );

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


    ArrEditableItem[INDEX_OF_EGOV_ACT_APPLICATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_APPLICATION), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_ACT_APPLICATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ACTUATOR_APPLICATIONS], 2, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_ACT_SPEED] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_SPEED), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_ACT_SPEED], arrUnit[ID_ACT_SPEED_UNIT], "%u", (uint8_t)1, (uint8_t)10, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_DIR] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_DIRECTION), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_DIR], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ACT_DIRECTION], 2, CEditableItem::PIN1_ALLOWED);

    ArrEditableItem[INDEX_OF_EGOV_CRANK_BOOST] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_CRANK_BOOST), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_CRANK_BOOST], "", "%u", (uint16_t)50, (uint16_t)5000, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_RPM_PID_ON] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_RPM_PID_ON_SPEED), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_RPM_PID_ON], arrUnit[ID_RPM], "%u", (uint16_t)20, (uint16_t)2800, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_RAMP_UP_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_RAMP_UP_TIME), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_RAMP_UP_TIME], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)180, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_PID_ON_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_PID_ON_TIME), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_PID_ON_TIME], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)180, CEditableItem::PIN1_ALLOWED);

    ArrEditableItem[INDEX_OF_EGOV_SET_POINT_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_SET_POINT_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_SET_POINT_SELECTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_EGOV_SET_POINT_SELECTION], 2, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_PERCENT_DROOP] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_PERCENT_DROOP), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_PERCENT_DROOP], arrUnit[ID_PERCENT], "%u", (uint8_t)0, (uint8_t)4, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_GEN_SET_SPEED] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_SET_SPEED), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_GEN_SET_SPEED], arrUnit[ID_RPM], "%u", (uint16_t)500, (uint16_t)4000, CEditableItem::PIN1_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_GEN_KP] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_KP), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_GEN_KP], "", "%u", (uint16_t)0, (uint16_t)1000, CEditableItem::PIN1_PIN2_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_GEN_KI] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_KI), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_GEN_KI], "", "%u", (uint16_t)0, (uint16_t)2000, CEditableItem::PIN1_PIN2_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_GEN_KD] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_KD), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_GEN_KD], "", "%u", (uint16_t)0, (uint16_t)1000, CEditableItem::PIN1_PIN2_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_GEN_DITHER] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_DITHER), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_GEN_DITHER], "", "%u", (uint16_t)0, (uint16_t)1000, CEditableItem::PIN1_PIN2_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_GEN_GAIN_SCHEDULE] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_GAIN_SCHEDULE), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_GEN_GAIN_SCHEDULE], arrUnit[ID_PERCENT_RPM_ERROR], "%f", (float)0.0, (float)100.0, (float)0.1,CEditableItem::PIN1_PIN2_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_GEN_LOADING_FAC] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_LOADING_FAC), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_GEN_LOADING_FAC], "", "%u", (uint16_t)0, (uint16_t)1000, CEditableItem::PIN1_PIN2_ALLOWED);
    ArrEditableItem[INDEX_OF_EGOV_GEN_UNLOADING_FAC] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_UNLOADING_FAC), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_GEN_UNLOADING_FAC], "", "%u", (uint16_t)0, (uint16_t)1000, CEditableItem::PIN1_PIN2_ALLOWED);

    ArrEditableItem[INDEX_OF_EGOV_RUNNING_STEPS] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_EGOV_RUNNING_STEPS), strLeafNode[_u8LanguageArrayIndex][SID_EGOV_RUNNING_STEPS], "", "%u", (uint16_t)0, (uint16_t)500, CEditableItem::PIN1_ALLOWED);


    _objcfgz.ReadMiscParam(&_MiscParam);

    if(_MiscParam.u8MiscParam[RESET_COUNTER] > 2)
    {
        ArrEditableItem[INDEX_OF_RESET_ENG_RUN_HRS]     = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_RUN_TIME], arrUnit[ID_HRS], "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::NOT_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_NO_OF_STARTS] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_NO_OF_STARTS], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::NOT_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_NO_OF_TRIPS] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_NO_OF_TRIPS], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::NOT_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_GEN_KWH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_ENG_KWH], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::NOT_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_GEN_KVAH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_ENG_KVAH], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::NOT_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_GEN_KVARH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_ENG_KVARH], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::NOT_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_MAINS_RUN_HRS] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_MAINS_RUN_TIME], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::NOT_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_MAINS_KWH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_MAINS_KWH], "kWh", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::NOT_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_MAINS_KVAH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_MAINS_KVAH], "kVAh", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::NOT_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_MAINS_KVARH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_MAINS_KVARH], "kVArh", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::NOT_ALLOWED );
    }
    else
    {
        ArrEditableItem[INDEX_OF_RESET_ENG_RUN_HRS]     = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_RUN_TIME], arrUnit[ID_HRS], "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_NO_OF_STARTS] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_NO_OF_STARTS], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_NO_OF_TRIPS] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_NO_OF_TRIPS], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_GEN_KWH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_ENG_KWH], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_GEN_KVAH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_ENG_KVAH], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_GEN_KVARH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_ENG_KVARH], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_MAINS_RUN_HRS] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_MAINS_RUN_TIME], "", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_MAINS_KWH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_MAINS_KWH], "kWh", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_MAINS_KVAH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_MAINS_KVAH], "kVAh", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::PIN1_ALLOWED );
        ArrEditableItem[INDEX_OF_RESET_MAINS_KVARH] = CEditableItem((uint16_t)0U,strLeafNode[_u8LanguageArrayIndex][SID_MAINS_KVARH], "kVArh", "%u", (uint16_t)0, (uint16_t)65000, CEditableItem::PIN1_ALLOWED );
    }

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

    // if(_MiscParam.u8MiscParam[DISP_VOLT_EN] > CFGZ::CFGZ_ENABLE)
    //     {
    //         _MiscParam.u8MiscParam[DISP_VOLT_EN] =0;
    //     }
    // if(_MiscParam.u8MiscParam[DISP_VOLT_CONST] > MAX_DISP_CONST)
    //     {
    //         _MiscParam.u8MiscParam[DISP_VOLT_CONST] =50;
    //     }

    // ArrEditableItem[INDEX_OF_DISP_VOLT_FILT_EN]  = CEditableItem((uint8_t)_MiscParam.u8MiscParam[DISP_VOLT_EN],strLeafNode[_u8LanguageArrayIndex][SID_DISP_VOLT_EN], "",  "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_ALLOWED );
    // ArrEditableItem[INDEX_OF_DISP_VOLT_FILT_CONST]  = CEditableItem((uint8_t)_MiscParam.u8MiscParam[DISP_VOLT_CONST],strLeafNode[_u8LanguageArrayIndex][SID_DISP_VOLT_CONST], "",  "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_ALLOWED );

   ArrEditableItem[INDEX_OF_DISP_VOLT_FILT_EN]  = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISP_VOLT_FILT_EN),strLeafNode[_u8LanguageArrayIndex][SID_DISP_VOLT_EN], "",  "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO], 2, CEditableItem::PIN1_ALLOWED );
   ArrEditableItem[INDEX_OF_DISP_VOLT_FILT_CONST]  = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISP_VOLT_FILT_VAL),strLeafNode[_u8LanguageArrayIndex][SID_DISP_VOLT_CONST], "",  "%u", (uint8_t)0, (uint8_t)99, CEditableItem::PIN1_ALLOWED );

}

void UI::InitEditableItemsScreens()
{

    ArrEditableItemScreen[INDEX_OF_POWER_ON_MODE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL], &ArrEditableItem[INDEX_OF_POWER_ON_MODE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_POWER_ON_LAMP_TEST] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL], &ArrEditableItem[INDEX_OF_POWER_ON_LAMP_TEST], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DEEP_SLEEP_ENABLE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL], &ArrEditableItem[INDEX_OF_DEEP_SLEEP_ENABLE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOAD_HISTOGRAM] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL], &ArrEditableItem[INDEX_OF_LOAD_HISTOGRAM], 10, 20);
    ArrEditableItemScreen[INDEX_OF_WARNING_AUTO_CLEAR] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL], &ArrEditableItem[INDEX_OF_WARNING_AUTO_CLEAR], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LANGUAGE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL], &ArrEditableItem[INDEX_OF_LANGUAGE], 10, 20);

    ArrEditableItemScreen[INDEX_OF_CONTRAST] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DISPLAY], &ArrEditableItem[INDEX_OF_CONTRAST], 10, 20);
    ArrEditableItemScreen[INDEX_OF_POWER_SAVE_MODE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DISPLAY], &ArrEditableItem[INDEX_OF_POWER_SAVE_MODE], 10, 20);

    ArrEditableItemScreen[INDEX_OF_COMM_MODE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RS485_COMM], &ArrEditableItem[INDEX_OF_COMM_MODE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MODBUS_SLAVE_ID] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RS485_COMM], &ArrEditableItem[INDEX_OF_MODBUS_SLAVE_ID], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MODBUS_BAUDRATE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RS485_COMM], &ArrEditableItem[INDEX_OF_MODBUS_BAUDRATE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MODBUS_PARITYBIT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RS485_COMM], &ArrEditableItem[INDEX_OF_MODBUS_PARITYBIT], 10, 20);

    ArrEditableItemScreen[INDEX_OF_SITE_BATTERY_MON] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SITE_BAT_CONFIG], &ArrEditableItem[INDEX_OF_SITE_BATTERY_MON], 10, 20);
    ArrEditableItemScreen[INDEX_OF_SITE_LOW_BATT_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SITE_BAT_CONFIG], &ArrEditableItem[INDEX_OF_SITE_LOW_BATT_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_SITE_LOW_BATT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SITE_BAT_CONFIG], &ArrEditableItem[INDEX_OF_SITE_LOW_BATT_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_SITE_GEN_RUN_DURATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SITE_BAT_CONFIG], &ArrEditableItem[INDEX_OF_SITE_GEN_RUN_DURATION], 10, 20);

    ArrEditableItemScreen[INDEX_OF_CYCLIC_MODE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CYCLIC_CONFIG], &ArrEditableItem[INDEX_OF_CYCLIC_MODE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_CYCLIC_GEN_OFF_DURATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CYCLIC_CONFIG], &ArrEditableItem[INDEX_OF_CYCLIC_GEN_OFF_DURATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_CYCLIC_GEN_ON_DURATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CYCLIC_CONFIG], &ArrEditableItem[INDEX_OF_CYCLIC_GEN_ON_DURATION], 10, 20);

    ArrEditableItemScreen[INDEX_OF_EXERCISER_EVENT_1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE1], &ArrEditableItem[INDEX_OF_EXERCISER_EVENT_1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EXERCISER_EVENT_OCCURENCE_1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE1], &ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EXERCISER_EVENT_DAY_1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE1], &ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EXERCISER_START_TIME_1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE1], &ArrEditableItem[INDEX_OF_EXERCISER_START_TIME_1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EXERCISER_DG_ON_DURATION_1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE1], &ArrEditableItem[INDEX_OF_EXERCISER_DG_ON_DURATION_1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EXERCISER_LOAD_TRANSFER_1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE1], &ArrEditableItem[INDEX_OF_EXERCISER_LOAD_TRANSFER_1], 10, 20);

    ArrEditableItemScreen[INDEX_OF_EXERCISER_EVENT_2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE2], &ArrEditableItem[INDEX_OF_EXERCISER_EVENT_2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EXERCISER_EVENT_OCCURENCE_2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE2], &ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EXERCISER_EVENT_DAY_2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE2], &ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EXERCISER_START_TIME_2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE2], &ArrEditableItem[INDEX_OF_EXERCISER_START_TIME_2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EXERCISER_DG_ON_DURATION_2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE2], &ArrEditableItem[INDEX_OF_EXERCISER_DG_ON_DURATION_2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EXERCISER_LOAD_TRANSFER_2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE2], &ArrEditableItem[INDEX_OF_EXERCISER_LOAD_TRANSFER_2], 10, 20);

    ArrEditableItemScreen[INDEX_OF_NIGHT_MODE_RESTRICT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_NIGHT_MODE], &ArrEditableItem[INDEX_OF_NIGHT_MODE_RESTRICT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_NIGHT_START_TIME] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_NIGHT_MODE], &ArrEditableItem[INDEX_OF_NIGHT_START_TIME], 10, 20);
    ArrEditableItemScreen[INDEX_OF_NIGHT_GEN_OFF_DURATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_NIGHT_MODE], &ArrEditableItem[INDEX_OF_NIGHT_GEN_OFF_DURATION], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_IN_A_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_A], &ArrEditableItem[INDEX_OF_DIG_IN_A_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_A_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_A], &ArrEditableItem[INDEX_OF_DIG_IN_A_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_A_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_A], &ArrEditableItem[INDEX_OF_DIG_IN_A_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_A_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_A], &ArrEditableItem[INDEX_OF_DIG_IN_A_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_A_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_A], &ArrEditableItem[INDEX_OF_DIG_IN_A_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_IN_B_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_B], &ArrEditableItem[INDEX_OF_DIG_IN_B_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_B_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_B], &ArrEditableItem[INDEX_OF_DIG_IN_B_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_B_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_B], &ArrEditableItem[INDEX_OF_DIG_IN_B_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_B_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_B], &ArrEditableItem[INDEX_OF_DIG_IN_B_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_B_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_B], &ArrEditableItem[INDEX_OF_DIG_IN_B_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_IN_C_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_C], &ArrEditableItem[INDEX_OF_DIG_IN_C_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_C_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_C], &ArrEditableItem[INDEX_OF_DIG_IN_C_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_C_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_C], &ArrEditableItem[INDEX_OF_DIG_IN_C_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_C_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_C], &ArrEditableItem[INDEX_OF_DIG_IN_C_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_C_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_C], &ArrEditableItem[INDEX_OF_DIG_IN_C_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_IN_D_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_D], &ArrEditableItem[INDEX_OF_DIG_IN_D_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_D_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_D], &ArrEditableItem[INDEX_OF_DIG_IN_D_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_D_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_D], &ArrEditableItem[INDEX_OF_DIG_IN_D_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_D_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_D], &ArrEditableItem[INDEX_OF_DIG_IN_D_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_D_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_D], &ArrEditableItem[INDEX_OF_DIG_IN_D_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_IN_E_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_E], &ArrEditableItem[INDEX_OF_DIG_IN_E_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_E_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_E], &ArrEditableItem[INDEX_OF_DIG_IN_E_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_E_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_E], &ArrEditableItem[INDEX_OF_DIG_IN_E_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_E_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_E], &ArrEditableItem[INDEX_OF_DIG_IN_E_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_E_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_E], &ArrEditableItem[INDEX_OF_DIG_IN_E_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_IN_F_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_F], &ArrEditableItem[INDEX_OF_DIG_IN_F_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_F_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_F], &ArrEditableItem[INDEX_OF_DIG_IN_F_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_F_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_F], &ArrEditableItem[INDEX_OF_DIG_IN_F_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_F_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_F], &ArrEditableItem[INDEX_OF_DIG_IN_F_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_F_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_F], &ArrEditableItem[INDEX_OF_DIG_IN_F_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_IN_G_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_G], &ArrEditableItem[INDEX_OF_DIG_IN_G_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_G_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_G], &ArrEditableItem[INDEX_OF_DIG_IN_G_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_G_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_G], &ArrEditableItem[INDEX_OF_DIG_IN_G_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_G_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_G], &ArrEditableItem[INDEX_OF_DIG_IN_G_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_G_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_G], &ArrEditableItem[INDEX_OF_DIG_IN_G_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_IN_H_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_H], &ArrEditableItem[INDEX_OF_DIG_IN_H_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_H_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_H], &ArrEditableItem[INDEX_OF_DIG_IN_H_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_H_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_H], &ArrEditableItem[INDEX_OF_DIG_IN_H_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_H_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_H], &ArrEditableItem[INDEX_OF_DIG_IN_H_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_H_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_H], &ArrEditableItem[INDEX_OF_DIG_IN_H_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_IN_I_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_I], &ArrEditableItem[INDEX_OF_DIG_IN_I_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_I_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_I], &ArrEditableItem[INDEX_OF_DIG_IN_I_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_I_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_I], &ArrEditableItem[INDEX_OF_DIG_IN_I_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_I_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_I], &ArrEditableItem[INDEX_OF_DIG_IN_I_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_I_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_I], &ArrEditableItem[INDEX_OF_DIG_IN_I_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_LOP_DIG_J_SENSOR] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_J_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_DIG_IN_J_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_J_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_DIG_IN_J_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_J_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_DIG_IN_J_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_J_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_DIG_IN_J_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_J_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_DIG_IN_J_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_CKT_FAULT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_CKT_FAULT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_R1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_R1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_V1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_V1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_R2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_R2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_V2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_V2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_R3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_R3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_V3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_V3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_R4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_R4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_V4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_V4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_R5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_R5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_V5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_V5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_R6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_R6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_V6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_V6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_R7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_R7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_V7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_V7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_R8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_R8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_V8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_V8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_R9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_R9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_V9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_V9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_R10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_R10], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SENS_V10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrEditableItem[INDEX_OF_LOP_SENS_V10], 10, 20);

    ArrEditableItemScreen[INDEX_OF_FUEL_DIG_K_SENSOR] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_DIG_K_SENSOR], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_K_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_DIG_IN_K_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_K_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_DIG_IN_K_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_K_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_DIG_IN_K_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_K_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_DIG_IN_K_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_K_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_DIG_IN_K_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LFL_SHUTDOWN_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_LFL_SHUTDOWN_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LFL_SHUTDOWN_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_LFL_SHUTDOWN_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LFL_WARNING_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_LFL_WARNING_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LFL_WARNING_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_LFL_WARNING_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_TANK_CAPACITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_TANK_CAPACITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_THEFT_ALARM_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_THEFT_ALARM_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_THEFT_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_THEFT_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_CKT_FAULT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_CKT_FAULT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_REFERENCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_REFERENCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_R1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_R1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_V1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_V1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_R2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_R2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_V2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_V2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_R3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_R3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_V3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_V3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_R4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_R4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_V4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_V4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_R5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_R5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_V5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_V5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_R6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_R6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_V6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_V6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_R7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_R7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_V7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_V7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_R8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_R8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_V8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_V8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_R9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_R9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_V9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_V9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_R10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_R10], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FUEL_SENS_V10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrEditableItem[INDEX_OF_FUEL_SENS_V10], 10, 20);

    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_DIG_L_SENSOR] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_L_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_DIG_IN_L_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_L_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_DIG_IN_L_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_L_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_DIG_IN_L_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_L_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_DIG_IN_L_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_L_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_DIG_IN_L_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_CKT_FAULT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_CKT_FAULT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_R1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_V1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_R2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_V2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_R3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_V3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_R4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_V4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_R5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_V5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_R6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_V6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_R7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_V7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_R8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_V8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_R9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_V9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_R10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_R10], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_CLNT_SENS_V10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrEditableItem[INDEX_OF_ENG_CLNT_SENS_V10], 10, 20);

    ArrEditableItemScreen[INDEX_OF_AUX_S1_DIG_M_SENSOR] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_M_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_DIG_IN_M_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_M_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_DIG_IN_M_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_M_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_DIG_IN_M_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_M_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_DIG_IN_M_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_M_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_DIG_IN_M_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_THRESHOLD_TYPE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_THRESHOLD_TYPE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SHUTDOWN_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SHUTDOWN_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SHUTDOWN_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SHUTDOWN_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_WARNING_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_WARNING_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_WARNING_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_WARNING_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_CKT_FAULT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_CKT_FAULT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_SHELT_TEMP_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_SHELT_TEMP_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_SHELT_TEMP_HYSTERISIS] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_SHELT_TEMP_HYSTERISIS], 10, 20);
    ArrEditableItemScreen[INDEX_OF_SHELT_TEMP_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_SHELT_TEMP_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_SHELT_TEMP_RUN_DURATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_SHELT_TEMP_RUN_DURATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_R1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_R1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_V1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_V1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_R2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_R2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_V2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_V2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_R3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_R3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_V3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_V3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_R4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_R4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_V4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_V4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_R5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_R5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_V5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_V5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_R6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_R6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_V6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_V6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_R7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_R7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_V7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_V7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_R8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_R8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_V8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_V8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_R9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_R9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_V9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_V9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_R10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_R10], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S1_SENS_V10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrEditableItem[INDEX_OF_AUX_S1_SENS_V10], 10, 20);

    ArrEditableItemScreen[INDEX_OF_AUX_S2_DIG_N_SENSOR] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_DIG_N_SENSOR], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_N_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_DIG_IN_N_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_N_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_DIG_IN_N_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_N_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_DIG_IN_N_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_N_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_DIG_IN_N_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_N_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_DIG_IN_N_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_THRESHOLD_TYPE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_THRESHOLD_TYPE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SHUTDOWN_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SHUTDOWN_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SHUTDOWN_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SHUTDOWN_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_WARNING_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_WARNING_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_WARNING_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_WARNING_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_CKT_FAULT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_CKT_FAULT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_R1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_R1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_V1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_V1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_R2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_R2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_V2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_V2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_R3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_R3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_V3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_V3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_R4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_R4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_V4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_V4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_R5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_R5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_V5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_V5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_R6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_R6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_V6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_V6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_R7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_R7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_V7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_V7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_R8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_R8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_V8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_V8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_R9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_R9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_V9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_V9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_R10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_R10], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S2_SENS_V10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrEditableItem[INDEX_OF_AUX_S2_SENS_V10], 10, 20);

    ArrEditableItemScreen[INDEX_OF_AUX_S3_DIG_O_SENSOR] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_O_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_DIG_IN_O_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_O_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_DIG_IN_O_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_O_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_DIG_IN_O_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_O_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_DIG_IN_O_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_O_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_DIG_IN_O_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_THRESHOLD_TYPE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_THRESHOLD_TYPE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SHUTDOWN_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_WARNING_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_WARNING_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_WARNING_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_CKT_FAULT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_CKT_FAULT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_I1_V1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_I1_V1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_VAL1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_I2_V2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_I2_V2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_VAL2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_I3_V3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_I3_V3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_VAL3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_I4_V4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_I4_V4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_VAL4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_I5_V5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_I5_V5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_VAL5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_I6_V6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_I6_V6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_VAL6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_I7_V7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_I7_V7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_VAL7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_I8_V8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_I8_V8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_VAL8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_I9_V9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_I9_V9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_VAL9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_I10_V10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_I10_V10], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S3_SENS_VAL10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL10], 10, 20);

    ArrEditableItemScreen[INDEX_OF_AUX_S4_DIG_P_SENSOR] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_P_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_DIG_IN_P_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_P_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_DIG_IN_P_POLARITY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_P_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_DIG_IN_P_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_P_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_DIG_IN_P_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_IN_P_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_DIG_IN_P_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_THRESHOLD_TYPE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_THRESHOLD_TYPE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SHUTDOWN_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SHUTDOWN_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SHUTDOWN_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SHUTDOWN_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_WARNING_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_WARNING_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_WARNING_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_WARNING_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_CKT_FAULT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_CKT_FAULT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_I1_V1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_I1_V1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_VAL1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_I2_V2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_I2_V2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_VAL2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL2], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_I3_V3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_I3_V3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_VAL3] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL3], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_I4_V4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_I4_V4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_VAL4] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL4], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_I5_V5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_I5_V5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_VAL5] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL5], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_I6_V6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_I6_V6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_VAL6] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL6], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_I7_V7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_I7_V7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_VAL7] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL7], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_I8_V8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_I8_V8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_VAL8] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL8], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_I9_V9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_I9_V9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_VAL9] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL9], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_I10_V10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_I10_V10], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUX_S4_SENS_VAL10] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL10], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_OUT_A_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_A], &ArrEditableItem[INDEX_OF_DIG_OUT_A_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_OUT_A_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_A], &ArrEditableItem[INDEX_OF_DIG_OUT_A_POLARITY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_OUT_B_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_B], &ArrEditableItem[INDEX_OF_DIG_OUT_B_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_OUT_B_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_B], &ArrEditableItem[INDEX_OF_DIG_OUT_B_POLARITY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_OUT_C_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_C], &ArrEditableItem[INDEX_OF_DIG_OUT_C_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_OUT_C_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_C], &ArrEditableItem[INDEX_OF_DIG_OUT_C_POLARITY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_OUT_D_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_D], &ArrEditableItem[INDEX_OF_DIG_OUT_D_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_OUT_D_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_D], &ArrEditableItem[INDEX_OF_DIG_OUT_D_POLARITY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_OUT_E_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_E], &ArrEditableItem[INDEX_OF_DIG_OUT_E_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_OUT_E_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_E], &ArrEditableItem[INDEX_OF_DIG_OUT_E_POLARITY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_OUT_F_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_F], &ArrEditableItem[INDEX_OF_DIG_OUT_F_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_OUT_F_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_F], &ArrEditableItem[INDEX_OF_DIG_OUT_F_POLARITY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DIG_OUT_G_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_G], &ArrEditableItem[INDEX_OF_DIG_OUT_G_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DIG_OUT_G_POLARITY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_OUT_G], &ArrEditableItem[INDEX_OF_DIG_OUT_G_POLARITY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_TMR_OF_CRANKING] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANKING_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_CRANKING], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_CRANK_REST] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANKING_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_CRANK_REST], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_MANUAL_START] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANKING_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_MANUAL_START], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_AUTO_START] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANKING_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_AUTO_START], 10, 20);

    ArrEditableItemScreen[INDEX_OF_TMR_OF_SAFETY_MON] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_START_STOP_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_SAFETY_MON], 10, 20);
   // ArrEditableItemScreen[INDEX_OF_TMR_OF_MAINS_DETECT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_START_STOP_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_MAINS_DETECT], 10, 20);
   // ArrEditableItemScreen[INDEX_OF_TMR_OF_ALT_DETECT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_START_STOP_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_ALT_DETECT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_WARM_UP] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_START_STOP_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_WARM_UP], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_RETURN_TO_MAINS] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_START_STOP_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_RETURN_TO_MAINS], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_COOLING] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_START_STOP_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_COOLING], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_STOP_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_START_STOP_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_STOP_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_ADDITIONAL_STOP] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_START_STOP_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_ADDITIONAL_STOP], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_TRANSFER] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_START_STOP_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_TRANSFER], 10, 20);

    ArrEditableItemScreen[INDEX_OF_TMR_OF_POWER_SAVE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_POWER_SAVE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_PAGE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_PAGE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_DEEP_SLEEP] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_DEEP_SLEEP], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_AUDIBLE_ALARM] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_AUDIBLE_ALARM], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_TEST_MODE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_TEST_MODE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_AUTO_EXIT_TIMER] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_AUTO_EXIT_TIMER], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_ISV_PULL_SIGNAL_TIMER] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_ISV_PULL_SIGNAL_TIMER], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_GEN_BREAKER] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_GEN_BREAKER], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_MAINS_BREAKER] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_MAINS_BREAKER], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_BREAKER_FEEDBACK] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_BREAKER_FEEDBACK], 10, 20);
    ArrEditableItemScreen[INDEX_OF_TMR_OF_BREAKER_CLOSE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrEditableItem[INDEX_OF_TMR_OF_BREAKER_CLOSE], 10, 20);

    ArrEditableItemScreen[INDEX_OF_ALT_PRESENT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_ALT_PRESENT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ALT_POLES] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_ALT_POLES], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ALT_SYS] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_ALT_SYS], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ALT_MIN_HEALTHY_VOLT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_ALT_MIN_HEALTHY_VOLT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ALT_MIN_HEALTHY_FREQ] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_ALT_MIN_HEALTHY_FREQ], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DG_PHASE_ROT_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_DG_PHASE_ROT_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DG_PHASE_ROT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_DG_PHASE_ROT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AUTO_LOAD_TRANSFER] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_AUTO_LOAD_TRANSFER], 10, 20);
    ArrEditableItemScreen[INDEX_OF_WAVEFORM_DETECT_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_WAVEFORM_DETECT_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_GEN_PT_ENABLE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_GEN_PT_ENABLE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_GEN_PT_PRIMARY_RATIO] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_GEN_PT_PRIMARY_RATIO], 10, 20);
    ArrEditableItemScreen[INDEX_OF_GEN_PT_SECONDARY_RATIO] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrEditableItem[INDEX_OF_GEN_PT_SECONDARY_RATIO], 10, 20);


    ArrEditableItemScreen[INDEX_OF_UV_SHUT_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_UV_SHUT_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_UV_SHUT_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_UV_SHUT_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_UV_SHUT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_UV_SHUT_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_UV_WARN_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_UV_WARN_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_UV_WARN_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_UV_WARN_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_UV_WARN_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_UV_WARN_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OV_SHUT_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_OV_SHUT_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OV_SHUT_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_OV_SHUT_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OV_SHUT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_OV_SHUT_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OV_WARN_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_OV_WARN_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OV_WARN_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_OV_WARN_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OV_WARN_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrEditableItem[INDEX_OF_OV_WARN_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_UF_SHUT_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_UF_SHUT_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_UF_SHUT_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_UF_SHUT_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_UF_SHUT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_UF_SHUT_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_UF_WARN_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_UF_WARN_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_UF_WARN_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_UF_WARN_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_UF_WARN_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_UF_WARN_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OF_SHUT_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_OF_SHUT_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OF_SHUT_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_OF_SHUT_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OF_SHUT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_OF_SHUT_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OF_WARN_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_OF_WARN_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OF_WARN_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_OF_WARN_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OF_WARN_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrEditableItem[INDEX_OF_OF_WARN_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_CM_CT_RATIO] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CURRENT_MONITOR], &ArrEditableItem[INDEX_OF_CM_CT_RATIO], 10, 20);
    ArrEditableItemScreen[INDEX_OF_CM_OC_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CURRENT_MONITOR], &ArrEditableItem[INDEX_OF_CM_OC_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_CM_OC_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CURRENT_MONITOR], &ArrEditableItem[INDEX_OF_CM_OC_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_CM_OC_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CURRENT_MONITOR], &ArrEditableItem[INDEX_OF_CM_OC_DELAY], 10, 20);
   // ArrEditableItemScreen[INDEX_OF_CURR_MON_RESERVED] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CURRENT_MONITOR], &ArrEditableItem[INDEX_OF_CURR_MON_RESERVED], 10, 20);
    ArrEditableItemScreen[INDEX_OF_CT_LOCATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CURRENT_MONITOR], &ArrEditableItem[INDEX_OF_CT_LOCATION], 10, 20);

    ArrEditableItemScreen[INDEX_OF_EFM_CURRENT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_EARTH_FAN_MONITOR], &ArrEditableItem[INDEX_OF_EFM_CURRENT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EFM_CT_RATIO] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_EARTH_FAN_MONITOR], &ArrEditableItem[INDEX_OF_EFM_CT_RATIO], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EFM_HIGH_CURR_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_EARTH_FAN_MONITOR], &ArrEditableItem[INDEX_OF_EFM_HIGH_CURR_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EFM_HIGH_CURR_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_EARTH_FAN_MONITOR], &ArrEditableItem[INDEX_OF_EFM_HIGH_CURR_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EFM_CURR_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_EARTH_FAN_MONITOR], &ArrEditableItem[INDEX_OF_EFM_CURR_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EFM_LOW_CURR_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_EARTH_FAN_MONITOR], &ArrEditableItem[INDEX_OF_EFM_LOW_CURR_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EFM_LOW_CURR_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_EARTH_FAN_MONITOR], &ArrEditableItem[INDEX_OF_EFM_LOW_CURR_THRESH], 10, 20);

    ArrEditableItemScreen[INDEX_OF_LM_RATING] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LM_RATING], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LM_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LM_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LM_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LM_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LM_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LM_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LM_UNBALANCE_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LM_UNBALANCE_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LM_UNBALANCE_TH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LM_UNBALANCE_TH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LM_UNBALANCE_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LM_UNBALANCE_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_LOW_LOAD_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LOW_LOAD_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOW_LOAD_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LOW_LOAD_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOW_LOAD_TRIP] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LOW_LOAD_TRIP], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOW_LOAD_RETURN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LOW_LOAD_RETURN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOW_LOAD_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrEditableItem[INDEX_OF_LOW_LOAD_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_MAINS_MON_ON] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_CONFIG], &ArrEditableItem[INDEX_OF_MAINS_MON_ON], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_SYSTEM] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_CONFIG], &ArrEditableItem[INDEX_OF_MAINS_SYSTEM], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_PHASE_ROT_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_CONFIG], &ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_PHASE_ROT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_CONFIG], &ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_CONFIG], &ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_PT_ENABLE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_CONFIG], &ArrEditableItem[INDEX_OF_MAINS_PT_ENABLE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_PT_PRIMARY_RATIO] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_CONFIG], &ArrEditableItem[INDEX_OF_MAINS_PT_PRIMARY_RATIO], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_PT_SECONDARY_RATIO] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_CONFIG], &ArrEditableItem[INDEX_OF_MAINS_PT_SECONDARY_RATIO], 10, 20);

    ArrEditableItemScreen[INDEX_OF_MAINS_UV_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_VOLT_MON], &ArrEditableItem[INDEX_OF_MAINS_UV_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_UV_TRIP_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_VOLT_MON], &ArrEditableItem[INDEX_OF_MAINS_UV_TRIP_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_UV_RETURN_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_VOLT_MON], &ArrEditableItem[INDEX_OF_MAINS_UV_RETURN_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_OV_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_VOLT_MON], &ArrEditableItem[INDEX_OF_MAINS_OV_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_OV_TRIP_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_VOLT_MON], &ArrEditableItem[INDEX_OF_MAINS_OV_TRIP_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_OV_RETURN_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_VOLT_MON], &ArrEditableItem[INDEX_OF_MAINS_OV_RETURN_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_VOLT_TRIP_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_VOLT_MON], &ArrEditableItem[INDEX_OF_MAINS_VOLT_TRIP_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_MAINS_UF_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_FREQ_MON], &ArrEditableItem[INDEX_OF_MAINS_UF_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_UF_TRIP_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_FREQ_MON], &ArrEditableItem[INDEX_OF_MAINS_UF_TRIP_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_UF_RETURN_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_FREQ_MON], &ArrEditableItem[INDEX_OF_MAINS_UF_RETURN_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_OF_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_FREQ_MON], &ArrEditableItem[INDEX_OF_MAINS_OF_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_OF_TRIP_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_FREQ_MON], &ArrEditableItem[INDEX_OF_MAINS_OF_TRIP_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_OF_RETURN_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_FREQ_MON], &ArrEditableItem[INDEX_OF_MAINS_OF_RETURN_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MAINS_FREQ_TRIP_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_FREQ_MON], &ArrEditableItem[INDEX_OF_MAINS_FREQ_TRIP_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_START_ATTEMPTS] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrEditableItem[INDEX_OF_START_ATTEMPTS], 10, 20);
    ArrEditableItemScreen[INDEX_OF_START_USE_LOP] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrEditableItem[INDEX_OF_START_USE_LOP], 10, 20);
    ArrEditableItemScreen[INDEX_OF_START_LOP_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrEditableItem[INDEX_OF_START_LOP_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_START_LLOP_AT_CRANK] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrEditableItem[INDEX_OF_START_LLOP_AT_CRANK], 10, 20);
    ArrEditableItemScreen[INDEX_OF_START_LOP_AT_CRANK] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrEditableItem[INDEX_OF_START_LOP_AT_CRANK], 10, 20);
    ArrEditableItemScreen[INDEX_OF_START_USE_LLOP] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrEditableItem[INDEX_OF_START_USE_LLOP], 10, 20);
    ArrEditableItemScreen[INDEX_OF_START_LLOP_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrEditableItem[INDEX_OF_START_LLOP_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_START_DISCONN_FREQ] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrEditableItem[INDEX_OF_START_DISCONN_FREQ], 10, 20);
    ArrEditableItemScreen[INDEX_OF_START_DISCONN_RPM] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrEditableItem[INDEX_OF_START_DISCONN_RPM], 10, 20);
    ArrEditableItemScreen[INDEX_OF_START_USE_CA_VOLT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrEditableItem[INDEX_OF_START_USE_CA_VOLT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_START_CA_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrEditableItem[INDEX_OF_START_CA_THRESHOLD], 10, 20);

    ArrEditableItemScreen[INDEX_OF_SPEED_SENSE_SOURCE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MPU_TEETH_W_PT_FREQ] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_MPU_TEETH_W_PT_FREQ], 10, 20);
    ArrEditableItemScreen[INDEX_OF_US_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_US_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_US_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_US_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_US_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_US_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OS_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_OS_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_OS_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_OS_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_GROSS_OS_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_GROSS_OS_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_IDLE_RATED_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_IDLE_RATED_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_STARTUP_IDLE_TIME] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_STARTUP_IDLE_TIME], 10, 20);
    ArrEditableItemScreen[INDEX_OF_STOPPING_IDLE_TIME] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_STOPPING_IDLE_TIME], 10, 20);
    ArrEditableItemScreen[INDEX_OF_IDLE_MODE_PULSE_TIME] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_IDLE_MODE_PULSE_TIME], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_INIT_LOW_SPEED] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrEditableItem[INDEX_OF_EGOV_INIT_LOW_SPEED], 10, 20);

    ArrEditableItemScreen[INDEX_OF_LOW_VBAT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_BATTERY_MONITOR], &ArrEditableItem[INDEX_OF_LOW_VBAT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOW_VBAT_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_BATTERY_MONITOR], &ArrEditableItem[INDEX_OF_LOW_VBAT_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOW_VBAT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_BATTERY_MONITOR], &ArrEditableItem[INDEX_OF_LOW_VBAT_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_HIGH_VBAT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_BATTERY_MONITOR], &ArrEditableItem[INDEX_OF_HIGH_VBAT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_HIGH_VBAT_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_BATTERY_MONITOR], &ArrEditableItem[INDEX_OF_HIGH_VBAT_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_HIGH_VBAT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_BATTERY_MONITOR], &ArrEditableItem[INDEX_OF_HIGH_VBAT_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_CA_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CHARGE_ALT_MON], &ArrEditableItem[INDEX_OF_CA_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_CA_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CHARGE_ALT_MON], &ArrEditableItem[INDEX_OF_CA_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_CA_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CHARGE_ALT_MON], &ArrEditableItem[INDEX_OF_CA_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_PH_TIMER] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_PREHEAT], &ArrEditableItem[INDEX_OF_PH_TIMER], 10, 20);
    ArrEditableItemScreen[INDEX_OF_PH_TEMP_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_PREHEAT], &ArrEditableItem[INDEX_OF_PH_TEMP_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_PH_TEMP_VALUE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_PREHEAT], &ArrEditableItem[INDEX_OF_PH_TEMP_VALUE], 10, 20);

    ArrEditableItemScreen[INDEX_OF_CLNT_TEMP_CTRL_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CLNT_TEMP_CONTROL], &ArrEditableItem[INDEX_OF_CLNT_TEMP_CTRL_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_CLNT_TEMP_CTRL] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CLNT_TEMP_CONTROL], &ArrEditableItem[INDEX_OF_CLNT_TEMP_CTRL], 10, 20);
    ArrEditableItemScreen[INDEX_OF_CLNT_TEMP_ON_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CLNT_TEMP_CONTROL], &ArrEditableItem[INDEX_OF_CLNT_TEMP_ON_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_CLNT_TEMP_OFF_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_CLNT_TEMP_CONTROL], &ArrEditableItem[INDEX_OF_CLNT_TEMP_OFF_THRESH], 10, 20);

    ArrEditableItemScreen[INDEX_OF_AFT_ACTIVATION_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AFT], &ArrEditableItem[INDEX_OF_AFT_ACTIVATION_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AFT_DEACTIVATION_THRESH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AFT], &ArrEditableItem[INDEX_OF_AFT_DEACTIVATION_THRESH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AFT_TIMEOUT_AFTER_ACTIVATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AFT], &ArrEditableItem[INDEX_OF_AFT_TIMEOUT_AFTER_ACTIVATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AFT_MONITOR_ON_ENGINE_ON] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_AFT], &ArrEditableItem[INDEX_OF_AFT_MONITOR_ON_ENGINE_ON], 10, 20);

    ArrEditableItemScreen[INDEX_OF_ENGINE_TYPE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_ENGINE_TYPE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_FROM_ECU] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_LOP_FROM_ECU], 10, 20);
    ArrEditableItemScreen[INDEX_OF_COOLANT_TEMP_FROM_ECU] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_SPEED_FROM_ECU] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_ENG_SPEED_FROM_ECU], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_RUN_HOURS_FROM_ECU] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_ENG_RUN_HOURS_FROM_ECU], 10, 20);
    ArrEditableItemScreen[INDEX_OF_BATT_VTG_FROM_ECU] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_BATT_VTG_FROM_ECU], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_SPEED_TO_ECU] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_ENG_SPEED_TO_ECU], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENG_REQ_SPEED] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_ENG_REQ_SPEED], 10, 20);
    ArrEditableItemScreen[INDEX_OF_START_STOP_ENG_CMD_ECU] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_START_STOP_ENG_CMD_ECU], 10, 20);

    ArrEditableItemScreen[INDEX_OF_PREHEAT_CMD_ECU] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_PREHEAT_CMD_ECU], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENGINE_FRQ_ECU] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_ENGINE_FRQ_ECU], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ENGINE_GAIN_ECU] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_ENGINE_GAIN_ECU], 10, 20);
    ArrEditableItemScreen[INDEX_OF_SGC_SOURCE_ADDRESS] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ECU_SOURCE_ADDRESS] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_ECU_SOURCE_ADDRESS], 10, 20);
    ArrEditableItemScreen[INDEX_OF_COMM_FAIL_ALARM_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_COMM_FAIL_ALARM_ACT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_COMM_FAIL_ALARM_ACT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACT_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AMBER_LAMP_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_AMBER_LAMP_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AMBER_LAMP_ACT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_AMBER_LAMP_ACT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RED_LAMP_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_RED_LAMP_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RED_LAMP_ACT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_RED_LAMP_ACT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RED_LAMP_ACT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_RED_LAMP_ACT_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MALFUNCTION_LAMP_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MALFUNCTION_LAMP_ACT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_MALFUNCTION_LAMP_ACT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT_DELAY], 10, 20);
    ArrEditableItemScreen[INDEX_OF_PROTECT_LAMP_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_PROTECT_LAMP_ACT] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT], 10, 20);
    ArrEditableItemScreen[INDEX_OF_PROTECT_LAMP_ACT_DELAY] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT_DELAY], 10, 20);

    ArrEditableItemScreen[INDEX_OF_LOP_SHUTDOWN_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP], &ArrEditableItem[INDEX_OF_LOP_SHUTDOWN_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_SHUTDOWN_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP], &ArrEditableItem[INDEX_OF_LOP_SHUTDOWN_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_WARNING_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP], &ArrEditableItem[INDEX_OF_LOP_WARNING_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_LOP_WARNING_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_LOP], &ArrEditableItem[INDEX_OF_LOP_WARNING_THRESHOLD], 10, 20);

    ArrEditableItemScreen[INDEX_OF_COOLANT_TEMP_THRESH_TYPE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_TEMP], &ArrEditableItem[INDEX_OF_COOLANT_TEMP_THRESH_TYPE], 10, 20);

    ArrEditableItemScreen[INDEX_OF_ECT_SHUTDOWN_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_TEMP], &ArrEditableItem[INDEX_OF_ECT_SHUTDOWN_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ECT_SHUTDOWN_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_TEMP], &ArrEditableItem[INDEX_OF_ECT_SHUTDOWN_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ECT_WARNING_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_TEMP], &ArrEditableItem[INDEX_OF_ECT_WARNING_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ECT_WARNING_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_TEMP], &ArrEditableItem[INDEX_OF_ECT_WARNING_THRESHOLD], 10, 20);

    ArrEditableItemScreen[INDEX_OF_FILT_MAINT_ACTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINT_ALARM], &ArrEditableItem[INDEX_OF_FILT_MAINT_ACTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FILT_MAINT_THRESHOLD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINT_ALARM], &ArrEditableItem[INDEX_OF_FILT_MAINT_THRESHOLD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FILT_MAINT_ASH_LOAD_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_MAINT_ALARM], &ArrEditableItem[INDEX_OF_FILT_MAINT_ASH_LOAD_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_FILT_MAINT_DUE_DATE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ALARM_DUE_DATE], &ArrEditableItem[INDEX_OF_FILT_MAINT_DUE_DATE], 10, 20);

    ArrEditableItemScreen[INDEX_OF_EGOV_ACT_APPLICATION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_EGOV_GENERAL], &ArrEditableItem[INDEX_OF_EGOV_ACT_APPLICATION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_ACT_SPEED] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_EGOV_GENERAL], &ArrEditableItem[INDEX_OF_EGOV_ACT_SPEED], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_DIR] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_EGOV_GENERAL], &ArrEditableItem[INDEX_OF_EGOV_DIR], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_CRANK_BOOST] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_START_STRGY], &ArrEditableItem[INDEX_OF_EGOV_CRANK_BOOST], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_RPM_PID_ON] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_START_STRGY], &ArrEditableItem[INDEX_OF_EGOV_RPM_PID_ON], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_RAMP_UP_TIME] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_START_STRGY], &ArrEditableItem[INDEX_OF_EGOV_RAMP_UP_TIME], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_PID_ON_TIME] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_START_STRGY], &ArrEditableItem[INDEX_OF_EGOV_PID_ON_TIME], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_SET_POINT_SELECTION] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], &ArrEditableItem[INDEX_OF_EGOV_SET_POINT_SELECTION], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_PERCENT_DROOP] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], &ArrEditableItem[INDEX_OF_EGOV_PERCENT_DROOP], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_GEN_SET_SPEED] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], &ArrEditableItem[INDEX_OF_EGOV_GEN_SET_SPEED], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_GEN_KP] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], &ArrEditableItem[INDEX_OF_EGOV_GEN_KP], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_GEN_KI] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], &ArrEditableItem[INDEX_OF_EGOV_GEN_KI], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_GEN_KD] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], &ArrEditableItem[INDEX_OF_EGOV_GEN_KD], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_GEN_DITHER] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], &ArrEditableItem[INDEX_OF_EGOV_GEN_DITHER], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_GEN_GAIN_SCHEDULE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], &ArrEditableItem[INDEX_OF_EGOV_GEN_GAIN_SCHEDULE], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_GEN_LOADING_FAC] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], &ArrEditableItem[INDEX_OF_EGOV_GEN_LOADING_FAC], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_GEN_UNLOADING_FAC] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], &ArrEditableItem[INDEX_OF_EGOV_GEN_UNLOADING_FAC], 10, 20);
    ArrEditableItemScreen[INDEX_OF_EGOV_RUNNING_STEPS] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_STR_STP_CNFG], &ArrEditableItem[INDEX_OF_EGOV_RUNNING_STEPS], 10, 20);

    ArrEditableItemScreen[INDEX_OF_ENG_SR_NO] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_ENG_SR_NO], &ArrEditableItem[INDEX_OF_ENG_SR_NO], 10, 20);
    ArrEditableItemScreen[INDEX_OF_PIN_1] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_PASSWORD_1], &ArrEditableItem[INDEX_OF_PIN_1], 10, 20);
    ArrEditableItemScreen[INDEX_OF_PIN_2] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_PASSWORD_2], &ArrEditableItem[INDEX_OF_PIN_2], 10, 20);

    ArrEditableItemScreen[INDEX_OF_RESET_ENG_RUN_HRS] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RESET_GENSET], &ArrEditableItem[INDEX_OF_RESET_ENG_RUN_HRS], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RESET_NO_OF_STARTS] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RESET_GENSET], &ArrEditableItem[INDEX_OF_RESET_NO_OF_STARTS], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RESET_NO_OF_TRIPS] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RESET_GENSET], &ArrEditableItem[INDEX_OF_RESET_NO_OF_TRIPS], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RESET_GEN_KWH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RESET_GENSET], &ArrEditableItem[INDEX_OF_RESET_GEN_KWH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RESET_GEN_KVAH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RESET_GENSET], &ArrEditableItem[INDEX_OF_RESET_GEN_KVAH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RESET_GEN_KVARH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RESET_GENSET], &ArrEditableItem[INDEX_OF_RESET_GEN_KVARH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RESET_MAINS_RUN_HRS] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RESET_MAINS], &ArrEditableItem[INDEX_OF_RESET_MAINS_RUN_HRS], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RESET_MAINS_KWH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RESET_MAINS], &ArrEditableItem[INDEX_OF_RESET_MAINS_KWH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RESET_MAINS_KVAH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RESET_MAINS], &ArrEditableItem[INDEX_OF_RESET_MAINS_KVAH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_RESET_MAINS_KVARH] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_RESET_MAINS], &ArrEditableItem[INDEX_OF_RESET_MAINS_KVARH], 10, 20);
    ArrEditableItemScreen[INDEX_OF_ACTIVE_PROFILE] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_SELECT_PROFILE], &ArrEditableItem[INDEX_OF_ACTIVE_PROFILE], 10, 20);

    ArrEditableItemScreen[INDEX_OF_DISP_VOLT_FILT_EN] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DISP_VOLT_FILT], &ArrEditableItem[INDEX_OF_DISP_VOLT_FILT_EN], 10, 20);
    ArrEditableItemScreen[INDEX_OF_DISP_VOLT_FILT_CONST] = CEditableItemsScreen(strSubMenu[_u8LanguageArrayIndex][ID_DISP_VOLT_FILT], &ArrEditableItem[INDEX_OF_DISP_VOLT_FILT_CONST], 10, 20);

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

        for(int i=INDEX_OF_POWER_ON_MODE; i<=INDEX_OF_EGOV_RUNNING_STEPS;i++)
        {
            menuItemsLowestLevel[i] = CMenuItem( &ArrEditableItemScreen[i]);
        }

        menuItemsLowestLevel[INDEX_OF_ENG_SR_NO] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_ENG_SR_NO], &ArrEditableItemScreen[INDEX_OF_ENG_SR_NO]);
        menuItemsLowestLevel[INDEX_OF_PIN_1] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_PASSWORD_1], &ArrEditableItemScreen[INDEX_OF_PIN_1]);
        menuItemsLowestLevel[INDEX_OF_PIN_2] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_PASSWORD_2], &ArrEditableItemScreen[INDEX_OF_PIN_2]);

        menuItemsLowestLevel[INDEX_OF_RESET_ENG_RUN_HRS] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_RESET_ENG_RUN_HRS]);
        menuItemsLowestLevel[INDEX_OF_RESET_NO_OF_STARTS] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_RESET_NO_OF_STARTS]);
        menuItemsLowestLevel[INDEX_OF_RESET_NO_OF_TRIPS] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_RESET_NO_OF_TRIPS]);

        menuItemsLowestLevel[INDEX_OF_RESET_GEN_KWH] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_RESET_GEN_KWH]);
        menuItemsLowestLevel[INDEX_OF_RESET_GEN_KVAH] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_RESET_GEN_KVAH]);
        menuItemsLowestLevel[INDEX_OF_RESET_GEN_KVARH] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_RESET_GEN_KVARH]);
        menuItemsLowestLevel[INDEX_OF_RESET_MAINS_RUN_HRS] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_RESET_MAINS_RUN_HRS]);
        menuItemsLowestLevel[INDEX_OF_RESET_MAINS_KWH] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_RESET_MAINS_KWH]);
        menuItemsLowestLevel[INDEX_OF_RESET_MAINS_KVAH] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_RESET_MAINS_KVAH]);
        menuItemsLowestLevel[INDEX_OF_RESET_MAINS_KVARH] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_RESET_MAINS_KVARH]);

        menuItemsLowestLevel[INDEX_OF_ACTIVE_PROFILE] = CMenuItem(strSubMenu[_u8LanguageArrayIndex][ID_SELECT_PROFILE], &ArrEditableItemScreen[INDEX_OF_ACTIVE_PROFILE]);

        menuItemsLowestLevel[INDEX_OF_DISP_VOLT_FILT_EN] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_DISP_VOLT_FILT_EN]);
        menuItemsLowestLevel[INDEX_OF_DISP_VOLT_FILT_CONST] = CMenuItem( &ArrEditableItemScreen[INDEX_OF_DISP_VOLT_FILT_CONST]);

    //Now create sub-menus and then initialize mid level menu items:

        ArrSubMenu[ID_GENERAL] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL], INDEX_OF_CONTRAST-INDEX_OF_POWER_ON_MODE , &(menuItemsLowestLevel[INDEX_OF_POWER_ON_MODE]));
        ArrSubMenu[ID_DISPLAY] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_DISPLAY], INDEX_OF_COMM_MODE-INDEX_OF_CONTRAST, &(menuItemsLowestLevel[INDEX_OF_CONTRAST]));
        ArrSubMenu[ID_RS485_COMM] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_RS485_COMM], INDEX_OF_SITE_BATTERY_MON-INDEX_OF_COMM_MODE, &(menuItemsLowestLevel[INDEX_OF_COMM_MODE]));
        ArrSubMenu[ID_SITE_BAT_CONFIG] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_SITE_BAT_CONFIG], INDEX_OF_CYCLIC_MODE-INDEX_OF_SITE_BATTERY_MON, &(menuItemsLowestLevel[INDEX_OF_SITE_BATTERY_MON]));
        ArrSubMenu[ID_CYCLIC_CONFIG] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_CYCLIC_CONFIG], INDEX_OF_EXERCISER_EVENT_1-INDEX_OF_CYCLIC_MODE, &(menuItemsLowestLevel[INDEX_OF_CYCLIC_MODE]));
        ArrSubMenu[ID_AUTO_EXERCISE1] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE1], INDEX_OF_EXERCISER_EVENT_2-INDEX_OF_EXERCISER_EVENT_1, &(menuItemsLowestLevel[INDEX_OF_EXERCISER_EVENT_1]));
        ArrSubMenu[ID_AUTO_EXERCISE2] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE2], INDEX_OF_NIGHT_MODE_RESTRICT-INDEX_OF_EXERCISER_EVENT_2, &(menuItemsLowestLevel[INDEX_OF_EXERCISER_EVENT_2]));
        ArrSubMenu[ID_NIGHT_MODE] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_NIGHT_MODE], INDEX_OF_DIG_IN_A_SOURCE-INDEX_OF_NIGHT_MODE_RESTRICT, &(menuItemsLowestLevel[INDEX_OF_NIGHT_MODE_RESTRICT]));

        ArrSubMenu[ID_DIG_IN_A] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_A], INDEX_OF_DIG_IN_B_SOURCE-INDEX_OF_DIG_IN_A_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_IN_A_SOURCE]));
        ArrSubMenu[ID_DIG_IN_B] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_B], INDEX_OF_DIG_IN_C_SOURCE-INDEX_OF_DIG_IN_B_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_IN_B_SOURCE]));
        ArrSubMenu[ID_DIG_IN_C] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_C], INDEX_OF_DIG_IN_D_SOURCE-INDEX_OF_DIG_IN_C_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_IN_C_SOURCE]));
        ArrSubMenu[ID_DIG_IN_D] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_D], INDEX_OF_DIG_IN_E_SOURCE-INDEX_OF_DIG_IN_D_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_IN_D_SOURCE]));
        ArrSubMenu[ID_DIG_IN_E] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_E], INDEX_OF_DIG_IN_F_SOURCE-INDEX_OF_DIG_IN_E_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_IN_E_SOURCE]));
        ArrSubMenu[ID_DIG_IN_F] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_F], INDEX_OF_DIG_IN_G_SOURCE-INDEX_OF_DIG_IN_F_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_IN_F_SOURCE]));
        ArrSubMenu[ID_DIG_IN_G] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_G], INDEX_OF_DIG_IN_H_SOURCE-INDEX_OF_DIG_IN_G_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_IN_G_SOURCE]));
        ArrSubMenu[ID_DIG_IN_H] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_H], INDEX_OF_DIG_IN_I_SOURCE-INDEX_OF_DIG_IN_H_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_IN_H_SOURCE]));
        ArrSubMenu[ID_DIG_IN_I] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_I], INDEX_OF_LOP_DIG_J_SENSOR-INDEX_OF_DIG_IN_I_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_IN_I_SOURCE]));
        ArrSubMenu[ID_LOP_DIJ_J] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], INDEX_OF_FUEL_DIG_K_SENSOR-INDEX_OF_LOP_DIG_J_SENSOR, &(menuItemsLowestLevel[INDEX_OF_LOP_DIG_J_SENSOR]));
        ArrSubMenu[ID_FUEL_DIG_K] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], INDEX_OF_ENG_CLNT_DIG_L_SENSOR-INDEX_OF_FUEL_DIG_K_SENSOR, &(menuItemsLowestLevel[INDEX_OF_FUEL_DIG_K_SENSOR]));
        ArrSubMenu[ID_ENG_COOL_DIG_L] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], INDEX_OF_AUX_S1_DIG_M_SENSOR-INDEX_OF_ENG_CLNT_DIG_L_SENSOR, &(menuItemsLowestLevel[INDEX_OF_ENG_CLNT_DIG_L_SENSOR]));
        ArrSubMenu[ID_AUX_S1_DIG_M] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], INDEX_OF_AUX_S2_DIG_N_SENSOR-INDEX_OF_AUX_S1_DIG_M_SENSOR, &(menuItemsLowestLevel[INDEX_OF_AUX_S1_DIG_M_SENSOR]));
        ArrSubMenu[ID_AUX_S2_DIG_N] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], INDEX_OF_AUX_S3_DIG_O_SENSOR-INDEX_OF_AUX_S2_DIG_N_SENSOR, &(menuItemsLowestLevel[INDEX_OF_AUX_S2_DIG_N_SENSOR]));
        ArrSubMenu[ID_AUX_S3_DIG_O] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], INDEX_OF_AUX_S4_DIG_P_SENSOR-INDEX_OF_AUX_S3_DIG_O_SENSOR, &(menuItemsLowestLevel[INDEX_OF_AUX_S3_DIG_O_SENSOR]));
        ArrSubMenu[ID_AUX_S4_DIG_P] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], INDEX_OF_DIG_OUT_A_SOURCE-INDEX_OF_AUX_S4_DIG_P_SENSOR, &(menuItemsLowestLevel[INDEX_OF_AUX_S4_DIG_P_SENSOR]));

        ArrSubMenu[ID_OUT_A] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_OUT_A], INDEX_OF_DIG_OUT_B_SOURCE-INDEX_OF_DIG_OUT_A_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_OUT_A_SOURCE]));
        ArrSubMenu[ID_OUT_B] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_OUT_B], INDEX_OF_DIG_OUT_C_SOURCE-INDEX_OF_DIG_OUT_B_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_OUT_B_SOURCE]));
        ArrSubMenu[ID_OUT_C] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_OUT_C], INDEX_OF_DIG_OUT_D_SOURCE-INDEX_OF_DIG_OUT_C_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_OUT_C_SOURCE]));
        ArrSubMenu[ID_OUT_D] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_OUT_D], INDEX_OF_DIG_OUT_E_SOURCE-INDEX_OF_DIG_OUT_D_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_OUT_D_SOURCE]));
        ArrSubMenu[ID_OUT_E] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_OUT_E], INDEX_OF_DIG_OUT_F_SOURCE-INDEX_OF_DIG_OUT_E_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_OUT_E_SOURCE]));
        ArrSubMenu[ID_OUT_F] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_OUT_F], INDEX_OF_DIG_OUT_G_SOURCE-INDEX_OF_DIG_OUT_F_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_OUT_F_SOURCE]));
        ArrSubMenu[ID_OUT_G] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_OUT_G], INDEX_OF_TMR_OF_CRANKING-INDEX_OF_DIG_OUT_G_SOURCE, &(menuItemsLowestLevel[INDEX_OF_DIG_OUT_G_SOURCE]));

        ArrSubMenu[ID_CRANKING_TIMER] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_CRANKING_TIMER], INDEX_OF_TMR_OF_SAFETY_MON-INDEX_OF_TMR_OF_CRANKING, &(menuItemsLowestLevel[INDEX_OF_TMR_OF_CRANKING]));
        ArrSubMenu[ID_START_STOP_TIMER] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_START_STOP_TIMER], INDEX_OF_TMR_OF_POWER_SAVE-INDEX_OF_TMR_OF_SAFETY_MON, &(menuItemsLowestLevel[INDEX_OF_TMR_OF_SAFETY_MON]));
        ArrSubMenu[ID_GENERAL_TIMER] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], INDEX_OF_ALT_PRESENT-INDEX_OF_TMR_OF_POWER_SAVE, &(menuItemsLowestLevel[INDEX_OF_TMR_OF_POWER_SAVE]));

        ArrSubMenu[ID_ALT_CONFIG] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], INDEX_OF_UV_SHUT_EN-INDEX_OF_ALT_PRESENT, &(menuItemsLowestLevel[INDEX_OF_ALT_PRESENT]));
        ArrSubMenu[ID_VOLT_MONITOR] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], INDEX_OF_UF_SHUT_EN-INDEX_OF_UV_SHUT_EN, &(menuItemsLowestLevel[INDEX_OF_UV_SHUT_EN]));
        ArrSubMenu[ID_FREQ_MONITOR] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], INDEX_OF_CM_CT_RATIO-INDEX_OF_UF_SHUT_EN, &(menuItemsLowestLevel[INDEX_OF_UF_SHUT_EN]));
        ArrSubMenu[ID_CURRENT_MONITOR] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_CURRENT_MONITOR], INDEX_OF_EFM_CURRENT-INDEX_OF_CM_CT_RATIO, &(menuItemsLowestLevel[INDEX_OF_CM_CT_RATIO]));
        ArrSubMenu[ID_EARTH_FAN_MONITOR] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_EARTH_FAN_MONITOR], INDEX_OF_LM_RATING-INDEX_OF_EFM_CURRENT, &(menuItemsLowestLevel[INDEX_OF_EFM_CURRENT]));
        ArrSubMenu[ID_LOAD_MONITOR] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], INDEX_OF_MAINS_MON_ON-INDEX_OF_LM_RATING, &(menuItemsLowestLevel[INDEX_OF_LM_RATING]));

        ArrSubMenu[ID_MAINS_CONFIG] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_CONFIG], INDEX_OF_MAINS_UV_EN-INDEX_OF_MAINS_MON_ON, &(menuItemsLowestLevel[INDEX_OF_MAINS_MON_ON]));
        ArrSubMenu[ID_MAINS_VOLT_MON] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_VOLT_MON], INDEX_OF_MAINS_UF_EN-INDEX_OF_MAINS_UV_EN, &(menuItemsLowestLevel[INDEX_OF_MAINS_UV_EN]));
        ArrSubMenu[ID_MAINS_FREQ_MON] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_MAINS_FREQ_MON], INDEX_OF_START_ATTEMPTS-INDEX_OF_MAINS_UF_EN, &(menuItemsLowestLevel[INDEX_OF_MAINS_UF_EN]));

        ArrSubMenu[ID_CRANK_DISCON] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], INDEX_OF_SPEED_SENSE_SOURCE-INDEX_OF_START_ATTEMPTS, &(menuItemsLowestLevel[INDEX_OF_START_ATTEMPTS]));
        ArrSubMenu[ID_SPEED_MONITOR] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], INDEX_OF_LOW_VBAT_ACTION-INDEX_OF_SPEED_SENSE_SOURCE, &(menuItemsLowestLevel[INDEX_OF_SPEED_SENSE_SOURCE]));
        ArrSubMenu[ID_BATTERY_MONITOR] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_BATTERY_MONITOR], INDEX_OF_CA_ACTION-INDEX_OF_LOW_VBAT_ACTION, &(menuItemsLowestLevel[INDEX_OF_LOW_VBAT_ACTION]));
        ArrSubMenu[ID_CHARGE_ALT_MON] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_CHARGE_ALT_MON],INDEX_OF_PH_TIMER-INDEX_OF_CA_ACTION, &(menuItemsLowestLevel[INDEX_OF_CA_ACTION]));
        ArrSubMenu[ID_PREHEAT] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_PREHEAT], INDEX_OF_CLNT_TEMP_CTRL_EN-INDEX_OF_PH_TIMER, &(menuItemsLowestLevel[INDEX_OF_PH_TIMER]));
        ArrSubMenu[ID_CLNT_TEMP_CONTROL] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_CLNT_TEMP_CONTROL], INDEX_OF_AFT_ACTIVATION_THRESH-INDEX_OF_CLNT_TEMP_CTRL_EN, &(menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_CTRL_EN]));
        ArrSubMenu[ID_AFT] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_AFT], INDEX_OF_ENGINE_TYPE-INDEX_OF_AFT_ACTIVATION_THRESH, &(menuItemsLowestLevel[INDEX_OF_AFT_ACTIVATION_THRESH]));
        ArrSubMenu[ID_ECU] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_ECU], INDEX_OF_LOP_SHUTDOWN_EN-INDEX_OF_ENGINE_TYPE, &(menuItemsLowestLevel[INDEX_OF_ENGINE_TYPE]));
        ArrSubMenu[ID_LOP] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_LOP], INDEX_OF_COOLANT_TEMP_THRESH_TYPE-INDEX_OF_LOP_SHUTDOWN_EN, &(menuItemsLowestLevel[INDEX_OF_LOP_SHUTDOWN_EN]));
        ArrSubMenu[ID_ENG_TEMP] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_ENG_TEMP], INDEX_OF_FILT_MAINT_ACTION-INDEX_OF_COOLANT_TEMP_THRESH_TYPE, &(menuItemsLowestLevel[INDEX_OF_COOLANT_TEMP_THRESH_TYPE]));

        ArrSubMenu[ID_MAINT_ALARM] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_MAINT_ALARM], INDEX_OF_FILT_MAINT_DUE_DATE-INDEX_OF_FILT_MAINT_ACTION, &(menuItemsLowestLevel[INDEX_OF_FILT_MAINT_ACTION]));
        ArrSubMenu[ID_ALARM_DUE_DATE] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_ALARM_DUE_DATE], INDEX_OF_EGOV_ACT_APPLICATION-INDEX_OF_FILT_MAINT_DUE_DATE, &(menuItemsLowestLevel[INDEX_OF_FILT_MAINT_DUE_DATE]));

        ArrSubMenu[ID_EGOV_GENERAL] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_EGOV_GENERAL], INDEX_OF_EGOV_CRANK_BOOST-INDEX_OF_EGOV_ACT_APPLICATION, &(menuItemsLowestLevel[INDEX_OF_EGOV_ACT_APPLICATION]));
        ArrSubMenu[ID_ENG_START_STRGY] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_ENG_START_STRGY], INDEX_OF_EGOV_SET_POINT_SELECTION-INDEX_OF_EGOV_CRANK_BOOST, &(menuItemsLowestLevel[INDEX_OF_EGOV_CRANK_BOOST]));
        ArrSubMenu[ID_GEN_EGOV_CNFG] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], INDEX_OF_EGOV_RUNNING_STEPS-INDEX_OF_EGOV_SET_POINT_SELECTION, &(menuItemsLowestLevel[INDEX_OF_EGOV_SET_POINT_SELECTION]));
        ArrSubMenu[ID_STR_STP_CNFG] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_STR_STP_CNFG], INDEX_OF_ENG_SR_NO-INDEX_OF_EGOV_RUNNING_STEPS, &(menuItemsLowestLevel[INDEX_OF_EGOV_RUNNING_STEPS]));

        ArrSubMenu[ID_ENG_SR_NO] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_ENG_SR_NO], INDEX_OF_PIN_1-INDEX_OF_ENG_SR_NO, &(menuItemsLowestLevel[INDEX_OF_ENG_SR_NO]));
        ArrSubMenu[ID_PASSWORD_1] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_PASSWORD_1], INDEX_OF_PIN_2-INDEX_OF_PIN_1, &(menuItemsLowestLevel[INDEX_OF_PIN_1]));
        ArrSubMenu[ID_PASSWORD_2] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_PASSWORD_2], INDEX_OF_RESET_ENG_RUN_HRS-INDEX_OF_PIN_2, &(menuItemsLowestLevel[INDEX_OF_PIN_2]));

        ArrSubMenu[ID_RESET_GENSET] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_RESET_GENSET], INDEX_OF_RESET_MAINS_RUN_HRS-INDEX_OF_RESET_ENG_RUN_HRS, &(menuItemsLowestLevel[INDEX_OF_RESET_ENG_RUN_HRS]));
        ArrSubMenu[ID_RESET_MAINS] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_RESET_MAINS], INDEX_OF_ACTIVE_PROFILE-INDEX_OF_RESET_MAINS_RUN_HRS, &(menuItemsLowestLevel[INDEX_OF_RESET_MAINS_RUN_HRS]));

        ArrSubMenu[ID_SELECT_PROFILE] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_SELECT_PROFILE], INDEX_OF_DISP_VOLT_FILT_EN-INDEX_OF_ACTIVE_PROFILE, &(menuItemsLowestLevel[INDEX_OF_ACTIVE_PROFILE]));

        ArrSubMenu[ID_DISP_VOLT_FILT] = CMenu(strSubMenu[_u8LanguageArrayIndex][ID_DISP_VOLT_FILT], INDEX_LAST-INDEX_OF_DISP_VOLT_FILT_EN, &(menuItemsLowestLevel[INDEX_OF_DISP_VOLT_FILT_EN]));

        // Submenu
        menuItemsMidLevel[ID_GENERAL] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_GENERAL], &ArrSubMenu[ID_GENERAL]);
        menuItemsMidLevel[ID_DISPLAY] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_DISPLAY], &ArrSubMenu[ID_DISPLAY]);
        menuItemsMidLevel[ID_RS485_COMM] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_RS485_COMM], &ArrSubMenu[ID_RS485_COMM]);
        menuItemsMidLevel[ID_SITE_BAT_CONFIG] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_SITE_BAT_CONFIG], &ArrSubMenu[ID_SITE_BAT_CONFIG]);
        menuItemsMidLevel[ID_CYCLIC_CONFIG] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_CYCLIC_CONFIG], &ArrSubMenu[ID_CYCLIC_CONFIG]);
        menuItemsMidLevel[ID_AUTO_EXERCISE1] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE1], &ArrSubMenu[ID_AUTO_EXERCISE1]);
        menuItemsMidLevel[ID_AUTO_EXERCISE2] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_AUTO_EXERCISE2], &ArrSubMenu[ID_AUTO_EXERCISE2]);
        menuItemsMidLevel[ID_NIGHT_MODE] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_NIGHT_MODE], &ArrSubMenu[ID_NIGHT_MODE]);

        menuItemsMidLevel[ID_DIG_IN_A] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_A], &ArrSubMenu[ID_DIG_IN_A]);
        menuItemsMidLevel[ID_DIG_IN_B] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_B], &ArrSubMenu[ID_DIG_IN_B]);
        menuItemsMidLevel[ID_DIG_IN_C] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_C], &ArrSubMenu[ID_DIG_IN_C]);
        menuItemsMidLevel[ID_DIG_IN_D] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_D], &ArrSubMenu[ID_DIG_IN_D]);
        menuItemsMidLevel[ID_DIG_IN_E] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_E], &ArrSubMenu[ID_DIG_IN_E]);
        menuItemsMidLevel[ID_DIG_IN_F] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_F], &ArrSubMenu[ID_DIG_IN_F]);
        menuItemsMidLevel[ID_DIG_IN_G] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_G], &ArrSubMenu[ID_DIG_IN_G]);
        menuItemsMidLevel[ID_DIG_IN_H] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_H], &ArrSubMenu[ID_DIG_IN_H]);
        menuItemsMidLevel[ID_DIG_IN_I] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_DIG_IN_I], &ArrSubMenu[ID_DIG_IN_I]);
        menuItemsMidLevel[ID_LOP_DIJ_J] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_LOP_DIJ_J], &ArrSubMenu[ID_LOP_DIJ_J]);
        menuItemsMidLevel[ID_FUEL_DIG_K] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_FUEL_DIG_K], &ArrSubMenu[ID_FUEL_DIG_K]);
        menuItemsMidLevel[ID_ENG_COOL_DIG_L] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_ENG_COOL_DIG_L], &ArrSubMenu[ID_ENG_COOL_DIG_L]);
        menuItemsMidLevel[ID_AUX_S1_DIG_M] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_AUX_S1_DIG_M], &ArrSubMenu[ID_AUX_S1_DIG_M]);
        menuItemsMidLevel[ID_AUX_S2_DIG_N] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_AUX_S2_DIG_N], &ArrSubMenu[ID_AUX_S2_DIG_N]);
        menuItemsMidLevel[ID_AUX_S3_DIG_O] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_AUX_S3_DIG_O], &ArrSubMenu[ID_AUX_S3_DIG_O]);
        menuItemsMidLevel[ID_AUX_S4_DIG_P] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_AUX_S4_DIG_P], &ArrSubMenu[ID_AUX_S4_DIG_P]);

        menuItemsMidLevel[ID_OUT_A] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_OUT_A], &ArrSubMenu[ID_OUT_A]);
        menuItemsMidLevel[ID_OUT_B] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_OUT_B], &ArrSubMenu[ID_OUT_B]);
        menuItemsMidLevel[ID_OUT_C] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_OUT_C], &ArrSubMenu[ID_OUT_C]);
        menuItemsMidLevel[ID_OUT_D] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_OUT_D], &ArrSubMenu[ID_OUT_D]);
        menuItemsMidLevel[ID_OUT_E] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_OUT_E], &ArrSubMenu[ID_OUT_E]);
        menuItemsMidLevel[ID_OUT_F] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_OUT_F], &ArrSubMenu[ID_OUT_F]);
        menuItemsMidLevel[ID_OUT_G] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_OUT_G], &ArrSubMenu[ID_OUT_G]);

        menuItemsMidLevel[ID_CRANKING_TIMER] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_CRANKING_TIMER], &ArrSubMenu[ID_CRANKING_TIMER]);
        menuItemsMidLevel[ID_START_STOP_TIMER] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_START_STOP_TIMER], &ArrSubMenu[ID_START_STOP_TIMER]);
        menuItemsMidLevel[ID_GENERAL_TIMER] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_GENERAL_TIMER], &ArrSubMenu[ID_GENERAL_TIMER]);

        menuItemsMidLevel[ID_ALT_CONFIG] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_ALT_CONFIG], &ArrSubMenu[ID_ALT_CONFIG]);
        menuItemsMidLevel[ID_VOLT_MONITOR] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_VOLT_MONITOR], &ArrSubMenu[ID_VOLT_MONITOR]);
        menuItemsMidLevel[ID_FREQ_MONITOR] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_FREQ_MONITOR], &ArrSubMenu[ID_FREQ_MONITOR]);
        menuItemsMidLevel[ID_CURRENT_MONITOR] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_CURRENT_MONITOR], &ArrSubMenu[ID_CURRENT_MONITOR]);
        menuItemsMidLevel[ID_EARTH_FAN_MONITOR] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_EARTH_FAN_MONITOR], &ArrSubMenu[ID_EARTH_FAN_MONITOR]);
        menuItemsMidLevel[ID_LOAD_MONITOR] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_LOAD_MONITOR], &ArrSubMenu[ID_LOAD_MONITOR]);

        menuItemsMidLevel[ID_MAINS_CONFIG] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_MAINS_CONFIG], &ArrSubMenu[ID_MAINS_CONFIG]);
        menuItemsMidLevel[ID_MAINS_VOLT_MON] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_MAINS_VOLT_MON], &ArrSubMenu[ID_MAINS_VOLT_MON]);
        menuItemsMidLevel[ID_MAINS_FREQ_MON] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_MAINS_FREQ_MON], &ArrSubMenu[ID_MAINS_FREQ_MON]);

        menuItemsMidLevel[ID_CRANK_DISCON] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_CRANK_DISCON], &ArrSubMenu[ID_CRANK_DISCON]);
        menuItemsMidLevel[ID_SPEED_MONITOR] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_SPEED_MONITOR], &ArrSubMenu[ID_SPEED_MONITOR]);
        menuItemsMidLevel[ID_BATTERY_MONITOR] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_BATTERY_MONITOR], &ArrSubMenu[ID_BATTERY_MONITOR]);
        menuItemsMidLevel[ID_CHARGE_ALT_MON] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_CHARGE_ALT_MON], &ArrSubMenu[ID_CHARGE_ALT_MON]);
        menuItemsMidLevel[ID_PREHEAT] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_PREHEAT], &ArrSubMenu[ID_PREHEAT]);
        menuItemsMidLevel[ID_CLNT_TEMP_CONTROL] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_CLNT_TEMP_CONTROL], &ArrSubMenu[ID_CLNT_TEMP_CONTROL]);
        menuItemsMidLevel[ID_AFT] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_AFT], &ArrSubMenu[ID_AFT]);
        menuItemsMidLevel[ID_ECU] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_ECU], &ArrSubMenu[ID_ECU]);
        menuItemsMidLevel[ID_LOP] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_LOP], &ArrSubMenu[ID_LOP]);
        menuItemsMidLevel[ID_ENG_TEMP] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_ENG_TEMP], &ArrSubMenu[ID_ENG_TEMP]);

        menuItemsMidLevel[ID_MAINT_ALARM] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_MAINT_ALARM], &ArrSubMenu[ID_MAINT_ALARM]);
        menuItemsMidLevel[ID_ALARM_DUE_DATE] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_ALARM_DUE_DATE], &ArrSubMenu[ID_ALARM_DUE_DATE]);

        menuItemsMidLevel[ID_EGOV_GENERAL] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_EGOV_GENERAL], &ArrSubMenu[ID_EGOV_GENERAL]);
        menuItemsMidLevel[ID_ENG_START_STRGY] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_ENG_START_STRGY], &ArrSubMenu[ID_ENG_START_STRGY]);
        menuItemsMidLevel[ID_GEN_EGOV_CNFG] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_GEN_EGOV_CNFG], &ArrSubMenu[ID_GEN_EGOV_CNFG]);
        menuItemsMidLevel[ID_STR_STP_CNFG] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_STR_STP_CNFG], &ArrSubMenu[ID_STR_STP_CNFG]);

        menuItemsMidLevel[ID_ENG_SR_NO] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_ENG_SR_NO], &ArrSubMenu[ID_ENG_SR_NO]);
        menuItemsMidLevel[ID_PASSWORD_1] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_PASSWORD_1], &ArrSubMenu[ID_PASSWORD_1]);
        menuItemsMidLevel[ID_PASSWORD_2] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_PASSWORD_2], &ArrSubMenu[ID_PASSWORD_2]);

        menuItemsMidLevel[ID_RESET_GENSET] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_RESET_GENSET], &ArrSubMenu[ID_RESET_GENSET]);
        menuItemsMidLevel[ID_RESET_MAINS] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_RESET_MAINS], &ArrSubMenu[ID_RESET_MAINS]);
        menuItemsMidLevel[ID_SELECT_PROFILE] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_SELECT_PROFILE], &ArrSubMenu[ID_SELECT_PROFILE]);

        menuItemsMidLevel[ID_DISP_VOLT_FILT] = CMenuItem (strSubMenu[_u8LanguageArrayIndex][ID_DISP_VOLT_FILT], &ArrSubMenu[ID_DISP_VOLT_FILT]);

        if(!CFGC::IsSGC421())
        {
            menuItemsMidLevel[ID_EGOV_GENERAL].isEnabled = false;
            menuItemsMidLevel[ID_ENG_START_STRGY].isEnabled = false;
            menuItemsMidLevel[ID_GEN_EGOV_CNFG].isEnabled = false;
            menuItemsMidLevel[ID_STR_STP_CNFG].isEnabled = false;
        }

        //MAainMenu
        ArrMenu[ID_MODULE] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_MODULE], ID_DIG_IN_A - ID_GENERAL, &(menuItemsMidLevel[ID_GENERAL]));
        menuItemsTopLevel[ID_MODULE] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_MODULE], &ArrMenu[ID_MODULE]);

        ArrMenu[ID_INPUTS] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_INPUTS], ID_OUT_A-ID_DIG_IN_A, &(menuItemsMidLevel[ID_DIG_IN_A]));
        menuItemsTopLevel[ID_INPUTS] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_INPUTS], &ArrMenu[ID_INPUTS]);

        ArrMenu[ID_OUTPUTS] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_OUTPUTS], ID_CRANKING_TIMER-ID_OUT_A, &(menuItemsMidLevel[ID_OUT_A]));
        menuItemsTopLevel[ID_OUTPUTS] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_OUTPUTS], &ArrMenu[ID_OUTPUTS]);

        ArrMenu[ID_TIMERS] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_TIMERS], ID_ALT_CONFIG-ID_CRANKING_TIMER, &(menuItemsMidLevel[ID_CRANKING_TIMER]));
        menuItemsTopLevel[ID_TIMERS] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_TIMERS], &ArrMenu[ID_TIMERS]);

        ArrMenu[ID_GENERATOR] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_GENERATOR], ID_MAINS_CONFIG-ID_ALT_CONFIG, &(menuItemsMidLevel[ID_ALT_CONFIG]));
        menuItemsTopLevel[ID_GENERATOR] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_GENERATOR], &ArrMenu[ID_GENERATOR]);

        ArrMenu[ID_MAINS] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_MAINS], ID_CRANK_DISCON-ID_MAINS_CONFIG, &(menuItemsMidLevel[ID_MAINS_CONFIG]));
        menuItemsTopLevel[ID_MAINS] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_MAINS], &ArrMenu[ID_MAINS]);

        ArrMenu[ID_ENGINE] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_ENGINE], ID_MAINT_ALARM - ID_CRANK_DISCON, &(menuItemsMidLevel[ID_CRANK_DISCON]));
        menuItemsTopLevel[ID_ENGINE] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_ENGINE], &ArrMenu[ID_ENGINE]);

        ArrMenu[ID_MAINTENANCE] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_MAINTENANCE], ID_EGOV_GENERAL-ID_MAINT_ALARM, &(menuItemsMidLevel[ID_MAINT_ALARM]));
        menuItemsTopLevel[ID_MAINTENANCE] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_MAINTENANCE], &ArrMenu[ID_MAINTENANCE]);


        ArrMenu[ID_ROTARY_ACTUATOR] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_ROTARY_ACTUATOR], ID_ENG_SR_NO-ID_EGOV_GENERAL, &(menuItemsMidLevel[ID_EGOV_GENERAL]));
        if(CFGC::IsSGC421())
        {
            menuItemsTopLevel[ID_ROTARY_ACTUATOR] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_ROTARY_ACTUATOR], &ArrMenu[ID_ROTARY_ACTUATOR]);
        }
        else
        {
            menuItemsTopLevel[ID_ROTARY_ACTUATOR].isEnabled = false;
        }

        ArrMenu[ID_PASSWORD] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_PASSWORD], ID_RESET_GENSET-ID_ENG_SR_NO, &(menuItemsMidLevel[ID_ENG_SR_NO]));
        menuItemsTopLevel[ID_PASSWORD] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_PASSWORD], &ArrMenu[ID_PASSWORD]);

        ArrMenu[ID_RESET_PARAMETERS] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_RESET_PARAMETERS], ID_SELECT_PROFILE-ID_RESET_GENSET, &(menuItemsMidLevel[ID_RESET_GENSET]));
        menuItemsTopLevel[ID_RESET_PARAMETERS] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_RESET_PARAMETERS], &ArrMenu[ID_RESET_PARAMETERS]);

        ArrMenu[ID_SELECT_PROFILE_MENU] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_SELECT_PROFILE_MENU], ID_DISP_VOLT_FILT-ID_SELECT_PROFILE, &(menuItemsMidLevel[ID_SELECT_PROFILE]));
        menuItemsTopLevel[ID_SELECT_PROFILE_MENU] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_SELECT_PROFILE_MENU], &ArrMenu[ID_SELECT_PROFILE_MENU]);

        ArrMenu[ID_DISP_VOLT_FILT_MENU] = CMenu(strMainMenu[_u8LanguageArrayIndex][ID_DISP_VOLT_FILT_MENU], ID_SUB_MENU_LAST-ID_DISP_VOLT_FILT, &(menuItemsMidLevel[ID_DISP_VOLT_FILT]));
            menuItemsTopLevel[ID_DISP_VOLT_FILT_MENU] = CMenuItem(strMainMenu[_u8LanguageArrayIndex][ID_DISP_VOLT_FILT_MENU], &ArrMenu[ID_DISP_VOLT_FILT_MENU]);


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
            AllParam.f32ArrParam[CFGZ::ID_LOW_VOLT_THRESH] = (float)ArrEditableItem[INDEX_OF_SITE_LOW_BATT_THRESH].value.fVal;

            for(int i = CFGZ::ID_ENG_CLNT_CALIB_R1, j = INDEX_OF_ENG_CLNT_SENS_R1; i <= CFGZ::ID_ENG_CLNT_CALIB_R10;i=i+2 ,j=j+2)
            {
                AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.u16Val;
                AllParam.f32ArrParam[i+1] = (float)ArrEditableItem[j+1].value.i16Val;

            }


            for(int i = CFGZ::ID_FUEL_CALIB_R1, j = INDEX_OF_FUEL_SENS_R1; i <= CFGZ::ID_FUEL_CALIB_R10;i=i+2 ,j=j+2)
            {
                AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.u16Val;
                AllParam.f32ArrParam[i+1] = (float)ArrEditableItem[j+1].value.u8Val;
            }

            for(int i = CFGZ::ID_LOP_CALIB_R1, j = INDEX_OF_LOP_SENS_R1; i <= CFGZ::ID_LOP_CALIB_R10;i=i+2 ,j=j+2)
            {
                AllParam.f32ArrParam[i] = (float)ArrEditableItem[j].value.u16Val;
                AllParam.f32ArrParam[i+1] = (float)ArrEditableItem[j+1].value.fVal;

            }


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

            AllParam.f32ArrParam[CFGZ::ID_ISV_PULL_SIGNAL_TIME] = (float)ArrEditableItem[INDEX_OF_TMR_OF_ISV_PULL_SIGNAL_TIMER].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_BREAKER_PULS_TIMER] = (float)ArrEditableItem[INDEX_OF_TMR_OF_GEN_BREAKER].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_MAINS_BREAKER_PULS_TIMER] = (float)ArrEditableItem[INDEX_OF_TMR_OF_MAINS_BREAKER].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_BREAKER_FEEDBACK_TIMER] = (float)ArrEditableItem[INDEX_OF_TMR_OF_BREAKER_FEEDBACK].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_BREAKER_CLOSE_DELAY] = (float)ArrEditableItem[INDEX_OF_TMR_OF_BREAKER_CLOSE].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_MIN_HEALTHY_FREQ] = (float)ArrEditableItem[INDEX_OF_ALT_MIN_HEALTHY_FREQ].value.u8Val;
            AllParam.f32ArrParam[CFGZ::ID_GEN_UV_SHUTDOWN_DELAY_SEC] = (float)ArrEditableItem[INDEX_OF_UV_SHUT_DELAY].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_UV_WARNING_DELAY_SEC] = (float)ArrEditableItem[INDEX_OF_UV_WARN_DELAY].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_OV_SHUTDOWN_DELAY_SEC] = (float)ArrEditableItem[INDEX_OF_OV_SHUT_DELAY].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_OV_WARNING_DELAY_SEC] = (float)ArrEditableItem[INDEX_OF_OV_WARN_DELAY].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_UNDER_FREQ_SHUT_DN_THRESH] = (float)ArrEditableItem[INDEX_OF_UF_SHUT_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_UF_SHUTDOWN_DELAY_SEC] = (float)ArrEditableItem[INDEX_OF_UF_SHUT_DELAY].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_UNDER_FREQ_WARN_THRESH] = (float)ArrEditableItem[INDEX_OF_UF_WARN_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_UF_WARNING_DELAY_SEC] = (float)ArrEditableItem[INDEX_OF_UF_WARN_DELAY].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_OVER_FREQ_SHUT_DN_THRESH] = (float)ArrEditableItem[INDEX_OF_OF_SHUT_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_OF_SHUTDOWN_DELAY_SEC] = (float)ArrEditableItem[INDEX_OF_OF_SHUT_DELAY].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_OVER_FREQ_WARN_THRESH] = (float)ArrEditableItem[INDEX_OF_OF_WARN_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_OF_WARNING_DELAY_SEC] = (float)ArrEditableItem[INDEX_OF_OF_WARN_DELAY].value.fVal;
            //AllParam.f32ArrParam[CFGZ::ID_CT_CORRECTION_FACTOR] = (float)ArrEditableItem[INDEX_OF_CURR_MON_RESERVED].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_HIGH_CURR_THRESH] = (float)ArrEditableItem[INDEX_OF_EFM_HIGH_CURR_THRESH].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_LOW_CURR_THRESH] = (float)ArrEditableItem[INDEX_OF_EFM_LOW_CURR_THRESH].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_GEN_RATING] = (float)ArrEditableItem[INDEX_OF_LM_RATING].value.u16Val;
            AllParam.f32ArrParam[CFGZ::ID_MAINS_VOLT_TRIP_DELAY_SEC] = (float)ArrEditableItem[INDEX_OF_MAINS_VOLT_TRIP_DELAY].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_MAINS_UNDER_FREQ_TRIP_THRESH] = (float)ArrEditableItem[INDEX_OF_MAINS_UF_TRIP_THRESH].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_MAINS_UNDER_FREQ_RET_THRESH] = (float)ArrEditableItem[INDEX_OF_MAINS_UF_RETURN_THRESH].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_MAINS_OVER_FREQ_TRIP_THRESH] = (float)ArrEditableItem[INDEX_OF_MAINS_OF_TRIP_THRESH].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_MAINS_OVER_FREQ_RET_THRESH] = (float)ArrEditableItem[INDEX_OF_MAINS_OF_RETURN_THRESH].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_MAINS_FREQ_TRIP_DELAY_SEC] = (float)ArrEditableItem[INDEX_OF_MAINS_FREQ_TRIP_DELAY].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_DISCONNECT_PRESURE_THRESH] = (float)ArrEditableItem[INDEX_OF_START_LOP_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_LOP_SW_TRANSIENT_TIME] = (float)ArrEditableItem[INDEX_OF_START_LLOP_DELAY].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_CRANK_DISCON_CHARG_ALT_THRESH] = (float)ArrEditableItem[INDEX_OF_START_CA_THRESHOLD    ].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_LOW_BAT_VTG_THRESH] = (float)ArrEditableItem[INDEX_OF_LOW_VBAT_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_HIGH_BAT_VTG_THRESH] = (float)ArrEditableItem[INDEX_OF_HIGH_VBAT_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_CHARG_ALT_FAIL_THRESH] = (float)ArrEditableItem[INDEX_OF_CA_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_ENGINE_GAIN] = (float)ArrEditableItem[INDEX_OF_ENGINE_GAIN_ECU].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_LOP_LVL_SHUTDOWN_THRESH] = (float)ArrEditableItem[INDEX_OF_LOP_SHUTDOWN_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_LOP_LVL_WARNING_THRESH] = (float)ArrEditableItem[INDEX_OF_LOP_WARNING_THRESHOLD].value.fVal;
            AllParam.f32ArrParam[CFGZ::ID_EGOV_GEN_GAIN_SCHEDULE] = (float)ArrEditableItem[INDEX_OF_EGOV_GEN_GAIN_SCHEDULE].value.fVal;

            AllParam.u16ArrParam[CFGZ::ID_BATT_MON_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_SITE_LOW_BATT_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_GEN_RUN_DURATION] = (uint16_t)ArrEditableItem[INDEX_OF_SITE_GEN_RUN_DURATION].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_GEN_OFF_TIME] = (uint16_t)ArrEditableItem[INDEX_OF_CYCLIC_GEN_OFF_DURATION].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_GEN_ON_TIME] = (uint16_t)ArrEditableItem[INDEX_OF_CYCLIC_GEN_ON_DURATION].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EXERCISE_1_START_TIME] = (uint16_t)ArrEditableItem[INDEX_OF_EXERCISER_START_TIME_1].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EXERCISE_1_ON_DURATION] = (uint16_t)ArrEditableItem[INDEX_OF_EXERCISER_DG_ON_DURATION_1].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EXERCISE_2_START_TIME] = (uint16_t)ArrEditableItem[INDEX_OF_EXERCISER_START_TIME_2].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EXERCISE_2_ON_DURATION] = (uint16_t)ArrEditableItem[INDEX_OF_EXERCISER_DG_ON_DURATION_2].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_NIGHT_START_TIME] = (uint16_t)ArrEditableItem[INDEX_OF_NIGHT_START_TIME].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_NIGHT_OFF_DURATION] = (uint16_t)ArrEditableItem[INDEX_OF_NIGHT_GEN_OFF_DURATION].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_FUEL_TANK_CAPACITY] = (uint16_t)ArrEditableItem[INDEX_OF_FUEL_TANK_CAPACITY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_SHELT_TEMP_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_SHELT_TEMP_THRESHOLD].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_SHELT_TEMP_HYST] = (uint16_t)ArrEditableItem[INDEX_OF_SHELT_TEMP_HYSTERISIS].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_SHELT_TEMP_MON_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_SHELT_TEMP_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_SHELT_TEMP_RUN_DURATION_MIN] = (uint16_t)ArrEditableItem[INDEX_OF_SHELT_TEMP_RUN_DURATION].value.u16Val;

            for(int i =CFGZ::ID_CRANK_HOLD_TIME , j=INDEX_OF_TMR_OF_CRANKING ;i <=CFGZ::ID_AUTO_EXIT_TIME;i++,j++)
            {
                AllParam.u16ArrParam[i] = (uint16_t)ArrEditableItem[j].value.u16Val;

            }

            AllParam.u16ArrParam[CFGZ::ID_MIN_HEALTHY_VTG] = (uint16_t)ArrEditableItem[INDEX_OF_ALT_MIN_HEALTHY_VOLT].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_GEN_PT_PRIMARY_RATIO] = (uint16_t)ArrEditableItem[INDEX_OF_GEN_PT_PRIMARY_RATIO].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_GEN_PT_SECONDARY_RATIO] = (uint16_t)ArrEditableItem[INDEX_OF_GEN_PT_SECONDARY_RATIO].value.u16Val;

            AllParam.u16ArrParam[CFGZ::ID_GEN_UNDER_VTG_SHUTDN_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_UV_SHUT_THRESHOLD].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_GEN_UNDER_VTG_WARN_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_UV_WARN_THRESHOLD].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_GEN_OVER_VTG_SHUTDN_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_OV_SHUT_THRESHOLD].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_GEN_OVER_VTG_WARN_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_OV_WARN_THRESHOLD].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_CM_CT_RATIO] = (uint16_t)ArrEditableItem[INDEX_OF_CM_CT_RATIO].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_OVER_CURR_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_CM_OC_THRESHOLD].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_OVER_CURR_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_CM_OC_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_FM_CT_RATIO] = (uint16_t)ArrEditableItem[INDEX_OF_EFM_CT_RATIO].value.u16Val;

            AllParam.u16ArrParam[CFGZ::ID_CURR_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_EFM_CURR_DELAY].value.u16Val;

            AllParam.u16ArrParam[CFGZ::ID_OVER_LOAD_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_LM_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_UNBAL_LOAD_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_LM_UNBALANCE_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_LOW_LOAD_DELAY_SEC] = (uint16_t)ArrEditableItem[INDEX_OF_LOW_LOAD_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_MAINS_PT_PRIMARY_RATIO] = (uint16_t)ArrEditableItem[INDEX_OF_MAINS_PT_PRIMARY_RATIO].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_MAINS_PT_SECONDARY_RATIO] = (uint16_t)ArrEditableItem[INDEX_OF_MAINS_PT_SECONDARY_RATIO].value.u16Val;

            AllParam.u16ArrParam[CFGZ::ID_MAINS_UNDER_VTG_TRIP_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_MAINS_UV_TRIP_THRESH].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_MAINS_UNDER_VTG_RET_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_MAINS_UV_RETURN_THRESH].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_MAINS_OVER_VTG_TRIP_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_MAINS_OV_TRIP_THRESH].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_MAINS_OVER_VTG_RET_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_MAINS_OV_RETURN_THRESH].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_CRANK_DISCON_ENG_SPEED_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_START_DISCONN_RPM].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_MPU_TEETH_W_POINT_FREQ] = (uint16_t)ArrEditableItem[INDEX_OF_MPU_TEETH_W_PT_FREQ].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_UNDER_SPEED_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_US_THRESHOLD].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_UNDER_SPEED_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_US_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_OVER_SPEED_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_OS_THRESHOLD ].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_OVER_SPEED_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_OS_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_IDLE_TO_RATED_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_IDLE_RATED_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_STARTUP_IDLE_TIME] = (uint16_t)ArrEditableItem[INDEX_OF_STARTUP_IDLE_TIME].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_STOPPING_IDLE_TIME] = (uint16_t)ArrEditableItem[INDEX_OF_STOPPING_IDLE_TIME].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_INITIAL_LOW_SPEED] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_INIT_LOW_SPEED].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_LOW_BAT_VTG_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_LOW_VBAT_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_HIGH_BAT_VTG_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_HIGH_VBAT_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_CHARG_ALT_FAIL_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_CA_DELAY].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_PREHEAT_TIMER] = (uint16_t)ArrEditableItem[INDEX_OF_PH_TIMER].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_ENGINE_TEMP_LIMIT_THREH] = (uint16_t)ArrEditableItem[INDEX_OF_PH_TEMP_VALUE].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_CLNT_TEMP_ON_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_CLNT_TEMP_ON_THRESH].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_CLNT_TEMP_OFF_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_CLNT_TEMP_OFF_THRESH].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_TIMEOUT_AFTER_ACTIVATION] = (uint16_t)ArrEditableItem[INDEX_OF_AFT_TIMEOUT_AFTER_ACTIVATION].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_ENG_REQUESTED_SPEED_TO_ECU] = (uint16_t)ArrEditableItem[INDEX_OF_ENG_REQ_SPEED].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_SGC_SOURCE_ADDRESS] = (uint16_t)ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_ECU_SOURCE_ADDRESS] = (uint16_t)ArrEditableItem[INDEX_OF_ECU_SOURCE_ADDRESS].value.u16Val;

            AllParam.u16ArrParam[CFGZ::ID_ECU_COMM_FAILURE_ACT_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACT_DELAY].value.u8Val;
            AllParam.u16ArrParam[CFGZ::ID_ECU_AMBER_ACT_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT_DELAY].value.u8Val;
            AllParam.u16ArrParam[CFGZ::ID_ECU_RED_ACT_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_RED_LAMP_ACT_DELAY].value.u8Val;
            AllParam.u16ArrParam[CFGZ::ID_ECU_MALFUNCTION_ACT_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT_DELAY].value.u8Val;
            AllParam.u16ArrParam[CFGZ::ID_ECU_PROTECT_ACT_DELAY] = (uint16_t)ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT_DELAY].value.u8Val;

            AllParam.u16ArrParam[CFGZ::ID_HIGH_CLNT_TEMP_SHUTDOWN_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_ECT_SHUTDOWN_THRESHOLD].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_HIGH_CLNT_TEMP_WARNING_THRESH] = (uint16_t)ArrEditableItem[INDEX_OF_ECT_WARNING_THRESHOLD].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_MAINTENANCE_DUE_HOURS] = (uint16_t)ArrEditableItem[INDEX_OF_FILT_MAINT_THRESHOLD].value.u16Val;

            AllParam.u16ArrParam[CFGZ::ID_EGOV_CRANK_BOOST] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_CRANK_BOOST].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EGOV_RPM_PID_ON_SPEED] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_RPM_PID_ON].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EGOV_RAMP_UP_TIME] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_RAMP_UP_TIME].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EGOV_PID_ON_TIME] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_PID_ON_TIME].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EGOV_GEN_SET_SPEED] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_GEN_SET_SPEED].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EGOV_GEN_KP] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_GEN_KP].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EGOV_GEN_KI] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_GEN_KI].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EGOV_GEN_KD] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_GEN_KD].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EGOV_GEN_DITHER] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_GEN_DITHER].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EGOV_GEN_LOADING_FAC] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_GEN_LOADING_FAC].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EGOV_GEN_UNLOADING_FAC] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_GEN_UNLOADING_FAC].value.u16Val;
            AllParam.u16ArrParam[CFGZ::ID_EGOV_RUNNING_STEPS] = (uint16_t)ArrEditableItem[INDEX_OF_EGOV_RUNNING_STEPS].value.u16Val;


            u8Month = (uint8_t)ArrEditableItem[INDEX_OF_FILT_MAINT_DUE_DATE].value.stDate.u8Month;
            u16Year = (uint16_t)ArrEditableItem[INDEX_OF_FILT_MAINT_DUE_DATE].value.stDate.u16Year;
            u8Date = (uint8_t)ArrEditableItem[INDEX_OF_FILT_MAINT_DUE_DATE].value.stDate.u8Date;
            if((_stMaintenanceDt.u8Date!=u8Date)||(_stMaintenanceDt.u8Month!=u8Month)||(_stMaintenanceDt.u16Year!=u16Year))
            {
                if(u8Date > prvMaxDaysInMonth( u8Month, u16Year))
                {
                    AllParam.u8ArrParam[CFGZ::ID_FILT_MAINT_THRESH_DAY] = (uint8_t)_stMaintenanceDt.u8Date;
                    AllParam.u8ArrParam[CFGZ::ID_FILT_MAINT_THRESH_MONTH] = (uint8_t)_stMaintenanceDt.u8Month;
                    AllParam.u16ArrParam[CFGZ:: ID_FILT_MAINT_THRESH_YEAR] = (uint16_t)_stMaintenanceDt.u16Year;
                    _objDisplay.gotoxy(GLCD_X(64),GLCD_Y(50));
                    _objDisplay.printStringCenterAligned((char *)StrMaintDate[_u8LanguageArrayIndex], FONT_VERDANA);
                }
                else
                {
                    AllParam.u8ArrParam[CFGZ::ID_FILT_MAINT_THRESH_DAY] = (uint8_t)ArrEditableItem[INDEX_OF_FILT_MAINT_DUE_DATE].value.stDate.u8Date;
                    AllParam.u8ArrParam[CFGZ::ID_FILT_MAINT_THRESH_MONTH] = (uint8_t)ArrEditableItem[INDEX_OF_FILT_MAINT_DUE_DATE].value.stDate.u8Month;
                    AllParam.u16ArrParam[CFGZ:: ID_FILT_MAINT_THRESH_YEAR] = (uint16_t)ArrEditableItem[INDEX_OF_FILT_MAINT_DUE_DATE].value.stDate.u16Year;
                    _objDisplay.gotoxy(GLCD_X(64),GLCD_Y(50));
                    _objDisplay.printStringCenterAligned((char *)StrMaintDate[_u8LanguageArrayIndex], FONT_VERDANA);
                }
            }
            else
            {
                AllParam.u8ArrParam[CFGZ::ID_FILT_MAINT_THRESH_DAY] = (uint8_t)_stMaintenanceDt.u8Date;
                AllParam.u8ArrParam[CFGZ::ID_FILT_MAINT_THRESH_MONTH] = (uint8_t)_stMaintenanceDt.u8Month;
                AllParam.u16ArrParam[CFGZ:: ID_FILT_MAINT_THRESH_YEAR] = (uint16_t)_stMaintenanceDt.u16Year;
                _objDisplay.gotoxy(GLCD_X(64),GLCD_Y(50));
            }

            AllParam.u8ArrParam[CFGZ::ID_POWER_ON_MODE] = (uint8_t)ArrEditableItem[INDEX_OF_POWER_ON_MODE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_POWER_ON_LAMP_TEST_EN] = (uint8_t)ArrEditableItem[INDEX_OF_POWER_ON_LAMP_TEST].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DEEP_SLEEP_EN] = (uint8_t)ArrEditableItem[INDEX_OF_DEEP_SLEEP_ENABLE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LOAD_HISTOGRAM] = (uint8_t)ArrEditableItem[INDEX_OF_LOAD_HISTOGRAM].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_WARNING_AUTO_CLEAR_EN] = (uint8_t)ArrEditableItem[INDEX_OF_WARNING_AUTO_CLEAR].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LANGUAGE] = (uint8_t)ArrEditableItem[INDEX_OF_LANGUAGE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_PERCENT_CONTRAST] = (uint8_t)ArrEditableItem[INDEX_OF_CONTRAST].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_POWER_SAVE_MODE_EN] = (uint8_t)ArrEditableItem[INDEX_OF_POWER_SAVE_MODE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_COMMUNICATION_EN_MB] = (uint8_t)ArrEditableItem[INDEX_OF_COMM_MODE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_SLAVEID_MB] = (uint8_t)ArrEditableItem[INDEX_OF_MODBUS_SLAVE_ID].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_BAUDRATE_MB] = (uint8_t)ArrEditableItem[INDEX_OF_MODBUS_BAUDRATE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_PARITYBIT_MB] = (uint8_t)ArrEditableItem[INDEX_OF_MODBUS_PARITYBIT].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_BATT_MON_EN] = (uint8_t)ArrEditableItem[INDEX_OF_SITE_BATTERY_MON].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CYCLIC_MODE_EN] = (uint8_t)ArrEditableItem[INDEX_OF_CYCLIC_MODE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EXERCISE_1_EN] = (uint8_t)ArrEditableItem[INDEX_OF_EXERCISER_EVENT_1].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EXERCISE_1_OCCURENCE] = (uint8_t)ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_1].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EXERCISE_1_START_DAY] = (uint8_t)ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EXERCISE_1_LOAD_TRANSFER] = (uint8_t)ArrEditableItem[INDEX_OF_EXERCISER_LOAD_TRANSFER_1].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EXERCISE_2_EN] = (uint8_t)ArrEditableItem[INDEX_OF_EXERCISER_EVENT_2].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EXERCISE_2_OCCURENCE] = (uint8_t)ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_2].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EXERCISE_2_START_DAY] = (uint8_t)ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EXERCISE_2_LOAD_TRANSFER] = (uint8_t)ArrEditableItem[INDEX_OF_EXERCISER_LOAD_TRANSFER_2].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_NIGHT_MODE_EN] = (uint8_t)ArrEditableItem[INDEX_OF_NIGHT_MODE_RESTRICT].value.u8Val;

            for(int i = CFGZ::ID_DIG_INPUTA_SOURCE, j = INDEX_OF_DIG_IN_A_SOURCE; i <= CFGZ::ID_DIG_INPUTI_ACTIVATION_DLY;i++ ,j++)
            {
                AllParam.u8ArrParam[i] = (uint8_t)ArrEditableItem[j].value.u8Val;

            }

            AllParam.u8ArrParam[CFGZ::ID_LOP_SENS_SELECTION] = (uint8_t)ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTJ_SOURCE] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_J_SOURCE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTJ_POLARITY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_J_POLARITY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTJ_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_J_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTJ_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_J_ACTIVATION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTJ_ACTIVATION_DLY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_J_DELAY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LOP_SENS_FAULT_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_LOP_CKT_FAULT_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_FUEL_SENS_SELECTION] = (uint8_t)ArrEditableItem[INDEX_OF_FUEL_DIG_K_SENSOR].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTK_SOURCE] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_K_SOURCE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTK_POLARITY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_K_POLARITY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTK_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_K_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTK_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_K_ACTIVATION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTK_ACTIVATION_DLY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_K_DELAY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_FUEL_LOW_LEVEL_SHUT_DN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_LFL_SHUTDOWN_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_FUEL_SHUT_DN_THRESH] = (uint8_t)ArrEditableItem[INDEX_OF_LFL_SHUTDOWN_THRESHOLD].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_FUEL_LOW_LEVEL_WARN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_LFL_WARNING_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_FUEL_WARN_THRESH] = (uint8_t)ArrEditableItem[INDEX_OF_LFL_WARNING_THRESHOLD].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_FUEL_THEFT_WARN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_FUEL_THEFT_ALARM_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_FUEL_THEFT_THRESH] = (uint8_t)ArrEditableItem[INDEX_OF_FUEL_THEFT_THRESHOLD].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_FUEL_SENS_FAULT_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_FUEL_CKT_FAULT_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_FUEL_SENSOR_REFERENCE] = (uint8_t)ArrEditableItem[INDEX_OF_FUEL_SENS_REFERENCE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ENG_TEMP_SENS_SELECTION] = (uint8_t)ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTL_SOURCE] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_L_SOURCE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTL_POLARITY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_L_POLARITY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTL_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_L_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTL_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_L_ACTIVATION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTL_ACTIVATION_DLY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_L_DELAY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ENG_TEMP_SENS_FAULT_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_ENG_CLNT_CKT_FAULT_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S1_SENS_SELECTION] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTM_SOURCE] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_M_SOURCE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTM_POLARITY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_M_POLARITY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTM_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_M_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTM_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_M_ACTIVATION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTM_ACTIVATION_DLY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_M_DELAY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S1_THRESH_TYPE] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S1_THRESHOLD_TYPE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S1_SHUT_DN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S1_SHUTDOWN_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S1_WARN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S1_WARNING_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S1_SENS_FAULT_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S1_CKT_FAULT_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S2_SENS_SELECTION] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S2_DIG_N_SENSOR].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTN_SOURCE] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_N_SOURCE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTN_POLARITY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_N_POLARITY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTN_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_N_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTN_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_N_ACTIVATION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTN_ACTIVATION_DLY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_N_DELAY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S2_THRESH_TYPE] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S2_THRESHOLD_TYPE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S2_SHUT_DN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S2_SHUTDOWN_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S2_WARN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S2_WARNING_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S2_SENS_FAULT_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S2_CKT_FAULT_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S3_SENS_SELECTION] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTO_SOURCE] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_O_SOURCE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTO_POLARITY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_O_POLARITY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTO_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_O_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTO_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_O_ACTIVATION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTO_ACTIVATION_DLY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_O_DELAY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S3_THRESH_TYPE] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S3_THRESHOLD_TYPE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S3_SHUT_DN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S3_WARN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S3_WARNING_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S3_SENS_FAULT_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S3_CKT_FAULT_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S4_SENS_SELECTION] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTP_SOURCE] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_P_SOURCE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTP_POLARITY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_P_POLARITY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTP_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_P_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTP_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_P_ACTIVATION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DIG_INPUTP_ACTIVATION_DLY] = (uint8_t)ArrEditableItem[INDEX_OF_DIG_IN_P_DELAY].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S4_THRESH_TYPE] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S4_THRESHOLD_TYPE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S4_SHUT_DN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S4_SHUTDOWN_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S4_WARN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S4_WARNING_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_S4_SENS_FAULT_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_AUX_S4_CKT_FAULT_ACTION].value.u8Val;

            for(int i =CFGZ::ID_DIG_OP_SOURCE_A , j=INDEX_OF_DIG_OUT_A_SOURCE ;i <=CFGZ::ID_DIG_OP_ACTIVATION_G;i++,j++)
            {
                AllParam.u8ArrParam[i] = (uint8_t)ArrEditableItem[j].value.u8Val;

            }

            AllParam.u8ArrParam[CFGZ::ID_ALTERNATOR_PRESENT] = (uint8_t)ArrEditableItem[INDEX_OF_ALT_PRESENT ].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_NUMBER_OF_POLES] = (uint8_t)ArrEditableItem[INDEX_OF_ALT_POLES].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_GEN_AC_SYSTEM_TYPE] = (uint8_t)ArrEditableItem[INDEX_OF_ALT_SYS].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_PH_REVERS_DETECT_EN] = (uint8_t)ArrEditableItem[INDEX_OF_DG_PHASE_ROT_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_PH_REVERS_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_DG_PHASE_ROT_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_AUTOLOAD_TRANSFER] = (uint8_t)ArrEditableItem[INDEX_OF_AUTO_LOAD_TRANSFER].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ALT_WAVE_DETECT_EN] = (uint8_t)ArrEditableItem[INDEX_OF_WAVEFORM_DETECT_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_GEN_PT_ENABLE] = (uint8_t)ArrEditableItem[INDEX_OF_GEN_PT_ENABLE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_GEN_UNDER_VTG_SHUT_DN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_UV_SHUT_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_GEN_UNDER_VTG_WARN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_UV_WARN_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_GEN_OVER_VTG_SHUT_DN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_OV_SHUT_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_GEN_OVER_VTG_WARN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_OV_WARN_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_GEN_UNDER_FREQ_SHUT_DN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_UF_SHUT_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_GEN_UNDER_FREQ_WARN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_UF_WARN_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_GEN_OVER_FREQ_SHUT_DN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_OF_SHUT_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_GEN_OVER_FREQ_WARN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_OF_WARN_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_OVER_CURR_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_CM_OC_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CT_LOCATION] = (uint8_t)ArrEditableItem[INDEX_OF_CT_LOCATION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_FAN_CM_SELECTION] = (uint8_t)ArrEditableItem[INDEX_OF_EFM_CURRENT].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_HIGH_CURR_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_EFM_HIGH_CURR_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LOW_CURR_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_EFM_LOW_CURR_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_OL_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_LM_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_OL_THRESH_PERCENT] = (uint8_t)ArrEditableItem[INDEX_OF_LM_THRESHOLD].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_UNBAL_LOAD_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_LM_UNBALANCE_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_UNBAL_LOAD_THRESH] = (uint8_t)ArrEditableItem[INDEX_OF_LM_UNBALANCE_TH].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LOW_LOAD_ALARM_EN] = (uint8_t)ArrEditableItem[INDEX_OF_LOW_LOAD_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LOW_LOAD_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_LOW_LOAD_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LOW_LOAD_TRIP] = (uint8_t)ArrEditableItem[INDEX_OF_LOW_LOAD_TRIP].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LOW_LOAD_RETURN] = (uint8_t)ArrEditableItem[INDEX_OF_LOW_LOAD_RETURN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINS_MON_EN] = (uint8_t)ArrEditableItem[INDEX_OF_MAINS_MON_ON].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINS_AC_SYTEM_TYPE] = (uint8_t)ArrEditableItem[INDEX_OF_MAINS_SYSTEM].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINS_PH_REVERS_DETECT_EN] = (uint8_t)ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINS_PH_REVERS_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINS_PARTIAL_HEALTHY_DETECT_EN] = (uint8_t)ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINS_PT_ENABLE] = (uint8_t)ArrEditableItem[INDEX_OF_MAINS_PT_ENABLE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINS_UNDER_VTG_MON_EN] = (uint8_t)ArrEditableItem[INDEX_OF_MAINS_UV_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINS_OVER_VTG_MON_EN] = (uint8_t)ArrEditableItem[INDEX_OF_MAINS_OV_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINS_UNDER_FREQ_MON_EN] = (uint8_t)ArrEditableItem[INDEX_OF_MAINS_UF_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINS_OVER_FREQ_MON_EN] = (uint8_t)ArrEditableItem[INDEX_OF_MAINS_OF_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CRANK_ATTEMPTS] = (uint8_t)ArrEditableItem[INDEX_OF_START_ATTEMPTS].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DISCON_ON_LOP_SENS_EN] = (uint8_t)ArrEditableItem[INDEX_OF_START_USE_LOP].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MONITOR_LLOP_BEFORE_CRANK] = (uint8_t)ArrEditableItem[INDEX_OF_START_LLOP_AT_CRANK].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MONITOR_LOP_SENSE_BEFORE_CRANK] = (uint8_t)ArrEditableItem[INDEX_OF_START_LOP_AT_CRANK].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_DISCON_ON_LOP_SW_EN] = (uint8_t)ArrEditableItem[INDEX_OF_START_USE_LLOP].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CRANK_DISCON_ALT_FREQ_THRESH] = (uint8_t)ArrEditableItem[INDEX_OF_START_DISCONN_FREQ].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CRANK_DISCON_CHARG_ALT_EN] = (uint8_t)ArrEditableItem[INDEX_OF_START_USE_CA_VOLT].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ENG_SPEED_SOURCE] = (uint8_t)ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_UNDERSPEED_SHUT_DN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_US_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_GROSS_OVER_SPEED_THRESH] = (uint8_t)ArrEditableItem[INDEX_OF_GROSS_OS_THRESHOLD].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_IDLE_MODE_PULSE_TIME] = (uint8_t)ArrEditableItem[INDEX_OF_IDLE_MODE_PULSE_TIME].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LOW_BAT_VTG_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_LOW_VBAT_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_HIGH_BAT_VTG_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_HIGH_VBAT_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CHARG_ALT_FAIL_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_CA_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ENGINE_TEMP_LIMIT_EN] = (uint8_t)ArrEditableItem[INDEX_OF_PH_TEMP_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CLNT_TEMP_CTRL_EN] = (uint8_t)ArrEditableItem[INDEX_OF_CLNT_TEMP_CTRL_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CLNT_TEMP_CTRL] = (uint8_t)ArrEditableItem[INDEX_OF_CLNT_TEMP_CTRL].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_AFT_ACTIVATION_THRESH] = (uint8_t)ArrEditableItem[INDEX_OF_AFT_ACTIVATION_THRESH].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_AFT_DEACTIVATION_THRESH] = (uint8_t)ArrEditableItem[INDEX_OF_AFT_DEACTIVATION_THRESH].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MON_AFT_FROM_ENGINE_ON] = (uint8_t)ArrEditableItem[INDEX_OF_AFT_MONITOR_ON_ENGINE_ON].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ENGINE_TYPE] = (uint8_t)ArrEditableItem[INDEX_OF_ENGINE_TYPE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LOP_FROM_ENG] = (uint8_t)ArrEditableItem[INDEX_OF_LOP_FROM_ECU].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CLNT_TEMP_FROM_ENG] = (uint8_t)ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ENGINE_SPEED_FROM_ENG] = (uint8_t)ArrEditableItem[INDEX_OF_ENG_SPEED_FROM_ECU].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_RUNNING_HOURS_FROM_ENG] = (uint8_t)ArrEditableItem[INDEX_OF_ENG_RUN_HOURS_FROM_ECU].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_BATTERY_VOLT_FROM_ENG] = (uint8_t)ArrEditableItem[INDEX_OF_BATT_VTG_FROM_ECU].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_SPEED_TO_ECU] = (uint8_t)ArrEditableItem[INDEX_OF_ENG_SPEED_TO_ECU].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_START_STOP_TO_ECU] = (uint8_t)ArrEditableItem[INDEX_OF_START_STOP_ENG_CMD_ECU].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_PREHEAT_TO_ECU] = (uint8_t)ArrEditableItem[INDEX_OF_PREHEAT_CMD_ECU].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ENGINE_FRQ] = (uint8_t)ArrEditableItem[INDEX_OF_ENGINE_FRQ_ECU].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ECU_COMM_FAILURE_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ECU_COMM_FAILURE_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACT].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ECU_AMBER_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_AMBER_LAMP_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ECU_AMBER_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_AMBER_LAMP_ACT].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ECU_RED_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_RED_LAMP_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ECU_RED_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_RED_LAMP_ACT].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ECU_MALFUNCTION_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ECU_MALFUNCTION_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACT].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ECU_PROTECT_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_ECU_PROTECT_ACTIVATION] = (uint8_t)ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACT].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LOP_LVL_SHUTDOWN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_LOP_SHUTDOWN_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_LOP_LVL_WARNING_EN] = (uint8_t)ArrEditableItem[INDEX_OF_LOP_WARNING_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CLNT_TEMP_THRESH_TYPE] = (uint8_t)ArrEditableItem[INDEX_OF_COOLANT_TEMP_THRESH_TYPE].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CLNT_TEMP_SHUTDOWN_EN] = (uint8_t)ArrEditableItem[INDEX_OF_ECT_SHUTDOWN_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_CLNT_TEMP_WARNING_EN] = (uint8_t)ArrEditableItem[INDEX_OF_ECT_WARNING_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINTENANCE_ALARM_ACTION] = (uint8_t)ArrEditableItem[INDEX_OF_FILT_MAINT_ACTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_MAINT_ASH_LOAD_EN] = (uint8_t)ArrEditableItem[INDEX_OF_FILT_MAINT_ASH_LOAD_EN].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EGOV_ACT_APPLICATION] = (uint8_t)ArrEditableItem[INDEX_OF_EGOV_ACT_APPLICATION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EGOV_ACT_SPEED] = (uint8_t)ArrEditableItem[INDEX_OF_EGOV_ACT_SPEED].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EGOV_ACT_DIRECTION] = (uint8_t)ArrEditableItem[INDEX_OF_EGOV_DIR].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EGOV_SET_POINT_SELECTION] = (uint8_t)ArrEditableItem[INDEX_OF_EGOV_SET_POINT_SELECTION].value.u8Val;
            AllParam.u8ArrParam[CFGZ::ID_EGOV_PERCENT_DROOP] = (uint8_t)ArrEditableItem[INDEX_OF_EGOV_PERCENT_DROOP].value.u8Val;

            for(uint16_t i = CFGZ::ID_ARR_AUX_INPUT_A;i<=CFGZ::ID_ARR_PROFILE;i++)
            {
                _objcfgz.GetCFGZ_Param((CFGZ::ARRAY_PARAMS_t)i, AllParam.u8ArrStringParam[i]);
            }
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_A, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_A]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_B, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_B]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_C, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_C]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_D, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_D]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_E, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_E]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_F, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_F]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_G, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_G]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_H, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_H]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_I, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_I]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_J, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_J]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_K, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_K]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_L, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_L]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_M, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_M]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_N, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_N]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_O, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_O]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_AUX_INPUT_P, AllParam.u8ArrStringParam[CFGZ::ID_ARR_AUX_INPUT_P]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_SENSOR_S1_NAME, AllParam.u8ArrStringParam[CFGZ::ID_ARR_SENSOR_S1_NAME]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_SENSOR_S2_NAME, AllParam.u8ArrStringParam[CFGZ::ID_ARR_SENSOR_S2_NAME]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_SENSOR_S3_NAME, AllParam.u8ArrStringParam[CFGZ::ID_ARR_SENSOR_S3_NAME]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_SENSOR_S4_NAME, AllParam.u8ArrStringParam[CFGZ::ID_ARR_SENSOR_S4_NAME]);
//            _objcfgz.GetCFGZ_Param(CFGZ::ID_ARR_PROFILE, AllParam.u8ArrStringParam[CFGZ::ID_ARR_PROFILE]);

        }

        _MiscParam.u8MiscParam[PROFILE_NO] = (uint8_t)ArrEditableItem[INDEX_OF_ACTIVE_PROFILE].value.u8Val;
        //Todo: Implement storing on Gen Cumulative Count and Mains counters.
        if(_MiscParam.u8MiscParam[RESET_COUNTER] <= 2)
        {
            bool bUpdateResetCntAttempt = false;
            if(ArrEditableItem[INDEX_OF_RESET_ENG_RUN_HRS].value.u16Val != 0)
            {
                _engMon.StoreEngRnCnt((uint32_t)ArrEditableItem[INDEX_OF_RESET_ENG_RUN_HRS].value.u16Val * 60);
                bUpdateResetCntAttempt = true;
            }
            if(ArrEditableItem[INDEX_OF_RESET_NO_OF_STARTS].value.u16Val != 0)
            {
                _engMon.StoreStartCnt(ArrEditableItem[INDEX_OF_RESET_NO_OF_STARTS].value.u16Val);
                bUpdateResetCntAttempt = true;
            }
            if(ArrEditableItem[INDEX_OF_RESET_NO_OF_TRIPS].value.u16Val != 0)
            {
                _engMon.StoreTripCnt(ArrEditableItem[INDEX_OF_RESET_NO_OF_TRIPS].value.u16Val);
                bUpdateResetCntAttempt = true;
            }
            if(ArrEditableItem[INDEX_OF_RESET_GEN_KWH].value.u16Val != 0)
            {
                _engMon.StoreGenEnery(ENGINE_MONITORING::ACTIVE_POWER, ArrEditableItem[INDEX_OF_RESET_GEN_KWH].value.u16Val);
                bUpdateResetCntAttempt = true;
            }
            if(ArrEditableItem[INDEX_OF_RESET_GEN_KVAH].value.u16Val != 0)
            {
                _engMon.StoreGenEnery(ENGINE_MONITORING::APARENT_POWER , ArrEditableItem[INDEX_OF_RESET_GEN_KVAH].value.u16Val);
                bUpdateResetCntAttempt = true;
            }
            if(ArrEditableItem[INDEX_OF_RESET_GEN_KVARH].value.u16Val != 0)
            {
                _engMon.StoreGenEnery(ENGINE_MONITORING::REACTIVE_POWER, ArrEditableItem[INDEX_OF_RESET_GEN_KVARH].value.u16Val);
                bUpdateResetCntAttempt = true;
            }

            if(ArrEditableItem[INDEX_OF_RESET_MAINS_RUN_HRS].value.u16Val != 0)
            {
                _engMon.StoreMainsRnCnt((uint32_t)ArrEditableItem[INDEX_OF_RESET_MAINS_RUN_HRS].value.u16Val * 60);
                bUpdateResetCntAttempt = true;
            }
            if(ArrEditableItem[INDEX_OF_RESET_MAINS_KWH].value.u16Val != 0)
            {
                _engMon.StoreMainsEnery(ENGINE_MONITORING::ACTIVE_POWER, ArrEditableItem[INDEX_OF_RESET_MAINS_KWH].value.u16Val);
                bUpdateResetCntAttempt = true;
            }
            if(ArrEditableItem[INDEX_OF_RESET_MAINS_KVAH].value.u16Val != 0)
            {
                _engMon.StoreMainsEnery(ENGINE_MONITORING::APARENT_POWER , ArrEditableItem[INDEX_OF_RESET_MAINS_KVAH].value.u16Val);
                bUpdateResetCntAttempt = true;
            }
            if(ArrEditableItem[INDEX_OF_RESET_MAINS_KVARH].value.u16Val != 0)
            {
                _engMon.StoreMainsEnery(ENGINE_MONITORING::REACTIVE_POWER, ArrEditableItem[INDEX_OF_RESET_MAINS_KVARH].value.u16Val);
                bUpdateResetCntAttempt = true;
            }
            if( bUpdateResetCntAttempt == true)
            {
                _MiscParam.u8MiscParam[RESET_COUNTER]++;
                _engMon.ReadEnergySetEnergyOffset(true);
                _engMon.StoreCummulativeCnt();
            }
        }

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
//                _objDisplay.printStringCenterAligned((char*)"Duplicate Password", FONT_VERDANA);
//                _objDisplay.gotoxy(GLCD_X(64),GLCD_Y( 50));
//                _objDisplay.printStringCenterAligned((char*)"Old password applied", FONT_VERDANA);
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

       AllParam.u8ArrParam[CFGZ::ID_DISP_VOLT_FILT_EN] = (uint8_t)ArrEditableItem[INDEX_OF_DISP_VOLT_FILT_EN].value.u8Val;
       AllParam.u8ArrParam[CFGZ::ID_DISP_VOLT_FILT_VAL] = (uint8_t)ArrEditableItem[INDEX_OF_DISP_VOLT_FILT_CONST].value.u8Val;

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
            case CFGZ::CFGZ_REMOTE_ALARM_MUTE:
            case CFGZ::CFGZ_REMOTE_ALARM_ACK:
            {
                menuItemsLowestLevel[u16SourceIndex + 2].isEnabled =false;
                menuItemsLowestLevel[u16SourceIndex + 3].isEnabled =false;
                menuItemsLowestLevel[u16SourceIndex + 4].isEnabled =false;
                TurnOFFActionActivation(u16SourceIndex);
            }
            break;

            case CFGZ::CFGZ_REMOTE_START_STOP:
            case CFGZ::CFGZ_STOP_PANEL_LOCK:
            case CFGZ::CFGZ_EXT_PANEL_LOCK:
            case CFGZ::CFGZ_GENERATOR_LOAD_INITHIBIT:
            case CFGZ::CFGZ_MAINS_LOAD_INHIBIT:
            {
                menuItemsLowestLevel[u16SourceIndex + 2].isEnabled =false;
                menuItemsLowestLevel[u16SourceIndex + 3].isEnabled =false;
               // TurnOFFActionActivation(u16SourceIndex);
            }
            break;


//todo TBD            case CFGZ::CFGZ_SOURCE_NEUTRAL_SW_SIGNAL:
            case CFGZ::CFGZ_SOURCE_REGENERATION_SW_INHIBIT_SIGNAL:
                menuItemsLowestLevel[u16SourceIndex + 2].isEnabled =false;
                menuItemsLowestLevel[u16SourceIndex + 3].isEnabled =false;
                menuItemsLowestLevel[u16SourceIndex + 4].isEnabled =false;
                if(!CFGC::IsSGC120())
                {
                    menuItemsLowestLevel[u16SourceIndex + 1].isEnabled =false;
                }
                TurnOFFActionActivation(u16SourceIndex);


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
    for(uint16_t i = INDEX_OF_DIG_OUT_A_SOURCE ; i<=INDEX_OF_DIG_OUT_G_SOURCE; i = i+2)
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

    bEn_Ds = (ArrEditableItem[INDEX_OF_COMM_MODE].value.u8Val == CFGZ::CFGZ_ENABLE );
    LowestLevelMenuEnDis(INDEX_OF_MODBUS_SLAVE_ID,INDEX_OF_MODBUS_PARITYBIT,bEn_Ds);

    bEn_Ds  = (ArrEditableItem[INDEX_OF_SITE_BATTERY_MON].value.u8Val ==  CFGZ::CFGZ_ENABLE );
    LowestLevelMenuEnDis(INDEX_OF_SITE_LOW_BATT_THRESH,INDEX_OF_SITE_GEN_RUN_DURATION,bEn_Ds);

    bEn_Ds  = (ArrEditableItem[INDEX_OF_CYCLIC_MODE].value.u8Val ==  CFGZ::CFGZ_ENABLE );
    LowestLevelMenuEnDis(INDEX_OF_CYCLIC_GEN_OFF_DURATION,INDEX_OF_CYCLIC_GEN_ON_DURATION,bEn_Ds);

    bEn_Ds  = (ArrEditableItem[INDEX_OF_EXERCISER_EVENT_1].value.u8Val ==  CFGZ::CFGZ_ENABLE );
    LowestLevelMenuEnDis(INDEX_OF_EXERCISER_EVENT_OCCURENCE_1,INDEX_OF_EXERCISER_LOAD_TRANSFER_1,bEn_Ds);

    if(ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_1].value.u8Val == 0)
    {
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].numOfStringFixedOptions = 1;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].formatString = "%s" ;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].stringFixedOptions = strOptions[_u8LanguageArrayIndex][ID_RUN_EVERY_DAY];
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].dataType = CEditableItem::DT_STRING_FIXED;
    }
    else if(ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_1].value.u8Val == 1)
    {
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].numOfStringFixedOptions = 7;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].formatString = "%s" ;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].stringFixedOptions = strOptions[_u8LanguageArrayIndex][ID_EVENT_DAY_OPTN];
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].dataType = CEditableItem::DT_STRING_FIXED;
    }
    else
    {
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].minVal.u16Val = 1;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].maxVal.u16Val = 28;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].formatString = "%u" ;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].numOfStringFixedOptions = 0;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].dataType = CEditableItem::DT_UINT8;

    }
    static uint8_t previousEvent1Occr = ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_1].value.u8Val;
    if(previousEvent1Occr != ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_1].value.u8Val)
    {
        if(ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_1].value.u8Val == 2)
        {
            ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].value.u8Val = 1;
            ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].tempValue.u8Val = 1;
        }
        else
        {
            ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].value.u8Val = 0;
            ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_1].tempValue.u8Val = 0;
        }

    }
    previousEvent1Occr = ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_1].value.u8Val;

    bEn_Ds  = (ArrEditableItem[INDEX_OF_EXERCISER_EVENT_2].value.u8Val ==  CFGZ::CFGZ_ENABLE );
    LowestLevelMenuEnDis(INDEX_OF_EXERCISER_EVENT_OCCURENCE_2,INDEX_OF_EXERCISER_LOAD_TRANSFER_2,bEn_Ds);


    if(ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_2].value.u8Val == 0)
    {
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].numOfStringFixedOptions = 1;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].formatString = "%s" ;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].stringFixedOptions = strOptions[_u8LanguageArrayIndex][ID_RUN_EVERY_DAY];
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].dataType = CEditableItem::DT_STRING_FIXED;
    }
    else if(ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_2].value.u8Val == 1)
    {
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].numOfStringFixedOptions = 7;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].formatString = "%s" ;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].stringFixedOptions = strOptions[_u8LanguageArrayIndex][ID_EVENT_DAY_OPTN];
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].dataType = CEditableItem::DT_STRING_FIXED;
    }
    else
    {
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].minVal.u16Val = 1;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].maxVal.u16Val = 28;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].formatString = "%u" ;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].numOfStringFixedOptions = 0;
        ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].dataType = CEditableItem::DT_UINT8;

    }
    static uint8_t previousEvent2Occr = ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_2].value.u8Val;

    if(previousEvent2Occr != ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_2].value.u8Val)
    {
        if(ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_2].value.u8Val == 2)
        {
            ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].value.u8Val = 1;
            ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].tempValue.u8Val = 1;
        }
        else
        {
            ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].value.u8Val = 0;
            ArrEditableItem[INDEX_OF_EXERCISER_EVENT_DAY_2].tempValue.u8Val = 0;
        }

    }
    previousEvent2Occr = ArrEditableItem[INDEX_OF_EXERCISER_EVENT_OCCURENCE_2].value.u8Val;

    bEn_Ds  = (ArrEditableItem[INDEX_OF_NIGHT_MODE_RESTRICT].value.u8Val ==  CFGZ::CFGZ_ENABLE );
    LowestLevelMenuEnDis(INDEX_OF_NIGHT_START_TIME,INDEX_OF_NIGHT_GEN_OFF_DURATION,bEn_Ds);

    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_A_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_B_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_C_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_D_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_E_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_F_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_G_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_H_SOURCE);
    DigitalInputMenuVisiblity(INDEX_OF_DIG_IN_I_SOURCE);

    static uint8_t PrevEngineType = ArrEditableItem[INDEX_OF_ENGINE_TYPE].value.u8Val;
    if(ArrEditableItem[INDEX_OF_ENGINE_TYPE].value.u8Val == CFGZ::ENG_CONVENTIONAL)
    {
        LowestLevelMenuEnDis(INDEX_OF_LOP_FROM_ECU,INDEX_OF_PROTECT_LAMP_ACT_DELAY, false);
        for(uint16_t i=INDEX_OF_LOP_FROM_ECU; i<=INDEX_OF_PROTECT_LAMP_ACT_DELAY; i++)
        {
            if((i != INDEX_OF_SGC_SOURCE_ADDRESS)
                    && (i != INDEX_OF_ECU_SOURCE_ADDRESS)
                    && (i != INDEX_OF_ENG_REQ_SPEED)
                    && (i !=  INDEX_OF_ENGINE_FRQ_ECU)
                    && (i != INDEX_OF_ENGINE_GAIN_ECU)
                    && (i != INDEX_OF_COMM_FAIL_ALARM_ACT_DELAY)
            )
            {
                ArrEditableItem[i].value.u8Val = 0;
                ArrEditableItem[i].tempValue.u8Val =0;
            }
        }
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_LOP_FROM_ECU,INDEX_OF_PROTECT_LAMP_ACT_DELAY, true);

        if((ArrEditableItem[INDEX_OF_ENGINE_TYPE].value.u8Val >= CFGZ::ENG_CUMMINS)&&(ArrEditableItem[INDEX_OF_ENGINE_TYPE].value.u8Val <= CFGZ::ENG_CUMMINS_2250))
         {

             menuItemsLowestLevel[INDEX_OF_ENGINE_FRQ_ECU].isEnabled = true;
             menuItemsLowestLevel[INDEX_OF_ENGINE_GAIN_ECU].isEnabled = true;

         }
         else
         {
             menuItemsLowestLevel[INDEX_OF_ENGINE_FRQ_ECU].isEnabled = false;
             menuItemsLowestLevel[INDEX_OF_ENGINE_GAIN_ECU].isEnabled = false;
         }
        //Todo: Assign Default Values to other parameters in ECU submenu
        if(PrevEngineType !=  ArrEditableItem[INDEX_OF_ENGINE_TYPE].tempValue.u8Val)
        {
            for(uint16_t i=INDEX_OF_LOP_FROM_ECU; i<=INDEX_OF_PREHEAT_CMD_ECU; i++)
            {
                if(i == INDEX_OF_BATT_VTG_FROM_ECU)
                {
                    ArrEditableItem[i].value.u8Val =0;
                    ArrEditableItem[i].tempValue.u8Val =0;
                }
                else if (i != INDEX_OF_ENG_REQ_SPEED)
                {
                    ArrEditableItem[i].value.u8Val =1;
                    ArrEditableItem[i].tempValue.u8Val =1;
                }
            }

            if((ArrEditableItem[INDEX_OF_ENGINE_TYPE].tempValue.u8Val >= CFGZ::ENG_CUMMINS)&&(ArrEditableItem[INDEX_OF_ENGINE_TYPE].tempValue.u8Val <= CFGZ::ENG_CUMMINS_2250))
            {
                ArrEditableItem[INDEX_OF_ENGINE_FRQ_ECU].value.u32Val =1;
                ArrEditableItem[INDEX_OF_ENGINE_FRQ_ECU].tempValue.u32Val =1;

            }
            else
            {
                ArrEditableItem[INDEX_OF_ENGINE_FRQ_ECU].value.u32Val = 0;
                ArrEditableItem[INDEX_OF_ENGINE_FRQ_ECU].tempValue.u32Val =0;
            }

            if((ArrEditableItem[INDEX_OF_ENGINE_TYPE].tempValue.u8Val >= CFGZ::ENG_CUMMINS)
                    &&(ArrEditableItem[INDEX_OF_ENGINE_TYPE].tempValue.u8Val <= CFGZ::ENG_CUMMINS_2250))
            {
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].value.u16Val = 220;
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].tempValue.u16Val = 220;
            }
            else if(ArrEditableItem[INDEX_OF_ENGINE_TYPE].tempValue.u8Val == CFGZ::ENG_SCANIA)
            {
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].value.u16Val = 39;
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].tempValue.u16Val = 39;
            }
            else if(ArrEditableItem[INDEX_OF_ENGINE_TYPE].tempValue.u8Val == CFGZ::ENG_MTU)
            {
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].value.u16Val = 234;
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].tempValue.u16Val = 234;
            }
            else if(ArrEditableItem[INDEX_OF_ENGINE_TYPE].tempValue.u8Val == CFGZ::ENG_VOLVO)
            {
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].value.u16Val = 17;
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].tempValue.u16Val = 17;
            }
            else if((ArrEditableItem[INDEX_OF_ENGINE_TYPE].tempValue.u8Val == CFGZ::ENG_DCEC_CUMMINS)
                    ||(ArrEditableItem[INDEX_OF_ENGINE_TYPE].tempValue.u8Val == CFGZ::ENG_PERKINS_ADAM4))
            {
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].value.u16Val = 0;
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].tempValue.u16Val = 0;
            }
            else
            {
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].value.u16Val = 3;
                ArrEditableItem[INDEX_OF_SGC_SOURCE_ADDRESS].tempValue.u16Val = 3;
            }

            ArrEditableItem[INDEX_OF_ECU_SOURCE_ADDRESS].value.u16Val = 0;
            ArrEditableItem[INDEX_OF_ECU_SOURCE_ADDRESS].tempValue.u16Val = 0;

        }
        menuItemsLowestLevel[INDEX_OF_ENG_REQ_SPEED].isEnabled = (ArrEditableItem[INDEX_OF_ENG_SPEED_TO_ECU].value.u8Val == CFGZ::CFGZ_ENABLE);
        bEn_Ds = (bool)(ArrEditableItem[INDEX_OF_COMM_FAIL_ALARM_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
        menuItemsLowestLevel[INDEX_OF_COMM_FAIL_ALARM_ACT].isEnabled =bEn_Ds;
        menuItemsLowestLevel[INDEX_OF_COMM_FAIL_ALARM_ACT_DELAY].isEnabled =bEn_Ds;

        bEn_Ds = (bool)(ArrEditableItem[INDEX_OF_AMBER_LAMP_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
        menuItemsLowestLevel[INDEX_OF_AMBER_LAMP_ACT].isEnabled =bEn_Ds;
        menuItemsLowestLevel[INDEX_OF_AMBER_LAMP_ACT_DELAY].isEnabled =bEn_Ds;

        bEn_Ds = (bool)(ArrEditableItem[INDEX_OF_RED_LAMP_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
        menuItemsLowestLevel[INDEX_OF_RED_LAMP_ACT].isEnabled =bEn_Ds;
        menuItemsLowestLevel[INDEX_OF_RED_LAMP_ACT_DELAY].isEnabled =bEn_Ds;

        bEn_Ds = (bool)(ArrEditableItem[INDEX_OF_MALFUNCTION_LAMP_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
        menuItemsLowestLevel[INDEX_OF_MALFUNCTION_LAMP_ACT].isEnabled =bEn_Ds;
        menuItemsLowestLevel[INDEX_OF_MALFUNCTION_LAMP_ACT_DELAY].isEnabled =bEn_Ds;

        bEn_Ds = (bool)(ArrEditableItem[INDEX_OF_PROTECT_LAMP_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
        menuItemsLowestLevel[INDEX_OF_PROTECT_LAMP_ACT].isEnabled =bEn_Ds;
        menuItemsLowestLevel[INDEX_OF_PROTECT_LAMP_ACT_DELAY].isEnabled =bEn_Ds;

    }
    PrevEngineType = ArrEditableItem[INDEX_OF_ENGINE_TYPE].value.u8Val;

    if(ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_J_SOURCE,INDEX_OF_LOP_SENS_V10,false);
    }
    else if(ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN)
    {
        LowestLevelMenuEnDis(INDEX_OF_LOP_CKT_FAULT_ACTION,INDEX_OF_LOP_SENS_V10,false);
        menuItemsLowestLevel[INDEX_OF_DIG_IN_J_SOURCE     ].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_DIG_IN_J_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_J_POLARITY,INDEX_OF_DIG_IN_J_DELAY,bEn_Ds);
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_LOP_CKT_FAULT_ACTION,INDEX_OF_LOP_SENS_V10,true);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_J_SOURCE,INDEX_OF_DIG_IN_J_DELAY, false);
    }

    if(ArrEditableItem[INDEX_OF_FUEL_DIG_K_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_K_SOURCE,INDEX_OF_FUEL_SENS_V10,false);
    }
    else if(ArrEditableItem[INDEX_OF_FUEL_DIG_K_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN)
    {
        LowestLevelMenuEnDis(INDEX_OF_LFL_SHUTDOWN_EN,INDEX_OF_FUEL_SENS_V10,false);
        menuItemsLowestLevel[INDEX_OF_DIG_IN_K_SOURCE].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_DIG_IN_K_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_K_POLARITY,INDEX_OF_DIG_IN_K_DELAY,bEn_Ds);
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_LFL_SHUTDOWN_EN,INDEX_OF_FUEL_SENS_V10,true);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_K_SOURCE,INDEX_OF_DIG_IN_K_DELAY, false);
        menuItemsLowestLevel[INDEX_OF_LFL_SHUTDOWN_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_LFL_SHUTDOWN_EN].value.u8Val == CFGZ::CFGZ_ENABLE );
        menuItemsLowestLevel[INDEX_OF_LFL_WARNING_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_LFL_WARNING_EN].value.u8Val == CFGZ::CFGZ_ENABLE );
        menuItemsLowestLevel[INDEX_OF_FUEL_THEFT_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_FUEL_THEFT_ALARM_EN].value.u8Val == CFGZ::CFGZ_ENABLE );
    }

    if(ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_L_SOURCE,INDEX_OF_ENG_CLNT_SENS_V10,false);
    }
    else if(ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN)
    {
        LowestLevelMenuEnDis(INDEX_OF_ENG_CLNT_CKT_FAULT_ACTION,INDEX_OF_ENG_CLNT_SENS_V10,false);
        menuItemsLowestLevel[INDEX_OF_DIG_IN_L_SOURCE].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_DIG_IN_L_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_L_POLARITY,INDEX_OF_DIG_IN_L_DELAY,bEn_Ds);
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_ENG_CLNT_CKT_FAULT_ACTION,INDEX_OF_ENG_CLNT_SENS_V10,true);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_L_SOURCE,INDEX_OF_DIG_IN_L_DELAY, false);
    }

//S1 Sensor
    if(ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_M_SOURCE,INDEX_OF_AUX_S1_SENS_V10,false);
    }
    else if(ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN)
    {
        LowestLevelMenuEnDis(INDEX_OF_AUX_S1_THRESHOLD_TYPE,INDEX_OF_AUX_S1_SENS_V10,false);
        menuItemsLowestLevel[INDEX_OF_DIG_IN_M_SOURCE].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_DIG_IN_M_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_M_POLARITY,INDEX_OF_DIG_IN_M_DELAY,bEn_Ds);
    }
    else if(ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_M_SOURCE,INDEX_OF_DIG_IN_M_DELAY, false);
        LowestLevelMenuEnDis(INDEX_OF_AUX_S1_THRESHOLD_TYPE,INDEX_OF_AUX_S1_CKT_FAULT_ACTION,true);
        ArrEditableItem[INDEX_OF_AUX_S1_CKT_FAULT_ACTION].u8PasswordLevel = CEditableItem::PIN1_PIN2_ALLOWED;

        menuItemsLowestLevel[INDEX_OF_AUX_S1_SHUTDOWN_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_AUX_S1_SHUTDOWN_EN].value.u8Val == CFGZ::CFGZ_ENABLE );
        menuItemsLowestLevel[INDEX_OF_AUX_S1_WARNING_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_AUX_S1_WARNING_EN].value.u8Val == CFGZ::CFGZ_ENABLE );

        LowestLevelMenuEnDis(INDEX_OF_SHELT_TEMP_THRESHOLD,INDEX_OF_SHELT_TEMP_RUN_DURATION,false);
        LowestLevelMenuEnDis(INDEX_OF_AUX_S1_SENS_R1,INDEX_OF_AUX_S1_SENS_V10,true);
        for(uint16_t i=INDEX_OF_AUX_S1_SENS_R1; i<INDEX_OF_AUX_S1_SENS_V10; i=i+2)
        {
            ArrEditableItem[i+1].unitOfMeasurement ="";
            ArrEditableItem[i+1].minVal.fVal = (float)0;
            ArrEditableItem[i+1].maxVal.fVal = (float)1000.0;
            ArrEditableItem[i+1].fValLC = (float)0.1;
            ArrEditableItem[i+1].dataType = CEditableItem:: DT_FLOAT;
        }
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_M_SOURCE,INDEX_OF_AUX_S1_WARNING_THRESHOLD, false);
        LowestLevelMenuEnDis(INDEX_OF_AUX_S1_CKT_FAULT_ACTION,INDEX_OF_AUX_S1_SENS_V10,true);
        ArrEditableItem[INDEX_OF_AUX_S1_CKT_FAULT_ACTION].value.u8Val = CFGZ::CFGZ_ACTION_NOTIFICATION; //Notification
        ArrEditableItem[INDEX_OF_AUX_S1_CKT_FAULT_ACTION].tempValue.u8Val = CFGZ::CFGZ_ACTION_NOTIFICATION;
        ArrEditableItem[INDEX_OF_AUX_S1_CKT_FAULT_ACTION].u8PasswordLevel = CEditableItem::NOT_ALLOWED;

        for(uint16_t i=INDEX_OF_AUX_S1_SENS_R1; i<INDEX_OF_AUX_S1_SENS_V10; i=i+2)
        {
            ArrEditableItem[i+1].unitOfMeasurement =arrUnit[ID_DEG_C];
            ArrEditableItem[i+1].minVal.i16Val = -25;
            ArrEditableItem[i+1].maxVal.i16Val = 300.0;
            ArrEditableItem[i+1].dataType = CEditableItem:: DT_INT16;
        }
    }

    // S2 Sensor
    if(ArrEditableItem[INDEX_OF_AUX_S2_DIG_N_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_N_SOURCE,INDEX_OF_AUX_S2_SENS_V10,false);
    }
    else if(ArrEditableItem[INDEX_OF_AUX_S2_DIG_N_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN)
    {
        LowestLevelMenuEnDis(INDEX_OF_AUX_S2_THRESHOLD_TYPE,INDEX_OF_AUX_S2_SENS_V10,false);
        menuItemsLowestLevel[INDEX_OF_DIG_IN_N_SOURCE].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_DIG_IN_N_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_N_POLARITY,INDEX_OF_DIG_IN_N_DELAY,bEn_Ds);
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_N_SOURCE,INDEX_OF_DIG_IN_N_DELAY, false);
        LowestLevelMenuEnDis(INDEX_OF_AUX_S2_THRESHOLD_TYPE,INDEX_OF_AUX_S2_SENS_V10,true);

        menuItemsLowestLevel[INDEX_OF_AUX_S2_SHUTDOWN_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_AUX_S2_SHUTDOWN_EN].value.u8Val == CFGZ::CFGZ_ENABLE );
        menuItemsLowestLevel[INDEX_OF_AUX_S2_WARNING_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_AUX_S2_WARNING_EN].value.u8Val == CFGZ::CFGZ_ENABLE );
    }

    // S3 Sensor

    if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_O_SOURCE,INDEX_OF_AUX_S3_SENS_VAL10,false);
    }
    else if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN)
    {
        LowestLevelMenuEnDis(INDEX_OF_AUX_S3_THRESHOLD_TYPE,INDEX_OF_AUX_S3_SENS_VAL10,false);
        menuItemsLowestLevel[INDEX_OF_DIG_IN_O_SOURCE].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_DIG_IN_O_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_O_POLARITY,INDEX_OF_DIG_IN_O_DELAY,bEn_Ds);
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_O_SOURCE,INDEX_OF_DIG_IN_O_DELAY, false);
        LowestLevelMenuEnDis(INDEX_OF_AUX_S3_CKT_FAULT_ACTION,INDEX_OF_AUX_S3_SENS_VAL10,true);

        menuItemsLowestLevel[INDEX_OF_AUX_S3_THRESHOLD_TYPE].isEnabled = (bool)(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val != CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3);
        menuItemsLowestLevel[INDEX_OF_AUX_S3_SHUTDOWN_EN].isEnabled = (bool)(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val != CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3);
        menuItemsLowestLevel[INDEX_OF_AUX_S3_WARNING_EN].isEnabled = (bool)(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val != CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3);

        menuItemsLowestLevel[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].isEnabled= (bool)((ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_EN].value.u8Val == CFGZ::CFGZ_ENABLE )
                                                                && (ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val != CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3));
        menuItemsLowestLevel[INDEX_OF_AUX_S3_WARNING_THRESHOLD].isEnabled= (bool)((ArrEditableItem[INDEX_OF_AUX_S3_WARNING_EN].value.u8Val == CFGZ::CFGZ_ENABLE )
                                                                 && (ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val != CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3));



        menuItemsLowestLevel[INDEX_OF_AUX_S3_WARNING_THRESHOLD].isEnabled= (bool)((ArrEditableItem[INDEX_OF_AUX_S3_WARNING_EN].value.u8Val == CFGZ::CFGZ_ENABLE )
                                                                 && (ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val != CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3));


        if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
        {
            ArrEditableItem[INDEX_OF_AUX_S3_CKT_FAULT_ACTION].stringFixedOptions = strOptions[_u8LanguageArrayIndex][ID_4_20MA_SENS_FAULT_ACTION];
            ArrEditableItem[INDEX_OF_AUX_S3_CKT_FAULT_ACTION].numOfStringFixedOptions = 4;
            for(uint16_t i=INDEX_OF_AUX_S3_SENS_I1_V1; i<INDEX_OF_AUX_S3_SENS_VAL10; i=i+2)
            {
                ArrEditableItem[i].unitOfMeasurement =arrUnit[ID_MILLI_AMPERE];
                ArrEditableItem[i].minVal.u8Val = 4;
                ArrEditableItem[i].maxVal.u8Val = 20;
                ArrEditableItem[i].dataType = CEditableItem:: DT_UINT8;
                ArrEditableItem[i+1].unitOfMeasurement ="";
                ArrEditableItem[i+1].minVal.fVal = (float)0;
                ArrEditableItem[i+1].maxVal.fVal = (float)1000;
                ArrEditableItem[i+1].fValLC = (float)0.1;
                ArrEditableItem[i+1].dataType = CEditableItem:: DT_FLOAT;
            }
        }
        else if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
        {
            ArrEditableItem[INDEX_OF_AUX_S3_CKT_FAULT_ACTION].stringFixedOptions = strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION];
            ArrEditableItem[INDEX_OF_AUX_S3_CKT_FAULT_ACTION].numOfStringFixedOptions = 5;
            for(uint16_t i=INDEX_OF_AUX_S3_SENS_I1_V1; i<INDEX_OF_AUX_S3_SENS_VAL10; i=i+2)
            {
                ArrEditableItem[i].unitOfMeasurement =arrUnit[ID_V];
                ArrEditableItem[i].minVal.fVal = (float)0;
                ArrEditableItem[i].maxVal.fVal = (float)5;
                ArrEditableItem[i].fValLC = (float)0.1;
                ArrEditableItem[i].dataType = CEditableItem:: DT_FLOAT;
                ArrEditableItem[i+1].unitOfMeasurement ="";
                ArrEditableItem[i+1].minVal.fVal = (float)0;
                ArrEditableItem[i+1].maxVal.fVal = (float)1000;
                ArrEditableItem[i+1].fValLC = (float)0.1;
                ArrEditableItem[i+1].dataType = CEditableItem:: DT_FLOAT;
            }
        }
        else
        {
            ArrEditableItem[INDEX_OF_AUX_S3_CKT_FAULT_ACTION].stringFixedOptions = strOptions[_u8LanguageArrayIndex][ID_4_20MA_SENS_FAULT_ACTION];
            ArrEditableItem[INDEX_OF_AUX_S3_CKT_FAULT_ACTION].numOfStringFixedOptions = 4;
            for(uint16_t i=INDEX_OF_AUX_S3_SENS_I1_V1; i<INDEX_OF_AUX_S3_SENS_VAL10; i=i+2)
            {
                ArrEditableItem[i].unitOfMeasurement =arrUnit[ID_MILLI_AMPERE];
                ArrEditableItem[i].minVal.u8Val = 4;
                ArrEditableItem[i].maxVal.u8Val = 20;
                ArrEditableItem[i].dataType = CEditableItem:: DT_UINT8;
                ArrEditableItem[i+1].unitOfMeasurement =arrUnit[ID_BAR];
                ArrEditableItem[i+1].minVal.fVal = (float)0;
                ArrEditableItem[i+1].maxVal.fVal = (float)10;
                ArrEditableItem[i+1].fValLC = (float)0.1;
                ArrEditableItem[i+1].dataType = CEditableItem:: DT_FLOAT;
            }
        }

        if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
        {
            ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].minVal.fVal=(float)0;
            ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].maxVal.fVal=(float)9.8;
            ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD].minVal.fVal=(float)0.2;
            ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD].maxVal.fVal=(float)10;
        }
        else
        {
            ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].minVal.fVal=(float)0;
            ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].maxVal.fVal=(float)1000;
            ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD].minVal.fVal=(float)0;
            ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD].maxVal.fVal=(float)1000;
        }

    }
    //S4 Sensor
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_SENSOR_NOT_USED)
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_P_SOURCE,INDEX_OF_AUX_S4_SENS_VAL10,false);
    }
    else if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_DIG_IN)
    {
        LowestLevelMenuEnDis(INDEX_OF_AUX_S4_THRESHOLD_TYPE,INDEX_OF_AUX_S4_SENS_VAL10,false);
        menuItemsLowestLevel[INDEX_OF_DIG_IN_P_SOURCE].isEnabled = true;
        bEn_Ds = (ArrEditableItem[INDEX_OF_DIG_IN_P_SOURCE].value.u8Val!=CFGZ::CFGZ_SENSOR_NOT_USED);
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_P_POLARITY,INDEX_OF_DIG_IN_P_DELAY,bEn_Ds);
    }
    else
    {
        LowestLevelMenuEnDis(INDEX_OF_DIG_IN_P_SOURCE,INDEX_OF_DIG_IN_P_DELAY, false);
        LowestLevelMenuEnDis(INDEX_OF_AUX_S4_THRESHOLD_TYPE,INDEX_OF_AUX_S4_SENS_VAL10,true);

        menuItemsLowestLevel[INDEX_OF_AUX_S4_SHUTDOWN_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_AUX_S4_SHUTDOWN_EN].value.u8Val == CFGZ::CFGZ_ENABLE );
        menuItemsLowestLevel[INDEX_OF_AUX_S4_WARNING_THRESHOLD].isEnabled= (bool)(ArrEditableItem[INDEX_OF_AUX_S4_WARNING_EN].value.u8Val == CFGZ::CFGZ_ENABLE );

        if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
        {
            ArrEditableItem[INDEX_OF_AUX_S4_CKT_FAULT_ACTION].stringFixedOptions = strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION];
            ArrEditableItem[INDEX_OF_AUX_S4_CKT_FAULT_ACTION].numOfStringFixedOptions = 5;
            for(uint16_t i=INDEX_OF_AUX_S4_SENS_I1_V1; i<INDEX_OF_AUX_S4_SENS_VAL10; i=i+2)
            {
                ArrEditableItem[i].unitOfMeasurement =arrUnit[ID_V];
                ArrEditableItem[i].minVal.fVal = (float)0;
                ArrEditableItem[i].maxVal.fVal = (float)5;
                ArrEditableItem[i].fValLC = (float)0.1;
                ArrEditableItem[i].dataType = CEditableItem:: DT_FLOAT;
            }
        }
        else
        {
            ArrEditableItem[INDEX_OF_AUX_S4_CKT_FAULT_ACTION].stringFixedOptions = strOptions[_u8LanguageArrayIndex][ID_4_20MA_SENS_FAULT_ACTION];
            ArrEditableItem[INDEX_OF_AUX_S4_CKT_FAULT_ACTION].numOfStringFixedOptions = 4;
            for(uint16_t i=INDEX_OF_AUX_S4_SENS_I1_V1; i<INDEX_OF_AUX_S4_SENS_VAL10; i=i+2)
            {
                ArrEditableItem[i].unitOfMeasurement =arrUnit[ID_MILLI_AMPERE];
                ArrEditableItem[i].minVal.u8Val = 4;
                ArrEditableItem[i].maxVal.u8Val = 20;
                ArrEditableItem[i].dataType = CEditableItem:: DT_UINT8;
            }
        }
    }

    ResetDigitalInputSource(INDEX_OF_LOP_DIG_J_SENSOR);
    ResetDigitalInputSource(INDEX_OF_FUEL_DIG_K_SENSOR);
    ResetDigitalInputSource(INDEX_OF_ENG_CLNT_DIG_L_SENSOR);
    ResetDigitalInputSource(INDEX_OF_AUX_S1_DIG_M_SENSOR);
    ResetDigitalInputSource(INDEX_OF_AUX_S2_DIG_N_SENSOR);
    ResetDigitalInputSource(INDEX_OF_AUX_S3_DIG_O_SENSOR);
    ResetDigitalInputSource(INDEX_OF_AUX_S4_DIG_P_SENSOR);

    //Digital O/P
    for(uint16_t i = INDEX_OF_DIG_OUT_A_SOURCE;i<=INDEX_OF_DIG_OUT_G_SOURCE;i=i+2)
    {
        DigitalOutputMenuVisiblity(i);
    }
//    DigitalOutputMenuVisiblity(INDEX_OF_DIG_OUT_A_SOURCE);
//    DigitalOutputMenuVisiblity(INDEX_OF_DIG_OUT_B_SOURCE);
//    DigitalOutputMenuVisiblity(INDEX_OF_DIG_OUT_C_SOURCE);
//    DigitalOutputMenuVisiblity(INDEX_OF_DIG_OUT_D_SOURCE);
//    DigitalOutputMenuVisiblity(INDEX_OF_DIG_OUT_E_SOURCE);
//    DigitalOutputMenuVisiblity(INDEX_OF_DIG_OUT_F_SOURCE);
//    DigitalOutputMenuVisiblity(INDEX_OF_DIG_OUT_G_SOURCE);

    //Generator
    if(ArrEditableItem[INDEX_OF_ALT_PRESENT].value.u8Val == CFGZ::CFGZ_DISABLE)
    {
        LowestLevelMenuEnDis(INDEX_OF_ALT_POLES,INDEX_OF_LOW_LOAD_DELAY,false);
        ArrMenu[ID_GENERATOR].numOfMenuItems = 1;
        ArrEditableItem[INDEX_OF_GEN_PT_ENABLE].value.u8Val = 0;
        ArrEditableItem[INDEX_OF_GEN_PT_ENABLE].tempValue.u8Val = 0;

    }
    else
    {
        ArrMenu[ID_GENERATOR].numOfMenuItems =  ID_MAINS_CONFIG-ID_ALT_CONFIG;
        LowestLevelMenuEnDis(INDEX_OF_ALT_POLES,INDEX_OF_LOW_LOAD_DELAY,true);

        if(ArrEditableItem[INDEX_OF_ALT_SYS].value.u8Val == CFGZ::CFGZ_3_PHASE_SYSTEM )
        {
            menuItemsLowestLevel[INDEX_OF_DG_PHASE_ROT_EN].isEnabled =true;
            menuItemsLowestLevel[INDEX_OF_DG_PHASE_ROT_ACTION].isEnabled =(ArrEditableItem[INDEX_OF_DG_PHASE_ROT_EN].value.u32Val == CFGZ::CFGZ_ENABLE);
        }
        else
        {
            menuItemsLowestLevel[INDEX_OF_DG_PHASE_ROT_EN].isEnabled =false;
            menuItemsLowestLevel[INDEX_OF_DG_PHASE_ROT_ACTION].isEnabled =false;
            ArrEditableItem[INDEX_OF_DG_PHASE_ROT_EN].value.u32Val = CFGZ::CFGZ_DISABLE;
            ArrEditableItem[INDEX_OF_DG_PHASE_ROT_EN].tempValue.u32Val = CFGZ::CFGZ_DISABLE;
        }

        if(ArrEditableItem[INDEX_OF_GEN_PT_ENABLE].value.u8Val == CFGZ::CFGZ_DISABLE)
        {
            menuItemsLowestLevel[INDEX_OF_GEN_PT_PRIMARY_RATIO].isEnabled =false;
            menuItemsLowestLevel[INDEX_OF_GEN_PT_SECONDARY_RATIO].isEnabled =false;
        }
        else
        {
            menuItemsLowestLevel[INDEX_OF_GEN_PT_PRIMARY_RATIO].isEnabled =true;
            menuItemsLowestLevel[INDEX_OF_GEN_PT_SECONDARY_RATIO].isEnabled =true;
        }

        MenuVisibilityOfGenThreshAndDly(INDEX_OF_UV_SHUT_EN);
        MenuVisibilityOfGenThreshAndDly(INDEX_OF_UV_WARN_EN);

        MenuVisibilityOfGenThreshAndDly(INDEX_OF_OV_SHUT_EN);
        MenuVisibilityOfGenThreshAndDly(INDEX_OF_OV_WARN_EN);

        MenuVisibilityOfGenThreshAndDly(INDEX_OF_UF_SHUT_EN);
        MenuVisibilityOfGenThreshAndDly(INDEX_OF_UF_WARN_EN);

        MenuVisibilityOfGenThreshAndDly(INDEX_OF_OF_SHUT_EN);
        MenuVisibilityOfGenThreshAndDly(INDEX_OF_OF_WARN_EN);

        menuItemsLowestLevel[INDEX_OF_CM_OC_THRESHOLD ].isEnabled =  (ArrEditableItem[INDEX_OF_CM_OC_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
        menuItemsLowestLevel[INDEX_OF_CM_OC_DELAY ].isEnabled =    (ArrEditableItem[INDEX_OF_CM_OC_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);

       if((ArrEditableItem[INDEX_OF_EFM_CURRENT].value.u8Val == CFGZ::CFGZ_EARTH_LEAKAGE_CURR))
       {
           menuItemsLowestLevel[INDEX_OF_EFM_LOW_CURR_ACTION].isEnabled =false;
           menuItemsLowestLevel[INDEX_OF_EFM_LOW_CURR_THRESH].isEnabled =false;

           menuItemsLowestLevel[INDEX_OF_EFM_HIGH_CURR_THRESH].isEnabled = (ArrEditableItem[INDEX_OF_EFM_HIGH_CURR_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
           menuItemsLowestLevel[INDEX_OF_EFM_CURR_DELAY].isEnabled = (ArrEditableItem[INDEX_OF_EFM_HIGH_CURR_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
       }
       else
       {
           menuItemsLowestLevel[INDEX_OF_EFM_HIGH_CURR_THRESH].isEnabled = (ArrEditableItem[INDEX_OF_EFM_HIGH_CURR_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
           menuItemsLowestLevel[INDEX_OF_EFM_LOW_CURR_THRESH].isEnabled =  (ArrEditableItem[INDEX_OF_EFM_LOW_CURR_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);

           if((ArrEditableItem[INDEX_OF_EFM_HIGH_CURR_ACTION].value.u8Val> CFGZ::CFGZ_ACTION_NONE)
                   ||(ArrEditableItem[INDEX_OF_EFM_LOW_CURR_ACTION].value.u8Val> CFGZ::CFGZ_ACTION_NONE))
           {
               menuItemsLowestLevel[INDEX_OF_EFM_CURR_DELAY ].isEnabled = true;
           }
           else
           {
               menuItemsLowestLevel[INDEX_OF_EFM_CURR_DELAY ].isEnabled = false;
           }
       }

        menuItemsLowestLevel[INDEX_OF_LM_THRESHOLD].isEnabled = (ArrEditableItem[INDEX_OF_LM_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
        menuItemsLowestLevel[INDEX_OF_LM_DELAY].isEnabled = (ArrEditableItem[INDEX_OF_LM_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);

        menuItemsLowestLevel[INDEX_OF_LM_UNBALANCE_TH].isEnabled = (ArrEditableItem[INDEX_OF_LM_UNBALANCE_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
        menuItemsLowestLevel[INDEX_OF_LM_UNBALANCE_DELAY].isEnabled = (ArrEditableItem[INDEX_OF_LM_UNBALANCE_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);




        if(ArrEditableItem[INDEX_OF_GEN_PT_ENABLE].value.u8Val == CFGZ::CFGZ_DISABLE)
        {
            menuItemsLowestLevel[INDEX_OF_GEN_PT_PRIMARY_RATIO].isEnabled = false;
            menuItemsLowestLevel[INDEX_OF_GEN_PT_SECONDARY_RATIO].isEnabled = false;

        }
        else
        {
            menuItemsLowestLevel[INDEX_OF_GEN_PT_PRIMARY_RATIO].isEnabled = true;
            menuItemsLowestLevel[INDEX_OF_GEN_PT_SECONDARY_RATIO].isEnabled = true;
        }

        if(ArrEditableItem[INDEX_OF_LOW_LOAD_EN].value.u8Val == 0)
        {
            menuItemsLowestLevel[ INDEX_OF_LOW_LOAD_ACTION].isEnabled = false;
            menuItemsLowestLevel[ INDEX_OF_LOW_LOAD_TRIP  ].isEnabled = false;
            menuItemsLowestLevel[ INDEX_OF_LOW_LOAD_RETURN].isEnabled = false;
            menuItemsLowestLevel[ INDEX_OF_LOW_LOAD_DELAY ].isEnabled = false;
        }
        else
        {
            if(ArrEditableItem[INDEX_OF_LOW_LOAD_ACTION].value.u8Val == CFGZ::CFGZ_ACTION_NONE )
            {
                menuItemsLowestLevel[ INDEX_OF_LOW_LOAD_TRIP  ].isEnabled = false;
                menuItemsLowestLevel[ INDEX_OF_LOW_LOAD_RETURN].isEnabled = false;
                menuItemsLowestLevel[ INDEX_OF_LOW_LOAD_DELAY ].isEnabled = false;
            }
        }

    }
    //Mains

        if(ArrEditableItem[INDEX_OF_MAINS_MON_ON].value.u8Val == CFGZ::CFGZ_DISABLE )
        {
            LowestLevelMenuEnDis(INDEX_OF_MAINS_SYSTEM,INDEX_OF_MAINS_FREQ_TRIP_DELAY,false);
            ArrMenu[ID_MAINS].numOfMenuItems = 1;
            ArrEditableItem[INDEX_OF_MAINS_PT_ENABLE].value.u8Val = 0;
            ArrEditableItem[INDEX_OF_MAINS_PT_ENABLE].tempValue.u8Val = 0;

        }
        else
        {
            ArrMenu[ID_MAINS].numOfMenuItems = ID_CRANK_DISCON-ID_MAINS_CONFIG;
            LowestLevelMenuEnDis(INDEX_OF_MAINS_SYSTEM,INDEX_OF_MAINS_FREQ_TRIP_DELAY, true);

            if(ArrEditableItem[INDEX_OF_MAINS_SYSTEM].value.u8Val == CFGZ::CFGZ_3_PHASE_SYSTEM )
            {
                  menuItemsLowestLevel[INDEX_OF_MAINS_PHASE_ROT_EN].isEnabled =true;
                  menuItemsLowestLevel[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].isEnabled = true;
                  ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN].u8PasswordLevel =  CEditableItem::PIN1_PIN2_ALLOWED ;
                  ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].u8PasswordLevel =  CEditableItem::PIN1_PIN2_ALLOWED ;


                  if((ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN].value.u32Val == CFGZ::CFGZ_ENABLE))
                  {
                      ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].value.u8Val = 0;
                      ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].tempValue.u8Val = 0;
                      ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].u8PasswordLevel = CEditableItem::NOT_ALLOWED;
//                      menuItemsLowestLevel[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].isEnabled = false;
                      menuItemsLowestLevel[INDEX_OF_MAINS_PHASE_ROT_ACTION].isEnabled =true;
                  }
                  else
                  {
                      menuItemsLowestLevel[INDEX_OF_MAINS_PHASE_ROT_ACTION].isEnabled =false;
//                      menuItemsLowestLevel[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].isEnabled = true;
                      ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].u8PasswordLevel =  CEditableItem::PIN1_PIN2_ALLOWED ;
                  }

                  if((ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].value.u32Val == CFGZ::CFGZ_ENABLE))
                  {
                      ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN].value.u32Val =0;
                      ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN].tempValue.u32Val =0;
//                      menuItemsLowestLevel[INDEX_OF_MAINS_PHASE_ROT_EN].isEnabled =false;
                      ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN].u8PasswordLevel = CEditableItem::NOT_ALLOWED;
                  }
                  else
                  {
//                      menuItemsLowestLevel[INDEX_OF_MAINS_PHASE_ROT_EN].isEnabled =true;
                      ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN].u8PasswordLevel =  CEditableItem::PIN1_PIN2_ALLOWED ;
                  }
            }
            else if(ArrEditableItem[INDEX_OF_MAINS_SYSTEM].value.u8Val == CFGZ::CFGZ_SPLIT_PHASE )
            {
                ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN].value.u32Val =0;
                ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN].tempValue.u32Val =0;
                menuItemsLowestLevel[INDEX_OF_MAINS_PHASE_ROT_EN].isEnabled =false;
                menuItemsLowestLevel[INDEX_OF_MAINS_PHASE_ROT_ACTION].isEnabled =false;
                menuItemsLowestLevel[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].isEnabled = true;
                ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].u8PasswordLevel =  CEditableItem::PIN1_PIN2_ALLOWED ;
            }
            else
            {
                ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN].value.u32Val =0;
                ArrEditableItem[INDEX_OF_MAINS_PHASE_ROT_EN].tempValue.u32Val =0;
                menuItemsLowestLevel[INDEX_OF_MAINS_PHASE_ROT_EN].isEnabled =false;
                menuItemsLowestLevel[INDEX_OF_MAINS_PHASE_ROT_ACTION].isEnabled =false;
                menuItemsLowestLevel[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].isEnabled =false;
                ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].value.u32Val =0;
                ArrEditableItem[INDEX_OF_MAINS_PARTIAL_HEALTHY_DETECT].tempValue.u32Val =0;
            }
            if(ArrEditableItem[INDEX_OF_MAINS_PT_ENABLE].value.u8Val == CFGZ::CFGZ_DISABLE)
            {
                menuItemsLowestLevel[INDEX_OF_MAINS_PT_PRIMARY_RATIO].isEnabled =false;
                menuItemsLowestLevel[INDEX_OF_MAINS_PT_SECONDARY_RATIO].isEnabled =false;
            }
            else
            {
                menuItemsLowestLevel[INDEX_OF_MAINS_PT_PRIMARY_RATIO].isEnabled =true;
                menuItemsLowestLevel[INDEX_OF_MAINS_PT_SECONDARY_RATIO].isEnabled =true;
            }

            MenuVisiblituyOfMainsThresh(INDEX_OF_MAINS_UV_EN);
            MenuVisiblituyOfMainsThresh(INDEX_OF_MAINS_OV_EN);
            MenuVisiblituyOfMainsThresh(INDEX_OF_MAINS_UF_EN);
            MenuVisiblituyOfMainsThresh(INDEX_OF_MAINS_OF_EN);

            if((ArrEditableItem[INDEX_OF_MAINS_UV_EN].value.u8Val == CFGZ::CFGZ_ENABLE )
                    ||(ArrEditableItem[INDEX_OF_MAINS_OV_EN].value.u8Val == CFGZ::CFGZ_ENABLE))
            {
                menuItemsLowestLevel[INDEX_OF_MAINS_VOLT_TRIP_DELAY].isEnabled = true;

            }
            else
            {
                menuItemsLowestLevel[INDEX_OF_MAINS_VOLT_TRIP_DELAY].isEnabled = false;
            }


            if((ArrEditableItem[INDEX_OF_MAINS_UF_EN].value.u8Val == CFGZ::CFGZ_ENABLE )
                    ||(ArrEditableItem[INDEX_OF_MAINS_OF_EN].value.u8Val == CFGZ::CFGZ_ENABLE))
            {
                menuItemsLowestLevel[INDEX_OF_MAINS_FREQ_TRIP_DELAY].isEnabled = true;

            }
            else
            {
                menuItemsLowestLevel[INDEX_OF_MAINS_FREQ_TRIP_DELAY].isEnabled = false;
            }

            if(ArrEditableItem[INDEX_OF_MAINS_PT_ENABLE].value.u8Val == CFGZ::CFGZ_DISABLE)
            {
                menuItemsLowestLevel[INDEX_OF_MAINS_PT_PRIMARY_RATIO].isEnabled = false;
                menuItemsLowestLevel[INDEX_OF_MAINS_PT_SECONDARY_RATIO].isEnabled = false;
            }
            else
            {
                menuItemsLowestLevel[INDEX_OF_MAINS_PT_PRIMARY_RATIO].isEnabled = true;
                menuItemsLowestLevel[INDEX_OF_MAINS_PT_SECONDARY_RATIO].isEnabled = true;
            }
        }

    if((ArrEditableItem[INDEX_OF_START_USE_LOP].value.u8Val == CFGZ::CFGZ_DISABLE)
        && (ArrEditableItem[INDEX_OF_START_LOP_AT_CRANK].value.u8Val == CFGZ::CFGZ_DISABLE))
    {
        menuItemsLowestLevel[INDEX_OF_START_LOP_THRESHOLD].isEnabled =false;
    }
    else
    {
        menuItemsLowestLevel[INDEX_OF_START_LOP_THRESHOLD].isEnabled =true;
    }

    menuItemsLowestLevel[INDEX_OF_START_LLOP_DELAY].isEnabled = (ArrEditableItem[INDEX_OF_START_USE_LLOP].value.u8Val == CFGZ::CFGZ_ENABLE);

    menuItemsLowestLevel[INDEX_OF_START_CA_THRESHOLD].isEnabled = (ArrEditableItem[INDEX_OF_START_USE_CA_VOLT].value.u8Val == CFGZ::CFGZ_ENABLE);

    if(ArrEditableItem[INDEX_OF_ENG_SPEED_FROM_ECU].value.u8Val == 1)
    {
        ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE].value.u8Val = CFGZ::CFGZ_ALT_FREQUENCY;
        ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE].tempValue.u8Val = CFGZ::CFGZ_ALT_FREQUENCY;
        ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE].u8PasswordLevel = CEditableItem::NOT_ALLOWED;
    }
    else
    {
        ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE].u8PasswordLevel = CEditableItem::PIN1_PIN2_PIN3_ALLOWED;
    }

    static uint8_t u8PrevSpeedSenseSource = ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE].value.u8Val;
    if(ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE].value.u8Val == CFGZ::CFGZ_ALT_FREQUENCY)
    {
        menuItemsLowestLevel[INDEX_OF_MPU_TEETH_W_PT_FREQ].isEnabled =false;
    }
    else
    {
        menuItemsLowestLevel[INDEX_OF_MPU_TEETH_W_PT_FREQ].isEnabled =true;
        if(ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE].value.u8Val!= u8PrevSpeedSenseSource)
        {
          if(ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE].value.u8Val == CFGZ::CFGZ_MAGNETIC_PICKUP)
          {
              ArrEditableItem[INDEX_OF_MPU_TEETH_W_PT_FREQ].minVal.u16Val =  1;
              ArrEditableItem[INDEX_OF_MPU_TEETH_W_PT_FREQ].maxVal.u16Val =  300;
              ArrEditableItem[INDEX_OF_MPU_TEETH_W_PT_FREQ].value.u16Val =  110;
              ArrEditableItem[INDEX_OF_MPU_TEETH_W_PT_FREQ].tempValue.u16Val =  110;
          }
          else if(ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE].value.u8Val == CFGZ::CFGZ_W_POINT_FREQ)
          {
              ArrEditableItem[INDEX_OF_MPU_TEETH_W_PT_FREQ].minVal.u16Val =  0;
              ArrEditableItem[INDEX_OF_MPU_TEETH_W_PT_FREQ].maxVal.u16Val =  500;
              ArrEditableItem[INDEX_OF_MPU_TEETH_W_PT_FREQ].value.u16Val =  274;
              ArrEditableItem[INDEX_OF_MPU_TEETH_W_PT_FREQ].tempValue.u16Val =  274;
          }
        }
    }
    u8PrevSpeedSenseSource = ArrEditableItem[INDEX_OF_SPEED_SENSE_SOURCE].value.u8Val;
    bEn_Ds = (ArrEditableItem[INDEX_OF_US_EN].value.u8Val == CFGZ::CFGZ_ENABLE);
    menuItemsLowestLevel[INDEX_OF_US_THRESHOLD].isEnabled =bEn_Ds;
    menuItemsLowestLevel[INDEX_OF_US_DELAY].isEnabled =bEn_Ds;

    bEn_Ds = (bool)(ArrEditableItem[INDEX_OF_LOW_VBAT_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
    menuItemsLowestLevel[INDEX_OF_LOW_VBAT_THRESHOLD].isEnabled =bEn_Ds;
    menuItemsLowestLevel[INDEX_OF_LOW_VBAT_DELAY].isEnabled =bEn_Ds;

    bEn_Ds = (bool)(ArrEditableItem[INDEX_OF_HIGH_VBAT_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
    menuItemsLowestLevel[INDEX_OF_HIGH_VBAT_THRESHOLD].isEnabled =bEn_Ds;
    menuItemsLowestLevel[INDEX_OF_HIGH_VBAT_DELAY].isEnabled =bEn_Ds;

    bEn_Ds = (bool)(ArrEditableItem[INDEX_OF_CA_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE);
    menuItemsLowestLevel[INDEX_OF_CA_THRESHOLD].isEnabled =bEn_Ds;
    menuItemsLowestLevel[INDEX_OF_CA_DELAY].isEnabled =bEn_Ds;


    if(IsOutputConfigured(CFGZ::CFGZ_PREHEAT) || (ArrEditableItem[INDEX_OF_PREHEAT_CMD_ECU].value.u8Val == CFGZ::CFGZ_ENABLE ))
    {
        menuItemsMidLevel[ID_PREHEAT].isEnabled = true;
        menuItemsLowestLevel[INDEX_OF_PH_TIMER].isEnabled = true;
        menuItemsLowestLevel[INDEX_OF_PH_TEMP_EN].isEnabled = ((ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)||(ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU].value.u8Val ==CFGZ::CFGZ_ENABLE)) ;
        menuItemsLowestLevel[INDEX_OF_PH_TEMP_VALUE].isEnabled = (menuItemsLowestLevel[INDEX_OF_PH_TEMP_EN].isEnabled)&&(ArrEditableItem[INDEX_OF_PH_TEMP_EN].value.u8Val == CFGZ::CFGZ_ENABLE);

    }
    else
    {
        menuItemsLowestLevel[INDEX_OF_PH_TIMER].isEnabled =false;
        menuItemsLowestLevel[INDEX_OF_PH_TEMP_EN].isEnabled =false;
        menuItemsLowestLevel[INDEX_OF_PH_TEMP_VALUE].isEnabled =false;
        menuItemsMidLevel[ID_PREHEAT].isEnabled = false;
        ArrEditableItem[INDEX_OF_PH_TEMP_EN].value.u8Val = 0;
        ArrEditableItem[INDEX_OF_PH_TEMP_EN].tempValue.u8Val = 0;
    }

    if(!IsOutputConfigured(CFGZ::CFGZ_CLNT_TEMP_CTRL))
    {
        menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_CTRL_EN].isEnabled =false;
        menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_CTRL].isEnabled =false;
        menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_ON_THRESH].isEnabled =false;
        menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_OFF_THRESH].isEnabled =false;
        menuItemsMidLevel[ID_CLNT_TEMP_CONTROL].isEnabled = false;
    }
    else
    {
        menuItemsMidLevel[ID_CLNT_TEMP_CONTROL].isEnabled = (ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                                    ||(ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU].value.u8Val == CFGZ::CFGZ_ENABLE);
        menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_CTRL_EN].isEnabled = menuItemsMidLevel[ID_CLNT_TEMP_CONTROL].isEnabled;
        menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_CTRL].isEnabled = (menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_CTRL_EN].isEnabled)&&(ArrEditableItem[INDEX_OF_CLNT_TEMP_CTRL_EN].value.u8Val == CFGZ::CFGZ_ENABLE);
        menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_ON_THRESH].isEnabled = (menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_CTRL_EN].isEnabled)&&(ArrEditableItem[INDEX_OF_CLNT_TEMP_CTRL_EN].value.u8Val == CFGZ::CFGZ_ENABLE);
        menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_OFF_THRESH].isEnabled =(menuItemsLowestLevel[INDEX_OF_CLNT_TEMP_CTRL_EN].isEnabled)&&(ArrEditableItem[INDEX_OF_CLNT_TEMP_CTRL_EN].value.u8Val == CFGZ::CFGZ_ENABLE);
    }


    if(ArrEditableItem[INDEX_OF_LOP_FROM_ECU].value.u8Val)
    {
        ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].numOfStringFixedOptions = 2;
        ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].numOfStringFixedOptions = 4;
        if(ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].value.u32Val == CFGZ:: CFGZ_ANLG_CUSTOM_SENSOR1)
        {
            ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].value.u32Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
            ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].tempValue.u32Val =  CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
        }
        if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u32Val ==CFGZ:: CFGZ_ANLG_CUSTOM_SENSOR3)
        {
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u32Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].tempValue.u32Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
        }
    }
    else
    {
        ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].numOfStringFixedOptions = 3;
        ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].numOfStringFixedOptions = 5;
    }


    menuItemsMidLevel[ID_LOP].isEnabled = ((ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                        ||(ArrEditableItem[INDEX_OF_LOP_FROM_ECU].value.u8Val == CFGZ::CFGZ_ENABLE) || (ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3) );

    menuItemsLowestLevel[INDEX_OF_LOP_SHUTDOWN_EN].isEnabled = menuItemsMidLevel[ID_LOP].isEnabled;
    menuItemsLowestLevel[INDEX_OF_LOP_WARNING_EN].isEnabled = menuItemsMidLevel[ID_LOP].isEnabled;

    menuItemsLowestLevel[INDEX_OF_LOP_SHUTDOWN_THRESHOLD].isEnabled = (menuItemsLowestLevel[INDEX_OF_LOP_SHUTDOWN_EN].isEnabled)&&(ArrEditableItem[INDEX_OF_LOP_SHUTDOWN_EN].value.u8Val == CFGZ::CFGZ_ENABLE);
    menuItemsLowestLevel[INDEX_OF_LOP_WARNING_THRESHOLD].isEnabled = (menuItemsLowestLevel[INDEX_OF_LOP_WARNING_EN].isEnabled)&&(ArrEditableItem[INDEX_OF_LOP_WARNING_EN].value.u8Val == CFGZ::CFGZ_ENABLE);


    if(ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU].value.u8Val)
    {
        ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR].numOfStringFixedOptions = 2;
        if(ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR].value.u32Val == CFGZ:: CFGZ_ANLG_CUSTOM_SENSOR1)
        {
            ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR].value.u32Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
            ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR].tempValue.u32Val =  CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
        }
    }
    else
    {
        ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR].numOfStringFixedOptions = 3;
    }
    menuItemsMidLevel[ID_ENG_TEMP].isEnabled = (ArrEditableItem[INDEX_OF_ENG_CLNT_DIG_L_SENSOR].value.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                        ||(ArrEditableItem[INDEX_OF_COOLANT_TEMP_FROM_ECU].value.u8Val == CFGZ::CFGZ_ENABLE);

    menuItemsLowestLevel[INDEX_OF_COOLANT_TEMP_THRESH_TYPE].isEnabled = menuItemsMidLevel[ID_ENG_TEMP].isEnabled;
    menuItemsLowestLevel[INDEX_OF_ECT_SHUTDOWN_EN].isEnabled = menuItemsMidLevel[ID_ENG_TEMP].isEnabled;
    menuItemsLowestLevel[INDEX_OF_ECT_WARNING_EN].isEnabled = menuItemsMidLevel[ID_ENG_TEMP].isEnabled;

    menuItemsLowestLevel[INDEX_OF_ECT_SHUTDOWN_THRESHOLD].isEnabled = (menuItemsLowestLevel[INDEX_OF_ECT_SHUTDOWN_EN].isEnabled)&&(ArrEditableItem[INDEX_OF_ECT_SHUTDOWN_EN].value.u8Val == CFGZ::CFGZ_ENABLE);
    menuItemsLowestLevel[INDEX_OF_ECT_WARNING_THRESHOLD].isEnabled = (menuItemsLowestLevel[INDEX_OF_ECT_WARNING_EN].isEnabled)&&(ArrEditableItem[INDEX_OF_ECT_WARNING_EN].value.u8Val == CFGZ::CFGZ_ENABLE);


    if(ArrEditableItem[INDEX_OF_FILT_MAINT_ACTION].value.u8Val > CFGZ::CFGZ_ACTION_NONE)
    {
        ArrEditableItem[INDEX_OF_FILT_MAINT_DUE_DATE].u8PasswordLevel = CEditableItem::PIN1_PIN2_ALLOWED;
        bEn_Ds = true;
    }
    else
    {
        bEn_Ds = false;
        ArrEditableItem[INDEX_OF_FILT_MAINT_DUE_DATE].u8PasswordLevel = CEditableItem::NOT_ALLOWED;
    }
    menuItemsLowestLevel[INDEX_OF_FILT_MAINT_THRESHOLD ].isEnabled =bEn_Ds;

    if(ArrEditableItem[INDEX_OF_ENGINE_TYPE].value.u8Val == CFGZ::ENG_KUBOTA)
    {
        menuItemsLowestLevel[INDEX_OF_FILT_MAINT_ASH_LOAD_EN ].isEnabled =true;
    }
    else
    {
        menuItemsLowestLevel[INDEX_OF_FILT_MAINT_ASH_LOAD_EN ].isEnabled =false;
    }

    if(ArrEditableItem[INDEX_OF_EGOV_ACT_APPLICATION].value.u8Val == CFGZ::CFGZ_AS_EGOV)
    {
        ArrMenu[ID_ROTARY_ACTUATOR].numOfMenuItems =  3;
        menuItemsMidLevel[ID_GEN_EGOV_CNFG].isEnabled = true;
        menuItemsMidLevel[ID_STR_STP_CNFG].isEnabled = false;
        menuItemsLowestLevel[INDEX_OF_EGOV_RUNNING_STEPS ].isEnabled = false;
        LowestLevelMenuEnDis(INDEX_OF_EGOV_SET_POINT_SELECTION,INDEX_OF_EGOV_GEN_UNLOADING_FAC, true);
        if(ArrEditableItem[INDEX_OF_EGOV_SET_POINT_SELECTION].value.u8Val == CFGZ::CFGZ_FIXED_SPEED)
        {
            menuItemsLowestLevel[INDEX_OF_EGOV_PERCENT_DROOP].isEnabled = false;
        }
    }
    else
    {
        ArrMenu[ID_ROTARY_ACTUATOR].numOfMenuItems =  4;
        menuItemsMidLevel[ID_GEN_EGOV_CNFG].isEnabled = false;
        menuItemsMidLevel[ID_STR_STP_CNFG].isEnabled = true;
        menuItemsLowestLevel[INDEX_OF_EGOV_RUNNING_STEPS ].isEnabled = true;
        LowestLevelMenuEnDis(INDEX_OF_EGOV_SET_POINT_SELECTION,INDEX_OF_EGOV_GEN_UNLOADING_FAC, false);
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
    bool bChineseSelected = (_objcfgz.GetCFGZ_Param(CFGZ::ID_LANGUAGE)==CFGZ::LANGUAGE_CHINSESE);
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

    if((ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].value.u8Val != ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].tempValue.u8Val))
    {
        if((ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].tempValue.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
        {
            ArrEditableItem[INDEX_OF_AUX_S1_CKT_FAULT_ACTION].value.u8Val = CFGZ::CFGZ_ACTION_NONE;
            ArrEditableItem[INDEX_OF_AUX_S1_CKT_FAULT_ACTION].tempValue.u8Val = CFGZ::CFGZ_ACTION_NONE;

            for(uint8_t i = 0; i<10; i++)
            {
                 ArrEditableItem[INDEX_OF_AUX_S1_SENS_R1 + (i*2)].value.u16Val = table_x_S1_sens[i];
                 ArrEditableItem[INDEX_OF_AUX_S1_SENS_R1 + (i*2)].tempValue.u16Val = table_x_S1_sens[i];
                 ArrEditableItem[INDEX_OF_AUX_S1_SENS_V1 + (i*2)].value.fVal = table_y_S1_sens[i];
                 ArrEditableItem[INDEX_OF_AUX_S1_SENS_V1 + (i*2)].tempValue.fVal = table_y_S1_sens[i];
            }
        }
        else if(ArrEditableItem[INDEX_OF_AUX_S1_DIG_M_SENSOR].tempValue.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
        {
          for(uint8_t i = 0; i<10; i++)
          {
               ArrEditableItem[INDEX_OF_AUX_S1_SENS_R1 + (i*2)].value.u16Val = table_x_Shelt_Temp[i];
               ArrEditableItem[INDEX_OF_AUX_S1_SENS_R1 + (i*2)].tempValue.u16Val = table_x_Shelt_Temp[i];
               ArrEditableItem[INDEX_OF_AUX_S1_SENS_V1 + (i*2)].value.i16Val = table_y_Shelt_Temp[i];
               ArrEditableItem[INDEX_OF_AUX_S1_SENS_V1 + (i*2)].tempValue.i16Val = table_y_Shelt_Temp[i];
          }
        }
    }
    //If LOP resistive is selected then disable the 4-20mA sensor if it is selected as LOP.
    if(ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].value.u8Val != ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].tempValue.u8Val)
    {
        if((ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
                &&(ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].tempValue.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
        {
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].tempValue.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
        }
    }
    //If LOP resistive is selected then disable the 4-20mA sensor if it is selected as LOP.
    if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val != ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].tempValue.u8Val)
    {
        if((ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].value.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                &&(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].tempValue.u8Val==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3))
        {
            ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].value.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
            ArrEditableItem[INDEX_OF_LOP_DIG_J_SENSOR].tempValue.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
        }
    }

    if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].tempValue.u8Val !=
            ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].value.u8Val)
    {
        ArrEditableItem[INDEX_OF_AUX_S3_CKT_FAULT_ACTION].value.u8Val = 0;
        ArrEditableItem[INDEX_OF_AUX_S3_CKT_FAULT_ACTION].tempValue.u8Val = 0;
        if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].tempValue.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
        {
            for(uint8_t i = 0; i<10; i++)
            {
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_I1_V1 + (i*2)].value.u8Val = table_x_4_20mA_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_I1_V1 + (i*2)].tempValue.u8Val = table_x_4_20mA_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL1 + (i*2)].value.fVal = table_y_4_20mA_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL1 + (i*2)].tempValue.fVal = table_y_4_20mA_Sens[i];

            }
            ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].value.fVal = 9.8f;
            ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].tempValue.fVal = 9.8f;
            ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD].value.fVal = 10.0f;
            ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD].tempValue.fVal = 10.0f;
        }
        else if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].tempValue.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
        {
            for(uint8_t i = 0; i<10; i++)
            {
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_I1_V1 + (i*2)].value.fVal = table_x_0to5V_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_I1_V1 + (i*2)].tempValue.fVal = table_x_0to5V_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL1 + (i*2)].value.fVal = table_y_0to5V_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL1 + (i*2)].tempValue.fVal = table_y_0to5V_Sens[i];
            }

            ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].value.fVal = 9.8f;
            ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].tempValue.fVal = 9.8f;
            ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD].value.fVal = 10.0f;
            ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD].tempValue.fVal = 10.0f;
        }
        else if(ArrEditableItem[INDEX_OF_AUX_S3_DIG_O_SENSOR].tempValue.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
        {

            for(uint8_t i = 0; i<10; i++)
            {
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_I1_V1 + (i*2)].value.u8Val = table_x_4_20mA_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_I1_V1 + (i*2)].tempValue.u8Val = table_x_4_20mA_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL1 + (i*2)].value.fVal = table_y_LOP_Curr_sens[i];
                ArrEditableItem[INDEX_OF_AUX_S3_SENS_VAL1 + (i*2)].tempValue.fVal = table_y_LOP_Curr_sens[i];
            }
            ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].value.fVal = 2.0f;
            ArrEditableItem[INDEX_OF_AUX_S3_SHUTDOWN_THRESHOLD].tempValue.fVal = 2.0f;
            ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD].value.fVal = 3.0f;
            ArrEditableItem[INDEX_OF_AUX_S3_WARNING_THRESHOLD].tempValue.fVal = 3.0f;
        }
    }
    if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR].tempValue.u8Val !=
            ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR].value.u8Val)
    {
        ArrEditableItem[INDEX_OF_AUX_S4_CKT_FAULT_ACTION].value.u8Val = 0;
        ArrEditableItem[INDEX_OF_AUX_S4_CKT_FAULT_ACTION].tempValue.u8Val = 0;
        ArrEditableItem[INDEX_OF_AUX_S4_SHUTDOWN_THRESHOLD].value.fVal = 9.8f;
        ArrEditableItem[INDEX_OF_AUX_S4_SHUTDOWN_THRESHOLD].tempValue.fVal = 9.8f;
        ArrEditableItem[INDEX_OF_AUX_S4_WARNING_THRESHOLD].value.fVal = 10.0f;
        ArrEditableItem[INDEX_OF_AUX_S4_WARNING_THRESHOLD].tempValue.fVal = 10.0f;
        if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR].tempValue.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
        {
            for(uint8_t i = 0; i<10; i++)
            {
                ArrEditableItem[INDEX_OF_AUX_S4_SENS_I1_V1 + (i*2)].value.u8Val = table_x_4_20mA_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S4_SENS_I1_V1 + (i*2)].tempValue.u8Val = table_x_4_20mA_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL1 + (i*2)].value.fVal = table_y_4_20mA_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL1 + (i*2)].tempValue.fVal = table_y_4_20mA_Sens[i];
            }
        }
        else if(ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR].tempValue.u8Val == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
        {
            for(uint8_t i = 0; i<10; i++)
            {
                ArrEditableItem[INDEX_OF_AUX_S4_SENS_I1_V1 + (i*2)].value.fVal = table_x_0to5V_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S4_SENS_I1_V1 + (i*2)].tempValue.fVal = table_x_0to5V_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL1 + (i*2)].value.fVal = table_y_0to5V_Sens[i];
                ArrEditableItem[INDEX_OF_AUX_S4_SENS_VAL1 + (i*2)].tempValue.fVal = table_y_0to5V_Sens[i];
            }

        }
    }
    if((ArrEditableItem[INDEX_OF_CM_CT_RATIO].tempValue.u16Val !=ArrEditableItem[INDEX_OF_CM_CT_RATIO].value.u16Val)
            ||(ArrEditableItem[INDEX_OF_GEN_PT_PRIMARY_RATIO].tempValue.u16Val !=ArrEditableItem[INDEX_OF_GEN_PT_PRIMARY_RATIO].value.u16Val)
            ||(ArrEditableItem[INDEX_OF_GEN_PT_SECONDARY_RATIO].tempValue.u16Val !=ArrEditableItem[INDEX_OF_GEN_PT_SECONDARY_RATIO].value.u16Val)
            ||(ArrEditableItem[INDEX_OF_MAINS_PT_PRIMARY_RATIO].tempValue.u16Val !=ArrEditableItem[INDEX_OF_MAINS_PT_PRIMARY_RATIO].value.u16Val)
            ||(ArrEditableItem[INDEX_OF_MAINS_PT_SECONDARY_RATIO].tempValue.u16Val !=ArrEditableItem[INDEX_OF_MAINS_PT_SECONDARY_RATIO].value.u16Val))
    {
        _engMon.ReadEnergySetEnergyOffset(false);
        _engMon.StoreCummulativeCnt();
    }
    if(CFGC::IsSGC421())
    {
        if(ArrEditableItem[INDEX_OF_EGOV_SET_POINT_SELECTION].value.u8Val == EGOV::ID_PID_PARALLELING)
        {
            ArrEditableItem[INDEX_OF_AUX_S4_DIG_P_SENSOR].value.u8Val = CFGZ::CFGZ_ANLG_SENSOR_NOT_USED;
        }
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
