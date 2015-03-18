#ifndef __I_DEVICE_H__
#define __I_DEVICE_H__

#include "driver/io/io.h"

#ifndef NULL
#define NULL 0
#endif

class IDevice
{
	public:
		
		IDevice(Io *io, int deviceType);
		inline int getDeviceType() const {return mDeviceType;}
		virtual void init(void) =0;
		virtual ~IDevice();
	protected:
		Io *mIo;
	private:
		IDevice();
		int mDeviceType;
};

#endif