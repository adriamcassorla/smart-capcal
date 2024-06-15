#ifndef SWITCHES_H
#define SWITCHES_H

#include <avdweb_Switch.h>
#include <Arduino.h>

#define NUM_SWITCHES 6

extern uint8_t switchPins[NUM_SWITCHES];
extern int switchPinsIds[NUM_SWITCHES];
extern class Switches switches;
class Switches {
public:
  Switches(uint8_t *p, int *ids);
  void setup();
  void poll();

private:
  uint8_t *pins;
  int *pinIds;
  Switch *toggleSwitches[NUM_SWITCHES];
  static void callback(void *s);
};

void switchesSetup();
void switchesLoop();

#endif // SWITCHES_H