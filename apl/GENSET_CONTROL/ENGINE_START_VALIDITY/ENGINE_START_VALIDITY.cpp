/*
 * ENGINE_START_VALIDITY.cpp
 *
 *  Created on: 03-Jun-2022
 *      Author: madhuri.abhang

 *  Modification History:
 *  Shubham Wader 22.09.2022
 *  -Rearrenged code neately.
 */


#include "ENGINE_START_VALIDITY.h"
#include "START_STOP.h"

//-------------------------------------------------------------------------------------------------/

/* Supportive macros */

#define IS_ENGINE_START_VALIDITY_ENABLED()       (_cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_ALT_WAVE_DETECTION) == CFGZ::CFGZ_ENABLE)
#define IS_GEN_IN_START_STATE()                  ((START_STOP::GetStartStopSMDState() == START_STOP::ID_STATE_SS_CRANKING)   || \
                                                  (START_STOP::GetStartStopSMDState() == START_STOP::ID_STATE_SS_CRANK_REST) || \
                                                  (START_STOP::GetStartStopSMDState() == START_STOP::ID_STATE_SS_ENG_ON))
#define IS_UNDER_SPEED_DETECTION_ENABLED()       (CFGZ::CFGZ_ENABLE == _cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_SHUTDOWN))
#define IS_ENGINE_TURNED_OFF()                   (START_STOP::GetStartStopSMDState() == START_STOP::ID_STATE_SS_ENG_OFF_OK)
#define GET_CONFIGURED_LOWER_SPEED()             (_cfgz.GetCFGZ_Param(CFGZ::ID_CRANK_DISCONN_ENGINE_SPEED))
#define GET_CONFIGURED_UPPER_SPEED()             (_cfgz.GetCFGZ_Param(CFGZ::ID_SPEED_MONITOR_UNDER_SPEED_THRESHOLD))
//#define GET_CURRENT_SPEED()                      (_GCUAlarms.GetSpeedValue())
#define GET_CURRENT_SPEED()                      (_GCUAlarms.GetInvalidDGSpeedValue())
#define GET_CURRENT_VOLTAGE()                    (_GCUAlarms.GetRawVoltageValue())
/* Time until which validity detection to be performed */
#define TIMEOUT_FOR_RAMP_DETECTION         (_cfgz.GetCFGZ_Param(CFGZ::ID_CRANKING_TIMER_CRANK_HOLD_TIME))

#define LOWER_VOLTAGE_THRESH 40U
#define HIGHER_VOLTAGE_THRESH 180U

bool ENGINE_START_VALIDITY::bStartValidDetectionEnabled = false;
bool ENGINE_START_VALIDITY::bFoundValidEngineStart = true;

ENGINE_START_VALIDITY::ENGINE_START_VALIDITY(CFGZ &cfgz, GCU_ALARMS &GCUAlarms):
_cfgz(cfgz),
_GCUAlarms(GCUAlarms),
_SpeedRampDetectTimer{0, false},
_eValidSpeedRampDetectionState(SV_SM_IDLE),
_eValidVoltRampDetectionState(SV_SM_IDLE),
_u16LowerSpeedThreshold_rpm(0U),
_u16HigherSpeedThreshold_rpm(0U),
_bFoundValidSpeedRamp(true),
_bFoundValidVoltRamp(true)
{
    InitEngineStartValidityConfig();
}

void ENGINE_START_VALIDITY::InitEngineStartValidityConfig()
{
    if(IS_ENGINE_START_VALIDITY_ENABLED())
    {
        if(CONSTANT_LOWER_SPEED_THRESHOLD > GET_CONFIGURED_LOWER_SPEED())
        {
            _u16LowerSpeedThreshold_rpm = CONSTANT_LOWER_SPEED_THRESHOLD;
        }
        else
        {
            _u16LowerSpeedThreshold_rpm = GET_CONFIGURED_LOWER_SPEED();
        }

        if((CONSTANT_UPPER_SPEED_THRESHOLD < GET_CONFIGURED_UPPER_SPEED()) &&
            IS_UNDER_SPEED_DETECTION_ENABLED())
        {
            _u16HigherSpeedThreshold_rpm = GET_CONFIGURED_UPPER_SPEED();
        }
        else
        {
            _u16HigherSpeedThreshold_rpm = CONSTANT_UPPER_SPEED_THRESHOLD;
        }
    }
    else
    {
        /* No need to initialize anything */
    }
}

