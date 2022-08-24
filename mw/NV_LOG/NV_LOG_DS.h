/*
* @file        [NV_OG_DS]
* @brief       This file contains the data structures that are used within NV_LOG.
*              First is the enum that contains the possible NV_LOG statuses and the
*              other is a function pointer used to call the callback function
*              provided by the user.
* @version     [Version number]
* @author      Naveen Ramaswamy, Sudeep Chandrasekaran
* @date        24th June 2021
* @author      None
* @date        None
* @copyright   SEDEMAC Mechatronics Pvt Ltd
*/


#ifndef _NV_LOG_DS_H_
#define _NV_LOG_DS_H_

typedef enum {
    NV_LOG_SUCCESS,
    NV_LOG_FLASH_ERR,
    NV_LOG_IN_PROGRESS,
    NV_LOG_BRN_UNAVAILABLE,
    NV_LOG_BRN_CORRUPTED,
    NV_LOG_LOCATION_NOT_BLANK,
    NV_LOG_LOCATION_BLANK,
    NV_LOG_LOG_NOT_AVAILABLE,
    NV_LOG_SIZE_MISMATCH,
    NV_LOG_CORRUPT,
    NV_LOG_TRANSACTION_PENDING
} NV_LOG_STATUS_t;

/* Function Pointer for callback */
typedef void  (*NV_LOG_EVT_CB_t)(NV_LOG_STATUS_t evt);

#endif
