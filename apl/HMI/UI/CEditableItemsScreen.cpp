/**
 * @file        [CEditableItemsScreen]
 * @brief       This is used to display edited config value value
 * @version     [Version number]
 * @author      [Pramod Ranade]
 * @date        05th July 2021
 * @author      Madhuri A
 * @date        27 Sep 2021
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#include "Configuration.h"
#include "UI.h"
#include "CEditableItemsScreen.h"
#include <stdio.h>
#include "CMenu.h"

extern Display *gpDisplay;
//extern CMenu* pCurMenu;
extern uint16_t u16IndexOfEditableItems,u16NumberofEditableItems;
CEditableItemsScreen::CEditableItemsScreen()
{
	numOfEditableItems = 0;
	indexOfSelectedEditableItem = 0;
	pScreenTitle = NULL;
	pEditableItems = NULL;
	pXpositions = NULL;
	pYpositions = NULL;
	xPos = 0;
	yPos = 0;
}
CEditableItemsScreen::CEditableItemsScreen(const char* ScreenTitle, int NumOfEditableItems, CEditableItem editableItems[], int xPositions[], int yPositions[])
{
	if (NumOfEditableItems < 0)
	{
		this->numOfEditableItems = 0;
	}
	else if (numOfEditableItems > MAX_EDITABLE_ITEMS_PER_SCREEN)
	{
		this->numOfEditableItems = MAX_EDITABLE_ITEMS_PER_SCREEN;
	}
	else
	{
		this->numOfEditableItems = NumOfEditableItems;
	}
	this->pEditableItems = &(editableItems[0]);
	this->indexOfSelectedEditableItem = 0;
	this->pScreenTitle = ScreenTitle;
	this->pXpositions = &(xPositions[0]);
	this->pYpositions = &(yPositions[0]);
	this->xPos = 0;
	this->yPos = 0;
}
CEditableItemsScreen::CEditableItemsScreen(const char* ScreenTitle, CEditableItem* EditableItem, int xPosition, int yPosition)
{
	this->numOfEditableItems = 1;
	this->pEditableItems = EditableItem;
	this->indexOfSelectedEditableItem = 0;
	this->pScreenTitle = ScreenTitle;
	this->xPos = xPosition;
	this->yPos = yPosition;

	this->pXpositions = NULL;
	this->pYpositions = NULL;
}
void CEditableItemsScreen::initTempValues()
{
	int i;
	for (i = 0; i < numOfEditableItems; i++)
	{
		pEditableItems[i].initTempValue();
	}
}
void CEditableItemsScreen::saveTempValues()
{
	int i;
	for (i = 0; i < numOfEditableItems; i++)
	{
		pEditableItems[i].saveTempValue();
	}
}

void CEditableItemsScreen::CopyPrevValue()
{
    int i;
    for (i = 0; i < numOfEditableItems; i++)
    {
        pEditableItems[i].CopyPrevValue();
    }
}

void CEditableItemsScreen::showNthEditableItem(int n, bool bTemp)
{
#if (RUNNING_ON == TARGET_DESKTOP)
	if (n == indexOfSelectedEditableItem)
	{
		printf("-->");
	}
	else
	{
		printf("   ");
	}
	if (numOfEditableItems == 1)
	{
		printf("Location co-ordinates are: %d, %d\n", xPos, yPos);
	}
	else
	{
		printf("Location co-ordinates are: %d, %d\n", pXpositions[n], pYpositions[n]);
	}
	pEditableItems[n].print(bTemp);
	//printf("Prompt is {%s}\n", pEditableItems[n].promptMessage);
	//printf("Value is {%s}\n", pEditableItems[n].getString());
	//printf("UoM is {%s}\n", pEditableItems[n].unitOfMeasurement);
#elif (RUNNING_ON == TARGET_GCU)

    if (numOfEditableItems == 1)
    {
        gpDisplay->gotoxy(GLCD_X(64),GLCD_Y(25));
        gpDisplay->printStringCenterAligned((char*)pEditableItems[n].promptMessage,FONT_VERDANA);
        gpDisplay->gotoxy(GLCD_X(64),GLCD_Y(45));
        pEditableItems[n].print(bTemp);
    }
    else
    {
        gpDisplay->gotoxy(GLCD_X(64),GLCD_Y(25));
        gpDisplay->printStringCenterAligned((char*)pEditableItems[n].promptMessage,FONT_VERDANA);
        gpDisplay->gotoxy(GLCD_X(pXpositions[n]),GLCD_Y( pYpositions[n]));
        pEditableItems[n].print(bTemp);
    }

#else
#error No valid Target defined!
#endif
}
void CEditableItemsScreen::show(bool bTemp)
{
    char arrTemp[20]={0};
#if (RUNNING_ON == TARGET_DESKTOP)
	printf("Screen title is {%s}\n", pScreenTitle);
	int i;
	for (i = 0; i < numOfEditableItems; i++)
	{
		showNthEditableItem(i, bTemp);
	}
#elif (RUNNING_ON == TARGET_GCU)
	 gpDisplay->gotoxy(GLCD_X(50),GLCD_Y(5));
     gpDisplay->printStringCenterAligned((char*)pScreenTitle,FONT_VERDANA);
     gpDisplay->gotoxy(GLCD_X(125),GLCD_Y(5));
     sprintf(arrTemp,"%d/%d", u16IndexOfEditableItems + 1 , u16NumberofEditableItems);
     gpDisplay->printStringRightAligned((char*)arrTemp,FONT_VERDANA);
     gpDisplay->gotoxy(GLCD_X(64),GLCD_Y(37));
     int i;
     for (i = 0; i < numOfEditableItems; i++)
     {
         showNthEditableItem(i, bTemp);
     }


#else
#error No valid Target defined!
#endif
}
void CEditableItemsScreen::show()
{
	show(false);
}
