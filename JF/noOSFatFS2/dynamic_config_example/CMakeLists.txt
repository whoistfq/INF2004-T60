# Add executable. Default name is the project name, version 0.1
add_executable(dynamic_config_example2 
    dynamic_config_example.cpp 
    hw_config.cpp
)

# Choose source and destination for standard input and output:
#   See 4.1. Serial input and output on Raspberry Pi Pico in Getting started with Raspberry Pi Pico (https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf)
#   and 2.7.1. Standard Input/Output (stdio) Support in Raspberry Pi Pico C/C++ SDK (https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf):
pico_enable_stdio_uart(dynamic_config_example2 1)
pico_enable_stdio_usb(dynamic_config_example2 1)

# Add the standard library and FatFS/SPI to the build
target_link_libraries(dynamic_config_example2 
    pico_stdlib
    FatFs_SPI
)

# add_compile_definitions(CARD2)

pico_add_extra_outputs(dynamic_config_example2)

