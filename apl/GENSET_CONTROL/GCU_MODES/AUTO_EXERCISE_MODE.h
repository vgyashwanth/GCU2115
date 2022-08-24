/*
 * AUTO_EXERCISE_MODE.h
 *
 *  Created on: 10-Feb-2022
 *      Author: madhuri.abhang
 */

#ifndef APL_GENSET_CONTROL_GCU_MODES_AUTO_EXERCISE_MODE_H_
#define APL_GENSET_CONTROL_GCU_MODES_AUTO_EXERCISE_MODE_H_
#include "stdint.h"
#include "BASE_MODES.h"
#include "START_STOP.h"
#include "../EGOV/EGOV.h"
#include "HAL_Manager.h"

class AUTO_EXERCISE_MODE:public BASE_MODES
{
public:
    AUTO_EXERCISE_MODE(HAL_Manager &hal, ENGINE_MONITORING &EngineMon,
                CFGZ &cfgz,  GCU_ALARMS &GCUAlarms,
                   START_STOP &StartStop, GCU_MODE_VARS_t &vars);

#define MINS_IN_HRS             (60U)
#define HOURS_IN_DAY            (24U)
#define MONTH_FEB               (2U)
#define DAYS_IN_FEB_NONLEAP     (28U)
#define DAYS_IN_FEB_LEAP        (29U)
#define SECONDS_IN_WEEK         (604800U)
#define NO_OF_SECOND_IN_DAYS(d) ((d) * SECONDS_IN_DAY)
#define NO_OF_DAYS_PER_WEEK     (7U)

#define SERVED                  (true)
#define UNSERVED                (false)

#define DAILY                   (0)
#define WEEKLY                  (1)
#define MONTHLY                 (2)

#define SECONDS_IN_DAY                         (86400U)
#define SECONDS_IN_HOUR                        (3600U)
#define SECONDS_IN_MINUTE                      (60U)
#define DAYS_IN_NONLEAPYEAR                    (365U)
#define MAX_NUMBER_OF_SECONDS_IN_MINS          (59U)
#define MAX_NUMBER_OF_MINUTES_IN_HOUR          (59U)
#define MAX_NUMBER_OF_HOURS_IN_DAY             (23U)
#define MAX_NUMBER_OF_DAYS_IN_MONTHS           (31U)
#define MAX_NUMBER_OF_MONTHS_IN_YEAR           (12U)
#define VALID_YEAR_RANGE_LOWER_LIMIT           (2000U)
#define VALID_YEAR_RANGE_HIGHER_LIMIT          (2099U)

    typedef enum
    {
        MONTH_START,
        JANUARY,
        FEBRUARY,
        MARCH,
        APRIL,
        MAY,
        JUNE,
        JULY,
        AUGUST,
        SEPTEMBER,
        OCTOBER,
        NOVEMBER,
        DECEMBER,
        MONTH_END
    }E_MONTH;

    typedef struct
    {
        uint8_t u8Enable;       /* This holds setting from configuration for enabling to particular exercise*/
        uint8_t u8Set;          /* This indicates, particular exercise will run for time match*/
        uint8_t u8Day;
        uint8_t u8Month;
        uint8_t u8DayOfWeek;
        uint8_t u8LoadTrans;
        uint8_t u8Occurence;
        uint8_t u8Status;
        uint16_t u16Year;
        uint32_t u32StartTime;
        uint32_t u32OnDuration;
    //  uint32_t u32EndTime; /* Not used now due to code size limitation */
    } EXERCISE_t;

     const uint8_t LY[MONTH_END] = {0U, 31U, 29U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U}; /* Non-leap-year */
     const uint16_t MONTH_DAYS[MONTH_END] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U}; /* Leap-year */
     const uint8_t ULY[MONTH_END] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U,
                                   31U, 30U, 31U};


     EXERCISE_t *pstNextExerciseToRun;



     void Update(bool bDeviceInConfigMode);
     void Init();

     void EnableAutoExerciser(void);
     static bool IsExerciserStarted(void);
     void ChageAutoExeSMDState(BASE_MODES::AUTO_EXE_STATE_t estate);
     void StopAutoExe(void);
     void GetExerciserInfo(EXERCISE_t* pstTemp, uint8_t u8TempExeType);
     uint8_t GetRunningExeType(void);
     uint32_t GetExerciserTime(void);
