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

#include "A_SENSE.h"
#include "UTILS_MinMax.h"
#include "AC_SENSE_DS.h"

volatile bool      A_SENSE::_bStatusUpdateRPM                  = false;
volatile uint32_t  A_SENSE::_u32DiffCycleCnt                   = 0;
uint16_t  A_SENSE::_u16NumberOfTeeth                  = 0;
volatile uint16_t  A_SENSE::_u16NoOfPulsesToskip               = 0;
volatile uint16_t  A_SENSE::_u16PulsesPerCalcCycle             = 0;
volatile float     A_SENSE::_f32CompInputRPM                   = 0.0;
volatile float     A_SENSE::_f32FilteredRPM                    = 0.0;
volatile float     A_SENSE::_f32FilteredPulseIpRPM             = 0.0;
volatile float     A_SENSE::_f32CompGenRFreq                   = 0.0;
volatile float     A_SENSE::_f32RPhaseFreq                     = 0.0;
volatile float     A_SENSE::_f32GenRPhaseRawVtg                = 0.0;
stTimer A_SENSE::_RPMResetTimerPulseIp ={0};
uint8_t   A_SENSE::_u8NumberOfPoles                   = 0;
stTimer   A_SENSE::_RPMResetTimerComparator           ={0};
A_SENSE::pRPMUpdate_t     A_SENSE::_UpdateCompRPM_cb  = NULL;
stTimer A_SENSE::SpeedSensorTimer={0};
A_SENSE::A_SENSE(ANLG_IP &anlgIp, PULSE_IP &pulseIp, AC_IP &AcIp, AC_SENSE &AcSensors ):
_AnlgIp(anlgIp),
_PulseIp(pulseIp),
_AcIp(AcIp),
_AC_SENSE(AcSensors),
_PulseInputType(MPU_TYPE),
_f32FacWpointCntToRPM(0.0),
_UpdatePulseRPM_cb(NULL),
_f32PulseInputRPM(0.0),
_f32FiltVbat(0.0),
_f32FiltVBTSbat(0.0),
_f32FiltChargingAltVtg(0.0),
_sensors{anlgIp,anlgIp,anlgIp,anlgIp,anlgIp,anlgIp,anlgIp},
_bSpeedSensorFault(false)
{
    _PulseIp.RegisterPulseInputCB(prvUpdatePulseInpuRPM_Status);
    _AcIp.RegisterCompRPMCB(prvUpdateCompInpuFreqRPM);
    UTILS_ResetTimer(&_UpdateTimerVbat);
    UTILS_ResetTimer(&_UpdateTimer);
    UTILS_ResetTimer(&_RPMResetTimerPulseIp);
    UTILS_ResetTimer(&_RPMResetTimerComparator);
    UTILS_ResetTimer(&SpeedSensorTimer);
}

void A_SENSE::ConfigureSensor(CFG_t &cfg)
{
    AnalogSensor::pGetVal ArrGetVal[HAL_ENG_AI_END]=
                                {&ANLG_IP::GetPin11ResistanceOhms,
                                 &ANLG_IP::GetPin12ResistanceOhms,
                                 &ANLG_IP::GetPin13ResistanceOhms,
                                 &ANLG_IP::GetPin14ResistanceOhms,
                                 &ANLG_IP::GetPin15ResistanceOhms,
                                 &ANLG_IP::GetPin21Value,
                                 &ANLG_IP::GetPin23Value
                                };

    for(int i=0; i<HAL_ENG_AI_END; i++)
    {
        if(i == HAL_PIN_21)
        {
            if((cfg.stAIConfig[HAL_PIN_21].eSensor == AnalogSensor::A_SENSE_LUBE_OIL_PRESSURE_4_20)||
                    (cfg.stAIConfig[HAL_PIN_21].eSensor == AnalogSensor::A_SENSE_S3_4_20_SENSOR))
            {
                _AnlgIp.ConfigPin21(ANLG_IP::BSP_MODE_CURRENT);
            }
            else
            {
               _AnlgIp.ConfigPin21(ANLG_IP::BSP_MODE_VOLTAGE);
            }
        }
        if(i == HAL_PIN_23)
        {
            if(cfg.stAIConfig[HAL_PIN_23].eSensor == AnalogSensor::A_SENSE_S4_4_20_SENSOR)
            {
                _AnlgIp.ConfigPin23(ANLG_IP::BSP_MODE_CURRENT);
            }
            else
            {
               _AnlgIp.ConfigPin23(ANLG_IP::BSP_MODE_VOLTAGE);
            }
        }

        _sensors[i].CfgAsAnalogSensor(cfg.stAIConfig[i].eSensor,
                                     cfg.stAIConfig[i].stTable,
                                     ArrGetVal[i],
                                     cfg.stAIConfig[i].eRef);
    }


    _AnlgIp.ResetOffsetState();
}

