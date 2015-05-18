#include "websocket_handler.h"

#include <unistd.h>
#include <cstring>
#include <iostream>
using namespace std;


void registerHandlerMap(libwebsocket_context* ctx, WsHandler* wsHandler);
void removeHandler(libwebsocket_context* ctx);
WsHandler *lookupHandler(libwebsocket_context* ctx);
void listHandlerMap();

#define WEBSOCKET_RECONNECT_INTERNAL	1000000UL



static int plainProtocolCallback(libwebsocket_context *ctx, libwebsocket *wsi, libwebsocket_callback_reasons reason, void *user, void *in, size_t len)
{
	WsHandler* wsHandler = lookupHandler(ctx);
	switch (reason)
	{
		case (LWS_CALLBACK_CLIENT_ESTABLISHED):
		{
			if (wsHandler!=NULL)
			{
				wsHandler->setWsStatus(WS_STATUS_CONNECTED);
			}
			break;
		}
		case (LWS_CALLBACK_CLOSED):
		{
			if (wsHandler != NULL)
			{
				wsHandler->setWsStatus(WS_STATUS_DISCONNECT);
			}
			break;
		}
		case (LWS_CALLBACK_CLIENT_WRITEABLE):
		{
			if (wsHandler != NULL)
			{
				wsHandler->doSendText();
			}
			break;
		}
		case (LWS_CALLBACK_CLIENT_RECEIVE):
		{
			if (wsHandler != NULL)
			{
				wsHandler->onMessageReceived((const char *)in, len);
			}
			break;
		}

		case (LWS_CALLBACK_ESTABLISHED):
		case (LWS_CALLBACK_CLIENT_CONNECTION_ERROR):
		case (LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH):
		case (LWS_CALLBACK_CLOSED_HTTP):
		case (LWS_CALLBACK_RECEIVE):

		case (LWS_CALLBACK_CLIENT_RECEIVE_PONG):
		case (LWS_CALLBACK_SERVER_WRITEABLE):
		case (LWS_CALLBACK_HTTP):
		case (LWS_CALLBACK_HTTP_BODY):
		case (LWS_CALLBACK_HTTP_BODY_COMPLETION):
		case (LWS_CALLBACK_HTTP_FILE_COMPLETION):
		case (LWS_CALLBACK_HTTP_WRITEABLE):
		case (LWS_CALLBACK_FILTER_NETWORK_CONNECTION):
		case (LWS_CALLBACK_FILTER_HTTP_CONNECTION):
		case (LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED):
		case (LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION):
		case (LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS):
		case (LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS):
		case (LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION):
		case (LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER):
		case (LWS_CALLBACK_CONFIRM_EXTENSION_OKAY):
		case (LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED):
		case (LWS_CALLBACK_PROTOCOL_INIT):
		case (LWS_CALLBACK_PROTOCOL_DESTROY):
		case (LWS_CALLBACK_WSI_CREATE):
		case (LWS_CALLBACK_WSI_DESTROY):
		case (LWS_CALLBACK_GET_THREAD_ID):
		case (LWS_CALLBACK_ADD_POLL_FD):
		case (LWS_CALLBACK_DEL_POLL_FD):
		case (LWS_CALLBACK_CHANGE_MODE_POLL_FD):
		case (LWS_CALLBACK_LOCK_POLL):
		case (LWS_CALLBACK_UNLOCK_POLL):
		case (LWS_CALLBACK_OPENSSL_CONTEXT_REQUIRES_PRIVATE_KEY):
		case (LWS_CALLBACK_USER):
		default:
		{
			//cout<<"reason: "<<reason<<endl;
			break;
		}
	}

	return 0;
}

libwebsocket_protocols lwsProtocolList[] = 
{
	{
		"plain-protocol",
		plainProtocolCallback,
		0,
		4096
	},
	{NULL, NULL, 0, 0}
};

void *startWebsocketDaemon(void *wsHandlerPtr)
{
	WsHandler *wsHandler = (WsHandler*)wsHandlerPtr;
	cout<<"stand for "<<wsHandlerPtr<<endl;
	while (wsHandler->isRunning())
	{
		int status = wsHandler->getStatus();
		//cout<<"routine statis:" << status<<endl;
		switch (status)
		{
			case (WS_STATUS_DISCONNECT):
			{
				wsHandler->connectServer();
				break;
			}
			case (WS_STATUS_CONNECTING):
			case (WS_STATUS_CONNECTED):
			{
				wsHandler->touchServer();
				break;
			}
			default:
			{
				break;
			}
		}

		usleep(WEBSOCKET_RECONNECT_INTERNAL);
	}
}



