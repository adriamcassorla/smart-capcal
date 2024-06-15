#include "lights.h"
#include "palettes.h"
#include <Arduino.h>
#include <FastLED.h>

/////
// Reading Light Implementation
/////
ReadingLight::ReadingLight(struct CRGB *array, uint8_t length, bool reverse): 
  readingLeds(array), 
      numLeds(length),
      isOn(false), 
      brightness(MAX_BRIGHTNESS),
      isReversed(reverse)
      {}

void ReadingLight::toggle() {
  isOn = !isOn;
  
  CRGB::HTMLColorCode color = isOn ? CRGB::Goldenrod : CRGB::Black;
  for (uint16_t i = 0; i < numLeds; ++i)
  {
    readingLeds[i] = color;
  }
  FastLED.show();
}

void ReadingLight::setBrightness(uint8_t value) {
  brightness = value;
}

void ReadingLight::reset() {
  isOn = false;
  brightness = MAX_BRIGHTNESS;
}

/////
// Ambient Light Implementation
/////
AmbientLight::AmbientLight(
      struct CRGB *ambientArray, 
      uint8_t ambientLength, 
      struct CRGB *topArray, 
      uint8_t topLength
    )
    : ambient(ambientArray), 
      numAmbient(ambientLength), 
      top(topArray), 
      numTop(topLength), 
      isOn(false), 
      brightness(MAX_BRIGHTNESS) {}

void AmbientLight::toggle() {
  isOn = !isOn;

  CRGB::HTMLColorCode color = isOn ? CRGB::Goldenrod : CRGB::Black;
  for (uint8_t i = 0; i < numAmbient; ++i) {
    ambient[i] = color;
  }
  for (uint8_t i = 0; i < numTop; ++i) {
    top[i] = color;
  }
  FastLED.show();
}

void AmbientLight::setBrightness(uint8_t value) {
  brightness = value;
}

void AmbientLight::reset() {
  isOn = false;
  brightness = MAX_BRIGHTNESS;
}

/////
// Demo Lights Implementation
/////
DemoLights::DemoLights(
      struct CRGB *ambientArray, 
      uint8_t ambientLength, 
      struct CRGB *topArray, 
      uint8_t topLength,
      struct CRGB *readingArray, 
      uint16_t readingLength
    ) :
      isOn(false), 
      ambient(ambientArray), 
      numAmbient(ambientLength), 
      top(topArray), 
      numTop(topLength), 
      reading(readingArray), 
      numReading(readingLength), 
      brightness(MAX_BRIGHTNESS),
      activeMode(Mode::Rainbow)
      {}

void DemoLights::toggle() {
  isOn = !isOn;
  FastLED.clear();
  FastLED.show();
}

void DemoLights::setBrightness(uint8_t value) {
  brightness = value;
}

void DemoLights::setMode(Mode mode) {
  activeMode = mode;
}

void DemoLights::stop() {
  isOn = false;
  FastLED.clear();
  FastLED.show();
}

void DemoLights::loop() {
  if (isOn) {
    switch (activeMode) {
    case Mode::Rainbow:
      rainbow_beat();
    break;
    
    case Mode::Chromotherapy:
      applyRandomPalette(ambient, bottomPalette, numAmbient, 100, brightness * 0.2, brightness * 0.8);
      applyRandomPalette(top, topPalette, numTop, 100, brightness * 0.2, brightness);
      applyRandomPalette(reading, topPalette, numReading, 10, brightness * 0.2, brightness * 0.7);
    break;

    default:
      break;
    }
  }
}

void DemoLights::rainbow_beat() {
  uint8_t beatA = beatsin8(9, 0, brightness);
  uint8_t beatB = beatsin8(13, 0, brightness);
  fill_rainbow(ambient, numAmbient, (beatA + beatB) / 2, MAX_BRIGHTNESS / numAmbient);
  fill_rainbow(reading, numReading, (beatA + beatB) / 2, numReading / MAX_BRIGHTNESS);
  fill_rainbow(top, numTop, (beatA + beatB) / 2, MAX_BRIGHTNESS / numTop);
}

void DemoLights::applyRandomPalette(
      struct CRGB *targetArray, 
      CRGBPalette16 &pal, 
      uint16_t numLeds, 
      uint8_t indexScale, 
      uint8_t minBrightness, 
      uint8_t maxBrightness
    ) {
    for (int i = 0; i < numLeds; i++) {
    uint8_t brightness = inoise8(i, millis() / 30);
    uint16_t index = inoise16(i * indexScale, millis() / 20);

    targetArray[i] = ColorFromPalette(
      pal,
      constrain(index, 0, numLeds - 1),
      constrain(brightness, minBrightness, maxBrightness));
  }
}

/////////
// SETUP
/////////

CRGB ambientLeds[NUM_LEDS_AMBIENT];
CRGB readingLeds[NUM_LEDS_READING * 2];
CRGB topLeds[NUM_LEDS_TOP];

ReadingLight readingLeft(readingLeds + NUM_LEDS_READING, NUM_LEDS_READING, true);
ReadingLight readingRight(readingLeds, NUM_LEDS_READING, false);
AmbientLight ambientLight(ambientLeds, NUM_LEDS_AMBIENT, topLeds, NUM_LEDS_TOP);
DemoLights demoLights(ambientLeds, NUM_LEDS_AMBIENT, topLeds, NUM_LEDS_TOP, readingLeds, NUM_LEDS_READING * 2);

void lightsSetup()
{
  FastLED.addLeds<CHPSET, DATA_PIN_AMBIENT, CLOCK_PIN_AMBIENT, COLOR_ORDER>(ambientLeds, NUM_LEDS_AMBIENT);
  FastLED.addLeds<CHPSET, DATA_PIN_READING, CLOCK_PIN_READING, COLOR_ORDER>(readingLeds, NUM_LEDS_READING * 2);
  FastLED.addLeds<CHPSET, DATA_PIN_TOP, CLOCK_PIN_TOP, COLOR_ORDER>(topLeds, NUM_LEDS_TOP);

  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
};

//////////
// LOOP
//////////

void lightsLoop()
{
  demoLights.loop();
  if (demoLights.isOn)
  {
    FastLED.show();
  }
};
