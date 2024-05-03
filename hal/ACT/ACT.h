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

#ifndef _ACT_H_
#define _ACT_H_

#include "stdint.h"

/* Forward declaration*/
class STPR_DRV;
class HSD;
class HSD_Manager;

/* A class to house all functionality based on DO based actuator*/
class ACTUATOR {

public:

    /*List of actuators to which ACTUATOR can be configured to*/
    typedef enum {
        ACT_NOT_CONFIGURED = 0,
        ACT_AUDIBLE_ALARM,
        ACT_VBAT_OV,
        ACT_VBAT_UV,
        ACT_CA_SHUTDOWN,
        ACT_CA_WARNING,
        ACT_CLOSE_GEN_CONTACTOR,
        ACT_CLOSE_MAINS_CONTACTOR,
        ACT_MAINS_FAILURE,
        ACT_ALARM,
        ACT_ELEC_TRIP,
        ACT_SHUTDOWN,
        ACT_WARNING,
        ACT_COOLING_ON,
        ACT_DIG_IN_A,
        ACT_DIG_IN_B,
        ACT_DIG_IN_C,
        ACT_DIG_IN_D,
        ACT_DIG_IN_E,
        ACT_DIG_IN_F,
        ACT_DIG_IN_G,
        ACT_DIG_IN_H,
        ACT_DIG_IN_I,
        ACT_DIG_IN_J,
        ACT_DIG_IN_K,
        ACT_DIG_IN_L,
        ACT_DIG_IN_M,
        ACT_DIG_IN_N,
        ACT_DIG_IN_O,
        ACT_DIG_IN_P,
        ACT_E_STOP,
        ACT_STOP_SOLENOID,
        ACT_FAIL_TO_START,
        ACT_FAIL_TO_STOP,
        ACT_FUEL_RELAY,
        ACT_GEN_AVLBL,
        ACT_GEN_R_OV_SHUTDOWN,
        ACT_GEN_R_UV_SHUTDOWN,
        ACT_GEN_Y_OV_SHUTDOWN,
        ACT_GEN_Y_UV_SHUTDOWN,
        ACT_GEN_B_OV_SHUTDOWN,
        ACT_GEN_B_UV_SHUTDOWN,
        ACT_GEN_OC,
        ACT_HIGH_TEMP,
        ACT_LOW_FUEL,
        ACT_LOW_FUEL_NOTIFICATION,
        ACT_LOW_PRES,
        ACT_MAINS_HIGH,
        ACT_MAINS_LOW,
        ACT_OIL_CKT_OPEN,
        ACT_OPEN_GEN_OUT,
        ACT_OPEN_MAINS_OUT,
        ACT_OF_SHUTDOWN,
        ACT_OS_SHUTDOWN,
        ACT_GROSS_OS_SHUTDOWN,
        ACT_START_RELAY,
        ACT_TEMP_CKT_OPEN,
        ACT_UF_SHUTDOWN,
        ACT_US_SHUTDOWN,
        ACT_FILT_MAINT,
        ACT_MODE_STOP,
        ACT_MODE_AUTO,
        ACT_MODE_MANUAL,
        ACT_BTS_BATTERY_HYBRID_MODE,
        ACT_PREHEAT,
        ACT_ECU_START,
        ACT_MIL,
        ACT_INDUCEMENT_BUZZER,
        ACT_EGR,
        ACT_AUTO_MODE_SW_OUTPUT,
        ACT_BATTERY_UNHEALTHY,
        ACT_SUPERCAP_UNHEALTHY
    } ACTUATOR_TYPS_t;

    /* Two possibilities for activation*/
    typedef enum {
        ENERGIZE_TO_ACTIVATE  = 0,
        DENERGIZE_TO_ACTIVATE
    } ACTIVATION_CFG_t;

    /**
     * Constructs this class
     * @param hsd - The hsd this actuator shall use
     * @return
     * None
     */
    ACTUATOR(HSD &hsd);

