
#define PIN_INA1	3
#define PIN_INA2	4

#define PIN_INB1	5
#define PIN_INB2	6

int stepperMotorPin[4] = {PIN_INA1, PIN_INB1, PIN_INA2, PIN_INB2};



#define PIN_LIMIT_TOP	8
#define PIN_LIMIT_BOTTOM	9


#define MOTOR_TICK_TIME 1


void setup()
{
	pinMode(PIN_INA1, OUTPUT);
	pinMode(PIN_INA2, OUTPUT);
	pinMode(PIN_INB1, OUTPUT);
	pinMode(PIN_INB2, OUTPUT);

	digitalWrite(PIN_INA1, LOW);
	digitalWrite(PIN_INA2, LOW);
	digitalWrite(PIN_INB1, LOW);
	digitalWrite(PIN_INB2, LOW);

	pinMode(PIN_LIMIT_TOP, INPUT);
	pinMode(PIN_LIMIT_BOTTOM, INPUT);

	digitalWrite(PIN_LIMIT_TOP, HIGH);
	digitalWrite(PIN_LIMIT_BOTTOM, HIGH);
	Serial.begin(9600);
}



void loop()
{
	if (!digitalRead(PIN_LIMIT_TOP))
	{
		moveHrDown();
	}
	else
	{
		if (!digitalRead(PIN_LIMIT_BOTTOM))
		{
			moveHrUp();
		}
		else
		{
			delay(10);
		}
	}


}


void moveHrUp()
{
	int i;
	
	for (i=0; i<4; ++i)
	{
		digitalWrite(stepperMotorPin[i], HIGH);
		digitalWrite(stepperMotorPin[(i+3) % 4], HIGH);
		delay(MOTOR_TICK_TIME);
		digitalWrite(stepperMotorPin[(i+3) % 4], LOW);
		delay(4 * MOTOR_TICK_TIME);
		digitalWrite(stepperMotorPin[i], LOW);

	}

}

void moveHrDown()
{
	int i;
	
	for (i=3; i>=0; --i)
	{
		digitalWrite(stepperMotorPin[i], HIGH);
		digitalWrite(stepperMotorPin[(i+1) % 4], HIGH);
		delay(MOTOR_TICK_TIME);
		digitalWrite(stepperMotorPin[(i+1) % 4], LOW);
		delay(4 * MOTOR_TICK_TIME);
		digitalWrite(stepperMotorPin[i], LOW);

	}

}