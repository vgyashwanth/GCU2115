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
#include "MODBUS.h"
#include "../HMI/MAIN_UI/MAIN_UI.h"
#include "bytes2arr.h"
/* Used to invert the byte order of a U16 number. This comes handy for conversion
   between endainess.
 */
#define UTILS_SWAP_UINT16(x) (uint16_t)(((x) >> 8) | ((x) << 8))

#define BITS_PER_BYTE              (8U)
/*Number of bits involved in transmitting a UART byte(i.e including overhead)*/
#define BITS_PER_UART_BYTE         (10)
/*Number of bytes of silence to observe. This should be ideally be 3.5 but since we
  can only handle whole number of characters, we are using 4
 */
#define BYTES_OF_SILENCE           (4)

#define MAX_ERROR_RESPONSE_SIZE    (10)

static MODBUS *pMb;

static void RS485Cb(uint8_t *pu8Data, uint16_t u16Len);

uint64_t MODBUS::MB_Valid_Count = 0;

const uint8_t SilenceMsArray[]=
{
 30,
 15,
 8,
 4,
 4,
 2,
 2,
 2
};

MODBUS::MODBUS(RS485 &rs485, ADDRESS_GRP_LST_t &addressGrp, INPUTS_STATUS_GRP_LST_t &InputCoilGrp):
_rs485(rs485),
_u8SlaveID(0x00),
_AddressGrp(addressGrp),
_InputStatusGroup(InputCoilGrp),
_CurrentAddressGroup(0),
_CurrentCoilByteGroup(0),
_eParserState(MB_WAIT_FOR_DATA),
_ParseComplete(false),
_ParseStatus(MB_VALID_PACKET),
_u8SilenceInMS(2),
_isModbusEnabled(false)
{
    rs485.RegisterReceiveCB(RS485Cb);
    UTILS_ResetTimer(&_tmr);
    pMb = this;
}

void MODBUS::Update()
{
    if(_ParseComplete && (UTILS_GetElapsedTimeInMs (&_waitToRespond) > 3 ))
    {
        if(_isModbusEnabled)
        {
            if(_ParseStatus == MB_VALID_PACKET)
            {
                prvProcessRequest();
                MB_Valid_Count++;
            }
            else
            {
                /*Send error response*/
                prvSendErrorResponse();
            }
        }
        _ParseComplete = false;
    }
}

void MODBUS::Configure(uint8_t u8BaudOption, uint8_t u8StationAddress, bool bEnable)
{
    _u8SlaveID = u8StationAddress;
    /*Calculate silence in milliseconds corresponding to BYTES_OF_SILENCE characters.
     */
  //  _u8SilenceInMS = (uint8_t)((1000/(u32Baud/BITS_PER_UART_BYTE))*BYTES_OF_SILENCE) + 1;
    _u8SilenceInMS = SilenceMsArray[u8BaudOption];
    _isModbusEnabled = bEnable;
}

