#include "mlx90620.h"
#include "i2cmaster.h"
#include "MLX90620_registers.h"

#include <Arduino.h>

#define DEFAULT_REFRESH_RATE	16	//I Don't know what kind of refesh rate it is -_-

Mlx90620::Mlx90620()
	:	lastPorCheckTime	(0UL),
		mRefreshRate	(DEFAULT_REFRESH_RATE)
{

}

void Mlx90620::init()
{
	i2c_init(); //Init the I2C pins
	PORTC = (1 << PORTC4) | (1 << PORTC5); //Enable pull-ups
	delay(5); //Init procedure calls for a 5ms delay after power-on


	initReadWholeEeprom();
	initCalibrationData();
	initCalculateAlpha();
	
	initWriteOscillatorTrimmingValue();
	writeConfigurationRegister();
	calculateTa();
}

double Mlx90620::getTemperatureAmbient()
{
	calculateTa();
	return mTa;
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
	Serial.println();
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
unsigned int Mlx90620::readPtat()
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

uint16_t Mlx90620::readU16FromEepromData(uint8_t lsb)
{
	uint16_t ret = mEepromData[lsb+1];
	ret <<= 8;
	ret |= mEepromData[lsb];
	return ret;
}

uint8_t Mlx90620::readU8FromEepromData(uint8_t address)
{
	return (uint8_t)mEepromData[address];
}

int8_t Mlx90620::readS8FromEepromData(uint8_t address)
{
	return (int8_t)mEepromData[address];
}



//Gets the latest PTAT (package ambient temperature) reading from the MLX
//Then calculates a new Tambient
//Many of these values (k_t1, v_th, etc) come from varInitialization and EEPROM reading
//This has been tested to match example 7.3.2
void Mlx90620::calculateTa()
{
	unsigned int ptat = readPtat();

	double temp;
	temp = mKT1*mKT1 - (4.0 * mKT2 * (mVTH - (double)ptat));
	temp = sqrt(temp) - mKT1;
	temp /= (2.0 * mKT2);
	temp += 25.0;

	mTa = temp;
}


void Mlx90620::updateFirArray()
{
	// check por
	if (millis() - lastPorCheckTime > POR_CHECK_TIMEOUT)
	{
		//1. update Ambient Temperature
		calculateTa();
		//2. check POR
		if (isPor())
		{
			//set POR/Brown-Out to 1 P11 on Datashit
			writeConfigurationRegister();
		}
		//3. update time
		lastPorCheckTime = millis();
	}

	updateIrData();
	calculateTo();
}

//Reads the current configuration register (2 bytes) from the MLX
//Returns two bytes
uint16_t Mlx90620::readConfigurationRegister()
{
  i2c_start_wait(MLX90620_WRITE); //The MLX configuration is in the MLX, not EEPROM
  i2c_write(CMD_READ_REGISTER); //Command = read configuration register
  i2c_write(0x92); //Start address
  i2c_write(0x00); //Address step of zero
  i2c_write(0x01); //Number of reads is 1

  i2c_rep_start(MLX90620_READ);

  byte configLow = i2c_readAck(); //Grab the two bytes
  byte configHigh = i2c_readAck();

  i2c_stop();

  return( (uint16_t)(configHigh << 8) | configLow); //Combine the configuration bytes and return as one unsigned int
}


//Poll the MLX for its current status
//Returns true if the POR/Brown out bit is set
bool Mlx90620::isPor()
{
	return ( (readConfigurationRegister()&(uint16_t)1<<POR_TEST) == 0);
}

//Reads 64 bytes of pixel data from the MLX
//Loads the data into the irData array
void Mlx90620::updateIrData()
{
	//read whole frame, P12 on Datashit
	i2c_start_wait(MLX90620_WRITE);
	i2c_write(CMD_READ_REGISTER); //Command = read a register
	i2c_write(0x00); //Start address = 0x00
	i2c_write(0x01); //Address step = 1
	i2c_write(0x40); //Number of reads is 64
	i2c_rep_start(MLX90620_READ);

	int i, j;
	for (j=0; j<16; ++j)
	{
		for (i=0; i<4; ++i)
		{
			byte pixelDataLow = i2c_readAck();
			byte pixelDataHigh = i2c_readAck();
			int16_t irData = pixelDataHigh;
			irData <<= 8;
			irData |= pixelDataLow;
			mIrData[i][j] = irData;
		}
	}

	i2c_stop();
}

void Mlx90620::initCalculateAlpha()
{
	uint16_t alpha0 = readU16FromEepromData(MLX_EEPROM_CAL_A0_L);
	uint8_t alpha0scale = readU8FromEepromData(MLX_EEPROM_CAL_A0_SCALE);
	uint8_t deltaAlphaScale = readU8FromEepromData(MLX_EEPROM_CAL_DELTA_A_SCALE);
	uint16_t alphaCp = readU16FromEepromData(MLX_EEPROM_CAL_alphaCP_L);


	double commonItem = ((double)alpha0 - mTgc/32.0*alphaCp) / (pow(2.0, alpha0scale));

	int i, j;
	for (j=0; j<16; ++j)
	{
		for (i=0; i<4; ++i)
		{
			double deltaAlpha = readU8FromEepromData(0x80 + j*4 + i);
			mAlpha[i][j] = commonItem + (deltaAlpha / pow(2.0, deltaAlphaScale));
		}
	}
}

void Mlx90620::printAlpha()
{
	int i, j;
	for (i=0; i<4; ++i)
	{
		for (j=0; j<16; ++j)
		{
			Serial.print(mAlpha[i][j], 20);
			Serial.print("\t");
		}
		Serial.println();
	}
}


//Read the compensation pixel 16 bit data
int Mlx90620::readCompensationPixel()
{
  i2c_start_wait(MLX90620_WRITE);
  i2c_write(CMD_READ_REGISTER); //Command = read register
  i2c_write(0x91);
  i2c_write(0x00);
  i2c_write(0x01);
  i2c_rep_start(MLX90620_READ);

  byte cpixLow = i2c_readAck(); //Grab the two bytes
  byte cpixHigh = i2c_readAck();
  i2c_stop();

  return ( (int)(cpixHigh << 8) | cpixLow);
}


void Mlx90620::initCalibrationData()
{
	mVTH = readS16FromEepromData(MLX_EEPROM_VTH_L);
	mKT1 = (double)readS16FromEepromData(MLX_EEPROM_KT1_L) / 1024.0;
	mKT2 = (double)readS16FromEepromData(MLX_EEPROM_KT2_L) / 1048576.0;

	mAcp = readS8FromEepromData(MLX_EEPROM_CAL_ACP);
	mBcp = readS8FromEepromData(MLX_EEPROM_CAL_BCP);
	mBiScale = readU8FromEepromData(MLX_EEPROM_CAL_BI_SCALE);
	mTgc = readS8FromEepromData(MLX_EEPROM_CAL_TGC);
	uint16_t epsilon = readU16FromEepromData(MLX_EEPROM_CAL_EMIS_L);
	mEmissivity = (double)epsilon/32768.0;
}


//Calculate the temperatures seen for each pixel
//Relies on the raw irData array
//Returns an 64-int array called temperatures
void Mlx90620::calculateTo()	//Object Temperature
{
	int vCp = readCompensationPixel();
	double vCpOffComp = vCp - mAcp - mBcp/pow(2.0, mBiScale)*(mTa-25.0);

	int i, j;
	for (j=0; j<16; ++j)
	{
		for (i=0; i<4; ++i)
		{
			int ai = readS8FromEepromData(4*j + i);
			int bi = readS8FromEepromData(0x40 + 4*j + i);
			double vIrOffComp = mIrData[i][j] - ai - bi/pow(2.0, mBiScale)*(mTa-25.0);
			double vIrTgcComp = vIrOffComp - (float)mTgc/32.0*vCpOffComp;
			double vIrCompensated = vIrTgcComp / mEmissivity;

			mTo[i][j] = 
				sqrt(
					sqrt(
						vIrCompensated/mAlpha[i][j] 
						+ pow(
							mTa+273.15, 4.0
						)
					)
				) - 273.15;
		}
	}

}


void Mlx90620::printTo()
{
	int i, j;
	for (i=0; i<4; ++i)
	{
		for (j=0; j<16; ++j)
		{
			Serial.print(mTo[i][j]);
			Serial.print(" ");
		}

		Serial.println();
	}
	Serial.println();
}

void Mlx90620::getTo(double thermalArray[][16])
{
	int i, j;
	for (i=0; i<4; ++i)
	{
		for (j=0; j<16; ++j)
		{
			thermalArray[i][j] = mTo[i][j];
		}
	}
}
