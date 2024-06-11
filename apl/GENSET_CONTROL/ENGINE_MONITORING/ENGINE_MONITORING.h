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

#include <time.h>
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

    typedef enum
    {
        CUM_STORE_GENERAL,
        CUM_STORE_CRANK_CNTS,
        CUM_STORE_OVLD_EXT_RUN_HRS,
        CUM_STORE_LAST
    }CUM_STORE_t;


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


    /* Constructor: Initializes this module */
    ENGINE_MONITORING(CFGZ &cfgz, GCU_ALARMS &GCUAlarms, HAL_Manager &hal);

    /**
     * Executes module functionality. To be called in the super while loop.
     * @param  : bool for checking whether device is in configuration.
     * @return
     * None
     */
    void Update();

/* Status prototypes */
    bool IsGenStartValid();
    bool IsLOPSensorConfigured();
    static uint8_t IsEngineOn();
    static uint8_t IsEngineOff();
    static uint8_t IsGenAvailable();
    static bool IsEngineCranked();
    static uint8_t IsGenReady();
    bool IsWarmUpTimeExpired();
    bool IsGenWarmUpEnabledAndNotExpired();

    bool IsModeSelectInputConfigured();

    bool isGensetContactorFeedbackAssigned();
    bool isMainsContactorFeedbackAssigned();
    bool gensetContactorFeedbackInputStatus();
    bool mainsContactorFeedbackInputStatus();
    bool isMainsContactorOutputAssigned();
    bool isGensetContactorOutputAssigned();
    LOAD_CONT_STATUS_t getAssumedLoadStatus(CONTACTOR_INFO_t ci);

    bool haveWeTriedToCloseGensetContactor();
    bool haveWeTriedToCloseMainsContactor();


/* Value return prototypes */
    float GetFilteredEngSpeed();

/* Action prototypes */
    static void ClearTriplatched();
    void DisableEngWarmUpTimer();
    void DisableGenWarmUpTimer();

/* returns private structure variables that are made static to accessible outside */
    static uint32_t GetEngineRunTimeMin();
    static uint32_t GetMainsRunTimeMin();
    static uint32_t GetBTSRunTimeMin();
    static uint32_t GetTamperedRunTimeMin();

    uint32_t GetEngineNoOfStarts();
    uint32_t GetEngineNoOfTrips();

    void UpdateStartStopState(uint8_t u8StartStopState);

/* store cumulative counts data to external eeprom */
    void StoreCummulativeCnt(CUM_STORE_t eType);

/* Updation prototypes */
    void ReadEnergySetEnergyOffset(bool bFromEeprom);

    float GetTamprEEPromCummEnergy();
//    float GetGenEEPromCummKWH();
//    float GetGenEEPromCummKVAH();
//    float GetGenEEPromCummKVARH();

    void StoreCumulativeEnergy();
    void StoreTamprCummulativeEnergy();

    uint16_t GetHistogramData(uint8_t);

    void UpdateEngineOnStatus(void);

    /* todo: following 3 functions can be made private */
    void UpdateContactorLoadStatus();
    static LOAD_CONT_STATUS_t GetContactorLoadStatus();
    static bool GetAndClearIsLoadStatusChanged();
    uint32_t GetRemoteRunTimeMin();
    uint32_t GetManualRunTimeMin();
    uint32_t GetNoLoadRunTimeMin();
    uint32_t GetOnLoadRunTimeMin();
    uint32_t GetCumCrankCnt();
    uint32_t GetCumFailedCrankCnt();

#if (TEST_AUTOMATION == YES)
    /**
     * This function overwrites the engine run hours (in mins) with the value received over
     * MODBUS while automation testing.
     * @param u32EngineRunTimeInmin - Desired engine run hours counts.
     */
    void SetEngineRunTime(uint32_t u32EngineRunTimeInmin);

    /**
     * This function overwrites the mains run hours (in mins) with the value received over
     * MODBUS while automation testing.
     * @param u32MainsRunTimeInmin - Desired engine run hours counts.
     */
    void SetMainsRunTime(uint32_t u32MainsRunTimeInmin);

    /**
     * This function overwrites the BTS run hours (in mins) with the value received over
     * MODBUS while automation testing.
     * @param u32BTSRunTimeInmin - Desired engine run hours counts.
     */
    void SetBTSRunTime(uint32_t u32BTSRunTimeInmin);

    /**
     * This function overwrites the Active Gen Energy (in kWH) with the value received over
     * MODBUS while automation testing.
     * @param u32GenActiveEnergy - Desired Gen Active Energy.
     */
    void SetGenActiveEnergy(uint32_t u32GenActiveEnergy);

    /**
     * This function overwrites the Apparent Gen Energy (in kVAH) with the value received over
     * MODBUS while automation testing.
     * @param u32GenApparentEnergy - Desired Gen Apparent Energy.
     */
    void SetGenApparentEnergy(uint32_t u32GenApparentEnergy);

    /**
     * This function overwrites the Reactive Gen Energy (in kVARH) with the value received over
     * MODBUS while automation testing.
     * @param u32GenReactiveEnergy - Desired Gen Reactive Energy.
     */
    void SetGenReactiveEnergy(uint32_t u32GenReactiveEnergy);

    /**
     * This function overwrites the no. of starts counts with the value received over
     * MODBUS while automation testing.
     * @param u32NumberOfStarts - Desired no. of starts counts.
     */
    void SetGenNumberOfStarts(uint32_t u32NumberOfStarts);

    /**
     * This function overwrites the no. of trips counts with the value received over
     * MODBUS while automation testing.
     * @param u32NumberOfTrips - Desired no. of trips counts.
     */
    void SetGenNumberOfTrips(uint32_t u32NumberOfTrips);


