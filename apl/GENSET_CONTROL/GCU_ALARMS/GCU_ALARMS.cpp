/**
 * @file        [GCU_ALARMS]
 * @brief
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        18th June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#include "GCU_ALARMS.h"
#include "config.h"
#include "ENGINE_MONITORING.h"
#include "START_STOP.h"
#include "CHARGING_ALT.h"
#include "BASE_MODES.h"
#include "../EGOV/EGOV.h"
#include "CFGC/CFGC.h"
#include "J1939_APP/J1939APP.h"
extern J1939APP *gpJ1939;
void ReadEventNumber(EEPROM::EVENTS_t evt);
void ReadRollOver(EEPROM::EVENTS_t evt);
void EventWriteCB(EEPROM::EVENTS_t evt);

bool GCU_ALARMS::_bEventNumberReadDone=false;
bool GCU_ALARMS::_bRollOverReadDone=false;
CircularQueue<GCU_ALARMS::EVENT_LOG_Q_t> GCU_ALARMS::_EventQueue = {GCU_ALARMS::_EventQArr,EVENT_LOG_Q_SIZE};
GCU_ALARMS::EVENT_LOG_Q_t GCU_ALARMS::_EventQArr[EVENT_LOG_Q_SIZE]={0};

bool GCU_ALARMS:: bEventWrittenSuccessfully = true;
uint8_t GCU_ALARMS::_u8PossibleMPULossCounter = 0;

GCU_ALARMS::GCU_ALARMS(HAL_Manager &hal, CFGZ &cfgz):
_hal(hal),
_cfgz(cfgz),
_bCommonAlarm(false),
_bCommonNotification(false),
_bCommonWarning(false),
_bCommonElectricTrip(false),
_bCommonShutdown(false),
_bOPSounderAlarm(false),
_bFailToStart(false),
_bFailToStop(false),
_bOpAutoFuelTransfer(false),
_bCLNTTempCtrl(false),
_bBTSBattHealthy(false),
_bHighShelterTemp(false),
_bUpdateFuelTheftCalc(false),
_u8UnderFreqAlarm(0),
_u8OverFreqAlarm(0),
_u8RPhaseOverVoltAlarm(0),
_u8YPhaseOverVoltAlarm(0),
_u8BPhaseOverVoltAlarm(0),
_u8RPhaseUnderVoltAlarm(0),
_u8YPhaseUnderVoltAlarm(0),
_u8BPhaseUnderVoltAlarm(0),
_u8LowFuelLevelAlarm(0),
_u8LowOilPressAlarm(0),
_u8HighOilPressDetectedAlarm(0),
_u8AuxSensS1Alarm(0),
_u8AuxSensS2Alarm(0),
_u8AuxSensS3Alarm(0),
_u8AuxSensS4Alarm(0),
_u8ActuatorFailAlarm(0),
_u8AFTActivationTimeout(0),
_u8HighEngTempAlarm(0),
_u8HighEngTempSwitch(0),
_u8HighCanopyTempAlarm(0),
_u8HighLubeOilTempAlarm(0),
_u8AlarmIndex(0),
_u8DummyZero(0),
_u8DummyOne(1),
_u8Dummy255(255),
_u8MonGenContactor(0),
_u8MonMainsContactor(0),
_u8CrankMon(0),
_u8EngineOff(0),
_u8GenAvailable(0),
_u8EngineOn(0),
_u8GenReady(0),
_u8MonOn(0),
_u8FuelRelayOn(0),
_u8LopSensMon(0),
_u8FuelSensMon(0),
_u8AuxSensS1(0),
_u8AuxSensS2(0),
_u8AuxSensS3(0),
_u8AuxSensS4(0),
_u8LopHiOilPressMon(0),
_u8TempSensMon(0),
_u8LowIdleSpeedMon(0),
_u8FuelTheftAlarm(0),
_u8MPULossAlarm(0),
_u8AlarmArrayIndex(0),
_u8MaintAlarm(0),
_f32FuelLevelOldValue(0),
_FuelSettlingTimer{0, false},
_AFTTimeoutAfterActTimer{0, false},
_SounderAlarmTimer{0,false},
_UpdateAlarmMonTimer{0, false},
_AlarmUpdate{0, false},
_FuelTheftOneHourTimer{0, false},
_FuelTheftWakeUpTimer{0, false},
_ArrAlarmStatus{NULL},
_ArrAlarmValue{0.0,0,0},
_ArrAlarmForDisplay{0},
_u32EventNumber(0),
_u32RolledOverByte(0),
_stEventLog{0}
{
    InitGCUAlarms();
    ClearAllAlarms();
    UTILS_ResetTimer(&_AlarmUpdate);
    UTILS_ResetTimer(&_FuelTheftOneHourTimer);
    _hal.Objeeprom.RequestRead(EXT_EEPROM_CURRENT_EVENT_NO_ADDRESS,(uint8_t*) &_u32EventNumber, 4, ReadEventNumber);
    _hal.Objeeprom.RequestRead(EXT_EEPROM_ROLLED_OVER_ADDRESS,(uint8_t*) &_u32RolledOverByte, 4, ReadRollOver);
}

void GCU_ALARMS::Update(bool bDeviceInConfigMode)
{
    static bool bAlarmUpdate = false;
    A_SENSE::SENSOR_RET_t stLOP = GetLOPSensorVal();
    A_SENSE::SENSOR_RET_t stFuel = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);
    A_SENSE::SENSOR_RET_t stTemp = GetSelectedTempSensVal();

    _u8LopSensMon = (uint8_t)(_u8MonOn && (((stLOP.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT)&&(stLOP.stValAndStatus.eState != ANLG_IP::BSP_STATE_SHORT_TO_BAT))
                                             || (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG ))));
    _u8FuelSensMon = (uint8_t)(stFuel.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT);
    A_SENSE::SENSOR_RET_t stAuxSensS1 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S1_SENSOR);
    A_SENSE::SENSOR_RET_t stAuxSensS2 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S2_SENSOR);
    A_SENSE::SENSOR_RET_t stAuxSensS3 = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED};
    A_SENSE::SENSOR_RET_t stAuxSensS4 = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED};

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        stAuxSensS3 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S3_4_20_SENSOR);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        stAuxSensS3 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S3_0_5_SENSOR);
    }

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        stAuxSensS4 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S4_4_20_SENSOR);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        stAuxSensS4 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S4_0_5_SENSOR);
    }

    _u8AuxSensS1 = (uint8_t)(stAuxSensS1.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT);
    _u8AuxSensS2 = (uint8_t)(stAuxSensS2.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT);
    _u8AuxSensS3 = (uint8_t)((stAuxSensS3.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT)&&(stAuxSensS3.stValAndStatus.eState != ANLG_IP::BSP_STATE_SHORT_TO_BAT));
    _u8AuxSensS4 = (uint8_t)((stAuxSensS4.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT)&&(stAuxSensS4.stValAndStatus.eState != ANLG_IP::BSP_STATE_SHORT_TO_BAT));
    _u8TempSensMon = (uint8_t)(_u8MonOn && ((stTemp.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT) || (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG))));
    _u8LopHiOilPressMon = (uint8_t)(stLOP.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT);

    if(_bEventNumberReadDone && _bRollOverReadDone)
    {
        _bEventNumberReadDone =0;
        _bRollOverReadDone =0;
        if(_u32EventNumber >= CFGC::GetMaxNumberOfEvents())
        {
            _u32EventNumber =0;
        }
        if(_u32RolledOverByte > 1)
        {
            _u32RolledOverByte =0;
        }
    }
    else
    {
        if(UTILS_GetElapsedTimeInSec(&_AlarmUpdate) >= FOUR_SEC)
        {
            bAlarmUpdate = true;
            UTILS_ResetTimer(&_UpdateAlarmMonTimer);
            UTILS_DisableTimer(&_AlarmUpdate);
            for(uint8_t u8Index = 0; u8Index < ALARM_LIST_LAST; u8Index++)
            {
                ConfigureGCUAlarms(u8Index);
            }
            for(uint8_t u8LoggingID = 0; u8LoggingID < ID_ALL_ALARMS_LAST; u8LoggingID++)
            {
                AssignAlarmsForDisplay(u8LoggingID);
            }
        }
        if(UTILS_GetElapsedTimeInSec(&_FuelTheftOneHourTimer) >= 3600)
        {
            UTILS_ResetTimer(&_FuelTheftOneHourTimer);
            _bUpdateFuelTheftCalc = true;
            UTILS_ResetTimer(&_FuelTheftWakeUpTimer);
        }

        _u8PossibleMPULossCounter = EGOV::GetCompCBCount();

        CheckMPULossAlarm();

        if((UTILS_GetElapsedTimeInMs(&_UpdateAlarmMonTimer) >= FIFTY_MSEC) && (bAlarmUpdate))
        {
            if(_EventQueue.Peek(&_stLog) && bEventWrittenSuccessfully)
            {
                bEventWrittenSuccessfully = false;
                _hal.Objeeprom.RequestWrite((_stLog.u32EventNo *sizeof(EVENT_LOG_t))+ EXT_EEPROM_EVENT_LOG_START_ADDRESS ,
                                            (uint8_t*)&_stLog.stEventLog, sizeof(EVENT_LOG_t), EventWriteCB);
                _stLog.u32EventNo++;
                _hal.Objeeprom.RequestWrite(EXT_EEPROM_CURRENT_EVENT_NO_ADDRESS , (uint8_t*)&_stLog.u32EventNo,
                                            EXT_EEPROM_CURRENT_EVENT_NO_LENGTH, NULL);
                EVENT_LOG_Q_t stLocalLog;

                _EventQueue.DeQueue(&stLocalLog);
            }
            if(!bDeviceInConfigMode)
            {
                UTILS_ResetTimer(&_UpdateAlarmMonTimer);
                prvUpdateGCUAlarmsValue();
                prvUpdateAlarmStatus();
                prvUpdateOutputs();
                prvIsFuelTheftAlarm();
                if((_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_SELECTION)==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                        &&(UTILS_GetElapsedTimeInMs(&_FuelSettlingTimer) >= FUEL_SETTLING_DELAY_MSEC))
                {
                    prvAutoFuelTransferFunction();
                }
                prvCoolantTempCtrlFunction();
                prvMainsHighLowOutputs();
                FillDisplayAlarmArray();
            }
        }
    }
}

void GCU_ALARMS::prvUpdateMonParams(uint8_t u8AlarmIndex, uint8_t* Pu8LocalEnable,
                                    bool bMonitoringPolarity, uint8_t u8LoggingID, 
                                    uint8_t u8Threshold, uint16_t u16CounterMax)
{
    ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = (uint8_t *)Pu8LocalEnable;
    ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = bMonitoringPolarity;
    ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = u8LoggingID;
    ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = u8Threshold;
    ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = u16CounterMax;
    ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
}

void GCU_ALARMS::prvUpdateMonParams(uint8_t u8AlarmIndex, uint8_t* Pu8LocalEnable,
                                    bool bMonitoringPolarity, uint8_t u8LoggingID, 
                                    uint16_t u16Threshold, uint16_t u16CounterMax)
{
    ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = (uint8_t *)Pu8LocalEnable;
    ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = bMonitoringPolarity;
    ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = u8LoggingID;
    ArrAlarmMonitoring[u8AlarmIndex].Threshold.u16Value = u16Threshold;
    ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = u16CounterMax;
    ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = TWO_BYTE_INT;
}

void GCU_ALARMS::prvUpdateMonParams(uint8_t u8AlarmIndex, uint8_t* Pu8LocalEnable,
                                    bool bMonitoringPolarity, uint8_t u8LoggingID, 
                                    float f32Threshold, uint16_t u16CounterMax)
{
    ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = (uint8_t *)Pu8LocalEnable;
    ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = bMonitoringPolarity;
    ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = u8LoggingID;
    ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = f32Threshold;
    ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = u16CounterMax;
    ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
}

void GCU_ALARMS::prvSetAlarmAction(uint8_t u8AlarmIndex, uint8_t u8AlarmAction)
{
    prvClearAllAction(u8AlarmIndex);

    if(u8AlarmAction == GCU_ALARMS::ID_NOTIFICATION)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
    }
    else if(u8AlarmAction == GCU_ALARMS::ID_WARNING)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
    }
    else if(u8AlarmAction == GCU_ALARMS::ID_ELECTRICAL_TRIP)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableElectricTrip = true;
    }
    else if(u8AlarmAction == GCU_ALARMS::ID_SHUTDOWN)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
    }
}

void GCU_ALARMS::prvSetAlarmActionfor4to20mASens(uint8_t u8AlarmIndex, uint8_t u8AlarmAction)
{
    prvClearAllAction(u8AlarmIndex);

    if(u8AlarmAction == GCU_ALARMS::ID_NOTIFICATION1)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
    }
    else if(u8AlarmAction == GCU_ALARMS::ID_WARNING1)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
    }
    else if(u8AlarmAction == GCU_ALARMS::ID_ELECTRICAL_TRIP1)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableElectricTrip = true;
    }
    else if(u8AlarmAction == GCU_ALARMS::ID_SHUTDOWN1)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
    }
}

void GCU_ALARMS::prvClearAllAction(uint8_t u8AlarmIndex)
{
    ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = false;
    ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = false;
    ArrAlarmMonitoring[u8AlarmIndex].bEnableElectricTrip = false;
    ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
}

void GCU_ALARMS::prvSetAlarmActivation(uint8_t u8AlarmIndex, uint8_t u8AlarmActivation)
{
    if(u8AlarmActivation == CFGZ::CFGZ_NEVER_ACTIVATE)
    {
        ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = (uint8_t *)&_u8DummyZero;
    }
    else if(u8AlarmActivation == CFGZ::CFGZ_FROM_ENGINE_START)
    {
        ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = (uint8_t *)&_u8EngineOn;
    }
    else if(u8AlarmActivation == CFGZ::CFGZ_FROM_MONITORING_ON)
    {
        ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = (uint8_t *)&_u8MonOn;
    }
    else if(u8AlarmActivation == CFGZ::CFGZ_ALWAYS)
    {
        ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = (uint8_t *)&_u8DummyOne;
    }
    else if((u8AlarmActivation == CFGZ::CFGZ_ACTIVATION_AFTER_FUEL_RELAY_ON) && (u8AlarmIndex == J1939_COM_FAIL))
    {
        ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = (uint8_t *)&_u8FuelRelayOn;
    }
    ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
    ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
    ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
}

void GCU_ALARMS::prvAssignInputSettings(uint8_t u8InputIndex, uint8_t u8InputSource, 
                                        uint8_t u8Activation, uint8_t u8ActivationDelay, 
                                        uint8_t u8AlarmAction, uint8_t u8LoggingID)
{
    if((u8InputIndex >= DIGITAL_INPUT_A )&&(u8InputIndex <= DIG_IN_P )
               &&(u8InputSource != CFGZ:: CFGZ_USER_CONFIGURED_SENSOR))
       {
             ArrAlarmMonitoring[u8InputIndex].bEnableMonitoring = true;
             ArrAlarmMonitoring[u8InputIndex].u8LoggingID = _u8Dummy255;
             ArrAlarmMonitoring[u8InputIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
             ArrAlarmMonitoring[u8InputIndex].bMonitoringPolarity = true;
             ArrAlarmMonitoring[u8InputIndex].Threshold.u8Value = 0;
             ArrAlarmMonitoring[u8InputIndex].ThreshDataType = ONE_BYTE_INT;
             prvClearAllAction(u8InputIndex);
       }

    switch(u8InputSource)
    {
        case CFGZ::CFGZ_USER_CONFIGURED_SENSOR:
            ArrAlarmMonitoring[u8InputIndex].bEnableMonitoring = true;
            ArrAlarmMonitoring[u8InputIndex].u8LoggingID = u8LoggingID;
            prvSetAlarmAction(u8InputIndex, u8AlarmAction);
            prvSetAlarmActivation(u8InputIndex, u8Activation);
            ArrAlarmMonitoring[u8InputIndex].Threshold.f32Value = 0;
            ArrAlarmMonitoring[u8InputIndex].Threshold.u16Value = 0;
            ArrAlarmMonitoring[u8InputIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            break;

        case CFGZ::CFGZ_LOW_FUEL_LEVEL_SWITCH:
            ArrAlarmMonitoring[LOW_FUEL_LVL_SWITCH].bEnableMonitoring = true;
            prvSetAlarmAction(LOW_FUEL_LVL_SWITCH, u8AlarmAction);
            prvSetAlarmActivation(LOW_FUEL_LVL_SWITCH, CFGZ::CFGZ_ALWAYS);
            ArrAlarmMonitoring[LOW_FUEL_LVL_SWITCH].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[LOW_FUEL_LVL_SWITCH_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_LOW_LUBE_OIL_PRESS_SWITCH:
            ArrAlarmMonitoring[LLOP_SWITCH].bEnableMonitoring = true;
            prvSetAlarmAction(LLOP_SWITCH, u8AlarmAction);
            prvSetAlarmActivation(LLOP_SWITCH, CFGZ::CFGZ_FROM_MONITORING_ON);
            ArrAlarmMonitoring[LLOP_SWITCH].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[LLOP_SWITCH_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8MonOn;
            break;

        case CFGZ::CFGZ_HIGH_ENGINE_TEMP_SWITCH:
            ArrAlarmMonitoring[HWT_SWITCH].bEnableMonitoring = true;
            prvSetAlarmAction(HWT_SWITCH, u8AlarmAction);
            prvSetAlarmActivation(HWT_SWITCH, CFGZ::CFGZ_FROM_MONITORING_ON);
            ArrAlarmMonitoring[HWT_SWITCH].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[HWT_SWITCH_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8MonOn;
            break;

        case CFGZ::CFGZ_LOW_WATER_LVL_SWITCH:
            ArrAlarmMonitoring[RWL_SWITCH].bEnableMonitoring = true;
            prvSetAlarmAction(RWL_SWITCH, u8AlarmAction);
            prvSetAlarmActivation(RWL_SWITCH, u8Activation);
            ArrAlarmMonitoring[RWL_SWITCH].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
             ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[WATER_LEVEL_SWITCH_STATUS];
             prvSetAlarmActivation(u8InputIndex, u8Activation);
             break;

        case CFGZ::CFGZ_EMERGENCY_STOP:
            ArrAlarmMonitoring[EMERGENCY_STOP].bEnableMonitoring = true;
            ArrAlarmMonitoring[EMERGENCY_STOP].bEnableShutdown = true;
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[EMERGENCY_STOP_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_REMOTE_START_STOP:
            ArrAlarmMonitoring[REMOTE_START_STOP].bEnableMonitoring = true;
            prvSetAlarmActivation(REMOTE_START_STOP, CFGZ::CFGZ_ALWAYS);
            ArrAlarmMonitoring[REMOTE_START_STOP].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[REMOTE_SS_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_SIMULATE_START:
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[SIM_START_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_SIMULATE_STOP:
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[SIM_STOP_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_SIMULATE_AUTO:
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[SIM_AUTO_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_CLOSE_GEN_OPEN_MAINS_SWITCH:
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[CLOSE_GEN_OPEN_MAINS_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_CLOSE_MAINS_OPEN_GEN_SWITCH:
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[CLOSE_MAINS_OPEN_GEN_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_SIMULATE_MAINS:
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[SIM_MAINS_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_VBEL_BROKEN_SWITCH:
            ArrAlarmMonitoring[V_BELT_BROKEN_SWITCH].bEnableMonitoring = true;
            prvSetAlarmAction(V_BELT_BROKEN_SWITCH, u8AlarmAction);
            prvSetAlarmActivation(V_BELT_BROKEN_SWITCH, u8Activation);
            ArrAlarmMonitoring[V_BELT_BROKEN_SWITCH].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
             ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[V_BELT_BROKEN_SWITCH_STATUS];
             prvSetAlarmActivation(u8InputIndex, u8Activation);
            break;

        case CFGZ::CFGZ_MAINS_CONTACTOR_LATCHED:
            ArrAlarmMonitoring[MAINS_CONTACTOR_LATCHED].bEnableMonitoring = true;
            ArrAlarmMonitoring[FAIL_TO_LATCH_MAINS_CONT].bEnableMonitoring = true;
            ArrAlarmMonitoring[FAIL_TO_OPEN_MAIN_CONT].bEnableMonitoring = true;
            //Delay of alarm value is assigned while assigning the input,
            ArrAlarmMonitoring[FAIL_TO_LATCH_MAINS_CONT].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[FAIL_TO_OPEN_MAIN_CONT].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            prvSetAlarmAction(FAIL_TO_LATCH_MAINS_CONT, u8AlarmAction);
            prvSetAlarmAction(FAIL_TO_OPEN_MAIN_CONT, u8AlarmAction);
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[MAINS_CONT_LATCHED_STATUS ];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_GEN_CONTACTOR_LATCHED:
            ArrAlarmMonitoring[GEN_CONTACTOR_LATCHED].bEnableMonitoring = true;
            ArrAlarmMonitoring[FAIL_TO_LATCH_GEN_CONT].bEnableMonitoring = true;
            ArrAlarmMonitoring[FAIL_TO_OPEN_GEN_CONT].bEnableMonitoring = true;
            //Delay of alarm value is assigned while assigning the input,
            ArrAlarmMonitoring[FAIL_TO_LATCH_GEN_CONT].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[FAIL_TO_OPEN_GEN_CONT].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            prvSetAlarmAction(FAIL_TO_LATCH_GEN_CONT, u8AlarmAction);
            prvSetAlarmAction(FAIL_TO_OPEN_GEN_CONT, u8AlarmAction);
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[GEN_CONT_LATCHED_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_BATTERY_CHARGER_FAIL:
            ArrAlarmMonitoring[BATT_CHG_FAIL].bEnableMonitoring = true;
            prvSetAlarmAction(BATT_CHG_FAIL, u8AlarmAction);
            prvSetAlarmActivation(BATT_CHG_FAIL, u8Activation);
            ArrAlarmMonitoring[BATT_CHG_FAIL].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[BATT_CHG_FAIL_STATUS ];
            prvSetAlarmActivation(u8InputIndex, u8Activation);
            break;

        case CFGZ::CFGZ_SMOKE_FIRE:
            ArrAlarmMonitoring[SMOKE_FIRE].bEnableMonitoring = true;
            prvSetAlarmAction(SMOKE_FIRE, u8AlarmAction);
            prvSetAlarmActivation(SMOKE_FIRE, u8Activation);
            ArrAlarmMonitoring[SMOKE_FIRE].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[SMOKE_FIRE_STATUS ];
            prvSetAlarmActivation(u8InputIndex, u8Activation);
            break;

        case CFGZ::CFGZ_STOP_PANEL_LOCK:
            ArrAlarmMonitoring[PANEL_LOCK].bEnableMonitoring = true;
            ArrAlarmMonitoring[PANEL_LOCK].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[PANEL_LOCK_STATUS ];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_EXT_PANEL_LOCK:
            ArrAlarmMonitoring[EX_AUTO_PANEL_LOCK].bEnableMonitoring = true;
            ArrAlarmMonitoring[EX_AUTO_PANEL_LOCK].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[EX_AUTO_PANEL_LOCK_STATUS ];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_GENERATOR_LOAD_INITHIBIT:
            ArrAlarmMonitoring[GEN_LOAD_INHIBIT].bEnableMonitoring = true;
            ArrAlarmMonitoring[GEN_LOAD_INHIBIT].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[GEN_LOAD_INHIBIT_STATUS ];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_MAINS_LOAD_INHIBIT:
            ArrAlarmMonitoring[MAINS_LOAD_INHIBIT].bEnableMonitoring = true;
            ArrAlarmMonitoring[MAINS_LOAD_INHIBIT].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[MAINS_LOAD_INHIBIT_STATUS ];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case  CFGZ::CFGZ_SOURCE_REGENERATION_SW_INHIBIT_SIGNAL:
            ArrAlarmMonitoring[REGEN_SW_INHIBIT_SIGNAL].bEnableMonitoring = true;
            //Below code is activate IO status on MODBUS/J1939 for particular digital input
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[REGENERATION_SW_STATUS ];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;
    }
}

void GCU_ALARMS::ConfigureGCUAlarms(uint8_t u8AlarmIndex)
{
    bool benableMon=false;
    switch(u8AlarmIndex)
    {
        case LOW_OIL_PRESS_SHUTDOWN:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                    || (_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
                    || (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG)))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_LVL_SHUTDOWN_EN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_LVL_SHUTDOWN_EN) == CFGZ::CFGZ_ENABLE);
                prvUpdateMonParams(u8AlarmIndex, &_u8LopSensMon, false, GCU_ALARMS::Low_Oil_Pressure_id, _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_LVL_SHUTDOWN_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC);
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LUBE_OIL_PRESSURE];
            break;

        case LOW_OIL_PRESS_WARNING:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                    || (_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
                    || (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG)))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_LVL_WARNING_EN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_LVL_WARNING_EN) == CFGZ::CFGZ_ENABLE);
                prvUpdateMonParams(u8AlarmIndex, &_u8LopSensMon, false, GCU_ALARMS::Low_Oil_Pressure_id, _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_LVL_WARNING_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC);
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LUBE_OIL_PRESSURE];
            break;

        case LOW_FUEL_LEVEL_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)) && (_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LOW_LEVEL_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)) && (_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LOW_LEVEL_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex, &_u8FuelSensMon, false, GCU_ALARMS::fuel_level_id, _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SHUT_DN_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FUEL_LEVEL];
            break;

        case LOW_FUEL_LEVEL_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)) && (_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LOW_LEVEL_WARN_EN) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)) && (_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LOW_LEVEL_WARN_EN) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex, &_u8FuelSensMon, false, GCU_ALARMS::fuel_level_id, _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_WARN_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FUEL_LEVEL];
            break;

        case HIGH_ENG_TEMP_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)|| (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG)))
            && (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_SHUTDOWN_EN) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)|| (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG))) && (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_SHUTDOWN_EN) == CFGZ::CFGZ_ENABLE);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_THRESH_TYPE) == CFGZ::CFGZ_LESS_THAN_THRESHOLD)
            {
                prvUpdateMonParams(u8AlarmIndex, &_u8TempSensMon, false, GCU_ALARMS::Low_Water_Temperature_id,(float) _cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CLNT_TEMP_SHUTDOWN_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC);
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_THRESH_TYPE) == CFGZ::CFGZ_GREATER_THAN_THRESHOLD)
            {
                prvUpdateMonParams(u8AlarmIndex, &_u8TempSensMon, true, GCU_ALARMS::High_Water_Temperature_id, (float)_cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CLNT_TEMP_SHUTDOWN_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC);
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENGINE_TEMPERATURE];
            break;

        case HIGH_ENG_TEMP_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_SELECTION)== CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)|| (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG)))
            && (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_WARNING_EN) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_SELECTION)== CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)|| (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG))) && (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_WARNING_EN) == CFGZ::CFGZ_ENABLE);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_THRESH_TYPE) == CFGZ::CFGZ_LESS_THAN_THRESHOLD)
            {
                prvUpdateMonParams(u8AlarmIndex, &_u8TempSensMon, false, GCU_ALARMS::Low_Water_Temperature_id,(float) _cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CLNT_TEMP_WARNING_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC);
            }
            else
            {
                prvUpdateMonParams(u8AlarmIndex, &_u8TempSensMon, true, GCU_ALARMS::High_Water_Temperature_id, (float)_cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CLNT_TEMP_WARNING_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC);
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENGINE_TEMPERATURE];
            break;

        case OVERSPEED:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::Over_Speed_id, _cfgz.GetCFGZ_Param(CFGZ::ID_OVER_SPEED_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_OVER_SPEED_DELAY));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENGINE_SPEED];
            break;

        case GROSS_OVERSPEED:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::Gross_Over_Speed_id, (uint16_t)((_cfgz.GetCFGZ_Param(CFGZ::ID_GROSS_OVER_SPEED_THRESH) * _cfgz.GetCFGZ_Param(CFGZ::ID_OVER_SPEED_THRESH))/100), 2);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENGINE_SPEED];
            break;

        case UNDERSPEED:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = _cfgz.GetCFGZ_Param(CFGZ::ID_UNDERSPEED_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE;
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = _cfgz.GetCFGZ_Param(CFGZ::ID_UNDERSPEED_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE;
            prvUpdateMonParams(u8AlarmIndex, &_u8LowIdleSpeedMon, false, GCU_ALARMS::Under_Speed_id ,_cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_SPEED_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_SPEED_DELAY));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENGINE_SPEED];
            break;

        case OVERFREQ_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_FREQ_SHUT_DN_EN)  == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_FREQ_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::Over_Frequency_id ,_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_FREQ_SHUT_DN_THRESH), (uint16_t)(NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OF_SHUTDOWN_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GENSET_FREQUENCY];
            break;

        case UNDERFREQ_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_FREQ_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_FREQ_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex, &_u8LowIdleSpeedMon, false, GCU_ALARMS::Under_Frequency_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_FREQ_SHUT_DN_THRESH),(uint16_t) (NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UF_SHUTDOWN_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GENSET_FREQUENCY];
            break;

        case GEN_R_OV_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::R_Over_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_SHUTDN_THRESH), (uint16_t)(NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OV_SHUTDOWN_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_R_PHASE_VOLTAGE];
            break;

        case GEN_R_UV_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex, &_u8LowIdleSpeedMon, false, GCU_ALARMS::R_Under_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_SHUTDN_THRESH), (uint16_t)(NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UV_SHUTDOWN_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_R_PHASE_VOLTAGE];
            break;

        case GEN_Y_OV_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::Y_Over_Voltage_id, _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_SHUTDN_THRESH), (uint16_t)(NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OV_SHUTDOWN_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_Y_PHASE_VOLTAGE];
            break;
        case GEN_Y_UV_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            prvUpdateMonParams(u8AlarmIndex, &_u8LowIdleSpeedMon, false, GCU_ALARMS::Y_Under_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_SHUTDN_THRESH), (uint16_t)(NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UV_SHUTDOWN_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_Y_PHASE_VOLTAGE];
            break;

        case GEN_B_OV_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::B_Over_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_SHUTDN_THRESH),(uint16_t)( NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OV_SHUTDOWN_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_B_PHASE_VOLTAGE];
            break;

        case GEN_B_UV_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM);
            prvUpdateMonParams(u8AlarmIndex, &_u8LowIdleSpeedMon, false, GCU_ALARMS::B_Under_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_SHUTDN_THRESH),(uint16_t)( NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UV_SHUTDOWN_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_B_PHASE_VOLTAGE];
            break;

        case CHARG_ALT_UV:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = _cfgz.GetCFGZ_Param(CFGZ::ID_CHARG_ALT_FAIL_ACTION);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, false, GCU_ALARMS::Charge_Fail_id , _cfgz.GetCFGZ_Param(CFGZ::ID_CHARG_ALT_FAIL_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_CHARG_ALT_FAIL_DELAY));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[CHARG_ALT_VOLTAGE];
            break;

        case CHARG_ALT_FAIL:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = _cfgz.GetCFGZ_Param(CFGZ::ID_CHARG_ALT_FAIL_ACTION);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::Charge_Fail_id , (uint16_t)1, 1U);
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_CHARG_ALT_FAIL_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[CHARG_ALT_ERROR];
            break;

        case VBAT_OV:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = _cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_BAT_VTG_ACTION);
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Battery_Over_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_BAT_VTG_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_BAT_VTG_DELAY));
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_BAT_VTG_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[BATTERY_VOLTAGE];
            break;

        case VBAT_UV:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = _cfgz.GetCFGZ_Param(CFGZ::ID_LOW_BAT_VTG_ACTION);
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, GCU_ALARMS::Battery_Under_Voltage_id  , _cfgz.GetCFGZ_Param(CFGZ::ID_LOW_BAT_VTG_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_LOW_BAT_VTG_DELAY));
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_LOW_BAT_VTG_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[BATTERY_VOLTAGE];
            break;

        case VBTS_UV:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = _cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON);
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_THRESHOLD), NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_MON_DELAY));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[BTS_VOLTAGE];
            break;

        case OVERCURRENT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_OVER_CURR_ACTION)));
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::Over_Current_id , _cfgz.GetCFGZ_Param(CFGZ::ID_OVER_CURR_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_OVER_CURR_DELAY));
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_OVER_CURR_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_MAX_CURRENT];
            break;

        case FILT_MAINTENANCE:
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Filter_maintenance_id , _cfgz.GetCFGZ_Param(CFGZ::ID_MAINTENANCE_DUE_HOURS), NO_OF_50MSEC_TICKS_FOR_1SEC*5U);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINTENANCE_ALARM_ACTION) > CFGZ::CFGZ_ACTION_NONE)
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
            }
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINTENANCE_ALARM_ACTION) == CFGZ::CFGZ_ACTION_NOTIFICATION_2)
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = false;
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINTENANCE_ALARM_ACTION) == CFGZ::CFGZ_ACTION_WARNING_2)
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = false;
            }
            else
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = false;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = false;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENG_RUN_HOURS];
            break;

        case   ID_FILT_MAINTENANCE_BY_DATE:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINTENANCE_ALARM_ACTION) > CFGZ::CFGZ_ACTION_NONE)
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true ;

            }

            if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINTENANCE_ALARM_ACTION) ==  CFGZ::CFGZ_ACTION_NOTIFICATION_2)
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = false;
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINTENANCE_ALARM_ACTION) == CFGZ::CFGZ_ACTION_WARNING_2)
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = false;
            }
            else
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = false;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = false;
            }
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Filter_maintenance_id, (uint8_t)0, 20);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINT_DATE];
            break;

        case OVERLOAD:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && _cfgz.GetCFGZ_Param(CFGZ::ID_OL_ACTION);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::Over_Load_id , _cfgz.GetCFGZ_Param(CFGZ::ID_OL_THRESH_PERCENT),  NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_OVER_LOAD_DELAY));
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_OL_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[TOTAL_KW_PERCENT];
            break;

        case FUEL_THEFT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_THEFT_WARN_EN) == CFGZ::CFGZ_ENABLE;
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Fuel_Theft_id , (uint8_t)0, 1U);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FUEL_THEFT_ALARM];
            break;

        case GEN_PHASE_ROTATION:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && _cfgz.GetCFGZ_Param(CFGZ::ID_PH_REVERS_DETECT_EN) == CFGZ::CFGZ_ENABLE;
            prvUpdateMonParams(u8AlarmIndex, &_u8GenReady, true, GCU_ALARMS::Gen_Phase_Rotation_id , (uint8_t)0, 10U);
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_PH_REVERS_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_PHASE_ROTATION_STATUS];
            break;

        case MPU_LOSS:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring =((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE) == CFGZ::CFGZ_MAGNETIC_PICKUP)
                    ||(_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE) == CFGZ::CFGZ_W_POINT_FREQ));
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MPU_LOSS_STATUS];
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = MPU_Loss_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 1;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            break;

        case LOP_CURR_OPEN_CKT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3);
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Lop_Curr_Sens_Open_Ckt_id , (uint8_t)0, 30U);
            prvSetAlarmActionfor4to20mASens(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_FAULT_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = & _ArrAlarmValue[LOP_CURRENT_OPEN_CKT];
            break;

        case OPEN_ENG_TEMP_CKT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_SELECTION)== CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1) && _cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_FAULT_ACTION));
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Engine_Temperature_Ckt_Open_id , (uint8_t)0U, 30U);
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_FAULT_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENG_TEMP_OPEN_CKT];
            break;

        case GEN_R_OV_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::R_Over_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_WARN_THRESH),(uint16_t)( NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OV_WARNING_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_R_PHASE_VOLTAGE];
            break;

        case GEN_R_UV_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex, &_u8LowIdleSpeedMon, false, GCU_ALARMS::R_Under_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_WARN_THRESH),(uint16_t)( NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UV_WARNING_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_R_PHASE_VOLTAGE];
            break;

        case GEN_Y_OV_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::Y_Over_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_WARN_THRESH),(uint16_t)( NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OV_WARNING_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_Y_PHASE_VOLTAGE];
            break;

        case GEN_Y_UV_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            prvUpdateMonParams(u8AlarmIndex, &_u8LowIdleSpeedMon, false, GCU_ALARMS::Y_Under_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_WARN_THRESH), (uint16_t)(NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UV_WARNING_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_Y_PHASE_VOLTAGE];
            break;

        case GEN_B_OV_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::B_Over_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_VTG_WARN_THRESH), (uint16_t)(NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OV_WARNING_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_B_PHASE_VOLTAGE];
            break;

        case GEN_B_UV_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM);
            prvUpdateMonParams(u8AlarmIndex, &_u8LowIdleSpeedMon, false, GCU_ALARMS::B_Under_Voltage_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_VTG_WARN_THRESH), (uint16_t)(NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UV_WARNING_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_B_PHASE_VOLTAGE];
            break;

        case OVERFREQ_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_FREQ_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_FREQ_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::Over_Frequency_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OVER_FREQ_WARN_THRESH), (uint16_t)(NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_OF_WARNING_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GENSET_FREQUENCY];
            break;

        case UNDERFREQ_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_FREQ_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_FREQ_WARN_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex, &_u8LowIdleSpeedMon, false, GCU_ALARMS::Under_Frequency_id , _cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UNDER_FREQ_WARN_THRESH), (uint16_t)(NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_UF_WARNING_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GENSET_FREQUENCY];
            break;

        case LOP_RES_SENS_OPEN_CKT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION)== CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1) && (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION));
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Oil_Pressure_Ckt_Open_id , (uint8_t)0, 30U);
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_CIRCUIT_FAULT_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LOP_RES_OPEN_CKT];
            break;

        case LOP_CURR_SENS_STB:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3);
            prvSetAlarmActionfor4to20mASens(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_FAULT_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = (uint8_t*)&_u8DummyOne;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LOP_CURR_STB];
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = LOP_CURR_SENS_STB_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 30;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            break;

        case OPEN_ANLG_SENS_S1_CKT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1);
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, Aux_S1_Sens_Open_Ckt_id,(uint8_t)0, 10);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[AUX_SENS_S1_OPEN_CKT];
            prvSetAlarmAction(OPEN_ANLG_SENS_S1_CKT, _cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_FAULT_ACTION));
            break;

        case OPEN_ANLG_SENS_S2_CKT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_S2_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1);
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, Aux_S2_Sens_Open_Ckt_id,(uint8_t)0, 10);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[AUX_SENS_S2_OPEN_CKT];
            prvSetAlarmAction(OPEN_ANLG_SENS_S2_CKT, _cfgz.GetCFGZ_Param(CFGZ::ID_S2_SENS_FAULT_ACTION));
            break;

        case OPEN_ANLG_SENS_S3_CKT:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1) //4-20mA Sensor
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmActionfor4to20mASens(OPEN_ANLG_SENS_S3_CKT, _cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_FAULT_ACTION));
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)//0-5V Sensor
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmAction(OPEN_ANLG_SENS_S3_CKT, _cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_FAULT_ACTION));
            }
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, Aux_S3_Sens_Open_Ckt_id,(uint8_t)0, 10);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[AUX_SENS_S3_OPEN_CKT];
            break;

        case OPEN_ANLG_SENS_S4_CKT:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1) //4-20mA Sensor
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmActionfor4to20mASens(OPEN_ANLG_SENS_S4_CKT, _cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_FAULT_ACTION));
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)//0-5V Sensor
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmAction(OPEN_ANLG_SENS_S4_CKT, _cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_FAULT_ACTION));
            }
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, Aux_S4_Sens_Open_Ckt_id,(uint8_t)0, 10);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[AUX_SENS_S4_OPEN_CKT];
            break;

        case STB_ANLG_SENS_S3_CKT:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1) //4-20mA Sensor
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmActionfor4to20mASens(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_FAULT_ACTION));
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)//0-5V Sensor
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_FAULT_ACTION));
            }
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, Aux_S3_STB_id,(uint8_t)0, 10);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[AUX_SENS_S3_STB];
            break;

        case STB_ANLG_SENS_S4_CKT:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1) //4-20mA Sensor
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmActionfor4to20mASens(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_FAULT_ACTION));
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)//0-5V Sensor
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_FAULT_ACTION));
            }
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, Aux_S4_STB_id,(uint8_t)0, 10);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[AUX_SENS_S4_STB];
            break;

        case ANLG_SENS_S1_MON_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE));
            prvUpdateMonParams(u8AlarmIndex, &_u8AuxSensS1, (bool)(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_THRESH_TYPE)), Aux_S1_id, _cfgz.GetCFGZ_Param(CFGZ::ID_S1_SHUTDOWN_THRESH), 40);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ANLG_SENS_S1_VAL];
            break;

        case ANLG_SENS_S1_MON_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_WARN_EN) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_WARN_EN) == CFGZ::CFGZ_ENABLE));
            prvUpdateMonParams(u8AlarmIndex, &_u8AuxSensS1, (bool)(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_THRESH_TYPE)), Aux_S1_id, _cfgz.GetCFGZ_Param(CFGZ::ID_S1_WARNING_THRESH), 40);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ANLG_SENS_S1_VAL];
            break;

        case ANLG_SENS_S2_MON_SHUTDOWN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_S2_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_S2_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_S2_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_S2_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE));
            prvUpdateMonParams(u8AlarmIndex, &_u8AuxSensS2, (bool)(_cfgz.GetCFGZ_Param(CFGZ::ID_S2_THRESH_TYPE)), Aux_S2_id, _cfgz.GetCFGZ_Param(CFGZ::ID_S2_SHUTDOWN_THRESH), 40);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ANLG_SENS_S2_VAL];
            break;

        case ANLG_SENS_S2_MON_WARNING:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_S2_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_S2_WARN_EN) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_S2_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_S2_WARN_EN) == CFGZ::CFGZ_ENABLE));
            prvUpdateMonParams(u8AlarmIndex, &_u8AuxSensS2, (bool)(_cfgz.GetCFGZ_Param(CFGZ::ID_S2_THRESH_TYPE)), Aux_S2_id, _cfgz.GetCFGZ_Param(CFGZ::ID_S2_WARNING_THRESH), 40);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ANLG_SENS_S2_VAL];
            break;

        case ANLG_SENS_S3_MON_SHUTDOWN:
            if(((_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)||(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2))
                    &&(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
            }
            prvUpdateMonParams(u8AlarmIndex, &_u8AuxSensS3, (bool)(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_THRESH_TYPE)), Aux_S3_id, _cfgz.GetCFGZ_Param(CFGZ::ID_S3_SHUTDOWN_THRESH), 40);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ANLG_SENS_S3_VAL];
            break;

        case ANLG_SENS_S3_MON_WARNING:
            if(((_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)||(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2))
                    &&(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_WARN_EN) == CFGZ::CFGZ_ENABLE))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
            }
            prvUpdateMonParams(u8AlarmIndex, &_u8AuxSensS3, (bool)(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_THRESH_TYPE)), Aux_S3_id, _cfgz.GetCFGZ_Param(CFGZ::ID_S3_WARNING_THRESH), 40);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ANLG_SENS_S3_VAL];
            break;

        case ANLG_SENS_S4_MON_SHUTDOWN:
            if(((_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)||(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2))
                    &&(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SHUT_DN_EN) == CFGZ::CFGZ_ENABLE))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
            }
            prvUpdateMonParams(u8AlarmIndex, &_u8AuxSensS4, (bool)(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_THRESH_TYPE)), Aux_S4_id, _cfgz.GetCFGZ_Param(CFGZ::ID_S4_SHUTDOWN_THRESH), 40);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ANLG_SENS_S4_VAL];
            break;

        case ANLG_SENS_S4_MON_WARNING:
            if(((_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)||(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2))
                    &&(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_WARN_EN) == CFGZ::CFGZ_ENABLE))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
            }
            prvUpdateMonParams(u8AlarmIndex, &_u8AuxSensS4, (bool)(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_THRESH_TYPE)), Aux_S4_id, _cfgz.GetCFGZ_Param(CFGZ::ID_S4_WARNING_THRESH), 40);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ANLG_SENS_S4_VAL];
            break;

        case OIL_PRESS_DETECTED:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                    || (_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
                    || ( _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG)))
            {
                benableMon = true;
            }
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = benableMon && (_cfgz.GetCFGZ_Param(CFGZ::ID_MONITOR_LOP_SENSE_BEFORE_CRANK) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex, &_u8LopHiOilPressMon, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_DISCONNECT_PRESURE_THRESH), 5);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LUBE_OIL_PRESSURE];
            break;

        case EARTH_LEAK_CURR:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_FAN_CM_SELECTION) == CFGZ::CFGZ_EARTH_LEAKAGE_CURR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
            }
            else
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = false;
            }
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, High_Earth_Leak_Curr_id, _cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CURR_THRESH),NO_OF_50MSEC_TICKS_FOR_1SEC* _cfgz.GetCFGZ_Param(CFGZ::ID_CURR_DELAY));
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CURR_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[EARTH_LEAKAGE_CURR_VAL];
            break;

        case SHELTER_TEMP_START_GEN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = (_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2);
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, High_Shelter_temp_id, (float)(_cfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_THRESH)),NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_MON_DELAY));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[SHELTER_TEMP_VAL];
            break;

        case SHELTER_TEMP_STOP_GEN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2);
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, (float)((_cfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_THRESH))-(_cfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_HYST))), NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_SHELT_TEMP_MON_DELAY));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[SHELTER_TEMP_VAL];
            break;

        case SHELTER_TEMP_OPEN_CKT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, Shelt_Temp_Open_id,(uint8_t)0,30);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[SHELT_TEMP_OPEN_CKT];
            break;

        case MAINS_R_UNDERVOLT_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_TRIP_THRESH),(uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_VOLT_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_R_PHASE_VOLTAGE];
            break;

        case MAINS_Y_UNDERVOLT_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_TRIP_THRESH), (uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_VOLT_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_Y_PHASE_VOLTAGE];
            break;

        case MAINS_B_UNDERVOLT_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_TRIP_THRESH), (uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_VOLT_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_B_PHASE_VOLTAGE];
            break;

        case MAINS_R_OVERVOLT_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_TRIP_THRESH), (uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_VOLT_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_R_PHASE_VOLTAGE];
            break;

        case MAINS_Y_OVERVOLT_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_TRIP_THRESH), (uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_VOLT_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_Y_PHASE_VOLTAGE];
            break;

        case MAINS_B_OVERVOLT_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_TRIP_THRESH), (uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_VOLT_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_B_PHASE_VOLTAGE];
            break;

        case MAINS_R_UNDERFREQ_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_TRIP_THRESH), (uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_FREQ_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_R_PHASE_FREQ];
            break;

        case MAINS_Y_UNDERFREQ_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_TRIP_THRESH), (uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_FREQ_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_Y_PHASE_FREQ];
            break;

        case MAINS_B_UNDERFREQ_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_TRIP_THRESH), (uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_FREQ_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_B_PHASE_FREQ];
            break;

        case MAINS_R_OVERFREQ_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_TRIP_THRESH), (uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_FREQ_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_R_PHASE_FREQ];
            break;

        case MAINS_Y_OVERFREQ_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_TRIP_THRESH),(uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_FREQ_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_Y_PHASE_FREQ];
            break;

        case MAINS_B_OVERFREQ_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_TRIP_THRESH), (uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_FREQ_TRIP_DELAY_SEC)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_B_PHASE_FREQ];
            break;

        case MAINS_R_UNDERVOLT_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_RET_THRESH), (NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_R_PHASE_VOLTAGE];
            break;

        case MAINS_Y_UNDERVOLT_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_RET_THRESH),(uint16_t) ((float)NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_Y_PHASE_VOLTAGE];
            break;

        case MAINS_B_UNDERVOLT_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_VTG_RET_THRESH), (NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_B_PHASE_VOLTAGE];
            break;

        case MAINS_R_OVERVOLT_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_RET_THRESH), (NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_R_PHASE_VOLTAGE];
            break;

        case MAINS_Y_OVERVOLT_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_RET_THRESH), (NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_Y_PHASE_VOLTAGE];
            break;

        case MAINS_B_OVERVOLT_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_VTG_RET_THRESH), (NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_B_PHASE_VOLTAGE];
            break;

        case MAINS_R_UNDERFREQ_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_RET_THRESH), (NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_R_PHASE_FREQ];
            break;

        case MAINS_Y_UNDERFREQ_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_RET_THRESH), (NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_Y_PHASE_FREQ];
            break;

        case MAINS_B_UNDERFREQ_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_UNDER_FREQ_RET_THRESH), (NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_B_PHASE_FREQ];
            break;

        case MAINS_R_OVERFREQ_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_RET_THRESH), (NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_R_PHASE_FREQ];
            break;

        case MAINS_Y_OVERFREQ_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) >= CFGZ::CFGZ_3_PHASE_SYSTEM));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_RET_THRESH), (NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_Y_PHASE_FREQ];
            break;

        case MAINS_B_OVERFREQ_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_MON_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE)&&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = false;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, false, _u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_OVER_FREQ_RET_THRESH), (NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_RETURN_TO_MAINS_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_R_PHASE_FREQ];
            break;

        case MAINS_PHASE_ROTATION:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PH_REVERS_DETECT_EN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PH_REVERS_DETECT_EN)) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PH_REVERS_ACTION)));
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Phase_EB_Rotation_id , (uint8_t)0, 4U);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_PHASE_ROTATION_STATUS];
            break;

        case OPEN_FUEL_CKT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_SELECTION)) && (_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_FAULT_ACTION));
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Fuel_Level_Ckt_Open_id , (uint8_t)0, 30U);
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_FAULT_ACTION));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FUEL_OPEN_CKT_VAL];
            break;

        case LOADING_VOLT_UNHEALTHY:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, false, GCU_ALARMS::Loading_Volt_Unhealthy_id , (uint16_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_MIN_HEALTHY_VTG)), 20U);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_MIN_VOLTAGE];
            break;

        case LOADING_FREQ_UNHEALTHY:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_ALTERNATOR_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, false, GCU_ALARMS::Loading_Freq_Unhealthy_id , (_cfgz.GetCFGZ_Param(CFGZ::ID_MIN_HEALTHY_FREQ)), 20U);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_MIN_FREQ];
            break;

        case ACTUATOR_FAIL_ABOVE_30:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = false;
            //ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_APPLICATION) == CFGZ::CFGZ_AS_EGOV)&&(CFGC::IsSGC421()));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Actuator_Fail_id;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*40U;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            // ArrAlarmMonitoring[u8AlarmIndex].Threshold.u16Value = EGOV::GetSetTragetRpm() + 30;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = TWO_BYTE_INT;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENGINE_SPEED];
            break;

        case  ACTUATOR_FAIL_BELOW_30:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = false;
            //ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_APPLICATION) == CFGZ::CFGZ_AS_EGOV)&&(CFGC::IsSGC421()));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Actuator_Fail_id;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*40U;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
            // ArrAlarmMonitoring[u8AlarmIndex].Threshold.u16Value = EGOV::GetSetTragetRpm() - 30;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = TWO_BYTE_INT;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENGINE_SPEED];
            break;

        case FAN_HIGH_CURR:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_FAN_CM_SELECTION) == CFGZ::CFGZ_FAN_CURRENT);
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CURR_ACTION));
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::Fan_High_Current_id , (_cfgz.GetCFGZ_Param(CFGZ::ID_HIGH_CURR_THRESH)), NO_OF_50MSEC_TICKS_FOR_1SEC*(_cfgz.GetCFGZ_Param(CFGZ::ID_CURR_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[EARTH_LEAKAGE_CURR_VAL];
            break;

        case FAN_LOW_CURR:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_FAN_CM_SELECTION) == CFGZ::CFGZ_FAN_CURRENT);
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_LOW_CURR_ACTION));
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, false, GCU_ALARMS::Fan_Low_Current_id , (_cfgz.GetCFGZ_Param(CFGZ::ID_LOW_CURR_THRESH)), NO_OF_50MSEC_TICKS_FOR_1SEC*(_cfgz.GetCFGZ_Param(CFGZ::ID_CURR_DELAY)));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[EARTH_LEAKAGE_CURR_VAL];
            break;

        case DIGITAL_INPUT_A:
            prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTION), GCU_ALARMS::Auxilary_Input_A_id);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_A];
                prvSetAlarmAction(DIGITAL_INPUT_A, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTION));
            }
            break;

        case DIGITAL_INPUT_B:
            prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTION), GCU_ALARMS::Auxilary_Input_B_id);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_B];
                prvSetAlarmAction(DIGITAL_INPUT_B, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTION));
            }
            break;

        case DIGITAL_INPUT_C:
            prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTION), GCU_ALARMS::Auxilary_Input_C_id);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_C];
                prvSetAlarmAction(DIGITAL_INPUT_C, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTION));
            }
            break;

        case DIGITAL_INPUT_D:
            prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTION), GCU_ALARMS::Auxilary_Input_D_id);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_D];
                prvSetAlarmAction(DIGITAL_INPUT_D, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTION));
            }
            break;

        case DIGITAL_INPUT_E:
            prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTION), GCU_ALARMS::Auxilary_Input_E_id);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_E];
                prvSetAlarmAction(DIGITAL_INPUT_E, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTION));
            }
            break;

        case DIG_IN_F:
            prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTION), GCU_ALARMS::Auxilary_Input_F_id);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_F];
                prvSetAlarmAction(DIG_IN_F, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTION));
            }
            break;

        case DIG_IN_G:
            prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTION), GCU_ALARMS::Auxilary_Input_G_id);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_G];
                prvSetAlarmAction(DIG_IN_G, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTION));
            }
            break;

        case DIG_IN_H:
            prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTION), GCU_ALARMS::Auxilary_Input_H_id);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_H];
                prvSetAlarmAction(DIG_IN_H, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTION));
            }
            break;

        case DIG_IN_I:
            prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTION), GCU_ALARMS::Auxilary_Input_I_id);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_I];
                prvSetAlarmAction(DIG_IN_I, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTION));
            }
            break;

        case DIG_IN_J:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTION), GCU_ALARMS::Auxilary_Input_J_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_J];
                    prvSetAlarmAction(DIG_IN_J, _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTION));
                }
            }
            break;

        case DIG_IN_K:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_SENS_SELECTION) == CFGZ::CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_ACTIVATION_DLY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_ACTION), GCU_ALARMS::Auxilary_Input_K_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_K];
                    prvSetAlarmAction(DIG_IN_K, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_ACTION));
                }
            }
            break;

        case DIG_IN_L:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_SELECTION) == CFGZ::CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_ACTIVATION_DLY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_ACTION), GCU_ALARMS::Auxilary_Input_L_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_L];
                    prvSetAlarmAction(DIG_IN_L, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_ACTION));
                }
            }
            break;

        case DIG_IN_M:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_S1_SENS_SELECTION) == CFGZ::CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_ACTIVATION_DLY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_ACTION), GCU_ALARMS::Auxilary_Input_M_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_M];
                    prvSetAlarmAction(DIG_IN_M, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_ACTION));
                }
            }
            break;

        case DIG_IN_N:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_S2_SENS_SELECTION) == CFGZ::CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_ACTIVATION_DLY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_ACTION), GCU_ALARMS::Auxilary_Input_N_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_N];
                    prvSetAlarmAction(DIG_IN_N, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_ACTION));
                }
            }
            break;

        case DIG_IN_O:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_ACTIVATION_DLY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_ACTION), GCU_ALARMS::Auxilary_Input_O_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_O];
                    prvSetAlarmAction(DIG_IN_O, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_ACTION));
                }
            }
            break;

        case DIG_IN_P:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_ACTIVATION_DLY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_ACTION), GCU_ALARMS::Auxilary_Input_P_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_P];
                    prvSetAlarmAction(DIG_IN_P, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_ACTION));
                }
            }
            break;

        case REMOTE_START_STOP:
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[REMOTE_SS_STATUS];
            break;

        case EMERGENCY_STOP:
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Emergency_Stop_id , (uint8_t)0, 2U);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[EMERGENCY_STOP_STATUS];
            break;

        case LOW_FUEL_LVL_SWITCH:
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = GCU_ALARMS::fuel_level_id;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LOW_FUEL_LVL_SWITCH_STATUS];
            break;

        case LLOP_SWITCH:
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = GCU_ALARMS::Low_Oil_Pressure_id;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LLOP_SWITCH_STATUS];
            break;

        case HWT_SWITCH:
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = GCU_ALARMS::High_Water_Temperature_id;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[HWT_SWITCH_STATUS];
            break;

        case RWL_SWITCH:
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = GCU_ALARMS::Radiator_Water_Level_id;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[WATER_LEVEL_SWITCH_STATUS];
            break;

        case V_BELT_BROKEN_SWITCH:
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = GCU_ALARMS::V_Belt_Broken_id;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[V_BELT_BROKEN_SWITCH_STATUS];
            break;

        case LLOP_SWITCH_AT_ENG_OFF:
            if((DigitalSensor::SENSOR_NOT_CONFIGRUED != _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_LUBE_OIL_PRESSURE_SWITCH))
                    &&(_cfgz.GetCFGZ_Param(CFGZ::ID_MONITOR_LLOP_BEFORE_CRANK)))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
            }
            else
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = false;
            }
            prvUpdateMonParams(u8AlarmIndex,&_u8EngineOff,false,_u8Dummy255,(uint8_t)1,1);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LLOP_SWITCH_STATUS];
            break;

        case MAINS_CONTACTOR_LATCHED :
            prvUpdateMonParams(u8AlarmIndex,&_u8DummyOne,true,_u8Dummy255,(uint8_t)0,1U);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_CONT_LATCHED_STATUS];
            break;

        case GEN_CONTACTOR_LATCHED :
            prvUpdateMonParams(u8AlarmIndex,&_u8DummyOne,true,_u8Dummy255,(uint8_t)0,1U);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_CONT_LATCHED_STATUS];
            break;

        case BATT_CHG_FAIL:
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Batt_Chg_Fail_id;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[BATT_CHG_FAIL_STATUS ];
            break;

        case SMOKE_FIRE:
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Smoke_Fire_id;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[SMOKE_FIRE_STATUS ];
            break;

        case PANEL_LOCK:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Panel_lock_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[PANEL_LOCK_STATUS];
            break;

        case EX_AUTO_PANEL_LOCK:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Ex_Auto_Panel_lock_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[EX_AUTO_PANEL_LOCK_STATUS];
            break;

        case GEN_LOAD_INHIBIT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Gen_load_inhibit_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_LOAD_INHIBIT_STATUS ];
            break;

        case MAINS_LOAD_INHIBIT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Mains_Load_Inhibit_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_LOAD_INHIBIT_STATUS ];
            break;

        case FAIL_TO_LATCH_GEN_CONT :
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonGenContactor;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Fail_To_Close_Gen_Cont_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 1;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FAIL_TO_LATCH_GEN_CONT_STATUS];
            break;

        case FAIL_TO_LATCH_MAINS_CONT :
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonMainsContactor;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Fail_To_Close_Mains_Cont_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 1;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FAIL_TO_LATCH_MAINS_CONT_STATUS];
            break;

        case FAIL_TO_OPEN_GEN_CONT:
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonGenContactorOpen;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Fail_To_Open_Gen_Cont_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FAIL_TO_LATCH_GEN_CONT_STATUS];
            break;

        case FAIL_TO_OPEN_MAIN_CONT:
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonMainsContactorOpen;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Fail_To_Open_Mains_Cont_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FAIL_TO_LATCH_MAINS_CONT_STATUS];
            break;

        case INVALID_DG_START:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_WAVE_DETECT_EN) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_WAVE_DETECT_EN) == CFGZ::CFGZ_ENABLE);
            prvUpdateMonParams(u8AlarmIndex,&_u8MonOn,true,Invalid_gen_start_id,(uint8_t)0,1);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[INVALID_GEN_START_STATUS];
            break;

        case J1939_COM_FAIL:
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_COMM_FAILURE_ACTION));
                prvSetAlarmActivation(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_COMM_FAILURE_ACTIVATION));
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[J1939_COM_FAIL_STATUS];
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = J1939_com_fail_id;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_ECU_COMM_FAILURE_ACT_DELAY);
            }
            break;

        case  HIGH_OIL_PRESSHURE:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
            prvUpdateMonParams(u8AlarmIndex, &_u8EngineOff, true, Oil_Press_Detected_id ,(uint8_t)0U, (uint8_t)1U);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[HIGH_OIL_PRESSURE];
            break;

        case FAIL_TO_STOP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Fail_To_Stop_id , (uint8_t)0, 1U);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FAIL_TO_STOP_STATUS];
            break;

        case FAIL_TO_START:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, GCU_ALARMS::Fail_To_Start_id , (uint8_t)0, 1U);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FAIL_TO_START_STATUS];
            break;

        case LOAD_UNBALANCE:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = _cfgz.GetCFGZ_Param(CFGZ::ID_UNBAL_LOAD_ACTION);
            prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_UNBAL_LOAD_ACTION));
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::Load_Unbalance_id , _cfgz.GetCFGZ_Param(CFGZ::ID_UNBAL_LOAD_THRESH), NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_UNBAL_LOAD_DELAY));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_UNBALANCED_LOAD];
            break;

        case J1939_PROTECT_LAMP:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_PROTECT_ACTION));
                prvSetAlarmActivation(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_PROTECT_ACTIVATION));
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = J1939_Protect_Lamp_id;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_ECU_PROTECT_ACT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[J1939_PROTECT_LAMP_STATUS];
            }
            break;

        case J1939_AMBER_LAMP:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_AMBER_ACTION));
                prvSetAlarmActivation(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_AMBER_ACTIVATION));
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = J1939_Amber_Lamp_id;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_ECU_AMBER_ACT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[J1939_AMBER_LAMP_STATUS];
            }
            break;

        case J1939_RED_LAMP:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_RED_ACTION));
                prvSetAlarmActivation(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_RED_ACTIVATION));
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = J1939_Red_Lamp_id;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_ECU_RED_ACT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[J1939_RED_LAMP_STATUS];
            }
            break;

        case J1939_MIL_LAMP:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_MALFUNCTION_ACTION));
                prvSetAlarmActivation(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ECU_MALFUNCTION_ACTIVATION));
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = J1939_Mil_Lamp_id;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_ECU_MALFUNCTION_ACT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[J1939_MIL_LAMP_STATUS];
            }
            break;

        case J1939_ECU_PREHEAT_FAIL:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE)== CFGZ::ENG_IVECO)
            {
                /* Hard coded the monitoring and Warning if above conditions are TRUE */
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
                prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, J1939_Preheat_fail_id ,(uint8_t)0U, 1U);
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[J1939_PREHEAT_FAIL_STATUS];
            }
            break;

        case REGEN_SW_INHIBIT_SIGNAL:
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = _u8Dummy255;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[REGENERATION_SW_STATUS];
            break;

        case ASH_LOAD_MAINTAINANCE:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE)== CFGZ::ENG_KUBOTA)
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = _cfgz.GetCFGZ_Param(CFGZ::ID_MAINT_ASH_LOAD_EN);
                prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINTENANCE_ALARM_ACTION));
                prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, Ash_load_maintenance_id ,(uint8_t)99U, 1U);
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[J1939_ASH_LOAD_STATUS];
            }
            break;

        case AFT_ACTIVATION_TIMEOUT:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
            prvUpdateMonParams(u8AlarmIndex, &_u8DummyOne, true, Aft_Activation_id ,(uint8_t)0U, 1U);
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[AFT_ACTIVATION_TIMEOUT_STATUS];
            break;

        case LOW_LOAD_TRIP:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = _cfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_ALARM_EN);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_ACTION))
            {
                prvSetAlarmAction(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_ACTION));
            }
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, false, GCU_ALARMS::Low_load_id, _cfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_TRIP), NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_DELAY_SEC));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LOW_LOAD_STATUS];
            break;

        case LOW_LOAD_RETURN:
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = _cfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_ALARM_EN);
            prvUpdateMonParams(u8AlarmIndex, &_u8MonOn, true, GCU_ALARMS::_u8Dummy255, _cfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_RETURN), NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_DELAY_SEC));
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LOW_LOAD_STATUS];
            break;
    }
}

