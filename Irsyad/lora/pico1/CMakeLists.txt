add_executable(pico1
pico1.c
)

# Include directories
target_include_directories(pico1 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts

)

# Link the necessary libraries, especially for Wi-Fi functionality
target_link_libraries(pico1
    pico_stdlib
#    pico_cyw43_arch_lwip_threadsafe_background
    hardware_uart
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(pico1 1)
pico_enable_stdio_uart(pico1 0)

# Create additional output files
pico_add_extra_outputs(pico1)