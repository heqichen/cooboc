#include "beeper_driver.h"
#include "io/gpio_handler.h"

#include <cstring>

BeeperDriver::BeeperDriver(Io *io, const char *gpioKey)
{
	int len = strlen(gpioKey);
	mGpioKey = new char [len + 1];
	strncpy(mGpioKey, gpioKey, len);

	mGpioHandler = io->getGpioHandler(mGpioKey);
	mGpioHandler->setPinMode(GPIO_OUTPUT);
}

BeeperDriver::BeeperDriver(Io *io)
{
	int len = strlen(DEFAULT_BEEPER_GPIO_KEY);
	mGpioKey = new char [len + 1];
	strncpy(mGpioKey, DEFAULT_BEEPER_GPIO_KEY, len);

	mGpioHandler = io->getGpioHandler(mGpioKey);
	mGpioHandler->setPinMode(GPIO_OUTPUT);
}

BeeperDriver::~BeeperDriver()
{
	delete [] mGpioKey;
}

void BeeperDriver::makeNoise()
{
	mGpioHandler->write(GPIO_HIGH);
}

void BeeperDriver::shutUp()
{
	mGpioHandler->write(GPIO_LOW);
}