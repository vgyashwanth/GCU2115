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
#include "START_STOP.h"
#include "../HMI/MAIN_UI/MAIN_UI.h"

extern J1939APP *gpJ1939;
MB_APP::KEY_MB_CAN_EVENT_t MB_APP::stMBEvent={};
uint64_t MB_APP::Curr_MB_Valid_Count = 0;
bool MB_APP::bFuelPctBelow15Pct = false;
uint8_t MB_APP::_u8FwRev = 0;

MB_APP::MB_APP(HAL_Manager &hal, CFGZ &cfgz, GCU_ALARMS &gcuAlarm,
        ENGINE_MONITORING &engineMonitoring, AUTO_MODE &Automode, START_STOP &StartStop):
MODBUS(hal.ObjRS485, _AddressGrpLst, _InputStatusGroupLst),
_hal(hal),
_cfgz(cfgz),
_gcuAlarm(gcuAlarm),
_engineMonitoring(engineMonitoring),
_Automode(Automode),
_StartStop(StartStop),
_u16MODBUSCommand(0),
_u16MODBUSOperModeCMD(0),
_au16Grp1Registers{0},
_au16Grp2Registers{0},
#if (TEST_AUTOMATION == YES)
_au16Grp3Registers{0},
_au16Grp4Registers{0},
#endif
_au16Grp5Registers{0},
_au8Grp1StatusBytes{0},
_au8Grp2StatusBytes{0},
_aAddressGrp{
    {DIG_ALARM_1_REG, MODBUS_GRP1_REG_CNT, _au16Grp1Registers, true , false, MODBUS_REG_ANY},
    {MB_COMMAND, MODBUS_GRP2_REG_CNT, _au16Grp2Registers, false, true, MODBUS_REG_ANY}
#if (TEST_AUTOMATION == YES)
    ,{MB_AUX_S1, MB_AUTOMATION_READ_REG_LAST, _au16Grp3Registers, true , false, MODBUS_REG_ANY},
    {MB_AUTOMATION_WRITE_COMMAND, MB_AUTOMATION_WRITE_REG_LAST, _au16Grp4Registers, false, true, MODBUS_REG_ANY}
#endif
    ,{MB_INPUT_REG_PRODUCT_TYPE, MB_INPUT_REG_LAST, _au16Grp5Registers, true, false, MODBUS_REG_INPUT},
    {MB_HOLDING_REG_DG_OP_VOLTAGE_LOW_CUTOFF, MB_HOLDING_REG_LAST, _au16Grp6Registers, true, false, MODBUS_REG_HOLDING}
},
_aInputStatusGrp{
    {MB_DISCRETE_INPUT_SMOKE_FIRE, MB_DISCRETE_INPUT_LAST , _au8Grp1StatusBytes, true, false, MODBUS_REG_DISCRETE_INPUT},
    {MB_COIL_DG_STOP_CMD_OR_OFF, MB_COIL_LAST , _au8Grp2StatusBytes, true, false, MODBUS_REG_COIL},
},
_AddressGrpLst{_aAddressGrp, MODBUS_ADDRESS_GROUPS},
_InputStatusGroupLst{_aInputStatusGrp , MODBUS_INPUTS_COIL_GROUPS},
_u16TempAlarmVal(0)
{

}

void MB_APP::Update()
{
    prvUpdateInputRegisters();
    prvUpdateDiscreteInputRegisters();
    prvUpdateCoils();
    prvUpdateHoldingRegisters();

    prvUpdateElectricalParams();
    prvUpdateAnalogParams();
    prvUpdateStartTripsRunHours();
    prvUpdateTimeStamp();
    prvUpadateDIGInOut();
    prvUpdateBtsParams();
    prvUpdateGCUAlarms();
    prvUpdateTmpParams();
    prvUpdateAUXSensorVal();

    prvUpdateModbusParamInEventLog();

    /*Todo: Need to study the effects of moving this update function here from top*/
    MODBUS::Update();


    prvUpdateMBCommandStatus();
    prvUpdateEGRrelatedRegisters();
    prvUpdateDm01FaultCodesOnModbus();
#if (TEST_AUTOMATION == YES)
    prvUpdatePGNNumber();
    prvUpdateMBWriteRegisterForAutomation();
#endif
}

void MB_APP::SetFwRevision(uint8_t uRevNo)
{
    _u8FwRev = uRevNo;
}

uint16_t MB_APP::GetRegisterValue(MODBUS_WRITE_REGISTERS_t eRegister)
{
    /*Determine the group*/
    uint8_t u8Grp =  prvIdentifyRegisterGroup((uint16_t)eRegister, false, true, MODBUS_REG_ANY);
    /*Determine the start address for the group*/
    uint16_t u16StartAddress =  _aAddressGrp[u8Grp].u16StartAddress;
    return _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress];
}

void MB_APP::SetReadRegisterValue(MODBUS_READ_REGISTERS_t eRegister, uint16_t u16Value)
{
    /*Determine the group*/
    uint8_t u8Grp =  prvIdentifyRegisterGroup((uint16_t)eRegister, true, false, MODBUS_REG_ANY);
    /*Determine the start address for the group*/
    uint16_t u16StartAddress =  _aAddressGrp[u8Grp].u16StartAddress;
    _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress] = u16Value;
}

void MB_APP::SetReadRegisterValue(MODBUS_HOLDING_REGISTERS_t eRegister, uint16_t u16Value)
{
    /*Determine the group*/
    uint8_t u8Grp =  prvIdentifyRegisterGroup((uint16_t)eRegister, true, false, MODBUS_REG_HOLDING);
    /*Determine the start address for the group*/
    uint16_t u16StartAddress =  _aAddressGrp[u8Grp].u16StartAddress;
    _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress] = u16Value;
}

void MB_APP::SetWriteRegisterValue(MODBUS_WRITE_REGISTERS_t eRegister, uint16_t u16Value)
{
    /*Determine the group*/
    uint8_t u8Grp =  prvIdentifyRegisterGroup((uint16_t)eRegister, false, true, MODBUS_REG_ANY);
    /*Determine the start address for the group*/
    uint16_t u16StartAddress =  _aAddressGrp[u8Grp].u16StartAddress;
    _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress] = u16Value;
}

bool MB_APP::prvSetMultipleInputRegisters(MODBUS_INPUT_REGISTERS_t eStartRegister, uint8_t* pu8DataStart, uint8_t u8DataLen)
{
    uint8_t u8RegCnt = (uint8_t)((u8DataLen + 1)/2);
    if((eStartRegister + u8RegCnt - 1) > MB_INPUT_REG_LAST)
    {
        return false;
    }
    else
    {
        uint8_t u8Grp =  prvIdentifyRegisterGroup((uint16_t)eStartRegister, true, false, MODBUS_REG_INPUT);
        if(u8Grp != prvIdentifyRegisterGroup((uint16_t)(eStartRegister + u8RegCnt - 1), true, false, MODBUS_REG_INPUT))
        {
            return false;
        }
        /*Determine the start address for the group*/
        uint16_t u16StartAddress =  _aAddressGrp[u8Grp].u16StartAddress;
        memcpy((void*)&_aAddressGrp[u8Grp].pu16Registers[eStartRegister-u16StartAddress], pu8DataStart, u8DataLen);
        return true;
    }
}

void MB_APP::SetReadRegisterValue(MODBUS_INPUT_REGISTERS_t eRegister, uint16_t u16Value)
{
    /*Determine the group*/
    uint8_t u8Grp =  prvIdentifyRegisterGroup((uint16_t)eRegister, true, false, MODBUS_REG_INPUT);
    /*Determine the start address for the group*/
    uint16_t u16StartAddress =  _aAddressGrp[u8Grp].u16StartAddress;
    _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress] = u16Value;
}

void MB_APP::SetReadDiscreteInputValue(MODBUS_DISCRETE_INPUTS_t eRegister , bool bSetDiscreteInput)
{
    /*Determine the group*/
    uint8_t u8Grp = prvIdentifyInputStatusGroup((uint16_t)eRegister , true , false, MODBUS_REG_DISCRETE_INPUT);
    /*Determine the input status byte index for the discrete input*/
    uint16_t u16InputStatusByteIndex =  (uint16_t)(((uint16_t)eRegister - _aInputStatusGrp[u8Grp].u16StartAddress)/8U);

    if(bSetDiscreteInput)
    {
        //Set the discrete input
        _aInputStatusGrp[u8Grp].pu8InputsStatusByte[u16InputStatusByteIndex] |= (uint8_t)(1U << (((uint8_t)(eRegister-_aInputStatusGrp[u8Grp].u16StartAddress))%8U));
    }
    else
    {
        //Clear the discrete input
        _aInputStatusGrp[u8Grp].pu8InputsStatusByte[u16InputStatusByteIndex] &= (uint8_t)(~(1U << (((uint8_t)(eRegister-_aInputStatusGrp[u8Grp].u16StartAddress))%8U)));
    }
}

void MB_APP::SetReadCoilValue(MODBUS_COILS_t eRegister , bool bSetDiscreteInput)
{
    /*Determine the group*/
    uint8_t u8Grp = prvIdentifyInputStatusGroup((uint16_t)eRegister , true , false, MODBUS_REG_COIL);
    /*Determine the input status byte index for the discrete input*/
    uint16_t u16InputStatusByteIndex =  (uint16_t)(((uint16_t)eRegister - _aInputStatusGrp[u8Grp].u16StartAddress)/8U);

    if(bSetDiscreteInput)
    {
        //Set the discrete input
        _aInputStatusGrp[u8Grp].pu8InputsStatusByte[u16InputStatusByteIndex] |= (uint8_t)(1U << (((uint8_t)(eRegister-_aInputStatusGrp[u8Grp].u16StartAddress))%8U));
    }
    else
    {
        //Clear the discrete input
        _aInputStatusGrp[u8Grp].pu8InputsStatusByte[u16InputStatusByteIndex] &= (uint8_t)(~(1U << (((uint8_t)(eRegister-_aInputStatusGrp[u8Grp].u16StartAddress))%8U)));
    }
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
                                                        bool bReadAccess, bool bWriteAccess, MODBUS_REG_TYPES eRegType)
{
    uint8_t u8RegGrp = 0;
    /*Iterate over all groups*/
    for(uint8_t i=0; i<MODBUS_ADDRESS_GROUPS; i++)
    {
        /*Check if register type is correct for the address group*/
        if(eRegType == _aAddressGrp[i].eRegType)
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
    }
    return u8RegGrp;
}