private:
     HAL_Manager         &_hal;
     ENGINE_MONITORING   &_EngineMon;
     CFGZ                &_cfgz;
     GCU_ALARMS          &_GCUAlarms;
     START_STOP          &_StartStop;
     GCU_MODE_VARS_t     &_vars;

     EXERCISE_t _stEvent1;
     EXERCISE_t _stEvent2;
     static stTimer      _ExeDnCntTmr;
     stTimer             _ExeTriggerUpdateTmr ;
     static bool _bSchStopped;
     bool _bSchOnTimeOver;
     static bool _bAutoExerciserSkipped;
     void prvGensetStartAction();
     void prvShutdownAction();
     void prvMonitorExcerciserTimer();
     void prvUpdateCallingForSchedulerRunOp();
     /**************************************************************************************************
     @Name           -   ScheduleExercise
     @brief          -   This function initialises configurable input structure's parameter - current
                         ADC reading based on sensor selection for Fuel sensor. This function should
                         get called before calling fn_populate_mon16() function.
     @param          -   pCnfgInput - pointer to the Fuel configurable input
     @return         -   none
     @notes          -   Search ID - GC1031 Scheduler Imitation
     @developer      -   Kedar Naphad
     @date           -   15-Dec-2018
     **************************************************************************************************/
     void prvScheduleExercise(void);
     /**************************************************************************************************
     @Name           -   SetExercise
     @brief          -   This function initialises configurable input structure's parameter - current
                         ADC reading based on sensor selection for Fuel sensor. This function should
                         get called before calling fn_populate_mon16() function.
     @param          -   pCnfgInput - pointer to the Fuel configurable input
     @return         -   none
     @notes          -   Search ID - GC1031 Scheduler Imitation
     @developer      -   Kedar Naphad
     @date           -   15-Dec-2018
     **************************************************************************************************/
     void prvSetExercise(void);
     void prvSetValidDateAndTimeExercise(EXERCISE_t *pstExercise);

     /**************************************************************************************************
     @Name           -   SetNextExercise
     @brief          -   This Function sets the next exercise run time in the event structure.
     @param          -   EXERCISE_t *pstExercise1 - Pointer to 1st exercise structure
                         EXERCISE_t *pstExercise2 - Pointer to 2nd exercise structure
     @return         -   uint8_t value = 0 if both exercise are at same time
                                         1 if 1st exercise is latest
                                         2 if 2nd exercise is latest
     @notes          -   Search ID - GC1031 Scheduler Imitation
     @developer      -   Kedar Naphad
     @date           -   15-Dec-2018
     **************************************************************************************************/
     void prvSetNextExercise(EXERCISE_t *pExercise);

     /**************************************************************************************************
     @Name           -   IsSetTimeLessThanCurrentTime
     @brief          -   This function checks start time of event with current time.
     @param          -   pstEvent - Pointer to exerciser structure
     @return         -   0 - If current time is less than set time
                         1 - If current time is greater than set time
     @notes          -   Search ID - GC1031 Scheduler Imitation
     @developer      -   Amruta Waghmare
     @date           -   5-Sept-2019
     **************************************************************************************************/
     uint8_t prvIsSetTimeLessThanCurrentTime(EXERCISE_t *pstExercise);
     /**************************************************************************************************
     @Name           -   ConvertTimeAndDateToSeconds (From PE generated HAL of component RTC_LDD)
     @brief          -   This function computes the number of seconds passed since 2000-01-01 till
                         current time.
     @param          -   TimePtr - Pointer to the time structure
                         SecondsPtr - Pointer to no. of seconds passed
     @return         -   none
     @notes          -   Search ID - GC1031 Scheduler Imitation
     @developer      -   Aishwarya Bhandari
     @date           -   8-Jul-2018
     **************************************************************************************************/
      bool prvConvertTimeAndDateToSeconds(RTC::TIME_t *TimePtr, uint32_t *SecondsPtr);

      /**************************************************************************************************
      @Name           -   ConvertSecondsToDateAndTime (From PE generated HAL of component RTC_LDD)
      @brief          -   This function computes date and time from the number of seconds passed since
                          2000-01-01.
      @param          -   TimePtr - Pointer to the time structure
                          SecondsPtr - Pointer to no. of seconds passed
      @return         -   none
      @notes          -   Search ID - GC1031 Scheduler Imitation
      @developer      -   Aishwarya Bhandari
      @date           -   8-Jul-2018
      **************************************************************************************************/
      void prvConvertSecondsToDateAndTime(RTC::TIME_t *TimePtr, uint32_t Seconds);

     /**************************************************************************************************
     @Name           -   CmpExercises
     @brief          -   This Function compares two Auto Exercise structures with respect to time and
                         returns the integer which tells, which event must be started first.

     @param          -   EXERCISE_t *pstExercise1 - Pointer to 1st exercise structure
                         EXERCISE_t *pstExercise2 - Pointer to 2nd exercise structure
     @return         -   uint8_t value = 0 if both exercise are at same time
                                         1 if 1st exercise is latest
                                         2 if 2nd exercise is latest
     @notes          -   Search ID - GC1031 Scheduler Imitation
     @developer      -   Kedar Naphad
     @date           -   15-Dec-2018
     **************************************************************************************************/
     uint8_t prvCmpExercises(EXERCISE_t *pstExercise1, EXERCISE_t *pstExercise2);
     /**************************************************************************************************
     @Name           -   AssignNextExerToRun
     @brief          -   This function decides which exercise is to be run next, based on
                         exercise time comparison result of both events.
     @param          -   u8CmprsnVal - Comparison result of which events' next run time is closer.
                         pstEvent1 - Pointer to the structure of 1st Event
                         pstEvent2 - Pointer to the structure of 2nd Event
     @return         -   none
     @notes          -   Search ID - GC1031 Scheduler Imitation
     @developer      -   Kedar Naphad
     @date           -   15-Dec-2018
     **************************************************************************************************/
     void prvAssignNextExerToRun(uint8_t u8CmprsnVal, EXERCISE_t *pstEvent1, EXERCISE_t *pstEvent2);
     /**************************************************************************************************
     @Name           -   DayOfWeekOnDate
     @brief          -   This function initialises configurable input structure's parameter - current
                         ADC reading based on sensor selection for Fuel sensor. This function should
                         get called before calling fn_populate_mon16() function.
     @param          -   pCnfgInput - pointer to the Fuel configurable input
     @return         -   none
     @notes          -   Search ID - GC1031 Scheduler Imitation
     @developer      -   Kedar Naphad
     @date           -   15-Dec-2018
     **************************************************************************************************/
     void prvDayOfWeekOnDate(RTC::TIME_t *pTimeStamp);

     /**************************************************************************************************
     @Name           -   DateOnDayOfWeek
     @brief          -   This function initialises configurable input structure's parameter - current
                         ADC reading based on sensor selection for Fuel sensor. This function should
                         get called before calling fn_populate_mon16() function.
     @param          -   pCnfgInput - pointer to the Fuel configurable input
     @return         -   none
     @notes          -   Search ID - GC1031 Scheduler Imitation
     @developer      -   Kedar Naphad
     @date           -   15-Dec-2018
     **************************************************************************************************/
     void prvDateOnDayOfWeek(RTC::TIME_t *pstCurrentTime, uint8_t u8DayOfWeek);

     /**************************************************************************************************
     @Name           -   IncrementDateByWeek
     @brief          -   This function increments the time stamp by a week.
     @param          -   pstTimeStamp - Pointer to the time stamp whose week is to be incremented.
     @return         -   none
     @notes          -   Search ID - GC1031 Scheduler Imitation
     @developer      -   Kedar Naphad
     @date           -   15-Dec-2018
     **************************************************************************************************/
     void prvIncrementDateByWeek(RTC::TIME_t *pstTimeStamp);

     /**************************************************************************************************
     @Name           -   IncrementTimeStampMonth
     @brief          -   This function increments the time stamp by a month.
     @param          -   pstTimeStamp - Pointer to the time stamp whose month is to be incremented.
     @return         -   none
     @notes          -   Search ID - GC1031 Scheduler Imitation
     @developer      -   Kedar Naphad
     @date           -   15-Dec-2018
     **************************************************************************************************/
     void  prvIncrementTimeStampMonth(RTC::TIME_t *pstTimeStamp);

};

#endif /* APL_GENSET_CONTROL_GCU_MODES_AUTO_EXERCISE_MODE_H_ */
