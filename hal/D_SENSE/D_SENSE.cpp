/**
 * @file        [D_SENSE]
 * @brief       D_SENSE
 *              This module provides a mechanism to map a set of digital inputs 
 *              to various digital sensors. The digital sensors are represented 
 *              using DigitalSensor class. D_SENSE class has a "has a" relationship
 *              with DigitalSensor objects.
 *              
 * @version     [Version number]
 * @author      Sudeep Chandrasekaran
 * @date        9th April 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#include "D_SENSE.h"

D_SENSE::D_SENSE(DIG_IP &digIp):
_digIp(digIp)
{

}

void D_SENSE::ConfigureSensor(CFG_t &cfg)
{
    /*Create an array of debouncers, The order of debouncers shall be same as
    defined in INPUTS_t*/
    Debouncer *db_arr[] = {&_digIp.InputA, &_digIp.InputB, &_digIp.InputC,
                           &_digIp.InputD, &_digIp.InputE, &_digIp.InputF,
                           &_digIp.InputG, &_digIp.InputH, &_digIp.InputI,
                           &_digIp.InputJ, &_digIp.InputK, &_digIp.InputL,
                           &_digIp.InputM, &_digIp.InputN, &_digIp.InputO,
                           &_digIp.InputP
                           };

    for(int i=0;i<DI_END;i++)
    {
        _aDigitalSensors[i].CfgDigitalSensor(cfg.stDIConfig[i].eActivationPolarity,
                        cfg.stDIConfig[i].u16ActivationDelaySeconds,
                        *db_arr[i], cfg.stDIConfig[i].eType);
    }
}

void D_SENSE::Update()
{
    for(int i=0;i<DI_END;i++)
    {
        _aDigitalSensors[i].Update();
    }
}

DigitalSensor::DigitalSensor():
_pDb(NULL),
_eActivationPolarity(CLOSE_TO_ACTIVATE),
_u16ActivDelaySeconds(0),
_ActTimer{0, false},
_DeactTimer{0, false},
_state(STATE_NOT_ACTIVATED),
_SensorId(DI_NOT_CONFIGURED)
{

}

void DigitalSensor::CfgDigitalSensor(ACTIV_POLARITY_t eActivationPolarity,
                                        uint16_t u16ActivationDelaySeconds,
                                            Debouncer &db, D_SENSOR_TYPS_t typ)
{
    _eActivationPolarity = eActivationPolarity;
    _u16ActivDelaySeconds = u16ActivationDelaySeconds;
    _pDb = &db;
    _SensorId = typ;
}

DigitalSensor::STATUS_t DigitalSensor::GetState()
{
    /*If the debouncer is NULL, it means that the sensor is not configured*/
    if(_pDb == NULL)
    {
        return SENSOR_NOT_CONFIGRUED;
    }
    if(_state == STATE_ACTIVATED)
    {
        return SENSOR_LATCHED;
    }
    else
    {
        return SENSOR_UNLATCHED;
    }
}

DigitalSensor::D_SENSOR_TYPS_t DigitalSensor::GetSensorId()
{
    return _SensorId;
}

DigitalSensor::STATUS_t D_SENSE::GetDigInputState(int InputPin)
{
    return _aDigitalSensors[InputPin].GetState();
}

DigitalSensor::STATUS_t D_SENSE::GetDigitalSensorState(DigitalSensor::D_SENSOR_TYPS_t typ)
{
    for(int i=0;i<DI_END;i++)
    {
        if(_aDigitalSensors[i].GetSensorId() == typ)
        {
            return _aDigitalSensors[i].GetState();
        }
    }
    return DigitalSensor::SENSOR_NOT_CONFIGRUED;
}


void DigitalSensor::Update()
{
    if(_pDb==NULL)
    {
        return;
    }
    switch(_state)
    {
        case STATE_NOT_ACTIVATED:
        {
            /*If the activation polarity is CLOSE_TO_ACTIVATE and the sensor
            pin is low, the sensor is about to get activated. or
            If the activation polarity is OPEN_TO_ACTIVATE and the sensor pin
            is high, the sensor is about to get activated*/
            if( ((_eActivationPolarity==CLOSE_TO_ACTIVATE) && 
                                            (_pDb->GetPinState()==Debouncer::PIN_LOW)) ||
                ((_eActivationPolarity==OPEN_TO_ACTIVATE) &&
                                            (_pDb->GetPinState()==Debouncer::PIN_HIGH)) )
            {
                _state = STATE_ACTIVATION_IN_PROGRESS;
            }
            break;
        }
        case STATE_ACTIVATION_IN_PROGRESS:
        {
            _state = STATE_ACTIVATED;
            /*Move to state STATE_NOT_ACTIVATED if the sensor is inactive.
              Note that activation delay is applicable only for activation
              of the sensor. We keep checking weather the sensor is active
              for the activation time before deeming the sensor to be active.
              However, if the sensor is inactive even momentarily, the sensor
              is deemed to inactive*/
            if( ((_eActivationPolarity == OPEN_TO_ACTIVATE) &&
                                            (_pDb->GetPinState()==Debouncer::PIN_LOW)) ||
                    ((_eActivationPolarity == CLOSE_TO_ACTIVATE) &&
                                            (_pDb->GetPinState()==Debouncer::PIN_HIGH)) )
            {
                _state = STATE_NOT_ACTIVATED;
            }
            break;
        }
        case STATE_ACTIVATED:
        {
            if( ((_eActivationPolarity == OPEN_TO_ACTIVATE) && 
                                            (_pDb->GetPinState()==Debouncer::PIN_LOW)) ||
                    ((_eActivationPolarity == CLOSE_TO_ACTIVATE) &&
                                            (_pDb->GetPinState()==Debouncer::PIN_HIGH)) )
            {
                _state = STATE_NOT_ACTIVATED;
            }
            break;
        }
        default:
        {
            /*No reason for the control to come here, but in case it comes here
            move state to STATE_NOT_ACTIVATED*/
            _state = STATE_NOT_ACTIVATED;
            break;
        }
    }
}



