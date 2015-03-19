#include "serial_handler.h"

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <iostream>
#include <stdint.h>
using namespace std;


SerialHandler::SerialHandler(const char *portName)
	:	mTtyFile	(0),
		mIsBlockRead(false)
{
	int portNameLen = strlen(portName);
	mPortName = new char[portNameLen+1];
	strncpy(mPortName, portName, portNameLen);
	mTtyFile = open(mPortName, O_RDWR | O_NOCTTY | O_NDELAY);

	fcntl(mTtyFile, F_SETFL, FNDELAY);

	termios options;
	tcgetattr(mTtyFile, &options);

	options.c_cflag |= (CLOCAL | CREAD);

	//8N1
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	//disable hardware control
	options.c_cflag &= ~CRTSCTS;

	//turn off canonical input, choosing raw input
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);


	//disable software flow control
	options.c_iflag &= ~(IXON | IXOFF | IXANY);

	//raw output
	options.c_oflag &= ~OPOST;

	tcsetattr(mTtyFile, TCSANOW, &options);

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


void SerialHandler::setBlockRead(bool isBlockRead)
{
	mIsBlockRead = isBlockRead;
	if (isBlockRead)
	{
		fcntl(mTtyFile, F_SETFL, 0);
	}
	else 
	{
		fcntl(mTtyFile, F_SETFL, FNDELAY);
	}
}

void SerialHandler::setBaudrate(int baudrate)
{
	termios options;
	tcgetattr(mTtyFile, &options);
	speed_t speed = B9600;
	switch (baudrate)
	{
		case(0):{speed = B0; break;}
		case(50):{speed = B50; break;}
		case(75):{speed = B75; break;}
		case(110):{speed = B110; break;}
		case(134):{speed = B134; break;}
		case(150):{speed = B150; break;}
		case(200):{speed = B200; break;}
		case(300):{speed = B300; break;}
		case(600):{speed = B600; break;}
		case(1200):{speed = B1200; break;}
		case(1800):{speed = B1800; break;}
		case(2400):{speed = B2400; break;}
		case(4800):{speed = B4800; break;}
		case(9600):{speed = B9600; break;}
		case(19200):{speed = B19200; break;}
		case(38400):{speed = B38400; break;}
		case(57600):{speed = B57600; break;}
		case(115200):{speed = B115200; break;}
		default:
		{
			speed = B9600;
			break;
		}

	}

	cfsetispeed(&options, speed);
	cfsetospeed(&options, speed);

	tcsetattr(mTtyFile, TCSANOW, &options);
}