#include "mlx90620_driver.h"

#include <utils/util.h>

#include <unistd.h>
#include <iostream>
using namespace std;

#define MLX_READ_INTERVAL	20UL


void *readMlxDataFromSerialThread(void *mlx90620DriverPtr)
{
	Mlx90620Driver *mlx = (Mlx90620Driver *)mlx90620DriverPtr;

	while (mlx->isRunning())
	{
		mlx->updateData();
	}

}



Mlx90620Driver::Mlx90620Driver(Io *io, char *serialPath)
	:	mSerialHandler	(NULL),
		mIsRunning		(false)
{
	mSerialHandler = io->getSerialHandler(serialPath);
	mSerialHandler->setBaudrate(57600);
	mSerialHandler->setBlockRead(true);
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
	uint8_t payloadBuffer[1024];
	int numRead = 0;
	numRead = mSerialHandler->recv(buffer, 1024);
	int i;

	for (i=0; i<numRead; ++i)
	{
		if (mModem.demodulateByte(buffer[i]))
		{
			cout<<endl<<"got packet"<<endl<<endl;
			int payloadSize = mModem.getLastPayload(payloadBuffer);
			decodePayload(payloadBuffer, payloadSize);
		}
	}
}


void Mlx90620Driver::decodePayload(const uint8_t *buffer, int length)
{
	int i;
	for (i=0; i<length; ++i)
	{
		cout<<hex<<(unsigned int)buffer[i]<<dec<<" ";
	}
	cout<<endl;
}




