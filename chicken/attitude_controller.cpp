#include "attitude_controller.h"
#include <utils/util.h>

#include <iostream>
using namespace std;

int count;

#ifndef PI
#define PI 3.14159265897932384626433832795
#endif

#define RAD2DEG 180.0/3.14159265358

/*

PID @ 4x speed
pitch:	300, 5, 0
roll:	300, 1, 0

PID @ 1x speed
pitch:	1000, 10, 0
roll:	500, 10, 0

*/

AttitudeController::AttitudeController(ADI *adi, ServoController *servoController)
	:	mAdi				(adi),
		mServoController	(servoController),
		mIsEnabled			(false),
		mPitchPid			(1000.0,	10.0,	0.0,	1000),
		mRollPid			(600.0, 	5.0, 	0.0, 	500),
		mYawPid				(3000.0,	20.0,	0.0,	10),
		mTargetPitch		(0.0),
		mTargetRoll			(0.0),
		mTargetYaw			(0.0),
		mLastHeading		(0.0)

{

}

void AttitudeController::work()
{
	if (!mIsEnabled)
	{
		return ;
	}

	ServoSignal currentRc = mServoController->getRawServoSignal();
	ImuAttitude attitude = mAdi->getAttitude();
	
	//calculate roll diff
	double rollError = mTargetRoll - attitude.roll;
	double rollDiff = mRollPid.updateError(rollError);

	//calculate pitch diff
	double pitchError = mTargetPitch - attitude.pitch;
	double pitchDiff = mPitchPid.updateError(pitchError);

	//calculate yaw diff
	double currentYaw = cycleCompare(attitude.heading, mLastHeading, 2*PI);
	mLastHeading = attitude.heading;
	double yawError = cycleCompare(mTargetYaw, currentYaw, 2*PI);
	double  yawDiff = mYawPid.updateError(yawError);

	double cosA = cos(attitude.roll);
	double sinA = sin(attitude.roll);

	mServo.aileron = DEFAULT_SERVO_VALUE + rollDiff;
	mServo.aileron = constraint(mServo.aileron, 1000, 2000);

	mServo.elevator = DEFAULT_SERVO_VALUE - cosA*pitchDiff + yawDiff*sinA;
	mServo.elevator = constraint(mServo.elevator, 1000, 2000);
	
	mServo.rudder = DEFAULT_SERVO_VALUE - sinA*pitchDiff + yawDiff*cosA;
	mServo.rudder = constraint(mServo.rudder, 1000, 2000);

	mServo.throttle = currentRc.throttle;

	



	//for testing, remove soon

	
	mServoController->writeServoSignal(mServo);



	//for debug
	++count;
	if (count >= 10)
	{
		count = 0;

		cout<<"target: pitch: "<< (mTargetPitch * RAD2DEG) << "\troll: "<< (mTargetRoll * RAD2DEG ) <<"\tyaw: "<<(mTargetYaw*RAD2DEG)<<endl;
		cout<<"current: pitch: "<< (attitude.pitch * RAD2DEG) << "\troll: "<< (attitude.roll * RAD2DEG ) <<"\tyaw: "<<(currentYaw*RAD2DEG)<<endl;
		cout<<"PID diff: pitch: "<< pitchDiff << "\troll: " << rollDiff<<"\tyaw: " << yawDiff <<endl;
		cout<<"sinA: "<<sinA<<"\tcosA: "<<cosA<<endl;
		cout<<"Servo aileron: " << mServo.aileron<<"\televator: "<<mServo.elevator<<"\trudder: "<<mServo.rudder<<endl;


		cout<<endl;
	}
}


void AttitudeController::reset()
{
	mServo = mServoController->getRawServoSignal();
	mPitchPid.reset();
	mRollPid.reset();
	mYawPid.reset();

	ImuAttitude attitude = mAdi->getAttitude();
	mLastHeading = attitude.heading;
}

void AttitudeController::setTargetAttitude(double pitch, double roll, double yaw)
{
	mTargetPitch = pitch;
	mTargetRoll = roll;
	mTargetYaw = yaw;
}

void AttitudeController::setTunning(double p, double i, double d)
{
	cout<<"set tunning: P: " << p << " I: " << i<<" D: " <<d <<endl;
	//mRollPid.setTunning(p, i, d);
	mYawPid.setTunning(p, i, d);
}