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
#include "POWER.h"
#if (SUPPORT_CALIBRATION == YES)
#include "ANLG_IP.h"
#endif /* SUPPORT_CALIBRATION */

POWER::POWER(uint16_t u16SamplesPerEntry):
_u16SamplesPerEntry(u16SamplesPerEntry),
_bWindowMoved(false),
#if (SUPPORT_CALIBRATION == YES)
_bWindowMovedResultLatched(false),
_f32LatchedCurrentOffsetValue(0.0f), /* After calibration, we pass DC offset de-referenced values to power class */
_f64CurrentOffsetAccumulator(0.0f),
#else
_u16LatchedCurrentOffsetValue(DC_VOLTAGE_OFFSET*V_TO_ADC_SAMPLE),
_u32CurrentOffsetAccumulator(0),
#endif /* SUPPORT_CALIBRATION */
_u16CurrentDCOffsetSampleCount(0),
_u16DCOffsetWindowSize(INIT_DC_OFFSET_WINDOW_SIZE),
_fActivePower(0),
_fApparentPower(0),
_fReactivePower(0),
_fFiltActivePower(0),
_fFiltApparentPower(0),
_fFiltReactivePower(0),
_fPowerFactor(1.0),
_fFiltPowerFactor(0.0),
_fActiveEnergy(0),
_fApparentEnergy(0),
_fReactiveEnergy(0),
_eICfg(NO_CURRENT_MEASUREMENT),
_bIsGenTamp(false),
_GensetFreqParams{
    0,                                         /*u16FreqSampleCtr*/
    0,                                         /*u16LatchedFreqSampleCtr*/
    0,                                         /*fFrequency*/
    0,                                         /*fPrevFreq*/
    false                                      /*bIsPrevPositiveHalfCycle*/
},
_MainsFreqParams{
    0,                                         /*u16FreqSampleCtr*/
    0,                                         /*u16LatchedFreqSampleCtr*/
    0,                                         /*fFrequency*/
    0,                                         /*fPrevFreq*/
    false                                      /*bIsPrevPositiveHalfCycle*/
},
#if (SUPPORT_CALIBRATION == YES)
/* RMS multiplication factor has been made '1' for the below parameters as scaling is taken care during adjustment of measurements due to calibration. */
_GensetVoltageCalc(_u16SamplesPerEntry, 1, true, 0, 0.02f),
_MainsVoltageCalc(_u16SamplesPerEntry, 1, true, 0, 0.5f),
_currentCalc(_u16SamplesPerEntry, 1, true, CURRENT_COMPUTATION_THRESHOLD_MW , 0.02f),
_powerCalc(_u16SamplesPerEntry, 1, false, POWER_COMPUTATION_THRESHOLD_MW),
#else
_GensetVoltageCalc(_u16SamplesPerEntry, ADC_SAMPLE_TO_V * V_ANLG_FRONTEND_UPSCALER, true, 0, 0.02f),
_MainsVoltageCalc(_u16SamplesPerEntry, ADC_SAMPLE_TO_V * V_ANLG_FRONTEND_UPSCALER, true, 0, 0.02f),
_currentCalc(_u16SamplesPerEntry, ADC_SAMPLE_TO_V * I_ANLG_FRONTEND_UPSCALER, true, CURRENT_COMPUTATION_THRESHOLD_MW,0.02f),
_powerCalc(_u16SamplesPerEntry, 
            ADC_SAMPLE_TO_V * ADC_SAMPLE_TO_V  * I_ANLG_FRONTEND_UPSCALER * V_ANLG_FRONTEND_UPSCALER,
                        false, POWER_COMPUTATION_THRESHOLD_MW),
#endif /* SUPPORT_CALIBRATION */
_pfSignParams{
    0,                                         /*i16DetectionCount*/
    1,                                         /*i16PFSign*/
    0                                          /*bIsPrevPositiveHalfCycle*/
},
_FreeRunningTimer{0}
{

}

void POWER::ConfigureISensing(I_CFG_t eICfg)
{
    _eICfg = eICfg;
}

void POWER::IsGenTamp(bool bIsGenTamp)
{
    _bIsGenTamp = bIsGenTamp;
}

/*Functions for power calculation*/
float POWER::GetGensetActivePower()
{
    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        return _fActivePower;
    }
    else
    {
        return 0;
    }
}

float POWER::GetGensetApparentPower()
{
    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        return _fApparentPower;
    }
    else
    {
        return 0;
    }
}

