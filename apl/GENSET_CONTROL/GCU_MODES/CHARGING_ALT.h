/**
 * @file        [CHARGING_ALT]
 * @brief       CHARGING_ALT
 *              Provides common API's for functionality offered by charging
 *              alternator SMD and the PWM duty cycle calculation.
 *              This module is implemented in class CHARGING_ALT.
 *
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        4st June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef _CHARGING_ALT_H_
#define _CHARGING_ALT_H_

#include "BASE_MODES.h"

class CHARGING_ALT{

public:
    // Constructor: Initializes this module
    CHARGING_ALT(HAL_Manager &hal, ENGINE_MONITORING &EngineMon, CFGZ &cfgz, 
                    GCU_ALARMS &gcuAlarms);

    /**
     * Executes module functionality. To be called in the super while loop.
     * @param  :  bool for checking whether device is in configuration.
     * @return
     * None
     */
    void Update();

    /**
     * This API provide the information about the charging alternator fail alarm.
     * @param  : None
     * @return Returns true if the charging alternator fails.
     */
    static bool IsChargAltFail();

    /**
    * Runs the PID controller. This is expected to be called at periodically
    * every 200 us.
    * @param: None
    * @return
    * None
    */
    void RunPIDLoop();

    void StartExcitation();

    void StopExcitation();

    static void ClearChargAltFail();
private:
    typedef enum{
        STATE_CHG_ALT_OFF,
        STATE_CHG_ALT_CMD_ON,
        STATE_CHG_ALT_MON_WAIT,
        STATE_CHG_ALT_CMD_OFF,
        STATE_CHG_ALT_ERR
    }CHG_ALT_STATE_t;

    #define CA_PULSE_TIME_SEC           (120U)
    #define CA_WAIT_TIME_SEC            (10U)
    #define CHRG_ALT_IIR_FILT_CONST     (0.95F)
    #define CHRG_ALT_SET_VAL            (250.0F)
    /*The proportional constant*/
    #define CHRG_ALT_KP                 (0.1F)
    /*The integral constant*/
    #define CHRG_ALT_KI                 (0.025F)
    /*The differential constant*/
    #define CHRG_ALT_KD                 (0)
    #define CHRG_MAX_DUTY               (5.0F)
    /*
     * CHRG_ALT_PID_UPDATE_CNT : 20*200us  = 4ms . Duty cycle will be updated after 4ms as per NXP*/
    #define CHRG_ALT_PID_UPDATE_CNT     (20U)

    HAL_Manager                 &_hal;
    ENGINE_MONITORING           &_EngineMon;
    CFGZ                        &_cfgz;
    GCU_ALARMS                  &_GCUAlarms;
    CHG_ALT_STATE_t             _StateCA;
    bool                        _bChargAltOut;
    bool                        _bStopChargAltPulse;
    bool                        _bStartChargAltPulse;
    stTimer                     _ChargAltPulseTimer;
    stTimer                     _ChargAltWaitTimer;
    stTimer                     _ChargAltMonTimer;
    stTimer                     _CASMUpdateTimer;
    /*For charging alternator excitation PID*/
    float                       _f32FilteredAltCurrent;
    uint16_t                    _u16TicksInCurrentWindow;
    /*Represents the I term of the controller*/
    float                       _f32Integral;
    static bool                 _ChargAltFail;
};

#endif
