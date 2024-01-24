/**
 * @file        [A_SENSE]
 * @brief       This module is used configure the sensor and then calculate the
 *              value of analog sensor by applying calibration table.
 *              It has API's to get the filtered values of all analog sensors
 * @version     [Version number]
 * @author      Madhuri Abhang
 * @date        2nd Jul 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef _A_SENSE_H_
#define _A_SENSE_H_

#include "stdint.h"
#include "D_SENSE.h"
#include "PULSE_IP.h"
#include "../bsp/ANLG_IP/ANLG_IP.h"
#include "../bsp/AC_IP/AC_IP.h"
#include "AC_SENSE.h"
#include "../../../utils/UTILS_Timer.h"

#define PER_MIN_CNT_IN_SEC         (60U)
#define MAX_INTERPOLATION_POINTS   (20U)
#define UPDATE_ANGL_VALUE_TIME_MS  (200U)
#define UPDATE_Vbat_VALUE_TIME_MS  (200U)
#define CA_FILTER_CONST            (0.95F)
#define CA_ROUND_OF_CONST          (0.15F)
#define VBAT_FILTER_CONST          (0.2F)
#define VBTS_BAT_FILTER_CONST      (0.2F)
#define SENSOR_FILT_CONST          (80U)
#define RPM_CONVERSION_CONST       (120U) // 60sec * 2 for positive and negative cycle
#define RPM_CLEAR_TIMER_MS         (1000)
/*
 * SuryaPranayTeja.BVV 17-11-2022
 * The MIN_COMPARTOR_SENSE_VTG was 70 previously.
 * The observations with O.micron is
 * From (30V to 50V) and (5Hz to 15Hz) fluctuations of 50-60 RPM is observed.
 * Changed to 30 as per the discussion with SysE.
 */
/*
 * SuryaPranayTeja.BVV 22-12-2022
 * Further changed the valueFrom 30V to 20V after facing the issue
 * of Invalid DG run. Do not have a solid reason to change it but
 * with assumption that there could be some other issue we changed it.
 */
#define MIN_COMPARTOR_SENSE_VTG    (20.0)
/*
 * SuryaPranayTeja.BVV 24-12-2022
 * Invented the macro with different value to solve the field issue.
 * Later decided to use the same value as MIN_COMPARTOR_SENSE_VTG.
 * Still the macro is in use to allow the flexibility to change in future if required.
 * Please find the data in the Confluence page GC2111 Field info.
 * This will be used only for the Invalid DG Run and
 * The Call Back function where we get the RPM also will be changed.
 * This has been agreed with SYSE.
 */
#define VOLTAGE_THRESHOLD_FOR_SPEED_COMPUTATION (20.0)
#define SPEED_FLT_DETECT_DLY       (250U)
/* Forward declaration of ANLG_IP*/
class ANLG_IP;

class AnalogSensor {
public:
    /**
     * Sensors list, can be vary as per product
     */
    typedef enum
    {
        A_SENSE_NOT_USED,
        A_SENSE_LUBE_OIL_PRESSURE,
        A_SENSE_FUEL_LEVEL_RESISTIVE,
        A_SENSE_ENG_TEMPERATURE,
        A_SENSE_LUBE_OIL_PRESSURE_4_20,
        A_SENSE_LUBE_OIL_PRESSURE_0_TO_5V,
        A_SENSE_FUEL_LEVEL_0_TO_5V,
        A_SENSE_SHELTER_TEMPERATURE,
        A_SENSE_S2_SENSOR,
        A_SENSE_DIG_J,
        A_SENSE_DIG_K,
        A_SENSE_DIG_L,
        A_SENSE_DIG_M,
        A_SENSE_DIG_N,
        A_SENSE_DIG_O,
        A_SENSE_DIG_P


    } TYPS_t;

    /**
     * Calibration x and y axis tables for interpolation
     */
    typedef struct
    {
        float af32IntrpolationTableX[MAX_INTERPOLATION_POINTS];
        float af32IntrpolationTableY[MAX_INTERPOLATION_POINTS];
        uint8_t u8InterPolationPoints;
    } INTERPOLATE_INFO_t;

    /**
     * Function pointer for getting the analog values from BSP
     * @param eRef :  REF_ENGINE_BOD or REF_BATT_NEGATIVE
     * @return It will return the Sensors value in (Resistor/Voltage/Current)
     * and the sensor status.
     */
    typedef ANLG_IP::ANALOG_VAL_t (ANLG_IP::*pGetVal)(ANLG_IP::SENSOR_REF_t eRef);


