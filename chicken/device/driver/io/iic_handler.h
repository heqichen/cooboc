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
		bool writeCommand(const uint8_t deviceAddr, const uint8_t command);
		bool writeBytes(const uint8_t deviceAddr, const uint8_t *dataBuffer, const int length);

		bool writeU8(const uint8_t deviceAddr, const uint8_t regAddr, const uint8_t value);

		bool readU8(const uint8_t deviceAddr, const uint8_t regAddr, uint8_t &value);
		bool readS8(const uint8_t deviceAddr, const uint8_t regAddr, int8_t &value);
		bool readU16BE(const uint8_t deviceAddr, const uint8_t regAddr, uint16_t &value);
		bool readS16LE(const uint8_t deviceAddr, const uint8_t regAddr, int16_t &value);
		bool readnU8(const uint8_t deviceAddr, const uint8_t regAddr, uint8_t *buffer, int readLength);
		
		bool receiveBytes(const uint8_t deviceAddr, uint8_t *buffer, int readLength);

		bool writeRead(const uint8_t deviceAddr, uint8_t *writeBuffer, int writeLength, uint8_t *readBuffer, int readLength);
	private:
		char *mPortName;
		int mI2cFile;
		uint8_t mCurrentDeviceAddr;
		pthread_mutex_t mMutex;
		uint8_t mBuffer[IIC_BUFFER_LENGTH];
		bool sendRegAddr(const uint8_t deviceAddr, const uint8_t regAddr);
};

#endif