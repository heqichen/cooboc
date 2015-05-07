#ifndef __HR_H__
#define __HR_H__

//Head Rest

#include "i_device.h"
#include "driver/hr_driver.h"

#ifndef NULL
#define NULL 0
#endif


class Hr	:	public IDevice
{
	public:
		Hr(Io *io);
		~Hr();
		virtual void init();

		bool hasNewData();
		const int *getDistData();
		void test();
	private:
		HrDriver *mHrDriver;
};

#endif