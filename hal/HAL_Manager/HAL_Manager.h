#ifndef _HAL_MANAGER_H_
#define _HAL_MANAGER_H_

#include "BSP_Manager.h"
#include "AC_SENSE.h"
#include "D_SENSE.h"
#include "A_SENSE.h"
#include "ACT.h"

class HAL_Manager: public BSP_Manager {

public:
    AC_SENSE    AcSensors;
    D_SENSE     DigitalSensors;
    A_SENSE     AnalogSensors;
    ACT_Manager actuators;

    HAL_Manager();
    
    void Update();
private:
    stTimer _PowerOnUpdate;
};

#endif
