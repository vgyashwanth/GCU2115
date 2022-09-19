/*
 * @file        [BASE_MODES]
 * @brief       BASE_MODES
 *              Provides common API's for functionality offered by Auto & Manual
 *              SMD's. This module is implemented in class BASE_MODES which is 
 *              the base class for all other SMD classes. Module also include 
 *              the contactor handling and mains detection.
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        1st June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 */

#ifndef _BASE_MODES_H
#define _BASE_MODES_H

#include "HAL_Manager.h"
#include "../../apl/CFGZ/CFGZ.h"
#include "ENGINE_MONITORING.h"

class BASE_MODES{
public:
    /*  Contains the list of the GCU states, the enum values correspond to the 
        current GCU state.
     */
    typedef enum{
        EMPTY_GCU_STATE,
        ENGINE_OFF_READY,
        ENGINE_STARTING,
        ENGINE_ON_HEALTHY,
        ENGINE_STOPPING,
        NOTIFICATION,
        WARNING,
        ELECTRIC_TRIP,
        SHUTDOWN,
        ENGINE_OFF
    }GCU_STATE_t;

    /*  Contains the list of the GCU timers, the enum values correspond to the 
        current running timer.
     */
    typedef enum{
        NO_TIMER_RUNNING,
        PREHEAT_TIMER,
        START_DELAY_TIMER,
        CRANK_START_TIMER,
        CRANK_REST_TIMER,
        SAFETY_MON_TIMER,
        COOL_DOWN_TIMER,
        STOP_ACTION_TIMER,
        ADDTIONAL_STOP_TIMER,
        TEST_MODE_TIMER,
        BATT_CHARGE_TIMER,
        SHELTER_TEMP_TIMER,
        CYCLIC_ON_TIMER,
        CYCLIC_OFF_TIMER,
    }TIMER_STATE_t;

    // Represents the mains status.
    typedef enum{
        MAINS_UNHELATHY,
        MAINS_HELATHY,
        MAINS_NOT_CONFIGURED
    }MAINS_STATUS_t;

    typedef struct{
        GCU_STATE_t GCUState;
        TIMER_STATE_t TimerState;
    }GCU_MODE_VARS_t;

    typedef enum
    {
        BOTH_CONTACTOR_OPEN,
        GEN_CONTACTOR_CLOSED,
        MAINS_CONTACTOR_CLOSED
    }CONTACTOR_STATUS_t;

    typedef enum{
        TEST_MODE,
        MANUAL_MODE,
        AUTO_MODE,
        BTS_MODE,
        CYCLIC_MODE,
        AUTO_EXERCISE_MODE
    }GCU_OPERATING_MODE_t;

    typedef enum{
        STATE_MANUAL_GEN_OFF,
        STATE_MANUAL_GEN_START,
        STATE_MANUAL_GEN_READY,
        STATE_MANUAL_ENGINE_COOLING,
        STATE_MANUAL_ENGINE_STOP
    }MANUAL_STATE_t;

    typedef enum{
        STATE_AMF_GEN_OFF_MAINS_OFF,
        STATE_AMF_GEN_OFF_MAINS_ON,
        STATE_AMF_GEN_START,
        STATE_AMF_GEN_ON_LOAD,
        STATE_AMF_RETURN_DELAY,
        STATE_AMF_ENGINE_COOLING,
        STATE_AMF_ENGINE_STOP
    }AMF_STATE_t;

    typedef enum ID_STATE_EXERCISER
    {
        ID_AUTO_EXE_DG_OFF = 0,
        ID_AUTO_EXE_DG_START,
        ID_AUTO_EXE_DG_ON_LOAD,
        ID_AUTO_EXE_ENGINE_COOLING,
        ID_AUTO_EXE_FAULT,
        ID_AUTO_EXE_ENGINE_STOP
    }AUTO_EXE_STATE_t;


    typedef enum
    {
        STATE_BTS_GEN_OFF_MAINS_OFF = 0,
        STATE_BTS_GEN_OFF_MAINS_ON,
        STATE_BTS_GEN_START,
        STATE_BTS_GEN_ON_LOAD,
        STATE_BTS_RETURN_DELAY,
        STATE_BTS_ENGINE_COOLING,
        STATE_BTS_ENGINE_STOP
    }BTS_STATE_t;