WsHandler::WsHandler(const char *serverAddress, int port)
	:	mIsGood	(false),
		mWsStatus	(WS_STATUS_DISCONNECT),
		mServerAddress	(NULL),
		mPort	(0),
		mSendLength	(0),
		mHasNewMessage	(false)
{
	int serverAddressLen = strlen(serverAddress);
	mServerAddress = new char[serverAddressLen+1];
	strncpy(mServerAddress, serverAddress, serverAddressLen+1);
	mPort = port;

	cout<<"ready to connect: "<<mServerAddress<<":"<<mPort<<endl;

	memset(&mWsInfo, 0, sizeof mWsInfo);
	lws_set_log_level(7, NULL);
	mWsInfo.port = CONTEXT_PORT_NO_LISTEN;	//client not listen any port
	mWsInfo.protocols = lwsProtocolList;
	mWsInfo.gid = -1;
	mWsInfo.uid = -1;

	mIsRunning = true;
	pthread_create(&mWsDaemon, NULL, &startWebsocketDaemon, this);
}

WsHandler::~WsHandler()
{
	mIsRunning = false;
	pthread_join(mWsDaemon, NULL);
	if (mServerAddress != NULL)
	{
		delete [] mServerAddress;
	}
}

void WsHandler::connectServer()
{
	cout<<"connecting server..."<<endl;
	mWsContext = libwebsocket_create_context(&mWsInfo);
	registerHandlerMap(mWsContext, this);

	mWsi = libwebsocket_client_connect(mWsContext, mServerAddress, mPort, false, "/", mServerAddress, mServerAddress, lwsProtocolList[0].name, -1);
	listHandlerMap();
	if (mWsi != NULL)
	{
		mWsStatus = WS_STATUS_CONNECTING;
		int lwsStatus = libwebsocket_service(mWsContext, 10);
		if (lwsStatus < 0)
		{
			mWsStatus = WS_STATUS_DISCONNECT;
			removeHandler(mWsContext);
		}
	}
}

void WsHandler::touchServer()
{
	int lwsStatus = libwebsocket_service(mWsContext, 10);
	if (lwsStatus < 0)
	{
		mWsStatus = WS_STATUS_DISCONNECT;
		mIsGood = false;
		removeHandler(mWsContext);
	}
}

void WsHandler::setWsStatus(int status)
{
	mWsStatus = status;
	switch (mWsStatus)
	{
		case (WS_STATUS_DISCONNECT):
		{
			mIsGood = false;
			removeHandler(mWsContext);
			break;
		}
		case (WS_STATUS_CONNECTED):
		{
			cout<<"connected"<<endl;
			mIsGood = true;
		}
		default:
		{
			break;
		}
	}
}

bool WsHandler::sendText(const char *text)
{
	if (mIsGood)
	{
		mSendLength = strlen(text);
		strncpy((char *)&mWsBuffer[LWS_SEND_BUFFER_PRE_PADDING], text, mSendLength);
		libwebsocket_callback_on_writable(mWsContext, mWsi);
		touchServer();
	}
	return mIsGood;
}

void WsHandler::doSendText(void)
{
	libwebsocket_write(mWsi, &mWsBuffer[LWS_SEND_BUFFER_PRE_PADDING], mSendLength, LWS_WRITE_TEXT);
}

void WsHandler::onMessageReceived(const char *data, int len)
{
	strncpy(mReceivedMessage, data, len);
	mReceivedMessage[len] = '\0';
	mHasNewMessage = true;
}

const char * WsHandler::getReceivedMessage()
{
	char *tmp;
	tmp = mReceivedMessage;
	mHasNewMessage =false;
	return tmp;
}

#define SOCKET_POOL_SIZE	1024

libwebsocket_context* ctxList[SOCKET_POOL_SIZE];
WsHandler * handlerList[SOCKET_POOL_SIZE];
int handlerLength = 0;

void registerHandlerMap(libwebsocket_context* ctx, WsHandler* wsHandler)
{
	ctxList[handlerLength] = ctx;
	handlerList[handlerLength] = wsHandler;
	++handlerLength;

}

void removeHandler(libwebsocket_context* ctx)
{
	int i;
	for (i=0; i<handlerLength; ++i)
	{
		if (ctxList[i] == ctx)
		{
			break;
		}
	}
	for (; i<handlerLength-1; ++i)
	{
		ctxList[i] = ctxList[i+1];
		handlerList[i] = handlerList[i+1];
	}
	--handlerLength;

}

WsHandler *lookupHandler(libwebsocket_context* ctx)
{
	int i;
	for (i=0; i<handlerLength; ++i)
	{
		if (ctxList[i] == ctx)
		{
			return handlerList[i];
		}
	}
	return NULL;
}

void listHandlerMap()
{
	int i;
	cout<<"====== begin ======"<<endl;
	for (i=0; i<handlerLength; ++i)
	{
		cout<<(void *)(ctxList[i])<<"\t"<<(void *)(handlerList[i])<<endl;
	}
	cout<<"======= end  ======"<<endl;
}