uint8_t MB_APP::prvIdentifyInputStatusGroup(uint16_t u16DiscreteInputAddress,bool bReadAccess, bool bWriteAccess, MODBUS_REG_TYPES eRegType)
{
    uint8_t u8InputStatusGrp = 0U;
    /*Iterate over all groups*/
    for(uint8_t i=0U; i<MODBUS_INPUTS_COIL_GROUPS; i++)
    {
        /*Check if register type is correct for the address group*/
        if(eRegType == _aInputStatusGrp[i].eRegType)
        {
            /*Check weather this register is within this group*/
            if( (u16DiscreteInputAddress >= _aInputStatusGrp[i].u16StartAddress) &&
                    (u16DiscreteInputAddress <= (_aInputStatusGrp[i].u16EndAddress)) )
            {
                if( (bReadAccess == _aInputStatusGrp[i].isReadSupported) &&
                        (bWriteAccess == _aInputStatusGrp[i].isWriteSupported) )
                {
                    u8InputStatusGrp = i;
                    break;
                }
            }
        }
    }
    return u8InputStatusGrp;
}

void MB_APP::prvUpdateInputRegisters()
{
    //SetReadRegisterValue(MB_INPUT_REG_1 , 27);
    SetReadRegisterValue(MB_INPUT_REG_PRODUCT_TYPE , CPCB4_GEN);
    SetReadRegisterValue(MB_INPUT_REG_PRODUCT_MAKE , MNM);

    uint8_t u8AcSys = 0;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM) == CFGZ::CFGZ_1_PHASE_SYSTEM)
    {
        u8AcSys = 1;
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        u8AcSys = 3;
    }
    SetReadRegisterValue(MB_INPUT_REG_GEN_PHASE, u8AcSys);

    SetReadRegisterValue(MB_INPUT_REG_FUEL_TYPE, 1); //Diesel Gen

    uint32_t u32ProductRating = (uint32_t)((_cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_GEN_RATING)*10)/(0.8));
    prvSetMultipleInputRegisters(MB_INPUT_REG_PRODUCT_RATING_2, (uint8_t*) (&u32ProductRating), 4);

    /*Firmware revision. Scale 0.01*/
    SetReadRegisterValue(MB_INPUT_REG_FW_VER, (uint16_t)(_u8FwRev*100));

    SetReadRegisterValue(MB_INPUT_REG_PROTOCOL_VER, 23);

    uint8_t u8StrSrNo[SR_NOS_MAX_SIZE];

    UI::GetSrNoByIndex(CEditableItem::SRNO_GENSET, u8StrSrNo);
    prvSetMultipleInputRegisters(MB_INPUT_REG_GEN_SERIAL_NO_10, (uint8_t*)u8StrSrNo, GEN_SRNO_LEN);

    UI::GetSrNoByIndex(CEditableItem::SRNO_ENGINE, u8StrSrNo);
    prvSetMultipleInputRegisters(MB_INPUT_REG_ENGINE_SERIAL_NO_10, (uint8_t*)u8StrSrNo, ENG_SRNO_LEN);

    UI::GetSrNoByIndex(CEditableItem::SRNO_ALT, u8StrSrNo);
    prvSetMultipleInputRegisters(MB_INPUT_REG_ALT_SERIAL_NO_10, (uint8_t*)u8StrSrNo, ALT_SRNO_LEN);

    UI::GetSrNoByIndex(CEditableItem::SRNO_MAINCONT, u8StrSrNo);
    prvSetMultipleInputRegisters(MB_INPUT_REG_MAIN_CONTROLLER_SERIAL_NO_10, (uint8_t*)u8StrSrNo, MAIN_CONT_SRNO_LEN);

    UI::GetSrNoByIndex(CEditableItem::SRNO_ENGCONT, u8StrSrNo);
    prvSetMultipleInputRegisters(MB_INPUT_REG_ENGINE_CONTROLLER_SERIAL_NO_10, (uint8_t*)u8StrSrNo, ENG_CONT_SRNO_LEN);

    UI::GetSrNoByIndex(CEditableItem::SRNO_SITEID, u8StrSrNo);
    prvSetMultipleInputRegisters(MB_INPUT_REG_SITE_ID_5, (uint8_t*)u8StrSrNo, SITE_ID_LEN);

    /*Get the current time*/
    RTC::TIME_t currentTime;
    _hal.ObjRTC.GetTime(&currentTime);
    SetReadRegisterValue(MB_INPUT_REG_DATE, (uint16_t)currentTime.u8Day);
    SetReadRegisterValue(MB_INPUT_REG_HOUR, (uint16_t)currentTime.u8Hour);
    SetReadRegisterValue(MB_INPUT_REG_MINUTES, (uint16_t)currentTime.u8Minute);
    SetReadRegisterValue(MB_INPUT_REG_SECONDS, (uint16_t)currentTime.u8Second);
    SetReadRegisterValue(MB_INPUT_REG_MONTH, (uint16_t)currentTime.u8Month);
    SetReadRegisterValue(MB_INPUT_REG_YEAR, currentTime.u16Year);
    
    /*Store generator voltage, resolution 0.01*/
    uint16_t u16Tmp;
    AC_SENSE ac = _hal.AcSensors;
    u16Tmp = (uint16_t)(ac.GENSET_GetVoltageVolts(R_PHASE)*100);
    SetReadRegisterValue(MB_INPUT_REG_GEN_R_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetVoltageVolts(Y_PHASE)*100);
    SetReadRegisterValue(MB_INPUT_REG_GEN_Y_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetVoltageVolts(B_PHASE)*100);
    SetReadRegisterValue(MB_INPUT_REG_GEN_B_VOLTAGE, u16Tmp);

    /*Store generator line voltage, resolution 0.01*/
    u16Tmp = (uint16_t)(ac.GENSET_GetRYVolts()*100);
    SetReadRegisterValue(MB_INPUT_REG_GEN_RY_LINE_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetYBVolts()*100);
    SetReadRegisterValue(MB_INPUT_REG_GEN_YB_LINE_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.GENSET_GetRBVolts()*100);
    SetReadRegisterValue(MB_INPUT_REG_GEN_BR_LINE_VOLTAGE, u16Tmp);

    /*Store generator frequency, resolution 0.01*/
    u16Tmp = (uint16_t)(_gcuAlarm.GetMinGenFreq()*100);
    SetReadRegisterValue(MB_INPUT_REG_GEN_FREQ, u16Tmp);
    
    u16Tmp = 0;
    SetReadRegisterValue(MB_INPUT_REG_ALWAYS0_77, u16Tmp);

    uint16_t u16TotalLoadCurr = 0U;
    /*Store load current values*/
    if(((_Automode.IsMainsContactorClosed()) && (_cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_LOCATION) == CFGZ::ON_LOAD_CABLE) && (_Automode.IsMainsContactorConfigured()) && (_Automode.GetGCUOperatingMode() != BASE_MODES::MANUAL_MODE)) ||
       ((_Automode.IsMainsContactorClosed()) && (_cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_LOCATION) == CFGZ::ON_LOAD_CABLE) && (_Automode.IsMainsContactorConfigured()) && (!_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring)) ||
       ((_cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_CT_LOCATION) == CFGZ::ON_LOAD_CABLE) && (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring) && (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bResultInstant) && (_Automode.GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)))
    {

        u16Tmp = (uint16_t)(ac.MAINS_GetCurrentAmps(R_PHASE)*100);
        u16TotalLoadCurr += u16Tmp;
        SetReadRegisterValue(MB_INPUT_REG_LOAD_CURR_R_PHASE, u16Tmp);
        u16Tmp = (uint16_t)(ac.MAINS_GetCurrentAmps(Y_PHASE)*100);
        u16TotalLoadCurr += u16Tmp;
        SetReadRegisterValue(MB_INPUT_REG_LOAD_CURR_Y_PHASE, u16Tmp);
        u16Tmp = (uint16_t)(ac.MAINS_GetCurrentAmps(B_PHASE)*100);
        u16TotalLoadCurr += u16Tmp;
        SetReadRegisterValue(MB_INPUT_REG_LOAD_CURR_B_PHASE, u16Tmp);

    }
    else if(((!_Automode.IsGenContactorConfigured()) && (!_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring)
             && (!(((_Automode.IsMainsContactorClosed()) && (_Automode.IsMainsContactorConfigured()) && (!_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring))
                   || ((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bEnableMonitoring) && (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bResultInstant) && (_Automode.GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)))))
            || ((_Automode.IsGenContactorClosed()) && (_Automode.IsGenContactorConfigured()) && (_Automode.GetGCUOperatingMode() != BASE_MODES::MANUAL_MODE))
            || ((_Automode.IsGenContactorClosed()) && (_Automode.IsGenContactorConfigured()) && (!_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring))
            || ((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bEnableMonitoring) && (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DG_CONTACTOR_LATCHED].bResultInstant) && (_Automode.GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE)))
    {

        u16Tmp = (uint16_t)(ac.GENSET_GetCurrentAmps(R_PHASE)*100);
        u16TotalLoadCurr += u16Tmp;
        SetReadRegisterValue(MB_INPUT_REG_LOAD_CURR_R_PHASE, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetCurrentAmps(Y_PHASE)*100);
        u16TotalLoadCurr += u16Tmp;
        SetReadRegisterValue(MB_INPUT_REG_LOAD_CURR_Y_PHASE, u16Tmp);
        u16Tmp = (uint16_t)(ac.GENSET_GetCurrentAmps(B_PHASE)*100);
        u16TotalLoadCurr += u16Tmp;
        SetReadRegisterValue(MB_INPUT_REG_LOAD_CURR_B_PHASE, u16Tmp);
    }
    else
    {
        u16Tmp = (uint16_t)(0);
        u16TotalLoadCurr = 0U;
        SetReadRegisterValue(MB_INPUT_REG_LOAD_CURR_R_PHASE, u16Tmp);
        SetReadRegisterValue(MB_INPUT_REG_LOAD_CURR_Y_PHASE, u16Tmp);
        SetReadRegisterValue(MB_INPUT_REG_LOAD_CURR_B_PHASE, u16Tmp);
    }

    /*Store the total load current*/
    SetReadRegisterValue(MB_INPUT_REG_LOAD_CURR_TOTAL, u16TotalLoadCurr);

    /*Store power factor*/
    /*Resolution 0.01*/
    u16Tmp = (uint16_t)(abs(ac.GENSET_GetDispPowerFactor(R_PHASE)*100));
    SetReadRegisterValue(MB_INPUT_REG_PF_R_PHASE, u16Tmp);
    u16Tmp = (uint16_t)(abs(ac.GENSET_GetDispPowerFactor(Y_PHASE)*100));
    SetReadRegisterValue(MB_INPUT_REG_PF_Y_PHASE, u16Tmp);
    u16Tmp = (uint16_t)(abs(ac.GENSET_GetDispPowerFactor(B_PHASE)*100));
    SetReadRegisterValue(MB_INPUT_REG_PF_B_PHASE, u16Tmp);

    /*Store engine speed*/
    uint32_t u32Tmp = (uint32_t)(_gcuAlarm.GetSpeedValue());
    prvSetMultipleInputRegisters(MB_INPUT_REG_ENGINE_SPEED_2, (uint8_t*)(&u32Tmp), 4);

    /*Store generator run hours, scale 0.1*/
    u32Tmp = (uint32_t)((_gcuAlarm.GetSelectedEngRunMin()/60)*10);
    prvSetMultipleInputRegisters(MB_INPUT_REG_GEN_RUN_HRS_TOTAL_2, (uint8_t*)(&u32Tmp), 4);

    /*Store remote run hours, scale 0.1*/
    u32Tmp = (((_engineMonitoring.GetRemoteRunTimeMin())/60)*10);
    prvSetMultipleInputRegisters(MB_INPUT_REG_GEN_RUN_HRS_REMOTE_2, (uint8_t*)(&u32Tmp), 4);
    
    /*Store manual run hours, scale 0.1*/
    u32Tmp = (((_engineMonitoring.GetManualRunTimeMin())/60)*10);
    prvSetMultipleInputRegisters(MB_INPUT_REG_GEN_RUN_HRS_MANUAL_2, (uint8_t*)(&u32Tmp), 4);

    /*Store no load run hours, scale 0.1*/
    u32Tmp = (((_engineMonitoring.GetNoLoadRunTimeMin())/60)*10);
    prvSetMultipleInputRegisters(MB_INPUT_REG_GEN_RUN_HRS_NOLOAD_2, (uint8_t*)(&u32Tmp), 4);

    /*Store on load run hours, scale 0.1*/
    u32Tmp = (((_engineMonitoring.GetOnLoadRunTimeMin())/60)*10);
    prvSetMultipleInputRegisters(MB_INPUT_REG_GEN_RUN_HRS_ONLOAD_2, (uint8_t*)(&u32Tmp), 4);

    /*Store real gen power*/
    /*Resolution of 0.01 KW , i.e 100/1000=> 1/10 */
    u16Tmp = (uint16_t)(ac.GENSET_GetTotalActivePowerWatts()/10);
    SetReadRegisterValue(MB_INPUT_REG_GEN_REAL_OUTPUT_POWER, u16Tmp);

    /*Store apparent gen power*/
    /*Resolution of 0.01 KVA , i.e 100/1000=> 1/10 */
    u16Tmp = (uint16_t)(ac.GENSET_GetTotalApparentPowerVA()/10);
    SetReadRegisterValue(MB_INPUT_REG_GEN_APPARENT_OUTPUT_POWER, u16Tmp);

    /*Store reactive gen power*/
    /*Resolution of 0.01 KVA , i.e 100/1000=> 1/10 */
    u16Tmp = (uint16_t)(ac.GENSET_GetTotalReactivePowerVAR()/10);
    SetReadRegisterValue(MB_INPUT_REG_GEN_REACTIVE_OUTPUT_POWER, u16Tmp);

    /*Total generator energy*/
    /*Resolution of 0.01 KWH, i.e 100/1000=> 1/10 */
    u32Tmp = (uint32_t)(ac.GENSET_GetTotalActiveEnergySinceInitWH()/10);
    prvSetMultipleInputRegisters(MB_INPUT_REG_GEN_ENERGY_TOTAL_2, (uint8_t*)(&u32Tmp), 4);

    SetReadRegisterValue(MB_INPUT_REG_ALWAYS0xFFFF_102, 0xFFFF);

    /*Store canopy temp at far side of engine*/
    A_SENSE::SENSOR_RET_t stCanopyTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_CANOPY_TEMPERATURE);
    if((stCanopyTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL)
        &&(stCanopyTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS))
    {
        SetReadRegisterValue(MB_INPUT_REG_CANOPY_TEMP_FAR_END_RADIATOR, (uint16_t)(stCanopyTemp.stValAndStatus.f32InstSensorVal*10));
    }
    else
    {
        SetReadRegisterValue(MB_INPUT_REG_CANOPY_TEMP_FAR_END_RADIATOR, 0xFFFF);
    }
    
    for(uint8_t i = 0U; i < 6; i++)
    {
        SetReadRegisterValue((MODBUS_INPUT_REGISTERS_t)(MB_INPUT_REG_ALWAYS0xFFFF_104_6 + i), 0xFFFF);
    }
    

    A_SENSE::SENSOR_RET_t sensorVal;
    /*Store engine coolant temperature*/
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE)!=CFGZ::CFGZ_CONVENTIONAL)
            && (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG) == CFGZ::CFGZ_ENABLE))
    {
        u32Tmp = (int16_t)gpJ1939->GetReadData(RX_PGN_ET1_65262,0)*10;
        prvSetMultipleInputRegisters(MB_INPUT_REG_ENGINE_COOLANT_TEMP_2, (uint8_t*)&u32Tmp, 4);
    }
    else
    {
        sensorVal = _gcuAlarm.GetSelectedTempSensVal();

        if((sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) &&
            (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u32Tmp = (int16_t)(round(sensorVal.stValAndStatus.f32InstSensorVal)*10);
            prvSetMultipleInputRegisters(MB_INPUT_REG_ENGINE_COOLANT_TEMP_2, (uint8_t*)&u32Tmp, 4);
        }
        else
        {
            u32Tmp = 0xFFFFFFFFU;
            prvSetMultipleInputRegisters(MB_INPUT_REG_ENGINE_COOLANT_TEMP_2, (uint8_t*)&u32Tmp, 4);
        }
    }

    for(uint8_t i = 0U; i < 6; i++)
    {
        SetReadRegisterValue((MODBUS_INPUT_REGISTERS_t)(MB_INPUT_REG_ALWAYS0xFFFF_112_6 + i), 0xFFFF);
    }

    sensorVal = _gcuAlarm.GetLOPSensorVal();

    if( (sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) &&
        (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
    {
        u16Tmp = (uint16_t)((sensorVal.stValAndStatus.f32InstSensorVal)*(PSI_CONVERSION)*100);
        SetReadRegisterValue(MB_INPUT_REG_LLOP, u16Tmp);
    }
    else
    {
        SetReadRegisterValue(MB_INPUT_REG_LLOP, 0xFFFFU);
    }


    /*Store battery voltage*/
    /*Scale factor 0.01*/
    u16Tmp = (uint16_t)(_gcuAlarm.GetSelectedBatteryVtg()*100);
    SetReadRegisterValue(MB_INPUT_REG_GEN_BATTERY_VOLTAGE, u16Tmp);

    /*Store fuel pct*/
    A_SENSE &sensor = _hal.AnalogSensors;
    sensorVal = sensor.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);

    bFuelPctBelow15Pct = (sensorVal.stValAndStatus.f32InstSensorVal <= 15.0F);
    if((sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) &&
        (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
    {
        /*Scale factor is 0.01*/
        u16Tmp = (uint16_t)(round(sensorVal.stValAndStatus.f32InstSensorVal*100));
        SetReadRegisterValue(MB_INPUT_REG_FUEL_LVL_PCT, u16Tmp);
    }
    else
    {
        sensorVal = sensor.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_0_TO_5V);

        bFuelPctBelow15Pct = (sensorVal.stValAndStatus.f32InstSensorVal <= 15.0F);
        if((sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) &&
            (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            /*Scale factor is 0.01*/
            u16Tmp = (uint16_t)(round(sensorVal.stValAndStatus.f32InstSensorVal*100));
            SetReadRegisterValue(MB_INPUT_REG_FUEL_LVL_PCT, u16Tmp);
        }
        else
        {
            SetReadRegisterValue(MB_INPUT_REG_FUEL_LVL_PCT, 0xFFFFU);
        }
    }

    for(uint8_t i = 0; i < 4; i++)
    {
        SetReadRegisterValue((MODBUS_INPUT_REGISTERS_t)(MB_INPUT_REG_ALWAYS0xFFFF_121_4 + i), 0xFFFFU);
    }

    /*Store total stored cranks. scaling 0.01*/
    u32Tmp = _engineMonitoring.GetCumCrankCnt()*100;
    prvSetMultipleInputRegisters(MB_INPUT_REG_TOTAL_NO_OF_CRANKS_2, (uint8_t*)(&u32Tmp), 4);

    /*Store total failed cranks*/
    u32Tmp = _engineMonitoring.GetCumFailedCrankCnt()*100;
    prvSetMultipleInputRegisters(MB_INPUT_REG_NO_OF_FAILED_CRANKS_2, (uint8_t*)(&u32Tmp), 4);

    for(uint8_t i = 0; i < 6; i++)
    {
        SetReadRegisterValue((MODBUS_INPUT_REGISTERS_t)(MB_INPUT_REG_ALWAYS0xFFFF_129_6 + i), 0xFFFFU);
    }
    
    /*Set fuel type*/
    SetReadRegisterValue(MB_INPUT_REG_ENGINE_DIESEL_GAS, 1); /*1-> Diesel, 2-> Gas*/

    for(uint8_t i = 0; i < 5; i++)
    {
        SetReadRegisterValue((MODBUS_INPUT_REGISTERS_t)(MB_INPUT_REG_ALWAYS0xFFFF_136_5 + i), 0xFFFFU);
    }

    /*Store the start mode*/
    BASE_MODES::GCU_OPERATING_MODE_t eOpMode = BASE_MODES::GetGCUOperatingMode();
    if((eOpMode != BASE_MODES::MANUAL_MODE) && _StartStop.IsGenStarted())
    {
        SetReadRegisterValue(MB_INPUT_STARTED_ON_REMOTE_MANUAL, 1);
    }
    else
    {
        SetReadRegisterValue(MB_INPUT_STARTED_ON_REMOTE_MANUAL, 0);
    }


    SetReadRegisterValue(MB_INPUT_REG_ALWAYS0xFFFF_142, 0xFFFF);

    for(uint8_t i = 0; i < 19; i++)
    {
        SetReadRegisterValue((MODBUS_INPUT_REGISTERS_t)(MB_INPUT_REG_RESERVED_START_19 + i), 0xFFFFU);
    }

    /*Store NCD error hours, scaling 0.1*/
    u32Tmp = (uint32_t)((_gcuAlarm.GetFaultPreset72HrsTimeInMin()*10)/60);
    //u32Tmp = 0xFFFFFFFFU;
    prvSetMultipleInputRegisters(MB_INPUT_REG_NCD_ERR_HRS_2, (uint8_t*)(&u32Tmp), 4);

    /*Store NCD heal hours, scaling 0.1*/
    u32Tmp = (uint32_t)((_gcuAlarm.GetFaultReset40HrsTimeInMin()*10)/60);
    prvSetMultipleInputRegisters(MB_INPUT_REG_NCD_HEAL_HRS_2, (uint8_t*)(&u32Tmp), 4);
    
}

void MB_APP::prvUpdateDiscreteInputRegisters()
{
    bool bStatus = (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::SMOKE_FIRE].bEnableMonitoring) &&
                   (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::SMOKE_FIRE].bResultInstant);
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_SMOKE_FIRE , bStatus);

    bStatus = (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::CANOPY_DOOR_OPEN].bEnableMonitoring) &&
                   (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::CANOPY_DOOR_OPEN].bResultInstant);
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_CANOPY_DOOR_OPEN , bStatus);

    /*Store load on gen*/
    bStatus = (_hal.actuators.GetActStatus(ACTUATOR::ACT_CLOSE_GEN_CONTACTOR) == ACT_Manager::ACT_LATCHED);
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_LOAD_ON_GEN , bStatus);

    /*Store gen fail to start*/
    bStatus = (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FAIL_TO_START].bEnableMonitoring) &&
                   (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FAIL_TO_START].bResultInstant);
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_FAIL_TO_START , bStatus);

    /*Store gen fail to stop*/
    bStatus = (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FAIL_TO_STOP].bEnableMonitoring) &&
                   (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FAIL_TO_STOP].bResultInstant);
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_FAIL_TO_STOP , bStatus);

    /*Store common alarm*/
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_GEN_COMMON_FAULT , _gcuAlarm.IsCommonAlarm());

    /*Store LLOP fault*/
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_GEN_LLOP_FAULT , _gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::LLOP_SWITCH].bResultInstant);

    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_ALWAYS0_7 , false); /*Always 0*/

    /*Store Fuel level below 15 pct alarm*/
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_LOW_FUEL_LVL_AT_15PCT , bFuelPctBelow15Pct);

    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_CANOPY_TEMP_HIGH , _gcuAlarm.IsCanopyTempSensFault());

    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_ENGINE_COOLANT_TEMP_HIGH , _gcuAlarm.IsHighEngTempAlarmActive());

    for(uint8_t i = 0; i < 3; i++)
    {
        SetReadDiscreteInputValue((MODBUS_DISCRETE_INPUTS_t)(MB_DISCRETE_INPUT_ALWAYS0_11_3 + i), false);
    }

    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_DG_IDLE_RUN , START_STOP::IsMonitorDGIdleRunTrue());

    bStatus = (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::OVERLOAD].bEnableMonitoring) &&
                   (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::OVERLOAD].bResultInstant);
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_DG_OVERLOAD , bStatus);

    bStatus = (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::VBAT_UV].bEnableMonitoring) &&
                   (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::VBAT_UV].bResultInstant);
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_DG_BATT_LOW , bStatus);

    for(uint8_t i = 0; i < 10; i++)
    {
        SetReadDiscreteInputValue((MODBUS_DISCRETE_INPUTS_t)(MB_DISCRETE_INPUT_ALWAYS0_17_10 + i) , false);
    }

    uint64_t u64Temp = (_gcuAlarm.GetSelectedEngRunMin())/60;
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_GEN_MAINT_ALARM_50HRS, (u64Temp >= 50));
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_GEN_MAINT_ALARM_100HRS, (u64Temp >= 100));
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_GEN_MAINT_ALARM_500HRS, (u64Temp >= 500));
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_GEN_MAINT_ALARM_1000HRS, (u64Temp >= 1000));
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_GEN_MAINT_ALARM_2000HRS, (u64Temp >= 2000));

    for(uint8_t i = 0; i < 5; i++)
    {
        SetReadDiscreteInputValue((MODBUS_DISCRETE_INPUTS_t)(MB_DISCRETE_INPUT_ALWAYS0_32_5 + i) , false);
    }

    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_MAIN_CONTROLLER_FAIL_ALARM, (_gcuAlarm.GetEgrEcuFaultStatus() > 0) 
                                     || ((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::ALARM_COM_FAIL].bEnableMonitoring == true) 
                                        && (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::ALARM_COM_FAIL].bResultInstant)));

    for(uint8_t i = 0; i < 3; i++)
    {
        SetReadDiscreteInputValue((MODBUS_DISCRETE_INPUTS_t)(MB_DISCRETE_INPUT_ALWAYS0_38_3 + i) , false);
    }

    bStatus = (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::OVERSPEED_L1].bEnableMonitoring) &&
                   (_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::OVERSPEED_L1].bResultInstant);
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_OVER_SPD_ALARM , bStatus);
    
    for(uint8_t i = 0; i < 4; i++)
    {
        SetReadDiscreteInputValue((MODBUS_DISCRETE_INPUTS_t)(MB_DISCRETE_INPUT_ALWAYS0_42_4 + i) , false);
    }

    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_RESERVED_46, false);

    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_WARNING , (_gcuAlarm.GetFaultPreset72HrsTimeInMin() >= _cfgz.GetEGRWarningTimer())); //(gpJ1939->IsEGRWarningPresent())
    
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_NCD_WARNING , gpJ1939->IsFaultCodeReceived(5838, 2));
    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_NCD_FAULT , gpJ1939->IsFaultCodeReceived(5838, 3));

    GCU_ALARMS::EGR_FAULT_LIST_t eEgrFault = _gcuAlarm.GetEgrEcuFaultStatus();

    SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_FAULT, (_gcuAlarm.GetFaultPreset72HrsTimeInMin() >= _cfgz.GetEGRShutdownTimer())); //(eEgrFault > GCU_ALARMS::EGR_NO_FAULT)

    if(eEgrFault == GCU_ALARMS::EGR_ECU_FAULT)
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_ECU_UNHEALTHY , true);
    }
    else
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_ECU_UNHEALTHY , false);
    }

    if(eEgrFault == GCU_ALARMS::EGR_TEMP_SENSOR_FAULTY)
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_TEMP_SENS_OPEN_WARNING , true);
    }
    else
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_TEMP_SENS_OPEN_WARNING , false);
    }

    if(eEgrFault == GCU_ALARMS::EGR_TEMP_SENSOR_OUT_OF_EX_PIPE)
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_TEMP_SENS_FAULTY_WARNING , true);
    }
    else
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_TEMP_SENS_FAULTY_WARNING , false);
    }

    if(eEgrFault == GCU_ALARMS::EGR_VALVE_WIRE_OPEN)
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_VALVE_OPEN_WARNING , true);
    }
    else
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_VALVE_OPEN_WARNING , false);
    }

    if(eEgrFault == GCU_ALARMS::EGR_SENSOR_FAULTY)
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_VALVE_SENS_FAULTY_WARNING , true);
    }
    else
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_VALVE_SENS_FAULTY_WARNING , false);
    }

    if(eEgrFault == GCU_ALARMS::EGR_ECU_VALVE_SHORT)
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_VALVE_NOT_LIFTING_WARNING , true);
    }
    else
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_VALVE_NOT_LIFTING_WARNING , false);
    }
    
    if(eEgrFault == GCU_ALARMS::EGR_VALVE_NOT_CLOSING)
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_VALVE_NOT_CLOSING_WARNING , true);
    }
    else
    {
        SetReadDiscreteInputValue(MB_DISCRETE_INPUT_EGR_VALVE_NOT_CLOSING_WARNING , false);
    }
}

