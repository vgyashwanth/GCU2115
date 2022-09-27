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
#include "START_STOP.h"

MB_APP::KEY_MB_CAN_EVENT_t MB_APP::stMBEvent={0};
MB_APP::MB_APP(HAL_Manager &hal, CFGZ &cfgz, GCU_ALARMS &gcuAlarm,
        ENGINE_MONITORING &engineMonitoring, AUTO_MODE &Automode):
MODBUS(hal.ObjRS485, _AddressGrpLst),
_hal(hal),
_cfgz(cfgz),
_gcuAlarm(gcuAlarm),
_engineMonitoring(engineMonitoring),
_Automode(Automode),
_u16MODBUSCommand(0),
_u16MODBUSOperModeCMD(0),
_au16Grp1Registers{0},
_au16Grp2Registers{0},
_aAddressGrp{
    {DIG_ALARM_1_REG, MODBUS_GRP1_REG_CNT, _au16Grp1Registers, true , false},
    {MB_COMMAND, MODBUS_GRP2_REG_CNT, _au16Grp2Registers, false, true}},
_AddressGrpLst{_aAddressGrp, MODBUS_ADDRESS_GROUPS},
_u16TempAlarmVal(0)
{

}

void MB_APP::Update()
{
    prvUpdateElectricalParams();
    prvUpdateAnalogParams();
    prvUpdateStartTripsRunHours();
    prvUpdateTimeStamp();
    prvUpadateDIGInOut();
    prvUpdateBtsParams();
    prvUpdateGCUAlarms();
    prvUpdateTmpParams();
    prvUpdateAUXSensorVal();

    /*Todo: Need to study the effects of moving this update function here from top*/
    MODBUS::Update();

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

#if AUTOMATION

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

#endif

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

    u16Tmp = (uint16_t)(ac.GENSET_GetVoltageVolts(R_PHASE));
    SetReadRegisterValue(MB_GEN_L1_N_VOLATGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetVoltageVolts(Y_PHASE));
    SetReadRegisterValue(MB_GEN_L2_N_VOLATGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetVoltageVolts(B_PHASE));
    SetReadRegisterValue(MB_GEN_L3_N_VOLATGE, u16Tmp);

#if AUTOMATION

    /*Resolution 0.1*/
    u16Tmp = (uint16_t)(ac.GENSET_GetRYVolts()*10);
    SetReadRegisterValue(MB_GEN_L1_L2_VOLATGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetYBVolts()*10);
    SetReadRegisterValue(MB_GEN_L2_L3_VOLATGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetRBVolts()*10);
    SetReadRegisterValue(MB_GEN_L3_L1_VOLATGE, u16Tmp);

#endif

    /*Resolution of 0.1 KW , i.e 10/1000=> 1/100 */
    u16Tmp = (uint16_t)(ac.MAINS_GetDispActivePowerWatts(R_PHASE)/100);
    SetReadRegisterValue(MB_MAINS_L1_POWER, u16Tmp);
    u16Tmp = (uint16_t)(ac.MAINS_GetDispActivePowerWatts(Y_PHASE)/100);
    SetReadRegisterValue(MB_MAINS_L2_POWER, u16Tmp);
    u16Tmp = (uint16_t)(ac.MAINS_GetDispActivePowerWatts(B_PHASE)/100);
    SetReadRegisterValue(MB_MAINS_L3_POWER, u16Tmp);


    /*Resolution of 0.1 KW , i.e 10/1000=> 1/100 */
    u16Tmp = (uint16_t)(ac.GENSET_GetDispActivePowerWatts(R_PHASE)/100);
    SetReadRegisterValue(MB_GEN_L1_POWER, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetDispActivePowerWatts(Y_PHASE)/100);
    SetReadRegisterValue(MB_GEN_L2_POWER, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetDispActivePowerWatts(B_PHASE)/100);
    SetReadRegisterValue(MB_GEN_L3_POWER, u16Tmp);

    /*Resolution of 0.1 KVA , i.e 10/1000=> 1/100 */
    u16Tmp = (uint16_t)(ac.GENSET_GetDispApparentPowerVA(R_PHASE)/100);
    SetReadRegisterValue(MB_GEN_L1_APPARENT_POWER, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetDispApparentPowerVA(Y_PHASE)/100);
    SetReadRegisterValue(MB_GEN_L2_APPARENT_POWER, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetDispApparentPowerVA(B_PHASE)/100);
    SetReadRegisterValue(MB_GEN_L3_APPARENT_POWER, u16Tmp);

    /*Resolution of 0.1 KVAr , i.e 10/1000=> 1/100 */
    u16Tmp = (uint16_t)(ac.GENSET_GetDispReactivePowerVAR(R_PHASE)/100);
    SetReadRegisterValue(MB_GEN_L1_REACTIVE_POWER, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetDispReactivePowerVAR(Y_PHASE)/100);
    SetReadRegisterValue(MB_GEN_L2_REACTIVE_POWER, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetDispReactivePowerVAR(B_PHASE)/100);
    SetReadRegisterValue(MB_GEN_L3_REACTIVE_POWER, u16Tmp);

    if(((_Automode.IsMainsContactorClosed()) && (_cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_LOCATION) == CFGZ::ON_LOAD_CABLE) && (_Automode.IsMainsContactorConfigured()) && (_Automode.GetGCUOperatingMode() != BASE_MODES::MANUAL_MODE)) ||
       ((_Automode.IsMainsContactorClosed()) && (_cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_LOCATION) == CFGZ::ON_LOAD_CABLE) && (_Automode.IsMainsContactorConfigured()) && (!_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring)) ||
       ((_cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_LOCATION) == CFGZ::ON_LOAD_CABLE) && (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring) && (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bResultInstant) && (_Automode.GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)))
    {

        u16Tmp = (uint16_t)(ac.MAINS_GetCurrentAmps(R_PHASE));
        SetReadRegisterValue(MB_LOAD_L1_CURRENT, u16Tmp);
        u16Tmp = (uint16_t)(ac.MAINS_GetCurrentAmps(Y_PHASE));
        SetReadRegisterValue(MB_LOAD_L2_CURRENT, u16Tmp);
        u16Tmp = (uint16_t)(ac.MAINS_GetCurrentAmps(B_PHASE));
        SetReadRegisterValue(MB_LOAD_L3_CURRENT, u16Tmp);

    }
    else if(((!_Automode.IsGenContactorConfigured()) && (!_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring)
             && (!(((_Automode.IsMainsContactorClosed()) && (_Automode.IsMainsContactorConfigured()) && (!_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring))
                   || ((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring) && (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bResultInstant) && (_Automode.GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)))))
            || ((_Automode.IsGenContactorClosed()) && (_Automode.IsGenContactorConfigured()) && (_Automode.GetGCUOperatingMode() != BASE_MODES::MANUAL_MODE))
            || ((_Automode.IsGenContactorClosed()) && (_Automode.IsGenContactorConfigured()) && (!_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring))
            || ((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring) && (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bResultInstant) && (_Automode.GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)))
    {

        u16Tmp = (uint16_t)(ac.GENSET_GetCurrentAmps(R_PHASE));
        SetReadRegisterValue(MB_LOAD_L1_CURRENT, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetCurrentAmps(Y_PHASE));
        SetReadRegisterValue(MB_LOAD_L2_CURRENT, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetCurrentAmps(B_PHASE));
        SetReadRegisterValue(MB_LOAD_L3_CURRENT, u16Tmp);
    }
    else
    {
        u16Tmp = (uint16_t)(0);
        SetReadRegisterValue(MB_LOAD_L1_CURRENT, u16Tmp);
        u16Tmp = (uint16_t)(0);
        SetReadRegisterValue(MB_LOAD_L2_CURRENT, u16Tmp);
        u16Tmp = (uint16_t)(0);
        SetReadRegisterValue(MB_LOAD_L3_CURRENT, u16Tmp);
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

    uint32_t u32Tmp = (uint32_t)(ac.GENSET_GetTotalActiveEnergySinceInitWH()/1000);
    SetReadRegisterValue(MB_GEN_CUMM_ACTIVE_ENERGY_1, (uint16_t)(u32Tmp & 0xFFFFU));
    SetReadRegisterValue(MB_GEN_CUMM_ACTIVE_ENERGY_2, (uint16_t)(u32Tmp>>16));

    /*KWH in 0.1 resolution*/

    u32Tmp = (uint32_t)(ac.GENSET_GetTotalApparentEnergySinceInitVAH()/100);
    SetReadRegisterValue(MB_GEN_CUMM_APPARENT_ENERGY_1, (uint16_t)(u32Tmp>>16));
    SetReadRegisterValue(MB_GEN_CUMM_APPARENT_ENERGY_2, (uint16_t)(u32Tmp & 0xFFFFU));

    u32Tmp = (uint32_t)(ac.GENSET_GetTotalReactiveEnergySinceInitVARH()/100);
    SetReadRegisterValue(MB_GEN_CUMM_REACTIVE_ENERGY_1, (uint16_t)(u32Tmp>>16));
    SetReadRegisterValue(MB_GEN_CUMM_REACTIVE_ENERGY_2, (uint16_t)(u32Tmp & 0xFFFFU));

    u32Tmp = (uint32_t)(_hal.AcSensors.MAINS_GetTotalActiveEnergySinceInitWH()/1000);
    SetReadRegisterValue(MB_MAINS_CUMM_ACTIVE_ENERGY_1, (uint16_t)(u32Tmp & 0xFFFFU));
    SetReadRegisterValue(MB_MAINS_CUMM_ACTIVE_ENERGY_2, (uint16_t)(u32Tmp>>16));

    u16Tmp = (uint16_t)(ac.MAINS_GetVoltageVolts(R_PHASE));
    SetReadRegisterValue(MB_MAINS_L1_N_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.MAINS_GetVoltageVolts(Y_PHASE));
    SetReadRegisterValue(MB_MAINS_L2_N_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.MAINS_GetVoltageVolts(B_PHASE));
    SetReadRegisterValue(MB_MAINS_L3_N_VOLTAGE, u16Tmp);

#if AUTOMATION

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

#endif

}


