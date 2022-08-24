#include "Tests.h"


#define MAX_ERROR     (1.5F)
#define CT_RATIO      (5)
#define CT_MUL_FACTOR (CT_RATIO/MAX_CT_SECONDARY_CURRENT_A)

static vector<Test*> testList;

bool Base_RmsTest(float shift, int cycle_count, AC_IP::AC_PARAMS_t &sampleSet,
                  uint16_t &sample, float (AC_SENSE::*fc)(PHASE_t),
          float ExpectedValue, I_CFG_t eICfg,
              PHASE_t Phase, Sinusoid::INPUT_TYPE_t = Sinusoid::VOLTAGE);

bool Base_PowerTest(float shiftV, float shiftI, 
                    int cycle_count,
                    AC_IP::AC_PARAMS_t &sampleSet, 
                    uint16_t &sampleV, uint16_t &sampleI, 
                    float (AC_SENSE::*fc)(PHASE_t), float ExpectedValue,
                    I_CFG_t eICfg, bool considerCtMultiplier, PHASE_t Phase);

bool Base_EnergyTest(float shiftV, float shiftI, 
                    int cycle_count,
                    AC_IP::AC_PARAMS_t &sampleSet, 
                    uint16_t &sampleV, uint16_t &sampleI, 
                    double (AC_SENSE::*fc)(PHASE_t), float ExpectedValue,
                    I_CFG_t eICfg, bool considerCtMultiplier, PHASE_t Phase);

bool Base_TotalPowerTest(float shiftVR, float shiftIR, float shiftVY,
                        float shiftIY, float shiftVB, float shiftIB,
                        int cycle_count, AC_IP::AC_PARAMS_t &sampleSet,
                              uint16_t &sampleVR, uint16_t &sampleIR,
                              uint16_t &sampleVY, uint16_t &sampleIY,
                              uint16_t &sampleVB, uint16_t &sampleIB,
                                 float (AC_SENSE::*fc)(), float ExpectedValue,
                                    I_CFG_t eICfg, bool considerCtMultiplier);


bool Base_TotalEnergyTest(float shiftVR, float shiftIR, float shiftVY,
                        float shiftIY, float shiftVB, float shiftIB,
                        int cycle_count, AC_IP::AC_PARAMS_t &sampleSet,
                              uint16_t &sampleVR, uint16_t &sampleIR,
                              uint16_t &sampleVY, uint16_t &sampleIY,
                              uint16_t &sampleVB, uint16_t &sampleIB,
                                 double (AC_SENSE::*fc)(), float ExpectedValue,
                                    I_CFG_t eICfg, bool considerCtMultiplier);


bool Base_PhaseRotTest(float shiftR, float shiftY, float shiftB, int cycle_count,
                           AC_IP::AC_PARAMS_t &sampleSet,
                              uint16_t &sampleVR, uint16_t &sampleVY,
                              uint16_t &sampleVB, bool (AC_SENSE::*fc)());

bool Base_PowerPhaseToPhaseRMS(float shiftV1, float shiftV2, 
                                int cycle_count,
                                    AC_IP::AC_PARAMS_t &sampleSet, 
                                    uint16_t &sampleV1, uint16_t &sampleV2, 
                                    float (AC_SENSE::*fc)(), float ExpectedValue);

class Test1_VRMS_RPhase: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(0, 600, snapshot,
                            snapshot.u16GensetRCnt, &AC_SENSE::GENSET_GetVoltageVolts,
                310/sqrt(2), GENSET_CURRENT_MEASUREMENT, R_PHASE);
    }

    string TestDescription()
    {
        return "Test1_VRMS_RPhase";
    }
};


class Test2_IRMS_RPhase: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(0, 600, snapshot,
                            snapshot.u16RCurrentCnt, &AC_SENSE::GENSET_GetCurrentAmps,
                    2/sqrt(2), GENSET_CURRENT_MEASUREMENT, R_PHASE, Sinusoid::CURRENT);
    }

    string TestDescription()
    {
        return "Test2_IRMS_RPhase";
    }
};


class Test3_VRMS_YPhase: public Test
{
    bool Run()
    {
        float shift       = PI*2/3;
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(shift, 600, snapshot,
                            snapshot.u16GensetYCnt, &AC_SENSE::GENSET_GetVoltageVolts,
                310/sqrt(2), GENSET_CURRENT_MEASUREMENT, Y_PHASE);
    }

    string TestDescription()
    {
        return "Test3_VRMS_YPhase";
    }
};


class Test4_VRMS_BPhase: public Test
{
    bool Run()
    {
        float shift       = PI*4/3;
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(shift, 600, snapshot,
                            snapshot.u16GensetBCnt, &AC_SENSE::GENSET_GetVoltageVolts,
                310/sqrt(2), GENSET_CURRENT_MEASUREMENT, B_PHASE);
    }

    string TestDescription()
    {
        return "Test4_VRMS_BPhase";
    }
};


