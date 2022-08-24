#include "HAL_Manager.h"


HAL_Manager::HAL_Manager():
AcSensors(this->AcIp, this->AnlgIp),
DigitalSensors(this->dip), // @suppress("Symbol is not resolved")
AnalogSensors(this->AnlgIp, this->PulseIp, this->AcIp, this->AcSensors),
actuators(this->stpr, this->hsdManager),
_PowerOnUpdate{0}
{
    UTILS_ResetTimer(&_PowerOnUpdate);
}

void HAL_Manager::Update()
{
    /*BSP Update*/
    BSP_Manager::Update();
    if(!AmIInSleep())
    {
        if(UTILS_GetElapsedTimeInSec(&_PowerOnUpdate) > 8)
        {
            DigitalSensors.Update();
        }
        AcSensors.Update();
        AnalogSensors.Update();
    }
}
