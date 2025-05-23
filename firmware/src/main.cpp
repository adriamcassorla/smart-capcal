
#include <Arduino.h>
#include <FastLED.h>

#include "lights.h"
#include "switches.h"

#define POLL_INTERVAL 50

/////////
// SETUP
/////////
void setup() {
  // sanity check delay
  delay(INITIAL_DELAY);

  // Internal parts setup
  lightsSetup();
  switchesSetup();
}

//////////
// LOOP
//////////
void loop() {
  EVERY_N_MILLISECONDS(POLL_INTERVAL) {
    switchesLoop();
  }

  EVERY_N_MINUTES(LIGHTS_WATCHDOG_INTERVAL) {
    lightsWatchdog();
  }
}