class Test5_IRMS_YPhase: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(0, 600, snapshot,
                            snapshot.u16YCurrentCnt, &AC_SENSE::GENSET_GetCurrentAmps,
                                2/sqrt(2), GENSET_CURRENT_MEASUREMENT, Y_PHASE, Sinusoid::CURRENT);
    }

    string TestDescription()
    {
        return "Test5_IRMS_YPhase";
    }
};


class Test6_IRMS_BPhase: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(0, 600, snapshot,
                            snapshot.u16BCurrentCnt, &AC_SENSE::GENSET_GetCurrentAmps,
                                2/sqrt(2), GENSET_CURRENT_MEASUREMENT, B_PHASE, Sinusoid::CURRENT);
    }

    string TestDescription()
    {
        return "Test6_IRMS_BPhase";
    }
};


class Test7_ActivePower: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_PowerTest(0, PI/6, 600, snapshot,
                           snapshot.u16GensetRCnt, snapshot.u16RCurrentCnt,
                &AC_SENSE::GENSET_GetActivePowerWatts, 300*cos(PI/6),
                 GENSET_CURRENT_MEASUREMENT, true, R_PHASE);
    }

    string TestDescription()
    {
        return "Test7_ActivePower";
    }
};


class Test8_ApparentPower: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
    return Base_PowerTest(0, PI/6, 600, snapshot,
                           snapshot.u16GensetYCnt, snapshot.u16YCurrentCnt,
                            &AC_SENSE::GENSET_GetApparentPowerVA, 300, GENSET_CURRENT_MEASUREMENT,
                            true, Y_PHASE);
    }

    string TestDescription()
    {
        return "Test8_ApparentPower";
    }
};


class Test9_ReactivePower: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
    return Base_PowerTest(0, PI/6, 600, snapshot,
                           snapshot.u16GensetBCnt, snapshot.u16BCurrentCnt,
                            &AC_SENSE::GENSET_GetReactivePowerVAR, 300*sin(PI/6),
                             GENSET_CURRENT_MEASUREMENT, true, B_PHASE);
    }

    string TestDescription()
    {
        return "Test9_ReactivePower";
    }
};


class Test10_RPowerFactor: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
    return Base_PowerTest(0, -PI/6, 600, snapshot,
                           snapshot.u16GensetRCnt, snapshot.u16RCurrentCnt,
                            &AC_SENSE::GENSET_GetPowerFactor, cos(PI/6), GENSET_CURRENT_MEASUREMENT,
                            false, R_PHASE);
    }

    string TestDescription()
    {
        return "Test10_RPowerFactor";
    }
};


class Test11_YPowerFactor: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
    return Base_PowerTest(0, PI/6, 600, snapshot,
                           snapshot.u16GensetYCnt, snapshot.u16YCurrentCnt,
                            &AC_SENSE::GENSET_GetPowerFactor, -cos(PI/6), GENSET_CURRENT_MEASUREMENT,
                            false, Y_PHASE);
    }

    string TestDescription()
    {
        return "Test11_YPowerFactor";
    }
};


class Test12_BPowerFactor: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
    return Base_PowerTest(0, PI/6, 600, snapshot,
                           snapshot.u16GensetBCnt, snapshot.u16BCurrentCnt,
                            &AC_SENSE::GENSET_GetPowerFactor, -cos(PI/6), GENSET_CURRENT_MEASUREMENT,
                            false, B_PHASE);
    }

    string TestDescription()
    {
        return "Test12_BPowerFactor";
    }
};


class Test13_RFrequency: public Test
{
    bool Run()
    {
        float shift       = 0;
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(shift, 600, snapshot,
                            snapshot.u16GensetRCnt, &AC_SENSE::GENSET_GetApproxFreq,
                50, GENSET_CURRENT_MEASUREMENT, R_PHASE);
    }

    string TestDescription()
    {
        return "Test13_RFrequency";
    }
};


class Test14_YFrequency: public Test
{
    bool Run()
    {   
        float shift       = 0;
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(shift, 600, snapshot,
                            snapshot.u16GensetYCnt, &AC_SENSE::GENSET_GetApproxFreq,
                            50, GENSET_CURRENT_MEASUREMENT, Y_PHASE);
    }   

    string TestDescription()
    {   
        return "Test14_YFrequency";
    }   
};


class Test15_BFrequency: public Test
{
    bool Run()
    {
        float shift       = 0;
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(shift, 600, snapshot,
                            snapshot.u16GensetBCnt, &AC_SENSE::GENSET_GetApproxFreq,
                            50, GENSET_CURRENT_MEASUREMENT, B_PHASE);
    }

    string TestDescription()
    {
        return "Test15_BFrequency";
    }
};


class Test16_Mains_VRMS_RPhase: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(0, 600, snapshot,
                            snapshot.u16MainsRCnt, &AC_SENSE::MAINS_GetVoltageVolts,
                            310/sqrt(2), MAINS_CURRENT_MEASUREMENT, R_PHASE);
    }

    string TestDescription()
    {
        return "Test16_Mains_VRMS_RPhase";
    }
};


