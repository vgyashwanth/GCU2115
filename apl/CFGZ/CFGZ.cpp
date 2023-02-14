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

#include "../../apl/CFGZ/CFGZ.h"
#include "../../apl/config.h"

CFGZ *pcfgz;

EEPROM::EVENTS_t CFGZ::_eEvent=EEPROM::NO_STATUS;


CFGZ::CFGZ(HAL_Manager &hal, MODBUS &modbus, CFGC &cfgc):
_hal(hal),
_modbus(modbus),
_cfgc(cfgc),
_All_Param{0},
_EraseAndWriteInitiated(false),
strCFGZMetadata{0},
_bDflashCallbackRcvd(false),
_bConfigWritten(true),
_stMiscParam{0}
{
    pcfgz = this;
    if(prvGetIntegrityStatus() == BSP_SUCCESS)
    {
        _CFGZ_Verification_status = BSP_SUCCESS;
    }
    else
    {
        ReadFactoryProfile();
        _CFGZ_Verification_status = BSP_SUCCESS;
    }
    ApplyConfigChanges();
    prvSetPassword();
    ReadMiscParam(&_stMiscParam);
}

bool CFGZ::IsC03Error()
{
    return (_CFGZ_Verification_status == BSP_FAIL);
}

BSP_STATUS_t CFGZ::ReadCFGZFromEEprom(CFGZ_PARAMS_t* Param)
{
    return (_hal.Objeeprom.BlockingRead(EXT_EEPROM_ACTIVE_PROFILE_START_ADDRESS,
              (uint8_t*)Param,  sizeof(CFGZ_PARAMS_t)));
}

void CFGZ::ReadMiscParam(MISC_PARAM_t *stParam)
{
    _hal.Objeeprom.BlockingRead(EXT_EEPROM_PASWORD_START_ADDRESS,
                                (uint8_t*)stParam,  sizeof(MISC_PARAM_t));
}
float  CFGZ::GetCFGZ_Param(FLOAT_PARAMS_t _pf32Param)
{
    return(_All_Param.f32ArrParam[_pf32Param]);
}

uint16_t  CFGZ::GetCFGZ_Param(UINT16_PARAMS_t _u16Paramm)
{
    return(_All_Param.u16ArrParam[_u16Paramm]);
}

uint8_t  CFGZ::GetCFGZ_Param(UINT8_PARAMS_t _u8Param)
{
   return(_All_Param.u8ArrParam[_u8Param]);
}

void  CFGZ::GetCFGZ_Param(ARRAY_PARAMS_t _ArrParam, char *pu8ArrValue)
{
    if(pu8ArrValue != NULL)
    {
        memcpy((void*)pu8ArrValue, &_All_Param.u8ArrStringParam[_ArrParam],
               MAX_AUX_STRING_SIZE);
    }
}
uint16_t CFGZ::GetMisc_Param(MISC_PARAM_16_t ePram)
{
    return _stMiscParam.u16MiscParam[ePram] ;
}

uint8_t CFGZ::GetMisc_Param(MISC_PARAM_8_t ePram)
{
    return _stMiscParam.u8MiscParam[ePram];
}

BSP_STATUS_t CFGZ::WriteActiveProfile(CFGZ_PARAMS_t* _pAllparams)
{
    _bConfigWritten = false;

    memcpy((uint8_t*)&_All_Param, _pAllparams, sizeof(CFGZ_PARAMS_t) );

    uint16_t u16CRC = CRC16::ComputeCRCGeneric((uint8_t*)&_All_Param,
                                               sizeof(CFGZ_PARAMS_t) - (sizeof(uint8_t)*DUMMY_ITEMS),
                                               CRC_MEMORY_SEED);
    strCFGZMetadata.u16CFGZCrc = u16CRC;
    strCFGZMetadata.u16Reserved1 = 0xFFFF;
    strCFGZMetadata.u32HighestAddrUsedByCFGZ = EXT_EEPROM_ACTIVE_PROFILE_START_ADDRESS+ sizeof(CFGZ_PARAMS_t)-1
            - sizeof(uint8_t)*DUMMY_ITEMS;

    _hal.ObjRTC.GetTime(&strCFGZMetadata.timeOfFlashing);

    _hal.Objeeprom.RequestWrite((EXT_EEPROM_ACTIVE_PROFILE_START_ADDRESS+ EXT_EEPROM_ACTIVE_PROFILE_LENGTH
            - sizeof(CFGZ_METADATA_t)) , (uint8_t*)&strCFGZMetadata, sizeof(CFGZ_METADATA_t),NULL);
    _eEvent = EEPROM::NO_STATUS;
    _hal.Objeeprom.RequestWrite(EXT_EEPROM_ACTIVE_PROFILE_START_ADDRESS, (uint8_t*)&_All_Param, sizeof(CFGZ_PARAMS_t),EEpromWritCB);

    return BSP_SUCCESS;
}

void CFGZ::EEpromWritCB(EEPROM::EVENTS_t eEvent)
{
    _eEvent = eEvent;
}

BSP_STATUS_t CFGZ::prvGetIntegrityStatus(void)
{
    /*
     *This function can check integrity of firmware, profiles, active profile
     *Pass the corresponding area start and end addresses.
     *
     *Read Profile form External EEprom if CRC not matches then read profile from D-flash.
     */
    CFGZ_METADATA_t stCFGZMetadata;
    _hal.Objeeprom.BlockingRead((EXT_EEPROM_ACTIVE_PROFILE_START_ADDRESS + EXT_EEPROM_ACTIVE_PROFILE_LENGTH - sizeof(CFGZ_METADATA_t)), (uint8_t*)&stCFGZMetadata,
                                sizeof(CFGZ_METADATA_t));
    ReadCFGZFromEEprom(&_All_Param);
    if(CRC16::ComputeCRCGeneric((uint8_t *)&_All_Param, sizeof(CFGZ_PARAMS_t) - (sizeof(uint8_t)*DUMMY_ITEMS)
                                , CRC_MEMORY_SEED) != stCFGZMetadata.u16CFGZCrc)
    {
        return BSP_FAIL;
    }

    return BSP_SUCCESS;   // active profile is valid
}

