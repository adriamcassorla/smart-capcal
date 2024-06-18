#define FASTLED_FORCE_SOFTWARE_SPI

#include <Arduino.h>
#include <FastLED.h>

#include "lights.h"
#include "switches.h"
#include "knobs.h"

#define SECOND 1000
#define INITIAL_DELAY 2 * SECOND
#define FRAMES_PER_SECOND 24
#define ANIMATION_INTERVAL SECOND / FRAMES_PER_SECOND
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
  EVERY_N_MILLISECONDS(POLL_INTERVAL)
  {
    switchesLoop();
    knobsLoop();
  }

  EVERY_N_MILLISECONDS(ANIMATION_INTERVAL)
  {
    lightsLoop();
  }
}

