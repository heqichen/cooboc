
#define PIN_INA1	3
#define PIN_INA2	4

#define PIN_INB1	5
#define PIN_INB2	6

int stepperMotorPin[4] = {PIN_INA1, PIN_INB1, PIN_INA2, PIN_INB2};



#define PIN_LIMIT_TOP	8
#define PIN_LIMIT_BOTTOM	9


#define MOTOR_TICK_TIME 1

#define MOVING_STATUS_STOP	0
#define MOVING_STATUS_UP	1
#define MOVING_STATUS_DOWN	2

int hrMovingStatus;


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

	hrMovingStatus = MOVING_STATUS_STOP;
}



void loop()
{
	while (Serial.available())
	{
		char c;
		c = Serial.read();
		switch (c)
		{
			case 'w':
			{
				hrMovingStatus = MOVING_STATUS_UP;
				break;
			}
			case 's':
			{
				hrMovingStatus = MOVING_STATUS_DOWN;
				break;
			}
			default:
			{
				hrMovingStatus = MOVING_STATUS_STOP;
				break;
			}
		}
	}

	switch (hrMovingStatus)
	{
		case MOVING_STATUS_UP:
		{
			moveHrUp();
			break;
		}
		case MOVING_STATUS_DOWN:
		{
			moveHrDown();
			break;
		}
		default:
		{
			delay(12 * MOTOR_TICK_TIME);
			break;
		}
	}



}


void moveHrUp()
{
	int i;
	if (!digitalRead(PIN_LIMIT_TOP))
	{
		hrMovingStatus = MOVING_STATUS_STOP;
		return ;
	}
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
	if (!digitalRead(PIN_LIMIT_BOTTOM))
	{
		hrMovingStatus = MOVING_STATUS_STOP;
		return ;
	}
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