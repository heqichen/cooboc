#include "hr_driver.h"

#include <unistd.h>
#include <iostream>
using namespace std;

void *startReadHrDataThread(void *hrDriverPtr)
{
	HrDriver *hrDriver = (HrDriver*)hrDriverPtr;
	while (hrDriver->isReadingThreadRunning())
	{
		hrDriver->readData();
	}
}

HrDriver::HrDriver(Io *io)
	:	mSerialHandler			(NULL),
		mIsReadThreadRunning	(false)
{
	mSerialHandler = io->getSerialHandler("/dev/ttyUSB0");
	mSerialHandler->setBlockRead(true);
	mSerialHandler->setBaudrate(9600);
	mIsReadThreadRunning = true;
	pthread_create(&mReadThread, NULL, &startReadHrDataThread, this);
}

HrDriver::~HrDriver()
{
	mIsReadThreadRunning = false;
	pthread_join(mReadThread, NULL);
}

void HrDriver::readData()
{
	int length = mSerialHandler->recv(mReadBuffer,1024);
	cout<<"read " << length <<endl;
	
	usleep(1000000UL);
}