void  A_SENSE::Update()
{
    ANLG_IP::ANALOG_VAL_t stVal;
    _f32GenRPhaseRawVtg = _AC_SENSE.GENSET_GetVoltageVoltsRaw(R_PHASE);
    _f32RPhaseFreq = _AC_SENSE.GENSET_GetApproxFreq(R_PHASE);
    if(_bStatusUpdateRPM)
    {
        _bStatusUpdateRPM = false;

        if(_PulseInputType == MPU_TYPE)
        {
            _f32PulseInputRPM = (float)((double)TMR1_CLK_FREQ *
                                                    PER_MIN_CNT_IN_SEC / _u32DiffCycleCnt);
        }
        else if(_PulseInputType == W_POINT_TYPE)
        {
            _f32PulseInputRPM = (float)(((double)TMR1_CLK_FREQ *
                                                    PER_MIN_CNT_IN_SEC * _f32FacWpointCntToRPM)
                                                    / _u32DiffCycleCnt);
        }

        if(_UpdatePulseRPM_cb != NULL)
        {
            _UpdatePulseRPM_cb(_f32PulseInputRPM);
        }
    }

    if(UTILS_GetElapsedTimeInMs(&_UpdateTimerVbat) >= UPDATE_Vbat_VALUE_TIME_MS)
    {
        stVal = _AnlgIp.GetVbat_V();
        //IIR filter
        _f32FiltVbat =  ((VBAT_FILTER_CONST) * stVal.f32InstSensorVal)
                        + ((1.0F - VBAT_FILTER_CONST) * _f32FiltVbat);
        UTILS_ResetTimer(&_UpdateTimerVbat);
    }

    if(UTILS_GetElapsedTimeInMs(&_UpdateTimer) >= UPDATE_ANGL_VALUE_TIME_MS)
    {
        stVal = _AnlgIp.GetChargingAltVtg_V();
        //IIR filter
        _f32FiltChargingAltVtg = (CA_FILTER_CONST - CA_ROUND_OF_CONST)
                                        * _f32FiltChargingAltVtg
                               + ((1.0F - CA_FILTER_CONST) + CA_ROUND_OF_CONST)
                                  * (float)stVal.f32InstSensorVal;
        /*Update VBTS Batt voltage
         * */
        _f32FiltVBTSbat =  ((VBTS_BAT_FILTER_CONST) * _AnlgIp.GetBTSVtg() )
                        + ((1.0F - VBTS_BAT_FILTER_CONST) * _f32FiltVBTSbat);

        //Update all analog sensor values
        for(int i=0;i<HAL_ENG_AI_END;i++)
        {
            _sensors[i].Update();
        }

        //To avoid noise comparator input should be considered when
        //R-Phase voltage is greater than MIN_COMPARTOR_SENSE_VTG
        if(_AC_SENSE.GENSET_GetVoltageVoltsRaw(R_PHASE) > MIN_COMPARTOR_SENSE_VTG)
        {
            _f32FilteredRPM = ((_f32FilteredRPM*0.8f) + (_f32CompInputRPM*0.2f));
        }
        else
        {
            _f32FilteredRPM = 0;
        }
        _f32FilteredPulseIpRPM = ((_f32FilteredPulseIpRPM*0.8f) + (_f32PulseInputRPM*0.2f));
        UTILS_ResetTimer(&_UpdateTimer);

    }

    //Clear the Pulse input RPM when there is no Pulse input
    if(UTILS_GetElapsedTimeInMs(&_RPMResetTimerPulseIp) >= RPM_CLEAR_TIMER_MS)
    {
        _f32PulseInputRPM = 0;
        _f32FilteredPulseIpRPM = 0;
    }

    //Clear the Comparator input RPM and frequency when there is no Pulse input
    if(UTILS_GetElapsedTimeInMs(&_RPMResetTimerComparator) >= RPM_CLEAR_TIMER_MS)
    {
        _f32CompInputRPM = 0;
        _f32CompGenRFreq = 0;
        _f32FilteredRPM = 0;
    }

    if(UTILS_GetElapsedTimeInMs(&SpeedSensorTimer) >= SPEED_FLT_DETECT_DLY)
    {
    _bSpeedSensorFault = true;
    }
    else
    {
    _bSpeedSensorFault =false;
    }
}


