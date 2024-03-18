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
boolean isChromotherapyOn = false;
boolean useChromotherapy = false;

Switch switch0 = Switch(switchPins[0]);
Switch switch1 = Switch(switchPins[1]);
Switch switch2 = Switch(switchPins[2]);
Switch switch3 = Switch(switchPins[3]);
Switch switch4 = Switch(switchPins[4]);
Switch switch5 = Switch(switchPins[5]);

Switch toggleSwitches[numSwitches] = { switch0, switch1, switch2, switch3, 
switch4, switch5 };

////// PALETTES /////

// Gradient palette "bhw1_07_gp", originally from
// 
http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_07.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

DEFINE_GRADIENT_PALETTE(trenPaletteSrc){
  0, 229, 227, 1,
  100, 255, 184, 83,
  200, 255, 255, 125,
  255, 255, 184, 83
};

// Gradient palette "bhw1_19_gp", originally from
// 
http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_19.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE(darkRedPalette){
  0, 82, 1, 3,
  79, 20, 1, 1,
  181, 139, 1, 0,
  252, 20, 1, 1,
  255, 20, 1, 1
};

// Gradient palette "rgi_13_gp", originally from
// 
http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/rgi/tn/rgi_13.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE(lightRedPalette){
  0, 133, 46, 2,
  127, 130, 14, 4,
  255, 100, 1, 8
};

// Gradient palette "bhw2_thanks_gp", originally from
// 
http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_thanks.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE(darkOrangePalette){
  0, 9, 5, 1,
  48, 25, 9, 1,
  76, 137, 27, 1,
  96, 98, 42, 1,
  124, 144, 79, 1,
  153, 98, 42, 1,
  178, 137, 27, 1,
  211, 23, 9, 1,
  255, 9, 5, 1
};

// Gradient palette "bhw1_23_gp", originally from
// 
http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_23.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE(lightOrangePalette){
  0, 83, 6, 1,
  84, 194, 52, 1,
  122, 182, 105, 34,
  165, 194, 52, 1,
  255, 83, 6, 1
};

// Gradient palette "bhw3_02_gp", originally from
// 
http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_02.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE(darkYellowPalette){
  0, 255, 57, 1,
  112, 255, 79, 25,
  145, 255, 79, 25,
  255, 244, 146, 3
};

// Gradient palette "63undercom_gp", originally from
// 
http://soliton.vm.bytemark.co.uk/pub/cpt-city/colo/evad/tn/63undercom.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

DEFINE_GRADIENT_PALETTE(lightYellowPalette){
  0, 255, 86, 1,
  102, 255, 86, 1,
  153, 255, 117, 1,
  204, 252, 193, 1,
  255, 255, 233, 64
};

// Gradient palette "cw2_046_gp", originally from
// 
http://soliton.vm.bytemark.co.uk/pub/cpt-city/cw/2/tn/cw2-046.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE(darkGreenPalette){
  0, 4, 12, 7,
  127, 9, 40, 6,
  255, 45, 67, 5
};

// Gradient palette "Naturel_gp", originally from
// 
http://soliton.vm.bytemark.co.uk/pub/cpt-city/colo/smorin2002/tn/Naturel.png.index.html
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


CRGBPalette16 trenPalette = trenPaletteSrc;
CRGBPalette16 bottomPalette;
CRGBPalette16 topPalette;
CRGBPalette16 lightPalettes[] = { lightYellowPalette, darkOrangePalette, 
lightRedPalette, lightGreenPalette };
CRGBPalette16 darkPalettes[] = { lightYellowPalette, darkOrangePalette, 
darkRedPalette, darkGreenPalette };
uint8_t currentPaletteIndex = 0;

unit8_t MAX_NIGHT_BRIGHTNESS = 50;


// void blackout {
//   fill_solid(ledsTop, NUM_LEDS_TOP, CRGB::Black);
//   fill_solid(ledsInferior, NUM_LEDS_INFERIOR, CRGB::Black);
//   fill_solid(ledsLectura, NUM_LEDS_LECTURA, CRGB::Black);
//   fill_solid(ledsTren, NUM_LEDS_TREN, CRGB::Black);
//   FastLED.show();
//   isLeftOn = false;
//   isRightOn = false;
//   isTopOn = false;
//   isTrenOn = false;
//   isChromotherapyOn = false;
// }

