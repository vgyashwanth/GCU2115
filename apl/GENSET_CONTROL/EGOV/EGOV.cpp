/*
 * EGOV.cpp
 *
 *  Created on: 02-Dec-2021
 *      Author: nikhil.mhaske
 */
#include "EGOV.h"
#include "START_STOP.h"


bool EGOV::_bSensIpRPMUpdate = false;
bool EGOV::_bCompartorIpRPMUpdate = false;
bool EGOV::_bResetEgovVariables = false;
uint8_t EGOV::_u8ComparatorCBCount = 0;


EGOV::EGOV(HAL_Manager &hal, CFGZ &cfgz, ENGINE_MONITORING  &engMon, START_STOP &StartStop):
_hal(hal),
_cfgz(cfgz),
_engMon(engMon),
_StartStop(StartStop),
_bUseErrSlopeChangePt(false),
_bPidOn(false),
_bStartInitiated(false),
_bStopInitiated(false),
_bFixedStepExecuted(false),
_bFixedStepForStopExecuted(false),
_bSwitchedToPID(false),
_f32RPMAvg(0),
_f32OldRpm(0),
_f32TargetRPM(1500),
_f32SetTargetRPM(1500),
_f32ErrorSlopechangePoint(0),
_f32OldParallelTargetRPM{0},
_f32FiltParallelTargetRPM{0},
_f32ArrGain{25.0f,0.0f,0.1f,30.0f,0.1f,1.0f,1.0f},
_f32ErrorP(0),
_f32ErrorD(0),
_f32ErrorPOld(0),
_f32ErrorI(0),
_f32DeltaU(0),
_f32U(0),
_f32UPrev(0),
_arrU16RPMTable{1400,1420,1440,1460,1480,1500,1520,1540,1560,1580,1600},
_EngineOnTimer{0},
_PIDOnTimer{0},
_SMUpdateTimer{0},
_eEgovMode(ID_PID_ISOCHRONOUS)
{
    _hal.stpr.ConfigureStepWidth(_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_SPEED) * 25);
    UTILS_DisableTimer(&_PIDOnTimer);
    UTILS_DisableTimer(&_EngineOnTimer);
    UTILS_ResetTimer(&_SMUpdateTimer);
    _hal.AnalogSensors.RegisterPulseInputUpdateCB(UpdateSensIpStatus);
    _hal.AnalogSensors.RegisterCompInputUpdateCB(UpdateCompIpStatus);
    _hal.actuators.STPR_ConfigDirection((STPR_DRV::DRV_DIR_t)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_DIRECTION));
    InitEgovParameters();
}


void EGOV::InitEgovParameters()
{
    uint16_t u16RPMLowLimit=0 , u16RPMHighLimit=0 , u16RPMStep=0;


        _f32ArrGain[P_GAIN] = ((float)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_KP))*0.01f;
        _f32ArrGain[I_GAIN] = ((float)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_KI))*0.001f;
        _f32ArrGain[D_GAIN] = ((float)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_KD))*0.01f;
        _f32ArrGain[FRICTION_SET_OFF] = ((float)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_DITHER))*0.01f;
        _f32ArrGain[GAIN_SCHEDULE_TRIGGER] =  ((float)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_GAIN_SCHEDULE))*0.01f;
        _f32ArrGain[LOADING_FACTOR] = ((float)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_LOADING_FAC))*0.01f;
        _f32ArrGain[UNLOADING_FACTOR] = ((float)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_UNLOADING_FAC))*0.01f;
        _f32SetTargetRPM =  (float)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_SET_SPEED);
        u16RPMLowLimit = (_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_SET_SPEED)-100);
        u16RPMHighLimit = (_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_SET_SPEED)+100);
        _f32TargetRPM = _cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_SET_SPEED);
        _f32OldParallelTargetRPM = _cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_SET_SPEED);
        _f32FiltParallelTargetRPM = _cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_GEN_SET_SPEED);

    _eEgovMode = (EGOV_MODE_t)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_SET_POINT_SELECTION);

    u16RPMStep = (u16RPMHighLimit - u16RPMLowLimit)/10;
    for(uint8_t i = 0;i < 11 ; i++)
    {
        _arrU16RPMTable[i] = u16RPMLowLimit + (u16RPMStep*i);
    }
    prvResetEgovParam();
}

