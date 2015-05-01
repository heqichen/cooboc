#ifndef __IIC_HANDLER_H__
#define __IIC_HANDLER_H__

#include <pthread.h>
#include <stdint.h>

#define IIC_BUFFER_LENGTH	256

class IicHandler
{
	public:
		IicHandler(const char *portName);
		~IicHandler();
		const char *getPortName() const;

		bool writeByte(const uint8_t deviceAddr, const uint8_t regAddr, const uint8_t value);

		bool readU8(const uint8_t deviceAddr, const uint8_t regAddr, uint8_t &value);
		bool readS8(const uint8_t deviceAddr, const uint8_t regAddr, int8_t &value);
		bool readU16BE(const uint8_t deviceAddr, const uint8_t regAddr, uint16_t &value);
		bool readS16LE(const uint8_t deviceAddr, const uint8_t regAddr, int16_t &value);

	private:
		char *mPortName;
		int mI2cFile;
		uint8_t mCurrentDeviceAddr;
		pthread_mutex_t mMutex;
		uint8_t mBuffer[IIC_BUFFER_LENGTH];
		bool sendRegAddr(const uint8_t deviceAddr, const uint8_t regAddr);
};

#endif