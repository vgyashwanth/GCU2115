#include "Test.h"


static vector<Test*> testList;
static NV_LOG_STATUS_t _requestResult;

void callback(NV_LOG_STATUS_t evt);
static void EraseFlash();
static void RunLog(NV_LOG &nvLog);

class Test1_NewFlashInit: public Test
{
public:
    bool Run()
    {
        EraseFlash();
        NV_LOG_Interface iface;
        NV_LOG::NV_LOG_CFG_t cfg = {20, 0, 64, 3};
        NV_LOG _nvLog(cfg, iface);
        uint8_t au8ReadData[4];
        FLASH flash;
        RunLog(_nvLog);

        flash.Read(0, au8ReadData, 4, NULL);
        uint32_t BRN = (au8ReadData[0]<<24)|(au8ReadData[1]<<16)|
                          (au8ReadData[2]<<8)|(au8ReadData[3]);
        if(BRN == 0)
        {
            return true;
        }
        return false;
    }

    string TestDescription()
    {
        return "Test1_NewFlashInit";
    }
};


class Test2_Write: public Test
{
public:
    bool Run()
    {
        EraseFlash();
        NV_LOG_Interface iface;
        NV_LOG::NV_LOG_CFG_t cfg = {20, 0, 64, 3};
        NV_LOG _nvLog(cfg, iface);

        uint8_t pu8Data[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
        _nvLog.LogData(pu8Data, 20, callback);
        RunLog(_nvLog);
        if(_requestResult == NV_LOG_SUCCESS)
        {
            return true;
        }
        return false;
    }

    string TestDescription()
    {
        return "Test2_Write";
    }
};


class Test3_Read: public Test
{
public:
    bool Run()
    {
        EraseFlash();
        NV_LOG_Interface iface;
        NV_LOG::NV_LOG_CFG_t cfg = {20, 0, 64, 3};
        NV_LOG _nvLog(cfg, iface);

        uint8_t pu8Data[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
        uint8_t au8ReadData[20];
        _nvLog.LogData(pu8Data, 20, callback);
        _nvLog.Read(au8ReadData, 20, 0, callback);
        for(int i=0;i<200;i++)
        {
            _nvLog.Update();
        }

        if(_requestResult == NV_LOG_SUCCESS)
        {
            return true;
        }
        return false;
    }

    string TestDescription()
    {
        return "Test3_Read";
    }
};

class Test4_LogNotAvailable: public Test
{
    public:
        bool Run()
        {
            EraseFlash();
            NV_LOG_Interface iface;
            NV_LOG::NV_LOG_CFG_t cfg = {20, 0, 64, 3};
            NV_LOG _nvLog(cfg, iface);

            uint8_t pu8Data[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
            uint8_t au8ReadData[20];
            _nvLog.LogData(pu8Data, 20, callback);
            _nvLog.LogData(pu8Data, 20, callback);
            _nvLog.Read(au8ReadData, 20, 2, callback);
            for(int i=0;i<200;i++)
            {
                _nvLog.Update();
            }
            if(_requestResult == NV_LOG_LOG_NOT_AVAILABLE)
            {
                return true;
            }
            return false;
        }

        string TestDescription()
        {
            return "Test4_LogNotAvailable";
        }
};

class Test5_LogCorrupted: public Test
{
    public:
        bool Run()
        {
            EraseFlash();
            NV_LOG_Interface iface;
            NV_LOG::NV_LOG_CFG_t cfg = {20, 0, 64, 3};
            NV_LOG _nvLog(cfg, iface);
            uint8_t pu8Data[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
            uint8_t au8ReadData[20] = {0};
            _nvLog.LogData(pu8Data, 20, callback);
            _nvLog.LogData(pu8Data, 20, callback);

            RunLog(_nvLog);
            
            FLASH flash;
            flash.Write(10, pu8Data, 1, NULL);
            
            _nvLog.Read(au8ReadData, 20, 0, callback);
            
            RunLog(_nvLog);
            
            if(_requestResult == NV_LOG_CORRUPT)
            {
                return true;
            }
            return false;
        }

        string TestDescription()
        {
            return "Test5_LogCorrupted";
        }
};

class Test6_RollOver: public Test
{
    public:
        bool Run()
        {
            EraseFlash();
            NV_LOG_Interface iface;
            NV_LOG::NV_LOG_CFG_t cfg = {20, 0, 64, 3};
            NV_LOG _nvLog(cfg, iface);
            uint8_t au8WriteData[20];
            uint8_t au8ReadData[20] = {0};

            
            memset(au8WriteData,1,sizeof(au8WriteData));
            _nvLog.LogData(au8WriteData, 20, callback);
            RunLog(_nvLog);

            memset(au8WriteData,2,sizeof(au8WriteData));
            _nvLog.LogData(au8WriteData, 20, callback);
            RunLog(_nvLog);

            memset(au8WriteData,3,sizeof(au8WriteData));
            _nvLog.LogData(au8WriteData, 20, callback);
            RunLog(_nvLog);

            memset(au8WriteData,4,sizeof(au8WriteData));
            _nvLog.LogData(au8WriteData, 20, callback);
            RunLog(_nvLog);

            memset(au8WriteData,5,sizeof(au8WriteData));
            _nvLog.LogData(au8WriteData, 20, callback);
            RunLog(_nvLog);

            memset(au8WriteData,6,sizeof(au8WriteData));
            _nvLog.LogData(au8WriteData, 20, callback);
            RunLog(_nvLog);

            memset(au8WriteData,7,sizeof(au8WriteData));
            _nvLog.LogData(au8WriteData, 20, callback);
            RunLog(_nvLog);
            
            _nvLog.Read(au8ReadData, 20, 0, callback);
            
            RunLog(_nvLog);

            /* Oldest log shall correspond to the 3rd logged value*/
            uint8_t au8ExpectedData[20];
            memset(au8ExpectedData,3, sizeof(au8ExpectedData));
            
            if(0 != memcmp(au8ExpectedData, au8ReadData, sizeof(au8ReadData)))
            {
                return false;
            }


            _nvLog.Read(au8ReadData, 20, 4, callback);            
            RunLog(_nvLog);

            /* 4th oldest log shall correspond to the last logged value*/
            memset(au8ExpectedData,7,sizeof(au8ExpectedData));
            if(0 != memcmp(au8ExpectedData, au8ReadData, sizeof(au8ReadData)))
            {
                return false;
            }
            return true;
        }

