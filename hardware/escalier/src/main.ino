#include <Button.h>
#include <Adafruit_NeoPixel.h>

// NAME : STANNAH

#define PIN_STRIP 3

// How many leds in your strip?
#define NUM_LEDS 240
#define MAX_LEDS 240

Button button = Button(12, INPUT_PULLUP);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_STRIP, NEO_GRB + NEO_KHZ800);

void setup() {
	Serial.begin(115200);
  strip.begin();
  strip.setBrightness(50);
  strip.show();
}

void loop() {
  if(button.uniquePress()){
    Serial.println("pressed");
    boom(strip.Color(255, 0, 0), 1000, MAX_LEDS, 5);
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