void ENGINE_START_VALIDITY:: EngineStartValiditySM(bool bDeviceInConfigMode)
{
    if(!bDeviceInConfigMode)
    {
        prvIsSpeedRampValid();
        prvIsVoltRampValid();
       // bFoundValidEngineStart = (_bFoundValidSpeedRamp || _bFoundValidVoltRamp);
        bFoundValidEngineStart = _bFoundValidSpeedRamp;
    }
    else
    {
        /* no execution in config mode */
    }
}

void ENGINE_START_VALIDITY::prvIsSpeedRampValid()
{
    switch(_eValidSpeedRampDetectionState)
    {
        case SV_SM_IDLE :
        {
            if(IS_ENGINE_START_VALIDITY_ENABLED())
            {
                bStartValidDetectionEnabled = true;

                if(GET_CURRENT_SPEED() > _u16LowerSpeedThreshold_rpm)
                {
                    UTILS_ResetTimer(&_SpeedRampDetectTimer);
                    _eValidSpeedRampDetectionState = SV_SM_CHECK_VALID_START;
                }
            }
            else
            {
                /* If validity check is disabled, every start to be considered as valid start.
                          Skip validity check execution  */
                bStartValidDetectionEnabled = false;
                _bFoundValidSpeedRamp = true;
                return;
            }
        }
        break;

        case SV_SM_CHECK_VALID_START :
        {
            if(GET_CURRENT_SPEED() >= _u16HigherSpeedThreshold_rpm)
            {
                if(UTILS_GetElapsedTimeInMs(&_SpeedRampDetectTimer) >= MINIMUM_RAMP_TIME_FOR_SPEED)
                {
                    _bFoundValidSpeedRamp = true;
                    _eValidSpeedRampDetectionState = SV_SM_VALIDITY_DETECTION_COMPLETE;
                }
                else
                {
                    _bFoundValidSpeedRamp = false;
                    _eValidSpeedRampDetectionState = SV_SM_VALIDITY_DETECTION_COMPLETE;
                }
            }
            else if((UTILS_GetElapsedTimeInSec(&_SpeedRampDetectTimer) >= TIMEOUT_FOR_RAMP_DETECTION) &&
                    (GET_CURRENT_SPEED() >= _u16LowerSpeedThreshold_rpm))
            {
                /* This instance of code shows that, even if engine is unable to cross upper threshold of speed,
                        has at least maintained speed greater than lower speed threshold for the configured portion of time.
                        Hence, Engine will enter in ON state anyway and GCU considers this as a valid start. */
                _bFoundValidSpeedRamp = true;
                _eValidSpeedRampDetectionState = SV_SM_VALIDITY_DETECTION_COMPLETE;
            }
            else if(UTILS_GetElapsedTimeInSec(&_SpeedRampDetectTimer) >= TIMEOUT_FOR_RAMP_DETECTION)
            {
                /* execution reaches here, that means, engine is unable to gain required minimum
                           speed within given portion of timeout(crank hold time).
                           So anyway GCU will start next attempt of crank until configured number of
                           cranks completes.
                           Hence reset the parameters and then wait in SV_SM_IDLE state for next attempt. */
                _eValidSpeedRampDetectionState = SV_SM_RESET;
            }
            else
            {
                /* do nothing */
            }
        }
        break;
        case SV_SM_VALIDITY_DETECTION_COMPLETE :
        {
             if(IS_ENGINE_TURNED_OFF())
            {
                _eValidSpeedRampDetectionState = SV_SM_RESET;
            }
            else
            {
                /* execution will remain in same state until engine gets off. */
            }
        }
        break;
        case SV_SM_RESET :
        {
            /* Execution will come here only when engine is in off state or unable to start */
            UTILS_DisableTimer(&_SpeedRampDetectTimer);
            _bFoundValidSpeedRamp = true;
            _eValidSpeedRampDetectionState = SV_SM_IDLE;
        }
        break;

        default:
            break;
    }
}

