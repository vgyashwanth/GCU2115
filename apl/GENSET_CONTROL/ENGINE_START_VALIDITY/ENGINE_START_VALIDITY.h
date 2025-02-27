/*
 * ENGINE_START_VALIDITY.h
 *
 *  Created on: 03-Jun-2022
 *      Author: madhuri.abhang

 *  Modification History:
 *  Shubham Wader 22.09.2022
 *  -Rearrenged code neately.
 */

#ifndef APL_GENSET_CONTROL_ENGINE_START_VALIDITY_ENGINE_START_VALIDITY_H_
#define APL_GENSET_CONTROL_ENGINE_START_VALIDITY_ENGINE_START_VALIDITY_H_
#include "stdio.h"
#include "HAL_Manager.h"
#include "../../apl/CFGZ/CFGZ.h"
#include "GCU_ALARMS.h"



class ENGINE_START_VALIDITY
{
    /* MACRO */
    /* While start operation , Expected minimum time that should be taken by engine
        to traverse from lower threshold speed to higher threshold. If higher speed achieved
        before that, the engine start process is been tampered. */
#define MINIMUM_RAMP_TIME_FOR_SPEED      (100U) /* 100 mili sec */
#define MINIMUM_RAMP_TIME_FOR_VOLT       (200U) /* 200 mili sec */

    /* General constant values to be considered to check start validity , if user sets unpractical value */
#define CONSTANT_LOWER_SPEED_THRESHOLD         (600U)  /* rpm */
#define CONSTANT_UPPER_SPEED_THRESHOLD         (1200U) /* rpm */

    /* Public class restricted enum */
    /* Start Validity State Machine States */
    typedef enum
    {
        SV_SM_IDLE = 0,           /* Start Validity State Machine Idle */
        SV_SM_CHECK_VALID_START,
        SV_SM_VALIDITY_DETECTION_COMPLETE,
        SV_SM_RESET,
        SV_SM_LAST
    }START_VALIDITY_SM_STATE_t;

public:

    ENGINE_START_VALIDITY(CFGZ &cfgz, GCU_ALARMS &GCUAlarms); /* constructor */

    /* PUBLIC FUNCTIONS */
    void InitEngineStartValidityConfig();
    void EngineStartValiditySM(bool bDeviceInConfigMode);

    /* Shubham Wader 22.09.2022
   Below prototypes and variables made static intentionally to expand
   scope of accessibility without object. */
    static bool IsEngineStartValidityDetectionEnabled();
    static bool IsValidEngineStartFound();

    /* PUBLIC variables */
    static bool bStartValidDetectionEnabled;
    static bool bFoundValidEngineStart;

private:
    /* Private object references */
    CFGZ            &_cfgz;
    GCU_ALARMS      &_GCUAlarms;

    /* Private Timer Instances */
    stTimer         _SpeedRampDetectTimer;
    stTimer         _VoltRampDetectTimer;

    /* Private variables */
    START_VALIDITY_SM_STATE_t  _eValidSpeedRampDetectionState;
    START_VALIDITY_SM_STATE_t  _eValidVoltRampDetectionState;
    uint16_t        _u16LowerSpeedThreshold_rpm;
    uint16_t        _u16HigherSpeedThreshold_rpm;

    bool _bFoundValidSpeedRamp;
    bool _bFoundValidVoltRamp;

    void prvIsSpeedRampValid();
    void prvIsVoltRampValid();

};

#endif /* APL_GENSET_CONTROL_ENGINE_START_VALIDITY_ENGINE_START_VALIDITY_H_ */
