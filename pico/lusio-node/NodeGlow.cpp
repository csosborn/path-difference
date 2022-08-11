/**
 * Copyright (c) 2022 Chris Osborn
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "pico/stdlib.h"

#include "Edge.hpp"
#include "Light.hpp"
#include "Node.hpp"

// #define LUSIO_PROTO 1

#ifdef LUSIO_PROTO

// Pin Assignments for Lusio Prototype Board
constexpr uint txPin1 = 16;
constexpr uint rxPin1 = 17;

constexpr uint txPin2 = 15;
constexpr uint rxPin2 = 10;

constexpr uint txPin3 = 14;
constexpr uint rxPin3 = 13;

constexpr uint lightPin = 22;

#else

// Pin Assignments for Lusio Production Board
constexpr uint txPin1 = 27;
constexpr uint rxPin1 = 28;

constexpr uint txPin2 = 26;
constexpr uint rxPin2 = 5;

constexpr uint txPin3 = 0;
constexpr uint rxPin3 = 1;

constexpr uint lightPin = 7;

#endif

int main()
{
  stdio_init_all();

  PIO pioTx = pio0;
  PIO pioRx = pio1;

  Light light(lightPin, pioRx, true);

  light.setBrightnessFraction(.5);

  if (light.init()) {
    printf("Light successfully configured.\n");
  }


  uint32_t counter = 0;
  int diff = 1;
  while (true)
  {

    auto rChannel = counter / 3;
    auto gChannel = counter;
    auto bChannel = counter / 2;

    for (int pix = 0; pix < 12; pix++) {
        light.setPixel(pix, rChannel, gChannel, bChannel);
    }
    light.update();

    if (counter >= 4092) {
        diff = -1;
    } else if (counter == 0) {
        diff = 1;
    }
    counter += diff;

    sleep_ms(1);

  }
}
