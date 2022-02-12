cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(read_rfid C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(read_rfid
        read_rfid.c
        )

pico_enable_stdio_usb(read_rfid 1)
pico_add_extra_outputs(read_rfid)
target_link_libraries(read_rfid pico_stdlib)
