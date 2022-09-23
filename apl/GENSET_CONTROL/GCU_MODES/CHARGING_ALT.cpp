/**
 * @file        [CHARGING_ALT]
 * @brief       CHARGING_ALT
 *              Provides common API's for functionality offered by charging
 *              alternator SMD and the PWM duty cycle calculation.
 *              This module is implemented in class CHARGING_ALT
 *
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        4st June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#include "CHARGING_ALT.h"
#include "START_STOP.h"

static void prvTimerCb();
static CHARGING_ALT *pChrgAlt = NULL;
bool CHARGING_ALT::_ChargAltFail = false;


CHARGING_ALT::CHARGING_ALT(HAL_Manager &hal, ENGINE_MONITORING &EngineMon, 
                                CFGZ &cfgz, GCU_ALARMS &gcuAlarms):
_hal(hal),
_EngineMon(EngineMon),
_cfgz(cfgz),
_GCUAlarms(gcuAlarms),
_StateCA (STATE_CHG_ALT_OFF),
_bChargAltOut(false),
_bStopChargAltPulse(false),
_bStartChargAltPulse(false),
_ChargAltPulseTimer{0},
_ChargAltWaitTimer{0},
_ChargAltMonTimer{0},
_CASMUpdateTimer{0},
_f32FilteredAltCurrent(0),
_u16TicksInCurrentWindow(0),
_f32Integral(0)
{
    pChrgAlt = this;
    _hal.Register200UsTimerCb(prvTimerCb);
    UTILS_ResetTimer(&_CASMUpdateTimer);
}

void CHARGING_ALT::Update(bool bDeviceInConfigMode)
{
    if((UTILS_GetElapsedTimeInMs(&_CASMUpdateTimer)) >= SM_UPDATE_TIME)
    {
        UTILS_ResetTimer(&_CASMUpdateTimer);
        switch(_StateCA)
        {
            case STATE_CHG_ALT_OFF:
                _bChargAltOut = false;
                _bStopChargAltPulse = false;
                if(_bStartChargAltPulse)
                {
                    _bStartChargAltPulse = false;
                    _StateCA = STATE_CHG_ALT_CMD_ON;
                    UTILS_ResetTimer(&_ChargAltPulseTimer);
                }
                break;

            case STATE_CHG_ALT_CMD_ON:
                _bChargAltOut = true;
                if(_bStartChargAltPulse)
                {
                    _bStartChargAltPulse = false;
                }
                else if(_bStopChargAltPulse)
                {
                    _bStopChargAltPulse = false;
                    UTILS_DisableTimer(&_ChargAltPulseTimer);
                    _StateCA = STATE_CHG_ALT_OFF;
                }
                else if(_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::CA_UV].bResultLatched)
                {
                    _ChargAltFail = true;
                    UTILS_DisableTimer(&_ChargAltPulseTimer);
                    _StateCA = STATE_CHG_ALT_ERR;
                }
                else if(UTILS_GetElapsedTimeInSec(&_ChargAltPulseTimer) >=  
                                                            CA_PULSE_TIME_SEC)
                {
                    if(START_STOP::IsGenMonOn() == 1U)
                    {
                        UTILS_DisableTimer(&_ChargAltPulseTimer);
                        UTILS_ResetTimer(&_ChargAltWaitTimer);
                        _StateCA = STATE_CHG_ALT_MON_WAIT;
                    }
                    else
                    {
                        UTILS_ResetTimer(&_ChargAltPulseTimer);
                    }
                }
                break;

            case STATE_CHG_ALT_MON_WAIT:
                _bChargAltOut = false;
                if(_bStartChargAltPulse)
                {
                    _bStartChargAltPulse = false;
                }
                else if((_EngineMon.IsEngineOn() == 0U) || (_bStopChargAltPulse))
                {
                    _bStopChargAltPulse = false;
                    _ChargAltFail = false;
                    UTILS_DisableTimer(&_ChargAltWaitTimer);
                    _StateCA = STATE_CHG_ALT_OFF;
                }
                else if(_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::CA_UV].bResultLatched)
                {
                    _ChargAltFail = true;
                    UTILS_DisableTimer(&_ChargAltWaitTimer);
                    _StateCA = STATE_CHG_ALT_ERR;
                }
                else if(UTILS_GetElapsedTimeInSec(&_ChargAltWaitTimer) >= 
                                                        CA_WAIT_TIME_SEC)
                {
                    UTILS_DisableTimer(&_ChargAltWaitTimer);
                    UTILS_ResetTimer(&_ChargAltMonTimer);
                    _StateCA = STATE_CHG_ALT_CMD_OFF;
                }
                break;

            case STATE_CHG_ALT_CMD_OFF:
                _bChargAltOut = false;
                if((_EngineMon.IsEngineOn() == 0U) || (_bStopChargAltPulse))
                {
                    _bStopChargAltPulse = false;
                    _ChargAltFail = false;
                    UTILS_DisableTimer(&_ChargAltMonTimer);
                    _StateCA = STATE_CHG_ALT_OFF;
                }
                else if(_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::CA_UV].bResultLatched)
                {
                    _ChargAltFail = true;
                    UTILS_DisableTimer(&_ChargAltMonTimer);
                    _StateCA = STATE_CHG_ALT_ERR;
                }
                else if((UTILS_GetElapsedTimeInSec(&_ChargAltMonTimer) >= 
                            _cfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_DELAY))
                                || (_cfgz.GetCFGZ_Param(CFGZ::ID_CHARGE_ALT_MON_FAIL_ACTION) == CFGZ::CFGZ_ACTION_NONE)
                                || _bStartChargAltPulse)
                {
                    _bStartChargAltPulse = false;
                    UTILS_DisableTimer(&_ChargAltMonTimer);
                    UTILS_ResetTimer(&_ChargAltPulseTimer);
                    _StateCA = STATE_CHG_ALT_CMD_ON;
                }
                break;

            case STATE_CHG_ALT_ERR:
                _bChargAltOut = false;
                if(_EngineMon.IsEngineOn() == 0U)
                {
                    _ChargAltFail = false;
                    _StateCA = STATE_CHG_ALT_OFF;
                }
                else if(!_GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::CA_UV].bResultInstant)
                {
                    _ChargAltFail = false;
                    _StateCA = STATE_CHG_ALT_CMD_OFF;
                    _GCUAlarms.ArrAlarmMonitoring[GCU_ALARMS::CA_UV].bResultLatched = 0;
                }
                break;
            default:
                break;
        }
    }
}

bool CHARGING_ALT::IsChargAltFail()
{
    return _ChargAltFail;
}

/*RunPIDLoop is called every 200us.*/
void CHARGING_ALT::RunPIDLoop()
{
    static uint8_t u8CntCAPIDUpdate = 0;
    static float f32DutyCycle = 0;

    u8CntCAPIDUpdate++;

    if(u8CntCAPIDUpdate >= CHRG_ALT_PID_UPDATE_CNT)
    {
        u8CntCAPIDUpdate =0;
//        if(!_bChargAltOut)
//        {
//            _f32FilteredAltCurrent   = 0;
//            _u16TicksInCurrentWindow = 0;
//            f32DutyCycle = 0;
//            /*Turn off HSD*/
//            _hal.hsdManager.AltExcitation.TurnOff();
//            return;
//        }

           /*Get current from analog input*/
           float fIntaneousAltCurrent = _hal.AnlgIp.GetChargingAltCurrent_A().f32InstSensorVal;
           /*Apply IIR filter*/
           _f32FilteredAltCurrent = (CHRG_ALT_IIR_FILT_CONST*_f32FilteredAltCurrent) +
                                           ((1-CHRG_ALT_IIR_FILT_CONST)*fIntaneousAltCurrent);
//         if(_f32FilteredAltCurrent > 200.0f)
//          _f32FilteredAltCurrent = _f32FilteredAltCurrent -200.0;
           /*Determine the previous error*/
           float f32PrevError = CHRG_ALT_SET_VAL - _f32FilteredAltCurrent;


           /*Determine the current error*/
           float f32CurrentError = CHRG_ALT_SET_VAL - _f32FilteredAltCurrent;
           /*Determine the differential term*/
           float f32Differential = f32CurrentError - f32PrevError;
           /*Determine the integral term*/
           _f32Integral += f32CurrentError;


           /*Integral windup*/
           if((_f32Integral*CHRG_ALT_KI) > CHRG_MAX_DUTY)
           {
               _f32Integral = (CHRG_MAX_DUTY/CHRG_ALT_KI);
           }
           else if(_f32Integral < 0)
           {
               _f32Integral = 0;
           }
           /*Determine the duty from controller transfer function*/
           f32DutyCycle = (f32CurrentError * CHRG_ALT_KP)
                                   + (_f32Integral*CHRG_ALT_KI)
                                       + (f32Differential * CHRG_ALT_KD);
    }

  // f32DutyCycle = 0;
    if(!_bChargAltOut)
    {
//        _f32FilteredAltCurrent   = 0;
        _u16TicksInCurrentWindow = 0;
        f32DutyCycle = 0;
        /*Turn off HSD*/
        _hal.hsdManager.AltExcitation.TurnOff();
        //  return;
    }
    else
    {
        if(f32DutyCycle <= _u16TicksInCurrentWindow)
        {
            /*Turn off HSD*/
            _hal.hsdManager.AltExcitation.TurnOff();
        }
        else
        {
            /*Turn on HSD*/
            _hal.hsdManager.AltExcitation.TurnOn();
        }
        _u16TicksInCurrentWindow++;

        _u16TicksInCurrentWindow %= (uint16_t)CHRG_MAX_DUTY;
    }
}

static void prvTimerCb()
{

    if(pChrgAlt != NULL)
    {
        pChrgAlt->RunPIDLoop();
    }
}

void CHARGING_ALT::StartExcitation()
{
    _bStartChargAltPulse = true;
    _bStopChargAltPulse = false;
}

void CHARGING_ALT::StopExcitation()
{
    _bStopChargAltPulse = true;
    _bStartChargAltPulse = false;
}

void CHARGING_ALT::ClearChargAltFail()
{
    _ChargAltFail = false;
}