#endif



/*____________________________________________________________________________________________________________________*/

private:
    /* Supportive private macros */
    #define KW_TO_WATT_CONVERSION       (1000U)
    #define ENGINE_OFF_TMR_THRESHOLD    (1U)
    #define FIFTY_MSEC                  (50U)
    #define RUN_MIN_1st_SLOT            (300U*60U)
    #define RUN_MIN_2nd_SLOT            (1000U*60U)
    #define RUN_MIN_3rd_SLOT            (5000U*60U)

    #define TIME_1st_SLOT_SEC           (1*60U)  //1min
    #define TIME_2nd_SLOT_SEC           (2*60U)  //2min
    #define TIME_3rd_SLOT_SEC           (5*60U)  //5min
    #define TIME_4th_SLOT_SEC           (15*60U) //15min

    #define ONE_MIN_CNT                 (60)
    #define TWELVE_HR_IN_SEC            (12*60*60) //in sec
    #define FIVE_MIN_IN_SEC             (5*60) //in sec
    #define MAX_NO_OF_STARTS            (65000)
    #define MAX_NO_OF_TRIPS             (65000)
    #define TMR_COUNT_FOR_TWO_SECS      (40)
    #define CHECK_GEN_MIN_HEALTHY_FREQ()  (_hal.AnalogSensors.GetGensetFreqThruCompartor() > _cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_MIN_HEALTHY_FREQ))
    #define CHECK_GEN_MIN_HEALTHY_VTG()   (_GCUAlarms.GetMinGensetVoltage() > _cfgz.GetCFGZ_Param(CFGZ::ID_ALT_CONFIG_MIN_HEALTHY_VOLT))


    /* todo: need to decide, wheather to make below structure private or public
     * Structure for storing Energy CUMULATIVE Count
     */
    typedef struct __attribute__((packed))
    {
        uint64_t u64Header;
        uint32_t u32EngineRunTime_min;
        uint32_t u32MainsRunTime_min;
        uint32_t u32BTSRunTime_min;
        uint32_t u32GenRemoteRunTime_min;
        uint32_t u32GenManualRunTime_min;
        uint32_t u32GenNoLoadRunTime_min;
        uint32_t u32GenOnLoadRunTime_min;
        uint32_t u32GenExtOverloadRunTime_min;
        time_t  ExtOvldStartTime;
        uint8_t u8ExtOvldStarted;
        uint8_t u8ExtOvldFault;
        uint8_t u8Dummy[2];

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

        uint32_t u32GenNumberOfTrips;
        uint32_t u32GenNumberOfStarts;
        uint32_t u32GenNumberOfCranks;
        uint32_t u32GenNumberOfFailedCranks;
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

    /* private object references */
    CFGZ                        &_cfgz;
    GCU_ALARMS                  &_GCUAlarms;
    HAL_Manager                 &_hal;

    /* private timer instances */
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

    /* private static variables */
    static bool                 _bTripLatched;
    static bool                 _bEngineCranked;
    static bool                 _bLoadContactorStatusChanged;
    static uint8_t              _u8EngineOff;
    static uint8_t              _u8EngineOn;
    static uint8_t              _u8GenReady;
    static uint8_t              _u8GenAvailable;
    static CUMULATIVE_t         _stCummulativeCnt;
    static CUMULATIVE_t         _stStoredCummulativeCnt; /*This variable will hold the last _stCummulativeCnt stored in EEprom*/
    static LOAD_CONT_STATUS_t   _eLoadStatusCurrent;

    uint8_t                     _u8StartStopSMState;
    uint8_t                     _u8ActiveSectorForCummulative;
    AC_SENSE::ENERGY_REGISTER_t _stTampEnergyRegister,_stEnergyRegister, _stMainsEnergyRegister;

    A_SENSE::SENSOR_RET_t       _stLOP;  /* Lop sensor structure */

    bool                        _bCrankStateLatched;
    bool                        _bFailedCrankStateLatched;

    uint8_t                     _u8OvldExtMinCnt;
    uint8_t                     _u8OvldExtOneHrContCnt;

#if(TEST_AUTOMATION == YES)
    /**
     * This variable is used to flag if request to store power related info
     * into EEPROM is from automation request or normal operation of GCU.
     */
    bool                        _bFromAutomation;
#endif
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

    uint16_t prvCheckTimeSlot(uint32_t u32RunTime);

    void prvUpdateLOPSensor();
    void prvUpdateEngineRunHrs();
    void prvUpdateMainsRunHrs();
    void prvUpdateBTSRunHrs();

    void prvUpdateEngineONstatus(void);
    void prvUpdateCumulativeEnergyCounts();
    void prvUpdateCumulativeTamperedEnergyCounts();

    bool prvDisconnectCranckByChanrgingAlt();
    bool prvDisconnectCranckByLOPSensor();
    bool prvDisconnectCranckByLOPSwitch();
   
    void prvUpdateCumCrankCnts();
    void prvUpdateExtOvldRunHrs();
    void prvCheckExtOvldReset();
    time_t prvGetCurrTimeStamp(); 
};

#endif
