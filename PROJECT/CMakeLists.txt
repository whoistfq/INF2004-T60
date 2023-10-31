cmake_minimum_required(VERSION 3.12)

project(pico_wifi_project)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add the access_point directory
add_subdirectory(access_point)
add_subdirectory(internet)
add_subdirectory(bridging)
add_subdirectory(FINAL)
