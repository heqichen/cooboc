#ifndef __VIRTUAL_IMU_H__
#define __VIRTUAL_IMU_H__

#include "i_device.h"
#include "imu.h"
#include "driver/io/io.h"

#define VIRTUAL_IMU_TIMEOUT	500UL

class VirtualImu	:	public IDevice
{
	public:
		VirtualImu(Io *io);
		~VirtualImu();
		virtual void init();
		void updateAttitude(ImuAttitude attitude);

		bool isInService();
		inline ImuAttitude getAttitude() const {return mAttitude;}
	private:
		bool mIsInService;
		unsigned long mLastValidDataTime;
		ImuAttitude mAttitude;
};

#endif

