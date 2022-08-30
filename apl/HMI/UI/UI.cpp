/**
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
uint8_t ParamInSubmenus[ID_SUB_MENU_LAST] = {4,2,4,4,3,3,   5,5,5,5,5,5,5,5,5,31,35,29,31,30,31,22,    2,2,2,2,2,2,2,    4,14,    9,8,8,6,7,9,    5,3,3,3,3,    11,8,7,4,4,   2,3,        1,1,1,  6,4,  1,  2};
uint8_t SubmenusInMenus[ID_MAIN_MENU_LAST] = {6,16,7,2,6,5,5,2,  3,2,1,1};
uint8_t ParamInSubmenus1[72] = {4,2,4,4,3,3,6,2,4,4,3,6,6,3,5,5,5,5,5,5,5,5,5,27,35,27,36,32,32,32,2,2,2,2,2,2,2,4,7,11,12,12,12,5,7,7,5,8,7,7,11,13,6,3,3,4,4,9,20,4,5,3,1,3,4,10,1,3,6,4,1,2};
uint8_t SubmenusInMenus1[14] = {6,8,16,7,3,6,3,10,2,4,3,2,1,1};

//End
//All editable option string screens
enum
{
    //RushiStart
    ID_MANUAL_AUTO_S,
    ID_ENABLE_DISABLE_S,
    ID_MODBUS_S,
    ID_PARITY_S,
    ID_YES_NO_S,
    //RushiEnd
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
  //RushiStart
  {
        "Manual",
        "Auto"
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
      "No",
      "Si"
  },
   //RushiEnd
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
        //RushiStart
        "MODULE",
        "INPUTS",
        "OUTPUTS",
        "TIMERS",
        "GENERATOR",
        "MAINS",
        "ENGINE",
        "MAINTENANCE",
        //RushiEnd,
        "ID",
        "RESET PARAMETERS",
        "SELECT PROFILE",
        "DISP VOLT FILT"
    },
    {
     //RushiStart
     "MODULO",
     "ENTRADAS",
     "SALIDAS",
     "TEMPORIZADORES",
     "GENERADOR",
     "RED",
     "MOTOR",
     "MANTENIMIENTO",
     //RushiEnd
       	"ID",
       	"RESET PARAM",
        "SELEC PERFIL",
        "VER FLTR VOLT"
    }
};

static const char* strSubMenu[2][ID_SUB_MENU_LAST]
{
    {
        //RushiStart
        "GENERAL",
        "DISPLAY",
        "MODBUS_COMM",
        "BTS_CONFIG",
        "CYCLIC_CONFIG",
        "NIGHT_MODE_CONFIG",

        "ID_DIG_IN_A",
        "ID_DIG_IN_B",
        "ID_DIG_IN_C",
        "ID_DIG_IN_D",
        "ID_DIG_IN_E",
        "ID_DIG_IN_F",
        "ID_DIG_IN_G",
        "ID_DIG_IN_H",
        "ID_DIG_IN_I",
        "ID_LOP_RES_DIG_J",
        "ID_FUEL_LVL_DIG_K",
        "ID_ENG_TEMP_DIG_M",
        "ID_SHEL_TEMP_DIG_N",
        "ID_AUX_S2_RES_DIG_N",
        "ID_AUX_S3_DIG_O",
        "ID_AUX_S4_DIG_P",
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

       //Reset Parameters
         "GENSET",
         "MAINS",

       //Select Profile
         "SELECT PROFILE",

        //Disp Volt Filt
         "DISP VOLT FILT"
    },
    {
     //RushiStart
     "GENERAL",
     "PANTALLA",
     "COMUNICACION",
     "CONFIG BATERIA",
     "CONFIG CICLICA",
     "NIGHT_MODE_CONFIG",

     "ID_DIG_IN_A",
     "ID_DIG_IN_B",
     "ID_DIG_IN_C",
     "ID_DIG_IN_D",
     "ID_DIG_IN_E",
     "ID_DIG_IN_F",
     "ID_DIG_IN_G",
     "ID_DIG_IN_H",
     "ID_DIG_IN_I",
     "ID_LOP_RES_DIG_J",
     "ID_FUEL_LVL_DIG_K",
     "ID_ENG_TEMP_DIG_M",
     "ID_SHEL_TEMP_DIG_N",
     "ID_AUX_S2_RES_DIG_N",
     "ID_AUX_S3_DIG_O",
     "ID_AUX_S4_DIG_P",
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
        //RushiStart
        /*Module*/
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
        /*Inputs*/
        "SID_DIG_IN_A_SOURCE",
        "SID_DIG_IN_A_POLARITY",
        "SID_DIG_IN_A_ACTION",
        "SID_DIG_IN_A_ACTIVATION",
        "SID_DIG_IN_A_ACTIVATION_DELAY",

        "SID_DIG_IN_B_SOURCE",
        "SID_DIG_IN_B_POLARITY",
        "SID_DIG_IN_B_ACTION",
        "SID_DIG_IN_B_ACTIVATION",
        "SID_DIG_IN_B_ACTIVATION_DELAY",

        "SID_DIG_IN_C_SOURCE",
        "SID_DIG_IN_C_POLARITY",
        "SID_DIG_IN_C_ACTION",
        "SID_DIG_IN_C_ACTIVATION",
        "SID_DIG_IN_C_ACTIVATION_DELAY",

        "SID_DIG_IN_D_SOURCE",
        "SID_DIG_IN_D_POLARITY",
        "SID_DIG_IN_D_ACTION",
        "SID_DIG_IN_D_ACTIVATION",
        "SID_DIG_IN_D_ACTIVATION_DELAY",

        "SID_DIG_IN_E_SOURCE",
        "SID_DIG_IN_E_POLARITY",
        "SID_DIG_IN_E_ACTION",
        "SID_DIG_IN_E_ACTIVATION",
        "SID_DIG_IN_E_ACTIVATION_DELAY",

        "SID_DIG_IN_F_SOURCE",
        "SID_DIG_IN_F_POLARITY",
        "SID_DIG_IN_F_ACTION",
        "SID_DIG_IN_F_ACTIVATION",
        "SID_DIG_IN_F_ACTIVATION_DELAY",

        "SID_DIG_IN_G_SOURCE",
        "SID_DIG_IN_G_POLARITY",
        "SID_DIG_IN_G_ACTION",
        "SID_DIG_IN_G_ACTIVATION",
        "SID_DIG_IN_G_ACTIVATION_DELAY",

        "SID_DIG_IN_H_SOURCE",
        "SID_DIG_IN_H_POLARITY",
        "SID_DIG_IN_H_ACTION",
        "SID_DIG_IN_H_ACTIVATION",
        "SID_DIG_IN_H_ACTIVATION_DELAY",

        "SID_DIG_IN_I_SOURCE",
        "SID_DIG_IN_I_POLARITY",
        "SID_DIG_IN_I_ACTION",
        "SID_DIG_IN_I_ACTIVATION",
        "SID_DIG_IN_I_ACTIVATION_DELAY",

        "SID_LOP_RES_DIG_J_SENSOR_SELECTION",
        "SID_LOP_RES_DIG_J_DIG_SOURCE",
        "SID_LOP_RES_DIG_J_DIG_POLARITY",
        "SID_LOP_RES_DIG_J_DIG_ACTION",
        "SID_LOP_RES_DIG_J_DIG_ACTIVATION",
        "SID_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY",
        "SID_LOP_RES_DIG_J_SHUTDOWN",
        "SID_LOP_RES_DIG_J_SHUTDOWN_THRESHOLD",
        "SID_LOP_RES_DIG_J_WARNING",
        "SID_LOP_RES_DIG_J_WARNING_THRESHOLD",
        "SID_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION",
        "SID_LOP_RES_DIG_J_R1",
        "SID_LOP_RES_DIG_J_V1",
        "SID_LOP_RES_DIG_J_R2",
        "SID_LOP_RES_DIG_J_V2",
        "SID_LOP_RES_DIG_J_R3",
        "SID_LOP_RES_DIG_J_V3",
        "SID_LOP_RES_DIG_J_R4",
        "SID_LOP_RES_DIG_J_V4",
        "SID_LOP_RES_DIG_J_R5",
        "SID_LOP_RES_DIG_J_V5",
        "SID_LOP_RES_DIG_J_R6",
        "SID_LOP_RES_DIG_J_V6",
        "SID_LOP_RES_DIG_J_R7",
        "SID_LOP_RES_DIG_J_V7",
        "SID_LOP_RES_DIG_J_R8",
        "SID_LOP_RES_DIG_J_V8",
        "SID_LOP_RES_DIG_J_R9",
        "SID_LOP_RES_DIG_J_V9",
        "SID_LOP_RES_DIG_J_R10",
        "SID_LOP_RES_DIG_J_V10",

        "SID_FUEL_LVL_DIG_K_SENSOR_SELECTION",
        "SID_FUEL_LVL_DIG_K_DIG_SOURCE",
        "SID_FUEL_LVL_DIG_K_DIG_POLARITY",
        "SID_FUEL_LVL_DIG_K_DIG_ACTION",
        "SID_FUEL_LVL_DIG_K_DIG_ACTIVATION",
        "SID_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY",
        "SID_FUEL_LVL_DIG_K_SHUTDOWN",
        "SID_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD",
        "SID_FUEL_LVL_DIG_K_NOTIFICATION",
        "SID_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD",
        "SID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY",
        "SID_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING",
        "SID_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD",
        "SID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION",
        "SID_FUEL_LVL_DIG_K_FUEL_IN_LITERS",
        "SID_FUEL_LVL_DIG_K_R1",
        "SID_FUEL_LVL_DIG_K_L1",
        "SID_FUEL_LVL_DIG_K_R2",
        "SID_FUEL_LVL_DIG_K_L2",
        "SID_FUEL_LVL_DIG_K_R3",
        "SID_FUEL_LVL_DIG_K_L3",
        "SID_FUEL_LVL_DIG_K_R4",
        "SID_FUEL_LVL_DIG_K_L4",
        "SID_FUEL_LVL_DIG_K_R5",
        "SID_FUEL_LVL_DIG_K_L5",
        "SID_FUEL_LVL_DIG_K_R6",
        "SID_FUEL_LVL_DIG_K_L6",
        "SID_FUEL_LVL_DIG_K_R7",
        "SID_FUEL_LVL_DIG_K_L7",
        "SID_FUEL_LVL_DIG_K_R8",
        "SID_FUEL_LVL_DIG_K_L8",
        "SID_FUEL_LVL_DIG_K_R9",
        "SID_FUEL_LVL_DIG_K_L9",
        "SID_FUEL_LVL_DIG_K_R10",
        "SID_FUEL_LVL_DIG_K_L10",

        "SID_ENG_TEMP_DIG_M_SENSOR_SELECTION",
        "SID_ENG_TEMP_DIG_M_DIG_SOURCE",
        "SID_ENG_TEMP_DIG_M_DIG_POLARITY",
        "SID_ENG_TEMP_DIG_M_DIG_ACTION",
        "SID_ENG_TEMP_DIG_M_DIG_ACTIVATION",
        "SID_ENG_TEMP_DIG_M_DIG_ACTIVATION_DELAY",
        "SID_ENG_TEMP_DIG_M_ACTION",
        "SID_ENG_TEMP_DIG_M_THRESHOLD",
        "SID_ENG_TEMP_DIG_M_OPEN_CKT_WARNING",
        "SID_ENG_TEMP_DIG_M_R1",
        "SID_ENG_TEMP_DIG_M_T1",
        "SID_ENG_TEMP_DIG_M_R2",
        "SID_ENG_TEMP_DIG_M_T2",
        "SID_ENG_TEMP_DIG_M_R3",
        "SID_ENG_TEMP_DIG_M_T3",
        "SID_ENG_TEMP_DIG_M_R4",
        "SID_ENG_TEMP_DIG_M_T4",
        "SID_ENG_TEMP_DIG_M_R5",
        "SID_ENG_TEMP_DIG_M_T5",
        "SID_ENG_TEMP_DIG_M_R6",
        "SID_ENG_TEMP_DIG_M_T6",
        "SID_ENG_TEMP_DIG_M_R7",
        "SID_ENG_TEMP_DIG_M_T7",
        "SID_ENG_TEMP_DIG_M_R8",
        "SID_ENG_TEMP_DIG_M_T8",
        "SID_ENG_TEMP_DIG_M_R9",
        "SID_ENG_TEMP_DIG_M_T9",
        "SID_ENG_TEMP_DIG_M_R10",
        "SID_ENG_TEMP_DIG_M_T10",

        "SID_SHEL_TEMP_DIG_N_SENSOR_SELECTION",
        "SID_SHEL_TEMP_DIG_N_DIG_SOURCE",
        "SID_SHEL_TEMP_DIG_N_DIG_POLARITY",
        "SID_SHEL_TEMP_DIG_N_DIG_ACTION",
        "SID_SHEL_TEMP_DIG_N_DIG_ACTIVATION",
        "SID_SHEL_TEMP_DIG_N_DIG_ACTIVATION_DELAY",
        "SID_SHEL_TEMP_DIG_N_HIGH_TEMP_THRESHOLD",
        "SID_SHEL_TEMP_DIG_N_LOW_TEMP_THRESHOLD",
        "SID_SHEL_TEMP_DIG_N_HIGH_TEMP_MON_DELAY",
        "SID_SHEL_TEMP_DIG_N_DG_RUN_DURATION",
        "SID_SHEL_TEMP_DIG_N_OPEN_CKT_NOTIFICATION",
        "SID_SHEL_TEMP_DIG_N_R1",
        "SID_SHEL_TEMP_DIG_N_T1",
        "SID_SHEL_TEMP_DIG_N_R2",
        "SID_SHEL_TEMP_DIG_N_T2",
        "SID_SHEL_TEMP_DIG_N_R3",
        "SID_SHEL_TEMP_DIG_N_T3",
        "SID_SHEL_TEMP_DIG_N_R4",
        "SID_SHEL_TEMP_DIG_N_T4",
        "SID_SHEL_TEMP_DIG_N_R5",
        "SID_SHEL_TEMP_DIG_N_T5",
        "SID_SHEL_TEMP_DIG_N_R6",
        "SID_SHEL_TEMP_DIG_N_T6",
        "SID_SHEL_TEMP_DIG_N_R7",
        "SID_SHEL_TEMP_DIG_N_T7",
        "SID_SHEL_TEMP_DIG_N_R8",
        "SID_SHEL_TEMP_DIG_N_T8",
        "SID_SHEL_TEMP_DIG_N_R9",
        "SID_SHEL_TEMP_DIG_N_T9",
        "SID_SHEL_TEMP_DIG_N_R10",
        "SID_SHEL_TEMP_DIG_N_T10",

        "SID_AUX_S2_RES_DIG_N_SENSOR_SELECTION",
        "SID_AUX_S2_RES_DIG_N_DIG_SOURCE",
        "SID_AUX_S2_RES_DIG_N_DIG_POLARITY",
        "SID_AUX_S2_RES_DIG_N_DIG_ACTION",
        "SID_AUX_S2_RES_DIG_N_DIG_ACTIVATION",
        "SID_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY",
        "SID_AUX_S2_RES_DIG_N_ACTION",
        "SID_AUX_S2_RES_DIG_N_THRESHOLD",
        "SID_AUX_S2_RES_DIG_N_THRESHOLD_TYPE",
        "SID_AUX_S2_RES_DIG_N_OPEN_CKT_WARNING",
        "SID_AUX_S2_RES_DIG_N_R1",
        "SID_AUX_S2_RES_DIG_N_V1",
        "SID_AUX_S2_RES_DIG_N_R2",
        "SID_AUX_S2_RES_DIG_N_V2",
        "SID_AUX_S2_RES_DIG_N_R3",
        "SID_AUX_S2_RES_DIG_N_V3",
        "SID_AUX_S2_RES_DIG_N_R4",
        "SID_AUX_S2_RES_DIG_N_V4",
        "SID_AUX_S2_RES_DIG_N_R5",
        "SID_AUX_S2_RES_DIG_N_V5",
        "SID_AUX_S2_RES_DIG_N_R6",
        "SID_AUX_S2_RES_DIG_N_V6",
        "SID_AUX_S2_RES_DIG_N_R7",
        "SID_AUX_S2_RES_DIG_N_V7",
        "SID_AUX_S2_RES_DIG_N_R8",
        "SID_AUX_S2_RES_DIG_N_V8",
        "SID_AUX_S2_RES_DIG_N_R9",
        "SID_AUX_S2_RES_DIG_N_V9",
        "SID_AUX_S2_RES_DIG_N_R10",
        "SID_AUX_S2_RES_DIG_N_V10",

        "SID_AUX_S3_DIG_O_SENSOR_SELECTION",
        "SID_AUX_S3_DIG_O_DIG_SOURCE",
        "SID_AUX_S3_DIG_O_DIG_POLARITY",
        "SID_AUX_S3_DIG_O_DIG_ACTION",
        "SID_AUX_S3_DIG_O_DIG_ACTIVATION",
        "SID_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY",
        "SID_AUX_S3_DIG_O_SHUTDOWN",
        "SID_AUX_S3_DIG_O_SHUTDOWN_THRESHOLD",
        "SID_AUX_S3_DIG_O_WARNING",
        "SID_AUX_S3_DIG_O_WARNING_THRESHOLD",
        "SID_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION",
        "SID_AUX_S3_DIG_O_I1_V1",
        "SID_AUX_S3_DIG_O_P1",
        "SID_AUX_S3_DIG_O_I2_V2",
        "SID_AUX_S3_DIG_O_P2",
        "SID_AUX_S3_DIG_O_I3_V3",
        "SID_AUX_S3_DIG_O_P3",
        "SID_AUX_S3_DIG_O_I4_V4",
        "SID_AUX_S3_DIG_O_P4",
        "SID_AUX_S3_DIG_O_I5_V5",
        "SID_AUX_S3_DIG_O_P5",
        "SID_AUX_S3_DIG_O_I6_V6",
        "SID_AUX_S3_DIG_O_P6",
        "SID_AUX_S3_DIG_O_I7_V7",
        "SID_AUX_S3_DIG_O_P7",
        "SID_AUX_S3_DIG_O_I8_V8",
        "SID_AUX_S3_DIG_O_P8",
        "SID_AUX_S3_DIG_O_I9_V9",
        "SID_AUX_S3_DIG_O_P9",
        "SID_AUX_S3_DIG_O_I10_V10",
        "SID_AUX_S3_DIG_O_P10",

        "SID_AUX_S4_DIG_P_SENSOR_SELECTION",
        "SID_AUX_S4_DIG_P_DIG_SOURCE",
        "SID_AUX_S4_DIG_P_DIG_POLARITY",
        "SID_AUX_S4_DIG_P_DIG_ACTION",
        "SID_AUX_S4_DIG_P_DIG_ACTIVATION",
        "SID_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY",
        "SID_AUX_S4_DIG_P_SHUTDOWN",
        "SID_AUX_S4_DIG_P_SHUTDOWN_THRESHOLD",
        "SID_AUX_S4_DIG_P_NOTIFICATION",
        "SID_AUX_S4_DIG_P_NOTIFICATION_THRESHOLD",
        "SID_AUX_S4_DIG_P_FUEL_THEFT_WARNING",
        "SID_AUX_S4_DIG_P_FUEL_THEFT_THRESHOLD",
        "SID_AUX_S4_DIG_P_FUEL_CONSUMPTION",
        "SID_AUX_S4_DIG_P_FUEL_IN_LITERS",
        "SID_AUX_S4_DIG_P_SENSOR_LOW_VTG",
        "SID_AUX_S4_DIG_P_SENSOR_HIGH_VTG",
        "SID_AUX_S4_DIG_P_TANK_WITH_STEP",
        "SID_AUX_S4_DIG_P_TANK_WIDTH",
        "SID_AUX_S4_DIG_P_TANK_LENGTH_1",
        "SID_AUX_S4_DIG_P_TANK_HEIGHT_1",
        "SID_AUX_S4_DIG_P_TANK_LENGTH_2",
        "SID_AUX_S4_DIG_P_TANK_HEIGHT_2",
        /*Outputs*/
        "SID_OUT_A_SOURCE",
        "SID_OUT_A_ON_ACTIVATION",

        "SID_OUT_B_SOURCE",
        "SID_OUT_B_ON_ACTIVATION",

        "SID_OUT_C_SOURCE",
        "SID_OUT_C_ON_ACTIVATION",

        "SID_OUT_D_SOURCE",
        "SID_OUT_D_ON_ACTIVATION",

        "SID_OUT_E_SOURCE",
        "SID_OUT_E_ON_ACTIVATION",

        "SID_OUT_F_SOURCE",
        "SID_OUT_F_ON_ACTIVATION",

        "SID_OUT_G_SOURCE",
        "SID_OUT_G_ON_ACTIVATION",
        /*Timers*/
        "SID_CRANKING_TIMER_CRANK_HOLD_TIME",
        "SID_CRANKING_TIMER_CRANK_REST_TIME",
        "SID_CRANKING_TIMER_MANUAL_START_DELAY",
        "SID_CRANKING_TIMER_AUTO_START_DELAY",

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
        "SID_GENERAL_TIMER__DEEP_SLP_MODE_DELAY",
        "SID_GENERAL_TIMER_SOUNDER_ALARM_TIMER",
        "SID_GENERAL_TIMER_TEST_MODE_TIMER",
        /*Generator*/
        "SID_ALT_CONFIG_ALT_PRESENT",
        "SID_ALT_CONFIG_NUMBER_OF_POLES",
        "SID_ALT_CONFIG_ALT_AC_SYSTEM",
        "SID_ALT_CONFIG_MIN_HEALTHY_VOLT",
        "SID_ALT_CONFIG_MIN_HEALTHY_FREQ",
        "SID_ALT_CONFIG_PHASE_REVERSAL_DETECT",
        "SID_ALT_CONFIG_PHASE_REVERSAL_ACTION",
        "SID_ALT_CONFIG_AUTO_LOAD_TRANSFER",
        "SID_ALT_CONFIG_ALT_WAVE_DETECTION",

        "SID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN",
        "SID_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD",
        "SID_VOLT_MONITOR_UNDER_VOLT_WARNING",
        "SID_VOLT_MONITOR_UV_WARNING_THRESHOLD",
        "SID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN",
        "SID_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD",
        "SID_VOLT_MONITOR_OVER_VOLT_WARNING",
        "SID_VOLT_MONITOR_OV_WARNING_THRESHOLD",

        "SID_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN",
        "SID_FREQ_MONITOR_UF_SHUTDOWN_THRESHOLD",
        "SID_FREQ_MONITOR_UNDER_FREQ_WARNING",
        "SID_FREQ_MONITOR_UF_WARNING_THRESHOLD",
        "SID_FREQ_MONITOR_OVER_FREQ_SHUTDOWN",
        "SID_FREQ_MONITOR_OF_SHUTDOWN_THRESHOLD",
        "SID_FREQ_MONITOR_OVER_FREQ_WARNING",
        "SID_FREQ_MONITOR_OF_WARNING_THRESHOLD",

        "SID_CURRENT_MONITOR_LOAD_CT_RATIO",
        "SID_CURRENT_MONITOR_OVER_CURR_ACTION",
        "SID_CURRENT_MONITOR_OVER_CURR_THRESHOLD",
        "SID_CURRENT_MONITOR_OVER_CURR_DELAY",
        "SID_CURRENT_MONITOR_CT_CORRECTION_FACTOR",
        "SID_CURRENT_MONITOR_CT_LOCATION",

        "SID_FAN_CURR_MONITOR_FAN_CURRENT_MON",
        "SID_FAN_CURR_MONITOR_FAN_MON_CT_RATIO",
        "SID_FAN_CURR_MONITOR_HIGH_CURR_THRESHOLD",
        "SID_FAN_CURR_MONITOR_HIGH_CURR_ACTION",
        "SID_FAN_CURR_MONITOR_LOW_CURR_THRESHOLD",
        "SID_FAN_CURR_MONITOR_LOW_CURR_ACTION",
        "SID_FAN_CURR_MONITOR_CURR_MON_DELAY",

        "SID_LOAD_MONITOR_GEN_RATING",
        "SID_LOAD_MONITOR_FULL_LOAD_CURRENT",
        "SID_LOAD_MONITOR_OVERLOAD_ACTION",
        "SID_LOAD_MONITOR_OVERLOAD_THRESHOLD",
        "SID_LOAD_MONITOR_OVERLOAD_MON_DELAY",
        "SID_LOAD_MONITOR_UNBAL_LOAD_ACTION",
        "SID_LOAD_MONITOR_UNBAL_LOAD_THRESHOLD",
        "SID_LOAD_MONITOR_UNBAL_LOAD_DELAY",
        "SID_LOAD_MONITOR_UNBAL_LOAD_ACT_THRESH",
        /*Mains*/
        "SID_MAINS_CONFIG_MAINS_MONITORING",
        "SID_MAINS_CONFIG_MAINS_AC_SYSTEM",
        "SID_MAINS_CONFIG_PHASE_REVERSAL_DETECT",
        "SID_MAINS_CONFIG_PHASE_REVERSAL_ACTION",
        "SID_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH",

        "SID_UNDER_VOLT_MON_ENABLE",
        "SID_UNDER_VOLT_MON_TRIP",
        "SID_UNDER_VOLT_MON_RETURN",

        "SID_OVER_VOLT_MON_ENABLE",
        "SID_OVER_VOLT_MON_TRIP",
        "SID_OVER_VOLT_MON_RETURN",

        "SID_UNDER_FREQ_MON_ENABLE",
        "SID_UNDER_FREQ_MON_TRIP",
        "SID_UNDER_FREQ_MON_RETURN",

        "SID_OVER_FREQ_MON_ENABLE",
        "SID_OVER_FREQ_MON_TRIP",
        "SID_OVER_FREQ_MON_RETURN",
        /*Engine*/
        "SID_CRANK_DISCONNECT_START_ATTEMPTS",
        "SID_CRANK_DISCONNECT_DISCONN_ON_LOP_SENS",
        "SID_CRANK_DISCONNECT_DISCONN_LOP_SENS",
        "SID_CRANK_DISCONNECT_MON_LLOP_BEFORE_CRANK",
        "SID_CRANK_DISCONNECT_MON_LOP_BEFORE_CRANK",
        "SID_CRANK_DISCONNECT_DISCONN_ON_LLOP_SW",
        "SID_CRANK_DISCONNECT_LLOP_SW_TRANS_TIME",
        "SID_CRANK_DISCONNECT_ALT_FREQUENCY",
        "SID_CRANK_DISCONNECT_ENGINE_SPEED",
        "SID_CRANK_DISCONNECT_DISCONN_ON_CHG_ALT_VOLT",
        "SID_CRANK_DISCONNECT_CHG_ALT_THRESHOLD",

        "SID_SPEED_MONITOR_SPEED_SENSE_SOURCE",
        "SID_SPEED_MONITOR_RESERVED",
        "SID_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN",
        "SID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD",
        "SID_SPEED_MONITOR_UNDER_SPEED_DELAY",
        "SID_SPEED_MONITOR_OVER_SPEED_THRESHOLD",
        "SID_SPEED_MONITOR_OVER_SPEED_DELAY",
        "SID_SPEED_MONITOR_GROSS_OS_THRESHOLD",

        "SID_BATTERY_MONITOR_LOW_VOLT_ACTION",
        "SID_BATTERY_MONITOR_LOW_VOLT_THRESHOLD",
        "SID_BATTERY_MONITOR_LOW_VOLT_DELAY",
        "SID_BATTERY_MONITOR_HIGH_VOLT_ACTION",
        "SID_BATTERY_MONITOR_HIGH_VOLT_THRESHOLD",
        "SID_BATTERY_MONITOR_HIGH_VOLT_DELAY",
        "SID_BATTERY_MONITOR_BATTERY_MON_BY_J1939",

        "SID_CHARGE_ALT_MON_FAIL_ACTION",
        "SID_CHARGE_ALT_MON_FAIL_THRESHOLD",
        "SID_CHARGE_ALT_MON_FAIL_DELAY",
        "SID_CHARGE_ALT_MON_CHARGE_ALT_MON_BY_J1939",
        "SID_PREHEAT_PREHEAT_TIMER",
        "SID_PREHEAT_ENG_TEMPERATURE",
        "SID_PREHEAT_ENG_TEMP_THRESHOLD",
        "SID_PREHEAT_AMB_TEMPERATURE",
        /*Maintenance*/
        "SID_MAINT_ALARM_ACTION",
        "SID_MAINT_ALARM_DUE_AT_ENGINE_HOURS",
        "SID_ALARM_DUE_DATE_SERVICE_DATE_1",
        "SID_ALARM_DUE_DATE_SERVICE_DATE_2",
        "SID_ALARM_DUE_DATE_SERVICE_DATE_3",


