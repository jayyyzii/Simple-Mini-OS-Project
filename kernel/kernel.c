#include "driver.h"
#include "syscall.h"

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

    driver_write("BLOCK_SIZE  : ");
    driver_write_dec(BLOCK_SIZE);
    driver_write(" bytes\n");
    driver_write("BLOCK_COUNT : ");
    driver_write_dec(blocks_to_test);
    driver_write("\n\n");

    // Write 1000 blok
    uint32_t w0 = sys_time_us();
    for (int b = 0; b < blocks_to_test; b++) {
        if (sys_write_block(b, buf, BLOCK_SIZE) != 0) {
            driver_write("ERROR: sys_write_block gagal\n");
            break;
        }
    }
    uint32_t w1 = sys_time_us();

    // Read 1000 blok
    uint32_t r0 = sys_time_us();
    for (int b = 0; b < blocks_to_test; b++) {
        if (sys_read_block(b, check, BLOCK_SIZE) != 0) {
            driver_write("ERROR: sys_read_block gagal\n");
            break;
        }
        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (check[i] != buf[i]) {
                driver_write("ERROR: data mismatch\n");
                goto done;
            }
        }
    }
    uint32_t r1 = sys_time_us();

    uint32_t write_time = w1 - w0; // us
    uint32_t read_time  = r1 - r0; // us
    uint32_t total_bytes = (uint32_t)BLOCK_SIZE * (uint32_t)blocks_to_test;

    uint32_t write_throughput_kb = 0;
    uint32_t read_throughput_kb  = 0;
    if (write_time > 0) {
        write_throughput_kb = (total_bytes / 1024U) * 1000U / write_time;
    }
    if (read_time > 0) {
        read_throughput_kb = (total_bytes / 1024U) * 1000U / read_time;
    }

    uint32_t write_latency_us = 0;
    uint32_t read_latency_us  = 0;
    if (blocks_to_test > 0) {
        write_latency_us = write_time / (uint32_t)blocks_to_test;
        read_latency_us  = read_time / (uint32_t)blocks_to_test;
    }

    driver_write("=== Hasil Benchmark Mini-OS ===\n");
    driver_write("Write time total     : ");
    driver_write_dec(write_time);
    driver_write(" us\n");
    driver_write("Read time total      : ");
    driver_write_dec(read_time);
    driver_write(" us\n");
    driver_write("Write latency /blok  : ");
    driver_write_dec(write_latency_us);
    driver_write(" us\n");
    driver_write("Read latency /blok   : ");
    driver_write_dec(read_latency_us);
    driver_write(" us\n");
    driver_write("Write throughput     : ");
    driver_write_dec(write_throughput_kb);
    driver_write(" KB/s (approx)\n");
    driver_write("Read throughput      : ");
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
        driver_write("\n\n[DONE] Kernel idle.\n");
    }

    while (1) {
        // idle loop
    }
}

