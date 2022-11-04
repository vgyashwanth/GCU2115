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

#include "Configuration.h"

#include "CEditableItem.h"
#include <HMI/UI/UI.h>
#include <stdio.h>
#include <string.h>
#include "CFGZ.h"

#include "../../mw/DISPLAY/Display.h"
extern Display *gpDisplay;
extern UI_STATES_t uiState;
extern GLCD *configGlcd;
//extern CFGZ *pcfgz;

bool CEditableItem::_bValuesChanged= false;

const char* CEditableItem::dt2str(EDITABLE_ITEMS_DATA_TYPE_t dt)
{
    /*LDRA_INSPECTED 62 S */
    switch (dt)
    {
    case    DT_CHAR:
        return "DT_CHAR";
    case    DT_UINT8:
        return "DT_UINT8";
    case    DT_INT16:
        return "DT_INT16";
    case    DT_UINT16:
        return "DT_UINT16";
    case    DT_INT32:
        return "DT_INT32";
    case    DT_UINT32:
        return "DT_UINT32";
    case    DT_FLOAT:
        return "DT_FLOAT";
//    case    DT_STRING:
//        return "DT_STRING";
    case    DT_STRING_FIXED:
        return "DT_STRING_FIXED";
    default:
        return "UNKNOWN";
    }
}



CEditableItem::CEditableItem()
{
    this->dataType = DT_UINT32; // default data type
    this->value.u32Val = 0;
    this->minVal.u32Val = 0;
    this->maxVal.u32Val = 99;
    this->promptMessage = NULL;
    this->unitOfMeasurement = NULL;
    this->formatString = NULL;
    this->stringFixedOptions = NULL;
    this->numOfStringFixedOptions = 0;
    this->u8PasswordLevel=PIN1_PIN2_PIN3_ALLOWED;
    u8MaxOneScreenEditItems = 0;
    initTempValue();
}
CEditableItem::CEditableItem(char chVal, const char* PromptMessage,
        const char* UnitOfMeasurement, const char* FormatString,
        char minChVal, char maxChVal, PASS_t  ePassLevel)
{
    CEditableItem();
    this->value.chVal = chVal;
    this->minVal.chVal = minChVal;
    this->maxVal.chVal = maxChVal;
    this->dataType = DT_CHAR;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->u8PasswordLevel = ePassLevel;
    u8MaxOneScreenEditItems = 0;
    initTempValue();
}
CEditableItem::CEditableItem(uint8_t u8Val, const char* PromptMessage,
        const char* UnitOfMeasurement, const char* FormatString,
        uint8_t minU8Val, uint8_t maxU8Val, PASS_t  ePassLevel)
{
    CEditableItem();
    this->value.u8Val = u8Val;
    this->minVal.u8Val = minU8Val;
    this->maxVal.u8Val = maxU8Val;
    this->dataType = DT_UINT8;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->u8PasswordLevel = ePassLevel;
    u8MaxOneScreenEditItems = 0;
    initTempValue();
}
CEditableItem::CEditableItem(int16_t i16Val, const char* PromptMessage,
        const char* UnitOfMeasurement, const char* FormatString,
        int16_t minI16val, int16_t maxI16val, PASS_t  ePassLevel)
{
    CEditableItem();
    this->value.i16Val = i16Val;
    this->minVal.i16Val = minI16val;
    this->maxVal.i16Val = maxI16val;
    this->dataType = DT_INT16;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->u8PasswordLevel = ePassLevel;
    u8MaxOneScreenEditItems = 0;
    initTempValue();
}
CEditableItem::CEditableItem(uint16_t u16Val, const char* PromptMessage,
        const char* UnitOfMeasurement, const char* FormatString,
        uint16_t minU16val, uint16_t maxU16val, PASS_t  ePassLevel)
{
    CEditableItem();
    this->value.u16Val = u16Val;
    this->minVal.u16Val = minU16val;
    this->maxVal.u16Val = maxU16val;
    this->dataType = DT_UINT16;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->u8PasswordLevel = ePassLevel;
    u8MaxOneScreenEditItems = 0;
    initTempValue();
}
CEditableItem::CEditableItem(uint16_t u16Val, const char* PromptMessage,
        const char* UnitOfMeasurement, const char* FormatString,
        uint16_t minU16val, uint16_t maxU16val, bool bflag ,PASS_t  ePassLevel)
{
    CEditableItem();
    this->value.u16Val = u16Val;
    this->minVal.u16Val = minU16val;
    this->maxVal.u16Val = maxU16val;
    this->dataType = DT_TIME_HRS_MINS;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->u8PasswordLevel = ePassLevel;
    u8MaxOneScreenEditItems =0;
    initTempValue();
}
CEditableItem::CEditableItem(PASSWORD_t stVal, const char* PromptMessage, const char* UnitOfMeasurement, const char* FormatString, PASSWORD_t stminval ,  PASSWORD_t stmaxval , PASS_t  ePassLevel  )
{
    CEditableItem();
    this->value.stPassword = stVal;
    this->minVal.stPassword  = stminval;
    this->maxVal.stPassword = stmaxval;
    this->dataType = DT_PASSWORD;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->u8PasswordLevel = ePassLevel;
    u8MultiItemEditIndex= 0;
    u8MaxOneScreenEditItems = 4;
    initTempValue();
}

