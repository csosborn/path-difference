#include "hardware/pio.h"

bool init_nec_transmit_program(PIO &pio, uint *offset);

void start_nec_transmit_program(PIO &pio, uint sm, uint offset, uint pin, float freq);