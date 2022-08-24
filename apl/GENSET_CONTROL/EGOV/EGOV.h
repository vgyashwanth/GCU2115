/*
 * EGOV.h
 *
 *  Created on: 02-Dec-2021
 *      Author: nikhil.mhaske
 */

#ifndef APL_GENSET_CONTROL_EGOV_EGOV_H_
#define APL_GENSET_CONTROL_EGOV_EGOV_H_

#include "HAL_Manager.h"
#include "CFGZ.h"
#include "ENGINE_MONITORING.h"
#include "START_STOP.h"

class EGOV
{
public:

    #define ATD_VALUE_HIGH          5000U
    #define ATD_VALUE_LOW           0U
    #define ATD_TABLE_MAX_NDEX      10U
    #define ATD_table_STEP_SIZE     500U
    #define SCALE_FACT              50U
    #define RATE_LIMIT_DOWN         10U
    #define RATE_LIMIT_UP           10U
    #define N_PULSES_STOP           1000U

    typedef enum
    {
        DIR_TOWARDS_STOP,
        DIR_TOWARDS_START,
    }DIR_t;

    typedef enum
    {
        ID_PID_ISOCHRONOUS,
        ID_PID_DROOP,
        //Below options are not available to SGC121
        ID_PID_PARALLELING,
        ID_AUTO_TUNE_ISOCHRONOUS,
        ID_AUTO_TUNE_PARALLELING,
        ID_AUTO_TUNE_DROOP,
        ID_EGOV_AS_START_STOP,
        ID_GOV_MODE_LAST
    }EGOV_MODE_t;

    typedef enum
    {
        P_GAIN,
        I_GAIN,
        D_GAIN,
        FRICTION_SET_OFF,
        GAIN_SCHEDULE_TRIGGER,
        LOADING_FACTOR,
        UNLOADING_FACTOR,
        GAINS_LAST
    }GAIN_t;

    EGOV(HAL_Manager &hal, CFGZ &cfgz, ENGINE_MONITORING &engMon, START_STOP &StartStop);

    void Update();

    void UpdateEgovCalc(void);

    float GetEngineAvgRPM(void);

    bool IsValidGoverningMode(void);

    bool IsRampingOver(void);

    static void ResetEgovVars(void);

    static void UpdateSensIpStatus(float f32RPM);

    static void UpdateCompIpStatus(float f32RPM);

    static uint8_t GetCompCBCount(void);

    static void ClearCompCBCount(void);

    void InitEgovParameters(void);

    float GetTragetRpm();

    float GetSetTragetRpm();

    uint32_t GetEngOnTime();

    uint32_t GetPIDOnTime();
private:
    HAL_Manager          &_hal;
    CFGZ                 &_cfgz;
    ENGINE_MONITORING    &_engMon;
    START_STOP           &_StartStop;

    bool                 _bUseErrSlopeChangePt;
    bool                 _bPidOn;
    bool                 _bStartInitiated;
    bool                 _bStopInitiated;
    bool                 _bFixedStepExecuted;
    bool                 _bFixedStepForStopExecuted;
    bool                 _bSwitchedToPID;
    float                _f32RPMAvg;
    float                _f32OldRpm;
    float                _f32TargetRPM;
    float                _f32SetTargetRPM;
    float                _f32ErrorSlopechangePoint;
    float                _f32OldParallelTargetRPM;
    float                _f32FiltParallelTargetRPM;
    float                _f32ArrGain[GAINS_LAST];
    volatile float       _f32ErrorP;
    volatile float       _f32ErrorD;
    volatile float       _f32ErrorPOld;
    volatile float       _f32ErrorDOld;
    volatile float       _f32ErrorI;
    volatile float       _f32DeltaU;
    volatile float       _f32U;
    volatile float       _f32UPrev;

    uint16_t             _arrU16RPMTable[11];

    stTimer              _EngineOnTimer;
    stTimer              _PIDOnTimer;
    stTimer              _SMUpdateTimer;

    EGOV_MODE_t          _eEgovMode;

    static bool          _bResetEgovVariables;
    static bool          _bSensIpRPMUpdate;
    static bool          _bCompartorIpRPMUpdate;
    static uint8_t       _u8ComparatorCBCount;

    void prvStartStopActionUsingEgov();

    void prvPIDLoop();

    void prvUpdateCrankBoostStatus(void);

    void prvResetEgovParam(void);
};
#endif /* APL_GENSET_CONTROL_EGOV_EGOV_H_ */
