#ifndef __ADS7828_DRIVER_H__
#define __ADS7828_DRIVER_H__

#include "io/io.h"

#ifndef NULL
#define NULL 0
#endif

#define ADS7828_SINGEL_DATA_COMMAND		0b10000000
#define ADS7828_DIFFERENTIAL_COMMAND	0b00000000

#define ADS7828_POWER_UP_BIT			0b00000100
#define ADS7828_INTERNAL_REF_BIT		0b00001000


const uint8_t	ADS7828_CHANNEL_BITS[8]	=	{
												0b00000000,
												0b01000000,
												0b00010000,
												0b01010000,
												0b00100000,
												0b01100000,
												0b00110000,
												0b01110000
											};

class Ads7828Driver
{
	public:
		Ads7828Driver(Io *io, uint8_t address);
		uint16_t readSingleChannel(int id);
		void useInternalRef(){mUsingInternalRef=true;}
		void useExternalRef(){mUsingInternalRef=false;}
	private:
		IicHandler *mIicHandler;
		uint8_t mAddress;
		bool mUsingInternalRef;
};

#endif