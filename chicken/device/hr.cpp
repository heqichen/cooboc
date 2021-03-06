#include "hr.h"
#include "device_types.h"


Hr::Hr(Io *io)
	:	IDevice		(io, DEVICE_TYPE_HR),
		mHrDriver	(NULL)
{

}

Hr::~Hr()
{
	if (mHrDriver != NULL)
	{
		delete mHrDriver;
	}
}

void Hr::init()
{
	mHrDriver = new HrDriver(mIo);
}


void Hr::test()
{
}

bool Hr::hasNewData()
{
	return mHrDriver->hasNewData();
}

const int *Hr::getDistData()
{
	return mHrDriver->getDistData();
}

const int Hr::getHrPosition()
{
	return mHrDriver->getHrPosition();
}


void Hr::moveUp()
{
	mHrDriver->moveUp();
}

void Hr::moveDown()
{
	mHrDriver->moveDown();
}

void Hr::stopMove()
{
	mHrDriver->stopMove();
}

void Hr::resetPosition()
{
	mHrDriver->resetPosition();
}