class Test17_Mains_IRMS_RPhase: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(0, 600, snapshot,
                            snapshot.u16RCurrentCnt, &AC_SENSE::MAINS_GetCurrentAmps,
                            2/sqrt(2), MAINS_CURRENT_MEASUREMENT, R_PHASE,
                Sinusoid::CURRENT);
    }

    string TestDescription()
    {
        return "Test17_Mains_IRMS_RPhase";
    }
};


class Test18_Mains_VRMS_YPhase: public Test
{
    bool Run()
    {
        float shift       = PI*2/3;
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(shift, 600, snapshot,
                            snapshot.u16MainsYCnt, &AC_SENSE::MAINS_GetVoltageVolts,
                            310/sqrt(2), MAINS_CURRENT_MEASUREMENT, Y_PHASE);
    }

    string TestDescription()
    {
        return "Test18_Mains_VRMS_YPhase";
    }
};


class Test19_Mains_VRMS_BPhase: public Test
{
    bool Run()
    {
        float shift       = PI*4/3;
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(shift, 600, snapshot,
                            snapshot.u16MainsBCnt, &AC_SENSE::MAINS_GetVoltageVolts,
                            310/sqrt(2), MAINS_CURRENT_MEASUREMENT, B_PHASE);
    }

    string TestDescription()
    {
        return "Test19_Mains_VRMS_BPhase";
    }
};


class Test20_Mains_IRMS_YPhase: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(0, 600, snapshot,
                            snapshot.u16YCurrentCnt, &AC_SENSE::MAINS_GetCurrentAmps,
                                2/sqrt(2), MAINS_CURRENT_MEASUREMENT, Y_PHASE, Sinusoid::CURRENT);
    }

    string TestDescription()
    {
        return "Test20_Mains_IRMS_YPhase";
    }
};


class Test21_Mains_IRMS_BPhase: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(0, 600, snapshot,
                            snapshot.u16BCurrentCnt, &AC_SENSE::MAINS_GetCurrentAmps,
                                2/sqrt(2), MAINS_CURRENT_MEASUREMENT, B_PHASE, Sinusoid::CURRENT);
    }

    string TestDescription()
    {
        return "Test21_Mains_IRMS_BPhase";
    }
};


class Test22_Mains_ActivePower: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_PowerTest(0, PI/6, 600, snapshot,
                           snapshot.u16MainsRCnt, snapshot.u16RCurrentCnt,
                            &AC_SENSE::MAINS_GetActivePowerWatts, 300*cos(PI/6),
                             MAINS_CURRENT_MEASUREMENT, true, R_PHASE);
    }

    string TestDescription()
    {
        return "Test22_Mains_ActivePower";
    }
};


class Test23_Mains_ApparentPower: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_PowerTest(0, PI/6, 600, snapshot,
                           snapshot.u16MainsYCnt, snapshot.u16YCurrentCnt,
                            &AC_SENSE::MAINS_GetApparentPowerVA, 300,
                             MAINS_CURRENT_MEASUREMENT, true, Y_PHASE);
    }

    string TestDescription()
    {
        return "Test23_Mains_ApparentPower";
    }
};


class Test24_Mains_ReactivePower: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_PowerTest(0, PI/6, 600, snapshot,
                           snapshot.u16MainsBCnt, snapshot.u16BCurrentCnt,
                            &AC_SENSE::MAINS_GetReactivePowerVAR, 300*sin(PI/6),
                             MAINS_CURRENT_MEASUREMENT, true, B_PHASE);
    }

    string TestDescription()
    {
        return "Test24_Mains_ReactivePower";
    }
};


class Test25_Mains_RPowerFactor: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_PowerTest(0, PI/6, 600, snapshot,
                           snapshot.u16MainsRCnt, snapshot.u16RCurrentCnt,
                            &AC_SENSE::MAINS_GetPowerFactor, -cos(PI/6),
                             MAINS_CURRENT_MEASUREMENT, false, R_PHASE);
    }

    string TestDescription()
    {
        return "Test25_Mains_RPowerFactor";
    }
};


class Test26_Mains_YPowerFactor: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_PowerTest(0, -PI/6, 600, snapshot,
                           snapshot.u16MainsYCnt, snapshot.u16YCurrentCnt,
                            &AC_SENSE::MAINS_GetPowerFactor, cos(PI/6),
                             MAINS_CURRENT_MEASUREMENT, false, Y_PHASE);
    }

    string TestDescription()
    {
        return "Test26_Mains_YPowerFactor";
    }
};


class Test27_Mains_BPowerFactor: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_PowerTest(0, PI/6, 600, snapshot,
                           snapshot.u16MainsBCnt, snapshot.u16BCurrentCnt,
                            &AC_SENSE::MAINS_GetPowerFactor, -cos(PI/6),
                             MAINS_CURRENT_MEASUREMENT, false, B_PHASE);
    }

    string TestDescription()
    {
        return "Test27_Mains_BPowerFactor";
    }
};