    /**
     * Configures this actuator
     * @param eType       - The type of actuator
     * @param eActivation - activation type(i.e ENERGIZE_TO_ACTIVATE/DENERGIZE_TO_ACTIVATE)
     * @return
     * None
     */
    void Configure(ACTUATOR_TYPS_t eType, ACTIVATION_CFG_t eActivation);

    /**
     * Returns the configured name of the actuator.
     * @param: None
     * @return
     * Name of the actuator
     */
    ACTUATOR_TYPS_t GetActuatorName();

    /**
     * Activates this actuator.
     * @param: None
     * @return
     * None
     */
    void Activate();

    /**
     * Deactivates this actuator.
     * @param: None
     * @return
     * None
     */
    void Deactivate();

    bool GetActuatorStatus();
private:
    /*Stores the name of the actuator*/
    ACTUATOR_TYPS_t  _actuatorName;
    /*Stores activation state*/
    ACTIVATION_CFG_t _eActivationState;

    bool _ActuatorStatus;
    /*Reference to the HSD corresponding to this actuator*/
    HSD &_hsd;
};


class ACT_Manager {

public:
    #define PULSE_TO_GO_HOME_POSITION 700

    typedef enum {
        ACT_SUCCESS,
        ACT_LATCHED,
        ACT_NOT_LATCHED,
        ACT_NOT_CONFIGURED
    } ACT_STATUS_t;

    /// Instances of outputs
    typedef enum {
        OP_A   = 0,
        OP_B   = 1,
        OP_C   = 2,
        OP_D   = 3,
        OP_E   = 4,
        OP_F   = 5,
        OP_G   = 6,
        OP_H   = 7,
        OP_I   = 8,
        OP_J   = 9,
        OP_K   = 10,
        OP_L   = 11,
        OP_M   = 12,
        OP_N   = 13,
        OP_O   = 14,
        OP_P   = 15,
        OP_END
    } HSD_OUTPUTS_t;

    typedef struct {
        ACTUATOR::ACTUATOR_TYPS_t  eType;
        ACTUATOR::ACTIVATION_CFG_t eActivation;    
    } PROPERTIES_t;

    typedef struct {
        PROPERTIES_t properties[OP_END];
    } CFG_t;

    typedef enum
    {
        A3981,
        A5976
    }ALEGRO_t;
    /**
     * Constructs this class
     * @param stprDrv   : Reference to the stepper driver object
     * @param HSDManager: Reference to the HSD manager object
     * @return
     * None
     */
    ACT_Manager(HSD_Manager &HSDManager);

    /**
     * Houses routines that needs to be called in the super-loop.
     * @param : None
     * @return
     * None
     */
    void Update();

    /**
     * Configures the actuators
     * @param cfg   : Configuration data structure.
     * @return
     * None
     */
    void Configure(CFG_t &cfg);

    /**
     * Activates the specified actuator
     * @param eType : Name of actuator
     * @return
     * ACT_SUCCESS         - The actuator was activated
     * ACT_NOT_CONFIGURED  - The actuator was not configured
     */
    ACT_STATUS_t Activate(ACTUATOR::ACTUATOR_TYPS_t eType);

    /**
     * Configures the actuators
     * @param cfg   : Configuration data structure.
     * @return
     * ACT_SUCCESS         - The actuator was deactivated
     * ACT_NOT_CONFIGURED  - The actuator was not configured
     */
    ACT_STATUS_t Deactivate(ACTUATOR::ACTUATOR_TYPS_t eType);

    ACT_STATUS_t GetActStatus(ACTUATOR::ACTUATOR_TYPS_t eType);

    /**
     * Deactivates all actuators
     * @param: None
     * @return
     * None
     */    
    void DeactivateAll();



private:
    /*Holds reference to the HSD driver*/
    HSD_Manager     &_HSDManager;
    /*Actuator objects*/
    ACTUATOR _actuators[OP_END];


};
#endif




