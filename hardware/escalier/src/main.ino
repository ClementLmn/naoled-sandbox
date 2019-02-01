#include <SPI.h>
#include <FastLED.h>
#include <Button.h>

const String id = "ELSA";

#define PIN_STRIP_1 2
#define PIN_STRIP_2 14
#define PIN_STRIP_3 7

#define NUM_STRIPS 3
#define NUM_LEDS_PER_STRIP 60
#define NUM_LEDS NUM_LEDS_PER_STRIP *NUM_STRIPS

CRGB leds[NUM_LEDS];
const int center_1 = 29;
const int center_2 = 89;
const int center_3 = 149;

Button button = Button(11, INPUT_PULLUP);

void setup()
{
    Serial.begin(115200);
    // tell FastLED there's 60 NEOPIXEL leds on pin 2, starting at index 0 in the led array
    FastLED.addLeds<NEOPIXEL, PIN_STRIP_1>(leds, 0, NUM_LEDS_PER_STRIP);
    // tell FastLED there's 60 NEOPIXEL leds on pin 14, starting at index 60 in the led array
    FastLED.addLeds<NEOPIXEL, PIN_STRIP_2>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
    // tell FastLED there's 60 NEOPIXEL leds on pin 17, starting at index 120 in the led array
    FastLED.addLeds<NEOPIXEL, PIN_STRIP_3>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
    delay(200);
    clear();
}

void loop()
{
    if (button.uniquePress())
    {
        anim(CRGB(0, 199, 183));
        anim(CRGB(0, 0, 0));
    }
    delay(100);
}

void anim(CRGB led_color)
{
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++)
    {
        if (i < 15)
        {
            leds[center_1 + i] = led_color;
            leds[center_1 - i] = led_color;
        }
        else if (i >= 15 && i < 30)
        {
            leds[center_1 + i] = led_color;
            leds[center_1 - i] = led_color;
            leds[center_2 + i - 15] = led_color;
            leds[center_2 - i + 15] = led_color;
        }
        else if (i >= 30 && i < 45)
        {
            leds[center_2 + i - 15] = led_color;
            leds[center_2 - i + 15] = led_color;
            leds[center_3 + i - 30] = led_color;
            leds[center_3 - i + 30] = led_color;
        }
        else if (i >= 45 && i < 60)
        {
            leds[center_3 + i - 30] = led_color;
            leds[center_3 - i + 30] = led_color;
        }
        FastLED.show();
        delay(10);
    }
}

void clear()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB(0, 0, 0);
    }
    FastLED.show();
}