//RushiEnd
        "PROFILE NAME",

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
      /*Module*/
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
      /*Inputs*/
      "SID_DIG_IN_A_SOURCE",
      "SID_DIG_IN_A_POLARITY",
      "SID_DIG_IN_A_ACTION",
      "SID_DIG_IN_A_ACTIVATION",
      "SID_DIG_IN_A_ACTIVATION_DELAY",

      "SID_DIG_IN_B_SOURCE",
      "SID_DIG_IN_B_POLARITY",
      "SID_DIG_IN_B_ACTION",
      "SID_DIG_IN_B_ACTIVATION",
      "SID_DIG_IN_B_ACTIVATION_DELAY",

      "SID_DIG_IN_C_SOURCE",
      "SID_DIG_IN_C_POLARITY",
      "SID_DIG_IN_C_ACTION",
      "SID_DIG_IN_C_ACTIVATION",
      "SID_DIG_IN_C_ACTIVATION_DELAY",

      "SID_DIG_IN_D_SOURCE",
      "SID_DIG_IN_D_POLARITY",
      "SID_DIG_IN_D_ACTION",
      "SID_DIG_IN_D_ACTIVATION",
      "SID_DIG_IN_D_ACTIVATION_DELAY",

      "SID_DIG_IN_E_SOURCE",
      "SID_DIG_IN_E_POLARITY",
      "SID_DIG_IN_E_ACTION",
      "SID_DIG_IN_E_ACTIVATION",
      "SID_DIG_IN_E_ACTIVATION_DELAY",

      "SID_DIG_IN_F_SOURCE",
      "SID_DIG_IN_F_POLARITY",
      "SID_DIG_IN_F_ACTION",
      "SID_DIG_IN_F_ACTIVATION",
      "SID_DIG_IN_F_ACTIVATION_DELAY",

      "SID_DIG_IN_G_SOURCE",
      "SID_DIG_IN_G_POLARITY",
      "SID_DIG_IN_G_ACTION",
      "SID_DIG_IN_G_ACTIVATION",
      "SID_DIG_IN_G_ACTIVATION_DELAY",

      "SID_DIG_IN_H_SOURCE",
      "SID_DIG_IN_H_POLARITY",
      "SID_DIG_IN_H_ACTION",
      "SID_DIG_IN_H_ACTIVATION",
      "SID_DIG_IN_H_ACTIVATION_DELAY",

      "SID_DIG_IN_I_SOURCE",
      "SID_DIG_IN_I_POLARITY",
      "SID_DIG_IN_I_ACTION",
      "SID_DIG_IN_I_ACTIVATION",
      "SID_DIG_IN_I_ACTIVATION_DELAY",

      "SID_LOP_RES_DIG_J_SENSOR_SELECTION",
      "SID_LOP_RES_DIG_J_DIG_SOURCE",
      "SID_LOP_RES_DIG_J_DIG_POLARITY",
      "SID_LOP_RES_DIG_J_DIG_ACTION",
      "SID_LOP_RES_DIG_J_DIG_ACTIVATION",
      "SID_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY",
      "SID_LOP_RES_DIG_J_SHUTDOWN",
      "SID_LOP_RES_DIG_J_SHUTDOWN_THRESHOLD",
      "SID_LOP_RES_DIG_J_WARNING",
      "SID_LOP_RES_DIG_J_WARNING_THRESHOLD",
      "SID_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION",
      "SID_LOP_RES_DIG_J_R1",
      "SID_LOP_RES_DIG_J_V1",
      "SID_LOP_RES_DIG_J_R2",
      "SID_LOP_RES_DIG_J_V2",
      "SID_LOP_RES_DIG_J_R3",
      "SID_LOP_RES_DIG_J_V3",
      "SID_LOP_RES_DIG_J_R4",
      "SID_LOP_RES_DIG_J_V4",
      "SID_LOP_RES_DIG_J_R5",
      "SID_LOP_RES_DIG_J_V5",
      "SID_LOP_RES_DIG_J_R6",
      "SID_LOP_RES_DIG_J_V6",
      "SID_LOP_RES_DIG_J_R7",
      "SID_LOP_RES_DIG_J_V7",
      "SID_LOP_RES_DIG_J_R8",
      "SID_LOP_RES_DIG_J_V8",
      "SID_LOP_RES_DIG_J_R9",
      "SID_LOP_RES_DIG_J_V9",
      "SID_LOP_RES_DIG_J_R10",
      "SID_LOP_RES_DIG_J_V10",

      "SID_FUEL_LVL_DIG_K_SENSOR_SELECTION",
      "SID_FUEL_LVL_DIG_K_DIG_SOURCE",
      "SID_FUEL_LVL_DIG_K_DIG_POLARITY",
      "SID_FUEL_LVL_DIG_K_DIG_ACTION",
      "SID_FUEL_LVL_DIG_K_DIG_ACTIVATION",
      "SID_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY",
      "SID_FUEL_LVL_DIG_K_SHUTDOWN",
      "SID_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD",
      "SID_FUEL_LVL_DIG_K_NOTIFICATION",
      "SID_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD",
      "SID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY",
      "SID_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING",
      "SID_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD",
      "SID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION",
      "SID_FUEL_LVL_DIG_K_FUEL_IN_LITERS",
      "SID_FUEL_LVL_DIG_K_R1",
      "SID_FUEL_LVL_DIG_K_L1",
      "SID_FUEL_LVL_DIG_K_R2",
      "SID_FUEL_LVL_DIG_K_L2",
      "SID_FUEL_LVL_DIG_K_R3",
      "SID_FUEL_LVL_DIG_K_L3",
      "SID_FUEL_LVL_DIG_K_R4",
      "SID_FUEL_LVL_DIG_K_L4",
      "SID_FUEL_LVL_DIG_K_R5",
      "SID_FUEL_LVL_DIG_K_L5",
      "SID_FUEL_LVL_DIG_K_R6",
      "SID_FUEL_LVL_DIG_K_L6",
      "SID_FUEL_LVL_DIG_K_R7",
      "SID_FUEL_LVL_DIG_K_L7",
      "SID_FUEL_LVL_DIG_K_R8",
      "SID_FUEL_LVL_DIG_K_L8",
      "SID_FUEL_LVL_DIG_K_R9",
      "SID_FUEL_LVL_DIG_K_L9",
      "SID_FUEL_LVL_DIG_K_R10",
      "SID_FUEL_LVL_DIG_K_L10",

      "SID_ENG_TEMP_DIG_M_SENSOR_SELECTION",
      "SID_ENG_TEMP_DIG_M_DIG_SOURCE",
      "SID_ENG_TEMP_DIG_M_DIG_POLARITY",
      "SID_ENG_TEMP_DIG_M_DIG_ACTION",
      "SID_ENG_TEMP_DIG_M_DIG_ACTIVATION",
      "SID_ENG_TEMP_DIG_M_DIG_ACTIVATION_DELAY",
      "SID_ENG_TEMP_DIG_M_ACTION",
      "SID_ENG_TEMP_DIG_M_THRESHOLD",
      "SID_ENG_TEMP_DIG_M_OPEN_CKT_WARNING",
      "SID_ENG_TEMP_DIG_M_R1",
      "SID_ENG_TEMP_DIG_M_T1",
      "SID_ENG_TEMP_DIG_M_R2",
      "SID_ENG_TEMP_DIG_M_T2",
      "SID_ENG_TEMP_DIG_M_R3",
      "SID_ENG_TEMP_DIG_M_T3",
      "SID_ENG_TEMP_DIG_M_R4",
      "SID_ENG_TEMP_DIG_M_T4",
      "SID_ENG_TEMP_DIG_M_R5",
      "SID_ENG_TEMP_DIG_M_T5",
      "SID_ENG_TEMP_DIG_M_R6",
      "SID_ENG_TEMP_DIG_M_T6",
      "SID_ENG_TEMP_DIG_M_R7",
      "SID_ENG_TEMP_DIG_M_T7",
      "SID_ENG_TEMP_DIG_M_R8",
      "SID_ENG_TEMP_DIG_M_T8",
      "SID_ENG_TEMP_DIG_M_R9",
      "SID_ENG_TEMP_DIG_M_T9",
      "SID_ENG_TEMP_DIG_M_R10",
      "SID_ENG_TEMP_DIG_M_T10",

      "SID_SHEL_TEMP_DIG_N_SENSOR_SELECTION",
      "SID_SHEL_TEMP_DIG_N_DIG_SOURCE",
      "SID_SHEL_TEMP_DIG_N_DIG_POLARITY",
      "SID_SHEL_TEMP_DIG_N_DIG_ACTION",
      "SID_SHEL_TEMP_DIG_N_DIG_ACTIVATION",
      "SID_SHEL_TEMP_DIG_N_DIG_ACTIVATION_DELAY",
      "SID_SHEL_TEMP_DIG_N_HIGH_TEMP_THRESHOLD",
      "SID_SHEL_TEMP_DIG_N_LOW_TEMP_THRESHOLD",
      "SID_SHEL_TEMP_DIG_N_HIGH_TEMP_MON_DELAY",
      "SID_SHEL_TEMP_DIG_N_DG_RUN_DURATION",
      "SID_SHEL_TEMP_DIG_N_OPEN_CKT_NOTIFICATION",
      "SID_SHEL_TEMP_DIG_N_R1",
      "SID_SHEL_TEMP_DIG_N_T1",
      "SID_SHEL_TEMP_DIG_N_R2",
      "SID_SHEL_TEMP_DIG_N_T2",
      "SID_SHEL_TEMP_DIG_N_R3",
      "SID_SHEL_TEMP_DIG_N_T3",
      "SID_SHEL_TEMP_DIG_N_R4",
      "SID_SHEL_TEMP_DIG_N_T4",
      "SID_SHEL_TEMP_DIG_N_R5",
      "SID_SHEL_TEMP_DIG_N_T5",
      "SID_SHEL_TEMP_DIG_N_R6",
      "SID_SHEL_TEMP_DIG_N_T6",
      "SID_SHEL_TEMP_DIG_N_R7",
      "SID_SHEL_TEMP_DIG_N_T7",
      "SID_SHEL_TEMP_DIG_N_R8",
      "SID_SHEL_TEMP_DIG_N_T8",
      "SID_SHEL_TEMP_DIG_N_R9",
      "SID_SHEL_TEMP_DIG_N_T9",
      "SID_SHEL_TEMP_DIG_N_R10",
      "SID_SHEL_TEMP_DIG_N_T10",

      "SID_AUX_S2_RES_DIG_N_SENSOR_SELECTION",
      "SID_AUX_S2_RES_DIG_N_DIG_SOURCE",
      "SID_AUX_S2_RES_DIG_N_DIG_POLARITY",
      "SID_AUX_S2_RES_DIG_N_DIG_ACTION",
      "SID_AUX_S2_RES_DIG_N_DIG_ACTIVATION",
      "SID_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY",
      "SID_AUX_S2_RES_DIG_N_ACTION",
      "SID_AUX_S2_RES_DIG_N_THRESHOLD",
      "SID_AUX_S2_RES_DIG_N_THRESHOLD_TYPE",
      "SID_AUX_S2_RES_DIG_N_OPEN_CKT_WARNING",
      "SID_AUX_S2_RES_DIG_N_R1",
      "SID_AUX_S2_RES_DIG_N_V1",
      "SID_AUX_S2_RES_DIG_N_R2",
      "SID_AUX_S2_RES_DIG_N_V2",
      "SID_AUX_S2_RES_DIG_N_R3",
      "SID_AUX_S2_RES_DIG_N_V3",
      "SID_AUX_S2_RES_DIG_N_R4",
      "SID_AUX_S2_RES_DIG_N_V4",
      "SID_AUX_S2_RES_DIG_N_R5",
      "SID_AUX_S2_RES_DIG_N_V5",
      "SID_AUX_S2_RES_DIG_N_R6",
      "SID_AUX_S2_RES_DIG_N_V6",
      "SID_AUX_S2_RES_DIG_N_R7",
      "SID_AUX_S2_RES_DIG_N_V7",
      "SID_AUX_S2_RES_DIG_N_R8",
      "SID_AUX_S2_RES_DIG_N_V8",
      "SID_AUX_S2_RES_DIG_N_R9",
      "SID_AUX_S2_RES_DIG_N_V9",
      "SID_AUX_S2_RES_DIG_N_R10",
      "SID_AUX_S2_RES_DIG_N_V10",

      "SID_AUX_S3_DIG_O_SENSOR_SELECTION",
      "SID_AUX_S3_DIG_O_DIG_SOURCE",
      "SID_AUX_S3_DIG_O_DIG_POLARITY",
      "SID_AUX_S3_DIG_O_DIG_ACTION",
      "SID_AUX_S3_DIG_O_DIG_ACTIVATION",
      "SID_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY",
      "SID_AUX_S3_DIG_O_SHUTDOWN",
      "SID_AUX_S3_DIG_O_SHUTDOWN_THRESHOLD",
      "SID_AUX_S3_DIG_O_WARNING",
      "SID_AUX_S3_DIG_O_WARNING_THRESHOLD",
      "SID_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION",
      "SID_AUX_S3_DIG_O_I1_V1",
      "SID_AUX_S3_DIG_O_P1",
      "SID_AUX_S3_DIG_O_I2_V2",
      "SID_AUX_S3_DIG_O_P2",
      "SID_AUX_S3_DIG_O_I3_V3",
      "SID_AUX_S3_DIG_O_P3",
      "SID_AUX_S3_DIG_O_I4_V4",
      "SID_AUX_S3_DIG_O_P4",
      "SID_AUX_S3_DIG_O_I5_V5",
      "SID_AUX_S3_DIG_O_P5",
      "SID_AUX_S3_DIG_O_I6_V6",
      "SID_AUX_S3_DIG_O_P6",
      "SID_AUX_S3_DIG_O_I7_V7",
      "SID_AUX_S3_DIG_O_P7",
      "SID_AUX_S3_DIG_O_I8_V8",
      "SID_AUX_S3_DIG_O_P8",
      "SID_AUX_S3_DIG_O_I9_V9",
      "SID_AUX_S3_DIG_O_P9",
      "SID_AUX_S3_DIG_O_I10_V10",
      "SID_AUX_S3_DIG_O_P10",

      "SID_AUX_S4_DIG_P_SENSOR_SELECTION",
      "SID_AUX_S4_DIG_P_DIG_SOURCE",
      "SID_AUX_S4_DIG_P_DIG_POLARITY",
      "SID_AUX_S4_DIG_P_DIG_ACTION",
      "SID_AUX_S4_DIG_P_DIG_ACTIVATION",
      "SID_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY",
      "SID_AUX_S4_DIG_P_SHUTDOWN",
      "SID_AUX_S4_DIG_P_SHUTDOWN_THRESHOLD",
      "SID_AUX_S4_DIG_P_NOTIFICATION",
      "SID_AUX_S4_DIG_P_NOTIFICATION_THRESHOLD",
      "SID_AUX_S4_DIG_P_FUEL_THEFT_WARNING",
      "SID_AUX_S4_DIG_P_FUEL_THEFT_THRESHOLD",
      "SID_AUX_S4_DIG_P_FUEL_CONSUMPTION",
      "SID_AUX_S4_DIG_P_FUEL_IN_LITERS",
      "SID_AUX_S4_DIG_P_SENSOR_LOW_VTG",
      "SID_AUX_S4_DIG_P_SENSOR_HIGH_VTG",
      "SID_AUX_S4_DIG_P_TANK_WITH_STEP",
      "SID_AUX_S4_DIG_P_TANK_WIDTH",
      "SID_AUX_S4_DIG_P_TANK_LENGTH_1",
      "SID_AUX_S4_DIG_P_TANK_HEIGHT_1",
      "SID_AUX_S4_DIG_P_TANK_LENGTH_2",
      "SID_AUX_S4_DIG_P_TANK_HEIGHT_2",
      /*Outputs*/
      "SID_OUT_A_SOURCE",
      "SID_OUT_A_ON_ACTIVATION",

      "SID_OUT_B_SOURCE",
      "SID_OUT_B_ON_ACTIVATION",

      "SID_OUT_C_SOURCE",
      "SID_OUT_C_ON_ACTIVATION",

      "SID_OUT_D_SOURCE",
      "SID_OUT_D_ON_ACTIVATION",

      "SID_OUT_E_SOURCE",
      "SID_OUT_E_ON_ACTIVATION",

      "SID_OUT_F_SOURCE",
      "SID_OUT_F_ON_ACTIVATION",

      "SID_OUT_G_SOURCE",
      "SID_OUT_G_ON_ACTIVATION",
      /*Timers*/
      "SID_CRANKING_TIMER_CRANK_HOLD_TIME",
      "SID_CRANKING_TIMER_CRANK_REST_TIME",
      "SID_CRANKING_TIMER_MANUAL_START_DELAY",
      "SID_CRANKING_TIMER_AUTO_START_DELAY",

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
      "SID_GENERAL_TIMER__DEEP_SLP_MODE_DELAY",
      "SID_GENERAL_TIMER_SOUNDER_ALARM_TIMER",
      "SID_GENERAL_TIMER_TEST_MODE_TIMER",
      /*Generator*/
      "SID_ALT_CONFIG_ALT_PRESENT",
      "SID_ALT_CONFIG_NUMBER_OF_POLES",
      "SID_ALT_CONFIG_ALT_AC_SYSTEM",
      "SID_ALT_CONFIG_MIN_HEALTHY_VOLT",
      "SID_ALT_CONFIG_MIN_HEALTHY_FREQ",
      "SID_ALT_CONFIG_PHASE_REVERSAL_DETECT",
      "SID_ALT_CONFIG_PHASE_REVERSAL_ACTION",
      "SID_ALT_CONFIG_AUTO_LOAD_TRANSFER",
      "SID_ALT_CONFIG_ALT_WAVE_DETECTION",

      "SID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN",
      "SID_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD",
      "SID_VOLT_MONITOR_UNDER_VOLT_WARNING",
      "SID_VOLT_MONITOR_UV_WARNING_THRESHOLD",
      "SID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN",
      "SID_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD",
      "SID_VOLT_MONITOR_OVER_VOLT_WARNING",
      "SID_VOLT_MONITOR_OV_WARNING_THRESHOLD",

      "SID_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN",
      "SID_FREQ_MONITOR_UF_SHUTDOWN_THRESHOLD",
      "SID_FREQ_MONITOR_UNDER_FREQ_WARNING",
      "SID_FREQ_MONITOR_UF_WARNING_THRESHOLD",
      "SID_FREQ_MONITOR_OVER_FREQ_SHUTDOWN",
      "SID_FREQ_MONITOR_OF_SHUTDOWN_THRESHOLD",
      "SID_FREQ_MONITOR_OVER_FREQ_WARNING",
      "SID_FREQ_MONITOR_OF_WARNING_THRESHOLD",

      "SID_CURRENT_MONITOR_LOAD_CT_RATIO",
      "SID_CURRENT_MONITOR_OVER_CURR_ACTION",
      "SID_CURRENT_MONITOR_OVER_CURR_THRESHOLD",
      "SID_CURRENT_MONITOR_OVER_CURR_DELAY",
      "SID_CURRENT_MONITOR_CT_CORRECTION_FACTOR",
      "SID_CURRENT_MONITOR_CT_LOCATION",

      "SID_FAN_CURR_MONITOR_FAN_CURRENT_MON",
      "SID_FAN_CURR_MONITOR_FAN_MON_CT_RATIO",
      "SID_FAN_CURR_MONITOR_HIGH_CURR_THRESHOLD",
      "SID_FAN_CURR_MONITOR_HIGH_CURR_ACTION",
      "SID_FAN_CURR_MONITOR_LOW_CURR_THRESHOLD",
      "SID_FAN_CURR_MONITOR_LOW_CURR_ACTION",
      "SID_FAN_CURR_MONITOR_CURR_MON_DELAY",

      "SID_LOAD_MONITOR_GEN_RATING",
      "SID_LOAD_MONITOR_FULL_LOAD_CURRENT",
      "SID_LOAD_MONITOR_OVERLOAD_ACTION",
      "SID_LOAD_MONITOR_OVERLOAD_THRESHOLD",
      "SID_LOAD_MONITOR_OVERLOAD_MON_DELAY",
      "SID_LOAD_MONITOR_UNBAL_LOAD_ACTION",
      "SID_LOAD_MONITOR_UNBAL_LOAD_THRESHOLD",
      "SID_LOAD_MONITOR_UNBAL_LOAD_DELAY",
      "SID_LOAD_MONITOR_UNBAL_LOAD_ACT_THRESH",
      /*Mains*/
      "SID_MAINS_CONFIG_MAINS_MONITORING",
      "SID_MAINS_CONFIG_MAINS_AC_SYSTEM",
      "SID_MAINS_CONFIG_PHASE_REVERSAL_DETECT",
      "SID_MAINS_CONFIG_PHASE_REVERSAL_ACTION",
      "SID_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH",

      "SID_UNDER_VOLT_MON_ENABLE",
      "SID_UNDER_VOLT_MON_TRIP",
      "SID_UNDER_VOLT_MON_RETURN",

      "SID_OVER_VOLT_MON_ENABLE",
      "SID_OVER_VOLT_MON_TRIP",
      "SID_OVER_VOLT_MON_RETURN",

      "SID_UNDER_FREQ_MON_ENABLE",
      "SID_UNDER_FREQ_MON_TRIP",
      "SID_UNDER_FREQ_MON_RETURN",

      "SID_OVER_FREQ_MON_ENABLE",
      "SID_OVER_FREQ_MON_TRIP",
      "SID_OVER_FREQ_MON_RETURN",
      /*Engine*/
      "SID_CRANK_DISCONNECT_START_ATTEMPTS",
      "SID_CRANK_DISCONNECT_DISCONN_ON_LOP_SENS",
      "SID_CRANK_DISCONNECT_DISCONN_LOP_SENS",
      "SID_CRANK_DISCONNECT_MON_LLOP_BEFORE_CRANK",
      "SID_CRANK_DISCONNECT_MON_LOP_BEFORE_CRANK",
      "SID_CRANK_DISCONNECT_DISCONN_ON_LLOP_SW",
      "SID_CRANK_DISCONNECT_LLOP_SW_TRANS_TIME",
      "SID_CRANK_DISCONNECT_ALT_FREQUENCY",
      "SID_CRANK_DISCONNECT_ENGINE_SPEED",
      "SID_CRANK_DISCONNECT_DISCONN_ON_CHG_ALT_VOLT",
      "SID_CRANK_DISCONNECT_CHG_ALT_THRESHOLD",

      "SID_SPEED_MONITOR_SPEED_SENSE_SOURCE",
      "SID_SPEED_MONITOR_RESERVED",
      "SID_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN",
      "SID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD",
      "SID_SPEED_MONITOR_UNDER_SPEED_DELAY",
      "SID_SPEED_MONITOR_OVER_SPEED_THRESHOLD",
      "SID_SPEED_MONITOR_OVER_SPEED_DELAY",
      "SID_SPEED_MONITOR_GROSS_OS_THRESHOLD",

      "SID_BATTERY_MONITOR_LOW_VOLT_ACTION",
      "SID_BATTERY_MONITOR_LOW_VOLT_THRESHOLD",
      "SID_BATTERY_MONITOR_LOW_VOLT_DELAY",
      "SID_BATTERY_MONITOR_HIGH_VOLT_ACTION",
      "SID_BATTERY_MONITOR_HIGH_VOLT_THRESHOLD",
      "SID_BATTERY_MONITOR_HIGH_VOLT_DELAY",
      "SID_BATTERY_MONITOR_BATTERY_MON_BY_J1939",

      "SID_CHARGE_ALT_MON_FAIL_ACTION",
      "SID_CHARGE_ALT_MON_FAIL_THRESHOLD",
      "SID_CHARGE_ALT_MON_FAIL_DELAY",
      "SID_CHARGE_ALT_MON_CHARGE_ALT_MON_BY_J1939",
      "SID_PREHEAT_PREHEAT_TIMER",
      "SID_PREHEAT_ENG_TEMPERATURE",
      "SID_PREHEAT_ENG_TEMP_THRESHOLD",
      "SID_PREHEAT_AMB_TEMPERATURE",
      /*Maintenance*/
      "SID_MAINT_ALARM_ACTION",
      "SID_MAINT_ALARM_DUE_AT_ENGINE_HOURS",
      "SID_ALARM_DUE_DATE_SERVICE_DATE_1",
      "SID_ALARM_DUE_DATE_SERVICE_DATE_2",
      "SID_ALARM_DUE_DATE_SERVICE_DATE_3",

