/**
 * @file        [ACT]
 * @brief       Actuators
 *              Provides a mechanism to actuate the actuators. The ACTUATOR
 *              class represents an actuator. The ACT_Manager class contains an 
 *              array of actuators, each element representing an actuator 
 *              connected to the HSD. These actuators are configured to a specific
 *              actuator type by calling the Configure function.
 * @version     [Version number]
 * @author      Sudeep Chandrasekaran
 * @date        8th July 2021
 * @author      
 * @date        
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#include "ACT.h"
#include "BSP_Manager.h"

ACTUATOR::ACTUATOR(HSD &hsd):
_actuatorName(ACT_NOT_CONFIGURED),
_eActivationState(ENERGIZE_TO_ACTIVATE),
_ActuatorStatus(false),
_hsd(hsd)
{

}

void ACTUATOR::Configure(ACTUATOR_TYPS_t eType, ACTIVATION_CFG_t eActivation)
{
    _actuatorName = eType;
    _eActivationState = eActivation;

    if(eType == ACT_NOT_CONFIGURED)
    {
        _hsd.TurnOff();
        _ActuatorStatus = false;
    }
}

ACTUATOR::ACTUATOR_TYPS_t ACTUATOR::GetActuatorName()
{
    return _actuatorName;
}

void ACTUATOR::Activate()
{
    if(_eActivationState == ENERGIZE_TO_ACTIVATE)
    {
        _hsd.TurnOn();
        _ActuatorStatus = true;
    }
    else
    {
        _hsd.TurnOff();   
        _ActuatorStatus = false;
    }
}

void ACTUATOR::Deactivate()
{
    if((_eActivationState == ENERGIZE_TO_ACTIVATE) ||
            (_actuatorName == ACT_NOT_CONFIGURED))
    {
        _hsd.TurnOff();
        _ActuatorStatus = false;
    }
    else
    {
        _hsd.TurnOn();   
        _ActuatorStatus = true;
    }
}

bool ACTUATOR::GetActuatorStatus()
{
    return _ActuatorStatus;
}

ACT_Manager::ACT_Manager(HSD_Manager &HSDManager):
_HSDManager(HSDManager),
_actuators{{HSDManager.HSD_OP_A}, {HSDManager.HSD_OP_B},
           {HSDManager.HSD_OP_C}, {HSDManager.HSD_OP_D},
           {HSDManager.HSD_OP_E}, {HSDManager.HSD_OP_F},
           {HSDManager.HSD_OP_G}, {HSDManager.HSD_OP_H},
           {HSDManager.HSD_OP_I}}
{
    
}

void ACT_Manager::Update()
{
    
}

void ACT_Manager::Configure(CFG_t &cfg)
{
    for(int i=0;i<OP_END;i++)
    {
        _actuators[i].Configure(cfg.properties[i].eType,
                                                 cfg.properties[i].eActivation);
    }
}

ACT_Manager::ACT_STATUS_t ACT_Manager::Activate(ACTUATOR::ACTUATOR_TYPS_t eType)
{
    /*Search for the sensor*/
    for(int i=0;i<OP_END;i++)
    {
        if(_actuators[i].GetActuatorName() == eType)
        {
            _actuators[i].Activate();
            return ACT_SUCCESS;
        }
    }
    return ACT_NOT_CONFIGURED;
}

ACT_Manager::ACT_STATUS_t ACT_Manager::Deactivate(ACTUATOR::ACTUATOR_TYPS_t eType)
{
    /*Search for the sensor*/
    for(int i=0;i<OP_END;i++)
    {
        if(_actuators[i].GetActuatorName() == eType)
        {
            _actuators[i].Deactivate();
            return ACT_SUCCESS;
        }
    }
    return ACT_NOT_CONFIGURED;
}

ACT_Manager::ACT_STATUS_t ACT_Manager::GetActStatus(ACTUATOR::ACTUATOR_TYPS_t eType)
{
    for(int i=0;i<OP_END;i++)
    {
        if(_actuators[i].GetActuatorName() == eType)
        {
            if(_actuators[i].GetActuatorStatus() == true )
            {
                return ACT_LATCHED;
            }
            else
            {
                return ACT_NOT_LATCHED;
            }
        }
    }
    return ACT_NOT_CONFIGURED;
}

void ACT_Manager::DeactivateAll()
{
    for(int i=0;i<OP_END;i++)
    {
        _actuators[i].Deactivate();
    }
}