    /**
     * Constructor of the AnalogSensor class
     * @param anlgIp  reference of BSP class ANLG_IP
     * @return None
     */
    AnalogSensor(ANLG_IP &anlgIp);

    /**
     * Update function of the AnalogSensor class, it
     * should be called in main loop.
     * @param None
     * @return None
     */
    void Update();

    /**
     * Analog sensor configuration function
     * @param eName - Name of the sensor from TYPS_t enum list
     * @param stTable - Interpolation x and y table
     * @param pGetValSens - Function address for getting the raw sensor value
     * @param eRef - It can be of Engine body or Battery negative.
     * @return None
     */
    void CfgAsAnalogSensor( TYPS_t  eName, INTERPOLATE_INFO_t stTable,
                            pGetVal pGetValSens,
                            ANLG_IP::SENSOR_REF_t eRef
                          );

    /**
     * This function will return the interpolated value of a sensor.
     * @param None
     * @return Value of Sensor, its open ckt status and sensor configuration
     * status.
     */
    ANLG_IP::ANALOG_VAL_t GetValue();

    /**
     * This function will return type of the sensor
     * @param None
     * @return sensor type
     */
     TYPS_t GetName();

     /**
      * This is used to Calculate the fuel in percentage for the 0-5V Fuel sensor.
      * It is two tank based system with different Height and Lengths.
      */
     float FuelCalcfor0_5VSens(float height);

     float GetS3SensorCurrentValue();

     float GetS3SensorVoltValue();

     float GetS4SensorVoltValue();
private:
    /**
     * Private variable to store the Sensor name
     */
    TYPS_t  _eName;

    /**
     * Private variable to store the interpolation table and number of points
     */
    INTERPOLATE_INFO_t _stTable;

    /**
     * Function pointer to store the address BSP class function.
     */
    pGetVal _pGetValSens;

    /**
     * Private variable to store the reference of the sensor
     */
    ANLG_IP::SENSOR_REF_t _eRef;

    /**
     * Private variable to store the value of the sensor and its open ckt status
     */
    ANLG_IP::ANALOG_VAL_t _stSensVal;

    /**
     * This is used to calculate the interpolated value of the sensor using
     * its calibration table(x,y)
     * @param f32Val Sensor raw value.
     * @param fpXValues X-Axis table
     * @param fpYValues Y-Axis table
     * @param u8No_of_points Number of points
     * @return It will return the interpolated value
     */
    float prvInterpolation(float f32Val, float * fpXValues, float * fpYValues,
                        uint8_t u8No_of_points);



    /**
     * BSP class ANLG_IP's reference for calling the BSP functions.
     */
    ANLG_IP &_AnlgIp;

    float _f32S3CurrentVal;

    float _f32S3VoltVal;

    float _f32S4VoltVal;

};

class A_SENSE {
public:
    /**
     * Represents the Pulse input type
     */
    typedef enum
    {
        MPU_TYPE,
        W_POINT_TYPE
    }PULSE_IP_TYPE_t;

    /**
     * Number of Physical analog inputs.
     */
    typedef enum
    {
        HAL_PIN_11   = 0,/**< HAL_PIN_11 */
        HAL_PIN_12  ,    /**< HAL_PIN_12 */
        HAL_PIN_13  ,    /**< HAL_PIN_13 */
        HAL_PIN_14  ,    /**< HAL_PIN_14 */
        HAL_PIN_15  ,    /**< HAL_PIN_15 */
        HAL_PIN_21  ,    /**< HAL_PIN_21 */
        HAL_PIN_23  ,    /**< HAL_PIN_23 */
        HAL_ENG_AI_END   /**< HAL_ENG_AI_END */
    } AI_INPUTS_t;

    /**
     * Structure to store the configuration parameters.
     */
    typedef struct
    {
        AnalogSensor::TYPS_t  eSensor;
        ANLG_IP::SENSOR_REF_t eRef;
        AnalogSensor::INTERPOLATE_INFO_t stTable;
    } PROPERTIES_t;

    /**
     * Configuration for all type of physical sensors
     */
    typedef struct
    {
        PROPERTIES_t stAIConfig[HAL_ENG_AI_END];
    } CFG_t;