class Test28_Mains_RFrequency: public Test
{
    bool Run()
    {
        float shift       = 0;
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(shift, 600, snapshot,
                            snapshot.u16MainsRCnt, &AC_SENSE::MAINS_GetApproxFreq,
                            50, MAINS_CURRENT_MEASUREMENT, R_PHASE);
    }

    string TestDescription()
    {
        return "Test28_Mains_RFrequency";
    }
};


class Test29_Mains_YFrequency: public Test
{
    bool Run()
    {
        float shift       = 0;
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(shift, 600, snapshot,
                            snapshot.u16MainsYCnt, &AC_SENSE::MAINS_GetApproxFreq,
                            50, MAINS_CURRENT_MEASUREMENT, Y_PHASE);
    }

    string TestDescription()
    {
        return "Test29_Mains_YFrequency";
    }
};


class Test30_Mains_BFrequency: public Test
{
    bool Run()
    {
        float shift       = 0;
        AC_IP::AC_PARAMS_t snapshot;
        return Base_RmsTest(shift, 600, snapshot,
                            snapshot.u16MainsBCnt, &AC_SENSE::MAINS_GetApproxFreq,
                            50, MAINS_CURRENT_MEASUREMENT, B_PHASE);
    }

    string TestDescription()
    {
        return "Test30_Mains_BFrequency";
    }
};


class Test31_ActiveEnergy: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_EnergyTest(0, PI/6, 600, snapshot,
                           snapshot.u16GensetRCnt, snapshot.u16RCurrentCnt,
                            &AC_SENSE::GENSET_GetActiveEnergyWH, 300*cos(PI/6)*12/3600,
                             GENSET_CURRENT_MEASUREMENT, true, R_PHASE);
    }

    string TestDescription()
    {
        return "Test31_ActiveEnergy";
    }
};


class Test32_ApparentEnergy: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_EnergyTest(0, PI/6, 600, snapshot,
                           snapshot.u16GensetYCnt, snapshot.u16YCurrentCnt,
                            &AC_SENSE::GENSET_GetApparentEnergyVAH, 300*(11.4f)/3600,
                             GENSET_CURRENT_MEASUREMENT, true, Y_PHASE);
    }

    string TestDescription()
    {
        return "Test32_ApparentEnergy";
    }
};


class Test33_ReactiveEnergy: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_EnergyTest(0, PI/6, 600, snapshot,
                           snapshot.u16GensetBCnt, snapshot.u16BCurrentCnt,
                            &AC_SENSE::GENSET_GetReactiveEnergyVARH, 300*sin(PI/6)*12/3600,
                             GENSET_CURRENT_MEASUREMENT, true, B_PHASE);
    }

    string TestDescription()
    {
        return "Test33_ReactiveEnergy";
    }
};


class Test34_Mains_ActiveEnergy: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_EnergyTest(0, PI/6, 600, snapshot,
                           snapshot.u16MainsRCnt, snapshot.u16RCurrentCnt,
                            &AC_SENSE::MAINS_GetActiveEnergyWH, 300*cos(PI/6)*12/3600,
                             MAINS_CURRENT_MEASUREMENT, true, R_PHASE);
    }

    string TestDescription()
    {
        return "Test34_Mains_ActiveEnergy";
    }
};


class Test35_Mains_ApparentEnergy: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_EnergyTest(0, PI/6, 600, snapshot,
                           snapshot.u16MainsYCnt, snapshot.u16YCurrentCnt,
                            &AC_SENSE::MAINS_GetApparentEnergyVAH, 300*12/3600,
                             MAINS_CURRENT_MEASUREMENT, true, Y_PHASE);
    }

    string TestDescription()
    {
        return "Test35_Mains_ApparentEnergy";
    }
};


class Test36_Mains_ReactiveEnergy: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_EnergyTest(0, PI/6, 600, snapshot,
                           snapshot.u16MainsBCnt, snapshot.u16BCurrentCnt,
                            &AC_SENSE::MAINS_GetReactiveEnergyVARH, 300*sin(PI/6)*12/3600,
                             MAINS_CURRENT_MEASUREMENT, true, B_PHASE);
    }

    string TestDescription()
    {
        return "Test36_Mains_ReactiveEnergy";
    }
};


class Test37_PhaseReversalPresent: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
    bool Expected = false;
        return (Expected == Base_PhaseRotTest(0, -PI*2/3, -PI*4/3, 600, snapshot,
                           snapshot.u16GensetRCnt, snapshot.u16GensetYCnt,
               snapshot.u16GensetBCnt, 
               &AC_SENSE::GENSET_GetPhaseRotStatus));
    }

    string TestDescription()
    {
        return "Test37_PhaseReversalPresent";
    }
};

class Test38_PhaseReversalAbsent: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        bool Expected = true;
        return (Expected == Base_PhaseRotTest(0, -PI*4/3, -PI*2/3, 600, snapshot,
                           snapshot.u16GensetRCnt, snapshot.u16GensetYCnt,
                           snapshot.u16GensetBCnt,
                           &AC_SENSE::GENSET_GetPhaseRotStatus));
    }

    string TestDescription()
    {
        return "Test37_PhaseReversalAbsent";
    }
};


