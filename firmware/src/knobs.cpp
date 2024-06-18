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
  uint8_t brightness = analogRead(POTEN_3);
  demoLights.setBrightness(brightness);
  ambientLight.setBrightness(brightness);
};