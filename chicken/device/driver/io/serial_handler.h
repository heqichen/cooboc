#ifndef __SERIAL_HANDLER_H__
#define __SERIAL_HANDLER_H__

#include <pthread.h>


#include <stdint.h>

class SerialHandler
{
	public:
		SerialHandler(const char *portName);
		~SerialHandler();

		const char *getPortName() const;

		void send(const uint8_t *buf, int len);
		int recv(uint8_t *buf, int maxLen);
	private:
		char *mPortName;
		int mTtyFile;
		pthread_mutex_t mMutex;

};

#endif