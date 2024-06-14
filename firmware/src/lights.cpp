#include "lights.h"
#include "palettes.h"
#include <Arduino.h>
#include <FastLED.h>

/////
// Reading Light Implementation
/////
ReadingLight::ReadingLight(struct CRGB *array, uint8_t length) 
    : readingLeds(array), numLeds(length), isOn(false), brightness(255) {
  // Initially, turn off all the LEDs
  for (uint8_t i = 0; i < numLeds; ++i) {
      readingLeds[i] = CRGB::Black;
  }
}

void ReadingLight::toggle(bool reverse) {
  isOn = !isOn;
  
  CRGB::HTMLColorCode color = isOn ? CRGB::Goldenrod : CRGB::Black;
  for (uint16_t i = numLeds - 1; i > numLeds / 2; --i) {
    readingLeds[i] = color;
  }
}

void ReadingLight::setBrightness(uint8_t value) {
  brightness = value;
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
      top(topArray), 
      numAmbient(ambientLength), 
      numTop(topLength), 
      isOn(false), 
      brightness(255) {
    
  // Initially, turn off all the LEDs
  for (uint8_t i = 0; i < numAmbient; ++i) {
    ambient[i] = CRGB::Black;
  }
  for (uint8_t i = 0; i < numTop; ++i) {
    top[i] = CRGB::Black;
  }
}

void AmbientLight::toggle() {
  isOn = !isOn;

  CRGB::HTMLColorCode color = isOn ? CRGB::Goldenrod : CRGB::Black;
  for (uint8_t i = 0; i < numAmbient; ++i) {
    ambient[i] = color;
  }
  for (uint8_t i = 0; i < numTop; ++i) {
    top[i] = color;
  }
}

void AmbientLight::setBrightness(uint8_t value) {
  brightness = value;
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
      uint8_t readingLength
    )
    : ambient(ambientArray), 
      top(topArray), 
      numAmbient(ambientLength), 
      numTop(topLength), 
      reading(readingArray), 
      numReading(readingLength), 
      isOn(false), 
      brightness(255),
      activeMode(Mode::Rainbow)
      {}

void DemoLights::setBrightness(uint8_t value) {
  brightness = value;
}

void DemoLights::setMode(Mode mode) {
  activeMode = mode;
}

void DemoLights::toggle() {
  isOn = !isOn;
  FastLED.clear();
}

void DemoLights::stop() {
  isOn = false;
  FastLED.clear();
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
  fill_rainbow(ambient, numAmbient, (beatA + beatB) / 2, 255 / numAmbient);
  fill_rainbow(reading, numReading, (beatA + beatB) / 2, numReading / 255);
  fill_rainbow(top, numTop, (beatA + beatB) / 2, 255 / numTop);
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
