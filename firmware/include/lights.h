#ifndef LIGHTS_H
#define LIGHTS_H

#include <Arduino.h>
#include <FastLED.h>

#define CHPSET SK9822
#define COLOR_ORDER BGR
#define VOLTS 5
#define MAX_AMPS 30000

#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 20
#define DEFAULT_BRIGHTNESS 218
#define WARM_WHITE_HUE 30
#define WARM_WHITE_SAT 200
#define WARM_WHITE_SAT_COMPENSATION 30
#define READING_MIN_VALUE 80

#define NUM_LEDS_AMBIENT 192 // 120 are AMBIENT and 72 are DIORAMA
#define NUM_LEDS_READING 144
#define NUM_LEDS_TOP 107

#define NUM_SECTIONS 7
#define NUM_LEDS_SIDE_AMBIENT 60
#define NUM_LEDS_DIORAMA 73 // Initialised as part of AMBIENT
#define DIORAMA_FIRST_LED NUM_LEDS_SIDE_AMBIENT
#define LEFT_AMBIENT_FIRST_LED NUM_LEDS_SIDE_AMBIENT + NUM_LEDS_DIORAMA
#define NUM_LEDS_HALF_TOP 54

#define DATA_PIN_TOP 6       
#define CLOCK_PIN_TOP 7     
#define DATA_PIN_READING 28  
#define CLOCK_PIN_READING 27 
#define CLOCK_PIN_AMBIENT 12 
#define DATA_PIN_AMBIENT 11 

#define MAX_BRIGHTNESS_DIFFERENCE 8 // Over 256

#define LIGHTS_WATCHDOG_INTERVAL 10 // minutes

extern CRGB topLeds[NUM_LEDS_TOP];
extern CRGB readingLeds[NUM_LEDS_READING * 2];
extern CRGB ambientLeds[NUM_LEDS_AMBIENT];

extern class ReadingLight readingLeft;
extern class ReadingLight readingRight;
extern class AmbientLight ambientLight;
extern class DemoLights demoLights;

struct SectionConfig {
  uint8_t lowerBound = MIN_BRIGHTNESS;
  uint8_t upperBound = MAX_BRIGHTNESS;
  uint8_t minBrightness = MIN_BRIGHTNESS;
  uint8_t maxBrightness = MAX_BRIGHTNESS;
  uint16_t firstLedOffset = 0; // Previous leds will be on from the start
  uint16_t lastLedOffset = 0;  // Substracted from the total length
  int hueOffset = 0;
  int satOffset = 0;
};

struct LightSection {
  struct CRGB *ledsArray;
  uint16_t length;
  struct SectionConfig *config;
  bool mirror = false;
};

class ReadingLight {
public:
  ReadingLight(struct CRGB *array, uint8_t length, bool reverse);
  void toggle();
  void refresh();
  void reset();
  bool getIsOn();
  void setBrightness(uint8_t value);

private:
  struct CRGB *readingLeds;
  uint8_t numLeds;

  bool isOn;
  uint8_t brightness;
  uint8_t lastBrightness;
  bool isReversed;

  void applyNewBrightness();
};

class AmbientLight {
public:
  AmbientLight(LightSection *lightSections, uint16_t length);
  void toggle();
  void refresh();
  void reset();
  bool getIsOn();
  void setBrightness(uint8_t value);

private:
  struct LightSection *lightSections;
  uint8_t numSections;
  bool isOn;
  uint8_t brightness;
  uint8_t lastBrightness;

  void applyNewBrightness();
};

class DemoLights {
public:
  DemoLights(LightSection *lightSections, uint16_t length);
  enum Mode {
    Rainbow,
    Chromotherapy,
  };

  void toggle();
  void stop();
  bool getIsOn();
  void setBrightness(uint8_t value);
  void setMode(Mode mode);
  void loop();

private:
  struct LightSection *lightSections;
  uint8_t numSections;

  uint8_t brightness;
  Mode activeMode;
  bool isOn;

  void applyRandomPalette(
      struct CRGB *targetArray, CRGBPalette16 &pal, uint16_t numLeds,
      uint8_t indexScale, uint8_t minBrightness, uint8_t maxBrightness
  );
  void rainbow_beat();
  void chromoteraphy_beat();
};

void lightsSetup();
void lightsLoop();
void lightsWatchdog();

#endif // LIGHTS_H