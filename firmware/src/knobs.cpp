#include "knobs.h"
#include "lights.h"
#include "math.h"
#include <Arduino.h>

Knob::Knob(uint8_t pinNumber, uint8_t stepResolution = 2)
    : pin(pinNumber), value(0), resolution(stepResolution) {};

void Knob::poll() {
  uint16_t read = analogRead(pin);
  if (abs(read - value) < resolution) return;
  else {
    value = read;
    if (callback) callback(value, callbackParam);
  }
};

void Knob::setCallback(knobCallback_t cb, void *param) {
  callback = cb;
  callbackParam = param;
}

uint16_t Knob::getValue() { return value; }

Knobs::Knobs() {}

/////////
// SETUP
/////////
void callback(uint16_t value, void *) {
  long mappedValue = map(value, 0, 1023, 0, MAX_BRIGHTNESS);
  uint8_t brightness = static_cast<uint8_t>(mappedValue);
  ambientLight.setBrightness(brightness);
}

Knob testKnob(POTEN_3);
void knobsSetup() { testKnob.setCallback(&callback); };

/////////
// LOOP
/////////
void knobsLoop() { testKnob.poll(); };