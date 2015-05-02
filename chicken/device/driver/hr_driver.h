#ifndef __HR_DRIVER_H__
#define __HR_DRIVER_H__

#include "io/io.h"
#include "io/serial_handler.h"


#ifndef NULL
#define NULL 0
#endif

class HrDriver
{
	public:
		HrDriver(Io *io);
		~HrDriver();

		inline bool isReadingThreadRunning() const {return mIsReadThreadRunning;}

		void readData();
	private:
		SerialHandler *mSerialHandler;
		bool mIsReadThreadRunning;
		pthread_t mReadThread;
		uint8_t mReadBuffer[1024];

		
};

#endif