void MB_APP::prvUpdateCoils()
{
    START_STOP::SS_STATE_t eSsState = START_STOP::GetStartStopSMDState();
    if( (eSsState == START_STOP::ID_STATE_SS_STOPPING) || (eSsState == START_STOP::ID_STATE_SS_STOP_HOLD)
        || (ENGINE_MONITORING::IsEngineOff()) || (START_STOP::IsStopCommand()))
    {
        SetReadCoilValue(MB_COIL_DG_STOP_CMD_OR_OFF, true);
    }
    else
    {
        SetReadCoilValue(MB_COIL_DG_STOP_CMD_OR_OFF, false);
    }
    
    if( (eSsState == START_STOP::ID_STATE_SS_PREHEAT) || (eSsState == START_STOP::ID_STATE_SS_START_WAIT)||
        (eSsState == START_STOP::ID_STATE_SS_CRANKING)|| (eSsState == START_STOP::ID_STATE_SS_CRANK_REST)|| (eSsState == START_STOP::ID_STATE_SS_ENG_ON)||
        (ENGINE_MONITORING::IsEngineOn()) )
    {
        SetReadCoilValue(MB_COIL_DG_START_CMD_OR_ON, true);
    }
    else
    {
        SetReadCoilValue(MB_COIL_DG_START_CMD_OR_ON, false);
    } 
    
    BASE_MODES::GCU_OPERATING_MODE_t eOpMode = BASE_MODES::GetGCUOperatingMode();
    if(eOpMode == BASE_MODES::MANUAL_MODE)
    {
        SetReadCoilValue(MB_COIL_MANUAL_MODE ,true);
        SetReadCoilValue(MB_COIL_REMOTE_MODE ,false);
    }
    else
    {
        SetReadCoilValue(MB_COIL_MANUAL_MODE ,false);
        SetReadCoilValue(MB_COIL_REMOTE_MODE ,true);
    }

    for(uint8_t i = 0; i < 5; i++)
    {
        SetReadCoilValue((MODBUS_COILS_t)(MB_COIL_FUTURE_USE_4_5 + i),false);
    }
}

