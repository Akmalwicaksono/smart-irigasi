# 📊 SLIDE PRESENTASI - SMART IRRIGATION SYSTEM

---

## SLIDE 1: JUDUL

```
╔═══════════════════════════════════════════════════════════════╗
║                                                               ║
║              🌱 SMART IRRIGATION SYSTEM 🌱                    ║
║                                                               ║
║         Sistem Penyiraman Otomatis Berbasis IoT              ║
║                                                               ║
║              Disusun oleh: [Nama Lengkap]                    ║
║              NIM: [Nomor Induk]                              ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
```

---

## SLIDE 2: LATAR BELAKANG MASALAH

```
┌─────────────────────────────────────────────────────────────┐
│  ❌ MASALAH YANG ADA                                         │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  1. Penyiraman manual tidak konsisten                       │
│     - Manusia lupa atau tidak punya waktu                   │
│                                                             │
│  2. Over-watering atau under-watering                       │
│     - Tanaman mati atau tidak optimal                       │
│                                                             │
│  3. Tidak bisa monitoring jarak jauh                        │
│     - Petani harus selalu hadir di lokasi                  │
│                                                             │
│  4. Kebutuhan air berbeda tiap fase pertumbuhan             │
│     - Tanaman butuh treatment berbeda tiap minggu           │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## SLIDE 3: TUJUAN & MANFAAT

```
┌─────────────────────────────────────────────────────────────┐
│  ✅ TUJUAN                                                  │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  1. Otomatisasi penyiraman berdasarkan jadwal               │
│                                                             │
│  2. Monitoring kelembaban tanah real-time                    │
│                                                             │
│  3. Kontrol jarak jauh via smartphone                       │
│                                                             │
│  4. Efisiensi penggunaan air                                │
│                                                             │
│  5. Adaptif terhadap fase pertumbuhan tanaman               │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## SLIDE 4: FITUR UTAMA SISTEM

```
┌─────────────────────────────────────────────────────────────┐
│  ⚙️  FITUR UTAMA                                            │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐         │
│  │ 📅 Jadwal   │  │ 💧 Sensor   │  │ 📱 Kontrol  │         │
│  │ Otomatis    │  │ Kelembaban  │  │ Blynk App   │         │
│  └─────────────┘  └─────────────┘  └─────────────┘         │
│                                                             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐         │
│  │ 🕐 RTC      │  │ 🌐 Web      │  │ 💾 Data     │         │
│  │ Real-time   │  │ Dashboard   │  │ Persistence │         │
│  └─────────────┘  └─────────────┘  └─────────────┘         │
│                                                             │
│  📌 2 Profil Tanaman: Melon (~65 hari) & Cabe (~90 hari)   │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## SLIDE 5: DIAGRAM BLOK SISTEM

```
                        ┌──────────────┐
                        │   SUMBER     │
                        │    AIR       │
                        └──────┬───────┘
                               │
                        ┌──────▼───────┐
                        │    POMPA     │
                        │   (Relay)    │
                        └──────┬───────┘
                               │
         ┌─────────────────────┼─────────────────────┐
         │                     │                     │
    ┌────▼────┐          ┌──────▼──────┐      ┌──────▼──────┐
    │ TANAMAN │          │    ESP32    │      │   SENSOR    │
    │  MELON  │          │   DevKit    │◄────►│   SOIL      │
    └────┬────┘          └──────┬──────┘      │  MOISTURE   │
         │                     │             └──────────────┘
    ┌────▼────┐          ┌──────▼──────┐
    │ TANAMAN │          │   OUTPUT    │
    │  CABE   │          └──────┬──────┘
    └─────────┘                 │
                         ┌──────▼──────┐
                         │  KONTROL    │
                         │   & MONITOR │
                         └─────────────┘
                               │
         ┌─────────────────────┼─────────────────────┐
         │                     │                     │
    ┌────▼────┐          ┌──────▼──────┐      ┌──────▼──────┐
    │  LCD   │          │   BLYNK     │      │    WEB      │
    │  16x2  │          │    APP      │      │  DASHBOARD  │
    └────────┘          └─────────────┘      └─────────────┘
