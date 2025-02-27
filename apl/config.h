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
#define EXT_EEPROM_CUMMULATIVE_LENGTH              256
#define EXT_EEPROM_CUMMULATIVE_START_ADD_SECT1    (EXT_EEPROM_CUMMULATIVE_START_ADD_SECT0 +\
                                                   (EXT_EEPROM_CUMMULATIVE_LENGTH))

#define PREV_FW_META_DATA_CRC_ADDRESS             (0x5230)
#define PREV_FACTORY_PROFILES_CRC_ADDRESS         (0x5234) /*Address to store previously stored active profile CRC to log active profile flashing*/
#define PREV_ACTIVE_CRC_ADDRESS                   (0x5238) /*Address to store previously stored factory profile CRC to log factory profile flashing*/
/*
 * SuryaPranayTeja.BVV 09-11-2022
 * The below address locations of EEPROM is used for the miscellaneous parameters
 * which need to be saved.
 * In GC2111 , One such parameter is Modbus Connection Count which is shown in the Event Log Screen.
 */
#define PRODUCT_SPECIFIC_AREA_START_ADDR          (0x5400)
#define PRODUCT_SPECIFIC_AREA_MAX_LENGTH          (0x318)//(0x3E0)
#define PRODUCT_SPECIFIC_DATA_VERSION_ADDR        (PRODUCT_SPECIFIC_AREA_START_ADDR + PRODUCT_SPECIFIC_AREA_MAX_LENGTH - 1 )

#define SERIAL_NOS_AREA_LENGTH                    (200)
#define SERIAL_NOS_AREA_START_ADDRESS             (0x5718)
#define SERIAL_NOS_SIGNATURE_LEN                  (4)
#define SERIAL_NOS_VER_LEN                        (2)
#define SERIAL_NOS_DATA_START_ADDRESS             (SERIAL_NOS_AREA_START_ADDRESS + SERIAL_NOS_SIGNATURE_LEN + SERIAL_NOS_VER_LEN)

/**********************************PFlash***********************************/
#define FACTORY_CFGZ_ADDRESS_1_MB_MCU               0xF8000
#define FACTORY_CFGZ_ADDRESS_512_KB_MCU             0x78000

#define MAX_NUMBER_OF_PROFILE   10
#define MAX_DISP_CONST   100

#define FW_AREA_END_ADDRESS                      (0xF7FFF)
#define FACTORY_PROFILES_END_ADDRESS             (0xFFFFF)

#define SIZE_OF_META_DATA_IN_BYTES               (16U)

#define FIRMWARE_META_DATA_ADDRESS              (FW_AREA_END_ADDRESS + 1 - SIZE_OF_META_DATA_IN_BYTES)
#define FACTORY_PROFILES_META_DATA_ADDRESS      (FACTORY_PROFILES_END_ADDRESS + 1 - SIZE_OF_META_DATA_IN_BYTES)

#endif /* APL_CONFIG_H_ */
