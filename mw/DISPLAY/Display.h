/*
 * Display.h
 *
 *  Created on: Jul 1, 2021
 *      Author: pramod.ranade
 */

#ifndef MW_DISPLAY_H_
#define MW_DISPLAY_H_

#include "../../bsp/GLCD/GLCD.h"

#define GLCD_X(xVal)  xVal
#define GLCD_Y(yVal)  yVal


typedef enum
{
    FONT_ARIAL,
    FONT_VERDANA
} FONT_t;
class Display
{
    uint8_t curX, curY;
    FONT_t curFont;
public:

    Display(GLCD &glcd);
    void ClearScreen();
    void drawHorizontalLine(unsigned int x1, unsigned int y, unsigned int x2);
    void drawVerticalLine(unsigned int x, unsigned int y1, unsigned int y2);
    void clearRectangularSection(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2);
    void drawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
    void drawRectangle();

    // move the current position to the specified co-ordinates:
    void gotoxy (unsigned int x, unsigned int y);
    void setFont (FONT_t font);
    void printChar(char ch);
    void printChar(char ch, FONT_t font);
    void printChar(char ch, FONT_t font, bool reverseVideo);
    uint8_t getCharWidth(char ch);
    uint8_t getCharWidth(char ch, FONT_t font);
    uint8_t FindStringWidth(char* str, FONT_t font);
    // display the given string at the current or specified position
    // text will be left-aligned, centre-aligned or right-aligned
    void printStringLeftAligned (char *str);
    void printStringLeftAligned (char *str, FONT_t font);
    void printStringLeftAligned (char *str, FONT_t font, bool reverseVideo);
//    void printStringLeftAligned (char *str, unsigned int x, unsigned int y);
    void printStringCenterAligned (char *str);
    void printStringCenterAligned (char *str, FONT_t font);
    void printStringCenterAligned (char *str, FONT_t font, bool reverseVideo);

//    void printStringCentreAligned (char *str, unsigned int x, unsigned int y);

    void printStringRightAligned (char *str);
    void printStringRightAligned (char *str, FONT_t font);
    void printStringRightAligned (char *str, FONT_t font, bool reverseVideo);
//    // displays the image at current or specified position
//    void printImage (struct Image *pImage);
     void printImage (uint8_t *pImage, uint8_t u8ImageSizeInColumns,
                                           uint8_t u8ImageSizeInRows, uint8_t u8Row, uint8_t u8Column);
//    // returns the length in pixels for the given string for current or specified font
//    unsigned int getTextLength (char *str);
//    unsigned int getTextLength (char *str, FONT_t font);
//    // returns the height in pixels for the given string for current or specified font
//    unsigned int getTextHeight (char *str);
//    unsigned int getTextHeight (char *str, FONT_t font);
//    // draws line from current or specified position to the (second) specified position
//    void drawLine(unsigned int x, unsigned int y);  // from cur pos to (x,y)
//    void drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

private:

    GLCD &_glcd;

};

#endif /* MW_DISPLAY_H_ */
