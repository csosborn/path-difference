#include "nec_transmit.hpp"
#include "PioUtils.hpp"
#include "nec_transmit.pio.h"

bool init_nec_transmit_program(PIO &pio, uint *offset) {
  return init_pio_program<&nec_transmit_program>(pio, offset, "NEC Transmit");
}

void start_nec_transmit_program(PIO &pio, uint sm, uint offset, uint pin, float freq) {
  nec_transmit_program_init(pio, sm, offset, pin, freq);
}