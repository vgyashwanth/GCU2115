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
#ifndef _RMS_H_
#define _RMS_H_

#include "stdint.h"
#include "AC_SENSE_CONFIG.h"
#include "AC_SENSE_DS.h"
#include "C_QUEUE.h"
#include "math.h"

class RMS {

public:
    typedef enum{
        VOLTAGE,
        CURRENT
    }SENSING_PARAM_t;

    /**
     * Constructs the class.
     * @param u16SamplesPerEntry - Defines the number of samples to be considered
     *                             for one entry in the window buffer.
     * @param fRmsMultiplicationFactor - The hardware multiplier factor.
     * @param _bisSqrtNeeded     - Determines weather a square root needs to be
     *                             performed in final step of the computation.
     *                             ex: square root calculation is needed if the 
     *                             parameter being calculated is RMS voltage.
     *                             The square root calculation is not needed for
     *                             active power calculation.
     * @param fValidityThreshold - Values above this are only considered as valid
     *                             RMS value. Otherwise, the RMS is set to 0.
     *  
     * @return
     * None
     */
    RMS(uint16_t u16SamplesPerEntry, 
                float fRmsMultiplicationFactor, 
                        bool _bisSqrtNeeded, float fValidityThreshold = 0.0, 
                                                    float fFilterConstant=1.0);

    /**
     * This function accumulates squared sample values.
     * @param i16SampleVal1 - First sample to be multiplied for RMS calculation
     * @param i16SampleVal2 - Second sample to be multiplied for RMS calculation
     * @return
     * true if the window has moved
     */
    bool  AccumulateSampleSet(int16_t i16SampleVal1, int16_t i16SampleVal2);


    /**
     * Returns the raw(i.e unfiltered) RMS value.
     * @param - None
     * @return
     * Computed RMS value
     */
    float GetRawRMS();

    /**
     * Returns the filtered RMS value.
     * @param - None
     * @return
     * Computed RMS value
     */
    float GetFilteredRMS(SENSING_PARAM_t SensedParameter);

    /**
     * Returns the filtered RMS value which should be used to display on monitoring screen.
     * @param - None
     * @return
     * Computed Display RMS value
     */
    float GetFilteredRMSValueForDisplay(SENSING_PARAM_t SensedParameter);

    /**
     * Expected to be called in the super-while loop.
     * @param - None
     * @return
     * None
     */
    void  Update();

    static void SetRMSFilterConstForDisplay(float FilterVal);

private:
    #define RMS_MOVING_WINDOW_SIZE        (16U)

    CircularQueue<int32_t> _windowBuffer;
    int32_t _ai32WindowArr[RMS_MOVING_WINDOW_SIZE];
    int64_t _i64SampleSum;
    /*This is used to accumulate the squared value of samples in SAMPLE_SET*/
    int32_t _i32AccumulatedVal;
    /*The accumulated value is latched to this value before it is reset*/
    int32_t _i32LastAccumulatedVal;
    /*Stores the filtered RMS value*/
    float   _fFilteredRms;

    /*
     * Below variable stores filtered value to be displayed on monitoring screen.
     * It should be only used for user display purpose. */
    float   _fFilteredRmsForDisplay;
    /*Stores raw(i.e unfiltered) RMS*/
    float   _fRawRms;
    /*This is a constant factor which when multiplied by the ADC count will
      give the the parameter that is being measured
     */
    float    _fRmsMultiplicationFactor;
    /*Maintains the count of samples since the window moved last*/
    uint16_t _u16RMSSampleCnt;
    /*Number of samples after which the window needs to be moved*/
    uint16_t _u16SamplesPerEntry;
    /*Flags whether the window moved*/
    bool     _bWindowMoved;

    bool     _bisSqrtNeeded;
    /*Threshold above which _fRms will be considered valid, else it will be made 0*/
    float    _fValidityThreshold;
    float    _fFilterConstant;
    static float _fDispFilterConstant;
    /**
     * Computes the RMS by averaging the accumulated squared sample values in the
     * sliding window.
     * @param - None
     * @return
     * None
     */
    void prvComputeRMS();
};
#endif
