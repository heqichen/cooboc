#ifndef __BEEPER_DRIVER_H__
#define __BEEPER_DRIVER_H__

#include "io/io.h"
#include "io/gpio_handler.h"

#define DEFAULT_BEEPER_GPIO_KEY	"P9_42"

class BeeperDriver
{
	public:
		BeeperDriver(Io *io, const char *gpioKey);
		BeeperDriver(Io *io);
		~BeeperDriver();
		void makeNoise();
		void shutUp();
	private:
		char *mGpioKey;
		GpioHandler *mGpioHandler;
};

#endif
