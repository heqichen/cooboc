#include <i2cmaster.h>
#include "MLX90620_registers.h"

#include "mlx90620.h"




//Global variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int irData[64]; //Contains the raw IR data from the sensor
float temperatures[64]; //Contains the calculated temperatures of each pixel in the array
float Tambient; //Tracks the changing ambient temperature of the sensor
byte eepromData[256]; //Contains the full EEPROM reading from the MLX (Slave 0x50)

//These are constants calculated from the calibration data stored in EEPROM
//See varInitialize and section 7.3 for more information
int v_th, a_cp, b_cp, tgc, b_i_scale;
float k_t1, k_t2, emissivity;
int a_ij[64], b_ij[64];

//These values are calculated using equation 7.3.3.2
//They are constants and can be calculated using the MLX90620_alphaCalculator sketch
float alpha_ij[64] = {
  1.66583E-8, 1.85792E-8, 1.78807E-8, 1.57270E-8, 1.87538E-8, 2.05582E-8, 1.98597E-8, 1.81717E-8, 
  2.05582E-8, 2.21880E-8, 2.27119E-8, 1.96269E-8, 2.27701E-8, 2.45745E-8, 2.45745E-8, 2.10239E-8, 
  2.43417E-8, 2.62044E-8, 2.59715E-8, 2.31776E-8, 2.50402E-8, 2.77178E-8, 2.74267E-8, 2.46328E-8, 
  2.57969E-8, 2.83580E-8, 2.76596E-8, 2.50984E-8, 2.60297E-8, 2.88237E-8, 2.86491E-8, 2.57387E-8, 
  2.62044E-8, 2.86491E-8, 2.85909E-8, 2.50402E-8, 2.62626E-8, 2.90565E-8, 2.85909E-8, 2.50402E-8, 
  2.55059E-8, 2.83580E-8, 2.78924E-8, 2.57387E-8, 2.52730E-8, 2.76596E-8, 2.74267E-8, 2.52730E-8, 
  2.41089E-8, 2.62044E-8, 2.66700E-8, 2.45745E-8, 2.27701E-8, 2.57387E-8, 2.55059E-8, 2.31194E-8, 
  2.12567E-8, 2.41089E-8, 2.41089E-8, 2.21880E-8, 1.92194E-8, 2.27119E-8, 2.21880E-8, 2.05582E-8, 
};

byte loopCount = 0; //Used in main loop
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

Mlx90620 m2;

void setup()
{
	Serial.begin(9600);
	m2.init();
	m2.printEeprom();
}

void loop()
{
	Serial.println(sizeof(double));
	delay(500);
}

//Begin Program code

void read_EEPROM_MLX90620(){}
unsigned int readPTAT_MLX90620(){return 0;}
void calculate_TA(void){}

void setup2()
{
  Serial.begin(9600);
  Serial.println("MLX90620 Example");

  i2c_init(); //Init the I2C pins
  PORTC = (1 << PORTC4) | (1 << PORTC5); //Enable pull-ups

  delay(5); //Init procedure calls for a 5ms delay after power-on

  read_EEPROM_MLX90620(); //Read the entire EEPROM

  //setConfiguration(refreshRate); //Configure the MLX sensor with the user's choice of refresh rate

  calculate_TA(); //Calculate the current Tambient
}

void loop2()
{
  if(loopCount++ == 16) //Tambient changes more slowly than the pixel readings. Update TA only every 16 loops.
  { 
    calculate_TA(); //Calculate the new Tambient

    if(checkConfig_MLX90620()) //Every 16 readings check that the POR flag is not set
    {
      Serial.println("POR Detected!");
      //setConfiguration(refreshRate); //Re-write the configuration bytes to the MLX
    }

    loopCount = 0; //Reset count
  }

  readIR_MLX90620(); //Get the 64 bytes of raw pixel data into the irData array

  calculate_TO(); //Run all the large calculations to get the temperature data for each pixel

  prettyPrintTemperatures(); //Print the array in a 4 x 16 pattern
  //rawPrintTemperatures(); //Print the entire array so it can more easily be read by Processing app
}

