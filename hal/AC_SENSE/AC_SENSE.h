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



#ifndef _AC_SENSE_H_
#define _AC_SENSE_H_

#include "stdint.h"
#include "AC_IP.h"
#include "AC_SENSE_CONFIG.h"
#if (SUPPORT_CALIBRATION == YES)
#include "EEPROM.h"
#endif /* SUPPORT_CALIBRATION */
#include "AC_SENSE_DS.h"
#include "POWER.h"
#include "RMS.h"
#include "ANLG_IP.h"

class AC_SENSE {

public:
    typedef struct {
        float f32InitialActiveEnergyWH;
        float f32InitialReactiveEnergyVAR;
        float f32InitialApparentEnergyVA;
    } ENERGY_REGISTER_t;

    typedef enum
    {
     PHASE_1_SYSTEM = 0,
     PHASE_3_SYSTEM,
     SPLIT_PHASE_SYSTEM
    } AC_SYSTEM_TYP_t;

    /**
     * Constructs the class.
     * @param - AcIp - Input from the BSP.
     * @return
     * None
     */
#if (SUPPORT_CALIBRATION == YES)
    AC_SENSE(AC_IP &AcIp, ANLG_IP &anlgIp, EEPROM &Eeprom);
#else
    AC_SENSE(AC_IP &AcIp, ANLG_IP &anlgIp);
#endif /* SUPPORT_CALIBRATION*/

    /**
     * Calls the update member function of the POWER class instances of each phase
     *     to update the AC sample values for each instance.
     * @param - None
     * @return
     * None
     */
    void Update();

    /**
     * Configures the CT multiplication factor.
     * @param u16CTFactor         : CT multiplier factor
     * @return
     * None
     */
    void ConfigureCTRatio(uint16_t u16CTFactor, uint16_t u16ECTFactor, float fCTCorrfactor);
    void ConfigureCTRatio(uint16_t u16CTFactor, uint16_t u16ECTFactor);

    /**
     * Configures the PT multiplication factors.
     * @param u16GenPTFactor         : PT multiplier factor for Gen
     * @param u16MainsPTFactor         : PT multiplier factor for Mains
     * @return
     * None
     */
    void ConfigureGenPTRatio(uint16_t u16GenPTPrimaryFactor,uint16_t u16GenPTSecondaryFactor);
    void ConfigureMainsPTRatio(uint16_t u16MainsPTPrimaryFactor,uint16_t u16MainsPTSecondaryFactor);
    /**
     * Configures the 3P system
     * @param eGenSystemType: 3 AC system Type
     *        eMainsSystemType: 3 AC system Type
     * @return
     * None
     */
    void ConfigureACSystem(AC_SYSTEM_TYP_t eGenSystemType, AC_SYSTEM_TYP_t eMainsSystemType);

    /**
     * Configures the current channel. The current channel can either measure
     * genset current or mains current.
     * @param iCfg: Current channel
     * @return
     * None
     */
    void ConfigureISensing(I_CFG_t iCfg);
    void ConfigureGenTamp(bool bIsGenTamp);

    void Configure3phCalculationEnableFor1Ph(uint8_t CalculationOf3PhFor1ph);

    void SetEnergyOffsets( ENERGY_REGISTER_t &stTampGensetEnergyOffset,ENERGY_REGISTER_t &stGensetEnergyOffset,
                               ENERGY_REGISTER_t &stMainsEnergyOffset);

    /**
     * Sets the energy offsets. The mains and genset energy calculation start from
     * this offset.
     * @param stGensetEnergyOffset: Mains energy offsets
     * @param stMainsEnergyOffset : Genset energy offsets
     * @return
     * None
     */
//    void SetEnergyOffsets(ENERGY_REGISTER_t &stGensetEnergyOffset,
//                            ENERGY_REGISTER_t &stMainsEnergyOffset);

    float GENSET_GetDispVoltageVolts(PHASE_t Phase);
    /**
     * Returns the Genset R, Y or B phase RMS voltage.
     * @param - None
     * @return
     * Genset phase RMS voltage in volts
     */
    float GENSET_GetVoltageVolts(PHASE_t Phase);


    /**
     * Returns the Genset R, Y or B phase RMS Raw voltage.
     * @param - None
     * @return
     * Genset phase RMS voltage in volts
     */
    float GENSET_GetVoltageVoltsRaw(PHASE_t Phase);

