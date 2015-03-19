#include "mlx90620_driver.h"

#include <utils/util.h>

#include <unistd.h>
#include <iostream>
using namespace std;

#define MLX_READ_INTERVAL	20


void *readMlxDataFromSerialThread(void *mlx90620DriverPtr)
{
	Mlx90620Driver *mlx = (Mlx90620Driver *)mlx90620DriverPtr;

	while (mlx->isRunning())
	{
		mlx->updateData();
		usleep(1000UL);
		while (true)
		{
			if (millis() % MLX_READ_INTERVAL == 0)
			{
				break;
			}
			usleep(300UL);
		}
	}

}



Mlx90620Driver::Mlx90620Driver(Io *io, char *serialPath)
	:	mSerialHandler	(NULL),
		mIsRunning		(false)
{
	mSerialHandler = io->getSerialHandler(serialPath);
	mIsRunning = true;
	pthread_create(&mUpdatePthread, NULL, &readMlxDataFromSerialThread, this);


}

Mlx90620Driver::~Mlx90620Driver()
{
	if (mIsRunning)
	{
		mIsRunning = false;
	}
	pthread_join(mUpdatePthread, NULL);
}

void Mlx90620Driver::updateData(void)
{
	uint8_t buffer[1024];
	int read = mSerialHandler->recv(buffer, 1024);
	if (read <= 0)
	{

	}
	else 
	{
		cout<<read<<endl;
	}
}