```

---

## SLIDE 6: TOPOLOGI KONEKSI

```
┌─────────────────────────────────────────────────────────────────┐
│                     TOPOLOGI KONEKSI                            │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│                         ESP32 DevKit                            │
│                         ┌───────────────┐                       │
│                         │               │                       │
│            ┌────────────┼─ I2C SDA (21)─┼──────────┐           │
│            │            │ I2C SCL (22) │          │           │
│            │            │ GPIO 26 (P1)  │          │           │
│            │            │ GPIO 27 (P2)  │          │           │
│            │            │ GPIO 34 (AIN) │          │           │
│            │            │               │          │           │
│            │            └───────────────┘          │           │
│            │                                     │           │
│     ┌──────▼──────┐                       ┌──────▼──────┐     │
│     │    LCD      │                       │    RELAY     │     │
│     │   16x2 I2C  │                       │  2 Channel  │     │
│     │   0x27      │                       │  P1 | P2    │     │
│     └─────────────┘                       └──────┬──────┘     │
│     ┌─────────────┐                              │            │
│     │  RTC        │                              │            │
│     │  DS3231    │                       ┌──────▼──────┐     │
│     │  (I2C)     │                       │   POMPA AIR  │     │
│     └─────────────┘                       └─────────────┘     │
│                                                      │            │
│                                              ┌──────▼──────┐     │
│                                              │  SENSOR     │     │
│                                              │  SOIL       │     │
│                                              │  (GPIO34)   │     │
│                                              └─────────────┘     │
│                                                                 │
│  WiFi ──────────► BLYNK CLOUD ──────────► Smartphone           │
│  (NTP Sync)      (Virtual Pins)         (Dashboard App)       │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## SLIDE 7: TABEL KOMPONEN

```
┌─────────────────────────────────────────────────────────────┐
│  📦 DAFTAR KOMPONEN                                         │
├──────────────────┬──────────────────┬──────────────────────┤
│     KOMPONEN     │      MODEL        │       FUNGSI          │
├──────────────────┼──────────────────┼──────────────────────┤
│ Mikrokontroler   │ ESP32 DevKit      │ Unit pemroses utama   │
│ Display          │ LCD 16x2 I2C      │ Tampilan status      │
│ RTC              │ DS3231            │ Jam real-time        │
│ Relay            │ 2 Channel 5V      │ Kontrol pompa        │
│ Sensor           │ Soil Moisture    │ Ukur kelembaban      │
│ Pompa            │ Mini Water Pump  │ Penyiraman otomatis  │
└──────────────────┴──────────────────┴──────────────────────┘
```

---

## SLIDE 8: TABEL PIN KONEKSI

```
┌─────────────────────────────────────────────────────────────┐
│  🔌 KONFIGURASI PIN                                         │
├──────────────────────┬──────────────────────────────────────┤
│       PIN           │          FUNGSI                       │
├──────────────────────┼──────────────────────────────────────┤
│ GPIO 21 (SDA)       │ I2C Data (LCD & RTC)                  │
│ GPIO 22 (SCL)       │ I2C Clock (LCD & RTC)                 │
│ GPIO 26             │ Relay Pompa 1 (Melon)                  │
│ GPIO 27             │ Relay Pompa 2 (Cabe)                   │
│ GPIO 34 (ADC1_6)    │ Sensor Soil Moisture (Input Analog)   │
│ LCD Address 0x27    │ LCD I2C Address                       │
│ RTC Address 0x68    │ RTC DS3231 I2C Address                │
└──────────────────────┴──────────────────────────────────────┘
```

---

## SLIDE 9: ARKITEKTUR SOFTWARE

