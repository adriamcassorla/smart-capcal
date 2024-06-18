#include "knobs.h"
#include "lights.h"
#include <Arduino.h>

Knobs::Knobs() {}

/////////
// SETUP
/////////
void knobsSetup() {};

/////////
// LOOP
/////////
void knobsLoop() {
  uint8_t brightness =
      map(analogRead(POTEN_3), 0, 1023, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  demoLights.setBrightness(brightness);
  ambientLight.setBrightness(brightness);
};