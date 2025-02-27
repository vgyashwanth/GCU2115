/*
 * UI_DS.h
 *
 *  Created on: 15-Aug-2021
 *      Author: madhuri.abhang
 */

#ifndef APL_HMI_UI_DS_H_
#define APL_HMI_UI_DS_H_

#include "Keypad.h"
typedef enum
{
    DISP_MON_MODE,
    DISP_ALARM_MODE,
    DISP_PASSWORD_EDIT_MODE,
    DISP_CONFIG_MODE,
    DISP_EVENT_LOG_MODE
}MODE_TYPE_t;

typedef enum
{
    PASSWORD1,
    PASSWORD2,
    MISC_16_LAST
}MISC_PARAM_16_t;

typedef enum
{
    PROFILE_NO,
    RESET_COUNTER,
    MISC_8_LAST
}MISC_PARAM_8_t;

#define MISC_PARAM_DUMMY_BYTES        (0U)

typedef struct
{
    uint16_t u16MiscParam[MISC_16_LAST];
    uint8_t u8MiscParam[MISC_8_LAST];
#if(MISC_PARAM_DUMMY_BYTES > 0)    
    uint8_t u8Dummy[MISC_PARAM_DUMMY_BYTES];
#endif
    uint16_t u16CRC;
 }MISC_PARAM_t;

#define SR_NOS_MAX_SIZE       (20)
#define GEN_SRNO_LEN (20)
#define ENG_SRNO_LEN (20)
#define ALT_SRNO_LEN (20)
#define MAIN_CONT_SRNO_LEN (20)
#define ENG_CONT_SRNO_LEN (20)
#define SITE_ID_LEN (10)
#define SR_NO_DATA_SIGNATURE  (0x72344268)
#define SR_NOS_DUMMY_BYTES    ((4 - (((GEN_SRNO_LEN) + (ENG_SRNO_LEN) + (ALT_SRNO_LEN) + (MAIN_CONT_SRNO_LEN) +\
                                      (ENG_CONT_SRNO_LEN) + (SITE_ID_LEN)) %4) ) %4)
#define SR_NOS_LATEST_VER     (0)
typedef struct
{
    uint32_t u32Signature;
    uint16_t u16ProdSrNoVer;
    uint16_t u16Crc; 
    uint8_t u8GenSrNo[GEN_SRNO_LEN]; 
    uint8_t u8EngSrNo[ENG_SRNO_LEN];
    uint8_t u8AltSrNo[ALT_SRNO_LEN];
    uint8_t u8MainContSrNo[MAIN_CONT_SRNO_LEN];
    uint8_t u8EngContSrNo[ENG_CONT_SRNO_LEN];
    uint8_t u8SiteId[SITE_ID_LEN];
#if(SR_NOS_DUMMY_BYTES > 0)    
    uint8_t u8Dummy[SR_NOS_DUMMY_BYTES];
#endif
}PRODUCT_SR_NOS_t;

#define SR_NOS_HEADER_SIZE    (SERIAL_NOS_SIGNATURE_LEN + SERIAL_NOS_VER_LEN + 2)


#define STOP_KEY_LONG_PRESS      KEYPAD::BSP_KEY_4_LONG_PRESS
#define STOP_KEY_SHORT_PRESS     KEYPAD::BSP_KEY_4_SHORT_PRESS
#define DN_STOP_KEY_LONG_PRESS   KEYPAD::BSP_KEY_4_7_LONG_PRESS
#define UP_DN_KEY_LONG_PRESS     KEYPAD::BSP_KEY_7_8_LONG_PRESS
#define UP_DN_KEY_SHORT_PRESS    KEYPAD::BSP_KEY_7_8_SHORT_PRESS
#define UP_SHORT_PRESS           KEYPAD::BSP_KEY_8_SHORT_PRESS
#define DN_SHORT_PRESS           KEYPAD::BSP_KEY_7_SHORT_PRESS
#define UP_LONG_PRESS            KEYPAD::BSP_KEY_8_LONG_PRESS
#define DN_LONG_PRESS            KEYPAD::BSP_KEY_7_LONG_PRESS
#define AUTO_KEY_LONG_PRESS      KEYPAD::BSP_KEY_2_LONG_PRESS
#define AUTO_KEY_SHORT_PRESS     KEYPAD::BSP_KEY_2_SHORT_PRESS

#define START_KEY_SHORT_PRESS    KEYPAD::BSP_KEY_3_SHORT_PRESS
#define ACK_KEY_PRESS            KEYPAD::BSP_KEY_1_SHORT_PRESS

#define GEN_CONT_KEY_PRESS       KEYPAD::BSP_KEY_5_SHORT_PRESS
#define MAINS_CONT_KEY_PRESS     KEYPAD::BSP_KEY_6_SHORT_PRESS

#endif /* APL_HMI_UI_DS_H_ */
