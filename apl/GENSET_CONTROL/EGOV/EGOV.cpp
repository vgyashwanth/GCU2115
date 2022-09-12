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

    UTILS_DisableTimer(&_PIDOnTimer);
    UTILS_DisableTimer(&_EngineOnTimer);
    UTILS_ResetTimer(&_SMUpdateTimer);
    _hal.AnalogSensors.RegisterPulseInputUpdateCB(UpdateSensIpStatus);
    _hal.AnalogSensors.RegisterCompInputUpdateCB(UpdateCompIpStatus);
   InitEgovParameters();
}


void EGOV::InitEgovParameters()
{}

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
{}

void EGOV::UpdateEgovCalc(void)
{}

void EGOV::prvPIDLoop(void)
{}

void EGOV::prvStartStopActionUsingEgov(void)
{}

void EGOV::prvResetEgovParam(void)
{}

void EGOV::prvUpdateCrankBoostStatus()
{}

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
    return false;
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
