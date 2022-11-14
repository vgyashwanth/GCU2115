/**
 * @file        [POWER]
 * @brief       This module calls functions from POWER.h returns AC Parameters that
 *              calculate the parameter values. The input parameters for this module are:
 *              1. Mains voltage sample
 *              2. Genset voltage sample
 *              3. CT Current sample - This could either correspond to mains
 *                 current or Genset current based on CT configuration.
 *              The o/p parameters are RMS voltages, current, frequencies,
 *              active power, apparent power, reactive power, power factor,
 *              active energy, apparent energy & reactive energy.
 *              ADC sample clipping
 *              If the ADC sample values are higher than the max sample value
 *              then they get clipped to the max sample value and if it is lower
 *              than the min sample value it get set to the min sample value.
 * @version     [Version number]
 * @author      Naveen Ramaswamy, Sudeep Chandrasekaran
 * @date        7th May 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/



#include "AC_SENSE.h"
#include "A_SENSE.h"
/*Number of consecutive samples to determine existence of phase reversal*/
#define PHASE_REVERSAL_SAMPLE_CNT (10)

static AC_SENSE *pACSense = NULL;

/* This function is a workaround that allows to pass the sample values to
   UpdateACData function in AC_SENSE class. The UpdateACData function cannot
   be directly registered to AC_IP because the register cb function of AC_IP
   expects a static function,and UpdateACData is not a static function.
 */
static void StaticUpdateACData(AC_IP::AC_PARAMS_t *sample);

AC_SENSE::AC_SENSE(AC_IP &AcIp,  ANLG_IP &anlgIp):
_aPowers{(SAMPLES_PER_ENTRY_W_BUFFER), (SAMPLES_PER_ENTRY_W_BUFFER), (SAMPLES_PER_ENTRY_W_BUFFER)},
_tampGensetEnergyOffset{
    0,
    0,
    0
},
_gensetEnergyOffset{
    0,
    0,
    0
},
_mainsEnergyOffset{
    0,
    0,
    0
},
_gensetPhaseRot{
    false,
    false,
    0
},
_mainsPhaseRot{
    false,
    false,
    0
},
_ryMainsPhaseVoltage(SAMPLES_PER_ENTRY_W_BUFFER,  ADC_SAMPLE_TO_V * V_ANLG_FRONTEND_UPSCALER, true, 0, 0.02f),
_ybMainsPhaseVoltage(SAMPLES_PER_ENTRY_W_BUFFER,  ADC_SAMPLE_TO_V * V_ANLG_FRONTEND_UPSCALER, true, 0, 0.02f),
_rbMainsPhaseVoltage(SAMPLES_PER_ENTRY_W_BUFFER,  ADC_SAMPLE_TO_V * V_ANLG_FRONTEND_UPSCALER, true, 0, 0.02f),

_ryGensetPhaseVoltage(SAMPLES_PER_ENTRY_W_BUFFER,  ADC_SAMPLE_TO_V * V_ANLG_FRONTEND_UPSCALER, true, 0, 0.02f),
_ybGensetPhaseVoltage(SAMPLES_PER_ENTRY_W_BUFFER,  ADC_SAMPLE_TO_V * V_ANLG_FRONTEND_UPSCALER, true, 0, 0.02f),
_rbGensetPhaseVoltage(SAMPLES_PER_ENTRY_W_BUFFER,  ADC_SAMPLE_TO_V * V_ANLG_FRONTEND_UPSCALER, true, 0, 0.02f),
_EarthCurrent(SAMPLES_PER_ENTRY_W_BUFFER,  ADC_SAMPLE_TO_V * I_ANLG_FRONTEND_UPSCALER, true, CURRENT_COMPUTATION_THRESHOLD_MW),

_AcIp(AcIp),
_Perform3phCalculationEnableFor1Ph(false),
_fCTMultiplier(1),
_fECTMultiplier(1),
_fGenPTMultiplier(1),
_fMainsPTMultiplier(1),
_fGenRating(0),
_AnlgIp(anlgIp),
_st200ms{0},
_f32DCOffserFiltV(0.0),
_eMainsSystemType(PHASE_3_SYSTEM),
_ePrvGenSysType(PHASE_3_SYSTEM),
_u16EarthCurrentDCOffsetSampleCount(0),
_u16LatchedECurrentOffsetValue(DC_VOLTAGE_OFFSET*V_TO_ADC_SAMPLE),
_u32ECurrentOffsetAccumulator(0),
_u16EarthCurrentDCOffsetWindowSize(INIT_DC_OFFSET_WINDOW_SIZE)
{
    AcIp.RegisterACParamsCB(StaticUpdateACData);
    pACSense = this;
	UTILS_ResetTimer(&_st200ms);
}

void AC_SENSE::Update()
{
    ANLG_IP::ANALOG_VAL_t  stDC_Offset;


    _aPowers[R_PHASE].Update();
    _aPowers[Y_PHASE].Update();
    _aPowers[B_PHASE].Update();

    _ryMainsPhaseVoltage.Update();
    _ybMainsPhaseVoltage.Update();
    _rbMainsPhaseVoltage.Update();

    _ryGensetPhaseVoltage.Update();
    _ybGensetPhaseVoltage.Update();
    _rbGensetPhaseVoltage.Update();

    _EarthCurrent.Update();
    if(UTILS_GetElapsedTimeInMs(&_st200ms) > 200U)
    {
        stDC_Offset = _AnlgIp.GetDCOffset_V();
        _f32DCOffserFiltV = (VBAT_FILTER_CONST * stDC_Offset.f32InstSensorVal)
                + (_f32DCOffserFiltV*(1.0F- VBAT_FILTER_CONST) );
        UTILS_ResetTimer(&_st200ms);
    }

}

void AC_SENSE::ConfigureCTRatio(uint16_t u16CTFactor, uint16_t u16ECTFactor, float fCTCorrfactor)
{
    _fCTMultiplier = (float)u16CTFactor/(float)MAX_CT_SECONDARY_CURRENT_A;
    _fCTMultiplier = _fCTMultiplier*fCTCorrfactor;
    _fECTMultiplier =  (float)u16ECTFactor/(float)MAX_CT_SECONDARY_CURRENT_A;
}
void AC_SENSE::ConfigureCTRatio(uint16_t u16CTFactor, uint16_t u16ECTFactor)
{
    _fCTMultiplier = (float)u16CTFactor/(float)MAX_CT_SECONDARY_CURRENT_A;
    _fECTMultiplier =  (float)u16ECTFactor/(float)MAX_CT_SECONDARY_CURRENT_A;
}
void AC_SENSE::ConfigureGenPTRatio(uint16_t u16GenPTPrimaryFactor,uint16_t u16GenPTSecondaryFactor)
{
    _fGenPTMultiplier = (float)u16GenPTPrimaryFactor/(float)u16GenPTSecondaryFactor;

}
void AC_SENSE::ConfigureMainsPTRatio(uint16_t u16MainsPTPrimaryFactor,uint16_t u16MainsPTSecondaryFactor)
{
    _fMainsPTMultiplier =  (float)u16MainsPTPrimaryFactor/(float)u16MainsPTSecondaryFactor;
}


