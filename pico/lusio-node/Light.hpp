#pragma once

#include "hardware/pio.h"
#include <functional>

constexpr uint32_t LIGHT_SIZE = 12;

class Light {
public:
  explicit Light(uint pin, PIO &pio, bool ditherOnCore1 = false);

  bool init();

  void put_pixel(uint32_t pixel_grb);

  void setPixel(uint32_t pixelIndex, uint32_t pixelGRB);

  void setPixel(uint32_t pixelIndex, uint32_t rVal, uint32_t gVal, uint32_t bVal);

  void setBrightnessFraction(float bf);

  void update();

private:
  uint pin_;
  PIO &pio_;
  uint ws2812Offset_;
  int ws2812SM_;
  uint32_t pixVals_[LIGHT_SIZE];
  uint32_t errors_[LIGHT_SIZE];
  
  uint8_t brightnessFraction_;
  bool dither_;
};