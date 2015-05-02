#ifndef __HR_H__
#define __HR_H__

//Head Rest

#include "i_device.h"


class Hr	:	public IDevice
{
	public:
		Hr(Io *io);
		~Hr();
		virtual void init();
	private:

};

#endif