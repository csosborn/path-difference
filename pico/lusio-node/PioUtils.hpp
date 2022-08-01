#pragma once

#include "hardware/pio.h"
#include <stdio.h>

template <const pio_program *program>
bool init_pio_program(PIO &pio, uint *offset, const char *progName) {
  if (pio == pio0) {
    printf("Initializing %s on pio0...\n", progName);
    static uint offset_ = 0;
    if (offset_ == 0) {
      if (pio_can_add_program(pio, program)) {
        offset_ = pio_add_program(pio, program);
        printf("Loaded at new offset %d.\n", offset_);
      } else {
        printf("Failed to add program!\n");
        return false;
      }
    } else {
      printf("Using existing offset %d.\n", offset_);
    }
    *offset = offset_;
    return true;
  } else if (pio == pio1) {
    printf("Initializing %s on pio1...\n", progName);
    static uint offset_ = 0;
    if (offset_ == 0) {
      if (pio_can_add_program(pio, program)) {
        offset_ = pio_add_program(pio, program);
        printf("Loaded at new offset %d.\n", offset_);
      } else {
        printf("Failed to add program!\n");
        return false;
      }
    } else {
      printf("Using existing offset %d.\n", offset_);
    }
    *offset = offset_;
    return true;
  } else {
    printf("Could not initialize %s on unknown PIO unit.\n", progName);
    return false;
  }
}
