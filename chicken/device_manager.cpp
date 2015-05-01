#include "device_manager.h"
#include "./device/device_types.h"

#include <iostream>
using namespace std;

DeviceManager::DeviceManager(Io *io)
	:	mIo	(io),
		mNumRunningDevices	(0)
{

}

DeviceManager::~DeviceManager()
{
	cout<<"shutting down device manager"<<endl;
	int i;
	for (i=0; i<mNumRunningDevices; ++i)
	{
		delete mRunningDevices[i];
	}
}

Imu *DeviceManager::getImu(void)
{
	IDevice *imuDev = findFirstDeviceByType(DEVICE_TYPE_IMU);
	if (NULL == imuDev)
	{
		imuDev = new Imu(mIo);
		imuDev->init();
		cout<<"imu inited"<<endl;
		mRunningDevices[mNumRunningDevices] = imuDev;
		++mNumRunningDevices;
	}
	return (Imu*)imuDev;
}

Radio *DeviceManager::getRadio(void)
{
	IDevice *radioDev = findFirstDeviceByType(DEVICE_TYPE_RADIO);

	if (NULL == radioDev)
	{
		radioDev = new Radio(mIo);
		radioDev->init();
		mRunningDevices[mNumRunningDevices] = radioDev;
		++mNumRunningDevices;
	}

	return (Radio*)radioDev;
}

ServoController *DeviceManager::getServoController(void)
{
	IDevice *scDev = findFirstDeviceByType(DEVICE_TYPE_SERVO_CONTROLLER);
	if (NULL == scDev)
	{
		scDev = new ServoController(mIo);
		scDev->init();
		mRunningDevices[mNumRunningDevices] = scDev;
		++mNumRunningDevices;
	}
	return (ServoController*) scDev;
}

Aoa *DeviceManager::getAoa(void)
{
	IDevice *aoaDev = findFirstDeviceByType(DEVICE_TYPE_AOA);
	if (NULL == aoaDev)
	{
		aoaDev = new Aoa(mIo);
		aoaDev->init();
		mRunningDevices[mNumRunningDevices] = aoaDev;
		++mNumRunningDevices;
	}
	return (Aoa*) aoaDev;
}

Beeper *DeviceManager::getBeeper(void)
{
	IDevice *beeperDev = findFirstDeviceByType(DEVICE_TYPE_BEEPER);
	if (NULL == beeperDev)
	{
		beeperDev = new Beeper(mIo);
		beeperDev->init();
		mRunningDevices[mNumRunningDevices] = beeperDev;
		++mNumRunningDevices;
	}
	return (Beeper*)beeperDev;
}

Agl *DeviceManager::getAgl(void)
{
	IDevice *aglDev = findFirstDeviceByType(DEVICE_TYPE_AGL);
	if (NULL == aglDev)
	{
		aglDev = new Agl(mIo);
		aglDev->init();
		mRunningDevices[mNumRunningDevices] = aglDev;
		++mNumRunningDevices;
	}
	return (Agl*)aglDev;
}

VirtualImu *DeviceManager::getVirtualImu(void)
{
	IDevice *virtualImuDev = findFirstDeviceByType(DEVICE_TYPE_VIRTUAL_IMU);
	if (NULL == virtualImuDev)
	{
		virtualImuDev = new VirtualImu(mIo);
		virtualImuDev->init();
		mRunningDevices[mNumRunningDevices] = virtualImuDev;
		++mNumRunningDevices;
	}
	return (VirtualImu*)virtualImuDev;
}

IDevice *DeviceManager::findFirstDeviceByType(int deviceType)
{
	int i;
	for (i=0; i<mNumRunningDevices; ++i)
	{
		if (mRunningDevices[i]->getDeviceType() == deviceType)
		{
			return mRunningDevices[i];
		}
	}
	return NULL;
}


