/**
 * Copyright (c) 2022 Chris Osborn
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

// #include "nec_receive.h"
// #include "nec_transmit.h" // include the library headers
#include "pico/stdlib.h"

#include "Edge.hpp"
#include "Light.hpp"
#include "Node.hpp"

#define LUSIO_PROTO 1

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

constexpr uint txPin2 = 31;
constexpr uint rxPin2 = 5;

constexpr uint txPin3 = 1;
constexpr uint rxPin3 = 2;

constexpr uint lightPin = 6;

#endif

int main()
{
  stdio_init_all();

  PIO pioTx = pio0;
  PIO pioRx = pio1;


  Light light(lightPin, pioRx);

  if (light.init())
  {
    printf("Light successfully configured.\n");
  }

  for (int pix = 0; pix < 12; pix++) {
    // light.put_pixel(0x000200);
    light.setPixel(pix, 0x000200);
  }
  light.update();

  sleep_ms(3000);

  Edge edge1(1, pioTx, txPin1, pioRx, rxPin1);
  Edge edge2(2, pioTx, txPin2, pioRx, rxPin2);
  Edge edge3(3, pioTx, txPin3, pioRx, rxPin3);

  if (edge1.init() && edge2.init() && edge3.init()) {
    printf("All edges successfully configured.\n");
  }

  // configure and enable the state machines
  // int tx_sm = nec_tx_init(
  //     pioTx,
  //     tx_gpio); // uses two state machines, 16 instructions and one IRQ
  // int rx_sm =
  //     nec_rx_init(pioRx, rx_gpio); // uses one state machine and 9 instructions

  // if (tx_sm == -1 || rx_sm == -1) {
  //   printf("could not configure PIO\n");
  //   return -1;
  // }

  // // transmit and receive frames
  // uint8_t tx_address = 0xFF, tx_data = 0x00, rx_address, rx_data;
  // while (true) {
  //   // create a 32-bit frame and add it to the transmit FIFO
  //   uint32_t tx_frame = nec_encode_frame(tx_address, tx_data);
  //   pio_sm_put(pioTx, tx_sm, tx_frame);
  //   printf("\nSent: %02x, %02x", tx_address, tx_data);

  //   // allow time for the frame to be transmitted (optional)
  //   sleep_ms(100);

  //   // display any frames in the receive FIFO
  //   while (!pio_sm_is_rx_fifo_empty(pioRx, rx_sm)) {
  //     uint32_t rx_frame = pio_sm_get(pioRx, rx_sm);

  //     if (nec_decode_frame(rx_frame, &rx_address, &rx_data)) {
  //       printf("\tReceived and decoded: %02x, %02x", rx_address, rx_data);
  //     } else {
  //       printf("\tReceived: %08x", rx_frame);
  //     }
  //   }

  //   sleep_ms(200);
  //   tx_data += 1;
  // }

  // int q = 0;
  // while (true) {
  //   for (int pix = 0; pix < 12; pix++) {
  //     light.put_pixel(pix == q || pix == (q + 1) % 12 ? 0x080003 : 0x021001);
  //     // light.put_pixel(pix == q || pix == (q + 1) % 12 ? 0xFF0030 : 0x200040);
  //   }
  //   q++;
  //   if (q == 12) {
  //     q = 0;
  //   }
  //   sleep_ms(50);
  // }

  gpio_init(rxPin1);
  gpio_init(rxPin2);
  gpio_init(rxPin3);

  gpio_set_dir(rxPin1, GPIO_IN);
  gpio_pull_up(rxPin1);
  gpio_set_dir(rxPin2, GPIO_IN);
  gpio_pull_up(rxPin2);
  gpio_set_dir(rxPin3, GPIO_IN);
  gpio_pull_up(rxPin3);

  bool last1 = true;
  bool last2 = true;
  bool last3 = true;

  // uint32_t counter = 0;

  while (true)
  {
    // gpio_put(PICO_DEFAULT_LED_PIN, 1);
    // gpio_put(txPin1, 1);
    // gpio_put(txPin2, 1);
    // gpio_put(txPin3, 1);
    // sleep_ms(50);
    // gpio_put(PICO_DEFAULT_LED_PIN, 0);
    // gpio_put(txPin1, 0);
    // gpio_put(txPin2, 0);
    // gpio_put(txPin3, 0);
    // sleep_ms(50);

    // for (int pix = 0; pix < 12; pix++) {
    //   light.put_pixel(0x000200);
    // }

    bool in1 = gpio_get(rxPin1);
    bool in2 = gpio_get(rxPin2);
    bool in3 = gpio_get(rxPin3);
  
    if (in1 != last1) {
      last1 = in1;
      light.setPixel(11, !last1 ? 0x333333 : 0x010001);
      light.update();
    }

    if (in2 != last2) {
      last2 = in2;
      light.setPixel(2, !last2 ? 0x333333 : 0x010001);
      light.update();
    }

    if (in3 != last3) {
      last3 = in3;
      light.setPixel(5, !last3 ? 0x333333 : 0x010001);
      light.update();
    }


    // counter++;

  }
}