float GCU_ALARMS::GetSpeedValue()
{
    if((_cfgz.GetEngType()!=CFGZ::ENG_CONVENTIONAL)
            && (_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_SPEED_FROM_ENG) == CFGZ::CFGZ_ENABLE))
    {
        if(!gpJ1939->IsCommunicationFail())
        {
            return (float)gpJ1939->GetReadData(RX_PGN_EEC1_61444, 3);
        }
        else return 0;
    }
    else if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE) == CFGZ::CFGZ_MAGNETIC_PICKUP)
              ||(_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE) == CFGZ::CFGZ_W_POINT_FREQ))
    {
       return _hal.AnalogSensors.GetFilteredPulseInpuRPM();
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE) == CFGZ::CFGZ_ALT_FREQUENCY)
    {
        return _hal.AnalogSensors.GetFiltRPMThruCompartor();
    }
    else
    {
        return 0;
    }
}

void GCU_ALARMS::prvUpdateGCUAlarmsValue()
{
    A_SENSE::SENSOR_RET_t stLOP  = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED};
    A_SENSE::SENSOR_RET_t stFuel = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);
    A_SENSE::SENSOR_RET_t stEngTemp = GetSelectedTempSensVal();
    A_SENSE::SENSOR_RET_t stAuxSensS1 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S1_SENSOR);
    A_SENSE::SENSOR_RET_t stAuxSensS2 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S2_SENSOR);
    A_SENSE::SENSOR_RET_t stAuxSensS3 = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED};
    A_SENSE::SENSOR_RET_t stAuxSensS4 = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED};

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        stAuxSensS3 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S3_4_20_SENSOR);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        stAuxSensS3 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S3_0_5_SENSOR);
    }

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        stAuxSensS4 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S4_4_20_SENSOR);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        stAuxSensS4 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S4_0_5_SENSOR);
    }

    A_SENSE::SENSOR_RET_t stShelterTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_SHELTER_TEMPERATURE);

    stLOP = GetLOPSensorVal();
    _ArrAlarmValue[LUBE_OIL_PRESSURE].f32Value = stLOP.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[FUEL_LEVEL].u8Value = (uint8_t)stFuel.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ENGINE_TEMPERATURE].f32Value = stEngTemp.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ENGINE_SPEED].u16Value =(uint16_t) GetSpeedValue();

    _ArrAlarmValue[GENSET_FREQUENCY].f32Value = _hal.AcSensors.GENSET_GetMinFrq();
    _ArrAlarmValue[GEN_R_PHASE_VOLTAGE].u16Value = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVolts(R_PHASE);
    _ArrAlarmValue[GEN_Y_PHASE_VOLTAGE].u16Value = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVolts(Y_PHASE);
    _ArrAlarmValue[GEN_B_PHASE_VOLTAGE].u16Value = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVolts(B_PHASE);
    _ArrAlarmValue[MAINS_R_PHASE_VOLTAGE].u16Value = (uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(R_PHASE);
    _ArrAlarmValue[MAINS_Y_PHASE_VOLTAGE].u16Value = (uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(Y_PHASE);
    _ArrAlarmValue[MAINS_B_PHASE_VOLTAGE].u16Value = (uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(B_PHASE);
    _ArrAlarmValue[MAINS_R_PHASE_FREQ].f32Value = (uint16_t)_hal.AcSensors.MAINS_GetApproxFreq(R_PHASE);
    _ArrAlarmValue[MAINS_Y_PHASE_FREQ].f32Value = (uint16_t)_hal.AcSensors.MAINS_GetApproxFreq(Y_PHASE);
    _ArrAlarmValue[MAINS_B_PHASE_FREQ].f32Value = (uint16_t)_hal.AcSensors.MAINS_GetApproxFreq(B_PHASE);
    _ArrAlarmValue[MAINS_PH_PH_MIN_VOLTAGE].u16Value = prvGetMinPhToPhMainsVoltage();

    _u8LowIdleSpeedMon = _u8MonOn && !(START_STOP::IsIdleToRatedRampOver());
    _ArrAlarmValue[CHARG_ALT_VOLTAGE].f32Value = _hal.AnalogSensors.GetFilteredChargingAltVolts();

    _ArrAlarmValue[CHARG_ALT_ERROR].u8Value = (uint8_t)CHARGING_ALT::IsChargAltFail();
    _ArrAlarmValue[BATTERY_VOLTAGE].f32Value = GetSelectedBatteryVtg();
    _ArrAlarmValue[BTS_VOLTAGE].f32Value =  _hal.AnalogSensors.GetFilteredVBTSbattVolts();
    _ArrAlarmValue[GEN_MAX_CURRENT].u16Value = (uint16_t)prvGetMaxGensetCurent();
    _ArrAlarmValue[ENG_RUN_HOURS].u16Value = (uint16_t)(GetSelectedEngRunMin()/60);
    _ArrAlarmValue[TOTAL_KW_PERCENT].u16Value = (uint16_t)((_hal.AcSensors.GENSET_GetActivePowerWatts(R_PHASE) +
                                                _hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE) +
                                                _hal.AcSensors.GENSET_GetActivePowerWatts(B_PHASE))/(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_RATING)*10));
    _ArrAlarmValue[FUEL_THEFT_ALARM].u8Value = _u8FuelTheftAlarm;
    _ArrAlarmValue[GEN_PHASE_ROTATION_STATUS].u8Value = (uint8_t)_hal.AcSensors.GENSET_GetPhaseRotStatus();
    _ArrAlarmValue[MAINS_PHASE_ROTATION_STATUS].u8Value = (uint8_t)_hal.AcSensors.MAINS_GetPhaseRotStatus();
    _ArrAlarmValue[ENG_TEMP_OPEN_CKT].u8Value = (uint8_t)(stEngTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT);
    _ArrAlarmValue[FUEL_OPEN_CKT_VAL].u8Value = (uint8_t)(stFuel.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT);

    _ArrAlarmValue[GEN_MIN_VOLTAGE].u16Value = (uint16_t)GetMinGensetVoltage();
    _ArrAlarmValue[GEN_MIN_FREQ].f32Value = GetMinGenFreq();

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
    {
        if(stLOP.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT)
        {
              _ArrAlarmValue[LOP_CURRENT_OPEN_CKT].u8Value =  (uint8_t)1;
              _ArrAlarmValue[LOP_CURR_STB].u8Value = (uint8_t)0;
        }
        else if(stLOP.stValAndStatus.eState == ANLG_IP::BSP_STATE_SHORT_TO_BAT)
        {
            _ArrAlarmValue[LOP_CURR_STB].u8Value =  (uint8_t)1;
            _ArrAlarmValue[LOP_CURRENT_OPEN_CKT].u8Value = (uint8_t)0;
        }
        else
        {
            _ArrAlarmValue[LOP_CURR_STB].u8Value =  (uint8_t)0;
            _ArrAlarmValue[LOP_CURRENT_OPEN_CKT].u8Value = (uint8_t)0;
        }
    }
    else
    {
        _ArrAlarmValue[LOP_RES_OPEN_CKT].u8Value = (uint8_t)(stLOP.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT);
    }
    _ArrAlarmValue[DIG_INPUT_A].u8Value = (_hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_A_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[DIG_INPUT_B].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_B_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_C].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_C_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_D].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_D_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_E].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_E_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_F].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_F_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_G].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_G_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_H].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_H_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_I].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_I_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_J].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_J_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_K].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_K_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_L].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_L_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_M].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_M_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_N].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_N_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_O].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_O_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_P].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_P_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;

    _ArrAlarmValue[AUX_SENS_S1_OPEN_CKT].u8Value = (uint8_t)(stAuxSensS1.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT);
    _ArrAlarmValue[AUX_SENS_S2_OPEN_CKT].u8Value = (uint8_t)(stAuxSensS2.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT);

    if((stAuxSensS3.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT))
    {
        _ArrAlarmValue[AUX_SENS_S3_OPEN_CKT].u8Value = 1;
        _ArrAlarmValue[AUX_SENS_S3_STB].u8Value = 0;
    }
    else if(stAuxSensS3.stValAndStatus.eState == ANLG_IP::BSP_STATE_SHORT_TO_BAT)
    {
        _ArrAlarmValue[AUX_SENS_S3_STB].u8Value = 1;
        _ArrAlarmValue[AUX_SENS_S3_OPEN_CKT].u8Value = 0;
    }
    else
    {
        _ArrAlarmValue[AUX_SENS_S3_STB].u8Value = 0;
        _ArrAlarmValue[AUX_SENS_S3_OPEN_CKT].u8Value = 0;
    }

    if((stAuxSensS4.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT))
    {
        _ArrAlarmValue[AUX_SENS_S4_OPEN_CKT].u8Value = 1;
        _ArrAlarmValue[AUX_SENS_S4_STB].u8Value = 0;
    }
    else if(stAuxSensS4.stValAndStatus.eState == ANLG_IP::BSP_STATE_SHORT_TO_BAT)
    {
        _ArrAlarmValue[AUX_SENS_S4_STB].u8Value = 1;
        _ArrAlarmValue[AUX_SENS_S4_OPEN_CKT].u8Value = 0;
    }
    else
    {
        _ArrAlarmValue[AUX_SENS_S4_STB].u8Value = 0;
        _ArrAlarmValue[AUX_SENS_S4_OPEN_CKT].u8Value = 0;
    }
    _ArrAlarmValue[SHELTER_TEMP_VAL].f32Value = stShelterTemp.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ANLG_SENS_S1_VAL].f32Value = stAuxSensS1.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ANLG_SENS_S2_VAL].f32Value = stAuxSensS2.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ANLG_SENS_S3_VAL].f32Value = stAuxSensS3.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ANLG_SENS_S4_VAL].f32Value = stAuxSensS4.stValAndStatus.f32InstSensorVal;

    _ArrAlarmValue[SHELT_TEMP_OPEN_CKT].u8Value = (uint8_t)(stShelterTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT);

    _ArrAlarmValue[EARTH_LEAKAGE_CURR_VAL].f32Value = _hal.AcSensors.EARTH_GetCurrentFilt() ;

    _ArrAlarmValue[REMOTE_SS_STATUS].u8Value = (_hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_REMOTE_START_STOP) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[FAIL_TO_STOP_STATUS].u8Value = _bFailToStop;
    _ArrAlarmValue[FAIL_TO_START_STATUS].u8Value = _bFailToStart;
    _ArrAlarmValue[EMERGENCY_STOP_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_EMERGENCY_STOP) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[GEN_UNBALANCED_LOAD].u8Value = prvGetUnbalancedLoadVal();
    _ArrAlarmValue[LOW_FUEL_LVL_SWITCH_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_LOW_FUEL_LEVEL_SWITCH) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[LLOP_SWITCH_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_LOW_LUBE_OIL_PRESSURE_SWITCH) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[HWT_SWITCH_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_HIGH_ENGINE_TEMP_SWITCH) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[WATER_LEVEL_SWITCH_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_LOW_WATER_LEVEL_SWITCH) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[V_BELT_BROKEN_SWITCH_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(
            DigitalSensor::DI_V_BELT_BROKEN_SWITCH) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[HIGH_OIL_PRESSURE].u8Value = _u8HighOilPressDetectedAlarm;

    _ArrAlarmValue[MAINS_CONT_LATCHED_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_MAINS_CONTACTOR_LATCHED) == DigitalSensor::SENSOR_LATCHED);

    _ArrAlarmValue[GEN_CONT_LATCHED_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_GEN_CONTACTOR_LATCHED) == DigitalSensor::SENSOR_LATCHED);

    _ArrAlarmValue[FAIL_TO_LATCH_GEN_CONT_STATUS].u8Value = ArrAlarmMonitoring[GEN_CONTACTOR_LATCHED].bResultInstant;
    _ArrAlarmValue[FAIL_TO_LATCH_MAINS_CONT_STATUS].u8Value = ArrAlarmMonitoring[MAINS_CONTACTOR_LATCHED].bResultInstant;

    _ArrAlarmValue[BATT_CHG_FAIL_STATUS].u8Value= (uint8_t)((_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_BATTERY_CHARGER_FAIL) == DigitalSensor::SENSOR_LATCHED) && (BASE_MODES::GetMainsStatus() == BASE_MODES::MAINS_HELATHY)
            && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN) == CFGZ::CFGZ_ENABLE));

    _ArrAlarmValue[SMOKE_FIRE_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_SMOKE_FIRE) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[PANEL_LOCK_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_STOP_PANEL_LOCK) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[EX_AUTO_PANEL_LOCK_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_EXT_PANEL_LOCK) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[GEN_LOAD_INHIBIT_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_GENERATOR_LOAD_INITHIBIT) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[MAINS_LOAD_INHIBIT_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_MAINS_LOAD_INHIBIT) == DigitalSensor::SENSOR_LATCHED);

    _ArrAlarmValue[INVALID_GEN_START_STATUS].u8Value = (uint8_t)(ENGINE_START_VALIDITY::GetEngineStartInvalidity());

    _ArrAlarmValue[AFT_ACTIVATION_TIMEOUT_STATUS].u8Value = _u8AFTActivationTimeout;
    _ArrAlarmValue[SIM_MAINS_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_SIMULATE_MAINS) == DigitalSensor::SENSOR_LATCHED);

   _ArrAlarmValue[SIM_AUTO_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_SIMULATE_AUTO) == DigitalSensor::SENSOR_LATCHED);

   _ArrAlarmValue[SIM_START_STATUS].u8Value=(uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_SIMULATE_START) == DigitalSensor::SENSOR_LATCHED);

   _ArrAlarmValue[SIM_STOP_STATUS].u8Value=(uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_SIMULATE_STOP) == DigitalSensor::SENSOR_LATCHED);

   _ArrAlarmValue[CLOSE_GEN_OPEN_MAINS_STATUS].u8Value=(uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_CLOSE_GEN_OPEN_MAINS_SWITCH) == DigitalSensor::SENSOR_LATCHED);

   _ArrAlarmValue[CLOSE_MAINS_OPEN_GEN_STATUS].u8Value=(uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
           DigitalSensor::DI_CLOSE_MAINS_OPEN_GEN_SWITCH) == DigitalSensor::SENSOR_LATCHED);

   _ArrAlarmValue[REGENERATION_SW_STATUS].u8Value=(uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(
              DigitalSensor::DI_SOURCE_REGENERATION_SW_INHIBIT_SIGNAL) == DigitalSensor::SENSOR_LATCHED);

    _ArrAlarmValue[MPU_LOSS_STATUS].u8Value = _u8MPULossAlarm;
    RTC::TIME_t CurrentTime;
    _hal.ObjRTC.GetTime(&CurrentTime);
    if(CurrentTime.u16Year > _cfgz.GetCFGZ_Param(CFGZ::ID_FILT_MAINT_THRESH_YEAR))
    {
        _ArrAlarmValue[MAINT_DATE].u8Value = 1;
    }
    else if(CurrentTime.u16Year == _cfgz.GetCFGZ_Param(CFGZ::ID_FILT_MAINT_THRESH_YEAR))
    {
        if(CurrentTime.u8Month == _cfgz.GetCFGZ_Param(CFGZ::ID_FILT_MAINT_THRESH_MONTH))
        {
            if(CurrentTime.u8Day > _cfgz.GetCFGZ_Param(CFGZ::ID_FILT_MAINT_THRESH_DAY))
            {
                _ArrAlarmValue[MAINT_DATE].u8Value = 1;
            }
            else
            {
                _ArrAlarmValue[MAINT_DATE].u8Value = 0;
            }
        }
        else if(CurrentTime.u8Month >_cfgz.GetCFGZ_Param(CFGZ::ID_FILT_MAINT_THRESH_MONTH))
        {
            _ArrAlarmValue[MAINT_DATE].u8Value = 1;
        }
        else
        {
            _ArrAlarmValue[MAINT_DATE].u8Value = 0;
        }
    }
    else
    {
        _ArrAlarmValue[MAINT_DATE].u8Value = 0;
    }

    if(ArrAlarmMonitoring[ASH_LOAD_MAINTAINANCE].bAlarmActive)
    {
        _ArrAlarmValue[MAINT_DATE].u8Value = 1;
    }

    _ArrAlarmValue[PIN23_SENSOR_CURRENT_VAL].f32Value = _hal.AnalogSensors.GetPin23CurrentValMilliAmp();
    _ArrAlarmValue[LOW_LOAD_STATUS].u8Value = (uint8_t)_hal.AcSensors.GENSET_GetPercentPower();


    _ArrAlarmValue[J1939_COM_FAIL_STATUS].u8Value =  (uint8_t)((gpJ1939->IsCommunicationFail())&&(START_STOP::IsKeySwitchOutputActive()));
    _ArrAlarmValue[J1939_PROTECT_LAMP_STATUS].u8Value = gpJ1939->IsProtectLampON();
    _ArrAlarmValue[J1939_AMBER_LAMP_STATUS].u8Value = gpJ1939->IsAmberLampON();
    _ArrAlarmValue[J1939_RED_LAMP_STATUS].u8Value = gpJ1939->IsRedLampON();
    _ArrAlarmValue[J1939_MIL_LAMP_STATUS].u8Value = gpJ1939->IsMilLampON();
    _ArrAlarmValue[J1939_ASH_LOAD_STATUS].u8Value =(uint8_t) gpJ1939->GetReadData(RX_PGN_AT1S_64891, 1);
    _ArrAlarmValue[J1939_PREHEAT_FAIL_STATUS].u8Value =(uint8_t)START_STOP::IsJ1939PreheatFaultPresent();

}

