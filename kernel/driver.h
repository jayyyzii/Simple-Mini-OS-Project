#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>

typedef struct {
    int total_writes;
    int total_reads;
    int total_io_time;   // tick IO
} DeviceStatus;

void driver_write(const char *s);            // "syscall" print
void init_driver(void);                      // reset statistik + vdisk
int  io_write_sector(int sector, const uint8_t *data);
int  io_read_sector(int sector, uint8_t *data);
DeviceStatus io_get_status(void);

#endif

