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
#include "MB_APP.h"
#include "../HMI/MON_UI/MON_UI.h"
#include "../HMI/MAIN_UI/MAIN_UI.h"

MB_APP::KEY_MB_CAN_EVENT_t MB_APP::stMBEvent={0};
MB_APP::MB_APP(HAL_Manager &hal, CFGZ &cfgz, GCU_ALARMS &gcuAlarm,
        ENGINE_MONITORING &engineMonitoring, AUTO_MODE &Automode, EGOV &EGOV):
MODBUS(hal.ObjRS485, _AddressGrpLst),
_hal(hal),
_cfgz(cfgz),
_gcuAlarm(gcuAlarm),
_engineMonitoring(engineMonitoring),
_Automode(Automode),
_Egov(EGOV),
_u16MODBUSCommand(0),
_u16MODBUSOperModeCMD(0),
_au16Grp1Registers{0},
_au16Grp2Registers{0},
_aAddressGrp{
    {MB_REG_PROTOCOL_VER, MODBUS_GRP1_REG_CNT, _au16Grp1Registers, true , false},
    {MB_COMMAND         , MODBUS_GRP2_REG_CNT, _au16Grp2Registers, false, true}},
_AddressGrpLst{_aAddressGrp, MODBUS_ADDRESS_GROUPS},
_u16TempAlarmVal(0)
{
    SetReadRegisterValue(MB_REG_PROTOCOL_VER, MODBUS_PROTOCOL_VERSION);
}

void MB_APP::Update()
{
    MODBUS::Update();
    prvUpdateElectricalParams();
    prvUpdateAnalogParams();
    prvUpdateStartTripsRunHours();
    prvUpdateGCUAlarms();
    prvUpdateTimeStamp();

    prvUpadateDIGInOut();
    prvUpdateBtsParams();
    prvUpdateAUXSensorVal();

    prvUpdateMBCommandStatus();
}

uint16_t MB_APP::GetRegisterValue(MODBUS_WRITE_REGISTERS_t eRegister)
{
    /*Determine the group*/
    uint8_t u8Grp =  prvIdentifyRegisterGroup((uint16_t)eRegister, false, true);
    /*Determine the start address for the group*/
    uint16_t u16StartAddress =  _aAddressGrp[u8Grp].u16StartAddress;
    return _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress];
}

void MB_APP::SetReadRegisterValue(MODBUS_READ_REGISTERS_t eRegister, uint16_t u16Value)
{
    /*Determine the group*/
    uint8_t u8Grp =  prvIdentifyRegisterGroup((uint16_t)eRegister, true, false);
    /*Determine the start address for the group*/
    uint16_t u16StartAddress =  _aAddressGrp[u8Grp].u16StartAddress;
    _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress] = u16Value;
}

void MB_APP::SetWriteRegisterValue(MODBUS_WRITE_REGISTERS_t eRegister, uint16_t u16Value)
{
    /*Determine the group*/
    uint8_t u8Grp =  prvIdentifyRegisterGroup((uint16_t)eRegister, false, true);
    /*Determine the start address for the group*/
    uint16_t u16StartAddress =  _aAddressGrp[u8Grp].u16StartAddress;
    _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress] = u16Value;
}


void MB_APP::GetMBEventStatus(KEY_MB_CAN_EVENT_t *stEvent)
{
    static MB_APP::KEY_MB_CAN_EVENT_t oldMbEvent={false,false,false,false,false};


    if(oldMbEvent.bKeyEvent != stMBEvent.bKeyEvent)
    {
        memcpy(stEvent, &stMBEvent, sizeof(MB_APP::KEY_MB_CAN_EVENT_t)) ;
        stMBEvent.bKeyEvent = false;
        stMBEvent.bAckKey = false;
        stMBEvent.bAutoKey = false;
        stMBEvent.bStartKey = false;
        stMBEvent.bStopKey = false;
    }
    else
    {
        memcpy(stEvent, &oldMbEvent, sizeof(MB_APP::KEY_MB_CAN_EVENT_t)) ;
    }
    memcpy(&oldMbEvent, &stMBEvent, sizeof(MB_APP::KEY_MB_CAN_EVENT_t)) ;
}

uint8_t MB_APP::prvIdentifyRegisterGroup(uint16_t u16RegisterAddress, 
                                                        bool bReadAccess, bool bWriteAccess)
{
    uint8_t u8RegGrp = 0;
    /*Iterate over all groups*/
    for(uint8_t i=0; i<MODBUS_ADDRESS_GROUPS; i++)
    {
        /*Check weather this register is within this group*/
        if( (u16RegisterAddress >= _aAddressGrp[i].u16StartAddress) && 
            (u16RegisterAddress < (_aAddressGrp[i].u16StartAddress+_aAddressGrp[i].u16length)) )
        {
            if( (bReadAccess == _aAddressGrp[i].isReadSupported) && 
                (bWriteAccess == _aAddressGrp[i].isWriteSupported) )
            {
                u8RegGrp = i;
                break;
            }
        }
    }
    return u8RegGrp;
}

void MB_APP::prvUpdateTimeStamp()
{
    RTC::TIME_t CurrentTime;
    _hal.ObjRTC.GetTime(&CurrentTime);
    _u16TempAlarmVal = CurrentTime.u8Minute;
    _u16TempAlarmVal =(uint16_t)((_u16TempAlarmVal <<8) + CurrentTime.u8Second);
    SetReadRegisterValue(MB_TIME_STAMP0, _u16TempAlarmVal);
    _u16TempAlarmVal = CurrentTime.u8DayOfWeek;
    _u16TempAlarmVal = (uint16_t)((_u16TempAlarmVal <<8) + CurrentTime.u8Hour);
    SetReadRegisterValue(MB_TIME_STAMP1, _u16TempAlarmVal);
    _u16TempAlarmVal = CurrentTime.u8Month;
    _u16TempAlarmVal = (uint16_t)((_u16TempAlarmVal <<8) + CurrentTime.u8Day);
    SetReadRegisterValue(MB_TIME_STAMP2, _u16TempAlarmVal);
    _u16TempAlarmVal = CurrentTime.u16Year;
    SetReadRegisterValue(MB_TIME_STAMP3, _u16TempAlarmVal);
}

