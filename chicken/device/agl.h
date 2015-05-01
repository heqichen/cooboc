#ifndef __AGL_H__
#define __AGL_H__

#include "i_device.h"
#include "driver/ads7828_driver.h"

#ifndef NULL
#define NULL 0
#endif

class Agl	:	public IDevice
{
	public:
		Agl(Io *io);
		~Agl();
		virtual void init();
		double measureAgl();
	private:
		Ads7828Driver *mAds7828;
};

#endif