class Test39_Mains_PhaseReversalPresent: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        bool Expected = false;
        return (Expected == Base_PhaseRotTest(0, -PI*2/3, -PI*4/3, 600, snapshot,
                           snapshot.u16MainsRCnt, snapshot.u16MainsYCnt,
                           snapshot.u16MainsBCnt,
                           &AC_SENSE::MAINS_GetPhaseRotStatus));
    }

    string TestDescription()
    {
        return "Test39_Mains_PhaseReversalPresent";
    }
};

class Test40_Mains_PhaseReversalAbsent: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        bool Expected = true;
        return (Expected == Base_PhaseRotTest(-PI*4/3, -PI*2/3, 0, 600, snapshot,
                           snapshot.u16MainsRCnt, snapshot.u16MainsYCnt,
                           snapshot.u16MainsBCnt,
                           &AC_SENSE::MAINS_GetPhaseRotStatus));
    }

    string TestDescription()
    {
        return "Test40_Mains_PhaseReversalAbsent";
    }
};


class Test41_TotalActiveEnergy: public Test
{
public:
    bool Run()
    {
        int noOfCycles = 1000000;
        AC_IP::AC_PARAMS_t snapshot;
        return Base_TotalEnergyTest(0, PI/6, 0, PI/6, 0, PI/6, noOfCycles, snapshot,
                           snapshot.u16GensetRCnt, snapshot.u16RCurrentCnt,
               snapshot.u16GensetYCnt, snapshot.u16YCurrentCnt,
               snapshot.u16GensetBCnt, snapshot.u16BCurrentCnt,
                            &AC_SENSE::GENSET_GetTotalActiveEnergySinceInitWH,
                3*300*cos(PI/6)*(noOfCycles/50)/3600, GENSET_CURRENT_MEASUREMENT, true);
    }

    string TestDescription()
    {
        return "Test41_TotalActiveEnergy";
    }
};


class Test42_Mains_TotalApparentEnergy: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        return Base_TotalEnergyTest(0, PI/6, 0, PI/6, 0, PI/6, 600, snapshot,
                           snapshot.u16MainsRCnt, snapshot.u16RCurrentCnt,
                           snapshot.u16MainsYCnt, snapshot.u16YCurrentCnt,
                           snapshot.u16MainsBCnt, snapshot.u16BCurrentCnt,
                            &AC_SENSE::MAINS_GetTotalApparentEnergyVAH,
                            3*300*12/3600, MAINS_CURRENT_MEASUREMENT, true);
    }

    string TestDescription()
    {
        return "Test42_Mains_TotalApparentEnergy";
    }
};

class Test43_Mains_PhaseToPhase: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        
        float ExpectedValue = (300.0/sqrt(2.0))*sqrt(3.0);

        return Base_PowerPhaseToPhaseRMS(0, (2*PI)/3, 600, snapshot,
                           snapshot.u16MainsRCnt,
                           snapshot.u16MainsYCnt,
                            &AC_SENSE::MAINS_GetRYVolts,
                            ExpectedValue);
    }

    string TestDescription()
    {
        return "Test43_Mains_PhaseToPhase";
    }
};


class Test44_Genset_PhaseToPhase: public Test
{
public:
    bool Run()
    {
        AC_IP::AC_PARAMS_t snapshot;
        
        float ExpectedValue = (300.0/sqrt(2.0))*sqrt(3.0);

        return Base_PowerPhaseToPhaseRMS(0, (2*PI)/3, 600, snapshot,
                           snapshot.u16GensetYCnt,
                           snapshot.u16GensetBCnt,
                            &AC_SENSE::GENSET_GetYBVolts,
                            ExpectedValue);
    }

    string TestDescription()
    {
        return "Test44_Mains_PhaseToPhase";
    }
};

bool Base_PowerPhaseToPhaseRMS(float shiftV1, float shiftV2, 
                                int cycle_count,
                                    AC_IP::AC_PARAMS_t &sampleSet, 
                                    uint16_t &sampleV1, uint16_t &sampleV2, 
                                    float (AC_SENSE::*fc)(), float ExpectedValue)
{
    float amplitudeV1  = 300;
    float amplitudeV2  = 300;
    float sample_freq  = 5000;
    float sine_freq    = 50;

    Sinusoid s1(amplitudeV1, sample_freq, sine_freq, shiftV1, cycle_count);
    Sinusoid s2(amplitudeV2, sample_freq, sine_freq, shiftV2, cycle_count);

    AC_IP ip;
    AC_SENSE ac_params(ip);
    ac_params.ConfigureCTRatio(CT_RATIO);

    sampleSet.u16GensetNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);
    sampleSet.u16MainsNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);

    for(int i = 0; i < s1.GetSampleCount(); i++)
    {
        sampleV1 = s1.GetSample(i);
        sampleV2 = s2.GetSample(i);
        ac_params.UpdateACData(&sampleSet);
        ac_params.Update();
    }

    float ComputedValue = ((&ac_params)->*fc)();

    cout<<"Computed Value:"<<ComputedValue<<endl;
    cout<<"Expected Value:"<<ExpectedValue<<endl;

    if(abs(ComputedValue-ExpectedValue) > MAX_ERROR)
    {
        return false;
    }
    return true;
}


