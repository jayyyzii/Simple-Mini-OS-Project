#include <stdint.h>
#include "driver.h"

// =====================================
// VGA text mode primitive
// =====================================
static uint16_t *const VGA_BUFFER = (uint16_t *)0xB8000;
static int cursor_row = 0;
static int cursor_col = 0;
static const uint8_t VGA_ATTR = 0x0F;

static void putchar_at(char c, int row, int col) {
    VGA_BUFFER[row * 80 + col] = ((uint16_t)VGA_ATTR << 8) | (uint8_t)c;
}

static void vga_putc(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else {
        putchar_at(c, cursor_row, cursor_col);
        cursor_col++;
        if (cursor_col >= 80) {
            cursor_col = 0;
            cursor_row++;
        }
    }
    if (cursor_row >= 25) {
        cursor_row = 0;
    }
}

// =====================================
// Primitive print
// =====================================
void driver_write(const char *s) {
    while (*s) {
        vga_putc(*s++);
    }
}

void driver_write_dec(uint32_t v) {
    char buf[16];
    int i = 0;

    if (v == 0) {
        driver_write("0");
        return;
    }

    while (v > 0 && i < 15) {
        buf[i++] = (char)('0' + (v % 10));
        v /= 10;
    }

    while (i > 0) {
        vga_putc(buf[--i]);
    }
}

// =====================================
// Virtual block device + syscall I/O
// =====================================
#define BLOCK_SIZE  4096
#define BLOCK_COUNT 1000

static uint8_t vdisk[BLOCK_SIZE * BLOCK_COUNT];
static DeviceStatus dev_stat;

// timer dummy: waktu naik per operasi blok, bukan per pemanggilan sys_time_us
static uint32_t fake_time_us = 0;

void init_driver(void) {
    for (int i = 0; i < BLOCK_SIZE * BLOCK_COUNT; i++) {
        vdisk[i] = 0;
    }

    dev_stat.total_reads  = 0;
    dev_stat.total_writes = 0;
    dev_stat.total_io_time = 0;

    fake_time_us = 0;
}

// simulasi: 50 us per operasi 4KB
#define IO_TIME_PER_BLOCK_US 200   // misal 200 µs per 4KB

static int io_write_block(int block, const uint8_t *data) {
    if (block < 0 || block >= BLOCK_COUNT) {
        return -1;
    }

    uint32_t base = (uint32_t)block * BLOCK_SIZE;
    for (int i = 0; i < BLOCK_SIZE; i++) {
        vdisk[base + i] = data[i];
    }

    dev_stat.total_writes++;
    dev_stat.total_io_time += 1;
    fake_time_us += IO_TIME_PER_BLOCK_US;

    return 0;
}

static int io_read_block(int block, uint8_t *data) {
    if (block < 0 || block >= BLOCK_COUNT) {
        return -1;
    }

    uint32_t base = (uint32_t)block * BLOCK_SIZE;
    for (int i = 0; i < BLOCK_SIZE; i++) {
        data[i] = vdisk[base + i];
    }

    dev_stat.total_reads++;
    dev_stat.total_io_time += 1;
    fake_time_us += IO_TIME_PER_BLOCK_US;

    return 0;
}

// syscall wrapper
int sys_write_block(int block_index, const uint8_t *buf, uint32_t size) {
    if (size != BLOCK_SIZE) return -1;
    return io_write_block(block_index, buf);
}

int sys_read_block(int block_index, uint8_t *buf, uint32_t size) {
    if (size != BLOCK_SIZE) return -1;
    return io_read_block(block_index, buf);
}

uint32_t sys_time_us(void) {
    // sekarang hanya mengembalikan nilai, tidak menambah waktu lagi
    return fake_time_us;
}

DeviceStatus io_get_status(void) {
    return dev_stat;
}

// =====================================
// Kernel entry: benchmark 4KB block
// =====================================
void kernel_main(void) {
    init_driver();

    driver_write("=== Simulasi I/O 4KB Block (Mini-OS) ===\n\n");

    static uint8_t buf[BLOCK_SIZE];
    static uint8_t check[BLOCK_SIZE];

    for (int i = 0; i < BLOCK_SIZE; i++) {
        buf[i] = (uint8_t)(i & 0xFF);
    }

    int blocks_to_test = 1000;

    driver_write("BLOCK_SIZE : ");
    driver_write_dec(BLOCK_SIZE);
    driver_write(" bytes\n");

    driver_write("BLOCK_COUNT : ");
    driver_write_dec(blocks_to_test);
    driver_write("\n\n");

    // -----------------------------
    // Write 1000 blok
    // -----------------------------
    uint32_t w0 = sys_time_us();

    for (int b = 0; b < blocks_to_test; b++) {
        if (sys_write_block(b, buf, BLOCK_SIZE) != 0) {
            driver_write("ERROR: sys_write_block gagal\n");
            break;
        }
    }

    uint32_t w1 = sys_time_us();

    // -----------------------------
    // Read 1000 blok
    // -----------------------------
    uint32_t r0 = sys_time_us();

    for (int b = 0; b < blocks_to_test; b++) {
        if (sys_read_block(b, check, BLOCK_SIZE) != 0) {
            driver_write("ERROR: sys_read_block gagal\n");
            break;
        }
    }

    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (check[i] != buf[i]) {
            driver_write("ERROR: data mismatch\n");
            goto done;
        }
    }

    uint32_t r1 = sys_time_us();

    // -----------------------------
    // Hasil waktu & throughput
    // -----------------------------
    uint32_t write_time = w1 - w0;   // us
    uint32_t read_time  = r1 - r0;   // us

    uint32_t total_bytes = (uint32_t)BLOCK_SIZE * (uint32_t)blocks_to_test;

    uint32_t write_throughput_kb = 0;
    uint32_t read_throughput_kb  = 0;

    if (write_time > 0) {
        write_throughput_kb =
            (total_bytes / 1024U) * 1000U / write_time;
    }

    if (read_time > 0) {
        read_throughput_kb =
            (total_bytes / 1024U) * 1000U / read_time;
    }

    uint32_t write_latency_us = 0;
    uint32_t read_latency_us  = 0;

    if (blocks_to_test > 0) {
        write_latency_us = write_time / (uint32_t)blocks_to_test;
        read_latency_us  = read_time  / (uint32_t)blocks_to_test;
    }

    driver_write("=== Hasil Benchmark Mini-OS ===\n");
    driver_write("Write time total      : ");
    driver_write_dec(write_time);
    driver_write(" us\n");

    driver_write("Read  time total      : ");
    driver_write_dec(read_time);
    driver_write(" us\n");

    driver_write("Write latency /blok   : ");
    driver_write_dec(write_latency_us);
    driver_write(" us\n");

    driver_write("Read  latency /blok   : ");
    driver_write_dec(read_latency_us);
    driver_write(" us\n");

    driver_write("Write throughput      : ");
    driver_write_dec(write_throughput_kb);
    driver_write(" KB/s (approx)\n");

    driver_write("Read  throughput      : ");
    driver_write_dec(read_throughput_kb);
    driver_write(" KB/s (approx)\n");

done:
    {
        DeviceStatus st = io_get_status();
        driver_write("\n[STAT] total_writes : ");
        driver_write_dec((uint32_t)st.total_writes);
        driver_write("\n[STAT] total_reads  : ");
        driver_write_dec((uint32_t)st.total_reads);
        driver_write("\n[STAT] io_ticks     : ");
        driver_write_dec((uint32_t)st.total_io_time);
        driver_write("\n");
    }

    driver_write("\n[DONE] Kernel idle.\n");

    while (1) {
        // idle
    }
}