    float GENSET_GetPollFrequency(PHASE_t Phase);

    /**
     * Returns the Genset R, Y or B phase RMS current.
     * @param - None
     * @return
     * Genset phase RMS current in amps
     */
    float GENSET_GetCurrentAmps(PHASE_t Phase);


    /**
     * Returns the Genset R, Y or B phase approximate frequency.
     * @param - None
     * @return
     * Genset phase approximate frequency in Hz
     */
    float GENSET_GetApproxFreq(PHASE_t Phase);

    /**
     * Returns the combined active power of the Genset R, Y and B phases. This will
     * return 0 in case the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset total active power in Watts
     */
    float GENSET_GetTotalActivePowerWatts();

    float GENSET_GetTotalFiltActivePowerWatts();

    /**
     * Returns the combined apparent power of the Genset R, Y and B phases. This will
     * return 0 in case the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset total apparent power in VA
     */
    float GENSET_GetTotalApparentPowerVA();

    float GENSET_GetTotalFiltApparentPowerVA();

    /**
     * Returns the combined reactive power the Genset of R, Y and B phases. This will
     * return 0 in case the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset total reactive power in VAR
     */
    float GENSET_GetTotalReactivePowerVAR();

    float GENSET_GetTotalFiltReactivePowerVAR();

    /**
     * Returns the active power of the Genset R, Y or B phase. This will return 0 in case the
     * CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset phase active power in Watts
     */
    float GENSET_GetActivePowerWatts(PHASE_t Phase);

    float GENSET_GetDispActivePowerWatts(PHASE_t Phase);

    /**
     * Returns the apparent power of the Genset R, Y or B phase. This will return 0 in case
     * the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset phase apparent power in VA
     */
    float GENSET_GetApparentPowerVA(PHASE_t Phase);

    float GENSET_GetDispApparentPowerVA(PHASE_t Phase);

    /**
     * Returns the reactive power of the Genset R, Y or B phase. This will return 0 in case
     * the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset phase reactive power in VAR
     */
    float GENSET_GetReactivePowerVAR(PHASE_t Phase);

    float GENSET_GetDispReactivePowerVAR(PHASE_t Phase);

    /**
     * Returns the power factor of the Genset R, Y or B phase. This will return 0 in case
     * the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset phase power factor
     */
    float GENSET_GetPowerFactor(PHASE_t Phase);

    float GENSET_GetDispPowerFactor(PHASE_t Phase);

    /**
     * Returns the average power factor of the three phases.
     * @param - None
     * @return
     * Genset average power factor
     */
    float GENSET_GetAveragePowerFactor();

    float GENSET_GetDispAveragePowerFactor();

    double GENSET_GetTotalTamperedActiveEnergySinceInitWH();

    /**
     * Returns the combined active energy of the Genset R, Y and B phases. This will
     * return 0 in case the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset total active energy in WH
     */
    double GENSET_GetTotalActiveEnergySinceInitWH();

    double GENSET_GetTotalTamperedApparentEnergySinceInitVAH();
    /**
     * Returns the combined apparent energy of the Genset R, Y and B phases. This
     * will return 0 in case the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset total apparent energy in VAH
     */
    double GENSET_GetTotalApparentEnergySinceInitVAH();

    double GENSET_GetTotalTamperedReactiveEnergySinceInitVARH();
    /**
     * Returns the combined reactive energy the Genset of R, Y and B phases. This
     * will return 0 in case the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset total reactive energy in VARH
     */
    double GENSET_GetTotalReactiveEnergySinceInitVARH();

    /**
     * Returns the active energy of the Genset R, Y or B phase. This will return 0 in case
     * the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset phase active energy in WH
     */
    double GENSET_GetActiveEnergyWH(PHASE_t Phase);

    /**
     * Returns the apparent energy of the Genset R, Y or B phase. This will return 0 in case
     * the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset phase apparent energy in VAH
     */
    double GENSET_GetApparentEnergyVAH(PHASE_t Phase);

    /**
     * Returns the reactive energy of the Genset R, Y or B phase. This will return 0 in case
     * the CT is configured to be on Mains.
     * @param - None
     * @return
     * Genset phase reactive energy in VARH
     */
    double GENSET_GetReactiveEnergyVARH(PHASE_t Phase);