void AC_SENSE::ConfigureACSystem(AC_SYSTEM_TYP_t eGenSystemType, AC_SYSTEM_TYP_t eMainsSystemType)
{
    _eGenSystemType = eGenSystemType;
    _eMainsSystemType = eMainsSystemType;

    if(_ePrvGenSysType != _eGenSystemType)
    {
        if(_ePrvGenSysType == PHASE_1_SYSTEM)
        {
            _gensetEnergyOffset.f32InitialActiveEnergyWH += (float)( _fCTMultiplier*_fGenPTMultiplier*_aPowers[R_PHASE].GetGensetActiveEnergy());
            _gensetEnergyOffset.f32InitialApparentEnergyVA += (float)(_fCTMultiplier*_fGenPTMultiplier*_aPowers[R_PHASE].GetGensetApparentEnergy());
            _gensetEnergyOffset.f32InitialReactiveEnergyVAR += (float)( _fCTMultiplier*_fGenPTMultiplier*_aPowers[R_PHASE].GetGensetReactiveEnergy());
        }
        else if(_ePrvGenSysType == SPLIT_PHASE_SYSTEM)
        {
            _gensetEnergyOffset.f32InitialActiveEnergyWH += (float)(_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetActiveEnergy() +
                                               _aPowers[Y_PHASE].GetGensetActiveEnergy() ));

            _gensetEnergyOffset.f32InitialApparentEnergyVA += (float)(_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetApparentEnergy() +
                                              _aPowers[Y_PHASE].GetGensetApparentEnergy()));

            _gensetEnergyOffset.f32InitialReactiveEnergyVAR += (float)( _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetReactiveEnergy() +
                                               _aPowers[Y_PHASE].GetGensetReactiveEnergy() ));
        }
        else
        {
            _gensetEnergyOffset.f32InitialActiveEnergyWH += (float)(_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetActiveEnergy() +
                                       _aPowers[Y_PHASE].GetGensetActiveEnergy() +
                                           _aPowers[B_PHASE].GetGensetActiveEnergy()));

            _gensetEnergyOffset.f32InitialApparentEnergyVA += (float)(_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetApparentEnergy() +
                                       _aPowers[Y_PHASE].GetGensetApparentEnergy() +
                                           _aPowers[B_PHASE].GetGensetApparentEnergy()));

            _gensetEnergyOffset.f32InitialReactiveEnergyVAR += (float)(_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetReactiveEnergy() +
                                       _aPowers[Y_PHASE].GetGensetReactiveEnergy() +
                                           _aPowers[B_PHASE].GetGensetReactiveEnergy()));
        }

       _aPowers[R_PHASE].ClearCumulativeEnergy();
       _aPowers[Y_PHASE].ClearCumulativeEnergy();
       _aPowers[B_PHASE].ClearCumulativeEnergy();

      _ePrvGenSysType = _eGenSystemType;
    }
}

void AC_SENSE::ConfigureISensing(I_CFG_t iCfg)
{
    _aPowers[R_PHASE].ConfigureISensing(iCfg);
    _aPowers[Y_PHASE].ConfigureISensing(iCfg);
    _aPowers[B_PHASE].ConfigureISensing(iCfg);
}

void AC_SENSE::ConfigureGenTamp(bool bIsGenTamp)
{
    _aPowers[R_PHASE].IsGenTamp(bIsGenTamp);
    _aPowers[Y_PHASE].IsGenTamp(bIsGenTamp);
    _aPowers[B_PHASE].IsGenTamp(bIsGenTamp);
}
void AC_SENSE::ConfigGeneratorRating(float f32GenRating)
{
    _fGenRating = f32GenRating;
}
void AC_SENSE::SetEnergyOffsets(ENERGY_REGISTER_t &stTampGensetEnergyOffset,ENERGY_REGISTER_t &stGensetEnergyOffset,
                                    ENERGY_REGISTER_t &stMainsEnergyOffset)
{
    _tampGensetEnergyOffset = stTampGensetEnergyOffset;
    _gensetEnergyOffset = stGensetEnergyOffset;
    _mainsEnergyOffset  = stMainsEnergyOffset;
    _aPowers[R_PHASE].ClearCumulativeEnergy();
    _aPowers[Y_PHASE].ClearCumulativeEnergy();
    _aPowers[B_PHASE].ClearCumulativeEnergy();
}

//void AC_SENSE::SetEnergyOffsets(ENERGY_REGISTER_t &stGensetEnergyOffset,
//                                    ENERGY_REGISTER_t &stMainsEnergyOffset)
//{
//    _gensetEnergyOffset = stGensetEnergyOffset;
//    _mainsEnergyOffset  = stMainsEnergyOffset;
//    _aPowers[R_PHASE].ClearCumulativeEnergy();
//    _aPowers[Y_PHASE].ClearCumulativeEnergy();
//    _aPowers[B_PHASE].ClearCumulativeEnergy();
//}

void AC_SENSE::Configure3phCalculationEnableFor1Ph(uint8_t CalculationOf3PhFor1ph)
{
    if(CalculationOf3PhFor1ph == 1)
    {
        _Perform3phCalculationEnableFor1Ph = true;
    }
    else
    {
        _Perform3phCalculationEnableFor1Ph = false;
    }
}

bool AC_SENSE::prvIsPhaseAvilableInSelectedACSys(PHASE_t Phase, AC_SYSTEM_TYP_t etype)
{
    if((etype==PHASE_1_SYSTEM) && (Phase != R_PHASE) && !_Perform3phCalculationEnableFor1Ph)
    {
       return false;
    }
    else if((etype==SPLIT_PHASE_SYSTEM) && (Phase == B_PHASE))
    {
       return false;
    }

    return true;
}

