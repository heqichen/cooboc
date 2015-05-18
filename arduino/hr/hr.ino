
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
int hrPosition;

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
	hrPosition = 0;
}



void loop()
{
	int d1;
	int d2;
	int tmp;
	unsigned char b;
	unsigned char crc;
	/*
	Serial.print(hrPosition);
	Serial.print("  :   ");
	Serial.print(analogRead(A0));
	Serial.print("  ");
	Serial.print(analogRead(A1));
	Serial.println();
	*/
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
			case ' ':
			{
				hrMovingStatus = MOVING_STATUS_STOP;
				break;
			}
			case 'r':
			{
				while (digitalRead(PIN_LIMIT_BOTTOM))
				{
					moveHrDown();
				}
				while (Serial.available())
				{
					Serial.read();
				}
			}
			default:
			{
				
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

	d1 = analogRead(A0);
	d2 = analogRead(A1);

	//send data
	Serial.write('S');
	Serial.write('A');
	Serial.write(6);
	tmp = hrPosition;
	b = (unsigned char)(tmp & 0x00FF);
	crc = b;
	Serial.write(b);
	tmp >>= 8;
	b = (unsigned char)(tmp & 0x00FF);
	crc ^= b;
	Serial.write(b);

	tmp = d1;
	b = (unsigned char)(tmp & 0xFF);
	crc ^= b;
	Serial.write(b);
	tmp >>= 8;
	b = (unsigned char)(tmp & 0xFF);
	crc ^= b;
	Serial.write(b);

	tmp = d2;
	b = (unsigned char)(tmp & 0xFF);
	crc ^= b;
	Serial.write(b);
	tmp >>= 8;
	b = (unsigned char)(tmp & 0xFF);
	crc ^= b;
	Serial.write(b);

	Serial.write(crc);
	Serial.write('\r');
	Serial.write('\n');
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
	++hrPosition;

}

void moveHrDown()
{
	int i;
	if (!digitalRead(PIN_LIMIT_BOTTOM))
	{
		hrPosition = 0;
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
	--hrPosition;

}