void EGOV::UpdateSensIpStatus(float f32RPM)
{
   _bSensIpRPMUpdate = true;
}

void EGOV::UpdateCompIpStatus(float f32RPM)
{
    _u8ComparatorCBCount++;
    _bCompartorIpRPMUpdate = true;
}

void EGOV::Update(void)
{
    if((UTILS_GetElapsedTimeInMs(&_SMUpdateTimer)) >= FIFTY_MSEC)
    {
        prvUpdateCrankBoostStatus();
        UTILS_ResetTimer(&_SMUpdateTimer);
    }

    if((_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE) == CFGZ::CFGZ_MAGNETIC_PICKUP)||
            (_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE) == CFGZ::CFGZ_W_POINT_FREQ))
    {
        if(_bSensIpRPMUpdate)
        {
            _engMon.UpdateEngineOnStatus();
            _bSensIpRPMUpdate = false;
            UpdateEgovCalc();
        }
    }
    else if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE) == CFGZ::CFGZ_ALT_FREQUENCY)
    {
        if(_bCompartorIpRPMUpdate)
        {
            _engMon.UpdateEngineOnStatus();
            _bCompartorIpRPMUpdate= false;
           UpdateEgovCalc();
        }
   }

   if(_bResetEgovVariables)
   {
       _bResetEgovVariables = false;
       prvResetEgovParam();
   }
}