CEditableItem::CEditableItem(ENG_SR_NO_t stVal, const char* PromptMessage, const char* UnitOfMeasurement, const char* FormatString, ENG_SR_NO_t stminval ,  ENG_SR_NO_t stmaxval , PASS_t  ePassLevel  )
{
    CEditableItem();
    this->value.stEngSrNo = stVal;
    this->minVal.stEngSrNo = stminval;
    this->maxVal.stEngSrNo = stmaxval;
    this->dataType = DT_ENG_SR_NO;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->u8PasswordLevel = ePassLevel;
    u8MultiItemEditIndex= 0;
    u8MaxOneScreenEditItems = 12;
    initTempValue();
}

CEditableItem::CEditableItem(int32_t i32Val, const char* PromptMessage,
        const char* UnitOfMeasurement, const char* FormatString,
        int32_t minI32val, int32_t maxI32val, PASS_t  ePassLevel)
{
    CEditableItem();
    this->value.i32Val = i32Val;
    this->minVal.i32Val = minI32val;
    this->maxVal.i32Val = maxI32val;
    this->dataType = DT_INT32;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->u8PasswordLevel = ePassLevel;
    u8MaxOneScreenEditItems = 0;
    initTempValue();
}
CEditableItem::CEditableItem(uint32_t u32Val, const char* PromptMessage,
        const char* UnitOfMeasurement, const char* FormatString,
        uint32_t minU32val, uint32_t maxU32val, PASS_t  ePassLevel)
{
    CEditableItem();
    this->value.u32Val = u32Val;
    this->minVal.u32Val = minU32val;
    this->maxVal.u32Val = maxU32val;
    this->dataType = DT_UINT32;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->u8PasswordLevel = ePassLevel;
    u8MaxOneScreenEditItems = 0;
    initTempValue();
}
CEditableItem::CEditableItem(float fVal, const char* PromptMessage,
        const char* UnitOfMeasurement, const char* FormatString,
        float minFval, float maxFval, float ValLC, PASS_t  ePassLevel)
{
    CEditableItem();
    this->value.fVal = fVal;
//    this->value.u8Val = (uint8_t)fVal;
//    this->value.i16Val = (int16_t)fVal;
    this->minVal.fVal = minFval;
    this->maxVal.fVal = maxFval;
    this->fValLC = ValLC;
    this->dataType = DT_FLOAT;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->u8PasswordLevel = ePassLevel;
    u8MaxOneScreenEditItems = 0;
    initTempValue();
}
//CEditableItem::CEditableItem(char* pStrVal, const char* promptMessage,
//        const char* unitOfMeasurement, const char* formatString,
//        unsigned int minLength, unsigned int maxLength, PASS_t  ePassLevel)
//{
//    CEditableItem();
//    strcpy(this->value.strVal, pStrVal);
//    this->dataType = DT_STRING;
//    this->promptMessage = promptMessage;
//    this->unitOfMeasurement = unitOfMeasurement;
//    this->formatString = formatString;
//    /*
//    * Although data type of value member is DT_STRING,
//    * the data types of minVal and maxVal will be u32Val.
//    * That is because we want to store min and max length
//    * of the string there. This can be used for validating
//    * user input.
//    */
//    this->minVal.u32Val = minLength;
//    this->maxVal.u32Val = maxLength;
//    u8MultiItemEditIndex= 0;
//    u8MaxOneScreenEditItems = maxLength;
//    this->u8PasswordLevel = ePassLevel;
//    initTempValue();
//}
CEditableItem::CEditableItem( uint32_t u32CurrentOption, const char* PromptMessage,
        const char* UnitOfMeasurement, const char* FormatString,
        const char** StringFixedOptions, uint32_t NumOfStringFixedOptions, PASS_t  ePassLevel)
{
    CEditableItem();
    this->dataType = DT_STRING_FIXED;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->numOfStringFixedOptions = NumOfStringFixedOptions;
    this->stringFixedOptions = StringFixedOptions;
    this->value.u32IndexIntoFixedOptions = u32CurrentOption;
    this->u8PasswordLevel = ePassLevel;
    u8MaxOneScreenEditItems = 0;
    initTempValue();
}

CEditableItem::CEditableItem(DATE_t stVal, const char* PromptMessage,
        const char* UnitOfMeasurement, const char* FormatString,
        DATE_t stminval ,  DATE_t stmaxval , PASS_t  ePassLevel  )
{
    CEditableItem();
    this->value.stDate = stVal;
    this->minVal.stDate  = stminval;
    this->maxVal.stDate = stmaxval;
    this->dataType = DT_DATE;
    this->promptMessage = PromptMessage;
    this->unitOfMeasurement = UnitOfMeasurement;
    this->formatString = FormatString;
    this->u8PasswordLevel = ePassLevel;
    u8MultiItemEditIndex= 0;
    u8MaxOneScreenEditItems = 3;
    initTempValue();
}