float POWER::GetGensetReactivePower()
{
    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        return _fReactivePower;
    }
    else
    {
        return 0;
    }
}


float POWER::GetMainsActivePower()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _fActivePower;
    }
    else
    {
        return 0;
    }
}

float POWER::GetMainsApparentPower()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _fApparentPower;
    }
    else
    {
        return 0;
    }
}

float POWER::GetMainsReactivePower()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _fReactivePower;
    }
    else
    {
        return 0;
    }
}

/*Function for frequency calculation*/
float POWER::GetGensetFrequency()
{
    if(_GensetVoltageCalc.GetFilteredRMS(RMS::VOLTAGE) < AC_SENSE_FREQ_CALC_VOLTS)
    {
        return 0;
    }
    return _GensetFreqParams.fFrequencyfilt;
}

float POWER::GetMainsFrequency()
{
    if(_MainsVoltageCalc.GetFilteredRMS(RMS::VOLTAGE) < AC_SENSE_FREQ_CALC_VOLTS)
    {
        return 0;
    }
    return _MainsFreqParams.fFrequencyfilt;
}


/*Function for energy calculation*/
double POWER::GetTamperedGensetActiveEnergy()
{
    if((_eICfg == GENSET_CURRENT_MEASUREMENT) && (_bIsGenTamp == true))
    {
        return _fActiveEnergy;
    }
    else
    {
        return 0;
    }
}
double POWER::GetGensetActiveEnergy()
{
    if((_eICfg == GENSET_CURRENT_MEASUREMENT) && (_bIsGenTamp == false))
    {
        return _fActiveEnergy;
    }
    else
    {
        return 0;
    }
}
double POWER::GetTamperedGensetApparentEnergy()
{
    if((_eICfg == GENSET_CURRENT_MEASUREMENT)&& (_bIsGenTamp == true))
    {
        return _fApparentEnergy;
    }
    else
    {
        return 0;
    }
}

double POWER::GetGensetApparentEnergy()
{
    if((_eICfg == GENSET_CURRENT_MEASUREMENT)&& (_bIsGenTamp == false))
    {
        return _fApparentEnergy;
    }
    else
    {
        return 0;
    }
}

double POWER::GetTamperedGensetReactiveEnergy()
{
    if((_eICfg == GENSET_CURRENT_MEASUREMENT)&& (_bIsGenTamp == true))
    {
        return _fReactiveEnergy;
    }
    else
    {
        return 0;
    }
}
double POWER::GetGensetReactiveEnergy()
{
    if((_eICfg == GENSET_CURRENT_MEASUREMENT)&& (_bIsGenTamp == false))
    {
        return _fReactiveEnergy;
    }
    else
    {
        return 0;
    }
}

double POWER::GetMainsActiveEnergy()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _fActiveEnergy;
    }
    else
    {
        return 0;
    }
}

double POWER::GetMainsApparentEnergy()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _fApparentEnergy;
    }
    else
    {
        return 0;
    }
}

double POWER::GetMainsReactiveEnergy()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _fReactiveEnergy;
    }
    else
    {
        return 0;
    }
}

/*Function for voltage calculation*/
float POWER::GetGensetVoltage()
{

    return _GensetVoltageCalc.GetFilteredRMS(RMS::VOLTAGE);

}

float POWER::GetDispGensetVoltage()
{
    return _GensetVoltageCalc.GetFilteredRMSValueForDisplay(RMS::VOLTAGE);
}

float POWER::GetGensetVoltageRaw()
{

    return _GensetVoltageCalc.GetRawRMS();

}
float POWER::GetGensetPollFrequency()
{
    return _GensetFreqParams.fFrequency;
}

float POWER::GetMainsVoltage()
{
    return _MainsVoltageCalc.GetFilteredRMS(RMS::VOLTAGE);
}

float POWER::GetDispMainsVoltage()
{
    return _MainsVoltageCalc.GetFilteredRMSValueForDisplay(RMS::VOLTAGE);
}