```
┌─────────────────────────────────────────────────────────────┐
│                  ARKITEKTUR SOFTWARE                        │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────────────────────────────────────────────┐   │
│  │                    USER LAYER                          │   │
│  │  ┌────────────┐  ┌────────────┐  ┌────────────────┐  │   │
│  │  │ Blynk App  │  │Web Dashboard│  │  LCD Display  │  │   │
│  │  └────────────┘  └────────────┘  └────────────────┘  │   │
│  └──────────────────────────────────────────────────────┘   │
│                            │                                 │
│                            ▼                                 │
│  ┌──────────────────────────────────────────────────────┐   │
│  │                  API / PROTOCOL LAYER                 │   │
│  │  ┌────────────┐  ┌────────────┐  ┌────────────────┐  │   │
│  │  │BlynkLib    │  │WebServer   │  │  Wire (I2C)   │  │   │
│  │  │Handler     │  │Handler     │  │  Handler      │  │   │
│  │  └────────────┘  └────────────┘  └────────────────┘  │   │
│  └──────────────────────────────────────────────────────┘   │
│                            │                                 │
│                            ▼                                 │
│  ┌──────────────────────────────────────────────────────┐   │
│  │                  BUSINESS LOGIC LAYER                 │   │
│  │  ┌────────────┐  ┌────────────┐  ┌────────────────┐  │   │
│  │  │ Irrigation │  │ Plant      │  │   Schedule     │  │   │
│  │  │ Controller │  │ Profiles   │  │   Manager      │  │   │
│  │  └────────────┘  └────────────┘  └────────────────┘  │   │
│  └──────────────────────────────────────────────────────┘   │
│                            │                                 │
│                            ▼                                 │
│  ┌──────────────────────────────────────────────────────┐   │
│  │                  DATA LAYER                           │   │
│  │  ┌────────────────────────────────────────────────┐  │   │
│  │  │           Preferences (EEPROM Flash)           │  │   │
│  │  │  - Plant Type  - Age  - Mode  - Schedules     │  │   │
│  │  └────────────────────────────────────────────────┘  │   │
│  └──────────────────────────────────────────────────────┘   │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

## SLIDE 10: PROFIL TANAMAN - MELON

```
┌─────────────────────────────────────────────────────────────┐
│  🍈 PROFIL TANAMAN MELON (~65 Hari)                         │
├──────────────┬────────────┬────────────────┬────────────────┤
│    FASE      │   MINGGU   │    JADWAL      │  THRESHOLD     │
├──────────────┼────────────┼────────────────┼────────────────┤
│              │            │  08:00 (2mnt)  │                │
│   DORMANT    │    1-2     │  16:00 (2mnt)  │    < 60%       │
│              │            │  (2x/hari)     │                │
├──────────────┼────────────┼────────────────┼────────────────┤
│              │            │  07:00 (2.5mnt) │                │
│  VEGETATIF   │    3-4     │  12:00 (2.5mnt)│    < 60%       │
│              │            │  17:00 (2.5mnt)│                │
│              │            │  (3x/hari)     │                │
├──────────────┼────────────┼────────────────┼────────────────┤
│              │            │  06:00 (2.5mnt)│                │
│              │            │  11:00 (2.5mnt)│                │
│ GENERATIF    │    5-7     │  15:00 (2.5mnt)│    < 60%       │
│              │            │  18:00 (2.5mnt)│                │
│              │            │  (4x/hari)     │                │
├──────────────┼────────────┼────────────────┼────────────────┤
│              │            │  07:00 (2.5mnt) │                │
│    PANEN     │    8-9     │  16:00 (2.5mnt) │    < 60%       │
│              │            │  (2x/hari)     │                │
└──────────────┴────────────┴────────────────┴────────────────┘
```

---

## SLIDE 11: PROFIL TANAMAN - CABE

```
┌─────────────────────────────────────────────────────────────┐
│  🌶️ PROFIL TANAMAN CABE (~90 Hari)                          │
├──────────────┬────────────┬────────────────┬────────────────┤
│    FASE      │   MINGGU   │    JADWAL      │  THRESHOLD     │
├──────────────┼────────────┼────────────────┼────────────────┤
│              │            │  07:30 (1.5mnt)│                │
│    SEMAI     │    1-2     │  17:30 (1.5mnt)│    < 65%       │
│              │            │  (2x/hari)     │                │
├──────────────┼────────────┼────────────────┼────────────────┤
│              │            │  06:30 (1.75mnt)│                │
│  VEGETATIF   │    3-6     │  11:00 (1.75mnt)│    < 65%       │
│              │            │  17:00 (1.75mnt)│                │
│              │            │  (3x/hari)     │                │
├──────────────┼────────────┼────────────────┼────────────────┤
│              │            │  06:00 (2mnt)  │                │
│              │            │  10:30 (2mnt)  │                │
│ GENERATIF    │   7-12     │  16:00 (2mnt)  │    < 65%       │
│              │            │  19:00 (2mnt)  │                │
│              │            │  (4x/hari)     │                │
├──────────────┼────────────┼────────────────┼────────────────┤
│              │            │  07:00 (2mnt)  │                │
│    PANEN     │    13      │  16:30 (2mnt)  │    < 65%       │
│              │            │  (2x/hari)     │                │
└──────────────┴────────────┴────────────────┴────────────────┘
```

---

## SLIDE 12: FLOWCHART SISTEM

```
┌─────────────────────────────────────────────────────────────┐
│                    FLOWCHART SISTEM                         │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│                      ┌─────────┐                            │
│                      │  START  │                            │
│                      └────┬────┘                            │
│                           │                                 │
│                           ▼                                 │
│                  ┌─────────────────┐                       │
│                  │  Initialize All  │                       │
│                  │  - Pins, LCD     │                       │
│                  │  - RTC, WiFi     │                       │
│                  │  - Load Prefs    │                       │
│                  └────────┬────────┘                       │
│                           │                                 │
│                           ▼                                 │
│               ┌────────────────────────┐                   │
│               │  Baca Sensor Kelembaban │                   │
│               └────────────┬─────────────┘                   │
│                            │                                │
│                            ▼                                │
│              ┌──────────────────────────┐                   │
│              │  Mode = AUTO?            │                   │
│              └────────────┬─────────────┘                   │
│                    ┌──────┴──────┐                           │
│                   YES           NO                          │
│                    │             │                          │
│                    ▼             ▼                          │
│          ┌─────────────┐   ┌─────────────┐                  │
│          │ Cek Jadwal │   │ Manual Mode │                  │
│          │ & Threshold│   │ Kontrol Via │                  │
│          └──────┬──────┘   │ Blynk/Web   │                  │
│                 │         └──────┬──────┘                  │
│                 ▼                │                         │
│       ┌─────────────────────┐    │                         │
│       │ Moisture < Threshold?│    │                         │
│       └──────────┬───────────┘    │                         │
│           ┌──────┴──────┐         │                         │
│          YES            NO        │                         │
│           │              │         │                         │
│           ▼              ▼         │                         │
│    ┌────────────┐  ┌───────────┐    │                         │
│    │  AKTIFKAN  │  │   SKIP    │    │                         │
│    │   POMPA    │  │ (No Act)  │    │                         │
│    └─────┬──────┘  └─────┬─────┘    │                         │
│          │               │          │                         │
│          └───────┬───────┘          │                         │
│                  │                  │                         │
│                  ▼                  │                         │
│         ┌─────────────────┐        │                         │
│         │ Update LCD/Web/ │◄────────┘                         │
│         │ Blynk Status    │                                  │
│         └────────┬────────┘                                  │
│                  │                                           │
│                  ▼                                           │
│         ┌─────────────────┐                                 │
│         │ Loop Forever    │                                 │
│         └─────────────────┘                                 │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

