void setup()
{
	Serial.begin(9600);
}

void loop()
{
	Serial.write('S');
	Serial.write('A');
	Serial.write(4);
	unsigned int a;
	unsigned char b;
	unsigned char crc;

	a = analogRead(A0);
	b = (unsigned char)(a & 0xFF);
	crc = b;
	Serial.write(b);
	a >>= 8;
	b = (unsigned char)(a & 0xFF);
	crc ^= b;
	Serial.write(b);

	a = analogRead(A1);
	b = (unsigned char)(a & 0xFF);
	crc ^= b;
	Serial.write(b);
	a >>= 8;
	b = (unsigned char)(a & 0xFF);
	crc ^= b;
	Serial.write(b);

	Serial.write(crc);

	Serial.write('\r');
	Serial.write('\n');

	delay(20);
}