bool Base_RmsTest(float shift, int cycle_count, AC_IP::AC_PARAMS_t &sampleSet,
                    uint16_t &sample, float (AC_SENSE::*fc)(PHASE_t),
            float ExpectedValue, I_CFG_t eICfg, PHASE_t Phase,
                                 Sinusoid::INPUT_TYPE_t InputParam)
{
    float amplitude;
    float sample_freq = 5000;
    float sine_freq   = 50;
    if(InputParam==Sinusoid::CURRENT)
    {
       ExpectedValue*=CT_MUL_FACTOR;
       amplitude = 2;
    }
    else
    {
       amplitude = 310;
    }

    Sinusoid s1(amplitude, sample_freq, sine_freq, shift, cycle_count, InputParam);

    AC_IP ip;
    AC_SENSE ac_params(ip);
    ac_params.ConfigureCTRatio(CT_RATIO);
    ac_params.ConfigureISensing(eICfg);

    sampleSet.u16GensetNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);
    sampleSet.u16MainsNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);

    for(int i = 0; i < s1.GetSampleCount(); i++)
    {
        sample = s1.GetSample(i);
        ac_params.UpdateACData(&sampleSet);
        ac_params.Update();
    }

    float ComputedValue = ((&ac_params)->*fc)(Phase);

    cout<<"Computed Value:"<<ComputedValue<<endl;
    cout<<"Expected Value:"<<ExpectedValue<<endl;

    if(abs(ComputedValue-ExpectedValue) > MAX_ERROR)
    {
        return false;
    }
    return true;
}


bool Base_PowerTest(float shiftV, float shiftI, 
                        int cycle_count,
                           AC_IP::AC_PARAMS_t &sampleSet, 
                              uint16_t &sampleV, uint16_t &sampleI, 
                                 float (AC_SENSE::*fc)(PHASE_t), float ExpectedValue,
                                    I_CFG_t eICfg, bool considerCtMultiplier,
                                              PHASE_t Phase)
{
    float amplitudeV  = 300;
    float amplitudeI  = 2;
    float sample_freq = 5000;
    float sine_freq   = 50;

    Sinusoid s1(amplitudeV, sample_freq, sine_freq, shiftV, cycle_count);
    Sinusoid s2(amplitudeI, sample_freq, sine_freq, shiftI, cycle_count, Sinusoid::CURRENT);

    AC_IP ip;
    AC_SENSE ac_params(ip);

    ac_params.ConfigureCTRatio(CT_RATIO);
    ac_params.ConfigureISensing(eICfg);

    sampleSet.u16GensetNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);
    sampleSet.u16MainsNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);

    for(int i = 0; i < s1.GetSampleCount(); i++)
    {
        sampleV = s1.GetSample(i);
        sampleI = s2.GetSample(i);
        ac_params.UpdateACData(&sampleSet);
        ac_params.Update();
    }

    float ComputedValue = ((&ac_params)->*fc)(Phase);
    if(considerCtMultiplier)
    {
        ExpectedValue *= CT_MUL_FACTOR;
    }
    cout<<"Computed Value:"<<ComputedValue<<endl;
    cout<<"Expected Value:"<<ExpectedValue<<endl;

    if(abs(ComputedValue-ExpectedValue) > MAX_ERROR)
    {
        return false;
    }
    return true;
}

bool Base_EnergyTest(float shiftV, float shiftI, 
                    int cycle_count,
                    AC_IP::AC_PARAMS_t &sampleSet, 
                    uint16_t &sampleV, uint16_t &sampleI, 
                    double (AC_SENSE::*fc)(PHASE_t), float ExpectedValue,
                    I_CFG_t eICfg, bool considerCtMultiplier, PHASE_t Phase)
{
    float amplitudeV  = 300;
    float amplitudeI  = 2;
    float sample_freq = 5000;
    float sine_freq   = 50;

    Sinusoid s1(amplitudeV, sample_freq, sine_freq, shiftV, cycle_count);
    Sinusoid s2(amplitudeI, sample_freq, sine_freq, shiftI, cycle_count, Sinusoid::CURRENT);

    AC_IP ip;
    AC_SENSE ac_params(ip);

    ac_params.ConfigureCTRatio(CT_RATIO);
    ac_params.ConfigureISensing(eICfg);

    sampleSet.u16GensetNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);
    sampleSet.u16MainsNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);

    for(int i = 0; i < s1.GetSampleCount(); i++)
    {
        sampleV = s1.GetSample(i);
        sampleI = s2.GetSample(i);
        ac_params.UpdateACData(&sampleSet);
        ac_params.Update();
    }

    float ComputedValue = ((&ac_params)->*fc)(Phase);
    if(considerCtMultiplier)
    {
        ExpectedValue *= CT_MUL_FACTOR;
    }
    cout<<"Computed Value:"<<ComputedValue<<endl;
    cout<<"Expected Value:"<<ExpectedValue<<endl;

    if(abs(ComputedValue-ExpectedValue) > MAX_ERROR)
    {
        return false;
    }
    return true;
}                    


