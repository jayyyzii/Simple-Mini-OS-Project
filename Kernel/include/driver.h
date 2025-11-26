// driver.h - Interface Driver Virtual
#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>

// Definisi ukuran simulasi
#define SECTOR_SIZE 512
#define MAX_SECTORS 128

// Struktur data untuk status hardware virtual
typedef struct {
    int is_busy;
    int error_code;
    uint32_t total_writes;
    uint32_t total_reads;
} DeviceStatus;

// Inisialisasi driver dan memori virtual device
void init_driver();

// Menulis satu sektor data (512 bytes) ke disk virtual
// Mengembalikan 0 jika sukses, -1 jika error
int io_write_sector(uint32_t sector_idx, const uint8_t* buffer);

// Membaca satu sektor data dari disk virtual
// Mengembalikan 0 jika sukses, -1 jika error
int io_read_sector(uint32_t sector_idx, uint8_t* buffer);

// Mendapatkan status hardware virtual terkini
DeviceStatus io_get_status();

// Wrapper print untuk output ke terminal
void print(const char *s);

#endif

