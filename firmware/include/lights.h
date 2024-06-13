#ifndef LIGHTS_H
#define LIGHTS_H

class ReadingLight {
public:
    ReadingLight(struct CRGB *array, uint8_t length);  
    void toggle(bool reverse);
    void setBrightness(uint8_t value);

private:
  struct CRGB *ledsArray;
  uint8_t numLeds;

  bool isOn;
  uint8_t brightness;
};

#endif // LIGHTS_H