//From the 256 bytes of EEPROM data, initialize 
void varInitialization(byte calibration_data[])
{
  v_th = 256 * calibration_data[VTH_H] + calibration_data[VTH_L];
  k_t1 = (256 * calibration_data[KT1_H] + calibration_data[KT1_L]) / 1024.0; //2^10 = 1024
  k_t2 = (256 * calibration_data[KT2_H] + calibration_data[KT2_L]) / 1048576.0; //2^20 = 1,048,576
  emissivity = ((unsigned int)256 * calibration_data[CAL_EMIS_H] + calibration_data[CAL_EMIS_L]) / 32768.0;
  
  a_cp = calibration_data[CAL_ACP];
  if(a_cp > 127) a_cp -= 256; //These values are stored as 2's compliment. This coverts it if necessary.

  b_cp = calibration_data[CAL_BCP];
  if(b_cp > 127) b_cp -= 256;

  tgc = calibration_data[CAL_TGC];
  if(tgc > 127) tgc -= 256;

  b_i_scale = calibration_data[CAL_BI_SCALE];

  for(int i = 0 ; i < 64 ; i++)
  {
    //Read the individual pixel offsets
    a_ij[i] = calibration_data[i]; 
    if(a_ij[i] > 127) a_ij[i] -= 256; //These values are stored as 2's compliment. This coverts it if necessary.

    //Read the individual pixel offset slope coefficients
    b_ij[i] = calibration_data[0x40 + i]; //Bi(i,j) begins 64 bytes into EEPROM at 0x40
    if(b_ij[i] > 127) b_ij[i] -= 256;
  }
  
}











//Calculate the temperatures seen for each pixel
//Relies on the raw irData array
//Returns an 64-int array called temperatures
void calculate_TO()
{
  float v_ir_off_comp;
  float v_ir_tgc_comp;
  float v_ir_comp;

  //Calculate the offset compensation for the one compensation pixel
  //This is a constant in the TO calculation, so calculate it here.
  int cpix = readCPIX_MLX90620(); //Go get the raw data of the compensation pixel
  float v_cp_off_comp = (float)cpix - (a_cp + (b_cp/pow(2, b_i_scale)) * (Tambient - 25)); 

  for (int i = 0 ; i < 64 ; i++)
  {
    v_ir_off_comp = irData[i] - (a_ij[i] + (float)(b_ij[i]/pow(2, b_i_scale)) * (Tambient - 25)); //#1: Calculate Offset Compensation 

    v_ir_tgc_comp = v_ir_off_comp - ( ((float)tgc/32) * v_cp_off_comp); //#2: Calculate Thermal Gradien Compensation (TGC)

    v_ir_comp = v_ir_tgc_comp / emissivity; //#3: Calculate Emissivity Compensation

    temperatures[i] = sqrt( sqrt( (v_ir_comp/alpha_ij[i]) + pow(Tambient + 273.15, 4) )) - 273.15;
  }
}

//Reads 64 bytes of pixel data from the MLX
//Loads the data into the irData array
void readIR_MLX90620()
{
  i2c_start_wait(MLX90620_WRITE);
  i2c_write(CMD_READ_REGISTER); //Command = read a register
  i2c_write(0x00); //Start address = 0x00
  i2c_write(0x01); //Address step = 1
  i2c_write(0x40); //Number of reads is 64
  i2c_rep_start(MLX90620_READ);

  for(int i = 0 ; i < 64 ; i++)
  {
    byte pixelDataLow = i2c_readAck();
    byte pixelDataHigh = i2c_readAck();
    irData[i] = (int)(pixelDataHigh << 8) | pixelDataLow;
  }

  i2c_stop();
}

//Read the compensation pixel 16 bit data
int readCPIX_MLX90620()
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

//Reads the current configuration register (2 bytes) from the MLX
//Returns two bytes
unsigned int readConfig_MLX90620()
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

  return( (unsigned int)(configHigh << 8) | configLow); //Combine the configuration bytes and return as one unsigned int
}

//Poll the MLX for its current status
//Returns true if the POR/Brown out bit is set
boolean checkConfig_MLX90620()
{
  if ( (readConfig_MLX90620() & (unsigned int)1<<POR_TEST) == 0)
    return true;
  else
    return false;
}

//Prints the temperatures in a way that's more easily viewable in the terminal window
void prettyPrintTemperatures()
{
  Serial.println();
  for(int i = 0 ; i < 64 ; i++)
  {
    if(i % 16 == 0) Serial.println();
    Serial.print(convertToFahrenheit(temperatures[i]));
    //Serial.print(irData[i]);
    Serial.print(", ");
  }
}

//Prints the temperatures in a way that's more easily parsed by a Processing app
//Each line starts with '$' and ends with '*'
void rawPrintTemperatures()
{
  Serial.print("$");
  for(int i = 0 ; i < 64 ; i++)
  {
    Serial.print(convertToFahrenheit(temperatures[i]));
    Serial.print(","); //Don't print comma on last temperature
  }
  Serial.println("*");
}

//Given a Celsius float, converts to Fahrenheit
float convertToFahrenheit (float Tc)
{
  float Tf = (9/5) * Tc + 32;

  return(Tf);
}