## SLIDE 13: DATA STORAGE (PENTING!)

```
╔═══════════════════════════════════════════════════════════════╗
║  💾 PERTANYAAN TENTANG DATABASE? JAWAB BEGINI:                ║
╠═══════════════════════════════════════════════════════════════╣
║                                                               ║
║  "Sistem ini TIDAK menggunakan database tradisional            ║
║   seperti MySQL atau Firebase"                                ║
║                                                               ║
║  ┌─────────────────────────────────────────────────────────┐  ║
║  │  STORAGE: Preferences (ESP32 Flash Memory)              │  ║
║  ├─────────────────────────────────────────────────────────┤  ║
║  │                                                         │  ║
║  │  📦 Data yang disimpan:                                 │  ║
║  │     • Jenis tanaman (Melon/Cabe/None)                  │  ║
║  │     • Umur tanaman (1-90 hari)                         │  ║
║  │     • Mode operasi (Auto/Manual)                       │  ║
║  │     • Jadwal kustom ( hingga 5 jadwal)                 │  ║
║  │                                                         │  ║
║  │  ⚡ Karakteristik:                                      │  ║
║  │     • Built-in Flash ESP32 (~512KB)                    │  ║
║  │     • Data persist setelah restart                      │  ║
║  │     • Cepat, murah, no external server                  │  ║
║  │                                                         │  ║
║  │  🌐 Blynk Cloud ( untuk monitoring only):               │  ║
║  │     • Bukan database, hanya untuk remote control        │  ║
║  │     • Data tidak permanen tersimpan di cloud             │  ║
║  │                                                         │  ║
║  └─────────────────────────────────────────────────────────┘  ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
```

