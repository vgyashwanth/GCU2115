/**
 * @file        [SLEEP_Handler]
 * @brief       This module decides when to move the MCU to sleep and wake it up
 *              from sleep.
 * @version     [Version number]
 * @author      [Sudeep Chandrasekaran]
 * @date        23rd July 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#ifndef _SLEEP_HANDLER_
#define _SLEEP_HANDLER_

#include "stdint.h"
#include "bsp.h"
#include "HAL_Manager.h"
#include "CFGZ.h"

class SLEEP_Handler {

public:

    /**
     * Constructs this class
     * @param hal - Reference to hal.
     * @return
     * None
     */
    SLEEP_Handler(HAL_Manager &hal, CFGZ &cfgz);

    /**
     * Contains the functionality which needs to be periodically executed. Expected
     * to be called in the super-loop.
     * @param - None
     * @return
     * None
     */
    void Update();

    /**
     * Kicking the timer is same as resetting the sleep timer. If this API is not
     * called for SLEEP_TIMEOUT_MS then the MCU will enter sleep.
     * @param - None
     * @return
     * None
     */
    void KickSleepTimer();

private:
    HAL_Manager &_hal;
    CFGZ        &_cfgz;
    stTimer     _sleepTmr;

};

#endif
