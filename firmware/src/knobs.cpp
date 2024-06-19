#include "knobs.h"
#include "lights.h"
#include "math.h"
#include <Arduino.h>

/////
// Single Knob Implementation
/////
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

/////
// MultiKnob Implementation
/////
MultiKnob::MultiKnob(uint8_t *pinNumbers, ReadingLight *light)
    : readingLight(light) {
  for (uint8_t i = 0; i < NUM_KNOBS; ++i) {
    knobs[i] = new Knob(pinNumbers[i]);
    lastInteraction[i] = 0;
  }
}

void MultiKnob::setup() {
  for (uint8_t i = 0; i < NUM_KNOBS; ++i) {
    MultiKnobCallbackData *callbackData =
        new MultiKnobCallbackData{this, pinIds[i]};
    knobs[i]->setCallback(&callback, (void *)callbackData);
  }
}

void MultiKnob::poll() {
  for (uint8_t i = 0; i < NUM_KNOBS; ++i) {
    knobs[i]->poll();
  }
}

void MultiKnob::callback(uint16_t value, void *callbackData) {
  // Cast the callbackData to MultiSwitchCallbackData
  MultiKnobCallbackData *data =
      static_cast<MultiKnobCallbackData *>(callbackData);

  // Access own instance of MultiSwitch and the pinId
  MultiKnob *multiKnob = data->instance;
  if (!multiKnob) { return; }

  int pinId = data->pinId;
  bool isReadingInUse = multiKnob->readingLight->getIsOn();

  switch (pinId) {
  case 0:
    // This knob controls the reading light when is in use
    // Otherwise, controls the ambient light
    if (isReadingInUse) {
      multiKnob->readingLight->setBrightness(value);
    } else {
      ambientLight.setBrightness(value);
    }

    break;
  case 1:
    // This knob controls the ambient light light when reading is not in use
    // Otherwise, will control ceiling light
    if (isReadingInUse) {
      multiKnob->readingLight->setBrightness(value);
    } else {
      // TODO: Call Shelly API
    }
    break;
  default:
    break;
  }
}

/////////
// SETUP
/////////
void callback(uint16_t value, void *) {
  long mappedValue = map(value, 0, 1023, 0, MAX_BRIGHTNESS);
  uint8_t brightness = static_cast<uint8_t>(mappedValue);
  ambientLight.setBrightness(brightness);
}

Knob testKnob(POTEN_3, DEFAULT_RESOLUTION);
void knobsSetup() { testKnob.setCallback(&callback); };

/////////
// LOOP
/////////
void knobsLoop() { testKnob.poll(); };