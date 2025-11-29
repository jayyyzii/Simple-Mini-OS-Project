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

void init_driver(void);
void driver_write(const char *s);
void driver_write_dec(uint32_t v);

int sys_write_block(int block_index, const uint8_t *buf, uint32_t size);
int sys_read_block(int block_index, uint8_t *buf, uint32_t size);

uint32_t sys_time_us(void);
DeviceStatus io_get_status(void);

#endif
