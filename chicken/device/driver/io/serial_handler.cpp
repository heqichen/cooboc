#include "serial_handler.h"

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
using namespace std;


SerialHandler::SerialHandler(const char *portName)
	:	mTtyFile	(0)
{
	int portNameLen = strlen(portName);
	mPortName = new char[portNameLen+1];
	strncpy(mPortName, portName, portNameLen);
	mTtyFile = open(mPortName, O_RDWR| O_NONBLOCK | O_NDELAY);
	pthread_mutex_init(&mMutex, NULL);
}

SerialHandler::~SerialHandler()
{
	delete [] mPortName;
	if (mTtyFile > 0)
	{
		close(mTtyFile);
	}
	pthread_mutex_destroy(&mMutex);
}

void SerialHandler::send(const uint8_t *buf, int len)
{
	pthread_mutex_lock(&mMutex);
	write(mTtyFile, buf, len);
	pthread_mutex_unlock(&mMutex);
}


int SerialHandler::recv(uint8_t *buffer, int maxLen)
{
	pthread_mutex_lock(&mMutex);
	int lenRead = read(mTtyFile, buffer, maxLen);
	pthread_mutex_unlock(&mMutex);
	return lenRead;
}

const char *SerialHandler::getPortName() const
{
	return mPortName;
}