bool AC_SENSE::prvIsACSyte1Phaseor1Phase3Wire(AC_SYSTEM_TYP_t etype)
{
    if((etype==PHASE_1_SYSTEM) || (etype==SPLIT_PHASE_SYSTEM))
    {
        return true;
    }
    return false;
}

float AC_SENSE::GENSET_GetDispVoltageVolts(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fGenPTMultiplier*_aPowers[Phase].GetDispGensetVoltage();
    }
    return 0;
}
/// Measuring Genset voltage
float AC_SENSE::GENSET_GetVoltageVolts(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fGenPTMultiplier*_aPowers[Phase].GetGensetVoltage();
    }
    return 0;
}


float AC_SENSE::GENSET_GetVoltageVoltsRaw(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _aPowers[Phase].GetGensetVoltageRaw();
    }
    return 0;
}

float AC_SENSE::GENSET_GetPercentPower()
{
     if(_fGenRating> 0.0f)
     {
         return (((GENSET_GetTotalActivePowerWatts()/1000.0f)/_fGenRating)*100.0f);
     }
     return 0;
}
float AC_SENSE::GENSET_GetDispPercentPower()
{
    if(_fGenRating> 0.0f)
    {
        return (((GENSET_GetTotalFiltActivePowerWatts()/1000.0f)/_fGenRating)*100.0f);
    }
    return 0;
}

///Measuring Genset current
float AC_SENSE::GENSET_GetCurrentAmps(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fCTMultiplier*_aPowers[Phase].GetGensetCurrent();
    }
    return 0;
}

float AC_SENSE::GENSET_GetApproxFreq(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
          return _aPowers[Phase].GetGensetFrequency();
    }
    return 0;
}

/// Measuring Genset power
//Active Power
float AC_SENSE::GENSET_GetTotalActivePowerWatts()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetActivePower());
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetActivePower()
                + _aPowers[Y_PHASE].GetGensetActivePower() );
    }
    return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetActivePower()
                                  + _aPowers[Y_PHASE].GetGensetActivePower() +
                                        _aPowers[B_PHASE].GetGensetActivePower());
}

float AC_SENSE::GENSET_GetTotalFiltActivePowerWatts()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetFiltGensetActivePower());
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetFiltGensetActivePower()
                + _aPowers[Y_PHASE].GetFiltGensetActivePower() );
    }
    return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetFiltGensetActivePower()
                                  + _aPowers[Y_PHASE].GetFiltGensetActivePower() +
                                        _aPowers[B_PHASE].GetFiltGensetActivePower());
}

float AC_SENSE::GENSET_GetActivePowerWatts(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fCTMultiplier*_fGenPTMultiplier*_aPowers[Phase].GetGensetActivePower();
    }
    return 0;
}

float AC_SENSE::GENSET_GetDispActivePowerWatts(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fCTMultiplier*_fGenPTMultiplier*_aPowers[Phase].GetFiltGensetActivePower();
    }
    return 0;
}
//Apparent power
float AC_SENSE::GENSET_GetTotalApparentPowerVA()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetApparentPower());
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*
                (_aPowers[R_PHASE].GetGensetApparentPower() +
                   _aPowers[Y_PHASE].GetGensetApparentPower() );
    }
    return _fCTMultiplier*_fGenPTMultiplier*
                (_aPowers[R_PHASE].GetGensetApparentPower() +
                           _aPowers[Y_PHASE].GetGensetApparentPower() +
                                    _aPowers[B_PHASE].GetGensetApparentPower());
}

float AC_SENSE::GENSET_GetTotalFiltApparentPowerVA()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetFiltGensetApparentPower());
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*
                (_aPowers[R_PHASE].GetFiltGensetApparentPower() +
                   _aPowers[Y_PHASE].GetFiltGensetApparentPower() );
    }
    return _fCTMultiplier*_fGenPTMultiplier*
                (_aPowers[R_PHASE].GetFiltGensetApparentPower() +
                           _aPowers[Y_PHASE].GetFiltGensetApparentPower() +
                                    _aPowers[B_PHASE].GetFiltGensetApparentPower());
}

float AC_SENSE::GENSET_GetApparentPowerVA(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fCTMultiplier*_fGenPTMultiplier*_aPowers[Phase].GetGensetApparentPower();
    }
    return 0;
}

float AC_SENSE::GENSET_GetDispApparentPowerVA(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fCTMultiplier*_fGenPTMultiplier*_aPowers[Phase].GetFiltGensetApparentPower();
    }
    return 0;
}

//Reactive power
float AC_SENSE::GENSET_GetTotalReactivePowerVAR()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetReactivePower());
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetReactivePower() +
                                _aPowers[Y_PHASE].GetGensetReactivePower() );
    }

    return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetReactivePower() +
                                _aPowers[Y_PHASE].GetGensetReactivePower() +
                                    _aPowers[B_PHASE].GetGensetReactivePower());
}

float AC_SENSE::GENSET_GetTotalFiltReactivePowerVAR()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetFiltGensetReactivePower());
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetFiltGensetReactivePower() +
                                _aPowers[Y_PHASE].GetFiltGensetReactivePower() );
    }

    return _fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetFiltGensetReactivePower() +
                                _aPowers[Y_PHASE].GetFiltGensetReactivePower() +
                                    _aPowers[B_PHASE].GetFiltGensetReactivePower());
}


float AC_SENSE::GENSET_GetReactivePowerVAR(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fCTMultiplier*_fGenPTMultiplier*_aPowers[Phase].GetGensetReactivePower();
    }
    return 0;
}

float AC_SENSE::GENSET_GetDispReactivePowerVAR(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fCTMultiplier*_fGenPTMultiplier*_aPowers[Phase].GetFiltGensetReactivePower();
    }
    return 0;
}

//Power factor
float AC_SENSE::GENSET_GetPowerFactor(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _aPowers[Phase].GetGensetPowerFactor();
    }
    return 0;
}

float AC_SENSE::GENSET_GetDispPowerFactor(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _aPowers[Phase].GetFiltGensetPowerFactor();
    }
    return 0;
}


