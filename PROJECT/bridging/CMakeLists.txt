# Master executable
add_executable(master
master.c
)

# Include directories for Master
target_include_directories(master PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
)

# Link libraries for Master
target_link_libraries(master
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    hardware_spi
)

# Enable USB output, disable UART output for Master
pico_enable_stdio_usb(master 1)
pico_enable_stdio_uart(master 0)

# Create additional output files for Master
pico_add_extra_outputs(master)

# Slave executable
add_executable(slave
slave.c
)

# Include directories for Slave
target_include_directories(slave PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
)

# Link libraries for Slave
target_link_libraries(slave
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    hardware_spi
)

# Enable USB output, disable UART output for Slave
pico_enable_stdio_usb(slave 1)
pico_enable_stdio_uart(slave 0)

# Create additional output files for Slave
pico_add_extra_outputs(slave)
