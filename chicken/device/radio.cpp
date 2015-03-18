#include "radio.h"
#include "device_types.h"
#include "driver/io/io.h"
#include <utils/util.h>
#include <utils/mavlink_wtf.h>


#include <cstring>
#include <iostream>
using namespace std;

#define RADIO_READ_INTERVAL	20	//20millis per time

void *radioReadingThread(void *radioPtr)
{
	Radio *radio = (Radio*)radioPtr;

	while (radio->isRadioRunning())
	{
		radio->fetchData();
		usleep(1000UL);
		while (true)
		{
			if (millis() % RADIO_READ_INTERVAL == 0 )
			{
				break;
			}
			usleep(300UL);
		}
	}
}

Radio::Radio(Io *io)
	:	IDevice	(io, DEVICE_TYPE_RADIO),
		mSerialRadioDriver	(NULL),
		mVirtualImu			(NULL),
		mIsRadioRunning		(false)
{
	
}

Radio::~Radio()
{
	if (mIsRadioRunning)
	{
		mIsRadioRunning = false;
	}
	pthread_join(mFetchDataPthread, NULL);

	if (mSerialRadioDriver != NULL)
	{
		delete mSerialRadioDriver;
	}
}

void Radio::print(const char *str)
{
	mSerialRadioDriver->send((uint8_t*)str, strlen(str));
}

void Radio::write(const uint8_t *buf, int len)
{
	mSerialRadioDriver->send(buf, len);
}

void Radio::init()
{
	mSerialRadioDriver = new SerialRadioDriver(mIo);
	mIsRadioRunning = true;
	pthread_create(&mFetchDataPthread, NULL, &radioReadingThread, this);
	mMavlinkComChannel = getMavlinkComChannel();
}

void Radio::fetchData()
{

	int numByteRead;
	do
	{
		numByteRead = mSerialRadioDriver->recv(mReadBuffer, RADIO_READ_BUFFER_LENGTH);
		int i;
		for (i=0; i<numByteRead; ++i)
		{
			if(mavlink_parse_char(mMavlinkComChannel, mReadBuffer[i], &mMavlinkDecodeMsg, &mMavlinkDecodeStatus))
			{
				switch (mMavlinkDecodeMsg.msgid)
				{
					case (MAVLINK_MSG_ID_ATTITUDE):
					{
						mavlink_attitude_t attitude;
						mavlink_msg_attitude_decode(&mMavlinkDecodeMsg, &attitude);
						/*
						cout<<"time_boot_ms: " << attitude.time_boot_ms << "\t"<<endl;
						cout<<"roll: " << attitude.roll << "\t"<<endl;
						cout<<"pitch: " << attitude.pitch << "\t"<<endl;
						cout<<"yaw: " << attitude.yaw << "\t"<<endl;
						cout<<"rollspeed: " << attitude.rollspeed << "\t"<<endl;
						cout<<"pitchspeed: " << attitude.pitchspeed << "\t"<<endl;
						cout<<"yawspeed: " << attitude.yawspeed << "\t"<<endl;
						*/
						ImuAttitude imuAttitude(attitude.pitch, attitude.roll, attitude.yaw);
						if (mVirtualImu != NULL)
						{
							mVirtualImu->updateAttitude(imuAttitude);
						}
						break;
					}
					default:
					{
						break;
					}

				}
			} 
		}
	} while (numByteRead > 0);
}