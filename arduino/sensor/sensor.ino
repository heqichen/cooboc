#include <i2cmaster.h>
#include "MLX90620_registers.h"

#include "mlx90620.h"




Mlx90620 mlx;

double thermalArray[4][16];

void setup()
{
	Serial.begin(9600);
	mlx.init();
	//mlx.printEeprom();
	//mlx.printAlpha();
}

void loop()
{
	mlx.updateFirArray();
	mlx.getTo(thermalArray);

	int i, j;
	for (i=0; i<4; ++i)
	{
		for (j=0; j<16; ++j)
		{
			Serial.print(thermalArray[i][j]);
			Serial.print(" ");
		}
		Serial.println();
	}
	Serial.println();
	delay(500);
}