---

## SLIDE 14: BLYNK VIRTUAL PINS

```
┌─────────────────────────────────────────────────────────────┐
│  📱 BLYNK VIRTUAL PINS                                       │
├────────────────┬──────────────────────────────────────────────┤
│   VIRTUAL PIN  │              FUNGSI                          │
├────────────────┼──────────────────────────────────────────────┤
│      V0        │ Pilihan Tanaman (0=Tidak Ada, 1=Melon, 2=Cabe) │
│      V1        │ Umur Tanaman (hari)                         │
│      V2        │ Status Pompa 1 (Melon)                      │
│      V3        │ Mode Auto/Manual (0=Manual, 1=Auto)         │
│      V4        │ Kontrol Manual Pompa 1                      │
│      V5        │ Status Pompa 2 (Cabe)                       │
│      V6        │ Kontrol Manual Pompa 2                      │
│      V7        │ Kelembaban Tanah (%)                       │
│      V8        │ Status Sistem (Text)                        │
│      V9        │ Tanggal/Waktu RTC                          │
│      V10       │ Info Tanaman (Nama|Hari|Minggu|Fase)       │
│      V11       │ Tombol Test Pompa (Demo)                   │
│      V12       │ Set Jam Jadwal (0-23)                      │
│      V13       │ Set Menit Jadwal (0-59)                    │
│      V14       │ Set Durasi Penyiraman (5-60 detik)         │
│      V15       │ Tombol Simpan Jadwal                       │
└────────────────┴──────────────────────────────────────────────┘
```

---

## SLIDE 15: HASIL PENGUJIAN

```
┌─────────────────────────────────────────────────────────────┐
│  ✅ HASIL PENGUJIAN                                          │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────────────────────────────────────────────┐   │
│  │  📊 PENGUJIAN SENSOR                                 │   │
│  │  ─────────────────────────────────────────────────   │   │
│  │  • Sensor membaca 0% saat tanah kering (TES)         │   │
│  │  • Sensor membaca 100% saat tanah basah (TES)       │   │
│  │  • Nilai ADC: 4095 (kering) → 0 (basah)            │   │
│  │  • Rata-rata 10 pembacaan untuk stabilitas          │   │
│  └──────────────────────────────────────────────────────┘   │
│                                                              │
│  ┌──────────────────────────────────────────────────────┐   │
│  │  ⚙️ PENGUJIAN RELAY & POMPA                          │   │
│  │  ─────────────────────────────────────────────────   │   │
│  │  • Relay aktif pada logika LOW (Active Low)         │   │
│  │  • Pompa menyala sesuai durasi terjadwal            │   │
│  │  • Pompa mati otomatis setelah timeout               │   │
│  └──────────────────────────────────────────────────────┘   │
│                                                              │
│  ┌──────────────────────────────────────────────────────┐   │
│  │  🌐 PENGUJIAN KONEKTIVITAS                           │   │
│  │  ─────────────────────────────────────────────────   │   │
│  │  • WiFi terhubung dengan stabil                        │   │
│  │  • Web dashboard accessible via IP lokal            │   │
│  │  • Blynk app berfungsi untuk monitoring & kontrol   │   │
│  │  • RTC sinkron dengan NTP server                     │   │
│  └──────────────────────────────────────────────────────┘   │
│                                                              │
│  ┌──────────────────────────────────────────────────────┐   │
│  │  💾 PENGUJIAN DATA PERSISTENCE                       │   │
│  │  ─────────────────────────────────────────────────   │   │
│  │  • Settings tetap tersimpan setelah restart          │   │
│  │  • Tidak perlu konfigurasi ulang                    │   │
│  └──────────────────────────────────────────────────────┘   │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

## SLIDE 16: KELEBIHAN & KEKURANGAN

```
┌─────────────────────────────────────────────────────────────┐
│  ✅ KELEBIHAN                    ❌ KEKURANGAN                │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ✓ Sistem autonomous (bisa        ✗ Kapasitas storage        │
│    kerja tanpa internet)             terbatas (ESP32 Flash)  │
│                                                              │
│  ✓ Monitoring via smartphone     ✗ Bergantung WiFi untuk     │
│    (Blynk App)                     remote monitoring         │
│                                                              │
│  ✓ Web dashboard modern          ✗ Tidak ada data logging   │
│    (modern UI)                      historis ke cloud        │
│                                                              │
│  ✓ 2 profil tanaman dengan        ✗ Hanya 2 jenis tanaman   │
│    jadwal berbeda tiap fase         yang didukung            │
│                                                              │
│  ✓ Efisiensi air karena           ✗ Threshold masih statis  │
│    berbasis sensor                    (belum adaptif)        │
│                                                              │
│  ✓ LCD untuk monitoring           ✗ Tidak ada notifikasi     │
│    offline                           push (email/SMS)         │
│                                                              │
│  ✓ RTC dengan backup baterai      ✗ Jarak pompa terbatas     │
│    (DS3231)                         berdasarkan kabel         │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

