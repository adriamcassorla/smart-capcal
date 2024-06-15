#ifndef LIGHTS_H
#define LIGHTS_H

#include <Arduino.h>
#include <FastLED.h>

#define CHPSET SK9822
#define COLOR_ORDER BGR
#define VOLTS 5
#define MAX_AMPS 30000
#define MAX_BRIGHTNESS 255

#define NUM_LEDS_AMBIENT 192
#define NUM_LEDS_READING 144
#define NUM_LEDS_TOP 107

#define CLOCK_PIN_AMBIENT 0 // 13 SCK
#define DATA_PIN_AMBIENT 1 // 11 MOSI
#define CLOCK_PIN_TOP  10// 27 SCK1
#define DATA_PIN_TOP 11 // 26 MOSI1
#define CLOCK_PIN_READING 6 // 49 SCK2
#define DATA_PIN_READING 7 // 50 MOSI2

extern CRGB ambientLeds[NUM_LEDS_AMBIENT];
extern CRGB readingLeds[NUM_LEDS_READING * 2];
extern CRGB topLeds[NUM_LEDS_TOP];

extern class ReadingLight readingLeft;
extern class ReadingLight readingRight;
extern class AmbientLight ambientLight;
extern class DemoLights demoLights;

class ReadingLight {
public:
    ReadingLight(struct CRGB *array, uint8_t length, bool reverse);  
    void toggle();
    void setBrightness(uint8_t value);
    void reset();

  private:
    struct CRGB *readingLeds;
    uint8_t numLeds;

    bool isOn;
    uint8_t brightness;
    bool isReversed;
};

class AmbientLight {
public:
    AmbientLight(
      struct CRGB *ambientArray, 
      uint8_t ambientLength, 
      struct CRGB *topArray, 
      uint8_t topLength
    );
    void toggle();
    void setBrightness(uint8_t value);
    void reset();

  private:
    struct CRGB *ambient;
    uint8_t numAmbient;
    struct CRGB *top;
    uint8_t numTop;

    bool isOn;
    uint8_t brightness;
};

class DemoLights {
public:
    DemoLights(
      struct CRGB *ambientArray, 
      uint8_t ambientLength, 
      struct CRGB *topArray, 
      uint8_t topLength,
      struct CRGB *readingArray, 
      uint16_t readingLength
    );  
    enum Mode {
        Rainbow,
        Chromotherapy,
    };
    void toggle();
    void setBrightness(uint8_t value);
    void setMode(Mode mode);
    void stop();
    void loop();
    bool isOn;

  private:
  struct CRGB *ambient;
  uint8_t numAmbient;
  struct CRGB *top;
  uint8_t numTop;
  struct CRGB *reading;
  uint16_t numReading;

    uint8_t brightness;
    Mode activeMode;

    void rainbow_beat();
    void applyRandomPalette(
      struct CRGB *targetArray, 
      CRGBPalette16 &pal, 
      uint16_t numLeds, 
      uint8_t indexScale, 
      uint8_t minBrightness, 
      uint8_t maxBrightness
    );
};

void lightsSetup();
void lightsLoop();

#endif // LIGHTS_H