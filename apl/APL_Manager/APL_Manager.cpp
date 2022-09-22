#include "APL_Manager.h"

Display *gpDisplay;
GLCD *configGlcd;
J1939APP *gpJ1939;
BASE_MODES::GCU_MODE_VARS_t _vars = {BASE_MODES::ENGINE_OFF_READY, BASE_MODES::NO_TIMER_RUNNING};

APL_Manager::APL_Manager():
 bDeviceInConfigMode(false),
_MbApp(*this, _cfgz, _gcuAlarms, _engineMonitoring, _AutoMode, _Egov),
_cfgc(*this),
_cfgz(*this, _MbApp, _cfgc),
_sleep(*this, _cfgz),
_gcuAlarms(*this, _cfgz),
_engineMonitoring(_cfgz, _gcuAlarms, *this),
_chrgAlt(*this, _engineMonitoring, _cfgz, _gcuAlarms),
_startStop(*this, _engineMonitoring, _cfgz,  _gcuAlarms, _vars, _chrgAlt,_EngineStartValidity),
_ManualMode(*this, _engineMonitoring, _cfgz, _gcuAlarms, _startStop, _vars),
_AutoMode(*this, _engineMonitoring, _cfgz, _gcuAlarms, _startStop, _vars),
_BTSMode(*this, _engineMonitoring, _cfgz, _gcuAlarms, _startStop, _vars),
_CyclicMode(*this, _engineMonitoring, _cfgz, _gcuAlarms, _startStop, _vars),
_display(this->ObjGlcd),
_MainUI(*this,_cfgz, _gcuAlarms,_engineMonitoring, _startStop,
         _ManualMode, _display, _cfgc, _sleep, _Egov, _J1939,_BTSMode,_CyclicMode, _EngineStartValidity),
_Egov(*this, _cfgz,_engineMonitoring, _startStop),
_J1939(*this, _cfgc, _cfgz, _engineMonitoring,_gcuAlarms,_MbApp,_AutoMode, _Egov ),
_EngineStartValidity(_cfgz, _gcuAlarms),
_PowerOnUpdateTimer{0}
{
    AcSensors.ConfigureISensing(NO_CURRENT_MEASUREMENT);
    gpDisplay = &_display;
    gpJ1939 = &_J1939;
    UTILS_ResetTimer(&_PowerOnUpdateTimer);
    configGlcd = &this->ObjGlcd;

    ObjRTC.Init();


    //  HSD_K_FACTOR        (470.0F)   //VNQ5E250AJTR-E HSD
    //  HSD_K_Factor        (1250.0f)  //VNQ7050AJ
    if((_cfgc.IsSGC110() && (_cfgc.GetPCBPID()==12430))
       ||(_cfgc.IsSGC120() && (_cfgc.GetPCBPID()==12429))
               ||(_cfgc.IsSGC421())||(_cfgc.IsSGC420()))
    {
//        AnlgIp.SetHSDKFactor(470.0F);

        AnlgIp.SetHSDKFactor(1528.0F);
    }
    else
    {
        AnlgIp.SetHSDKFactor(1250.0F);
    }


    if(_cfgc.IsSGC110())
    {
        _stFirmwareInfo.u32PLMID=12808;
    }
    else if(_cfgc.IsSGC120())
    {
        _stFirmwareInfo.u32PLMID=13195;
    }
    prvCheckFirmwareInfo();
}

void APL_Manager::Update()
{
    HAL_Manager::Update();
    _sleep.Update();
    if(!AmIInSleep())
    {
        if((!_cfgc.IsC02Error()) && (!_cfgz.IsC03Error()) )
        {

            _EngineStartValidity.EngineStartValiditySM(bDeviceInConfigMode);
            _engineMonitoring.Update(bDeviceInConfigMode);
            _startStop.Update(bDeviceInConfigMode);
            _gcuAlarms.Update(bDeviceInConfigMode);
            _ManualMode.Update(bDeviceInConfigMode);
            _AutoMode.Update(bDeviceInConfigMode);
            _BTSMode.Update(bDeviceInConfigMode);
            _CyclicMode.Update(bDeviceInConfigMode);

            _cfgc.Update();
            if(_cfgc.IsSGC421())
            {
                _Egov.Update();
            }

        }
    }

    bDeviceInConfigMode = _MainUI.Update();
    if(UTILS_GetElapsedTimeInSec(&_PowerOnUpdateTimer) >= BOOT_TIMER_SEC)
    {
        /*Don't update in case you are in sleep*/
        if(!AmIInSleep() &&(!_cfgc.IsC02Error()) && (!_cfgz.IsC03Error()))
        {
            _cfgz.Update();
            _MbApp.Update();
            _J1939.Update(bDeviceInConfigMode);
            _chrgAlt.Update(bDeviceInConfigMode);
        }
    }
}

void APL_Manager::prvCheckFirmwareInfo()
{
    //Read Stored F/W info
    INFO_t _stLocalFirmwareInfo ;

    HAL_Manager::Objeeprom.BlockingRead(EXT_EEPROM_FIRMWARE_INFO_ADDRESS,
                                        (uint8_t*)&_stLocalFirmwareInfo, sizeof(INFO_t));
    if((_stFirmwareInfo.u8Revision!=_stLocalFirmwareInfo.u8Revision)
            || (_stFirmwareInfo.u8Version!=_stLocalFirmwareInfo.u8Version))
    {
        HAL_Manager::Objeeprom.RequestWrite(EXT_EEPROM_FIRMWARE_INFO_ADDRESS,
                                            (uint8_t*)&_stFirmwareInfo,  sizeof(INFO_t), NULL);
    }
}