//RushiEnd
     "PERFIL",

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

//RushiStart
    ArrEditableItem[INDEX_OF_GENERAL_PROFILE_NAME] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_PROFILE_NAME), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_PROFILE_NAME]," ", "%u", (uint8_t)1, (uint8_t)10, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_POWER_ON_MODE] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_POWER_ON_MODE), strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_POWER_ON_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MANUAL_AUTO_S], 2  , CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_POWER_ON_LAMP_TEST] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_POWER_ON_LAMP_TEST),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_POWER_ON_LAMP_TEST], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE_S], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_GENERAL_PASSWORD_LOCK] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_PASSWORD_LOCK),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_PASSWORD_LOCK], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE_S], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DISPLAY_CONTRAST] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISPLAY_CONTRAST), strLeafNode[_u8LanguageArrayIndex][SID_DISPLAY_CONTRAST], arrUnit[ID_PERCENT], "%u", (uint8_t)0, (uint8_t)100, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DISPLAY_POWER_SAVE_MODE] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DISPLAY_POWER_SAVE_MODE),strLeafNode[_u8LanguageArrayIndex][SID_DISPLAY_POWER_SAVE_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_ENABLE_DISABLE_S], 2, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_MODBUS_COMM_COMM_MODE] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_COMM_MODE),strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_COMM_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_MODBUS_S], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_COMM_MODBUS_SLAVE_ID] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_MODBUS_SLAVE_ID), strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_MODBUS_SLAVE_ID], "", "%u", (uint8_t)1, (uint8_t)247, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_COMM_MODBUS_BAUDRATE] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_MODBUS_BAUDRATE),strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_MODBUS_BAUDRATE], "", "%s", strBaudrateOptions, 8, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_MODBUS_COMM_PARITY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_MODBUS_COMM_PARITY),strLeafNode[_u8LanguageArrayIndex][SID_MODBUS_COMM_PARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_PARITY_S], 3, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_BTS_CONFIG_BATTERY_MON] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_BATTERY_MON], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO_S], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BTS_CONFIG_LOW_BATT_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_THRESHOLD),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_LOW_BATT_THRESHOLD], "", "%f", (float)40.0,(float) 55.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BTS_CONFIG_LOW_BATT_MON_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_MON_DELAY),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_LOW_BATT_MON_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)300, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_BTS_CONFIG_DG_RUN_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_DG_RUN_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_BTS_CONFIG_DG_RUN_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_CYCLIC_MODE] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_CYCLIC_MODE),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_CONFIG_CYCLIC_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO_S], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_DG_OFF_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_OFF_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_CONFIG_DG_OFF_DURATION], arrUnit[ID_SEC], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_CYCLIC_CONFIG_DG_ON_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_CYCLIC_CONFIG_DG_ON_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_CYCLIC_CONFIG_DG_ON_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)720, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFIG_NIGHT_MODE] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_NIGHT_MODE),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_MODE_CONFIG_NIGHT_MODE], "", "%s", strOptions[_u8LanguageArrayIndex][ID_YES_NO_S], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFIG_START_TIME] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_START_TIME),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_MODE_CONFIG_START_TIME], arrUnit[ID_SEC], "%u", (uint16_t)0, (uint16_t)2359, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_NIGHT_MODE_CONFIG_OFF_DURATION] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_NIGHT_MODE_CONFIG_OFF_DURATION),strLeafNode[_u8LanguageArrayIndex][SID_NIGHT_MODE_CONFIG_OFF_DURATION], arrUnit[ID_MINS], "%u", (uint16_t)1, (uint16_t)1439, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_A_SOURCE_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_POLARITY_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTIVATION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_A_ACTIVATION_DELAY_S] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_A_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_B_SOURCE_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_POLARITY_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTIVATION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_B_ACTIVATION_DELAY_S] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_B_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_C_SOURCE_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_POLARITY_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTIVATION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_C_ACTIVATION_DELAY_S] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_C_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_D_SOURCE_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_POLARITY_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTIVATION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_D_ACTIVATION_DELAY_S] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_D_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_E_SOURCE_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_POLARITY_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTIVATION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_E_ACTIVATION_DELAY_S] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_E_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_F_SOURCE_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_POLARITY_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTIVATION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_F_ACTIVATION_DELAY_S] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_F_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_G_SOURCE_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_POLARITY_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTIVATION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_G_ACTIVATION_DELAY_S] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_G_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_H_SOURCE_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_POLARITY_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTIVATION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_H_ACTIVATION_DELAY_S] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_H_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

    ArrEditableItem[INDEX_OF_DIG_IN_I_SOURCE_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_SOURCE),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_POLARITY_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_POLARITY),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_POLARITY], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_ACTION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTIVATION_S] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION),strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_ACTIVATION], "", "%s", strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_DIG_IN_I_ACTIVATION_DELAY_S] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_DIG_IN_I_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );

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
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_CONSUMPTION] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_CONSUMPTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_FUEL_LVL_DIG_K_FUEL_IN_LITERS] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_IN_LITERS), strLeafNode[_u8LanguageArrayIndex][SID_FUEL_LVL_DIG_K_FUEL_IN_LITERS], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_FUEL_REF], 2, CEditableItem::PIN1_PIN2_ALLOWED );
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

    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENG_CLNT_DIG_L_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_ACTION],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_THRESHOLD], "%/Hour", "%u", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_OPEN_CKT_WARNING] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_OPEN_CKT_WARNING), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_OPEN_CKT_WARNING],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_R1), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_R1], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_T1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_T1), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_T1],arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_R2), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_R2], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_T2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_T2), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_T2], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_R3), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_R3],arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_T3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_T3), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_T3], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_R4), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_R4], arrUnit[ID_OHM], "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_T4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_T4), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_T4], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_R5), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_R5], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_T5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_T5), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_T5], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_R6), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_R6], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_T6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_T6), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_T6], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_R7), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_R7], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_T7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_T7), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_T7], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_R8), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_R8], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_T8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_T8), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_T8], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_R9), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_R9], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_T9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_T9), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_T9], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_R10), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_R10], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_ENG_TEMP_DIG_M_T10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_M_T10), strLeafNode[_u8LanguageArrayIndex][SID_ENG_TEMP_DIG_M_T10], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );

    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_SENSOR_SELECTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_SENSOR_SELECTION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_SENSOR_SELECTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_ENG_CLNT_DIG_L_SENS_SEL], 3, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_DIG_SOURCE] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_DIG_SOURCE), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_DIG_SOURCE], "", "%s",  strInputSources[_u8LanguageArrayIndex], 27, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_DIG_POLARITY] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_DIG_POLARITY), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_DIG_POLARITY], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_POLARITY], 2, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_DIG_ACTION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_DIG_ACTION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_DIG_ACTION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_DIG_ACTIVATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_DIG_ACTIVATION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_DIG_ACTIVATION], "", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTIVATION], 4, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_DIG_ACTIVATION_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_DIG_ACTIVATION_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_DIG_ACTIVATION_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_HIGH_TEMP_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_HIGH_TEMP_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_HIGH_TEMP_THRESHOLD], "%/Hour", "%u",(float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_LOW_TEMP_THRESHOLD] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_LOW_TEMP_THRESHOLD), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_LOW_TEMP_THRESHOLD], "%/Hour", "%u",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_HIGH_TEMP_MON_DELAY] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_HIGH_TEMP_MON_DELAY), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_HIGH_TEMP_MON_DELAY], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_DG_RUN_DURATION] = CEditableItem((uint8_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_DG_RUN_DURATION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_DG_RUN_DURATION], arrUnit[ID_SEC], "%u", (uint8_t)0, (uint8_t)60, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_OPEN_CKT_NOTIFICATION] = CEditableItem((uint32_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_OPEN_CKT_NOTIFICATION), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_OPEN_CKT_NOTIFICATION],"", "%s",  strOptions[_u8LanguageArrayIndex][ID_DIG_IP_ACTION], 5, CEditableItem::PIN1_PIN2_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_R1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_R1), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_R1], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_T1] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_T1), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_T1],arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_R2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_R2), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_R2], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_T2] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_T2), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_T2], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_R3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_R3), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_R3],arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_T3] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_T3), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_T3], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_R4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_R4), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_R4], arrUnit[ID_OHM], "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_T4] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_T4), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_T4], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_R5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_R5), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_R5], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_T5] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_T5), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_T5], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_R6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_R6), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_R6], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_T6] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_T6), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_T6], arrUnit[ID_DEG_C],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_R7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_R7), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_R7], arrUnit[ID_OHM],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_T7] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_T7), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_T7], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_R8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_R8), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_R8], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1,CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_T8] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_T8), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_T8], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_R9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_R9), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_R9], arrUnit[ID_OHM],  "%f",(float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_T9] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_T9), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_T9], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_R10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_R10), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_R10], arrUnit[ID_OHM],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );
    ArrEditableItem[INDEX_OF_SHEL_TEMP_DIG_N_T10] = CEditableItem((float)_objcfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_N_T10), strLeafNode[_u8LanguageArrayIndex][SID_SHEL_TEMP_DIG_N_T10], arrUnit[ID_DEG_C],  "%f", (float)0, (float)5000.0,(float)0.1, CEditableItem::PIN1_ALLOWED );

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
    ArrEditableItem[INDEX_OF_GENERAL_TIMER__DEEP_SLP_MODE_DELAY] = CEditableItem((uint16_t)_objcfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER__DEEP_SLP_MODE_DELAY),strLeafNode[_u8LanguageArrayIndex][SID_GENERAL_TIMER__DEEP_SLP_MODE_DELAY], arrUnit[ID_SEC], "%u", (uint16_t)5, (uint16_t)1800, CEditableItem::PIN1_ALLOWED );
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

        for(int i=INDEX_OF_GENERAL_PROFILE_NAME; i<=INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE_3;i++)
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
   /*     if(!CFGC::IsSGC421())
        {
            menuItemsMidLevel[ID_EGOV_GENERAL].isEnabled = false;
            menuItemsMidLevel[ID_ENG_START_STRGY].isEnabled = false;
            menuItemsMidLevel[ID_GEN_EGOV_CNFG].isEnabled = false;
            menuItemsMidLevel[ID_STR_STP_CNFG].isEnabled = false;
        }*/

        //MAainMenu
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

