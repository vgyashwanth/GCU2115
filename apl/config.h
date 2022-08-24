/*
 * config.h
 *
 *  Created on: 14-Jun-2021
 *      Author: sudeep.c
 */

#ifndef APL_CONFIG_H_
#define APL_CONFIG_H_

/***************************DFLASH Partitions**********************************/
#define DFLASH_BLOCK_SIZE                       (64U)
#define DFLASH_EHW_TEST_RESULT_START_ADDRESS    (0x40100000)
#define DFLASH_EHW_TEST_RESULT_LENGTH           (128U)

#define DFLASH_FIRMWARE_INFO_START_ADDRESS      (DFLASH_EHW_TEST_RESULT_START_ADDRESS \
													+ DFLASH_EHW_TEST_RESULT_LENGTH)
#define DFLASH_FIRMWARE_INFO_LENGTH             (64U)

#define DFLASH_PASSWORD_START_ADDRESS           (DFLASH_FIRMWARE_INFO_START_ADDRESS \
                                                    + DFLASH_FIRMWARE_INFO_LENGTH)
#define DFLASH_PASSWORD_LENGTH                  (64U)

#define DFLASH_CFGC_START_ADDRESS               (DFLASH_PASSWORD_START_ADDRESS \
													+ DFLASH_PASSWORD_LENGTH)
#define DFLASH_CFGC_LENGTH                      (128U)


#define DFLASH_ACTIVE_PROFILE_START_ADDRESS     (DFLASH_CFGC_START_ADDRESS \
													+ DFLASH_CFGC_LENGTH)
#define DFLASH_ACTIVE_PROFILE_LENGTH            (2048U)

/******************************************************************************/

/**********************************EEPROM Address**********************************/
#define  EXT_EEPROM_FIRMWARE_INFO_ADDRESS           0
#define  EXT_EEPROM_FIRMWARE_INFO_LENGTH            0x100

#define  EXT_EEPROM_EHW_TEST_RESULT_START_ADDRESS  (EXT_EEPROM_FIRMWARE_INFO_ADDRESS+\
                                                    EXT_EEPROM_FIRMWARE_INFO_LENGTH)

#define  EXT_EEPROM_EHW_TEST_RESULT_START_LENGTH    0x100

#define  EXT_EEPROM_PASWORD_START_ADDRESS           (EXT_EEPROM_EHW_TEST_RESULT_START_ADDRESS \
                                                     + EXT_EEPROM_EHW_TEST_RESULT_START_LENGTH)
#define  EXT_EEPROM_PASWORD_START_LENGTH            0x200

#define  EXT_EEPROM_ACTIVE_PROFILE_START_ADDRESS    (EXT_EEPROM_PASWORD_START_ADDRESS + \
                                                    EXT_EEPROM_PASWORD_START_LENGTH  )
#define  EXT_EEPROM_ACTIVE_PROFILE_LENGTH           0XC00

#define  EXT_EEPROM_CURRENT_EVENT_NO_ADDRESS       (EXT_EEPROM_ACTIVE_PROFILE_START_ADDRESS +\
                                                    EXT_EEPROM_ACTIVE_PROFILE_LENGTH)
#define  EXT_EEPROM_CURRENT_EVENT_NO_LENGTH        (4)

#define  EXT_EEPROM_ROLLED_OVER_ADDRESS            (EXT_EEPROM_CURRENT_EVENT_NO_ADDRESS  +\
                                                   EXT_EEPROM_CURRENT_EVENT_NO_LENGTH)

#define  EXT_EEPROM_ROLLED_OVER_LENGTH             252

#define  EXT_EEPROM_EVENT_LOG_START_ADDRESS       (EXT_EEPROM_ROLLED_OVER_ADDRESS +\
                                                   EXT_EEPROM_ROLLED_OVER_LENGTH)

#define  MAX_EVENT_LOGS                           100

#define EXT_EEPROM_CUMMULATIVE_START_ADD_SECT0     0x5000
#define EXT_EEPROM_CUMMULATIVE_LENGTH              512
#define EXT_EEPROM_CUMMULATIVE_START_ADD_SECT1    (EXT_EEPROM_CUMMULATIVE_START_ADD_SECT0 +\
                                                   EXT_EEPROM_CUMMULATIVE_LENGTH)

/**********************************************************/

 /* Token numbers, this can be changed as per product
     * requirement.
     */
    typedef enum
    {
        VEEPROM_GEN_NO_OF_STARTS = 0,
        VEEPROM_GEN_NO_OF_TRIPS,
        VEEPROM_GEN_RUN_MIN,
        VEEPROM_GEN_ENERGY,
        VEEPROM_HISTOGRAM,
        VEEPROM_EXT_OVERLOAD,
        VEEPROM_FIRMWARE_INFO,
    }TOKEN_t;

/**********************************************************/

/**********************************PFlash***********************************/
#define FACTORY_CFGZ_ADDRESS                      0x78000


#define MAX_NUMBER_OF_PROFILE   10
#define MAX_DISP_CONST   100

#endif /* APL_CONFIG_H_ */