    typedef enum
    {
        STATE_CYCLIC_GEN_OFF_MAINS_OFF = 0,
        STATE_CYCLIC_GEN_OFF_MAINS_ON,
        STATE_CYCLIC_GEN_START,
        STATE_CYCLIC_GEN_ON_LOAD,
        STATE_CYCLIC_RETURN_DELAY,
        STATE_CYCLIC_ENGINE_COOLING,
        STATE_CYCLIC_ENGINE_STOP
    }CYCLIC_STATE_t;

    /**
     * Constructor, initiates this module.
     * @param : hal       : reference to the hardware abstraction layer. 
     *          cfgz      : reference to the configuration module.
     *          GCUAlarms : reference to the GCU alarms module.
     * @return
     * None
     */
    BASE_MODES(HAL_Manager &hal, CFGZ &cfgz,  GCU_ALARMS &GCUAlarms,
            ENGINE_MONITORING &EngineMon, GCU_MODE_VARS_t &vars);

    /**
     * Executes module functionality. To be called in the super while loop.
     * @param  : None
     * @return
     * None
     */
    void Update();

    /**
     * This API provides the information about the current state of the GCU.
     * @param  None
     * @return GCU_STATE_t
     * ENGINE_OFF_READY         : when the engine is off and ready to start.
     * ENGINE_STARTING_ATTEMPTS : when engine is starting(during cranking).
     * ENGINE_ON_HEALTHY        : when is engine is on and warm up delay is over.
     * ENGINE_STOPPING          : while engine stopping.
     * NOTIFICATION             : when any notification alarm is present.
     * WARNING                  : when any warning alarm is present.
     * ELECTRIC_TRIP            : when any electric trip alarm is present.
     * SHUTDOWN                 : when any shutdown alarm is present.
     * ENGINE_OFF               : when the engine is off.
     */
    GCU_STATE_t GetGCUState();

    /**
     * API provides the information about the current running timer.
     * @param  None
     * @return TIMER_STATE_t
     * PREHEAT_TIMER        : when the preheat timer is running.
     * START_DELAY_TIMER    : when start delay is running.
     * CRANK_START_TIMER    : when the GCU is in cranking state.
     * CRANK_REST_TIMER     : when the GCU is in crank rest state.
     * SAFETY_MON_TIMER     : when the safety monitoring delay is running.
     * COOL_DOWN_TIMER      : while the cooling down delay.
     * STOP_ACTION_TIMER    : while stooping the engine in stop action time.
     * ADDTIONAL_STOP_TIMER : after stopping the engine in addition stop delay.
     * TEST_MODE_TIMER      : When the GCU is in the test mode.
     */
    TIMER_STATE_t GetTimerState();

    /**
     * API provide the information about the genset is to be started or not.
     * @param  : None
     * @return Returns true if start command is given to the GCU.
     */
    bool StartCommand();

    /**
     * This API provide the information about the genset is to be stopped or not.
     * @param  : None
     * @return Returns true if stop command is given to the GCU.
     */
    bool StopCommand();

    /**
     * Sets the stop command variable. To be called in keypad module when
     * the stop key is pressed.
     * @param  : None
     * @return : None
     */
    static void Stop();

    /**
     * Sets the start command variable. To be called in keypad module when
     * the stop key is pressed.
     * @param  : None
     * @return : None
     */
    void Start();

    /**
     * This function will initializes and assign the states of manaul mode
     * similar to the auto mode when switched from auto to manual.
     * @param  : None
     * @return : None
     */
    void SwitchToManualMode();

    /**
     * This function updates the mains status whether mains is healthy or unhealthy.
     * @param  None
     * @return None */
    void UpdateMainsStatus();

    void SetGCUOperatingMode(GCU_OPERATING_MODE_t eGCUMode);

    static GCU_OPERATING_MODE_t GetGCUOperatingMode();

    void InitNightModeParam(void);

    bool IsNightModeRestrictOn(void);

    static MAINS_STATUS_t GetMainsStatus();
    void DisableEngCoolTimer();
    void DisableReturnToMains();
    AMF_STATE_t GetAutoModeState();
    MANUAL_STATE_t GetManualModeState();
    AUTO_EXE_STATE_t GetAutoExeState();
    BTS_STATE_t GetBTSModeState();
    CYCLIC_STATE_t GetCyclicModeState();
    void AssignModechangeParameters();
    void SwitchLoadToMains();
    void SwitchLoadToGen();
    void OpenGenLoad();
    void OpenMainsLoad();
    void ChangeAutoState(AMF_STATE_t eAutoSMDState);
    void SetMainsStatusHealthy();
    bool GenStartCondition();
    void SetTimerState(TIMER_STATE_t state);
    static bool IsGenContactorClosed(void);
    static bool IsMainsContactorClosed(void);
    bool IsGCUStateChanged();
    void ClearGCUStateChangedStatus();
    bool GetPressureSensorStatusBeforeStart();
    bool IsMainsContactorConfigured();
    bool IsGenContactorConfigured();
    static bool GetMainsLowStatus();
    static bool GetMainsHighStatus();
    bool IsLoadOnMains();
    static void SetModeState(MANUAL_STATE_t eState);
    static void SetModeState(AMF_STATE_t eState);
    static void SetModeState(AUTO_EXE_STATE_t eState);
    static void SetModeState(BTS_STATE_t eState);
    static void SetModeState(CYCLIC_STATE_t eState);
    static bool GetIndividualPhaseStatus(PHASE_t ePhase);

