#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

// Define constants for the UART interface that will be used.
#define UART_ID uart0
#define BAUD_RATE 9600

// Define the GPIO pins to be used for UART transmission (TX) and reception (RX).
#define UART0_TX_PIN 12
#define UART0_RX_PIN 13


// UART interrupt handler for receive
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch;
        uart_read_blocking(UART_ID, &ch, 1); //Read the data received
        putchar(ch); // Echo the character
    }
}

int main() {

    // Initialize standard I/O functionality.
    stdio_init_all();

    // Initialize UART with the defined baud rate.
    uart_init(UART_ID, BAUD_RATE);

    // Configure the GPIO pins to act as UART TX and RX by setting their functions.
    gpio_set_function(UART0_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX_PIN, GPIO_FUNC_UART);

    // Enable UART receive (RX) interrupts, but not transmit (TX) interrupts.
    uart_set_irq_enables(UART_ID, true, false);
    
    // Set up the ISR
    irq_set_exclusive_handler(UART0_IRQ, on_uart_rx);
    irq_set_enabled(UART0_IRQ, true);

    while (true) {
        tight_loop_contents();
    }
}