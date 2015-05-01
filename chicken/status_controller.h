#ifndef __STATUS_CONTROLLER_H__
#define __STATUS_CONTROLLER_H__

#include "utils/i_interval_thread.h"

#include <device/servo_controller.h>
#include <navigator/manully_navigator.h>
#include <navigator/attitude_navigator.h>
#include <navigator/i_navigator.h>

#ifndef NULL 
#define NULL 0
#endif

#define FLIGHT_STATUS_DEFAULT	0x00
#define FLIGHT_STATUS_MANULLY	0x01
#define FLIGHT_STATUS_ATTITUDE	0x02


#define MAX_NUMBER_NAVIGATORS	256


class StatusController : public IIntervalThread
{
	public:
		StatusController(ServoController *servoController);
		inline int getFlightStatus(){return mFlightStatus;}
		void work();
		void setNavigator(int naviId, INavigator *navigator);
	private:
		ServoController *mServoController;
		int mFlightStatus;

		int decideFlightStatus();
		void onFlightStatusChange(int status);

		INavigator *mNavigators[MAX_NUMBER_NAVIGATORS];
		

};



#endif


