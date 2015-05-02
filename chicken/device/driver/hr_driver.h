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

		void test();
	private:
		SerialHandler *mSerialHandler;

		
};

#endif

