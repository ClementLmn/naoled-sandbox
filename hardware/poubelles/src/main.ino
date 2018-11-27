#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

void setup() {
	Serial.begin(115200);
	WiFi.begin("iPhoneX de Clément", "blablabla");
	while (WiFi.status() != WL_CONNECTED) {
		Serial.println(".");
		delay(500);
	}
}

void loop() {
	if (WiFi.status() == WL_CONNECTED) {
		StaticJsonBuffer<300> JSONbuffer;
		JsonObject& JSONencoder = JSONbuffer.createObject();
		JSONencoder["name"] = "test";
		JSONencoder["value"] = random(100);
		char JSONmessageBuffer[300];
		JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
		Serial.println(JSONmessageBuffer);
		HTTPClient http;
		http.begin("http://192.168.1.230:3000/api");
		http.addHeader("Content-Type", "application/json");
		int httpCode = http.POST(JSONmessageBuffer);
		String payload = http.getString();
		Serial.println(httpCode);
		Serial.println(payload);
		http.end();
	}
	delay(5000);
}