void EGOV::UpdateEgovCalc(void)
{
    _f32RPMAvg = (_engMon.GetRawEngSpeed() + _f32OldRpm) / 2U;
    _f32OldRpm = _engMon.GetRawEngSpeed();

    if(_bUseErrSlopeChangePt == false)
    {
       if(_f32RPMAvg > _f32SetTargetRPM)
       {
           _bUseErrSlopeChangePt = true;
       }
    }

    if((_bPidOn == false) && (_bStartInitiated == true))
    {
       //check engine on time for PID ON time & if that time is over then set pid_on flag.
       if((uint16_t)_f32RPMAvg >= _cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_RPM_PID_ON_SPEED))
       {
       /* Start of change 2 of 4 on 14 Aug 2014 for KOEL startability test */
           _hal.stpr.ConfigureStepWidth(_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_SPEED) * 25);
       /* End of change 2 of 4 on 14 Aug 2014 for KOEL startability test */
           _bPidOn = true;
           _hal.stpr.Activate();
           UTILS_ResetTimer(&_PIDOnTimer);
       }
       /* Start of change 3 of 4 on 14 Aug 2014 for KOEL startability test */
       else
       {
           prvStartStopActionUsingEgov();
       }
       /* End of change 3 of 4 on 14 Aug 2014 for KOEL startability test */
    }
    else
    {
        if(START_STOP::IsStopRelayON())
        {
            // Do nothing
        }
        else
        {
            if((_eEgovMode == ID_PID_ISOCHRONOUS)||(_eEgovMode == ID_PID_PARALLELING)
                        || (_eEgovMode == ID_PID_DROOP))
            {
                prvPIDLoop();
            }
            else if(_eEgovMode == ID_EGOV_AS_START_STOP)
            {
                    prvStartStopActionUsingEgov();
            }
            else
            {
                //Do nothing
            }
        }
    }

    if(UTILS_GetElapsedTimeInSec(&_PIDOnTimer) <= (_cfgz.GetCFGZ_Param(CFGZ::ID_STARTUP_IDLE_TIME)))
    {
       _f32TargetRPM = _cfgz.GetCFGZ_Param(CFGZ::ID_INITIAL_LOW_SPEED);
    }
    else if(UTILS_GetElapsedTimeInSec(&_PIDOnTimer) <= (_cfgz.GetCFGZ_Param(CFGZ::ID_STARTUP_IDLE_TIME)
                        + _cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_RAMP_UP_TIME)))
    {
       _f32TargetRPM = _cfgz.GetCFGZ_Param(CFGZ::ID_INITIAL_LOW_SPEED)
               + ((UTILS_GetElapsedTimeInSec(&_PIDOnTimer) - _cfgz.GetCFGZ_Param(CFGZ::ID_STARTUP_IDLE_TIME))
                   *((_f32SetTargetRPM - _cfgz.GetCFGZ_Param(CFGZ::ID_INITIAL_LOW_SPEED))
                           /_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_RAMP_UP_TIME)));
    }
    else if((_eEgovMode == ID_PID_PARALLELING) || (_eEgovMode == ID_AUTO_TUNE_PARALLELING))
    {
        uint16_t              u16IndexATD = 0, u16IndexOffATDTable = 0;
        ANLG_IP::ANALOG_VAL_t stSensVal =_hal.AnlgIp.GetPin23Value(ANLG_IP::REF_BATT_NEGATIVE);
        float                 f32AtdValue = stSensVal.f32InstSensorVal * 1000U;

        if (f32AtdValue <= ATD_VALUE_LOW)
        {
            u16IndexATD = 0U;
            u16IndexOffATDTable = 0U;
        }
        else if (f32AtdValue>=ATD_VALUE_HIGH)
        {
            u16IndexATD = ATD_TABLE_MAX_NDEX - 1U;
            u16IndexOffATDTable = 50U;
        }
        else
        {
            /* LDRA violaion 131D, Global denominator not checked for  zero*/
            if(0 != ATD_table_STEP_SIZE)
            {
                u16IndexATD = (unsigned char)((f32AtdValue - ATD_VALUE_LOW)/ATD_table_STEP_SIZE);
                u16IndexOffATDTable = (unsigned char)((((unsigned int)(f32AtdValue - ATD_VALUE_LOW - u16IndexATD*ATD_table_STEP_SIZE))*SCALE_FACT)/ATD_table_STEP_SIZE); // interpolation: should give between 1-9
            }
        }

        float f32ParallelTargetRPM = (((long)_arrU16RPMTable[u16IndexATD])*(50U - u16IndexOffATDTable) + ((long)(_arrU16RPMTable[u16IndexATD + 1U]))*u16IndexOffATDTable)/50.0f;
        // Lookup table implementation: END
        ////////////////////////////////////////////////////////////////////////////////////
        // Rate saturation for decreasing engine set speed. The value "3" means
        // 1550->820RPM takes about 12 seconds. Smaller value in place of "3" will
        // make 12s larger (slower rate will be allowed when speed decreases) & vice versa.

        if((_f32OldParallelTargetRPM - f32ParallelTargetRPM) > RATE_LIMIT_DOWN)
        {
          f32ParallelTargetRPM = _f32OldParallelTargetRPM - RATE_LIMIT_DOWN;
        }

        if((f32ParallelTargetRPM - _f32OldParallelTargetRPM) > RATE_LIMIT_UP)
        {
          f32ParallelTargetRPM = _f32OldParallelTargetRPM + RATE_LIMIT_UP;
        }
        _f32OldParallelTargetRPM = f32ParallelTargetRPM;
        // Saturation on max engine set speed
        if(f32ParallelTargetRPM > _arrU16RPMTable[10])
        {
            f32ParallelTargetRPM = _arrU16RPMTable[10];
        }
        else if (f32ParallelTargetRPM < _arrU16RPMTable[0])
        {
            f32ParallelTargetRPM = _arrU16RPMTable[0];
        }
        // Recovery mode detection. Used only when there is a frequency roll-off supported by
        // alternator. To enable recovery mode detection, put a meaningful value in RPM_RECOVERY_TRIGGER

        _f32FiltParallelTargetRPM = (0.99f)*_f32FiltParallelTargetRPM + (1-0.99f)*f32ParallelTargetRPM;
        _f32TargetRPM = _f32FiltParallelTargetRPM;
    }
    else if((ID_AUTO_TUNE_DROOP == _eEgovMode) || (ID_PID_DROOP == _eEgovMode))
    {
       _f32TargetRPM = (uint16_t)(_f32SetTargetRPM *
                       (float)((1U + ((float)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_PERCENT_DROOP)/100.0f)) -
                       ((float)_hal.AcSensors.GENSET_GetPercentPower())*
                       ((float)_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_PERCENT_DROOP)/10000.0f)));
    }
    else
    {
        _f32TargetRPM = _f32SetTargetRPM;
    }
}