    /**
     * Sensor Status
     */
    typedef enum
    {
        SENSOR_NOT_CONFIGRUED,
        SENSOR_READ_SUCCESS,
    } STATUS_t;

    /**
     * This structure is store the analog after interpolation.
     * and its open ckt status
     * and read status.
     */
    typedef struct
    {
       ANLG_IP::ANALOG_VAL_t stValAndStatus;
       STATUS_t eStatus;
    } SENSOR_RET_t;

    /**
     *
     */
    typedef struct
    {
        uint8_t TankwithStep;
        uint16_t TankWidth;
        uint16_t TankStep1Length;
        uint16_t TankStep2Length;
        uint16_t TankStep1Height;
        uint16_t TankStep2Height;
    } FUEL_0_5V_t;

    static FUEL_0_5V_t _Fuel_0_5V_vals;

    /**
     * CB that will be registered to get measured
     * RPM for MPU or Wpoint input
     */
    typedef void (*pRPMUpdate_t)(float f32RPM);


    /** Timer for reseting the RPM through Pulse Input
    */
   static stTimer SpeedSensorTimer;

    /**
     * Constructor of A_SENSE class
     * @param  anlgIp reference of BSP class ANLG_IP
     * @param  pulseIp reference of BSP class PULSE_IP
     * @param  AcIp reference of BSP class AC_IP
     * @return None
     */
    A_SENSE(ANLG_IP &anlgIp, PULSE_IP &pulseIp, AC_IP &AcIp, AC_SENSE &AcSensors);

    /**
     * Update function of A_SENSE class.
     * It updates the filtered value of Vbat , Charging alternator
     * analog sensors, pulse input RPM.
     * @param  None
     * @return None
     */
    void Update();

    float GetFilteredPulseInpuRPM();

    float GetS3CurrentValMilliAmp();

    float GetS3VoltVal();
    float GetS4VoltVal();

    /** This function is used to get the RPM of R-phase
     * through comparator input
     * @param None
     * @return RPM value
     */
    float GetRPMForInvalidDG();

    /** This function is used to get the RPM of R-phase
     * through comparator input
     * @param None
     * @return RPM value
     */
    float GetFiltRPMThruCompartor();

    /** This function is used to get the filtered RPM through comparator input
     * @param None
     * @return Frequency
     */
    float GetGensetFreqThruCompartor();

    /**
     * This is to configure the pulse input, it can be of MPU or W-point type.
     * For MPU number of teeth configuration is required
     * For W-point, frequency at 1500 is required.
     * @param  u16NoOfTeethOrFreq
     * @param  _type
     * @return none
     */
    void ConfigPulseInput(uint16_t u16NoOfTeethOrFreq,  PULSE_IP_TYPE_t etype);

    /**
     * This is to configure number of poles, which is used to
     * calculate the RPM though the comparator input.
     * @param u8Poles
     * @return none
     */
    void ConfigureNumberOfPoles(uint8_t u8Poles);

    /**
     * This function is used to initialize the value of pulses to be skipped at
     * power or at every crank.
     * @param u16NumberofPulses
     * @return None
     */
    void SkipPulses( uint16_t u16NumberofPulses);

    /**
     * Registers a callback to receive the measured
     * RPM.
     * The registered CB gets called within Update function of A_SENSE
     * Register this for calling EGOV PID loop.
     * @param pCB : Pointer to the callback to be registered.
     * @return None
     */
    void RegisterPulseInputUpdateCB(pRPMUpdate_t pCB);

    /**
     * Registers a callback to receive the measured
     * RPM.
     * The registered CB gets called within Update function of A_SENSE
     * Register this for calling EGOV PID loop.
     * @param pCB : Pointer to the callback to be registered.
     * @return None
     */
    void RegisterCompInputUpdateCB(pRPMUpdate_t pCB);

    void ConfigureFuel0_5V_SensorValue(FUEL_0_5V_t &Fuel_0_5V);
    /**
     * Sensor Configuration function it should be called
     * at power on and after changing the configuration
     * @param configuration of the sensor
     * @return None
     */
    void ConfigureSensor(CFG_t &cfg);

    /**
     * Get charging alternator filtered value
     * @param None
     * @return float value
     */
    float GetFilteredChargingAltVolts();

    /**
     * Get Battery filtered value
     * @param None
     * @return float value
     */
    float GetFilteredVbattVolts();

