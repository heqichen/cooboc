#include "virtual_imu.h"
#include "device_types.h"
#include <utils/util.h>

VirtualImu::VirtualImu(Io *io)
	:	IDevice	(io, DEVICE_TYPE_VIRTUAL_IMU),
		mIsInService	(false),
		mLastValidDataTime	(0UL)
{

}

VirtualImu::~VirtualImu()
{


}

void VirtualImu::init()
{

}

void VirtualImu::updateAttitude(ImuAttitude attitude)
{
	mAttitude = attitude;
	mLastValidDataTime = millis();
	mIsInService = true;
}

bool VirtualImu::isInService()
{
	if (mIsInService)
	{
		if (millis() - mLastValidDataTime > VIRTUAL_IMU_TIMEOUT)
		{
			mIsInService = false;
		}
		return mIsInService;
	}
	else
	{
		return false;
	}
}