const char* CEditableItem::getString(EditableItemValue_t aValue)
{

    switch (dataType)
    {
    case    DT_CHAR:
        sprintf(valueString, formatString, aValue.chVal);
        break;
    case    DT_UINT8:
        sprintf(valueString, formatString, aValue.u8Val);
        break;
    case    DT_INT16:
        sprintf(valueString, formatString, aValue.i16Val);
        break;
    case    DT_UINT16:
        sprintf(valueString, formatString, aValue.u16Val);
        break;
    case    DT_INT32:
        sprintf(valueString, formatString, aValue.i32Val);
        break;
    case    DT_UINT32:
        sprintf(valueString, formatString, aValue.u32Val);
        break;
    case    DT_FLOAT:
        sprintf(valueString, formatString, aValue.fVal);
        break;
//    case    DT_STRING:
//        sprintf(valueString, formatString, aValue.strVal);
//        break;
    case    DT_STRING_FIXED:
        sprintf(valueString, formatString, stringFixedOptions[aValue.u32IndexIntoFixedOptions]);
        break;
    default:
        strcpy(valueString, "Unknown type!");
        break;
    }
    return(const char*)valueString;
}
const char* CEditableItem::getString()
{
    return getString(value);
}
CEditableItem::EditableItemValue_t CEditableItem::incrementValue(
        EditableItemValue_t aValue, bool bIncrementBy5)
{
    // action for up arrow key
    uint16_t u16Mins=0;
    uint16_t u16Hours = 0;
    _bValuesChanged = true; // To indicated as value is edited
    switch (dataType)
    {
    case    DT_CHAR:
        if(bIncrementBy5 && (aValue.chVal + 5 < maxVal.chVal) )
        {
            aValue.chVal += 5 ;
        }
        else if (aValue.chVal < maxVal.chVal)
        {
            aValue.chVal++;
        }
        else
        {
            aValue.chVal = maxVal.chVal;
        }
        break;
    case    DT_UINT8:
        if(bIncrementBy5 && (aValue.u8Val + 5 < maxVal.u8Val) )
        {
            aValue.u8Val += 5 ;
        }
        else if (aValue.u8Val < maxVal.u8Val)
        {
            aValue.u8Val++;
        }
        else
        {
            aValue.u8Val = maxVal.u8Val;
        }
        break;
    case    DT_INT16:
        if(bIncrementBy5 && (aValue.i16Val + 10 < maxVal.i16Val) )
        {
            aValue.i16Val += 10 ;
        }
        else if (aValue.i16Val < maxVal.i16Val)
        {
            aValue.i16Val++;
        }
        else
        {
            aValue.i16Val = maxVal.i16Val;
        }
        break;
    case    DT_UINT16:
        if((bIncrementBy5
                || (strcmp(this->promptMessage , "DUE AT ENGINE HOURS") == 0))
                && (aValue.u16Val + 10 <= maxVal.u16Val) )
        {
            aValue.u16Val += 10 ;
        }
        else if (aValue.u16Val < maxVal.u16Val)
        {
          aValue.u16Val++;
        }
        else
        {
            aValue.u16Val = maxVal.u16Val;
        }
        break;
    case    DT_INT32:
        if(bIncrementBy5 && (aValue.i32Val + 20 < maxVal.i32Val) )
        {
            aValue.i32Val += 20 ;
        }
        else if (aValue.i32Val < maxVal.i32Val)
        {
            aValue.i32Val++;
        }
        else
        {
            aValue.i32Val = maxVal.i32Val;
        }
        break;
    case    DT_UINT32:
        if(bIncrementBy5 && (aValue.u32Val + 20 < maxVal.u32Val) )
        {
            aValue.u32Val += 20 ;
        }
        else if (aValue.u32Val < maxVal.u32Val)
        {
            aValue.u32Val++;
        }
        else
        {
            aValue.u32Val = maxVal.u32Val;
        }
        break;
    case    DT_FLOAT:
        if(bIncrementBy5 && (aValue.fVal + 10 < maxVal.fVal) )
        {
            aValue.fVal = aValue.fVal + 10.0f;
            aValue.fVal = round(aValue.fVal/ this->fValLC)* this->fValLC;
        }
        else if (aValue.fVal < maxVal.fVal)
        {
            aValue.fVal = aValue.fVal + this->fValLC;
            aValue.fVal = round(aValue.fVal/ this->fValLC)* this->fValLC;
            if(aValue.fVal > maxVal.fVal)
            {
                aValue.fVal =  maxVal.fVal;
            }
        }
        else
        {
            aValue.fVal = maxVal.fVal;
        }
        break;
    case DT_DATE:
        if(u8MultiItemEditIndex == 0)
        {
            if( aValue.stDate.u8Date < maxVal.stDate.u8Date)
            {
                aValue.stDate.u8Date++;
            }
            else
            {
                aValue.stDate.u8Date = maxVal.stDate.u8Date;
            }
        }
        else if(u8MultiItemEditIndex == 1)
        {
            if( aValue.stDate.u8Month < maxVal.stDate.u8Month)
            {
                aValue.stDate.u8Month++;
            }
            else
            {
                aValue.stDate.u8Month = maxVal.stDate.u8Month;
            }
        }
        else if(u8MultiItemEditIndex == 2)
        {
          if( aValue.stDate.u16Year < maxVal.stDate.u16Year)
          {
              aValue.stDate.u16Year++;
          }
          else
          {
              aValue.stDate.u16Year = maxVal.stDate.u16Year;
          }
        }
        break;
    case    DT_STRING_FIXED:
        if (aValue.u32IndexIntoFixedOptions < (unsigned)(numOfStringFixedOptions - 1))
        {
            aValue.u32IndexIntoFixedOptions++;
        }
        else
        {
            aValue.u32IndexIntoFixedOptions = numOfStringFixedOptions - 1;
        }
        break;
    case DT_TIME_HRS_MINS:
        u16Mins = aValue.u16Val%100;
        u16Hours = aValue.u16Val/100;
        if(bIncrementBy5 && (u16Mins + 10 < ((maxVal.u16Val)%100)))
        {
            aValue.u16Val += 10 ;
        }
        else if (u16Mins < ((maxVal.u16Val)%100))
        {
          aValue.u16Val++;
        }
        else if(u16Mins == ((maxVal.u16Val)%100) )
        {
            if(u16Hours < ((maxVal.u16Val)/100))
            {
                aValue.u16Val = (uint16_t)((u16Hours+1) * 100U);
            }
            else
            {
                aValue.u16Val = maxVal.u16Val;
            }
        }
        break;
    case DT_PASSWORD:
        if(this->u8PasswordLevel == PIN1_ALLOWED)
        {
            UI::stPassEdit.bPIN1Changed = true;
        }
        if(this->u8PasswordLevel == PIN1_PIN2_ALLOWED)
        {
            UI::stPassEdit.bPIN2Changed = true;
        }

        if(u8MultiItemEditIndex == 0)
        {
            if( aValue.stPassword.u8PassChar1 < maxVal.stPassword.u8PassChar1)
            {
                aValue.stPassword.u8PassChar1++;
            }
            else
            {
                aValue.stPassword.u8PassChar1 = minVal.stPassword.u8PassChar1;
            }
        }
        else if(u8MultiItemEditIndex == 1)
        {
            if( aValue.stPassword.u8PassChar2 < maxVal.stPassword.u8PassChar2)
            {
                aValue.stPassword.u8PassChar2++;
            }
            else
            {
                aValue.stPassword.u8PassChar2 = minVal.stPassword.u8PassChar2;
            }
        }
        else if(u8MultiItemEditIndex== 2)
        {
            if( aValue.stPassword.u8PassChar3 < maxVal.stPassword.u8PassChar3)
            {
                aValue.stPassword.u8PassChar3++;
            }
            else
            {
                aValue.stPassword.u8PassChar3 = minVal.stPassword.u8PassChar3;
            }
        }
        else if(u8MultiItemEditIndex == 3)
        {
            if( aValue.stPassword.u8PassChar4 < maxVal.stPassword.u8PassChar4)
            {
                aValue.stPassword.u8PassChar4++;
            }
            else
            {
                aValue.stPassword.u8PassChar4 = minVal.stPassword.u8PassChar4;
            }
        }
        break;

    case DT_ENG_SR_NO:
        if(u8MultiItemEditIndex < 12)
        {
            if( aValue.stEngSrNo.u8EngSrNo[u8MultiItemEditIndex] < maxVal.stEngSrNo.u8EngSrNo[u8MultiItemEditIndex])
            {
                aValue.stEngSrNo.u8EngSrNo[u8MultiItemEditIndex]++;
            }
            else
            {
                aValue.stEngSrNo.u8EngSrNo[u8MultiItemEditIndex] = minVal.stEngSrNo.u8EngSrNo[u8MultiItemEditIndex];
            }
        }
        break;
    }
    return aValue;
}
void CEditableItem::incrementValue()
{
    value = incrementValue(value, false);
}
void CEditableItem::incrementValue(bool bTemp, bool bIncrementBy5 )
{
    if (bTemp)
    {
        tempValue = incrementValue(tempValue, bIncrementBy5);
    }
    else
    {
        incrementValue();
    }
}
CEditableItem::EditableItemValue_t CEditableItem::decrementValue(
        EditableItemValue_t aValue, bool bdecrementBy5)
{
    uint16_t u16Mins=0;
    uint16_t u16Hours = 0;
    _bValuesChanged = true; // To indicated as value is edited
    // action for down arrow key
    switch (dataType)
    {
    case    DT_CHAR:

        if(bdecrementBy5 && ((aValue.chVal- 5) > minVal.chVal))
        {
            aValue.chVal -= 5;
        }
        else if (aValue.chVal > minVal.chVal)
        {
            aValue.chVal--;
        }
        else
        {
            aValue.chVal = minVal.chVal;
        }
        break;
    case    DT_UINT8:
        if(bdecrementBy5 && ((aValue.u8Val- 5) > minVal.u8Val))
        {
            aValue.u8Val -= 5;
        }
        else if (aValue.u8Val > minVal.u8Val)
        {
            aValue.u8Val--;
        }
        else
        {
            aValue.u8Val = minVal.u8Val;
        }
        break;
    case    DT_INT16:
        if(bdecrementBy5
                && ((aValue.i16Val- 10) > minVal.i16Val))
        {
           aValue.i16Val -= 10;
        }
        else if (aValue.i16Val > minVal.i16Val)
        {
            aValue.i16Val--;
        }
        else
        {
            aValue.i16Val = minVal.i16Val;
        }
        break;
    case    DT_UINT16:
        if((bdecrementBy5
                || (strcmp(this->promptMessage , "DUE AT ENGINE HOURS") == 0))
                && ((aValue.u16Val- 10) >= minVal.u16Val))
        {
           aValue.u16Val -= 10;
        }
        else if (aValue.u16Val > minVal.u16Val)
        {
            aValue.u16Val--;
        }
        else
        {
            aValue.u16Val = minVal.u16Val;
        }
        break;
    case    DT_INT32:
        if(bdecrementBy5 && ((aValue.i32Val- 20) > minVal.i32Val))
        {
           aValue.i32Val -= 20;
        }
        else if (aValue.i32Val > minVal.i32Val)
        {
            aValue.i32Val--;
        }
        else
        {
            aValue.i32Val = minVal.i32Val;
        }
        break;
    case    DT_UINT32:
        if(bdecrementBy5 && ((aValue.u32Val- 20) > minVal.u32Val))
        {
           aValue.u32Val -= 20;
        }
        else if (aValue.u32Val > minVal.u32Val)
        {
            aValue.u32Val--;
        }
        else
        {
            aValue.u32Val = minVal.u32Val;
        }
        break;
    case    DT_FLOAT:
        if(bdecrementBy5 && ((aValue.fVal- 10) > minVal.fVal))
        {
            aValue.fVal = aValue.fVal - 10.0f;
            aValue.fVal = round(aValue.fVal/ this->fValLC)* this->fValLC;
        }
        else if (aValue.fVal > (minVal.fVal))
        {
            aValue.fVal = aValue.fVal - this->fValLC;
            aValue.fVal = round(aValue.fVal/ this->fValLC)* this->fValLC;
            if(aValue.fVal < minVal.fVal)
            {
                aValue.fVal =  minVal.fVal;
            }
        }
        else
        {
            aValue.fVal = minVal.fVal;
        }
        break;
    case DT_DATE:
        if(u8MultiItemEditIndex == 0)
        {
            if( aValue.stDate.u8Date > minVal.stDate.u8Date)
            {
                aValue.stDate.u8Date--;
            }
            else
            {
                aValue.stDate.u8Date = minVal.stDate.u8Date;
            }
        }
        else if(u8MultiItemEditIndex == 1)
        {
            if( aValue.stDate.u8Month > minVal.stDate.u8Month)
            {
                aValue.stDate.u8Month--;
            }
            else
            {
                aValue.stDate.u8Month = minVal.stDate.u8Month;
            }
        }
        else if(u8MultiItemEditIndex == 2)
        {
          if( aValue.stDate.u16Year > minVal.stDate.u16Year)
          {
              aValue.stDate.u16Year--;
          }
          else
          {
              aValue.stDate.u16Year = minVal.stDate.u16Year;
          }
        }
        break;
    case    DT_STRING_FIXED:
        if (aValue.u32IndexIntoFixedOptions > 0)
        {
            aValue.u32IndexIntoFixedOptions--;
        }
        else
        {
            aValue.u32IndexIntoFixedOptions = 0;
        }
        break;
    case DT_PASSWORD:
        if(this->u8PasswordLevel == PIN1_ALLOWED)
        {
            UI::stPassEdit.bPIN1Changed = true;
        }
        if(this->u8PasswordLevel == PIN1_PIN2_ALLOWED)
        {
            UI::stPassEdit.bPIN2Changed = true;
        }
        if(u8MultiItemEditIndex == 0)
        {

            if( aValue.stPassword.u8PassChar1 > minVal.stPassword.u8PassChar1)
            {
                aValue.stPassword.u8PassChar1--;
            }
            else
            {
                aValue.stPassword.u8PassChar1 = maxVal.stPassword.u8PassChar1;
            }
        }
        else if(u8MultiItemEditIndex == 1)
        {
            if( aValue.stPassword.u8PassChar2 > minVal.stPassword.u8PassChar2)
            {
                aValue.stPassword.u8PassChar2--;
            }
            else
            {
                aValue.stPassword.u8PassChar2 = maxVal.stPassword.u8PassChar2;
            }
        }
        else if(u8MultiItemEditIndex == 2)
        {
            if( aValue.stPassword.u8PassChar3 > minVal.stPassword.u8PassChar3)
            {
                aValue.stPassword.u8PassChar3--;
            }
            else
            {
                aValue.stPassword.u8PassChar3 = maxVal.stPassword.u8PassChar3;
            }
        }
        else if(u8MultiItemEditIndex== 3)
        {
            if( aValue.stPassword.u8PassChar4 > minVal.stPassword.u8PassChar4)
            {
                aValue.stPassword.u8PassChar4--;
            }
            else
            {
                aValue.stPassword.u8PassChar4 = maxVal.stPassword.u8PassChar4;
            }
        }
        break;
    case DT_ENG_SR_NO:
        if(u8MultiItemEditIndex < 12)
        {
            if( aValue.stEngSrNo.u8EngSrNo[u8MultiItemEditIndex] > minVal.stEngSrNo.u8EngSrNo[u8MultiItemEditIndex])
            {
                aValue.stEngSrNo.u8EngSrNo[u8MultiItemEditIndex]--;
            }
            else
            {
                aValue.stEngSrNo.u8EngSrNo[u8MultiItemEditIndex] = maxVal.stEngSrNo.u8EngSrNo[u8MultiItemEditIndex];
            }
        }
                break;
    case DT_TIME_HRS_MINS:

        u16Mins = aValue.u16Val%100;
        u16Hours = aValue.u16Val/100;
        if(bdecrementBy5 && (u16Mins - 10 > ((minVal.u16Val)%100)))
        {

            aValue.u16Val -= 10 ;
        }
        else if (u16Mins > ((minVal.u16Val)%100))
        {
          aValue.u16Val--;
        }
        else if(u16Mins == ((minVal.u16Val)%100) )
        {
            if(u16Hours > ((minVal.u16Val)/100))
            {
                aValue.u16Val = (uint16_t)((u16Hours-1)*100U + 59U);
            }
            else if(u16Hours == ((minVal.u16Val)/100))
            {
                aValue.u16Val = minVal.u16Val;
            }
        }
        break;
    }
    return aValue;
}
void CEditableItem::decrementValue()
{
    value = decrementValue(value, false);
}
void CEditableItem::decrementValue(bool bTemp, bool bdecrementBy5)
{
    if (bTemp)
    {
        tempValue = decrementValue(tempValue, bdecrementBy5);
    }
    else
    {
        decrementValue();
    }
}
void CEditableItem::initTempValue()
{
    if(dataType != DT_PASSWORD) // Not password values
    {
        tempValue = value;
    }
    else
    {
        // Password values
        tempValue.u32Val = 0;
    }


}
void CEditableItem::saveTempValue()
{
    if(dataType == DT_ENG_SR_NO)
    {
        value = tempValue;
    }
    else if( strncmp((char *)stringFixedOptions[tempValue.u8Val] , "Reserved", 7) != 0)
    {
        if(dataType != DT_PASSWORD) // Not password values
        {
            value = tempValue;
        }
        else
        {
            if((value.u32Val != tempValue.u32Val)  && _bValuesChanged)
            {
                value = tempValue;
            }
        }
    }
    else
    {
        tempValue = value;
    }
}

