/**
 * @file        [CMenu]
 * @brief
 * @version     [Version number]
 * @author      [Pramod Ranade]
 * @date        05th July 2021
 * @author      Madhuri A
 * @date        27 Sep 2021
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef _CMENU_H_
#define _CMENU_H_

#pragma once

class CMenuItem;

#include "CMenuItem.h"
#include "../../mw/DISPLAY/Display.h"
#include "HAL_Manager.h"

class CMenu
{
private:
public:
    CMenuItem* pMenuItems;
    int numOfMenuItems;
    int indexOfSelectedMenuItem;
    const char* pMenuTitle;
    bool isEnabled;
    CMenu();
    CMenu(const char* MenuTitle, int NumOfMenuItems, CMenuItem menuItems[]);
    void show();
};




#endif /* _CMENU_H_ */
