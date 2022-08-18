/**
 * Copyright (c) 2022 Chris Osborn
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <random>
#include "pico/stdlib.h"
#include "pico/double.h"

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


typedef enum {
  OFF,
  STARTUP,
  ALIGNMENT,
  RUNNING
} NodeState;

constexpr uint secondsInAlignment = 3; //60 * 5;

NodeState currentState = OFF;
uint32_t stateEntryTime = 0;


PixelRange allLeds(0, 12);

// Values for Alignment State

PixelRange range1(6, 10);
PixelRange range2(10, 2);
PixelRange range3(2, 6);

Color allOff;
auto startup = Color::forRatios(0.3, 0.3, 0.3);
auto purple = Color::forRatios(0.5, 0.0, 0.5);

auto alertGreen = Color::forRatios(0.0, 0.6, 0.2);
auto calmGreen = Color::forRatios(0.0, 0.2, 0.05);
auto calmBlue = Color::forRatios(0.0, 0.1, 0.3);

auto alertRed = Color::forRatios(0.7, 0.1, 0.1);
auto calmRed = Color::forRatios(0.3, 0.04, 0.04);


// Values for Running State

auto calmColor = Color::forRatios(0.2, 0.5, 0.3);
auto currColor = calmColor;
auto targetColor = calmColor;

Color blues[] = {
  Color(0x680, 0xFF0, 0xA00),
  Color(0x680, 0xFF0, 0xCD0),
  Color(0x680, 0xFF0, 0xDB0),
  Color(0x680, 0xFF0, 0xEC0),
  Color(0x680, 0xFD0, 0xFF0),
  Color(0x680, 0xE50, 0xFF0),
  Color(0x680, 0x990, 0xFF0),
  Color(0x680, 0x790, 0xFF0),
  Color(0x000, 0xDF0, 0xFF0),
  Color(0x000, 0xFC0, 0xFF0),
  Color(0x000, 0x6F0, 0xFF0),
  Color(0x000, 0x120, 0xFF0),
};

Color gold(0xFF0, 0xD00, 0x000);


//
//

struct PixelTarget {
  PixelTarget() : start(0,0,0), startTime(0), end(0,0,0), endTime(0) {}

  void update(uint32_t currentTime) {
    if (currentTime <= startTime) {
      current = start;
    } else if (currentTime >= endTime) {
      current = end;
    } else {
      float ratio = ((float)currentTime - startTime) / ((float)endTime - startTime);
      current.r = start.r + ratio * ((int)end.r - (int)start.r);
      current.g = start.g + ratio * ((int)end.g - (int)start.g);
      current.b = start.b + ratio * ((int)end.b - (int)start.b);
    }
  }

  void sweep(uint32_t newStartTime, const Color& newStart, const Color& newEnd, uint32_t duration) {
    current = newStart;
    start = newStart;
    end = newEnd;
    startTime = newStartTime;
    endTime = newStartTime + duration;
  }

  Color start;
  uint32_t startTime;
  Color end;
  uint32_t endTime;

  Color current;
};

PixelTarget targets[LIGHT_SIZE];

void setPixelTargets(uint32_t now, const PixelRange& range, const Color& start, const Color& end, uint32_t duration) {
  uint16_t pix = range.from;
  while (pix != range.to) {
    targets[pix].sweep(now, start, end, duration);
    pix++;
    if (pix == LIGHT_SIZE) {
      if (range.to >= pix) {
        return;
      } else {
        pix = 0;
      }
    }
  }
}

void setPixelTargets(uint32_t now, const PixelRange& range, const Color& end, uint32_t duration) {
  uint16_t pix = range.from;
  while (pix != range.to) {
    targets[pix].sweep(now, targets[pix].current, end, duration);
    pix++;
    if (pix == LIGHT_SIZE) {
      if (range.to >= pix) {
        return;
      } else {
        pix = 0;
      }
    }
  }
}



int main()
{
  stdio_init_all();

  PIO pioTx = pio0;
  PIO pioRx = pio1;

  Light light(lightPin, pioRx, true);

  if (light.init()) {
    printf("Light successfully configured.\n");
  }

  Edge edge1(1, pioTx, txPin1, pioRx, rxPin1);
  Edge edge2(2, pioTx, txPin2, pioRx, rxPin2);
  Edge edge3(3, pioTx, txPin3, pioRx, rxPin3);

  if (edge1.init() && edge2.init() && edge3.init()) {
    printf("All edges successfully configured.\n");
  }

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  gpio_init(rxPin1);
  gpio_init(rxPin2);
  gpio_init(rxPin3);

  gpio_set_dir(rxPin1, GPIO_IN);
  gpio_pull_up(rxPin1);
  gpio_set_dir(rxPin2, GPIO_IN);
  gpio_pull_up(rxPin2);
  gpio_set_dir(rxPin3, GPIO_IN);
  gpio_pull_up(rxPin3);

  bool last1 = false;
  bool last2 = false;
  bool last3 = false;

  bool update = false;
  uint32_t counter = 0;

  bool enabled = true;

  while (true)
  {
    auto absTime = get_absolute_time();
    auto now = to_ms_since_boot(absTime);
    auto timeInState = now - stateEntryTime;

    // printf("Current state is %d, time in state is %d.\n", currentState, timeInState);

    if (currentState == OFF) {
      currentState = ALIGNMENT; //STARTUP;
      stateEntryTime = now;
    
    } else if (currentState == STARTUP) {
      if (timeInState < 100) {
        setPixelTargets(now, allLeds, allOff, startup, 1000); 
      } else if (timeInState > 1500) {
        setPixelTargets(now, allLeds, alertRed, calmRed, 5000);
        currentState = ALIGNMENT;
        stateEntryTime = now;
      }
    
    } else if (currentState == ALIGNMENT) {
      bool in1 = !gpio_get(rxPin1);
      bool in2 = !gpio_get(rxPin2);
      bool in3 = !gpio_get(rxPin3);
    
      if (in1 != last1) {
        last1 = in1;
        if (last1) {
          setPixelTargets(now, range1, alertGreen, calmBlue, 10000);
        } else {
          setPixelTargets(now, range1, alertRed, calmRed, 5000);
        }
      }

      if (in2 != last2) {
        last2 = in2;
        if (last2) {
          setPixelTargets(now, range2, alertGreen, calmBlue, 10000);
        } else {
          setPixelTargets(now, range2, alertRed, calmRed, 5000);
        }
      }

      if (in3 != last3) {
        last3 = in3;
        if (last3) {
          setPixelTargets(now, range3, alertGreen, calmBlue, 10000);
        } else {
          setPixelTargets(now, range3, alertRed, calmRed, 5000);
        }
      }

      if (timeInState > 1000 * secondsInAlignment) {
        currentState = RUNNING;
        stateEntryTime = now;
      }

      for (uint16_t pix = 0; pix < LIGHT_SIZE; pix++) {
        targets[pix].update(now);
        light.setPixel(pix, targets[pix].current);
      }

    } else if (currentState == RUNNING) {

      auto phase = timeInState / 1000.0;
      auto factor = 1 + (sin(phase)-1)/4.0;
      targetColor = Color(gold.r * factor, gold.g * factor, gold.b * factor);
      printf("%f  %d   %d   %d\n", factor, targetColor.r, targetColor.g, targetColor.b);
      setPixelTargets(now, allLeds, targetColor, 0);

      for (uint16_t pix = 0; pix < LIGHT_SIZE; pix++) {
        light.setPixel(pix, targetColor);
      }

    }


    sleep_ms(1);
  }
}
