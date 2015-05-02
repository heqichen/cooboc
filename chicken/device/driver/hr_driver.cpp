#include "hr_driver.h"



#include <iostream>
using namespace std;


HrDriver::HrDriver(Io *io)
	:	mSerialHandler	(NULL)
{
	mSerialHandler = io->getSerialHandler("/dev/ttyUSB0");

}

HrDriver::~HrDriver()
{
}

void HrDriver::test()
{
	cout<<"test"<<endl;
}

