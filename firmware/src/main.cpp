#define FASTLED_FORCE_SOFTWARE_SPI

#include <Arduino.h>
#include <avdweb_Switch.h>
#include <FastLED.h>
#include "lights.h"

#define CHPSET SK9822
#define COLOR_ORDER BGR
#define VOLTS 5
#define MAX_AMPS 30000
#define FRAMES_PER_SECOND 24

#define NUM_LEDS_AMBIENT 192
#define NUM_LEDS_READING 144
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
CRGB readingLeds[NUM_LEDS_READING * 2]; // Two strips
CRGB topLeds[NUM_LEDS_TOP];

// Define the pin numbers for the switches
const uint8_t switchPins[] = { 25, 31, 27, 28, 29, 30 };
const int switchPinsIds[] = { 0, 1, 2, 3, 4, 5 };

Switch switch0 = Switch(switchPins[0]);
Switch switch1 = Switch(switchPins[1]);
Switch switch2 = Switch(switchPins[2]);
Switch switch3 = Switch(switchPins[3]);
Switch switch4 = Switch(switchPins[4]);
Switch switch5 = Switch(switchPins[5]);

Switch toggleSwitches[NUM_SWITCHES] = { switch0, switch1, switch2, switch3, switch4, switch5 };

ReadingLight readingLeft(readingLeds + NUM_LEDS_READING, NUM_LEDS_READING);
ReadingLight readingRight(readingLeds, NUM_LEDS_READING);
AmbientLight ambientLight(ambientLeds, NUM_LEDS_AMBIENT, topLeds, NUM_LEDS_TOP);
DemoLights demoLights(ambientLeds, NUM_LEDS_AMBIENT, topLeds, NUM_LEDS_TOP, readingLeds, NUM_LEDS_READING * 2);

void toggleCallbackFunction(void *s) {
  int *switchIndex = (int *)s;  // converts s to int pointer (int *)
    
    // Turns off animations for regular lights
    if ((*switchIndex) != 0 && (*switchIndex) != 5) {
      demoLights.stop();
      FastLED.clear();
    }

    switch ((*switchIndex)) {
    case 0:
      demoLights.setMode(DemoLights::Mode::Rainbow);
      demoLights.toggle();
      break;
    case 1:
    case 4:
      ambientLight.toggle();
      break;
    case 5:
      demoLights.setMode(DemoLights::Mode::Chromotherapy);
      demoLights.toggle();
      break;
    case 2:
      readingLeft.toggle(false);
      break;
    case 3:
      readingRight.toggle(false);
      break;
    default:
      break;
  }
  FastLED.show();
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
    uint8_t brightness = map(analogRead(POTEN_3), 0, 1023, 10, 255);
    demoLights.setBrightness(brightness);
  }

  EVERY_N_MILLISECONDS(1000 / FRAMES_PER_SECOND) {
    demoLights.loop();
    if (demoLights.isOn) {
      FastLED.show();
    }
  }
}

