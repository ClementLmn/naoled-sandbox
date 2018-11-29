#include <Button.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// NAME : STANNAH

#define PIN_STRIP 3

// How many leds in your strip?
#define NUM_LEDS 240
#define MAX_LEDS 240

Button button = Button(12, INPUT_PULLUP);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_STRIP, NEO_GRB + NEO_KHZ800);

void setup() {
	Serial.begin(115200);
	WiFi.begin("Honor 5C", "azertyuiop");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}

  strip.begin();
  strip.setBrightness(50);
  strip.show();
}

void loop() {
  if(button.uniquePress()){
    Serial.println("pressed");
    boom(strip.Color(255, 0, 0), 1000, MAX_LEDS, 5);

		// Envoi des données
		StaticJsonBuffer<300> JSONbuffer;
		JsonObject& listData = JSONbuffer.createObject();
		listData["name"] = "STANNAH";
		sendData(listData);
  }else{
    Serial.println("not pressed");
  }
}

void boom(uint32_t c, uint8_t wait, uint8_t total, uint8_t wave_size) {
  uint8_t center = ceil(total / 2);

  uint16_t i, j;
  for(i=0; i<total + wave_size / 2; i++) {
    strip.setPixelColor(center-i, c);
    strip.setPixelColor(center+i, c);
    strip.setPixelColor(center-i+wave_size, strip.Color(0, 0, 0));
    strip.setPixelColor(center+i-wave_size, strip.Color(0, 0, 0));
    strip.show();
    delay(10);
  }

  for(j=0; j<wave_size; j++) {
    strip.setPixelColor(j + 1, strip.Color(0, 0, 0));
    strip.setPixelColor(total-j+1, strip.Color(0, 0, 0));
    strip.show();
  }
  delay(wait);
}

void sendData(listData) {
  if (WiFi.status() == WL_CONNECTED) {
    //Declare object of class HTTPClient
    HTTPClient http;
    // Prettier data
    char JSONmessageBuffer[300];
    listData.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    // connect api
    http.begin("http://api-naoled.cleverapps.io/ADDSTAIRS");
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
