#include "APL_Manager.h"

Display *gpDisplay;
GLCD *configGlcd;
J1939APP *gpJ1939;
BASE_MODES::GCU_MODE_VARS_t _vars = {BASE_MODES::ENGINE_OFF_READY, BASE_MODES::NO_TIMER_RUNNING};

APL_Manager::APL_Manager():
 bDeviceInConfigMode(false),
_MbApp(*this, _cfgz, _gcuAlarms, _engineMonitoring, _AutoMode, _startStop),
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
         _ManualMode, _display, _cfgc, _sleep,_J1939,_BTSMode,_CyclicMode, _EngineStartValidity, _MbApp),
_J1939(*this, _cfgc, _cfgz, _engineMonitoring,_gcuAlarms,_AutoMode ),
_EngineStartValidity(_cfgz, _gcuAlarms),
_PowerOnUpdateTimer{0, false}
{
    AcSensors.ConfigureISensing(NO_CURRENT_MEASUREMENT);
    gpDisplay = &_display;
    gpJ1939 = &_J1939;
    UTILS_ResetTimer(&_PowerOnUpdateTimer);
    configGlcd = &this->ObjGlcd;

    ObjRTC.Init();

    /* HSD K-factors are different for different PCBs
     The SetHSDKFactor function will set the calculations properly.
     Can make use of CFGC or DeviceMap files to set the value
     instead of changing the firmware for all different values.
     */

    if(_cfgc.GetPCBPID()==12209)
    {
        AnlgIp.SetHSDKFactor(1547.0F);
    }
    else if((_cfgc.GetPCBPID()==15046) || (_cfgc.GetPCBPID()==15559)) ////VN7140ASTR HSD
    {
        AnlgIp.SetHSDKFactor(475.0F);
    }
    else
    {
        AnlgIp.SetHSDKFactor(1547.0F);
    }

    prvCheckFirmwareInfo();
    prvSetFwVerInMb();
    prvCheckAndLogFlashingEvent();
}

void APL_Manager::Update()
{

    HAL_Manager::Update();
    _sleep.Update();
    if(!AmIInSleep())
    {

        if( (!_cfgc.IsC02Error()) && (!_cfgz.IsC03Error()) )
        {
            _EngineStartValidity.EngineStartValiditySM(bDeviceInConfigMode);
            _engineMonitoring.Update();
            _startStop.Update();
            _gcuAlarms.Update(bDeviceInConfigMode);
            _ManualMode.Update(bDeviceInConfigMode);
            _AutoMode.Update(bDeviceInConfigMode);
            _BTSMode.Update(bDeviceInConfigMode);
            _CyclicMode.Update(bDeviceInConfigMode);
            _cfgc.Update();
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
            _chrgAlt.Update();
        }

    }



}

void APL_Manager::prvSetFwVerInMb()
{
    _MbApp.SetFwRevision(_stFirmwareInfo.u8Revision);
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

bool APL_Manager::prvCheckIfFirmwareUpdated()
{
    static uint16_t u16PrevFirmwareCRC;
    uint16_t u16CurrentFirmwareCRC;
    bool bFirmwareUpdated = false;

    Objeeprom.BlockingRead(PREV_FW_META_DATA_CRC_ADDRESS, (uint8_t*)&u16PrevFirmwareCRC,
                                sizeof(u16PrevFirmwareCRC));
    Objpflash.Read(FIRMWARE_META_DATA_ADDRESS, (uint8_t*)&u16CurrentFirmwareCRC,
                                sizeof(u16CurrentFirmwareCRC));

    if(u16PrevFirmwareCRC != u16CurrentFirmwareCRC)
    {
        u16PrevFirmwareCRC = u16CurrentFirmwareCRC;
        Objeeprom.RequestWrite(PREV_FW_META_DATA_CRC_ADDRESS,
                                            (uint8_t*)&u16PrevFirmwareCRC, sizeof(u16PrevFirmwareCRC), NULL);
        bFirmwareUpdated = true;
    }

    return bFirmwareUpdated;
}

void APL_Manager::prvCheckAndLogFlashingEvent()
{
    if(prvCheckIfFirmwareUpdated())
    {
        _gcuAlarms.LogEvent(GCU_ALARMS::Firmware_Flashing_id, (uint8_t)0);
    }

    if(_cfgz.CheckIfFactoryProfilesUpdatedViaBL())
    {
        _gcuAlarms.LogEvent(GCU_ALARMS::Factory_Profile_flashing_id,(uint8_t)0);
    }

    if(_cfgz.CheckIfActiveProfileUpdatedViaBL())
    {
        _gcuAlarms.LogEvent(GCU_ALARMS::Active_Profile_flashing_id,(uint8_t)0);
    }
}
