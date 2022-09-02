/**
 * @file        [MAIN_UI]
 * @brief       This module is used to handle all display related functionality.
 *              It handles display mode switching and key handling
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        4th Aug 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef APL_HMI_MAIN_UI_MAIN_UI_H_
#define APL_HMI_MAIN_UI_MAIN_UI_H_

#include <HMI/ALARM_UI/ALARM_UI.h>
#include <HMI/EVENT_LOG_UI/EVENT_LOG_UI.h>
#include <HMI/MON_UI/MON_UI.h>
#include <HMI/PASSWORD_ENTRY_UI/PASSWORD_ENTRY_UI.h>
#include <HMI/UI_DS.h>
#include "HAL_Manager.h"
#include "Keypad.h"
#include "../UI/UI.h"
#include "../UI/CKeyCodes.h"
#include "../../SLEEP_Handler/SLEEP_Handler.h"
#include "../GENSET_CONTROL/EGOV/EGOV.h"
#include "../GENSET_CONTROL/GCU_MODES/AUTO_EXERCISE_MODE.h"
#include "../ENGINE_START_VALIDITY/ENGINE_START_VALIDITY.h"

#define SCREEN_CHANGE_OVER_PAUSE (2*60U) // 2 Min time.
#define REFRESH_TIME_MS     (300U)
#define BOOT_DELAY          (4U)
#define TIME_OUT_SEC        (3U)

class MAIN_UI
{
public:
    /**
     * Constructor of MAIN_UI class
     * @param Hal object reference, sleep object reference
     * @return None
     */
    MAIN_UI(HAL_Manager &hal, CFGZ &pcfgz, GCU_ALARMS &GCUAlarms,
            ENGINE_MONITORING  &EngMon, START_STOP &StartStop,
            MANUAL_MODE &ManualMode,
            Display &Disp, CFGC &CFGC, SLEEP_Handler &sleep, EGOV &Egov,
            AUTO_EXERCISE_MODE &autoExercise, J1939APP &j1939 , BTS_MODE &BTSMode , CYCLIC_MODE &CyclicMode, ENGINE_START_VALIDITY &EngineStartValidity);

    static bool _sbKeyEventAvailable;
    /**
     * Update routine for UI class
     * @param None
     * @return None
     */
    bool Update();

   /* Callback function for keypad
    * @param Evt
    * @return None
    */
    static void prvKeypad_callback(KEYPAD::KEYPAD_EVENTS_t Evt);

    static MODE_TYPE_t GetDispMode(void);
private:

    // Macros for easy understanding
    #define IS_DISP_MON_MODE()            (MON_UI::eDisplayMode == DISP_MON_MODE)
    #define IS_DISP_ALARM_MODE()          (MON_UI::eDisplayMode == DISP_ALARM_MODE)
    #define IS_DISP_PASSWORD_ENTRY_MODE() (MON_UI::eDisplayMode == DISP_PASSWORD_EDIT_MODE)
    #define IS_DISP_EVENT_LOG_MODE()      (MON_UI::eDisplayMode == DISP_EVENT_LOG_MODE)
    #define IS_DISP_CONFIG_MODE()         (MON_UI::eDisplayMode == DISP_CONFIG_MODE)

    #define IS_GCU_MANUAL_MODE()          (_ManualMode.GetGCUOperatingMode()\
                                                 == BASE_MODES::MANUAL_MODE )
    #define IS_GCU_AUTO_MODE()            (_ManualMode.GetGCUOperatingMode()\
                                                 == BASE_MODES::AUTO_MODE )
    #define IS_GCU_BTS_MODE()            (_ManualMode.GetGCUOperatingMode()\
                                                 == BASE_MODES::BTS_MODE )
    #define IS_GCU_CYCLIC_MODE()          (_ManualMode.GetGCUOperatingMode()\
                                             == BASE_MODES::CYCLIC_MODE )
    #define IS_GCU_AUTO_EXE_MODE()        (_ManualMode.GetGCUOperatingMode()\
                                               == BASE_MODES::AUTO_EXERCISE_MODE )

    #define IS_GCU_TEST_MODE()            (_ManualMode.GetGCUOperatingMode()\
                                               == BASE_MODES::TEST_MODE )


    //Object references of different class
    HAL_Manager         &_hal;
    CFGZ                &_cfgz;
    SLEEP_Handler       &_sleep;
    MANUAL_MODE         &_ManualMode;
    ENGINE_MONITORING   &_EngMon;
    GCU_ALARMS          &_GCUAlarms;
    START_STOP          &_StartStop;
    EGOV                &_Egov;
    //Objects of different display modes
    J1939APP             &_j1939;
    BTS_MODE             &_BTSMode;
    CYCLIC_MODE          &_CyclicMode;
    MON_UI              _MonUI;
    AUTO_EXERCISE_MODE   &_autoExercise;
    ALARM_UI            _DispAlarm;
    EVENT_LOG_UI        _DispEventLog;
    PASSWORD_ENTRY_UI   _PasswordEntry;
    UI                  _objUI;
    ENGINE_START_VALIDITY &_EngineStartValidity;
    bool                 _bPanelLockOnce;
    bool                 _bExternalPanelLockOnce;
    /**
     * Screen refresh timer.
     */
    stTimer _RefreshTimer;

    /**
     * Power save mode delay timer.
     */
    stTimer _PoweSaveModeTimer;

    /** Screen change over timer
     */
    stTimer _ScreenChangeOverTimer;

    /** Power save mode delay timer.
     */
    stTimer _LEDBlinkTimer;

    /** Boot timer for displaying boot logo and product info screen
     */
    stTimer _BootTimer;

    /** It is used to pause some time after sleep and used to
     * ignore the key press after sleep
     */
    stTimer _SleepExitTimer;

    /** Auto Exit Config Mode timer
     */
    stTimer _AutoExitTimer;

    /** It stores the Key Event
     */
    static KEYPAD::KEYPAD_EVENTS_t _sKeyEvent;

//    /** Display mode variable of MODE_TYPE_t
//     */
//   volatile  MODE_TYPE_t _DisplayMode;

    /** To change the screen time during running
     *  It can be 2 min after key press or configured
     *  screen change over time.
     */
    static uint16_t _u16ScreenChangeTime;



    /** Used to handle all LED related functionality
     * @param  none
     * @return none
     */
    void prvLEDHandling(void);

    /** This will return true if engine in stopping state or stopped
     * @param None
     * @return true/false
     */
    bool prvIsEnginNotInONstate(void);


    void prvExitFromConfigMode(void);

    /** This function is used to update the LED status of 3 LED's
     * @param stLED1
     * @param stLED2
     * @param stLED3
     * @return None
     */
    void UpdateLEDStatus(bsp_io_level_t stLED1, bsp_io_level_t stLED2,
            bsp_io_level_t stLED3);


};



#endif /* APL_HMI_MAIN_UI_MAIN_UI_H_ */
