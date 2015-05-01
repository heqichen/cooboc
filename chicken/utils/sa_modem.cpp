#include "sa_modem.h"

#include "util.h"

#include <string.h>
#include <iostream>
using namespace std;

SaModem::SaModem()
	:	mIsPacketLocked	(false),
		mPacketIndex	(0),
		mPayloadIndex	(0),
		mPayloadSize	(0),
		lastDecodeTime	(0UL)
{
//cout<<"mIsPacketLocked: "<<mIsPacketLocked<<endl;
}

SaModem::~SaModem()
{

}

bool SaModem::demodulateByte(uint8_t byte)
{
/*
	cout<<"demodulateByte: "<<hex<<(int)byte<<dec;
	cout<<"\tmPacketIndex: "<<mPacketIndex;
	cout<<"\tmIsPacketLocked: "<<mIsPacketLocked;
	cout<<"\tmPayloadIndex: "<<mPayloadIndex;
	cout<<endl;
*/
	unsigned long lastTime = lastDecodeTime;
	lastDecodeTime = millis();
	if (mIsPacketLocked)
	{
		/*
			5 part
			1. S
			2. A
			3. payloadSize
			4. payload
			5 crc
			6. \n
		*/

		if (millis() - lastTime > SA_PACKET_TIMEOUT)
		{
			//cout<<"timeout, locked failed"<<endl;
			resetStatus();
			return false;
		}



		switch (mPacketIndex)
		{
			case (0):
			{
				//Impossible
				resetStatus();
				return false;
			}
			case (1):
			{
				if (byte == SA_CHECK_BYTE)
				{
					//cout<<"check byte found, move packet id to 2"<<endl;
					mPacketIndex = 2;
					return false;
				}
			}
			case (2):
			{
				mPayloadSize = (int) byte;
				mPayloadIndex = 0;
				mPacketIndex = 3;
				if (0 == mPayloadSize)
				{
					mPacketIndex = 4;
				}
				//cout<<"read payload length is " << mPayloadSize<<endl;
				return false;
			}
			case (3):
			{
				//read payload
				mPayloadBuffer[mPayloadIndex] = byte;
				mPayloadIndex++;
				if (mPayloadIndex == mPayloadSize)
				{
					//cout<<"read payload finish, move to packetIndex: 4"<<endl;
					mPacketIndex++;
				}
				return false;
			}
			case (4):
			{
				//read crc;
				uint8_t crcInPacket = byte;
				uint8_t crcCalculated = 0x00;
				int i;
				for (i=0; i<mPayloadSize; ++i)
				{
					crcCalculated ^= mPayloadBuffer[i];
				}
				if (crcInPacket == crcCalculated)
				{
					mIsPacketLocked = false;
					return true;
				}
				else
				{
					//cout<<"crc check failed"<<endl;
					//cout<<"expected: "<<hex<<(int)crcCalculated<<"  actually: "<<(int)crcInPacket<<dec<<endl;
					resetStatus();
					return false;
				}
			}
			default:
			{
				resetStatus();
				return false;
			}
		}
	}
	else
	{
		if (byte == SA_LEADING_BYTE)
		{
			mPacketIndex = 1;
			mPayloadIndex = 0;
			mIsPacketLocked = true;
			return false;
		}
		else
		{
			resetStatus();
			return false;
		}
	}
	return false;
}

void SaModem::resetStatus()
{
	mPacketIndex = 0;
	mPayloadIndex = 0;
	mIsPacketLocked = false;
	mPayloadSize = 0;
}

int SaModem::getLastPayload(uint8_t *buffer)
{
	memcpy(buffer, mPayloadBuffer, mPayloadSize);
	return mPayloadSize;
}