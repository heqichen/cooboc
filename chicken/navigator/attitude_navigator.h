#ifndef __ATTITUDE_NAVIGATOR_H__
#define __ATTITUDE_NAVIGATOR_H__

#include "i_navigator.h"
#include <attitude_controller.h>
#include <device/servo_controller.h>
#include <instruments/adi.h>

class AttitudeNavigator	:	public INavigator
{
	public:
		AttitudeNavigator(ServoController *servoController, AttitudeController *attitudeController, ADI *adi);
		virtual void navigate();
		virtual void onTakeover();
		virtual void onRelease();
	private:
		ServoController *mServoController;
		AttitudeController *mAttitudeController;
		ADI *mAdi;
		ServoSignal mRcSignal;
		double mPitchTarget;
		double mRollTarget;
		double mYawTarget;
		
		void updateTarget();
		double calculateTargetDiff(int servoValue);
};

#endif

