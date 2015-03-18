#ifndef __MLX90620_H__
#define __MLX90620_H__

#include <Arduino.h>
#include <stdint.h>

/* Pin 
	
	A4	SDA
	A5	SCL
	
*/


class Mlx90620
{
	public:
		Mlx90620();
		void init();

		void printEeprom();
	private:
		//init seq #1
		void initReadWholeEeprom();	//7.1 P10 on Datashit
		//init seq #2
		void initWriteOscillatorTrimmingValue(); //7.1 P10 on Datashit
		void writeConfigurationRegister();	//7.1 P10 on Datashit
		void initCalibrationData();

		unsigned int readPTAT_MLX90620();
		void calculateTa();


		int16_t readS16FromEepromData(uint8_t lsb);


		byte mEepromData[256];
		int mRefreshRate; //Set this value to your desired refresh frequency
		double mTa;//Temperature Ambient
		double mKT1;
		double mKT2;
		double mVTH;
};


#endif
