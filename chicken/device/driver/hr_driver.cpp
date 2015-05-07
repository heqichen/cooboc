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
		usleep(10000UL);
	}
}

HrDriver::HrDriver(Io *io)
	:	mSerialHandler			(NULL),
		mIsReadThreadRunning	(false),
		mHasNewData				(false)
{
	mSerialHandler = io->getSerialHandler("/dev/ttyUSB0");
	mSerialHandler->setBlockRead(true);
	mSerialHandler->setBaudrate(9600);
	mIsReadThreadRunning = true;
	pthread_create(&mReadThread, NULL, &startReadHrDataThread, this);
	mDist[0] = 0;
	mDist[1] = 0;
}

HrDriver::~HrDriver()
{
	mIsReadThreadRunning = false;
	pthread_join(mReadThread, NULL);
}

void HrDriver::readData(void)
{
	int i;
	int a0, a1;
#warning "current length of packet from hr sensor arduino is 4"

	int length = mSerialHandler->recv(mReadBuffer,1024);
	for (i=0; i<length; ++i)
	{
		if (mSaModem.demodulateByte(mReadBuffer[i]))
		{
			mSaModem.getLastPayload(mReadDataBuffer);
			a0 = mReadDataBuffer[1];
			a0 <<= 8;
			a0 |= mReadDataBuffer[0];

			a1 = mReadDataBuffer[3];
			a1 <<= 8;
			a1 |= mReadDataBuffer[2];

			mDist[0] = a0;
			mDist[1] = a1;
			mHasNewData = true;
		}
	}

	//cout<<"read " << length <<endl;
}



const int *HrDriver::getDistData()
{
	mHasNewData = false;
	return mDist;
}