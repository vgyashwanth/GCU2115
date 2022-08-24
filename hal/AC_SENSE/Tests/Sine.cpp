#include "Sine.h"
using namespace std;


#define ADC_QUANTIZATION_LEVELS     (4096.0F)
#define ADC_REFERENCE_VOLTAGE       (3.0F)
#define R1_K_OHM                    (4000.0F)
#define R2_K_OHM                    (9.09F)
#define PI                          (3.14f)
#define V_ANLG_FRONTEND_MULTIPLIER  (R2_K_OHM/(R1_K_OHM+R2_K_OHM))
#define V_TO_ADC_CNT_MULTIPLIER     (ADC_QUANTIZATION_LEVELS/3.0F)
#define I_ANLG_FRONTEND_MULTIPLIER  (1.0F/10.0F)

Sinusoid::Sinusoid(float fAmplitudeV, float fSampleFreqHz, 
                       float fSineFreqHz, float fShiftRadians, int _cycle_count,
		       		INPUT_TYPE_t InputParam):
cycle_count(_cycle_count), 
sample_no((int)(fSampleFreqHz/fSineFreqHz)), 
values(new double[sample_no]),
pu16Res(new uint16_t[sample_no])
{
    memset(values, 0, sample_no);
    PopulateSamples(fAmplitudeV, fSineFreqHz, fShiftRadians, fSampleFreqHz);
    if(!InputParam)
	    ConvertSamplesV();
    else
	    ConvertSamplesI();
}

Sinusoid::~Sinusoid() 
{
    delete[] values;
}

void Sinusoid::PopulateSamples(float fAmplitudeV, float fSineFreqHz, 
                                    float fShiftRadians, float fSampleFreqHz) 
{
    for(int t = 0 ; t < sample_no; t++) 
    {
        values[t] = 
                fAmplitudeV*sin(2*PI*t*fSineFreqHz/fSampleFreqHz + fShiftRadians);
    }
}

int Sinusoid::GetSampleCount() 
{
    return cycle_count * sample_no;
}

// iter Sinusoid::begin() 
// {
//     return pu16Res;
// }

// iter Sinusoid::end() 
// {
//     return pu16Res+sample_no;
// }

uint16_t Sinusoid::GetSample(int i)
{
    return pu16Res[i%sample_no];
}

void Sinusoid::ConvertSamplesV() 
{
    for(int t = 0 ; t < sample_no; t++) 
    {
        double tmp = (values[t]*V_ANLG_FRONTEND_MULTIPLIER+1.5f)*V_TO_ADC_CNT_MULTIPLIER;
        pu16Res[t] = (uint16_t)round(tmp);
    }
}

void Sinusoid::ConvertSamplesI()
{
    for(int t = 0 ; t < sample_no; t++)
    {
        double tmp = (values[t]*I_ANLG_FRONTEND_MULTIPLIER+1.5f)*V_TO_ADC_CNT_MULTIPLIER;
        pu16Res[t] = (uint16_t)round(tmp);
    }
}


