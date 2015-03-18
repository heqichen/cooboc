#ifndef __AOA_H__
#define __AOA_H__

#include "i_device.h"
#include "driver/ads7828_driver.h"

#ifndef NULL
#define NULL 0
#endif

class Aoa	:	public IDevice
{
	public:
		Aoa(Io *io);
		~Aoa();
		virtual void init();
		double readAoa();
		double readSideSlip();
	private:
		Ads7828Driver *mAds7828;
};

#endif
