# 📝 TEKS PRESENTASI - SMART IRRIGATION SYSTEM
## (Naskah Lengkap untuk Diplkan)

---

# 🎤 PEMBUKAAN

---

## SLIDE 1 - SAMBUTAN

> *Assalamualaikum Warahmatullahi Wabarakatuh.*
>
> *Bapak/Ibu dosen serta teman-teman yang saya hormati.*
>
> *Perkenalkan, nama saya **[NAMA LENGKAP]**, NIM **[NOMOR NIM]**.*
>
> *Pada kesempatan kali ini, saya akan mempresentasikan proyek IoT yang telah saya buat dengan judul:*

---

## SLIDE 2 - JUDUL PROYEK

> **"SMART IRRIGATION BERDASARKAN PROFIL TANAMAN"**

> *Proyek ini adalah sistem penyiraman tanaman otomatis yang menggunakan mikrokontroler ESP32, sensor kelembaban tanah, dan dapat dikontrol melalui smartphone.*

---

# 📖 PENDAHULUAN

---

## SLIDE 3 - LATAR BELAKANG

> *Pertama, saya akan menjelaskan latar belakang pembuatan proyek ini.*
>
> *Pada dasarnya, tanaman membutuhkan air untuk bertahan hidup dan tumbuh dengan baik. Namun, ada beberapa masalah yang sering dihadapi dalam penyiraman manual:*

> 1. **Ketidakkonsistenan** - Manusia sering lupa atau tidak punya waktu untuk menyiram secara rutin
>
> 2. **Over-watering atau Under-watering** - Penyiraman yang berlebihan atau terlalu sedikit sama-sama buruk untuk tanaman
>
> 3. **Tidak bisa monitoring jarak jauh** - Petani harus selalu hadir di lokasi untuk mengecek kondisi tanaman
>
> 4. **Kebutuhan air berbeda tiap fase** - Tanaman butuh treatment berbeda di setiap tahap pertumbuhan*

> *Dari masalah-masalah tersebut, saya membuat sistem yang dapat menyiram secara otomatis dengan pemantauan kondisi tanah secara real-time.*

---

## SLIDE 4 - TUJUAN

> *Tujuan dari proyek ini adalah:*
>
> 1. **Otomasi penyiraman** - Tanaman disiram otomatis berdasarkan jadwal yang sudah ditentukan
>
> 2. **Monitoring real-time** - Mengukur kelembaban tanah secara langsung menggunakan sensor
>
> 3. **Kontrol jarak jauh** - Petani dapat mengontrol dan memantau sistem dari mana saja melalui smartphone
>
> 4. **Efisiensi air** - Menggunakan air secara tepat sesuai kebutuhan tanaman
>
> 5. **Adaptif terhadap fase pertumbuhan** - Jadwal penyiraman berubah otomatis sesuai minggu ke berapa tanaman*

---

## SLIDE 5 - FITUR SISTEM

> *Sistem ini memiliki beberapa fitur utama:*

> - **📅 Jadwal Otomatis** - Penyiraman dilakukan secara otomatis berdasarkan waktu yang sudah ditentukan
>
> - **💧 Sensor Kelembaban** - Sensor soil moisture membaca kelembaban tanah secara real-time
>
> - **📱 Kontrol Blynk App** - Monitoring dan kontrol via smartphone menggunakan aplikasi Blynk
>
> - **🕐 RTC Real-time** - Jam akurat menggunakan modul RTC DS3231
>
> - **🌐 Web Dashboard** - Antarmuka web modern untuk mengakses sistem
>
> - **💾 Data Persistence** - Pengaturan tersimpan即使 ESP32 dimatikan
>
> - **🌱 2 Profil Tanaman** - Mendukung tanaman Melon selama 65 hari dan Cabe selama 90 hari*

---

# 🔧 PERANGKAT KERAS

---

## SLIDE 6 - DIAGRAM BLOK

> *Berikut adalah diagram blok dari sistem yang saya buat:*

> *Sistem terdiri dari beberapa komponen utama:*
>
> - **Input**: Sensor Soil Moisture dan RTC
> - **Proses**: Mikrokontroler ESP32
> - **Output**: Pompa Air, LCD 16x2, Relay
> - **Kontrol**: Blynk App dan Web Dashboard*