float A_SENSE::GetFilteredVbattVolts()
{
    return _f32FiltVbat;
}
float A_SENSE::GetFilteredVBTSbattVolts()
{
    return _f32FiltVBTSbat;
}

A_SENSE::SENSOR_RET_t A_SENSE::GetSensorValue(AnalogSensor::TYPS_t eSensor)
{
    SENSOR_RET_t ret;

    for(int i=0; i<HAL_ENG_AI_END; i++)
    {
        if(_sensors[i].GetName()== eSensor)
        {
            ret.stValAndStatus = _sensors[i].GetValue();
            ret.eStatus = SENSOR_READ_SUCCESS;
            return ret;
        }
    }

    ret.eStatus = SENSOR_NOT_CONFIGRUED;

    return ret;
}

float A_SENSE::GetFilteredChargingAltVolts()
{
    return _f32FiltChargingAltVtg;
}


void A_SENSE::RegisterPulseInputUpdateCB(pRPMUpdate_t pCB)
{
    _UpdatePulseRPM_cb = pCB;
}


void A_SENSE::RegisterCompInputUpdateCB(pRPMUpdate_t pCB)
{
    _UpdateCompRPM_cb = pCB;
}


void A_SENSE::ConfigPulseInput(uint16_t u16NoOfTeethOrFreq,  PULSE_IP_TYPE_t etype)
{
    #define W_POINT_FREQ_AT_RATED_RPM     1500

    if(etype == MPU_TYPE)
    {
        _u16NumberOfTeeth = u16NoOfTeethOrFreq;
        if(u16NoOfTeethOrFreq > 1)
        {
            // For 1/2 cyclic measurement
            _u16PulsesPerCalcCycle = u16NoOfTeethOrFreq / 2 ;
        }
        else
        {
            _u16PulsesPerCalcCycle = u16NoOfTeethOrFreq ;
        }
    }
    else if(etype == W_POINT_TYPE)
    {
        // Calculate the pulses 1/2 cyclic measurement.
        _u16PulsesPerCalcCycle = (uint16_t)(u16NoOfTeethOrFreq * PER_MIN_CNT_IN_SEC / W_POINT_FREQ_AT_RATED_RPM);
        _u16NumberOfTeeth = _u16PulsesPerCalcCycle;
        _f32FacWpointCntToRPM = (float)_u16PulsesPerCalcCycle /
                                ((float)u16NoOfTeethOrFreq * PER_MIN_CNT_IN_SEC / W_POINT_FREQ_AT_RATED_RPM);
    }

    _PulseInputType = etype;
}

void A_SENSE::SkipPulses(uint16_t u16NumberofPulses)
{
    _u16NoOfPulsesToskip = u16NumberofPulses;
}