void CFGZ::Update(void)
{
    if(_eEvent == EEPROM::WRITE_COMPLETE )
    {
        _eEvent = EEPROM::NO_STATUS;
        if(prvGetIntegrityStatus() == BSP_SUCCESS)
        {
            ReadCFGZFromEEprom(&_All_Param);
            ReadMiscParam(&_stMiscParam);
            _bConfigWritten = true;
        }
        else
        {
            _bConfigWritten = false;
        }
    }
}

BSP_STATUS_t CFGZ::GetCFGZ_VerificationStatus(void)
{
    return _CFGZ_Verification_status;
}

bool CFGZ::IsConfigWritten()
{
    return  _bConfigWritten;
}

void CFGZ::ReadFactoryProfile()
{
    _hal.Objpflash.Read(FACTORY_CFGZ_ADDRESS,
                        (uint8_t*)&_All_Param,  sizeof(CFGZ_PARAMS_t));
}

void CFGZ::prvConfigureDSENSE()
{
    D_SENSE::CFG_t cfg;

    cfg.stDIConfig[D_SENSE::DI_A].eType                     =
            prvGetDigitalSensor(ID_DIG_IN_A_SOURCE, D_SENSE::DI_A);
    cfg.stDIConfig[D_SENSE::DI_A].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_DIG_IN_A_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_A].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_DIG_IN_A_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_B].eType                     =
            prvGetDigitalSensor(ID_DIG_IN_B_SOURCE, D_SENSE::DI_B);
    cfg.stDIConfig[D_SENSE::DI_B].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_DIG_IN_B_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_B].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_DIG_IN_B_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_C].eType                     =
            prvGetDigitalSensor(ID_DIG_IN_C_SOURCE, D_SENSE::DI_C);
    cfg.stDIConfig[D_SENSE::DI_C].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_DIG_IN_C_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_C].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_DIG_IN_C_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_D].eType                     =
            prvGetDigitalSensor(ID_DIG_IN_D_SOURCE, D_SENSE::DI_D);
    cfg.stDIConfig[D_SENSE::DI_D].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_DIG_IN_D_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_D].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_DIG_IN_D_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_E].eType                     =
            prvGetDigitalSensor(ID_DIG_IN_E_SOURCE, D_SENSE::DI_E);
    cfg.stDIConfig[D_SENSE::DI_E].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_DIG_IN_E_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_E].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_DIG_IN_E_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_F].eType                     =
            prvGetDigitalSensor(ID_DIG_IN_F_SOURCE, D_SENSE::DI_F);
    cfg.stDIConfig[D_SENSE::DI_F].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_DIG_IN_F_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_F].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_DIG_IN_F_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_G].eType                     =
            prvGetDigitalSensor(ID_DIG_IN_G_SOURCE, D_SENSE::DI_G);
    cfg.stDIConfig[D_SENSE::DI_G].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_DIG_IN_G_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_G].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_DIG_IN_G_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_H].eType                     =
            prvGetDigitalSensor(ID_DIG_IN_H_SOURCE, D_SENSE::DI_H);
    cfg.stDIConfig[D_SENSE::DI_H].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_DIG_IN_H_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_H].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_DIG_IN_H_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_I].eType                     =
            prvGetDigitalSensor(ID_DIG_IN_I_SOURCE, D_SENSE::DI_I);
    cfg.stDIConfig[D_SENSE::DI_I].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_DIG_IN_I_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_I].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_DIG_IN_I_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_J].eType                     =
            prvGetDigitalSensor(ID_LOP_RES_DIG_J_DIG_SOURCE, D_SENSE::DI_J);
    cfg.stDIConfig[D_SENSE::DI_J].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_LOP_RES_DIG_J_DIG_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_J].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY];


    cfg.stDIConfig[D_SENSE::DI_K].eType                     =
            prvGetDigitalSensor(ID_FUEL_LVL_DIG_K_DIG_SOURCE, D_SENSE::DI_K);
    cfg.stDIConfig[D_SENSE::DI_K].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_FUEL_LVL_DIG_K_DIG_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_K].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY];


    cfg.stDIConfig[D_SENSE::DI_L].eType                     =
            prvGetDigitalSensor(ID_ENG_TEMP_DIG_L_DIG_SOURCE, D_SENSE::DI_L);
    cfg.stDIConfig[D_SENSE::DI_L].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_ENG_TEMP_DIG_L_DIG_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_L].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY];


    cfg.stDIConfig[D_SENSE::DI_M].eType                     =
            prvGetDigitalSensor(ID_SHEL_TEMP_DIG_M_DIG_SOURCE, D_SENSE::DI_M);
    cfg.stDIConfig[D_SENSE::DI_M].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_SHEL_TEMP_DIG_M_DIG_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_M].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_SHEL_TEMP_DIG_M_DIG_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_N].eType                     =
            prvGetDigitalSensor(ID_AUX_S2_RES_DIG_N_DIG_SOURCE, D_SENSE::DI_N);
    cfg.stDIConfig[D_SENSE::DI_N].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_AUX_S2_RES_DIG_N_DIG_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_N].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_O].eType                     =
            prvGetDigitalSensor(ID_AUX_S3_DIG_O_DIG_SOURCE, D_SENSE::DI_O);
    cfg.stDIConfig[D_SENSE::DI_O].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_AUX_S3_DIG_O_DIG_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_O].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY];

    cfg.stDIConfig[D_SENSE::DI_P].eType                     =
            prvGetDigitalSensor(ID_AUX_S4_DIG_P_DIG_SOURCE, D_SENSE::DI_P);
    cfg.stDIConfig[D_SENSE::DI_P].eActivationPolarity       =
            (DigitalSensor::ACTIV_POLARITY_t)_All_Param.u8ArrParam[ID_AUX_S4_DIG_P_DIG_POLARITY];
    cfg.stDIConfig[D_SENSE::DI_P].u16ActivationDelaySeconds =
            (uint8_t)_All_Param.u8ArrParam[ID_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY];

    _hal.DigitalSensors.ConfigureSensor(cfg);
}