> *Data dari sensor dibaca oleh ESP32, kemudian diproses untuk menentukan apakah pompa perlu dinyalakan atau tidak. Hasilnya ditampilkan di LCD dan dapat dipantau melalui Blynk App atau Web Dashboard.*

---

## SLIDE 7 - DAFTAR KOMPONEN

> *Berikut adalah daftar komponen yang digunakan dalam proyek ini:*

| No | Komponen | Spesifikasi | Fungsi |
|----|----------|-------------|--------|
| 1 | ESP32 DevKit | 240MHz, 4MB Flash | Mikrokontroler utama |
| 2 | LCD 16x2 I2C | 0x27, 16x2 karakter | Tampilan status |
| 3 | RTC DS3231 | Akurasi tinggi | Jam real-time |
| 4 | Relay 2 Channel | 5V, Active Low | Kontrol pompa |
| 5 | Sensor Soil Moisture | Analog, GPIO 34 | Ukur kelembaban tanah |
| 6 | Pompa Air | DC 12V | Penyiraman otomatis |
| 7 | Kabel & Konektor | - | Penghubung komponen |

---

## SLIDE 8 - KONFIGURASI PIN

> *Berikut adalah konfigurasi pin yang digunakan:*

| Pin GPIO | Fungsi |
|----------|--------|
| GPIO 21 | SDA (I2C) - untuk LCD dan RTC |
| GPIO 22 | SCL (I2C) - untuk LCD dan RTC |
| GPIO 26 | Relay Pompa (Pin 26) |
| GPIO 27 | Relay Pompa (Pin 27) |
| GPIO 34 | Sensor Soil Moisture (Input Analog) |
| LCD Address | 0x27 (I2C) |
| RTC Address | 0x68 (I2C) |

---

# 💻 PERANGKAT LUNAK

---

## SLIDE 9 - ARKITEKTUR SOFTWARE

> *Dari sisi software, sistem ini menggunakan beberapa library:*

> - **Blynk** - Untuk komunikasi dengan smartphone
> - **LiquidCrystal_I2C** - Untuk mengontrol LCD
> - **Preferences** - Untuk menyimpan pengaturan di flash memory ESP32
> - **WebServer** - Untuk membuat web dashboard
> - **WiFi** - Untuk koneksi internet

> *Data disimpan menggunakan Preferences, yaitu flash memory internal ESP32. Ini memungkinkan pengaturan tetap tersimpan meskipun ESP32 dimatikan.*

---

## SLIDE 10 - PROFIL TANAMAN MELON

> *Tanaman pertama yang didukung adalah Melon dengan umur panen sekitar 65 hari:*

| Fase | Minggu | Jadwal | Threshold |
|------|--------|---------|-----------|
| Dormant | 1-2 | 08:00, 16:00 | < 60% |
| Vegetatif | 3-4 | 07:00, 12:00, 17:00 | < 60% |
| Generatif | 5-7 | 06:00, 11:00, 15:00, 18:00 | < 60% |
| Panen | 8-9 | 07:00, 16:00 | < 60% |

> *Jadwal penyiraman berubah otomatis setiap minggu. Semakin tanaman tumbuh, semakin banyak frekuensi penyiramannya.*

---

## SLIDE 11 - PROFIL TANAMAN CABE

> *Tanaman kedua adalah Cabe dengan umur panen sekitar 90 hari:*

| Fase | Minggu | Jadwal | Threshold |
|------|--------|---------|-----------|
| Semai | 1-2 | 07:30, 17:30 | < 65% |
| Vegetatif | 3-6 | 06:30, 11:00, 17:00 | < 65% |
| Generatif | 7-12 | 06:00, 10:30, 16:00, 19:00 | < 65% |
| Panen | 13 | 07:00, 16:30 | < 65% |

---

## SLIDE 12 - FLOWCHART ALUR KERJA

> *Sistem bekerja dengan alur sebagai berikut:*

> 1. **Inisialisasi** - ESP32 menginisialisasi semua komponen (LCD, RTC, WiFi, sensor)
>
> 2. **Baca Sensor** - Sensor soil moisture membaca kelembaban tanah
>
> 3. **Cek Mode** - Jika mode AUTO, cek apakah sudah waktunya menyiram
>
> 4. **Cek Threshold** - Jika kelembaban di bawah threshold, nyalakan pompa
>
> 5. **Update Status** - Tampilkan status di LCD, Blynk, dan Web Dashboard
>
> 6. **Loop** - Ulangi proses dari langkah 2

