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

CMenu::CMenu()
{
    this->pMenuTitle = "CONFIGRATION";
    this->numOfMenuItems = 0;
    indexOfSelectedMenuItem = 0;
    isEnabled = false;
    pMenuItems = NULL;
}
CMenu::CMenu( const char *MenuTitle, int NumOfMenuItems, CMenuItem menuItems[])
{
    this->pMenuTitle = MenuTitle;
    if (NumOfMenuItems < 0)
    {
        this->numOfMenuItems = 0;
    }
    else if (NumOfMenuItems > MAX_MENU_ITEMS)
    {
        this->numOfMenuItems = MAX_MENU_ITEMS;
    }
    else
    {
        this->numOfMenuItems = NumOfMenuItems;
    }
    pMenuItems = menuItems;
    indexOfSelectedMenuItem = 0;
    isEnabled = true;
}
void CMenu::show()
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
