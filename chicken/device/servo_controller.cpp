#include "servo_controller.h"

#include "device_types.h"

ServoController::ServoController(Io *io)
	:	IDevice	(io, DEVICE_TYPE_SERVO_CONTROLLER),
		mSerialServoDriver	(NULL)
{

}

ServoController::~ServoController()
{
	if (mSerialServoDriver != NULL)
	{
		delete mSerialServoDriver;
	}
}

void ServoController::init()
{
	mSerialServoDriver = new SerialServoDriver(mIo);
}