        string TestDescription()
        {
            return "Test6_RollOver";
        }
};


class Test7_WriteMismatch: public Test
{
    public:
        bool Run()
        {
            EraseFlash();
            NV_LOG_Interface iface;
            NV_LOG::NV_LOG_CFG_t cfg = {20, 0, 64, 3};
            NV_LOG _nvLog(cfg, iface);

            uint8_t pu8Data[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
            NV_LOG_STATUS_t status = _nvLog.LogData(pu8Data, 22, callback);
            RunLog(_nvLog);
            if(status == NV_LOG_SIZE_MISMATCH)
            {
                return true;
            }
            return false;
        }

        string TestDescription()
        {
            return "Test7_WriteMismatch";
        }
};


class Test8_ReadMismatch: public Test
{
    public:
        bool Run()
        {
            EraseFlash();
            NV_LOG_Interface iface;
            NV_LOG::NV_LOG_CFG_t cfg = {20, 0, 64, 3};
            NV_LOG _nvLog(cfg, iface);

            uint8_t pu8Data[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
            uint8_t au8ReadData[20];
            _nvLog.LogData(pu8Data, 20, callback);
            NV_LOG_STATUS_t status = _nvLog.Read(au8ReadData, 22, 0, callback);
            RunLog(_nvLog);
            if(status == NV_LOG_SIZE_MISMATCH)
            {
                return true;
            }
            return false;
        }

        string TestDescription()
        {
            return "Test8_ReadMismatch";
        }
};

class Test9_BRNCorruption: public Test
{
    public:
        bool Run()
        {
            EraseFlash();
            NV_LOG_Interface iface;
            NV_LOG::NV_LOG_CFG_t cfg = {20, 0, 64, 3};
            NV_LOG _nvLog(cfg, iface);

            uint8_t pu8Data[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
            uint8_t au8ReadData[20];

            _nvLog.LogData(pu8Data, 20, callback);
            RunLog(_nvLog);
            _nvLog.LogData(pu8Data, 20, callback);
            RunLog(_nvLog);
            _nvLog.LogData(pu8Data, 20, callback);
            RunLog(_nvLog);
            _nvLog.LogData(pu8Data, 20, callback);
            RunLog(_nvLog);
            _nvLog.LogData(pu8Data, 20, callback);
            RunLog(_nvLog);
            _nvLog.LogData(pu8Data, 20, callback);
            RunLog(_nvLog);
            _nvLog.LogData(pu8Data, 20, callback);
            RunLog(_nvLog);

            FLASH flash;
            flash.Erase(0, 1, NULL);

            NV_LOG _TmpNvLog(cfg, iface);
            RunLog(_TmpNvLog);

            if(_TmpNvLog.GetLogCount() == 4)
            {
                return true;
            }
            return false;
        }

        string TestDescription()
        {
            return "Test9_BRNCorruption";
        }
};

vector<Test*>& CreateTestSuite()
{
    testList.push_back(new Test1_NewFlashInit);
    testList.push_back(new Test2_Write);
    testList.push_back(new Test3_Read);
    testList.push_back(new Test4_LogNotAvailable);
    testList.push_back(new Test5_LogCorrupted);
    testList.push_back(new Test6_RollOver);
    testList.push_back(new Test7_WriteMismatch);
    testList.push_back(new Test8_ReadMismatch);
    testList.push_back(new Test9_BRNCorruption);

    return testList;
}

static void EraseFlash()
{
    FLASH flash;
    flash.Erase(0, NO_OF_BLOCKS, NULL);
}

static void RunLog(NV_LOG &nvLog)
{
    for(int i=0;i<500;i++)
    {
        nvLog.Update();
    }
}

void callback(NV_LOG_STATUS_t evt)
{
    _requestResult = evt;
}
