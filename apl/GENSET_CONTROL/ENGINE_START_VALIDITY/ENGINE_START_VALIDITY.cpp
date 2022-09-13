/*
 * ENGINE_START_VALIDITY.cpp
 *
 *  Created on: 03-Jun-2022
 *      Author: madhuri.abhang
 */


#include "ENGINE_START_VALIDITY.h"
#include "START_STOP.h"

//-------------------------------------------------------------------------------------------------/

bool ENGINE_START_VALIDITY::_bEngineStartWasInValid = false;
bool ENGINE_START_VALIDITY::_bStartWaveDetection = false;

ENGINE_START_VALIDITY::ENGINE_START_VALIDITY(CFGZ &cfgz, GCU_ALARMS &GCUAlarms):
_cfgz(cfgz),
_GCUAlarms(GCUAlarms)
{
    UpdateStartValidyParam();
}


void ENGINE_START_VALIDITY:: InitEngineStartValidityParams(void)
{
    InitInvalidDgDetectionStateMachine();
    _bEngineStartWasInValid = false;

    /*
     * Lower rpm threshold is assigned as  -
     * either - 600rpm           - when crank disconnect rpm threshold is greater than 600
     * or     - crank disconnect - when crank disconnect rpm threshold is less than 600
     */
    if(RPM_THRESHOLD_LOW_CONST < _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_ENGINE_SPEED))
    {
        _u16LowRpmThreshold = RPM_THRESHOLD_LOW_CONST;
    }
    else
    {
        _u16LowRpmThreshold = _cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONNECT_ENGINE_SPEED);
    }

    /*
     * Higher rpm threshold is assigned as  -
     * either - 1200rpm     - when under speed threshold is less than 1200
     * or     - under speed - when under speed threshold is greater than 1200
     */
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD)  < RPM_THRESHOLD_HIGH_CONST)
    {
        _u16HighRpmThreshold = RPM_THRESHOLD_HIGH_CONST;
    }
    else
    {
        _u16HighRpmThreshold = _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD);
    }

    /*
     * Crank hold time is considered as timeout for rpm to reach from lower threshold to higher.
     * After this time, engine start is considered as valid start.
     */
    _u32TimeoutForRpm1ToRpm2_us = (CNTS_FOR_1_SEC *_cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_HOLD_TIME));
}


/**
 ** @brief  The function initializes variables to their default state for engine start validity SMD.
 **
 ** @param None
 ** @return void
 **/

void ENGINE_START_VALIDITY::InitInvalidDgDetectionStateMachine(void)
{
    _bStrtTmrToDetectInvalidDgRun = false;

    _u32TimeFromRpm1ToRpm2_ms = 0;
    _bStartWaveDetection = false;
    _eInvalidDgDetectionState = INVALID_DG_RUN_IDLE;
}

/*
////////////////////////////////////////////////////////////////////////////////////////////////////
 **
 ** @brief  The function gives latest engine start validity status.
 **
 ** @param None
 ** @return bool - Invalid engine start status
 **                 true - Engine start was invalid
 **                 false - Engine start was valid
 **/
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ENGINE_START_VALIDITY::GetEngineStartInvalidity(void)
{
    return _bEngineStartWasInValid;
}

/*
////////////////////////////////////////////////////////////////////////////////////////////////////
 **
 ** @brief  The function sets latest engine start validity status.
 **
 ** @param bool - Invalid engine start status
 **                 true - Engine start was invalid
 **                 false - Engine start was valid
 ** @return void
 **/
////////////////////////////////////////////////////////////////////////////////////////////////////
void ENGINE_START_VALIDITY::SetEngineStartInvalidity(bool bStatus)
{
    _bEngineStartWasInValid = bStatus;
}

/*
////////////////////////////////////////////////////////////////////////////////////////////////////
 **
 ** @brief  The function gives status whether start wave is detected or not.
 **
 ** @param None
 ** @return bool - Start Wave Detection status
 **                 true - Start wave detected.
 **                 false - Start wave is not detected.
 **/
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ENGINE_START_VALIDITY::GetStartWaveDetectionStatus(void)
{
    return _bStartWaveDetection;
}

/*
////////////////////////////////////////////////////////////////////////////////////////////////////
 **
 ** @brief  The function sets Start Wave Detection status.
 **
 ** @param bool - Start Wave Detection status
 **                 true - Start wave detected.
 **                 false - Start wave is not detected
 ** @return void
 **/