void MB_APP::prvUpdateElectricalParams()
{
    AC_SENSE &ac = _hal.AcSensors;
    uint16_t u16Tmp;
    
    /*Resolution 0.1*/
    u16Tmp = (uint16_t)(ac.GENSET_GetApproxFreq(R_PHASE)*10);
    SetReadRegisterValue(MB_GEN_R_FREQUENCY, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetApproxFreq(Y_PHASE)*10);
    SetReadRegisterValue(MB_GEN_Y_FREQUENCY, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetApproxFreq(B_PHASE)*10);
    SetReadRegisterValue(MB_GEN_B_FREQUENCY, u16Tmp);

    /*Resolution 0.1*/
    u16Tmp = (uint16_t)(ac.GENSET_GetVoltageVolts(R_PHASE)*10);
    SetReadRegisterValue(MB_GEN_L1_N_VOLATGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetVoltageVolts(Y_PHASE)*10);
    SetReadRegisterValue(MB_GEN_L2_N_VOLATGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetVoltageVolts(B_PHASE)*10);
    SetReadRegisterValue(MB_GEN_L3_N_VOLATGE, u16Tmp);

    /*Resolution 0.1*/
    u16Tmp = (uint16_t)(ac.GENSET_GetRYVolts()*10);
    SetReadRegisterValue(MB_GEN_L1_L2_VOLATGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetYBVolts()*10);
    SetReadRegisterValue(MB_GEN_L2_L3_VOLATGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetRBVolts()*10);
    SetReadRegisterValue(MB_GEN_L3_L1_VOLATGE, u16Tmp);

    if(_engineMonitoring.GetContactorLoadStatus()==ENGINE_MONITORING::LOAD_ON_GEN)
    {
        /*Resolution 0.1*/
        u16Tmp = (uint16_t)(ac.GENSET_GetCurrentAmps(R_PHASE)*10);
        SetReadRegisterValue(MB_GEN_L1_CURRENT, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetCurrentAmps(Y_PHASE)*10);
        SetReadRegisterValue(MB_GEN_L2_CURRENT, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetCurrentAmps(B_PHASE)*10);
        SetReadRegisterValue(MB_GEN_L3_CURRENT, u16Tmp);

        /*Resolution of 0.1 KW , i.e 10/1000=> 1/100 */
        u16Tmp = (uint16_t)(ac.GENSET_GetDispActivePowerWatts(R_PHASE)/100);
        SetReadRegisterValue(MB_GEN_L1_POWER, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetDispActivePowerWatts(Y_PHASE)/100);
        SetReadRegisterValue(MB_GEN_L2_POWER, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetDispActivePowerWatts(B_PHASE)/100);
        SetReadRegisterValue(MB_GEN_L3_POWER, u16Tmp);

        u16Tmp = (uint16_t)(ac.GENSET_GetTotalFiltActivePowerWatts()/100);
        SetReadRegisterValue(MB_GEN_TOTAL_POWER, u16Tmp);

        u16Tmp = (uint16_t)round(ac.GENSET_GetDispPercentPower());
        SetReadRegisterValue(MB_GEN_PERCENT_LOAD, u16Tmp);

        /*Resolution of 0.1 KVA , i.e 10/1000=> 1/100 */
        u16Tmp = (uint16_t)(ac.GENSET_GetDispApparentPowerVA(R_PHASE)/100);
        SetReadRegisterValue(MB_GEN_L1_APPARENT_POWER, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetDispApparentPowerVA(Y_PHASE)/100);
        SetReadRegisterValue(MB_GEN_L2_APPARENT_POWER, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetDispApparentPowerVA(B_PHASE)/100);
        SetReadRegisterValue(MB_GEN_L3_APPARENT_POWER, u16Tmp);

        u16Tmp = (uint16_t)(ac.GENSET_GetTotalFiltApparentPowerVA()/100);
        SetReadRegisterValue(MB_GEN_TOTAL_APPARENT_POWER, u16Tmp);

        /*Resolution of 0.1 KVAr , i.e 10/1000=> 1/100 */
        u16Tmp = (uint16_t)(ac.GENSET_GetDispReactivePowerVAR(R_PHASE)/100);
        SetReadRegisterValue(MB_GEN_L1_REACTIVE_POWER, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetDispReactivePowerVAR(Y_PHASE)/100);
        SetReadRegisterValue(MB_GEN_L2_REACTIVE_POWER, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetDispReactivePowerVAR(B_PHASE)/100);
        SetReadRegisterValue(MB_GEN_L3_REACTIVE_POWER, u16Tmp);

        u16Tmp = (uint16_t)(ac.GENSET_GetTotalFiltReactivePowerVAR()/100);
        SetReadRegisterValue(MB_GEN_TOTAL_REACTIVE_POWER, u16Tmp);
    }
    else if(_engineMonitoring.GetContactorLoadStatus()==ENGINE_MONITORING::LOAD_ON_MAINS)
    {
        /*Resolution 0.1*/
           u16Tmp = (uint16_t)(ac.MAINS_GetCurrentAmps(R_PHASE)*10);
           SetReadRegisterValue(MB_GEN_L1_CURRENT, u16Tmp);
           u16Tmp = (uint16_t)(ac.MAINS_GetCurrentAmps(Y_PHASE)*10);
           SetReadRegisterValue(MB_GEN_L2_CURRENT, u16Tmp);
           u16Tmp = (uint16_t)(ac.MAINS_GetCurrentAmps(B_PHASE)*10);
           SetReadRegisterValue(MB_GEN_L3_CURRENT, u16Tmp);

           /*Resolution of 0.1 KW , i.e 10/1000=> 1/100 */
           u16Tmp = (uint16_t)(ac.MAINS_GetDispActivePowerWatts(R_PHASE)/100);
           SetReadRegisterValue(MB_GEN_L1_POWER, u16Tmp);
           u16Tmp = (uint16_t)(ac.MAINS_GetDispActivePowerWatts(Y_PHASE)/100);
           SetReadRegisterValue(MB_GEN_L2_POWER, u16Tmp);
           u16Tmp = (uint16_t)(ac.MAINS_GetDispActivePowerWatts(B_PHASE)/100);
           SetReadRegisterValue(MB_GEN_L3_POWER, u16Tmp);

           u16Tmp = (uint16_t)(ac.MAINS_GetTotalFiltActivePowerWatts()/100);
           SetReadRegisterValue(MB_GEN_TOTAL_POWER, u16Tmp);

//           u16Tmp = (uint16_t)round(ac.MAINS_GetPercentPower());
//           SetReadRegisterValue(MB_GEN_PERCENT_LOAD, u16Tmp);

           /*Resolution of 0.1 KVA , i.e 10/1000=> 1/100 */
           u16Tmp = (uint16_t)(ac.MAINS_GetDispApparentPowerVA(R_PHASE)/100);
           SetReadRegisterValue(MB_GEN_L1_APPARENT_POWER, u16Tmp);
           u16Tmp = (uint16_t)(ac.MAINS_GetDispApparentPowerVA(Y_PHASE)/100);
           SetReadRegisterValue(MB_GEN_L2_APPARENT_POWER, u16Tmp);
           u16Tmp = (uint16_t)(ac.MAINS_GetDispApparentPowerVA(B_PHASE)/100);
           SetReadRegisterValue(MB_GEN_L3_APPARENT_POWER, u16Tmp);

           u16Tmp = (uint16_t)(ac.MAINS_GetTotalFiltApparentPowerVA()/100);
           SetReadRegisterValue(MB_GEN_TOTAL_APPARENT_POWER, u16Tmp);

           /*Resolution of 0.1 KVAr , i.e 10/1000=> 1/100 */
           u16Tmp = (uint16_t)(ac.MAINS_GetDispReactivePowerVAR(R_PHASE)/100);
           SetReadRegisterValue(MB_GEN_L1_REACTIVE_POWER, u16Tmp);
           u16Tmp = (uint16_t)(ac.MAINS_GetDispReactivePowerVAR(Y_PHASE)/100);
           SetReadRegisterValue(MB_GEN_L2_REACTIVE_POWER, u16Tmp);
           u16Tmp = (uint16_t)(ac.MAINS_GetDispReactivePowerVAR(B_PHASE)/100);
           SetReadRegisterValue(MB_GEN_L3_REACTIVE_POWER, u16Tmp);

           u16Tmp = (uint16_t)(ac.MAINS_GetTotalFiltReactivePowerVAR()/100);
           SetReadRegisterValue(MB_GEN_TOTAL_REACTIVE_POWER, u16Tmp);
    }
    /*Resolution 0.01*/
    u16Tmp = (uint16_t)(abs(ac.GENSET_GetDispPowerFactor(R_PHASE)*100));
    SetReadRegisterValue(MB_GEN_L1_PF, u16Tmp);
    u16Tmp = (uint16_t)(abs(ac.GENSET_GetDispPowerFactor(Y_PHASE)*100));
    SetReadRegisterValue(MB_GEN_L2_PF, u16Tmp);
    u16Tmp = (uint16_t)(abs(ac.GENSET_GetDispPowerFactor(B_PHASE)*100));
    SetReadRegisterValue(MB_GEN_L3_PF, u16Tmp);

    /*Resolution 0.01*/
    u16Tmp = (uint16_t)(abs(ac.GENSET_GetDispAveragePowerFactor()*100));
    SetReadRegisterValue(MB_GEN_AVERAGE_PF, (uint16_t)(u16Tmp));

    /*KWH in 0.1 resolution*/
    uint32_t u32Tmp = (uint32_t)(ac.GENSET_GetTotalActiveEnergySinceInitWH()/100);
    SetReadRegisterValue(MB_GEN_CUMM_ACTIVE_ENERGY_1, (uint16_t)(u32Tmp>>16));
    SetReadRegisterValue(MB_GEN_CUMM_ACTIVE_ENERGY_2, (uint16_t)(u32Tmp & 0xFFFFU));

    u32Tmp = (uint32_t)(ac.GENSET_GetTotalApparentEnergySinceInitVAH()/100);
    SetReadRegisterValue(MB_GEN_CUMM_APPARENT_ENERGY_1, (uint16_t)(u32Tmp>>16));
    SetReadRegisterValue(MB_GEN_CUMM_APPARENT_ENERGY_2, (uint16_t)(u32Tmp & 0xFFFFU));

    u32Tmp = (uint32_t)(ac.GENSET_GetTotalReactiveEnergySinceInitVARH()/100);
    SetReadRegisterValue(MB_GEN_CUMM_REACTIVE_ENERGY_1, (uint16_t)(u32Tmp>>16));
    SetReadRegisterValue(MB_GEN_CUMM_REACTIVE_ENERGY_2, (uint16_t)(u32Tmp & 0xFFFFU));


    /*KWH in 0.1 resolution*/
    u32Tmp = (uint32_t)(_hal.AcSensors.MAINS_GetTotalActiveEnergySinceInitWH()/100);
    SetReadRegisterValue(MB_MAINS_CUMM_ACTIVE_ENERGY_1, (uint16_t)(u32Tmp>>16));
    SetReadRegisterValue(MB_MAINS_CUMM_ACTIVE_ENERGY_2, (uint16_t)(u32Tmp & 0xFFFFU));

    u32Tmp = (uint32_t)(_hal.AcSensors.MAINS_GetTotalApparentEnergySinceInitVAH()/100);
    SetReadRegisterValue(MB_MAINS_CUMM_APPARENT_ENERGY_1, (uint16_t)(u32Tmp>>16));
    SetReadRegisterValue(MB_MAINS_CUMM_APPARENT_ENERGY_2, (uint16_t)(u32Tmp & 0xFFFFU));

    u32Tmp = (uint32_t)(_hal.AcSensors.MAINS_GetTotalReactiveEnergySinceInitVARH()/100);
    SetReadRegisterValue(MB_MAINS_CUMM_REACTIVE_ENERGY_1, (uint16_t)(u32Tmp>>16));
    SetReadRegisterValue(MB_MAINS_CUMM_REACTIVE_ENERGY_2, (uint16_t)(u32Tmp & 0xFFFFU));


    u16Tmp = (uint16_t)(ac.MAINS_GetVoltageVolts(R_PHASE)*10);
    SetReadRegisterValue(MB_MAINS_L1_N_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.MAINS_GetVoltageVolts(Y_PHASE)*10);
    SetReadRegisterValue(MB_MAINS_L2_N_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.MAINS_GetVoltageVolts(B_PHASE)*10);
    SetReadRegisterValue(MB_MAINS_L3_N_VOLTAGE, u16Tmp);

    u16Tmp = (uint16_t)(ac.MAINS_GetRYVolts()*10);
    SetReadRegisterValue(MB_MAINS_L1_L2_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.MAINS_GetYBVolts()*10);
    SetReadRegisterValue(MB_MAINS_L2_L3_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)int(ac.MAINS_GetRBVolts()*10);
    SetReadRegisterValue(MB_MAINS_L3_L1_VOLTAGE, u16Tmp);

    u16Tmp = (uint16_t)(ac.MAINS_GetApproxFreq(R_PHASE)*10);
    SetReadRegisterValue(MB_MAINS_R_FREQUENCY, u16Tmp);
    u16Tmp = (uint16_t)(ac.MAINS_GetApproxFreq(Y_PHASE)*10);
    SetReadRegisterValue(MB_MAINS_Y_FREQUENCY, u16Tmp);
    u16Tmp = (uint16_t)(ac.MAINS_GetApproxFreq(B_PHASE)*10);
    SetReadRegisterValue(MB_MAINS_B_FREQUENCY, u16Tmp);
}


