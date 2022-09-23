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
_bCLNTTempCtrl(false),
_bBTSBattHealthy(false),
_bHighShelterTemp(false),
_bLowShelterTemp(false),
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
#if TEST_ALARM
    UTILS_ResetTimer(&_AlaramtestTimer);
#endif

}

void GCU_ALARMS::Update(bool bDeviceInConfigMode)
{
    static bool bAlarmUpdate = false;
    A_SENSE::SENSOR_RET_t stLOP = GetLOPSensorVal();
    A_SENSE::SENSOR_RET_t stFuel = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_FUEL_LEVEL_RESISTIVE);
    A_SENSE::SENSOR_RET_t stTemp = GetSelectedTempSensVal();

    _u8LopSensMon = (uint8_t)(_u8MonOn && (((stLOP.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT)&&(stLOP.stValAndStatus.eState != ANLG_IP::BSP_STATE_SHORT_TO_BAT))
                                             ));
    _u8FuelSensMon = (uint8_t)(stFuel.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT);
    A_SENSE::SENSOR_RET_t stAuxSensS1 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S1_SENSOR);
    A_SENSE::SENSOR_RET_t stAuxSensS2 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S2_SENSOR);
    A_SENSE::SENSOR_RET_t stAuxSensS3 = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED};
    A_SENSE::SENSOR_RET_t stAuxSensS4 = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED};

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        stAuxSensS3 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S3_4_20_SENSOR);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        stAuxSensS3 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S3_0_5_SENSOR);
    }

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        stAuxSensS4 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S4_4_20_SENSOR);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        stAuxSensS4 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S4_0_5_SENSOR);
    }

    _u8AuxSensS1 = (uint8_t)(stAuxSensS1.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT);
    _u8AuxSensS2 = (uint8_t)(stAuxSensS2.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT);
    _u8AuxSensS3 = (uint8_t)((stAuxSensS3.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT)&&(stAuxSensS3.stValAndStatus.eState != ANLG_IP::BSP_STATE_SHORT_TO_BAT));
    _u8AuxSensS4 = (uint8_t)((stAuxSensS4.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT)&&(stAuxSensS4.stValAndStatus.eState != ANLG_IP::BSP_STATE_SHORT_TO_BAT));
    _u8TempSensMon = (uint8_t)(_u8MonOn && ((stTemp.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT)));
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

                prvCoolantTempCtrlFunction();
                prvMainsHighLowOutputs();
#if TEST_ALARM
                prvTestAlarm();
#endif
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

