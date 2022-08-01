#include "pio_neopixel.hpp"
#include "PioUtils.hpp"
#include "ws2812.pio.h"

bool init_neopixel_program(PIO &pio, uint *offset) {
  return init_pio_program<&ws2812_program>(pio, offset, "NeoPixel");
}