void MB_APP::prvUpdateAnalogParams()
{

    A_SENSE &sensor = _hal.AnalogSensors;
    
    uint16_t  u16Tmp = 0;

    /*Initialize registers to zero*/
    SetReadRegisterValue(MB_OIL_PRESSURE, u16Tmp);
    SetReadRegisterValue(MB_COOLANT_TEMPERATURE, u16Tmp);
    SetReadRegisterValue(MB_FUEL_LEVEL, u16Tmp);
    SetReadRegisterValue(MB_FUEL_IN_LIT, u16Tmp);

    A_SENSE::SENSOR_RET_t sensorVal = _gcuAlarm.GetLOPSensorVal();

    if((_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
            && (sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS)
            && (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL))
    {
        /*Scale factor is 0.1*/
        u16Tmp = (uint16_t)(sensorVal.stValAndStatus.f32InstSensorVal*10);
        SetReadRegisterValue(MB_OIL_PRESSURE, u16Tmp);
    }
    else if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1) &&
        (sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) &&
        (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
    {
        /*Scale factor is 0.1*/
        u16Tmp = (uint16_t)(sensorVal.stValAndStatus.f32InstSensorVal*10);
        SetReadRegisterValue(MB_OIL_PRESSURE, u16Tmp);
    }
    else if ((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG)== CFGZ::CFGZ_ENABLE)&&(_cfgz.GetEngType()!=CFGZ::ENG_CONVENTIONAL))
    {
        u16Tmp = (uint16_t)(sensorVal.stValAndStatus.f32InstSensorVal*10);
                SetReadRegisterValue(MB_OIL_PRESSURE, u16Tmp);
    }

    sensorVal = _gcuAlarm.GetSelectedTempSensVal();

    if((sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && 
        (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
    {
        /*Scale factor is 0.1*/
        u16Tmp = (int16_t)(round(sensorVal.stValAndStatus.f32InstSensorVal)*10);
        SetReadRegisterValue(MB_COOLANT_TEMPERATURE, u16Tmp);
    }
    else if ((_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG)== CFGZ::CFGZ_ENABLE)&&(_cfgz.GetEngType()!=CFGZ::ENG_CONVENTIONAL))
    {
        u16Tmp = (int16_t)(round(sensorVal.stValAndStatus.f32InstSensorVal)*10);
                SetReadRegisterValue(MB_COOLANT_TEMPERATURE, u16Tmp);
    }

    sensorVal = sensor.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);

    if((sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && 
        (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
    {
        /*Scale factor is 0.1*/
        u16Tmp = (uint16_t)(round(sensorVal.stValAndStatus.f32InstSensorVal));
        SetReadRegisterValue(MB_FUEL_LEVEL, u16Tmp);

        u16Tmp = (uint16_t)round(sensorVal.stValAndStatus.f32InstSensorVal *_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_TANK_CAPACITY)/10);
        SetReadRegisterValue(MB_FUEL_IN_LIT, u16Tmp);
    }

    /*Scale factor 1*/
    u16Tmp = (uint16_t)(sensor.GetFilteredChargingAltVolts()*10);
    SetReadRegisterValue(MB_CHARGING_ALT_VOLTAGE, u16Tmp);

    /*Scale factor 0.1*/
    u16Tmp = (uint16_t)(_gcuAlarm.GetSelectedBatteryVtg()*10);
    SetReadRegisterValue(MB_BATTERY_VOLTAGE, u16Tmp);

    u16Tmp = (uint16_t)(_gcuAlarm.GetSpeedValue());
    SetReadRegisterValue(MB_ENG_SPEED, u16Tmp);

    ANLG_IP::ANALOG_VAL_t stVal;
    stVal = _hal.AnlgIp.GetChargingAltVtg_V();
    u16Tmp = (uint16_t)stVal.f32InstSensorVal;

}

void MB_APP::prvUpdateStartTripsRunHours()
{
    SetReadRegisterValue(MB_NO_OF_STARTS, (uint16_t)_engineMonitoring.GetEngineNoOfStarts());
    SetReadRegisterValue(MB_NO_OF_TRIPS, (uint16_t)_engineMonitoring.GetEngineNoOfTrips());

    SetReadRegisterValue(MB_ENG_RUN_HOURS_1, (uint16_t)(((_gcuAlarm.GetSelectedEngRunMin()/60)>>16)&0x0000FFFF));
    SetReadRegisterValue(MB_ENG_RUN_HOURS_2, (uint16_t)((_gcuAlarm.GetSelectedEngRunMin()/60)& 0x0000FFFF));
    SetReadRegisterValue(MB_ENG_RUN_MINUTES, (uint16_t)(_gcuAlarm.GetSelectedEngRunMin()%60));

    SetReadRegisterValue(MB_MAINS_RUN_HOURS, (uint16_t)((_engineMonitoring.GetMainsRunTimeMin()/60)& 0x0000FFFF));
    SetReadRegisterValue(MB_MAINS_RUN_MINUTES, (uint16_t)(_engineMonitoring.GetMainsRunTimeMin()%60));
}

void MB_APP::prvUpdateMBCommandStatus()
{
    if(GetRegisterValue(MB_APP::MB_COMMAND) != 0U)
   {

       uint8_t u8Local =(uint8_t) GetRegisterValue(MB_APP::MB_COMMAND);

       if(u8Local & 4)
       {
         stMBEvent.bAutoKey = true;
         stMBEvent.bKeyEvent =true;
       }
       else if(u8Local & 1)
       {
         stMBEvent.bStopKey  = true;
         stMBEvent.bKeyEvent =true;
       }
       else if (u8Local & 2)
       {
         stMBEvent.bStartKey = true;
         stMBEvent.bKeyEvent =true;
       }

       if(u8Local & 64)
       {
         stMBEvent.bAckKey = true;
         stMBEvent.bKeyEvent =true;
       }

       SetWriteRegisterValue(MB_APP::MB_COMMAND, 0);

   }
#if (AUTOMATION==1)
    if(GetRegisterValue(MB_APP::MB_DATE_TIME5)== 1U)
    {
          RTC::TIME_t currentTime;

          currentTime.u8Second =0;
          currentTime.u8Minute =(uint8_t)(((uint16_t)GetRegisterValue(MB_APP::MB_DATE_TIME1) & 0xFF00)>>8);
          currentTime.u8Hour =(uint8_t)((uint16_t)GetRegisterValue(MB_APP::MB_DATE_TIME2) & 0xFF);
          currentTime.u8DayOfWeek =(uint8_t)(((uint16_t)GetRegisterValue(MB_APP::MB_DATE_TIME2)& 0xFF00)>>8);
          currentTime.u8Day =(uint8_t)((uint16_t)GetRegisterValue(MB_APP::MB_DATE_TIME3)& 0xFF);
          currentTime.u8Month =(uint8_t)(((uint16_t)GetRegisterValue(MB_APP::MB_DATE_TIME3) & 0xFF00)>>8);
          currentTime.u16Year = (uint16_t)GetRegisterValue(MB_APP::MB_DATE_TIME4) ;
          _hal.ObjRTC.SetTime(&currentTime);

          SetWriteRegisterValue(MB_APP::MB_DATE_TIME5, 0);
    }
#endif

}

void MB_APP::prvUpdateAUXReg(uint16_t u16AlarmID1, uint16_t u16AlarmID2, uint16_t u16Offset)
{
    if(_gcuAlarm.ArrAlarmMonitoring[u16AlarmID1].bEnableMonitoring ||
    _gcuAlarm.ArrAlarmMonitoring[u16AlarmID2].bEnableMonitoring)
    {
        if(_gcuAlarm.ArrAlarmMonitoring[u16AlarmID1].bAlarmActive)
        {
            _u16TempAlarmVal |= (uint16_t)(ALARM_SHUTDOWN << u16Offset);
        }
        else if(_gcuAlarm.ArrAlarmMonitoring[u16AlarmID2].bAlarmActive)
        {
            _u16TempAlarmVal |= (uint16_t)(ALARM_WARNING << u16Offset);
        }
        else
        {
            _u16TempAlarmVal |=(uint16_t)(ALARM_INACTIVE << u16Offset);
        }
    }
}

void MB_APP::prvUpdateAUXSensorVal()
{
    A_SENSE &sensor = _hal.AnalogSensors;
    A_SENSE::SENSOR_RET_t  stTemp;
    uint16_t u16AuxSensorVal = 0;
//S1
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        // S1 as shelter tempreture
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_SHELTER_TEMPERATURE);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (int16_t)(round(stTemp.stValAndStatus.f32InstSensorVal*10));
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        // S1 as s1 Analog sensor
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_S1_SENSOR);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (uint16_t)(stTemp.stValAndStatus.f32InstSensorVal*10);
        }
    }
    SetReadRegisterValue(MB_AUX_S1, u16AuxSensorVal);