void ENGINE_START_VALIDITY::prvIsVoltRampValid()
{
    switch(_eValidVoltRampDetectionState)
    {
        case SV_SM_IDLE :
        {
            if(IS_ENGINE_START_VALIDITY_ENABLED())
            {
                bStartValidDetectionEnabled = true;

                if(GET_CURRENT_VOLTAGE() > LOWER_VOLTAGE_THRESH)
                {
                    UTILS_ResetTimer(&_VoltRampDetectTimer);
                    _eValidVoltRampDetectionState = SV_SM_CHECK_VALID_START;
                }
            }
            else
            {
                /* If validity check is disabled, every start to be considered as valid start.
                          Skip validity check execution  */
                bStartValidDetectionEnabled = false;
                _bFoundValidVoltRamp = true;
                return;
            }
        }
        break;

        case SV_SM_CHECK_VALID_START :
        {
            if(GET_CURRENT_VOLTAGE() >= HIGHER_VOLTAGE_THRESH)
            {
                if(UTILS_GetElapsedTimeInMs(&_VoltRampDetectTimer) >= MINIMUM_RAMP_TIME_FOR_VOLT)
                {
                    _bFoundValidVoltRamp = true;
                    _eValidVoltRampDetectionState = SV_SM_VALIDITY_DETECTION_COMPLETE;
                }
                else
                {
                    _bFoundValidVoltRamp = false;
                    _eValidVoltRampDetectionState = SV_SM_VALIDITY_DETECTION_COMPLETE;
                }
            }
            else if((UTILS_GetElapsedTimeInSec(&_VoltRampDetectTimer) >= TIMEOUT_FOR_RAMP_DETECTION) &&
                    (GET_CURRENT_VOLTAGE() >= LOWER_VOLTAGE_THRESH))
            {
                /* This instance of code shows that, even if engine is unable to cross upper threshold of speed,
                        has at least maintained speed greater than lower speed threshold for the configured portion of time.
                        Hence, Engine will enter in ON state anyway and GCU considers this as a valid start. */
                _bFoundValidVoltRamp = true;
                _eValidVoltRampDetectionState = SV_SM_VALIDITY_DETECTION_COMPLETE;
            }
            else if(UTILS_GetElapsedTimeInSec(&_VoltRampDetectTimer) >= TIMEOUT_FOR_RAMP_DETECTION)
            {
                /* execution reaches here, that means, engine is unable to gain required minimum
                           speed within given portion of timeout(crank hold time).
                           So anyway GCU will start next attempt of crank until configured number of
                           cranks completes.
                           Hence reset the parameters and then wait in SV_SM_IDLE state for next attempt. */
                _eValidVoltRampDetectionState = SV_SM_RESET;
            }
            else
            {
                /* do nothing */
            }
        }
        break;
        case SV_SM_VALIDITY_DETECTION_COMPLETE :
        {
            if(IS_ENGINE_TURNED_OFF())
            {
                _eValidVoltRampDetectionState = SV_SM_RESET;
            }
            else
            {
                /* execution will remain in same state until engine gets off. */
            }
        }
        break;
        case SV_SM_RESET :
        {
            /* Execution will come here only when engine is in off state or unable to start */
            UTILS_DisableTimer(&_VoltRampDetectTimer);
            _bFoundValidVoltRamp = true;
            _eValidVoltRampDetectionState = SV_SM_IDLE;
        }
        break;

        default:
            break;
    }
}

bool ENGINE_START_VALIDITY::IsEngineStartValidityDetectionEnabled()
{
    /* return true : if Engine start validity detection is enabled
       return false: if Engine start validity detection is disabled */
    return bStartValidDetectionEnabled;
}

bool ENGINE_START_VALIDITY::IsValidEngineStartFound()
{
    /* return true : if valid engine start observed
       return false: if invalid engine start observed */
    return bFoundValidEngineStart;
}
