// ssi.c
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"

const char *ssi_tags[] = {"volt"};  // Change the SSI tag to "volt"
#define IP_ADDRESS_BUFFER_SIZE 46  // Sufficient for IPv6 addresses
char ip_buffer[IP_ADDRESS_BUFFER_SIZE];

void unmountFile() {
    FRESULT fr;
    // Unmount drive
    fr = f_unmount("0:");
    if (fr != FR_OK) {
        printf("ERROR: Could not unmount filesystem (%d)\r\n", fr);
        // Handle error appropriately
    }

    printf("File Unmounted\r\n");
}

char *readContentFromFile(FIL *file, const char *filename, size_t *buf_size) {
    *buf_size = f_size(file);  // Get the size of the file

    // Ensure the buffer size does not exceed the allocated size
    if (*buf_size > IP_ADDRESS_BUFFER_SIZE - 1) {
        printf("WARNING: File content exceeds buffer size. Truncating content.\r\n");
        *buf_size = IP_ADDRESS_BUFFER_SIZE - 1;
    }

    // Read the entire file content directly into ip_buffer
    FRESULT read_result = f_read(file, ip_buffer, *buf_size, NULL);
    if (read_result != FR_OK) {
        printf("WARNING: Could not read from file (Error Code: %d). Using default value.\r\n", read_result);
        // Set default value ("0.0.0.0")
        strcpy(ip_buffer, "0.0.0.0");
        *buf_size = strlen(ip_buffer);
    }

    ip_buffer[*buf_size] = '\0';  // Null-terminate the buffer

    return ip_buffer;
}


u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
    size_t printed = 0;
    printf("SSI Handler called for index %d\n", iIndex);

    switch (iIndex) {
        case 0:  // volt
            {
                FIL fil;
                const char filename[] = "ip.txt";
                size_t buf_size;
                char *buf = readContentFromFile(&fil, filename, &buf_size);

                // Print or process the content as needed
                printed = snprintf(pcInsert, iInsertLen, "%s", buf);

                // Close the file if the value is not the default one
                if (strcmp(buf, "0.0.0.0") != 0) {
                    f_close(&fil);  // Close the file
                    unmountFile();  // Unmount the file system only when a new value is read
                }

                // No need to free buf here, it's the static ip_buffer
            }
            break;
        // Add more cases if needed for additional SSI tags
        default:  // Handle other SSI tags
            printed = snprintf(pcInsert, iInsertLen, "0.0.0.0");
            break;
    }

    printf("SSI Handler finished. Printed: %zu\n", printed);
    return (u16_t)printed;
}


void ssi_init() {
    http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}