float AC_SENSE::GENSET_GetDispAveragePowerFactor()
{
    float f32PF;
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return _aPowers[R_PHASE].GetFiltGensetPowerFactor();
    }
    //Instead of calculating average of all power factor.
    //following calculation will give accurate result

    //Soujanya Mulik 13.08.22
    // GENSET_GetTotalFiltApparentPowerVA() checked for greater than 1.0W and GENSET_GetTotalFiltActivePowerWatts checked for greater than 15W
    //since after power on or after unlatching the contactor ,the filtered apparent power as well as filtered active power were
    // taking considerable amount of time(4-5s) to reach 0 . the numerator was getting some non zero value
    // and the denominator was very less which was resulting in some huge value as PF.
    if(( GENSET_GetTotalFiltApparentPowerVA() > 1.0)&& (GENSET_GetTotalFiltActivePowerWatts() > 15.0)) //check for divide by zero error
    {
        f32PF = GENSET_GetTotalFiltActivePowerWatts() / GENSET_GetTotalFiltApparentPowerVA();
    }
    else
    {
        return 0;
    }

    if((isnan(f32PF) == true) || (isinf(f32PF) == true))
    {
        return 0;
    }
    else if(f32PF > 1.0)
    {
        return 1;
    }
    else
    {
        return (f32PF);
    }
}



float AC_SENSE::GENSET_GetAveragePowerFactor()
{
    float f32PF;
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return _aPowers[R_PHASE].GetGensetPowerFactor();
    }
    //Instead of calculating average of all power factor.
    //following calculation will give accurate result
    if( GENSET_GetTotalApparentPowerVA() > 0.0) //check for divide by zero error
    {
        f32PF = GENSET_GetTotalActivePowerWatts() / GENSET_GetTotalApparentPowerVA();
    }
    else
    {
        return 0;
    }

    if((isnan(f32PF) == true) || (isinf(f32PF) == true))
    {
        return 0;
    }
    else
    {
        return (f32PF);
    }
}

double AC_SENSE::GENSET_GetTotalTamperedActiveEnergySinceInitWH()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetTamperedGensetActiveEnergy())) + _tampGensetEnergyOffset.f32InitialActiveEnergyWH);
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetTamperedGensetActiveEnergy() +
                                    _aPowers[Y_PHASE].GetTamperedGensetActiveEnergy())) +
                _tampGensetEnergyOffset.f32InitialActiveEnergyWH);
    }

    return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetTamperedGensetActiveEnergy() +
                            _aPowers[Y_PHASE].GetTamperedGensetActiveEnergy() +
                               _aPowers[B_PHASE].GetTamperedGensetActiveEnergy())) +
            _tampGensetEnergyOffset.f32InitialActiveEnergyWH);
}

/// Genset energy since init
//Active energy
double AC_SENSE::GENSET_GetTotalActiveEnergySinceInitWH()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetActiveEnergy())) + _gensetEnergyOffset.f32InitialActiveEnergyWH);
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetActiveEnergy() +
                                    _aPowers[Y_PHASE].GetGensetActiveEnergy())) +
                                            _gensetEnergyOffset.f32InitialActiveEnergyWH);
    }

    return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetActiveEnergy() +
                            _aPowers[Y_PHASE].GetGensetActiveEnergy() +
                               _aPowers[B_PHASE].GetGensetActiveEnergy())) +
                                    _gensetEnergyOffset.f32InitialActiveEnergyWH);
}

double AC_SENSE::GENSET_GetActiveEnergyWH(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fCTMultiplier*_fGenPTMultiplier*_aPowers[Phase].GetGensetActiveEnergy();
    }
    return 0;
}

double AC_SENSE::GENSET_GetTotalTamperedApparentEnergySinceInitVAH()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetTamperedGensetApparentEnergy())) + _tampGensetEnergyOffset.f32InitialApparentEnergyVA);
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetTamperedGensetApparentEnergy() +
                           _aPowers[Y_PHASE].GetTamperedGensetApparentEnergy() )) +
                _tampGensetEnergyOffset.f32InitialApparentEnergyVA);
    }
    return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetTamperedGensetApparentEnergy() +
                            _aPowers[Y_PHASE].GetTamperedGensetApparentEnergy() +
                              _aPowers[B_PHASE].GetTamperedGensetApparentEnergy())) +
            _tampGensetEnergyOffset.f32InitialApparentEnergyVA);
}

//Apparent energy
double AC_SENSE::GENSET_GetTotalApparentEnergySinceInitVAH()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetApparentEnergy())) + _gensetEnergyOffset.f32InitialApparentEnergyVA);
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetApparentEnergy() +
                           _aPowers[Y_PHASE].GetGensetApparentEnergy() )) +
                                _gensetEnergyOffset.f32InitialApparentEnergyVA);
    }
    return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetApparentEnergy() +
                            _aPowers[Y_PHASE].GetGensetApparentEnergy() +
                              _aPowers[B_PHASE].GetGensetApparentEnergy())) +
                                 _gensetEnergyOffset.f32InitialApparentEnergyVA);
}

double AC_SENSE::GENSET_GetApparentEnergyVAH(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fCTMultiplier*_fGenPTMultiplier*_aPowers[Phase].GetGensetApparentEnergy();
    }
    return 0;
}

double AC_SENSE::GENSET_GetTotalTamperedReactiveEnergySinceInitVARH()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetTamperedGensetReactiveEnergy())) + _tampGensetEnergyOffset.f32InitialReactiveEnergyVAR);
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetTamperedGensetReactiveEnergy() +
                               _aPowers[Y_PHASE].GetTamperedGensetReactiveEnergy() )) +
                _tampGensetEnergyOffset.f32InitialReactiveEnergyVAR);
    }
    return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetTamperedGensetReactiveEnergy() +
                             _aPowers[Y_PHASE].GetTamperedGensetReactiveEnergy() +
                             _aPowers[B_PHASE].GetTamperedGensetReactiveEnergy())) +
            _tampGensetEnergyOffset.f32InitialReactiveEnergyVAR);
}
//Reactive energy
double AC_SENSE::GENSET_GetTotalReactiveEnergySinceInitVARH()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetReactiveEnergy())) + _gensetEnergyOffset.f32InitialReactiveEnergyVAR);
    }
    else if(_eGenSystemType==SPLIT_PHASE_SYSTEM)
    {
        return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetReactiveEnergy() +
                               _aPowers[Y_PHASE].GetGensetReactiveEnergy() )) +
                               _gensetEnergyOffset.f32InitialReactiveEnergyVAR);
    }
    return ((_fCTMultiplier*_fGenPTMultiplier*(_aPowers[R_PHASE].GetGensetReactiveEnergy() +
                             _aPowers[Y_PHASE].GetGensetReactiveEnergy() +
                             _aPowers[B_PHASE].GetGensetReactiveEnergy())) +
                       _gensetEnergyOffset.f32InitialReactiveEnergyVAR);
}

