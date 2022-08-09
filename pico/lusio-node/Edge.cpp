#include <stdio.h>

#include "Edge.hpp"
#include "nec_transmit.hpp"


Edge::Edge(uint edgeNum, PIO &pioTx, uint txPin, PIO &pioRx, uint rxPin)
    : edgeNum_(edgeNum), pioTx_(pioTx), txPin_(txPin), pioRx_(pioRx),
      rxPin_(rxPin) {
  printf("Created edge %d with tx pin %d, rx pin %d.\n", edgeNum_, txPin_,
         rxPin_);
}

bool Edge::init() {

  txSM_ = pio_claim_unused_sm(pioTx_, true);
  if (txSM_ == -1) {
    printf("Failed to configure PIO (tx) for edge %d.\n", edgeNum_);
    return false;
  }

  if (!init_nec_transmit_program(pioTx_, &txOffset_)) {
    printf("Error: could not initialize NEC transmit program for edge %d.\n",
           edgeNum_);
  }

  start_nec_transmit_program(pioTx_, txSM_, txOffset_, txPin_, 38.222e3);

  // if (pio_can_add_program(pioTx_, &nec_carrier_control_program)) {
  //   carrierControlOffset_ =
  //       pio_add_program(pioTx_, &nec_carrier_control_program);
  //   txSM_ = pio_claim_unused_sm(pioTx_, true);
  //   if (txSM_ == -1) {
  //     printf("Failed to configure PIO (tx) for edge %d.\n", edgeNum_);
  //     return false;
  //   }
  // } else {
  //   printf("Failed to configure carrier control program for edge %d.\n",
  //          edgeNum_);
  //   return false;
  // }

  // if (pio_can_add_program(pioRx_, &nec_receive_program)) {
  //   receiveOffset_ = pio_add_program(pioRx_, &nec_receive_program);
  //   rxSM_ = pio_claim_unused_sm(pioRx_, true);
  //   if (rxSM_ == -1) {
  //     printf("Failed to configure PIO (rx) for edge %d.\n", edgeNum_);
  //     return false;
  //   }
  // } else {
  //   printf("Failed to configure receive program for edge %d.\n", edgeNum_);
  //   return false;
  // }

  printf("Edge configured with tx SM %d and rx SM %d.\n", txSM_, rxSM_);
  return true;
}


void Edge::enable() {
  pio_sm_set_enabled(pioTx_, txSM_, true);
}

void Edge::disable() {
  pio_sm_set_enabled(pioTx_, txSM_, false);
}
