#include <FastLED.h>
#define NUM_LEDS 256
#define DATA_PIN 5

CRGB leds[NUM_LEDS];
CRGB ledColor = CRGB(15,0,8);

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  for (int i = 0 ; i < NUM_LEDS ; i++)
  {
     leds[i]= ledColor;
  }
  FastLED.show();
}
