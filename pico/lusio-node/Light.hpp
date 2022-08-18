#pragma once

#include "hardware/pio.h"
#include <functional>

constexpr uint32_t LIGHT_SIZE = 12;
constexpr int bit_depth = 12;
constexpr uint32_t maxChannelVal = 1 << 12;

inline uint32_t limit(uint32_t in) {
  return in > maxChannelVal-1 ? maxChannelVal : in;
}

struct Color {
  public:
  Color() : r(0), g(0), b(0) {}
  explicit Color(uint32_t ri, uint32_t gi, uint32_t bi) 
    : r(limit(ri)), 
      g(limit(gi)), 
      b(limit(bi)) {}

  static Color forRatios(float rf, float gf, float bf) {
    return Color(rf * maxChannelVal, gf * maxChannelVal, bf * maxChannelVal);
  }

  uint32_t r; 
  uint32_t g; 
  uint32_t b;
};

struct PixelRange {
  public:
  PixelRange(uint16_t only) : from(only), to(only+1) {}
  PixelRange(uint16_t pfrom, uint16_t pto) : from(pfrom), to(pto) {}
  uint16_t from;
  uint16_t to;
};

class Light {
public:
  explicit Light(uint pin, PIO &pio, bool ditherOnCore1 = false);

  bool init();

  void put_pixel(uint32_t pixel_grb);

  void setPixels(const PixelRange& range, const Color& color);

  void setPixel(uint32_t pixelIndex, const Color& color);

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