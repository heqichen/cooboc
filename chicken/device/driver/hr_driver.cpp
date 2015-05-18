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
		mHasNewData				(false),
		mHrPosition			(0)
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
	int16_t hrPos;
#warning "current length of packet from hr sensor arduino is 6"

	int length = mSerialHandler->recv(mReadBuffer,1024);
	for (i=0; i<length; ++i)
	{
		if (mSaModem.demodulateByte(mReadBuffer[i]))
		{
			mSaModem.getLastPayload(mReadDataBuffer);
			hrPos = mReadDataBuffer[1];
			hrPos <<= 8;
			hrPos |= mReadDataBuffer[0];

			a0 = mReadDataBuffer[3];
			a0 <<= 8;
			a0 |= mReadDataBuffer[2];

			a1 = mReadDataBuffer[5];
			a1 <<= 8;
			a1 |= mReadDataBuffer[4];

			mDist[0] = a0;
			mDist[1] = a1;
			mHrPosition = hrPos;
			mHasNewData = true;
			//cout<<mHrPosition<<endl;
		}
	}

	//cout<<"read " << length <<endl;
}



const int *HrDriver::getDistData()
{
	mHasNewData = false;
	return mDist;
}


void HrDriver::moveUp()
{
	mSerialHandler->send((const uint8_t *)"w", 1);
}


void HrDriver::moveDown()
{
	mSerialHandler->send((const uint8_t *)"s", 1);
}


void HrDriver::stopMove()
{
	mSerialHandler->send((const uint8_t *)" ", 1);
}

