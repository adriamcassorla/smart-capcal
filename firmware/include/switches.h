#ifndef SWITCHES_H
#define SWITCHES_H

#include "lights.h"
#include <Arduino.h>
#include <avdweb_Switch.h>

#define SWITCH_1 21
#define SWITCH_2 22
#define SWITCH_3 23
#define SWITCH_4 14
#define SWITCH_5 15
#define SWITCH_6 16

#define NUM_SWITCHES 3

#define SWITCHES_GROUND_PIN 18

extern uint8_t leftPins[NUM_SWITCHES];
extern uint8_t rightPins[NUM_SWITCHES];
extern class MultiSwitch leftSwitches;
extern class MultiSwitch rightSwitches;

struct MultiSwitchCallbackData {
  MultiSwitch *instance;
  int pinId;
};

class MultiSwitch {
public:
  MultiSwitch(
      uint8_t *pinNumbers, ReadingLight *light, enum DemoLights::Mode mode
  );
  void setup();
  void poll();

private:
  static int pinIds[NUM_SWITCHES];
  Switch *toggleSwitches[NUM_SWITCHES];
  ReadingLight *readingLight;
  enum DemoLights::Mode demoMode;
  static void callback(void *callbackData);
};

void switchesSetup();
void switchesLoop();

#endif // SWITCHES_H