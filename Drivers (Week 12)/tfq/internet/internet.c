#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

char ssid[] = "Marco";
char pass[] = "test";

int main() {
    stdio_init_all();

    if (cyw43_arch_init_with_country(CYW43_COUNTRY_SINGAPORE)) {
        printf("Failed to initialise\n");
        return 1;
    }
    printf("Initialised\n");

    cyw43_arch_enable_sta_mode();

    while (true) {
        if (cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
            printf("Failed to connect, retrying...\n");
            sleep_ms(5000); // Wait for 5 seconds before retrying
        } else {
            printf("Connected\n");
            break; // Exit the loop if connected
        }
    }
}
