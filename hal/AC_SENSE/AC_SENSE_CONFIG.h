/**
 * @file        [AC_SENSE_CONFIG]
 * @brief       This header houses the configuration parameters for the AC_SENSE
 *              module.
 * @version     [Version number]
 * @author      Naveen Ramaswamy, Sudeep Chandrasekaran
 * @date        31st May 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#ifndef _AC_SENSE_CONFIG_H_
#define _AC_SENSE_CONFIG_H_

#include "bsp.h"

#define R1_K_OHM                           (4000.0F)
#define R2_K_OHM                           (9.09F)
/*The H/W design is such that the ADC values can be never be above this*/
#define MAX_SAMPLE_VALUE                   (3600)
/*The H/W design is such that the ADC values can be never be below this*/
#define MIN_SAMPLE_VALUE                   (496)
#define FREQ_FILTER_MULTIPLIER             (0.05F)
#define SAMPLES_PER_ENTRY_W_BUFFER         (100)
/*Valid power is expected to be present only above this value*/
#define POWER_COMPUTATION_THRESHOLD_MW     (0.05F)
#define CURRENT_COMPUTATION_THRESHOLD_MW   (0.05F)
/*Frequency will be set to 0 if voltage is below this threshold*/
#define AC_SENSE_FREQ_CALC_VOLTS           (30)

#ifndef BSP_ADC_QUANTIZATION_LEVELS
    #error "Unable to find definition of BSP_ADC_QUANTIZATION_LEVELS in bsp.h"
#endif

#ifndef BSP_ADC_REF_VTG
    #error "Unable to find definition of BSP_ADC_REF_VTG in bsp.h"
#endif

#endif
