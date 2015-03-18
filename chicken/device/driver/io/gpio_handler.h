#ifndef __GPIO_HANDLER_H__
#define __GPIO_HANDLER_H__

#include "gpio_const.h"
#include "gpio_manager.h"

#ifndef NULL
#define NULL 0
#endif

#define GPIO_INPUT	0
#define GPIO_OUTPUT	1

#define GPIO_HIGH	1
#define GPIO_LOW	0

class GpioHandler
{
	public:
		GpioHandler(const char *gpioKey);
		~GpioHandler();
		inline const char *getGpioKey() const {return mGpioKey;}
		void write(int status);
		bool read();
		void setPinMode(int status);
	private:
		char *mGpioKey;

		GPIO::GPIOManager* mGpioManager;
		int mGpioPin;
};

#endif