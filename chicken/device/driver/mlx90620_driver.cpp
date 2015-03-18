#include "mlx90620_driver.h"

#include <iostream>
using namespace std;

Mlx90620Driver::Mlx90620Driver(Io *io, uint8_t address)
	:	mIicHandler	(NULL),
		mAddress	(address)
{
	mIicHandler = io->getIicHandler("/dev/i2c-1");
	uint8_t writingBuffer[1024];
	uint8_t buffer[256];
	uint8_t readingBuffer[1024];
	
	bool iicOk = mIicHandler->readnU8(0x50, 0x00, buffer, 256);

	if (iicOk)
	{
		cout<<"read ok"<<endl;
		int x, y;
		for (y=0; y<16; ++y)
		{
			for (x=0; x<16; ++x)
			{
				cout<<hex<<(unsigned int)buffer[y*16 + x]<<" ";
			}
			cout<<endl;
		}

		
		writingBuffer[0] = 0x04;	//command
		writingBuffer[1] = buffer[0xF7] - 0xAA;
		writingBuffer[2] = buffer[0xF7];
		writingBuffer[3] = 0x56;	//0x00 - 0xAA
		writingBuffer[4] = 0x00;
		cout<<"writingBuffer is: "<<endl;
		for (x=0; x<5; ++x)
		{
			cout<<hex<<(unsigned int)writingBuffer[x]<<" ";
		}
		cout<<endl;

		iicOk = mIicHandler->writeBytes(0x60, writingBuffer, 5);

		cout<<"write oscillator trimming : " <<iicOk<<endl;

		writingBuffer[0] = 0x03;
		writingBuffer[1] = buffer[0xF5] - 0x55;
		writingBuffer[2] = buffer[0xF5];
		uint8_t bufferF6 = buffer[0xF6];
		bufferF6 |= 0x04;
		writingBuffer[3] = bufferF6 - 0x55;
		writingBuffer[4] = bufferF6;

		iicOk = mIicHandler->writeBytes(0x60, writingBuffer, 5);
		cout<<"write configuration value : " <<iicOk<<endl;

		//reading configuration register
		
		writingBuffer[0] = 0x02;
		writingBuffer[1] = 0x93;
		writingBuffer[2] = 0x00;
		writingBuffer[3] = 0x01;

		/*
		iicOk = mIicHandler->writeBytes(0x60, writingBuffer, 4);
		cout<<"write reading configuration register: " << iicOk <<endl;

		iicOk = mIicHandler->receiveBytes(0x60, readingBuffer, 2);
		cout<<"reading from 0x60 ok? " << iicOk <<endl;

		cout<<hex<<(unsigned int)readingBuffer[0]<<" "<<hex<<(unsigned int)readingBuffer[1]<<endl;
	*/
		bool iicOk = mIicHandler->writeRead(0x60, writingBuffer, 4, readingBuffer, 2);
		cout<<"readign configuration ok? " << iicOk<<endl;


	}
	else
	{
		cout<<"iic read failed";
	}
}