## SLIDE 17: KESIMPULAN

```
╔═══════════════════════════════════════════════════════════════╗
║                                                               ║
║                         KESIMPULAN                           ║
║                                                               ║
╠═══════════════════════════════════════════════════════════════╣
║                                                               ║
║  1. Sistem Smart Irrigation ini telah berhasil               ║
║     diimplementasikan dengan fitur:                          ║
║                                                               ║
║     ✅ Penyiraman otomatis berbasis jadwal                   ║
║     ✅ Monitoring kelembaban tanah real-time                  ║
║     ✅ Kontrol via Blynk App & Web Dashboard                 ║
║     ✅ 2 profil tanaman (Melon & Cabe)                       ║
║     ✅ Data persistence via Preferences (EEPROM)             ║
║                                                               ║
║  2. Sistem mampu menyiram secara OTOMATIS                    ║
║     sesuai jadwal dan threshold yang ditentukan,             ║
║     serta dapat dikontrol dari jarak jauh                    ║
║     melalui smartphone.                                       ║
║                                                               ║
║  3. Sistem ini cocok untuk:                                   ║
║     🌱 Greenhouse / Rumah Kaca                               ║
║     🏠 Urban Farming / Berkebun di Rumah                     ║
║     📚 Proyek Belajar IoT                                     ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
```

---

## SLIDE 18: PENUTUP / Q&A

```
╔═══════════════════════════════════════════════════════════════╗
║                                                               ║
║                                                               ║
║                                                               ║
║                                                               ║
║                         Terima Kasih                         ║
║                                                               ║
║                                                               ║
║                     ? ? ? ? ? ? ? ?                          ║
║                                                               ║
║                    PERTANYAAN & JAWAB                        ║
║                                                               ║
║                                                               ║
║                                                               ║
║                                                               ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
```

---

## 📝 CATATAN PENTING UNTUK Q&A

### 1. "Database-nya apa?"
> **Jawab:** "Sistem ini menggunakan **Preferences (EEPROM Flash)** di ESP32 untuk menyimpan pengaturan secara lokal. Tidak menggunakan database tradisional karena sistem dirancang untuk bekerja **standalone** tanpa bergantung pada server eksternal."

### 2. "Kenapa nggak pakai Firebase?"
> **Jawab:** "ESP32 memiliki keterbatasan memori dan sumber daya. Dengan Preferences, sistem bersifat **autonomous** dan tetap berfungsi meskipun internet mati."

### 3. "Berapa kapasitas penyimpanannya?"
> **Jawab:** "Sekitar **512KB** flash memory ESP32, yang dipakai hanya beberapa kilobyte untuk Preferences."

### 4. "Berapa biaya pembuatan?"
> **Jawab:** "Kurang lebih Rp 150.000 - Rp 300.000** untuk semua komponen (ESP32, LCD, RTC, Relay, Sensor, Pompa)."

### 5. "Bagaimana jika WiFi mati?"
> **Jawab:** "Sistem tetap berjalan dengan jadwal default. LCD masih menampilkan status. Yang tidak bisa: remote control via Blynk dan web dashboard."

### 6. "Kenapa pakai Blynk?"
> **Jawab:** "Blynk menyediakan **dashboard app** gratis yang mudah digunakan untuk monitoring dan kontrol jarak jauh via smartphone tanpa perlu coding UI sendiri."
