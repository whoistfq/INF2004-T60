#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "sd_card.h"
#include "ff.h"
#include "ssi.h"
#include "cgi.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"

// WIFI Credentials, change to own wifi name and password
const char WIFI_SSID[] = "SusFi";
const char WIFI_PASSWORD[] = "aaps5930";

void SD_init() {
    // Sleep for 10 seconds before the program starts
    printf("\r\nProg Started. Wait for 10 seconds\r\n");
    sleep_ms(10000);
    printf("\r\nMain Prog Starting\r\n");

    // Initialize SD card
    if (!sd_init_driver()) {
        printf("ERROR: Could not initialize SD card\r\n");
        while (true);
    }
}

void mountFile(FATFS *fs) {
    FRESULT fr;
    // Mount drive
    fr = f_mount(fs, "0:", 1);
    if (fr != FR_OK) {
        printf("ERROR: Could not mount filesystem (%d)\r\n", fr);
        while (true);
    }
}

int main() {
    FATFS fs;
    stdio_init_all();

    cyw43_arch_init();

    cyw43_arch_enable_sta_mode();

    int connection_attempts = 0;

    SD_init();
    mountFile(&fs);

    // Connect to the WiFI network - loop until connected
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0) {
        printf("Attempt %d: Unable to connect to Wi-Fi network.\n", ++connection_attempts);
        if (connection_attempts >= 5) {
            printf("Failed to connect after %d attempts. Please check your SSID and PASSWORD. \n", connection_attempts);
        }
    }

    // Print a success message once connected
    printf("Connected! \n");

    // Fetch and print the IP address
    if (netif_default != NULL) {
        printf("IP Address: %s\n", ip4addr_ntoa(&netif_default->ip_addr));
    }

    // Initialize web server
    httpd_init();
    printf("Http server initialised\n");

    // Configure SSI and CGI handler
    ssi_init();
    printf("SSI Handler initialised\n");

    cgi_init();
    printf("CGI Handler initialised\n");

    // Infinite loop
    while (1) {
        tight_loop_contents();
    }
}