void MB_APP::prvUpdateAnalogParams()
{

    A_SENSE &sensor = _hal.AnalogSensors;

    uint16_t  u16Tmp = 0;

    /*Initialize registers to zero*/
    SetReadRegisterValue(MB_OIL_PRESSURE, u16Tmp);
    SetReadRegisterValue(MB_COOLANT_TEMPERATURE, u16Tmp);
    SetReadRegisterValue(MD_FUEL_PERCENTAGE, u16Tmp);
    SetReadRegisterValue(MB_FUEL_IN_LIT, u16Tmp);

    A_SENSE::SENSOR_RET_t sensorVal = _gcuAlarm.GetLOPSensorVal();

    if((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
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


    sensorVal = _gcuAlarm.GetSelectedTempSensVal();

    if((sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) &&
        (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
    {
        /*Scale factor is 0.1*/
        u16Tmp = (int16_t)(round(sensorVal.stValAndStatus.f32InstSensorVal)*10);
        SetReadRegisterValue(MB_COOLANT_TEMPERATURE, u16Tmp);
    }


    sensorVal = sensor.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);

    if((sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) &&
        (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
    {
        /*Scale factor is 0.1*/
        u16Tmp = (uint16_t)(round(sensorVal.stValAndStatus.f32InstSensorVal));
        SetReadRegisterValue(MD_FUEL_PERCENTAGE, u16Tmp);

        u16Tmp = (uint16_t)round(sensorVal.stValAndStatus.f32InstSensorVal *_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY)/10);
        SetReadRegisterValue(MB_FUEL_IN_LIT, u16Tmp);
    }

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

#if AUTOMATION

    SetReadRegisterValue(MB_NO_OF_STARTS, (uint16_t)_engineMonitoring.GetEngineNoOfStarts());
    SetReadRegisterValue(MB_NO_OF_TRIPS, (uint16_t)_engineMonitoring.GetEngineNoOfTrips());

#endif

    SetReadRegisterValue(MB_ENG_RUN_HOURS, (uint16_t)(_gcuAlarm.GetSelectedEngRunMin()/60));
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

   if(GetRegisterValue(MB_APP::MB_MODE_REG) == 1U)
   {
       stMBEvent.bAutoKey = true;
       stMBEvent.bKeyEvent =true;

       SetWriteRegisterValue(MB_APP::MB_MODE_REG, 0);
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

/*
Abhishek   Date- 26-08-2022
This function should be called when we want to set particular nibble of MB_ALARM register. By default u8AlarmID1 is given highest priority, if it is active then other alarm ID will not be checked.
*/
void MB_APP::prvUpdateEngSensorAlarms(uint8_t u8AlarmID1, uint8_t u8AlarmID2, uint8_t u8AlarmID3, uint8_t u8Offset)
{
    if((_gcuAlarm.ArrAlarmMonitoring[u8AlarmID1].bEnableMonitoring) ||
       (_gcuAlarm.ArrAlarmMonitoring[u8AlarmID2].bEnableMonitoring) ||
       (_gcuAlarm.ArrAlarmMonitoring[u8AlarmID3].bEnableMonitoring))
    {
        if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID1].bAlarmActive)
        {
            prvUpdateAlarmRegValue(u8AlarmID1, u8Offset);
        }
        else if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID2].bAlarmActive)
        {
            prvUpdateAlarmRegValue(u8AlarmID2, u8Offset);
        }
        else if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID3].bAlarmActive)
        {
            prvUpdateAlarmRegValue(u8AlarmID2, u8Offset);
        }
        else
        {
            _u16TempAlarmVal |=(uint16_t)(ALARM_INACTIVE << u8Offset);
        }
    }
    else
    {
        _u16TempAlarmVal |= (uint16_t)(ALARM_DISABLED << u8Offset);
    }
}

