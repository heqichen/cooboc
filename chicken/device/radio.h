#ifndef __RADIO_H__
#define __RADIO_H__

#include "i_device.h"
#include "driver/serial_radio_driver.h"
#include "driver/io/io.h"
#include "virtual_imu.h"


#include <pthread.h>
#include <mavlink.h>

#define RADIO_READ_BUFFER_LENGTH	1024

#ifndef NULL
#define NULL 0
#endif

class Radio : public IDevice
{
	public:
		Radio(Io *io);
		~Radio();


		void print(const char *str);
		void write(const uint8_t *buf, int len);
		void fetchData();
		virtual void init();

		inline void setVirtualImu(VirtualImu *virtualImu) {mVirtualImu=virtualImu;}
		inline bool isRadioRunning(void){return mIsRadioRunning;}
	private:
		bool mIsRadioRunning;
		SerialRadioDriver *mSerialRadioDriver;
		VirtualImu *mVirtualImu;
		pthread_t mFetchDataPthread;
		uint8_t mReadBuffer[RADIO_READ_BUFFER_LENGTH];
		uint8_t mMavlinkComChannel;
		mavlink_message_t mMavlinkDecodeMsg; 
		mavlink_status_t mMavlinkDecodeStatus;

};

#endif