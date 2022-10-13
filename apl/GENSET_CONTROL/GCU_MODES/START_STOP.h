/**
 * @file        [START_STOP]
 * @brief       START_STOP
 *              Provides common API's for functionality offered by start stop.
 *              state machine. The module is implemented in class START_STOP
 *              which is inherited from class BASE_MODES.
 *
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        1st June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#ifndef _START_STOP_H_
#define _START_STOP_H_

#include "stdint.h"
#include "HAL_Manager.h"
#include "BASE_MODES.h"
#include "CHARGING_ALT.h"
#include "../ENGINE_START_VALIDITY/ENGINE_START_VALIDITY.h"

class START_STOP{

public:
     #define PULSES_TO_IGNORE        (10U)

	typedef enum{
		ID_STATE_SS_ENG_OFF_OK,
		ID_STATE_SS_ENG_OFF_ERR,
		ID_STATE_SS_PREHEAT,
		ID_STATE_SS_START_WAIT,
		ID_STATE_SS_CRANKING,
		ID_STATE_SS_CRANK_REST,
		ID_STATE_SS_ENG_ON,
		ID_STATE_SS_STOPPING,
		ID_STATE_SS_STOP_HOLD,
		ID_STATE_SS_FAIL_TO_STOP
	}SS_STATE_t;


     enum
    {
        IDLE_MODE_ACTIVE,
        IDLE_MODE_DEACTIVE
    } ;

     #define IS_GCU_PREHEAT_TEMP_ENABLED()       ( _cfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_ENG_TEMPERATURE) == 1)

     #define IS_GCU_TEMP_CONFIGURED()             ( _cfgz.GetCFGZ_Param(CFGZ::ID_ENG_TEMP_DIG_L_SENSOR_SELECTION) == CFGZ::CFGZ_ANLG_CUSTOM_SENSOR1)

     #define IS_PRHEAT_RELAY_CONFIGURED()       ( ACT_Manager::ACT_NOT_CONFIGURED != _hal.actuators.GetActStatus(ACTUATOR::ACT_PREHEAT))

     #define IS_AMB_TEMP_ENABLED()              ( _cfgz.GetCFGZ_Param(CFGZ::ID_PREHEAT_AMB_TEMPERATURE) == 1U)

     #define DG_IDLE_RUN_DELAY_IN_SEC           (5U)

	// Constructor: Initializes this module
	START_STOP(HAL_Manager &hal, ENGINE_MONITORING &EngineMon, CFGZ &cfgz,  
                    GCU_ALARMS &GCUAlarms, BASE_MODES::GCU_MODE_VARS_t &vars, CHARGING_ALT &ChargeAlt,   ENGINE_START_VALIDITY  &EngineStartValidity);

    /**
     * Executes module functionality. To be called in the super while loop.
     * @param  : None
     * @return
     * None
     */
	void Update(bool bDeviceInConfigMode);

    /**
     * Provides the current state of the start stop state machine.
     * @param  : None
     * @return : SS_STATE_t - return the current state of start stop State machine.
     *
     */
	static SS_STATE_t GetStartStopSMDState();

    /**
     * This API tells cranking attempt number. To be called in HMI Module while
     * displaying the cranking number on home screen.
     * @param  : None
     * @return : returns the current crank attempt number.
     */
	uint8_t GetCrankAttemptNumber();

    /**
     * This API provide the information about the genset is started or not.
     * @param  : None
     * @return Returns true if start command is given to the GCU.
     */
    bool IsGenStarted();

    /**
     * API provide the information about the genset is to be started or not.
     * @param  : None
     * @return : None
     */
    void StartCommand();

    /**
     * This API provide the information about the genset is to be stopped or not.
     * @param  : None
     * @return : None
     */
    void StopCommand();


    /**
     * provide the status of engine monitoring timer.
     * @param  : None
     * @return : returns true if the engine monitoring timer is enabled or not.
     */
    bool IsEngStoppingTimerEnabled();


    /**
     * provide the status of safety monitoring delay.
     * @param  : None
     * @return : returns true if the safety monitoring delay is over.
     */
    static uint8_t IsGenMonOn();

    /**
     * This API calculates the remaining time of the GCU display timers.
     * @param  eTimer : timer enum whose remaining time needs to be calculated. 
     * @return : returns the remaining time in seconds of the requested timer.
     */
    uint32_t GetTimersRemainingTime(BASE_MODES::TIMER_STATE_t eTimer);

    static bool IsChargAltStopLtached();

    static void ClearChargAltStopLtached();

    static bool IsMonitorChargAltTrue();

    static void ClearPreheatOutput();

    bool IsTimerRunning();

    bool IsAdditionalStopTimerRunnint();

    bool IsModeSwitchAutoKeyReceived();

    bool IsSimAutoReceived();

    bool IsSimStartReceived();

    bool IsSimStopReceived();

    void ClearSimAutoPulse();

    void ClearSimStopPulse();

    void ClearSimStartPulse();

    void Init();

    bool IsStartCommand();

    static bool IsStopRelayON();

    static bool IsStartRelayON();

    static bool IsFuelRelayOn();

    static bool IsStartPreheatON();

    static bool IsEngineOnFailToStopAck();

    static bool IsMonitorDGIdleRunTrue();

    void UpdateGcuStatusAndTimerDisplay(BASE_MODES::GCU_STATE_t eGcuState, BASE_MODES::TIMER_STATE_t eTimerDisplay);

    static bool IsStopCommand();

    bool CheckPreheatTempCondition();

    void StartKeyPressed();
    void StopKeyPressed();

    uint32_t GetCrankStartDelay();

