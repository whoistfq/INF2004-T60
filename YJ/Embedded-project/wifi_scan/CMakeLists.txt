set(PROGRAM_NAME wifi_scan)
set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)
pico_sdk_init()

project(wifi_scan)

add_executable(${PROGRAM_NAME}
    main.c
)

target_include_directories(${PROGRAM_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROGRAM_NAME}
    pico_stdlib
    hardware_uart
)

pico_enable_stdio_usb(${PROGRAM_NAME} 1)
pico_enable_stdio_uart(${PROGRAM_NAME} 1)

pico_add_extra_outputs(${PROGRAM_NAME})
target_include_directories(${PROGRAM_NAME} PRIVATE ${PICO_SDK_PATH}/src/rp2_common/hardware_uart/include)


# Enable floating-point support if needed
#target_compile_definitions(PicoWifiScanner PRIVATE -DFP_SUPPORT_AVAILABLE)

# Add any other compile flags or settings if necessary
#target_compile_options(PicoWifiScanner PRIVATE ...)

# Add your source directories if you have multiple
#target_include_directories(PicoWifiScanner PRIVATE ...)

# Set the Pico flash offset (0x0000_0000 by default)
#pico_set_boot_stage(PicoWifiScanner 2)