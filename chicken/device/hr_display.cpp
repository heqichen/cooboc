#include "hr_display.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>

#ifndef NULL
#define NULL 0
#endif

HrDisplay::HrDisplay(Io *io, const char *serverAddress, const int port)
	:	IDevice		(io, DEVICE_TYPE_HR_DISPLAY),
		mWebsocketDriver	(NULL),
		mServerAddress		(NULL),
		mPort				(0)
{
	setServer(serverAddress, port);
}


HrDisplay::~HrDisplay()
{
	if (mWebsocketDriver != NULL)
	{
		delete mWebsocketDriver;
	}
	if (mServerAddress != NULL)
	{
		delete [] mServerAddress;
	}
}

void HrDisplay::setServer(const char *serverAddress, int port)
{
	int len = strlen(serverAddress);
	mServerAddress = new char[len+1];
	strncpy(mServerAddress, serverAddress, len+1);
	mPort = port;
}

void HrDisplay::init()
{
	if (mServerAddress != NULL)
	{
		mWebsocketDriver = new WebsocketDriver(mIo, mServerAddress, mPort);
	}
}




void HrDisplay::sendDistance(const int *value)
{
	sprintf(mTextBuffer, "{dist1:%d, dist2:%d}", value[0], value[1]);
	mWebsocketDriver->sendText(mTextBuffer);
}