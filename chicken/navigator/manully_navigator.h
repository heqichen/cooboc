#ifndef __MANULLY_NAVIGATOR_H__
#define __MANULLY_NAVIGATOR_H__

#include "i_navigator.h"
#include <device/servo_controller.h>

class ManullyNavigator	:	public INavigator
{
	public:
		ManullyNavigator(ServoController *servoController);
		virtual void navigate();
		virtual void onTakeover();
		virtual void onRelease();
	private:
		ServoController *mServoController;
};

#endif