void GCU_ALARMS::AssignAlarmsForDisplay(uint8_t u8LoggingID)
{
    switch(u8LoggingID)
    {
        case NoAlarm_id:
            _ArrAlarmStatus[u8LoggingID] = &_u8DummyZero;
        break;
        case Low_Oil_Pressure_id:
            _ArrAlarmStatus[u8LoggingID] = &_u8LowOilPressAlarm;
        break;
        case fuel_level_id:
            _ArrAlarmStatus[u8LoggingID] = &_u8LowFuelLevelAlarm;
        break;
        case High_Water_Temperature_id :
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_THRESH_TYPE) == CFGZ::CFGZ_LESS_THAN_THRESHOLD)
            {
                _ArrAlarmStatus[u8LoggingID] = &_u8HighEngTempSwitch;
            }
            else
            {
                _ArrAlarmStatus[u8LoggingID] = &_u8HighEngTempAlarm;
            }
        break;
        case Low_Water_Temperature_id:
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_THRESH_TYPE) == CFGZ::CFGZ_LESS_THAN_THRESHOLD)
            {
                _ArrAlarmStatus[u8LoggingID] = &_u8HighEngTempAlarm;
            }
            else
            {
                _ArrAlarmStatus[u8LoggingID] = &_u8DummyZero;
            }
            break;
        case Radiator_Water_Level_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[RWL_SWITCH].bAlarmActive;
        break;
        case Over_Speed_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OVERSPEED].bAlarmActive;
        break;
        case Gross_Over_Speed_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[GROSS_OVERSPEED].bAlarmActive;
        break;
        case Under_Speed_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[UNDERSPEED].bAlarmActive;
        break;
        case R_Over_Voltage_id :
            _ArrAlarmStatus[u8LoggingID] = &_u8RPhaseOverVoltAlarm;
        break;
        case R_Under_Voltage_id :
            _ArrAlarmStatus[u8LoggingID] = &_u8RPhaseUnderVoltAlarm;
        break;
        case Y_Over_Voltage_id :
            _ArrAlarmStatus[u8LoggingID] = &_u8YPhaseOverVoltAlarm;
        break;
        case Y_Under_Voltage_id :
            _ArrAlarmStatus[u8LoggingID] = &_u8YPhaseUnderVoltAlarm;
        break;
        case B_Over_Voltage_id :
            _ArrAlarmStatus[u8LoggingID] = &_u8BPhaseOverVoltAlarm;
        break;
        case B_Under_Voltage_id :
            _ArrAlarmStatus[u8LoggingID] = &_u8BPhaseUnderVoltAlarm;
        break;
        case Over_Frequency_id :
            _ArrAlarmStatus[u8LoggingID] = &_u8OverFreqAlarm;
        break;
        case Under_Frequency_id :
            _ArrAlarmStatus[u8LoggingID] = &_u8UnderFreqAlarm;
        break;
        case Emergency_Stop_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[EMERGENCY_STOP].bAlarmActive;
        break;
        case Charge_Fail_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[CHARG_ALT_FAIL].bAlarmActive;
        break;
        case Battery_Over_Voltage_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[VBAT_OV].bAlarmActive;
        break;
        case Battery_Under_Voltage_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[VBAT_UV].bAlarmActive;
        break;
        case Over_Current_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OVERCURRENT].bAlarmActive;
        break;
        case Filter_maintenance_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&_u8MaintAlarm;
        break;
        case Over_Load_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OVERLOAD].bAlarmActive;
        break;
        case Auxilary_Input_A_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIGITAL_INPUT_A].bAlarmActive;
            }
        break;
        case Auxilary_Input_B_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
             {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIGITAL_INPUT_B].bAlarmActive;
             }
        break;
        case Auxilary_Input_C_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIGITAL_INPUT_C].bAlarmActive;
            }
        break;
        case Auxilary_Input_D_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIGITAL_INPUT_D].bAlarmActive;
            }
        break;
        case Auxilary_Input_E_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIGITAL_INPUT_E].bAlarmActive;
            }
        break;
        case Auxilary_Input_F_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_F].bAlarmActive;
            }
        break;
        case Auxilary_Input_G_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_G].bAlarmActive;
            }
        break;
        case Auxilary_Input_H_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_H].bAlarmActive;
            }
        break;
        case Auxilary_Input_I_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_I].bAlarmActive;
            }
        break;
        case Auxilary_Input_J_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_J].bAlarmActive;
            }
        break;
        case Auxilary_Input_K_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTK_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_K].bAlarmActive;
            }
        break;
        case Auxilary_Input_L_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTL_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_L].bAlarmActive;
            }
        break;
        case Auxilary_Input_M_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTM_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_M].bAlarmActive;
            }
        break;
        case Auxilary_Input_N_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTN_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_N].bAlarmActive;
            }
        break;
        case Auxilary_Input_O_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTO_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_O].bAlarmActive;
            }
        break;
        case Auxilary_Input_P_id :
            if( _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_INPUTP_SOURCE) == CFGZ:: CFGZ_USER_CONFIGURED_SENSOR)
            {
                _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_P].bAlarmActive;
            }
        break;

        case Fail_To_Stop_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[FAIL_TO_STOP].bAlarmActive;
            break;
        case Fuel_Theft_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[FUEL_THEFT].bAlarmActive;
            break;
        case Load_Unbalance_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[LOAD_UNBALANCE].bAlarmActive;
            break;

        case MPU_Loss_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[MPU_LOSS].bAlarmActive;
            break;

        case Fail_To_Start_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[FAIL_TO_START].bAlarmActive;
            break;

        case Lop_Curr_Sens_Open_Ckt_id:
                  _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[LOP_CURR_OPEN_CKT].bAlarmActive;
              break;
        case Engine_Temperature_Ckt_Open_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OPEN_ENG_TEMP_CKT].bAlarmActive;
            break;
        case Gen_Phase_Rotation_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[GEN_PHASE_ROTATION].bAlarmActive;
            break;
        case Phase_EB_Rotation_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[MAINS_PHASE_ROTATION].bAlarmActive;
            break;
        case Oil_Pressure_Ckt_Open_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[LOP_RES_SENS_OPEN_CKT].bAlarmActive;
            break;

        case V_Belt_Broken_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[V_BELT_BROKEN_SWITCH].bAlarmActive;
            break;

        case Aux_S1_Sens_Open_Ckt_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OPEN_ANLG_SENS_S1_CKT].bAlarmActive;
            break;

        case Aux_S2_Sens_Open_Ckt_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OPEN_ANLG_SENS_S2_CKT].bAlarmActive;
            break;

        case Aux_S3_Sens_Open_Ckt_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OPEN_ANLG_SENS_S3_CKT].bAlarmActive;
            break;

        case Aux_S4_Sens_Open_Ckt_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OPEN_ANLG_SENS_S4_CKT].bAlarmActive;
            break;

        case Aux_S1_id:
            _ArrAlarmStatus[u8LoggingID] =  &_u8AuxSensS1Alarm;
            break;

        case Aux_S2_id:
            _ArrAlarmStatus[u8LoggingID] =  &_u8AuxSensS2Alarm;
            break;

        case Aux_S3_id:
            _ArrAlarmStatus[u8LoggingID] =  &_u8AuxSensS3Alarm;
            break;

        case Aux_S4_id:
            _ArrAlarmStatus[u8LoggingID] =  &_u8AuxSensS4Alarm;
            break;

        case Oil_Press_Detected_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[HIGH_OIL_PRESSHURE].bAlarmActive;
            break;
        case High_Earth_Leak_Curr_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[EARTH_LEAK_CURR].bAlarmActive;
            break;
        case Batt_Chg_Fail_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[BATT_CHG_FAIL].bAlarmActive;
            break;
        case Smoke_Fire_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[SMOKE_FIRE].bAlarmActive;
            break;

        case High_Shelter_temp_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[SHELTER_TEMP_START_GEN].bAlarmActive;
            break;

        case Shelt_Temp_Open_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[SHELTER_TEMP_OPEN_CKT].bAlarmActive;
            break;


        case Fuel_Level_Ckt_Open_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OPEN_FUEL_CKT].bAlarmActive;
            break;

        case Panel_lock_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[PANEL_LOCK].bAlarmActive;
            break;
        case Ex_Auto_Panel_lock_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[EX_AUTO_PANEL_LOCK].bAlarmActive;
            break;
        case Gen_load_inhibit_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[GEN_LOAD_INHIBIT].bAlarmActive;
            break;
        case Mains_Load_Inhibit_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[MAINS_LOAD_INHIBIT].bAlarmActive;
            break;

        case Fail_To_Close_Gen_Cont_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[FAIL_TO_LATCH_GEN_CONT].bAlarmActive;
            break;

        case Fail_To_Close_Mains_Cont_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[FAIL_TO_LATCH_MAINS_CONT].bAlarmActive;
            break;

        case Fail_To_Open_Gen_Cont_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t*)&ArrAlarmMonitoring[FAIL_TO_OPEN_GEN_CONT].bAlarmActive;
            break;
        case Fail_To_Open_Mains_Cont_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t*)&ArrAlarmMonitoring[FAIL_TO_OPEN_MAIN_CONT].bAlarmActive;
            break;
        case Loading_Volt_Unhealthy_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[LOADING_VOLT_UNHEALTHY].bAlarmActive;
            break;


        case Loading_Freq_Unhealthy_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[LOADING_FREQ_UNHEALTHY].bAlarmActive;
            break;

        case Actuator_Fail_id:
            _ArrAlarmStatus[u8LoggingID] = &_u8ActuatorFailAlarm;
            break;

        case Fan_High_Current_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[FAN_HIGH_CURR].bAlarmActive;
            break;

        case Fan_Low_Current_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[FAN_LOW_CURR].bAlarmActive;
            break;

        case Invalid_gen_start_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[INVALID_DG_START].bAlarmActive;
            break;

        case J1939_com_fail_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[J1939_COM_FAIL].bAlarmActive;
            break;
        case J1939_Protect_Lamp_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[J1939_PROTECT_LAMP].bAlarmActive;
            break;
        case J1939_Amber_Lamp_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[J1939_AMBER_LAMP].bAlarmActive;
            break;
        case J1939_Red_Lamp_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[J1939_RED_LAMP].bAlarmActive;
            break;
        case J1939_Mil_Lamp_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[J1939_MIL_LAMP].bAlarmActive;
            break;
        case J1939_Preheat_fail_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[J1939_ECU_PREHEAT_FAIL].bAlarmActive;
            break;
        case Ash_load_maintenance_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[ASH_LOAD_MAINTAINANCE].bAlarmActive;
            break;
        case Aft_Activation_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[AFT_ACTIVATION_TIMEOUT].bAlarmActive;
            break;
        case LOP_CURR_SENS_STB_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[LOP_CURR_SENS_STB].bAlarmActive;
            break;
        case Aux_S3_STB_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[STB_ANLG_SENS_S3_CKT].bAlarmActive;
            break;
        case Aux_S4_STB_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[STB_ANLG_SENS_S4_CKT].bAlarmActive;
            break;

