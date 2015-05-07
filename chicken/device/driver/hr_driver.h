#ifndef __HR_DRIVER_H__
#define __HR_DRIVER_H__

#include "io/io.h"
#include "io/serial_handler.h"

#include <utils/sa_modem.h>


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

		const int *getDistData();
		bool hasNewData() const {return mHasNewData;}

	private:
		SerialHandler *mSerialHandler;
		bool mIsReadThreadRunning;
		pthread_t mReadThread;
		uint8_t mReadBuffer[1024];
		uint8_t mReadDataBuffer[1024];
		int mDist[2];
		bool mHasNewData;
		SaModem mSaModem;
		
};

#endif