/*
Abhishek   Date- 26-08-2022
This function should be called when we want to set particular nibble of MB_ALARM register. By default u8AlarmID1 is given highest priority, if it is active then other alarm ID will not be checked.
*/
void MB_APP::prvUpdateEngSensorAlarms(uint8_t u8AlarmID1, uint8_t u8AlarmID2, uint8_t u8Offset)
{
    if((_gcuAlarm.ArrAlarmMonitoring[u8AlarmID1].bEnableMonitoring) ||
       (_gcuAlarm.ArrAlarmMonitoring[u8AlarmID2].bEnableMonitoring))
    {
        if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID1].bAlarmActive)
        {
            prvUpdateAlarmRegValue(u8AlarmID1, u8Offset);
        }
        else if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID2].bAlarmActive)
        {
            prvUpdateAlarmRegValue(u8AlarmID2, u8Offset);
        }
        else
        {
            _u16TempAlarmVal |=(uint16_t)(ALARM_INACTIVE << u8Offset);
        }
    }
    else
    {
        _u16TempAlarmVal |= (uint16_t)(ALARM_DISABLED << u8Offset);
    }
}

/*
Abhishek   Date- 26-08-2022
This function should be called when we want to set particular nibble of MB_ALARM register. By default u8AlarmID1 is given highest priority, if it is active then other alarm ID will not be checked.
*/
void MB_APP::prvUpdateEngSensorAlarms(uint8_t u8AlarmID1, uint8_t u8Offset)
{
    if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID1].bEnableMonitoring)
    {
        if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID1].bAlarmActive)
        {
            prvUpdateAlarmRegValue(u8AlarmID1, u8Offset);
        }
        else
        {
            _u16TempAlarmVal |=(uint16_t)(ALARM_INACTIVE << u8Offset);
        }
    }
    else
    {
        _u16TempAlarmVal |= (uint16_t)(ALARM_DISABLED << u8Offset);
    }
}