void GCU_ALARMS::prvSetAlarmAction_NoWESN(uint8_t u8AlarmIndex, uint8_t u8AlarmAction)
{
    prvClearAllAction(u8AlarmIndex);
    if(u8AlarmAction == CFGZ::CFGZ_ACTION_NOTIFICATION_NoWESN)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
    }
    else if(u8AlarmAction == CFGZ::CFGZ_ACTION_WARNING_NoWESN)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
    }
    else if(u8AlarmAction == CFGZ::CFGZ_ACTION_ELECTRICAL_TRIP_NoWESN)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableElectricTrip = true;
    }
    else if(u8AlarmAction == CFGZ::CFGZ_ACTION_SHUTDOWN_NoWESN)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
    }

}
void GCU_ALARMS::prvSetAlarmAction_NoWS(uint8_t u8AlarmIndex, uint8_t u8AlarmAction)
{
    prvClearAllAction(u8AlarmIndex);

    if(u8AlarmAction == CFGZ::CFGZ_ACTION_WARNING_NoWS)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
    }
    else if(u8AlarmAction == CFGZ::CFGZ_ACTION_SHUTDOWN_NoWS)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
    }
}
void GCU_ALARMS::prvSetAlarmAction_WS(uint8_t u8AlarmIndex, uint8_t u8AlarmAction)
{
    prvClearAllAction(u8AlarmIndex);

    if(u8AlarmAction == CFGZ::CFGZ_ACTION_WARNING_WS)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
    }
    else if(u8AlarmAction == CFGZ::CFGZ_ACTION_SHUTDOWN_WS)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
    }
}
void GCU_ALARMS::prvSetAlarmAction_NW(uint8_t u8AlarmIndex, uint8_t u8AlarmAction)
{
    prvClearAllAction(u8AlarmIndex);

    if(u8AlarmAction == CFGZ::CFGZ_ACTION_NOTIFICATION_NW)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
    }
    else if(u8AlarmAction == CFGZ::CFGZ_ACTION_WARNING_NW)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
    }
}
void GCU_ALARMS::prvSetAlarmAction_NoN(uint8_t u8AlarmIndex, uint8_t u8AlarmAction)
{
    prvClearAllAction(u8AlarmIndex);
    if(u8AlarmAction == CFGZ::CFGZ_ACTION_NOTIFICATION_NoN)
    {
        ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = true;
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
    ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
    ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
    ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
}

void GCU_ALARMS::prvAssignInputSettings(uint8_t u8InputIndex, uint8_t u8InputSource,
                                        uint8_t u8Activation, uint8_t u8ActivationDelay,
                                        uint8_t u8AlarmAction, uint8_t u8LoggingID)
{
    if((u8InputIndex >= DIG_IN_A )&&(u8InputIndex <= DIG_IN_P )
               &&(u8InputSource != CFGZ:: CFGZ_USER_CONFIGURED_SENSOR))
       {
             ArrAlarmMonitoring[u8InputIndex].bEnableMonitoring = true;
             ArrAlarmMonitoring[u8InputIndex].u8LoggingID = NoAlarm_id;
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
            prvSetAlarmAction_NoWESN(u8InputIndex, u8AlarmAction);
            prvSetAlarmActivation(u8InputIndex, u8Activation);
            ArrAlarmMonitoring[u8InputIndex].Threshold.f32Value = 0;
            ArrAlarmMonitoring[u8InputIndex].Threshold.u16Value = 0;
            ArrAlarmMonitoring[u8InputIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            break;

        case CFGZ::CFGZ_LOW_FUEL_LEVEL_SWITCH:
            ArrAlarmMonitoring[LFL_SWITCH].bEnableMonitoring = true;
            prvSetAlarmAction_NoWESN(LFL_SWITCH, u8AlarmAction);
            prvSetAlarmActivation(LFL_SWITCH, CFGZ::CFGZ_ALWAYS);
            ArrAlarmMonitoring[LFL_SWITCH].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[LOW_FUEL_LVL_SWITCH_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;

            ArrAlarmMonitoring[LFL_SWITCH].u8LoggingID = GCU_ALARMS::fuel_level_id;
            ArrAlarmMonitoring[LFL_SWITCH].pValue = &_ArrAlarmValue[LOW_FUEL_LVL_SWITCH_STATUS];

            break;

        case CFGZ::CFGZ_LOW_LUBE_OIL_PRESS_SWITCH:

            ArrAlarmMonitoring[LLOP_SWITCH].bEnableMonitoring = true;
            prvSetAlarmAction_NoWESN(LLOP_SWITCH, u8AlarmAction);
            prvSetAlarmActivation(LLOP_SWITCH, CFGZ::CFGZ_FROM_MONITORING_ON);
            ArrAlarmMonitoring[LLOP_SWITCH].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[LLOP_SWITCH].u8LoggingID = GCU_ALARMS::Low_Oil_Pressure_id;
            ArrAlarmMonitoring[LLOP_SWITCH].pValue = &_ArrAlarmValue[LLOP_SWITCH_STATUS];

            ArrAlarmMonitoring[LLOP_MON].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_MON_LLOP_BEFORE_CRANK) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[LLOP_MON].LocalEnable = &_u8EngineOff ;
            ArrAlarmMonitoring[LLOP_MON].bMonitoringPolarity = false;
            ArrAlarmMonitoring[LLOP_MON].Threshold.u8Value = 1;
            ArrAlarmMonitoring[LLOP_MON].ThreshDataType = ONE_BYTE_INT;
            ArrAlarmMonitoring[LLOP_MON].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
            ArrAlarmMonitoring[LLOP_MON].u8LoggingID = GCU_ALARMS::NoAlarm_id;
            ArrAlarmMonitoring[LLOP_MON].pValue = &_ArrAlarmValue[LLOP_SWITCH_STATUS];

            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[LLOP_SWITCH_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8MonOn;

            break;

        case CFGZ::CFGZ_HIGH_ENGINE_TEMP_SWITCH:

            ArrAlarmMonitoring[HWT_SWITCH].bEnableMonitoring = true;
            prvSetAlarmAction_NoWESN(HWT_SWITCH, u8AlarmAction);
            prvSetAlarmActivation(HWT_SWITCH, CFGZ::CFGZ_FROM_MONITORING_ON);
            ArrAlarmMonitoring[HWT_SWITCH].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[HWT_SWITCH_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8MonOn;


            ArrAlarmMonitoring[HWT_SWITCH].u8LoggingID = GCU_ALARMS::High_Water_Temperature_id;
            ArrAlarmMonitoring[HWT_SWITCH].pValue = &_ArrAlarmValue[HWT_SWITCH_STATUS];

            break;

        case CFGZ::CFGZ_LOW_WATER_LVL_SWITCH:
            ArrAlarmMonitoring[RWL_SWITCH].bEnableMonitoring = true;
            prvSetAlarmAction_NoWESN(RWL_SWITCH, u8AlarmAction);
            prvSetAlarmActivation(RWL_SWITCH, u8Activation);
            ArrAlarmMonitoring[RWL_SWITCH].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[WATER_LEVEL_SWITCH_STATUS];
            prvSetAlarmActivation(u8InputIndex, u8Activation);

            ArrAlarmMonitoring[RWL_SWITCH].u8LoggingID = GCU_ALARMS::Radiator_Water_Level_id;
            ArrAlarmMonitoring[RWL_SWITCH].pValue = &_ArrAlarmValue[WATER_LEVEL_SWITCH_STATUS];

             break;

        case CFGZ::CFGZ_EMERGENCY_STOP:
            ArrAlarmMonitoring[ESTOP].bEnableMonitoring = true;
            ArrAlarmMonitoring[ESTOP].bEnableShutdown = true;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[EMERGENCY_STOP_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;

            prvUpdateMonParams(ESTOP, &_u8DummyOne, true, GCU_ALARMS::Emergency_Stop_id , (uint8_t)0, 2U);
            ArrAlarmMonitoring[ESTOP].pValue = &_ArrAlarmValue[EMERGENCY_STOP_STATUS];

            break;

        case CFGZ::CFGZ_REMOTE_START_STOP:
            ArrAlarmMonitoring[REMOTE_SS].bEnableMonitoring = true;
            prvSetAlarmActivation(REMOTE_SS, CFGZ::CFGZ_ALWAYS);
            ArrAlarmMonitoring[REMOTE_SS].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[REMOTE_SS_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;

            ArrAlarmMonitoring[REMOTE_SS].pValue = &_ArrAlarmValue[REMOTE_SS_STATUS];

            break;

        case CFGZ::CFGZ_SIMULATE_START:
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[SIM_START_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_SIMULATE_STOP:
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[SIM_STOP_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_SIMULATE_AUTO:
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[SIM_AUTO_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_CLOSE_GEN_OPEN_MAINS_SWITCH:
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[CLOSE_GEN_OPEN_MAINS_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_CLOSE_MAINS_OPEN_GEN_SWITCH:
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[CLOSE_MAINS_OPEN_GEN_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_SIMULATE_MAINS:
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[SIM_MAINS_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_VBEL_BROKEN_SWITCH:
            ArrAlarmMonitoring[V_BELT_BROKEN_SWITCH].bEnableMonitoring = true;
            prvSetAlarmAction_NoWESN(V_BELT_BROKEN_SWITCH, u8AlarmAction);
            prvSetAlarmActivation(V_BELT_BROKEN_SWITCH, u8Activation);
            ArrAlarmMonitoring[V_BELT_BROKEN_SWITCH].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
             ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[V_BELT_BROKEN_SWITCH_STATUS];
             prvSetAlarmActivation(u8InputIndex, u8Activation);

             ArrAlarmMonitoring[V_BELT_BROKEN_SWITCH].u8LoggingID = GCU_ALARMS::V_Belt_Broken_id;
             ArrAlarmMonitoring[V_BELT_BROKEN_SWITCH].pValue = &_ArrAlarmValue[V_BELT_BROKEN_SWITCH_STATUS];

            break;

        case CFGZ::CFGZ_MAINS_CONTACTOR_LATCHED:
            ArrAlarmMonitoring[MAINS_CONTACTOR_LATCHED].bEnableMonitoring = true;
            prvSetAlarmActivation(MAINS_CONTACTOR_LATCHED, CFGZ::CFGZ_ALWAYS);
            ArrAlarmMonitoring[MAINS_CONTACTOR_LATCHED].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[MAINS_CONTACTOR_LATCHED].u8LoggingID = GCU_ALARMS::NoAlarm_id;
            ArrAlarmMonitoring[MAINS_CONTACTOR_LATCHED].pValue = &_ArrAlarmValue[MAINS_CONT_LATCHED_STATUS];

            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[MAINS_CONT_LATCHED_STATUS ];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_GEN_CONTACTOR_LATCHED:
            ArrAlarmMonitoring[DG_CONTACTOR_LATCHED].bEnableMonitoring = true;
            prvSetAlarmActivation(DG_CONTACTOR_LATCHED, CFGZ::CFGZ_ALWAYS);
            ArrAlarmMonitoring[DG_CONTACTOR_LATCHED].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[DG_CONTACTOR_LATCHED].u8LoggingID = GCU_ALARMS::NoAlarm_id;
            ArrAlarmMonitoring[DG_CONTACTOR_LATCHED].pValue = &_ArrAlarmValue[GEN_CONT_LATCHED_STATUS];

            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[GEN_CONT_LATCHED_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;

        case CFGZ::CFGZ_BATTERY_CHARGER_FAIL:
            ArrAlarmMonitoring[BATT_CHG_FAIL].bEnableMonitoring = true;
            prvSetAlarmAction_NoWESN(BATT_CHG_FAIL, u8AlarmAction);
            prvSetAlarmActivation(BATT_CHG_FAIL, u8Activation);
            ArrAlarmMonitoring[BATT_CHG_FAIL].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[BATT_CHG_FAIL_STATUS ];
            prvSetAlarmActivation(u8InputIndex, u8Activation);


            ArrAlarmMonitoring[BATT_CHG_FAIL].u8LoggingID = Batt_Chg_Fail_id;
            ArrAlarmMonitoring[BATT_CHG_FAIL].pValue = &_ArrAlarmValue[BATT_CHG_FAIL_STATUS ];

            break;

        case CFGZ::CFGZ_SMOKE_FIRE:
            ArrAlarmMonitoring[SMOKE_FIRE].bEnableMonitoring = true;
            prvSetAlarmAction_NoWESN(SMOKE_FIRE, u8AlarmAction);
            prvSetAlarmActivation(SMOKE_FIRE, u8Activation);
            ArrAlarmMonitoring[SMOKE_FIRE].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[SMOKE_FIRE_STATUS];
            prvSetAlarmActivation(u8InputIndex, u8Activation);


            ArrAlarmMonitoring[SMOKE_FIRE].u8LoggingID = Smoke_Fire_id;
            ArrAlarmMonitoring[SMOKE_FIRE].pValue = &_ArrAlarmValue[SMOKE_FIRE_STATUS ];

            break;
        case CFGZ:: CFGZ_MODE_SELECT:
            ArrAlarmMonitoring[MODE_SELECT].bEnableMonitoring = true;
            prvSetAlarmActivation(MODE_SELECT, CFGZ::CFGZ_ALWAYS);
            ArrAlarmMonitoring[MODE_SELECT].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[MODE_SELECT].u8LoggingID = GCU_ALARMS::NoAlarm_id;
            ArrAlarmMonitoring[MODE_SELECT].pValue = &_ArrAlarmValue[MODE_SELECT_STATUS];

            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[MODE_SELECT_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;
        case CFGZ:: CFGZ_AMB_TEMP_SELECT:
            ArrAlarmMonitoring[AMB_TEMP_SWITCH].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_AMB_TEMPERATURE) == CFGZ::CFGZ_ENABLE);
            prvSetAlarmActivation(AMB_TEMP_SWITCH, CFGZ::CFGZ_ALWAYS);
            ArrAlarmMonitoring[AMB_TEMP_SWITCH].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*u8ActivationDelay;
            ArrAlarmMonitoring[AMB_TEMP_SWITCH].u8LoggingID = GCU_ALARMS::NoAlarm_id;
            ArrAlarmMonitoring[AMB_TEMP_SWITCH].pValue = &_ArrAlarmValue[MODE_SELECT_STATUS];

            ArrAlarmMonitoring[u8InputIndex].pValue = &_ArrAlarmValue[AMB_TEMP_SELECT_STATUS];
            ArrAlarmMonitoring[u8InputIndex].LocalEnable = &_u8DummyOne;
            break;
    }
}

void GCU_ALARMS::ConfigureGCUAlarms(uint8_t u8AlarmIndex)
{
    /*
     * SuryaPranayTeja.Bvv : 14-09-2022
     * Todo: The Threshold.values and the ThreshDataTypes need to be reviewed at the end before giving the build.
     * Currently the data types may not match as the CFGZ data types are not yet freezed.
     */
    bool benableMon=false;

    switch(u8AlarmIndex)
    {
        case LOW_OIL_PRESS_SHUTDOWN:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Low_Oil_Pressure_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            else if((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                    ||    (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2))
            {

                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8LowOilPressAlarm;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Low_Oil_Pressure_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LUBE_OIL_PRESSURE];
            break;
        case LOW_OIL_PRESS_WARNING:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_WARNING) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_WARNING) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Low_Oil_Pressure_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_WARNING_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            else if((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
                               ||    (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2))
            {

                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_WARNING) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_WARNING) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8LowOilPressAlarm;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Low_Oil_Pressure_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_WARNING_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LUBE_OIL_PRESSURE];
            break;
        case LOW_FUEL_LEVEL_SHUTDOWN:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = fuel_level_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            else if((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8LowFuelLevelAlarm;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = fuel_level_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FUEL_LEVEL];
            break;
        case LOW_FUEL_LEVEL_NOTIFICATION:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_NOTIFICATION) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = (_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_NOTIFICATION) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = fuel_level_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_NOTIFICATION_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            else if((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_NOTIFICATION) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_NOTIFICATION) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8LowFuelLevelAlarm;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = fuel_level_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_NOTIFICATION_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FUEL_LEVEL];
            break;

        case HIGH_WATER_TEMP:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_ACTION) != CFGZ::CFGZ_ACTION_NONE_SENS);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_ACTION) == CFGZ::CFGZ_ACTION_SHUTDOWN_SENS);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = (_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_ACTION) == CFGZ::CFGZ_ACTION_WARNING_SENS);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = High_Water_Temperature_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENGINE_TEMPERATURE];
            break;
        case OVERSPEED_L1:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Over_Speed_l1_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_OVER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_OVER_SPEED_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENGINE_SPEED];
            break;
        case OVERSPEED_L2:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Over_Speed_l2_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_GROSS_OS_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * 0.2;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENGINE_SPEED];
            break;
        case UNDERSPEED:
             {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Under_Speed_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
             ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENGINE_SPEED];
            break;
        case OVERFREQ_SHUTDOWN:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring =((_cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OVER_FREQ_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE)) ;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OVER_FREQ_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Over_Frequency_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OF_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC*_cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GENSET_FREQUENCY];
            break;
        case UNDERFREQ_SHUTDOWN:
             {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UNDER_FREQ_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Under_Frequency_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UF_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
             ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GENSET_FREQUENCY];
            break;
        case DG_R_OV_SHUTDOWN:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = R_Over_Voltage_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_R_PHASE_VOLTAGE];
            break;
        case DG_R_UV_SHUTDOWN:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = R_Under_Voltage_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_R_PHASE_VOLTAGE];
            break;
        case DG_Y_OV_SHUTDOWN:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Y_Over_Voltage_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_Y_PHASE_VOLTAGE];
            break;
        case DG_Y_UV_SHUTDOWN:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Y_Under_Voltage_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_Y_PHASE_VOLTAGE];
            break;
        case DG_B_OV_SHUTDOWN:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = B_Over_Voltage_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OV_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_B_PHASE_VOLTAGE];
            break;
        case DG_B_UV_SHUTDOWN:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_SHUTDOWN) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = B_Under_Voltage_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UV_SHUTDOWN_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_B_PHASE_VOLTAGE];
            break;
        case MAINS_UNDERVOLT_TRIP:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (false);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_TRIP);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_MAINS_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_MIN_PHASE_VOLTAGE];
            break;
        case MAINS_OVERVOLT_TRIP:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_OVER_VOLT_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (false);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_OVER_VOLT_MON_TRIP);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_MAINS_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_MAX_PHASE_VOLTAGE];
            break;
        case MAINS_UNDERVOLT_RETURN:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (false);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_RETURN);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_MAINS_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_MIN_PHASE_VOLTAGE];
            break;
        case MAINS_OVERVOLT_RETURN:
             {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_OVER_VOLT_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (false);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_OVER_VOLT_MON_RETURN);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_MAINS_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
             ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_MAX_PHASE_VOLTAGE];
            break;
        case MAINS_UNDERFREQ_TRIP:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_FREQ_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (false);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_FREQ_MON_TRIP);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_MAINS_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_MIN_PHASE_FREQ];
            break;
        case MAINS_OVERFREQ_TRIP:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_OVER_FREQ_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (false);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_OVER_FREQ_MON_TRIP);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_MAINS_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_MAX_PHASE_FREQ];
            break;
        case MAINS_UNDERFREQ_RETURN:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_FREQ_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (false);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_FREQ_MON_RETURN);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_MAINS_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_MIN_PHASE_FREQ];
            break;
        case MAINS_OVERFREQ_RETURN:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_OVER_FREQ_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (false);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_OVER_FREQ_MON_RETURN);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_MAINS_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_MAX_PHASE_FREQ];
            break;
        case CA_UV:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_ACTION) != CFGZ::CFGZ_ACTION_NONE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (false);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Charge_Fail_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[CHARG_ALT_VOLTAGE];
            break;
        case CA_FAIL:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_ACTION) != CFGZ::CFGZ_ACTION_NONE);
                prvSetAlarmAction_NoWESN(u8AlarmIndex,CFGZ::ID_CHARGE_ALT_MON_FAIL_ACTION );
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Charge_Fail_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _u8DummyZero ;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC / NO_OF_50MSEC_TICKS_FOR_1SEC; //Need immediate Alarm
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[CHARG_ALT_ERROR];
            break;
        case VBAT_OV:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_HIGH_VOLT_ACTION) != CFGZ::CFGZ_ACTION_NONE);
                prvSetAlarmAction_NoWESN(u8AlarmIndex,CFGZ::ID_BATTERY_MONITOR_HIGH_VOLT_ACTION );
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Battery_Over_Voltage_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_HIGH_VOLT_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_HIGH_VOLT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[BATTERY_VOLTAGE];
            break;
        case VBAT_UV:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_LOW_VOLT_ACTION) != CFGZ::CFGZ_ACTION_NONE);
                prvSetAlarmAction_NoWESN(u8AlarmIndex,CFGZ::ID_BATTERY_MONITOR_LOW_VOLT_ACTION );
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Battery_Under_Voltage_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_LOW_VOLT_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_LOW_VOLT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[BATTERY_VOLTAGE];
            break;
        case VBTS_UV:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_BATTERY_MON) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (false);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_BTS_CONFIG_LOW_BATT_MON_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[BTS_VOLTAGE];
            break;
        case OVERCURRENT:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_OVER_CURR_ACTION) != CFGZ::CFGZ_ACTION_NONE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                prvSetAlarmAction_NoWESN(u8AlarmIndex,CFGZ::ID_CURRENT_MONITOR_OVER_CURR_ACTION );
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Over_Current_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_OVER_CURR_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_CURRENT_MONITOR_OVER_CURR_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_MAX_CURRENT];
            break;
        case FILT_MAINTENANCE:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                prvSetAlarmAction_NW(u8AlarmIndex,CFGZ::ID_MAINT_ALARM_ACTION );
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Filter_maintenance_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_MAINT_ALARM_DUE_AT_ENGINE_HOURS);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC * 5;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENG_RUN_HOURS];
            break;
        case OVERLOAD:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_OVERLOAD_ACTION) != CFGZ::CFGZ_ACTION_NONE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                prvSetAlarmAction_NoWESN(u8AlarmIndex,CFGZ::ID_LOAD_MONITOR_OVERLOAD_ACTION);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Over_Load_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_OVERLOAD_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_OVERLOAD_MON_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[TOTAL_KW_PERCENT];
        break;
        case FUEL_THEFT:
        {
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
            {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_THEFT_WARNING) == CFGZ::CFGZ_ENABLE);
            }
            else if(_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_FUEL_THEFT_WARNING) == CFGZ::CFGZ_ENABLE);
            }
            else
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = false;
            }
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Fuel_Theft_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FUEL_THEFT_ALARM];
            break;
        case DG_PHASE_ROTATION:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_PHASE_REVERSAL_DETECT) == CFGZ::CFGZ_ENABLE) &&(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                prvSetAlarmAction_NoWESN(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_PHASE_REVERSAL_ACTION));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8GenReady;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Phase_DG_Rotation_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 1 ;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_PHASE_ROTATION_STATUS];
            break;
        case EB_PHASE_ROTATION:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_PHASE_REVERSAL_DETECT) == CFGZ::CFGZ_ENABLE) &&(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                prvSetAlarmAction_NoN(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_PHASE_REVERSAL_ACTION));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Phase_EB_Rotation_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 1;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_PHASE_ROTATION_STATUS];
            break;
        case MPU_LOSS:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = MPU_Loss_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 1;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MPU_LOSS_STATUS];
            break;
        case OPEN_LOP_CURR_SENS_CKT:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1);
                prvSetAlarmAction_WS(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Lop_Curr_Sens_Open_Ckt_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LOP_CURRENT_OPEN_CKT];
            break;
        case OPEN_LOP_SENS_CKT:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2);
                prvSetAlarmAction_NoWS(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_CIRCUIT_FAULT_ACTION));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Lop_Sen_Ckt_Open_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = 0;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[LOP_RES_OPEN_CKT];
            break;
        case OPEN_ENG_TEMP_CKT:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Engine_Temperature_Ckt_Open_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = 0;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ENG_TEMP_OPEN_CKT];
            break;
        case DG_R_OV_WARNING:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = R_Over_Voltage_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u16Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OV_WARNING_THRESHOLD);
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = TWO_BYTE_INT;
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_R_PHASE_VOLTAGE];
        break;
        case DG_R_UV_WARNING:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = R_Under_Voltage_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u16Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UV_WARNING_THRESHOLD);
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = TWO_BYTE_INT;
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_R_PHASE_VOLTAGE];
        break;
        case DG_Y_OV_WARNING:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Y_Over_Voltage_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u16Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OV_WARNING_THRESHOLD);
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = TWO_BYTE_INT;
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_Y_PHASE_VOLTAGE];
        break;
        case DG_Y_UV_WARNING:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Y_Under_Voltage_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u16Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UV_WARNING_THRESHOLD);
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = TWO_BYTE_INT;
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_Y_PHASE_VOLTAGE];
        break;
        case DG_B_OV_WARNING:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OVER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = B_Over_Voltage_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u16Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_OV_WARNING_THRESHOLD);
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = TWO_BYTE_INT;
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_B_PHASE_VOLTAGE];
        break;
        case DG_B_UV_WARNING:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UNDER_VOLT_WARNING) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = B_Under_Voltage_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u16Value = _cfgz.GetCFGZ_Param(CFGZ::ID_VOLT_MONITOR_UV_WARNING_THRESHOLD);
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = TWO_BYTE_INT;
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_B_PHASE_VOLTAGE];
        break;
        case OVERFREQ_WARNING:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OVER_FREQ_WARNING) == CFGZ::CFGZ_ENABLE)&& (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OVER_FREQ_WARNING) == CFGZ::CFGZ_ENABLE)&& (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Over_Frequency_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_OF_WARNING_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GENSET_FREQUENCY];
            break;
        case UNDERFREQ_WARNING:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UNDER_FREQ_WARNING) == CFGZ::CFGZ_ENABLE)&& (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = ((_cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UNDER_FREQ_WARNING) == CFGZ::CFGZ_ENABLE)&& (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Under_Frequency_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_FREQ_MONITOR_UF_WARNING_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_ALT_DETECT_DELAY);
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GENSET_FREQUENCY];
            break;
        case LOP_CURR_SENS_OVER_CURR:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = LOP_Curr_Sen_Over_Curr_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[PIN23_SENSOR_CURRENT_VAL];
            break;
        case OPEN_AN_SEN_S1_CKT:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Aux_S1_Ckt_Open_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[AUX_SENS_S1_OPEN_CKT];
            break;
        case OPEN_AN_SEN_S2_CKT:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Aux_S2_Ckt_Open_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[AUX_SENS_S2_OPEN_CKT];
            break;
        case AN_SEN_S2_MON:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Aux_S2_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[ANLG_SENS_S2_VAL];
            break;
        case OIL_PRESS_MON:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[HIGH_OIL_PRESSURE];
            break;
        case SHELTER_TEMP_START_DG:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = High_Shelter_temp_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[SHELTER_TEMP_VAL];
            break;
        case SHELTER_TEMP_STOP_DG:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[SHELTER_TEMP_VAL];
            break;
        case MAINS_LL_UNDERVOLT_TRIP:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_PH_PH_MIN_VOLTAGE];
            break;
        case MAINS_LL_UNDERVOLT_RETURN:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = ((_cfgz.GetCFGZ_Param(CFGZ::ID_UNDER_VOLT_MON_ENABLE) == CFGZ::CFGZ_ENABLE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = NoAlarm_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINS_PH_PH_MIN_VOLTAGE];
            break;
        case FAN_HIGH_CURR:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Fan_High_Current_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[EARTH_LEAKAGE_CURR_VAL];
            break;
        case FAN_LOW_CURR:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Fan_Low_Current_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[EARTH_LEAKAGE_CURR_VAL];
            break;
        case LOP_SENS_SHORT_TO_BATTERY:
            if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1))
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = false;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Lop_Short_To_Battery_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
            }
            break;
        case DIG_IN_A:
        {
            prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTION), GCU_ALARMS::Auxilary_Input_A_id);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_A];
                prvSetAlarmAction_NoWESN(DIG_IN_A, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_A_ACTION));
            }
        }
        break;
        case DIG_IN_B:
        {
            prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTION), GCU_ALARMS::Auxilary_Input_B_id);
            if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
            {
                ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_B];
                prvSetAlarmAction_NoWESN(DIG_IN_B, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_B_ACTION));
            }
        }
        break;
        case DIG_IN_C:
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTION), GCU_ALARMS::Auxilary_Input_C_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_C];
                    prvSetAlarmAction_NoWESN(DIG_IN_C, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_C_ACTION));
                }
            }
            break;
        case DIG_IN_D:
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTION), GCU_ALARMS::Auxilary_Input_D_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_D];
                    prvSetAlarmAction_NoWESN(DIG_IN_D, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_D_ACTION));
                }
            }
            break;
        case DIG_IN_E:
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTION), GCU_ALARMS::Auxilary_Input_E_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_E];
                    prvSetAlarmAction_NoWESN(DIG_IN_E, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_E_ACTION));
                }
            }
            break;
        case DIG_IN_F:
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTION), GCU_ALARMS::Auxilary_Input_F_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_F];
                    prvSetAlarmAction_NoWESN(DIG_IN_F, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_F_ACTION));
                }
            }
            break;
        case DIG_IN_G:
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTION), GCU_ALARMS::Auxilary_Input_G_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_G];
                    prvSetAlarmAction_NoWESN(DIG_IN_G, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_G_ACTION));
                }
            }
            break;
        case DIG_IN_H:
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTION), GCU_ALARMS::Auxilary_Input_H_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_H];
                    prvSetAlarmAction_NoWESN(DIG_IN_H, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_H_ACTION));
                }
            }
            break;
        case DIG_IN_I:
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTION), GCU_ALARMS::Auxilary_Input_I_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_I];
                    prvSetAlarmAction_NoWESN(DIG_IN_I, _cfgz.GetCFGZ_Param(CFGZ::ID_DIG_IN_I_ACTION));
                }
            }
            break;
        case DIG_IN_J:
            if(_cfgz.GetCFGZ_Param(CFGZ:: ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ:: CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTION), GCU_ALARMS::Auxilary_Input_J_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_J];
                    prvSetAlarmAction_NoWESN(DIG_IN_J, _cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_DIG_ACTION));
                }
            }
            break;
        case DIG_IN_K:
            if(_cfgz.GetCFGZ_Param(CFGZ:: ID_FUEL_LVL_DIG_K_SENSOR_SELECTION) == CFGZ:: CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_ACTION), GCU_ALARMS::Auxilary_Input_K_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_K];
                    prvSetAlarmAction_NoWESN(DIG_IN_K, _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_DIG_ACTION));
                }
            }
            break;
        case DIG_IN_L:
            if(_cfgz.GetCFGZ_Param(CFGZ:: ID_ENG_TEMP_DIG_L_SENSOR_SELECTION) == CFGZ:: CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_ACTION), GCU_ALARMS::Auxilary_Input_L_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_L];
                    prvSetAlarmAction_NoWESN(DIG_IN_L, _cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_DIG_ACTION));
                }
            }
            break;
        case DIG_IN_M:
            if(_cfgz.GetCFGZ_Param(CFGZ:: ID_SHEL_TEMP_DIG_M_SENSOR_SELECTION) == CFGZ:: CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_ACTION), GCU_ALARMS::Auxilary_Input_M_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_M];
                    prvSetAlarmAction_NoWESN(DIG_IN_M, _cfgz.GetCFGZ_Param(CFGZ::ID_SHEL_TEMP_DIG_M_DIG_ACTION));
                }
            }
            break;
        case DIG_IN_N:
            if(_cfgz.GetCFGZ_Param(CFGZ:: ID_AUX_S2_RES_DIG_N_SENSOR_SELECTION) == CFGZ:: CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTION), GCU_ALARMS::Auxilary_Input_N_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_N];
                    prvSetAlarmAction_NoWESN(DIG_IN_N, _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S2_RES_DIG_N_DIG_ACTION));
                }
            }
            break;
        case DIG_IN_O:
            if(_cfgz.GetCFGZ_Param(CFGZ:: ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ:: CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_ACTION), GCU_ALARMS::Auxilary_Input_O_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_O];
                    prvSetAlarmAction_NoWESN(DIG_IN_O, _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_DIG_ACTION));
                }
            }
            break;
        case DIG_IN_P:
            if(_cfgz.GetCFGZ_Param(CFGZ:: ID_AUX_S4_DIG_P_SENSOR_SELECTION) == CFGZ:: CFGZ_ANLG_DIG_IN)
            {
                prvAssignInputSettings(u8AlarmIndex, _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_SOURCE), _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_ACTIVATION), _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_ACTIVATION_DELAY), _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_ACTION), GCU_ALARMS::Auxilary_Input_P_id);
                if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_SOURCE) == CFGZ::CFGZ_USER_CONFIGURED_SENSOR)
                {
                    ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[DIG_INPUT_P];
                    prvSetAlarmAction_NoWESN(DIG_IN_P, _cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_DIG_ACTION));
                }
            }
            break;
        case REMOTE_SS:
            {
                //Implemented Completely in prvAssignInputSettings func.
            }
            break;
        case FAIL_TO_STOP:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Fail_To_Stop_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 1;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FAIL_TO_STOP_STATUS];
            break;
        case FAIL_TO_START:
            {
                ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
                ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = true;
                ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
                ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
                ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Fail_To_Start_id;
                ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
                ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 1;
                ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
            }
            ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[FAIL_TO_START_STATUS];
            break;
        case ESTOP:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
            break;
        case LOAD_UNBALANCE:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = ((_cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_ACTION) != CFGZ::CFGZ_ACTION_NONE) && (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE));
            prvSetAlarmAction_NoWESN(u8AlarmIndex,CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_ACTION);
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Load_Unbalance_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_THRESHOLD));
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax =  NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_UNBAL_LOAD_DELAY);
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[GEN_UNBALANCED_LOAD];
        break;
        case LFL_SWITCH:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
            break;
        case LLOP_SWITCH:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
            break;
        case HWT_SWITCH:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
            break;
        case RWL_SWITCH:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
            break;
        case FILT_MAINTENANCE_BY_DATE:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true ;
            prvSetAlarmAction_NW(u8AlarmIndex,CFGZ::ID_MAINT_ALARM_ACTION);
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Filter_maintenance_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[MAINT_DATE];
            break;
        case V_BELT_BROKEN_SWITCH:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
            break;
        case LLOP_MON:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
        break;
        case HIGH_OIL_PRESS_DETECTED:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = true;
            ArrAlarmMonitoring[u8AlarmIndex].bEnableWarning = true;
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8DummyOne;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = High_Oil_Press_Detected_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[HIGH_OIL_PRESSURE];
        break;
        case MAINS_CONTACTOR_LATCHED:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
        break;
        case DG_CONTACTOR_LATCHED:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
        break;
        case BATT_CHG_FAIL:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
            break;
        case SMOKE_FIRE:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
            break;
        case MODE_SELECT:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
            break;
        case INVALID_DG_START:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableNotification = (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_PRESENT) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Invalid_gen_start_id;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.u8Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = ONE_BYTE_INT;
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[INVALID_GEN_START_STATUS];
        break;
        case AMB_TEMP_SWITCH:
        {
            //Implemented Completely in prvAssignInputSettings func.
        }
        break;
        case ALARM_AMBER_LAMP :
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_AMBER) != CFGZ::CFGZ_ACTION_NONE);
            prvSetAlarmAction_NoWESN(ALARM_AMBER_LAMP, _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_AMBER));
            prvSetAlarmActivation(ALARM_AMBER_LAMP,  _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTIVATION_AMBER));
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Alarm_Amber_Lamp_id;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACT_DELAY_AMBER);
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[J1939_AMBER_LAMP_STATUS];
        break;
        case ALARM_RED_LAMP :
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_RED) != CFGZ::CFGZ_ACTION_NONE);
            prvSetAlarmAction_NoWESN(ALARM_RED_LAMP, _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_RED));
            prvSetAlarmActivation(ALARM_RED_LAMP,  _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTIVATION_RED));
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Alarm_Red_Lamp_id;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACT_DELAY_RED);
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[J1939_RED_LAMP_STATUS];
        break;
        case ALARM_MIL_LAMP:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_MIL) != CFGZ::CFGZ_ACTION_NONE);
            prvSetAlarmAction_NoWESN(ALARM_MIL_LAMP, _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_MIL));
            prvSetAlarmActivation(ALARM_MIL_LAMP,  _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTIVATION_MIL));
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Alarm_Mil_Lamp_id;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACT_DELAY_MIL);
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[J1939_MIL_LAMP_STATUS];
        break;
        case ALARM_PROTECT_LAMP:
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (_cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_PROTECT) != CFGZ::CFGZ_ACTION_NONE);
            prvSetAlarmAction_NoWESN(ALARM_PROTECT_LAMP, _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTION_PROTECT));
            prvSetAlarmActivation(ALARM_PROTECT_LAMP,  _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACTIVATION_PROTECT));
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Alarm_Protect_Lamp_id;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = NO_OF_50MSEC_TICKS_FOR_1SEC * _cfgz.GetCFGZ_Param(CFGZ::ID_CAN_J1939_COMM_ACT_DELAY_PROTECT);
        }
        ArrAlarmMonitoring[u8AlarmIndex].pValue = &_ArrAlarmValue[J1939_PROTECT_LAMP_STATUS];
        break;
        case ALARM_P0031 :
        case ALARM_P0032 :
        case ALARM_P0107 :
        case ALARM_P0108 :
        case ALARM_P0112 :
        case ALARM_P0113 :
        case ALARM_P0117 :
        case ALARM_P0118 :
        case ALARM_P0121 :
        case ALARM_P0122 :
        case ALARM_P0123 :
        case ALARM_P0131 :
        case ALARM_P0132 :
        case ALARM_P0133 :
        case ALARM_P0134 :
        case ALARM_P0217 :
        case ALARM_P0236 :
        case ALARM_P0237 :
        case ALARM_P0238 :
        case ALARM_P0335 :
        case ALARM_P0340 :
        case ALARM_P0522 :
        case ALARM_P0523 :
        case ALARM_P0524 :
        case ALARM_P0562 :
        case ALARM_P0563 :
        case ALARM_P0638 :
        case ALARM_P0657 :
        case ALARM_P0658 :
        case ALARM_P0659 :
        case ALARM_P1518 :
        case ALARM_P1630 :
        case ALARM_P1631 :
        case ALARM_P1632 :
        case ALARM_P1633 :
        case ALARM_P2100 :
        case ALARM_P2102 :
        case ALARM_P2103 :
        case ALARM_P2111 :
        case ALARM_P2112 :
        case ALARM_P2177 :
        case ALARM_P2178 :
        case ALARM_P2191 :
        case ALARM_P2192 :
        case ALARM_P2231 :
        case ALARM_P2300 :
        case ALARM_P2301 :
        case ALARM_P2302 :
        case ALARM_P2303 :
        case ALARM_P2304 :
        case ALARM_P2305 :
        case ALARM_P2306 :
        case ALARM_P2307 :
        case ALARM_P2308 :
        case ALARM_P2309 :
        case ALARM_P2310 :
        case ALARM_P2311 :
        case ALARM_P2312 :
        case ALARM_P2313 :
        case ALARM_P2314 :
        case ALARM_P2315 :
        case ALARM_P2316 :
        case ALARM_P2317 :
        {
            ArrAlarmMonitoring[u8AlarmIndex].bEnableMonitoring = (false);
            ArrAlarmMonitoring[u8AlarmIndex].bEnableShutdown = (_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SHUTDOWN) == CFGZ::CFGZ_ENABLE);
            ArrAlarmMonitoring[u8AlarmIndex].LocalEnable = &_u8MonOn;
            ArrAlarmMonitoring[u8AlarmIndex].bMonitoringPolarity = true;
            ArrAlarmMonitoring[u8AlarmIndex].u8LoggingID = Alarm_P0031_id + u8AlarmIndex - ALARM_P0031;
            ArrAlarmMonitoring[u8AlarmIndex].Threshold.f32Value = 0;
            ArrAlarmMonitoring[u8AlarmIndex].u16CounterMax = 20;
            ArrAlarmMonitoring[u8AlarmIndex].ThreshDataType = FLOAT_TYPE;

        }
        break;

        default :
            break;
    }
}

