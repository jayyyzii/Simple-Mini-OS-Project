# Simple-Mini-OS-Project

## 🧩 Gambaran Umum

Simple-Mini-OS-Project adalah proyek untuk membuat *mini OS* dengan kernel sederhana yang mendukung driver simulasi dan system call minimal. Proyek ini dijalankan di `QEMU` emulator dan dapat digunakan untuk melakukan benchmark performa dibandingkan OS modern seperti Linux, Windows, atau Android.

Fitur utama:

- Driver simulasi untuk I/O virtual
- System call minimal:
  - `print` — menampilkan teks ke layar
  - `read` — membaca input
  - `allocate` — alokasi memori sederhana
  - `create_task` — membuat task baru
- Logging hasil benchmark:
  - Waktu akses file
  - Total execution time
- Menampilkan boot screen mini OS
- Task execution sederhana

---

## 📁 Struktur Folder

Simple-Mini-OS-Project \
├── boot \
│ └── grub \
│ └── grub.cfg \
├── build \
│ ├── boot.o \
│ ├── driver.o \
│ ├── kernel.bin \
│ ├── kernel.o \
│ ├── keyboard.o \
│ └── syscall.o \
├── iso \
│ └── boot \
│ ├── grub \
│ │ └── grub.cfg \
│ └── kernel.bin \
├── kernel \
│ ├── boot.s \
│ ├── driver.c \
│ ├── driver.h \
│ ├── kernel.c \
│ ├── keyboard.c \
│ ├── keyboard.h \
│ ├── linker.ld \
│ ├── reboot.h \
│ ├── syscall.c \
│ └── syscall.h \
├── Makefile \
├── minios.iso \
└── README.md

---

## ⚙️ Cara Build dan Jalankan

1. **Download Repository** \
   Buka terminal lalu jalankan:
   ```
   git clone https://github.com/muhammadrossiramadhan/Simple-Mini-OS-Project.git
   ```

2. **Masuk ke directory repository**
   ```
   cd Simple-Mini-OS-Project
   ```

3. **Build kernel dan ISO** \
   Pastikan `Makefile` sudah tersedia, lalu jalankan:
   ```
   make clean; make && make run
   ```

---

📝 Notes

Mini-OS ini menggunakan GRUB sebagai bootloader.

Struktur proyek modular untuk memudahkan pengembangan driver atau system call tambahan.

🔧 Tools yang Digunakan (Dependency)

- GCC & NASM — untuk compile kernel
- LD (linker) — untuk linking binary
- QEMU (32-bit) — untuk menjalankan mini-OS