double AC_SENSE::GENSET_GetReactiveEnergyVARH(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eGenSystemType))
    {
        return _fCTMultiplier*_fGenPTMultiplier*_aPowers[Phase].GetGensetReactiveEnergy();
    }
    return 0;
}

float AC_SENSE::GENSET_GetRYVolts()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return 0;
    }
    return _fGenPTMultiplier*_ryGensetPhaseVoltage.GetFilteredRMS(RMS::VOLTAGE);
}

float AC_SENSE::GENSET_GetYBVolts()
{
    if(prvIsACSyte1Phaseor1Phase3Wire(_eGenSystemType))
    {
        return 0;
    }
    return _fGenPTMultiplier*_ybGensetPhaseVoltage.GetFilteredRMS(RMS::VOLTAGE);
}

float AC_SENSE::GENSET_GetRBVolts()
{
    if(prvIsACSyte1Phaseor1Phase3Wire(_eGenSystemType))
    {
        return 0;
    }
    return _fGenPTMultiplier*_rbGensetPhaseVoltage.GetFilteredRMS(RMS::VOLTAGE);
}

float AC_SENSE::GENSET_GetDispRYVolts()
{
    if(_eGenSystemType==PHASE_1_SYSTEM)
    {
        return 0;
    }
    return _fGenPTMultiplier*_ryGensetPhaseVoltage.GetFilteredRMSValueForDisplay(RMS::VOLTAGE);
}

float AC_SENSE::GENSET_GetDispYBVolts()
{
    if(prvIsACSyte1Phaseor1Phase3Wire(_eGenSystemType))
    {
        return 0;
    }
    return _fGenPTMultiplier*_ybGensetPhaseVoltage.GetFilteredRMSValueForDisplay(RMS::VOLTAGE);
}

float AC_SENSE::GENSET_GetDispRBVolts()
{
    if(prvIsACSyte1Phaseor1Phase3Wire(_eGenSystemType))
    {
        return 0;
    }
    return _fGenPTMultiplier*_rbGensetPhaseVoltage.GetFilteredRMSValueForDisplay(RMS::VOLTAGE);
}

float AC_SENSE::GENSET_GetMaxFrq()
{
    float f32MaxFreq = GENSET_GetApproxFreq(R_PHASE);
    if(_eGenSystemType != PHASE_1_SYSTEM)
    {
        if(GENSET_GetApproxFreq(Y_PHASE) > f32MaxFreq)
        {
            f32MaxFreq = GENSET_GetApproxFreq(Y_PHASE);
        }
    }
    else if(_eGenSystemType==PHASE_3_SYSTEM)
    {
        if(GENSET_GetApproxFreq(B_PHASE) > f32MaxFreq)
        {
            f32MaxFreq = GENSET_GetApproxFreq(B_PHASE);
        }
    }

    return f32MaxFreq;

}

float AC_SENSE::GENSET_GetMinFrq()
{
    float f32MinFreq = GENSET_GetApproxFreq(R_PHASE);

    if(_eGenSystemType != PHASE_1_SYSTEM)
    {
        if(GENSET_GetApproxFreq(Y_PHASE) < f32MinFreq)
        {
            f32MinFreq = GENSET_GetApproxFreq(Y_PHASE);
        }
    }
    else if(_eGenSystemType==PHASE_3_SYSTEM)
    {
        if(GENSET_GetApproxFreq(B_PHASE) < f32MinFreq)
        {
            f32MinFreq = GENSET_GetApproxFreq(B_PHASE);
        }
    }

    return f32MinFreq;

}
///Measuring mains voltage
float AC_SENSE::MAINS_GetVoltageVolts(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fMainsPTMultiplier*_aPowers[Phase].GetMainsVoltage();
    }
    return 0;
}

float AC_SENSE::MAINS_GetDispVoltageVolts(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fMainsPTMultiplier*_aPowers[Phase].GetDispMainsVoltage();
    }
    return 0;
}


///Measuring mains current
float AC_SENSE::MAINS_GetCurrentAmps(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fCTMultiplier*_aPowers[Phase].GetMainsCurrent();
    }
    return 0;
}

///Measuring mains frequency
float AC_SENSE::MAINS_GetApproxFreq(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _aPowers[Phase].GetMainsFrequency();
    }
    return 0;
}

///Measuring mains power
//Active power
float AC_SENSE::MAINS_GetTotalActivePowerWatts()
{
    if((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsActivePower());
    }
    else if(_eMainsSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsActivePower() +
                                 _aPowers[Y_PHASE].GetMainsActivePower());
    }
    return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsActivePower() +
                              _aPowers[Y_PHASE].GetMainsActivePower() +
                                  _aPowers[B_PHASE].GetMainsActivePower());
}

float AC_SENSE::MAINS_GetTotalFiltActivePowerWatts()
{
    if((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetFiltMainsActivePower());
    }
    else if(_eMainsSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetFiltMainsActivePower() +
                                 _aPowers[Y_PHASE].GetFiltMainsActivePower());
    }
    return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetFiltMainsActivePower() +
                              _aPowers[Y_PHASE].GetFiltMainsActivePower() +
                                  _aPowers[B_PHASE].GetFiltMainsActivePower());
}


float AC_SENSE::MAINS_GetActivePowerWatts(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fCTMultiplier*_fMainsPTMultiplier*_aPowers[Phase].GetMainsActivePower();
    }
    return 0;
}

float AC_SENSE::MAINS_GetDispActivePowerWatts(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fCTMultiplier*_fMainsPTMultiplier*_aPowers[Phase].GetFiltMainsActivePower();
    }
    return 0;
}

//Apparent power
float AC_SENSE::MAINS_GetTotalApparentPowerVA()
{
    if((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsApparentPower());
    }
    else if(_eMainsSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsApparentPower() +
                                   _aPowers[Y_PHASE].GetMainsApparentPower());
    }
    return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsApparentPower() +
                             _aPowers[Y_PHASE].GetMainsApparentPower() +
                                _aPowers[B_PHASE].GetMainsApparentPower());
}

float AC_SENSE::MAINS_GetTotalFiltApparentPowerVA()
{
    if((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetFiltMainsApparentPower());
    }
    else if(_eMainsSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetFiltMainsApparentPower() +
                                   _aPowers[Y_PHASE].GetFiltMainsApparentPower());
    }
    return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetFiltMainsApparentPower() +
                             _aPowers[Y_PHASE].GetFiltMainsApparentPower() +
                                _aPowers[B_PHASE].GetFiltMainsApparentPower());
}


