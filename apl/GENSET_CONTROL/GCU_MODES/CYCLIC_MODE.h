/**
 * @file        [SMDCYCLIC]
 * @brief       SMDCYCLIC
 *              Provides common API's for functionality offered by CYCLIC SMD.
 *              The module is implemented in class CYCLIC SMD which is inherited
 *              from class BASE_MODES.
 *
 * @version     [Version number]
 * @author      Soujanya Mulik
 * @date        4nd June 2022
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef _CYCLIC_MODE_H_
#define _CYCLIC_MODE_H_

#include "BASE_MODES.h"
#include "START_STOP.h"


class CYCLIC_MODE:public BASE_MODES{

public:
    // Constructor: Initializes this module
   CYCLIC_MODE(HAL_Manager &hal, ENGINE_MONITORING &EngineMon,
                CFGZ &cfgz,  GCU_ALARMS &GCUAlarms, START_STOP &StartStop,
                    GCU_MODE_VARS_t &vars);

    /**
     * Executes module functionality. To be called in the super while loop.
     * @param  :  bool for checking whether device is in configuration.
     * @return
     * None
     */
    void Update(bool bDeviceInConfigMode);
    uint32_t GetCyclicModeTime(BASE_MODES::TIMER_STATE_t eTimer);
    void ClearCyclicModeVars(void);
    void SwitchFromManualToCyclic();
    bool IsCyclicOnTimerExpired();

    bool IsCyclicOffTimerExpired();

    void ClearStartOffTimerFlag();
private:
    HAL_Manager         &_hal;
    ENGINE_MONITORING   &_EngineMon;
    CFGZ                &_cfgz;
    GCU_ALARMS          &_GCUAlarms;
    START_STOP          &_StartStop;
    GCU_MODE_VARS_t     &_vars;

    stTimer             _CyclicOnTimer;
    stTimer             _CyclicOffTimer;
    bool                _bStartOffTimer;
};

#endif  /*_CYCLIC_MODE_H_*/
