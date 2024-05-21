/**
 * @file        [ui]
 * @brief
 * @version     [Version number]
 * @author      [Pramod Ranade]
 * @date        05th July 2021
 * @author      Madhuri A
 * @date        27 Sep 2021
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef _UI_H_
#define _UI_H_

#pragma once

#include <stdint.h>
#include "Keypad.h"
#include "Configuration.h"
#include "CEditableItem.h"
#include "CEditableItemsScreen.h"
#include "CMenu.h"
#include "CMenuItem.h"
#include "HAL_Manager.h"
#include "../PASSWORD_ENTRY_UI/PASSWORD_ENTRY_UI.h"
#include "CFGC/CFGC.h"
#include <HMI/UI_DS.h>
#include "productSelection.h"
#include "../../MB_APP/MB_APP.h"

/*extern char* arrMonth[12];*/

    const uint8_t DaysInMonth[] =
    {
        31, // Jan
        29, // Feb
        31, // Mar
        30, // Apr
        31, // May
        30, // June
        31, // July
        31, // Aug
        30, // Sept
        31, // Oct
        30, // Nov
        31  // Dec
    };

class UI
{

public:

    typedef struct
    {
        bool bPIN1Changed;
        bool bPIN2Changed;
    }PASSWORD_EDIT_FLAGS_t;

    UI(HAL_Manager &hal, PASSWORD_ENTRY_UI &Password, CFGZ &cfgz, Display &Disp,ENGINE_MONITORING &engMon, MB_APP &MbApp);
    static PASSWORD_EDIT_FLAGS_t stPassEdit;
    void Handler(int keyCode);
    void ClearScreen();
    void SaveConfigFile();
    void CheckKeyPress();
    void ConfigUIUpdate();
    void ConfigCheckKeyPress(KEYPAD::KEYPAD_EVENTS_t _sKeyEvent);
    void Initialize(void);
    void InitEditableItems();
    void InitEditableItemsScreens();
    void InitMenuItemsAndMenus();
    void UpdateMaxNumberOfItem();
    bool IsOutputConfigured(uint8_t u8Source);
    void TurnOFFActionActivation(uint16_t u16Index);
    void DigitalInputMenuVisiblity(uint16_t u16SourceIndex);
    void ResetDigitalInputSource(uint16_t u16SourceIndex);
    void DigitalOutputMenuVisiblity(uint16_t u16SourceIndex);
    void MenuVisibilityOfGenThreshAndDly(uint16_t u16Index);
    void MenuVisiblituyOfMainsThresh(uint16_t u16Index);
    void HandleMenuVisibility(void);
    void LowestLevelMenuEnDis(uint16_t u16StartIndex , uint16_t u16EndIndex , bool bEn_Ds);
    void InitialiseCustomSensor();

    CEditableItem ArrEditableItem[INDEX_LAST];
    CEditableItemsScreen ArrEditableItemScreen[INDEX_LAST];
    CMenu ArrSubMenu[ID_SUB_MENU_LAST];
    CMenu ArrMenu[ID_MAIN_MENU_LAST];
    static CMenuItem menuItemsLowestLevel[INDEX_LAST];
    static CMenuItem menuItemsMidLevel[NUM_OF_MENU_ITEMS_AT_MID_LEVEL];
    static CMenuItem menuItemsTopLevel[NUM_OF_MENU_ITEMS_AT_TOP_LEVEL];

private:
    #define IsLeapYear(Y) ((!((Y)%4)) && (((Y)%100) || (!((Y)%400))))

    HAL_Manager &_objHal;
    PASSWORD_ENTRY_UI &_objPassword;
    CFGZ &_objcfgz;
    Display &_objDisplay;
    ENGINE_MONITORING &_engMon;
    MB_APP &_mbApp;
    uint16_t _u16MenuSp;
//     menu stack pointer (actually index into menuStack)
    CMenu* _menuStack[MENU_STACK_DEPTH];
    MISC_PARAM_t _MiscParam;
    char* _arrProfileNames[MAX_NUMBER_OF_PROFILE];
    char profilename[MAX_NUMBER_OF_PROFILE][21];
    uint8_t _u8LanguageArrayIndex;
    CEditableItemsScreen* _pCurEditableItemsScreen;
    stTimer _ValIncDecTimer;
    CEditableItem::DATE_t _stMaintenanceDt;


    void prvFetchProfileNames();
    uint16_t prvMaxDaysInMonth(uint8_t u8Month ,uint16_t u16Year);
    static uint16_t GetEditableItemIndex();
    void prvInitialiseECUParam(void);
    void prvUpdateEngineParam(void);
    void prvUpdateEngineTypeDependency(void);
    void prvSetPasswordAccessLevel(uint16_t u16Index, uint8_t u8PasswordLevel);
    void prvInitModbusMap();
};
#endif /* _UI_H_ */
