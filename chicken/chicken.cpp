#include "device_manager.h"
#include "./device/radio.h"
#include "./device/driver/io/io.h"
#include "./device/imu.h"
#include "./device/servo_controller.h"
#include "./device/aoa.h"
#include "./device/beeper.h"
#include "./device/virtual_imu.h"
#include "./instruments/adi.h"
#include "./device/thermal_array.h"
#include "status_controller.h"
#include "attitude_controller.h"

#include "navigator/attitude_navigator.h"
#include "navigator/manully_navigator.h"

#include <mavlink.h>

#include <iostream>
#include <utils/util.h>
#include <unistd.h>
#include <utils/sa_modem.h>

using namespace std;

Io *io;
DeviceManager *devManager;
Radio *radio;
Imu *imu;
ServoController *servoController;
Aoa *aoa;
Beeper *beeper;
Agl *agl;
ThermalArray *thermalArray;


VirtualImu *virtualImu;

ADI *adi;


StatusController *statusController;
AttitudeController *attitudeController;

ManullyNavigator *manullyNavigator;
AttitudeNavigator *attitudeNavigator;


void setupDevice(void);

int main(int argc, char *argv[])
{



/*
uint8_t testbyte[] = {0x53, 0x41, 0x81, 0x1, 0x50, 0x49, 0xb4, 0x41, 0xd0, 0x73, 0xb4, 0x41, 0x40, 0x54, 0xa9, 0x41, 0x80, 0x51, 0x9c, 0x41, 0xd0, 0xdd, 0xaa, 0x41, 0x30, 0x37, 0xae, 0x41, 0xd0, 0x29, 0x9f, 0x41, 0x0, 0xb1, 0x9f, 0x41, 0x40, 0x9b, 0xa8, 0x41, 0xe0, 0xf0, 0xa2, 0x41, 0x20, 0x2c, 0x9c, 0x41, 0x20, 0x7b, 0xa2, 0x41, 0x50, 0xe3, 0xa7, 0x41, 0xf0, 0xf, 0xae, 0x41, 0xb0, 0x3d, 0xa0, 0x41, 0x90, 0xcd, 0xa5, 0x41, 0x30, 0xfe, 0xb7, 0x41, 0x40, 0xbe, 0xb0, 0x41, 0xc0, 0x65, 0xa7, 0x41, 0x70, 0x3c, 0xa8, 0x41, 0x70, 0x8d, 0xa0, 0x41, 0xc0, 0x14, 0xa4, 0x41, 0xf0, 0xda, 0xa1, 0x41, 0x90, 0xb5, 0xa4, 0x41, 0x10, 0x63, 0xa7, 0x41, 0xb0, 0x99, 0xa4, 0x41, 0x90, 0xb5, 0xa4, 0x41, 0x90, 0x9, 0xaf, 0x41, 0x30, 0x99, 0xa6, 0x41, 0x80, 0x34, 0xac, 0x41, 0xf0, 0x96, 0xa2, 0x41, 0x90, 0xb4, 0xa7, 0x41, 0x7d, 0xa};

SaModem m;
int i;
for (i=0; i<sizeof(testbyte); ++i)
{
	m.demodulateByte(testbyte[i]);
}

return 0;
*/





	setupDevice();

/*
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
*/
	thermalArray->test();

	int a;
	cin>>a;

	return 0;
}

void setupDevice(void)
{

	io = new Io();
	devManager = new DeviceManager(io);
	
	/*
	radio = devManager->getRadio();
	imu = devManager->getImu();
	servoController = devManager->getServoController();
	servoController->setOutputMode(SERVO_OUTPUT_MODE_MANULLY);
	aoa = devManager->getAoa();
	beeper = devManager->getBeeper();
	agl = devManager->getAgl();
	*/
	thermalArray = devManager->getThermalArray();

/*
	virtualImu = devManager->getVirtualImu();


	radio->setVirtualImu(virtualImu);
*/

}
