
#include <Arduino.h>
#include <FastLED.h>

#include "knobs.h"
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
  knobsSetup();
}

//////////
// LOOP
//////////
void loop() {
  EVERY_N_MILLISECONDS(POLL_INTERVAL) {
    switchesLoop();
    knobsLoop();
  }

  EVERY_N_MILLISECONDS(ANIMATION_INTERVAL) {
    lightsLoop();
  }
}
