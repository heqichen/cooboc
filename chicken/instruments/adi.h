#ifndef __ADI_H__
#define __ADI_H__

#include <device/imu.h>
#include <device/virtual_imu.h>

#ifndef NULL 
#define NULL 0
#endif

class ADI
{
	public:
		ADI(Imu *imu);
		~ADI();
		inline void setVirtualImu(VirtualImu *virtualImu) {mVirtualImu = virtualImu;}
		ImuAttitude getAttitude();
	private:
		Imu *mImu;
		VirtualImu *mVirtualImu;


};

#endif
