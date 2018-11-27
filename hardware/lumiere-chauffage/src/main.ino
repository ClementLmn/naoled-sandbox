#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

String arduinoId = "arduino-cafet";
int photocellPin = 0;
int photocellReading;

void setup() {
	Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);

  sensor.startConversion(false);
  sensor.setConfig(DS1621::POL | DS1621::ONE_SHOT);

  Wire.begin(D1, D2);
}

void loop() {
	if (WiFi.status() == WL_CONNECTED) {
		photocellReading = analogRead(photocellPin);
		int photocellValue = (photocellReading / 1024.0) * 100;
		StaticJsonBuffer<300> JSONbuffer;
		JsonObject& JSONencoder = JSONbuffer.createObject();
		JSONencoder["id"] = arduinoId;
		JSONencoder["value"] = photocellValue;
		char JSONmessageBuffer[300];
		JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
		Serial.println(JSONmessageBuffer);
		HTTPClient http;
		http.begin("http://172.20.10.3:3000/api");
		http.addHeader("Content-Type", "application/json");
		int httpCode = http.POST(JSONmessageBuffer);
		String payload = http.getString();
		Serial.println(httpCode);
		Serial.println(payload);
		http.end();
	}
	delay(250);
}
