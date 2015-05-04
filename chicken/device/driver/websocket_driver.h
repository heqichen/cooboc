#ifndef __WEBSOCKET_DRIVER_H__
#define __WEBSOCKET_DRIVER_H__

#include "io/io.h"
#include "io/websocket_handler.h"

class WebsocketDriver
{
	public:
		WebsocketDriver(Io *io, const char *serverAddr, const int port);
		~WebsocketDriver();
		bool sendText(const char *text);
	private:
		WsHandler *mWebsocketHandler;
};


#endif