#include "serial_radio_driver.h"

SerialRadioDriver::SerialRadioDriver(Io *io)
	:	mSerialHandler	(NULL)
{
	mSerialHandler = io->getSerialHandler("/dev/ttyO1");
}

void SerialRadioDriver::send(const uint8_t *buf, int len)
{
	mSerialHandler->send(buf, len);
}

int SerialRadioDriver::recv(uint8_t *buf, int nMaxRead)
{
	return mSerialHandler->recv(buf, nMaxRead);
}