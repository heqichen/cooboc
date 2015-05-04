#ifndef __WS_HANDLER_H__
#define __WS_HANDLER_H__

#include "../../../utils/libwebsockets/lws_config.h"
#include "../../../utils/libwebsockets/libwebsockets.h"

#include <pthread.h>

#define WS_STATUS_DISCONNECT	0x00
#define WS_STATUS_CONNECTED		0x01
#define WS_STATUS_CONNECTING	0X02


class WsHandler
{
	public:
		WsHandler(char *serverAddress, int port);
		~WsHandler();

		void connectServer();
		void touchServer();

		inline bool isGood() const{return mIsGood;}
		inline bool isRunning() const {return mIsRunning;}
		inline int getStatus() const {return mWsStatus;}
		void setWsStatus(int status);
		bool sendText(char *text);
	private:
		bool mIsGood;
		bool mIsRunning;
		int mWsStatus;
		char *mServerAddress;
		int mPort;
		pthread_t mWsDaemon;
		libwebsocket_context *mWsContext;
		libwebsocket *mWsi;
		lws_context_creation_info mWsInfo;

};

#endif

