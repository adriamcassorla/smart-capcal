#define FASTLED_FORCE_SOFTWARE_SPI

#include <Arduino.h>
#include <avdweb_Switch.h>
#include <FastLED.h>

#define CHPSET SK9822
#define COLOR_ORDER BGR
#define VOLTS 5
#define MAX_AMPS 30000
#define FRAMES_PER_SECOND 24

#define NUM_LEDS_AMBIENT 192
#define NUM_LEDS_READING 288
#define NUM_LEDS_TOP 107
#define NUM_SWITCHES 6

#define CLOCK_PIN_AMBIENT 0 // 13 SCK
#define DATA_PIN_AMBIENT 1 // 11 MOSI
#define CLOCK_PIN_TOP  10// 27 SCK1
#define DATA_PIN_TOP 11 // 26 MOSI1
#define CLOCK_PIN_READING 6 // 49 SCK2
#define DATA_PIN_READING 7 // 50 MOSI2

#define POTEN_1 A1
#define POTEN_2 A0
#define POTEN_3 A17
#define POTEN_4 A16

#define SENSOR_L A15
#define SENSOR_R A14


CRGB ambientLeds[NUM_LEDS_AMBIENT];
CRGB readingLeds[NUM_LEDS_READING];
CRGB topLeds[NUM_LEDS_TOP];

// Define the pin numbers for the switches
const uint8_t switchPins[] = { 25, 31, 27, 28, 29, 30 };
const int switchPinsIds[] = { 0, 1, 2, 3, 4, 5 };

boolean isRainbowPlaying = false;
boolean isTopOn = false;
boolean isLeftOn = false;
boolean isRightOn = false;
boolean isChromotherapyOn = false;

Switch switch0 = Switch(switchPins[0]);
Switch switch1 = Switch(switchPins[1]);
Switch switch2 = Switch(switchPins[2]);
Switch switch3 = Switch(switchPins[3]);
Switch switch4 = Switch(switchPins[4]);
Switch switch5 = Switch(switchPins[5]);

Switch toggleSwitches[NUM_SWITCHES] = { switch0, switch1, switch2, switch3, switch4, switch5 };

// Gradient palette "cw2_046_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/cw/2/tn/cw2-046.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE(darkGreenPalette){
  0, 4, 12, 7,
  127, 9, 40, 6,
  255, 45, 67, 5
};

// Gradient palette "Naturel_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/colo/smorin2002/tn/Naturel.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

DEFINE_GRADIENT_PALETTE(lightGreenPalette){
  0, 13, 62, 7,
  51, 13, 62, 7,
  51, 19, 68, 7,
  102, 19, 68, 7,
  102, 32, 77, 9,
  153, 32, 77, 9,
  153, 58, 79, 9,
  204, 58, 79, 9,
  204, 90, 80, 13,
  255, 90, 80, 13
};

CRGBPalette16 bottomPalette = darkGreenPalette;
CRGBPalette16 topPalette = lightGreenPalette;

void toggleCallbackFunction(void *s) {
  int *switchIndex = (int *)s;  // converts s to int pointer (int *)
    
    // Turns off animations for regular lights
    if ((*switchIndex) != 0 && (*switchIndex) != 5) {
      isChromotherapyOn = false;
      isRainbowPlaying = false;
      FastLED.clear();
    }

    switch ((*switchIndex)) {
    case 0:
      isChromotherapyOn = false;
      isRainbowPlaying = !isRainbowPlaying;
      if (!isRainbowPlaying) {
        FastLED.clear();
      }
      break;
    case 1:
    case 4:
      isTopOn = !isTopOn;
      if (isTopOn) {
        fill_solid(topLeds, NUM_LEDS_TOP, CRGB::Goldenrod);
      } else {
        fill_solid(topLeds, NUM_LEDS_TOP, CRGB::Black);
      }
      break;
    case 5:
      isChromotherapyOn = !isChromotherapyOn;
      isRainbowPlaying = false;
      if (!isChromotherapyOn)
      {
        FastLED.clear();
      }
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
      break;
    default:
      break;
  }
  FastLED.show();
}

//////////////
// Helprers
//////////////

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

void chromotherapy(int maxBrightness) {
  applyRandomPalette(ambientLeds, bottomPalette, NUM_LEDS_AMBIENT, 75, maxBrightness * 0.2, maxBrightness);
  applyRandomPalette(readingLeds, topPalette, NUM_LEDS_READING, 10, maxBrightness * 0.2, maxBrightness * 0.7);
  applyRandomPalette(topLeds, topPalette, NUM_LEDS_TOP, 100, maxBrightness * 0.2, maxBrightness);
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


  for (uint8_t i = 0; i < NUM_SWITCHES; ++i) {
    toggleSwitches[i].setPushedCallback(&toggleCallbackFunction, (void *)&switchPinsIds[i]);
    toggleSwitches[i].setReleasedCallback(&toggleCallbackFunction, (void *)&switchPinsIds[i]);
  }
}

void loop() {
  EVERY_N_MILLISECONDS(50) {
    for (uint8_t i = 0; i < NUM_SWITCHES; ++i) {
      toggleSwitches[i].poll();
    }
  }

  EVERY_N_MILLISECONDS(1000 / FRAMES_PER_SECOND) {
    if (isRainbowPlaying) {
      rainbow_beat();
    }
    if (isChromotherapyOn) {
      uint8_t brightness = map(analogRead(POTEN_3), 0, 1023, 10, 255);
      chromotherapy(brightness);
    }
    if (isRainbowPlaying || isChromotherapyOn) {
      FastLED.show();
    }
  }
}

