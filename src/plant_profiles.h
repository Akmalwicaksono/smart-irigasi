/**
 * ============================================
 * SMART IRRIGATION - PLANT PROFILES
 * ============================================
 * Profil tanaman dengan jadwal penyiraman
 * berdasarkan umur dan kebutuhan air
 * ============================================
 */

#ifndef PLANT_PROFILES_H
#define PLANT_PROFILES_H

#include <Arduino.h>

// ============================================
// KONFIGURASI THRESHOLD KELembaban TANAH
// Nilai default - dapat diubah dari Blynk atau kode
#define DEFAULT_THRESHOLD_MIN   55   // Batas bawah default (%)
#define DEFAULT_THRESHOLD_MAX   75   // Batas atas default (%)
#define MELON_THRESHOLD_MIN     60   // Melon
#define MELON_THRESHOLD_MAX     80
// Threshold Cabe sekarang bersifat dinamis per fase,
// diatur oleh fungsi getCabeThresholds()

// ============================================
// KONSTANTA UMUR TANAMAN (dalam hari)
#define MELON_HARI_AWAL       1
#define MELON_HARI_TANAMAN    65    // Total umur panen ~65 hari
#define CABE_HARI_AWAL        1
#define CABE_HARI_TANAMAN     90    // Total umur panen ~90 hari

// ============================================
// DEFINISI JENIS TANAMAN
enum PlantType {
    PLANT_NONE = 0,
    PLANT_MELON = 1,
    PLANT_CABE = 2
};

// ============================================
// STRUKTUR JADWAL PENYIRAMAN
struct IrrigationSchedule {
    int jam;          // Jam penyiraman (0-23)
    int menit;        // Menit penyiraman (0-59)
    int durasiDetik;  // Durasi penyiraman dalam detik
};

// ============================================
// STRUKTUR PROFIL TANAMAN
struct PlantProfile {
    const char* nama;                    // Nama tanaman
    PlantType jenis;                     // Jenis tanaman
    int umurMinimal;                     // Umur minimal (hari)
    int umurMaksimal;                    // Umur maksimal (hari)
    int jumlahJadwal;                    // Jumlah jadwal per hari
    const IrrigationSchedule* jadwal;    // Array jadwal penyiraman
};

// ============================================
// JADWAL PENYIRAMAN MELON (JADWAL ASLI)
// ============================================

const IrrigationSchedule MELON_MINGGU_1_2[] = {
    {8, 0, 120},    // 08:00 - 2 menit
    {16, 0, 120}    // 16:00 - 2 menit
};

const IrrigationSchedule MELON_MINGGU_3_4[] = {
    {7, 0, 150},    // 07:00 - 2.5 menit (150 detik)
    {12, 0, 150},   // 12:00 - 2.5 menit
    {17, 0, 150}    // 17:00 - 2.5 menit
};

const IrrigationSchedule MELON_MINGGU_5_7[] = {
    {6, 0, 150},    // 06:00 - 2.5 menit
    {11, 0, 150},   // 11:00 - 2.5 menit
    {15, 0, 150},   // 15:00 - 2.5 menit
    {18, 0, 150}    // 18:00 - 2.5 menit
};

const IrrigationSchedule MELON_MINGGU_8_9[] = {
    {7, 0, 150},    // 07:00 - 2.5 menit
    {16, 0, 150}    // 16:00 - 2.5 menit
};

// ============================================
// JADWAL PENYIRAMAN CABE (JADWAL ASLI)
// ============================================
// Failsafe timeout diset 120 detik, pompa otomatis mati jika target kelembaban tercapai
const IrrigationSchedule CABE_JADWAL_2X[] = {
    {6, 30, 120},    // 06:30
    {16, 0, 120}     // 16:00
};

const IrrigationSchedule CABE_JADWAL_3X[] = {
    {6, 30, 120},    // 06:30
    {12, 0, 120},    // 12:00
    {16, 30, 120}    // 16:30
};

// ============================================
// PROFIL TANAMAN MELON
const PlantProfile PROFILE_MELON = {
    "Melon",
    PLANT_MELON,
    MELON_HARI_AWAL,
    MELON_HARI_TANAMAN,
    4,  // Max jadwal per periode
    NULL  // Akan diisi dinamis berdasarkan umur
};

