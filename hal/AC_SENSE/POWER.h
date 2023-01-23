/**
 * @file        [POWER]
 * @brief       This module derives AC Parameters for a phase from the ADC samples
 *              of the respective phase. The input parameters for this module are:
 *              1. Mains voltage sample
 *              2. Genset voltage sample
 *              3. CT Current sample - This could either correspond to mains
 *                 current or Genset current based on CT configuration.
 *              The o/p parameters are RMS voltages, current, frequencies,
 *              active power, apparent power, reactive power, power factor,
 *              active energy, apparent energy & reactive energy.
 *              RMS Computation
 *              RMS is computed via a moving window average of the squared
 *              parameter(i.e voltage, current etc). The squared parameters are
 *              stored in the window buffer. Every time the window moves the RMS
 *              is calculated.
 *              Active Power:
 *              Active power is computed using moving window average. The product
 *              of the instantaneous voltage & current is stored in the window
 *              buffer. All powers are computed every time the moving window moves.
 *              Apparent Power:
 *              The product of Vrms*Irms is the apparent power.
 *              Reactive Power:
 *              reactive_power=sqrt(apparent_power^2 -active_power^2).
 *              Power Factor:
 *              PF = active_power/apparent_power.
 *              Frequency Computation:
 *              Frequency computation is done using the voltage samples.To compute
 *              frequency, the number of samples between successive AC cycle
 *              positive to negative zero crosses are counted. The frequency
 *              is inversely proportional to the sample count.
 * @version     [Version number]
 * @author      Naveen Ramaswamy, Sudeep Chandrasekaran
 * @date        7th May 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/
#ifndef _POWER_H_
#define _POWER_H_

#include "stdint.h"
#include "AC_SENSE_CONFIG.h"
#include "AC_SENSE_DS.h"
#include "C_QUEUE.h"
#include "math.h"
#include "RMS.h"
#include "UTILS_Timer.h"

#define V_ANLG_FRONTEND_UPSCALER ( (R1_K_OHM+R2_K_OHM)/ R2_K_OHM)
#define ADC_SAMPLE_TO_V          ( BSP_ADC_REF_VTG/BSP_ADC_QUANTIZATION_LEVELS )
#define V_TO_ADC_SAMPLE          (1.0F/ADC_SAMPLE_TO_V)
#define I_ANLG_FRONTEND_UPSCALER (10.0F)
#define DC_VOLTAGE_OFFSET        (1.5F)
#define TIME_PERIOD_80HZ         (0.0125)

class POWER {
public:
    /**
     * Constructs the class.
     * @param u16SamplesPerEntry - Defines the number of samples to be considered
     *                     for one entry in the window buffer.
     * @return
     * None
     */
    POWER(uint16_t u16SamplesPerEntry=100);

    /**
     * Configures the CT position
     * @param eCtPos - Configures the CT either on genset or mains for current
     *                 measurement.
     * @return
     * None
     */
    void ConfigureISensing(I_CFG_t eICfg);
    void IsGenTamp(bool bGenTamp);

    /**
     * Returns the Genset RMS voltage.
     * @param - None
     * @return
     * Genset RMS voltage in volts
     */
    float GetGensetVoltage();

    /**
     * Returns the Genset RMS voltage which is filtered(configurable filter constant applied)
     * @param - None
     * @return
     * Display Genset RMS voltage in volts
     */
    float GetDispGensetVoltage();

    /**
     * Returns the Genset RMS Raw voltage.
     * @param - None
     * @return
     * Genset RMS voltage in volts
     */
    float GetGensetVoltageRaw();

    float GetGensetPollFrequency();


    /**
     * Returns the Mains RMS voltage.
     * @param - None
     * @return
     * Mains RMS voltage in volts
     */
    float GetMainsVoltage();

    /**
       * Returns the Mains RMS voltage which is filtered(configurable Filter constant applied)
       * @param - None
       * @return
       * Display Mains RMS voltage in volts
       */
    float GetDispMainsVoltage();

     /**
     * Returns the Genset RMS current. This will return 0 in case the CT is
     * configured to be on Mains.
     * @param - None
     * @return
     * Genset RMS current in MA
     */
    float GetGensetCurrent();

     /**
     * Returns the Mains RMS current. This will return 0 in case the CT is
     * configured to be on Genset.
     * @param - None
     * @return
     * Mains RMS current in MA
     */
    float GetMainsCurrent();

     /**
     * Returns the Genset active power. This will return 0 in case the CT is
     * configured to be on Mains.
     * @param - None
     * @return
     * Genset active power in Watts
     */
    float GetGensetActivePower();

     /**
     * Returns the Genset apparent power. This will return 0 in case the CT is
     * configured to be on Mains.
     * @param - None
     * @return
     * Genset apparent power in VA
     */
    float GetGensetApparentPower();

     /**
     * Returns the Genset reactive power. This will return 0 in case the CT is
     * configured to be on Mains.
     * @param - None
     * @return
     * Genset reactive power in VAR
     */
    float GetGensetReactivePower();

     /**
     * Returns the Mains active powerr. This will return 0 in case the CT is
     * configured to be on Genset.
     * @param - None
     * @return
     * Mains active power in Watts
     */
    float GetMainsActivePower();

     /**
     * Returns the Mains apparent power. This will return 0 in case the CT is
     * configured to be on Genset.
     * @param - None
     * @return
     * Mains apparent power in VA
     */
    float GetMainsApparentPower();

     /**
     * Returns the Mains reactive power. This will return 0 in case the CT is
     * configured to be on Genset.
     * @param - None
     * @return
     * Mains reactive power in VAR
     */
    float GetMainsReactivePower();

     /**
     * Returns the Genset power factor. This will return 0 in case the CT is
     * configured to be on Mains.
     * @param - None
     * @return
     * Genset power factor
     */
    float GetGensetPowerFactor();

    /**
     * Returns the Mains power factor. This will return 0 in case the CT is
     * configured to be on Genset.
     * @param - None
     * @return
     * Mains power factor
     */
    float GetMainsPowerFactor();

    float GetFiltGensetPowerFactor();
    float GetFiltGensetActivePower();
    float GetFiltGensetApparentPower();
    float GetFiltGensetReactivePower();

    float GetFiltMainsPowerFactor();
    float GetFiltMainsActivePower();
    float GetFiltMainsReactivePower();
    float GetFiltMainsApparentPower();


    /**
     * Returns the Genset frequency.
     * @param - None
     * @return
     * Genset frequency in Hz
     */
    float GetGensetFrequency();

    /**
     * Returns the Mains frequency.
     * @param - None
     * @return
     * Mains frequency in Hz
     */
    float GetMainsFrequency();

    double GetTamperedGensetActiveEnergy();

    /**
     * Returns the Genset active energy. This will return 0 in case the CT is
     * configured to be on Mains.
     * @param - None
     * @return
     * Genset active energy in Watt-Hour
     */
    double GetGensetActiveEnergy();

    double GetTamperedGensetApparentEnergy();
     /**
     * Returns the Genset apparent energy. This will return 0 in case the CT is
     * configured to be on Mains.
     * @param - None
     * @return
     * Genset apparent energy in VAH
     */
    double GetGensetApparentEnergy();

    double GetTamperedGensetReactiveEnergy();
     /**
     * Returns the Genset reactive energy. This will return 0 in case the CT is
     * configured to be on Mains.
     * @param - None
     * @return
     * Genset reactive energy in VARH
     */
    double GetGensetReactiveEnergy();

     /**
     * Returns the Mains active energy. This will return 0 in case the CT is
     * configured to be on Genset.
     * @param - None
     * @return
     * Mains active energy in Watt-Hour
     */
    double GetMainsActiveEnergy();

     /**
     * Returns the Mains apparent energy. This will return 0 in case the CT is
     * configured to be on Genset.
     * @param - None
     * @return
     * Mains apparent energy in VAH
     */
    double GetMainsApparentEnergy();

     /**
     * Returns the Mains reactive energy. This will return 0 in case the CT is
     * configured to be on Genset.
     * @param - None
     * @return
     * Mains reactive energy in VARH
     */
    double GetMainsReactiveEnergy();

    /**
     * Houses the routines that need to be run periodically. This API is expected
     * to be called in the super-loop.
     * @param - None
     * @return
     * None
     */
    void  Update();

    /**
     * This API is used to pass the inputs of this module(i.e ADC samples). This
     * is expected to be called at the ADC sampling rate.
     * @param u16GensetPhaseVoltageSample - Genset phase voltage ADC sample value
     * @param u16MainsPhaseVoltageSample - Mains phase voltage ADC sample value
     * @param u16NeutralVoltageSample - Neutral voltage ADC sample value
     * @param u16PhaseCurrentSample   - Current ADC sample value
     * @return
     * None
     */
    void UpdateSample(uint16_t u16GensetPhaseVoltageSample,
                            uint16_t u16MainsPhaseVoltageSample,
                                uint16_t u16GensetNeutralVoltageSample,
				    uint16_t u16MainsNeutralVoltageSample,
                                        uint16_t u16PhaseCurrentSample);

    void ClearCumulativeEnergy();