DigitalSensor::D_SENSOR_TYPS_t CFGZ::prvGetDigitalSensor(uint8_t u8CfgSensorIdx, D_SENSE::INPUTS_t eInput)
{
    typedef struct
    {
        CFGZ_DIGITAL_SENSORS_t         eCfgSensorTyp;
        DigitalSensor::D_SENSOR_TYPS_t eDsenseSensorTyp;
    } DSENSOR_MAP_ROW_t;

    const DSENSOR_MAP_ROW_t stSensorMap[] =
    {
     { CFGZ_SENSOR_NOT_USED                          , DigitalSensor:: DI_NOT_CONFIGURED              },
     { CFGZ_LOW_FUEL_LEVEL_SWITCH                    , DigitalSensor:: DI_LOW_FUEL_LEVEL_SWITCH       },
     { CFGZ_LOW_LUBE_OIL_PRESS_SWITCH                , DigitalSensor:: DI_LOW_LUBE_OIL_PRESSURE_SWITCH},
     { CFGZ_HIGH_ENGINE_TEMP_SWITCH                  , DigitalSensor:: DI_HIGH_ENGINE_TEMP_SWITCH     },
     { CFGZ_LOW_WATER_LVL_SWITCH                     , DigitalSensor:: DI_LOW_WATER_LEVEL_SWITCH      },
     { CFGZ_EMERGENCY_STOP                           , DigitalSensor:: DI_EMERGENCY_STOP              },
     { CFGZ_REMOTE_START_STOP                        , DigitalSensor:: DI_REMOTE_START_STOP           },
     { CFGZ_SIMULATE_START                           , DigitalSensor:: DI_SIMULATE_START              },
     { CFGZ_SIMULATE_STOP                            , DigitalSensor:: DI_SIMULATE_STOP               },
     { CFGZ_SIMULATE_AUTO                            , DigitalSensor:: DI_SIMULATE_AUTO               },
     { CFGZ_CLOSE_GEN_OPEN_MAINS_SWITCH              , DigitalSensor:: DI_CLOSE_GEN_OPEN_MAINS_SWITCH },
     { CFGZ_CLOSE_MAINS_OPEN_GEN_SWITCH              , DigitalSensor:: DI_CLOSE_MAINS_OPEN_GEN_SWITCH },
     { CFGZ_SIMULATE_MAINS                           , DigitalSensor:: DI_SIMULATE_MAINS              },
     { CFGZ_VBEL_BROKEN_SWITCH                       , DigitalSensor:: DI_V_BELT_BROKEN_SWITCH        },
     { CFGZ_MAINS_CONTACTOR_LATCHED                  , DigitalSensor:: DI_MAINS_CONTACTOR_LATCHED     },
     { CFGZ_GEN_CONTACTOR_LATCHED                    , DigitalSensor:: DI_GEN_CONTACTOR_LATCHED       },
     { CFGZ_BATTERY_CHARGER_FAIL                     , DigitalSensor:: DI_BATTERY_CHARGER_FAIL        },
     { CFGZ_SMOKE_FIRE                               , DigitalSensor:: DI_SMOKE_FIRE                  },
     { CFGZ_MODE_SELECT                              , DigitalSensor:: DI_MODE_SELECT                 },
     { CFGZ_AMB_TEMP_SELECT                          , DigitalSensor:: DI_AMB_TEMP_SELECT             },
    };

    /*Configurable input types*/
    DigitalSensor::D_SENSOR_TYPS_t aeCfgIpTypes[] =
    {
     DigitalSensor::DI_A_USER_CONFIGURED,
     DigitalSensor::DI_B_USER_CONFIGURED,
     DigitalSensor::DI_C_USER_CONFIGURED,
     DigitalSensor::DI_D_USER_CONFIGURED,
     DigitalSensor::DI_E_USER_CONFIGURED,
     DigitalSensor::DI_F_USER_CONFIGURED,
     DigitalSensor::DI_G_USER_CONFIGURED,
     DigitalSensor::DI_H_USER_CONFIGURED,
     DigitalSensor::DI_I_USER_CONFIGURED,
     DigitalSensor::DI_J_USER_CONFIGURED,
     DigitalSensor::DI_K_USER_CONFIGURED,
     DigitalSensor::DI_L_USER_CONFIGURED,
     DigitalSensor::DI_M_USER_CONFIGURED,
     DigitalSensor::DI_N_USER_CONFIGURED,
     DigitalSensor::DI_O_USER_CONFIGURED,
     DigitalSensor::DI_P_USER_CONFIGURED
    };
    if(_All_Param.u8ArrParam[u8CfgSensorIdx] == CFGZ_USER_CONFIGURED_SENSOR)
    {
        return aeCfgIpTypes[eInput];
    }

    for(uint8_t i=0; i<(sizeof(stSensorMap)/sizeof(DSENSOR_MAP_ROW_t)); i++)
    {
        if(stSensorMap[i].eCfgSensorTyp == _All_Param.u8ArrParam[u8CfgSensorIdx])
        {
            return stSensorMap[i].eDsenseSensorTyp;
        }
    }
    return DigitalSensor::DI_NOT_CONFIGURED;
}

