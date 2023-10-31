/*
Firstly, Pico 1 would connect to your main router (internet source) and receive data. 
Secondly, Pico 1 would forward this data to Pico 2, which is acting as an AP. The user
devices connected to Pico 2 would then use this data to access the internet.

Here's a step-by-step breakdown:

Pico 1: Internet Connection and Data Retrieval
Pico 1 connects to the main router using WiFi as shown in the internet.c code you provided.
Once connected, Pico 1 should have the capability to retrieve internet data.
This data is then forwarded to Pico 2 using SPI, which is set up in your master.c code.
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/cyw43_wifi.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"

#define BUF_LEN 128

char ssid[] = "NOKIA-F671";
char pass[] = "snbh5jV7cb";

bool connect_to_wifi(const char* ssid, const char* pass) {
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_SINGAPORE)) {
        printf("failed to initialise\n");
        return false;
    }
    printf("initialised\n");
    cyw43_arch_enable_sta_mode();
    while (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("failed to connect. Retrying...\n");
        sleep_ms(5000); // Wait for 5 seconds before retrying
    }
    printf("connected\n");
    return true;
}

uint8_t fetch_data_from_router() {
    // TODO: Implement your method to fetch data from the router.
    return 42; // Placeholder value.
}

void spi_master_init_and_run() {
    sleep_ms(2 * 1000);
    printf("SPI Central Example\n");
    spi_init(spi_default, 1 * 1000000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);
    uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN];
    for (u_int8_t i = 0; i < BUF_LEN; ++i) {
        out_buf[i] = 0;
        in_buf[i] = 0;
    }
    for (;;) {
        uint8_t data = fetch_data_from_router();
        printf("Sending data %d to SPI Peripheral\n", data);
        out_buf[0] = data;
        spi_write_read_blocking(spi_default, out_buf, in_buf, 1);
        sleep_ms(2 * 1000);
    }
}

int main() {
    stdio_init_all();
    if (!connect_to_wifi(ssid, pass)) {
        return 1;
    }
    spi_master_init_and_run();
    return 0;
}
