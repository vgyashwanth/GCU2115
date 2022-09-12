/**
 * @file        [ENGINE_MONITORING]
 * @brief       ENGINE_MONITORING
 *              Provides common API's for functionality related to the engine.
 *
 * @version     [Version number]
 * @author      Nikhil Mhaske
 * @date        5th June 2021
 * @author      None
 * @date        None
 * @copyright   SEDEMAC Mechatronics Pvt Ltd
 **/

#ifndef _ENGINE_MONITORING_H_
#define _ENGINE_MONITORING_H_

#include "stdio.h"
#include "HAL_Manager.h"
#include "../../apl/CFGZ/CFGZ.h"
#include "GCU_ALARMS.h"
#include "AC_SENSE.h"
#include "SLEEP_Handler.h"


class ENGINE_MONITORING{

public:
    typedef enum
    {
        LOAD_ON_GEN,
        LOAD_ON_MAINS,
        LOAD_NOT_ON_GEN_MAINS
    }LOAD_CONT_STATUS_t;

    typedef enum
    {
       ACTIVE_POWER,   /**< ACTIVE */
       APARENT_POWER,  /**< APARENT */
       REACTIVE_POWER, /**< REACTIVE */
       POWER_LAST/**< POWER_LAST */
    }POWER_TP_t;
    /// Constructor: Initializes this module
    ENGINE_MONITORING(CFGZ &cfgz, GCU_ALARMS &GCUAlarms, HAL_Manager &hal);

    /**
     * Executes module functionality. To be called in the super while loop.
     * @param  : bool for checking whether device is in configuration.
     * @return
     * None
     */
    void Update(bool bDeviceInConfigMode);

    /**
     * provide the status whether engine is cranked or not.
     * @param  : None
     * @return : returns true if the engine is cranked.
     */
    static bool IsEngineCranked();

    /**
     * provide the status whether warm up delay is over or not.
     * @param  : None
     * @return : returns true if the warm up delay is expired.
     */
    bool IsWarmUpTimeExpired();

    void DisableEngWarmUpTimer();

    void DisableGenWarmUpTimer();

    bool IsGenWarmUpEnabledAndNotExpired();

    void EngineStoppingComplete();

    static uint32_t GetEngineRunTimeMin();
    static uint32_t  GetMainsRunTimeMin();
    static uint32_t  GetBTSRunTimeMin();
    static uint32_t  GetTamperedRunTimeMin();
    uint32_t GetEngineNoOfStarts();

    uint32_t GetEngineNoOfTrips();

    void StoreEngRnCnt(uint32_t u32cnt);
    void StoreMainsRnCnt(uint32_t u32cnt);
    void StoreStartCnt(uint32_t u32cnt);
    void StoreTripCnt(uint32_t u32cnt);
    void StoreCummulativeCnt();
    void StoreGenEnery(POWER_TP_t eTyep, uint32_t u32cnt);
    void StoreMainsEnery(POWER_TP_t eTyep, uint32_t u32cnt);
    void UpdateStartStopState(uint8_t u8StartStopState);


    /**
     * provide the status whether Genset is ready or not depending on the values
     * of voltage and frequency.
     * @param  : None
     * @return : returns true when the genset is ready.
     */
    static uint8_t IsGenReady();

    /**
     * provide the status whether engine is on or not.
     * @param  : None
     * @return : returns true if the engine is on.
     */
    static uint8_t IsEngineOn();

    static uint8_t IsEngineOff();

    static uint8_t IsGenAvailable();

    static void ClearTriplatched();

    void ReadEnergySetEnergyOffset(bool bFromEeprom);

    float GetFilteredEngSpeed();

    float GetRawEngSpeed();
//    float GetTamprEEPromCummEnergy();
//    float GetGenEEPromCummKWH();
//    float GetGenEEPromCummKVAH();
//    float GetGenEEPromCummKVARH();

    void StoreCumulativeEnergy();
    void StoreTamprCummulativeEnergy();

    uint16_t GetHistogramData(uint8_t);

    void UpdateEngineOnStatus(void);
    void UpdateContactorLoadStatus();
    static LOAD_CONT_STATUS_t GetContactorLoadStatus();
    static bool GetAndClearIsLoadStatusChanged();

   // bool IsGenTamp();
    typedef struct
    {
        bool bCtOnLoadCable;    // true means on load cable, false means on Alternator output cable
        bool bGensetContactorFeedbackIsAssigned;    // to some input
        bool bGensetContactorFeedbackInputStatus;   // true means contactor is latched so as to connect load to Genset
        bool bMainsContactorFeedbackIsAssigned; // to some input
        bool bMainsContactorFeedbackInputStatus;   // true means contactor is latched so as to connect load to Mains
        bool bGensetContactorOutputAssigned;
        bool bMainsContactorOutputAssigned;
        bool bGensetContactorOutputStatus;  // can be on or off; only meaningful if assigned AND is not pulse type
        bool bMainsContactorOutputStatus;  // can be on or off; only meaningful if assigned AND is not pulse type
    } CONTACTOR_INFO_t;
    LOAD_CONT_STATUS_t getAssumedLoadStatus(CONTACTOR_INFO_t ci);
    bool isGensetContactorFeedbackAssigned();
    bool isMainsContactorFeedbackAssigned();
    bool gensetContactorFeedbackInputStatus();
    bool mainsContactorFeedbackInputStatus();
    bool isMainsContactorOutputAssigned();
    bool isGensetContactorOutputAssigned();

