#define FASTLED_FORCE_SOFTWARE_SPI

#include <Arduino.h>
#include <FastLED.h>
#include <avdweb_Switch.h>
#include "lights.h"

#define FRAMES_PER_SECOND 24
#define INITIAL_DELAY 2000

#define NUM_SWITCHES 6

#define POTEN_1 A1
#define POTEN_2 A0
#define POTEN_3 A17
#define POTEN_4 A16

#define SENSOR_L A15
#define SENSOR_R A14

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

void toggleCallbackFunction(void *s) {
  int *switchIndex = (int *)s;  // converts s to int pointer (int *)

  // Turns off animations and resets all other lights
  if (demoLights.isOn && (*switchIndex) != 0 && (*switchIndex) != 5)
  {
    demoLights.stop();
  }
  else if ((*switchIndex) == 0 || (*switchIndex) == 5)
  {
    ambientLight.reset();
    readingLeft.reset();
    readingRight.reset();
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
      readingLeft.toggle();
      break;
    case 3:
      readingRight.toggle();
      break;
    default:
      break;
  }
}

void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(INITIAL_DELAY);

  // Setup internal parts
  setupLights();

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
    uint8_t brightness = map(analogRead(POTEN_3), 0, 1023, 10, MAX_BRIGHTNESS);
    demoLights.setBrightness(brightness);
  }

  EVERY_N_MILLISECONDS(1000 / FRAMES_PER_SECOND) {
    demoLights.loop();
    if (demoLights.isOn) {
      FastLED.show();
    }
  }
}