   /**
    * Returns the phase rotation status of the Genset i.e., whether phase
    * reversal has occurred or not.
    * @param - None
    * @return
    * Boolean stating whether there is phase reversal in Genset
    */
    bool GENSET_GetPhaseRotStatus();

    /**
    * Returns the R-Y genset phase to phase voltage
    * @param - None
    * @return
    * phase to phase voltage
    */
    float GENSET_GetRYVolts();

    /**
    * Returns the Y-B genset phase to phase voltage
    * @param - None
    * @return
    * phase to phase voltage
    */
    float GENSET_GetYBVolts();

    /**
    * Returns the R-B genset phase to phase voltage
    * @param - None
    * @return
    * phase to phase voltage
    */
    float GENSET_GetRBVolts();

    float GENSET_GetDispRYVolts();
    float GENSET_GetDispYBVolts();
    float GENSET_GetDispRBVolts();

    float GENSET_GetMaxFrq();
    float GENSET_GetMinFrq();
     /**
     * Returns the mains R, Y or B phase RMS voltage.
     * @param - None
     * @return
     * Mains phase RMS voltage in volts
     */
    float MAINS_GetDispVoltageVolts(PHASE_t Phase);
    float MAINS_GetVoltageVolts(PHASE_t Phase);

    /**
     * Returns the mains R, Y or B phase RMS current.
     * @param - None
     * @return
     * Mains phase RMS current in MA
     */
    float MAINS_GetCurrentAmps(PHASE_t Phase);

    /**
     * Returns the mains R, Y or B phase approximate frequency.
     * @param - None
     * @return
     * Mains phase approximate frequency in Hz
     */
    float MAINS_GetApproxFreq(PHASE_t Phase);

    /**
     * Returns the combined active power of the mains R, Y and B phases. This will
     * return 0 in case the CT is configured to be on Genset.
     * @param - None
     * @return
     * Mains total active power in Watts
     */
    float MAINS_GetTotalActivePowerWatts();

    float MAINS_GetTotalFiltActivePowerWatts();

    /**
     * Returns the combined apparent power of the mains R, Y and B phases. This
     * will return 0 in case the CT is configured to be on Genset.
     * @param - None
     * @return
     * Mains total apparent power in VA
     */
    float MAINS_GetTotalApparentPowerVA();

    float MAINS_GetTotalFiltApparentPowerVA();

    /**
     * Returns the combined reactive power of the mains R, Y and B phases. This
     * will return 0 in case the CT is configured to be on Genset.
     * @param - None
     * @return
     * Mains total reactive power in VAR
     */
    float MAINS_GetTotalReactivePowerVAR();

    float MAINS_GetTotalFiltReactivePowerVAR();

    /**
     * Returns the active power of the mains R, Y or B phase. This will
     * return 0 in case the CT is configured to be on Genset.
     * @param - None
     * @return
     * Mains phase active power in Watts
     */
    float MAINS_GetActivePowerWatts(PHASE_t Phase);

    float MAINS_GetDispActivePowerWatts(PHASE_t Phase);

    /**
     * Returns the apparent power of the mains R, Y or B phase. This will
     * return 0 in case the CT is configured to be on Genset.
     * @param - None
     * @return
     * Mains phase apparent power in VA
     */
    float MAINS_GetApparentPowerVA(PHASE_t Phase);

    float MAINS_GetDispApparentPowerVA(PHASE_t Phase);

    /**
     * Returns the reactive power of the mains R, Y or B phase. This will
     * return 0 in case the CT is configured to be on Genset.
     * @param - None
     * @return
     * Mains phase reactive power in VAR
     */
    float MAINS_GetReactivePowerVAR(PHASE_t Phase);

    float MAINS_GetDispReactivePowerVAR(PHASE_t Phase);

    /**
     * Returns the power factor of the mains R, Y or B phase. This will
     * return 0 in case the CT is configured to be on Genset.
     * @param - None
     * @return
     * Mains phase power factor
     */
    float MAINS_GetPowerFactor(PHASE_t Phase);

    float MAINS_GetDispPowerFactor(PHASE_t Phase);

    /// Energy since init
    /**
     * Returns the combined active energy the mains of R, Y and B phases. This
     * will return 0 in case the CT is configured to be on Genset.
     * @param - None
     * @return
     * Mains total active energy in WH
     */
    double MAINS_GetTotalActiveEnergySinceInitWH();

