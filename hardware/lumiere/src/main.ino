#include <elapsedMillis.h>
#include <ArduinoJson.h>
#include <ECVHelpers.h>


const String id = "THOMAS-GUILLET";

elapsedMillis timeElapsed;
unsigned int interval = 30000; // 30s

bool shouldSend = true;
// On défini à partir de quelle valeur la lumière est allumé
int lightIsOn = 80;

int motionSensor = D7;

int photoSensor = A0;
// int photoSensorRight = 1;

void setup() {
	Serial.begin(115200);
	wifiConnect();
	// Photosensor
	pinMode(photoSensor, INPUT_PULLUP);
	// pinMode(photoSensorRight, INPUT_PULLUP);
	// Motionsensor
	pinMode(motionSensor, INPUT);
}


void loop() {
	if (movement()) {
		if (!shouldSend) {
			StaticJsonBuffer<300> JSONbuffer;
			JsonObject& listData = JSONbuffer.createObject();
			listData["name"] = id;
			listData["status"] = 0;
			post(listData, "lights");
		}
		timeElapsed = 0;
		shouldSend = true;
	}
	if (timeElapsed > interval && light() && shouldSend) {
		// Création de l'array des données
		// Inside the brackets, 300 is the size of the pool in bytes. Don't forget to change this value to match your JSON document. Use arduinojson.org/assistant to compute the capacity.
		StaticJsonBuffer<300> JSONbuffer;
		// Create the root of the object tree.
		// It's a reference to the JsonObject, the actual bytes are inside the JsonBuffer with all the other nodes of the object tree. Memory is freed when jsonBuffer goes out of scope.
		JsonObject& listData = JSONbuffer.createObject();
		// Set values
		listData["name"] = id;
		listData["status"] = 1;

		post(listData, "lights");
		shouldSend = false;
	}
}

bool movement() {
	long stateMotionSensor = digitalRead(motionSensor);
	if (stateMotionSensor != HIGH) {
		return false;
	}
	else {
		return true;
	}
}

bool light() {
	// On récupère les données du capteur de lumière
	int statePhotoSensor = analogRead(photoSensor);
	// on formate
	int statePhotoSensorFormate = (statePhotoSensor / 1024.0) * 100;
	if (statePhotoSensorFormate > lightIsOn) {
		return true;
	}
	else {
		return false;
	}
}
