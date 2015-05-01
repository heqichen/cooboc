#ifndef __SERVO_CONTROLLER_H__
#define __SERVO_CONTROLLER_H__

#include "i_device.h"
#include "driver/io/io.h"
#include "driver/serial_servo_driver.h"

#ifndef NULL
#define NULL 0
#endif

class ServoController : public IDevice
{
	public:
		ServoController(Io *io);
		~ServoController();
		virtual void init();
		inline ServoSignal getRawServoSignal(){return mSerialServoDriver->getRawServoSignal();}
		inline void writeServoSignal(const ServoSignal &servoSignal){mSerialServoDriver->setOverrideServoSignal(servoSignal);}
		inline void setOutputMode(int mode){mSerialServoDriver->setOutputMode(mode);}
	private:
		SerialServoDriver *mSerialServoDriver;
};

#endif