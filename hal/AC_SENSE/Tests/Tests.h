#ifndef _TESTS_H_
#define _TESTS_H_

#include "../AC_SENSE.h"
#include "Sine.h"
#include "mocks/AC_IP.h"
#include "../AC_SENSE_CONFIG.h"
#include "../AC_SENSE_DS.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>


#define PI                          (3.14f)

class Test
{
    public:
	    virtual bool Run()=0;
	    virtual string TestDescription()=0;
};

vector<Test*>& CreateTestSuite();

#endif
