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
  int potenRead = analogRead(POTEN_3);
  long mappedValue = map(potenRead, 0, 1023, 0, MAX_BRIGHTNESS);
  uint8_t brightness = static_cast<uint8_t>(mappedValue);
  demoLights.setBrightness(brightness);
  ambientLight.setBrightness(brightness);
};