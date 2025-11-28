#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>

typedef struct {
    int total_writes;
    int total_reads;
    int total_io_time;   // tick IO (dummy)
} DeviceStatus;

void driver_write(const char *s);
void driver_write_dec(uint32_t v);

void init_driver(void);

int  sys_write_block(int block_index, const uint8_t *buf, uint32_t size);
int  sys_read_block(int block_index, uint8_t *buf, uint32_t size);
uint32_t sys_time_us(void);

DeviceStatus io_get_status(void);

#endif