bool Base_TotalPowerTest(float shiftVR, float shiftIR, float shiftVY, 
                float shiftIY, float shiftVB, float shiftIB, 
            int cycle_count, AC_IP::AC_PARAMS_t &sampleSet,
                              uint16_t &sampleVR, uint16_t &sampleIR,
                  uint16_t &sampleVY, uint16_t &sampleIY,
                  uint16_t &sampleVB, uint16_t &sampleIB,
                                 float (AC_SENSE::*fc)(), float ExpectedValue,
                                    I_CFG_t eICfg, bool considerCtMultiplier)
{
    float amplitudeV  = 300;
    float amplitudeI  = 2;
    float sample_freq = 5000;
    float sine_freq   = 50;

    Sinusoid s1R(amplitudeV, sample_freq, sine_freq, shiftVR, cycle_count);
    Sinusoid s2R(amplitudeI, sample_freq, sine_freq, shiftIR, cycle_count, Sinusoid::CURRENT);
    Sinusoid s1Y(amplitudeV, sample_freq, sine_freq, shiftVY, cycle_count);
    Sinusoid s2Y(amplitudeI, sample_freq, sine_freq, shiftIY, cycle_count, Sinusoid::CURRENT);
    Sinusoid s1B(amplitudeV, sample_freq, sine_freq, shiftVB, cycle_count);
    Sinusoid s2B(amplitudeI, sample_freq, sine_freq, shiftIB, cycle_count, Sinusoid::CURRENT);

    AC_IP ip;
    AC_SENSE ac_params(ip);
    

    ac_params.ConfigureCTRatio(CT_RATIO);
    ac_params.ConfigureISensing(eICfg);

    sampleSet.u16GensetNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);
    sampleSet.u16MainsNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);


    for(int i = 0; i < s1R.GetSampleCount(); i++)
    {
        sampleVR = s1R.GetSample(i);
        sampleIR = s2R.GetSample(i);
        sampleVY = s1Y.GetSample(i);
        sampleIY = s2Y.GetSample(i);
        sampleVB = s1B.GetSample(i);
        sampleIB = s2B.GetSample(i);
        ac_params.UpdateACData(&sampleSet);
        ac_params.Update();
    }

    float ComputedValue = ((&ac_params)->*fc)();
    if(considerCtMultiplier)
    {
        ExpectedValue *= CT_MUL_FACTOR;
    }
    cout<<"Computed Value:"<<ComputedValue<<endl;
    cout<<"Expected Value:"<<ExpectedValue<<endl;

    if(abs(ComputedValue-ExpectedValue) > MAX_ERROR)
    {
        return false;
    }
    return true;
}

bool Base_TotalEnergyTest(float shiftVR, float shiftIR, float shiftVY,
                        float shiftIY, float shiftVB, float shiftIB,
                        int cycle_count, AC_IP::AC_PARAMS_t &sampleSet,
                              uint16_t &sampleVR, uint16_t &sampleIR,
                              uint16_t &sampleVY, uint16_t &sampleIY,
                              uint16_t &sampleVB, uint16_t &sampleIB,
                                 double (AC_SENSE::*fc)(), float ExpectedValue,
                                    I_CFG_t eICfg, bool considerCtMultiplier)
{
    float amplitudeV  = 300;
    float amplitudeI  = 2;
    float sample_freq = 5000;
    float sine_freq   = 50;

    Sinusoid s1R(amplitudeV, sample_freq, sine_freq, shiftVR, cycle_count);
    Sinusoid s2R(amplitudeI, sample_freq, sine_freq, shiftIR, cycle_count, Sinusoid::CURRENT);
    Sinusoid s1Y(amplitudeV, sample_freq, sine_freq, shiftVY, cycle_count);
    Sinusoid s2Y(amplitudeI, sample_freq, sine_freq, shiftIY, cycle_count, Sinusoid::CURRENT);
    Sinusoid s1B(amplitudeV, sample_freq, sine_freq, shiftVB, cycle_count);
    Sinusoid s2B(amplitudeI, sample_freq, sine_freq, shiftIB, cycle_count, Sinusoid::CURRENT);

    AC_IP ip;
    AC_SENSE ac_params(ip);
    

    ac_params.ConfigureCTRatio(CT_RATIO);
    ac_params.ConfigureISensing(eICfg);

    sampleSet.u16GensetNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);
    sampleSet.u16MainsNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);


    for(int i = 0; i < s1R.GetSampleCount(); i++)
    {
        sampleVR = s1R.GetSample(i);
        sampleIR = s2R.GetSample(i);
        sampleVY = s1Y.GetSample(i);
        sampleIY = s2Y.GetSample(i);
        sampleVB = s1B.GetSample(i);
        sampleIB = s2B.GetSample(i);
        ac_params.UpdateACData(&sampleSet);
        ac_params.Update();
    }

    double ComputedValue = ((&ac_params)->*fc)();
    if(considerCtMultiplier)
    {
        ExpectedValue *= CT_MUL_FACTOR;
    }
    cout<<"Computed Value:"<<ComputedValue<<endl;
    cout<<"Expected Value:"<<ExpectedValue<<endl;

    if(abs(ComputedValue-ExpectedValue) > MAX_ERROR)
    {
        return false;
    }
    return true;
}                                    


