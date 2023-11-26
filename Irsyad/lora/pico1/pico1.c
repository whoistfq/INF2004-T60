#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

// Define constants for the UART interface that will be used.
#define UART_ID uart0
#define BAUD_RATE 9600

// Define the GPIO pins to be used for UART transmission (TX) and reception (RX).
#define UART_TX_PIN 12
#define UART_RX_PIN 13

int main() {

    // Initialize standard I/O functionality.
    stdio_init_all();
    
    // Initialize UART with the defined baud rate.
    uart_init(UART_ID, BAUD_RATE);

    // Configure the GPIO pins to act as UART TX and RX by setting their functions.
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    //uart_set_fifo_enabled(UART_ID, true);


    const char *message = "Hello from PICO1 to PICO2\n";
    size_t message_len = strlen(message);


    while (true) {

        // Check if the UART hardware is ready to send data.    
        if (uart_is_writable(UART_ID)) {

            // Send the message via UART, block until the entire message has been queued for sending
            uart_write_blocking(UART_ID, (const uint8_t *)message, message_len);
        
        } else {

            printf("UART is not writable\n");
        
        }
        // Wait for 1 second before attempting to send the message again.
        sleep_ms(1000);
    }
}