//S2
    u16AuxSensorVal = 0;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_S2_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        // S2 as s2 analog sensor
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_S2_SENSOR);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (uint16_t)(stTemp.stValAndStatus.f32InstSensorVal*10);
        }
    }
    SetReadRegisterValue(MB_AUX_S2, u16AuxSensorVal);

//S3
    u16AuxSensorVal = 0;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        // S3 as 4-20 mA sensor
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_S3_4_20_SENSOR);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (uint16_t)(stTemp.stValAndStatus.f32InstSensorVal*10);
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        // S3 as 0-5 V Sensor
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_S3_0_5_SENSOR);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (uint16_t)(stTemp.stValAndStatus.f32InstSensorVal*10);
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
    {
        // S3 as Lube Oil Pressure
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE_4_20);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (uint16_t)(stTemp.stValAndStatus.f32InstSensorVal*10);
        }
    }
    SetReadRegisterValue(MB_AUX_S3, u16AuxSensorVal);

//S4
    u16AuxSensorVal = 0;
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        // S4 as 4-20 mA sensor
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_S4_4_20_SENSOR);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (uint16_t)(stTemp.stValAndStatus.f32InstSensorVal*10);
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        // S4 as 0-5 V Sensor
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_S4_0_5_SENSOR);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (uint16_t)(stTemp.stValAndStatus.f32InstSensorVal*10);
        }
    }
    SetReadRegisterValue(MB_AUX_S4, u16AuxSensorVal);

}
void MB_APP::prvUpdateBtsParams()
{
    SetReadRegisterValue(MB_BTS_CHARGE_STATE,(uint16_t)(_hal.AnalogSensors.GetFilteredVBTSbattVolts()*10));
    SetReadRegisterValue(MB_BTS_RUN_MIN, (uint16_t)(_engineMonitoring.GetBTSRunTimeMin()%60));
    SetReadRegisterValue(MB_BTS_RUN_HRS, (uint16_t)(_engineMonitoring.GetBTSRunTimeMin()/60));
}
void MB_APP::prvUpadateDIGInOut()
{

    uint8_t u8LocalCnt=15,u8Local = 0;
    _u16TempAlarmVal =0;

    SetReadRegisterValue(MB_DIG_IP_STATUS, _u16TempAlarmVal);

    //Output Diagnostic Status
    _u16TempAlarmVal =0;
    u8LocalCnt = 15;

    for(u8Local=CFGZ::ID_OUT_A_SOURCE; u8Local <=CFGZ::ID_OUT_G_SOURCE; u8Local=u8Local+2)
    {
        if(_hal.actuators.GetActStatus((ACTUATOR::ACTUATOR_TYPS_t)_cfgz.GetCFGZ_Param((CFGZ::UINT8_PARAMS_t)u8Local))
                == ACT_Manager::ACT_LATCHED)
        {
            _u16TempAlarmVal |= (uint16_t)(1U << u8LocalCnt);
        }
        u8LocalCnt--;
    }
    SetReadRegisterValue(MB_DIG_OP_STATUS, _u16TempAlarmVal);
}


