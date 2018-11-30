#include <Button.h>
#include <Adafruit_NeoPixel.h>

// NAME : STANNAH

#define DEBUG 0
#define PIN_STRIP_1 D10
#define PIN_STRIP_2 D6
#define NUM_LED 8

#if DEBUG
#define NUM_STRIPS 2
#else
#define PIN_STRIP_3 D2
#define NUM_STRIPS 3
#endif

#define NUM_LEDS_PER_STRIP 60
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS

CRGB leds[NUM_LEDS];

Button button = Button(D3, INPUT_PULLUP);

void setup() {
	Serial.begin(115200);
	// tell FastLED there's 60 NEOPIXEL leds on pin 10, starting at index 0 in the led array
	FastLED.addLeds<NEOPIXEL, PIN_STRIP_1>(leds, 0, NUM_LEDS_PER_STRIP);
	// tell FastLED there's 60 NEOPIXEL leds on pin 11, starting at index 60 in the led array
	FastLED.addLeds<NEOPIXEL, PIN_STRIP_2>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
	#if !DEBUG
	// tell FastLED there's 60 NEOPIXEL leds on pin 12, starting at index 120 in the led array
	FastLED.addLeds<NEOPIXEL, PIN_STRIP_3>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
	#endif
}

void loop() {
	if (button.uniquePress()) {
    anim(60, 8, CRGB(0, 199, 183));
    anim(60, 8, CRGB(0, 0, 0));
	}
}

void anim(uint8_t num_led, uint8_t wave_size, CRGB led_color) {

  uint8_t center_1 = 29;
  uint8_t center_2 = 89;
  uint8_t center_3 = 149;

  uint16_t i, j;

  for (i = 0; i < NUM_LEDS_PER_STRIP + wave_size / 2; i++) {
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

  for(j=0; j<wave_size; j++) {
    strip.setPixelColor(j + 1, strip.Color(0, 0, 0));
    strip.setPixelColor(total-j+1, strip.Color(0, 0, 0));
    strip.show();
  }
  delay(wait);
}