// ============================================
// PROFIL TANAMAN CABE
const PlantProfile PROFILE_CABE = {
    "Cabe",
    PLANT_CABE,
    CABE_HARI_AWAL,
    CABE_HARI_TANAMAN,
    4,  // Max jadwal per periode
    NULL  // Akan diisi dinamis berdasarkan umur
};

// ============================================
// FUNGSI UNTUK MENDAPATKAN JADWAL BERDASARKAN UMUR
inline int getMingguKe(int hari) {
    return (hari - 1) / 7 + 1;
}

inline const IrrigationSchedule* getMelonSchedule(int hari, int& jumlahJadwal) {
    int minggu = getMingguKe(hari);

    if (minggu <= 2) {
        jumlahJadwal = sizeof(MELON_MINGGU_1_2) / sizeof(IrrigationSchedule);
        return MELON_MINGGU_1_2;
    } else if (minggu <= 4) {
        jumlahJadwal = sizeof(MELON_MINGGU_3_4) / sizeof(IrrigationSchedule);
        return MELON_MINGGU_3_4;
    } else if (minggu <= 7) {
        jumlahJadwal = sizeof(MELON_MINGGU_5_7) / sizeof(IrrigationSchedule);
        return MELON_MINGGU_5_7;
    } else {
        jumlahJadwal = sizeof(MELON_MINGGU_8_9) / sizeof(IrrigationSchedule);
        return MELON_MINGGU_8_9;
    }
}

inline const IrrigationSchedule* getCabeSchedule(int hari, int& jumlahJadwal) {
    if (hari <= 35) { // Fase 1, 2, 3 (Hari 0-35) -> 2x sehari
        jumlahJadwal = sizeof(CABE_JADWAL_2X) / sizeof(IrrigationSchedule);
        return CABE_JADWAL_2X;
    } else if (hari <= 69) { // Fase 4, 5, 6 (Hari 36-69) -> 3x sehari
        jumlahJadwal = sizeof(CABE_JADWAL_3X) / sizeof(IrrigationSchedule);
        return CABE_JADWAL_3X;
    } else { // Fase 7 (Hari 70+) -> 2x sehari
        jumlahJadwal = sizeof(CABE_JADWAL_2X) / sizeof(IrrigationSchedule);
        return CABE_JADWAL_2X;
    }
}

// ============================================
// FUNGSI THRESHOLD DINAMIS
// ============================================
inline void getMelonThresholds(int hari, int& minMoisture, int& maxMoisture) {
    minMoisture = MELON_THRESHOLD_MIN;
    maxMoisture = MELON_THRESHOLD_MAX;
}

inline void getCabeThresholds(int hari, int& minMoisture, int& maxMoisture) {
    if (hari <= 7) { 
        minMoisture = 60; maxMoisture = 75; // Pindah Tanam
    } else if (hari <= 21) { 
        minMoisture = 55; maxMoisture = 72; // Vegetatif Awal
    } else if (hari <= 35) { 
        minMoisture = 55; maxMoisture = 70; // Vegetatif Aktif
    } else if (hari <= 42) { 
        minMoisture = 60; maxMoisture = 75; // Pembungaan
    } else if (hari <= 62) { 
        minMoisture = 60; maxMoisture = 75; // Pembentukan Buah
    } else if (hari <= 69) { 
        minMoisture = 55; maxMoisture = 72; // Pembesaran Buah
    } else { 
        minMoisture = 50; maxMoisture = 65; // Pematangan
    }
}

// ============================================
// FUNGSI HELPER
inline const char* getPlantName(PlantType type) {
    switch(type) {
        case PLANT_MELON: return "Melon";
        case PLANT_CABE: return "Cabe";
        default: return "Tidak Ada";
    }
}

inline const char* getPlantPhase(int hari, PlantType type) {
    int minggu = getMingguKe(hari);

    if (type == PLANT_MELON) {
        if (minggu <= 2) return "Dormant";
        else if (minggu <= 4) return "Vegetatif";
        else if (minggu <= 7) return "Generatif";
        else return "Panen";
    } else if (type == PLANT_CABE) {
        // Fase Cabe sesuai data penelitian
        if (hari <= 7) return "Pindah Tanam";
        else if (hari <= 21) return "Vegetatif Awal";
        else if (hari <= 35) return "Vegetatif Aktif";
        else if (hari <= 42) return "Pembungaan";
        else if (hari <= 62) return "Pemb. Buah";
        else if (hari <= 69) return "Pembesaran";
        else return "Pematangan";
    }
    return "Unknown";
}

#endif // PLANT_PROFILES_H