# Smart Irrigation System IoT (ESP32)

Sistem Irigasi Pintar (Smart Irrigation System) berbasis ESP32 yang dirancang khusus untuk memonitor dan menyiram tanaman secara otomatis berdasarkan umur fase tanaman (Melon & Cabe) dan kelembaban tanah. Sistem ini dilengkapi dengan Web Dashboard lokal yang responsif serta terintegrasi dengan aplikasi Blynk IoT.

## Fitur Utama
1. **Profil Tanaman Pintar**: Penjadwalan penyiraman dinamis (otomatis berubah durasi dan frekuensinya) mengikuti fase pertumbuhan tanaman dari minggu ke minggu (contoh: Fase Semai, Vegetatif, Generatif, Panen).
2. **Web Dashboard Modern**: Antarmuka kontrol via web lokal yang elegan (menggunakan HTML, CSS) untuk memonitor status alat, merubah pengaturan, mengaktifkan mode simulasi presentasi, dan mengatur jadwal kustom.
3. **Integrasi Blynk IoT**: Monitoring dan kontrol pompa air secara real-time dari mana saja menggunakan smartphone melalui aplikasi Blynk.
4. **Sensor Kelembaban Tanah**: Pembacaan ADC untuk mendeteksi apakah tanah dalam keadaan kering (butuh disiram) atau sudah basah (melewati penyiraman).
5. **Real-Time Clock (RTC DS3231)**: Menjaga akurasi waktu penyiraman secara offline, dilengkapi fitur sinkronisasi waktu otomatis dengan NTP saat terhubung ke WiFi.
6. **EEPROM (Preferences)**: Sistem memori non-volatile untuk memastikan data penting seperti jenis tanaman, umur tanaman, dan jadwal kustom tidak hilang meskipun alat mati/mati listrik.

## Tabel Konfigurasi Pin (Wiring)

Berikut adalah panduan wiring komponen ke board ESP32:

| Komponen | Pin Modul | Pin ESP32 | Keterangan Tambahan |
| :--- | :--- | :--- | :--- |
| **LCD 16x2 I2C** | SDA <br> SCL <br> VCC <br> GND | GPIO 21 <br> GPIO 22 <br> 5V / VIN <br> GND | Menampilkan info tanaman, umur minggu, dan kelembaban. Berbagi jalur I2C dengan RTC. |
| **RTC DS3231** | SDA <br> SCL <br> VCC <br> GND | GPIO 21 <br> GPIO 22 <br> 3.3V <br> GND | Menyimpan jadwal offline. |
| **Relay 2-Channel** | IN1 <br> IN2 <br> VCC <br> GND | GPIO 26 <br> GPIO 27 <br> 5V / VIN <br> GND | Mengontrol Pompa 1 (Melon) dan Pompa 2 (Cabe). Menggunakan logika **Active LOW** (LOW = ON, HIGH = OFF). Pompa dipasang ke terminal **NO (Normally Open)**. |
| **Soil Moisture** | A0 <br> VCC <br> GND | GPIO 34 <br> 3.3V <br> GND | Sensor analog pembaca kelembaban tanah (nilai ADC 0 - 4095 dikalibrasi ke 0-100%). |
| **Pompa Air DC** | (+) <br> (-) | Relay NO <br> GND Adaptor | Diberi daya melalui modul relay untuk mengalirkan air ke tanaman. |

## Konfigurasi Blynk (Virtual Pins)
Sistem ini menggunakan pin virtual berikut pada aplikasi Blynk:
- **V0**: Pilihan Tanaman (0=Tidak Ada, 1=Melon, 2=Cabe)
- **V1**: Input Umur Tanaman (Hari)
- **V2**: Indikator / Kontrol Pompa 1 (Melon)
- **V5**: Indikator / Kontrol Pompa 2 (Cabe)
- **V7**: Gauge Kelembaban Tanah (%)
- **V8**: Terminal/Label Status Sistem
- **V9**: Tampilan Jam & Tanggal (RTC)
- **V10**: Tampilan Informasi Detail Tanaman
- **V11**: Tombol Demo/Test (opsional)

## Catatan Khusus
- **Active LOW Relay**: Kode ini telah dikalibrasi untuk mencegah fenomena *auto-on* saat alat pertama kali dihubungkan ke listrik dengan memberikan status `HIGH` sebelum pin dikonfigurasi sebagai output.
- **Simulasi Presentasi**: Web Dashboard dilengkapi tombol khusus `▶ Simulasi` dan `⏹ Stop Simulasi` yang memfasilitasi loncatan minggu instan ke minggu tertentu untuk keperluan demonstrasi atau penilaian tanpa harus menunggu berminggu-minggu secara fisik.