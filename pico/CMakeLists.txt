cmake_minimum_required(VERSION 3.12)

message("Using PICO_SDK_PATH from environment ('${PICO_SDK_PATH}')")

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(lusio_2022 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()


add_subdirectory("lusio-node")