void MB_APP::prvUpdateHoldingRegisters()
{
    SetReadRegisterValue(MB_HOLDING_REG_DG_OP_VOLTAGE_LOW_CUTOFF, _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD));
    SetReadRegisterValue(MB_HOLDING_REG_DG_OP_VOLTAGE_HIGH_CUTOFF, _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD));
    SetReadRegisterValue(MB_HOLDING_REG_START_FIRST_CRANK_SET, _cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_HOLD_TIME));
    for(uint8_t i = 0; i < 2; i++)
    {
        SetReadRegisterValue((MODBUS_HOLDING_REGISTERS_t)(MB_HOLDING_REG_ALWAYS0xFFFF_3_2 + i), 0xFFFF);
    }
    SetReadRegisterValue(MB_HOLDING_REG_REST_AFTER_FIRST_CRANK, _cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_REST_TIME));
    SetReadRegisterValue(MB_HOLDING_REG_ALWAYS0xFFFF_6_1, 0xFFFF);
    SetReadRegisterValue(MB_HOLDING_REG_DG_OVERLOAD_TRIP, _cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_OVERLOAD_MON_DELAY));
}

void MB_APP::prvUpdateTimeStamp()
{

#if (TEST_AUTOMATION == YES)

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

#if (TEST_AUTOMATION == YES)

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

    u32Tmp = (uint32_t)(ac.GENSET_GetTotalApparentEnergySinceInitVAH()/1000);
    SetReadRegisterValue(MB_GEN_CUMM_APPARENT_ENERGY_1, (uint16_t)(u32Tmp & 0xFFFFU));
    SetReadRegisterValue(MB_GEN_CUMM_APPARENT_ENERGY_2, (uint16_t)(u32Tmp>>16));

    u32Tmp = (uint32_t)(ac.GENSET_GetTotalReactiveEnergySinceInitVARH()/1000);
    SetReadRegisterValue(MB_GEN_CUMM_REACTIVE_ENERGY_1, (uint16_t)(u32Tmp & 0xFFFFU));
    SetReadRegisterValue(MB_GEN_CUMM_REACTIVE_ENERGY_2, (uint16_t)(u32Tmp>>16));

    u32Tmp = (uint32_t)(_hal.AcSensors.MAINS_GetTotalActiveEnergySinceInitWH()/1000);
    SetReadRegisterValue(MB_MAINS_CUMM_ACTIVE_ENERGY_1, (uint16_t)(u32Tmp & 0xFFFFU));
    SetReadRegisterValue(MB_MAINS_CUMM_ACTIVE_ENERGY_2, (uint16_t)(u32Tmp>>16));

    u16Tmp = (uint16_t)(ac.MAINS_GetVoltageVolts(R_PHASE));
    SetReadRegisterValue(MB_MAINS_L1_N_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.MAINS_GetVoltageVolts(Y_PHASE));
    SetReadRegisterValue(MB_MAINS_L2_N_VOLTAGE, u16Tmp);
    u16Tmp = (uint16_t)(ac.MAINS_GetVoltageVolts(B_PHASE));
    SetReadRegisterValue(MB_MAINS_L3_N_VOLTAGE, u16Tmp);

#if (TEST_AUTOMATION == YES)

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
    A_SENSE::SENSOR_RET_t sensorVal = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED};
    uint16_t  u16Tmp = 0;

    /*Initialize registers to zero*/
    SetReadRegisterValue(MB_OIL_PRESSURE, u16Tmp);
    SetReadRegisterValue(MB_COOLANT_TEMPERATURE, u16Tmp);
    SetReadRegisterValue(MD_FUEL_PERCENTAGE, u16Tmp);
    SetReadRegisterValue(MB_FUEL_IN_LIT, u16Tmp);


    if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE)!=CFGZ::CFGZ_CONVENTIONAL)
        && (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG) == CFGZ::CFGZ_ENABLE))
    { 
        u16Tmp =(uint16_t)(round(gpJ1939->GetReadData(RX_PGN_EFL_P1_65263, 0)*10) * 0.01);
        SetReadRegisterValue(MB_OIL_PRESSURE, u16Tmp);
    }
    else
    {
        sensorVal = _gcuAlarm.GetLOPSensorVal();

        if((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) >= CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
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

    }

    if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE)!=CFGZ::CFGZ_CONVENTIONAL)
            && (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG) == CFGZ::CFGZ_ENABLE))
    {
        u16Tmp = (int16_t)gpJ1939->GetReadData(RX_PGN_ET1_65262,0);
        SetReadRegisterValue(MB_COOLANT_TEMPERATURE, u16Tmp);
    }
    else
    {
        sensorVal = _gcuAlarm.GetSelectedTempSensVal();

        if((sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) &&
            (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16Tmp = (int16_t)(round(sensorVal.stValAndStatus.f32InstSensorVal));
            SetReadRegisterValue(MB_COOLANT_TEMPERATURE, u16Tmp);
        }
    }


    sensorVal = sensor.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);

    if((sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) &&
        (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
    {
        /*Scale factor is 0.1*/
        u16Tmp = (uint16_t)(round(sensorVal.stValAndStatus.f32InstSensorVal*10));
        SetReadRegisterValue(MD_FUEL_PERCENTAGE, u16Tmp);

        u16Tmp = (uint16_t)round((sensorVal.stValAndStatus.f32InstSensorVal/100) *_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY));
        SetReadRegisterValue(MB_FUEL_IN_LIT, u16Tmp);
    }
    else
    {
        sensorVal = sensor.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_0_TO_5V);

        if((sensorVal.eStatus == A_SENSE::SENSOR_READ_SUCCESS) &&
            (sensorVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            /*Scale factor is 0.1*/
            u16Tmp = (uint16_t)(round(sensorVal.stValAndStatus.f32InstSensorVal*10));
            SetReadRegisterValue(MD_FUEL_PERCENTAGE, u16Tmp);

            if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_WITH_STEP) == CFGZ::CFGZ_ENABLE)
            {
                sensorVal.stValAndStatus.f32InstSensorVal =
                        sensorVal.stValAndStatus.f32InstSensorVal
                        * (float)((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_HEIGHT_1)* _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_LENGTH_1))
                                + (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_HEIGHT_2)* _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_LENGTH_2)))
                                *(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_WIDTH))/(1000000 * 100);
            }
            else
            {
                sensorVal.stValAndStatus.f32InstSensorVal =
                        sensorVal.stValAndStatus.f32InstSensorVal
                        * (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_HEIGHT_1)* _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_LENGTH_1))
                        *(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_TANK_WIDTH))/(1000000 * 100);
            }

            u16Tmp = (uint16_t)round(sensorVal.stValAndStatus.f32InstSensorVal);
            SetReadRegisterValue(MB_FUEL_IN_LIT, u16Tmp);
        }
        else
        {
            SetReadRegisterValue(MD_FUEL_PERCENTAGE, 0U);
            SetReadRegisterValue(MB_FUEL_IN_LIT, 0U);
        }
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

