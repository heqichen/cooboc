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

union ByteFloat
{
	float value;
	uint8_t byte[4];
};


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
	int type = (int)buffer[0];
	ByteFloat byteFloat;
	if (type == 1)
	{
		double halfArray[16*2];
		for (i=0; i<32; ++i)
		{
			byteFloat.byte[0] = buffer[i*4+1];
			byteFloat.byte[1] = buffer[i*4+2];
			byteFloat.byte[2] = buffer[i*4+3];
			byteFloat.byte[3] = buffer[i*4+4];
			halfArray[i] = byteFloat.value;

			cout<<halfArray[i]<<" ";
		}
	}

	cout<<endl;
}