bool Base_PhaseRotTest(float shiftR, float shiftY, float shiftB, int cycle_count,
                           AC_IP::AC_PARAMS_t &sampleSet,
                              uint16_t &sampleVR, uint16_t &sampleVY,
                  uint16_t &sampleVB, bool (AC_SENSE::*fc)())
{
    float amplitude  = 300;
    float sample_freq = 5000;
    float sine_freq   = 50;

    Sinusoid s1(amplitude, sample_freq, sine_freq, shiftR, cycle_count);
    Sinusoid s2(amplitude, sample_freq, sine_freq, shiftY, cycle_count);
    Sinusoid s3(amplitude, sample_freq, sine_freq, shiftB, cycle_count);

    AC_IP ip;
    AC_SENSE ac_params(ip);
    sampleSet.u16GensetNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);
    sampleSet.u16MainsNCnt = 1.5*(BSP_ADC_QUANTIZATION_LEVELS/BSP_ADC_REF_VTG);


    for(int i = 0; i < s1.GetSampleCount(); i++)
    {
        sampleVR = s1.GetSample(i);
        sampleVY = s2.GetSample(i);
        sampleVB = s3.GetSample(i);
        ac_params.UpdateACData(&sampleSet);
        ac_params.Update();
    }

    bool result = ((&ac_params)->*fc)();
    if(result)
    {
        cout<<"Phase is reversed"<<endl;
    }
    else
    {
        cout<<"Phase is not reversed"<<endl;
    }
    return result;
}


vector<Test*>& CreateTestSuite()
{
    testList.push_back(new Test1_VRMS_RPhase);
    testList.push_back(new Test2_IRMS_RPhase);
    testList.push_back(new Test3_VRMS_YPhase);
    testList.push_back(new Test4_VRMS_BPhase);
    testList.push_back(new Test5_IRMS_YPhase);
    testList.push_back(new Test6_IRMS_BPhase);
    testList.push_back(new Test7_ActivePower);
    testList.push_back(new Test8_ApparentPower);
    testList.push_back(new Test9_ReactivePower);
    testList.push_back(new Test10_RPowerFactor);
    testList.push_back(new Test11_YPowerFactor);
    testList.push_back(new Test12_BPowerFactor);
    testList.push_back(new Test13_RFrequency);
    testList.push_back(new Test14_YFrequency);
    testList.push_back(new Test15_BFrequency);
    testList.push_back(new Test16_Mains_VRMS_RPhase);
    testList.push_back(new Test17_Mains_IRMS_RPhase);
    testList.push_back(new Test18_Mains_VRMS_YPhase);
    testList.push_back(new Test19_Mains_VRMS_BPhase);
    testList.push_back(new Test20_Mains_IRMS_YPhase);
    testList.push_back(new Test21_Mains_IRMS_BPhase);
    testList.push_back(new Test22_Mains_ActivePower);
    testList.push_back(new Test23_Mains_ApparentPower);
    testList.push_back(new Test24_Mains_ReactivePower);
    testList.push_back(new Test25_Mains_RPowerFactor);
    testList.push_back(new Test26_Mains_YPowerFactor);
    testList.push_back(new Test27_Mains_BPowerFactor);
    testList.push_back(new Test28_Mains_RFrequency);
    testList.push_back(new Test29_Mains_YFrequency);
    testList.push_back(new Test30_Mains_BFrequency);
    testList.push_back(new Test31_ActiveEnergy);
    testList.push_back(new Test32_ApparentEnergy);
    testList.push_back(new Test33_ReactiveEnergy);
    testList.push_back(new Test34_Mains_ActiveEnergy);
    testList.push_back(new Test35_Mains_ApparentEnergy);
    testList.push_back(new Test36_Mains_ReactiveEnergy);
    testList.push_back(new Test37_PhaseReversalPresent);
    testList.push_back(new Test38_PhaseReversalAbsent);
    testList.push_back(new Test39_Mains_PhaseReversalPresent);
    testList.push_back(new Test40_Mains_PhaseReversalAbsent);
    testList.push_back(new Test41_TotalActiveEnergy);
    testList.push_back(new Test42_Mains_TotalApparentEnergy);
    testList.push_back(new Test43_Mains_PhaseToPhase);
    testList.push_back(new Test44_Genset_PhaseToPhase);
    return testList;
}
