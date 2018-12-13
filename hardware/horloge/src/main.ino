#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ECVHelpers.h>

const String id = "JEAN-CLAUDE";

ESP8266WebServer server(80);

const int clockPin1 = D5;
const int clockPin2 = D6;

int currentClockPin = clockPin1;
bool clockWise = true;

int interval = 10;

void setup() {
	Serial.begin(115200);
	wifiConnect("zaki", "f35dac11528da");
	delay(5000);

	server.on("/", handleHomeRequest);
	server.on("/api", handleAPIRequest);
	server.begin();

	pinMode(clockPin1, OUTPUT);
	pinMode(clockPin2, OUTPUT);
	digitalWrite(clockPin1, LOW);
	digitalWrite(clockPin2, LOW);
	delay(100);
}

void loop() {
	server.handleClient();
	tick();
	delay(interval);
}

void handleHomeRequest() {
	StaticJsonBuffer<300> JSONbuffer;
	JsonObject& data = JSONbuffer.createObject();
	data["name"] = id;
	data["switch"] = clockWise;
	data["interval"] = interval;
	char JSONmessageBuffer[300];
	data.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
	server.send(200, "application/json", JSONmessageBuffer);
}

void handleAPIRequest() {
	if (server.hasArg("interval")) {
		int newValue = server.arg("interval").toInt();
		if (newValue == 0) newValue = interval;
		interval = constrain(newValue, 10, 5000);
	}
	if (server.hasArg("switch")) {
		clockWise = !clockWise;
		delay(100);
	}
	Serial.println("test");
	server.send(200, "application/json", "{\"state\":\"ok\"}");
}

void tick() {
	digitalWrite(currentClockPin, clockWise ? LOW : HIGH);
	delay(10);
	digitalWrite(currentClockPin, clockWise ? HIGH : LOW);
	if (currentClockPin == clockPin1) currentClockPin = clockPin2;
	else currentClockPin = clockPin1;
}