void MB_APP::prvUpdateBtsParams()
{
    SetReadRegisterValue(MB_BTS_CHARGE_STATE,(uint16_t)(_hal.AnalogSensors.GetFilteredVBTSbattVolts()*10));
    SetReadRegisterValue(MB_BTS_RUN_MIN, (uint16_t)(_engineMonitoring.GetBTSRunTimeMin()%60));
    SetReadRegisterValue(MB_BTS_RUN_HRS, (uint16_t)(_engineMonitoring.GetBTSRunTimeMin()/60));
}

void MB_APP::prvUpdateTmpParams()
{
    SetReadRegisterValue(MB_TMP_RUN_HOURS, (uint16_t)(_engineMonitoring.GetTamperedRunTimeMin()/60));
    SetReadRegisterValue(MB_TMP_RUN_MINUTES, (uint16_t)(_engineMonitoring.GetTamperedRunTimeMin()%60));

    uint32_t u32Tmp = (uint32_t)(_engineMonitoring.GetTamprEEPromCummEnergy()/1000);
    SetReadRegisterValue(MB_GEN_TMP_ACTIVE_ENERGY_1, (uint16_t)(u32Tmp & 0xFFFFU));
    SetReadRegisterValue(MB_GEN_TMP_ACTIVE_ENERGY_2, (uint16_t)(u32Tmp>>16));
}

