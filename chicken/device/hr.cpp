#include "hr.h"
#include "device_types.h"


Hr::Hr(Io *io)
	:	IDevice		(io, DEVICE_TYPE_HR),
		mHrDriver	(NULL)
{

}

Hr::~Hr()
{
	if (mHrDriver != NULL)
	{
		delete mHrDriver;
	}
}

void Hr::init()
{
	mHrDriver = new HrDriver(mIo);
}


void Hr::test()
{
	mHrDriver->test();
}