    /**
     * Returns the combined active energy the mains of R, Y and B phases. This
     * will return 0 in case the CT is configured to be on Genset.
     * @param - None
     * @return
     * Mains total apparent energy in VAH
     */
    double MAINS_GetTotalApparentEnergySinceInitVAH();

    /**
     * Returns the combined reactive energy the mains of R, Y and B phases. This
     * will return 0 in case the CT is configured to be on Genset.
     * @param - None
     * @return
     * Mains total reactive energy in VARH
     */
    double MAINS_GetTotalReactiveEnergySinceInitVARH();



    /**
    * Returns the active energy of the mains R, Y or B phase. This will return 0 in
    * case the CT is configured to be on Genset.
    * @param - None
    * @return
    * Mains phase active energy in WH
    */
    double MAINS_GetActiveEnergyWH(PHASE_t Phase);

    /**
    * Returns the apparent energy of the mains R, Y or B phase. This will return 0 in
    * case the CT is configured to be on Genset.
    * @param - None
    * @return
    * Mains phase apparent energy in VAH
    */
    double MAINS_GetApparentEnergyVAH(PHASE_t Phase);

    /**
    * Returns the reactive energy of the mains R, Y or B phase. This will return 0 in
    * case the CT is configured to be on Genset.
    * @param - None
    * @return
    * Mains phase reactive energy in VARH
    */
    double MAINS_GetReactiveEnergyVARH(PHASE_t Phase);

    /**
    * Returns the phase rotation status of the Mains i.e., whether phase
    * reversal has occurred or not.
    * @param - None
    * @return
    * Boolean stating whether there is phase reversal in Mains
    */
    bool MAINS_GetPhaseRotStatus();

    /**
    * Returns the R-Y mains phase to phase voltage
    * @param - None
    * @return
    * phase to phase voltage
    */
    float MAINS_GetRYVolts();

    /**
    * Returns the Y-B mains phase to phase voltage
    * @param - None
    * @return
    * phase to phase voltage
    */
    float MAINS_GetYBVolts();

    /**
    * Returns the R-B mains phase to phase voltage
    * @param - None
    * @return
    * phase to phase voltage
    */
    float MAINS_GetRBVolts();

    float MAINS_GetDispRYVolts();
    float MAINS_GetDispYBVolts();
    float MAINS_GetDispRBVolts();

    float MAINS_GetMaxFrq();

    /**
    * Does clipping and then updates the ADC sample values within the POWER class
    * @param - sample - Reference to the struct containing the ADC sample values
    * @return
    * None
    */
    void UpdateACData(AC_IP::AC_PARAMS_t *sample);

    void ClearPhaseReverseAlarms();

    void ConfigGeneratorRating(float f32GenRating);

    float GENSET_GetPercentPower();

    float GENSET_GetDispPercentPower();

    float EARTH_GetCurrentRAW();

    float EARTH_GetCurrentFilt();

    double GetGensetkVAEnergyOffset();
    double GetGensetkWEnergyOffset();
    double GetGensetkVAREnergyOffset();

    double GetGensetTotalReactiveEnergy();
    double GetGensetTotalActiveEnergy();
    double GetGensetTotalApparentEnergy();

    /**
     *
    * Returns the raw R-Y genset phase to phase voltage
    * @param - None
    * @return
    * raw RY phase to phase voltage
    */
    float GENSET_GetRawRYVolts();

    /**
     *
    * Returns the raw Y-B genset phase to phase voltage
    * @param - None
    * @return
    * raw YB phase to phase voltage
    */
    float GENSET_GetRawYBVolts();

    /**
     *
    * Returns the raw R-B genset phase to phase voltage
    * @param - None
    * @return
    * raw RB phase to phase voltage
    */
    float GENSET_GetRawRBVolts();

    /**
     *
    * Returns the  average of raw genset phase to phase voltages
    * @param - None
    * @return
    * average of raw phase to phase voltage
    */
    float GENSET_GetRawAverageL_L();

    /**
     *
    * Returns the  average of genset phase to neutral voltages
    * @param - None
    * @return
    * average of raw phase to neutral voltages
    */
    float GENSET_GetRawAverageL_N();
    /**
     *
    * Returns the  average of filtered genset frequencies .
    * @param - None
    * @return
    * average of filtered frequencies of each phase
    */
    float GENSET_GetAvgFreq();
    /**
     *
    * Returns the  raw genset current of the desired phase
    * @param - PHASE_t Phase
    * @return
    * raw current of desired phase
    */
    float GENSET_GetRawCurrentAmps(PHASE_t Phase);

