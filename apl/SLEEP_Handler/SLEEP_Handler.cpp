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
#include "SLEEP_Handler.h"
#include "Keypad.h"
#include "DFLASH/DFLASH.h"

static void KeypadCb(KEYPAD::KEYPAD_EVENTS_t Evt);
static bool bKeyEvent = false;

SLEEP_Handler::SLEEP_Handler(HAL_Manager &hal, CFGZ &cfgz):
_hal(hal),
_cfgz(cfgz)
{
    UTILS_ResetTimer(&_sleepTmr);
    _hal.ObjKeypad.RegisterKeyEventCB(KeypadCb);

}

void SLEEP_Handler::Update()
{
    /*Do not proceed if sleep mode is disabled*/
//    if(_cfgz.GetCFGZ_Param(CFGZ::ID_DEEP_SLEEP_EN) == CFGZ::CFGZ_DISABLE)
//    {
//        return;
//    }
    /* Shubham Wader 19.09.2022
     * As there are no any Sleep mode related config parameters, It is highly likely that sleep mode functionality is no there
     * in GC2111.
     * todo: need to confirm once at the end.*/
    if(_hal.AmIInSleep())
    {
        /*No key press in progress, enter sleep*/
        if(!_hal.ObjKeypad.IsAnyKeyPressedRaw())
        {
            /*Deactivate all actuators*/
            _hal.actuators.DeactivateAll();
            _hal.hsdManager.AltExcitation.TurnOff();
//            _hal.EnterSleep(); Commented by SuryaPranayTeja as GC2111 should not enter Sleep in any conditions
        }
        /*Exit sleep if key press event received*/
        else if(bKeyEvent)
        {
            bKeyEvent = false;
            _hal.ExitSleep();
            UTILS_ResetTimer(&_sleepTmr);
        }
    }
    /*Enter sleep if sleep timer is expired*/
    // Avoid going in sleep mode during Dflash write operation
    else if((UTILS_GetElapsedTimeInSec(&_sleepTmr) >=
                                _cfgz.GetCFGZ_Param(CFGZ::ID_GENERAL_TIMER_DEEP_SLP_MODE_DELAY))
             )
    {
        if(!_hal.Objeeprom.IsRquestPending())
        {
            bKeyEvent = false;

            /*De-energize all outputs*/
//            _hal.EnterSleep();Commented by SuryaPranayTeja as GC2111 should not enter Sleep in any conditions
        }
    }
}

void SLEEP_Handler::KickSleepTimer()
{
    UTILS_ResetTimer(&_sleepTmr);
}

static void KeypadCb(KEYPAD::KEYPAD_EVENTS_t Evt)
{
    bKeyEvent = true;
}
