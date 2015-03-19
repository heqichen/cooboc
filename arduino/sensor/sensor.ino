#include <i2cmaster.h>
#include "MLX90620_registers.h"

#include "mlx90620.h"



Mlx90620 mlx;
double thermalArray[4][16];

uint8_t sendBuffer[256];

void emitData(uint8_t *data, int length);

void setup()
{
	Serial.begin(57600);
	mlx.init();
	mlx.printEeprom();
	mlx.printAlpha();
}

void testLoop()
{
	int i;
	for (i=0; i<256; ++i)
	{
		Serial.write((uint8_t)0);
	}
	delay(1000);
}

void loop()
{
	mlx.updateFirArray();
	mlx.getTo(thermalArray);

	int i, j, k;
	sendBuffer[0] = 0x01;
	for (i=0; i<2; ++i)
	{
		for (j=0; j<16; ++j)
		{
			uint8_t *doubleBuffer;
			doubleBuffer = (uint8_t *)((void *)(thermalArray[i]+j));
			for (k=0; k<4; ++k)
			{
				sendBuffer[1 + i*16*4 + j*4 + k] = doubleBuffer[k];
			}
			
		}
	}
	emitData(sendBuffer, 2*16*4+1);

	delay(1000);
}

void emitData(uint8_t *data, int length)
{
	Serial.print("Dummy");	//write some dummy bytes for starting uart communication
	Serial.write('S');
	Serial.write('A');
	Serial.write((unsigned char)length);
	uint8_t crc = 0x00;
	int i;
	for (i=0; i<length; ++i)
	{
		crc ^= data[i];
		Serial.write(data[i]);
	}
	Serial.write(crc);
	Serial.write('\n');
}
