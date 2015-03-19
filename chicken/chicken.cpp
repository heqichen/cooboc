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
#include <utils/sa_modem.h>
#include <utils/util.h>

#include <iostream>
#include <unistd.h>
#include <fstream>



using namespace std;

Io *io;
DeviceManager *devManager;
ThermalArray *thermalArray;



void setupDevice(void);

int main(int argc, char *argv[])
{

	char * fileName;
	if (argc < 2)
	{
		fileName = "test.txt";
	}
	else 
	{
		fileName = argv[1];
	}

	cout<<"write to file: "<<fileName<<endl;
	
	setupDevice();

	while (true)
	{
		usleep(200000UL);
	
		const double * const firArray = thermalArray->getFirArray();
		int i, j;

		for (i=0; i<4; ++i)
		{
			for (j=0; j<16; ++j)
			{
				cout<<firArray[i*16 + j]<<"\t";
			}
			cout<<endl;
		}
		cout<<endl;

		ofstream file;
		file.open("test.txt", ofstream::out | ofstream::trunc);
		file<<"{\"firArray\":[";
		for (i=0; i<63; ++i)
		{
			file<<firArray[i]<<",";
		}
		file<<firArray[63]<<"]}"<<endl<<flush;
		file.close();
	

	}
	return 0;
}

void setupDevice(void)
{

	io = new Io();
	devManager = new DeviceManager(io);
	thermalArray = devManager->getThermalArray();
}
