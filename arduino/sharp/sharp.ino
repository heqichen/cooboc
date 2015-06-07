void setup()
{
	pinMode(13, OUTPUT);
	Serial.begin(9600);
}


void loop()
{
	digitalWrite(13, HIGH);
	delay(200);
	digitalWrite(13, LOW);
	delay(200);
	int a = analogRead(A0);
	int b = analogRead(A1);

	Serial.print(a);
	Serial.print(" ,\t");
	Serial.println(b);
}
