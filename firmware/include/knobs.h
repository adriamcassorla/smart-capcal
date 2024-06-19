#ifndef KNOBS_H
#define KNOBS_H

#include "lights.h"
#include <Arduino.h>

#define POTEN_1 A1
#define POTEN_2 A0
#define POTEN_3 A17
#define POTEN_4 A16

#define NUM_KNOBS 2

#define KNOB_RESOLUTION 10                 // Over 1024
#define MAX_GAP 50                         // Over 1024
#define SECONDARY_LIGHT_MAX_TIME 30 * 1000 // 30 seconds

extern uint8_t leftKnobPins[NUM_KNOBS];
extern uint8_t rightKnobPins[NUM_KNOBS];
extern class MultiKnob leftKnobs;
extern class MultiKnob rightKnobs;

typedef void (*knobCallback_t)(uint16_t value, void *);
struct MultiKnobCallbackData {
  MultiKnob *instance;
  int pinId;
};

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

class MultiKnob {
public:
  MultiKnob(uint8_t *pinNumbers, ReadingLight *light);
  void setup();
  void poll();

private:
  static int pinIds[NUM_KNOBS];
  Knob *knobs[NUM_KNOBS];
  ReadingLight *readingLight;
  elapsedMillis lastInteraction[NUM_KNOBS];

  static void callback(uint16_t value, void *callbackData);
};

void knobsSetup();
void knobsLoop();

#endif // KNOBS_H