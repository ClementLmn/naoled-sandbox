#include <Button.h>
#include <ECVHelpers.h>
#include <ArduinoJson.h>

const String id = "EYJAFJALLAJOKULL";

Button button = Button(D4, INPUT_PULLUP);

void setup() {
	Serial.begin(115200);
	wifiConnect();
	pinMode(D5, OUTPUT);
}

void loop() {
	if(button.uniquePress()){
		StaticJsonBuffer<300> JSONbuffer;
		JsonObject& listData = JSONbuffer.createObject();
		listData["name"] = id;
		post(listDataIn, "addAshbin");
		goLight();
	}
}

void goLight() {
	digitalWrite(D5, HIGH);
	delay(5000);
	digitalWrite(D5, LOW);
}
