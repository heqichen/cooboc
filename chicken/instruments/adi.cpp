#include "adi.h"

ADI::ADI(Imu *imu)
	:	mImu	(imu),
		mVirtualImu	(NULL)
{

}

ADI::~ADI()
{

}

ImuAttitude ADI::getAttitude()
{
	ImuAttitude imu;
	if (mVirtualImu!=NULL && mVirtualImu->isInService())
	{
		imu = mVirtualImu->getAttitude();
	}
	else
	{
		imu = mImu->getAttitude();
	}
	return imu;
}