float AC_SENSE::MAINS_GetApparentPowerVA(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fCTMultiplier*_fMainsPTMultiplier*_aPowers[Phase].GetMainsApparentPower();
    }
    return 0;
}

float AC_SENSE::MAINS_GetDispApparentPowerVA(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fCTMultiplier*_fMainsPTMultiplier*_aPowers[Phase].GetFiltMainsApparentPower();
    }
    return 0;
}

//Reactive power
float AC_SENSE::MAINS_GetTotalReactivePowerVAR()
{
    if((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsReactivePower());
    }
    else if(_eMainsSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsReactivePower() +
                             _aPowers[Y_PHASE].GetMainsReactivePower() );
    }
    return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsReactivePower() +
                             _aPowers[Y_PHASE].GetMainsReactivePower() +
                                _aPowers[B_PHASE].GetMainsReactivePower());
}

float AC_SENSE::MAINS_GetTotalFiltReactivePowerVAR()
{
    if((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetFiltMainsReactivePower());
    }
    else if(_eMainsSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetFiltMainsReactivePower() +
                             _aPowers[Y_PHASE].GetFiltMainsReactivePower() );
    }
    return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetFiltMainsReactivePower() +
                             _aPowers[Y_PHASE].GetFiltMainsReactivePower() +
                                _aPowers[B_PHASE].GetFiltMainsReactivePower());
}

float AC_SENSE::MAINS_GetReactivePowerVAR(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fCTMultiplier*_fMainsPTMultiplier*_aPowers[Phase].GetMainsReactivePower();
    }
    return 0;
}

float AC_SENSE::MAINS_GetDispReactivePowerVAR(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fCTMultiplier*_fMainsPTMultiplier*_aPowers[Phase].GetFiltMainsReactivePower();
    }
    return 0;
}

//Power factor
float AC_SENSE::MAINS_GetPowerFactor(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _aPowers[Phase].GetMainsPowerFactor();
    }
    return 0;
}

float AC_SENSE::MAINS_GetDispPowerFactor(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _aPowers[Phase].GetFiltMainsPowerFactor();
    }
    return 0;
}

///Measuring mains energy
//Active energy
double AC_SENSE::MAINS_GetTotalActiveEnergySinceInitWH()
{
    if((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsActiveEnergy())+
                _mainsEnergyOffset.f32InitialActiveEnergyWH;
    }
    else if (_eMainsSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsActiveEnergy() +
                               _aPowers[Y_PHASE].GetMainsActiveEnergy())+
                _mainsEnergyOffset.f32InitialActiveEnergyWH;
    }
    return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsActiveEnergy() +
                             _aPowers[Y_PHASE].GetMainsActiveEnergy() +
                                _aPowers[B_PHASE].GetMainsActiveEnergy()) +
                       _mainsEnergyOffset.f32InitialActiveEnergyWH;

}

double AC_SENSE::MAINS_GetActiveEnergyWH(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fCTMultiplier*_fMainsPTMultiplier*_aPowers[Phase].GetMainsActiveEnergy();
    }
    return 0;
}

//Apparent energy
double AC_SENSE::MAINS_GetTotalApparentEnergySinceInitVAH()
{
    if((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsApparentEnergy())+
                       _mainsEnergyOffset.f32InitialApparentEnergyVA;
    }
    else if (_eMainsSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsApparentEnergy() +
                                    _aPowers[Y_PHASE].GetMainsApparentEnergy()) +
                _mainsEnergyOffset.f32InitialApparentEnergyVA;
    }


    return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsApparentEnergy() +
                         _aPowers[Y_PHASE].GetMainsApparentEnergy() +
                            _aPowers[B_PHASE].GetMainsApparentEnergy()) +
               _mainsEnergyOffset.f32InitialApparentEnergyVA;

}

double AC_SENSE::MAINS_GetApparentEnergyVAH(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fCTMultiplier*_fMainsPTMultiplier*_aPowers[Phase].GetMainsApparentEnergy();
    }
    return 0;
}

//Reactive energy
double AC_SENSE::MAINS_GetTotalReactiveEnergySinceInitVARH()
{
    if((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsReactiveEnergy())+
                _mainsEnergyOffset.f32InitialReactiveEnergyVAR;
    }
    else if (_eMainsSystemType==SPLIT_PHASE_SYSTEM)
    {
        return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsReactiveEnergy() +
                         _aPowers[Y_PHASE].GetMainsReactiveEnergy()) +
               _mainsEnergyOffset.f32InitialReactiveEnergyVAR;
    }
    return _fCTMultiplier*_fMainsPTMultiplier*(_aPowers[R_PHASE].GetMainsReactiveEnergy() +
                             _aPowers[Y_PHASE].GetMainsReactiveEnergy() +
                                _aPowers[B_PHASE].GetMainsReactiveEnergy()) +
                   _mainsEnergyOffset.f32InitialReactiveEnergyVAR;
}

double AC_SENSE::MAINS_GetReactiveEnergyVARH(PHASE_t Phase)
{
    if(prvIsPhaseAvilableInSelectedACSys(Phase, _eMainsSystemType))
    {
        return _fCTMultiplier*_fMainsPTMultiplier*_aPowers[Phase].GetMainsReactiveEnergy();
    }
    return 0;
}

float AC_SENSE::MAINS_GetRYVolts()
{
    if(_eMainsSystemType==PHASE_1_SYSTEM)
     {
         return 0;
     }
     return _fMainsPTMultiplier*_ryMainsPhaseVoltage.GetFilteredRMS(RMS::VOLTAGE);
}

float AC_SENSE::MAINS_GetYBVolts()
{
    if(prvIsACSyte1Phaseor1Phase3Wire(_eMainsSystemType))
    {
        return 0;
    }
    return _fMainsPTMultiplier*_ybMainsPhaseVoltage.GetFilteredRMS(RMS::VOLTAGE);
}

float AC_SENSE::MAINS_GetRBVolts()
{
    if(prvIsACSyte1Phaseor1Phase3Wire(_eMainsSystemType))
    {
        return 0;
    }
    return _fMainsPTMultiplier*_rbMainsPhaseVoltage.GetFilteredRMS(RMS::VOLTAGE);
}

