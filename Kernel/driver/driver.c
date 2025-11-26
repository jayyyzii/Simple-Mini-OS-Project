// driver.c - Implementasi Driver Simulasi Hardware Virtual
#include "driver.h"
#include <string.h> // Untuk memcpy simulasi transfer data
#include <stdio.h>  // Untuk output logging ke terminal

// Simulasi hardware penyimpanan: array RAM sebagai disk virtual
static uint8_t virtual_disk_storage[MAX_SECTORS][SECTOR_SIZE];
static DeviceStatus driver_status;

void init_driver() {
    memset(virtual_disk_storage, 0, sizeof(virtual_disk_storage));
    driver_status.is_busy = 0;
    driver_status.error_code = 0;
    driver_status.total_reads = 0;
    driver_status.total_writes = 0;

    // Tampilkan inisialisasi ke terminal
    printf("[DRIVER] Virtual Block Device Initialized, Size: %d KB\n",
           (MAX_SECTORS * SECTOR_SIZE) / 1024);
}

int io_write_sector(uint32_t sector_idx, const uint8_t* buffer) {
    if (sector_idx >= MAX_SECTORS) 
        return -1; // Indeks sektor invalid

    driver_status.is_busy = 1; // Simulasi lock hardware

    // Simulasi delay I/O
    for(volatile int i=0; i<1000; i++);

    // Salin data buffer ke disk virtual
    memcpy(virtual_disk_storage[sector_idx], buffer, SECTOR_SIZE);

    driver_status.total_writes++;
    driver_status.is_busy = 0; // Lepas lock
    return 0;
}

int io_read_sector(uint32_t sector_idx, uint8_t* buffer) {
    if (sector_idx >= MAX_SECTORS)
        return -1; // Indeks sektor invalid

    driver_status.is_busy = 1;

    // Simulasi delay baca data
    for(volatile int i=0; i<1000; i++);

    memcpy(buffer, virtual_disk_storage[sector_idx], SECTOR_SIZE);

    driver_status.total_reads++;
    driver_status.is_busy = 0;
    return 0;
}

DeviceStatus io_get_status() {
    return driver_status;
}

void print(const char *s) {
    // Output langsung ke terminal (stdout)
    printf("%s", s);
}