//        case LOP_Curr_Sen_Over_Curr_id:
//                    _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[STB_ANLG_SENS_S4_CKT].bAlarmActive;
//                    break;
//        case Aux_S3_Ckt_Over_Current_id:
//                    _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[STB_ANLG_SENS_S4_CKT].bAlarmActive;
//                    break;
//        case Aux_S4_Ckt_Over_Current_id:
//                    _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[STB_ANLG_SENS_S4_CKT].bAlarmActive;
//                    break;
//        case Aux_S3_Ckt_Over_Voltage_id:
//                    _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[STB_ANLG_SENS_S4_CKT].bAlarmActive;
//                    break;
//        case Aux_S4_Ckt_Over_Voltage_id:
//                           _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[STB_ANLG_SENS_S4_CKT].bAlarmActive;
//                           break;
        case Low_load_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[LOW_LOAD_TRIP].bAlarmActive;
            break;
        case Config_Modified_id:
            _ArrAlarmStatus[u8LoggingID] = &_u8DummyZero;
            break;
        case Engine_Start_id:
            _ArrAlarmStatus[u8LoggingID] = &_u8DummyZero;
            break;
        case Engine_Stop_id:
            _ArrAlarmStatus[u8LoggingID] = &_u8DummyZero;
            break;
    }
}