float GCU_ALARMS::GetSpeedValue()
{
   if(_cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_SPEED_SENSE_SOURCE) == CFGZ::CFGZ_ALT_FREQUENCY)
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
    A_SENSE::SENSOR_RET_t stShelterTemp = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_SHELTER_TEMPERATURE);
    A_SENSE::SENSOR_RET_t stAuxSensS1 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S1_SENSOR);
    A_SENSE::SENSOR_RET_t stAuxSensS2 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S2_SENSOR);
    A_SENSE::SENSOR_RET_t stAuxSensS3 = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED};
    A_SENSE::SENSOR_RET_t stAuxSensS4 = {{0.0f,ANLG_IP::BSP_STATE_NORMAL},A_SENSE::SENSOR_NOT_CONFIGRUED};

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        stAuxSensS3 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S3_4_20_SENSOR);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        stAuxSensS3 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S3_0_5_SENSOR);
    }

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        stAuxSensS4 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S4_4_20_SENSOR);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S4_DIG_P_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        stAuxSensS4 = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_S4_0_5_SENSOR);
    }

    stLOP = GetLOPSensorVal();

    _ArrAlarmValue[LUBE_OIL_PRESSURE].f32Value = stLOP.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[FUEL_LEVEL].u8Value = (uint8_t)stFuel.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ENGINE_TEMPERATURE].f32Value = stEngTemp.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ENGINE_SPEED].u16Value =(uint16_t) GetSpeedValue();

    _ArrAlarmValue[GENSET_FREQUENCY].f32Value = _hal.AcSensors.GENSET_GetMinFrq();
    _ArrAlarmValue[GEN_R_PHASE_VOLTAGE].u16Value = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVolts(R_PHASE);
    _ArrAlarmValue[GEN_Y_PHASE_VOLTAGE].u16Value = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVolts(Y_PHASE);
    _ArrAlarmValue[GEN_B_PHASE_VOLTAGE].u16Value = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVolts(B_PHASE);

    _ArrAlarmValue[MAINS_MIN_PHASE_VOLTAGE].u16Value = prvGetMinMainsVoltage();
    _ArrAlarmValue[MAINS_MAX_PHASE_VOLTAGE].u16Value = prvGetMaxMainsVoltage();

    _ArrAlarmValue[MAINS_MIN_PHASE_FREQ].f32Value = prvGetMinMainsFreq();
    _ArrAlarmValue[MAINS_MAX_PHASE_FREQ].f32Value = prvGetMaxMainsFreq();

    _ArrAlarmValue[MAINS_PH_PH_MIN_VOLTAGE].u16Value = prvGetMinPhToPhMainsVoltage();

    _ArrAlarmValue[CHARG_ALT_VOLTAGE].f32Value = _hal.AnalogSensors.GetFilteredChargingAltVolts();

    _ArrAlarmValue[CHARG_ALT_ERROR].u8Value = (uint8_t)CHARGING_ALT::IsChargAltFail();
    _ArrAlarmValue[BATTERY_VOLTAGE].f32Value = GetSelectedBatteryVtg();
    _ArrAlarmValue[BTS_VOLTAGE].f32Value =  _hal.AnalogSensors.GetFilteredVBTSbattVolts();
    _ArrAlarmValue[GEN_MAX_CURRENT].u16Value = (uint16_t)prvGetMaxGensetCurent();
    _ArrAlarmValue[ENG_RUN_HOURS].u16Value = (uint16_t)(GetSelectedEngRunMin()/60);
    _ArrAlarmValue[TOTAL_KW_PERCENT].u16Value = (uint16_t)((_hal.AcSensors.GENSET_GetActivePowerWatts(R_PHASE) +
                                                _hal.AcSensors.GENSET_GetActivePowerWatts(Y_PHASE) +
                                                _hal.AcSensors.GENSET_GetActivePowerWatts(B_PHASE))/(_cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_GEN_RATING)*10));

    _ArrAlarmValue[FUEL_THEFT_ALARM].u8Value = _u8FuelTheftAlarm;
    _ArrAlarmValue[GEN_PHASE_ROTATION_STATUS].u8Value = (uint8_t)_hal.AcSensors.GENSET_GetPhaseRotStatus();
    _ArrAlarmValue[MAINS_PHASE_ROTATION_STATUS].u8Value = (uint8_t)_hal.AcSensors.MAINS_GetPhaseRotStatus();
    _ArrAlarmValue[ENG_TEMP_OPEN_CKT].u8Value = (uint8_t)(stEngTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT);
    _ArrAlarmValue[FUEL_OPEN_CKT_VAL].u8Value = (uint8_t)(stFuel.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT);


    if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR3)
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
    _ArrAlarmValue[DIG_INPUT_A].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_A_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_B].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_B_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_C].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_C_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_D].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_D_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_E].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_E_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_F].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_F_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_G].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_G_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_H].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_H_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_I].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_I_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_J].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_J_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_K].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_K_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_L].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_L_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_M].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_M_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_N].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_N_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_O].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_O_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[DIG_INPUT_P].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_P_USER_CONFIGURED) == DigitalSensor::SENSOR_LATCHED;

    _ArrAlarmValue[LOW_FUEL_LVL_SWITCH_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_FUEL_LEVEL_SWITCH) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[LLOP_SWITCH_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_LUBE_OIL_PRESSURE_SWITCH) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[HWT_SWITCH_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_HIGH_ENGINE_TEMP_SWITCH) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[WATER_LEVEL_SWITCH_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_LOW_WATER_LEVEL_SWITCH) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[EMERGENCY_STOP_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_EMERGENCY_STOP) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[REMOTE_SS_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_REMOTE_START_STOP) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[SIM_START_STATUS].u8Value=(uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_START) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[SIM_STOP_STATUS].u8Value=(uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_STOP) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[SIM_AUTO_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_AUTO) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[CLOSE_GEN_OPEN_MAINS_STATUS].u8Value=(uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_CLOSE_GEN_OPEN_MAINS_SWITCH) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[CLOSE_MAINS_OPEN_GEN_STATUS].u8Value=(uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_CLOSE_MAINS_OPEN_GEN_SWITCH) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[SIM_MAINS_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SIMULATE_MAINS) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[V_BELT_BROKEN_SWITCH_STATUS].u8Value = _hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_V_BELT_BROKEN_SWITCH) == DigitalSensor::SENSOR_LATCHED;
    _ArrAlarmValue[MAINS_CONT_LATCHED_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_MAINS_CONTACTOR_LATCHED) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[GEN_CONT_LATCHED_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_GEN_CONTACTOR_LATCHED) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[BATT_CHG_FAIL_STATUS].u8Value= (uint8_t)((_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_BATTERY_CHARGER_FAIL) == DigitalSensor::SENSOR_LATCHED)
                      && (BASE_MODES::GetMainsStatus() == BASE_MODES::MAINS_HELATHY) && (_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_MONITORING) == CFGZ::CFGZ_ENABLE));
    _ArrAlarmValue[SMOKE_FIRE_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_SMOKE_FIRE) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[MODE_SELECT_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_MODE_SELECT) == DigitalSensor::SENSOR_LATCHED);
    _ArrAlarmValue[AMB_TEMP_SELECT_STATUS].u8Value= (uint8_t)(_hal.DigitalSensors.GetDigitalSensorState(DigitalSensor::DI_AMB_TEMP_SELECT) == DigitalSensor::SENSOR_LATCHED);





    _ArrAlarmValue[SHELTER_TEMP_VAL].f32Value = stShelterTemp.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ANLG_SENS_S1_VAL].f32Value = stAuxSensS1.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ANLG_SENS_S2_VAL].f32Value = stAuxSensS2.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ANLG_SENS_S3_VAL].f32Value = stAuxSensS3.stValAndStatus.f32InstSensorVal;
    _ArrAlarmValue[ANLG_SENS_S4_VAL].f32Value = stAuxSensS4.stValAndStatus.f32InstSensorVal;

    _ArrAlarmValue[SHELT_TEMP_OPEN_CKT].u8Value = (uint8_t)(stShelterTemp.stValAndStatus.eState == ANLG_IP::BSP_STATE_OPEN_CKT);

    _ArrAlarmValue[EARTH_LEAKAGE_CURR_VAL].f32Value = _hal.AcSensors.EARTH_GetCurrentFilt() ;


    _ArrAlarmValue[FAIL_TO_STOP_STATUS].u8Value = _bFailToStop;
    _ArrAlarmValue[FAIL_TO_START_STATUS].u8Value = _bFailToStart;

    _ArrAlarmValue[GEN_UNBALANCED_LOAD].u8Value = prvGetUnbalancedLoadVal();
    _ArrAlarmValue[HIGH_OIL_PRESSURE].u8Value = _u8HighOilPressDetectedAlarm;



    _ArrAlarmValue[INVALID_GEN_START_STATUS].u8Value = !(uint8_t)(ENGINE_START_VALIDITY::IsValidEngineStartFound());

    _ArrAlarmValue[MPU_LOSS_STATUS].u8Value = _u8MPULossAlarm;

    RTC::TIME_t CurrentTime;
    _hal.ObjRTC.GetTime(&CurrentTime);
    if(CurrentTime.u16Year > _cfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_3))
    {
        _ArrAlarmValue[MAINT_DATE].u8Value = 1;
    }
    else if(CurrentTime.u16Year == _cfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_3))
    {
        if(CurrentTime.u8Month == _cfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_2))
        {
            if(CurrentTime.u8Day > _cfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_1))
            {
                _ArrAlarmValue[MAINT_DATE].u8Value = 1;
            }
            else
            {
                _ArrAlarmValue[MAINT_DATE].u8Value = 0;
            }
        }
        else if(CurrentTime.u8Month >_cfgz.GetCFGZ_Param(CFGZ::ID_ALARM_DUE_DATE_SERVICE_DATE_2))
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
    _ArrAlarmValue[PIN23_SENSOR_CURRENT_VAL].f32Value = _hal.AnalogSensors.GetPin23CurrentValMilliAmp();

    _ArrAlarmValue[J1939_PROTECT_LAMP_STATUS].u8Value = gpJ1939->IsProtectLampON();
    _ArrAlarmValue[J1939_AMBER_LAMP_STATUS].u8Value = gpJ1939->IsAmberLampON();
    _ArrAlarmValue[J1939_RED_LAMP_STATUS].u8Value = gpJ1939->IsRedLampON();
    _ArrAlarmValue[J1939_MIL_LAMP_STATUS].u8Value = gpJ1939->IsMilLampON();


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
            _ArrAlarmStatus[u8LoggingID] = &_u8DummyZero;
            break;
        case Radiator_Water_Level_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[RWL_SWITCH].bAlarmActive;
            break;
        case Over_Speed_l1_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OVERSPEED_L1].bAlarmActive;
            break;
        case Over_Speed_l2_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OVERSPEED_L2].bAlarmActive;
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
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[ESTOP].bAlarmActive;
            break;
        case Charge_Fail_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[CA_FAIL].bAlarmActive;
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
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_A].bAlarmActive;
            break;
        case Auxilary_Input_B_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_B].bAlarmActive;
            break;
        case Auxilary_Input_C_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_C].bAlarmActive;
            break;
        case Auxilary_Input_D_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_D].bAlarmActive;
            break;
        case Auxilary_Input_E_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_E].bAlarmActive;
            break;
        case Auxilary_Input_F_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_F].bAlarmActive;
            break;
        case Auxilary_Input_G_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_G].bAlarmActive;
            break;
        case Auxilary_Input_H_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_H].bAlarmActive;
            break;
        case Auxilary_Input_I_id :
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_I].bAlarmActive;
            break;
        case Auxilary_Input_J_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_J].bAlarmActive;
            break;
        case Auxilary_Input_K_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_K].bAlarmActive;
            break;
        case Auxilary_Input_L_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_L].bAlarmActive;
            break;
        case Auxilary_Input_M_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_M].bAlarmActive;
            break;
        case Auxilary_Input_N_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_N].bAlarmActive;
            break;
        case Auxilary_Input_O_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_O].bAlarmActive;
            break;
        case Auxilary_Input_P_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DIG_IN_P].bAlarmActive;
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
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OPEN_LOP_CURR_SENS_CKT].bAlarmActive;
            break;
        case Lop_Sen_Ckt_Open_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OPEN_LOP_SENS_CKT].bAlarmActive;
            break;
        case Engine_Temperature_Ckt_Open_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OPEN_ENG_TEMP_CKT].bAlarmActive;
            break;
        case Phase_DG_Rotation_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[DG_PHASE_ROTATION].bAlarmActive;
            break;
        case Phase_EB_Rotation_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[EB_PHASE_ROTATION].bAlarmActive;
            break;
        case V_Belt_Broken_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[V_BELT_BROKEN_SWITCH].bAlarmActive;
            break;
        case LOP_Curr_Sen_Over_Curr_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[LOP_CURR_SENS_OVER_CURR].bAlarmActive;
            break;
        case Aux_S1_Ckt_Open_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OPEN_AN_SEN_S1_CKT].bAlarmActive;
            break;
        case Aux_S2_Ckt_Open_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[OPEN_AN_SEN_S2_CKT].bAlarmActive;
            break;
        case Aux_S2_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[AN_SEN_S2_MON].bAlarmActive;
            break;
        case High_Oil_Press_Detected_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[HIGH_OIL_PRESS_DETECTED].bAlarmActive;
            break;
        case Batt_Chg_Fail_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[BATT_CHG_FAIL].bAlarmActive;
            break;
        case Smoke_Fire_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[SMOKE_FIRE].bAlarmActive;
            break;
        case Engine_Start_id:
            _ArrAlarmStatus[u8LoggingID] = &_u8DummyZero;
            break;
        case Engine_Stop_id:
            _ArrAlarmStatus[u8LoggingID] = &_u8DummyZero;
            break;
        case Config_Modified_By_Master_id  :
            _ArrAlarmStatus[u8LoggingID] = &_u8DummyZero;
            break;
        case Config_Modified_By_User_id :
            _ArrAlarmStatus[u8LoggingID] = &_u8DummyZero;
            break;
        case High_Shelter_temp_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[SHELTER_TEMP_START_DG].bAlarmActive;
            break;
        case Fan_High_Current_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[FAN_HIGH_CURR].bAlarmActive;
            break;
        case Fan_Low_Current_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[FAN_LOW_CURR].bAlarmActive;
            break;
        case Invalid_gen_start_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[INVALID_DG_START].bAlarmActive;
            break;
        case Lop_Short_To_Battery_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[LOP_SENS_SHORT_TO_BATTERY].bAlarmActive;
            break;
        case Alarm_Amber_Lamp_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[ALARM_AMBER_LAMP].bAlarmActive;
            break;
        case Alarm_Red_Lamp_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[ALARM_RED_LAMP].bAlarmActive;
            break;
        case Alarm_Mil_Lamp_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[ALARM_MIL_LAMP].bAlarmActive;
            break;
        case Alarm_Protect_Lamp_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[ALARM_PROTECT_LAMP].bAlarmActive;
            break;
        case Alarm_P0031_id:
        case Alarm_P0032_id:
        case Alarm_P0107_id:
        case Alarm_P0108_id:
        case Alarm_P0112_id:
        case Alarm_P0113_id:
        case Alarm_P0117_id:
        case Alarm_P0118_id:
        case Alarm_P0121_id:
        case Alarm_P0122_id:
        case Alarm_P0123_id:
        case Alarm_P0131_id:
        case Alarm_P0132_id:
        case Alarm_P0133_id:
        case Alarm_P0134_id:
        case Alarm_P0217_id:
        case Alarm_P0236_id:
        case Alarm_P0237_id:
        case Alarm_P0238_id:
        case Alarm_P0335_id:
        case Alarm_P0340_id:
        case Alarm_P0522_id:
        case Alarm_P0523_id:
        case Alarm_P0524_id:
        case Alarm_P0562_id:
        case Alarm_P0563_id:
        case Alarm_P0638_id:
        case Alarm_P0657_id:
        case Alarm_P0658_id:
        case Alarm_P0659_id:
        case Alarm_P1518_id:
        case Alarm_P1630_id:
        case Alarm_P1631_id:
        case Alarm_P1632_id:
        case Alarm_P1633_id:
        case Alarm_P2100_id:
        case Alarm_P2102_id:
        case Alarm_P2103_id:
        case Alarm_P2111_id:
        case Alarm_P2112_id:
        case Alarm_P2177_id:
        case Alarm_P2178_id:
        case Alarm_P2191_id:
        case Alarm_P2192_id:
        case Alarm_P2231_id:
        case Alarm_P2300_id:
        case Alarm_P2301_id:
        case Alarm_P2302_id:
        case Alarm_P2303_id:
        case Alarm_P2304_id:
        case Alarm_P2305_id:
        case Alarm_P2306_id:
        case Alarm_P2307_id:
        case Alarm_P2308_id:
        case Alarm_P2309_id:
        case Alarm_P2310_id:
        case Alarm_P2311_id:
        case Alarm_P2312_id:
        case Alarm_P2313_id:
        case Alarm_P2314_id:
        case Alarm_P2315_id:
        case Alarm_P2316_id:
        case Alarm_P2317_id:
            _ArrAlarmStatus[u8LoggingID] = (uint8_t *)&ArrAlarmMonitoring[(u8LoggingID-Alarm_P0031_id+ALARM_P0031)].bAlarmActive;
            break;
        default:
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
                if(((ArrAlarmMonitoring[_u8AlarmIndex].bWarningLatched))
                        || (ArrAlarmMonitoring[_u8AlarmIndex].bNotificationLatched ))
                {
                    ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant = false;
                    ArrAlarmMonitoring[_u8AlarmIndex].bResultLatched = false;

                }
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
        _u8LowFuelLevelAlarm = 0;
        _u8LowOilPressAlarm = 0;
        _u8AuxSensS1Alarm =  0;
        _u8AuxSensS2Alarm =  0;
        _u8AuxSensS3Alarm = 0;
        _u8AuxSensS4Alarm =  0;
        _u8ActuatorFailAlarm = 0;
        _bBTSBattHealthy = 1;
        if(_u8EngineOff == 0)
       {
           _u8HighOilPressDetectedAlarm =0;
       }
       else
        {
           A_SENSE::SENSOR_RET_t stLOP1 = GetLOPSensorVal();
          if(1)
          {
              _u8HighOilPressDetectedAlarm = 0;
          }
        }




    prvGensetRelatedAlarms();
    if((!_bCommonShutdown && !_bCommonWarning && !_bCommonElectricTrip)
            ||(UTILS_GetElapsedTimeInSec(&_SounderAlarmTimer)
               >= _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_SOUNDER_ALARM_TIMER)))
    {
       _bOPSounderAlarm = false;
       UTILS_DisableTimer(&_SounderAlarmTimer);
    }


}

