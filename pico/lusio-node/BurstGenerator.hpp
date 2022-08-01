#pragma once

#include "hardware/pio.h"

class BurstGenerator {
public:
  explicit BurstGenerator(PIO &pioTx);

  bool init();

private:
  PIO &pioTx_;
  bool initialized_ = false;
  uint carrierBurstOffset_;
  int carrierBurstSM_;
};