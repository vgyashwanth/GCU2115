/**
 * @file        [PASSWORD_ENTRY_UI]
 * @brief       This module is used to password entry screens.
 *              This module will validate the entered password.
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        15th July 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef APL_HMI_PASSWORD_ENTRY_UI_PASSWORD_ENTRY_UI_H_
#define APL_HMI_PASSWORD_ENTRY_UI_PASSWORD_ENTRY_UI_H_

#include <HMI/UI_DS.h>

#include "../../MW/Display/Display.h"
#include "../../apl/GENSET_CONTROL/GCU_MODES/MANUAL_MODE.h"
#include "CFGZ.h"
#include "config.h"
#include "../CONST_UI/CONST_UI.h"
#define PASSWORD_MIN_CHAR (0U)
#define PASSWORD_MAX_CHAR (9U)

#define MASTER_PASSWORD   (718U)

#define ASCII_CONST       (0x30U)

class PASSWORD_ENTRY_UI
{
public:
    typedef enum
    {
        PIN1 = 1,
        PIN2,
        PIN3,
        MASTER_PIN
    }PIN_TYPE_t;

    typedef enum
    {
        ENTRY,
        READ_OR_WRITE,
        PASSWORD_ENTRY
    }STATE_t;


    /**
     * Constructor of Password entry class
     * @param hal object
     * @param cfgz object
     * @param Disp object
     */
    PASSWORD_ENTRY_UI(HAL_Manager &hal, CFGZ &cfgz, Display &Disp);

    /** Update function of password class
     * @param bRefresh
     * @return none
     */
    void Update(bool bRefresh);

    /** Check for key press
     * @param _sKeyEvent -key event
     * @return none
     */
    void CheckKeyPress(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent);

    /** It is used to check password entry status
     * @return true when valid password entered, otherwise false
     */
    bool CheckPasswordStatus();

    /**This will return the state of Password entry class
     * @param none
     * @return STATE_t
     */
    STATE_t GetPasswordEnteryState();

    /**
     * It is used to set the password entry state when going
     * Password entry mode
     * @param eState
     * @return none
     */
    void SetPasswordEntryState(STATE_t eState);

    uint8_t GetEnteredPassword();

private:

    typedef enum
    {
        CHAR1,
        CHAR2,
        CHAR3,
        CHAR4,
        CHAR_LAST,
    }EDIT_CHAR_t;

    /**Object references
     */
    HAL_Manager &_hal;
    CFGZ &_cfgz;
    Display &_Disp;

    /**To display password while editing
     */
    char ArrChar[CHAR_LAST];

    /**This will contain entered password value
     */
    uint16_t _u16EnterdPassword;

    /**This will contain entered password valid or invalid
     */
    bool _bPasswordStatus;

    /** Password entry state
     */
    STATE_t _eState;

    /** Char number for editing
     */
    EDIT_CHAR_t _eCharNumber;

    uint8_t _u8EnteredPassword;

    uint16_t _u16PIN[2];

    /**
     * To increment the password value
     * @param ch
     * @return char
     */
    char prvIncrementChar(char ch);

    /**
     * To decrement the password value
     * @param ch
     * @return char
     */
    char prvDecrementChar(char ch);

    /**
     * Check entered password validity
     * @param none
     * @param none
     */
    void prvCheckEnteredPassword();

    /**
     * Display screen for password editing
     * @param none
     * @return none
     */
    void prvDisplayPassEditScreen();

    /**
     * Display entry question screen for password editing
     * @param none
     * @return none
     */
    void prvDisplayEntryScreen();


};



#endif /* APL_HMI_PASSWORD_ENTRY_UI_PASSWORD_ENTRY_UI_H_ */