void MODBUS::HandleIncomingData(uint8_t u8Byte)
{
    /*Reset the state machine in case we hadn't received data for _u8SilenceInMS*/
    if(UTILS_GetElapsedTimeInMs(&_tmr) > _u8SilenceInMS)
    {
        _eParserState = MB_WAIT_FOR_DATA;
    }

    /* Used by states to keep track of count by states that process more than
       one byte of data
     */
    volatile static uint8_t  su8Cnt;
    static uint16_t su16CalculateCRC;
    static uint16_t su16ReceivedCRC;

    /*Keep computing CRC for states except for states MB_WAIT_FOR_DATA & MB_VALIDATE_CRC*/
    if( (_eParserState != MB_VALIDATE_CRC) && (_eParserState != MB_WAIT_FOR_DATA) )
    {
        /*Wait for the silence time to pick the next frame*/
        su16CalculateCRC = CRC16::ComputeCRCGeneric(&u8Byte, sizeof(uint8_t), su16CalculateCRC);
    }

    switch(_eParserState)
    {
        case MB_WAIT_FOR_DATA:
        {
            /*Wait for the silence time to pick the next frame*/
            if (UTILS_GetElapsedTimeInMs (&_tmr) > _u8SilenceInMS)
            {
                /*Start using this packet only if it contains the station address
                  of this server*/
                if(u8Byte == _u8SlaveID)
                {
                    _eParserState = MB_FX_CODE_WAIT;
                    _pkt.u16StartAddress = 0;
                    /*Clear the register buffer*/
                    memset(_pkt.au16Registers, 0x00, sizeof(_pkt.au16Registers));
                    /*Clear byte count to be used by other states*/
                    su8Cnt = 0;
                    /*Initialize CRC computation by initializing the seed*/
                    su16CalculateCRC = CRC_MODBUS_SEED;
                    su16CalculateCRC = CRC16::ComputeCRCGeneric(&u8Byte, 
                                                sizeof(uint8_t), su16CalculateCRC);
                    su16ReceivedCRC  = 0;
                    /*Clear packet*/
                    _pkt.u16NoOfRegistersOrInputs = 0;
                    _pkt.u16StartAddress  = 0;
                }
            }
            break;
        }
        case MB_FX_CODE_WAIT:
        {
            _pkt.u8FunctionCode = u8Byte;
            /*Check weather the function code is supported*/
            if( (_pkt.u8FunctionCode == MB_READ_COILS) ||
                (_pkt.u8FunctionCode == MB_READ_DISCRETE_INPUTS) ||
                (_pkt.u8FunctionCode == MB_READ_INPUT_REGISTERS) ||
                (_pkt.u8FunctionCode == MB_READ_HOLDING_REGISTERS) ||
                (_pkt.u8FunctionCode == MB_WRITE_HOLDING_REGISTERS)||
                (_pkt.u8FunctionCode == MB_WRITE_HOLDING_SINGLE_REG))
            {
                _eParserState = MB_REG_ADDRESS_WAIT;
            }
            else
            {
                _ParseComplete = true;
                _ParseStatus   = MB_UNSUPPORTED_FUNCTION_CODE;
                _eParserState  = MB_WAIT_FOR_DATA;
            }
            break;
        }
        case MB_REG_ADDRESS_WAIT:
        {
            /*The address is 2 byte, hence this state will be run 2 times for a
              packet. Shift the first received byte and then receive the second byte.
             */
            _pkt.u16StartAddress = (uint16_t)(_pkt.u16StartAddress<<(BITS_PER_BYTE*su8Cnt));
            _pkt.u16StartAddress = (uint16_t)(_pkt.u16StartAddress| (uint16_t)u8Byte);


            su8Cnt++;
            if(su8Cnt >= sizeof(uint16_t))
            {
                su8Cnt = 0;
                if(_pkt.u8FunctionCode == MB_WRITE_HOLDING_SINGLE_REG )
                {
                    _eParserState = MB_DATA_ACCUMULATE;
                }
                else
                {
                    _eParserState = MB_REG_CNT_WAIT;
                }
            }
            break;
        }
        case MB_REG_CNT_WAIT:
        {
            /*The register count is 2 byte, hence this state will be run 2 times for a
              packet. Shift the first received byte and then receive the second byte.
             */
            _pkt.u16NoOfRegistersOrInputs = (uint16_t)(_pkt.u16NoOfRegistersOrInputs << (uint8_t)(BITS_PER_BYTE*su8Cnt));
            _pkt.u16NoOfRegistersOrInputs = (uint16_t)(_pkt.u16NoOfRegistersOrInputs| (uint16_t)u8Byte);
            su8Cnt++;
            if(su8Cnt >= sizeof(uint16_t))
            {
                su8Cnt=0;
                if(prvValidatePacket())
                {
                    _eParserState = (_pkt.u8FunctionCode==MB_WRITE_HOLDING_REGISTERS)?
                                                           MB_DATA_LEN_WAIT:MB_VALIDATE_CRC;
                }
                else
                {
                    _ParseComplete = true;
                    _ParseStatus   = MB_ILLEGAL_ADDRESS;
                    _eParserState  = MB_WAIT_FOR_DATA;
                }
            }
            break;
        }
        case MB_DATA_LEN_WAIT:
        {
            _eParserState = MB_DATA_ACCUMULATE;
            break;
        }
        case MB_DATA_ACCUMULATE:
        {

            uint8_t u8Idx = (uint8_t)(su8Cnt/sizeof(uint16_t));

            /*Group of every two byte is used to for a register value*/
            _pkt.au16Registers[u8Idx] = (uint16_t)(_pkt.au16Registers[u8Idx]<<(BITS_PER_BYTE*(su8Cnt%2)));
            _pkt.au16Registers[u8Idx] = (uint16_t)(_pkt.au16Registers[u8Idx]| (uint16_t)u8Byte);
            su8Cnt++;

            /*Check whether we have received all registers*/
            if( (((su8Cnt/sizeof(uint16_t)) >= _pkt.u16NoOfRegistersOrInputs)
                    &&(_pkt.u8FunctionCode == MB_WRITE_HOLDING_REGISTERS))

                    ||((_pkt.u8FunctionCode == MB_WRITE_HOLDING_SINGLE_REG) && (su8Cnt==2)))
            {
                su8Cnt = 0;
                 _eParserState = MB_VALIDATE_CRC;
            }
            break;
        }
        case MB_VALIDATE_CRC:
        {
            su16ReceivedCRC = (uint16_t)(su16ReceivedCRC<<(BITS_PER_BYTE*su8Cnt));
            su16ReceivedCRC = (uint16_t)(su16ReceivedCRC| (uint16_t)u8Byte);
            su8Cnt++;
            if(su8Cnt >= sizeof(uint16_t))
            {
                /*Change endianess of the received CRC*/
                su16ReceivedCRC = UTILS_SWAP_UINT16(su16ReceivedCRC);
                if(su16ReceivedCRC == su16CalculateCRC)
                {
                    _ParseComplete = true;
                    _ParseStatus = MB_VALID_PACKET;
                }
                _eParserState = MB_WAIT_FOR_DATA;
            }
            break;
        }
        default:
        {
            _eParserState = MB_WAIT_FOR_DATA;
            break;
        }
    }

    if(_ParseComplete == true)
    {
        UTILS_ResetTimer(&_waitToRespond);
    }
    UTILS_ResetTimer(&_tmr);
}

