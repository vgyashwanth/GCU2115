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

#ifndef _CEDITABLEITEMSSCREEN_H_
#define _CEDITABLEITEMSSCREEN_H_

#pragma once

#include "CEditableItem.h"

class CEditableItemsScreen
{
private:
    const char* pScreenTitle;
    // x and y coordinates
    // use these if numOfEditableItems == 1
    int xPos, yPos;
    // arrays of x and y coordinates
    // use these if numOfEditableItems > 1
    int* pXpositions;
    int* pYpositions;
public:
    int indexOfSelectedEditableItem;
    int numOfEditableItems;
    CEditableItem* pEditableItems;
    CEditableItemsScreen();
    // use the constructor below if the screen has multiple "editable items"
    CEditableItemsScreen(const char* pScreenTitle, int numOfEditableItems, CEditableItem editableItems[], int xPositions[], int yPositions[]);
    // use the constructor below if the screen has only one "editable items"
    CEditableItemsScreen(const char* pScreenTitle, CEditableItem* pEditableItem, int xPosition, int yPosition);
    void showNthEditableItem(int n, bool bTemp);
    void show();
    void show(bool bTemp);
    void initTempValues();
    void saveTempValues();
    void CopyPrevValue();
};




#endif /* _CEDITABLEITEMSSCREEN_H_ */
