#include "bmx055_driver.h"

#include <iostream>
using namespace std;

Bmx055Driver::Bmx055Driver(Io *io)
	:	mIicHandler	(NULL)
{
	mIicHandler = io->getIicHandler("/dev/i2c-1");

	bool iicOk;
	uint8_t chipId;

	iicOk = mIicHandler->readU8(ACC_ADDR, ACC_CHIPID_ADDR, chipId);
	if (iicOk && 0xFA == chipId)
	{
		cout<<"acc good"<<endl;
	}
	else
	{
		cout<<"acc bad"<<endl;
	}

	iicOk = mIicHandler->readU8(GYRO_ADDR, GYRO_CHIPID_ADDR, chipId);
	if (iicOk && 0x0F == chipId)
	{
		cout<<"gyro good"<<endl;
	}
	else
	{
		cout<<"gyro bad"<<endl;
	}
	
	if (wakeupMag())
	{
		cout<<"mag ok"<<endl;
	}
	else
	{
		cout<<"mag bad"<<endl;
	}
	
	
	resetAcc();
	resetGyro();

}

bool Bmx055Driver::wakeupMag()
{
	bool iicOk = true;
	uint8_t chipId = 0;
	//write 1 to power on bit
	iicOk &= mIicHandler->writeByte(MAG_ADDR, MAG_POWER_ADDR, 0x01);
	iicOk &= mIicHandler->readU8(MAG_ADDR, MAG_CHIPID_ADDR, chipId);
	//datashit P 135, set mag to 30hz ODR, and normal mode.
	iicOk &= mIicHandler->writeByte(MAG_ADDR, MAG_OP_ADDR, 0b00111000);
	//cannot bring mag to suspend / sleep mode after soft reset

	//set nXY to 47 repetions, see datashit p122
	iicOk &= mIicHandler->writeByte(MAG_ADDR, MAG_REP_XY_ADDR, 23);
	//set nZ to 83 repetions, see datashit P122
	iicOk &= mIicHandler->writeByte(MAG_ADDR, MAG_REP_Z_ADDR, 41);

	return iicOk && (chipId == 0x32);
}

void Bmx055Driver::resetAcc(void)
{
	//soft reset acc meter
	mIicHandler->writeByte(ACC_ADDR, ACC_SOFTRESET_ADDR, 0xB6);
	//set range to += 16g
	//datasheet page 57.
	mIicHandler->writeByte(ACC_ADDR, ACC_RANGE_ADDR, 0b00001100);
}

void Bmx055Driver::resetGyro(void)
{
	//soft reset gyro
	mIicHandler->writeByte(GYRO_ADDR, GYRO_SOFTRESET_ADDR, 0xB6);
	//set range to +=1000degree/s
	//datasheet page 99.
	mIicHandler->writeByte(GYRO_ADDR, GYRO_RANGE_ADDR, 0b0000001);
}

bool Bmx055Driver::readGyroAxis(uint8_t msbAddr, uint8_t lsbAddr, int &value)
{

	bool iicOk = true;
	int8_t msb;
	uint8_t lsb;
	int ret;
	
	iicOk &= mIicHandler->readS8(GYRO_ADDR, msbAddr, msb);
	ret = msb;
	ret <<= 8;
	
	iicOk &= mIicHandler->readU8(GYRO_ADDR, lsbAddr, lsb);
	ret |= lsb;
	value = ret;
	
	return iicOk;
}


bool Bmx055Driver::readGyro(RawGyroValue &value)
{
	bool readOk = true;
	
	int axisValue = value.x;
	readOk &= readGyroAxis(GYRO_X_MSB_ADDR, GYRO_X_LSB_ADDR, axisValue);
	value.x = axisValue;

	axisValue = value.y;
	readOk &= readGyroAxis(GYRO_Y_MSB_ADDR, GYRO_Y_LSB_ADDR, axisValue);
	value.y = axisValue;

	axisValue = value.z;
	readOk &= readGyroAxis(GYRO_Z_MSB_ADDR, GYRO_Z_LSB_ADDR, axisValue);
	value.z = axisValue;

	return readOk;
}

bool Bmx055Driver::readAccAxis(uint8_t msbAddr, uint8_t lsbAddr, int &value)
{
	bool iicOk = true;
	int8_t msb;
	uint8_t lsb;
	int result;

	iicOk &= mIicHandler->readU8(ACC_ADDR, lsbAddr, lsb);
	iicOk &= mIicHandler->readS8(ACC_ADDR, msbAddr, msb);

	result = msb;
	result <<= 8;
	result |= lsb;
	result >>= 4;
	
	value = result;
		
	return iicOk;
}

bool Bmx055Driver::readAcc(RawAccValue &value)
{
	bool readOk = true;
	int axisValue = value.x;
	readOk &= readAccAxis(ACC_X_MSB_ADDR, ACC_X_LSB_ADDR, axisValue);
	value.x = axisValue;

	axisValue = value.y;
	readOk &= readAccAxis(ACC_Y_MSB_ADDR, ACC_Y_LSB_ADDR, axisValue);
	value.y = axisValue;

	axisValue = value.z;
	readOk &= readAccAxis(ACC_Z_MSB_ADDR, ACC_Z_LSB_ADDR, axisValue);
	value.z = axisValue;

	return readOk;
}

bool Bmx055Driver::readMag(RawMagValue &value)
{
	bool iicOk = true;
	int16_t axisValue = (int16_t)value.x;
	iicOk &= mIicHandler->readS16LE(MAG_ADDR, MAG_X_LSB_ADDR, axisValue);
	value.x = axisValue >> 3;

	axisValue = (int16_t)value.y;
	iicOk &= mIicHandler->readS16LE(MAG_ADDR, MAG_Y_LSB_ADDR, axisValue);
	value.y = axisValue >> 3;

	axisValue = (int16_t)value.z;
	iicOk &= mIicHandler->readS16LE(MAG_ADDR, MAG_Z_LSB_ADDR, axisValue);
	value.z = axisValue >> 1;

	return iicOk;
}
