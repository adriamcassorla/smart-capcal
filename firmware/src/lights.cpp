#include <algorithm>
#include <Arduino.h>
#include <cmath>
#include <FastLED.h>

#include "lights.h"
#include "palettes.h"

/////
// Reading Light Implementation
/////
Light::Light(struct CRGB *array, uint8_t length)
    : Leds(array), numLeds(length), isOn(false), brightness(0) {}

void Light::toggle() {
  // Stores the last value when switching off
  isOn = !isOn;
  brightness = isOn ? DEFAULT_BRIGHTNESS : 0;
  processBrightnessChange();
}

void Light::reset() {
  isOn = false;
  brightness = 0;
  processBrightnessChange();
}

bool Light::getIsOn() {
  return isOn;
}

void Light::processBrightnessChange() {
  fill_solid(Leds, numLeds, CRGB::DarkSalmon);
  FastLED.show();
}

/////////
// SETUP
/////////

CRGB readingLeds[NUM_LEDS_READING * 2];
CRGB topLeds[NUM_LEDS_TOP];

// These instances are only used for spot lights
Light readingLeft(readingLeds + NUM_LEDS_READING, NUM_LEDS_READING);
Light readingRight(readingLeds, NUM_LEDS_READING);
Light topLight(readingLeds, NUM_LEDS_TOP);

void lightsSetup() {
  FastLED.addLeds<CHPSET, DATA_PIN_READING, CLOCK_PIN_READING, COLOR_ORDER>(
      readingLeds, NUM_LEDS_READING * 2
  );
  FastLED.addLeds<CHPSET, DATA_PIN_TOP, CLOCK_PIN_TOP, COLOR_ORDER>(
      topLeds, NUM_LEDS_TOP
  );

  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
};

//////////
// LOOP
//////////

void lightsReset() {
  readingLeft.reset();
  readingRight.reset();
  topLight.reset();

  FastLED.clear();
  FastLED.show();
}

//////////
// WATCHDOG

//////////

// Returns early if any light is currently on.
// Otherwise, clears the strips
void lightsWatchdog() {
  if (readingLeft.getIsOn() || readingRight.getIsOn() | topLight.getIsOn()) {
    return;
  } else {
    FastLED.clear();
    FastLED.show();
  }
};
