#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// NAME : ELSA

int led = D1;                // the pin that the LED is atteched to
// int sensor = 2;              // the pin that the sensor is atteched to
// int sensorTrash = 3;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int stateTrash = LOW;             // by default, no motion detected
int val, valTrash, nbGobelet;                 // variable to store the sensor status (value)

void setup() {
  Serial.begin(115200);
  WiFi.begin("Honor 5C", "azertyuiop");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  pinMode(led, OUTPUT);      // initalize LED as an output
  // pinMode(sensor, INPUT);    // initialize sensor as an input
  // pinMode(sensorTrash, INPUT);    // initialize sensor as an input

}

void loop() {
  // val = digitalRead(sensor);   // read sensor value
  val = random(100);

  // PARTIE 1: Check lorsqu'un gobelet est acheté

  // if (val == HIGH) {           // check if the sensor is HIGH
  if (val > 49) {           // check if the sensor is HIGH
    digitalWrite(led, HIGH);   // turn LED ON
    delay(100);                // delay 100 milliseconds


    if (state == LOW) {
      StaticJsonBuffer<300> JSONbuffer;
      JsonObject& listDataIn = JSONbuffer.createObject();
      listDataIn["name"] = "ELSA";
      sendDataTrashIn(listDataIn);

      nbGobelet++;
      state = HIGH;       // update variable state to HIGH
    }
  } else {
    digitalWrite(led, LOW); // turn LED OFF
    if (state == HIGH){
      state = LOW;       // update variable state to LOW
    }
  }

  // PARTIE 2: Check lorsqu'un gobelet est jeté
  // valTrash = digitalRead(sensorTrash);   // read sensor value
  valTrash = random(100);

  if (valTrash > 74){
    if (stateTrash == LOW) {
      StaticJsonBuffer<300> JSONbuffer;
      JsonObject& listDataOut = JSONbuffer.createObject();
      listDataOut["name"] = "ELSA";
      sendDataTrashOut(listDataOut);

      stateTrash = HIGH;       // update variable state to HIGH
      nbGobelet--;

    } else{
      if (stateTrash == HIGH){
        stateTrash = LOW;       // update variable state to LOW
      }
    }
  }

  Serial.print("GOBELETS GASPILLÉS : ");
  Serial.println(nbGobelet);

  delay(1000);
}

void sendDataTrashIn(JsonObject& listDataIn) {
  if (WiFi.status() == WL_CONNECTED) {
    //Declare object of class HTTPClient
    HTTPClient http;
    // Prettier data
    char JSONmessageBuffer[300];
    listDataIn.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    // connect api
    http.begin("http://api-naoled.cleverapps.io/setTrashIn");
    http.addHeader("Content-Type", "application/json");
    // send data
    int httpCode = http.POST(JSONmessageBuffer);
    // <0 if error
    if(httpCode > 0){
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      String payload = http.getString();
      // HTTP return code
      Serial.println(httpCode);
      // request response payload
      Serial.println(payload);
    }else{
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
    Serial.print("Message envoyé");
  }else{
    Serial.printf("[HTTP} Internet not found\n");
  }
}

void sendDataTrashOut(JsonObject& listDataOut) {
  if (WiFi.status() == WL_CONNECTED) {
    //Declare object of class HTTPClient
    HTTPClient http;
    // Prettier data
    char JSONmessageBuffer[300];
    listDataOut.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    // connect api
    http.begin("http://api-naoled.cleverapps.io/setTrashOut");
    http.addHeader("Content-Type", "application/json");
    // send data
    int httpCode = http.POST(JSONmessageBuffer);
    // <0 if error
    if(httpCode > 0){
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      String payload = http.getString();
      // HTTP return code
      Serial.println(httpCode);
      // request response payload
      Serial.println(payload);
    }else{
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
    Serial.print("Message envoyé");
  }else{
    Serial.printf("[HTTP} Internet not found\n");
  }
}
