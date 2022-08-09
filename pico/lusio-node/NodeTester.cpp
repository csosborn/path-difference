
#include <stdio.h>

#include "pico/stdlib.h"

// Pin Assignments for Lusio Production Board
constexpr uint txPin1 = 27;
constexpr uint rxPin1 = 28;

constexpr uint txPin2 = 26;
constexpr uint rxPin2 = 5;

constexpr uint txPin3 = 0;
constexpr uint rxPin3 = 1;

constexpr uint lightPin = 7;


int main()
{

  gpio_init(25);

  gpio_init(rxPin1);
  gpio_init(rxPin2);
  gpio_init(rxPin3);

  gpio_init(txPin1);
  gpio_init(txPin2);
  gpio_init(txPin3);

  gpio_init(lightPin);

  gpio_set_dir(25, GPIO_OUT);;

  gpio_set_dir(rxPin1, GPIO_OUT);
  gpio_set_dir(rxPin2, GPIO_OUT);
  gpio_set_dir(rxPin3, GPIO_OUT);

  gpio_set_dir(txPin1, GPIO_OUT);
  gpio_set_dir(txPin2, GPIO_OUT);
  gpio_set_dir(txPin3, GPIO_OUT);

  gpio_set_dir(lightPin, GPIO_OUT);

while (true) {

    gpio_put(25, 1);

    gpio_put(rxPin1, 1);
    gpio_put(rxPin2, 1);
    gpio_put(rxPin3, 1);

    gpio_put(txPin1, 1);
    gpio_put(txPin2, 1);
    gpio_put(txPin3, 1);

    gpio_put(lightPin, 1);

    sleep_ms(20);

    gpio_put(25, 0);

    gpio_put(rxPin1, 0);
    gpio_put(rxPin2, 0);
    gpio_put(rxPin3, 0);

    gpio_put(txPin1, 0);
    gpio_put(txPin2, 0);
    gpio_put(txPin3, 0);

    gpio_put(lightPin, 0);

    sleep_ms(100);


}


}