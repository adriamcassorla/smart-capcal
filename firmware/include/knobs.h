#ifndef KNOBS_H
#define KNOBS_H

#include <Arduino.h>

#define POTEN_1 A1
#define POTEN_2 A0
#define POTEN_3 A17
#define POTEN_4 A16

#define NUM_KNOBS 2

typedef void (*knobCallback_t)(uint16_t value, void *);

class Knob {
public:
  Knob(uint8_t pinNumber, uint8_t stepResolution);
  void poll();
  void setCallback(knobCallback_t cb, void *param = nullptr);
  uint16_t getValue();

private:
  uint8_t pin;
  uint16_t value;
  uint8_t resolution;

  knobCallback_t callback;
  void *callbackParam = nullptr;
};

class Knobs {
public:
  Knobs();
  //   void setup();
  //   void poll();

private:
  //   static int pinIds[NUM_KNOBS];
  //   Switch *toggleSwitches[NUM_KNOBS];
  //   ReadingLight *readingLight;
  //   enum DemoLights::Mode demoMode;
  //   static void callback(void *callbackData);
};

void knobsSetup();
void knobsLoop();

#endif // KNOBS_H