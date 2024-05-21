/**
 * @file        [MODBUS]
 * @brief       This module implements the modbus RTU slave protocol. The following
 *              function codes are supported:
 *              4 - Read input registers
 *              3 - Read holding registers
 *              16- Write multiple holding registers
 *              Supports registers with non-continuous addresses. The module
 *              does not implement an RS485 drive and expects one to be available.
 *              Important Data structures:
 *              1. ADDRESS_GROUP_t - This is used to record information of modbus 
 *              registers whose addresses are continuous. A group of registers with
 *              continuous addresses is called an address group. ex: A group of 10
 *              registers in address range [0-9] can be represented using one
 *              instance of this data structure.
 *              2. ADDRESS_GRP_LST_t - This data structure is a collection of 
 *              ADDRESS_GROUP_t. This is used to represent a collection of
 *              registers with discontinuous address. A user of this module
 *              has to define this data structure based on the modbus address
 *              map of the product an pass it in as an input(via constructor).
 * @version     [Version number]
 * @author      Sudeep Chandrasekaran
 * @date        13th June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#ifndef _MODBUS_H_
#define _MODBUS_H_

#include "stdint.h"
#include "RS485.h"
#include "UTILS_Timer.h"
#include "CRC16.h"



class MODBUS {

public:
    /*Types of modbus registers*/
    typedef enum{
        MODBUS_REG_COIL = 0,
        MODBUS_REG_DISCRETE_INPUT,
        MODBUS_REG_HOLDING,
        MODBUS_REG_INPUT,
        MODBUS_REG_ANY,
        MODBUS_REG_TYP_LAST
    } MODBUS_REG_TYPES;

    /*Represents information of a consecutive group of MODBUS registers*/
    typedef struct ADDRESS_GROUP_t {
        /*Modbus start address of this address group*/
        uint16_t u16StartAddress;
        /*Number of registers in this address group*/
        uint16_t u16length; 
        /*Pointer to registers*/
        uint16_t *pu16Registers;
        /*true if read is supported in this address group*/
        bool     isReadSupported;
        /*true if write is supported in this address group*/
        bool     isWriteSupported;
        /*Value indicates type of register supported. Refer to MODBUS_REG_TYPES*/
        MODBUS_REG_TYPES  eRegType;
    } ADDRESS_GROUP_t;

    /*Represents information of a consecutive group of MODBUS inputs status bytes*/
    typedef struct INPUTS_STATUS_GROUP_t {
        /*Modbus start address of this address group*/
        uint16_t u16StartAddress;
        /*Modbus end address of this address group*/
        uint16_t u16EndAddress;
        /*Pointer to Inputs Status byte.
         * The discrete inputs in the message are packed
         * as one input per bit of the Status byte*/
        uint8_t *pu8InputsStatusByte;
        /*true if read is supported in this address group*/
        bool     isReadSupported;
        /*true if write is supported in this address group*/
        bool     isWriteSupported;
        /*Value indicates type of register supported. Refer to MODBUS_REG_TYPES*/
        MODBUS_REG_TYPES  eRegType;
    } INPUTS_STATUS_GROUP_t;

    /*Represents list of ADDRESS_GROUP_t*/
    typedef struct ADDRESS_GRP_LST_t {
        ADDRESS_GROUP_t *pau8Registers;
        uint8_t         u8NoOfRegisterGroups;
    } ADDRESS_GRP_LST_t;

    /*Represents list of INPUTS_STATUS_GRP_LST_t*/
    typedef struct INPUTS_STATUS_GRP_LST_t {
        INPUTS_STATUS_GROUP_t *pau8Registers;
        uint8_t         u8NoOfStatusByteGroups;
    } INPUTS_STATUS_GRP_LST_t;

    /**
     * Constructs the modbus class.
     * @param rs485       - Reference to the RS485 driver
     * @param addressGrp  - The list of modbus address groups.
     * @return
     * None
     */
    MODBUS(RS485 &rs485, ADDRESS_GRP_LST_t &addressGrp, INPUTS_STATUS_GRP_LST_t &InputCoilGrp);

    /**
     * Configures the station address and silence period. Silence is determined
     * in this API using the baud rate.
     * @param u32Baud - Baud rate at which RS485 is working. Note that this does
     *                  not set the baud rate of RS485 and is used only to determine
     *                  the silence period.
     * @param u8StationAddress - Modbus station address
     * @param bEnable - Determine weather to enable/disable module.
     * @return
     * None
     */
    void Configure(uint8_t u8BaudOption, uint8_t u8StationAddress, bool bEnable);

    /**
     * Runs the modbus slave procedures. The function is expected to be called in
     * the super-while loop.
     * @param - None
     * @return
     * None
     */
    void Update();

    /**
     * API used to pass incoming bytes from RS485 to this module.
     * @param u8Byte - Incoming data byte
     * @return
     * None
     */
    void HandleIncomingData(uint8_t u8Byte);

    static void SetModbusConfigRegSpecific(bool isRegSpecific);

    static uint64_t MB_Valid_Count;

