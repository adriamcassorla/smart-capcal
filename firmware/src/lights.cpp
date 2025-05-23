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
    : Leds(array), numLeds(length), isOn(false) {}

void Light::toggle() {
  // Stores the last value when switching off
  isOn = !isOn;
  Serial.println(isOn ? "on" : "off");

  CRGB::HTMLColorCode color = isOn ? CRGB::Goldenrod : CRGB::Black;
  for (uint16_t i = 0; i < numLeds; ++i)
    Leds[i] = color;
  FastLED.show();
}

void Light::reset() {
  isOn = false;
  fill_solid(Leds, numLeds, CRGB::Black);
  FastLED.show();
}

bool Light::getIsOn() {
  return isOn;
}

/////////
// SETUP
/////////

CRGB readingLeds[NUM_LEDS_READING * 2];
CRGB topLeds[NUM_LEDS_TOP];
CRGB ambientLeds[NUM_LEDS_AMBIENT];

// These instances are only used for spot lights
Light readingLeft(readingLeds + NUM_LEDS_READING, NUM_LEDS_READING);
Light readingRight(readingLeds, NUM_LEDS_READING);
Light topLight(topLeds, NUM_LEDS_TOP);
Light ambientLight(ambientLeds, NUM_LEDS_AMBIENT);

void lightsSetup() {
  FastLED.addLeds<CHPSET, DATA_PIN_READING, CLOCK_PIN_READING, COLOR_ORDER>(
      readingLeds, NUM_LEDS_READING * 2
  );
  FastLED.addLeds<CHPSET, DATA_PIN_TOP, CLOCK_PIN_TOP, COLOR_ORDER>(
      topLeds, NUM_LEDS_TOP
  );
  FastLED.addLeds<CHPSET, DATA_PIN_AMBIENT, CLOCK_PIN_AMBIENT, COLOR_ORDER>(
      ambientLeds, NUM_LEDS_AMBIENT
  );

  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
};

void lightsReset() {
  readingLeft.reset();
  readingRight.reset();
  topLight.reset();
  ambientLight.reset();

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
