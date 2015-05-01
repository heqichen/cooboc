#include "iic_handler.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

IicHandler::IicHandler(const char *portName)
	:	mI2cFile			(0),
		mCurrentDeviceAddr	(0x00)
{
	int portNameLen = strlen(portName);
	mPortName = new char[portNameLen+1];
	strncpy(mPortName, portName, portNameLen);
	mI2cFile = open(mPortName, O_RDWR);
	pthread_mutex_init(&mMutex, NULL);
}

IicHandler::~IicHandler()
{
	delete [] mPortName;
	if (mI2cFile > 0)
	{
		close(mI2cFile);
	}
	pthread_mutex_destroy(&mMutex);
}

const char *IicHandler::getPortName() const
{
	return mPortName;
}

bool IicHandler::sendRegAddr(const uint8_t deviceAddr, const uint8_t regAddr)
{
	//TODO
	//need resend device address if last IO fail
	if (deviceAddr != mCurrentDeviceAddr)
	{
		mCurrentDeviceAddr = deviceAddr;
		ioctl(mI2cFile, I2C_SLAVE, mCurrentDeviceAddr);
	}
	
	mBuffer[0] = regAddr;
	
	int numRead = write(mI2cFile, mBuffer, 1);
	
	if (numRead < 1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool IicHandler::writeByte(const uint8_t deviceAddr, const uint8_t regAddr, const uint8_t value)
{
	int numRead = 0;
	
	pthread_mutex_lock(&mMutex);
	
	if (deviceAddr != mCurrentDeviceAddr)
	{
		mCurrentDeviceAddr = deviceAddr;
		ioctl(mI2cFile, I2C_SLAVE, mCurrentDeviceAddr);
	}
	
	mBuffer[0] = regAddr;
	mBuffer[1] = value;
	numRead = write(mI2cFile, mBuffer, 2);
	
	pthread_mutex_unlock(&mMutex);
	
	if (numRead < 2)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

bool IicHandler::readU8(const uint8_t deviceAddr, const uint8_t regAddr, uint8_t &value)
{
	bool iicOk = false;
	pthread_mutex_lock(&mMutex);
	if (sendRegAddr(deviceAddr, regAddr))
	{
		int numRead = 0;
		numRead = read(mI2cFile, mBuffer, 1);
		if (numRead == 1)
		{
			value = mBuffer[0];
			iicOk = true;
		}
	}
	pthread_mutex_unlock(&mMutex);
	return iicOk;
}

bool IicHandler::readS8(const uint8_t deviceAddr, const uint8_t regAddr, int8_t &value)
{
	bool iicOk = false;
	pthread_mutex_lock(&mMutex);
	if (sendRegAddr(deviceAddr, regAddr))
	{
		int numRead = 0;
		numRead = read(mI2cFile, mBuffer, 1);
		if (numRead == 1)
		{
			value = (int8_t)mBuffer[0];
			iicOk = true;
		}
	}
	pthread_mutex_unlock(&mMutex);
	return iicOk;
}

bool IicHandler::readU16BE(const uint8_t deviceAddr, const uint8_t regAddr, uint16_t &value)
{
	bool iicOk = false;
	pthread_mutex_lock(&mMutex);

	if (sendRegAddr(deviceAddr, regAddr))
	{
		int numRead = 0;
		numRead = read(mI2cFile, mBuffer, 2);
		if (numRead == 2)
		{
			value = (uint8_t)mBuffer[0];
			value <<= 8;
			value |= (uint8_t)mBuffer[1];
			iicOk = true;
		}
	}

	pthread_mutex_unlock(&mMutex);
	return iicOk;
}

bool IicHandler::readS16LE(const uint8_t deviceAddr, const uint8_t regAddr, int16_t &value)
{
	bool iicOk = false;
	pthread_mutex_lock(&mMutex);

	if (sendRegAddr(deviceAddr, regAddr))
	{
		int numRead = 0;
		numRead = read(mI2cFile, mBuffer, 2);
		if (numRead == 2)
		{
			value = (int8_t)mBuffer[1];
			value <<= 8;
			value |= (uint8_t)mBuffer[0];
			iicOk = true;
		}
	}

	pthread_mutex_unlock(&mMutex);
	return iicOk;
}

