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
  lightsSetup();
  switchesSetup();
  potentiometersSetup();
}

void loop() {
  EVERY_N_MILLISECONDS(50) {
    switchesLoop();
    potentiometersLoop();
  }

  EVERY_N_MILLISECONDS(1000 / FRAMES_PER_SECOND) {
    lightsLoop();
  }
}