bool MODBUS::prvValidatePacket()
{
    bool bValidPacket = false;
    switch(_pkt.u8FunctionCode)
    {
        case MB_READ_INPUT_REGISTERS:
        case MB_READ_HOLDING_REGISTERS:
        case MB_WRITE_HOLDING_REGISTERS:
        case MB_WRITE_HOLDING_SINGLE_REG:
            /* Iterate over all address groups to find a address and length match corresponding
               to the current modbus request.
             */
            for(uint8_t i=0; i<_AddressGrp.u8NoOfRegisterGroups; i++)
            {
                ADDRESS_GROUP_t &_addrGrp = _AddressGrp.pau8Registers[i];
                
                bool bModbusTypeAny = (!isModbusConfigRegSpecific()) && (_addrGrp.eRegType == MODBUS_REG_ANY)
                /*Check the validity of function code for this address group, and verify if the address group is of correct register type*/
                if( (_addrGrp.isReadSupported  && (_pkt.u8FunctionCode==MB_READ_INPUT_REGISTERS) && (((_addrGrp.eRegType == MODBUS_REG_INPUT) && isModbusConfigRegSpecific()) ||  ) ||
                    (_addrGrp.isReadSupported  && (_pkt.u8FunctionCode==MB_READ_HOLDING_REGISTERS) && (_addrGrp.eRegType == MODBUS_REG_HOLDING)) ||
                    (_addrGrp.isWriteSupported && (_pkt.u8FunctionCode==MB_WRITE_HOLDING_REGISTERS) && (_addrGrp.eRegType == MODBUS_REG_HOLDING)) ||
                    (_addrGrp.isWriteSupported && (_pkt.u8FunctionCode==MB_WRITE_HOLDING_SINGLE_REG) && (_addrGrp.eRegType == MODBUS_REG_HOLDING)) )
                {
                    /*Check weather the group matches*/
                    if( (_pkt.u16StartAddress >= _addrGrp.u16StartAddress) &&
                            ( (_pkt.u16StartAddress+_pkt.u16NoOfRegistersOrInputs) <=
                                    (_addrGrp.u16StartAddress+_addrGrp.u16length) ) )
                    {
                        /*Record the index of the address group to be used by further operations
                          involved in processing the modbus packet
                         */
                        _CurrentAddressGroup = i;
                        bValidPacket =  true;
                    }
                }
            }
            break;
        case MB_READ_COILS:
        case MB_READ_DISCRETE_INPUTS:
            for(uint8_t i=0; i<_InputStatusGroup.u8NoOfStatusByteGroups; i++)
            {
                INPUTS_STATUS_GROUP_t &_InputStatusGrp = _InputStatusGroup.pau8Registers[i];
                
                /*Check the validity of function code for this group, and verify if the address group is of correct register type*/
                if((_InputStatusGrp.isReadSupported  && (_pkt.u8FunctionCode == (MB_READ_DISCRETE_INPUTS)) && (_InputStatusGrp.eRegType == MODBUS_REG_DISCRETE_INPUT))||
                   (_InputStatusGrp.isReadSupported  && (_pkt.u8FunctionCode == (MB_READ_COILS)) && (_InputStatusGrp.eRegType == MODBUS_REG_COIL)) )
                {
                    /*Check whether the group matches*/
                    if( (_pkt.u16StartAddress >= _InputStatusGrp.u16StartAddress) &&
                            ( (_pkt.u16StartAddress+_pkt.u16NoOfRegistersOrInputs - 1) <=
                                    (_InputStatusGrp.u16EndAddress)) )
                    {
                        /*Record the index of the address group to be used by further operations
                                      involved in processing the modbus packet
                         */
                        _CurrentCoilByteGroup = i;
                        bValidPacket =  true;
                    }
                }
            }
            break;

        default:
            /* Do Nothing */
            break;
    }
    return bValidPacket;

}
void MODBUS::prvProcessRequest()
{
    /*Array used to frame the response*/
    uint8_t au8TempBuffer[MB_MAX_BUFFER_LENGTH];
    uint8_t u8BuffLen = 0;
    /*Append station address*/
    au8TempBuffer[u8BuffLen]=_u8SlaveID;
    u8BuffLen++;
    /*Append function code*/
    au8TempBuffer[u8BuffLen]=_pkt.u8FunctionCode;
    u8BuffLen++;

    switch(_pkt.u8FunctionCode)
    {
        case MB_READ_COILS:
        case MB_READ_DISCRETE_INPUTS:
        {
            uint8_t u8ByteCount = 0;
            /*For function code 02(read discrete inputs we fill the byte count in the response packet.
             *  The Byte Count field specifies the quantity of complete bytes of data.
             * Byte count = Quantity of Inputs / 8 if the remainder is different of 0 ⇒ N = N+1
             * where N is the Byte Count
             * */
            if((_pkt.u16NoOfRegistersOrInputs%8)==0)
            {
                u8ByteCount =  (uint8_t)((_pkt.u16NoOfRegistersOrInputs)/8);
            }
            else
            {
                u8ByteCount =  (uint8_t)((_pkt.u16NoOfRegistersOrInputs/8) + 1);
            }
            au8TempBuffer[u8BuffLen] = u8ByteCount;
            u8BuffLen++;
            u8BuffLen = (uint8_t)(u8BuffLen+prvProcessReadReqForDiscreteInput((uint8_t*)(au8TempBuffer+u8BuffLen),
                                           (uint8_t)(MB_MAX_BUFFER_LENGTH-u8BuffLen) , u8ByteCount) );
            break;
        }
        case MB_READ_INPUT_REGISTERS:
        case MB_READ_HOLDING_REGISTERS:
        {
            au8TempBuffer[u8BuffLen] = (uint8_t)(_pkt.u16NoOfRegistersOrInputs*sizeof(uint16_t));
            u8BuffLen++;
            u8BuffLen = (uint8_t)(u8BuffLen+prvProcessReadReq((uint8_t*)(au8TempBuffer+u8BuffLen),
                                           (uint8_t)(MB_MAX_BUFFER_LENGTH-u8BuffLen) ));
            break;
        }
        case MB_WRITE_HOLDING_REGISTERS:
        {
            prvProcessWriteReq();
//            ADDRESS_GROUP_t &_addrGrp = _AddressGrp.pau8Registers[_CurrentAddressGroup];

             if(_ParseStatus != MB_VALID_PACKET)
            {
                break;
            }
            /*Append start address*/
            PUT_U16_IN_U8ARR_BE(au8TempBuffer,u8BuffLen, _pkt.u16StartAddress);
            u8BuffLen = u8BuffLen + 2;
            /*Append number of written  registers*/
            PUT_U16_IN_U8ARR_BE(au8TempBuffer,u8BuffLen, _pkt.u16NoOfRegistersOrInputs);
            u8BuffLen = u8BuffLen + 2;
            break;
        }
        case MB_WRITE_HOLDING_SINGLE_REG:
        {
            ADDRESS_GROUP_t &_addrGrp = _AddressGrp.pau8Registers[1];
            uint16_t u16RegisterIdx = _pkt.u16StartAddress-_addrGrp.u16StartAddress;
            _addrGrp.pu16Registers[u16RegisterIdx] = _pkt.au16Registers[0];
           if(_ParseStatus != MB_VALID_PACKET)
           {
                break;
           }
           PUT_U16_IN_U8ARR_BE(au8TempBuffer, u8BuffLen, _pkt.u16StartAddress);
           u8BuffLen = u8BuffLen + 2;

           PUT_U16_IN_U8ARR_BE(au8TempBuffer,u8BuffLen, _pkt.au16Registers[0]);
           u8BuffLen = u8BuffLen + 2;
        }
        break;

        default:
        {
            break;
        }
    }

    if(_ParseStatus != MB_VALID_PACKET)
    {
      prvSendErrorResponse();
    }
    else
    {
        /*Append CRC*/
        uint16_t u16CRC = CRC16::ComputeModbusCRC(au8TempBuffer, u8BuffLen);
        /*Change endianess*/
        u16CRC = UTILS_SWAP_UINT16(u16CRC);
        PUT_U16_IN_U8ARR_BE(au8TempBuffer,u8BuffLen, u16CRC);
        u8BuffLen = u8BuffLen + 2;
        _rs485.SendData(au8TempBuffer, u8BuffLen);
    }
}

void MODBUS::prvProcessWriteReq()
{
    ADDRESS_GROUP_t &_addrGrp = _AddressGrp.pau8Registers[_CurrentAddressGroup];
    for(uint8_t j=0;j<_pkt.u16NoOfRegistersOrInputs;j++)
    {
        /*Determine the register index within this group*/
        uint16_t u16RegisterIdx = _pkt.u16StartAddress-_addrGrp.u16StartAddress;
        _addrGrp.pu16Registers[u16RegisterIdx+j] = _pkt.au16Registers[j];
    }
}

uint8_t MODBUS::prvProcessReadReqForDiscreteInput(uint8_t *pu8Resp, uint8_t u8BuffLen , uint8_t u8ByteCount)
{
    uint8_t *puTmpBuffPointer = pu8Resp;

    INPUTS_STATUS_GROUP_t &_InputStatusGrp = _InputStatusGroup.pau8Registers[_CurrentCoilByteGroup];

    /*Check whether buffer length is enough*/
    if(u8BuffLen < u8ByteCount)
    {
        return 0;
    }

    uint16_t u16DiscreteInputAddress = _pkt.u16StartAddress;

    /*
     * The response to function code 2 consists of function code ,
     * byte count , followed by the data in bytes.
     * which indicate the status of discrete inputs.
     * If GCU has a map starting from 22500 and ending at 22510,
     * and suppose the Master requests for 2 discrete inputs from
     * 22500 address. Then response format will have 1 data byte
     * in which the first 2 bits will indicate the status of the
     * requested discrete inputs .Please note that here the remaining part
     * of data byte will not represent the status of 22502 and onward inputs.
     * the remaining part of data byte is not considered by the Master(confirmed in MODBUS poll).
     * If the master would have requested for 9 inputs then
     * GCU will send 2 bytes. The following logic copies the state of
     * inputs from the StatusByte array and fills each bit of the response
     * data byte accordingly.
     * */
    for(uint8_t i = 0; i< u8ByteCount; i++)
    {
        uint8_t u8ResponseByteValue = 0;
        uint8_t u8BitPosition = 0;

        for(uint8_t j=0; j<8; j++)
        {
            //Iterate over all the bits of the
            uint16_t u16ByteIndex = (uint16_t)((u16DiscreteInputAddress-_InputStatusGrp.u16StartAddress)/8);
            u8BitPosition = (uint8_t)(u16DiscreteInputAddress-_InputStatusGrp.u16StartAddress)%8;

            /*Set the values for One status byte. Each status byte has status of 8 Discrete Inputs*/
            if(_InputStatusGrp.pu8InputsStatusByte[u16ByteIndex] & (1<<u8BitPosition))
            {
                u8ResponseByteValue|= (uint8_t)(1<<j);
            }

            u16DiscreteInputAddress++;
            if((u16DiscreteInputAddress >= (_pkt.u16StartAddress +_pkt.u16NoOfRegistersOrInputs)))
            {
                /*Suppose the number of inputs required is < 8 and we should skip iterating
                 * through all 8 bits of the data as the remaining bits should be padded with 0*/

                break;
            }
        }
        //One byte of response updated
        (*puTmpBuffPointer) = (uint8_t)(u8ResponseByteValue);
        puTmpBuffPointer++;
    }

    return (uint8_t)(puTmpBuffPointer-pu8Resp);
}

uint8_t MODBUS::prvProcessReadReq(uint8_t *pu8Resp, uint8_t u8BuffLen)
{
    uint8_t *puTmpBuffPointer = pu8Resp;
    /*Check weather buffer length is enough*/
    if(u8BuffLen < (_pkt.u16NoOfRegistersOrInputs*sizeof(uint16_t)) )
    {
        return 0;
    }
    ADDRESS_GROUP_t &_addrGrp = _AddressGrp.pau8Registers[_CurrentAddressGroup];
    for(uint8_t j=0; j<_pkt.u16NoOfRegistersOrInputs; j++)
    {
        /*Determine the register index within this group*/
        uint16_t u16RegisterIdx = _pkt.u16StartAddress-_addrGrp.u16StartAddress;
        /*MSB*/
        (*puTmpBuffPointer) = GET_HIGHER_BYTE_FROM_U16_LE(_addrGrp.pu16Registers[u16RegisterIdx+j]);
        puTmpBuffPointer++;
        /*LSB*/
        (*puTmpBuffPointer) = (uint8_t)_addrGrp.pu16Registers[u16RegisterIdx+j] & 0xFFU;
        puTmpBuffPointer++;
    }
    return (uint8_t)(puTmpBuffPointer-pu8Resp);
}

void MODBUS::prvSendErrorResponse()
{
    uint8_t au8TempBuffer[MAX_ERROR_RESPONSE_SIZE];
    uint8_t u8BuffLen = 0;
    /*Append station address*/
    au8TempBuffer[u8BuffLen]=_u8SlaveID;
    u8BuffLen++;
    /*Append function code + error flag*/
    au8TempBuffer[u8BuffLen]=_pkt.u8FunctionCode|0x80U;
    u8BuffLen++;
    au8TempBuffer[u8BuffLen]=_ParseStatus;
    u8BuffLen++;
    /*Append CRC*/
    uint16_t u16CRC = CRC16::ComputeModbusCRC(au8TempBuffer, u8BuffLen);
    /*Change endianess*/
    u16CRC = UTILS_SWAP_UINT16(u16CRC);
    PUT_U16_IN_U8ARR_BE(au8TempBuffer,u8BuffLen, u16CRC);
    u8BuffLen = u8BuffLen + 2;
    /*Send response*/
    _rs485.SendData(au8TempBuffer, u8BuffLen);
}

static void RS485Cb(uint8_t *pu8Data, uint16_t u16Len)
{
    if(pMb != NULL)
    {
        for(uint8_t i=0;i<u16Len; i++)
        {
            pMb->HandleIncomingData(pu8Data[i]);
        }
    }
}
