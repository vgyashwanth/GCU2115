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
#include "Configuration.h"
#include "CMenu.h"
#include "CMenuItem.h"
extern UI_STATES_t uiState;
extern Display *gpDisplay;

extern uint16_t u16IndexOfEditableItems,u16NumberofEditableItems;
static const uint8_t gau8ChineseConfiguration[5][15] =
{
        {0x00,  0x07,   0x01,   0x01,   0x07,   0x05,   0x05,   0x05,   0x06,   0x04,   0x04,   0x07,   0x04,   0x04,   0x07},
        {0x00,  0xF3,   0x40,   0x40,   0xF0,   0x50,   0x53,   0x52,   0x72,   0x12,   0x12,   0xF2,   0x12,   0x12,   0xF1},
        {0x07,  0xE4,   0x27,   0x20,   0x27,   0x20,   0xE0,   0x04,   0x04,   0x04,   0x04,   0x04,   0x14,   0x14,   0xF7},
        {0xFF,  0x44,   0xFF,   0x10,   0xFF,   0x10,   0xFF,   0x80,   0xFF,   0x80,   0xFF,   0x80,   0xFF,   0x00,   0xFF},
        {0xE0,  0x20,   0xE0,   0x00,   0xF0,   0x00,   0x80,   0x80,   0x80,   0x80,   0x80,   0x80,   0x80,   0x00,   0xF0},
};

CMenu::CMenu()
{
    this->pMenuTitle = "CONFIGRATION";
    this->numOfMenuItems = 0;
    indexOfSelectedMenuItem = 0;
    isEnabled = false;
}
CMenu::CMenu( const char *MenuTitle, int numOfMenuItems, CMenuItem menuItems[])
{
    this->pMenuTitle = MenuTitle;
    if (numOfMenuItems < 0)
    {
        this->numOfMenuItems = 0;
    }
    else if (numOfMenuItems > MAX_MENU_ITEMS)
    {
        this->numOfMenuItems = MAX_MENU_ITEMS;
    }
    else
    {
        this->numOfMenuItems = numOfMenuItems;
    }
    pMenuItems = menuItems;
    indexOfSelectedMenuItem = 0;
    isEnabled = true;
}
void CMenu::show(bool bChineseSelected)
{
#if (RUNNING_ON == TARGET_DESKTOP)
    if (isEnabled)
    {
        ClearScreen();
        printf("%s\n", pMenuTitle);
        int i;
        for (i = 0; i < numOfMenuItems; i++)
        {
            if (i == indexOfSelectedMenuItem)
            {
                printf("-->");
            }
            else
            {
                printf("   ");
            }
            printf("%s\n", pMenuItems[i].pItemName);
        }
    }
    else
    {
        printf("This menu is disabled!\n");
    }
#elif (RUNNING_ON == TARGET_GCU)
    char arrTemp[20]={0};

    if(bChineseSelected && (strcmp("CONFIGURATION", (char*)this->pMenuTitle) == 0))
    {
        gpDisplay->printImage((uint8_t*)gau8ChineseConfiguration, 5, 15, 3, 30);
    }
    else
    {
        gpDisplay->gotoxy(GLCD_X(50),GLCD_Y(5));
        gpDisplay->printStringCenterAligned((char*)this->pMenuTitle,FONT_VERDANA);
    }
    gpDisplay->gotoxy(GLCD_X(125),GLCD_Y(5));
    sprintf(arrTemp,"%d/%d", u16IndexOfEditableItems + 1, u16NumberofEditableItems );
    gpDisplay->printStringRightAligned((char*)arrTemp,FONT_VERDANA);
    gpDisplay->gotoxy(GLCD_X(64),GLCD_Y(37));
    if(uiState == UI_STATE_SHOWING_MENU)
    {
        if(pMenuItems[indexOfSelectedMenuItem].isLeafNode)
        {
            gpDisplay->gotoxy(GLCD_X(64),GLCD_Y(25));
            gpDisplay->printStringCenterAligned(
                    (char*)pMenuItems[indexOfSelectedMenuItem].pEditableItemsScreen->pEditableItems[0].promptMessage,FONT_VERDANA);
            gpDisplay->gotoxy(GLCD_X(64),GLCD_Y(45));
            pMenuItems[indexOfSelectedMenuItem].pEditableItemsScreen->pEditableItems[0].print(true);
        }
        else
        {
            gpDisplay->printStringCenterAligned((char *)pMenuItems[indexOfSelectedMenuItem].pItemName,FONT_VERDANA);
        }

    }
    else if(uiState == UI_STATE_SHOWING_SCREEN_ITEM_SELECTION)
    {
        gpDisplay->printStringCenterAligned((char *)pMenuItems[0].pItemName,FONT_VERDANA);
    }


#else
#error No valid Target defined!
#endif
}
