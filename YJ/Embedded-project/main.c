#include <stdio.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"

#define UART_ID uart0
#define BAUD_RATE 115200

int main() {
    stdio_usb_init();
    uart_init(UART_ID, BAUD_RATE);
    
    FRESULT fr;
    FATFS fs;
    FIL fil;
    int ret;
    char buf[100];
    char filename[] = "test02.txt";

    // Initialize chosen serial port
    stdio_init_all();
    sleep_ms(10000);

    // Initialize SD card
    printf("Initializing SD card...\n");
    if (!sd_init_driver()) {
        printf("ERROR: Could not initialize SD card\n");
        while (true);
    }
    printf("SD card initialized.\n");

    // Mount drive
    printf("Mounting filesystem...\n");
    fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        printf("ERROR: Could not mount filesystem (%d)\n", fr);
        while (true);
    }
    printf("Filesystem mounted successfully.\n");

    // Open file for writing
    printf("Opening file '%s' for writing...\n", filename);
    fr = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK) {
        printf("ERROR: Could not open file (%d)\n", fr);
        while (true);
    }
    printf("Writing to file...\n");

    // Write something to file
    ret = f_printf(&fil, "This is another test\r\n");
    if (ret < 0) {
        printf("ERROR: Could not write to file (%d)\n", ret);
        f_close(&fil);
        while (true);
    }
    ret = f_printf(&fil, "of writing to an SD card.\r\n");
    if (ret < 0) {
        printf("ERROR: Could not write to file (%d)\n", ret);
        f_close(&fil);
        while (true);
    }

    printf("File write successful.\n");

    // Close file
    fr = f_close(&fil);
    if (fr != FR_OK) {
        printf("ERROR: Could not close file (%d)\n", fr);
        while (true);
    }
    printf("File closed.\n");

    // Open file for reading
    printf("Opening file '%s' for reading...\n", filename);
    fr = f_open(&fil, filename, FA_READ);
    if (fr != FR_OK) {
        printf("ERROR: Could not open file (%d)\n", fr);
        while (true);
    }
    printf("Reading from file '%s':\n---\n");

    // Print every line in file over serial
    while (f_gets(buf, sizeof(buf), &fil)) {
        printf(buf);
    }

    printf("---\nFile read successful.\n");

    // Close file
    fr = f_close(&fil);
    if (fr != FR_OK) {
        printf("ERROR: Could not close file (%d)\n", fr);
        while (true);
    }
    printf("File closed.\n");

    // Loop forever doing nothing
    while (true) {
        sleep_ms(1000);
    }
}