private:

    /*DC offset averaging window size to be considered initially*/
    #define INIT_DC_OFFSET_WINDOW_SIZE    (5000U)
    /*DC offset averaging window size to be considered post 
      INIT_DC_OFFSET_WINDOW_SIZE samples */ 
    #define DC_OFFSET_WINDOW_SIZE         (10000U)
    #define ZCD_UPPER_THRESHOLD_SAMPLES   (100)
    #define ZCD_LOWER_THRESHOLD_SAMPLES   (-100)
    #define SAMPLE_UPDATE_TIME_PERIOD_S   (0.0002F)
    #define MILLI_SECONDS_IN_HOUR         (3600000.0F)
    #define SUCCESSIVE_SAMPLES_THRESHOLD  (10)
    #define DISP_FILTER_FOR_POWER         (0.2F)

    /* Encapsulates all the variables necessary to compute the frequency */
    typedef struct {
        /* Used to count the ADC samples between successive positive to negative
           zero crosses
         */
        uint16_t   u16FreqSampleCtr;
        /* The number of ADC samples between the previous two zero positive to
           negative crosses
         */
        uint16_t   u16LatchedFreqSampleCtr;

        /* Holds the latest frequency value */
        float      fFrequency;

        /* Holds the filtered frequency value */
        float      fFrequencyfilt;

        /* Holds the second last frequency value. Needed as an i/p for frequency
           filter
         */
        float      fPrevFreq;
        /* Used to identify a transition from positive half cycle to negative
           half cycle
         */
        bool       bIsPrevPositiveHalfCycle;
    } FREQ_VARS_t;

    /* Encapsulates all the variables necessary to compute the Power Factor sign */
    typedef struct{
	/* Used to count the number of successive times a leading reading is 
	 *obtained when the current state is lagging and vice versa */
	uint16_t i16DetectionCount;
	/* Holds the Power Factor sign. Current is lagging if this is 1 and is
	 * leading if the sign is -1 */
	int16_t i16PFSign;
	/* Used to identify a transition from positive half cycle to negative
           half cycle */
        bool bIsPrevPositiveHalfCycle;
    } PFSIGN_VARS_t;

    uint16_t      _u16SamplesPerEntry;
    /*This is used to identify whether the window has moved*/
    bool          _bWindowMoved;
    /*This is offset value in terms of ADC samples for current samples*/
    uint16_t      _u16LatchedCurrentOffsetValue;
    /*Accumulator of current sample values, used in deducing DC offset*/
    uint32_t      _u32CurrentOffsetAccumulator;
    /*DC Offset sample count*/
    uint16_t      _u16CurrentDCOffsetSampleCount;
    /* DC offset window size*/
    uint16_t      _u16DCOffsetWindowSize;
    float         _fActivePower;
    float         _fApparentPower;
    float         _fReactivePower;
    float         _fFiltActivePower;
    float         _fFiltApparentPower;
    float         _fFiltReactivePower;
    float         _fPowerFactor;

    float         _fFiltPowerFactor;
    double        _fActiveEnergy;
    double        _fApparentEnergy;
    double        _fReactiveEnergy;

    I_CFG_t       _eICfg;
    bool          _bIsGenTamp;
    FREQ_VARS_t   _GensetFreqParams;
    FREQ_VARS_t   _MainsFreqParams;
    
    // RMS_VARS_t    _GensetVoltageParams;
    // RMS_VARS_t    _MainsVoltageParams;
    // RMS_VARS_t    _currentParams;
    // RMS_VARS_t    _powerParams;

    RMS           _GensetVoltageCalc;
    RMS           _MainsVoltageCalc;
    RMS           _currentCalc;
    RMS           _powerCalc;


    PFSIGN_VARS_t _pfSignParams;

    stTimer       _FreeRunningTimer;
    /**
     * This function accumulates squared sample values.
     * @param - i16SampleVal1 - First sample to be multiplied for RMS calculation
     * @param - i16SampleVal2 - Second sample to be multiplied for RMS calculation
     * @param - &param - Reference to RMS_VARS_t type struct
     * @return
     * None
     */
    // void prvAccumulateSampleSet(int16_t i16SampleVal1, int16_t i16SampleVal2,
    //                                                RMS_VARS_t &param);

    /**
     * Computes the RMS by averaging the accumulated squared sample values in the
     * sliding window.
     * @param - parameter - Reference to RMS_VARS_t type struct
     * @param - bSqrtReqd - Second sample to be multiplied for RMS calculation
     * @return
     * None
     */
    // void prvComputeRMS(RMS_VARS_t &parameter, bool bSqrtReqd);

    /**
     * Computes the active power by averaging the instantaneous voltage and
     * current values in the sliding window
     * @param - None
     * @return
     * None
     */
    void prvComputeActivePower();

    /**
     * Computes the apparent power by multiplying the Vrms and Irms values.
     * @param - None
     * @return
     * None
     */
    void prvComputeApparentPower();

    /**
     * Computes the reactive power by square rooting the squared active power
     * subtracted from the squared apparent power.
     * @param - None
     * @return
     * None
     */
    void prvComputeReactivePower();

    /**
     * Computes the power factor by dividing the active power by apparent power
     * subtracted from the squared apparent power.
     * @param - None
     * @return
     * None
     */
    void prvComputePowerFactor();

    /**
     * Computes the power factor by dividing the active power by apparent power
     * subtracted from the squared apparent power.
     * @param - FreqParams - Reference to FREQ_VARS_t type struct
     * @return
     * None
     */
    void prvComputeFrequency(FREQ_VARS_t &FreqParams);

    /**
     * Counts the number of samples between two zero crossings.
     * @param - i16FilteredVoltageSample
     * @param - FreqParams - Reference to FREQ_VARS_t type struct
     * @return
     * None
     */
    void prvCountFreqSamples(int16_t i16FilteredVoltageSample, FREQ_VARS_t &FreqParams);

    /**
     * Does a weighted average over the previous and current frequency sample.
     * @param - FreqParams - Reference to FREQ_VARS_t type struct
     * @return
     * None
     */
    void prvFrequencyFilter(FREQ_VARS_t &FreqParams);

     /**
     * Computes the active, apparent and reactive energies from their respective power
     * by multiplying it with the time it takes for the window to move.
     * @param - None
     * @return
     * None
     */
    void prvComputeEnergies();

    /**
     * Computes the sign of the Power Factor by comparing the voltage and current
     * with each other at the voltage positive to negative zero crossing. 
     * @param - i16FilteredVoltageSample
     * @param - i16FilteredCurrentSample 
     * @param - PfSignParams - Reference to PFSIGN_VARS_t type struct
     * @return
     * None
     */
    void prvCalculatePowerFactorSign(int16_t i16FilteredVoltageSample,
                         int16_t i16FilteredCurrentSample, PFSIGN_VARS_t &PfSignParams);
};
#endif