void CEditableItem::CopyPrevValue()
{
    if(dataType != DT_PASSWORD) // Not password values
    {
        tempValue = value;
    }
    else
    {
        tempValue.u32Val = 0; //for password only
    }

}
void CEditableItem:: DisplayEngSrChar(uint8_t val)
{
    char arrTemp[32]={0};
    if(val == 47)
    {
        sprintf(arrTemp," %c ",35);
    }
    else if(val > 57)
    {
        sprintf(arrTemp," %c ",val+7);
    }
    else
    {
        sprintf(arrTemp," %c ",val);
    }
    gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
}
void CEditableItem::prvPrint_Password_Edit_Screen(EditableItemValue_t val)
{
    char arrTemp[32]={0};
    if(u8MultiItemEditIndex == 0)
    {
        gpDisplay->gotoxy(GLCD_X(50),GLCD_Y(37));
        sprintf(arrTemp," %u ",val.stPassword.u8PassChar1);
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->drawHorizontalLine(GLCD_X(46), GLCD_Y(47), GLCD_Y(53));
        gpDisplay->drawHorizontalLine(GLCD_X(46), GLCD_Y(48), GLCD_Y(53));
        gpDisplay->gotoxy(GLCD_X(60),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->gotoxy(GLCD_X(70),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->gotoxy(GLCD_X(80),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
    }
    else if(u8MultiItemEditIndex == 1)
    {
        gpDisplay->gotoxy(GLCD_X(50),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->gotoxy(GLCD_X(60),GLCD_Y(37));
        sprintf(arrTemp," %u ",val.stPassword.u8PassChar2);
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->drawHorizontalLine(GLCD_X(56), GLCD_Y(47), GLCD_Y(63));
        gpDisplay->drawHorizontalLine(GLCD_X(56), GLCD_Y(48), GLCD_Y(63));
        gpDisplay->gotoxy(GLCD_X(70),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->gotoxy(GLCD_X(80),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
    }
    else if(u8MultiItemEditIndex== 2)
    {
        gpDisplay->gotoxy(GLCD_X(50),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->gotoxy(GLCD_X(60),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->gotoxy(GLCD_X(70),GLCD_Y(37));
        sprintf(arrTemp," %u ",val.stPassword.u8PassChar3);
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->drawHorizontalLine(GLCD_X(66), GLCD_Y(47), GLCD_Y(73));
        gpDisplay->drawHorizontalLine(GLCD_X(66), GLCD_Y(48), GLCD_Y(73));
        gpDisplay->gotoxy(GLCD_X(80),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);

    }
    else if(u8MultiItemEditIndex== 3)
    {
        gpDisplay->gotoxy(GLCD_X(50),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->gotoxy(GLCD_X(60),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->gotoxy(GLCD_X(70),GLCD_Y(37));
        sprintf(arrTemp,"%s","#");
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->gotoxy(GLCD_X(80),GLCD_Y(37));
        sprintf(arrTemp," %u ",val.stPassword.u8PassChar4);
        gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        gpDisplay->drawHorizontalLine(GLCD_X(76), GLCD_Y(47), GLCD_Y(83));
        gpDisplay->drawHorizontalLine(GLCD_X(76), GLCD_Y(48), GLCD_Y(83));
    }
}
#if (RUNNING_ON == TARGET_DESKTOP)
void CEditableItem::print(EditableItemValue_t val)
{
    printf("-------------------------------------\n");
    printf("Data type = %s\n", dt2str(dataType));
    printf("promptMessage = '%s'\n", promptMessage);
    printf("unitOfMeasurement = '%s'\n", unitOfMeasurement);
    printf("formatString = '%s'\n", formatString);
    printf("Value = {%s}\n", getString(val));
    if (dataType == DT_STRING_FIXED)
    {
        printf("Available options in drop-down list are:\n");
        for (int i = 0; i < numOfStringFixedOptions; i++)
        {
            printf("\t{%s}\n", stringFixedOptions[i]);
        }
    }
}
void CEditableItem::print()
{
    print(value);
}
void CEditableItem::print(bool bTemp)
{
    if (bTemp)
    {
        // i.e. print tempValue rather than value
        print(tempValue);
    }
    else
    {
        print(value);
    }
}
#elif (RUNNING_ON == TARGET_GCU)
void CEditableItem::print(EditableItemValue_t val)
{
    char arrTemp[32]={0};
    static bool bToggle = true, bFirstTimeReset= true;
    static  stTimer _BlinkTimer;
    static uint8_t prvContrastValue = 50;
    if(bFirstTimeReset)
    {
        bFirstTimeReset = false;
        UTILS_ResetTimer(&_BlinkTimer);
    }

    gpDisplay->gotoxy(GLCD_X(64),GLCD_Y(37));

    if(UTILS_GetElapsedTimeInMs(&_BlinkTimer) >= 500)
    {
        if(bToggle==true)
        {
            bToggle =false;
        }
        else
        {
            bToggle = true;
        }
        UTILS_ResetTimer(&_BlinkTimer);
    }

    if(bToggle || (uiState != UI_STATE_SHOWING_SCREEN_ITEM_EDIT))
    {
        if (dataType == DT_STRING_FIXED)
        {
            gpDisplay->printStringCenterAligned((char *)stringFixedOptions[val.u8Val],FONT_VERDANA);
        }
        else if (dataType == DT_FLOAT)
        {
            char Format[20] = {0};
            strcpy(Format,formatString);
            strcat(Format,"%s");
            if(strcmp(formatString,"%f") == 0)
            {
                sprintf(arrTemp,"%0.1f%s",val.fVal, unitOfMeasurement);
            }
            else
            {
                sprintf(arrTemp,Format,val.fVal, unitOfMeasurement);
            }
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        }
//        else if(dataType == DT_STRING )
//        {
//            gpDisplay->printStringCenterAligned((char *)val.strVal,FONT_VERDANA);
//        }
        else if(dataType == DT_UINT8)
        {

            sprintf(arrTemp,"%u %s",val.u8Val, (char*)unitOfMeasurement);
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);

        }
        else if(dataType == DT_UINT16)
        {
            sprintf(arrTemp,"%u %s",val.u16Val, (char*)unitOfMeasurement);
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        }
        else if(dataType == DT_INT16)
       {
           sprintf(arrTemp,"%d %s",val.i16Val, (char*)unitOfMeasurement);
           gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
       }
        else if(dataType == DT_UINT32)
        {
            sprintf(arrTemp,"%lu %s",val.u32Val, (char*)unitOfMeasurement);
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        }
        else if(dataType == DT_INT32)
        {
            sprintf(arrTemp,"%ld %s",val.i32Val, (char*)unitOfMeasurement);
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        }
        else if(dataType == DT_CHAR )
        {
            gpDisplay->printChar(val.chVal, FONT_VERDANA);
        }
        else if(dataType == DT_DATE)
        {
            gpDisplay->gotoxy(GLCD_X(48),GLCD_Y(37));
            sprintf(arrTemp,"%u",val.stDate.u8Date);
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
            gpDisplay->gotoxy(GLCD_X(68),GLCD_Y(37));
            sprintf(arrTemp,"%s",arrMonth[val.stDate.u8Month -1]);
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
            gpDisplay->gotoxy(GLCD_X(94),GLCD_Y(37));
            sprintf(arrTemp,"%04d",val.stDate.u16Year);
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);

        }
        else if(dataType == DT_TIME_HRS_MINS)
        {
            sprintf(arrTemp,"%02d %s %02d %s",(val.u16Val/100), (char*)"Hrs",(val.u16Val%100),(char*)"Mins");
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
        }
        else if(dataType == DT_PASSWORD)
        {
            if((uiState != UI_STATE_SHOWING_SCREEN_ITEM_EDIT))
            {
                gpDisplay->gotoxy(GLCD_X(50),GLCD_Y(37));
                sprintf(arrTemp,"%s","#");
                gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
                gpDisplay->gotoxy(GLCD_X(60),GLCD_Y(37));
                sprintf(arrTemp,"%s","#");
                gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
                gpDisplay->gotoxy(GLCD_X(70),GLCD_Y(37));
                sprintf(arrTemp,"%s","#");
                gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
                gpDisplay->gotoxy(GLCD_X(80),GLCD_Y(37));
                sprintf(arrTemp,"%s","#");
                gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);

               switch(u8MultiItemEditIndex)
               {
                   case 0:
                       gpDisplay->drawHorizontalLine(GLCD_X(46), GLCD_Y(47), GLCD_Y(53));
                       gpDisplay->drawHorizontalLine(GLCD_X(46), GLCD_Y(48), GLCD_Y(53));
                       break;
                   case 1:
                       gpDisplay->drawHorizontalLine(GLCD_X(56), GLCD_Y(47), GLCD_Y(63));
                       gpDisplay->drawHorizontalLine(GLCD_X(56), GLCD_Y(48), GLCD_Y(63));
                       break;
                   case 2:
                       gpDisplay->drawHorizontalLine(GLCD_X(66), GLCD_Y(47), GLCD_Y(73));
                       gpDisplay->drawHorizontalLine(GLCD_X(66), GLCD_Y(48), GLCD_Y(73));
                       break;
                   case 3:
                       gpDisplay->drawHorizontalLine(GLCD_X(76), GLCD_Y(47), GLCD_Y(83));
                       gpDisplay->drawHorizontalLine(GLCD_X(76), GLCD_Y(48), GLCD_Y(83));
                       break;
                  default:
                     break;

               }
            }
            else
            {
                prvPrint_Password_Edit_Screen(val);
            }
        }
        else if(dataType == DT_ENG_SR_NO)
        {
            for(int i=0;i<12;i++)
            {
                gpDisplay->gotoxy(GLCD_X(10+10*i),GLCD_Y(37));
                DisplayEngSrChar(val.stEngSrNo.u8EngSrNo[i]);
            }

            gpDisplay->drawHorizontalLine(GLCD_X(6+u8MultiItemEditIndex*10), GLCD_Y(47), GLCD_Y(13+u8MultiItemEditIndex*10));
            gpDisplay->drawHorizontalLine(GLCD_X(6+u8MultiItemEditIndex*10), GLCD_Y(48), GLCD_Y(13+u8MultiItemEditIndex*10));
        }

    }
    else
    {
        if(dataType == DT_DATE)
        {
            gpDisplay->gotoxy(GLCD_X(48),GLCD_Y(37));
            sprintf(arrTemp,"%d",val.stDate.u8Date);
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
            gpDisplay->gotoxy(GLCD_X(68),GLCD_Y(37));
            sprintf(arrTemp,"%s",arrMonth[val.stDate.u8Month - 1]);
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
            gpDisplay->gotoxy(GLCD_X(94),GLCD_Y(37));
            sprintf(arrTemp,"%04d",val.stDate.u16Year);
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);

            if(u8MultiItemEditIndex == 0)
            {
                gpDisplay->gotoxy(GLCD_X(48),GLCD_Y(37));
                sprintf(arrTemp,"%s","    ");
                //sprintf(arrTemp,"%s -%02d -%04d ","  ",val.stDate.u8Month, val.stDate.u16Year);
                gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
            }
            else if(u8MultiItemEditIndex == 1)
            {
               gpDisplay->gotoxy(GLCD_X(68),GLCD_Y(37));
               sprintf(arrTemp,"%s","      ");
               gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
            }
            else if(u8MultiItemEditIndex== 2)
            {
               gpDisplay->gotoxy(GLCD_X(94),GLCD_Y(37));
               sprintf(arrTemp,"%s","        ");
               //sprintf(arrTemp,"%d -%02d -%s ",val.stDate.u8Date,val.stDate.u8Month,"      ");
               gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);
            }
        }
        else if(dataType == DT_PASSWORD)
        {
            prvPrint_Password_Edit_Screen(val);
        }
        else if(dataType == DT_ENG_SR_NO)
        {
            for(int i=0;i<12;i++)
            {
                gpDisplay->gotoxy(GLCD_X(10+10*i),GLCD_Y(37));
                DisplayEngSrChar(val.stEngSrNo.u8EngSrNo[i]);
            }

            gpDisplay->gotoxy(GLCD_X(10+u8MultiItemEditIndex*10), GLCD_Y(37));
            sprintf(arrTemp,"  %c",32);
            gpDisplay->printStringCenterAligned((char *)arrTemp,FONT_VERDANA);

        }
        else
        {
            gpDisplay->printStringCenterAligned((char *)"                                          ",FONT_VERDANA);
        }
    }


     if(strcmp(promptMessage,"CONTRAST") == 0)
     {
         if(tempValue.u8Val != prvContrastValue )
         {
             prvContrastValue = tempValue.u8Val;
             configGlcd->AdjustContrast(tempValue.u8Val);
         }
     }

}

void CEditableItem::print()
{
    print(value);
}
void CEditableItem::print(bool bTemp)
{
   if (bTemp)
   {
       // i.e. print tempValue rather than value
       print(tempValue);
   }
   else
   {
       print(value);
   }
}
#else
#error No valid Target defined!
#endif // (RUNNING_ON == TARGET_DESKTOP)


bool CEditableItem::IsAnyConfigValueEdited()
{
   return  _bValuesChanged;
}

void CEditableItem::ClearConfigEditStatus()
{
    _bValuesChanged = false;
}
