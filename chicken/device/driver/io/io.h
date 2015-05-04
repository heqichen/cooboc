#ifndef __IO_H__
#define __IO_H__

#include "serial_handler.h"
#include "iic_handler.h"
#include "gpio_handler.h"
#include "websocket_handler.h"


#ifndef NULL
#define NULL 0
#endif


#define MAX_PORT_NUM	256

class Io
{
	public:
		Io();
		SerialHandler *getSerialHandler(const char *portName);
		IicHandler *getIicHandler(const char *portName);
		GpioHandler *getGpioHandler(const char *gpioKey);
		WsHandler *getWebsocketHandler(const char *serverAddress, const int port);
	private:
		SerialHandler *mOpenedSerialHandler[MAX_PORT_NUM];
		int mNumOpenedSerialHandler;
		IicHandler *mOpenedIicHandler[MAX_PORT_NUM];
		int mNumOpenedIicHandler;
		GpioHandler *mOpenedGpioHandler[MAX_PORT_NUM];
		int mNumOpenedGpioHandler;
		WsHandler *mOpenedWebsocketHandler[MAX_PORT_NUM];
		int mNumOpenedWebsocketHandler;

};

#endif