---

# 📊 HASIL & PENGUJIAN

---

## SLIDE 13 - HASIL PENGUJIAN

> *Sistem telah diuji dengan hasil sebagai berikut:*

> **Pengujian Sensor:**
> - Sensor membaca 0% saat tanah kering
> - Sensor membaca 100% saat tanah basah
> - Rata-rata 10 pembacaan untuk stabilitas

> **Pengujian Pompa:**
> - Pompa menyala sesuai jadwal
> - Pompa mati otomatis setelah durasi selesai
> - Relay berfungsi dengan baik (Active Low)

> **Pengujian Konektivitas:**
> - WiFi terhubung dengan stabil
> - Web dashboard accessible via IP lokal
> - Blynk App berfungsi untuk monitoring

---

## SLIDE 14 - TAMPILAN LCD

> *LCD 16x2 menampilkan informasi real-time:*

> **Baris 1:** Waktu (jam:menit) dan nama tanaman
> **Baris 2:** Persentase kelembaban tanah dan status (mode AUTO/MANUAL atau PUMP ON)

> *Contoh:*
> ```
> 14:30 Melon
> SOIL:55% AUTO
> ```

---

## SLIDE 15 - WEB DASHBOARD

> *Web dashboard menyediakan antarmuka yang modern untuk:*

> - Melihat status sistem secara real-time
> - Memilih jenis tanaman
> - Mengatur umur tanaman
> - Mengontrol pompa secara manual
> - Menambah jadwal kustom
> - Sinkronisasi waktu RTC

---

# 💾 DATABASE & STORAGE

---

## SLIDE 16 - PERTANYAAN TENTANG DATABASE

> *Kalau ditanya tentang database:*

> *"Sistem ini tidak menggunakan database tradisional seperti MySQL atau Firebase."*

> *"Data disimpan secara lokal menggunakan **Preferences** di flash memory ESP32."*

> **Data yang disimpan:**
> - Jenis tanaman (Melon/Cabe)
> - Umur tanaman (1-90 hari)
> - Mode operasi (Auto/Manual)
> - Jadwal kustom (hingga 5 jadwal)

> **Alasan tidak pakai cloud database:**
> - ESP32 memiliki keterbatasan memori
> - Sistem dirancang untuk bekerja standalone tanpa internet
> - Lebih murah dan sederhana

---

# ✅ KESIMPULAN

---

## SLIDE 17 - KESIMPULAN

> *Berdasarkan penjelasan yang telah saya paparkan, dapat disimpulkan bahwa:*

> 1. Sistem Smart Irrigation ini telah berhasil diimplementasikan dengan fitur:
>    - ✅ Penyiraman otomatis berbasis jadwal
>    - ✅ Monitoring kelembaban tanah real-time
>    - ✅ Kontrol via Blynk App & Web Dashboard
>    - ✅ 2 profil tanaman (Melon & Cabe)
>    - ✅ Data persistence via Preferences (EEPROM Flash)

> 2. Sistem mampu menyiram secara otomatis sesuai jadwal dan threshold yang ditentukan, serta dapat dikontrol dari jarak jauh melalui smartphone.

> 3. Sistem ini cocok untuk:
>    - Greenhouse / Rumah Kaca
>    - Urban Farming / Berkebun di Rumah
>    - Proyek Pembelajaran IoT

---

## SLIDE 18 - KELEBIHAN & KEKURANGAN

> **Kelebihan sistem ini:**
> - Sistem autonomous (bisa kerja tanpa internet)
> - Monitoring via smartphone
> - Web dashboard modern
> - Efisiensi air berbasis sensor
> - LCD untuk monitoring offline
> - RTC dengan backup baterai

> **Kekurangan yang masih ada:**
> - Kapasitas storage terbatas (ESP32 Flash)
> - Bergantung WiFi untuk remote monitoring
> - Tidak ada data logging historis ke cloud
> - Threshold masih statis

---

## SLIDE 19 - PENUTUP

> *Demikian presentasi yang dapat saya sampaikan.*
>
> *Sistem ini masih memiliki ruang untuk pengembangan lebih lanjut, seperti:*
> - Penambahan sensor suhu dan kelembaban udara
> - Integrasi dengan cloud database untuk logging historis
> - Penambahan notifikasi push via email/SMS
> - Penggunaan solar panel untuk kekuasaan

