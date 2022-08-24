/**
 * @file        [MANUAL_MODE]
 * @brief       MANUAL_MODE
 *              Provides common API's for functionality offered by manual SMD.
 *              The module is implemented in class ManualSMD which is inherited
 *              from class BASE_MODES.
 *
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        1st June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#ifndef _MANUAL_MODE_H_
#define _MANUAL_MODE_H_

#include "BASE_MODES.h"
#include "START_STOP.h"

class MANUAL_MODE:public BASE_MODES{

public:
    // Constructor: Initializes this module
    MANUAL_MODE(HAL_Manager &hal, ENGINE_MONITORING &EngineMon,CFGZ &cfgz, 
                    GCU_ALARMS &GCUAlarms, START_STOP &StartStop, 
                        GCU_MODE_VARS_t &vars);

    /**
     * Executes module functionality. To be called in the super while loop.
     * @param  : bool for checking whether device is in configuration.
     * @return
     * None
     */
    void Update(bool bDeviceInConfigMode);

    bool IsOperInProgress();

    bool IsGenRunTimersInProcess();

    void ChangeManualState(MANUAL_STATE_t estate);

    void ComeOutOfTestMode();

    void DisableTestModeTimer();

    bool IsTestModeTimerExpired();

    uint32_t GetEngCoolDownRemTime();

    uint32_t GetTestModeRemTime();
private:
    HAL_Manager         &_hal;
    ENGINE_MONITORING   &_EngineMon;
    CFGZ                &_cfgz;
    GCU_ALARMS          &_GCUAlarms;
    START_STOP          &_StartStop;
    GCU_MODE_VARS_t     &_vars;
    bool                _bTestMode;
    stTimer             _TestModeTimer;
    void prvSwitchToGenStartState();
};

#endif
