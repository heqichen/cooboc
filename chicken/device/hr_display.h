#ifndef __HR_DISPLAY_H__
#define __HR_DISPLAY_H__

#include "i_device.h"
#include "driver/websocket_driver.h"
#include "./device/device_types.h"

class HrDisplay	:	public IDevice
{
	public:
		HrDisplay(Io *io, const char *serverAddress=NULL, const int port=0);
		~HrDisplay();
		virtual void init();

		void setServer(const char *serverAddress, const int port);

		void sendDistance(const int *value);
	private:
		WebsocketDriver *mWebsocketDriver;
		char *mServerAddress;
		int mPort;
		char mTextBuffer[1024];
};

#endif