void regularLightSwitcher(int switchIndex) {
  switch (switchIndex) {
    case 0:
      useChromotherapy = !useChromotherapy;
      FastLED.clear();
      break;
    case 1:
    case 4:
      isTopOn = !isTopOn;
      if (isTopOn) {
        fill_solid(ledsTop, NUM_LEDS_TOP, CRGB::Goldenrod);
      } else {
        fill_solid(ledsTop, NUM_LEDS_TOP, CRGB::Black);
        if (switchIndex == 1 && isTrenOn) {
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
        for (uint16_t i = NUM_LEDS_LECTURA - 1; i > NUM_LEDS_LECTURA / 2; 
--i) {
          ledsLectura[i] = CRGB::Goldenrod;
        }
      } else {
        for (uint16_t i = NUM_LEDS_LECTURA / 2; i < NUM_LEDS_LECTURA - 1; 
++i) {
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

const uint8_t OFFSET = 2;
void chromotherapySwitcher(int switchIndex) {
  uint8_t newPaletteIndex;

  switch (switchIndex) {
    case 0:
      useChromotherapy = !useChromotherapy;
      FastLED.clear();
      break;
    case 1:
      isRainbowPlaying = !isRainbowPlaying;
      if (!isRainbowPlaying) {
        FastLED.clear();
      }
      break;
    case 2:
    case 3:
    case 4:
    case 5:
      newPaletteIndex = switchIndex - OFFSET;
      if (isChromotherapyOn) {
        if (currentPaletteIndex == newPaletteIndex) {
          isChromotherapyOn = false;
          FastLED.clear();
        } else {
          currentPaletteIndex = newPaletteIndex;
          bottomPalette = darkPalettes[currentPaletteIndex];
          topPalette = lightPalettes[currentPaletteIndex];
        }
      } else {
        isChromotherapyOn = true;
        currentPaletteIndex = newPaletteIndex;
        bottomPalette = darkPalettes[currentPaletteIndex];
        topPalette = lightPalettes[currentPaletteIndex];
      }
      break;
    default:
      break;
  }
}



void toggleCallbackFunction(void *s) {
  int *switchIndex = (int *)s;  // convert s to int pointer (int *)
  if (useChromotherapy) {
    chromotherapySwitcher((*switchIndex));
  } else {
    regularLightSwitcher((*switchIndex));
  }
}

void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power 
w/leds
  delay(2000);

  FastLED.addLeds<CHPSET, DATA_PIN_INFERIOR, CLOCK_PIN_INFERIOR, 
COLOR_ORDER>(ledsInferior, NUM_LEDS_INFERIOR);
  FastLED.addLeds<CHPSET, DATA_PIN_LECTURA, CLOCK_PIN_LECTURA, 
COLOR_ORDER>(ledsLectura, NUM_LEDS_LECTURA);
  FastLED.addLeds<CHPSET, DATA_PIN_TOP, CLOCK_PIN_TOP, 
COLOR_ORDER>(ledsTop, NUM_LEDS_TOP);
  FastLED.addLeds<CHPSET, DATA_PIN_TREN, CLOCK_PIN_TREN, 
COLOR_ORDER>(ledsTren, NUM_LEDS_TREN);


  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(255);
  FastLED.show();


  for (uint8_t i = 0; i < numSwitches; ++i) {
    toggleSwitches[i].setPushedCallback(&toggleCallbackFunction, (void 
*)&switchPinsIds[i]);
    toggleSwitches[i].setReleasedCallback(&toggleCallbackFunction, (void 
*)&switchPinsIds[i]);
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
    if (isTrenOn) {
      maquetaSuau();
    }
    if (isChromotherapyOn) {
      uint8_t brightness = map(analogRead(POTEN_3), 0, 1023, 10, 255);
      chromotherapy(brightness);
    }
    if (isTrenOn || isRainbowPlaying || isChromotherapyOn) {
      FastLED.show();
    }
  }
}


void rainbow_beat() {
  // Starting hue
  uint8_t beatA = beatsin8(9, 0, 255);
  uint8_t beatB = beatsin8(13, 0, 255);
  fill_rainbow(ledsInferior, NUM_LEDS_INFERIOR, (beatA + beatB) / 2, 255 / 
NUM_LEDS_INFERIOR);
  fill_rainbow(ledsLectura, NUM_LEDS_LECTURA, (beatA + beatB) / 2, 
NUM_LEDS_LECTURA / 255);
  fill_rainbow(ledsTop, NUM_LEDS_TOP, (beatA + beatB) / 2, 255 / 
NUM_LEDS_TOP);
}

void applyRandomPalette(struct CRGB *targetArray, CRGBPalette16 &pal, int 
numLeds, int indexScale, int minBrightness, int maxBrightness) {
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

void chromotherapy(int maxBrightness) {
  applyRandomPalette(ledsInferior, bottomPalette, NUM_LEDS_INFERIOR, 75, 
maxBrightness * 0.2, maxBrightness);
  applyRandomPalette(ledsLectura, topPalette, NUM_LEDS_LECTURA, 10, 
maxBrightness * 0.2, maxBrightness * 0.7);
  applyRandomPalette(ledsTop, topPalette, NUM_LEDS_TOP, 100, maxBrightness 
* 0.2, maxBrightness);
  applyRandomPalette(ledsTren, bottomPalette, NUM_LEDS_TREN, 100, 
maxBrightness * 0.2, maxBrightness * 0.8);
}

uint8_t tempNightBrightness = 0;
uint32_t maxBrightnessTimestamp = 0;
void animateNightLight(bool isLeftSide) {
  if (tempNightBrightness < MAX_NIGHT_BRIGHTNESS) {
    tempNightBrightness++;
  } else {
    tempNightBrightness--;
  }

  for (uint8_t n=90; n<NUM_LEDS_INFERIOR; n++){
    ledsInferior[n] = blend(CRGB::Black, CRGB::DarkGoldenrod, 
tempNightBrightness);
  }

}