//RushiStart
uint16_t id_float = (CFGZ::ID_BTS_CONFIG_LOW_BATT_THRESHOLD);
uint16_t id_uint16 = (CFGZ::ID_BTS_CONFIG_LOW_BATT_MON_DELAY);
uint16_t id_uint8 = (CFGZ::ID_GENERAL_PROFILE_NAME);
for (uint16_t i = 0 ; i<INDEX_OF_ENG_SR_NO ; i++)
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
    else if(ArrEditableItem[i].dataType == CEditableItem::DT_UINT8)
    {
        AllParam.u8ArrParam[id_uint8] = (uint8_t)ArrEditableItem[i].value.u8Val;
        id_uint8++;
    }
    else if(ArrEditableItem[i].dataType == CEditableItem::DT_STRING_FIXED)
    {
        AllParam.u8ArrParam[id_uint8] = (uint8_t)ArrEditableItem[i].value.u8Val;
        id_uint8++;
    }
    else
    {

    }

}
//RushiEnd
//RushiStart

//RushiEnd


/*//RushiStart

//RushiEnd*/

            u8Month = (uint8_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE_1].value.stDate.u8Month;
            u16Year = (uint16_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE_1].value.stDate.u16Year;
            u8Date = (uint8_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE_1].value.stDate.u8Date;
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
                    AllParam.u8ArrParam[CFGZ::ID_FILT_MAINT_THRESH_DAY] = (uint8_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE_1].value.stDate.u8Date;
                    AllParam.u8ArrParam[CFGZ::ID_FILT_MAINT_THRESH_MONTH] = (uint8_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE_1].value.stDate.u8Month;
                    AllParam.u16ArrParam[CFGZ:: ID_FILT_MAINT_THRESH_YEAR] = (uint16_t)ArrEditableItem[INDEX_OF_ALARM_DUE_DATE_SERVICE_DATE_1].value.stDate.u16Year;
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
/*//RushiStart

//RushiEnd*/


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
  /*  bool bEn_Ds=true;

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
    }*/
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
/*
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
*/
}



uint16_t UI::prvMaxDaysInMonth(uint8_t u8Month ,uint16_t u16Year)
{
    if(u8Month == 2)
    {
        return IsLeapYear(u16Year)?(DaysInMonth[u8Month - 1]) : (DaysInMonth[u8Month - 1] - 1);
    }

    return (DaysInMonth[u8Month - 1]);
}
