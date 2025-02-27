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


#include "CMenuItem.h"
#include "Configuration.h"

CMenuItem::CMenuItem()
{
	pItemName = NULL;
	isLeafNode = false;
	pEditableItemsScreen = NULL;
	pMenu = NULL;
	isEnabled = false;
}

CMenuItem::CMenuItem(const char* ItemName, CEditableItemsScreen* EditableItemsScreen)
{
	this->pItemName = ItemName;
	isLeafNode = true;
	this->pEditableItemsScreen = EditableItemsScreen;
	pMenu = NULL;
	isEnabled = true;
}
CMenuItem::CMenuItem(CEditableItemsScreen* EditableItemsScreen)
{
    this->pItemName = NULL;
    isLeafNode = true;
    this->pEditableItemsScreen = EditableItemsScreen;
    pMenu = NULL;
    isEnabled = true;
}

CMenuItem::CMenuItem(const char* ItemName, CMenu* Menu)
{
	this->pItemName = ItemName;
	isLeafNode = false;
	this->pEditableItemsScreen = NULL;
	this->pMenu = Menu;
	isEnabled = true;
}
