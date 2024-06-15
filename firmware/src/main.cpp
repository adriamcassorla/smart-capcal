#define FASTLED_FORCE_SOFTWARE_SPI

#include <Arduino.h>
#include <FastLED.h>

#include "lights.h"
#include "switches.h"
#include "potentiometers.h"

#define FRAMES_PER_SECOND 24
#define INITIAL_DELAY 2000

void setup() {
  // sanity check delay
  delay(INITIAL_DELAY);

  // Internal parts setup
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

