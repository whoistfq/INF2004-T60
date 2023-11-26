add_executable(pico2
pico2.c
)

# Include directories
target_include_directories(pico2 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts

)

# Link the necessary libraries, especially for Wi-Fi functionality
target_link_libraries(pico2
    pico_stdlib
#    pico_cyw43_arch_lwip_threadsafe_background
    hardware_uart
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(pico2 1)
pico_enable_stdio_uart(pico2 0)

# Create additional output files
pico_add_extra_outputs(pico2)