void GCU_ALARMS::prvCheckTripAction(uint8_t u8ReturnIndex, uint8_t u8TripIndex, bool status)
{
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM) == CFGZ::CFGZ_1_PHASE_SYSTEM)
                  )
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
//        if((_u8AlarmIndex != REMOTE_START_STOP)&&(_u8AlarmIndex != VBTS_UV))
//        {
            ArrAlarmMonitoring[_u8AlarmIndex].bNotificationLatched = false;
            ArrAlarmMonitoring[_u8AlarmIndex].bWarningLatched = false;
            ArrAlarmMonitoring[_u8AlarmIndex].bElectricTripLatched = false;
            ArrAlarmMonitoring[_u8AlarmIndex].bShutdownLatched = false;
            ArrAlarmMonitoring[_u8AlarmIndex].bAlarmActive = false;
            if(!(((_u8AlarmIndex >= MAINS_UNDERVOLT_TRIP) &&
                    (_u8AlarmIndex <= MAINS_OVERFREQ_RETURN))||
                    (_u8AlarmIndex == MAINS_LL_UNDERVOLT_TRIP)||
                    (_u8AlarmIndex == MAINS_LL_UNDERVOLT_RETURN)))
            {
                ArrAlarmMonitoring[_u8AlarmIndex].bResultInstant = false;
                ArrAlarmMonitoring[_u8AlarmIndex].bResultLatched = false;
            }