#if (TEST_AUTOMATION == YES)

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
            prvUpdateAlarmRegValue(u8AlarmID3, u8Offset);
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
            prvUpdateAlarmRegStatus(u8AlarmID1, u8Offset);
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
    AC_SENSE &ac = _hal.AcSensors;
    SetReadRegisterValue(MB_TMP_RUN_HOURS, (uint16_t)(_engineMonitoring.GetTamperedRunTimeMin()/60));
    SetReadRegisterValue(MB_TMP_RUN_MINUTES, (uint16_t)(_engineMonitoring.GetTamperedRunTimeMin()%60));

    uint32_t u32Tmp = (uint32_t)(ac.GENSET_GetTotalTamperedActiveEnergySinceInitWH()/1000);
    SetReadRegisterValue(MB_GEN_TMP_ACTIVE_ENERGY_1, (uint16_t)(u32Tmp & 0xFFFFU));
    SetReadRegisterValue(MB_GEN_TMP_ACTIVE_ENERGY_2, (uint16_t)(u32Tmp>>16));
}

void MB_APP::prvUpdateAUXSensorVal()
{

#if (TEST_AUTOMATION == YES)

    A_SENSE &sensor = _hal.AnalogSensors;
    A_SENSE::SENSOR_RET_t  stTemp;
    uint16_t u16AuxSensorVal = 0;

/* S1 */
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        // S1 as shelter temperature
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_SHELTER_TEMPERATURE);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (int16_t)(round(stTemp.stValAndStatus.f32InstSensorVal*10));
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        // S1 as shelter temperature
        stTemp = sensor.GetSensorValue(AnalogSensor::A_SENSE_CANOPY_TEMPERATURE);
        if((stTemp.eStatus == A_SENSE::SENSOR_READ_SUCCESS) && (stTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_NORMAL) )
        {
            u16AuxSensorVal = (int16_t)(round(stTemp.stValAndStatus.f32InstSensorVal*10));
        }
    }
    SetReadRegisterValue(MB_AUX_S1, u16AuxSensorVal);

    u16AuxSensorVal = 0;
/* S2 */
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

void MB_APP::prvUpdateModbusParamInEventLog()
{
    
    CFGZ::LATEST_PRODUCT_SPECIFIC_DATA_t static ProductData={};
    static uint64_t Prev_MB_Valid_Count = 0;
    if(GCU_ALARMS::_bUpdateModbusCountCalc)
    {
        GCU_ALARMS::_bUpdateModbusCountCalc =  false;
        Curr_MB_Valid_Count = MODBUS::MB_Valid_Count;
        if( Prev_MB_Valid_Count!= Curr_MB_Valid_Count)
        {
            _cfgz.GetProductSpecificData(&ProductData);
            ProductData.u16ProductParam[CFGZ::PS_MB_COUNT]++;
            _cfgz.WriteProductSpecificData(&ProductData);
        }
        Prev_MB_Valid_Count = Curr_MB_Valid_Count;
    }
}

