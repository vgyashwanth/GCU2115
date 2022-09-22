/**
 * @file        [SMDAuto]
 * @brief       SMDAuto
 *              Provides common API's for functionality offered by Auto SMD.
 *              The module is implemented in class AutoSMD which is inherited
 *              from class BASE_MODES.
 *
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        1st June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#ifndef _AUTO_MODE_H_
#define _AUTO_MODE_H_

#include "BASE_MODES.h"
#include "START_STOP.h"

class AUTO_MODE:public BASE_MODES{

public:
    // Constructor: Initializes this module
    AUTO_MODE(HAL_Manager &hal, ENGINE_MONITORING &EngineMon,
                CFGZ &cfgz,  GCU_ALARMS &GCUAlarms, START_STOP &StartStop,  
                    GCU_MODE_VARS_t &vars);

    /**
     * Executes module functionality. To be called in the super while loop.
     * @param  :  bool for checking whether device is in configuration.
     * @return
     * None
     */
    void Update(bool bDeviceInConfigMode);

    void MBStartCMDReceived();

    void MBStopCMDReceived();

    bool IsMainsHealthyOrRemoteStopRcvd();
    bool IsMainsUnhealthyOrRemoteStartRcvd();
private:
    HAL_Manager         &_hal;
    ENGINE_MONITORING   &_EngineMon;
    CFGZ                &_cfgz;
    GCU_ALARMS          &_GCUAlarms;
    START_STOP          &_StartStop;
    GCU_MODE_VARS_t     &_vars;
    void prvGensetStartAction();
    void prvShutdownAction();
    bool prvIsMainsFailedAndOpenMainsCont();
    bool prvIsMainsHealthyAndCloseMainsCont();
};

#endif
