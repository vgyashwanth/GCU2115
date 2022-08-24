#ifndef _Sine_H_
#define _Sine_H_

#include <iostream>
#include <cmath>
#include "stdint.h"
#include <cstring>


using namespace std;
typedef uint16_t* iter;

class Sinusoid {
    private:

    float    sample_freq;
    float    sine_freq;
    float    shift;
    int      cycle_count;
    int      sample_no;
    double   *values;
    uint16_t *pu16Res;

    public:

    typedef enum
    {
	    VOLTAGE,
	    CURRENT,
    } INPUT_TYPE_t;
   
    Sinusoid(float fAmplitudeV, float fSampleFreqHz, float fSineFreqHz, 
                                       float fShiftRadians=0, int _cycle_count=1,
					    INPUT_TYPE_t InputParam=VOLTAGE);

    ~Sinusoid();

    void PopulateSamples(float fAmplitudeV, float fSineFreqHz, 
                                    float fShiftRadians, float fSampleFreqHz);

    int GetSampleCount();

    uint16_t GetSample(int i);

    void ConvertSamplesV();

    void ConvertSamplesI();
};
#endif
