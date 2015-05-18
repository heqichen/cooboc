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
		WsHandler(const char *serverAddress, int port);
		~WsHandler();

		void connectServer();
		void touchServer();

		inline bool isGood() const{return mIsGood;}
		inline bool isRunning() const {return mIsRunning;}
		inline int getStatus() const {return mWsStatus;}
		inline bool hasNewMessage() const {return mHasNewMessage;}
		void setWsStatus(int status);
		bool sendText(const char *text);
		void doSendText();
		void onMessageReceived(const char *data, int len);
		const char * getReceivedMessage();
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
		//TODO: check length before sending
		unsigned char mWsBuffer[LWS_SEND_BUFFER_PRE_PADDING + 4096 +LWS_SEND_BUFFER_POST_PADDING];
		int mSendLength;
		char mReceivedMessage[1024];
		bool mHasNewMessage;
};

#endif

