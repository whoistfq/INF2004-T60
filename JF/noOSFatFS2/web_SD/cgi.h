// cgi.h
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "sd_card.h"
#include "ff.h"
#include <stdio.h>
#include "pico/stdlib.h"

void writeContentToFile(FIL *file, const char *filename, const char *content) {
    FRESULT fr;
    int ret;

    // Open file for writing or create if it doesn't exist
    fr = f_open(file, filename, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK) {
        printf("ERROR: Could not open file for writing (%d)\r\n", fr);
        // Handle error appropriately
    }

    // Write content to the file
    ret = f_printf(file, content);
    if (ret < 0) {
        printf("ERROR: Could not write to file (%d)\r\n", ret);
        f_close(file);
        // Handle error appropriately
    }
    printf("Data written to file successfully.\r\n");
}

int fileChecker(const char *fname) {
    FILINFO fInfo;
    FRESULT res = f_stat(fname, &fInfo);
    return (res == FR_OK && (fInfo.fattrib & AM_DIR) == 0); // Returns 1 if the file exists, 0 otherwise
}

void closeFile(FIL *file) {
    FRESULT fr;
    // Close file
    fr = f_close(file);
    if (fr != FR_OK) {
        printf("ERROR: Could not close file (%d)\r\n", fr);
        // Handle error appropriately
    }
}

void deleteFile(const char *fname) {
    FRESULT fr = f_unlink(fname);
    if (fr != FR_OK) {
        printf("ERROR: Could not delete file '%s' (%d)\r\n", fname, fr);
        // Handle error appropriately
    }
    printf("File '%s' deleted\r\n", fname);
}

const char *cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    // Check if a request for LED has been made (/index.cgi?led=x)
    if (strcmp(pcParam[0], "volt") == 0) {
        printf("Value: %s \r\n", pcValue[0]);
        FIL fil;
        const char filename[] = "ip.txt";
        if (fileChecker(filename)) {
            deleteFile(filename);
        }
        writeContentToFile(&fil, filename, pcValue[0]);
        closeFile(&fil);
        return "/index.shtml";
    }

    // Send the index page back to the user
    return "/index.shtml";
}

static const tCGI cgi_handlers[] = {
    {
        "/index.cgi", cgi_led_handler
    },
};

void cgi_init(void) {
    http_set_cgi_handlers(cgi_handlers, LWIP_ARRAYSIZE(cgi_handlers));
}
