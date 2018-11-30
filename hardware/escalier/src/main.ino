#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_D1_PIN_ORDER
#include <FastLED.h>
#include <Button.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
// #include <ArduinoJson.h>

// NAME : STANNAH



#define DEBUG 1
#define PIN_STRIP_1 D2
#define PIN_STRIP_2 D6

#if DEBUG
#define NUM_STRIPS 2
#else
#define PIN_STRIP_3 D10
#define NUM_STRIPS 3
#endif

#define NUM_LEDS_PER_STRIP 60
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS

CRGB leds[NUM_LEDS];

Button button = Button(D3, INPUT_PULLUP);

void setup() {
	Serial.begin(115200);
  WiFi.begin("Honor 5C", "azertyuiop");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(500);
  }

	// tell FastLED there's 60 NEOPIXEL leds on pin 10, starting at index 0 in the led array
	FastLED.addLeds<NEOPIXEL, PIN_STRIP_1>(leds, 0, NUM_LEDS_PER_STRIP);
	// tell FastLED there's 60 NEOPIXEL leds on pin 11, starting at index 60 in the led array
	FastLED.addLeds<NEOPIXEL, PIN_STRIP_2>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
	#if !DEBUG
	// tell FastLED there's 60 NEOPIXEL leds on pin 12, starting at index 120 in the led array
	FastLED.addLeds<NEOPIXEL, PIN_STRIP_3>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
	#endif
  delay(200);
}

void loop() {
  Serial.println("mdr");
	if (button.uniquePress()) {
    anim(CRGB(0, 199, 183));
    Serial.println("LOL");
    anim(CRGB(0, 0, 0));
    HTTPClient http;
    http.begin("http://api-naoled.cleverapps.io/STAIRS");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST("{\"name\":\"STANNAH\"}");
    Serial.println(httpCode);

    // StaticJsonBuffer<300> JSONbuffer;
    // JsonObject& listData = JSONbuffer.createObject();
    // listData["name"] = "STANNAH";
    // sendData(listData);
	}
  delay(50);
}

void anim(CRGB led_color) {

  uint8_t center_1 = 29;
  uint8_t center_2 = 89;
  uint8_t center_3 = 149;

  uint16_t i;

  for (i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    if (i < 15) {
      leds[center_1 + i] = led_color;
      leds[center_1 - i] = led_color;
    } else if (i >= 15 && i < 30) {
      leds[center_1 + i] = led_color;
      leds[center_1 - i] = led_color;
      leds[center_2 + i - 15] = led_color;
      leds[center_2 - i + 15] = led_color;
    } else if (i >= 30 && i < 45) {
      leds[center_2 + i - 15] = led_color;
      leds[center_2 - i + 15] = led_color;
      leds[center_3 + i - 30] = led_color;
      leds[center_3 - i + 30] = led_color;
    } else if (i >= 45 && i < 60) {
      leds[center_3 + i - 30] = led_color;
      leds[center_3 - i + 30] = led_color;
    }
    FastLED.show();

    delay(10);
  }
}

// void sendData(JsonObject& listData) {
//   if (WiFi.status() == WL_CONNECTED) {
//     //Declare object of class HTTPClient
//     HTTPClient http;
//     // Prettier data
//     char JSONmessageBuffer[300];
//     listData.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
//     // connect api
//     http.begin("http://api-naoled.cleverapps.io/STAIRS");
//     http.addHeader("Content-Type", "application/json");
//     // send data
//     int httpCode = http.POST(JSONmessageBuffer);
//     // <0 if error
//     if(httpCode > 0){
//       Serial.printf("[HTTP] POST... code: %d\n", httpCode);
//       String payload = http.getString();
//       // HTTP return code
//       Serial.println(httpCode);
//       // request response payload
//       Serial.println(payload);
//     }else{
//       Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
//     }
//     http.end();
//     Serial.print("Message envoyé");
//   }else{
//     Serial.printf("[HTTP} Internet not found\n");
//   }
// }