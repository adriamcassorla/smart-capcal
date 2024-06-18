#include <Arduino.h>
#include <FastLED.h>
#include <algorithm>
#include <cmath>

#include "lights.h"
#include "palettes.h"

/////
// Reading Light Implementation
/////
ReadingLight::ReadingLight(struct CRGB *array, uint8_t length, bool reverse)
    : readingLeds(array), numLeds(length), isOn(false),
      brightness(DEFAULT_BRIGHTNESS), isReversed(reverse) {}

void ReadingLight::toggle() {
  isOn = !isOn;

  brightness = isOn ? DEFAULT_BRIGHTNESS : 0;
  CHSV color = CHSV(WARM_WHITE_HUE, WARM_WHITE_SAT, brightness);
  fill_solid(readingLeds, numLeds, color);

  FastLED.show();
}

void ReadingLight::setBrightness(uint8_t value) { brightness = value; }

void ReadingLight::reset() {
  isOn = false;
  brightness = DEFAULT_BRIGHTNESS;
}

/////
// Ambient Light Implementation
/////
AmbientLight::AmbientLight(LightSection *sections, uint16_t length)
    : lightSections(sections), numSections(length), isOn(false),
      brightness(DEFAULT_BRIGHTNESS) {}

void AmbientLight::toggle() {
  isOn = !isOn;

  brightness = isOn ? MAX_BRIGHTNESS : 0;
  applyNewBrightness();
}

void AmbientLight::setBrightness(uint8_t value) {

  // Prevents big jumps when using multiple knobs / toggles
  if (value != brightness && abs8(value - brightness) > 2) {
    brightness = value;
    applyNewBrightness();
  }
}

void AmbientLight::applyNewBrightness() {
  FastLED.clear();

  for (uint8_t n = 0; n < numSections; n++) {
    if (brightness >= lightSections[n].config->lowerBound) {

      // When brightness is lower than the upperBound, maps the section
      // brightness betweeen min and max values set in the config
      uint8_t sectionBrightness =
          brightness >= lightSections[n].config->upperBound
              ? lightSections[n].config->maxBrightness
              : map(brightness, lightSections[n].config->lowerBound,
                    lightSections[n].config->upperBound,
                    lightSections[n].config->minBrightness,
                    lightSections[n].config->maxBrightness);

      // Max length of the section is given by the total length minus the offset
      uint8_t maxLength =
          lightSections[n].length - lightSections[n].config->lastLedOffset;
      // When brightness is lower than the upperBound, maps the section
      // length betweeen first led offset and max length
      bool skipLengthMapping = lightSections[n].config->firstLedOffset == 0 &&
                               lightSections[n].config->lastLedOffset == 0;
      uint8_t sectionLength =
          skipLengthMapping || brightness >= lightSections[n].config->upperBound
              ? maxLength
              : map(brightness, lightSections[n].config->lowerBound,
                    lightSections[n].config->upperBound,
                    lightSections[n].config->firstLedOffset, maxLength);

      // When the section is mirrorred, adds an offset to the array pointer
      uint16_t pOffset =
          lightSections[n].mirror ? lightSections[n].length - sectionLength : 0;

      // Creates a color with the resulting brightness and fills the section
      CHSV color = CHSV(WARM_WHITE_HUE, WARM_WHITE_SAT, sectionBrightness);
      fill_solid(lightSections[n].ledsArray + pOffset, sectionLength, color);

      // Finally blurs the whole array to create a smoother transition
      for (uint8_t i = 0; i < lightSections[n].config->blurAmount; i++) {
        blur1d(
            lightSections[n].ledsArray, lightSections[n].length,
            lightSections[n].config->blurFactor
        );
      }
    }
  }

  FastLED.show();
}

void AmbientLight::reset() {
  isOn = false;
  brightness = MAX_BRIGHTNESS;
}

/////
// Demo Lights Implementation
/////
DemoLights::DemoLights(LightSection *sections, uint16_t length)
    : isOn(false), lightSections(sections), numSections(length),
      brightness(DEFAULT_BRIGHTNESS), activeMode(Mode::Rainbow) {}

void DemoLights::toggle() {
  isOn = !isOn;
  FastLED.clear();
  FastLED.show();
}

void DemoLights::setBrightness(uint8_t value) { brightness = value; }

