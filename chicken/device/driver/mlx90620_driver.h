#ifndef __MLX90620_DRIVER_H__
#define __MLX90620_DRIVER_H__




#include "io/io.h"


#ifndef NULL
#define NULL 0
#endif

class Mlx90620Driver
{

	public:
		Mlx90620Driver(Io *io, char *serialPath);
	private:
		SerialHandler *mSerialHandler;
		char *serialPath;
};


#endif


