#include "potentiometers.h"
#include <Arduino.h>
#include "lights.h"

Potentiometers::Potentiometers() {
    
}

/////////
// SETUP
/////////
void potentiometersSetup() {};

/////////
// LOOP
/////////
void potentiometersLoop()
{
  uint8_t brightness = map(analogRead(POTEN_3), 0, 1023, 10, MAX_BRIGHTNESS);
  demoLights.setBrightness(brightness);
};