#if (SUPPORT_CALIBRATION == YES)
void POWER::prvCountFreqSamples(float f32FilteredVoltage, FREQ_VARS_t &FreqParams)
{
    FreqParams.u16FreqSampleCtr++;
    if(FreqParams.bIsPrevPositiveHalfCycle &&
            (f32FilteredVoltage < ZCD_LOWER_THRESHOLD_VOLTS))
    {
        FreqParams.u16LatchedFreqSampleCtr = FreqParams.u16FreqSampleCtr;
        FreqParams.u16FreqSampleCtr = 0;
        FreqParams.bIsPrevPositiveHalfCycle = false;
    }
    if(f32FilteredVoltage > ZCD_UPPER_THRESHOLD_VOLTS)
    {
        FreqParams.bIsPrevPositiveHalfCycle = true;
    }
}
#else
void POWER::prvCountFreqSamples(int16_t i16FilteredVoltageSample, FREQ_VARS_t &FreqParams)
{
    FreqParams.u16FreqSampleCtr++;
    if(FreqParams.bIsPrevPositiveHalfCycle &&
            (i16FilteredVoltageSample < ZCD_LOWER_THRESHOLD_SAMPLES))
    {
        FreqParams.u16LatchedFreqSampleCtr = FreqParams.u16FreqSampleCtr;
        FreqParams.u16FreqSampleCtr = 0;
        FreqParams.bIsPrevPositiveHalfCycle = false;
    }
    if(i16FilteredVoltageSample > ZCD_UPPER_THRESHOLD_SAMPLES)
    {
        FreqParams.bIsPrevPositiveHalfCycle = true;
    }
}
#endif /* SUPPORT_CALIBRATION */


#if (SUPPORT_CALIBRATION == YES)
void POWER::prvCalculatePowerFactorSign(float f32FilteredVoltage,
         float f32FilteredCurrent, PFSIGN_VARS_t &PfSignParams)
{
    /* Checks if a positive to negative zero cross has occurred*/
    if(PfSignParams.bIsPrevPositiveHalfCycle &&
                    (f32FilteredVoltage < ZCD_LOWER_THRESHOLD_VOLTS))
    {
        PfSignParams.bIsPrevPositiveHalfCycle = false;
        /* Increments counter if current sign is leading and we get a lagging
         * reading and vice versa */
        if((f32FilteredVoltage < f32FilteredCurrent &&
                (PfSignParams.i16PFSign < 0)) ||
            (f32FilteredVoltage > f32FilteredCurrent &&
                (PfSignParams.i16PFSign > 0)))
        {
            PfSignParams.i16DetectionCount++;
        }
        /* Resets counter if we do not get a successive reading of the same
         * type as the previous reading */
        else
        {
            PfSignParams.i16DetectionCount = 0;
        }
        /* If the number of successive same readings crosses a threshold
         * value the Power Factor sign is switched */
        if(PfSignParams.i16DetectionCount > SUCCESSIVE_SAMPLES_THRESHOLD)
        {
            PfSignParams.i16PFSign = -PfSignParams.i16PFSign;
            PfSignParams.i16DetectionCount = 0;
        }
    }
    if(f32FilteredVoltage > ZCD_UPPER_THRESHOLD_VOLTS)
    {
        PfSignParams.bIsPrevPositiveHalfCycle = true;
    }
}
#else
void POWER::prvCalculatePowerFactorSign(int16_t i16FilteredVoltageSample,
         int16_t i16FilteredCurrentSample, PFSIGN_VARS_t &PfSignParams)
{
    /* Checks if a positive to negative zero cross has occurred*/
    if(PfSignParams.bIsPrevPositiveHalfCycle &&
                    (i16FilteredVoltageSample < ZCD_LOWER_THRESHOLD_SAMPLES))
    {
        PfSignParams.bIsPrevPositiveHalfCycle = false;
        /* Increments counter if current sign is leading and we get a lagging
         * reading and vice versa */
        if((i16FilteredVoltageSample < i16FilteredCurrentSample &&
                (PfSignParams.i16PFSign < 0)) ||
            (i16FilteredVoltageSample > i16FilteredCurrentSample &&
                (PfSignParams.i16PFSign > 0)))
        {
            PfSignParams.i16DetectionCount++;
        }
        /* Resets counter if we do not get a successive reading of the same
         * type as the previous reading */
        else
        {
            PfSignParams.i16DetectionCount = 0;
        }
        /* If the number of successive same readings crosses a threshold
         * value the Power Factor sign is switched */
        if(PfSignParams.i16DetectionCount > SUCCESSIVE_SAMPLES_THRESHOLD)
        {
            PfSignParams.i16PFSign = (int16_t)(PfSignParams.i16PFSign*-1);
            PfSignParams.i16DetectionCount = 0;
        }   
    }
    if(i16FilteredVoltageSample > ZCD_UPPER_THRESHOLD_SAMPLES)
    {
        PfSignParams.bIsPrevPositiveHalfCycle = true;
    }
}
#endif /* SUPPORT_CALIBRATION */

