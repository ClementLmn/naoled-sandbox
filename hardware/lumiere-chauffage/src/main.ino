#include <Wire.h>
#include <DS1621.h>

int photocellPin = 0;
int photocellReading;

void setup() {
	Serial.begin(115200);
	WiFi.begin("iPhone de Guillaume", "commentestvotreblanquette?");
	while (WiFi.status() != WL_CONNECTED) {
		Serial.println(".");
		delay(500);
	}
}

void loop() {
	if (WiFi.status() == WL_CONNECTED) {
		photocellReading = analogRead(photocellPin);
		StaticJsonBuffer<300> JSONbuffer;
		JsonObject& JSONencoder = JSONbuffer.createObject();
		JSONencoder["value"] = photocellReading;
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
	byte addr = (0x90 >> 1) | 0;
	DS1621 sensor=DS1621(addr);
}