#include <elapsedMillis.h>
#include <ArduinoJson.h>
#include <ECVHelpers.h>

const String id = "MAMAN";

elapsedMillis timeElapsed;
unsigned int interval = 60000; // 60s

int stateDoor = 0;
bool messageStatus = false;

void setup() {
	Serial.begin(115200);
	wifiConnect();
	pinMode(D5, INPUT_PULLUP);
}

void loop(){
	int stateDoor = digitalRead(D5);

	if (stateDoor) {
		if ((timeElapsed > interval) && !messageStatus) {
			StaticJsonBuffer<300> JSONbuffer;
			JsonObject& data = JSONbuffer.createObject();
			data["name"] = id;
			data["status"] = 1;
			post(data, "doors");
			messageStatus = true;
		}
	}else{
		if (messageStatus) {
			StaticJsonBuffer<300> JSONbuffer;
			JsonObject& data = JSONbuffer.createObject();
			data["name"] = id;
			data["status"] = 0;
			post(data, "doors");
			messageStatus = false;
		}
		timeElapsed = 0;
	}
}
