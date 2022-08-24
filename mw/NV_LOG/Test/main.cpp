#include "NVlog.h"
#include  "Test.h"
#include "NV_LOG_Interface.h"
#include <iostream>


int main()
{
    vector<Test*> &testSuite = CreateTestSuite();
    for(auto test:testSuite)
    {
        if(test->Run())
        {
            cout<<test->TestDescription()<<":"<<"PASS"<<endl;
        }
        else
        {
            cout<<test->TestDescription()<<":"<<"FAIL"<<endl;
        }
    }

    NV_LOG_Interface nv;
}