void EGOV::prvPIDLoop(void)
{
    _f32ErrorP = _f32TargetRPM - _f32RPMAvg;     //error in rpm
    if(_bUseErrSlopeChangePt)
    {
        _f32ErrorSlopechangePoint = _f32ArrGain[GAIN_SCHEDULE_TRIGGER]*_f32TargetRPM;   // GST is gain[5]
        if(_f32ErrorP > _f32ErrorSlopechangePoint)
        {
            _f32ErrorP = _f32ErrorSlopechangePoint + (_f32ErrorP - _f32ErrorSlopechangePoint)*_f32ArrGain[LOADING_FACTOR];   // Loading Factor _f32ArrGain[6]
        }
        else if (_f32ErrorP < (-_f32ErrorSlopechangePoint))
        {
            _f32ErrorP = -_f32ErrorSlopechangePoint + (_f32ErrorP + _f32ErrorSlopechangePoint)*_f32ArrGain[UNLOADING_FACTOR]; // Unloading Factor _f32ArrGain[7]
        }
    }

//    if(_f32TargetRPM < _f32SetTargetRPM)
//    {
//        _f32ErrorP = _f32TargetRPM - _f32RPMAvg;
//        if (_f32ErrorP < -50)
//        {
//            _f32ErrorP = 2*_f32ErrorP;
//        }
//    }

    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ENG_SPEED_SOURCE) == CFGZ::CFGZ_ALT_FREQUENCY)
    {
        _f32ErrorD = 0.2f*(_f32ErrorP - _f32ErrorPOld) + 0.8f*(_f32ErrorDOld); //derivative of error
    }
    else
    {
        _f32ErrorD = _f32ErrorP - _f32ErrorPOld;  //derivative of error
    }

    _f32ErrorDOld = _f32ErrorD;
    _f32ErrorPOld = _f32ErrorP;
    _f32UPrev = _f32U;
    // New code added by AD on 30 Nov 2014 for engine startability
    if((UTILS_GetElapsedTimeInSec(&_EngineOnTimer) < _cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_PID_ON_TIME))
            && ((uint16_t)_f32RPMAvg < _cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_RPM_PID_ON_SPEED)))
    {
        _bSwitchedToPID = true;
    }
    else
    {
        _f32ErrorI = (float)(_f32ErrorI + (float)(_f32ArrGain[I_GAIN]*_f32ErrorP));
        _f32U = (_f32ArrGain[P_GAIN]*_f32ErrorP) + _f32ErrorI + (_f32ArrGain[D_GAIN]*_f32ErrorD) + _f32ArrGain[FRICTION_SET_OFF];
        if(_bSwitchedToPID)
        {
            _f32UPrev = _f32U;
            _bSwitchedToPID = false;
        }
    }

    _f32ArrGain[FRICTION_SET_OFF] = -_f32ArrGain[FRICTION_SET_OFF];
    _f32DeltaU = _f32U - _f32UPrev;
    if(_f32DeltaU > 0)
    {
        _hal.actuators.STPR_Rotate((uint16_t)round(abs(_f32DeltaU)), DIR_TOWARDS_START);
    }
    else
    {
        _hal.actuators.STPR_Rotate((uint16_t)round(abs(_f32DeltaU)), DIR_TOWARDS_STOP);
    }
}

void EGOV::prvStartStopActionUsingEgov(void)
{
    if((uint16_t)_f32RPMAvg >= _cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_RPM_PID_ON_SPEED))
    {
        _hal.stpr.ConfigureStepWidth((_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_SPEED)) * 25);
        _hal.stpr.Activate();
        _hal.actuators.STPR_Rotate(_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_RUNNING_STEPS), (bool)DIR_TOWARDS_START);
    }
    else
    {
        _hal.stpr.ConfigureStepWidth(_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_SPEED) * 25);
        _hal.stpr.Activate();
        _hal.actuators.STPR_Rotate(_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_CRANK_BOOST), (bool)DIR_TOWARDS_START);
    }
}

