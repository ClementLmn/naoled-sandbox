int photocellPin = 0;
int photocellReading;

void setup() {
	Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);
}

void loop() {
	photocellReading = analogRead(photocellPin);
	int photocellValue = (photocellReading / 1024.0) * 100;
	Serial.println(photocellValue);
	delay(250);
}
