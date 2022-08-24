#include "../AC_SENSE.h"
#include "Sine.h"
#include "mocks/AC_IP.h"
#include "Tests.h"
#include <vector>
#include <iostream>




int main() 
{
	vector<Test*> Tests = CreateTestSuite();

	for(auto tst:Tests)
	{
		cout<<tst->TestDescription()<<endl;
		if(tst->Run())
			cout<<"TEST PASS"<<endl<<endl;
		else
			cout<<"TEST FAIL"<<endl<<endl;
	}
    return 0;
}

