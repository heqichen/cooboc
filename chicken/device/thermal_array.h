#ifndef __THERMAL_ARRAY_H__
#define __THERMAL_ARRAY_H__

#include "i_device.h"
#include "driver/mlx90620_driver.h"

#ifndef NULL
#define NULL 0
#endif


class ThermalArray	:	public IDevice
{
	public:
		ThermalArray(Io *io);
		~ThermalArray();
		virtual void init();
		const double *const getFirArray();
	private:
		Mlx90620Driver *mMlx90620;
};


#endif


