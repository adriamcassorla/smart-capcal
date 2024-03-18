#include <Arduino.h>

#define FASTLED_FORCE_SOFTWARE_SPI
#include <avdweb_Switch.h>
#include <FastLED.h>

#define CHPSET SK9822
#define COLOR_ORDER BGR
#define VOLTS 5
#define MAX_AMPS 30000
#define FRAMES_PER_SECOND 24

#define NUM_LEDS_AMBIENT 193
#define NUM_LEDS_READING 288
#define NUM_LEDS_TOP 107

#define CLOCK_PIN_AMBIENT 13 // SCK
#define DATA_PIN_AMBIENT 11 // MOSI
#define CLOCK_PIN_TOP 27 // SCK1
#define DATA_PIN_TOP 26 // MOSI1
#define CLOCK_PIN_READING 49 // SCK2
#define DATA_PIN_READING 50 // MOSI2

#define POTEN_1 A1
#define POTEN_2 A0
#define POTEN_3 A17
#define POTEN_4 A16

#define SENSOR_L A15
#define SENSOR_R A14


CRGB ambientLeds[NUM_LEDS_AMBIENT];
CRGB readingLeds[NUM_LEDS_READING];
CRGB topLeds[NUM_LEDS_TOP];

// Define the pin numbers for your switches
const uint8_t switchPins[] = { 25, 31, 27, 28, 29, 30 };
const int switchPinsIds[] = { 0, 1, 2, 3, 4, 5 };
const uint8_t numSwitches = 6;

boolean isRainbowPlaying = false;
boolean isTopOn = false;
boolean isLeftOn = false;
boolean isRightOn = false;
boolean isTrenOn = false;

Switch switch0 = Switch(switchPins[0]);
Switch switch1 = Switch(switchPins[1]);
Switch switch2 = Switch(switchPins[2]);
Switch switch3 = Switch(switchPins[3]);
Switch switch4 = Switch(switchPins[4]);
Switch switch5 = Switch(switchPins[5]);

Switch toggleSwitches[numSwitches] = { switch0, switch1, switch2, switch3, switch4, switch5 };

////// PALETTES /////

// Gradient palette "bhw1_07_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_07.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

DEFINE_GRADIENT_PALETTE(trenPaletteSrc){
  0, 229, 227, 1,
  100, 255, 184, 83,
  200, 255, 255, 125,
  255, 255, 184, 83
};
CRGBPalette16 trenPalette = trenPaletteSrc;

void toggleCallbackFunction(void *s) {
  int *switchIndex = (int *)s;  // convert s to int pointer (int *)
    switch ((*switchIndex)) {
    case 0:
      FastLED.clear();
      isRainbowPlaying = !isRainbowPlaying;
      break;
    case 1:
    case 4:
      isTopOn = !isTopOn;
      if (isTopOn) {
        fill_solid(topLeds, NUM_LEDS_TOP, CRGB::Goldenrod);
      } else {
        fill_solid(topLeds, NUM_LEDS_TOP, CRGB::Black);
      }
      FastLED.show();
      break;
    case 5:
      isTrenOn = !isTrenOn;
      break;
    case 2:
      isLeftOn = !isLeftOn;
      if (isLeftOn) {
        for (uint16_t i = NUM_LEDS_READING - 1; i > NUM_LEDS_READING / 2; --i) {
          readingLeds[i] = CRGB::Goldenrod;
        }
      } else {
        for (uint16_t i = NUM_LEDS_READING / 2; i < NUM_LEDS_READING - 1; ++i) {
          readingLeds[i] = CRGB::Black;
        }
      }
      FastLED.show();
      break;
    case 3:
      isRightOn = !isRightOn;
      if (isRightOn) {
        for (uint8_t i = 0; i < NUM_LEDS_READING / 2; ++i) {
          readingLeds[i] = CRGB::Goldenrod;
        }
      } else {
        for (uint8_t i = NUM_LEDS_READING / 2; i > 0; --i) {
          readingLeds[i] = CRGB::Black;
        }
        readingLeds[0] = CRGB::Black;
      }
      FastLED.show();
      break;
    default:
      break;
  }
}

void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);

  FastLED.addLeds<CHPSET, DATA_PIN_AMBIENT, CLOCK_PIN_AMBIENT, COLOR_ORDER>(ambientLeds, NUM_LEDS_AMBIENT);
  FastLED.addLeds<CHPSET, DATA_PIN_READING, CLOCK_PIN_READING, COLOR_ORDER>(readingLeds, NUM_LEDS_READING);
  FastLED.addLeds<CHPSET, DATA_PIN_TOP, CLOCK_PIN_TOP, COLOR_ORDER>(topLeds, NUM_LEDS_TOP);


  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(255);
  FastLED.show();


  for (uint8_t i = 0; i < numSwitches; ++i) {
    toggleSwitches[i].setPushedCallback(&toggleCallbackFunction, (void *)&switchPinsIds[i]);
    toggleSwitches[i].setReleasedCallback(&toggleCallbackFunction, (void *)&switchPinsIds[i]);
  }
}

void rainbow_beat() {
  // Starting hue
  uint8_t beatA = beatsin8(9, 0, 255);
  uint8_t beatB = beatsin8(13, 0, 255);
  fill_rainbow(ambientLeds, NUM_LEDS_AMBIENT, (beatA + beatB) / 2, 255 / NUM_LEDS_AMBIENT);
  fill_rainbow(readingLeds, NUM_LEDS_READING, (beatA + beatB) / 2, NUM_LEDS_READING / 255);
  fill_rainbow(topLeds, NUM_LEDS_TOP, (beatA + beatB) / 2, 255 / NUM_LEDS_TOP);
}

void applyRandomPalette(struct CRGB *targetArray, CRGBPalette16 &pal, int numLeds, int indexScale, int minBrightness, int maxBrightness) {
  for (int i = 0; i < numLeds; i++) {
    uint8_t brightness = inoise8(i, millis() / 30);
    uint16_t index = inoise16(i * indexScale, millis() / 20);

    targetArray[i] = ColorFromPalette(
      pal,
      constrain(index, 0, numLeds - 1),
      constrain(brightness, minBrightness, maxBrightness));
  }
}

void loop() {
  EVERY_N_MILLISECONDS(50) {
    for (uint8_t i = 0; i < numSwitches; ++i) {
      toggleSwitches[i].poll();
    }
  }

  EVERY_N_MILLISECONDS(1000 / FRAMES_PER_SECOND) {
    if (isRainbowPlaying) {
      rainbow_beat();
    }
    if (isTrenOn || isRainbowPlaying) {
      FastLED.show();
    }
  }
}