void MB_APP::prvUpdateGCUAlarms()
{
    /* DIG ALARM 1 */
    _u16TempAlarmVal =0;
    
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::CANOPY_DOOR_OPEN].bAlarmActive << 0);
    _u16TempAlarmVal |= (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::SMOKE_FIRE].bAlarmActive << 1U); /* smoke fire */
    _u16TempAlarmVal |= (uint16_t)(_gcuAlarm.IsLowOilPresAlarmActive() << 2U); /* LOP sensor/switch */
    _u16TempAlarmVal |=   (uint16_t)(((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DIG_IN_D].bAlarmActive) || (_gcuAlarm.IsHighEngTempAlarmActive()) )<< 3);
    _u16TempAlarmVal |= (uint16_t)((_Automode.IsGenContactorClosed()) << 4U); /* DG contactor on */
    _u16TempAlarmVal |= (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::V_BELT_BROKEN_SWITCH].bAlarmActive << 5U); /* V belt*/
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DIG_IN_F].bAlarmActive << 6);
    /* Low fuel level switch / sensor*/
    _u16TempAlarmVal |=   (uint16_t)((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::LFL_SWITCH].bAlarmActive ||
                                      _gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::LOW_FUEL_LEVEL_NOTIFICATION].bAlarmActive ||
                                      _gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::LOW_FUEL_LEVEL_SHUTDOWN].bAlarmActive) << 7U); 

    _u16TempAlarmVal |= (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::ESTOP].bAlarmActive << 8U); /* Emergency stop */

    if(_Automode.GetGCUOperatingMode() == BASE_MODES::MANUAL_MODE) /* Auto / Manual mode */
    {
        _u16TempAlarmVal |=   (uint16_t)(1 << 9U);
    }
    else if((_Automode.GetGCUOperatingMode() == BASE_MODES::AUTO_MODE) ||
            (_Automode.GetGCUOperatingMode() == BASE_MODES::BTS_MODE) ||
            (_Automode.GetGCUOperatingMode() == BASE_MODES::CYCLIC_MODE))
    {
        _u16TempAlarmVal |=   (uint16_t)(0 << 9U);
    }

    _u16TempAlarmVal |= (uint16_t)(1 << 10U); /* Reserved*/
    _u16TempAlarmVal |= (uint16_t)(1 << 11U); /* Reserved*/
    _u16TempAlarmVal |= (uint16_t)(1 << 12U); /* Reserved */

    _u16TempAlarmVal |= (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::RWL_SWITCH].bAlarmActive << 13U); /* Low water level */

    _u16TempAlarmVal |= (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::SUPERCAP_FAIL].bAlarmActive << 14U); /* Supercapacitor failure */

    _u16TempAlarmVal |= (uint16_t)(1 << 15U); /* Reserved */

    SetReadRegisterValue(DIG_ALARM_1_REG, _u16TempAlarmVal);

    /* DIG ALARM 2 */
    _u16TempAlarmVal =0;

    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::MAINS_CONTACTOR_LATCHED].bResultInstant << 0);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::OVERLOAD].bAlarmActive << 1U);
    _u16TempAlarmVal |=   (uint16_t)((_Automode.GetMainsStatus() != BASE_MODES::MAINS_HELATHY)<< 2U);
    _u16TempAlarmVal |=   (uint16_t)(1 << 3U);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::FAIL_TO_START].bResultLatched << 4U);
    _u16TempAlarmVal |=   (uint16_t)(1 << 5); /* Reserved */
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::OVERSPEED_L1].bAlarmActive << 6U);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::UNDERSPEED].bAlarmActive << 7U);
    _u16TempAlarmVal |=   (uint16_t)((uint16_t)_engineMonitoring.IsEngineOn() << 8U);
    _u16TempAlarmVal |=   (uint16_t)(1 << 9U); /* Reserved */
    //For Remote_SS bresultInstant is applicable and not bAlarmActive because not action is enabled.
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::REMOTE_SS].bResultInstant << 10U);
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DIG_IN_G].bAlarmActive << 11U);
    _u16TempAlarmVal |=   (uint16_t)((_gcuAlarm.IsRPhaseOverVoltAlarmActive() || _gcuAlarm.IsYPhaseOverVoltAlarmActive() || _gcuAlarm.IsBPhaseOverVoltAlarmActive() || _gcuAlarm.IsRPhaseUnderVoltAlarmActive() || _gcuAlarm.IsYPhaseUnderVoltAlarmActive() || _gcuAlarm.IsBPhaseUnderVoltAlarmActive()) <<12U);
    _u16TempAlarmVal |=   (uint16_t)(1 << 13); /* Reserved */
    _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::BATT_CHG_FAIL].bAlarmActive << 14);
    _u16TempAlarmVal |=   (uint16_t)(1 << 15); /* Reserved */

    SetReadRegisterValue(DIG_ALARM_2_REG, _u16TempAlarmVal);

#if (TEST_AUTOMATION == YES)

    /* Alarm 1 */
    _u16TempAlarmVal =0;

    prvUpdateEngSensorAlarms(GCU_ALARMS::LOW_OIL_PRESS_SHUTDOWN, GCU_ALARMS::LOW_OIL_PRESS_WARNING, GCU_ALARMS::LLOP_SWITCH, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::HIGH_WATER_TEMP_SHUTDOWN, GCU_ALARMS::HIGH_WATER_TEMP_WARNING, GCU_ALARMS::HWT_SWITCH, THIRD_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::LOW_FUEL_LEVEL_SHUTDOWN, GCU_ALARMS::LOW_FUEL_LEVEL_NOTIFICATION, GCU_ALARMS::LFL_SWITCH, SECOND_NIBBLE);
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
        prvUpdateEngSensorAlarms(GCU_ALARMS::OPEN_LOP_SENS_CKT, SECOND_NIBBLE);
    }
    else
    {
        prvUpdateEngSensorAlarms(GCU_ALARMS::OPEN_LOP_SENS_CKT, SECOND_NIBBLE);
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

    prvUpdateEngSensorAlarms(GCU_ALARMS::DG_B_UV_SHUTDOWN, GCU_ALARMS::DG_B_UV_WARNING, FOURTH_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::DG_B_OV_SHUTDOWN, GCU_ALARMS::DG_B_OV_WARNING, THIRD_NIBBLE);
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
    prvUpdateEngSensorAlarms(GCU_ALARMS::SUPERCAP_FAIL, SECOND_NIBBLE);
    prvUpdateEngSensorAlarms(GCU_ALARMS::HIGH_OIL_PRESS_DETECTED, FIRST_NIBBLE);
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
    prvUpdateEngSensorAlarms(GCU_ALARMS::HIGH_LUBE_OIL_TEMP_SHUTDOWN,GCU_ALARMS::HIGH_LUBE_OIL_TEMP_WARNING,THIRD_NIBBLE);

    _u16TempAlarmVal |= (uint16_t)(_Automode.IsTimerStateEnabled(BASE_MODES::CYCLIC_ON_TIMER) << 12);

    _u16TempAlarmVal |= (uint16_t)(1 << 13U);

    _u16TempAlarmVal |= (uint16_t)(_Automode.IsTimerStateEnabled(BASE_MODES::CYCLIC_OFF_TIMER) << 14);

    _u16TempAlarmVal |= (uint16_t)(1 << 15U);


    SetReadRegisterValue(MB_ALARM_16, _u16TempAlarmVal);

    /* Alarm 17 */
    _u16TempAlarmVal = 0;

    _u16TempAlarmVal |= (uint16_t)(1 << 4U);
    _u16TempAlarmVal |= (uint16_t)(1 << 3U);
    
    _u16TempAlarmVal |= 0xFFF0;

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
        /*
         * Soujanya M. 06.07.2023
         * If GCU is in Manual mode and night mode restrict is active
         * then send 7 value.
         * As per request received from Automation
         * test team(Amol S.)*/
        if(_Automode.IsNightModeRestrictOn())
        {
            _u16TempAlarmVal |= ((uint16_t)1U << 13U);
            _u16TempAlarmVal |= ((uint16_t)1U << 12U);
            _u16TempAlarmVal |= ((uint16_t)1U << 11U);
        }
        else
        {
            _u16TempAlarmVal |= ((uint16_t)1U << 12U);
        }
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


  _u16TempAlarmVal =0;

  _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::EB_MCCB_ON_FEEDBACK_ALARM].bAlarmActive << 0);
  _u16TempAlarmVal |=   (uint16_t)(_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::DG_MCCB_ON_FEEDBACK_ALARM].bAlarmActive << 1);
  SetReadRegisterValue(MB_MCCB_FEEDBACK_ALARM, _u16TempAlarmVal);
}