//        }
    }
    _bFailToStop = false;
    _bFailToStart = false;
    _u8FuelTheftAlarm = 0;

    _u8AFTActivationTimeout = 0;
    _u8HighOilPressDetectedAlarm = 0;

    _hal.AcSensors.ClearPhaseReverseAlarms();

    ENGINE_MONITORING::ClearTriplatched();


    _u8UnderFreqAlarm = 0;

    _u8OverFreqAlarm = 0;

    _u8RPhaseOverVoltAlarm = 0;

    _u8YPhaseOverVoltAlarm = 0;

    _u8BPhaseOverVoltAlarm = 0;

    _u8RPhaseUnderVoltAlarm = 0;

    _u8YPhaseUnderVoltAlarm = 0;

    _u8BPhaseUnderVoltAlarm = 0;


    gpJ1939->ResetLampsStatus();
    gpJ1939->RequestDM11PGN();
    gpJ1939->SetDm2MsgCount(0);
    gpJ1939->ClearDM2Messages();
    gpJ1939->ClearNCDandPCDAlarms();
}


void GCU_ALARMS::ResetMainsMonParams()
{
    uint8_t u8Index;

//    for(u8Index = MAINS_R_UNDERVOLT_TRIP; u8Index <= MAINS_B_OVERFREQ_TRIP; u8Index++)
//    {
//        ArrAlarmMonitoring[u8Index].u16Counter = 0;
//        ArrAlarmMonitoring[u8Index].bResultInstant = false;
//        ArrAlarmMonitoring[u8Index].bResultLatched = false;
//    }
//
//    for(u8Index = MAINS_R_UNDERVOLT_RETURN; u8Index <= MAINS_B_OVERFREQ_RETURN; u8Index++)
//    {
//        ArrAlarmMonitoring[u8Index].u16Counter = 0;
//        ArrAlarmMonitoring[u8Index].bResultInstant = true;
//        ArrAlarmMonitoring[u8Index].bResultLatched = true;
//    }
//
//    if(ArrAlarmMonitoring[REMOTE_START_STOP].bEnableMonitoring)
//    {
//       ArrAlarmMonitoring[REMOTE_START_STOP].bResultInstant = false;
//       ArrAlarmMonitoring[REMOTE_START_STOP].u16Counter = 0;
//    }
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
        LogEvent(ArrAlarmMonitoring[u8Index].u8LoggingID, CFGZ::CFGZ_ACTION_SHUTDOWN_NoWESN);
    }
    else if(ArrAlarmMonitoring[u8Index].bEnableWarning)
    {
        _bOPSounderAlarm = true;
        UTILS_ResetTimer(&_SounderAlarmTimer);
        LogEvent(ArrAlarmMonitoring[u8Index].u8LoggingID, CFGZ::CFGZ_ACTION_WARNING_NoWESN);
    }
    else if(ArrAlarmMonitoring[u8Index].bEnableElectricTrip)
    {
        _bOPSounderAlarm = true;
        UTILS_ResetTimer(&_SounderAlarmTimer);
        LogEvent(ArrAlarmMonitoring[u8Index].u8LoggingID, CFGZ::CFGZ_ACTION_ELECTRICAL_TRIP_NoWESN);
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
    _u8RPhaseOverVoltAlarm = ArrAlarmMonitoring[DG_R_OV_SHUTDOWN].bAlarmActive ||
                            ArrAlarmMonitoring[DG_R_OV_WARNING].bAlarmActive;
    _u8YPhaseOverVoltAlarm = ArrAlarmMonitoring[DG_Y_OV_SHUTDOWN].bAlarmActive ||
                            ArrAlarmMonitoring[DG_Y_OV_WARNING].bAlarmActive;
    _u8BPhaseOverVoltAlarm = ArrAlarmMonitoring[DG_B_OV_SHUTDOWN].bAlarmActive ||
                            ArrAlarmMonitoring[DG_B_OV_WARNING].bAlarmActive;
    _u8RPhaseUnderVoltAlarm = ArrAlarmMonitoring[DG_R_UV_SHUTDOWN].bAlarmActive ||
                             ArrAlarmMonitoring[DG_R_UV_WARNING].bAlarmActive;
    _u8YPhaseUnderVoltAlarm = ArrAlarmMonitoring[DG_Y_UV_SHUTDOWN].bAlarmActive ||
                             ArrAlarmMonitoring[DG_Y_UV_WARNING].bAlarmActive;
    _u8BPhaseUnderVoltAlarm = ArrAlarmMonitoring[DG_B_UV_SHUTDOWN].bAlarmActive ||
                             ArrAlarmMonitoring[DG_B_UV_WARNING].bAlarmActive;
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
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
    {
        float f32GenLoadDiff = prvGetGenMaxKWPercent() - prvGetGenMinKWPercent();
        return (uint8_t)((3*100*f32GenLoadDiff)/(_cfgz.GetCFGZ_Param(CFGZ::ID_LOAD_MONITOR_GEN_RATING)));
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
    prvActDeactOutput(ArrAlarmMonitoring[VBAT_OV].bResultInstant, ACTUATOR::ACT_VBAT_OV);
    prvActDeactOutput(ArrAlarmMonitoring[VBAT_UV].bResultInstant, ACTUATOR::ACT_VBAT_UV);
    prvActDeactOutput(ArrAlarmMonitoring[CA_FAIL].bShutdownLatched, ACTUATOR::ACT_CA_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[CA_FAIL].bWarningLatched, ACTUATOR::ACT_CA_WARNING);
    prvActDeactOutput(_bCommonAlarm, ACTUATOR::ACT_ALARM);
    prvActDeactOutput(_bCommonElectricTrip, ACTUATOR::ACT_ELEC_TRIP);
    prvActDeactOutput(_bCommonShutdown, ACTUATOR::ACT_SHUTDOWN);
    prvActDeactOutput(_bCommonWarning, ACTUATOR::ACT_WARNING);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_A].bResultInstant, ACTUATOR::ACT_DIG_IN_A);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_B].bResultInstant, ACTUATOR::ACT_DIG_IN_B);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_C].bResultInstant, ACTUATOR::ACT_DIG_IN_C);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_D].bResultInstant, ACTUATOR::ACT_DIG_IN_D);
    prvActDeactOutput(ArrAlarmMonitoring[DIG_IN_E].bResultInstant, ACTUATOR::ACT_DIG_IN_E);
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
    prvActDeactOutput(ArrAlarmMonitoring[ESTOP].bResultLatched, ACTUATOR::ACT_E_STOP);
    prvActDeactOutput(ArrAlarmMonitoring[FAIL_TO_START].bResultLatched, ACTUATOR::ACT_FAIL_TO_START);
    prvActDeactOutput(ArrAlarmMonitoring[FAIL_TO_STOP].bResultLatched, ACTUATOR::ACT_FAIL_TO_STOP);
    prvActDeactOutput(ArrAlarmMonitoring[DG_R_OV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_R_OV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[DG_Y_OV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_Y_OV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[DG_B_OV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_B_OV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[DG_R_UV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_R_UV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[DG_Y_UV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_Y_UV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[DG_B_UV_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_GEN_B_UV_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[OVERCURRENT].bResultLatched, ACTUATOR::ACT_GEN_OC);
    prvActDeactOutput(ArrAlarmMonitoring[HIGH_WATER_TEMP].bAlarmActive || ArrAlarmMonitoring[HWT_SWITCH].bAlarmActive, ACTUATOR::ACT_HIGH_TEMP);
    prvActDeactOutput(_u8LowFuelLevelAlarm, ACTUATOR::ACT_LOW_FUEL);
    prvActDeactOutput(ArrAlarmMonitoring[LOW_FUEL_LEVEL_NOTIFICATION].bNotificationLatched, ACTUATOR::ACT_LOW_FUEL_NOTIFICATION);
    prvActDeactOutput(_u8LowOilPressAlarm, ACTUATOR::ACT_LOW_PRES);
    prvActDeactOutput(ArrAlarmMonitoring[MAINS_OVERVOLT_TRIP].bResultInstant, ACTUATOR::ACT_MAINS_HIGH);
    prvActDeactOutput(ArrAlarmMonitoring[MAINS_UNDERVOLT_TRIP].bResultInstant, ACTUATOR::ACT_MAINS_LOW);
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION)==CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1) || (_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION)==CFGZ::CFGZ_ANLG_LOP_VOL_SENSOR))
    {
        prvActDeactOutput(ArrAlarmMonitoring[OPEN_LOP_SENS_CKT].bResultLatched, ACTUATOR::ACT_OIL_CKT_OPEN);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION)==CFGZ::CFGZ_ANLG_LOP_CUR_SENSOR)
    {
        prvActDeactOutput(ArrAlarmMonitoring[OPEN_LOP_CURR_SENS_CKT].bResultLatched, ACTUATOR::ACT_OIL_CKT_OPEN);
    }
    prvActDeactOutput(ArrAlarmMonitoring[OVERFREQ_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_OF_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[OVERSPEED_L1].bShutdownLatched, ACTUATOR::ACT_OS_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[OVERSPEED_L2].bShutdownLatched, ACTUATOR::ACT_GROSS_OS_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[OPEN_ENG_TEMP_CKT].bResultLatched, ACTUATOR::ACT_TEMP_CKT_OPEN);
    prvActDeactOutput(ArrAlarmMonitoring[UNDERFREQ_SHUTDOWN].bShutdownLatched, ACTUATOR::ACT_UF_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[UNDERSPEED].bShutdownLatched, ACTUATOR::ACT_US_SHUTDOWN);
    prvActDeactOutput(ArrAlarmMonitoring[FILT_MAINTENANCE].bAlarmActive || ArrAlarmMonitoring[FILT_MAINTENANCE_BY_DATE].bAlarmActive, ACTUATOR::ACT_FILT_MAINT);
    prvActDeactOutput(ArrAlarmMonitoring[ALARM_MIL_LAMP].bResultInstant, ACTUATOR::ACT_MIL);/*DOUBT*/
}



uint16_t GCU_ALARMS::GetMinGensetVoltage()
{
    static uint16_t u16MinGensetVoltage;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM) != CFGZ::CFGZ_1_PHASE_SYSTEM)
    {
        u16MinGensetVoltage = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVoltsRaw(R_PHASE);
        if((uint16_t)_hal.AcSensors.GENSET_GetVoltageVoltsRaw(Y_PHASE) < u16MinGensetVoltage)
        {
            u16MinGensetVoltage = (uint16_t)_hal.AcSensors.GENSET_GetVoltageVoltsRaw(Y_PHASE);
        }
        if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM) == CFGZ::CFGZ_1_PHASE_SYSTEM)
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
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
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
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
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
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
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
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
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
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
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
float GCU_ALARMS::prvGetMinMainsFreq()
{
    static float f32MinMainsFreq;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
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
    else
    {
        f32MinMainsFreq = _hal.AcSensors.MAINS_GetApproxFreq(R_PHASE);
    }
    return f32MinMainsFreq;
}
float GCU_ALARMS::prvGetMaxMainsFreq()
{
    static float f32MaxMainsFreq;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_MAINS_CONFIG_MAINS_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
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
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
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
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
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

    return (f32MaxGenKW/1000);
}