> *Atas perhatian dan waktu Bapak/Ibu serta teman-teman, saya ucapkan terima kasih.*
>
> *Wassalamualaikum Warahmatullahi Wabarakatuh.*

---

# ❓ JAWABAN PERTANYAAN

---

## Q1: "Database-nya apa?"

> **Jawaban:**
> *"Sistem ini menggunakan **Preferences (EEPROM Flash)** di ESP32 untuk menyimpan pengaturan secara lokal. Data yang disimpan meliputi: jenis tanaman, umur tanaman, mode operasi, dan jadwal penyiraman kustom. Tidak menggunakan database tradisional karena sistem dirancang untuk bekerja **standalone** tanpa bergantung pada server eksternal."*

---

## Q2: "Kenapa nggak pakai Firebase?"

> **Jawaban:**
> *"ESP32 memiliki keterbatasan memori dan sumber daya. Dengan Preferences, sistem bersifat **autonomous** dan tetap berfungsi meskipun internet mati. Jika menggunakan cloud database, sistem akan sepenuhnya bergantung pada koneksi internet."*

---

## Q3: "Berapa kapasitas penyimpanannya?"

> **Jawaban:**
> *"Sekitar **512KB** flash memory ESP32 yang tersedia untuk Preferences. Yang dipakai hanya beberapa kilobyte saja untuk menyimpan pengaturan."*

---

## Q4: "Bagaimana jika WiFi mati?"

> **Jawaban:**
> *"Sistem tetap berjalan dengan jadwal default. LCD masih menampilkan status real-time. Yang tidak bisa: remote control via Blynk dan web dashboard."*

---

## Q5: "Tombol simulasi untuk apa?"

> **Jawaban:**
> *"Tombol simulasi digunakan untuk **menguji jadwal** tanpa perlu menunggu tanaman benar-benar mencapai minggu tersebut. Misalnya, untuk demo atau debugging, kita bisa simulasi tanaman di minggu ke-3 meskipun tanaman asli masih di minggu ke-1."*

---

## Q6: "Berapa biaya pembuatan?"

> **Jawaban:**
> *"Kurang lebih **Rp 150.000 - Rp 300.000** untuk semua komponen (ESP32, LCD, RTC, Relay, Sensor, Pompa). Tergantung kualitas komponen yang dipilih."*

---

## Q7: "Berapa banyak jadwal per hari?"

> **Jawaban:**
> *"Tergantung fase tanaman:*
> - *Minggu pertama: 2 jadwal per hari*
> - *Minggu tengah: 3 jadwal per hari*
> - *Fase generatif: 4 jadwal per hari"*

---

## Q8: "Mengapa pakai ESP32?"

> **Jawaban:**
> *"ESP32 dipilih karena memiliki fitur:*
> - *WiFi built-in untuk konektivitas*
> - *Bluetooth (jika diperlukan)*
> - *Prosesor dual-core 240MHz*
> - *Ukuran kecil dan harga terjangkau*
> - *Komunitas dan library yang besar"*

---

# 📌 CATATAN PENTING

---

## Hal yang Perlu Disiapkan Saat Presentasi:

1. **Hardware siap展示** - Pastikan semua kabel tersambung dengan baik
2. **Demo live** - Siapkan demo nyala pompa untuk показать fungsionalitas
3. **Web dashboard terbuka** - Tunjukkan interface modern
4. **Blynk App terbuka** - Tunjukkan kontrol via smartphone
5. **Serial Monitor** - Untuk menunjukkan data real-time saat demo

---

## Skrip Demo:

> *"Izinkan saya mendemonstrasikan sistem ini secara langsung..."*

> 1. *"Terlihat di LCD, waktu saat ini adalah [WAKTU] dan tanaman yang dipilih adalah [NAMA TANAMAN]."*
>
> 2. *"Sensor soil moisture membaca kelembaban tanah sebesar [PERSEN]%."*
>
> 3. *"Sekarang saya akan menekan tombol untuk mengaktifkan pompa..."*
>
> 4. *"Pompa menyala! Dan di LCD terlihat status berubah menjadi PUMP ON."*
>
> 5. *"Di Serial Monitor, dapat terlihat log aktivitas sistem."*

---

**~ SELESAI ~**
