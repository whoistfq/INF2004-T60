if (NOT PICO_NO_HARDWARE)

    project(pico_wifi_project)


    # Initialize the Raspberry Pi Pico SDK
    pico_sdk_init()

    # Add the access_point directory
    add_subdirectory(access_point)
    add_subdirectory(internet)
    add_subdirectory(bridging)
    add_subdirectory(FINAL)
    add_subdirectory(test)
    add_subdirectory(master_pico)
    add_subdirectory(slave_pico)
endif ()