float GCU_ALARMS::prvGetGenMinKWPercent()
{
    static float f32MinGenKW=0;
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_AC_SYSTEM) == CFGZ::CFGZ_3_PHASE_SYSTEM)
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
                        _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_TANK_CAPACITY))/100)
                            > _cfgz.GetCFGZ_Param(CFGZ::ID_FUEL_LVL_DIG_K_FUEL_THEFT_THRESHOLD))
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
    UTILS_ResetTimer(&_FuelSettlingTimer);

/* Shubham Wader 16.09.2022 '
   shifting below snippet of code here which was initially in MAIN_UI.  Keeping MAIN UI abstact.*/
    for(uint8_t u8AlarmIndex = 0; u8AlarmIndex < GCU_ALARMS::ALARM_LIST_LAST; u8AlarmIndex++)
    {
      ConfigureGCUAlarms(u8AlarmIndex);
    }
    for(uint8_t u8LoggingID = 0; u8LoggingID < GCU_ALARMS::ID_ALL_ALARMS_LAST; u8LoggingID++)
    {
      AssignAlarmsForDisplay(u8LoggingID);
    }

    ClearAllAlarms();
    ResetMainsMonParams();
}

void GCU_ALARMS::prvMainsHighLowOutputs()
{
    if(ArrAlarmMonitoring[MAINS_UNDERVOLT_TRIP].bResultInstant && ArrAlarmMonitoring[MAINS_UNDERVOLT_TRIP].bResultLatched)
    {
        _hal.actuators.Activate(ACTUATOR::ACT_MAINS_LOW);
    }
    else if(ArrAlarmMonitoring[MAINS_UNDERVOLT_TRIP].bResultLatched && ArrAlarmMonitoring[MAINS_UNDERVOLT_RETURN].bResultInstant)
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_MAINS_LOW);
    }

    if(ArrAlarmMonitoring[MAINS_OVERVOLT_TRIP].bResultInstant && ArrAlarmMonitoring[MAINS_OVERVOLT_TRIP].bResultLatched)
    {
        _hal.actuators.Activate(ACTUATOR::ACT_MAINS_HIGH);
    }
    else if(ArrAlarmMonitoring[MAINS_OVERVOLT_TRIP].bResultLatched && ArrAlarmMonitoring[MAINS_OVERVOLT_RETURN].bResultInstant)
    {
        _hal.actuators.Deactivate(ACTUATOR::ACT_MAINS_HIGH);
    }
}

