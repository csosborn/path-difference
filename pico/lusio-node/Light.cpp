#include <stdio.h>

#include "Light.hpp"
#include "hardware/clocks.h"
#include "pio_neopixel.hpp"
#include "ws2812.pio.h"

constexpr bool IS_RGBW = false;

Light::Light(uint pin, PIO &pio) : pin_(pin), pio_(pio) {
  for (uint32_t i = 0; i < LIGHT_SIZE; i++) {
    pixVals_[i] = 0;
  }
  printf("Created light with pin %d.\n", pin_);
}

bool Light::init() {
  ws2812SM_ = pio_claim_unused_sm(pio_, true);
  if (ws2812SM_ == -1) {
    printf("Error: could not claim state machine for light.\n");
    return false;
  }

  if (!init_neopixel_program(pio_, &ws2812Offset_)) {
    printf("Error: could not initialize Neopixel program.\n");
    return false;
  }

  ws2812_program_init(pio_, ws2812SM_, ws2812Offset_, pin_, 800000, IS_RGBW);
  printf("Started light on SM %d.\n", ws2812SM_);

  update();

  return true;
}

void Light::put_pixel(uint32_t pixel_grb) {
  pio_sm_put_blocking(pio_, ws2812SM_, pixel_grb << 8u);
}

void Light::setPixel(uint32_t pixelIndex, uint32_t pixelGRB) {
  pixVals_[pixelIndex] = pixelGRB;
}

void Light::update() {
  for (uint32_t i = 0; i < LIGHT_SIZE; i++) {
    pio_sm_put_blocking(pio_, ws2812SM_, pixVals_[i] << 8u);
  }
}