bool A_SENSE::GetSpeedSensFltStatus()
{
    return _bSpeedSensorFault;
}
void A_SENSE::prvUpdatePulseInpuRPM_Status( uint32_t u32TimerLatchVal)
{
    #define N_CALC_CYCLES   2U    //bi-cyclic measurement

    static uint8_t u8CycleCnt = 0;
    static uint16_t u16PulsesCnt = 0;
    static uint32_t u32ArrPrevCounter[N_CALC_CYCLES]={0},
                  u32ArrCurrentCounter[N_CALC_CYCLES]={0};  // Array taken to store the timer counts


    // Skip pulses at power on to avoid pulse input driver IC misbehavior
    if(_u16NoOfPulsesToskip == 0)
    {
        u16PulsesCnt++;
        if(u16PulsesCnt >= A_SENSE::_u16PulsesPerCalcCycle)
        {
            u32ArrPrevCounter[u8CycleCnt]    = u32ArrCurrentCounter[u8CycleCnt];

            u32ArrCurrentCounter[u8CycleCnt] = u32TimerLatchVal;

            _u32DiffCycleCnt = u32ArrCurrentCounter[u8CycleCnt]
                                - u32ArrPrevCounter[u8CycleCnt]  ;

            u8CycleCnt++;
            if(u8CycleCnt >= N_CALC_CYCLES)
            {
               u16PulsesCnt = 0;
                u8CycleCnt = 0;
            }

            _u16PulsesPerCalcCycle =(uint16_t)( (A_SENSE::_u16NumberOfTeeth *
                                            (u8CycleCnt + 1)) / N_CALC_CYCLES );

            _bStatusUpdateRPM = true;
        }
    }
    else
    {
        _u16NoOfPulsesToskip--;
    }
    UTILS_ResetTimer(&SpeedSensorTimer);
    UTILS_ResetTimer(&_RPMResetTimerPulseIp);
}

void A_SENSE::ConfigureNumberOfPoles(uint8_t u8Poles)
{
    _u8NumberOfPoles = u8Poles;
}

float A_SENSE::GetGensetFreqThruCompartor()
{
    //To avoid noise comparator input should be considered when
    //R-Phase voltage is greater than MIN_COMPARTOR_SENSE_VTG
    if(_AC_SENSE.GENSET_GetVoltageVoltsRaw(R_PHASE) > MIN_COMPARTOR_SENSE_VTG)
    {
        return _f32CompGenRFreq;
    }
    else
    {
        return (0);
    }
}

float A_SENSE::GetRPMThruCompartor()
{
    //To avoid noise comparator input should be considered when
    //R-Phase voltage is greater than MIN_COMPARTOR_SENSE_VTG
    if(_AC_SENSE.GENSET_GetVoltageVoltsRaw(R_PHASE) > MIN_COMPARTOR_SENSE_VTG)
    {
        return (_f32CompInputRPM);
    }
    else
    {
        return (0);
    }
}

float A_SENSE::GetFiltRPMThruCompartor()
{
    return (_f32FilteredRPM);
}

void A_SENSE::prvUpdateCompInpuFreqRPM(float f32Freq)
{
    if(_f32GenRPhaseRawVtg > 20.0f)
    {
        _f32CompGenRFreq = f32Freq;
        _f32CompInputRPM = ((_f32CompGenRFreq * RPM_CONVERSION_CONST) / _u8NumberOfPoles);

        if(_UpdateCompRPM_cb !=NULL)
        {
            _UpdateCompRPM_cb(_f32CompInputRPM) ;
        }
        UTILS_ResetTimer(&A_SENSE::_RPMResetTimerComparator);
    }
}

float A_SENSE::GetPulseInpuRPM(void)
{
    return _f32PulseInputRPM;
}

float A_SENSE::GetFilteredPulseInpuRPM(void)
{
    return _f32FilteredPulseIpRPM;
}

