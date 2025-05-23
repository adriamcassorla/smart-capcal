#ifndef LIGHTS_H
#define LIGHTS_H

#include <Arduino.h>
#include <FastLED.h>

#define CHPSET SK9822
#define COLOR_ORDER BGR
#define VOLTS 5
#define MAX_AMPS 30000

#define MAX_BRIGHTNESS 255
#define DEFAULT_BRIGHTNESS 200

#define NUM_LEDS_AMBIENT 192 // 120 are AMBIENT and 72 are DIORAMA
#define NUM_LEDS_READING 144
#define NUM_LEDS_TOP 107

#define NUM_LEDS_SIDE_AMBIENT 60
#define NUM_LEDS_DIORAMA 73 // Initialised as part of AMBIENT
#define DIORAMA_FIRST_LED NUM_LEDS_SIDE_AMBIENT
#define LEFT_AMBIENT_FIRST_LED NUM_LEDS_SIDE_AMBIENT + NUM_LEDS_DIORAMA

#define DATA_PIN_TOP 6
#define CLOCK_PIN_TOP 7
#define DATA_PIN_READING 28
#define CLOCK_PIN_READING 27
#define CLOCK_PIN_AMBIENT 12
#define DATA_PIN_AMBIENT 11

#define MAX_BRIGHTNESS_DIFFERENCE 8 // Over 256

#define SECOND 1000
#define INITIAL_DELAY 2 * SECOND

#define LIGHTS_WATCHDOG_INTERVAL 10 // minutes

extern CRGB readingLeds[NUM_LEDS_READING * 2];
extern CRGB topLeds[NUM_LEDS_TOP];
extern CRGB ambientLeds[NUM_LEDS_AMBIENT];

extern class Light readingLeft;
extern class Light readingRight;
extern class Light topLight;
extern class Light ambientLight;

class Light {
public:
  Light(struct CRGB *array, uint8_t length);
  void toggle();
  void reset();
  bool getIsOn();

private:
  struct CRGB *Leds;
  uint8_t numLeds;
  bool isOn;
};

void lightsSetup();
void lightsLoop();
void lightsReset();
void lightsWatchdog();

#endif // LIGHTS_H