add_executable(access_point
access_point.c
)

# Include directories
target_include_directories(access_point PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts

)

# Link the necessary libraries, especially for Wi-Fi functionality
target_link_libraries(access_point
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(access_point 1)
pico_enable_stdio_uart(access_point 0)

# Create additional output files
pico_add_extra_outputs(access_point)
