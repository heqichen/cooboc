#include "i_device.h"

#include "device_types.h"

IDevice::IDevice()
	:	mDeviceType(DEVICE_TYPE_UNDEFINED),
		mIo	(NULL)

{

}

IDevice::IDevice(Io *io, int deviceType)
	:	mIo	(io),
		mDeviceType(deviceType)
{

}


IDevice::~IDevice()
{}