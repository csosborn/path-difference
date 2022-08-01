
add_executable(lusio_node)

# define library to contain Neopixel driver
add_library(pio_neopixel pio_neopixel.cpp)
target_link_libraries(pio_neopixel LINK_PUBLIC
    pico_stdlib
    hardware_pio)

# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(pio_neopixel ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)


# define library to contain NEC transmitter driver
add_library(nec_transmit nec_transmit.cpp)
target_link_libraries(nec_transmit LINK_PUBLIC 
    pico_stdlib 
    hardware_pio)

# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(nec_transmit ${CMAKE_CURRENT_LIST_DIR}/nec_transmit.pio)


target_link_libraries(lusio_node LINK_PUBLIC
    pico_stdlib
    hardware_pio
    # nec_receive_library
    # nec_transmit_library
    pio_neopixel
    nec_transmit
)

target_sources(lusio_node PRIVATE Node.cpp Edge.cpp Light.cpp)

pico_add_extra_outputs(lusio_node)
pico_enable_stdio_usb(lusio_node 1)
