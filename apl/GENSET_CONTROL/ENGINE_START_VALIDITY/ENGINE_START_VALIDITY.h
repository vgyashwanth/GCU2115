/*
 * ENGINE_START_VALIDITY.h
 *
 *  Created on: 03-Jun-2022
 *      Author: madhuri.abhang
 */

#ifndef APL_GENSET_CONTROL_ENGINE_START_VALIDITY_ENGINE_START_VALIDITY_H_
#define APL_GENSET_CONTROL_ENGINE_START_VALIDITY_ENGINE_START_VALIDITY_H_
#include "stdio.h"
#include "HAL_Manager.h"
#include "../../apl/CFGZ/CFGZ.h"
#include "GCU_ALARMS.h"



class ENGINE_START_VALIDITY
{

    //-------------------------------------------------------------------------------------------------/
    //*                                     PRIVATE MACRO                                             */
    //-------------------------------------------------------------------------------------------------/
    #define RPM_THRESHOLD_LOW_CONST                     600
    #define RPM_THRESHOLD_HIGH_CONST                    1200
    #define MIN_CNTS_FOR_RPM1TORPM2                     (100)   //(1msec * 100 count = 100msec)
    #define CNTS_FOR_1_SEC                              (1000)  //(1msec * 1000 count = 1sec)


    typedef enum
    {
        INVALID_DG_RUN_IDLE,
        INVALID_DG_RUN_CHECK_HIGH_THRSH,
        INVALID_DU_RUN_FALSE
    }INVALID_DG_DETECTION_STATE_t;

    /**
     ** @brief  The function initialises  -
     **         Sets lower, higher rpm threshold depending on crank disconnect, under speed threshold.
     **         Sets timeout to wait for lower to higher threshold depending on crank hold time.
     **         Resets all necessary variables for engine start validity SMD.
     **
     ** @param None
     ** @return void
     **/


    void InitEngineStartValidityParams(void);
    void SetStartWaveDetectionStatus(bool bStatus);

public:
    static void SetEngineStartInvalidity(bool bStatus);
    static bool GetEngineStartInvalidity(void);
    static bool GetStartWaveDetectionStatus(void);
    void UpdateStartValidyParam(void);
    ENGINE_START_VALIDITY(CFGZ &cfgz, GCU_ALARMS &GCUAlarms);
    void SMDToChkEngStartValidity(void);
    void InitInvalidDgDetectionStateMachine(void);

private:
    CFGZ &_cfgz;
    GCU_ALARMS          &_GCUAlarms;
    static bool _bEngineStartWasInValid;
    bool _bStrtTmrToDetectInvalidDgRun = false;
    static bool _bStartWaveDetection;
    uint32_t _u32TimeFromRpm1ToRpm2_ms = 0;
    uint32_t _u32TimeoutForRpm1ToRpm2_us = 0;
    uint16_t _u16LowRpmThreshold = 0;
    uint16_t _u16HighRpmThreshold = 0;
    INVALID_DG_DETECTION_STATE_t _eInvalidDgDetectionState = INVALID_DG_RUN_IDLE;
};





#endif /* APL_GENSET_CONTROL_ENGINE_START_VALIDITY_ENGINE_START_VALIDITY_H_ */
