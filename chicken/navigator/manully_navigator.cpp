#include "manully_navigator.h"

#include <iostream>
using namespace std;

ManullyNavigator::ManullyNavigator(ServoController *servoController)
	:	mServoController	(servoController)
{

}

void ManullyNavigator::navigate()
{
	ServoSignal servo = mServoController->getRawServoSignal();
	mServoController->writeServoSignal(servo);
}

void ManullyNavigator::onTakeover()
{

}

void ManullyNavigator::onRelease()
{
	
}