void MB_APP::prvUpdateGCUAlarms()
{
    // Alarm 1
    _u16TempAlarmVal =0;


    SetReadRegisterValue(MB_ALARM_1, _u16TempAlarmVal);

    //Alarm 2
    _u16TempAlarmVal =0;

    SetReadRegisterValue(MB_ALARM_2, _u16TempAlarmVal);

    // Alarm 3
    _u16TempAlarmVal = 0;




//     UpdateAlarmRegValue(GCU_ALARMS::LOW_LOAD_TRIP, MB_ALARM_3, THIRD_NIBBLE);
//     _u16TempAlarmVal |= 0xF000U;


    SetReadRegisterValue(MB_ALARM_3, _u16TempAlarmVal);

    //Alarm 4
    _u16TempAlarmVal = 0;

    SetReadRegisterValue(MB_ALARM_4, _u16TempAlarmVal);

    //Alarm 5
    _u16TempAlarmVal = 0;
    SetReadRegisterValue(MB_ALARM_5, _u16TempAlarmVal);

    // Alarm 6
    _u16TempAlarmVal =0;
    SetReadRegisterValue(MB_ALARM_6, _u16TempAlarmVal);

    //Alarm 7
    _u16TempAlarmVal =0;

//    UpdateAlarmRegValue(GCU_ALARMS::DIG_IN_I, MB_ALARM_7, FIRST_NIBBLE);

    SetReadRegisterValue(MB_ALARM_7, _u16TempAlarmVal);

    //Alarm 8
    _u16TempAlarmVal =0;

    SetReadRegisterValue(MB_ALARM_8, _u16TempAlarmVal);

    //Alarm 9
    _u16TempAlarmVal =0;

    SetReadRegisterValue(MB_ALARM_9, _u16TempAlarmVal);


    //Alarm 10
    _u16TempAlarmVal =0;

    SetReadRegisterValue(MB_ALARM_10, _u16TempAlarmVal);

    //Alarm 11
    _u16TempAlarmVal =0;

    SetReadRegisterValue(MB_ALARM_11, _u16TempAlarmVal);

    // Alarm 12
    _u16TempAlarmVal =0;

    SetReadRegisterValue(MB_ALARM_12, _u16TempAlarmVal);

    // Alarm 13
    _u16TempAlarmVal =0;

    SetReadRegisterValue(MB_ALARM_13, _u16TempAlarmVal);

    // Alarm 14
    _u16TempAlarmVal =0;


//#if (AUTOMATION==1)
//    UpdateAlarmRegValue(GCU_ALARMS::J1939_ECU_PREHEAT_FAIL, MB_ALARM_12, SECOND_NIBBLE);
//#else
//    _u16TempAlarmVal |= 0x00F0U;
//#endif
    SetReadRegisterValue(MB_ALARM_14, _u16TempAlarmVal);


    //Alarm 15
    _u16TempAlarmVal =0;


    SetReadRegisterValue(MB_ALARM_15, _u16TempAlarmVal);

    //Alarm 16
    _u16TempAlarmVal = 0;

    SetReadRegisterValue(MB_ALARM_16, _u16TempAlarmVal);

    //Alarm 17
    _u16TempAlarmVal = 0;
    SetReadRegisterValue(MB_ALARM_17, _u16TempAlarmVal);


    // uint8_t u8LocalCnt=15,u8Local = 0;
    // _u16TempAlarmVal =0;
    // for(u8Local=GCU_ALARMS::DIGITAL_INPUT_A; u8Local <=GCU_ALARMS::DIG_IN_I; u8Local++)
    // {
    //     _u16TempAlarmVal |= ((uint16_t)_gcuAlarm.ArrAlarmMonitoring[u8Local].bAlarmActive << u8LocalCnt);
    //     u8LocalCnt--;
    // }
    // u8LocalCnt = 6;
    // for(u8Local=CFGZ::ID_OUT_A_SOURCE; u8Local <=CFGZ::ID_OUT_F_SOURCE; u8Local=u8Local+2)
    // {
    //     if(_hal.actuators.GetActStatus((ACTUATOR::ACTUATOR_TYPS_t)_cfgz.GetCFGZ_Param((CFGZ::UINT8_PARAMS_t)u8Local))
    //             == ACT_Manager::ACT_LATCHED)
    //     {
    //         _u16TempAlarmVal |= (uint16_t)(1U << u8LocalCnt);
    //     }
    //     u8LocalCnt--;
    // }

    // _u16TempAlarmVal |= 0x0001;
    // SetReadRegisterValue(MB_IP_OP_DIAGNOSTICS ,_u16TempAlarmVal);

    _u16TempAlarmVal =0;

    if(
         (MAIN_UI::GetDispMode() ==  DISP_PASSWORD_EDIT_MODE)
       ||(MAIN_UI::GetDispMode() ==  DISP_CONFIG_MODE)
       ||(MAIN_UI::GetDispMode() ==  DISP_EVENT_LOG_MODE)
       )
    {
        _u16TempAlarmVal |= ((uint16_t)1U << 15U);
    }

    if((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) ==CFGZ::CFGZ_ENABLE)
                &&(_Automode.GetMainsStatus() == BASE_MODES::MAINS_HELATHY))
    {
        _u16TempAlarmVal |= ((uint16_t)1U << 14U);
    }