void GCU_ALARMS::prvUpdateAlarmStatus()
{
    _bCommonNotification = false;
    _bCommonWarning = false;
    _bCommonElectricTrip = false;
    _bCommonShutdown = false;
    _u8EngineOff = ENGINE_MONITORING::IsEngineOff();
    _u8EngineOn = ENGINE_MONITORING::IsEngineOn();
    _u8GenReady = ENGINE_MONITORING::IsGenReady();
    _u8GenAvailable = ENGINE_MONITORING::IsGenAvailable();
    _u8MonOn = START_STOP::IsGenMonOn();
    _u8FuelRelayOn = START_STOP::IsFuelRelayOn();

    _u8MonGenContactor =  BASE_MODES::IsGenContactorClosed();
    _u8MonMainsContactor = BASE_MODES::IsMainsContactorClosed();
    _u8MonGenContactorOpen = BASE_MODES::IsGenContactorOpen();
    _u8MonMainsContactorOpen = BASE_MODES::IsMainsContactorOpen();
    for(_u8AlarmIndex = 0; _u8AlarmIndex < ALARM_LIST_LAST; _u8AlarmIndex++)
    {
        if(ArrAlarmMonitoring[_u8AlarmIndex].bEnableMonitoring)
        {
            if(*(ArrAlarmMonitoring[_u8AlarmIndex].LocalEnable))
            {
                if(ArrAlarmMonitoring[_u8AlarmIndex].bMonitoringPolarity)
                {
                    if(ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant)
                    {
                       prvAssignAlarmLatchedAction(_u8AlarmIndex);

                        if(((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == ONE_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u8Value) <= ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u8Value) || \
                                ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == TWO_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u16Value) < ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u16Value) ||   \
                                ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == FLOAT_TYPE) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->f32Value) < ArrAlarmMonitoring[_u8AlarmIndex].Threshold.f32Value))
                        {
                            ArrAlarmMonitoring[_u8AlarmIndex].u16Counter++;
                            if(ArrAlarmMonitoring[_u8AlarmIndex].u16Counter >= ArrAlarmMonitoring[_u8AlarmIndex].u16CounterMax)
                            {
                                ArrAlarmMonitoring[_u8AlarmIndex].u16Counter = 0;
                                ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant = false;
                            }
                        }
                        else
                        {
                            ArrAlarmMonitoring[_u8AlarmIndex].u16Counter = 0;
                        }
                    }
                    else
                    {
                        if(((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == ONE_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u8Value) > ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u8Value) || \
                                ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == TWO_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u16Value) >= ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u16Value) ||   \
                                ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == FLOAT_TYPE) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->f32Value) > ArrAlarmMonitoring[_u8AlarmIndex].Threshold.f32Value))
                        {
                            ArrAlarmMonitoring[_u8AlarmIndex].u16Counter++;
                            if(ArrAlarmMonitoring[_u8AlarmIndex].u16Counter >= ArrAlarmMonitoring[_u8AlarmIndex].u16CounterMax)
                            {
                                ArrAlarmMonitoring[_u8AlarmIndex].u16Counter = 0;
                                ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant = true;
                                ArrAlarmMonitoring[_u8AlarmIndex].bResultLatched = true;
                                prvAssignAlarmLatchedAction(_u8AlarmIndex);
                                prvLogAlarmEvent(_u8AlarmIndex);

                            }
                        }
                        else
                        {
                            ArrAlarmMonitoring[_u8AlarmIndex].u16Counter = 0;
                        }
                    }
                }
                else
                {
                    if(ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant)
                    {
                        prvAssignAlarmLatchedAction(_u8AlarmIndex);
                        if(((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == ONE_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u8Value) >= ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u8Value) || \
                                ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == TWO_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u16Value) > ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u16Value) ||   \
                                ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == FLOAT_TYPE) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->f32Value) > ArrAlarmMonitoring[_u8AlarmIndex].Threshold.f32Value))
                        {
                            ArrAlarmMonitoring[_u8AlarmIndex].u16Counter++;
                            if(ArrAlarmMonitoring[_u8AlarmIndex].u16Counter >= ArrAlarmMonitoring[_u8AlarmIndex].u16CounterMax)
                            {
                                ArrAlarmMonitoring[_u8AlarmIndex].u16Counter = 0;
                                ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant = false;
                            }
                        }
                        else
                        {
                            ArrAlarmMonitoring[_u8AlarmIndex].u16Counter = 0;
                        }
                    }
                    else
                    {
                        if(((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == ONE_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u8Value) < ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u8Value) || \
                                ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == TWO_BYTE_INT) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->u16Value) <= ArrAlarmMonitoring[_u8AlarmIndex].Threshold.u16Value) ||   \
                                ((ArrAlarmMonitoring[_u8AlarmIndex].ThreshDataType == FLOAT_TYPE) && (ArrAlarmMonitoring[_u8AlarmIndex].pValue->f32Value) < ArrAlarmMonitoring[_u8AlarmIndex].Threshold.f32Value))
                        {
                            ArrAlarmMonitoring[_u8AlarmIndex].u16Counter++;
                            if(ArrAlarmMonitoring[_u8AlarmIndex].u16Counter >= ArrAlarmMonitoring[_u8AlarmIndex].u16CounterMax)
                            {
                                ArrAlarmMonitoring[_u8AlarmIndex].u16Counter = 0;
                                ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant = true;
                                ArrAlarmMonitoring[_u8AlarmIndex].bResultLatched = true;
                                prvAssignAlarmLatchedAction(_u8AlarmIndex);
                                prvLogAlarmEvent(_u8AlarmIndex);
                            }
                        }
                        else
                        {
                            ArrAlarmMonitoring[_u8AlarmIndex].u16Counter = 0;
                        }
                    }
                }
            }
            else
            {
                ArrAlarmMonitoring[_u8AlarmIndex].u16Counter = 0;
                if(((ArrAlarmMonitoring[_u8AlarmIndex].bWarningLatched) && (_cfgz.GetCFGZ_Param(CFGZ::ID_WARNING_AUTO_CLEAR_EN) == CFGZ::CFGZ_ENABLE))
                        || (ArrAlarmMonitoring[_u8AlarmIndex].bNotificationLatched ))
                {
                    ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant = false;
                    ArrAlarmMonitoring[_u8AlarmIndex].bResultLatched = false;
                    if(_u8AlarmIndex == CHARG_ALT_FAIL)
                    {
                        CHARGING_ALT::ClearChargAltFail();
                        ArrAlarmMonitoring[CHARG_ALT_UV].bResultInstant = false;
                        ArrAlarmMonitoring[CHARG_ALT_UV].bResultLatched = false;
                        ArrAlarmMonitoring[CHARG_ALT_UV].u16Counter = 0;
                    }
                }
            }
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_WARNING_AUTO_CLEAR_EN) == CFGZ::CFGZ_ENABLE) && (_u8AlarmIndex != LOW_LOAD_TRIP))
            {
                ArrAlarmMonitoring[_u8AlarmIndex].bWarningLatched = ArrAlarmMonitoring[_u8AlarmIndex].bEnableWarning &&
                        ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant;
            }

            if(_u8AlarmIndex != LOW_LOAD_TRIP)
            {
                ArrAlarmMonitoring[_u8AlarmIndex].bNotificationLatched = ArrAlarmMonitoring[_u8AlarmIndex].bEnableNotification &&
                        ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant;
            }

            _bCommonNotification = _bCommonNotification || ArrAlarmMonitoring[_u8AlarmIndex].bNotificationLatched;
            _bCommonWarning = _bCommonWarning || ArrAlarmMonitoring[_u8AlarmIndex].bWarningLatched;
            _bCommonElectricTrip = _bCommonElectricTrip || ArrAlarmMonitoring[_u8AlarmIndex].bElectricTripLatched;
            _bCommonShutdown = _bCommonShutdown || ArrAlarmMonitoring[_u8AlarmIndex].bShutdownLatched;
            ArrAlarmMonitoring[_u8AlarmIndex].bAlarmActive = (bool)(ArrAlarmMonitoring[_u8AlarmIndex].bNotificationLatched ||
                    ArrAlarmMonitoring[_u8AlarmIndex].bWarningLatched ||
                    ArrAlarmMonitoring[_u8AlarmIndex].bElectricTripLatched||
                    ArrAlarmMonitoring[_u8AlarmIndex].bShutdownLatched);
        }

    }
        _bCommonAlarm = (_bCommonWarning || _bCommonElectricTrip || _bCommonShutdown);
        _u8LowFuelLevelAlarm = ArrAlarmMonitoring[LOW_FUEL_LEVEL_SHUTDOWN].bAlarmActive || ArrAlarmMonitoring[LOW_FUEL_LVL_SWITCH].bAlarmActive || ArrAlarmMonitoring[LOW_FUEL_LEVEL_WARNING].bAlarmActive;
        _u8LowOilPressAlarm = ArrAlarmMonitoring[LOW_OIL_PRESS_WARNING].bAlarmActive || ArrAlarmMonitoring[LOW_OIL_PRESS_SHUTDOWN].bAlarmActive || ArrAlarmMonitoring[LLOP_SWITCH].bAlarmActive;

        _u8AuxSensS1Alarm =  ArrAlarmMonitoring[ANLG_SENS_S1_MON_SHUTDOWN].bAlarmActive ||  ArrAlarmMonitoring[ANLG_SENS_S1_MON_WARNING].bAlarmActive;
        _u8AuxSensS2Alarm =  ArrAlarmMonitoring[ANLG_SENS_S2_MON_SHUTDOWN].bAlarmActive ||  ArrAlarmMonitoring[ANLG_SENS_S2_MON_WARNING].bAlarmActive;
        _u8AuxSensS3Alarm =  ArrAlarmMonitoring[ANLG_SENS_S3_MON_SHUTDOWN].bAlarmActive ||  ArrAlarmMonitoring[ANLG_SENS_S3_MON_WARNING].bAlarmActive;
        _u8AuxSensS4Alarm =  ArrAlarmMonitoring[ANLG_SENS_S4_MON_SHUTDOWN].bAlarmActive ||  ArrAlarmMonitoring[ANLG_SENS_S4_MON_WARNING].bAlarmActive;

        _u8ActuatorFailAlarm = ArrAlarmMonitoring[ACTUATOR_FAIL_ABOVE_30].bAlarmActive || ArrAlarmMonitoring[ACTUATOR_FAIL_BELOW_30].bAlarmActive;

        _bBTSBattHealthy = !(ArrAlarmMonitoring[VBTS_UV].bEnableMonitoring && ArrAlarmMonitoring[VBTS_UV].bResultInstant);
       if(_u8EngineOff == 0)
       {
           _u8HighOilPressDetectedAlarm =0;
       }
       else
        {
           A_SENSE::SENSOR_RET_t stLOP1 = GetLOPSensorVal();
          if(!((ArrAlarmMonitoring[GCU_ALARMS::OIL_PRESS_DETECTED].bResultInstant
                    &&( stLOP1.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT))
             ||ArrAlarmMonitoring[GCU_ALARMS::LLOP_SWITCH_AT_ENG_OFF].bResultInstant
             )
           )
          {
              _u8HighOilPressDetectedAlarm = 0;
          }
        }

       if(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_THRESH_TYPE) == CFGZ::CFGZ_LESS_THAN_THRESHOLD)
       {
           _u8HighEngTempAlarm = ArrAlarmMonitoring[HIGH_ENG_TEMP_SHUTDOWN].bAlarmActive || ArrAlarmMonitoring[HIGH_ENG_TEMP_WARNING].bAlarmActive;

       }
       else
       {
           _u8HighEngTempAlarm = ArrAlarmMonitoring[HIGH_ENG_TEMP_SHUTDOWN].bAlarmActive || ArrAlarmMonitoring[HIGH_ENG_TEMP_WARNING].bAlarmActive || ArrAlarmMonitoring[HWT_SWITCH].bAlarmActive;

       }

       _u8HighEngTempSwitch = ArrAlarmMonitoring[HWT_SWITCH].bAlarmActive;
       _u8MaintAlarm = ArrAlarmMonitoring[FILT_MAINTENANCE].bAlarmActive || ArrAlarmMonitoring[ID_FILT_MAINTENANCE_BY_DATE].bAlarmActive;


   if(ArrAlarmMonitoring[SHELTER_TEMP_STOP_GEN].bResultInstant && ArrAlarmMonitoring[SHELTER_TEMP_START_GEN].bResultLatched )
   {
       ArrAlarmMonitoring[SHELTER_TEMP_START_GEN].bResultLatched = false;
       /* As result latched is getting cleared, result instantaneous should also get cleared*/
       ArrAlarmMonitoring[SHELTER_TEMP_START_GEN].bResultInstant = false;
   }

   _bHighShelterTemp = ArrAlarmMonitoring[SHELTER_TEMP_START_GEN].bResultLatched ;
   ArrAlarmMonitoring[SHELTER_TEMP_START_GEN].bNotificationLatched = _bHighShelterTemp;


    prvGensetRelatedAlarms();
    if((!_bCommonShutdown && !_bCommonWarning && !_bCommonElectricTrip)
            ||(UTILS_GetElapsedTimeInSec(&_SounderAlarmTimer)
               >= _cfgz.GetCFGZ_Param(CFGZ::ID_SOUNDER_ALARM_TIME)))
    {
       _bOPSounderAlarm = false;
       UTILS_DisableTimer(&_SounderAlarmTimer);
    }

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_MON_EN)==CFGZ::CFGZ_ENABLE)
    {
        prvCheckTripAction( MAINS_R_UNDERVOLT_RETURN,   MAINS_R_UNDERVOLT_TRIP, BASE_MODES::GetIndividualPhaseStatus(R_PHASE));
        prvCheckTripAction( MAINS_Y_UNDERVOLT_RETURN,   MAINS_Y_UNDERVOLT_TRIP, BASE_MODES::GetIndividualPhaseStatus(Y_PHASE));
        prvCheckTripAction( MAINS_B_UNDERVOLT_RETURN,   MAINS_B_UNDERVOLT_TRIP, BASE_MODES::GetIndividualPhaseStatus(B_PHASE));

        prvCheckTripAction( MAINS_R_OVERVOLT_RETURN,   MAINS_R_OVERVOLT_TRIP,   BASE_MODES::GetIndividualPhaseStatus(R_PHASE));
        prvCheckTripAction( MAINS_Y_OVERVOLT_RETURN,   MAINS_Y_OVERVOLT_TRIP,   BASE_MODES::GetIndividualPhaseStatus(Y_PHASE));
        prvCheckTripAction( MAINS_B_OVERVOLT_RETURN,   MAINS_B_OVERVOLT_TRIP,   BASE_MODES::GetIndividualPhaseStatus(B_PHASE));

        prvCheckTripAction( MAINS_R_UNDERFREQ_RETURN,   MAINS_R_UNDERFREQ_TRIP, BASE_MODES::GetIndividualPhaseStatus(R_PHASE));
        prvCheckTripAction( MAINS_Y_UNDERFREQ_RETURN,   MAINS_Y_UNDERFREQ_TRIP, BASE_MODES::GetIndividualPhaseStatus(Y_PHASE));
        prvCheckTripAction( MAINS_B_UNDERFREQ_RETURN,   MAINS_B_UNDERFREQ_TRIP, BASE_MODES::GetIndividualPhaseStatus(B_PHASE));

        prvCheckTripAction( MAINS_R_OVERFREQ_RETURN,   MAINS_R_OVERFREQ_TRIP,   BASE_MODES::GetIndividualPhaseStatus(R_PHASE));
        prvCheckTripAction( MAINS_Y_OVERFREQ_RETURN,   MAINS_Y_OVERFREQ_TRIP,   BASE_MODES::GetIndividualPhaseStatus(Y_PHASE));
        prvCheckTripAction( MAINS_B_OVERFREQ_RETURN,   MAINS_B_OVERFREQ_TRIP,   BASE_MODES::GetIndividualPhaseStatus(B_PHASE));

    }
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_ALARM_EN)==CFGZ::CFGZ_ENABLE)
            && (_cfgz.GetCFGZ_Param(CFGZ::ID_LOW_LOAD_ACTION)<=ID_WARNING))
    {
        if(ArrAlarmMonitoring[LOW_LOAD_RETURN].bResultInstant && ArrAlarmMonitoring[LOW_LOAD_TRIP].bResultLatched  )
        {
            ArrAlarmMonitoring[LOW_LOAD_TRIP].bResultLatched = false;
            /* As result latched is getting cleared, result instantaneous should also get cleared*/
            ArrAlarmMonitoring[LOW_LOAD_TRIP].bResultInstant = false;
            ArrAlarmMonitoring[LOW_LOAD_TRIP].bAlarmActive =false;
            ArrAlarmMonitoring[LOW_LOAD_TRIP].bWarningLatched =false;
            ArrAlarmMonitoring[LOW_LOAD_TRIP].bNotificationLatched=false;

        }
        else if(ArrAlarmMonitoring[LOW_LOAD_RETURN].bResultLatched  && ArrAlarmMonitoring[LOW_LOAD_TRIP].bResultInstant)
        {
            ArrAlarmMonitoring[LOW_LOAD_RETURN].bResultLatched = false;
                /* As result latched is getting cleared, result instantaneous should also get cleared*/
            ArrAlarmMonitoring[LOW_LOAD_RETURN].bResultInstant = false;
        }
    }
}

