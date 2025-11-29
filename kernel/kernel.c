#include "driver.h"
#include "syscall.h"
#include <stdint.h>

void kernel_main(void) {
    init_driver();
    sys_clear_screen();

    sys_print("=== Simulasi I/O 4KB Block (Mini-OS) ===\n\n");

    static uint8_t buf[BLOCK_SIZE];
    static uint8_t check[BLOCK_SIZE];

    for (int i = 0; i < BLOCK_SIZE; i++) buf[i] = (uint8_t)(i & 0xFF);

    int blocks_to_test = 1000;

    sys_print("BLOCK_SIZE  : "); sys_print_dec(BLOCK_SIZE); sys_print(" bytes\n");
    sys_print("BLOCK_COUNT : "); sys_print_dec(blocks_to_test); sys_print("\n\n");

    uint32_t w0 = sys_time_us();
    for (int b = 0; b < blocks_to_test; b++) {
        if (sys_write_block(b, buf, BLOCK_SIZE) != 0) { sys_print("ERROR write\n"); break; }
    }
    uint32_t w1 = sys_time_us();

    uint32_t r0 = sys_time_us();
    for (int b = 0; b < blocks_to_test; b++) {
        if (sys_read_block(b, check, BLOCK_SIZE) != 0) { sys_print("ERROR read\n"); break; }
        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (check[i] != buf[i]) { sys_print("ERROR: data mismatch\n"); goto done; }
        }
    }
    uint32_t r1 = sys_time_us();

    uint32_t write_time = w1 - w0;
    uint32_t read_time  = r1 - r0;
    uint32_t total_bytes = (uint32_t)BLOCK_SIZE * (uint32_t)blocks_to_test;

    uint32_t write_throughput_kb = 0;
    uint32_t read_throughput_kb  = 0;
    if (write_time > 0) write_throughput_kb = (total_bytes / 1024U) * 1000U / write_time;
    if (read_time > 0)  read_throughput_kb  = (total_bytes / 1024U) * 1000U / read_time;

    uint32_t write_latency_us = 0;
    uint32_t read_latency_us  = 0;
    if (blocks_to_test > 0) {
        write_latency_us = write_time / (uint32_t)blocks_to_test;
        read_latency_us  = read_time / (uint32_t)blocks_to_test;
    }

    sys_print("=== Hasil Benchmark Mini-OS ===\n");
    sys_print("Write time total     : "); sys_print_dec(write_time); sys_print(" us\n");
    sys_print("Read time total      : "); sys_print_dec(read_time); sys_print(" us\n");
    sys_print("Write latency /blok  : "); sys_print_dec(write_latency_us); sys_print(" us\n");
    sys_print("Read latency /blok   : "); sys_print_dec(read_latency_us); sys_print(" us\n");
    sys_print("Write throughput     : "); sys_print_dec(write_throughput_kb); sys_print(" KB/s (approx)\n");
    sys_print("Read throughput      : "); sys_print_dec(read_throughput_kb); sys_print(" KB/s (approx)\n");

done:
    {
        DeviceStatus st = sys_get_status();
        sys_print("\n[STAT] total_writes : "); sys_print_dec((uint32_t)st.total_writes);
        sys_print("\n[STAT] total_reads  : "); sys_print_dec((uint32_t)st.total_reads);
        sys_print("\n[STAT] io_ticks     : "); sys_print_dec((uint32_t)st.total_io_time);
        sys_print("\n\n[DONE] Kernel idle.\n");
    }

    while (1) { }
}