#ifndef __MLX90620_DRIVER_H__
#define __MLX90620_DRIVER_H__

#include "io/io.h"

#include <utils/sa_modem.h>

#include <pthread.h>

#ifndef NULL
#define NULL 0
#endif

class Mlx90620Driver
{

	public:
		Mlx90620Driver(Io *io, char *serialPath);
		~Mlx90620Driver();
		void updateData(void);
		inline bool isRunning() const {return mIsRunning;}

	private:
		SerialHandler *mSerialHandler;
		bool mIsRunning;
		pthread_t mUpdatePthread;
		SaModem mModem;


		void decodePayload(const uint8_t *buffer, int length);
};


#endif


