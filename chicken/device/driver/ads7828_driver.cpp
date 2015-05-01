#include "ads7828_driver.h"

#include <iostream>
using namespace std;

Ads7828Driver::Ads7828Driver(Io *io, uint8_t address)
	:	mIicHandler	(NULL),
		mAddress(address),
		mUsingInternalRef	(true)
{
	mIicHandler = io->getIicHandler("/dev/i2c-1");
}


uint16_t Ads7828Driver::readSingleChannel(int id)
{
	uint8_t command = ADS7828_SINGEL_DATA_COMMAND;
	command |= ADS7828_CHANNEL_BITS[id];
	command |= ADS7828_POWER_UP_BIT;
	if (mUsingInternalRef)
	{
		command |= ADS7828_INTERNAL_REF_BIT;
	}
	uint16_t value;
	bool iicOk = mIicHandler->readU16BE(mAddress, command, value);
	return value;
}
