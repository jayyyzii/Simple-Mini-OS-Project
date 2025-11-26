# TUGAS PROYEK AKHIR ORGANISASI DAN ARSITEKTUR KOMPUTER

## 🎯 **Judul Final Project**

**“Perancangan Mini Operating System Kernel dan Evaluasi Performa pada Satu Studi Kasus Nyata terhadap Sistem Operasi Modern”**

---

### 🧩 **Gambaran Umum Project**

Mahasiswa diminta melakukan 3 hal besar:

### 1️⃣ **Mendesain Mini OS (Kernel Sederhana)**

Komponen minimal yang wajib dibuat:

* **Task Scheduler** (round robin / priority queue)
* **Memory Manager sederhana** (static allocation / simple paging simulation)
* **Driver simulasi** (I/O virtual)
* **System call minimal** (print, read, allocate, create_task)

Mini OS dibuat dalam:

* **C** dan dijalankan di:

  * **Raspbian baremetal**
  * **QEMU emulator**
  * **x86 / ARM simulator**

Atau untuk yang lebih ringan:

* **NachOS**
* **XV6 Lab**
* **OSDev baremetal starter**

---

### 2️⃣ **Mengimplementasikan 1 Kasus Nyata**

Mahasiswa memilih satu studi kasus, lalu **menjalankan program tersebut** di mini-OS dan **membandingkan performanya** di OS modern (Linux/Windows/Android).

Contoh studi kasus:

* Sorting 10.000 data
* CPU-bound task (fibonacci, matrix multiplication)
* I/O-bound task (file read/write simulasi)
* Multitasking 3 proses kalkulasi
* Simulasi server sederhana (request handling)
* Image processing (grayscale)
* Cryptography XOR/ Caesar
* Thread concurrency test

Program **harus dijalankan dua kali**:

1. Di mini-OS buatan mahasiswa
2. Di OS modern di laptop/PC
   → Lalu dibandingkan performanya

---

### 3️⃣ **Benchmark Sistem Operasi Modern**

Mahasiswa menguji OS modern:

* **Windows vs Linux**
* **Linux vs Android**
* **Ubuntu vs Arch**
* **Linux baremetal vs VM**

Parameter yang diukur:

* Latency & throughput
* Scheduling delay
* Context switching time
* File I/O time
* Memory allocation time
* Total execution time

---

## 🛠️ **Output yang Harus Dikumpulkan**

### 1. **Desain Mini OS**

Berisi:

* Diagram kernel
* Scheduler design
* Memory layout
* System call table
* Driver model

### 2. **Kode Program**

Contoh minimum:

* `kernel.c`
* `scheduler.c`
* `memory.c`
* `syscall.c`
* `app.c` (program kasus)

OS mahasiswa **tidak harus didesain sempurna**, tapi minimal menjalankan 1 atau lebih “task” secara terjadwal.

### 3. **Simulasi / Eksekusi**

Menggunakan:

* QEMU (disarankan)
* Bochs
* VirtualBox minimal OS image
* Raspberry Pi (opsional)

Harus menunjukkan:

* Boot screen mini OS
* Task execution
* Logging hasil benchmark

### 4. **Benchmark OS Modern**

Mahasiswa menjalankan program yang sama di:

* Linux / Windows / Android
  Lalu mengukur:
* Execution time
* Memory usage
* Number of context switch (opsional)
* CPU time

### 5. **Analisis Perbandingan**

Mahasiswa harus menjawab:

* Mengapa OS modern jauh lebih cepat?
* Apa keterbatasan desain OS buatan?
* Mana OS modern yang “terbaik” untuk kasus mereka?

---

## 📘 **Struktur Laporan Akhir**

1. **Pendahuluan**
   – Alasan menguji OS modern
2. **Desain Mini OS**
   – Arsitektur kernel, scheduler, memory
3. **Implementasi Kode**
   – Penjelasan modul
4. **Studi Kasus**
   – Program real to run
5. **Evaluasi & Benchmark**
   – Tabel performa
   – Windows vs Linux vs OS Buatan
6. **Diskusi**
   – Analisis OS mana yang paling optimal
   – Kelebihan/kekurangan tiap OS
7. **Kesimpulan**
   – Pemilihan OS terbaik untuk case

---

## 🚀 **Contoh Case Final Project**

Berikut 5 contoh studi kasus nyata untuk dipilih mahasiswa:

---

#### **1️⃣ OS untuk Multitasking Kalkulasi Fibonacci**

Mahasiswa:

* Membuat 3 task
* Scheduler round robin
* Mengukur konteks switching
* Membandingkan runtime vs Linux

---

#### **2️⃣ OS untuk Sorting Task (Multi-task Sorting)**

2 proses sorting jalan paralel.
Ukuran data 10.000 elemen.

Uji:

* Task switch time
* Memory allocation time

---

#### **3️⃣ OS untuk File I/O Simulation**

Membuat driver I/O virtual:

* write
* read

Bandingkan kecepatan file I/O mini-OS vs Linux.

---

#### **4️⃣ OS untuk Server Mini (Request Handler)**

Simulasi server menangani:

* 50 request dummy
* Menggunakan queue

Compare dengan server mini di Linux.

---

#### **5️⃣ OS untuk Image Processing**

Program berjalan di kernel:

* grayscale
* thresholding

Compare hasil & speed.

---

## 🧪 **Rubrik Penilaian (opsional, bisa saya buatkan versi tabel)**

* Arsitektur OS (25%)
* Implementasi kode (25%)
* Studi kasus + program (20%)
* Benchmark & analisis (20%)
* Presentasi (10%)

# PEMBAGIAN TUGAS KELOMPOK

*1. Desain kernel dan Diagram* :
Deskripsi : *Membuat desain arsitektur kernel dan diagramnya.*

1. Naufal
2. Totti
3. rossi
4. Davar

*2. Scheduler Design & Coding* :
Deskripsi : *Membuat scheduler (round robin / priority queue)*

1. Salman
2.zaidan
3.

*3. Memory Manager & Layout*
Deskripsi : *Implementasi manajemen memori sederhana*

1. Wildan
2. Yaya

*4. System Call*
Deskripsi : *Membuat dan mengatur system calls minimal (print, read)*

1. Rafa
2. Wicak

*5. Driver Model*
Deskripsi : *Membuat driver I/O virtual (simulasi baca/tulis)*

1. Rossi
2. Shofi

*6. Studi Kasus Program*
Deksripsi : *Membuat program studi kasus nyata yang akan diuji*

1. zhulva 
2. ame

*7. Benchmark dan Pengujian*
Deksripsi : *Menjalankan program di mini OS dan OS modern, ukur metric*

1. Davar
2. Nova
