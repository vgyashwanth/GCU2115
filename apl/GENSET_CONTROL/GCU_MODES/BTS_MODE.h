/**
 * @file        [SMDBTS]
 * @brief       SMDBTS
 *              Provides common API's for functionality offered by BTS SMD.
 *              The module is implemented in class BTSSMD which is inherited
 *              from class BASE_MODES.
 *
 * @version     [Version number]
 * @author      Soujanya Mulik
 * @date        2nd June 2022
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef _BTS_MODE_H_
#define _BTS_MODE_H_

#include "BASE_MODES.h"
#include "START_STOP.h"


class BTS_MODE:public BASE_MODES{

public:
    // Constructor: Initializes this module
   BTS_MODE(HAL_Manager &hal, ENGINE_MONITORING &EngineMon,
                CFGZ &cfgz,  GCU_ALARMS &GCUAlarms, START_STOP &StartStop,
                    GCU_MODE_VARS_t &vars);

    /**
     * Executes module functionality. To be called in the super while loop.
     * @param  :  bool for checking whether device is in configuration.
     * @return
     * None
     */
    void Update(bool bDeviceInConfigMode);

    uint32_t GetBTSModeTime(BASE_MODES::TIMER_STATE_t eTimer);
    void ClearBTSVars(void);
    bool IsGenOnDuetoBTS(void);
    bool IsGenOnDueToSheltTemp(void);
    void TurnOffBattCharging(void);
    void ManualToBTSOnLoad(void);
    void ClearSheltTempTimer(void);
private:
    HAL_Manager         &_hal;
    ENGINE_MONITORING   &_EngineMon;
    CFGZ                &_cfgz;
    GCU_ALARMS          &_GCUAlarms;
    START_STOP          &_StartStop;
    GCU_MODE_VARS_t     &_vars;

    bool                _bGenOnDuetoBTS;    /*Need to shift to Start Stop*/
    bool                _bGenOnDueToSheltTemp;
    stTimer             _stBatteryChargingTimer;
    stTimer             _stShelterTempTimer;

    bool prvGenStartCondition();
    void prvUpdateGenOnReason();
};

#endif  /*_BTS_MODE_H_*/
