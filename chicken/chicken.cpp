#include "device_manager.h"
#include "./device/radio.h"
#include "./device/driver/io/io.h"
#include "./device/imu.h"
#include "./device/servo_controller.h"
#include "./device/aoa.h"
#include "./device/beeper.h"
#include "./device/virtual_imu.h"
#include "./instruments/adi.h"
#include "status_controller.h"
#include "attitude_controller.h"

#include "navigator/attitude_navigator.h"
#include "navigator/manully_navigator.h"

#include <mavlink.h>

#include <iostream>
#include <utils/util.h>
#include <unistd.h>

using namespace std;

Io *io;
DeviceManager *devManager;
Radio *radio;
Imu *imu;
ServoController *servoController;
Aoa *aoa;
Beeper *beeper;
Agl *agl;

VirtualImu *virtualImu;

ADI *adi;


StatusController *statusController;
AttitudeController *attitudeController;

ManullyNavigator *manullyNavigator;
AttitudeNavigator *attitudeNavigator;


void setupDevice(void);

int main(int argc, char *argv[])
{
	setupDevice();

	adi = new ADI(imu);
	adi->setVirtualImu(virtualImu);



	attitudeController = new AttitudeController(adi, servoController);
	attitudeController->setInterval(50);
	attitudeController->start();

	manullyNavigator = new ManullyNavigator(servoController);
	manullyNavigator->setInterval(50);
	manullyNavigator->setEnabled(false);
	manullyNavigator->start();

	attitudeNavigator = new AttitudeNavigator(servoController, attitudeController, adi);
	attitudeNavigator->setEnabled(false);
	attitudeNavigator->setInterval(100);
	attitudeNavigator->start();


	statusController = new StatusController(servoController);
	statusController->setInterval(500);
	statusController->setNavigator(FLIGHT_STATUS_DEFAULT, attitudeNavigator);
	statusController->setNavigator(FLIGHT_STATUS_MANULLY, manullyNavigator);
	statusController->setNavigator(FLIGHT_STATUS_ATTITUDE, attitudeNavigator);
	statusController->start();

	double p, i, d;
	while (true)
	{
		cin>>p>>i>>d;
		attitudeController->setTunning(p, i, d);
	}
	return 0;
}

void setupDevice(void)
{
	io = new Io();
	devManager = new DeviceManager(io);
	
	radio = devManager->getRadio();
	imu = devManager->getImu();
	servoController = devManager->getServoController();
	servoController->setOutputMode(SERVO_OUTPUT_MODE_MANULLY);
	aoa = devManager->getAoa();
	beeper = devManager->getBeeper();
	agl = devManager->getAgl();

	virtualImu = devManager->getVirtualImu();


	radio->setVirtualImu(virtualImu);


}
