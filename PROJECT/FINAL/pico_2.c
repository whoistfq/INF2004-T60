/*
Pico 2: Access Point Setup and Data Provisioning
Pico 2 sets itself up as an Access Point (as shown in your access_point.c code).
Once a device connects to Pico 2, it requests data (essentially internet data).
Pico 2 gets this data from Pico 1 (using SPI communication) and provides it to the connected device.
For the entire system to work, the most critical part is setting up efficient and synchronized SPI
communication between Pico 1 and Pico 2. You need to ensure that data packets received from the main
router on Pico 1 are correctly segmented, transmitted over SPI, re-assembled on Pico 2, and then
correctly served to user devices.
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"

#define BUF_LEN 128

bool initialize_wifi_access_point() {
    stdio_init_all();
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_SINGAPORE)) {
        printf("Failed to initialize WiFi module\n");
        return false;
    }
    printf("WiFi module initialized\n");
    char ap_ssid[] = "PICO_AP (Not Sus)";
    char ap_pass[] = "123";
    cyw43_arch_enable_ap_mode(ap_ssid, ap_pass, CYW43_AUTH_WPA2_AES_PSK);
    printf("Access Point configured and started\n");
    return true;
}

void serve_data_to_connected_devices(uint8_t data) {
    // TODO: Implement logic to serve the data to connected devices.
    printf("Serving data: %d\n", data); // Placeholder logic.
}

void initialize_spi_slave() {
    printf("SPI Peripheral Example\n");
    spi_init(spi_default, 1 * 1000000);
    spi_set_slave(spi_default, true);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);
}

int main() {
    if (!initialize_wifi_access_point()) {
        return 1;
    }
    initialize_spi_slave();
    uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN];
    for (uint8_t i = 0; i < BUF_LEN; ++i) {
        out_buf[i] = 0;
        in_buf[i] = 0;
    }
    while (1) {
        if (spi_is_readable(spi_default)) {
            printf("Reading data from SPI..\n");
            spi_read_blocking(spi_default, 0, in_buf, 1);
            serve_data_to_connected_devices(in_buf[0]);
        }
    }
    return 0;
}