#if (AUTOMATION==1)
    if(_Automode.GetGCUOperatingMode() != BASE_MODES::MANUAL_MODE)
    {
        if(_Automode.IsNightModeRestrictOn())
        {
            _u16TempAlarmVal |= ((uint16_t)1U << 13U);
            _u16TempAlarmVal |= ((uint16_t)1U << 12U);
            _u16TempAlarmVal |= ((uint16_t)1U << 11U);
        }
        else if(_Automode.GetGCUOperatingMode() == BASE_MODES::CYCLIC_MODE)
        {
            _u16TempAlarmVal |= ((uint16_t)1U << 12U);
            _u16TempAlarmVal |= ((uint16_t)1U << 11U);
        }
        else if(_Automode.GetGCUOperatingMode() == BASE_MODES::AUTO_EXERCISE_MODE)
        {
            /*
             * Note : In NXP code only 12 th bit is set when Opertaing mode is Auto Exercise.
             * However, Automation of SGC1XX is used for testing SGC4XX
             * where the 13th and 12th bit are set when Operating mode is Auto Exercise.
             * Hence in Renesas Code similar has been implemented to continue with Automation testing.*/
            _u16TempAlarmVal |= ((uint16_t)1U << 13U);
            _u16TempAlarmVal |= ((uint16_t)1U << 12U);
        }
        else if(_Automode.GetGCUOperatingMode() == BASE_MODES::AUTO_MODE)
        {
            _u16TempAlarmVal |= ((uint16_t)1U << 13U);
            _u16TempAlarmVal |= ((uint16_t)1U << 11U);
        }
        else if(_Automode.GetGCUOperatingMode() == BASE_MODES::BTS_MODE)
        {
            _u16TempAlarmVal |= ((uint16_t)1U << 13U);
        }
    }
    else
    {
        _u16TempAlarmVal |= ((uint16_t)1U << 12U);
    }
