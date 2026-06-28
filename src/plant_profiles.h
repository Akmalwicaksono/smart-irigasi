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
#define DEFAULT_THRESHOLD_MIN   40   // Batas bawah default (%)
#define DEFAULT_THRESHOLD_MAX   70   // Batas atas default (%)
#define MELON_THRESHOLD_MIN     35   // Melon butuh air lebih banyak
#define MELON_THRESHOLD_MAX     70
#define CABE_THRESHOLD_MIN      40   // Cabe butuh air sedang-konsisten
#define CABE_THRESHOLD_MAX      75

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

const IrrigationSchedule CABE_MINGGU_1_2[] = {
    {7, 30, 90},    // 07:30 - 1.5 menit (90 detik)
    {17, 30, 90}    // 17:30 - 1.5 menit
};

const IrrigationSchedule CABE_MINGGU_3_6[] = {
    {6, 30, 105},   // 06:30 - ~1.75 menit (105 detik)
    {11, 0, 105},   // 11:00 - ~1.75 menit
    {17, 0, 105}    // 17:00 - ~1.75 menit
};

const IrrigationSchedule CABE_MINGGU_7_12[] = {
    {6, 0, 120},    // 06:00 - ~2 menit (120 detik)
    {10, 30, 120},  // 10:30 - ~2 menit
    {16, 0, 120},   // 16:00 - ~2 menit
    {19, 0, 120}    // 19:00 - ~2 menit
};

const IrrigationSchedule CABE_MINGGU_13[] = {
    {7, 0, 120},    // 07:00 - 2 menit
    {16, 30, 120}   // 16:30 - 2 menit
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
    int minggu = getMingguKe(hari);

    if (minggu <= 2) {
        jumlahJadwal = sizeof(CABE_MINGGU_1_2) / sizeof(IrrigationSchedule);
        return CABE_MINGGU_1_2;
    } else if (minggu <= 6) {
        jumlahJadwal = sizeof(CABE_MINGGU_3_6) / sizeof(IrrigationSchedule);
        return CABE_MINGGU_3_6;
    } else if (minggu <= 12) {
        jumlahJadwal = sizeof(CABE_MINGGU_7_12) / sizeof(IrrigationSchedule);
        return CABE_MINGGU_7_12;
    } else {
        jumlahJadwal = sizeof(CABE_MINGGU_13) / sizeof(IrrigationSchedule);
        return CABE_MINGGU_13;
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
        if (minggu <= 2) return "Semai";
        else if (minggu <= 6) return "Vegetatif";
        else if (minggu <= 12) return "Generatif";
        else return "Panen";
    }
    return "Unknown";
}

#endif // PLANT_PROFILES_H