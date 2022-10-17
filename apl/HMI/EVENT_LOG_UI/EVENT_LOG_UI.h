/**
 * @file        [EVENT_LOG_UI]
 * @brief       This module is used to display events logs.
 *              This module will fetch the event log from using NV log module
 *              And displays the latest events. Up and down key short press for
 *              incrementing and decrementing. Up and down long press for mode
 *              change.
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        15th July 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef APL_HMI_EVENT_LOG_UI_EVENT_LOG_UI_UI_H_
#define APL_HMI_EVENT_LOG_UI_EVENT_LOG_UI_UI_H_


#include "../GENSET_CONTROL/GCU_ALARMS/GCU_ALARMS.h"
#include "../../MW/Display/Display.h"
#include "../CONST_UI/CONST_UI.h"
#include "CFGC/CFGC.h"
#include "config.h"

//#define USB_DEBUG

class EVENT_LOG_UI
{
public:
    EVENT_LOG_UI(HAL_Manager &hal, Display &Disp, CFGC &cfgc , GCU_ALARMS &gcuAlarms , CFGZ &cfgz);
    /**
     * Update routing for Event log mode
     * @param bEntry
     * @return None
     */
    void Update(bool bEntry);

    /**
     * Key press handling, It handles increment and decrement event number
     * and mode change.
     * @param _sKeyEvent
     * @return None
     */
    void CheckKeyPress(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent);
private:
    #define  MAX_EVENT_LOG  _cfgc.GetMaxNumberOfEvents()
    HAL_Manager &_hal;
    Display     &_Disp;
    CFGC        &_cfgc;
    GCU_ALARMS  &_gcuAlarms;
    CFGZ        &_cfgz;
    const char *strAlaramType[CFGZ::CFGZ_ACTION_NoWESN_LAST]=
    {
        " ",
        "Warning",
        "Electrical Trip",
        "Shutdown",
        "Notification"
    };

    /** Variables for fetching the event log
     */
    uint16_t _u16LatestFetchedEventNum;
    uint16_t _u16NumberOfEvents;

    /**Variables for displaying event log
     */
    uint16_t _u16DispEventScreenNumber;
    uint16_t _u16DispTotalEventNumber;

    /**This will store the event log status
     * update this in event log call back and
     * if successful then only display events
     */
    static  bool _bReadNVStatus;
    static bool _bNoEventPresnet;

   /**
    * To display the event log
    * @param None
    * @return None
    */
   void prvDisplayEventScreen();
   void prvDisplayNoEventScreen();

   /**
    * Screen refresh timer.
    */
   stTimer _RefreshTimer;

   /**Struct for event log
    *
    */
   GCU_ALARMS::EVENT_LOG_t _stEventLog;

   /**
    * Call back for event read
    * @param evt
    * @return none
    */
   static void prvEventReadCb(EEPROM::EVENTS_t evt) ;

};




#endif /* APL_HMI_EVENT_LOG_UI_EVENT_LOG_UI_UI_H_ */