volatile int32_t temp;
#if (SUPPORT_CALIBRATION == YES)
void  POWER::UpdateSample(float f32GensetPhaseVoltage,
        float f32MainsPhaseVoltage,
        float f32GensetNeutralVoltage,
        float f32MainsNeutralVoltage,
        float f32PhaseCurrent)
{
    _u16CurrentDCOffsetSampleCount++;
    /* Accumulate DC offset value */
    _f64CurrentOffsetAccumulator += f32PhaseCurrent;

    /* Check whether current offset fixed window is over*/
    if(_u16CurrentDCOffsetSampleCount >= _u16DCOffsetWindowSize)
    {
        _f32LatchedCurrentOffsetValue = (_f64CurrentOffsetAccumulator / _u16DCOffsetWindowSize);
        _f64CurrentOffsetAccumulator = 0;
        _u16CurrentDCOffsetSampleCount = 0;
        /* Change DC offset window size */
        _u16DCOffsetWindowSize = DC_OFFSET_WINDOW_SIZE_FOR_CURR;
    }

    /* Remove DC offset for voltages */
    float f32GensetPhToNVoltage = f32GensetPhaseVoltage - f32GensetNeutralVoltage;
    float f32MainsPhToNVoltage = f32MainsPhaseVoltage - f32MainsNeutralVoltage;

    /* Remove DC offset for phase current */
    float f32AbsolutePhaseCurrent = f32PhaseCurrent - _f32LatchedCurrentOffsetValue;

    prvCountFreqSamples(f32GensetPhToNVoltage, _GensetFreqParams);
    prvCountFreqSamples(f32MainsPhToNVoltage, _MainsFreqParams);

    _GensetVoltageCalc.AccumulateSampleSet(f32GensetPhToNVoltage, f32GensetPhToNVoltage);
    _MainsVoltageCalc.AccumulateSampleSet(f32MainsPhToNVoltage, f32MainsPhToNVoltage);
    _currentCalc.AccumulateSampleSet(f32AbsolutePhaseCurrent, f32AbsolutePhaseCurrent);

    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        _bWindowMoved = _powerCalc.AccumulateSampleSet(f32GensetPhToNVoltage, f32AbsolutePhaseCurrent);
        prvCalculatePowerFactorSign(f32GensetPhToNVoltage,
                f32AbsolutePhaseCurrent, _pfSignParams);
    }
    else
    {
        _bWindowMoved = _powerCalc.AccumulateSampleSet(f32MainsPhToNVoltage, f32AbsolutePhaseCurrent);
        prvCalculatePowerFactorSign(f32MainsPhToNVoltage,
                f32AbsolutePhaseCurrent, _pfSignParams);
    }

    if(!_bWindowMovedResultLatched)
    {
        _bWindowMovedResultLatched = _bWindowMoved;
    }
}

#else
void  POWER::UpdateSample(uint16_t u16GensetPhaseVoltageSample,
                    uint16_t u16MainsPhaseVoltageSample,
                                    uint16_t u16GensetNeutralVoltageSample,
                            uint16_t u16MainsNeutralVoltageSample,
                                            uint16_t u16PhaseCurrentSample)
{
    _u16CurrentDCOffsetSampleCount++;
    /* Accumulate DC offset value*/
    _u32CurrentOffsetAccumulator += u16PhaseCurrentSample;

    /* Check weather current offset fixed window is over*/
    if(_u16CurrentDCOffsetSampleCount >= _u16DCOffsetWindowSize)
    {
        _u16LatchedCurrentOffsetValue =
                        (uint16_t)(_u32CurrentOffsetAccumulator/_u16DCOffsetWindowSize);
        _u32CurrentOffsetAccumulator = 0;
        _u16CurrentDCOffsetSampleCount = 0;
        /*Change DC offset window size*/
        _u16DCOffsetWindowSize = DC_OFFSET_WINDOW_SIZE;
    }

    temp =  u16GensetPhaseVoltageSample;
    /*Remove DC offset for voltage sample*/
    int16_t i16GensetPhToNSample = (int16_t)((int16_t)u16GensetPhaseVoltageSample-
                                               (int16_t)u16GensetNeutralVoltageSample);
    int16_t i16MainsPhToNSample = (int16_t)((int16_t)u16MainsPhaseVoltageSample-
                                            (int16_t)u16MainsNeutralVoltageSample);


    /*Remove DC offset for current sample*/
    int16_t i16AbsoluteCurrentSample = (int16_t)(u16PhaseCurrentSample
                                            - _u16LatchedCurrentOffsetValue);

    prvCountFreqSamples(i16GensetPhToNSample, _GensetFreqParams);
    prvCountFreqSamples(i16MainsPhToNSample, _MainsFreqParams);


    _GensetVoltageCalc.AccumulateSampleSet(i16GensetPhToNSample, i16GensetPhToNSample);
    _MainsVoltageCalc.AccumulateSampleSet(i16MainsPhToNSample, i16MainsPhToNSample);
    _currentCalc.AccumulateSampleSet(i16AbsoluteCurrentSample, i16AbsoluteCurrentSample);

    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        _bWindowMoved = _powerCalc.AccumulateSampleSet(i16GensetPhToNSample, i16AbsoluteCurrentSample);
        prvCalculatePowerFactorSign(i16GensetPhToNSample,
                            i16AbsoluteCurrentSample, _pfSignParams);
    }
    else
    {
        _bWindowMoved = _powerCalc.AccumulateSampleSet(i16MainsPhToNSample, i16AbsoluteCurrentSample);
        prvCalculatePowerFactorSign(i16MainsPhToNSample,
                            i16AbsoluteCurrentSample, _pfSignParams);
    }

}
#endif /* SUPPORT_CALIBRATION */

