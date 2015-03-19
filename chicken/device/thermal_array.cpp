#include "thermal_array.h"

#include "i_device.h"
#include "driver/mlx90620_driver.h"
#include "device_types.h"


#include <iostream>
#include <unistd.h>
using namespace std;



ThermalArray::ThermalArray(Io *io)
	:	IDevice	(io, DEVICE_TYPE_THERMAL_ARRAY),
		mMlx90620	(NULL)
{
	
}

ThermalArray::~ThermalArray()
{
	if (mMlx90620 != NULL)
	{
		delete mMlx90620;
	}
}


void ThermalArray::init()
{
	mMlx90620 = new Mlx90620Driver(mIo, "/dev/ttyUSB0");
}


void ThermalArray::test()
{



}

