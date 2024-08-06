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

  // Only calls the callback when the difference is greater than the resolution
  // Also calls it at the extreme values
  if (read != value && (abs(read - value) > resolution || read == MIN_VALUE ||
                        read == MAX_VALUE)) {
    value = read;
    if (callback) callback(value, callbackParam);
  }
};

void Knob::setCallback(knobCallback_t cb, void *param) {
  callback = cb;
  callbackParam = param;
}

/////
// MultiKnob Implementation
/////
MultiKnob::MultiKnob(uint8_t *pinNumbers, ReadingLight *light)
    : readingLight(light) {
  for (uint8_t i = 0; i < NUM_KNOBS; ++i)
    knobs[i] = new Knob(pinNumbers[i], KNOB_RESOLUTION);
}

int MultiKnob::pinIds[NUM_KNOBS] = {0, 1};

void MultiKnob::setup() {
  for (uint8_t i = 0; i < NUM_KNOBS; ++i) {
    MultiKnobCallbackData *callbackData =
        new MultiKnobCallbackData{this, pinIds[i]};
    knobs[i]->setCallback(&callback, (void *)callbackData);
  }
}

void MultiKnob::poll() {
  for (uint8_t i = 0; i < NUM_KNOBS; ++i)
    knobs[i]->poll();
}

void MultiKnob::callback(uint16_t value, void *callbackData) {
  // Cast the callbackData to MultiSwitchCallbackData
  MultiKnobCallbackData *data =
      static_cast<MultiKnobCallbackData *>(callbackData);

  // Access own instance of MultiSwitch and the pinId
  MultiKnob *multiKnob = data->instance;
  if (!multiKnob) return;

  int pinId = data->pinId;

  // Determines which knob controls what
  bool isReadingInUse = multiKnob->readingLight->getIsOn();

  switch (pinId) {
    case 0:
      // This knob controls the reading light when is in use
      // Otherwise, controls the ambient light
      if (isReadingInUse) {
        uint8_t brightness =
            map(value, MIN_VALUE, MAX_VALUE, READING_MIN_VALUE, MAX_BRIGHTNESS);
        multiKnob->readingLight->setBrightness(brightness, KNOB_ANIMATION_TIME);
      } else {
        uint8_t brightness =
            map(value, MIN_VALUE, MAX_VALUE, MIN_VALUE, MAX_BRIGHTNESS);
        ambientLight.setBrightness(brightness, KNOB_ANIMATION_TIME);
      }

      break;
    case 1:
      // This knob controls the ambient light light when reading is not in use
      // Otherwise, will control ceiling light
      if (isReadingInUse) {
        uint8_t brightness =
            map(value, MIN_VALUE, MAX_VALUE, MIN_VALUE, MAX_BRIGHTNESS);
        ambientLight.setBrightness(brightness, KNOB_ANIMATION_TIME);
      } else {
        // TODO: Call Shelly API
      }
      break;
    default: break;
  }
}

/////////
// SETUP
/////////

uint8_t leftKnobPins[] = {POTEN_1, POTEN_2};
uint8_t rightKnobPins[] = {POTEN_3, POTEN_4};
MultiKnob leftKnobs(leftKnobPins, &readingLeft);
MultiKnob rightKnobs(rightKnobPins, &readingRight);

void knobsSetup() {
  rightKnobs.setup();
};

/////////
// LOOP
/////////
void knobsLoop() {
  rightKnobs.poll();
};