void MB_APP::prvUpdateAUXSensorVal()
{

#if AUTOMATION

    A_SENSE &sensor = _hal.AnalogSensors;
    A_SENSE::SENSOR_RET_t  stTemp;
    uint16_t u16AuxSensorVal = 0;
/* S1 */
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        // S1 as shelter tempreture
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_SHELTER_TEMPERATURE);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (int16_t)(round(stTemp.stValAndStatus.f32InstSensorVal*10));
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        // S1 as s1 Analog sensor
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_S1_SENSOR);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (uint16_t)(stTemp.stValAndStatus.f32InstSensorVal*10);
        }
    }
    SetReadRegisterValue(MB_AUX_S1, u16AuxSensorVal);

/* S2 */
    u16AuxSensorVal = 0;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        // S2 as s2 analog sensor
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_S2_SENSOR);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (uint16_t)(stTemp.stValAndStatus.f32InstSensorVal*10);
        }
    }
    SetReadRegisterValue(MB_AUX_S2, u16AuxSensorVal);

#endif
}

void MB_APP::prvUpdateGCUAlarms()
{
    /* DIG ALARM 1 */
    _u16TempAlarmVal =0;

    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DIG_IN_H].bAlarmActive << 0);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::SMOKE_FIRE].bAlarmActive << 1);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.IsLowOilPresAlarmActive() << 2);
    _u16TempAlarmVal |=   (uint16_t)(((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DIG_IN_D].bAlarmActive) || (_gcuAlarm.IsHighEngTempAlarmActive()) )<< 3);
    _u16TempAlarmVal |=   (uint16_t)(_Automode.IsGenContactorClosed() << 4);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::V_BELT_BROKEN_SWITCH].bAlarmActive << 5);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DIG_IN_F].bAlarmActive << 6);
    _u16TempAlarmVal |=   (uint16_t)((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::LFL_SWITCH].bAlarmActive ||
                                      _gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::LOW_FUEL_LEVEL_NOTIFICATION].bAlarmActive ||
                                      _gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::LOW_FUEL_LEVEL_SHUTDOWN].bAlarmActive) << 7);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::ESTOP].bAlarmActive << 8);

    if(_Automode.GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)
    {
         _u16TempAlarmVal |=   (uint16_t)(1 << 9);
    }
    else if((_Automode.GetGCUOperatingMode() == BASE_MODES::AUTO_MODE) ||
            (_Automode.GetGCUOperatingMode() == BASE_MODES::BTS_MODE) ||
            (_Automode.GetGCUOperatingMode() == BASE_MODES::CYCLIC_MODE))
    {
         _u16TempAlarmVal |=   (uint16_t)(0 << 9);
    }
    else
    {
         _u16TempAlarmVal |=   (uint16_t)(1 << 9);
    }

    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::LOW_FUEL_LEVEL_NOTIFICATION].bAlarmActive << 10);
    _u16TempAlarmVal |=   (uint16_t)(0 << 11);
    _u16TempAlarmVal |=   (uint16_t)(0 << 12);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::RWL_SWITCH].bAlarmActive << 13);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::INVALID_DG_START].bAlarmActive << 15);

    SetReadRegisterValue(DIG_ALARM_1_REG, _u16TempAlarmVal);

    /* DIG ALARM 2 */
    _u16TempAlarmVal =0;

    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bResultInstant << 0);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::OVERLOAD].bAlarmActive << 1);
    _u16TempAlarmVal |=   (uint16_t)((_Automode.GetMainsStatus() != BASE_MODES::MAINS_HELATHY)<< 2);
    _u16TempAlarmVal |=   (uint16_t)(0 << 3);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FAIL_TO_START].bResultLatched << 4);
    _u16TempAlarmVal |=   (uint16_t)(0 << 5);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::OVERSPEED_L1].bAlarmActive << 6);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::UNDERSPEED].bAlarmActive << 7);
    _u16TempAlarmVal |=   (uint16_t)((uint16_t)_engineMonitoring.IsEngineOn() << 8);
    _u16TempAlarmVal |=   (uint16_t)(0 << 9);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::REMOTE_SS].bAlarmActive << 10);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DIG_IN_G].bAlarmActive << 11);
    _u16TempAlarmVal |=   (uint16_t)((_gcuAlarm.IsRPhaseOverVoltAlarmActive() || _gcuAlarm.IsYPhaseOverVoltAlarmActive() || _gcuAlarm.IsBPhaseOverVoltAlarmActive() || _gcuAlarm.IsRPhaseUnderVoltAlarmActive() || _gcuAlarm.IsYPhaseUnderVoltAlarmActive() || _gcuAlarm.IsBPhaseUnderVoltAlarmActive()) << 12);
    _u16TempAlarmVal |=   (uint16_t)(0 << 13);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::BATT_CHG_FAIL].bAlarmActive << 14);
    _u16TempAlarmVal |=   (uint16_t)(0 << 15);

    SetReadRegisterValue(DIG_ALARM_2_REG, _u16TempAlarmVal);