    /**
     *
    * Returns the  average of raw genset current values
    * @param
    * @return
    * average of raw current values
    */
    float GENSET_GetRawAvgCurrent();
private:

    typedef struct{

        bool    bIsPrevPositiveHalfCycle;
        bool    bLatchedPhaseReversalStatus;
        uint8_t u8PhaseReverseSampleCnt;
    } PHASE_ROT_VARS_t;


    #define MAX_CT_SECONDARY_CURRENT_A (5U)
    #define MIN_AC_DISP_VTG            (20.0f)

    POWER             _aPowers[PHASE_END];
    ENERGY_REGISTER_t _tampGensetEnergyOffset;
    ENERGY_REGISTER_t _gensetEnergyOffset;
    ENERGY_REGISTER_t _mainsEnergyOffset;
    PHASE_ROT_VARS_t  _gensetPhaseRot;
    PHASE_ROT_VARS_t  _mainsPhaseRot;

    RMS               _ryMainsPhaseVoltage;
    RMS               _ybMainsPhaseVoltage;
    RMS               _rbMainsPhaseVoltage;

    RMS               _ryGensetPhaseVoltage;
    RMS               _ybGensetPhaseVoltage;
    RMS               _rbGensetPhaseVoltage;

    RMS               _EarthCurrent;
    AC_IP             &_AcIp;

    bool              _Perform3phCalculationEnableFor1Ph;
    /* Contains the CT multiplication factor, ex:this will have 10 in case of
       50/5 CT
     */
    float             _fCTMultiplier;
    float             _fECTMultiplier;
    float             _fGenPTMultiplier;
    float             _fMainsPTMultiplier;
    float             _fGenRating;

    ANLG_IP &_AnlgIp;
    stTimer           _st200ms;
    float             _f32DCOffserFiltV;

    AC_SYSTEM_TYP_t   _eGenSystemType;
    AC_SYSTEM_TYP_t   _eMainsSystemType;
    AC_SYSTEM_TYP_t   _ePrvGenSysType;

    float              _f32ECurrentOffsetAccumulator;
    uint16_t          _u16EarthCurrentDCOffsetSampleCount;
    float              _f32LatchedECurrentOffsetValue;

    uint16_t          _u16LatchedECurrentOffsetValue;
    /*Accumulator of current sample values, used in deducing DC offset*/
    uint32_t          _u32ECurrentOffsetAccumulator;
    uint16_t          _u16EarthCurrentDCOffsetWindowSize;

    /**
     * Function that clips the ADC sample within the upper and lower limits.
     * @param - u16Sample - Reference to the ADC sample value
     * @return
     * None
     */
    void prvClipToLimit(uint16_t &u16Sample);

    /**
    * Function that checks whether there is a phase reversal.
    * @param - _phaseRotData - Reference to struct containg phase rotation data
    * @param - i16RCnt
    * @param - i16YCnt
    * @param - i16BCnt
    * @param - i16NCnt
    * @return
    * None
    */
    void prvCheckPhaseReversal(PHASE_ROT_VARS_t &_phaseRotData, int16_t i16RCnt,
                           int16_t i16YCnt, int16_t i16BCnt, int16_t i16NCnt, bool GensetMainsSource);

    void prvUpdateEarthCurrentDCOffset(float f32EarthCurrent);

    void prvUpdateEarthCurrentDCOffset(uint16_t u16Sample );

#if (SUPPORT_CALIBRATION == YES)
    EEPROM            &_eeprom;
    /* This variable stores the calibration parameter values obtained from EEPROM. */
    CALIBRATED_DATA_t _stCalibrationData;
    /**
     * This function reads and validates the calibration values loaded from EEPROM.
     * @param
     * None
     * @return
     * None
     */
    bool prvReadCalibFactors();
    /**
     * This function initializes calibration values structure with default values.
     * @param
     * None
     * @return
     * None
     */
    void prvInitCalibFactors();
#endif /* SUPPORT_CALIBRATION*/

    bool prvIsPhaseAvilableInSelectedACSys(PHASE_t Phase, AC_SYSTEM_TYP_t etype);
    bool prvIsACSyte1Phaseor1Phase3Wire(AC_SYSTEM_TYP_t etype);
};

#endif