void GCU_ALARMS::prvCheckTripAction(uint8_t u8ReturnIndex, uint8_t u8TripIndex, bool status)
{
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_1_PHASE_SYSTEM)
                    || (!_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_PARTIAL_HEALTHY_DETECT_EN)))
     {
        status = BASE_MODES::GetMainsStatus();
     }

   if(ArrAlarmMonitoring[u8ReturnIndex].bResultInstant && ArrAlarmMonitoring[u8TripIndex].bResultLatched && (!status) )
   {
       ArrAlarmMonitoring[u8TripIndex].bResultLatched = false;
       /* As result latched is getting cleared, result instantaneous should also get cleared*/
       ArrAlarmMonitoring[u8TripIndex].bResultInstant = false;
   }
   else if(ArrAlarmMonitoring[u8ReturnIndex].bResultLatched  && ArrAlarmMonitoring[u8TripIndex].bResultInstant && (status))
   {
       ArrAlarmMonitoring[u8ReturnIndex].bResultLatched = false;
           /* As result latched is getting cleared, result instantaneous should also get cleared*/
       ArrAlarmMonitoring[u8ReturnIndex].bResultInstant = false;
   }
}

bool GCU_ALARMS::IsCommonNotification()
{
    return _bCommonNotification;
}

bool GCU_ALARMS::IsCommonWarning()
{
    return _bCommonWarning;
}

bool GCU_ALARMS::IsCommonElectricTrip()
{
    return _bCommonElectricTrip;
}

bool GCU_ALARMS::IsCommonShutdown()
{
    return _bCommonShutdown;
}

bool GCU_ALARMS::IsAlarmActive(ALARM_LIST_t AlarmID)
{
    return ArrAlarmMonitoring[AlarmID].bAlarmActive;
}

bool GCU_ALARMS::AlarmResultInstat(ALARM_LIST_t AlarmID)
{
    return ArrAlarmMonitoring[AlarmID].bResultInstant;
}

bool GCU_ALARMS::AlarmResultLatched(ALARM_LIST_t AlarmID)
{
    return ArrAlarmMonitoring[AlarmID].bResultLatched;
}

bool GCU_ALARMS::IsAlarmMonEnabled(ALARM_LIST_t AlarmID)
{
    return ArrAlarmMonitoring[AlarmID].bEnableMonitoring;
}

void GCU_ALARMS::ClearAllAlarms()
{
    _bCommonNotification = false;
    _bCommonWarning = false;
    _bCommonElectricTrip = false;
    _bCommonShutdown = false;
    _bCommonAlarm = false;

    for(_u8AlarmIndex = 0; _u8AlarmIndex < ALARM_LIST_LAST;_u8AlarmIndex++)
    {
        if((_u8AlarmIndex != REMOTE_START_STOP)&&(_u8AlarmIndex != VBTS_UV))
        {
            ArrAlarmMonitoring[_u8AlarmIndex].bNotificationLatched = false;
            ArrAlarmMonitoring[_u8AlarmIndex].bWarningLatched = false;
            ArrAlarmMonitoring[_u8AlarmIndex].bElectricTripLatched = false;
            ArrAlarmMonitoring[_u8AlarmIndex].bShutdownLatched = false;
            ArrAlarmMonitoring[_u8AlarmIndex].bAlarmActive = false;
            if(!((_u8AlarmIndex >= MAINS_R_UNDERVOLT_TRIP) &&
                    (_u8AlarmIndex <= MAINS_B_OVERFREQ_RETURN)))
            {
                ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant = false;
                ArrAlarmMonitoring[_u8AlarmIndex].bResultLatched = false;
            }
        }
    }
    _bFailToStop = false;
    _bFailToStart = false;
    _u8FuelTheftAlarm = 0;
    _u8MPULossAlarm = 0;
    _u8AFTActivationTimeout = 0;
    _u8HighOilPressDetectedAlarm = 0;

    _hal.AcSensors.ClearPhaseReverseAlarms();

    ENGINE_MONITORING::ClearTriplatched();
    if(ArrAlarmMonitoring[AFT_ACTIVATION_TIMEOUT].bAlarmActive)
    {
        UTILS_DisableTimer(&_AFTTimeoutAfterActTimer);
    }

    _u8UnderFreqAlarm = 0;

    _u8OverFreqAlarm = 0;

    _u8RPhaseOverVoltAlarm = 0;

    _u8YPhaseOverVoltAlarm = 0;

    _u8BPhaseOverVoltAlarm = 0;

    _u8RPhaseUnderVoltAlarm = 0;

    _u8YPhaseUnderVoltAlarm = 0;

    _u8BPhaseUnderVoltAlarm = 0;

//    if(_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION) == CFGZ::CFGZ_ANLG_LOP_CURR_SENSOR)
//    {
//        _hal.AnlgIp.ConfigPin23(ANLG_IP::BSP_MODE_CURRENT);
//    }

    gpJ1939->ResetLampsStatus();
    gpJ1939->RequestDM11PGN();
    gpJ1939->SetDm2MsgCount(0);
    gpJ1939->ClearDM2Messages();
    gpJ1939->ClearNCDandPCDAlarms();
}


void GCU_ALARMS::ResetMainsMonParams()
{
    uint8_t u8Index;

    for(u8Index = MAINS_R_UNDERVOLT_TRIP; u8Index <= MAINS_B_OVERFREQ_TRIP; u8Index++)
    {
        ArrAlarmMonitoring[u8Index].u16Counter = 0;
        ArrAlarmMonitoring[u8Index].bResultInstant = false;
        ArrAlarmMonitoring[u8Index].bResultLatched = false;
    }

    for(u8Index = MAINS_R_UNDERVOLT_RETURN; u8Index <= MAINS_B_OVERFREQ_RETURN; u8Index++)
    {
        ArrAlarmMonitoring[u8Index].u16Counter = 0;
        ArrAlarmMonitoring[u8Index].bResultInstant = true;
        ArrAlarmMonitoring[u8Index].bResultLatched = true;
    }

    if(ArrAlarmMonitoring[REMOTE_START_STOP].bEnableMonitoring)
    {
       ArrAlarmMonitoring[REMOTE_START_STOP].bResultInstant = false;
       ArrAlarmMonitoring[REMOTE_START_STOP].u16Counter = 0;
    }
}

void GCU_ALARMS::prvAssignAlarmLatchedAction(uint8_t u8Index)
{
    ArrAlarmMonitoring[u8Index].bNotificationLatched = ArrAlarmMonitoring[u8Index].bEnableNotification;
    ArrAlarmMonitoring[u8Index].bWarningLatched = ArrAlarmMonitoring[u8Index].bEnableWarning;
    ArrAlarmMonitoring[u8Index].bShutdownLatched = ArrAlarmMonitoring[u8Index].bEnableShutdown;
    ArrAlarmMonitoring[u8Index].bElectricTripLatched = ArrAlarmMonitoring[u8Index].bEnableElectricTrip;
}


void GCU_ALARMS::prvLogAlarmEvent(uint8_t u8Index)
{
    if(ArrAlarmMonitoring[u8Index].bEnableShutdown)
    {
        _bOPSounderAlarm = true;
        UTILS_ResetTimer(&_SounderAlarmTimer);
        LogEvent(ArrAlarmMonitoring[u8Index].u8LoggingID, ID_SHUTDOWN);
    }
    else if(ArrAlarmMonitoring[u8Index].bEnableWarning)
    {
        _bOPSounderAlarm = true;
        UTILS_ResetTimer(&_SounderAlarmTimer);
        LogEvent(ArrAlarmMonitoring[u8Index].u8LoggingID, ID_WARNING);
    }
    else if(ArrAlarmMonitoring[u8Index].bEnableElectricTrip)
    {
        _bOPSounderAlarm = true;
        UTILS_ResetTimer(&_SounderAlarmTimer);
        LogEvent(ArrAlarmMonitoring[u8Index].u8LoggingID, ID_ELECTRICAL_TRIP);
    }
    else if(ArrAlarmMonitoring[u8Index].bEnableNotification)
    {
       _bOPSounderAlarm = true;
    }

}