    /**
     * Get Battery filtered value
     * @param None
     * @return float value
     */
    float GetFilteredVBTSbattVolts();

    /**
     * This function will return the interpolated value of a sensor.
     * @param Type of the sensor
     * @return Value of Sensor, its open ckt status and sensor configuration
     *  status.
     */
    SENSOR_RET_t GetSensorValue(AnalogSensor::TYPS_t eSensor);

    /**
     * It will return speed sensor fault status true when fault is present
     * @param None
     * @return bool
     */
    bool GetSpeedSensFltStatus();

private:
    /**
     * Reference of ANLG_IP class
     */
    ANLG_IP &_AnlgIp;

    /**
     * Reference of PULSE_IP class
     */
    PULSE_IP &_PulseIp;

    /**
     * Reference of AC_IP class
     */
    AC_IP &_AcIp;
    /**
     * Reference of AC_SENSE class.
     */
    AC_SENSE &_AC_SENSE;
    /**
     *  It will contain the pulse input type.
     */
    PULSE_IP_TYPE_t _PulseInputType;


    /**
     * This will contain the multiplication factor RPM calculation
     * when pulse input type is W-Point.
     */
    float _f32FacWpointCntToRPM;

    /**
     * CB to get RPM from Pulse input.
     */
    pRPMUpdate_t _UpdatePulseRPM_cb;

    /**
     * Pulse Input RPM value
     */
    float _f32PulseInputRPM;

    /**
     * Private variable to store the filtered value of Vbat
     */
    float _f32FiltVbat;

    /**
     * Private variable to store the filtered value of VBTSbat
     */
    float _f32FiltVBTSbat;

    /**
     * Private variable to store the filtered value of Charging Alternator
     */
    float _f32FiltChargingAltVtg;

    /**
     * Private timer for updating the Vbat Value
     */
    stTimer _UpdateTimerVbat;

    /**
     * Private variable for polling the analog values at specific rate
     */
    stTimer _UpdateTimer;

    /**
     * Private timer for reseting the RPM through Pulse Input
     */
    static stTimer _RPMResetTimerPulseIp;

    /**
     * Array of the AnalogSensor objects with size of number of Physical
     * Analog inputs.
     */
    AnalogSensor _sensors[HAL_ENG_AI_END];

    /**
     * This is to store the 1/2 cycle( of fly wheel) time count,
     * used for RPM measurement.
     */
    volatile static uint32_t _u32DiffCycleCnt;

    /**
     * This will give the status that we should update the RPM value.
     */
    volatile static bool _bStatusUpdateRPM;

    /**
     * This contains the value of number of teeth fly wheel has,
     * should be initialized after reading the configuration.
     */
    static uint16_t _u16NumberOfTeeth;

    /**
     * It will contain the value of number of pulses to be skipped
     * at power on or at every crank
     */
    volatile static uint16_t _u16NoOfPulsesToskip;

    /**
     * We are measuring RPM in 1/2 cyclic mode, so this variable is used
     * to calculate the number of pulses.
     */
    volatile static uint16_t _u16PulsesPerCalcCycle;
    
    /**
     * CB to get RPM from Comparator input.
     */
    static pRPMUpdate_t _UpdateCompRPM_cb;
    
    /**
     * Comparator Input RPM value
     */
    volatile static float _f32CompInputRPM;

    volatile static float _f32InvalidDGRPM;

    volatile static float _f32FilteredRPM;

    volatile static float _f32FilteredPulseIpRPM;

    volatile static float _f32RPhaseFreq;

    /**
     * Comparator input frequency value.
     */
    volatile static float _f32CompGenRFreq;

    volatile static float _f32GenRPhaseRawVtg;

    /**
     * Private variable to store the number of alternator poles
     */
    static uint8_t _u8NumberOfPoles;

    /**
     * Private timer for reseting the RPM & Freq through Comparator
     */
    static stTimer _RPMResetTimerComparator;

    /**
     * This function is used to update the RPM and its status
     * @param  Timer latched value of 1/2 cyclic measurement
     * @return None
     */
    static void prvUpdatePulseInpuRPM_Status(uint32_t u32TimerLatchVal);

     /**
     * This function is used to update the RPM and its status
     * @param  Timer latched value of 1/2 cyclic measurement
     * @return None
     */
    static void prvUpdateCompInpuFreqRPM(float f32Freq);
    /** Speed sensor fault status
        */
    bool _bSpeedSensorFault;
};

#endif
