/**
 * @file        [CMenuItem]
 * @brief       This class is used for configuring all menus.
 *              eg. Main Menu, Sub Menu, Leaf node.
 * @version     [Version number]
 * @author      [Pramod Ranade]
 * @date        05th July 2021
 * @author      Madhuri A
 * @date        27 Sep 2021
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef _CMENUITEM_H_
#define _CMENUITEM_H_


#pragma once

class CMenu;
#include "stdint.h"
#include "string.h"
#include "CMenu.h"
#include "CEditableItemsScreen.h"
#include "GLCD.h"

class CMenuItem
{
private:
public:
    bool isLeafNode;
    CEditableItemsScreen* pEditableItemsScreen;   // use this if leaf node
    CMenu* pMenu;   // use this if not leaf node
    bool isEnabled;
    const char* pItemName;
    CMenuItem();
    CMenuItem(const char *ItemName, CEditableItemsScreen *EditableItemsScreen);
    CMenuItem(CEditableItemsScreen *EditableItemsScreen);
    CMenuItem(const char *ItemName, CMenu *Menu);
};



#endif /* _CMENUITEM_H_ */
