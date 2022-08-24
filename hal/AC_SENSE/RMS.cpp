/**
 * @file        [RMS]
 * @brief       Encapsulates all the variables and functions necessary to compute
 *              the RMS of an AC parameter using moving window.
 * @version     [Version number]
 * @author      Naveen Ramaswamy, Sudeep Chandrasekaran
 * @date        3rd July 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#include "RMS.h"

/* This display filter constant is configurable, initializing it with 20% . ("SetRMSFilterConstForDisplay" function will update the value.)*/
float RMS::_fDispFilterConstant = 0.2f;

RMS::RMS(uint16_t u16SamplesPerEntry, float fRmsMultiplicationFactor,
                                    bool bisSqrtNeeded, float fValidityThreshold,
                                        float fFilterConstant)
:_windowBuffer{_ai32WindowArr, (uint16_t)RMS_MOVING_WINDOW_SIZE},
_ai32WindowArr{0},
_i64SampleSum(0),
_i32AccumulatedVal(0),
_i32LastAccumulatedVal(0),
_fFilteredRms(0),
_fFilteredRmsForDisplay(0),
_fRawRms(0),
_fRmsMultiplicationFactor(fRmsMultiplicationFactor),
_u16RMSSampleCnt(0),
_u16SamplesPerEntry(u16SamplesPerEntry),
_bWindowMoved(false),
_bisSqrtNeeded(bisSqrtNeeded),
_fValidityThreshold(fValidityThreshold),
_fFilterConstant(fFilterConstant)
{
}

bool RMS::AccumulateSampleSet(int16_t i16SampleVal1, int16_t i16SampleVal2)
{
    _u16RMSSampleCnt++;

    _i32AccumulatedVal += (i16SampleVal1*i16SampleVal2);

    /* Reset count as summing current SAMPLE_SET is over */
    if(_u16RMSSampleCnt >= _u16SamplesPerEntry)
    {

        /* The current SAMPLE_SET sum(i.e _i32LastAccumulatedVal) is copied to
           _i32LastAccumulatedVal and cleared. _i32LastAccumulatedVal values is
           used within the update loop to derive the RMS for the current window
         */

        _i32LastAccumulatedVal = _i32AccumulatedVal;
        _i32AccumulatedVal     = 0;

        _u16RMSSampleCnt = 0;
        /* The _bWindowMoved flag signals to the update loop that the window
           has moved
         */
        _bWindowMoved = true;
        return true;
    }
    return false;
}

void RMS::prvComputeRMS()
{
    /* If we don't have RMS_MOVING_WINDOW_SIZE of samples, fill the _windowBuffer
     * to with the same _i32LastAccumulatedVal. This while loop runs for 
     * RMS_MOVING_WINDOW_SIZE times once after initialization.
     */
    while(_windowBuffer.GetRemainingQSize() != RMS_MOVING_WINDOW_SIZE)
    {
        _i64SampleSum += _i32LastAccumulatedVal;
        /* Populate the accumulated value for the last SAMPLE_SET to the
           window buffer */
        _windowBuffer.EnQueue(&_i32LastAccumulatedVal);
    }
    int32_t i32DequedVal;
    _windowBuffer.DeQueue(&i32DequedVal);
    /* Remove older sample from the sum*/
    _i64SampleSum -= i32DequedVal;
    /* Add newer sample to the sum */
    _i64SampleSum += _i32LastAccumulatedVal;
    /* Populate the accumulated value for the last SAMPLE_SET to the
       window buffer */
    _windowBuffer.EnQueue(&_i32LastAccumulatedVal);

    /* Compute RMS from the squared samples in the current window */
    float fInstaneousValue;
    /* Compute RMS */
    if(_bisSqrtNeeded)
    {
        fInstaneousValue = sqrt(float(_i64SampleSum/(RMS_MOVING_WINDOW_SIZE*
                                  _u16SamplesPerEntry)));
    }
    else
    {
        fInstaneousValue = (float(_i64SampleSum/(RMS_MOVING_WINDOW_SIZE*
                                  _u16SamplesPerEntry)));
    }
    fInstaneousValue *= _fRmsMultiplicationFactor;

    _fRawRms = fInstaneousValue;
    /*Apply the IIR filter*/
    _fFilteredRms = (float)( (_fFilterConstant*fInstaneousValue) + 
                                        ( (1.0-_fFilterConstant) * _fFilteredRms) );

    _fFilteredRmsForDisplay = (float)((_fDispFilterConstant *_fFilteredRmsForDisplay) +
                                            ((1.0 - _fDispFilterConstant) *_fFilteredRms ));

    _fFilteredRms = abs(_fFilteredRms);
    _fRawRms      = abs(_fRawRms);

    _fFilteredRmsForDisplay = abs(_fFilteredRmsForDisplay);

    /*This ensures that extremely low value of fRms is considered as zero*/
    if(_fRawRms < _fValidityThreshold)
    {
       _fFilteredRms = 0;
       _fRawRms      = 0;
       _fFilteredRmsForDisplay = 0;
    }
}

float RMS::GetRawRMS()
{
    return _fRawRms;
}
float RMS::GetFilteredRMSValueForDisplay(SENSING_PARAM_t SensedParameter)
{
    // This is to avoid the noise on voltage channel
   if(SensedParameter == VOLTAGE)
   {
       if(_fFilteredRmsForDisplay > 20.0)
       {
           return _fFilteredRmsForDisplay;
       }
       else
       {
           return 0.0;
       }
   }
   else
   {
       return _fFilteredRmsForDisplay;
   }
}
float RMS::GetFilteredRMS(SENSING_PARAM_t SensedParameter)
{
    // This is to avoid the noise on voltage channel
    if(SensedParameter == VOLTAGE)
    {
        if(_fFilteredRms > 20.0)
        {
            return _fFilteredRms;
        }
        else
        {
            return 0.0;
        }
    }
    else
    {
        return _fFilteredRms;
    }
}

void RMS::Update()
{
    if(_bWindowMoved)
    {
        prvComputeRMS();
        _bWindowMoved = false;
    }
}
void RMS::SetRMSFilterConstForDisplay(float FilterVal)
{
    _fDispFilterConstant = (float)(FilterVal/100.0f);
}
