#ifndef __BEEPER_H__
#define __BEEPER_H__

#include "i_device.h"
#include "driver/io/io.h"
#include "driver/beeper_driver.h"


#ifndef NULL
#define NULL 0
#endif

class Beeper :	public IDevice
{
	public:
		Beeper(Io *io);
		~Beeper();
		virtual void init();
		void test();
	private:
		BeeperDriver *mBeeperDriver;
};

#endif

