/**
 * @file        [UTILS_Crc]
 * @brief       The module provides easy API's to calculate different types
 *              of CRC's
 * @version     [Version number]
 * @module      To LT1000
 * @author      Sudeep Chandrasekaran
 * @date        19th September 2018
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef UTILS_CRC_H_
#define UTILS_CRC_H_

//-------------------------------------------------------------------------------------------------/
//*                                HEADER FILE INCLUSION                                          */
//-------------------------------------------------------------------------------------------------/
#include "stdint.h"
#include "string.h"


//-------------------------------------------------------------------------------------------------/
//*                                    COMMON DATASTRUCTURES                                      */
//-------------------------------------------------------------------------------------------------/

//-------------------------------------------------------------------------------------------------/
//*                             PUBLIC FUNCTION DECLARATION                                       */
//-------------------------------------------------------------------------------------------------/

/**
 * @Name   Calculate_CRC16CCITXMODEM
 * @brief
 *  Calculates the CRC16CCITXMODEM
 * @param  pu8Data      - Pointer to the data.
 * @param  u16Len       - Lenth of data.(max len value can be 5000)
 * @return [UTILS_BUFF_FULL]   - No space in the buffer.
 *         [UTILS_INVALID_PTR] - Invalid pointer passed
 *         [UTILS_SUCCESS]     - Successfully stored data to the buffer.
 */
uint16_t Calculate_CRC16CCITXMODEM(const uint8_t *pu8Data, uint16_t u16Len);

/**
 * @Name   Calculate_CRCGeneric
 * @brief
 * Calculates the CRC16CCITXMODEM. This API has a option to change the seed
 * polynomial. This means that this API can be used to calculate the CRC for
 * long data streams by calculating the CRC in steps and passsing output of
 * each step as the seed to the next step.
 * @param  pu8Data      - Pointer to the data.
 * @param  u16Len       - Lenth of data.(max len value can be 5000)
 * @return [UTILS_BUFF_FULL]   - No space in the buffer.
 *         [UTILS_INVALID_PTR] - Invalid pointer passed
 *         [UTILS_SUCCESS]     - Successfully stored data to the buffer.
 */
uint16_t Calculate_CRCGeneric(const uint8_t *pu8Data, uint16_t u16Len, uint16_t u16CrcSeed);

/**
 * @Name   Calculate_CRCGeneric
 * @brief
 * Calculates the CRC16CCITXMODEM. This API is a wrapper to find CRC of data
 * with length more than 5000. Internally calls Calculate_CRCGeneric.
 * @param  pu8Data      - Pointer to the data.
 * @param  u16Len       - Lenth of data.(max len value can be 5000)
 * @return [UTILS_BUFF_FULL]   - No space in the buffer.
 *         [UTILS_INVALID_PTR] - Invalid pointer passed
 *         [UTILS_SUCCESS]     - Successfully stored data to the buffer.
 */
uint16_t Calculate_CRC(const uint8_t *pu8Data, uint32_t u32Len);

#endif /* UTILS_CRC_H_ */

