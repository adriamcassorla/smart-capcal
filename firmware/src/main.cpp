#include <Arduino.h>

#define FASTLED_FORCE_SOFTWARE_SPI
#include <avdweb_Switch.h>
#include <FastLED.h>

#define CHPSET SK9822
#define COLOR_ORDER BGR
#define VOLTS 5
#define MAX_AMPS 30000
#define FRAMES_PER_SECOND 24

#define NUM_LEDS_INFERIOR 120
#define NUM_LEDS_LECTURA 288
#define NUM_LEDS_TOP 107
#define NUM_LEDS_TREN 73

#define DATA_PIN_INFERIOR 1
#define CLOCK_PIN_INFERIOR 0
#define DATA_PIN_LECTURA 7
#define CLOCK_PIN_LECTURA 6
#define DATA_PIN_TOP 11
#define CLOCK_PIN_TOP 10
#define DATA_PIN_TREN 5
#define CLOCK_PIN_TREN 4

#define POTEN_1 A1
#define POTEN_2 A0
#define POTEN_3 A17
#define POTEN_4 A16

#define SENSOR_L A15
#define SENSOR_R A14


CRGB ledsInferior[NUM_LEDS_INFERIOR];
CRGB ledsLectura[NUM_LEDS_LECTURA];
CRGB ledsTop[NUM_LEDS_TOP];
CRGB ledsTren[NUM_LEDS_TREN];

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

void regularLightSwitcher(int switchIndex) {
  
}

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
        fill_solid(ledsTop, NUM_LEDS_TOP, CRGB::Goldenrod);
      } else {
        fill_solid(ledsTop, NUM_LEDS_TOP, CRGB::Black);
        if ((*switchIndex) == 1 && isTrenOn) {
          isTrenOn = false;
          fill_solid(ledsTren, NUM_LEDS_TREN, CRGB::Black);
        }
      }
      FastLED.show();
      break;
    case 5:
      isTrenOn = !isTrenOn;
      if (!isTrenOn) {
        fill_solid(ledsTren, NUM_LEDS_TREN, CRGB::Black);
        FastLED.show();
      }
      break;
    case 2:
      isLeftOn = !isLeftOn;
      if (isLeftOn) {
        for (uint16_t i = NUM_LEDS_LECTURA - 1; i > NUM_LEDS_LECTURA / 2; --i) {
          ledsLectura[i] = CRGB::Goldenrod;
        }
      } else {
        for (uint16_t i = NUM_LEDS_LECTURA / 2; i < NUM_LEDS_LECTURA - 1; ++i) {
          ledsLectura[i] = CRGB::Black;
        }
      }
      FastLED.show();
      break;
    case 3:
      isRightOn = !isRightOn;
      if (isRightOn) {
        for (uint8_t i = 0; i < NUM_LEDS_LECTURA / 2; ++i) {
          ledsLectura[i] = CRGB::Goldenrod;
        }
      } else {
        for (uint8_t i = NUM_LEDS_LECTURA / 2; i > 0; --i) {
          ledsLectura[i] = CRGB::Black;
        }
        ledsLectura[0] = CRGB::Black;
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

  FastLED.addLeds<CHPSET, DATA_PIN_INFERIOR, CLOCK_PIN_INFERIOR, COLOR_ORDER>(ledsInferior, NUM_LEDS_INFERIOR);
  FastLED.addLeds<CHPSET, DATA_PIN_LECTURA, CLOCK_PIN_LECTURA, COLOR_ORDER>(ledsLectura, NUM_LEDS_LECTURA);
  FastLED.addLeds<CHPSET, DATA_PIN_TOP, CLOCK_PIN_TOP, COLOR_ORDER>(ledsTop, NUM_LEDS_TOP);
  FastLED.addLeds<CHPSET, DATA_PIN_TREN, CLOCK_PIN_TREN, COLOR_ORDER>(ledsTren, NUM_LEDS_TREN);


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
  fill_rainbow(ledsInferior, NUM_LEDS_INFERIOR, (beatA + beatB) / 2, 255 / NUM_LEDS_INFERIOR);
  fill_rainbow(ledsLectura, NUM_LEDS_LECTURA, (beatA + beatB) / 2, NUM_LEDS_LECTURA / 255);
  fill_rainbow(ledsTop, NUM_LEDS_TOP, (beatA + beatB) / 2, 255 / NUM_LEDS_TOP);
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

void maquetaSuau() {
  applyRandomPalette(ledsTren, trenPalette, NUM_LEDS_TREN, 100, 50, 80);
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
    if (isTrenOn) {
      maquetaSuau();
    }
    if (isTrenOn || isRainbowPlaying) {
      FastLED.show();
    }
  }
}