float A_SENSE::GetPin23CurrentValMilliAmp()
{
    return _sensors[HAL_PIN_23].GetPin23SensorCurrentValue();
}
AnalogSensor::AnalogSensor(ANLG_IP &anlgIp):
 _eName(A_SENSE_NOT_USED),
 _stTable{{0},{0},{0}},
 _pGetValSens(NULL),
 _eRef(ANLG_IP::REF_BATT_NEGATIVE),
 _stSensVal{0.0, ANLG_IP::BSP_STATE_NORMAL},
 _AnlgIp(anlgIp),
 _f32Pin23CurrentVal(0)
{

}

void AnalogSensor::CfgAsAnalogSensor( TYPS_t  eName,
                                    INTERPOLATE_INFO_t stTable,
                                    pGetVal pGetValSens,
                                    ANLG_IP::SENSOR_REF_t eRef
                                    )
{
    _eName = eName;
    _stTable = stTable;
    _pGetValSens = pGetValSens;
    _eRef = eRef;
}

float AnalogSensor::prvInterpolation(float f32Val, float * fpXValues,
                                     float * fpYValues, uint8_t u8No_of_points)
{
    float f32Interpolated = 0;
     uint8_t ii = 0;
     if(fpXValues[0] < fpXValues[u8No_of_points - 1]) /* Indicates increasing resistances */
     {
         if(fpXValues[0] >= f32Val) f32Interpolated = fpYValues[0];
         else if(fpXValues[u8No_of_points - 1] <= f32Val) f32Interpolated = fpYValues[u8No_of_points - 1];
         else
         {
             while(fpXValues[ii] <= f32Val)
             {
                 ii++;
             }
             ii = ii - 1;
             f32Interpolated = (fpYValues[ii] + (((fpYValues[ii + 1] - fpYValues[ii])*(f32Val - fpXValues[ii]))/(fpXValues[ii + 1] - fpXValues[ii])));
         }
     }
     else /* Indicates decreasing resistances */
     {
         if(fpXValues[0] <= f32Val) f32Interpolated = fpYValues[0];
         else if(fpXValues[u8No_of_points - 1] >= f32Val) f32Interpolated = fpYValues[u8No_of_points - 1];
         else
         {
             ii = u8No_of_points - 1;
             while(fpXValues[ii] <= f32Val)
             {
                 ii--;
             }
             f32Interpolated = (fpYValues[ii] + ((fpYValues[ii + 1] - fpYValues[ii])*(f32Val - fpXValues[ii]))/(fpXValues[ii + 1] - fpXValues[ii]));
         }
     }
     return f32Interpolated;
}

ANLG_IP::ANALOG_VAL_t AnalogSensor::GetValue()
{
    return _stSensVal;
}

AnalogSensor::TYPS_t AnalogSensor::GetName()
{
    return _eName;
}

void AnalogSensor::Update()
{
    ANLG_IP::ANALOG_VAL_t stVal;

    if((_pGetValSens != NULL) && (_eName != A_SENSE_NOT_USED))
    {
        stVal = ((&_AnlgIp)->*_pGetValSens)(_eRef);

        if(_eName == A_SENSE_LUBE_OIL_PRESSURE_4_20)
        {
            _f32Pin23CurrentVal =  stVal.f32InstSensorVal;
        }

        stVal.f32InstSensorVal = prvInterpolation(stVal.f32InstSensorVal,
                &_stTable.af32IntrpolationTableX[0],
                &_stTable.af32IntrpolationTableY[0],
                _stTable.u8InterPolationPoints
                );
        //IIR filter
        _stSensVal.f32InstSensorVal =  ((_stSensVal.f32InstSensorVal * (SENSOR_FILT_CONST))
                             + (stVal.f32InstSensorVal * (100-SENSOR_FILT_CONST))) /100;

        _stSensVal.eState = stVal.eState;
    }

}

float AnalogSensor::GetPin23SensorCurrentValue()
{
    return _f32Pin23CurrentVal;
}
