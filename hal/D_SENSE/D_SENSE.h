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
 * @author      Nikhil Mhaske
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#ifndef _DSENSE_H_
#define _DSENSE_H_

#include "stdint.h"
#include "DIG_IP.h"
#include "../../../utils/UTILS_Debouncer.h"

class DigitalSensor {
public:
    /// The digital i/p's available over the connector can be configured
    /// to connect to different types of digital sensors. The below
    /// enum lists the sensors that the digital i/p can be connected to.
    typedef enum {
         DI_NOT_CONFIGURED              ,
         DI_A_USER_CONFIGURED           ,
         DI_B_USER_CONFIGURED           ,
         DI_C_USER_CONFIGURED           ,
         DI_D_USER_CONFIGURED           ,
         DI_E_USER_CONFIGURED           ,
         DI_F_USER_CONFIGURED           ,
         DI_G_USER_CONFIGURED           ,
         DI_H_USER_CONFIGURED           ,
         DI_I_USER_CONFIGURED           ,
         DI_J_USER_CONFIGURED           ,
         DI_K_USER_CONFIGURED           ,
         DI_L_USER_CONFIGURED           ,
         DI_M_USER_CONFIGURED           ,
         DI_N_USER_CONFIGURED           ,
         DI_O_USER_CONFIGURED           ,
         DI_P_USER_CONFIGURED           ,
         DI_LOW_FUEL_LEVEL_SWITCH       ,
         DI_LOW_LUBE_OIL_PRESSURE_SWITCH,
         DI_HIGH_ENGINE_TEMP_SWITCH     ,
         DI_LOW_WATER_LEVEL_SWITCH      ,
         DI_EMERGENCY_STOP              ,
         DI_REMOTE_START_STOP           ,
         DI_SIMULATE_START              ,
         DI_SIMULATE_STOP               ,
         DI_SIMULATE_AUTO               ,
         DI_CLOSE_GEN_OPEN_MAINS_SWITCH ,
         DI_CLOSE_MAINS_OPEN_GEN_SWITCH ,
         DI_SIMULATE_MAINS              ,
         DI_V_BELT_BROKEN_SWITCH        ,
         DI_MAINS_CONTACTOR_LATCHED,
         DI_GEN_CONTACTOR_LATCHED,
         DI_BATTERY_CHARGER_FAIL,
         DI_SMOKE_FIRE,
         DI_MODE_SELECT,
         DI_AMB_TEMP_SELECT,
         DI_EGR_ECU_DIGITAL_IN,
         DI_EB_MCCB_ON_FEEDBACK,
         DI_DG_MCCB_ON_FEEDBACK,
         DI_SENSOR_END
    } D_SENSOR_TYPS_t;

    typedef enum {
        SENSOR_UNLATCHED,
        SENSOR_LATCHED,
        SENSOR_NOT_CONFIGRUED
    } STATUS_t;

    /// Defines when to consider the input to be active(active low/high)
    typedef enum {
        CLOSE_TO_ACTIVATE,
        OPEN_TO_ACTIVATE ,
    } ACTIV_POLARITY_t;

    /**
     * Constructor for a digital sensor.
     * @param  : None
     * @return
     * None
     */
    DigitalSensor();

    /**
     * Configures the digital sensor.
     * @param  eActivationPolarity : The polarity in which the sensor needs to be
     *                               considered active.
     * @param u16ActivationDelaySeconds : The time for which the sensor needs to
     *                               be active to change the activation status.
     * @param db                   : The debouncer to be used to fetch the sensor
     *                               status.
     * @param typ                  : Type of the sensor.
     * @return
     * None
     */
    void CfgDigitalSensor(ACTIV_POLARITY_t eActivationPolarity,
                          uint16_t u16ActivationDelaySeconds,
                          Debouncer &db, D_SENSOR_TYPS_t typ);

    /**
     * Returns the current status of the sensor.
     * @param - None
     * @return
     * SENSOR_LATCHED
     * SENSOR_UNLATCHED
     * SENSOR_NOT_CONFIGRUED
     */
    STATUS_t GetState();


   /**
    * Returns the type of the sensor.
    * @param - None
    * @return D_SENSOR_TYPS_t - returns the type of the configured sensor assigned
    *                           to the digital input.
    */
    D_SENSOR_TYPS_t GetSensorId();

    /**
     * Contains code that needs to be frequently run. This is expected to be called
     * in the super-loop.
     * @param - None
     * @return
     * None
     */
    void Update();

private:
    /*States of internal state machine*/
    typedef enum {
        STATE_NOT_ACTIVATED,
        STATE_ACTIVATION_IN_PROGRESS,
        STATE_ACTIVATED
    } STATE_t;

    Debouncer *_pDb;
    ACTIV_POLARITY_t _eActivationPolarity;
    uint16_t _u16ActivDelaySeconds;
    stTimer _ActTimer;
    stTimer _DeactTimer;
    STATE_t _state;
    D_SENSOR_TYPS_t _SensorId;
};

class D_SENSE {

public:

    /// Instances of digital inputs
    typedef enum {
        DI_A   = 0,
        DI_B,
        DI_C,
        DI_D,
        DI_E,
        /* DI_F, DI_G, DI_H and DI_I are analog sensors used as a digital inputs. */
        DI_F,
        DI_G,
        DI_H,
        DI_I,
        DI_J,
        DI_K,
        DI_L,
        DI_M,
        DI_N,
        DI_O,
        DI_P,
        DI_END
    } INPUTS_t;

    /// Represents the configuration associated with a configurable
    /// digital input
    typedef struct {
        DigitalSensor::D_SENSOR_TYPS_t  eType;
        DigitalSensor::ACTIV_POLARITY_t eActivationPolarity;
        uint16_t                        u16ActivationDelaySeconds;
    } PROPERTIES_t;

    typedef struct {
        PROPERTIES_t stDIConfig[DI_END];
    } CFG_t;

    /**
     * Constructs the digital sensor object.
     * @param  digIp - Reference to the digital input object from BSP.
     * @return
     * None
     */
    D_SENSE(DIG_IP &digIp);

    /**
     * Maps the digital input to the specified sensors.
     * @param  cfg - The configuration structure containing mapping between
     *               inputs and sensors.
     * @return
     * None
     */
    void ConfigureSensor(CFG_t &cfg);

    /**
     * Contains code that needs to be frequently run. This is expected to be called
     * in the super-loop.
     * @param - None
     * @return
     * None
     */
    void Update();

    DigitalSensor::STATUS_t GetDigInputState(int InputPin);

    /**
     * Returns the status of the requested digital input source. This is expected
     * to be called in the application code.
     * @param  typ - Source type assigned to the digital input.
     * @return STATUS_t -  Status of the requested digital input type.
     * None
     */
    DigitalSensor::STATUS_t GetDigitalSensorState(DigitalSensor::D_SENSOR_TYPS_t typ);


private:
    /*Variable to store the reference of the digital input object from BSP*/
    DIG_IP &_digIp;

    /*Array of all possible sensors here*/
    DigitalSensor _aDigitalSensors[DI_END];
};

#endif