bool GCU_ALARMS::IsMainsSeqFail()
{
    {
        return false;
    }
}
void GCU_ALARMS::prvCoolantTempCtrlFunction(void)
{
    A_SENSE::SENSOR_RET_t stTemp = GetSelectedTempSensVal();

    if(((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
           &&(stTemp.stValAndStatus.eState != ANLG_IP::BSP_STATE_OPEN_CKT)))
//            || ((_cfgz.GetCFGZ_Param(CFGZ::ID_ENGINE_TYPE) > CFGZ::ENG_CONVENTIONAL)))
//                    &&(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_FROM_ENG) == CFGZ::CFGZ_ENABLE)))&& (_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_CTRL_EN) == CFGZ::CFGZ_ENABLE))
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

//    if(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_CTRL) == CFGZ::CFGZ_HEATER_CONTROL)
//    {
//        if((int16_t)round(stTemp.stValAndStatus.f32InstSensorVal) <= (int16_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_ON_THRESH)))
//        {
//            _bCLNTTempCtrl = true;
//        }
//        else if((int16_t)round(stTemp.stValAndStatus.f32InstSensorVal) >= (int16_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_OFF_THRESH)))
//        {
//            _bCLNTTempCtrl = false;
//        }
//    }
//    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_CTRL) == CFGZ::CFGZ_COOLER_CONTROL)
//    {
//        if((int16_t)round(stTemp.stValAndStatus.f32InstSensorVal) >= (int16_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_ON_THRESH)))
//        {
//            _bCLNTTempCtrl = true;
//        }
//        else if((int16_t)round(stTemp.stValAndStatus.f32InstSensorVal) <= (int16_t)(_cfgz.GetCFGZ_Param(CFGZ::ID_CLNT_TEMP_OFF_THRESH)))
//        {
//            _bCLNTTempCtrl = false;
//        }
//    }
}


bool GCU_ALARMS::prvIsNeedToCheckSensFltAlarm()
{
    if(START_STOP::IsFuelRelayOn()
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
        _u8MPULossAlarm = 0;
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

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_LOP_RES_DIG_J_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
         stLOP = _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)
    {
        stLOP =_hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE_4_20);
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_AUX_S3_DIG_O_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR2)
    {
        stLOP =_hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE_0_TO_5V);
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
    return _hal.AnalogSensors.GetSensorValue(AnalogSensor::A_SENSE_ENG_TEMPERATURE);
}