void MB_APP::prvUpadateDIGInOut()
{
    /* SOLID STATE Output */
    _u16TempAlarmVal = 0;
    /* DIG in A to H*/
    uint8_t u8LocalCnt = 7;

    for(uint16_t u16Local= CFGZ::ID_OUT_A_SOURCE; u16Local <= CFGZ::ID_OUT_H_SOURCE; u16Local=u16Local+2)
    {
        if(_hal.actuators.GetActStatus((ACTUATOR::ACTUATOR_TYPS_t)_cfgz.GetCFGZ_Param((CFGZ::UINT8_PARAMS_t)u16Local))
                == ACT_Manager::ACT_LATCHED)
        {
            _u16TempAlarmVal |= (uint16_t)(1U << u8LocalCnt);
        }
        u8LocalCnt--;
    } 

    _u16TempAlarmVal |= (uint16_t)(1 << 8U); /* Reserved */
    _u16TempAlarmVal |= (uint16_t)(1 << 9U); /* Reserved */

    _u16TempAlarmVal |=   (uint16_t)((_Automode.IsGenContactorClosed()) << 10U);
    _u16TempAlarmVal |=   (uint16_t)((_Automode.IsMainsContactorClosed()) << 11U);
    _u16TempAlarmVal |=   (uint16_t)(1 << 12U); /* Reserved */
    _u16TempAlarmVal |=   (uint16_t)(1 << 13U); /* Reserved */
    _u16TempAlarmVal |=   (uint16_t)(1 << 14U); /* Reserved */
    _u16TempAlarmVal |=   (uint16_t)(1 << 15U); /* Reserved */

    SetReadRegisterValue(SOLID_STATE_OP_REG, _u16TempAlarmVal);

#if (TEST_AUTOMATION == YES)

    u8LocalCnt=15;
    _u16TempAlarmVal =0;

    for(uint8_t u8Local=D_SENSE::DI_A; u8Local <=D_SENSE::DI_P; u8Local++)
    {
        if(_hal.DigitalSensors.GetDigInputState(u8Local)==DigitalSensor::SENSOR_LATCHED)
        {
            _u16TempAlarmVal |= (uint16_t)(1U<<u8LocalCnt);
        }
        u8LocalCnt--;
    }

    SetReadRegisterValue(MB_DIG_IP_STATUS, _u16TempAlarmVal);

    _u16TempAlarmVal =0;
    u8LocalCnt = 15;

    for(uint16_t u16Local=CFGZ::ID_OUT_A_SOURCE; u16Local <=CFGZ::ID_OUT_P_SOURCE; u16Local=u16Local+2)
    {
        if(_hal.actuators.GetActStatus((ACTUATOR::ACTUATOR_TYPS_t)_cfgz.GetCFGZ_Param((CFGZ::UINT8_PARAMS_t)u16Local))
                == ACT_Manager::ACT_LATCHED)
        {
            _u16TempAlarmVal |= (uint16_t)(1U << u8LocalCnt);
        }
        u8LocalCnt--;
    }
    SetReadRegisterValue(MB_DIG_OP_STATUS, _u16TempAlarmVal);

    _u16TempAlarmVal =0;
    _u16TempAlarmVal |= (uint16_t)((_gcuAlarm.ArrAlarmMonitoring[GCU_ALARMS::SHELTER_TEMP_START_DG].bAlarmActive)  << 0U);
    SetReadRegisterValue(MB_SHELT_TEMP, _u16TempAlarmVal);

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

void MB_APP::prvUpdateLatestDM1Messages(void)
{
    uint8_t u8NoOfSPNToBeSent;
    uint16_t u16Local = MB_SPN1;
    J1939APP::J1939_DM_MSG_DECODE stDmMsg = {};

    if(gpJ1939->GetDm1MsgCount() < 15)
    {
        u8NoOfSPNToBeSent = gpJ1939->GetDm1MsgCount();

    }
    else
    {
        u8NoOfSPNToBeSent = 15;
    }

    if(u8NoOfSPNToBeSent==0)
    {
        u8NoOfSPNToBeSent =15;

    }
    _u16TempAlarmVal = 0;
    for(u16Local = MB_SPN1 ; u16Local <= MB_FMI15; u16Local++ )
    {
        SetReadRegisterValue((MODBUS_READ_REGISTERS_t)u16Local, _u16TempAlarmVal);
    }

    for(u16Local = MB_SPN1 ; u16Local <= MB_FMI15; u16Local++ )
    {
        /* Here the FMI_index = SPN_index+2, where SPN will be using
         * two registers and FMI uses one register */
        u8NoOfSPNToBeSent--;

        _u16TempAlarmVal = 0;
        stDmMsg = gpJ1939->GetDM1Message(u8NoOfSPNToBeSent);
        _u16TempAlarmVal= (uint16_t) (stDmMsg.u32SpnNo >> 16U);
        SetReadRegisterValue((MODBUS_READ_REGISTERS_t)u16Local, _u16TempAlarmVal);

        _u16TempAlarmVal= 0;
        u16Local++;
        _u16TempAlarmVal = (uint16_t)( stDmMsg.u32SpnNo & 0xFFFF);
        SetReadRegisterValue((MODBUS_READ_REGISTERS_t)u16Local, _u16TempAlarmVal);

        u16Local++;
        _u16TempAlarmVal=0;
        _u16TempAlarmVal = (uint16_t)stDmMsg.u8FMI;
        SetReadRegisterValue((MODBUS_READ_REGISTERS_t)u16Local, _u16TempAlarmVal);

        if(!u8NoOfSPNToBeSent)
        {
            break;
        }
    }

}

void MB_APP::prvUpdateEGRrelatedRegisters(void)
{
    uint16_t u16Temp = 0U;
    if(_gcuAlarm.IsEgrInputConfigured())
    {
        switch(_gcuAlarm.GetEgrEcuFaultStatus())
        {
            case GCU_ALARMS::EGR_NO_FAULT:
            {
                u16Temp = 1U; /* bit 0 : ECU Healthy */
            }
            break;
            case GCU_ALARMS::EGR_ECU_FAULT:
            {
                u16Temp = (uint16_t)(1 << 1); /* bit 1: ECU Faulty */
            }
            break;
            case GCU_ALARMS::EGR_VALVE_NOT_CLOSING:
            {
                u16Temp = (uint16_t)(1 << 7); /* bit 7: Valve Not Closing */
            }
            break;
            case GCU_ALARMS::EGR_ECU_VALVE_SHORT:
            {
                u16Temp = (uint16_t)(1 << 6); /* bit 6: Valve Not Lifting / EGR valve is short */
            }
            break;
            case GCU_ALARMS::EGR_SENSOR_FAULTY:
            {
                u16Temp = (uint16_t)(1 << 5); /* bit 5: Valve Sensor not connected or faulty */
            }
            break;
            case GCU_ALARMS::EGR_VALVE_WIRE_OPEN:
            {
                u16Temp = (uint16_t)(1 << 4); /* bit 4: Valve Wire Open */
            }
            break;
            case GCU_ALARMS::EGR_TEMP_SENSOR_OUT_OF_EX_PIPE:
            {
                u16Temp = (uint16_t)(1 << 3); /* bit 3: Temperature Sensor Keep in Ambient Temperature */
            }
            break;
            case GCU_ALARMS::EGR_TEMP_SENSOR_FAULTY:
            {
                u16Temp = (uint16_t)(1 << 2); /* bit 2: Temperature Sensor is open or faulty. */
            }
            break;

            default:
            {
                /* default value(0000H) of u16Temp should be sent */
            }
            break;
        }
    }
    else
    {
        /* keep 0 in register. as EGR input is not configured, GCU can't identify the fault. */
    }
    SetReadRegisterValue(MB_EGR_FAULT_NOTIFICATION_INFO, u16Temp);

    u16Temp = 0U;
    /* Max value of fault time : 72 * 60 = 4320 */
    u16Temp = _gcuAlarm.GetFaultPreset72HrsTimeInMin();
    SetReadRegisterValue(MB_EGR_FAULT_NOTIFICATION_TIME, u16Temp);

    u16Temp = 0U;
    /* Max value of heal time : 40 * 60 = 2400 */
    u16Temp = _gcuAlarm.GetFaultReset40HrsTimeInMin();
    SetReadRegisterValue(MB_EGR_HEAL_TIME, u16Temp);

    /* Lower Nibble is used to indicate EGR alarms info
       0000B -> No alarm from EGR
       0001B -> Notification from EGR
       0010B -> Warning from EGR
       0011B -> Shutdown from EGR
       Upper 3 nibbles reserved
    */
    u16Temp = 0U;
    if(gpJ1939 -> IsEGRShutdownPresent())
    {
        u16Temp |= 3U; /* 0011B for shutdown from EGR */
    }
    else if(gpJ1939->IsEGRWarningPresent())
    {
        u16Temp |= 2U; /* 0010B for warning from EGR */
    }
    else if(_gcuAlarm.IsAlarmActive(GCU_ALARMS::EGR_FAULT_NOTIFICATION))
    {
        u16Temp |= 1U; /* 0001B for notification from EGR */
    }
    else
    {
        /* 0000B for no shutdown from EGR*/
    }
    SetReadRegisterValue(MB_EGR_ALARMS_INFO, u16Temp);

}

void MB_APP::prvUpdateDm01FaultCodesOnModbus(void)
{
    uint8_t u8NoOfSPNToBeSent = 0U;
     uint16_t u16Local = MB_SPN1;
    J1939APP::J1939_DM_MSG_DECODE stDmMsg = {0U, 0U, 0U};

    u8NoOfSPNToBeSent = ((gpJ1939->GetDm1MsgCount()) & 0x0FU); /* limited to Max 15 Messages i.e 0 to 14 */

    /* clear DM registers */
    _u16TempAlarmVal = 0U;
    for(u16Local = MB_SPN1 ; u16Local <= MB_FMI3; u16Local++ )
    {
        SetReadRegisterValue((MODBUS_READ_REGISTERS_t)u16Local, _u16TempAlarmVal);
    }

    if((!gpJ1939->IsCommunicationFail()) && (u8NoOfSPNToBeSent > 0U))
    {
        for(u16Local = MB_SPN1 ; u16Local <= MB_FMI3; u16Local += 3U)
        {
            /* Loop will run from first SPN registers to last SPN register */

            u8NoOfSPNToBeSent--; /* decrementing variable to transmit recently received Fault codes */

            _u16TempAlarmVal = 0;
            stDmMsg = gpJ1939->GetDM1Message(u8NoOfSPNToBeSent);
            /* 16 MSB's of SPN of one Fault code */
            _u16TempAlarmVal= (uint16_t) (stDmMsg.u32SpnNo >> 16U);
            SetReadRegisterValue((MODBUS_READ_REGISTERS_t)u16Local, _u16TempAlarmVal);

            _u16TempAlarmVal= 0;
            /* 16 LSB's of SPN of one Fault code */
            _u16TempAlarmVal = (uint16_t)( stDmMsg.u32SpnNo & 0xFFFF);
            SetReadRegisterValue((MODBUS_READ_REGISTERS_t)(u16Local + 1U), _u16TempAlarmVal);

            _u16TempAlarmVal=0;
            /* FMI of one Fault code */
            _u16TempAlarmVal = (uint16_t)stDmMsg.u8FMI;
            SetReadRegisterValue((MODBUS_READ_REGISTERS_t)(u16Local + 2U), _u16TempAlarmVal);

            if(0U == u8NoOfSPNToBeSent)
            {
                /* No more fault codes received to send */
                break;
            }
        }
    }
}
#if (TEST_AUTOMATION == YES)

void MB_APP::prvUpdatePGNNumber(void)
{
    uint32_t u32PGNNumber;
    static uint16_t u16RxSPNNum;
    static uint64_t* SPNVal;
    uint8_t SPNStatus;

    static DATABASE_RX_PGN_LIST_t eRxPGN;
    if((GetRegisterValue(MB_APP::MB_PGN_LOW_WORD) != 0U)
            || (GetRegisterValue(MB_APP::MB_PGN_HIGH_WORD) != 0U)
            || (GetRegisterValue(MB_APP::MB_SPN_BIT_POSITION) != 0U))
    {
        u32PGNNumber = (uint32_t)GetRegisterValue(MB_APP::MB_PGN_LOW_WORD) | (uint32_t)((GetRegisterValue(MB_APP::MB_PGN_HIGH_WORD) & 0x0003)<<16);

        SetWriteRegisterValue(MB_APP::MB_PGN_LOW_WORD, 0);
        SetWriteRegisterValue(MB_APP::MB_PGN_HIGH_WORD, 0);
        eRxPGN = gpJ1939->GetRXPGNEnum(u32PGNNumber);

        if(eRxPGN < RX_PGN_LAST)
        {
            u16RxSPNNum = gpJ1939->GetSPNIndexFromStartBit(eRxPGN,GetRegisterValue(MB_APP::MB_SPN_BIT_POSITION));
        }
        else
        {
            u16RxSPNNum = 65535;

        }
        SetWriteRegisterValue(MB_APP::MB_SPN_BIT_POSITION , 0);

    }

    if((eRxPGN < RX_PGN_LAST) && (u16RxSPNNum !=65535U))
    {
        double SPNValTemp = gpJ1939->GetReadData(eRxPGN, (uint8_t)u16RxSPNNum);
        SPNVal =  (uint64_t*)&(SPNValTemp);

        SPNStatus = gpJ1939->GetSPNErrorStatus(eRxPGN,(uint8_t)u16RxSPNNum);

        SetReadRegisterValue(MB_APP::MB_SPN_VALUE_1, (uint16_t)((*SPNVal) & 0xFFFF));
        SetReadRegisterValue(MB_APP::MB_SPN_VALUE_2, (uint16_t)((*SPNVal>>16U) & 0xFFFF));
        SetReadRegisterValue(MB_APP::MB_SPN_VALUE_3, (uint16_t)((*SPNVal>>32U) & 0xFFFF));
        SetReadRegisterValue(MB_APP::MB_SPN_VALUE_4, (uint16_t)((*SPNVal>>48U) & 0xFFFF));
        SetReadRegisterValue(MB_APP::MB_SPN_STATUS, SPNStatus);
    }
    else
    {
        SetReadRegisterValue(MB_APP::MB_SPN_VALUE_1,  0xFFFF);
        SetReadRegisterValue(MB_APP::MB_SPN_VALUE_2,  0xFFFF);
        SetReadRegisterValue(MB_APP::MB_SPN_VALUE_3,  0xFFFF);
        SetReadRegisterValue(MB_APP::MB_SPN_VALUE_4,  0xFFFF);
        SetReadRegisterValue(MB_APP::MB_SPN_STATUS,   0xFFFF);
    }

}

void MB_APP::prvUpdateMBWriteRegisterForAutomation(void)
{
    bool bStoreInEeprom = false;
    MBWriteCommand command;
    command.u16CommandSet = GetRegisterValue(MB_APP::MB_AUTOMATION_WRITE_COMMAND);

    if(command.stMBWriteCommandForAutomation.RTC == 1U)
    {
        RTC::TIME_t currentTime;

        currentTime.u8Second =0U;
        currentTime.u8Minute =(uint8_t)(((uint16_t)GetRegisterValue(MB_APP::MB_AUTOMATION_DATE_TIME1) & 0xFF00U)>>8);
        currentTime.u8Hour =(uint8_t)((uint16_t)GetRegisterValue(MB_APP::MB_AUTOMATION_DATE_TIME2) & 0xFFU);
        currentTime.u8DayOfWeek =(uint8_t)(((uint16_t)GetRegisterValue(MB_APP::MB_AUTOMATION_DATE_TIME2)& 0xFF00U)>>8);
        currentTime.u8Day =(uint8_t)((uint16_t)GetRegisterValue(MB_APP::MB_AUTOMATION_DATE_TIME3)& 0xFF);
        currentTime.u8Month =(uint8_t)(((uint16_t)GetRegisterValue(MB_APP::MB_AUTOMATION_DATE_TIME3) & 0xFF00U)>>8);
        currentTime.u16Year = (uint16_t)GetRegisterValue(MB_APP::MB_AUTOMATION_DATE_TIME4) ;
        _hal.ObjRTC.SetTime(&currentTime);
        command.stMBWriteCommandForAutomation.RTC=0;
    }

    if(command.stMBWriteCommandForAutomation.EngineRunTime == 1U)
    {
        bStoreInEeprom = true;
        uint32_t u32EngineRunTimeInMin = (uint32_t)(GetRegisterValue(MB_AUTOMATION_ENGINE_RUN_HOURS_1)*60U) + GetRegisterValue(MB_AUTOMATION_ENGINE_RUN_HOURS_2);
        _engineMonitoring.SetEngineRunTime(u32EngineRunTimeInMin);
        command.stMBWriteCommandForAutomation.EngineRunTime=0;
    }
    
    if(command.stMBWriteCommandForAutomation.MainsRunTime == 1U)
    {
        bStoreInEeprom = true;
        uint32_t u32MainsRunTimeInMin = (uint32_t)(GetRegisterValue(MB_AUTOMATION_MAINS_RUN_HOURS_1)*60U) + GetRegisterValue(MB_AUTOMATION_MAINS_RUN_HOURS_2);
        _engineMonitoring.SetMainsRunTime(u32MainsRunTimeInMin);
        command.stMBWriteCommandForAutomation.MainsRunTime=0;
    }

    if(command.stMBWriteCommandForAutomation.BTSRunTime == 1U)
    {
        bStoreInEeprom = true;
        uint32_t u32BTSRunTimeInMin = (uint32_t)(GetRegisterValue(MB_AUTOMATION_BTS_RUN_HOUR_1)*60U) + GetRegisterValue(MB_AUTOMATION_BTS_RUN_HOUR_2);
        _engineMonitoring.SetBTSRunTime(u32BTSRunTimeInMin);
        command.stMBWriteCommandForAutomation.BTSRunTime=0;
    }

    if(command.stMBWriteCommandForAutomation.ActiveEnergy == 1U)
    {
        bStoreInEeprom = true;
        uint32_t u32GenEnergy = (uint32_t)(GetRegisterValue(MB_AUTOMATION_GEN_ACTIVE_ENERGY_1));
        u32GenEnergy = (u32GenEnergy<<16U) | GetRegisterValue(MB_AUTOMATION_GEN_ACTIVE_ENERGY_2);
        _engineMonitoring.SetGenActiveEnergy(u32GenEnergy);
        command.stMBWriteCommandForAutomation.ActiveEnergy=0;
    }

    if(command.stMBWriteCommandForAutomation.ApparentEnergy == 1U)
    {
        bStoreInEeprom = true;
        uint32_t u32GenEnergy = (uint32_t)(GetRegisterValue(MB_AUTOMATION_GEN_APPARENT_ENERGY_1));
        u32GenEnergy = (u32GenEnergy<<16U) | GetRegisterValue(MB_AUTOMATION_GEN_APPARENT_ENERGY_2);
        _engineMonitoring.SetGenApparentEnergy(u32GenEnergy);
        command.stMBWriteCommandForAutomation.ApparentEnergy=0;
    }

    if(command.stMBWriteCommandForAutomation.ReactiveEnergy == 1U)
    {
        bStoreInEeprom = true;
        uint32_t u32GenEnergy = (uint32_t)(GetRegisterValue(MB_AUTOMATION_GEN_REACTIVE_ENERGY_1));
        u32GenEnergy = (u32GenEnergy<<16U) | GetRegisterValue(MB_AUTOMATION_GEN_REACTIVE_ENERGY_2);
        _engineMonitoring.SetGenReactiveEnergy(u32GenEnergy);
        command.stMBWriteCommandForAutomation.ReactiveEnergy=0;
    }

    if(command.stMBWriteCommandForAutomation.NumberOfStarts == 1U)
    {
        bStoreInEeprom = true;
        uint16_t u16NumberOfstarts = (uint16_t)GetRegisterValue(MB_AUTOMATION_NUMBER_OF_STARTS);
        _engineMonitoring.SetGenNumberOfStarts(u16NumberOfstarts);
        command.stMBWriteCommandForAutomation.NumberOfStarts=0;
    }

    if(command.stMBWriteCommandForAutomation.NumberOfTrips == 1U)
    {
        bStoreInEeprom = true;
        uint16_t u16NumberOftrips = (uint16_t)GetRegisterValue(MB_AUTOMATION_NUMBER_OF_TRIPS);
        _engineMonitoring.SetGenNumberOfTrips(u16NumberOftrips);
        command.stMBWriteCommandForAutomation.NumberOfTrips=0;
    }

    if(bStoreInEeprom)
    {
        _engineMonitoring.StoreCummulativeCnt(ENGINE_MONITORING::CUM_STORE_GENERAL);
        _engineMonitoring.ReadEnergySetEnergyOffset(true);
        bStoreInEeprom = false;
    }
    SetWriteRegisterValue(MB_APP::MB_AUTOMATION_WRITE_COMMAND,command.u16CommandSet);
}


uint16_t MB_APP::GetRegisterValue(MODBUS_FOR_AUTOMATION_WRITE eRegister)
{
    uint8_t u8Grp;
    /*Determine the group*/
    if(MODBUS::GetModbusConfigRegSpecific())
    {
        u8Grp = prvIdentifyRegisterGroup((uint16_t)eRegister, false, true, MODBUS_REG_HOLDING);
    }
    else
    {
        u8Grp = prvIdentifyRegisterGroup((uint16_t)eRegister, false, true, MODBUS_REG_ANY);
    }
    /*Determine the start address for the group*/
    uint16_t u16StartAddress = _aAddressGrp[u8Grp].u16StartAddress;
    return _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress];
}