void CFGZ::prvConfigureASENSE()
{
    A_SENSE::CFG_t cfg;
    uint8_t u8MapSize = 0;

    //PIN 11 Configuration
    /*Map between the sensor names defined in A_SENSE and CFGZ*/
    const ASENSOR_MAP_ROW_t aPIN11MAP [] =
    {
     {CFGZ_ANLG_SENSOR_NOT_USED , AnalogSensor::A_SENSE_NOT_USED},
     {CFGZ_ANLG_DIG_IN          , AnalogSensor::A_SENSE_DIG_J   },
     {CFGZ_ANLG_CUSTOM_SENSOR1  , AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE}
    };

    u8MapSize = sizeof(aPIN11MAP)/sizeof(ASENSOR_MAP_ROW_t);
    cfg.stAIConfig[A_SENSE::HAL_PIN_11].eSensor = prGetAnalogSensor(ID_LOP_RES_DIG_J_SENSOR_SELECTION, aPIN11MAP, u8MapSize);
    cfg.stAIConfig[A_SENSE::HAL_PIN_11].eRef    = ANLG_IP::REF_ENGINE_BODY;
    prvCpyInterpolationTable(ID_LOP_RES_DIG_J_R1, cfg.stAIConfig[A_SENSE::HAL_PIN_11].stTable);
    cfg.stAIConfig[A_SENSE::HAL_PIN_11].stTable.u8InterPolationPoints = 10;

    //PIN 12 Configuration
    /*Map between the sensor names defined in A_SENSE and CFGZ*/
    const ASENSOR_MAP_ROW_t aPIN12MAP [] =
    {
     {CFGZ_ANLG_SENSOR_NOT_USED , AnalogSensor::A_SENSE_NOT_USED},
     {CFGZ_ANLG_DIG_IN          , AnalogSensor::A_SENSE_DIG_K   },
     {CFGZ_ANLG_CUSTOM_SENSOR1  , AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE}
    };

    u8MapSize = sizeof(aPIN12MAP)/sizeof(ASENSOR_MAP_ROW_t);
    cfg.stAIConfig[A_SENSE::HAL_PIN_12].eSensor = prGetAnalogSensor(ID_FUEL_LVL_DIG_K_SENSOR_SELECTION, aPIN12MAP,u8MapSize);
    cfg.stAIConfig[A_SENSE::HAL_PIN_12].eRef    = ANLG_IP::REF_ENGINE_BODY;
    prvCpyInterpolationTable(ID_FUEL_LVL_DIG_K_R1, cfg.stAIConfig[A_SENSE::HAL_PIN_12].stTable);
    cfg.stAIConfig[A_SENSE::HAL_PIN_12].stTable.u8InterPolationPoints = 10;

    //PIN 13 Configuration
    /*Map between the sensor names defined in A_SENSE and CFGZ*/
    const ASENSOR_MAP_ROW_t aPIN13MAP [] =
    {
     {CFGZ_ANLG_SENSOR_NOT_USED , AnalogSensor::A_SENSE_NOT_USED},
     {CFGZ_ANLG_DIG_IN          , AnalogSensor::A_SENSE_DIG_L   },
     {CFGZ_ANLG_CUSTOM_SENSOR1  , AnalogSensor::A_SENSE_ENG_TEMPERATURE}
    };

    u8MapSize = sizeof(aPIN13MAP)/sizeof(ASENSOR_MAP_ROW_t);
    cfg.stAIConfig[A_SENSE::HAL_PIN_13].eSensor = prGetAnalogSensor(ID_ENG_TEMP_DIG_L_SENSOR_SELECTION, aPIN13MAP,u8MapSize);
    cfg.stAIConfig[A_SENSE::HAL_PIN_13].eRef    = ANLG_IP::REF_ENGINE_BODY;
    prvCpyInterpolationTable(ID_ENG_TEMP_DIG_L_R1, cfg.stAIConfig[A_SENSE::HAL_PIN_13].stTable);
    cfg.stAIConfig[A_SENSE::HAL_PIN_13].stTable.u8InterPolationPoints = 10;

    //PIN 14 Configuration
    /*Map between the sensor names defined in A_SENSE and CFGZ*/
    const ASENSOR_MAP_ROW_t aPIN14MAP [] =
    {
     {CFGZ_ANLG_SENSOR_NOT_USED , AnalogSensor::A_SENSE_NOT_USED},
     {CFGZ_ANLG_DIG_IN          , AnalogSensor::A_SENSE_DIG_M   },
     {CFGZ_ANLG_CUSTOM_SENSOR1  , AnalogSensor::A_SENSE_SHELTER_TEMPERATURE}
    };

    u8MapSize = sizeof(aPIN14MAP)/sizeof(ASENSOR_MAP_ROW_t);
    cfg.stAIConfig[A_SENSE::HAL_PIN_14].eSensor = prGetAnalogSensor(ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION, aPIN14MAP,u8MapSize);
    cfg.stAIConfig[A_SENSE::HAL_PIN_14].eRef    = ANLG_IP::REF_ENGINE_BODY;
    prvCpyInterpolationTable(ID_SHEL_TEMP_DIG_M_R1, cfg.stAIConfig[A_SENSE::HAL_PIN_14].stTable);
    cfg.stAIConfig[A_SENSE::HAL_PIN_14].stTable.u8InterPolationPoints = 10;

    //PIN 15 Configuration
    /*Map between the sensor names defined in A_SENSE and CFGZ*/
    const ASENSOR_MAP_ROW_t aPIN15MAP [] =
    {
     {CFGZ_ANLG_SENSOR_NOT_USED , AnalogSensor::A_SENSE_NOT_USED},
     {CFGZ_ANLG_DIG_IN          , AnalogSensor::A_SENSE_DIG_N   },
     {CFGZ_ANLG_CUSTOM_SENSOR1  , AnalogSensor::A_SENSE_S2_SENSOR}
    };

    u8MapSize = sizeof(aPIN15MAP)/sizeof(ASENSOR_MAP_ROW_t);
    cfg.stAIConfig[A_SENSE::HAL_PIN_15].eSensor = prGetAnalogSensor(ID_AUX_S2_RES_DIG_N_SENSOR_SELECTION, aPIN15MAP,u8MapSize);
    cfg.stAIConfig[A_SENSE::HAL_PIN_15].eRef    = ANLG_IP::REF_ENGINE_BODY;
    prvCpyInterpolationTable(ID_AUX_S2_RES_DIG_N_R1, cfg.stAIConfig[A_SENSE::HAL_PIN_15].stTable);
    cfg.stAIConfig[A_SENSE::HAL_PIN_15].stTable.u8InterPolationPoints = 10;

    //PIN 21 Configuration
    /*Map between the sensor names defined in A_SENSE and CFGZ*/
    const ASENSOR_MAP_ROW_t aPIN21MAP [] =
    {
     {CFGZ_ANLG_SENSOR_NOT_USED , AnalogSensor::A_SENSE_NOT_USED},
     {CFGZ_ANLG_DIG_IN          , AnalogSensor::A_SENSE_DIG_O   },
     {CFGZ_ANLG_CUSTOM_SENSOR1  , AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE_4_20},
     {CFGZ_ANLG_CUSTOM_SENSOR2  , AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE_0_TO_5V}
    };

    u8MapSize = sizeof(aPIN21MAP)/sizeof(ASENSOR_MAP_ROW_t);
    cfg.stAIConfig[A_SENSE::HAL_PIN_21].eSensor = prGetAnalogSensor(ID_AUX_S3_DIG_O_SENSOR_SELECTION, aPIN21MAP,u8MapSize);
    cfg.stAIConfig[A_SENSE::HAL_PIN_21].eRef    = ANLG_IP::REF_ENGINE_BODY;
    prvCpyInterpolationTable(ID_AUX_S3_DIG_O_I1_V1, cfg.stAIConfig[A_SENSE::HAL_PIN_21].stTable);
    cfg.stAIConfig[A_SENSE::HAL_PIN_21].stTable.u8InterPolationPoints = 10;


    //PIN 23 Configuration
    /*Map between the sensor names defined in A_SENSE and CFGZ*/
    const ASENSOR_MAP_ROW_t aPIN23MAP [] =
    {
     {CFGZ_ANLG_SENSOR_NOT_USED , AnalogSensor::A_SENSE_NOT_USED},
     {CFGZ_ANLG_DIG_IN          , AnalogSensor::A_SENSE_DIG_P   },
     {CFGZ_ANLG_CUSTOM_SENSOR1  , AnalogSensor::A_SENSE_FUEL_LEVEL_0_TO_5V}
    };

    u8MapSize = sizeof(aPIN23MAP)/sizeof(ASENSOR_MAP_ROW_t);
    cfg.stAIConfig[A_SENSE::HAL_PIN_23].eSensor = prGetAnalogSensor(ID_AUX_S4_DIG_P_SENSOR_SELECTION, aPIN23MAP,u8MapSize);
    cfg.stAIConfig[A_SENSE::HAL_PIN_23].eRef    = ANLG_IP::REF_ENGINE_BODY;
//    prvCpyInterpolationTable(ID_AUX_S4_DIG_P_SENSOR_HIGH_VTG, cfg.stAIConfig[A_SENSE::HAL_PIN_23].stTable);
//    cfg.stAIConfig[A_SENSE::HAL_PIN_23].stTable.u8InterPolationPoints = 10;

     cfg.stAIConfig[A_SENSE::HAL_PIN_23].stTable.af32IntrpolationTableX[0] = _All_Param.f32ArrParam[ID_AUX_S4_DIG_P_SENSOR_HIGH_VTG];
     cfg.stAIConfig[A_SENSE::HAL_PIN_23].stTable.af32IntrpolationTableX[1] = _All_Param.f32ArrParam[ID_AUX_S4_DIG_P_SENSOR_LOW_VTG];
     if(_All_Param.u8ArrParam[ID_AUX_S4_DIG_P_TANK_WITH_STEP] == CFGZ_ENABLE)
     {
         cfg.stAIConfig[A_SENSE::HAL_PIN_23].stTable.af32IntrpolationTableY[0] = (float)(_All_Param.u16ArrParam[ID_AUX_S4_DIG_P_TANK_HEIGHT_1] + _All_Param.u16ArrParam[ID_AUX_S4_DIG_P_TANK_HEIGHT_2]);
     }
     else
     {
         cfg.stAIConfig[A_SENSE::HAL_PIN_23].stTable.af32IntrpolationTableY[0] = (float)(_All_Param.u16ArrParam[ID_AUX_S4_DIG_P_TANK_HEIGHT_1]);
     }
     cfg.stAIConfig[A_SENSE::HAL_PIN_23].stTable.af32IntrpolationTableY[1] = 0.0f;

     A_SENSE::FUEL_0_5V_t Fuel_0_5V;

     Fuel_0_5V.TankwithStep = _All_Param.u8ArrParam[ID_AUX_S4_DIG_P_TANK_WITH_STEP];
     Fuel_0_5V.TankWidth = _All_Param.u16ArrParam[ID_AUX_S4_DIG_P_TANK_WIDTH];
     Fuel_0_5V.TankStep1Length = _All_Param.u16ArrParam[ID_AUX_S4_DIG_P_TANK_LENGTH_1];
     Fuel_0_5V.TankStep2Length = _All_Param.u16ArrParam[ID_AUX_S4_DIG_P_TANK_LENGTH_2];
     Fuel_0_5V.TankStep1Height = _All_Param.u16ArrParam[ID_AUX_S4_DIG_P_TANK_HEIGHT_1];
     Fuel_0_5V.TankStep2Height = _All_Param.u16ArrParam[ID_AUX_S4_DIG_P_TANK_HEIGHT_2];

     _hal.AnalogSensors.ConfigureFuel0_5V_SensorValue(Fuel_0_5V);


     cfg.stAIConfig[A_SENSE::HAL_PIN_23].stTable.u8InterPolationPoints = 2;

    _hal.AnalogSensors.ConfigureSensor(cfg);

    //Following calculation is for config value enum to the number.
    _hal.AnalogSensors.ConfigureNumberOfPoles((uint8_t)((_All_Param.u8ArrParam[ID_ALT_CONFIG_NUMBER_OF_POLES] * 2) + 2));
}

