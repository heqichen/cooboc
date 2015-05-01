#ifndef __SERIAL_RADIO_DRIVER_H__
#define __SERIAL_RADIO_DRIVER_H__

#include "io/io.h"
#include "io/serial_handler.h"

#ifndef NULL
#define NULL 0
#endif

class SerialRadioDriver
{
	public:
		SerialRadioDriver(Io *io);
		void send(const uint8_t *buf, int len);
		int recv(uint8_t *buf, int nMaxRead);
	private:
		SerialHandler *mSerialHandler;

};

#endif