void EGOV::prvResetEgovParam(void)
{
    if(_bPidOn)
    {
        _bPidOn = false;
        _hal.stpr.Deactivate();
        _hal.actuators.STPR_Rotate(0, DIR_TOWARDS_STOP);
        _hal.stpr.ConfigureStepWidth(_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_SPEED) * 25);
        _f32TargetRPM = _f32SetTargetRPM;
        _f32FiltParallelTargetRPM = _f32SetTargetRPM;
        _f32OldRpm = 0.0f;
        _f32ErrorPOld = 0.0f;
        _f32ErrorI = 0.0f;
        _f32DeltaU = 0.0f;
        _f32U = 0.0f;
        _f32UPrev = 0.0f;
        _bUseErrSlopeChangePt = false;
        _f32OldParallelTargetRPM = _f32SetTargetRPM;
        _bFixedStepExecuted = false;
        _bFixedStepForStopExecuted = false;
    }

    if(!(START_STOP::IsStartRelayON()) && (_bFixedStepExecuted || _bFixedStepForStopExecuted))
    {
        _bFixedStepExecuted = false;
        _bFixedStepForStopExecuted = false;
        _hal.stpr.Deactivate();
    }
    UTILS_DisableTimer(&_PIDOnTimer);
    UTILS_DisableTimer(&_EngineOnTimer);
}

void EGOV::prvUpdateCrankBoostStatus()
{
    if(START_STOP::IsStopRelayON())
    {
        _hal.stpr.ConfigureStepWidth((_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_SPEED)/2) * 25);
        _hal.stpr.Activate();
        _hal.actuators.STPR_Rotate(N_PULSES_STOP, (bool)DIR_TOWARDS_STOP);
        _bStopInitiated = true;
    }
    else if(START_STOP::IsStartRelayON())
    {
        _bStartInitiated = true;
        if(!_EngineOnTimer.bEnabled)
        {
            UTILS_ResetTimer(&_EngineOnTimer);
        }
        prvStartStopActionUsingEgov();
    }
    else
    {
        if(_bStopInitiated)
        {
            _bStopInitiated = false;
            _bStartInitiated = false;
            _hal.stpr.Deactivate();
        }
        else
        {
            _hal.stpr.ConfigureStepWidth(_cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_ACT_SPEED) * 25);
        }
    }
}

float EGOV::GetEngineAvgRPM(void)
{
    return _f32RPMAvg;
}


bool EGOV::IsValidGoverningMode(void)
{
    return ((_eEgovMode == ID_PID_ISOCHRONOUS) || (_eEgovMode == ID_PID_DROOP));
}

bool EGOV::IsRampingOver(void)
{
    return ((UTILS_GetElapsedTimeInSec(&_PIDOnTimer)) > (_cfgz.GetCFGZ_Param(CFGZ::ID_STARTUP_IDLE_TIME) +
        _cfgz.GetCFGZ_Param(CFGZ::ID_EGOV_RAMP_UP_TIME)));
}

void EGOV::ResetEgovVars()
{
    _bResetEgovVariables = true;
}

uint8_t EGOV::GetCompCBCount()
{
    return _u8ComparatorCBCount;
}

void EGOV::ClearCompCBCount()
{
    _u8ComparatorCBCount = 0;
}

float EGOV::GetTragetRpm()
{
    return _f32TargetRPM;
}

float EGOV::GetSetTragetRpm()
{
    return _f32SetTargetRPM;
}

uint32_t EGOV::GetEngOnTime()
{
    return ((uint32_t)UTILS_GetElapsedTimeInSec(&_EngineOnTimer));
}

uint32_t EGOV::GetPIDOnTime()
{
    return ((uint32_t)UTILS_GetElapsedTimeInSec(&_PIDOnTimer));
}
