#include "websocket_driver.h"





#ifndef NULL
#define NULL 0
#endif

WebsocketDriver::WebsocketDriver(Io *io, const char *serverAddr, const int port)
{
	mWebsocketHandler = io->getWebsocketHandler(serverAddr, port);
}

WebsocketDriver::~WebsocketDriver()
{
	
}

bool WebsocketDriver::sendText(const char *text)
{
	return mWebsocketHandler->sendText(text);
}

bool WebsocketDriver::hasNewMessage()
{
	return mWebsocketHandler->hasNewMessage();
}
const char *WebsocketDriver::getReceivedMessage()
{
	return mWebsocketHandler->getReceivedMessage();
}

