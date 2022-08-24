/**
 * @file        [CEditableItem]
 * @brief       Is used to edit the actual value of the Item.
 *              Value can be of any type 8bit, 16bit, 32bit, float.
 * @version     [Version number]
 * @author      [Pramod Ranade]
 * @date        05th July 2021
 * @author      Madhuri A
 * @date        27 Sep 2021
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/


#ifndef _CEDITABLEITEM_H_
#define _CEDITABLEITEM_H_
#include "stdint.h"
#include "GLCD.h"
#include "../mw/DISPLAY/Display.h"
#pragma once
#include "Configuration.h"



class CEditableItem
{

public:

    typedef enum
    {
        NOT_ALLOWED             =0,
        PIN1_ALLOWED            =1,
        PIN2_ALLOWED            =2,
        PIN1_PIN2_ALLOWED       =3,
        PIN3_ALLOWED            =4,
        PIN1_PIN3_ALLOWED       =5,
        PIN2_PIN3_ALLOWED       =6,
        PIN1_PIN2_PIN3_ALLOWED  =7,
    }PASS_t;

    typedef struct
    {
       uint8_t u8Date;
       uint8_t u8Month;
       uint16_t u16Year;
    }DATE_t;

    typedef struct
    {
       uint8_t u8PassChar1;
       uint8_t u8PassChar2;
       uint8_t u8PassChar3;
       uint8_t u8PassChar4;
    }PASSWORD_t;

    typedef struct
    {
        uint8_t u8EngSrNoChar1;
        uint8_t u8EngSrNoChar2;
        uint8_t u8EngSrNoChar3;
        uint8_t u8EngSrNoChar4;
        uint8_t u8EngSrNoChar5;
        uint8_t u8EngSrNoChar6;
        uint8_t u8EngSrNoChar7;
        uint8_t u8EngSrNoChar8;
        uint8_t u8EngSrNoChar9;
        uint8_t u8EngSrNoChar10;
        uint8_t u8EngSrNoChar11;
        uint8_t u8EngSrNoChar12;
    }ENG_SR_NO_t;

    typedef enum
    {
        DT_CHAR,
        DT_UINT8,
        DT_INT16,
        DT_UINT16,
        DT_INT32,
        DT_UINT32,
        DT_FLOAT,
        DT_TIME_HRS_MINS,
        DT_DATE,
        DT_PASSWORD,
        DT_ENG_SR_NO,
       // DT_STRING,  // string, entered by user
        DT_STRING_FIXED // string, selected from available options
    } EDITABLE_ITEMS_DATA_TYPE_t;


    typedef union
    {
        char chVal;
        uint8_t u8Val;
        int16_t i16Val;
        uint16_t u16Val;
        int32_t i32Val;
        uint32_t u32Val;
        float fVal;
        DATE_t stDate;
        PASSWORD_t stPassword;
        ENG_SR_NO_t stEngSrNo;
     //   char strVal[STRING_PARAM_MAX_LEN + 1];  // useful only if type is DT_STRING
        uint32_t u32IndexIntoFixedOptions;  // useful only if type is DT_STRING_FIXED
    } EditableItemValue_t;

    EditableItemValue_t value;
    EditableItemValue_t minVal, maxVal;
    float fValLC;  //Least Count of float variable
    const char* promptMessage;
    const char* unitOfMeasurement;
    /*
        On the view/edit parameter screen, the item will be displaued thus:
            promptMessage   value   unitOfMeasurement
        e.g.
            "Kp     0.75    "
        or
            "Low Voltage Threshold      350     Volts"
        Actual call to sprintf can be of the form:
            sprintf(dest, formatString, promptMessage, value.fVal, unitOfMeasurement);
            or
            sprintf(dest, formatString, promptMessage, value.i32Val, unitOfMeasurement);
            etc.
        First and last %s in the formatString correspond to prompt and UoM respectively.
    */
    EditableItemValue_t tempValue;  // the one user is editing
    // tempValue will be copied to value if user presses OK button
    EDITABLE_ITEMS_DATA_TYPE_t dataType;    // enum for data type
    const char* formatString; // e.g. "%s%5.1%s" for float or "%s%3u%s" for uint16_t etc
    const char** stringFixedOptions;    // actually an array of char*
    int numOfStringFixedOptions;
    uint8_t u8PasswordLevel;
    uint8_t u8MultiItemEditIndex;
    uint8_t u8MaxOneScreenEditItems;

    CEditableItem();
    CEditableItem(DATE_t stVal, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, DATE_t stminval ,  DATE_t stmaxval , PASS_t  ePassLevel);
    CEditableItem(char chVal, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, char minChVal = -128, char maxChVal = 127, PASS_t  ePassLevel = PIN1_PIN2_PIN3_ALLOWED );
    CEditableItem(uint8_t u8Val, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, uint8_t minU8Val = 0, uint8_t maxU8Val = 255, PASS_t  ePassLevel = PIN1_PIN2_PIN3_ALLOWED );
    CEditableItem(int16_t i16val, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, int16_t minI16val = -32768, int16_t maxI16val = 32767, PASS_t  ePassLevel = PIN1_PIN2_PIN3_ALLOWED );
    CEditableItem(uint16_t u16val, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, uint16_t minU16val = 0, uint16_t maxU16val = 65535, PASS_t  ePassLevel = PIN1_PIN2_PIN3_ALLOWED );
    CEditableItem(uint16_t u16Val, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, uint16_t minU16val = 0 , uint16_t maxU16val = 65535, bool bflag = true ,PASS_t  ePassLevel = PIN1_PIN2_PIN3_ALLOWED);

    CEditableItem(int32_t i32val, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, int32_t minI32val = -2147483648L, int32_t maxI32val = 2147483647L, PASS_t  ePassLevel = PIN1_PIN2_PIN3_ALLOWED );
    CEditableItem(uint32_t u32val, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, uint32_t minU32val = 0, uint32_t maxU32val = 4294967295UL, PASS_t  ePassLevel = PIN1_PIN2_PIN3_ALLOWED );
    CEditableItem(float fVal, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, float minFVal = -999999999999.0f, float maxFVal = 999999999999.0f, float fValLC = 0.1f, PASS_t  ePassLevel = PIN1_PIN2_PIN3_ALLOWED );
    CEditableItem(PASSWORD_t stVal, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, PASSWORD_t stminval = {0,0,0,0} ,  PASSWORD_t stmaxval = {9,9,9,9} , PASS_t  ePassLevel  = PIN1_PIN2_PIN3_ALLOWED);
    CEditableItem(ENG_SR_NO_t stVal, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, ENG_SR_NO_t stminval = {1,1,1,1,1,1,1,1,1,1,1,1} ,  ENG_SR_NO_t stmaxval = {255,255,255,255,255,255,255,255,255,255,255,255} , PASS_t  ePassLevel  = PIN1_PIN2_PIN3_ALLOWED);
    //CEditableItem(char *pStrVal, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, unsigned int minLength = 1, unsigned int maxLength = STRING_PARAM_MAX_LEN, PASS_t  ePassLevel = PIN1_PIN2_PIN3_ALLOWED );
    CEditableItem(uint32_t u32CurrentOption, const char* promptMessage, const char* unitOfMeasurement, const char* formatString, const char** stringFixedOptions, uint32_t numOfStringFixedOptions, PASS_t  ePassLevel = PIN1_PIN2_PIN3_ALLOWED );
             // converts value to string and returns the string
    const char* getString(EditableItemValue_t aValue);   // converts given value
    const char* getString();    // converts the member 'value'
    bool isValidValue(EditableItemValue_t newVal);
    EditableItemValue_t getValue();
    bool setValue(EditableItemValue_t newVal);
    bool string2value(const char* str, EditableItemValue_t* destVal);
    void incrementValue();  // increment value
    void decrementValue();  // decrement value
    void incrementValue(bool bTemp, bool bIncrementBy5);  // increment value or tempValue
    void decrementValue(bool bTemp,  bool bdecrementBy5);  // decrement value or tempValue
    EditableItemValue_t incrementValue(EditableItemValue_t aValue, bool bIncrementBy5); // increments the given value and returns it
    EditableItemValue_t decrementValue(EditableItemValue_t aValue, bool bdecrementBy5); // decrements the given value and returns it
    void print();
    void print(EditableItemValue_t val);
    void print(bool bTemp);
    void initTempValue();
    void saveTempValue();
    void CopyPrevValue();
    static bool IsAnyConfigValueEdited();
    static void ClearConfigEditStatus(void);

private:

    char valueString[STRING_PARAM_MAX_LEN + 1];
    static bool _bValuesChanged;
    const char* dt2str(EDITABLE_ITEMS_DATA_TYPE_t dt);

    void prvPrint_Password_Edit_Screen(EditableItemValue_t val);
    void DisplayEngSrChar(uint8_t val);
};

#endif /* _CEDITABLEITEM_H_ */
