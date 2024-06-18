#ifndef KNOBS_H
#define KNOBS_H

#define POTEN_1 A1
#define POTEN_2 A0
#define POTEN_3 A17
#define POTEN_4 A16

#define NUM_KNOBS 2

class Knobs
{
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