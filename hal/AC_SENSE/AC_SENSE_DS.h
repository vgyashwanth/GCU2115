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

#endif
