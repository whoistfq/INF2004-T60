# Master executable
add_executable(pico_1
pico_1.c
)

# Include directories for Master
target_include_directories(pico_1 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
)

# Link libraries for Master
target_link_libraries(pico_1
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    hardware_spi
)

# Enable USB output, disable UART output for Master
pico_enable_stdio_usb(pico_1 1)
pico_enable_stdio_uart(pico_1 0)

# Create additional output files for Master
pico_add_extra_outputs(pico_1)

# Slave executable
add_executable(pico_2
pico_2.c
)

# Include directories for Slave
target_include_directories(pico_2 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
)

# Link libraries for Slave
target_link_libraries(pico_2
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    hardware_spi
)

# Enable USB output, disable UART output for Slave
pico_enable_stdio_usb(pico_2 1)
pico_enable_stdio_uart(pico_2 0)

# Create additional output files for Slave
pico_add_extra_outputs(pico_2)
