#include "lights.h"
#include <Arduino.h>
#include <FastLED.h>

ReadingLight::ReadingLight(struct CRGB *array, uint8_t length) {
  ledsArray = array;
  numLeds = length;
}

void ReadingLight::toggle(bool reverse) {
     isOn = !isOn;

      if (isOn) {
        for (uint16_t i = numLeds - 1; i > numLeds / 2; --i) {
          ledsArray[i] = CRGB::Goldenrod;
        }
      } else {
        for (uint16_t i = numLeds / 2; i < numLeds - 1; ++i) {
          ledsArray[i] = CRGB::Black;
        }
      }
      FastLED.show();
}