AnalogSensor::TYPS_t CFGZ::prGetAnalogSensor(uint8_t u8CfgSensorIdx,
                                        const ASENSOR_MAP_ROW_t *pMap, uint8_t u8MapSize)
{
    for(int i=0; i<u8MapSize; i++)
    {
        if(pMap[i].eCfgSensorTyp == _All_Param.u8ArrParam[u8CfgSensorIdx])
        {
            return pMap[i].eDsenseSensorTyp;
        }
    }
    return AnalogSensor::A_SENSE_NOT_USED;
}

void CFGZ::prvCpyInterpolationTable(FLOAT_PARAMS_t eTableStart,
                                    AnalogSensor::INTERPOLATE_INFO_t &stDestination)
{
    uint8_t u8NoOfPoints = sizeof(stDestination.af32IntrpolationTableX)/sizeof(float);
    /*Copy all 'x' & 'y' points */
    for(int i=0; i<u8NoOfPoints; i++)
    {
        if((eTableStart+(i*2)) + 1 < 150)
        {
            stDestination.af32IntrpolationTableX[i] = _All_Param.f32ArrParam[eTableStart+(i*2)];
            stDestination.af32IntrpolationTableY[i] = _All_Param.f32ArrParam[eTableStart+(i*2)+1];
        }
    }
}

