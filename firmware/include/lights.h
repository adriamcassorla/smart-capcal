#ifndef LIGHTS_H
#define LIGHTS_H

#include <Arduino.h>
#include <FastLED.h>

class ReadingLight {
public:
    ReadingLight(struct CRGB *array, uint8_t length);  
    void toggle(bool reverse);
    void setBrightness(uint8_t value);

private:
  struct CRGB *readingLeds;
  uint8_t numLeds;

  bool isOn;
  uint8_t brightness;
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

private:
  struct CRGB *ambient;
  struct CRGB *top;
  uint8_t numAmbient;
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
      uint8_t readingLength
    );  
    enum Mode {
        Rainbow,
        Chromotherapy,
    };
    void setBrightness(uint8_t value);
    void setMode(Mode mode);
    void toggle();
    void stop();
    void loop();
    bool isOn;

  private:
  struct CRGB *ambient;
  struct CRGB *top;
  struct CRGB *reading;
  uint8_t numAmbient;
  uint8_t numTop;
  uint8_t numReading;

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

#endif // LIGHTS_H