////////////////////////////////////////////////////////////////////////////////////////////////////
void ENGINE_START_VALIDITY::SetStartWaveDetectionStatus(bool bStatus)
{
    _bStartWaveDetection = bStatus;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 ** @brief  This is SMD to detect engine start validity. It has two states -
 **         1. INVALID_DG_RUN_IDLE - This is default state of SMD. Cranking or engine on conditions
 **            are checked. If any of the condition mets then it checks for lower rpm threshold.
 **            Once rpm reaches to lower threshold then timer has started and SMD is moves forward state.
  **        2. INVALID_DG_RUN_CHECK_HIGH_THRSH - In this state, engine rpm is checked with higher threshold.
 **             If it reaches to higher threshold within 100msec, then engine start is declared as invalid.
 **             If it reaches to higher threshold after 100msec and it is greater than lower threshold,
 **             then engine start is declared as valid.
 **             If it does not reach to higher threshold till crank hold time, it is declared that engine
 **             is not even started.
 **             Previously there were 3 states in this SMD. There was no exit criteria for middle state,
 **             hence it is decided to have 2 state for this SMD.
 **
 ** @param  none
 ** @return void
 **/
////////////////////////////////////////////////////////////////////////////////////////////////////
void ENGINE_START_VALIDITY::SMDToChkEngStartValidity(void)
{
    switch(_eInvalidDgDetectionState)
    {
        case INVALID_DG_RUN_IDLE :
            if((START_STOP::GetStartStopSMDState() == START_STOP::ID_STATE_SS_CRANKING)        ||
               (START_STOP::GetStartStopSMDState() == START_STOP::ID_STATE_SS_CRANK_REST)      ||
               (START_STOP::GetStartStopSMDState() == START_STOP::ID_STATE_SS_ENG_ON))
            {
                if (_GCUAlarms.GetSpeedValue() >= _u16LowRpmThreshold)
                {
                    _u32TimeFromRpm1ToRpm2_ms = 0;
                    _bStrtTmrToDetectInvalidDgRun = true;
                    _eInvalidDgDetectionState = INVALID_DG_RUN_CHECK_HIGH_THRSH;
                }
            }
            break;
        case INVALID_DG_RUN_CHECK_HIGH_THRSH :
#ifdef DEBUG_RPM_TIME_ON_MB
                gu32DbgTimeFromRpm1ToRpm2_ms = _u32TimeFromRpm1ToRpm2_ms;
#endif
            // Does engine rpm reaches to higher threshold within 100msec
            if (_GCUAlarms.GetSpeedValue() >= _u16HighRpmThreshold)
            {
                // If engine rpm reaches to higher threshold within 100msec then it is invalid start
                if (_u32TimeFromRpm1ToRpm2_ms >= MIN_CNTS_FOR_RPM1TORPM2)
                {
                    // engine start was valid
                    _bEngineStartWasInValid = false;
                    _bStartWaveDetection = true;
                }
                else
                {
                    // someone is cheating
                    _bEngineStartWasInValid = true;
                    _bStartWaveDetection = true;
                }
                _eInvalidDgDetectionState = INVALID_DU_RUN_FALSE;
                _bStrtTmrToDetectInvalidDgRun = false;
            }
            // If engine rpm does not reach to the HIGH threshold in crank hold time, and
            // engine rpm is greater than lower threshold then it is considered as Valid start
            else if((_u32TimeFromRpm1ToRpm2_ms >= _u32TimeoutForRpm1ToRpm2_us)  &&
                    (_GCUAlarms.GetSpeedValue() > _u16LowRpmThreshold))
            {
                // engine start was valid
                _bEngineStartWasInValid = false;
                _bStartWaveDetection = true;
                _eInvalidDgDetectionState = INVALID_DU_RUN_FALSE;
                _bStrtTmrToDetectInvalidDgRun = false;
            }
            // If engine rpm does not reach to the HIGH threshold in crank hold time, and
            // engine rpm is reduced below lower threshold then do nothing and reinit SMD.
            else if(_u32TimeFromRpm1ToRpm2_ms >= _u32TimeoutForRpm1ToRpm2_us)
            {
                InitInvalidDgDetectionStateMachine();
            }
            break;
        case INVALID_DU_RUN_FALSE :
            break;
    }
}

void ENGINE_START_VALIDITY::UpdateStartValidyParam()
{
    if(_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_WAVE_DETECTION) == CFGZ::CFGZ_ENABLE)
    {
        InitEngineStartValidityParams();
    }
    else
    {
        SetStartWaveDetectionStatus(true);
    }
}