void POWER::Update()
{
    _GensetVoltageCalc.Update();
    _MainsVoltageCalc.Update();
    _currentCalc.Update();
    _powerCalc.Update();
/*Compute RMS if moving average window has been moved*/
#if (SUPPORT_CALIBRATION == YES)
    if(_bWindowMovedResultLatched)
#else
    if(_bWindowMoved)
#endif /* SUPPORT_CALIBRATION */
    {
        prvComputeActivePower();
        prvComputeApparentPower();
        prvComputeReactivePower();
        prvComputePowerFactor();
        prvComputeEnergies();
#if (SUPPORT_CALIBRATION == YES)
        _bWindowMovedResultLatched = false;
#else
        _bWindowMoved = false;
#endif /* SUPPORT_CALIBRATION */
    }
    prvComputeFrequency(_GensetFreqParams);
    prvComputeFrequency(_MainsFreqParams);
}
float POWER::GetGensetCurrent()
{
    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        return _currentCalc.GetFilteredRMS(RMS::CURRENT);
    }
    else
    {
        return 0;
    }
}

float POWER::GetGensetRawCurrent()
{
    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        return _currentCalc.GetRawRMS();
    }
    else
    {
        return 0;
    }
}


float POWER::GetMainsCurrent()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _currentCalc.GetFilteredRMS(RMS::CURRENT);
    }
    else
    {
        return 0;
    }
}

/*Power factor calculation*/
float POWER::GetGensetPowerFactor()
{
    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        return _fPowerFactor * _pfSignParams.i16PFSign;
    }
    else
    {
        return 0;
    }
}

float POWER::GetMainsPowerFactor()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _fPowerFactor * _pfSignParams.i16PFSign;
    }
    else
    {
        return 0;
    }
}

float POWER::GetFiltGensetPowerFactor()
{
    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        return _fFiltPowerFactor* _pfSignParams.i16PFSign;
    }
    else
    {
        return 0;
    }
}
float POWER::GetFiltGensetActivePower()
{
    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        return _fFiltActivePower;
    }
    else
    {
        return 0;
    }
}
float POWER::GetFiltGensetReactivePower()
{
    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        return _fFiltReactivePower;
    }
    else
    {
        return 0;
    }
}
float POWER::GetFiltGensetApparentPower()
{
    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        return _fFiltApparentPower;
    }
    else
    {
        return 0;
    }
}

float POWER::GetFiltMainsPowerFactor()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _fFiltPowerFactor* _pfSignParams.i16PFSign;
    }
    else
    {
        return 0;
    }
}
float POWER::GetFiltMainsActivePower()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _fFiltActivePower;
    }
    else
    {
        return 0;
    }
}
float POWER::GetFiltMainsReactivePower()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _fFiltReactivePower;
    }
    else
    {
        return 0;
    }
}
float POWER::GetFiltMainsApparentPower()
{
    if(_eICfg == MAINS_CURRENT_MEASUREMENT)
    {
        return _fFiltApparentPower;
    }
    else
    {
        return 0;
    }
}

