#include <Button.h>
#include <Adafruit_NeoPixel.h>

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
  for(i=0; i<total / 2; i++) {
    strip.setPixelColor(center-i, c);
    strip.setPixelColor(center+i, c);
    strip.setPixelColor(center-i+wave_size, strip.Color(0, 0, 0));
    strip.setPixelColor(center+i-wave_size, strip.Color(0, 0, 0));
    strip.show();
    delay(50);
  }

  for(j=0; j<wave_size; j++) {
    strip.setPixelColor(j + 1, strip.Color(0, 0, 0));
    strip.setPixelColor(total-j+1, strip.Color(0, 0, 0));
    strip.show();
  }
  delay(wait);
}



void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}