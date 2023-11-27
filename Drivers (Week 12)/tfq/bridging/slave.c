#include <stdio.h>
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
//=================================================================================//
#define BUF_LEN 128
//=================================================================================//
int main() {
  // Enable UART so we can print
  stdio_init_all();
  sleep_ms(2 * 1000);
  printf("SPI Peripheral Example\n");
  // Enable SPI 0 at 1 MHz and connect to GPIOs
  spi_init(spi_default, 1 * 1000000);
  spi_set_slave(spi_default, true);
  gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);
  uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN];
  // Initialize output buffer
  for (uint8_t i = 0; i < BUF_LEN; ++i) {
    out_buf[i] = 0;
    in_buf[i] = 0;
  }
  while (1) {
    // Check if there is incoming data from the master.
    if (spi_is_readable(spi_default)) {
      printf("Reading data from SPI...\n");
      // Read the data from the master.
      spi_read_blocking(spi_default, 0, in_buf, 1);
      printf("Data received from PICO 1: %d\n", in_buf[0]);

      // Prepare data to send back to the master.
      // This could be a response or any data you want to transmit.
      out_buf[0] = in_buf[0] + 1; // Example: send back received value incremented by 1.

      // Wait until SPI is ready to be written to.
      while (!spi_is_writable(spi_default)) {
        tight_loop_contents();
      }

      // Send the data back to the master.
      spi_write_blocking(spi_default, out_buf, 1);
      printf("Data sent to PICO 1: %d\n", out_buf[0]);
    }
  }
}
//=================================================================================//