#else
    if(_Automode.GetGCUOperatingMode() != BASE_MODES::MANUAL_MODE)
    {
        if(_Automode.GetGCUOperatingMode() == BASE_MODES::CYCLIC_MODE)
        {
            _u16TempAlarmVal |= ((uint16_t)1U << 12U);
            _u16TempAlarmVal |= ((uint16_t)1U << 11U);
        }
        else if(_Automode.GetGCUOperatingMode() == BASE_MODES::AUTO_EXERCISE_MODE)
        {
            _u16TempAlarmVal |= ((uint16_t)1U << 12U);
        }
        else if(_Automode.GetGCUOperatingMode() == BASE_MODES::AUTO_MODE)
        {
            _u16TempAlarmVal |= ((uint16_t)1U << 11U);
        }
        else if(_Automode.GetGCUOperatingMode() == BASE_MODES::BTS_MODE)
        {
            _u16TempAlarmVal |= ((uint16_t)1U << 13U);
        }
    }
    else
    {
        _u16TempAlarmVal |= ((uint16_t)0U << 11U);
    }
#endif
    //b10- Load On DG Status
   if((_engineMonitoring.GetContactorLoadStatus() ==  ENGINE_MONITORING::LOAD_ON_GEN) && (_engineMonitoring.IsEngineOn()))
   {
       _u16TempAlarmVal |= ((uint16_t)1U << 10U);
   }
   else
   {
       _u16TempAlarmVal |= ((uint16_t)0U << 10U);
   }

   if(_engineMonitoring.GetContactorLoadStatus() ==  ENGINE_MONITORING::LOAD_ON_MAINS)
   {
       _u16TempAlarmVal |= (uint16_t)((uint16_t)1U << 9);
   }
   else
   {
       _u16TempAlarmVal |= (uint16_t)((uint16_t)0U << 9);
   }

   _u16TempAlarmVal |= (uint16_t)((uint16_t)_engineMonitoring.IsEngineOn() << 8);


   if(!_engineMonitoring.IsEngineOn() &&( _gcuAlarm.GetActiveAlarmCount()==0))
   {
       _u16TempAlarmVal |= ((uint16_t)1 << 7);
   }
   else
   {
       _u16TempAlarmVal |= ((uint16_t)0 << 7);
   }

   if(!_engineMonitoring.IsEngineOn()&&( _gcuAlarm.IsCommonShutdown() || _gcuAlarm.IsCommonElectricTrip()))
    {
     _u16TempAlarmVal |= ((uint16_t)1 << 6);
    }
    else
    {
     _u16TempAlarmVal |= ((uint16_t)0 << 6);
    }

    if(_gcuAlarm.IsFailToStart())
    {
        _u16TempAlarmVal |= ((uint16_t)1<<5);
    }


    if(_engineMonitoring.IsGenAvailable())
    {
      _u16TempAlarmVal |= ((uint16_t)1<<4);
    }

    if(_gcuAlarm.IsCommonShutdown())
    {
        _u16TempAlarmVal |= ((uint16_t)1<<3);
    }
    if(_gcuAlarm.IsCommonElectricTrip())
     {
         _u16TempAlarmVal |= ((uint16_t)1<<2);
     }
    if(_gcuAlarm.IsCommonWarning())
     {
         _u16TempAlarmVal |= ((uint16_t)1<<1);
     }
    if(_gcuAlarm.IsCommonNotification())
     {
         _u16TempAlarmVal |= ((uint16_t)1<<0);
     }

    SetReadRegisterValue(MB_GEN_STATUS ,(uint16_t)_u16TempAlarmVal);

}

