#include "lights.h"
#include "palettes.h"
#include <Arduino.h>
#include <FastLED.h>

/////
// Utils
/////
void fillColourWithBrightness(
    struct CRGB *ledsArray,
    uint16_t length,
    uint8_t brightness,
    uint8_t hue = WARM_WHITE_HUE,
    uint8_t saturation = WARM_WHITE_SAT)
{
  for (uint16_t i = 0; i < length; ++i)
  {
    ledsArray[i] = CHSV(hue, saturation, brightness);
  }
}

/////
// Reading Light Implementation
/////
ReadingLight::ReadingLight(struct CRGB *array, uint8_t length, bool reverse)
    : readingLeds(array),
      numLeds(length),
      isOn(false),
      brightness(DEFAULT_BRIGHTNESS),
      isReversed(reverse)
{
}

void ReadingLight::toggle() {
  isOn = !isOn;
  brightness = isOn ? DEFAULT_BRIGHTNESS : 0;
  fillColourWithBrightness(readingLeds, numLeds, brightness);
  FastLED.show();
}

void ReadingLight::setBrightness(uint8_t value) {
  brightness = value;
}

void ReadingLight::reset() {
  isOn = false;
  brightness = DEFAULT_BRIGHTNESS;
}

/////
// Ambient Light Implementation
/////
AmbientLight::AmbientLight(LightSection *sections, uint8_t length)
    : lightSections(sections),
      numSections(length),
      isOn(false),
      brightness(DEFAULT_BRIGHTNESS)
{
}

void AmbientLight::toggle() {
  isOn = !isOn;
  brightness = isOn ? DEFAULT_BRIGHTNESS : 0;
  // fillColourWithBrightness(ambientLeft, numAmbient, brightness);
  // fillColourWithBrightness(top, numTop, brightness);
  FastLED.show();
}

void AmbientLight::setBrightness(uint8_t value) {
  if (value != brightness)
  {
    brightness = value;
    if (isOn)
    {
      mapNewBrightness(brightness);
    }
  }
}

void AmbientLight::mapNewBrightness(uint8_t newBrightness)
{
  FastLED.show();
}

void AmbientLight::reset() {
  isOn = false;
  brightness = MAX_BRIGHTNESS;
}

/////
// Demo Lights Implementation
/////
DemoLights::DemoLights(LightSection *sections, uint8_t length)
    : isOn(false),
      lightSections(sections),
      numSections(length),
      brightness(DEFAULT_BRIGHTNESS),
      activeMode(Mode::Rainbow)
{
}

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
      chromoteraphy_beat();
      break;

    default:
      break;
    }
  }
}

void DemoLights::rainbow_beat() {
  uint8_t beatA = beatsin8(9, 0, brightness);
  uint8_t beatB = beatsin8(13, 0, brightness);
  for (uint8_t n = 0; n < NUM_SECTIONS; n++)
  {
    fill_rainbow(
        lightSections[n].ledsArray,
        lightSections[n].length,
        (beatA + beatB) / 2,
        lightSections[n].config.maxBrightness / lightSections[n].length);
  }
}

void DemoLights::chromoteraphy_beat()
{
  for (uint8_t n = 0; n < NUM_SECTIONS; n++)
  {
    applyRandomPalette(
        lightSections[n].ledsArray,
        topPalette,
        lightSections[n].length,
        100,
        brightness * 0.2,
        lightSections[n].config.maxBrightness);
  }
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

// Section configurations
SectionConfig ambientConfig = {
  lowerBound : 30,
  upperBound : MAX_BRIGHTNESS,
  maxBrightness : MAX_BRIGHTNESS,
};
SectionConfig topConfig = {
  lowerBound : 100,
  upperBound : MAX_BRIGHTNESS,
  maxBrightness : MAX_BRIGHTNESS
};
SectionConfig dioramaConfig = {
  lowerBound : MAX_BRIGHTNESS / 2,
  upperBound : MAX_BRIGHTNESS,
  maxBrightness : MAX_BRIGHTNESS / 2,
};
SectionConfig readingConfig = {
  lowerBound : 200,
  upperBound : MAX_BRIGHTNESS,
  maxBrightness : MAX_BRIGHTNESS / 2,
};

// Sections initialisation
LightSection ambientLeft = {
  ledsArray : ambientLeds + LEFT_AMBIENT_FIRST_LED,
  length : NUM_LEDS_SIDE_AMBIENT - 1, // Problems with soldering...
  config : ambientConfig
};
LightSection ambientRight = {
  ledsArray : ambientLeds,
  length : NUM_LEDS_SIDE_AMBIENT,
  config : ambientConfig
};
LightSection top = {
  ledsArray : topLeds,
  length : NUM_LEDS_TOP,
  config : topConfig
};
LightSection diorama = {
  ledsArray : ambientLeds + DIORAMA_FIRST_LED,
  length : NUM_LEDS_DIORAMA,
  config : dioramaConfig
};
LightSection reading = {
  ledsArray : readingLeds,
  length : NUM_LEDS_READING,
  config : readingConfig
};

// Assignation to the main sections array
LightSection lightSections[NUM_SECTIONS] = {ambientLeft, ambientRight, top, diorama, reading};

AmbientLight ambientLight(lightSections, NUM_SECTIONS);
DemoLights demoLights(lightSections, NUM_SECTIONS);

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
