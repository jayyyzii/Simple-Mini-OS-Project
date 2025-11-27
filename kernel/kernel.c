#include <stdint.h>
#include "driver.h"

// =====================================
// VGA text mode primitive
// =====================================
static uint16_t *const VGA_BUFFER = (uint16_t *)0xB8000;
static int cursor_row = 0;
static int cursor_col = 0;
static const uint8_t VGA_ATTR = 0x0F; // putih di atas hitam

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
// "Syscall" print
// =====================================
void driver_write(const char *s) {
    while (*s) {
        vga_putc(*s++);
    }
}

// =====================================
// Driver I/O virtual
// =====================================
static uint8_t vdisk[1][64];   // 1 sektor 64 byte
static DeviceStatus dev_stat;

void init_driver(void) {
    for (int i = 0; i < 64; i++) {
        vdisk[0][i] = 0;
    }
    dev_stat.total_reads   = 0;
    dev_stat.total_writes  = 0;
    dev_stat.total_io_time = 0;
}

int io_write_sector(int sector, const uint8_t *data) {
    (void)sector; // hanya sektor 0
    for (int i = 0; i < 64; i++) {
        vdisk[0][i] = data[i];
    }
    dev_stat.total_writes++;
    dev_stat.total_io_time += 1;
    return 0;
}

int io_read_sector(int sector, uint8_t *data) {
    (void)sector;
    for (int i = 0; i < 64; i++) {
        data[i] = vdisk[0][i];
    }
    dev_stat.total_reads++;
    dev_stat.total_io_time += 1;
    return 0;
}

DeviceStatus io_get_status(void) {
    return dev_stat;
}

// =====================================
// helper print integer sederhana (tanpa stack protector)
// =====================================
static void print_digit(int n) {
    char buf[2];
    buf[0] = (char)('0' + (n % 10));
    buf[1] = '\0';
    driver_write(buf);
}

// =====================================
// entry point dari boot.s
// =====================================
void kernel_main(void) {
    init_driver();

    driver_write("== MINI OS I/O DEMO ==\n\n");

    // siapkan data
    uint8_t buf[64];
    for (int i = 0; i < 64; i++) {
        buf[i] = (uint8_t)(i * 3);
    }

    // tulis sektor
    int ret = io_write_sector(0, buf);
    if (ret == 0)
        driver_write("[I/O] Write sektor 0: OK\n");
    else
        driver_write("[I/O] Write sektor 0: FAIL\n");

    // baca kembali
    uint8_t check[64];
    ret = io_read_sector(0, check);
    if (ret == 0)
        driver_write("[I/O] Read sektor 0: OK\n");
    else
        driver_write("[I/O] Read sektor 0: FAIL\n");

    // verifikasi
    int ok = 1;
    for (int i = 0; i < 64; i++) {
        if (check[i] != buf[i]) {
            ok = 0;
            break;
        }
    }
    if (ok)
        driver_write("[I/O] Verifikasi data: MATCH\n");
    else
        driver_write("[I/O] Verifikasi data: MISMATCH\n");

    // statistik
    DeviceStatus st = io_get_status();
    driver_write("\n[STAT] total_writes : "); print_digit(st.total_writes); driver_write("\n");
    driver_write("[STAT] total_reads  : ");  print_digit(st.total_reads);  driver_write("\n");
    driver_write("[STAT] io_ticks     : ");  print_digit(st.total_io_time);driver_write("\n");

    driver_write("\n[DONE] Kernel idle.\n");

    while (1) {
        // idle loop
    }
}

