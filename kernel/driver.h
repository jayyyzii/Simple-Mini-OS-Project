#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>

#define BLOCK_SIZE 4096
#define BLOCK_COUNT 1000

typedef struct {
    uint32_t total_reads;
    uint32_t total_writes;
    uint32_t total_io_time;
} DeviceStatus;

/* low-level driver API (implemented in driver.c) */
void init_driver(void);
void driver_clear_screen(void);
void driver_write(const char *s);
void driver_write_dec(uint32_t v);

/* low-level block I/O primitives (driver-level, not syscall) */
int io_write_block(int block, const uint8_t *buf);
int io_read_block(int block, uint8_t *buf);

/* driver status/time access */
uint32_t driver_time_us(void);
DeviceStatus driver_status(void);

#endif
