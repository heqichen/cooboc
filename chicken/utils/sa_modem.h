#ifndef __SA_MODEM_H__
#define __SA_MODEM_H__

#include <stdint.h>

#define SA_LEADING_BYTE		'S'
#define SA_CHECK_BYTE		'A'

#define SA_PACKET_TIMEOUT	500UL

class SaModem
{
	public:
		SaModem();
		~SaModem();
		bool demodulateByte(uint8_t byte);
		int getLastPayload(uint8_t *buffer);
	private:
		bool mIsPacketLocked;
		int mPacketIndex;
		int mPayloadIndex;
		int mPayloadSize;
		unsigned long lastDecodeTime;
		uint8_t mPayloadBuffer[1024];

		void resetStatus();

};



#endif

