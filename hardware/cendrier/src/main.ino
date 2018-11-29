#include <Button.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// NAME : EYJAFJALLAJOKULL

Button button = Button(D4, INPUT_PULLUP);

void setup() {
  Serial.begin(115200);
  WiFi.begin("Honor 5C", "azertyuiop");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  pinMode(D5, OUTPUT);
}

void loop() {
  if(button.uniquePress()){
    StaticJsonBuffer<300> JSONbuffer;
    JsonObject& listData = JSONbuffer.createObject();
    listData["name"] = "EYJAFJALLAJOKULL";
    sendData(listData);
    goLight();
  }
}

void goLight() {
  digitalWrite(D5, HIGH);
  delay(5000);
  digitalWrite(D5, LOW);
}

void sendData(JsonObject& listData) {
  if (WiFi.status() == WL_CONNECTED) {
    //Declare object of class HTTPClient
    HTTPClient http;
    // Prettier data
    char JSONmessageBuffer[300];
    listData.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
    // connect api
    http.begin("http://api-naoled.cleverapps.io/addAshbin");
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