void MB_APP::SetWriteRegisterValue(MODBUS_FOR_AUTOMATION_WRITE eRegister, uint16_t u16Value)
{
    uint8_t u8Grp;
    /*Determine the group*/
    if(MODBUS::GetModbusConfigRegSpecific())
    {
        u8Grp = prvIdentifyRegisterGroup((uint16_t)eRegister, false, true, MODBUS_REG_HOLDING);
    }
    else
    {
        u8Grp = prvIdentifyRegisterGroup((uint16_t)eRegister, false, true, MODBUS_REG_ANY);
    }
    /*Determine the start address for the group*/
    uint16_t u16StartAddress = _aAddressGrp[u8Grp].u16StartAddress;
    _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress] = u16Value;
}

uint16_t MB_APP::GetRegisterValue(MODBUS_FOR_AUTOMATION_READ eRegister)
{
    uint8_t u8Grp;
    /*Determine the group*/
    if(MODBUS::GetModbusConfigRegSpecific())
    {
        u8Grp = prvIdentifyRegisterGroup((uint16_t)eRegister, true, false, MODBUS_REG_INPUT);
    }
    else
    {
        u8Grp = prvIdentifyRegisterGroup((uint16_t)eRegister, true, false, MODBUS_REG_ANY);
    }
    /*Determine the start address for the group*/
    uint16_t u16StartAddress = _aAddressGrp[u8Grp].u16StartAddress;
    return _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress];
}

void MB_APP::SetReadRegisterValue(MODBUS_FOR_AUTOMATION_READ eRegister, uint16_t u16Value)
{
    uint8_t u8Grp;
    /*Determine the group*/
    if(MODBUS::GetModbusConfigRegSpecific())
    {
        u8Grp = prvIdentifyRegisterGroup((uint16_t)eRegister, true, false, MODBUS_REG_INPUT);
    }
    else
    {
        u8Grp = prvIdentifyRegisterGroup((uint16_t)eRegister, true, false, MODBUS_REG_ANY);
    }
    /*Determine the start address for the group*/
    uint16_t u16StartAddress = _aAddressGrp[u8Grp].u16StartAddress;
    _aAddressGrp[u8Grp].pu16Registers[eRegister-u16StartAddress] = u16Value;
}

void MB_APP::SetAutomationRegTypeToAny()
{
    _aAddressGrp[2].eRegType = MODBUS::MODBUS_REG_ANY;
    _aAddressGrp[3].eRegType = MODBUS::MODBUS_REG_ANY;
}

void MB_APP::SetAutomationRegTypeToInput()
{
    _aAddressGrp[2].eRegType = MODBUS::MODBUS_REG_INPUT;
    _aAddressGrp[3].eRegType = MODBUS::MODBUS_REG_HOLDING;
}


#endif /* TEST_AUTOMATION == YES */