float AC_SENSE::MAINS_GetDispRYVolts()
{
    if(((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph) || _eMainsSystemType==SPLIT_PHASE_SYSTEM)
     {
         return 0;
     }
     return _fMainsPTMultiplier*_ryMainsPhaseVoltage.GetFilteredRMSValueForDisplay(RMS::VOLTAGE);
}
float AC_SENSE::MAINS_GetDispYBVolts()
{
    if(((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph) || _eMainsSystemType==SPLIT_PHASE_SYSTEM)
    {
        return 0;
    }
    return _fMainsPTMultiplier*_ybMainsPhaseVoltage.GetFilteredRMSValueForDisplay(RMS::VOLTAGE);
}
float AC_SENSE::MAINS_GetDispRBVolts()
{
    if(((_eMainsSystemType==PHASE_1_SYSTEM) && !_Perform3phCalculationEnableFor1Ph) || _eMainsSystemType==SPLIT_PHASE_SYSTEM)
    {
        return 0;
    }
    return _fMainsPTMultiplier*_rbMainsPhaseVoltage.GetFilteredRMSValueForDisplay(RMS::VOLTAGE);
}

bool AC_SENSE::GENSET_GetPhaseRotStatus()
{
    if(prvIsACSyte1Phaseor1Phase3Wire(_eGenSystemType))
    {
        return false;
    }
    return _gensetPhaseRot.bLatchedPhaseReversalStatus;
}

bool AC_SENSE::MAINS_GetPhaseRotStatus()
{
    if(prvIsACSyte1Phaseor1Phase3Wire(_eMainsSystemType))
    {
        return false;
    }
    return _mainsPhaseRot.bLatchedPhaseReversalStatus;
}

float AC_SENSE::MAINS_GetMaxFrq()
{
    float f32MaxFreq = MAINS_GetApproxFreq(R_PHASE);
    if(_eMainsSystemType!=PHASE_1_SYSTEM)
    {
        if(MAINS_GetApproxFreq(Y_PHASE) > f32MaxFreq)
        {
            f32MaxFreq = MAINS_GetApproxFreq(Y_PHASE);
        }
    }
    else if(_eMainsSystemType==PHASE_3_SYSTEM)
    {
        if(MAINS_GetApproxFreq(B_PHASE) > f32MaxFreq)
        {
            f32MaxFreq = MAINS_GetApproxFreq(B_PHASE);
        }
    }
    return f32MaxFreq;

}


void AC_SENSE::UpdateACData(AC_IP::AC_PARAMS_t *sample)
{
    if(sample == NULL)
    {
        return;
    }
    /*Clipping the ADC sample value to the required limits*/
    prvClipToLimit(sample->u16GensetRCnt);
    prvClipToLimit(sample->u16GensetYCnt);
    prvClipToLimit(sample->u16GensetBCnt);
    prvClipToLimit(sample->u16GensetNCnt);
    prvClipToLimit(sample->u16RCurrentCnt);
    prvClipToLimit(sample->u16YCurrentCnt);
    prvClipToLimit(sample->u16BCurrentCnt);
    prvClipToLimit(sample->u16MainsRCnt);
    prvClipToLimit(sample->u16MainsYCnt);
    prvClipToLimit(sample->u16MainsBCnt);
    prvClipToLimit(sample->u16MainsNCnt);
    prvClipToLimit(sample->u16EarthCurrentCnt);


    /*Updating the ADC sample values in the POWER class*/
    _aPowers[R_PHASE].UpdateSample(sample->u16GensetRCnt, sample->u16MainsRCnt,
                                sample->u16GensetNCnt, sample->u16MainsNCnt,
                                          sample->u16RCurrentCnt);
    _aPowers[Y_PHASE].UpdateSample(sample->u16GensetYCnt, sample->u16MainsYCnt,
                                sample->u16GensetNCnt, sample->u16MainsNCnt,
                                      sample->u16YCurrentCnt);
    _aPowers[B_PHASE].UpdateSample(sample->u16GensetBCnt, sample->u16MainsBCnt,
                                sample->u16GensetNCnt, sample->u16MainsNCnt,
                                      sample->u16BCurrentCnt);
    prvUpdateEarthCurrentDCOffset(sample->u16EarthCurrentCnt);
    int16_t i16AbsoluteECurrentSample = (int16_t)(sample->u16EarthCurrentCnt
                                                 - _u16LatchedECurrentOffsetValue);
    _EarthCurrent.AccumulateSampleSet(i16AbsoluteECurrentSample, i16AbsoluteECurrentSample);


    /*Check for phase reversal*/
    prvCheckPhaseReversal(_gensetPhaseRot, sample->u16GensetRCnt,
                   sample->u16GensetYCnt, sample->u16GensetBCnt,
                                            sample->u16GensetNCnt, true);
    prvCheckPhaseReversal(_mainsPhaseRot, sample->u16MainsRCnt,
                    sample->u16MainsYCnt, sample->u16MainsBCnt,
                                                  sample->u16MainsNCnt, false);
    /*Compute mains phase to phase voltages*/
    uint16_t u16SampleDiff = (uint16_t)abs(sample->u16MainsRCnt - sample->u16MainsYCnt);
    _ryMainsPhaseVoltage.AccumulateSampleSet(u16SampleDiff, u16SampleDiff);
    u16SampleDiff = (uint16_t)abs(sample->u16MainsYCnt - sample->u16MainsBCnt);
    _ybMainsPhaseVoltage.AccumulateSampleSet(u16SampleDiff, u16SampleDiff);
    u16SampleDiff = (uint16_t)abs(sample->u16MainsRCnt - sample->u16MainsBCnt);
    _rbMainsPhaseVoltage.AccumulateSampleSet(u16SampleDiff, u16SampleDiff);

    /*Compute genset phase to phase voltages*/
     u16SampleDiff = (uint16_t)abs(sample->u16GensetRCnt - sample->u16GensetYCnt);
    _ryGensetPhaseVoltage.AccumulateSampleSet(u16SampleDiff, u16SampleDiff);
    u16SampleDiff = (uint16_t)abs(sample->u16GensetYCnt - sample->u16GensetBCnt);
    _ybGensetPhaseVoltage.AccumulateSampleSet(u16SampleDiff, u16SampleDiff);
    u16SampleDiff =(uint16_t)abs(sample->u16GensetRCnt - sample->u16GensetBCnt);
    _rbGensetPhaseVoltage.AccumulateSampleSet(u16SampleDiff, u16SampleDiff);
}

static void StaticUpdateACData(AC_IP::AC_PARAMS_t *sample)
{
    if(pACSense != NULL)
    {
        pACSense->UpdateACData(sample);
    }
}

void AC_SENSE::prvCheckPhaseReversal(PHASE_ROT_VARS_t &_phaseRotData, int16_t i16RCnt,
                                        int16_t i16YCnt, int16_t i16BCnt, int16_t i16NCnt, bool GensetMainsSource)
{
    bool bInstantaneousPhaseReversal = _phaseRotData.bLatchedPhaseReversalStatus;
    i16RCnt = (int16_t)(i16RCnt-i16NCnt);
    i16YCnt = (int16_t)(i16YCnt-i16NCnt);
    i16BCnt = (int16_t)(i16BCnt-i16NCnt);
    if(_phaseRotData.bIsPrevPositiveHalfCycle &&
                      (i16RCnt < ZCD_LOWER_THRESHOLD_SAMPLES))
    {
        _phaseRotData.bIsPrevPositiveHalfCycle = false;
        if((i16RCnt < i16YCnt) && (i16RCnt > i16BCnt))
        {
            bInstantaneousPhaseReversal = false;
        }
        else
        {
            bInstantaneousPhaseReversal = true;
        }
        /* We finalize the phase reversal status after we see the same observation
           in last PHASE_REVERSAL_SAMPLE_CNT of samples.
         */
        if(bInstantaneousPhaseReversal != _phaseRotData.bLatchedPhaseReversalStatus)
        {
            _phaseRotData.u8PhaseReverseSampleCnt++;
            if(_phaseRotData.u8PhaseReverseSampleCnt > PHASE_REVERSAL_SAMPLE_CNT)
            {
                _phaseRotData.bLatchedPhaseReversalStatus = bInstantaneousPhaseReversal;
                _phaseRotData.u8PhaseReverseSampleCnt = 0;
            }
        }
        else
        {
           _phaseRotData.u8PhaseReverseSampleCnt=0;
        }
    }

    if(i16RCnt > ZCD_UPPER_THRESHOLD_SAMPLES)
    {
        _phaseRotData.bIsPrevPositiveHalfCycle = true;
    }

    if((GensetMainsSource) && ((_aPowers[R_PHASE].GetGensetVoltageRaw() < 20.0) ||  (_aPowers[Y_PHASE].GetGensetVoltageRaw() < 20.0)
            || (_aPowers[B_PHASE].GetGensetVoltageRaw() < 20.0)))
    {
        bInstantaneousPhaseReversal = false;
        _phaseRotData.bLatchedPhaseReversalStatus = false;
    }
    else if((!GensetMainsSource) && ((_aPowers[R_PHASE].GetMainsVoltage() < 20.0) ||  (_aPowers[Y_PHASE].GetMainsVoltage() < 20.0)
            || (_aPowers[B_PHASE].GetMainsVoltage() < 20.0)))
    {
        if(!_phaseRotData.bLatchedPhaseReversalStatus)
        {
            bInstantaneousPhaseReversal = false;
            _phaseRotData.bLatchedPhaseReversalStatus = false;
        }
    }
}

void AC_SENSE::prvClipToLimit(uint16_t &u16Sample)
{
    static uint16_t u16prvValue=0;
    #define DC_OFFSET_MIN    1.3f  //Min Value
    #define DC_OFFSET_MAX    1.6f  //Max Value

    //This condition is added to avoid noise on AC lines when USB powered.
    if((_f32DCOffserFiltV > DC_OFFSET_MIN) &&
            ( _f32DCOffserFiltV < DC_OFFSET_MAX))
    {
        if(u16Sample > MAX_SAMPLE_VALUE)
        {
            u16Sample = MAX_SAMPLE_VALUE;
        }
        if(u16Sample < MIN_SAMPLE_VALUE)
        {
            u16Sample = MIN_SAMPLE_VALUE;
        }
        u16prvValue = u16Sample;
    }
    else
    {
        u16Sample = u16prvValue;
    }
}

void AC_SENSE::ClearPhaseReverseAlarms()
{
    _mainsPhaseRot.bLatchedPhaseReversalStatus = false;
    _gensetPhaseRot.bLatchedPhaseReversalStatus = false;
}

float AC_SENSE::EARTH_GetCurrentRAW()
{
    return _fECTMultiplier*_EarthCurrent.GetRawRMS();
}

float AC_SENSE::EARTH_GetCurrentFilt()
{
    return _fECTMultiplier*_EarthCurrent.GetFilteredRMS(RMS::CURRENT);
}

void AC_SENSE::prvUpdateEarthCurrentDCOffset(uint16_t u16Sample )
{
    _u16EarthCurrentDCOffsetSampleCount++;
    _u32ECurrentOffsetAccumulator+= u16Sample;
    if(_u16EarthCurrentDCOffsetSampleCount >= _u16EarthCurrentDCOffsetWindowSize)
    {
        _u16LatchedECurrentOffsetValue = (uint16_t)(_u32ECurrentOffsetAccumulator/_u16EarthCurrentDCOffsetWindowSize);
        _u32ECurrentOffsetAccumulator =0;
        _u16EarthCurrentDCOffsetSampleCount=0;
        _u16EarthCurrentDCOffsetWindowSize = DC_OFFSET_WINDOW_SIZE;
    }
}

double AC_SENSE::GetGensetkVAEnergyOffset()
{
    return _gensetEnergyOffset.f32InitialApparentEnergyVA;
}

double AC_SENSE::GetGensetkWEnergyOffset()
{
    return _gensetEnergyOffset.f32InitialActiveEnergyWH;
}

double AC_SENSE::GetGensetkVAREnergyOffset()
{
    return _gensetEnergyOffset.f32InitialReactiveEnergyVAR;
}

double AC_SENSE::GetGensetTotalReactiveEnergy()
{
    return (_fCTMultiplier*_fGenPTMultiplier*((_aPowers[R_PHASE].GetGensetReactiveEnergy())+
            (_aPowers[Y_PHASE].GetGensetReactiveEnergy())
            +(_aPowers[B_PHASE].GetGensetReactiveEnergy())));
}

double AC_SENSE::GetGensetTotalActiveEnergy()
{
    return (_fCTMultiplier*_fGenPTMultiplier*((_aPowers[R_PHASE].GetGensetActiveEnergy())+
            (_aPowers[Y_PHASE].GetGensetActiveEnergy())
            +(_aPowers[B_PHASE].GetGensetActiveEnergy())));
}
double AC_SENSE::GetGensetTotalApparentEnergy()
{
    return (_fCTMultiplier*_fGenPTMultiplier*((_aPowers[R_PHASE].GetGensetApparentEnergy())+
            (_aPowers[Y_PHASE].GetGensetApparentEnergy())
            +(_aPowers[B_PHASE].GetGensetApparentEnergy())));
}