#if AUTOMATION

    /* Alarm 1 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::LOW_OIL_PRESS_SHUTDOWN, GCU_ALARMS::LOW_OIL_PRESS_WARNING, GCU_ALARMS::LLOP_SWITCH, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::HIGH_WATER_TEMP, GCU_ALARMS::HWT_SWITCH, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::LOW_FUEL_LEVEL_SHUTDOWN, GCU_ALARMS::LOW_FUEL_LEVEL_NOTIFICATION, GCU_ALARMS::LOW_FUEL_LVL_SWITCH, SECOND_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::RWL_SWITCH, FIRST_NIBBLE);

    SetReadRegisterValue(MB_ALARM_1, _u16TempAlarmVal);

    /* Alarm 2 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::UNDERSPEED, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::OVERSPEED_L1, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::FAIL_TO_START, SECOND_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::FAIL_TO_STOP, FIRST_NIBBLE);

    SetReadRegisterValue(MB_ALARM_2, _u16TempAlarmVal);

    /* Alarm 3 */
    _u16TempAlarmVal = 0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::UNDERFREQ_SHUTDOWN, GCU_ALARMS::UNDERFREQ_WARNING, SECOND_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::OVERFREQ_SHUTDOWN, GCU_ALARMS::OVERFREQ_WARNING, FIRST_NIBBLE);
    _u16TempAlarmVal |= 0xFF00;

    SetReadRegisterValue(MB_ALARM_3, _u16TempAlarmVal);

    /* Alarm 4 */
    _u16TempAlarmVal = 0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::OVERCURRENT, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::OVERLOAD, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::LOAD_UNBALANCE, SECOND_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::ESTOP, FIRST_NIBBLE);

    SetReadRegisterValue(MB_ALARM_4, _u16TempAlarmVal);

    /* Alarm 5 */
    _u16TempAlarmVal = 0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::CA_FAIL, FOURTH_NIBBLE);
    if(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FILT_MAINTENANCE].bEnableMonitoring)
    {
        if(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FILT_MAINTENANCE].bAlarmActive ||
                _gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FILT_MAINTENANCE_BY_DATE].bAlarmActive)
        {
            if((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FILT_MAINTENANCE].bEnableWarning) ||
                    (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FILT_MAINTENANCE_BY_DATE].bEnableWarning))
            {
                _u16TempAlarmVal |=  (uint16_t)(ALARM_WARNING << THIRD_NIBBLE);
            }
            else if((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FILT_MAINTENANCE].bEnableNotification) ||
                    (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FILT_MAINTENANCE_BY_DATE].bEnableNotification))
            {
                _u16TempAlarmVal |=   (uint16_t)(ALARM_NOTIFICATION << THIRD_NIBBLE);
            }
        }
        else
        {
            _u16TempAlarmVal |=   (uint16_t)(ALARM_INACTIVE << THIRD_NIBBLE);
        }

    }
    else
    {
        _u16TempAlarmVal |=   (uint16_t)(ALARM_DISABLED << THIRD_NIBBLE);
    }
    _u16TempAlarmVal |= 0x00FF;

    SetReadRegisterValue(MB_ALARM_5, _u16TempAlarmVal);

    /* Alarm 6 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::VBAT_UV, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::VBAT_OV, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::OPEN_ENG_TEMP_CKT, SECOND_NIBBLE);
    _u16TempAlarmVal |= 0x000FU;

    SetReadRegisterValue(MB_ALARM_6, _u16TempAlarmVal);

    /* Alarm 7 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::FUEL_THEFT, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::MPU_LOSS, THIRD_NIBBLE);
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        prvUpdateEngSensorAlarms(GCU_ALARMS::LOP_RES_OPEN_CKT, SECOND_NIBBLE);
    }
    else
    {
        prvUpdateEngSensorAlarms(GCU_ALARMS::LOP_CURRENT_OPEN_CKT, SECOND_NIBBLE);
    }
    _u16TempAlarmVal |= 0x000FU;

    SetReadRegisterValue(MB_ALARM_7, _u16TempAlarmVal);

    /* Alarm 8 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_A, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_B, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_C, SECOND_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_D, FIRST_NIBBLE);

    SetReadRegisterValue(MB_ALARM_8, _u16TempAlarmVal);

    /* Alarm 9 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_E, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_F, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_G, SECOND_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_H, FIRST_NIBBLE);

    SetReadRegisterValue(MB_ALARM_9, _u16TempAlarmVal);


    /* Alarm 10 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_I, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_J, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_K, SECOND_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_L, FIRST_NIBBLE);

    SetReadRegisterValue(MB_ALARM_10, _u16TempAlarmVal);

    /* Alarm 11 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_M, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_N, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_O, SECOND_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DIG_IN_P, FIRST_NIBBLE);

    SetReadRegisterValue(MB_ALARM_11, _u16TempAlarmVal);

    /* Alarm 12 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::DG_R_UV_SHUTDOWN, GCU_ALARMS::DG_R_UV_WARNING, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DG_R_OV_SHUTDOWN, GCU_ALARMS::DG_R_OV_WARNING, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DG_Y_UV_SHUTDOWN, GCU_ALARMS::DG_Y_UV_WARNING, SECOND_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DG_Y_OV_SHUTDOWN, GCU_ALARMS::DG_Y_OV_WARNING, FIRST_NIBBLE);

    SetReadRegisterValue(MB_ALARM_12, _u16TempAlarmVal);

    /* Alarm 13 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::DG_B_UV_WARNING, GCU_ALARMS::DG_B_UV_SHUTDOWN, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DG_B_OV_WARNING, GCU_ALARMS::DG_B_OV_SHUTDOWN, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DG_PHASE_ROTATION, SECOND_NIBBLE);
    if(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::EB_PHASE_ROTATION].bEnableMonitoring)
    {
        if(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::EB_PHASE_ROTATION].bEnableNotification && _gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::EB_PHASE_ROTATION].bAlarmActive)
        {
            _u16TempAlarmVal |=   (uint16_t)(ALARM_NOTIFICATION << FIRST_NIBBLE);
        }
        else
        {
            _u16TempAlarmVal |=   (uint16_t)(ALARM_INACTIVE << FIRST_NIBBLE);
        }
    }
    else
    {
        _u16TempAlarmVal |=   (uint16_t)(ALARM_DISABLED << FIRST_NIBBLE);
    }

    SetReadRegisterValue(MB_ALARM_13, _u16TempAlarmVal);

    /* Alarm 14 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::V_BELT_BROKEN_SWITCH, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::HIGH_OIL_PRESSURE, FIRST_NIBBLE);

    A_SENSE::SENSOR_RET_t stFuel = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);
    _u16TempAlarmVal |= (uint16_t)((uint8_t)(stFuel.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT) << SECOND_NIBBLE);

    _u16TempAlarmVal |= 0xF000;

    SetReadRegisterValue(MB_ALARM_14, _u16TempAlarmVal);


    /* Alarm 15 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::AN_SEN_S2_MON, SECOND_NIBBLE);
    _u16TempAlarmVal |= 0xFF0F;

    SetReadRegisterValue(MB_ALARM_15, _u16TempAlarmVal);

    /* Alarm 16 */
    _u16TempAlarmVal = 0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::OPEN_AN_SEN_S1_CKT, FIRST_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::OPEN_AN_SEN_S2_CKT, SECOND_NIBBLE);
    _u16TempAlarmVal |= 0xFF00;

    SetReadRegisterValue(MB_ALARM_16, _u16TempAlarmVal);

    /* Alarm 17 */
    _u16TempAlarmVal = 0;

    _u16TempAlarmVal |= 0xFFFF;

    SetReadRegisterValue(MB_ALARM_17, _u16TempAlarmVal);

    /* DG status */
    _u16TempAlarmVal =0;

    if((MAIN_UI::GetDispMode() ==  DISP_PASSWORD_EDIT_MODE) || (MAIN_UI::GetDispMode() ==  DISP_CONFIG_MODE))
    {
        _u16TempAlarmVal |= ((uint16_t)1U << 15U);
    }

    if((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) ==CFGZ::CFGZ_ENABLE)
                &&(_Automode.GetMainsStatus() == BASE_MODES::MAINS_HELATHY))
    {
        _u16TempAlarmVal |= ((uint16_t)1U << 14U);
    }

    if(_Automode.IsNightModeRestrictOn())
    {
        _u16TempAlarmVal |= ((uint16_t)1U << 13U);
    }
    else if(_Automode.GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)
    {
        _u16TempAlarmVal |= ((uint16_t)0U << 11U);
    }
    else if(_Automode.GetGCUOperatingMode() == BASE_MODES::AUTO_MODE)
    {
        _u16TempAlarmVal |= ((uint16_t)1U << 11U);
    }
    else if(_Automode.GetGCUOperatingMode() == BASE_MODES::CYCLIC_MODE)
    {
        _u16TempAlarmVal |= ((uint16_t)1U << 12U);
    }
    else if(_Automode.GetGCUOperatingMode() == BASE_MODES::BTS_MODE)
    {
        _u16TempAlarmVal |= ((uint16_t)1U << 12U);
        _u16TempAlarmVal |= ((uint16_t)1U << 11U);
    }

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

   if(!_engineMonitoring.IsEngineOn()&& ( _gcuAlarm.GetActiveAlarmCount() > 0))
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
#endif

}