    bool haveWeTriedToCloseGensetContactor();
    bool haveWeTriedToCloseMainsContactor();

private:

    typedef enum
    {
        ID_HIST_1 = 0,
        ID_HIST_2,
        ID_HIST_3,
        ID_HIST_4,
        ID_HIST_5,
        ID_HIST_6,
        ID_HIST_LAST
    }HISTOGRAM_t;


    /**
     * Structure for storing Energy CUMULATIVE Count
     */
    typedef struct __attribute__((packed))
    {
        uint64_t u64Header;
        uint32_t u32EngineRunTime_min;
        uint32_t u32MainsRunTime_min;
        uint32_t u32BTSRunTime_min;
        uint32_t u32TamperedRunTime_min;
        float f32GenKWH;
        float f32GenKVAH;
        float f32GenKVARH;

        float f32MainsKWH;
        float f32MainsKVAH;
        float f32MainsKVARH;

        float f32TamprGenKWH;
        float f32TamprGenKVAH;
        float f32TamprGenKVARH;

        uint16_t u16ArrHistogram[ID_HIST_LAST];
//        uint16_t u16Dummy[2];
        uint32_t u32GenNumberOfTrips;
        uint32_t u32GenNumberOfStarts;
        uint32_t u32CRC;
    }CUMULATIVE_t;

    /**
     * Structure for storing Energy CUMULATIVE Count
     */
    typedef struct
    {
        float f32GenKWH;
        float f32GenKVAH;
        float f32GenKVARH;
    }GEN_t;

    #define CHECK_GEN_MIN_HEALTHY_FREQ()  (_hal.AnalogSensors.GetGensetFreqThruCompartor() > _cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_MIN_HEALTHY_FREQ))
    #define CHECK_GEN_MIN_HEALTHY_VTG()   (_GCUAlarms.GetMinGensetVoltage() > _cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_MIN_HEALTHY_VOLT))
    #define KW_TO_WATT_CONVERSION       1000U
    #define ENGINE_OFF_TMR_THRESHOLD    1U
    #define FIFTY_MSEC                  50U
    #define RUN_MIN_1st_SLOT            (1000U*60U)
    #define RUN_MIN_2nd_SLOT            (5000U*60U)

    #define TIME_1st_SLOT_SEC           (5*60U)  //5min
    #define TIME_2nd_SLOT_SEC           (10*60U)  //10min
    #define TIME_3rd_SLOT_SEC           (15*60U)  //15min

    #define ONE_MIN_CNT                 60
    #define MAX_NO_OF_STARTS            65000
    #define MAX_NO_OF_TRIPS             65000
    #define TMR_COUNT_FOR_TWO_SECS      40
    #define HISTO_RANGE_MIN             0
    #define HISTO_RANGE_MAX             20

    CFGZ                        &_cfgz;
    GCU_ALARMS                  &_GCUAlarms;
    HAL_Manager                 &_hal;
    uint8_t                     _u8StartStopSMState;
    static bool                 _bTripLatched;
    static bool                 _bEngineCranked;
    static bool                 _bLoadContactorStatusChanged;
    stTimer                     _LLOPCrankingTimer;
    stTimer                     _Timer50MS;
    stTimer                     _TimerOneMin;
    stTimer                     _MainsRunTimeBaseTimer;
    stTimer                     _TimerGenUpdateCumulative;
    stTimer                     _TimerMainsUpdateCumulative;
    stTimer                     _TimerBTSUpdateCumulative;
    stTimer                     _TimerUpdateTamperedCumulative;
    stTimer                     _BTSRunTimeBaseTimer;
    stTimer                     _EngWarmUpTimer;
    stTimer                     _GenWarmUpTimer;
    stTimer                     _LOPSensMonTimer;
    AC_SENSE::ENERGY_REGISTER_t _stTampEnergyRegister,_stEnergyRegister, _stMainsEnergyRegister;
    static uint8_t              _u8EngineOff;
    static uint8_t              _u8EngineOn;
    static uint8_t              _u8GenReady;
    static uint8_t              _u8GenAvailable;
    static CUMULATIVE_t         _stCummulativeCnt;
    static LOAD_CONT_STATUS_t   _eLoadStatusCurrent;
    uint8_t                     _u8ActiveSectorForCummulative;
    /**
     * Checks if the any trip occurs and set the flag to updated the trip count.
     * @param  : None
     * @return : None
     */
    void prvCheckTrips();
    /**
     * Checks if engine is off.
     * @param  : None
     * @return : None
     */
    void prvCheckEngineOff();
    /**
     * Update the engine on status depending on the speed and frequency.
     * @param  : None
     * @return : None
     */
    void prvUpdateEngineOn();
    /**
     * Update the engine cranked status depending on the speed, frequency,
     * charging alternator voltage, lube oil pressure and lube oil switch.
     * @param  : None
     * @return : None
     */
    void prvUpdateEngineCranked();
    /**
     * Update the genset ready status depending on minimum healthy voltage and
     * frequency.
     * @param  : None
     * @return : None
     */
    void prvUpdateGenReady();
    void prvGetCumulativeCnt();

    void prvLoadHistogram();
    void prvClearHistogram();

    uint16_t prvCheckTimeSlot(uint32_t u32RunTime);
};

#endif
