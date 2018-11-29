// #include <ESP8266HTTPClient.h>
// #include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// NAME : ELSA

int led = 13;                // the pin that the LED is atteched to
int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)

void setup() {
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
	Serial.begin(115200);
	// WiFi.begin("iPhoneX de Clément", "blablabla");
	// while (WiFi.status() != WL_CONNECTED) {
	// 	Serial.println(".");
	// 	delay(500);
	// }
}

void loop() {

  val = digitalRead(sensor);   // read sensor value
  // Serial.println(LOW);
  // Serial.println(HIGH);
  Serial.println(val);
  if (val == HIGH) {           // check if the sensor is HIGH
    // digitalWrite(led, HIGH);   // turn LED ON
    delay(100);                // delay 100 milliseconds

    if (state == LOW) {
      Serial.println("Motion detected!");
      state = HIGH;       // update variable state to HIGH
    }
  }
  else {
      // digitalWrite(led, LOW); // turn LED OFF
      delay(200);             // delay 200 milliseconds

      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }

	// if (WiFi.status() == WL_CONNECTED) {
  //   sensorValue = digitalRead(SensorSignal); // read the value of pin 2, should be high or low
	// 	StaticJsonBuffer<300> JSONbuffer;
	// 	JsonObject& JSONencoder = JSONbuffer.createObject();
	// 	JSONencoder["name"] = "test";
	// 	JSONencoder["value"] = sensorValue;
	// 	char JSONmessageBuffer[300];
	// 	JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
	// 	Serial.println(JSONmessageBuffer);
	// 	HTTPClient http;
	// 	http.begin("http://192.168.1.230:3000/api");
	// 	http.addHeader("Content-Type", "application/json");
	// 	int httpCode = http.POST(JSONmessageBuffer);
	// 	String payload = http.getString();
	// 	Serial.println(httpCode);
	// 	Serial.println(payload);
	// 	http.end();
  //   delay(100);
	// }
}