void GCU_ALARMS::prvGensetRelatedAlarms()
{
    _u8UnderFreqAlarm = ArrAlarmMonitoring[UNDERFREQ_SHUTDOWN].bAlarmActive ||
                       ArrAlarmMonitoring[UNDERFREQ_WARNING].bAlarmActive;
    _u8OverFreqAlarm = ArrAlarmMonitoring[OVERFREQ_SHUTDOWN].bAlarmActive ||
                      ArrAlarmMonitoring[OVERFREQ_WARNING].bAlarmActive;
    _u8RPhaseOverVoltAlarm = ArrAlarmMonitoring[GEN_R_OV_SHUTDOWN].bAlarmActive ||
                            ArrAlarmMonitoring[GEN_R_OV_WARNING].bAlarmActive;
    _u8YPhaseOverVoltAlarm = ArrAlarmMonitoring[GEN_Y_OV_SHUTDOWN].bAlarmActive ||
                            ArrAlarmMonitoring[GEN_Y_OV_WARNING].bAlarmActive;
    _u8BPhaseOverVoltAlarm = ArrAlarmMonitoring[GEN_B_OV_SHUTDOWN].bAlarmActive ||
                            ArrAlarmMonitoring[GEN_B_OV_WARNING].bAlarmActive;
    _u8RPhaseUnderVoltAlarm = ArrAlarmMonitoring[GEN_R_UV_SHUTDOWN].bAlarmActive ||
                             ArrAlarmMonitoring[GEN_R_UV_WARNING].bAlarmActive;
    _u8YPhaseUnderVoltAlarm = ArrAlarmMonitoring[GEN_Y_UV_SHUTDOWN].bAlarmActive ||
                             ArrAlarmMonitoring[GEN_Y_UV_WARNING].bAlarmActive;
    _u8BPhaseUnderVoltAlarm = ArrAlarmMonitoring[GEN_B_UV_SHUTDOWN].bAlarmActive ||
                             ArrAlarmMonitoring[GEN_B_UV_WARNING].bAlarmActive;
}


bool GCU_ALARMS::IsAlarmPresent()
{
    if(_bCommonNotification || _bCommonWarning || _bCommonElectricTrip || _bCommonShutdown)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GCU_ALARMS::UpdateFailToStart()
{
    _bFailToStart = true;
}

void GCU_ALARMS::UpdateFailToStop()
{
    _bFailToStop = true;
}

bool GCU_ALARMS::IsFailToStart()
{
    return _bFailToStart;
}

uint8_t GCU_ALARMS::GetActiveAlarmCount()
{
    return _u8AlarmArrayIndex;
}

void GCU_ALARMS::FillDisplayAlarmArray()
{
    _u8AlarmArrayIndex = 0;
    for(uint8_t i = 0; i < ID_ALL_ALARMS_LAST; i++)
    {
        if(*_ArrAlarmStatus[i] == 1)
        {
            _ArrAlarmForDisplay[_u8AlarmArrayIndex] = i;
            _u8AlarmArrayIndex++;
        }
        else
        {
            _ArrAlarmForDisplay[_u8AlarmArrayIndex] = 0;
        }
    }
}

void GCU_ALARMS::LogEvent(uint8_t u8EventID, uint8_t u8EventType)
{
    EVENT_LOG_Q_t  stLogLocal;
    RTC::TIME_t currentTime;

    stLogLocal.stEventLog.u8EventId = u8EventID;
    stLogLocal.stEventLog.u8EventType = u8EventType;

    stLogLocal.stEventLog.u16Speed =  (uint16_t)GetSpeedValue();

    stLogLocal.stEventLog.u16GensetTotlKW =(uint16_t) ((_hal.AcSensors.GENSET_GetActivePowerWatts(R_PHASE) +
            _hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE) +
            _hal.AcSensors.GENSET_GetActivePowerWatts(B_PHASE))/100);

    A_SENSE::SENSOR_RET_t stTempVal;
    stTempVal = GetLOPSensorVal();
    if(stTempVal.eStatus == A_SENSE::SENSOR_NOT_CONFIGRUED)
    {
        stLogLocal.stEventLog.u16Lop =  EVENT_LOG_LOP_SENSOR_NA;
    }
    else
    {
        if(stTempVal.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT)
        {
            stLogLocal.stEventLog.u16Lop =  EVENT_LOG_LOP_SENSOR_OC;
        }
        else
        {
            stLogLocal.stEventLog.u16Lop = (uint16_t)(stTempVal.stValAndStatus.f32InstSensorVal*100);
        }
    }

    stLogLocal.stEventLog.u8Dummy = 0;

    _hal.ObjRTC.GetTime(&currentTime);
    stLogLocal.stEventLog.u8Hour= currentTime.u8Hour;
    stLogLocal.stEventLog.u8Minute= currentTime.u8Minute;
    stLogLocal.stEventLog.u8Second= currentTime.u8Second;
    stLogLocal.stEventLog.u8Day= currentTime.u8Day;
    stLogLocal.stEventLog.u8Month = currentTime.u8Month;
    stLogLocal.stEventLog.u16Year =  currentTime.u16Year;

    stLogLocal.stEventLog.u32EngineHrs = GetSelectedEngRunMin();
    stLogLocal.u32EventNo =_u32EventNumber;
    _EventQueue.EnQueue(&stLogLocal);

    _u32EventNumber++;
    if(_u32EventNumber >= CFGC::GetMaxNumberOfEvents())
    {
       _u32EventNumber =0;
       _u32RolledOverByte =1;
       _hal.Objeeprom.RequestWrite(EXT_EEPROM_ROLLED_OVER_ADDRESS , (uint8_t*)&_u32RolledOverByte,
                                   sizeof(_u32RolledOverByte), NULL);
    }
}

uint8_t GCU_ALARMS::prvGetUnbalancedLoadVal()
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        float f32GenLoadDiff = prvGetGenMaxKWPercent() - prvGetGenMinKWPercent();
        return (uint8_t)((3*100*f32GenLoadDiff)/(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_RATING)));
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_SPLIT_PHASE)
    {
        float f32GenLoadDiff = prvGetGenMaxKWPercent() - prvGetGenMinKWPercent();
        return (uint8_t)((2*100*f32GenLoadDiff)/(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_RATING)));
    }
    else
    {
        return 0;
    }
}

void GCU_ALARMS::prvActDeactOutput(bool bOutputCondition, ACTUATOR::ACTUATOR_TYPS_t eACTType)
{
    if(bOutputCondition)
    {
        _hal.actuators.Activate(eACTType);
    }
    else
    {
        _hal.actuators.Deactivate(eACTType);
    }
}

void GCU_ALARMS::prvUpdateOutputs()
{
    prvActDeactOutput(_bOPSounderAlarm, ACTUATOR::ACT_AUDIBLE_ALARM);
    prvActDeactOutput(_bCommonWarning, ACTUATOR::ACT_WARNING);
    prvActDeactOutput(_bCommonElectricTrip, ACTUATOR::ACT_ELEC_TRIP);
    prvActDeactOutput(_bCommonShutdown, ACTUATOR::ACT_SHUTDOWN);
    prvActDeactOutput(_bCommonAlarm, ACTUATOR::ACT_ALARM);
    prvActDeactOutput(ArrAlarmMonitoring[VBAT_OV].bResultInstant, ACTUATOR::ACT_VBAT_OV);
    prvActDeactOutput(ArrAlarmMonitoring[VBAT_UV].bResultInstant, ACTUATOR::ACT_VBAT_UV);
    prvActDeactOutput(ArrAlarmMonitoring[DIGITAL_INPUT_A].bResultInstant, ACTUATOR::ACT_DIG_IN_A);
    prvActDeactOutput(ArrAlarmMonitoring[DIGITAL_INPUT_B].bResultInstant, ACTUATOR::ACT_DIG_IN_B);
    prvActDeactOutput(ArrAlarmMonitoring[DIGITAL_INPUT_C].bResultInstant, ACTUATOR::ACT_DIG_IN_C);
    prvActDeactOutput(ArrAlarmMonitoring[DIGITAL_INPUT_D].bResultInstant, ACTUATOR::ACT_DIG_IN_D);
    prvActDeactOutput(ArrAlarmMonitoring[DIGITAL_INPUT_E].bResultInstant, ACTUATOR::ACT_DIG_IN_E);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_F].bResultInstant, ACTUATOR::ACT_DIG_IN_F);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_G].bResultInstant, ACTUATOR::ACT_DIG_IN_G);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_H].bResultInstant, ACTUATOR::ACT_DIG_IN_H);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_I].bResultInstant, ACTUATOR::ACT_DIG_IN_I);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_J].bResultInstant, ACTUATOR::ACT_DIG_IN_J);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_K].bResultInstant, ACTUATOR::ACT_DIG_IN_K);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_L].bResultInstant, ACTUATOR::ACT_DIG_IN_L);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_M].bResultInstant, ACTUATOR::ACT_DIG_IN_M);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_N].bResultInstant, ACTUATOR::ACT_DIG_IN_N);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_O].bResultInstant, ACTUATOR::ACT_DIG_IN_O);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_P].bResultInstant, ACTUATOR::ACT_DIG_IN_P);
    prvActDeactOutput(ArrAlarmMonitoring[PANEL_LOCK].bAlarmActive, ACTUATOR::ACT_STOP_PANEL_LOCK);
    prvActDeactOutput(ArrAlarmMonitoring[EX_AUTO_PANEL_LOCK].bAlarmActive, ACTUATOR::ACT_EXTERNAL_PANEL_LOCK);
    prvActDeactOutput(ArrAlarmMonitoring[FAIL_TO_LATCH_GEN_CONT].bAlarmActive, ACTUATOR::ACT_FAIL_TO_CLOSE_GEN_CONT);
    prvActDeactOutput(ArrAlarmMonitoring[FAIL_TO_LATCH_MAINS_CONT].bAlarmActive, ACTUATOR::ACT_FAIL_TO_CLOSE_MAINS_CONT);
    prvActDeactOutput(ArrAlarmMonitoring[LOADING_VOLT_UNHEALTHY].bAlarmActive, ACTUATOR::ACT_LOADING_VTG_NOT_REACHED);
    prvActDeactOutput(ArrAlarmMonitoring[LOADING_FREQ_UNHEALTHY].bAlarmActive, ACTUATOR::ACT_LOADING_FREQ_NOT_REACHED);
    prvActDeactOutput(ArrAlarmMonitoring[MPU_LOSS].bAlarmActive, ACTUATOR::ACT_MPU_LOSS);


    prvActDeactOutput(_bFailToStart, ACTUATOR::ACT_FAIL_TO_START);
    prvActDeactOutput(_bFailToStop, ACTUATOR::ACT_FAIL_TO_STOP);
    prvActDeactOutput(ArrAlarmMonitoring[GEN_R_OV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_R_OV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[GEN_Y_OV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_Y_OV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[GEN_B_OV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_B_OV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[GEN_R_UV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_R_UV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[GEN_Y_UV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_Y_UV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[GEN_B_UV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_B_UV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[OVERCURRENT].bResultLatched, ACTUATOR::ACT_GEN_OC);
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_THRESH_TYPE) != CFGZ::CFGZ_LESS_THAN_THRESHOLD)
    {
        prvActDeactOutput(_u8HighEngTempAlarm, ACTUATOR::ACT_HIGH_TEMP);
    }
    else
    {
        prvActDeactOutput(_u8HighEngTempSwitch, ACTUATOR::ACT_HIGH_TEMP);
    }

    prvActDeactOutput(_u8LowFuelLevelAlarm, ACTUATOR::ACT_LOW_FUEL);
    prvActDeactOutput(_u8LowOilPressAlarm, ACTUATOR::ACT_LOW_PRES);
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION)==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        prvActDeactOutput((ArrAlarmMonitoring[LOP_RES_SENS_OPEN_CKT].bResultInstant), ACTUATOR::ACT_OIL_CKT_OPEN);
    }
    else if((_cfgz.GetCFGZ_Param(CFGZ::ID_S4_SENS_SELECTION)==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
    {
        if(ArrAlarmMonitoring[LOP_CURR_OPEN_CKT].bResultInstant || ArrAlarmMonitoring[LOP_CURR_SENS_STB].bResultInstant)
        {
            prvActDeactOutput(true, ACTUATOR::ACT_OIL_CKT_OPEN);
        }

    }
    prvActDeactOutput(_u8LowFuelLevelAlarm, ACTUATOR::ACT_LOW_FUEL);
    prvActDeactOutput(ArrAlarmMonitoring[OPEN_ENG_TEMP_CKT].bAlarmActive, ACTUATOR::ACT_TEMP_CKT_OPEN);
    prvActDeactOutput(ArrAlarmMonitoring[OVERSPEED].bShutdownLatched, ACTUATOR::ACT_OS_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[GROSS_OVERSPEED].bShutdownLatched, ACTUATOR::ACT_GROSS_OS_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[UNDERSPEED].bShutdownLatched, ACTUATOR::ACT_US_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[OVERFREQ_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_OF_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[UNDERFREQ_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_UF_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[CHARG_ALT_FAIL].bShutdownLatched, ACTUATOR::ACT_CA_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[CHARG_ALT_FAIL].bWarningLatched, ACTUATOR::ACT_CA_WARNING);
    prvActDeactOutput(_u8MaintAlarm, ACTUATOR::ACT_FILT_MAINT);
    prvActDeactOutput(ArrAlarmMonitoring[EMERGENCY_STOP].bAlarmActive, ACTUATOR::ACT_E_STOP);
    prvActDeactOutput(_bOpAutoFuelTransfer, ACTUATOR::ACT_AUTOMATIC_FUEL_TRANSFER);
    prvActDeactOutput(_bCLNTTempCtrl, ACTUATOR::ACT_CLNT_TEMP_CTRL);



    prvActDeactOutput(BASE_MODES::GetMainsHighStatus(), ACTUATOR::ACT_MAINS_HIGH);
    prvActDeactOutput(BASE_MODES::GetMainsLowStatus(), ACTUATOR::ACT_MAINS_LOW);
    if(BASE_MODES::GetMainsStatus() ==  BASE_MODES::MAINS_HELATHY)
    {
        prvActDeactOutput(0, ACTUATOR::ACT_MAINS_FAILURE);
    }
    else
    {
        prvActDeactOutput(1, ACTUATOR::ACT_MAINS_FAILURE);
    }


}

uint16_t GCU_ALARMS::GetMinGensetVoltage()
{
    static uint16_t u16MinGensetVoltage;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) != CFGZ::CFGZ_1_PHASE_SYSTEM)
    {
        u16MinGensetVoltage = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVoltsRaw(R_PHASE);
        if((uint16_t)_hal.AcSensors.GENSET_GetVoltageVoltsRaw(Y_PHASE) < u16MinGensetVoltage)
        {
            u16MinGensetVoltage = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVoltsRaw(Y_PHASE);
        }
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_1_PHASE_SYSTEM)
        {
            if((uint16_t)_hal.AcSensors.GENSET_GetVoltageVoltsRaw(B_PHASE) < u16MinGensetVoltage)
            {
                u16MinGensetVoltage = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVoltsRaw(B_PHASE);
            }
        }
    }
    else
    {
        u16MinGensetVoltage = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVoltsRaw(R_PHASE);
    }
    return u16MinGensetVoltage;
}

uint16_t GCU_ALARMS::prvGetMinMainsVoltage()
{
    static uint16_t u16MinMainsVoltage;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        u16MinMainsVoltage = (uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(R_PHASE);
        if((uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(Y_PHASE) < u16MinMainsVoltage)
        {
            u16MinMainsVoltage = (uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(Y_PHASE);
        }

        if((uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(B_PHASE) < u16MinMainsVoltage)
        {
            u16MinMainsVoltage = (uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(B_PHASE);
        }
    }
    else
    {
        u16MinMainsVoltage = (uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(R_PHASE);
    }
    return u16MinMainsVoltage;
}

uint16_t GCU_ALARMS::prvGetMinPhToPhMainsVoltage()
{
    static uint16_t u16MinMainsVoltage = 0;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        u16MinMainsVoltage = (uint16_t)_hal.AcSensors.MAINS_GetRYVolts();
        if((uint16_t)_hal.AcSensors.MAINS_GetYBVolts() < u16MinMainsVoltage)
        {
            u16MinMainsVoltage = (uint16_t)_hal.AcSensors.MAINS_GetYBVolts();
        }

        if((uint16_t)_hal.AcSensors.MAINS_GetRBVolts() < u16MinMainsVoltage)
        {
            u16MinMainsVoltage = (uint16_t)_hal.AcSensors.MAINS_GetRBVolts();
        }
    }
    return u16MinMainsVoltage;
}

uint16_t GCU_ALARMS::prvGetMaxMainsVoltage()
{
    static uint16_t u16MaxMainsVoltage;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        u16MaxMainsVoltage = (uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(R_PHASE);
        if((uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(Y_PHASE) > u16MaxMainsVoltage)
        {
            u16MaxMainsVoltage = (uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(Y_PHASE);
        }

        if((uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(B_PHASE) > u16MaxMainsVoltage)
        {
            u16MaxMainsVoltage = (uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(B_PHASE);
        }
    }
    else
    {
        u16MaxMainsVoltage = (uint16_t)_hal.AcSensors.MAINS_GetVoltageVolts(R_PHASE);
    }
    return u16MaxMainsVoltage;
}

float GCU_ALARMS::GetMinMainsFreq()
{
    static float f32MinMainsFreq;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        f32MinMainsFreq = _hal.AcSensors.MAINS_GetApproxFreq(R_PHASE);
        if(_hal.AcSensors.MAINS_GetApproxFreq(Y_PHASE) < f32MinMainsFreq)
        {
            f32MinMainsFreq = _hal.AcSensors.MAINS_GetApproxFreq(Y_PHASE);
        }

        if(_hal.AcSensors.MAINS_GetApproxFreq(B_PHASE) < f32MinMainsFreq)
        {
            f32MinMainsFreq = _hal.AcSensors.MAINS_GetApproxFreq(B_PHASE);
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_SPLIT_PHASE)
    {
        f32MinMainsFreq = _hal.AcSensors.MAINS_GetApproxFreq(R_PHASE);
          if(_hal.AcSensors.MAINS_GetApproxFreq(Y_PHASE) < f32MinMainsFreq)
          {
              f32MinMainsFreq = _hal.AcSensors.MAINS_GetApproxFreq(Y_PHASE);
          }
    }
    else
    {
        f32MinMainsFreq = _hal.AcSensors.MAINS_GetApproxFreq(R_PHASE);
    }
    return f32MinMainsFreq;
}

float GCU_ALARMS::GetMinGenFreq()
{
    static float f32MinGenFreq;
    static float f32GenFreqOld1=0, f32GenFreqOld2=0;
    static float f32MaxGenOldFreq;
    f32MinGenFreq = _hal.AcSensors.GENSET_GetApproxFreq(R_PHASE);
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        if(_hal.AcSensors.GENSET_GetApproxFreq(Y_PHASE) < f32MinGenFreq)
        {
            f32MinGenFreq = _hal.AcSensors.GENSET_GetApproxFreq(Y_PHASE);
        }

        if(_hal.AcSensors.GENSET_GetApproxFreq(B_PHASE) < f32MinGenFreq)
        {
            f32MinGenFreq = _hal.AcSensors.GENSET_GetApproxFreq(B_PHASE);
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_SPLIT_PHASE)
    {
        if(_hal.AcSensors.GENSET_GetApproxFreq(Y_PHASE) < f32MinGenFreq)
        {
            f32MinGenFreq = _hal.AcSensors.GENSET_GetApproxFreq(Y_PHASE);
        }
    }
    f32MaxGenOldFreq = f32MinGenFreq;
    if(f32GenFreqOld1 > f32MaxGenOldFreq)
    {
        f32MaxGenOldFreq = f32GenFreqOld1;
    }

    if(f32GenFreqOld2 > f32MaxGenOldFreq)
    {
        f32MaxGenOldFreq = f32GenFreqOld2;
    }
    f32GenFreqOld2 = f32GenFreqOld1;
    f32GenFreqOld1 = f32MinGenFreq;
    return f32MaxGenOldFreq;

}


uint8_t GCU_ALARMS::GetAlarmId(uint8_t u8Val)
{
    return _ArrAlarmForDisplay[u8Val];
}

float GCU_ALARMS::prvGetMaxMainsFreq()
{
    static float f32MaxMainsFreq;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_AC_SYTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        f32MaxMainsFreq = _hal.AcSensors.MAINS_GetApproxFreq(R_PHASE);
        if(_hal.AcSensors.MAINS_GetApproxFreq(Y_PHASE) > f32MaxMainsFreq)
        {
            f32MaxMainsFreq = _hal.AcSensors.MAINS_GetApproxFreq(Y_PHASE);
        }

        if(_hal.AcSensors.MAINS_GetApproxFreq(B_PHASE) > f32MaxMainsFreq)
        {
            f32MaxMainsFreq = _hal.AcSensors.MAINS_GetApproxFreq(B_PHASE);
        }
    }
    else
    {
        f32MaxMainsFreq = _hal.AcSensors.MAINS_GetApproxFreq(R_PHASE);
    }
    return f32MaxMainsFreq;
}

float GCU_ALARMS::prvGetMaxGensetCurent()
{
    static float uf32MaxGenCurrent;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        uf32MaxGenCurrent = _hal.AcSensors.GENSET_GetCurrentAmps(R_PHASE);
        if(_hal.AcSensors.GENSET_GetCurrentAmps(Y_PHASE) > uf32MaxGenCurrent)
        {
            uf32MaxGenCurrent = _hal.AcSensors.GENSET_GetCurrentAmps(Y_PHASE);
        }

        if(_hal.AcSensors.GENSET_GetCurrentAmps(B_PHASE) > uf32MaxGenCurrent)
        {
            uf32MaxGenCurrent = _hal.AcSensors.GENSET_GetCurrentAmps(B_PHASE);
        }
    }
    else
    {
        uf32MaxGenCurrent = _hal.AcSensors.GENSET_GetCurrentAmps(R_PHASE);
    }
    return uf32MaxGenCurrent;
}

float GCU_ALARMS::prvGetGenMaxKWPercent()
{
    static float f32MaxGenKW =0;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        f32MaxGenKW = _hal.AcSensors.GENSET_GetActivePowerWatts(R_PHASE);
        if(_hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE) > f32MaxGenKW)
        {
            f32MaxGenKW = _hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE);
        }

        if(_hal.AcSensors.GENSET_GetActivePowerWatts(B_PHASE) > f32MaxGenKW)
        {
            f32MaxGenKW = _hal.AcSensors.GENSET_GetActivePowerWatts(B_PHASE);
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_SPLIT_PHASE)
    {
        f32MaxGenKW = _hal.AcSensors.GENSET_GetActivePowerWatts(R_PHASE);
        if(_hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE) > f32MaxGenKW)
        {
            f32MaxGenKW = _hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE);
        }

    }
    return (f32MaxGenKW/1000);
}

float GCU_ALARMS::prvGetGenMinKWPercent()
{
    static float f32MinGenKW=0;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        f32MinGenKW = _hal.AcSensors.GENSET_GetActivePowerWatts(R_PHASE);
        if(_hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE) < f32MinGenKW)
        {
            f32MinGenKW = _hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE);
        }

        if(_hal.AcSensors.GENSET_GetActivePowerWatts(B_PHASE) < f32MinGenKW)
        {
            f32MinGenKW = _hal.AcSensors.GENSET_GetActivePowerWatts(B_PHASE);
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_GEN_AC_SYSTEM_TYPE) == CFGZ::CFGZ_SPLIT_PHASE)
    {
        f32MinGenKW = _hal.AcSensors.GENSET_GetActivePowerWatts(R_PHASE);
       if(_hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE) < f32MinGenKW)
       {
           f32MinGenKW = _hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE);
       }

    }
    return (f32MinGenKW/1000);
}