    uint8_t GetMainsHealthyPhaseCnt(void);
protected:
    #define SM_UPDATE_TIME      50U

    static bool                 _bOpenMainsLoad;
    static bool                 _bOpenGenLoad;
    static bool                 _bRemoteStartRCVD;
    static bool                 _bRemoteStopRCVD;
    static bool                 _bContactorTransferOn;
    static bool                 _bCloseGenContactor;
    static bool                 _bCloseMainsContactor;
    static bool                 _bModeSwitchFromAuto;
    static bool                 _bStopRequest;
    static bool                 _bStartRequest;
    static bool                 _bEmergencyStop;
    static bool                 _bMBStartCmdRequested;
    static bool                 _bMBStopCmdRequested;
    static bool                 _bMBStartCmdReceived;
    static bool                 _bMBStopCmdReceived;
    static MANUAL_STATE_t       _eManualState;
    static AMF_STATE_t          _eAutoState;
    static AUTO_EXE_STATE_t     _eAutoExeState;
    static BTS_STATE_t          _eBTSState;
    static CYCLIC_STATE_t       _eCyclicState;
    static MAINS_STATUS_t       _MainsStatus;
    static stTimer              _ReturnToMainsTimer;
    static stTimer              _EngCoolDownTimer;
    static stTimer              _GCUSMUpdateTimer;
    static stTimer              _BaseModeUpdateTimer;
    static stTimer              _MainsMonUpdateTimer;
    static GCU_OPERATING_MODE_t _eOperatingMode;
    static uint16_t             _u16SchOnTimeMin;
    static uint16_t             _u16SchOnTimeHr;
    static uint16_t             _u16NightModeStartTime;
    static uint16_t             _u16NightModeStopTime;
    static uint16_t             _u16NightModeDurationHrsMin;
    static uint32_t             _u32SchRemTime_sec;
    static bool                _bNightModeRestrict;

    static bool               _bCLoseGenFlag;

    static bool  _bStartPress;
    static bool  _bStopPress;
    /**
     * Sets the GCU state according to the current active alarm.
     * @param  : None
     * @return : None
     */
    void prvSetGCUState();


private:
    HAL_Manager         &_hal;
    CFGZ                &_cfgz;
    GCU_ALARMS          &_GCUAlarms;
    ENGINE_MONITORING   &_EngineMon;
    GCU_MODE_VARS_t     &_vars;
    static stTimer      _ContactorTransferTimer;
    static bool         _bContTransferToMainsOn;
    static bool         _bContTransferToGenOn;
    static bool         _bSwitchLoadToMains;
    static bool         _bSwitchLoadToGen;
    static bool         _bGCUStateChanged;
    static GCU_STATE_t  _ePrevGCUState;
//    static TIMER_STATE_t _ePrevTimerState;
//    static GCU_OPERATING_MODE_t _ePrevOperatingMode;
    static bool         _bRPhasHealthyStatus;
    static bool         _bYPhasHealthyStatus;
    static bool         _bBPhasHealthyStatus;
    static bool         _bMainsHigh ;
    static bool         _bMainsLow  ;

    /**
     * This function updates the contactor status depending on the mains and
     * engine status and depending on that it will latch the mains or genset contactor.
     * @param  None
     * @return None */
    void prvUpdateContactorOutputs();

    /**
     * This function opens the mains or genset contactor.
     * @param  None
     * @return None */
    void prvOpenGenMainLoad(bool *bOpenLoad, bool *bContTransferToLoadOn , 
                                                bool *bCloseContactor);

    void prvUpdateBTSBattHybrdModeStatus();
    void prvOperatingModeOutputs();

    void prvUpdateNightModeRestrictStatus(void);

    /**
     * This function returns true if Engine is not in Cooling stage in any mode.
     */
    bool prvEngineNotInCoolingStage();

};

#endif
