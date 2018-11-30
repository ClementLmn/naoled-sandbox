#include <elapsedMillis.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// NAME : THOMAS-GUILLET

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
  WiFi.begin("Honor 5C", "azertyuiop");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Photosensor
  pinMode(photoSensor, INPUT_PULLUP);
  // pinMode(photoSensorRight, INPUT_PULLUP);
  // Motionsensor
  pinMode(motionSensor, INPUT);
}


void loop() {
  if (movement()) {
    if(!shouldSend){
      StaticJsonBuffer<300> JSONbuffer;
      JsonObject& listData = JSONbuffer.createObject();
      listData["name"] = "THOMAS-GUILLET";
      listData["status"] = 0;

      sendData(listData);
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
      listData["name"] = "THOMAS-GUILLET";
      listData["status"] = 1;

      sendData(listData);
      shouldSend = false;
  }
}

bool movement() {
  long stateMotionSensor = digitalRead(motionSensor);
  if(stateMotionSensor != HIGH){
    return false;
  }else{
    return true;
  }
}

bool light() {
  // On récupère les données du capteur de lumière
  int statePhotoSensor = analogRead(photoSensor);
  // on formate
  int statePhotoSensorFormate = (statePhotoSensor / 1024.0) * 100;
  if(statePhotoSensorFormate > lightIsOn){
    return true;
  }else{
    return false;
  }
}

void sendData(JsonObject& listData) {
  if (WiFi.status() == WL_CONNECTED) {
    //Declare object of class HTTPClient
    HTTPClient http;
    // Prettier data
    char JSONmessageBuffer[300];
    listData.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    // connect api
    http.begin("http://api-naoled.cleverapps.io/lights");
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
