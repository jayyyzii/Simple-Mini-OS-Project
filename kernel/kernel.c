// kernel/kernel.c
#include "driver.h"
#include "syscall.h"
#include "reboot.h"
#include <stdint.h>

// Membaca integer desimal dari input (meng-echo tiap karakter).
// Mengembalikan 0 jika input invalid atau kosong.
static int read_int_from_user(void) {
    char buf[16];
    int idx = 0;
    char c;

    // baca sampai newline
    while (1) {
        c = sys_read();
        // echo character
        char tmp[2] = { c, 0 };
        sys_print(tmp);

        if (c == '\r') continue; // ignore CR (Windows style)
        if (c == '\n') break;
        if (idx < (int)(sizeof(buf) - 1)) {
            buf[idx++] = c;
        }
    }
    buf[idx] = 0;

    if (idx == 0) return 0; // no input

    // konversi string ke int (desimal)
    int val = 0;
    for (int i = 0; buf[i]; i++) {
        if (buf[i] < '0' || buf[i] > '9') return 0; // invalid char
        val = val * 10 + (buf[i] - '0');
    }
    return val;
}

void kernel_main(void) {
    init_driver();
    sys_clear_screen();

    sys_print("=== Simulasi I/O 4KB Block (Mini-OS) ===\n\n");

    sys_print("Default block size: ");
    sys_print_dec(BLOCK_SIZE);
    sys_print(" bytes\n");

    // Prompt input jumlah blok
    sys_print("\nMasukkan jumlah blok untuk benchmark (max ");
    sys_print_dec(BLOCK_COUNT);
    sys_print("). Tekan Enter untuk default 1000: ");

    int user_val = read_int_from_user();
    sys_print("\n");

    int blocks_to_test;
    if (user_val <= 0) {
        blocks_to_test = 1000;
        sys_print("Input invalid/empty. Menggunakan default: ");
        sys_print_dec(blocks_to_test);
        sys_print("\n\n");
    } else {
        if (user_val > BLOCK_COUNT) {
            blocks_to_test = BLOCK_COUNT;
            sys_print("Input terlalu besar. Menggunakan maximal: ");
            sys_print_dec(blocks_to_test);
            sys_print("\n\n");
        } else {
            blocks_to_test = user_val;
            sys_print("Menggunakan jumlah blok: ");
            sys_print_dec(blocks_to_test);
            sys_print("\n\n");
        }
    }

    // buffer data
    static uint8_t buf[BLOCK_SIZE];
    static uint8_t check[BLOCK_SIZE];

    for (int i = 0; i < BLOCK_SIZE; i++) buf[i] = (uint8_t)(i & 0xFF);

    sys_print("Mulai write test...\n");

    uint32_t w0 = sys_time_us();
    for (int b = 0; b < blocks_to_test; b++) {
        if (sys_write_block(b, buf, BLOCK_SIZE) != 0) {
            sys_print("ERROR: sys_write_block gagal\n");
            break;
        }
    }
    uint32_t w1 = sys_time_us();

    sys_print("Mulai read & verify test...\n");

    uint32_t r0 = sys_time_us();
    for (int b = 0; b < blocks_to_test; b++) {
        if (sys_read_block(b, check, BLOCK_SIZE) != 0) {
            sys_print("ERROR: sys_read_block gagal\n");
            break;
        }
        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (check[i] != buf[i]) {
                sys_print("ERROR: data mismatch\n");
                goto done;
            }
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

    sys_print("\n=== Hasil Benchmark Mini-OS ===\n");
    sys_print("Jumlah blok          : "); sys_print_dec(blocks_to_test); sys_print("\n");
    sys_print("Write time total     : "); sys_print_dec(write_time); sys_print(" us\n");
    sys_print("Read time total      : "); sys_print_dec(read_time); sys_print(" us\n");
    sys_print("Write latency /blok  : "); sys_print_dec(write_latency_us); sys_print(" us\n");
    sys_print("Read latency /blok   : "); sys_print_dec(read_latency_us); sys_print(" us\n");
    sys_print("Write throughput     : "); sys_print_dec(write_throughput_kb); sys_print(" KB/s (approx)\n");
    sys_print("Read throughput      : "); sys_print_dec(read_throughput_kb); sys_print(" KB/s (approx)\n");

done:
    {
        DeviceStatus st = sys_get_status();
        sys_print("\n=== STAT ===");
        sys_print("\ntotal_writes : "); sys_print_dec((uint32_t)st.total_writes);
        sys_print("\ntotal_reads  : "); sys_print_dec((uint32_t)st.total_reads);
        sys_print("\nio_ticks     : "); sys_print_dec((uint32_t)st.total_io_time);
        sys_print("\n");
    }

    sys_print("\nBenchmark selesai.\n");
    sys_print("Press any key to force quit...\n");
    sys_read();    // baca 1 karakter apa saja
    reboot_qemu();

    while (1) { }
}
