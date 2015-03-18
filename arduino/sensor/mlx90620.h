#ifndef __MLX90620_H__
#define __MLX90620_H__

#include <Arduino.h>
#include <stdint.h>

/* Pin 
	
	A4	SDA
	A5	SCL
	3v3	VCC
	GND	GND
	
*/

#define POR_CHECK_TIMEOUT	1000UL

class Mlx90620
{
	public:
		Mlx90620();
		void init();

		void printEeprom();
		double getTemperatureAmbient();
		void printAlpha();
		void updateFirArray();
		void getTo(double thermalArray[][16]);
		void printTo();

	private:
		//init seq #1
		void initReadWholeEeprom();	//7.1 P10 on Datashit
		//init seq #2
		void initWriteOscillatorTrimmingValue(); //7.1 P10 on Datashit
		//init seq #3
		void initCalibrationData();
		//init seq #4
		void initCalculateAlpha();
		

		void writeConfigurationRegister();	//7.1 P10 on Datashit
		
		


		unsigned int readPtat();
		void calculateTa();

		void updateIrData();
		int readCompensationPixel();
		void calculateTo();


		uint16_t readConfigurationRegister();
		bool isPor();

		int16_t readS16FromEepromData(uint8_t lsb);
		uint16_t readU16FromEepromData(uint8_t lsb);
		uint8_t readU8FromEepromData(uint8_t address);
		int8_t readS8FromEepromData(uint8_t address);

		unsigned long lastPorCheckTime;
		byte mEepromData[256];
		int mRefreshRate; //Set this value to your desired refresh frequency
		double mTa;//Temperature Ambient
		double mKT1;
		double mKT2;
		double mVTH;
		int mIrData[4][16]; //Contains the raw IR data from the sensor
		double mAlpha[4][16];

		int mAcp;
		int mBcp;
		int mBiScale;
		int mTgc;
		double mEmissivity;

		double mTo[4][16];
};


#endif
