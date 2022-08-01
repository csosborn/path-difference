#include "BurstGenerator.hpp"
#include <stdio.h>

#include "hardware/clocks.h"    // for clock_get_hz()

#include "nec_carrier_burst.pio.h"

// static bool programsConfigured_ = false;

BurstGenerator::BurstGenerator(PIO &pioTx) : pioTx_(pioTx) {
  printf("Created tx burst generator.\n");
}

bool BurstGenerator::init() {
  if (!initialized_) {
    if (pio_can_add_program(pioTx_, &nec_carrier_burst_program)) {
      carrierBurstOffset_ = pio_add_program(pioTx_, &nec_carrier_burst_program);

      // claim an unused state machine on this PIO
      carrierBurstSM_ = pio_claim_unused_sm(pioTx_, true);
      if (carrierBurstSM_ == -1) {
        printf("Error: could not claim state machine for burst generator.\n");
        return false;
      }

      nec_carrier_burst_program_init(pioTx_,
                                   carrierBurstSM_,
                                   carrierBurstOffset_,
                                   1,
                                   38.222e3);                   // 38.222 kHz carrier
      printf("Configured burst generator on SM %d.\n", carrierBurstSM_);
      initialized_ = true;
    } else {
      printf("Error: could not add PIO program for burst generator.");
      return false;
    }
  }
  return true;
}