void DemoLights::setMode(Mode mode) { activeMode = mode; }

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
  for (uint8_t n = 0; n < numSections; n++) {
    fill_rainbow(
        lightSections[n].ledsArray, lightSections[n].length,
        (beatA + beatB) / 2,
        lightSections[n].config->maxBrightness / lightSections[n].length
    );
  }
}

void DemoLights::chromoteraphy_beat() {
  for (uint8_t n = 0; n < numSections; n++) {
    applyRandomPalette(
        lightSections[n].ledsArray, topPalette, lightSections[n].length, 100,
        brightness * 0.2,
        std::min(lightSections[n].config->maxBrightness, brightness)
    );
  }
}

void DemoLights::applyRandomPalette(
    struct CRGB *targetArray, CRGBPalette16 &pal, uint16_t numLeds,
    uint8_t indexScale, uint8_t minBrightness, uint8_t maxBrightness
) {
  for (u_int16_t i = 0; i < numLeds; i++) {
    uint8_t b = inoise8(i, millis() / 30);
    uint16_t index = inoise16(i * indexScale, millis() / 20);

    targetArray[i] = ColorFromPalette(
        pal, constrain(index, 0, numLeds - 1),
        constrain(b, minBrightness, maxBrightness)
    );
  }
}

/////////
// SETUP
/////////

CRGB ambientLeds[NUM_LEDS_AMBIENT];
CRGB readingLeds[NUM_LEDS_READING * 2];
CRGB topLeds[NUM_LEDS_TOP];

// These instances are only used for spot lights
ReadingLight
    readingLeft(readingLeds + NUM_LEDS_READING, NUM_LEDS_READING, true);
ReadingLight readingRight(readingLeds, NUM_LEDS_READING, false);

// Section configurations for ambient and demo modes
SectionConfig ambientConfig = {
  upperBound : 200,
  firstLedOffset : 30 // Starts with full floor lights on
};
SectionConfig topConfig = {
  lowerBound : 100,
  minBrightness : 80,
  firstLedOffset : 5, // Starts from the center but not from a point
};
SectionConfig dioramaConfig = {
  lowerBound : 127,
  upperBound : 215,
  maxBrightness : 80,
  blurAmount : 0,
};
SectionConfig readingConfig = {
  lowerBound : 200,
  maxBrightness : 200,
  lastLedOffset : 100,
  blurAmount : 10,
};

// Sections initialisation
LightSection ambientLeft = {
    .ledsArray = ambientLeds + LEFT_AMBIENT_FIRST_LED,
    .length = NUM_LEDS_SIDE_AMBIENT - 1, // Bad soldering -> one led missing
    .config = &ambientConfig,
    .mirror = true
};

LightSection ambientRight = {
    .ledsArray = ambientLeds,
    .length = NUM_LEDS_SIDE_AMBIENT,
    .config = &ambientConfig
};

LightSection topLeft = {
    .ledsArray = topLeds + NUM_LEDS_HALF_TOP,
    .length = NUM_LEDS_HALF_TOP,
    .config = &topConfig,
};

LightSection topRight = {
    .ledsArray = topLeds,
    .length = NUM_LEDS_HALF_TOP,
    .config = &topConfig,
    .mirror = true
};

LightSection diorama = {
    .ledsArray = ambientLeds + DIORAMA_FIRST_LED,
    .length = NUM_LEDS_DIORAMA,
    .config = &dioramaConfig
};

LightSection readingSectionLeft = {
    .ledsArray = readingLeds + NUM_LEDS_READING,
    .length = NUM_LEDS_READING,
    .config = &readingConfig,
    .mirror = true
};

LightSection readingSectionRight = {
    .ledsArray = readingLeds,
    .length = NUM_LEDS_READING,
    .config = &readingConfig
};

// Assignation to the main sections array
LightSection lightSections[NUM_SECTIONS] = {
    ambientLeft,        ambientRight,       topLeft, topRight, diorama,
    readingSectionLeft, readingSectionRight
};

AmbientLight ambientLight(lightSections, NUM_SECTIONS);
DemoLights demoLights(lightSections, NUM_SECTIONS);

void lightsSetup() {
  FastLED.addLeds<CHPSET, DATA_PIN_AMBIENT, CLOCK_PIN_AMBIENT, COLOR_ORDER>(
      ambientLeds, NUM_LEDS_AMBIENT
  );
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

void lightsLoop() {
  demoLights.loop();
  if (demoLights.isOn) { FastLED.show(); }
};