float GCU_ALARMS::GetSelectedBatteryVtg()
{
    if((_cfgz.GetCFGZ_Param(CFGZ::ID_BATTERY_MONITOR_BATTERY_MON_BY_J1939)== CFGZ::CFGZ_ENABLE)
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
//    if((_cfgz.GetCFGZ_Param(CFGZ::ID_RUNNING_HOURS_FROM_ENG)== CFGZ::CFGZ_ENABLE)
//            &&(_cfgz.GetEngType()!=CFGZ::ENG_CONVENTIONAL)
//           )
//    {
//        if(!gpJ1939->IsCommunicationFail())
//        {
//            if((isnan(gpJ1939->GetReadData(RX_PGN_HOURS_65253, 0) * 60) == true) || (isinf(gpJ1939->GetReadData(RX_PGN_HOURS_65253, 0) * 60) == true))
//            {
//                return 0;
//            }
//             return (uint32_t)(gpJ1939->GetReadData(RX_PGN_HOURS_65253, 0) * 60);
//        }
//        else
//        {
//            return 0;
//        }
//    }
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

bool GCU_ALARMS::IsShelterTempLow(void)
{
    return _bLowShelterTemp;
}

void GCU_ALARMS::UpdateFuelTheftCalculation()
{
    /*
     * This function is used to set _bUpdateFuelTheftCalc
     *  which updates the fuel theft calculations*/
    _bUpdateFuelTheftCalc = true;
}


#if TEST_ALARM
void GCU_ALARMS::prvTestAlarm()
{
/* Shubham Wader 21.09.2022
 This function is written for testing the Alarm UI. This will intentionally trigger all the alarms
 consecutively with delay of 1 Sec.
 */
    static uint16_t u16index = 0;
    if(UTILS_GetElapsedTimeInSec(&_AlaramtestTimer) >= 1U)
    {
        *_ArrAlarmStatus[u16index] = 1;
        UTILS_ResetTimer(&_AlaramtestTimer);
        if(u16index < ID_ALL_ALARMS_LAST)
        {
            u16index++;
            //*_ArrAlarmStatus[u16index-1] = 0;
        }
        else
        {
            u16index = 0;
        }
    }
    else
    {
        /* Do nothing */
    }
}
#endif

