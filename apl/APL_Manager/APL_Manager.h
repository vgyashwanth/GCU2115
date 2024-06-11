#ifndef _APL_MANAGER_H_
#define _APL_MANAGER_H_

#include <HMI/MAIN_UI/MAIN_UI.h>
#include <HMI/MON_UI/MON_UI.h>
#include "HAL_Manager.h"
#include "CFGZ/CFGZ.h"
#include "MB_APP.h"
#include "ENGINE_MONITORING.h"
#include "GCU_ALARMS.h"
#include "MANUAL_MODE.h"
#include "START_STOP.h"
#include "CHARGING_ALT.h"
#include "config.h"
#include "BASE_MODES.h"
#include "AUTO_MODE.h"
#include "BTS_MODE.h"
#include "CYCLIC_MODE.h"
#include "SLEEP_Handler.h"
#include "../CFGC/CFGC.h"
#include "../J1939_APP/J1939APP.h"
#include "../ENGINE_START_VALIDITY/ENGINE_START_VALIDITY.h"

class APL_Manager: public HAL_Manager
{
public:

    APL_Manager();

    void Update();
    bool bDeviceInConfigMode;
private:
    MB_APP                      _MbApp;
    CFGC                        _cfgc;
    CFGZ                        _cfgz;
    SLEEP_Handler               _sleep;
    GCU_ALARMS                  _gcuAlarms;
    ENGINE_MONITORING           _engineMonitoring;
    CHARGING_ALT                _chrgAlt;
    START_STOP                  _startStop;
    MANUAL_MODE                 _ManualMode;
    AUTO_MODE                   _AutoMode;
    BTS_MODE                    _BTSMode;
    CYCLIC_MODE                 _CyclicMode;
    Display                     _display;
    MAIN_UI                     _MainUI;
    J1939APP                    _J1939;
    ENGINE_START_VALIDITY       _EngineStartValidity;
    stTimer                     _PowerOnUpdateTimer;
    #define BOOT_TIMER_SEC      (4U)
    typedef struct
    {
       uint8_t u8Revision;
       uint8_t u8Version;
       uint32_t u32PLMID;
       uint16_t u16Dummy;
    }INFO_t;
    INFO_t _stFirmwareInfo={0, 22, 15558, 0};
    void prvCheckFirmwareInfo();
    void prvSetFwVerInMb();
    void prvCheckAndLogFlashingEvent();
    bool prvCheckIfFirmwareUpdated();
};

#endif