//Unused function
void CFGZ::GetExpInterpolationTable(float *af32TableX, float *af32TableY, FLOAT_PARAMS_t eTableStart)
{
    for(int i=0; i<10; i++)
    {
        af32TableX[i] = _All_Param.f32ArrParam[eTableStart+(i*2)];
        af32TableY[i] = _All_Param.f32ArrParam[eTableStart+(i*2)+1];
    }
}

//Unused function
bool CFGZ::IsDigOutputConfigured(UINT8_PARAMS_t eDigitalOutput)
{
    if(_All_Param.u8ArrParam[eDigitalOutput] != CFGZ_NOT_CONFIGURED)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CFGZ::prvConfigureACT()
{
    ACT_Manager::CFG_t cfg;
    uint8_t u8SourceIdx     = ID_OUT_A_SOURCE;
    for(uint8_t u8Idx=0; u8Idx<ACT_Manager::OP_END; u8Idx++)
    {
        cfg.properties[u8Idx].eType = prvGetACTType(u8SourceIdx);
        /* <LDRA Phase code 9S> <Assignment operation in expression.: Resolved.>
         * <Verified by: Nikhil Mhaske> <9/9/2021> */

        u8SourceIdx++; //Index got changed to output Activation Type
        if((CFGZ_ACT_ACTIVATION_TYP_t)_All_Param.u8ArrParam[u8SourceIdx] == CFGZ_DENERGIZE_TO_ACTIVATE)
        {
            cfg.properties[u8Idx].eActivation = ACTUATOR::DENERGIZE_TO_ACTIVATE;
        }
        else
        {
            cfg.properties[u8Idx].eActivation = ACTUATOR::ENERGIZE_TO_ACTIVATE;
        }
        u8SourceIdx++; //Index changed to next output source
    }

    _hal.actuators.Configure(cfg);
}

ACTUATOR::ACTUATOR_TYPS_t CFGZ::prvGetACTType(uint8_t u8CfgzActuatorTypeIdx)
{
    ACTUATOR_MAP_ROW_t dsenseMap[] =
    {
     { CFGZ_NOT_CONFIGURED                    , ACTUATOR::ACT_NOT_CONFIGURED             },
     { CFGZ_AUDIBLE_ALARM                     , ACTUATOR::ACT_AUDIBLE_ALARM              },
     { CFGZ_VBAT_OV                           , ACTUATOR::ACT_VBAT_OV                    },
     { CFGZ_VBAT_UV                           , ACTUATOR::ACT_VBAT_UV                    },
     { CFGZ_CA_SHUTDOWN                       , ACTUATOR::ACT_CA_SHUTDOWN                },
     { CFGZ_CA_WARNING                        , ACTUATOR::ACT_CA_WARNING                 },
     { CFGZ_CLOSE_GEN_CONTACTOR               , ACTUATOR::ACT_CLOSE_GEN_CONTACTOR        },
     { CFGZ_CLOSE_MAINS_CONTACTOR             , ACTUATOR::ACT_CLOSE_MAINS_CONTACTOR      },
     { CFGZ_MAINS_FAILURE                     , ACTUATOR::ACT_MAINS_FAILURE              },
     { CFGZ_ALARM                             , ACTUATOR::ACT_ALARM                      },
     { CFGZ_ELEC_TRIP                         , ACTUATOR::ACT_ELEC_TRIP                  },
     { CFGZ_SHUTDOWN                          , ACTUATOR::ACT_SHUTDOWN                   },
     { CFGZ_WARNING                           , ACTUATOR::ACT_WARNING                    },
     { CFGZ_COOLING_ON                        , ACTUATOR::ACT_COOLING_ON                 },
     { CFGZ_DIG_IN_A                          , ACTUATOR::ACT_DIG_IN_A                   },
     { CFGZ_DIG_IN_B                          , ACTUATOR::ACT_DIG_IN_B                   },
     { CFGZ_DIG_IN_C                          , ACTUATOR::ACT_DIG_IN_C                   },
     { CFGZ_DIG_IN_D                          , ACTUATOR::ACT_DIG_IN_D                   },
     { CFGZ_DIG_IN_E                          , ACTUATOR::ACT_DIG_IN_E                   },
     { CFGZ_DIG_IN_F                          , ACTUATOR::ACT_DIG_IN_F                   },
     { CFGZ_DIG_IN_G                          , ACTUATOR::ACT_DIG_IN_G                   },
     { CFGZ_DIG_IN_H                          , ACTUATOR::ACT_DIG_IN_H                   },
     { CFGZ_DIG_IN_I                          , ACTUATOR::ACT_DIG_IN_I                   },
     { CFGZ_DIG_IN_J                          , ACTUATOR::ACT_DIG_IN_J                   },
     { CFGZ_DIG_IN_K                          , ACTUATOR::ACT_DIG_IN_K                   },
     { CFGZ_DIG_IN_L                          , ACTUATOR::ACT_DIG_IN_L                   },
     { CFGZ_DIG_IN_M                          , ACTUATOR::ACT_DIG_IN_M                   },
     { CFGZ_DIG_IN_N                          , ACTUATOR::ACT_DIG_IN_N                   },
     { CFGZ_DIG_IN_O                          , ACTUATOR::ACT_DIG_IN_O                   },
     { CFGZ_DIG_IN_P                          , ACTUATOR::ACT_DIG_IN_P                   },
     { CFGZ_E_STOP                            , ACTUATOR::ACT_E_STOP                     },
     { CFGZ_STOP_SOLENOID                     , ACTUATOR::ACT_STOP_SOLENOID              },
     { CFGZ_FAIL_TO_START                     , ACTUATOR::ACT_FAIL_TO_START              },
     { CFGZ_FAIL_TO_STOP                      , ACTUATOR::ACT_FAIL_TO_STOP               },
     { CFGZ_FUEL_RELAY                        , ACTUATOR::ACT_FUEL_RELAY                 },
     { CFGZ_GEN_AVLBL                         , ACTUATOR::ACT_GEN_AVLBL                  },
     { CFGZ_GEN_R_OV_SHUTDOWN                 , ACTUATOR::ACT_GEN_R_OV_SHUTDOWN          },
     { CFGZ_GEN_R_UV_SHUTDOWN                 , ACTUATOR::ACT_GEN_R_UV_SHUTDOWN          },
     { CFGZ_GEN_Y_OV_SHUTDOWN                 , ACTUATOR::ACT_GEN_Y_OV_SHUTDOWN          },
     { CFGZ_GEN_Y_UV_SHUTDOWN                 , ACTUATOR::ACT_GEN_Y_UV_SHUTDOWN          },
     { CFGZ_GEN_B_OV_SHUTDOWN                 , ACTUATOR::ACT_GEN_B_OV_SHUTDOWN          },
     { CFGZ_GEN_B_UV_SHUTDOWN                 , ACTUATOR::ACT_GEN_B_UV_SHUTDOWN          },
     { CFGZ_GEN_OC                            , ACTUATOR::ACT_GEN_OC                     },
     { CFGZ_HIGH_TEMP                         , ACTUATOR::ACT_HIGH_TEMP                  },
     { CFGZ_LOW_FUEL                          , ACTUATOR::ACT_LOW_FUEL                   },
     { CFGZ_LOW_FUEL_NOTIFICATION             , ACTUATOR::ACT_LOW_FUEL_NOTIFICATION      },
     { CFGZ_LOW_PRES                          , ACTUATOR::ACT_LOW_PRES                   },
     { CFGZ_MAINS_HIGH                        , ACTUATOR::ACT_MAINS_HIGH                 },
     { CFGZ_MAINS_LOW                         , ACTUATOR::ACT_MAINS_LOW                  },
     { CFGZ_OIL_CKT_OPEN                      , ACTUATOR::ACT_OIL_CKT_OPEN               },
     { CFGZ_OPEN_GEN_OUT                      , ACTUATOR::ACT_OPEN_GEN_OUT               },
     { CFGZ_OPEN_MAINS_OUT                    , ACTUATOR::ACT_OPEN_MAINS_OUT             },
     { CFGZ_OF_SHUTDOWN                       , ACTUATOR::ACT_OF_SHUTDOWN                },
     { CFGZ_OS_SHUTDOWN                       , ACTUATOR::ACT_OS_SHUTDOWN                },
     { CFGZ_GROSS_OS_SHUTDOWN                 , ACTUATOR::ACT_GROSS_OS_SHUTDOWN          },
     { CFGZ_START_RELAY                       , ACTUATOR::ACT_START_RELAY                },
     { CFGZ_TEMP_CKT_OPEN                     , ACTUATOR::ACT_TEMP_CKT_OPEN              },
     { CFGZ_UF_SHUTDOWN                       , ACTUATOR::ACT_UF_SHUTDOWN                },
     { CFGZ_US_SHUTDOWN                       , ACTUATOR::ACT_US_SHUTDOWN                },
     { CFGZ_FILT_MAINT                        , ACTUATOR::ACT_FILT_MAINT                 },
     { CFGZ_MODE_STOP                         , ACTUATOR::ACT_MODE_STOP                  },
     { CFGZ_MODE_AUTO                         , ACTUATOR::ACT_MODE_AUTO                  },
     { CFGZ_MODE_MANUAL                       , ACTUATOR::ACT_MODE_MANUAL                },
     { CFGZ_BTS_BATTERY_HYBRID_MODE           , ACTUATOR::ACT_BTS_BATTERY_HYBRID_MODE    },
     { CFGZ_PREHEAT                           , ACTUATOR::ACT_PREHEAT                    },
     { CFGZ_ECU_START                         , ACTUATOR::ACT_ECU_START                  },
     { CFGZ_MIL                               , ACTUATOR::ACT_MIL                        }
    };

    for(uint8_t i=0;i<(sizeof(dsenseMap)/sizeof(ACTUATOR_MAP_ROW_t));i++)
    {
        if(dsenseMap[i].cfgzSensorType == _All_Param.u8ArrParam[u8CfgzActuatorTypeIdx])
        {
            return dsenseMap[i].actuatorType;
        }
    }
    return ACTUATOR::ACT_NOT_CONFIGURED;
}

void CFGZ::prvConfigureMODBUS()
{
    bool bModbusEnable;

    if(_All_Param.u8ArrParam[ID_MODBUS_COMM_COMM_MODE] == CFGZ_DISABLE)
    {
        bModbusEnable = false;
    }
    else
    {
        bModbusEnable = true;
    }

    uint8_t u8MBSlaveID = _All_Param.u8ArrParam[ID_MODBUS_COMM_MODBUS_SLAVE_ID];

    RS485::BAUD_t eBaud     = prvGetRS485Baud();
    RS485::PARITY_t eParity = prvGetRS485Parity();
    /*Baud is needed by MODBUS module to configure silence period*/
    _modbus.Configure((uint8_t)_All_Param.u8ArrParam[ID_MODBUS_COMM_MODBUS_BAUDRATE], u8MBSlaveID, bModbusEnable);
    _hal.ObjRS485.ConfigureBaudParity(eBaud, eParity);
}

RS485::BAUD_t CFGZ::prvGetRS485Baud()
{
    typedef struct
    {
        CFGZ_MODBUS_BAUD_t eCfgzBaud;
        RS485::BAUD_t      eRS485Baud;
    } CFGZ_RS485_BAUD_MAP_t;

    CFGZ_RS485_BAUD_MAP_t map[] =
    {
     {CFGZ_MODBUS_BAUD_1200   ,RS485::BAUD_1200  },
     {CFGZ_MODBUS_BAUD_2400   ,RS485::BAUD_2400  },
     {CFGZ_MODBUS_BAUD_4800   ,RS485::BAUD_4800  },
     {CFGZ_MODBUS_BAUD_9600   ,RS485::BAUD_9600  },
     {CFGZ_MODBUS_BAUD_19200  ,RS485::BAUD_19200 },
     {CFGZ_MODBUS_BAUD_38400  ,RS485::BAUD_38400 },
     {CFGZ_MODBUS_BAUD_57600  ,RS485::BAUD_57600 },
     {CFGZ_MODBUS_BAUD_115200 ,RS485::BAUD_115200}
    };

#if (AUTOMATION==1)
    CFGZ_MODBUS_BAUD_t eBaud = (CFGZ_MODBUS_BAUD_t)3;
#else
    CFGZ_MODBUS_BAUD_t eBaud = (CFGZ_MODBUS_BAUD_t)_All_Param.u8ArrParam[ID_MODBUS_COMM_MODBUS_BAUDRATE];
#endif

    for(uint8_t i=0;i<(sizeof(map)/sizeof(CFGZ_RS485_BAUD_MAP_t));i++)
    {
        if(map[i].eCfgzBaud == eBaud)
        {
            return map[i].eRS485Baud;
        }
    }
    /*Default in case a match was not found from the map*/
    return RS485::BAUD_9600;
}

RS485::PARITY_t CFGZ::prvGetRS485Parity()
{
    RS485::PARITY_t eParity = (RS485::PARITY_t)_All_Param.u8ArrParam[ID_MODBUS_COMM_PARITY];
    return eParity;
}

void CFGZ::prvConfigureACSense()
{
    uint16_t u16CTRation = _All_Param.u16ArrParam[ID_CURRENT_MONITOR_LOAD_CT_RATIO];
    float fCTCorrFactor =  _All_Param.f32ArrParam[ID_CURRENT_MONITOR_CT_CORRECTION_FACTOR];

    /*
     * By SuryaPranayTeja.BVV
     * The feature of applying filter to voltage values is not available as on 30-10-2022
     * The below function sets the default values such that filter is not applied.
     */
    RMS::SetRMSFilterConstForDisplay(0);

    _hal.AcSensors.ConfigureCTRatio(u16CTRation, _All_Param.u16ArrParam[ID_FAN_CURR_MONITOR_FAN_MON_CT_RATIO], fCTCorrFactor);

    /*
     *By SuryaPranayTeja.BVV
     *Mains and Gen PT feature is not available in GC2111 as on 30-10-2022.
     *The below ConfigureGenPTRatio and ConfigureMainsPTRatio are used to set the default values
     */
    _hal.AcSensors.ConfigureGenPTRatio(100,100);
    _hal.AcSensors.ConfigureMainsPTRatio(100,100);

    _hal.AcSensors.ConfigGeneratorRating(_All_Param.f32ArrParam[ID_LOAD_MONITOR_GEN_RATING]);

    AC_SENSE::AC_SYSTEM_TYP_t eGenACSystem = (AC_SENSE::AC_SYSTEM_TYP_t)_All_Param.u8ArrParam[ID_ALT_CONFIG_ALT_AC_SYSTEM];
    AC_SENSE::AC_SYSTEM_TYP_t eMainsACSystem = (AC_SENSE::AC_SYSTEM_TYP_t)_All_Param.u8ArrParam[ID_MAINS_CONFIG_MAINS_AC_SYSTEM];

    _hal.AcSensors.ConfigureACSystem(eGenACSystem, eMainsACSystem);

    _hal.AcSensors.Configure3phCalculationEnableFor1Ph(_All_Param.u8ArrParam[ID_MAINS_CONFIG_3PH_CALC_EN_FOR_1PH]);
}

void CFGZ::ApplyConfigChanges()
{
    prvConfigureDSENSE();
    prvConfigureASENSE();
    prvConfigureACT();
    prvConfigureMODBUS();
    prvConfigureACSense();
}

void CFGZ::prvSetPassword()
{
    uint16_t u16ArrPassword[3];

    u16ArrPassword[0] = _cfgc.GetPasswords(0);
    u16ArrPassword[1] = _cfgc.GetPasswords(1);

    BSP_STATUS_t eStatus;
    eStatus = _hal.Objeeprom.BlockingRead(EXT_EEPROM_PASWORD_START_ADDRESS, (uint8_t*)&_stMiscParam, sizeof(MISC_PARAM_t));

    if(((CRC16::ComputeCRCGeneric((uint8_t *)&_stMiscParam, sizeof(MISC_PARAM_t) - sizeof(uint16_t)
                                            , CRC_MEMORY_SEED)) != _stMiscParam.u16CRC)
            || (eStatus == BSP_FAIL))
    {
        _stMiscParam.u8MiscParam[PROFILE_NO] = 0;
        _stMiscParam.u16MiscParam[PASSWORD1]= u16ArrPassword[0];
        _stMiscParam.u16MiscParam[PASSWORD2]= u16ArrPassword[1];
        _stMiscParam.u8MiscParam[RESET_COUNTER] = 0;

        for(int i= ID_ENG_CHAR0;i<ENG_ID_CHAR_LAST;i++)
        {
            _stMiscParam.u8EngId[i] = 48; //Sets the default Engine Serial number
        }
        _stMiscParam.u16CRC = CRC16::ComputeCRCGeneric((uint8_t *)&_stMiscParam, sizeof(MISC_PARAM_t) -sizeof(uint16_t)
                                                       , CRC_MEMORY_SEED);

        _hal.Objeeprom.RequestWrite(EXT_EEPROM_PASWORD_START_ADDRESS, (uint8_t*)&_stMiscParam, sizeof(MISC_PARAM_t), NULL);
    }

}

void CFGZ::EnableDisableMainsParam()
{
    if((GetCFGZ_Param(ID_UNDER_VOLT_MON_ENABLE) == CFGZ_DISABLE) &&
          (GetCFGZ_Param(ID_OVER_VOLT_MON_ENABLE) == CFGZ_DISABLE) &&
          (GetCFGZ_Param(ID_UNDER_FREQ_MON_ENABLE) == CFGZ_DISABLE) &&
          (GetCFGZ_Param(ID_OVER_FREQ_MON_ENABLE) == CFGZ_DISABLE) &&
          (GetCFGZ_Param(ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ_ENABLE))
    {
        _All_Param.u8ArrParam[ID_MAINS_CONFIG_MAINS_MONITORING] = CFGZ_DISABLE;
    }
}

CFGZ::ENGINE_TYPES_t CFGZ::GetEngType(void)
{
    ENGINE_TYPES_t geEngType =(ENGINE_TYPES_t) 0;

    return geEngType;
}

void CFGZ::GetEngSrNo(char EngSrNo[])
{
    for(int i=0;i<12;i++)
    {
        EngSrNo[i] = (char) _stMiscParam.u8EngId[i];
    }
}

uint8_t CFGZ::GetArrLanguageIndex()
{
    return (uint8_t)(0); //No multiple language support.
}

uint8_t CFGZ::GetCustomerCodefromCFGC()
{
    return _cfgc.GetCustomerCode();
}
