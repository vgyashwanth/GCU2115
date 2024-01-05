/**
 * @file        [AC_SENSE_DS]
 * @brief       This header houses the definitions of data structures used by
 *              sub-modules of AC_SENSE
 * @version     [Version number]
 * @author      Naveen Ramaswamy, Sudeep Chandrasekaran
 * @date        31st May 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef _AC_SENSE_DS_H_
#define _AC_SENSE_DS_H_

/*Position of CT's in the system*/
// typedef enum {
//     CT_ON_ALT_SIDE,
//     CT_ON_LOAD_SIDE
// } I_CFG_t;

#if (SUPPORT_CALIBRATION == YES)

/** Macros containing information related to calibration parameters and their storage in the EEPROM. */
#define TOTAL_NO_OF_CALIB_PARAM                                                           (12)
#define TOLARANCE_LIMIT_OF_CF_IN_PERCENT                                                   (5)
#define DEFAULT_VALUE_OF_CF_VOLT                                       ((4000.0f+9.09f)/9.09f)
#define DEFAULT_VALUE_OF_ADC_OS_VOLT                                                       (0)
#define LOWER_VALUE_OF_CF_VOLT                                (DEFAULT_VALUE_OF_CF_VOLT*0.95f)
#define LOWER_VALUE_OF_ADC_OS_VOLT                                                         (0)
#define HIGHER_VALUE_OF_CF_VOLT                               (DEFAULT_VALUE_OF_CF_VOLT*1.05f)
#define HIGHER_VALUE_OF_ADC_OS_VOLT                                                        (0)
#define DEFAULT_VALUE_OF_CF_CURR                                                       (10.0f)
#define DEFAULT_VALUE_OF_ADC_OS_CURR                                                       (0)
#define LOWER_VALUE_OF_CF_CURR                                (DEFAULT_VALUE_OF_CF_CURR*0.95f)
#define LOWER_VALUE_OF_ADC_OS_CURR                                                         (0)
#define HIGHER_VALUE_OF_CF_CURR                               (DEFAULT_VALUE_OF_CF_CURR*1.05f)
#define HIGHER_VALUE_OF_ADC_OS_CURR                                                        (0)
#define EEPROM_CALIB_RESULT_ADDR                                                      (0x0114)
#define EEPROM_CALIB_RESULT_LENGTH                                                         (1)
#define EEPROM_CF_START_ADDR             (EEPROM_CALIB_RESULT_ADDR+EEPROM_CALIB_RESULT_LENGTH)
#define EEPROM_CF_LENGTH                                                              (2*4*12)

#endif /* SUPPORT_CALIBRATION */

typedef enum {
    GENSET_CURRENT_MEASUREMENT,
    MAINS_CURRENT_MEASUREMENT,
    NO_CURRENT_MEASUREMENT
} I_CFG_t;

/*Phase for which the parameter is to be calculated*/
typedef enum {
    R_PHASE,
    Y_PHASE,
    B_PHASE,
    PHASE_END
} PHASE_t;


#if (SUPPORT_CALIBRATION == YES)
/** Enum to store the order of parameters whose calibration values are stored in NV. */
typedef enum{
    PARAM_CT1 =0,
    PARAM_CT2,
    PARAM_CT3,
    PARAM_CT4,
    PARAM_MAINS_R_PHASE,
    PARAM_MAINS_Y_PHASE,
    PARAM_MAINS_B_PHASE,
    PARAM_MAINS_N_PHASE,
    PARAM_GEN_R_PHASE,
    PARAM_GEN_Y_PHASE,
    PARAM_GEN_B_PHASE,
    PARAM_GEN_N_PHASE,
    PARAM_LAST
}PARAM_t;
/** Structure to store the values and metadata of the calibration parameters. */
typedef struct
{
    float Multiplier[PARAM_LAST];
    float Offset[PARAM_LAST];
    uint32_t u32CRC;
} CALIBRATED_DATA_t;
#endif /* SUPPORT_CALIBRATION */


#endif
