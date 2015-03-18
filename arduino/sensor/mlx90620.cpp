#include "mlx90620.h"
#include "i2cmaster.h"
#include "MLX90620_registers.h"

#include <Arduino.h>

#define DEFAULT_REFRESH_RATE	16	//I Don't know what kind of refesh rate it is -_-

Mlx90620::Mlx90620()
	:	mRefreshRate	(DEFAULT_REFRESH_RATE)
{

}

void Mlx90620::init()
{
	i2c_init(); //Init the I2C pins
	PORTC = (1 << PORTC4) | (1 << PORTC5); //Enable pull-ups
	delay(5); //Init procedure calls for a 5ms delay after power-on


	initReadWholeEeprom();
	initCalibrationData();
	initWriteOscillatorTrimmingValue();
	writeConfigurationRegister();


	while (true)
	{
		calculateTa();

		Serial.println(readPTAT_MLX90620());
		Serial.println(mTa);
		Serial.println();
		delay(500);
	}
	

}


//Read the 256 bytes from the MLX EEPROM and setup the various constants (*lots* of math)
//Note: The EEPROM on the MLX has a different I2C address from the MLX. I've never seen this before.
void Mlx90620::initReadWholeEeprom()
{
	i2c_start_wait(MLX90620_EEPROM_WRITE);
	i2c_write(0x00); //EEPROM info starts at location 0x00
	i2c_rep_start(MLX90620_EEPROM_READ);

	//Read all 256 bytes from the sensor's EEPROM
	int i;
	for (i=0; i<256; ++i)
	{
		mEepromData[i] = i2c_readAck();
	}
	i2c_stop(); //We're done talking

}

void Mlx90620::printEeprom()
{
	int i;
	for (i=0; i<256; ++i)
	{
		if (i>0 && i%16==0)
		{
			Serial.println();
		}
		Serial.print(mEepromData[i], HEX);
		Serial.print(" ");
	}
}


//Given a 8-bit number from EEPROM (Slave address 0x50), write value to MLX sensor (Slave address 0x60)
void Mlx90620::initWriteOscillatorTrimmingValue()
{
	byte val = mEepromData[0xF7];
	i2c_start_wait(MLX90620_WRITE); //Write to the sensor
	i2c_write(0x04); //Command = write oscillator trimming value
	i2c_write((byte)val - 0xAA);
	i2c_write(val);
	i2c_write(0x56); //Always 0x56
	i2c_write(0x00); //Always 0x00
	i2c_stop();
}


//Receives the refresh rate for sensor scanning
//Sets the two byte configuration registers
//This function overwrites what is currently in the configuration registers
//The MLX doesn't seem to mind this (flags are read only)
void Mlx90620::writeConfigurationRegister()
{
	byte Hz_LSB;
	switch(mRefreshRate)
	{
		case 0:
			Hz_LSB = 0b00001111;
			break;
		case 1:
			Hz_LSB = 0b00001110;
			break;
		case 2:
			Hz_LSB = 0b00001101;
			break;
		case 4:
			Hz_LSB = 0b00001100;
			break;
		case 8:
			Hz_LSB = 0b00001011;
			break;
		case 16:
			Hz_LSB = 0b00001010;
			break;
		case 32:
			Hz_LSB = 0b00001001;
			break;
		default:
			Hz_LSB = 0b00001110;
			break;
	}

	byte defaultConfig_H = 0b01110100; // x111.01xx, Assumes NA = 0, ADC low reference enabled, Ta Refresh rate of 2Hz

	i2c_start_wait(MLX90620_WRITE);
	i2c_write(0x03); //Command = configuration value
	i2c_write((byte)Hz_LSB - 0x55);
	i2c_write(Hz_LSB);
	i2c_write(defaultConfig_H - 0x55); //Assumes NA = 0, ADC low reference enabled, Ta Refresh rate of 2Hz
	i2c_write(defaultConfig_H);
	i2c_stop();
}

//Reads the PTAT data from the MLX
//Returns an unsigned int containing the PTAT
unsigned int Mlx90620::readPTAT_MLX90620()
{
  i2c_start_wait(MLX90620_WRITE);
  i2c_write(CMD_READ_REGISTER); //Command = read PTAT
  i2c_write(0x90); //Start address is 0x90
  i2c_write(0x00); //Address step is 0
  i2c_write(0x01); //Number of reads is 1
  i2c_rep_start(MLX90620_READ);

  byte ptatLow = i2c_readAck(); //Grab the lower and higher PTAT bytes
  byte ptatHigh = i2c_readAck();

  i2c_stop();
  
  return( (unsigned int)(ptatHigh << 8) | ptatLow); //Combine bytes and return
}


int16_t Mlx90620::readS16FromEepromData(uint8_t lsb)
{
	int16_t ret = mEepromData[lsb+1];
	ret <<= 8;
	ret |= mEepromData[lsb];
	return ret;
}

void Mlx90620::initCalibrationData()
{
	mVTH = readS16FromEepromData(MLX_EEPROM_VTH_L);
	mKT1 = (double)readS16FromEepromData(MLX_EEPROM_KT1_L) / 1024.0;
	mKT2 = (double)readS16FromEepromData(MLX_EEPROM_KT2_L) / 1048576.0;

	Serial.print("mVTH: ");
	Serial.println(mVTH);
	Serial.print("mKT1: ");
	Serial.println(mKT1);
	Serial.print("mKT2: ");
	Serial.println(mKT2);

}



//Gets the latest PTAT (package temperature ambient) reading from the MLX
//Then calculates a new Tambient
//Many of these values (k_t1, v_th, etc) come from varInitialization and EEPROM reading
//This has been tested to match example 7.3.2
void Mlx90620::calculateTa()
{
	unsigned int ptat = readPTAT_MLX90620();

	double temp;
	temp = mKT1*mKT1 - (4.0 * mKT2 * (mVTH - (double)ptat));
	temp = sqrt(temp) - mKT1;
	temp /= (2.0 * mKT2);
	temp += 25.0;

	mTa = temp;
	//mTa = (sqrt(mKT1*mKT1-4*mKT2*(mVTH-(float)ptat)) - mKT1) / 2.0 / mKT2 + 25.0;
}
