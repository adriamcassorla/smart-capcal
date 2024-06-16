#ifndef SWITCHES_H
#define SWITCHES_H

#include <avdweb_Switch.h>
#include "lights.h"
#include <Arduino.h>

#define NUM_SWITCHES 3

extern uint8_t leftPins[NUM_SWITCHES];
extern uint8_t rightPins[NUM_SWITCHES];
extern class MultiSwitch leftSwitches;
extern class MultiSwitch rightSwitches;

struct MultiSwitchCallbackData
{
  MultiSwitch *instance;
  int pinId;
};

class MultiSwitch
{
public:
  MultiSwitch(uint8_t *pinNumbers, ReadingLight *light);
  void setup();
  void poll();

private:
  static int pinIds[NUM_SWITCHES];
  Switch *toggleSwitches[NUM_SWITCHES];
  ReadingLight *readingLight;
  static void callback(void *callbackData);
};

void switchesSetup();
void switchesLoop();

#endif // SWITCHES_H