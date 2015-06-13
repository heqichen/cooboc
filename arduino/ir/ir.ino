void setup()
{
	Serial.begin(9600);
	pinMode(13, OUTPUT);
	pinMode(12, INPUT);
}

void loop()
{
	//Serial.println("ready");
	while (digitalRead(12))
	{
		//Serial.println(micros());
	}

	unsigned long t = micros();
	unsigned long d = 0UL;
	while (d < 2000000UL)
	{
		while (!digitalRead(12))
		{}
		d = micros() - t;
		t = micros();
		Serial.print("ON  \t");
		Serial.println(d);
		while (digitalRead(12))
		{
			if ((micros() - t) > 2000000UL)
			{
				break;
			}
		}
		d = micros() - t;
		t = micros();
		if (d < 2000000UL)
		{
			Serial.print("OFF \t");
			Serial.println(d);
		}
	}
	Serial.println();
	Serial.println();
	Serial.println();
	//Serial.println("end");
	//delay(2000);
	
}

