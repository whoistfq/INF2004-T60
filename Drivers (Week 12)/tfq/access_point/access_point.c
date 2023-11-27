#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main() {
    // Initialize standard libraries
    stdio_init_all();

    // Initialize the WiFi module with a specific country code
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_SINGAPORE)) {
        printf("Failed to initialize WiFi module\n");
        return 1;
    }
    printf("WiFi module initialized\n");

    // Configure and enable the Access Point mode with an SSID and Password
    char ap_ssid[] = "MYPICO_AP";
    char ap_pass[] = "testing123";

    cyw43_arch_enable_ap_mode(ap_ssid, ap_pass, CYW43_AUTH_WPA2_AES_PSK);

    printf("Access Point configured and started\n");

    // Main loop to keep the program running
    while (1) {
        // Do any additional tasks or just keep the program running
    }

    return 0;
}