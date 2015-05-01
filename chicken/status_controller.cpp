#include "status_controller.h"

#include <iostream>
using namespace std;

const char *FLIGHT_STATUS_NAME_STR[] = {
	"FLIGHT_STATUS_DEFAULT",
	"FLIGHT_STATUS_MANULLY",
	"FLIGHT_STATUS_ATTITUDE"
};

StatusController::StatusController(ServoController *servoController)
	:	mServoController	(servoController),
		mFlightStatus		(FLIGHT_STATUS_DEFAULT)
{
	int i;
	for (i=0; i<MAX_NUMBER_NAVIGATORS; ++i)
	{
		mNavigators[i] = NULL;
	}
}

void StatusController::work()
{
	int targetStatus = decideFlightStatus();
	if (targetStatus != mFlightStatus)	
	{
		onFlightStatusChange(targetStatus);
	}
}


int StatusController::decideFlightStatus()
{
	int status = FLIGHT_STATUS_DEFAULT;
	ServoSignal signal = mServoController->getRawServoSignal();
	if (signal.status > 1500)
	{
		status = FLIGHT_STATUS_ATTITUDE;
	}
	else
	{
		status = FLIGHT_STATUS_MANULLY;
	}

	return status;
}


void StatusController::onFlightStatusChange(int status)
{
	if (mNavigators[mFlightStatus] != NULL)
	{
		mNavigators[mFlightStatus]->setEnabled(false);
	}
	

	mFlightStatus = status;
	cout<<"change status to " << FLIGHT_STATUS_NAME_STR[mFlightStatus] << endl;;
	
	if (mNavigators[mFlightStatus] != NULL)
	{
		mNavigators[mFlightStatus]->setEnabled(true);
	}
}


void StatusController::setNavigator(int naviId, INavigator *navigator)
{
	mNavigators[naviId] = navigator;
}