private:
    HAL_Manager                 &_hal;
    ENGINE_MONITORING           &_EngineMon;
    CFGZ                        &_cfgz;
    GCU_ALARMS                  &_GCUAlarms;
    BASE_MODES::GCU_MODE_VARS_t &_vars;
    CHARGING_ALT                &_ChargeAlt;
    ENGINE_START_VALIDITY       &_EngineStartValidity;
	static SS_STATE_t                  _State;
	uint8_t                     _u8NoOfCrankAttempts;
	static uint16_t             _u16ConfiguredSafetyMonDelay;
    bool                        _bGenStarted;
    static bool                 _bOPFuelRelay;
    bool                        _bStartCommand;
    static bool                 _bStopCommand;
    bool                        _bEmergencyStopLatched;
    bool                        _bEmergencyStop;
    bool                        _bSimStartReceived;
    bool                        _bSimStopReceived;
    bool                        _bSimAutoReceived;
    bool                        _bSimAutoLatched;
    bool                        _bSimStartLatched;
    bool                        _bSimStopLatched;
    bool                        _bModeSwicthAutoKeyPress;
    bool                       _bDGIdleRunDelayRunning;
    static bool                 _bMonitorDGIdleRun;
    static bool                _bEngineOnFailToStopAck;

    static DigitalSensor::STATUS_t prevModeSwitchState;
    static DigitalSensor::STATUS_t currModeSwitchState;

    stTimer                     _PreheatTimer;
    stTimer                     _EngStartTimer;
    stTimer                     _EngCrankingTimer;
    stTimer                     _EngCrankRestTimer;
    stTimer                     _StartStopSMUpdateTimer;
    stTimer                     _StopHoldTimer;
    stTimer                     _PowerOnTimer;
    stTimer                     _DGIDleRunTimer;

    bool                        _bStartKeyPressed;
    bool                        _bStopKeyPressed;

    static stTimer              _EngStoppingTimer;
    static stTimer              _SafetyMonTimer;
    static bool                 _bChargAltStopLatched;
    static bool                 _bMonitorChargAlt;
    static bool                 _bOPStopSolenoid;
    static bool                 _bOPStartRelay;
    static bool                 _bOPPreheat;

    void prvTurnOnOffOutputs();

    void prvSetOutputVariables(bool bStartRelayStatus, bool bStopSolenoidStatus,
                                    bool bFuelRelayStatus, bool bPreheatStatus);
    void prvStopCommandAction();

    void prvHandleEngineCranked();

    void prvUpdateSimStartStopStatus();

    void prvTurnOffPreheatStartCranking();

    void prvSkipPreheatStartCranking();

    void prvReqStartDGIdleRunTimer();

    void prvUpdateDGIDLERunStatus();

    void prvEngineOnGenStartAction();

};
#endif