void MB_APP::prvUpadateDIGInOut()
{
    /* SOLID STATE Output */
    _u16TempAlarmVal = 0;

    _u16TempAlarmVal |=   (uint16_t)(((START_STOP::IsMonitorDGIdleRunTrue() && (_hal.AcSensors.GENSET_GetPercentPower() <5)
                                   && (_Automode.EngineNotInCoolingStage())) || START_STOP::IsEngineOnFailToStopAck()) << 3);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.IsCommonAlarm() << 4);
    _u16TempAlarmVal |=   (uint16_t)((_gcuAlarm.IsSounderAlarmOn()) << 5);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FAIL_TO_START].bResultLatched << 6);
    _u16TempAlarmVal |=   (uint16_t)((START_STOP::IsStartPreheatON()) << 7);
    _u16TempAlarmVal |=   (uint16_t)((START_STOP::IsStopRelayON()) << 8);
    _u16TempAlarmVal |=   (uint16_t)((START_STOP::IsStartRelayON()) << 9);
    _u16TempAlarmVal |=   (uint16_t)((_Automode.IsGenContactorClosed()) << 10);
    _u16TempAlarmVal |=   (uint16_t)((_Automode.IsMainsContactorClosed()) << 11);

    SetReadRegisterValue(SOLID_STATE_OP_REG, _u16TempAlarmVal);

#if AUTOMATION

    uint8_t u8LocalCnt=15,u8Local = 0;
    _u16TempAlarmVal =0;

    for(u8Local=GCU_ALARMS::DIG_IN_A; u8Local <=GCU_ALARMS::DIG_IN_P; u8Local++)
    {
        _u16TempAlarmVal |= ((uint16_t)_gcuAlarm.ArrAlarmMonitoring[u8Local].bAlarmActive << u8LocalCnt);
        u8LocalCnt--;
    }

    SetReadRegisterValue(MB_DIG_IP_STATUS, _u16TempAlarmVal);

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

#endif
}

void MB_APP::prvUpdateAlarmRegValue(uint8_t u8AlarmID, uint8_t u8Offset)
{
    if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID].bEnableMonitoring)
    {
        if(_gcuAlarm.ArrAlarmMonitoring[u8AlarmID].bAlarmActive)
        {
            prvUpdateAlarmRegStatus(u8AlarmID, u8Offset);
        }
        else
        {
            _u16TempAlarmVal |= (uint16_t)(ALARM_INACTIVE << u8Offset);
        }
    }
}

void MB_APP::prvUpdateAlarmRegStatus(uint8_t u8AlarmID, uint8_t u8Offset)
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
