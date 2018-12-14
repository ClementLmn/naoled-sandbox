#include <ArduinoJson.h>
#include <ECVHelpers.h>

const String id = "ELSA";

const int trigPin = D4;
const int echoPin = D3;

long duration;
float avg;
const int measuresCount = 300;
const float threshold = 10.0;

bool previousCheck = false;

void setup() {
	Serial.begin(115200);
	wifiConnect();
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
}

void loop() {
	// check multiple times to get a better measure
	avg = checkMultiple(measuresCount);
	Serial.print("Average distance: ");
	Serial.println(avg);
	bool currentCheck = avg < threshold;
	// only post when a cup is detected and wasn't during the previous loop
	if (currentCheck && !previousCheck) {
		StaticJsonBuffer<300> JSONbuffer;
		JsonObject& listDataIn = JSONbuffer.createObject();
		listDataIn["name"] = id;
		post(listDataIn, "setTrashOut");
	}
	previousCheck = currentCheck;
	delay(100);
}

float check() {
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	return (duration / 2) / 29.1;
}

float checkMultiple(int count) {
	float total = 0.0;
	float max = 0.0;
	float min = 2000.0;
	for (int i = 0; i < count; i++) {
		float current = check();
		if (current < min) min = current;
		if (current > max) max = current;
		total += current;
		if(i > (count - 16)){
			max += current;
		}
		if(i < 16){
			min += current;
		}
		delayMicroseconds(10);
	}
	// remove the biggest and smallest measurements to avoid bad ones
	return (total - max - min) / (count - 2);
}