void POWER::prvComputeActivePower()
{
    _fActivePower = _powerCalc.GetRawRMS();
    _fFiltActivePower = (1.0f - DISP_FILTER_FOR_POWER)*_fFiltActivePower + DISP_FILTER_FOR_POWER*_fActivePower;
}

void POWER::prvComputeApparentPower()
{
    if(_eICfg == GENSET_CURRENT_MEASUREMENT)
    {
        _fApparentPower = _GensetVoltageCalc.GetRawRMS() * _currentCalc.GetRawRMS();
    }
    else
    {
        _fApparentPower = _MainsVoltageCalc.GetRawRMS() * _currentCalc.GetRawRMS();
    }
    _fFiltApparentPower = (1.0f - DISP_FILTER_FOR_POWER)*_fFiltApparentPower + DISP_FILTER_FOR_POWER*_fApparentPower;
}

void POWER::prvComputeReactivePower()
{
    _fReactivePower = sqrt(_fApparentPower*_fApparentPower - _fActivePower*
                                       _fActivePower);
    /* The square root could become NaN in case apparent power is less than active
       power. This could happen in no load conditions where slight power is computed
       from the noise floor. This check ensures that the NaN does not propagate to
       other parts of the code.
     */
    if(isnan(_fReactivePower) == true)
    {
        _fReactivePower = 0;
        _fFiltReactivePower = 0;
    }
    _fFiltReactivePower = (1.0f - DISP_FILTER_FOR_POWER)*_fFiltReactivePower + DISP_FILTER_FOR_POWER*_fReactivePower;
}

void POWER::prvComputePowerFactor()
{
   _fPowerFactor = (_fActivePower/_fApparentPower);
   /*_fPowerFactor could be "not a number" or infinity due to division by zero.
     This converts the values to 0 in both cases*/
   if((isnan(_fPowerFactor) == true) || (isinf(_fPowerFactor) == true))
   {
        _fPowerFactor = 0;
        _fFiltPowerFactor = 0;
   }
   if(_fPowerFactor > 1.0f)
   {
       _fPowerFactor = 1;
   }
   _fFiltPowerFactor = (1.0f - DISP_FILTER_FOR_POWER)*_fFiltPowerFactor + DISP_FILTER_FOR_POWER*_fPowerFactor;

}

void POWER::prvFrequencyFilter(FREQ_VARS_t &FreqParams)
{
    /* IIR filter formula
       Y[n] = (FREQ_FILTER_MULTIPLIER*X[n]) + (1-FREQ_FILTER_MULTIPLIER)*Y[n-1]
     */
    FreqParams.fFrequencyfilt = (FREQ_FILTER_MULTIPLIER * FreqParams.fFrequency)
                            + ((1 - FREQ_FILTER_MULTIPLIER) * FreqParams.fFrequencyfilt);
}

void POWER::prvComputeFrequency(FREQ_VARS_t &FreqParams)
{
    if(FreqParams.u16LatchedFreqSampleCtr != 0)
    {
        float fTimePeriod = FreqParams.u16LatchedFreqSampleCtr *
                                                    SAMPLE_UPDATE_TIME_PERIOD_S;

        if(fTimePeriod > TIME_PERIOD_80HZ) // 80hz
        {
            FreqParams.fFrequency = 1/fTimePeriod;

            prvFrequencyFilter(FreqParams);
        }
        FreqParams.u16LatchedFreqSampleCtr = 0;

    }
}

void POWER::prvComputeEnergies()
{
    if(_FreeRunningTimer.bEnabled == false)
    {
        UTILS_ResetTimer(&_FreeRunningTimer);
    }
    else
    {
        /*Energy is computed by multiplying the respective power with the time it
          takes for the window to move*/
        uint64_t u64ElapsTime = UTILS_GetElapsedTimeInMs(&_FreeRunningTimer);
        UTILS_ResetTimer(&_FreeRunningTimer);
        _fActiveEnergy   += (_fActivePower   * (uint16_t)u64ElapsTime )/MILLI_SECONDS_IN_HOUR;
        _fApparentEnergy += (_fApparentPower * (uint16_t)u64ElapsTime )/MILLI_SECONDS_IN_HOUR;
        _fReactiveEnergy += (_fReactivePower * (uint16_t)u64ElapsTime )/MILLI_SECONDS_IN_HOUR;
    }
}

void POWER::ClearCumulativeEnergy()
{
    _fActiveEnergy = 0;
    _fApparentEnergy = 0;
    _fReactiveEnergy = 0;
}