void  MB_APP::UpdateEngSensorAlarms(uint8_t u8AlarmReg, uint8_t u8ShutdownID, uint8_t u8WarningID,
                    uint8_t u8SwitchID, uint8_t u8Offset)
{
    if((_gcuAlarm.ArrAlarmMonitoring[u8ShutdownID].bEnableMonitoring)
        || (_gcuAlarm.ArrAlarmMonitoring[u8WarningID].bEnableMonitoring)
        || (_gcuAlarm.ArrAlarmMonitoring[u8SwitchID].bEnableMonitoring))
    {
        if(_gcuAlarm.ArrAlarmMonitoring[u8ShutdownID].bAlarmActive)
        {
            _u16TempAlarmVal |= (uint16_t)(ALARM_SHUTDOWN << u8Offset);
        }
        else if(_gcuAlarm.ArrAlarmMonitoring[u8WarningID].bAlarmActive)
        {
            _u16TempAlarmVal |= (uint16_t)(ALARM_WARNING << u8Offset);
        }
        else if(_gcuAlarm.ArrAlarmMonitoring[u8SwitchID].bAlarmActive)
        {
            UpdateAlarmRegValue((MODBUS_READ_REGISTERS_t)u8SwitchID, u8AlarmReg, u8Offset);
        }
        else
        {
            _u16TempAlarmVal |= (uint16_t)(ALARM_INACTIVE << u8Offset);
        }
    }
    else
    {
        _u16TempAlarmVal |= (uint16_t)(ALARM_DISABLED << u8Offset);
    }
}

void MB_APP::UpdateAlarmRegValue(uint8_t u8AlarmID, uint8_t u8AlarmReg, uint8_t u8Offset)
{
    if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID].bEnableMonitoring)
    {
        if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID].bAlarmActive)
        {
            UpdateAlarmRegStatus(u8AlarmID, u8AlarmReg, u8Offset);
        }
        else
        {
            _u16TempAlarmVal |= (uint16_t)(ALARM_INACTIVE << u8Offset);
        }
    }
}

void MB_APP::UpdateAlarmRegStatus(uint8_t u8AlarmID, uint8_t u8AlarmReg, uint8_t u8Offset)
{
    if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID].bEnableShutdown)
    {
        _u16TempAlarmVal |= (uint16_t)(ALARM_SHUTDOWN << u8Offset);
    }
    else if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID].bEnableElectricTrip)
    {
        _u16TempAlarmVal |= (uint16_t)(ALARM_ELEC_TRIP << u8Offset);
    }
    else if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID].bEnableWarning)
    {
        _u16TempAlarmVal |= (uint16_t)(ALARM_WARNING << u8Offset);
    }
    else if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID].bEnableNotification)
    {
        _u16TempAlarmVal |= (uint16_t)(ALARM_NOTIFICATION << u8Offset);
    }

}

void  MB_APP::UpdateDGVoltAlarms(uint8_t u8AlarmReg, uint8_t u8WarningID, uint8_t u8ShutdownID, uint8_t u8Offset)
{
    if((_gcuAlarm.ArrAlarmMonitoring[u8WarningID].bEnableMonitoring)
            || (_gcuAlarm.ArrAlarmMonitoring[u8ShutdownID].bEnableMonitoring))
    {
        if((_gcuAlarm.ArrAlarmMonitoring[u8ShutdownID].bEnableShutdown)
                && (_gcuAlarm.ArrAlarmMonitoring[u8ShutdownID].bAlarmActive))
        {
            _u16TempAlarmVal |=  (uint16_t)(ALARM_SHUTDOWN << u8Offset);
        }
        else if((_gcuAlarm.ArrAlarmMonitoring[u8WarningID].bEnableWarning)
                && (_gcuAlarm.ArrAlarmMonitoring[u8WarningID].bAlarmActive))
        {
            _u16TempAlarmVal |=  (uint16_t)(ALARM_WARNING << u8Offset);
        }
        else
        {
            _u16TempAlarmVal |=  (uint16_t)(ALARM_INACTIVE << u8Offset);
        }
    }
    else
    {
        _u16TempAlarmVal |=  (uint16_t)(ALARM_DISABLED << u8Offset);
    }
}
