#ifndef _TESTS_H_
#define _TESTS_H_

#include "NVlog.h"
#include "FLASH.h"

class Test
{
    public:
        virtual bool Run()=0;
        virtual string TestDescription()=0;
};

vector<Test*>& CreateTestSuite();

#endif