private:

    /*MODBUS function codes*/
    #define MB_READ_COILS                (1U)
    #define MB_READ_DISCRETE_INPUTS      (2U)

    
    #define MB_READ_INPUT_REGISTERS      (4U)
    #define MB_READ_HOLDING_REGISTERS    (3U)
    #define MB_WRITE_HOLDING_REGISTERS   (16U)
    #define MB_WRITE_HOLDING_SINGLE_REG  (6U)

    #define MB_MAX_BUFFER_LENGTH        (256U)

    #define GET_HIGHER_BYTE_FROM_U16_LE(val)   (uint8_t)(val >> BITS_PER_BYTE)

    typedef enum {
        MB_WAIT_FOR_DATA,
        MB_FX_CODE_WAIT,
        MB_REG_ADDRESS_WAIT,
        MB_REG_CNT_WAIT,
        MB_DATA_LEN_WAIT,
        MB_DATA_ACCUMULATE,
        MB_VALIDATE_CRC
    } MB_PARSER_STATES_t;

    /*MODBUS error codes*/
    typedef enum {
        MB_UNSUPPORTED_FUNCTION_CODE = 0x01,
        MB_ILLEGAL_ADDRESS           = 0x02,
        MB_ILLEGAL_DATA_VALUE        = 0x03,
        MB_SLAVE_DEV_BUSY            = 0x06,
        MB_VALID_PACKET
    } MB_PARSE_STATUS_t;

    typedef struct {
        uint8_t  u8FunctionCode;
        uint16_t u16StartAddress;
        uint16_t au16Registers[MB_MAX_BUFFER_LENGTH];
        uint16_t u16NoOfRegistersOrInputs;
    } MODBUS_PKT_t;

    /*Reference to the RS485 driver*/
    RS485              &_rs485;
    /*Stores the modbus slave address/ID*/
    uint8_t            _u8SlaveID;
    /*Stores the information of the registers.*/
    ADDRESS_GRP_LST_t  &_AddressGrp; 

    INPUTS_STATUS_GRP_LST_t &_InputStatusGroup;
    /*Index of the register address group for which a command has been received*/
    uint8_t            _CurrentAddressGroup;
    uint8_t            _CurrentCoilByteGroup;
    /*State of the incoming data parser state machine*/
    MB_PARSER_STATES_t _eParserState;
    /*Status of parsing*/
    bool               _ParseComplete;
    /*Result of parsing*/
    MB_PARSE_STATUS_t  _ParseStatus;
    uint8_t            _u8SilenceInMS;
    /*Timer used to handle silence during parsing*/
    stTimer            _tmr;

    stTimer             _waitToRespond;
    MODBUS_PKT_t       _pkt;
    /*Determines weather modbus is enabled/disabled*/
    bool               _isModbusEnabled;

    /*Flag to indicate if the modbus config will store different address
     groups for different register types. Set by function called in MB_APP*/
    static bool _isModbusConfigRegSpecific;
    /**
     * A helper function to validate the incoming packet
     * @param - None
     * @return
     * true if the packet is valid
     */
    bool    prvValidatePacket();

    /**
     * Function processes an incoming read read request(i.e function code 0x03 or
     * 0x04). It also populates the read registers into pu8Resp as defined by 
     * modbus RTU.
     * @param pu8Resp   - The buffer onto which the read data needs to be populated.
     * @param u8BuffLen - Buffer size.
     * @return
     * Number of bytes of data populated into the buffer.
     */
    uint8_t prvProcessReadReq(uint8_t *pu8Resp, uint8_t u8BuffLen);

    /**
     * Function processes an incoming read request(i.e function code 0x02)
     *  It also populates the read registers into pu8Resp as defined by
     * modbus RTU.
     * @param pu8Resp   - The buffer onto which the read data needs to be populated.
     * @param u8BuffLen - Buffer size.
     * @param u8ByteCount - The Byte Count field specifies the quantity of complete bytes of data..
     * @return
     * Number of bytes of data populated into the buffer.
     */
    uint8_t prvProcessReadReqForDiscreteInput(uint8_t *pu8Resp, uint8_t u8BuffLen , uint8_t u8ByteCount);

    /**
     * Function processes a write request(i.e function code 16). The data in the
     * request is written into appropriate registers
     * @param - None
     * @return
     * None
     */
    void    prvProcessWriteReq();

    /**
     * Function processes an incoming modbus request.
     * @param - None
     * @return
     * None
     */
    void    prvProcessRequest();

    /**
     * Sends an error response.
     * @param - None
     * @return
     * None
     */
    void    prvSendErrorResponse();


};

#endif
