#include "switches.h"
#include "lights.h"
#include "avdweb_Switch.h"

Switches::Switches(uint8_t *p, int *id) : pins(p), pinIds(id)
{
  for (uint8_t i = 0; i < NUM_SWITCHES; ++i)
  {
    toggleSwitches[i] = new Switch(p[i]);
  }
}

void Switches::setup()
{
  for (uint8_t i = 0; i < NUM_SWITCHES; ++i)
  {
    toggleSwitches[i]->setPushedCallback(&callback, (void *)&pinIds[i]);
    toggleSwitches[i]->setReleasedCallback(&callback, (void *)&pinIds[i]);
  }
}

void Switches::poll()
{
  for (uint8_t i = 0; i < NUM_SWITCHES; ++i)
  {
    toggleSwitches[i]->poll();
  }
}

void Switches::callback(void *s)
{
  int *switchIndex = (int *)s; // converts s to int pointer (int *)

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

  switch ((*switchIndex))
  {
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

/////////
// SETUP
/////////

// Define the pin numbers for the switches
uint8_t switchPins[] = {25, 31, 27, 28, 29, 30};
int switchPinsIds[] = {0, 1, 2, 3, 4, 5};
Switches switches(switchPins, switchPinsIds);

void switchesSetup()
{
  switches.setup();
}

////////
// LOOP
////////
void switchesLoop()
{
  switches.poll();
}
