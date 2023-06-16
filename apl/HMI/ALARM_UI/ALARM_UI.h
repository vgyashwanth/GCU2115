/**
 * @file        [ALARM_UI]
 * @brief       This module is used to display Alarm screens
 *              with the help go Display class and CONST_UI.h
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        15th July 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef APL_HMI_ALARM_UI_H_
#define APL_HMI_ALARM_UI_H_

#include <HMI/UI_DS.h>
#include "../GENSET_CONTROL/GCU_ALARMS/GCU_ALARMS.h"
#include "../../MW/Display/Display.h"
#include "../CONST_UI/CONST_UI.h"
#include "../MON_UI/MON_UI.h"

class ALARM_UI
{
public:

    typedef enum
    {
        ALARM,
        DM1,
        DM2,
        PCD,
        NCD
    }ALARM_st;

    /**
     * For scrolling alarm screens
     */
    static uint8_t u8AlarmScreenNum;

    /**
     * Constructor of ALARM_UI class
     * @param GCU_ALARMS object reference
     * @param Display object reference
     * @return None
     */
    ALARM_UI(GCU_ALARMS &alarm, Display &Disp, CFGZ &cfgz, J1939APP &J1939, HAL_Manager &hal);

    /**
     * Update routine for ALARM_UI class.
     * @param bRefresh
     * @return None
     */
    void Update(bool bRefresh);

    /**
     * Key press event handler
     * @param _sKeyEvent
     * @return None
     */
    void CheckKeyPress(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent);

    static void ClearAlarmScreenNum();

    static void ChangeAlarmScreenType(ALARM_st eType);

private:

    // Other object references.
    GCU_ALARMS  &_alarm;
    Display     &_Disp;
    CFGZ        &_cfgz;
    J1939APP    &_J1939;
    HAL_Manager         &_hal;
    uint8_t     _u8NumberOfAlarms;
    static ALARM_st    _eAlarmType;
//    uint8_t     _u8ArrPCDAlarmForDisp[TOTAL_PCD_ALARMS];
//    uint8_t     _u8ArrNCDAlarmForDisp[TOTAL_NCD_ALARMS];
    ALARM_st    _eOldAlarmType ;
    /**
     * Used to display alarm
     * @param None
     * @return None
     */
    void prvDisplayAlarmScreen();
    void prvDisplayDMScreen();
    void prvDisplayPCDNCDScreen();
    uint8_t prvFillPCDAlarmArray(void);
    uint8_t prvFillNCDAlarmArray(void);

};

#endif /* APL_HMI_ALARM_UI_H_ */