void GCU_ALARMS::prvIsFuelTheftAlarm()
{
    A_SENSE::SENSOR_RET_t stFuel = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);
    if(_bUpdateFuelTheftCalc) //Will be set every hour.
    {
        _bUpdateFuelTheftCalc = false;

        //Monitor the fuel theft alarm only when Genset is Off
        if(_u8EngineOn == 0U)
        {
            if(stFuel.stValAndStatus.f32InstSensorVal < _f32FuelLevelOldValue)
            {
                if((((_f32FuelLevelOldValue - stFuel.stValAndStatus.f32InstSensorVal) *
                        _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_TANK_CAPACITY))/100)
                            > _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_THEFT_THRESH))
                {
                    _u8FuelTheftAlarm = 1;
                }
                else
                {
                    _u8FuelTheftAlarm = 0;
                }
            } else
            {
                _u8FuelTheftAlarm = 0;
            }
        }
        _f32FuelLevelOldValue = stFuel.stValAndStatus.f32InstSensorVal;
    }
}
void GCU_ALARMS::InitGCUAlarms()
{
    for(uint8_t u8Index = 0; u8Index < ALARM_LIST_LAST; u8Index++)
    {
        ArrAlarmMonitoring[u8Index].bEnableMonitoring = false;
        ArrAlarmMonitoring[u8Index].LocalEnable = &_u8DummyZero;
        ArrAlarmMonitoring[u8Index].bMonitoringPolarity = true;
        ArrAlarmMonitoring[u8Index].bEnableNotification = false;
        ArrAlarmMonitoring[u8Index].bEnableWarning = false;
        ArrAlarmMonitoring[u8Index].bEnableElectricTrip = false;
        ArrAlarmMonitoring[u8Index].bEnableShutdown = false;
        ArrAlarmMonitoring[u8Index].bResultInstant = false;
        ArrAlarmMonitoring[u8Index].bResultLatched = false;
        ArrAlarmMonitoring[u8Index].bNotificationLatched = false;
        ArrAlarmMonitoring[u8Index].bWarningLatched = false;
        ArrAlarmMonitoring[u8Index].bElectricTripLatched = false;
        ArrAlarmMonitoring[u8Index].bShutdownLatched = false;
        ArrAlarmMonitoring[u8Index].bAlarmActive = false;
        ArrAlarmMonitoring[u8Index].Threshold.f32Value = 65000;
        ArrAlarmMonitoring[u8Index].Threshold.u16Value = 65000;
        ArrAlarmMonitoring[u8Index].Threshold.u8Value = 255;
        ArrAlarmMonitoring[u8Index].u16Counter = 0;
        ArrAlarmMonitoring[u8Index].u16CounterMax = 0xFFFF;
    }
    _bOpAutoFuelTransfer = false;
    UTILS_ResetTimer(&_FuelSettlingTimer);
    UTILS_DisableTimer(&_AFTTimeoutAfterActTimer);
}

void GCU_ALARMS::prvMainsHighLowOutputs()
{
    if(ArrAlarmMonitoring[MAINS_R_UNDERVOLT_TRIP].bResultInstant && ArrAlarmMonitoring[MAINS_R_UNDERVOLT_TRIP].bResultLatched)
    {
        _hal.actuators.Activate(ACTUATOR::ACT_MAINS_LOW);
    }
    else if(ArrAlarmMonitoring[MAINS_R_UNDERVOLT_TRIP].bResultLatched && ArrAlarmMonitoring[MAINS_R_UNDERVOLT_RETURN].bResultInstant)
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_MAINS_LOW);
    }

    if(ArrAlarmMonitoring[MAINS_R_OVERVOLT_TRIP].bResultInstant && ArrAlarmMonitoring[MAINS_R_OVERVOLT_TRIP].bResultLatched)
    {
        _hal.actuators.Activate(ACTUATOR::ACT_MAINS_HIGH);
    }
    else if(ArrAlarmMonitoring[MAINS_R_OVERVOLT_TRIP].bResultLatched && ArrAlarmMonitoring[MAINS_R_OVERVOLT_RETURN].bResultInstant)
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_MAINS_HIGH);
    }
}

bool GCU_ALARMS::IsMainsSeqFail()
{
    if(_hal.AcSensors.MAINS_GetPhaseRotStatus() && ((!ArrAlarmMonitoring[MAINS_R_UNDERVOLT_RETURN].bEnableMonitoring || ArrAlarmMonitoring[MAINS_R_UNDERVOLT_RETURN].bResultInstant)
            && (!ArrAlarmMonitoring[MAINS_R_OVERVOLT_RETURN].bEnableMonitoring || ArrAlarmMonitoring[MAINS_R_OVERVOLT_RETURN].bResultInstant)
            && (!ArrAlarmMonitoring[MAINS_R_UNDERFREQ_RETURN].bEnableMonitoring || ArrAlarmMonitoring[MAINS_R_UNDERFREQ_RETURN].bResultInstant)
            && (!ArrAlarmMonitoring[MAINS_R_OVERFREQ_RETURN].bEnableMonitoring || ArrAlarmMonitoring[MAINS_R_OVERFREQ_RETURN].bResultInstant)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
void GCU_ALARMS::prvCoolantTempCtrlFunction(void)
{
    A_SENSE::SENSOR_RET_t stTemp = GetSelectedTempSensVal();

    if((((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
           &&(stTemp.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT))
            || ((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) > CFGZ::ENG_CONVENTIONAL)
                    &&(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG) == CFGZ::CFGZ_ENABLE)))&& (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_CTRL_EN) == CFGZ::CFGZ_ENABLE))
    {
           prvActDeactCLNTTempCtrlOutput();
    }
    else
    {
        _bCLNTTempCtrl = false;
    }

}

void GCU_ALARMS::prvActDeactCLNTTempCtrlOutput(void)
{
    A_SENSE::SENSOR_RET_t stTemp = GetSelectedTempSensVal();

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_CTRL) == CFGZ::CFGZ_HEATER_CONTROL)
    {
        if((int16_t)round(stTemp.stValAndStatus.f32InstSensorVal) <= (int16_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_ON_THRESH)))
        {
            _bCLNTTempCtrl = true;
        }
        else if((int16_t)round(stTemp.stValAndStatus.f32InstSensorVal) >= (int16_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_OFF_THRESH)))
        {
            _bCLNTTempCtrl = false;
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_CTRL) == CFGZ::CFGZ_COOLER_CONTROL)
    {
        if((int16_t)round(stTemp.stValAndStatus.f32InstSensorVal) >= (int16_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_ON_THRESH)))
        {
            _bCLNTTempCtrl = true;
        }
        else if((int16_t)round(stTemp.stValAndStatus.f32InstSensorVal) <= (int16_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_OFF_THRESH)))
        {
            _bCLNTTempCtrl = false;
        }
    }
}
void GCU_ALARMS::prvAutoFuelTransferFunction(void)
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MON_AFT_FROM_ENGINE_ON) == CFGZ::CFGZ_ENABLE)
    {
        if((_u8EngineOn==1) && (!(_bCommonShutdown || _bCommonElectricTrip)))
        {
            prvActDeactAFTOutput();
        }
        else
        {
            _u8AFTActivationTimeout = 0;
            _bOpAutoFuelTransfer = false;
        }
    }
    else if(!(_bCommonShutdown || _bCommonElectricTrip))
    {
        prvActDeactAFTOutput();
    }
    else
    {
        _u8AFTActivationTimeout = 0;
        _bOpAutoFuelTransfer = false;
    }
}


void GCU_ALARMS::prvActDeactAFTOutput(void)
{
    A_SENSE::SENSOR_RET_t  stFuel =_hal.AnalogSensors.GetSensorValue(
            AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);

    if(stFuel.stValAndStatus.f32InstSensorVal < (float)_cfgz.GetCFGZ_Param(CFGZ::ID_AFT_ACTIVATION_THRESH))
    {
        if((!_AFTTimeoutAfterActTimer.bEnabled) &&
                (!ArrAlarmMonitoring[AFT_ACTIVATION_TIMEOUT].bAlarmActive))
        {
            UTILS_ResetTimer(&_AFTTimeoutAfterActTimer);
        }
        else if(_AFTTimeoutAfterActTimer.bEnabled
                    && (UTILS_GetElapsedTimeInSec(&_AFTTimeoutAfterActTimer) >= _cfgz.GetCFGZ_Param(CFGZ::ID_TIMEOUT_AFTER_ACTIVATION))
                    && (_bOpAutoFuelTransfer))
        {
            _u8AFTActivationTimeout = 1;
            _bOpAutoFuelTransfer = false;
            UTILS_DisableTimer(&_AFTTimeoutAfterActTimer);
        }
        else{/* do nothing*/}

        if(!((bool)_u8AFTActivationTimeout))
        {
            _bOpAutoFuelTransfer = true;
        }
    }
    else if(_AFTTimeoutAfterActTimer.bEnabled
            && (UTILS_GetElapsedTimeInSec(&_AFTTimeoutAfterActTimer) >= _cfgz.GetCFGZ_Param(CFGZ::ID_TIMEOUT_AFTER_ACTIVATION))
            && (_bOpAutoFuelTransfer))
    {
        _u8AFTActivationTimeout = 1;
        _bOpAutoFuelTransfer = false;
        UTILS_DisableTimer(&_AFTTimeoutAfterActTimer);
    }
    else if(stFuel.stValAndStatus.f32InstSensorVal >= (float)_cfgz.GetCFGZ_Param(CFGZ::ID_AFT_DEACTIVATION_THRESH))
     {
        _u8AFTActivationTimeout = 0;
        _bOpAutoFuelTransfer = false;
        UTILS_DisableTimer(&_AFTTimeoutAfterActTimer);
    }
    else{/* do nothing*/}
}
void GCU_ALARMS::DisableAFTTimeoutAfterActTimer()
{
    UTILS_DisableTimer(&_AFTTimeoutAfterActTimer);
}


bool GCU_ALARMS::prvIsNeedToCheckSensFltAlarm()
{
    if(    START_STOP::IsFuelRelayOn()
        && (!IsAlarmActive(GCU_ALARMS::MPU_LOSS)
        &&  ENGINE_MONITORING::IsEngineCranked())
      )
    {
        return true;
    }

    return false;
}

void GCU_ALARMS::CheckMPULossAlarm()
{
    if(_hal.AnalogSensors.GetSpeedSensFltStatus())
    {
        if(prvIsNeedToCheckSensFltAlarm())
        {
           if(   (_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE) == CFGZ::CFGZ_MAGNETIC_PICKUP)
                     ||(_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE) == CFGZ::CFGZ_W_POINT_FREQ)
               )
           {
               _u8MPULossAlarm = 1;
           }
        }

    }
    else
    {
        _u8MPULossAlarm = 0;
    }
}
void ReadEventNumber(EEPROM::EVENTS_t evt)
{
    if(evt ==EEPROM::READ_COMPLETE)
    {
        GCU_ALARMS:: _bEventNumberReadDone =1;
    }
}

void ReadRollOver(EEPROM::EVENTS_t evt)
{
    if(evt ==EEPROM::READ_COMPLETE)
    {
        GCU_ALARMS:: _bRollOverReadDone =1;
    }
}

uint32_t GCU_ALARMS::GetCurrentEventNumber()
{
    uint32_t u32EventNumber=0;
    _hal.Objeeprom.BlockingRead(EXT_EEPROM_CURRENT_EVENT_NO_ADDRESS, (uint8_t*)&u32EventNumber, 4);
    if(u32EventNumber >(uint32_t)(CFGC::GetMaxNumberOfEvents()+1))
    {
        u32EventNumber =0;
    }
    return u32EventNumber;
}

bool GCU_ALARMS::GetEventRolloverBit()
{
    return (bool)(_u32RolledOverByte);
}

void EventWriteCB(EEPROM::EVENTS_t evt)
{
    if(evt ==EEPROM::WRITE_COMPLETE)
    {
        GCU_ALARMS::bEventWrittenSuccessfully = true;
    }
}

A_SENSE::SENSOR_RET_t GCU_ALARMS::GetLOPSensorVal()
{
    A_SENSE::SENSOR_RET_t stLOP = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED} ;
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_FROM_ENG)== CFGZ::CFGZ_ENABLE)
            &&(_cfgz.GetEngType()!=CFGZ::ENG_CONVENTIONAL)
            )
     {
        if(!gpJ1939->IsCommunicationFail())
        {

            stLOP.stValAndStatus.f32InstSensorVal = (float)gpJ1939->GetReadData(RX_PGN_EFL_P1_65263, 2) / 100.0f;
            if((isnan(stLOP.stValAndStatus.f32InstSensorVal) == true) || (isinf(stLOP.stValAndStatus.f32InstSensorVal) == true))
            {
                stLOP.stValAndStatus.f32InstSensorVal =0;
            }
        }
        else
        {
            stLOP.stValAndStatus.f32InstSensorVal =0;
        }
        stLOP.stValAndStatus.eState = ANLG_IP:: BSP_STATE_NORMAL;
        stLOP.eStatus = A_SENSE::SENSOR_READ_SUCCESS;
     }
     else  if(_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
         stLOP = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_S3_SENS_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
    {
        stLOP =_hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE_4_20);
    }
    return stLOP;
}



void GCU_ALARMS::ActivateHighOilPressAlarmSens()
{
  _u8HighOilPressDetectedAlarm = 1;

}


void GCU_ALARMS::TurnOffSounderAlarm()
{
    _bOPSounderAlarm = false;
}


bool GCU_ALARMS::IsCommonAlarm()
{
    return _bCommonAlarm;
}

A_SENSE::SENSOR_RET_t GCU_ALARMS::GetSelectedTempSensVal()
{
    A_SENSE::SENSOR_RET_t sensVal;

    if((_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG)== CFGZ::CFGZ_ENABLE)
            &&(_cfgz.GetEngType()!=CFGZ::ENG_CONVENTIONAL)
            )
    {
        if(!gpJ1939->IsCommunicationFail())
        {
            sensVal.stValAndStatus.f32InstSensorVal = gpJ1939->GetReadData(RX_PGN_ET1_65262, 0);
            if((isnan(sensVal.stValAndStatus.f32InstSensorVal) == true) || (isinf(sensVal.stValAndStatus.f32InstSensorVal) == true))
            {
                 sensVal.stValAndStatus.f32InstSensorVal =0;
            }
        }
        else
        {
            sensVal.stValAndStatus.f32InstSensorVal =0;
        }
        sensVal.stValAndStatus.eState = ANLG_IP:: BSP_STATE_NORMAL;
        sensVal.eStatus = A_SENSE::SENSOR_READ_SUCCESS;
        return sensVal;
    }

    return _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_ENG_TEMPERATURE);
}


float GCU_ALARMS::GetSelectedBatteryVtg()
{
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_VOLT_FROM_ENG)== CFGZ::CFGZ_ENABLE)
            &&(_cfgz.GetEngType()!=CFGZ::ENG_CONVENTIONAL)
            )
    {
        if(!gpJ1939->IsCommunicationFail())
        {
            if((isnan(gpJ1939->GetReadData(RX_PGN_VEP1_65271, 0)) == true) || (isinf(gpJ1939->GetReadData(RX_PGN_VEP1_65271, 0)) == true))
            {
                 return 0;
            }
            return (float)gpJ1939->GetReadData(RX_PGN_VEP1_65271, 0);
        }
        else
        {
            return 0;
        }

    }
    return _hal.AnalogSensors.GetFilteredVbattVolts();
}

uint32_t GCU_ALARMS::GetSelectedEngRunMin()
{
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_RUNNING_HOURS_FROM_ENG)== CFGZ::CFGZ_ENABLE)
            &&(_cfgz.GetEngType()!=CFGZ::ENG_CONVENTIONAL)
           )
    {
        if(!gpJ1939->IsCommunicationFail())
        {
            if((isnan(gpJ1939->GetReadData(RX_PGN_HOURS_65253, 0) * 60) == true) || (isinf(gpJ1939->GetReadData(RX_PGN_HOURS_65253, 0) * 60) == true))
            {
                return 0;
            }
             return (uint32_t)(gpJ1939->GetReadData(RX_PGN_HOURS_65253, 0) * 60);
        }
        else
        {
            return 0;
        }
    }
    return ( ENGINE_MONITORING::GetEngineRunTimeMin());
}

bool GCU_ALARMS::IsBTSBattHealthy(void)
{
    return _bBTSBattHealthy;
}
bool GCU_ALARMS::IsShelterTempHigh(void)
{
    return _bHighShelterTemp;
}

bool GCU_ALARMS::IsMainsFeedbackAvailable()
{
    if(IsAlarmMonEnabled(GCU_ALARMS::MAINS_CONTACTOR_LATCHED))
    {
        return AlarmResultInstat(GCU_ALARMS::MAINS_CONTACTOR_LATCHED);
    }
    else
    {
        return true;
    }

}

bool GCU_ALARMS::IsGenFeedbackAvailable()
{
    if(IsAlarmMonEnabled(GCU_ALARMS::GEN_CONTACTOR_LATCHED))
    {
        return AlarmResultInstat(GCU_ALARMS::GEN_CONTACTOR_LATCHED);
    }
    else
    {
        return true;
    }
}
