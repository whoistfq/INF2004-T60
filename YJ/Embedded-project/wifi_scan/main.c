#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Define UART pins
#define UART_ID uart0
#define TX_PIN 16
#define RX_PIN 17
#define BAUD_RATE 115200

// Function to send AT command and receive response
void sendATCommand(const char *command) {
    printf("Sending command: %s\n", command);

    // Send command to ESP-01 module
    for (int i = 0; i < strlen(command); i++) {
        uart_putc(UART_ID, command[i]);
    }

    // Wait for response
    sleep_ms(500);  // Adjust delay as needed

    // Print response
    printf("Response:\n");
    while (uart_is_readable(UART_ID)) {
        putchar(uart_getc(UART_ID));
    }

    printf("\n");
}

int main() {
    stdio_init_all();

    // Initialize the UART with the specified pins and baud rate
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);

    // Buffer to store user input
    char ssid[32];

    // Get SSID from the user
    printf("Enter WiFi SSID: ");
    scanf("%s", ssid);

    // Command to set the WiFi SSID on ESP-01 module
    char command[64];
    snprintf(command, sizeof(command), "AT+CWJAP=\"%s\"\r\n", ssid);

    // Send command to ESP-01 module
    sendATCommand(command);

    // Command to get WiFi information
    const char *infoCommand = "AT+CWJAP?\r\n";

    // Send command to ESP-01 module
    sendATCommand(infoCommand);

    return 0;
}
