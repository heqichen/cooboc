#include "gpio_handler.h"

#include <cstring>
#include <iostream>
using namespace std;

GpioHandler::GpioHandler(const char *gpioKey)
	:	mGpioKey	(NULL),
		mGpioManager	(NULL),
		mGpioPin	(-1)
{

	int len = strlen(gpioKey);
	mGpioKey = new char [len+1];
	strncpy(mGpioKey, gpioKey, len);

	mGpioManager = GPIO::GPIOManager::getInstance();
	mGpioPin = GPIO::GPIOConst::getInstance()->getGpioByKey(mGpioKey);
	mGpioManager->exportPin(mGpioPin);
}

GpioHandler::~GpioHandler()
{
	delete [] mGpioKey;
}

void GpioHandler::setPinMode(int status)
{
	if (status == GPIO_INPUT)
	{
		mGpioManager->setDirection(mGpioPin, GPIO::INPUT);
	}
	else
	{
		mGpioManager->setDirection(mGpioPin, GPIO::OUTPUT);
	}
	
}

void GpioHandler::write(int status)
{
	if (GPIO::LOW == status)
	{
		mGpioManager->setValue(mGpioPin, GPIO::LOW);
	}
	else
	{
		mGpioManager->setValue(mGpioPin, GPIO::HIGH);
	}
}

bool GpioHandler::read()
{
	int value = mGpioManager->getValue(mGpioPin);
	return value == GPIO::HIGH;
}



