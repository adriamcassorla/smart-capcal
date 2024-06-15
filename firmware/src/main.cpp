#define FASTLED_FORCE_SOFTWARE_SPI

#include <Arduino.h>
#include <FastLED.h>
#include "lights.h"
#include "switches.h"

#define FRAMES_PER_SECOND 24
#define INITIAL_DELAY 2000

#define POTEN_1 A1
#define POTEN_2 A0
#define POTEN_3 A17
#define POTEN_4 A16

#define SENSOR_L A15
#define SENSOR_R A14

void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(INITIAL_DELAY);

  // Setup internal parts
  setupLights();
  switches.setup();
}

void loop() {
  EVERY_N_MILLISECONDS(50) {
    switches.poll();
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

