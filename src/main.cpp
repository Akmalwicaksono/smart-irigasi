/**
 * ============================================
 * SMART IRRIGATION SYSTEM - IoT Project
 * ============================================
 * Sistem penyiraman otomatis berbasis ESP32
 * dengan LCD, RTC, dan Sensor Soil Moisture
 * ============================================
 *
 * Hardware:
 * - ESP32 DevKit
 * - LCD 16x2 I2C (SDA=G21, SCL=G22)
 * - RTC DS3231 (SDA=G21, SCL=G22)
 * - Relay 2 Channel (G26, G27)
 * - Sensor Soil Moisture (G34)
 * - Baterai UPS 18650 (backup)
 *
 * Virtual Pins Blynk:
 * V0  - Pilihan Tanaman (0=Tidak Ada, 1=Melon, 2=Cabe)
 * V1  - Umur Tanaman (hari)
 * V2  - Status Pompa 1 (G26)
 * V3  - Mode Auto/Manual (0=Manual, 1=Auto)
 * V4  - Kontrol Manual Pompa 1
 * V5  - Status Pompa 2 (G27)
 * V6  - Kontrol Manual Pompa 2
 * V7  - Kelembaban Tanah (%)
 * V8  - Status Sistem (Text)
 * V9  - Tanggal/Waktu RTC
 * V10 - Info Tanaman
 * V11 - TOMBOL TEST POMPA (DEMO)
 * V12 - Set Jam Jadwal (0-23)
 * V13 - Set Menit Jadwal (0-59)
 * V14 - Set Durasi (5-60 detik)
 * V15 - Tombol Simpan Jadwal
 * ============================================
 */

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <Wire.h>
#include <WebServer.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <Preferences.h>
#include "plant_profiles.h"

// ============================================
// WEB SERVER
// ============================================
WebServer server(80);

// HTML Dashboard - Improved Version
const char HTML_PAGE[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="id">
<head>
<title>Smart Irrigation</title>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link href="https://fonts.googleapis.com/css2?family=Outfit:wght@400;500;600;700&display=swap" rel="stylesheet">
<style>
:root {
  --bg-color: #0F172A;
  --card-bg: #1E293B;
  --text-main: #F8FAFC;
  --text-muted: #94A3B8;
  --primary: #38BDF8;
  --primary-hover: #0EA5E9;
  --success: #10B981;
  --danger: #EF4444;
  --warning: #F59E0B;
  --border: #334155;
}
* { box-sizing: border-box; margin: 0; padding: 0; }
body {
  font-family: 'Outfit', sans-serif;
  background-color: var(--bg-color);
  color: var(--text-main);
  min-height: 100vh;
  padding: 20px;
}
.container { max-width: 800px; margin: 0 auto; }
h1 {
  text-align: center;
  font-size: 32px;
  font-weight: 700;
  margin-bottom: 30px;
  color: var(--text-main);
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 12px;
}
.card {
  background: var(--card-bg);
  border: 1px solid var(--border);
  border-radius: 16px;
  padding: 24px;
  margin-bottom: 20px;
  box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1);
  transition: transform 0.2s;
}
.card:hover { border-color: #475569; }
.card h2 {
  font-size: 18px;
  font-weight: 600;
  margin-bottom: 20px;
  color: var(--primary);
  display: flex;
  align-items: center;
  gap: 8px;
}
/* Status Grid */
.status-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(140px, 1fr));
  gap: 16px;
}
.status-item {
  background: #0F172A;
  padding: 16px;
  border-radius: 12px;
  border: 1px solid var(--border);
  display: flex;
  flex-direction: column;
}
.status-item .label {
  color: var(--text-muted);
  font-size: 12px;
  font-weight: 500;
  text-transform: uppercase;
  display: flex;
  align-items: center;
  gap: 6px;
}
.status-item .label svg { width: 14px; height: 14px; }
.status-item .value {
  font-size: 18px;
  font-weight: 700;
  margin-top: 8px;
}
.status-item .sub-value {
  font-size: 12px;
  color: var(--text-muted);
  margin-top: 4px;
  font-weight: 400;
}
.status-item .value.on { color: var(--success); }
.status-item .value.off { color: var(--danger); }
.status-item .value.primary { color: var(--primary); }

/* Form Controls */
.form-row { display: flex; gap: 16px; margin-bottom: 16px; }
.form-group { flex: 1; }
.form-group label {
  display: block; color: var(--text-muted); font-size: 13px; font-weight: 500; margin-bottom: 8px;
}
.form-group input, .form-group select {
  width: 100%; padding: 12px 16px; background: #0F172A; border: 1px solid var(--border);
  color: var(--text-main); border-radius: 8px; font-size: 15px; font-family: 'Outfit', sans-serif;
  transition: all 0.2s;
}
.form-group input:focus, .form-group select:focus {
  outline: none; border-color: var(--primary); box-shadow: 0 0 0 2px rgba(56, 189, 248, 0.2);
}

/* Buttons */
.btn-group { display: flex; gap: 12px; flex-wrap: wrap; margin-top: 20px; }
.btn {
  padding: 14px 24px; border: none; border-radius: 8px; font-size: 15px; font-weight: 600;
  font-family: 'Outfit', sans-serif; cursor: pointer; transition: all 0.2s; flex: 1; min-width: 120px;
  display: flex; align-items: center; justify-content: center; gap: 8px;
}
.btn-primary { background: var(--primary); color: #0F172A; }
.btn-primary:hover { background: var(--primary-hover); transform: translateY(-2px); }
.btn-success { background: var(--success); color: white; }
.btn-success:hover { background: #059669; transform: translateY(-2px); }
.btn-danger { background: var(--danger); color: white; }
.btn-danger:hover { background: #DC2626; transform: translateY(-2px); }
.btn-warning { background: var(--warning); color: #0F172A; }
.btn-warning:hover { background: #D97706; transform: translateY(-2px); }
.btn:active { transform: translateY(0); }

/* Plant Selection */
.plant-cards { display: grid; grid-template-columns: repeat(3, 1fr); gap: 16px; margin-bottom: 24px; }
.plant-card {
  padding: 20px 16px; background: #0F172A; border: 2px solid var(--border); border-radius: 12px;
  text-align: center; cursor: pointer; transition: all 0.2s; color: var(--text-muted);
}
.plant-card:hover { border-color: #64748B; color: var(--text-main); }
.plant-card.selected { border-color: var(--primary); background: rgba(56, 189, 248, 0.05); color: var(--text-main); }
.plant-card .icon { margin-bottom: 12px; display: flex; justify-content: center;}
.plant-card .name { font-weight: 600; font-size: 15px; }

/* Alert */
.alert {
  padding: 14px 20px; border-radius: 8px; margin-top: 16px; font-size: 14px; font-weight: 500;
  display: none; border-left: 4px solid;
}
.alert.success { background: rgba(16, 185, 129, 0.1); color: var(--success); border-color: var(--success); }
.alert.error { background: rgba(239, 68, 68, 0.1); color: var(--danger); border-color: var(--danger); }
@keyframes pulse { 0%, 100% { opacity: 1; } 50% { opacity: 0.5; } }
.pumping { animation: pulse 0.8s infinite; color: var(--primary); margin-left: 4px;}
@media (max-width: 600px) { .form-row { flex-direction: column; gap: 12px; } .btn-group { flex-direction: column; gap: 12px; } .btn { min-width: 100%; } }
.footer { text-align: center; color: var(--text-muted); margin-top: 32px; font-size: 13px; padding-bottom: 20px; }
.profile-table { width: 100%; border-collapse: collapse; margin-top: 10px; font-size: 14px; }
.profile-table th, .profile-table td { padding: 12px; border: 1px solid var(--border); text-align: left; }
.profile-table th { background: rgba(0,0,0,0.2); font-weight: 600; color: var(--text-muted); }
.profile-title { display: flex; align-items: center; gap: 8px; font-weight: 600; font-size: 16px; margin-bottom: 12px; margin-top: 16px; color: var(--text-main); }
.sched-list { display: flex; flex-direction: column; gap: 8px; margin-top: 16px; }
.sched-item { display: flex; justify-content: space-between; align-items: center; background: #0F172A; padding: 12px 16px; border-radius: 8px; border: 1px solid var(--border); }
.sched-info { font-weight: 600; font-size: 15px; }
.sched-actions { display: flex; gap: 8px; }
.btn-sm { padding: 6px 12px; font-size: 13px; border-radius: 6px; }

svg.icon-md { width: 18px; height: 18px; }
</style>
</head>
<body>
<div class="container">
<h1>
  <svg width="28" height="28" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 22a7 7 0 0 0 7-7c0-2-1-3.9-3-5.5s-3.5-4-4-6.5c-.5 2.5-2 4.9-4 6.5C6 11.1 5 13 5 15a7 7 0 0 0 7 7z"/></svg>
  Smart Irrigation
</h1>

<!-- Status Card -->
<div class="card">
<h2>
  <svg class="icon-md" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="3" width="7" height="9"/><rect x="14" y="3" width="7" height="5"/><rect x="14" y="12" width="7" height="9"/><rect x="3" y="16" width="7" height="5"/></svg>
  Dashboard Status
</h2>
<div class="status-grid">
  <div class="status-item">
    <div class="label"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"/><polyline points="12 6 12 12 16 14"/></svg> Waktu & Tanggal</div>
    <div class="value primary" id="time">--:--:--</div>
    <div class="sub-value" id="date">--/--/----</div>
  </div>
  <div class="status-item">
    <div class="label"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M11 20A7 7 0 0 1 9.8 6.1C15.5 5 17 4.48 19 2c1 2 2 4.18 2 8 0 5.5-4.78 10-10 10Z"/><path d="M2 21c0-3 1.85-5.36 5.08-6C9.5 14.52 12 13 13 12"/></svg> Info Tanaman</div>
    <div class="value" id="plant">-</div>
    <div class="sub-value" id="phase">Fase: - | Mg: -</div>
  </div>
  <div class="status-item">
    <div class="label"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 22a7 7 0 0 0 7-7c0-2-1-3.9-3-5.5s-3.5-4-4-6.5c-.5 2.5-2 4.9-4 6.5C6 11.1 5 13 5 15a7 7 0 0 0 7 7z"/></svg> Kelembaban</div>
    <div class="value" id="moisture">-%</div>
  </div>
  <div class="status-item">
    <div class="label"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"/><path d="M12 16v-4"/><path d="M12 8h.01"/></svg> Pompa 1</div>
    <div class="value" id="pump1">OFF</div>
  </div>
  <div class="status-item">
    <div class="label"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"/><path d="M12 16v-4"/><path d="M12 8h.01"/></svg> Pompa 2</div>
    <div class="value" id="pump2">OFF</div>
  </div>
  <div class="status-item">
    <div class="label"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"/><line x1="12" y1="16" x2="12" y2="12"/><line x1="12" y1="8" x2="12.01" y2="8"/></svg> Sistem</div>
    <div class="value primary" id="mode">AUTO</div>
  </div>
  <div class="status-item">
    <div class="label"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M5 12.55a11 11 0 0 1 14.08 0"/><path d="M1.42 9a16 16 0 0 1 21.16 0"/><path d="M8.53 16.11a6 6 0 0 1 6.95 0"/><line x1="12" y1="20" x2="12.01" y2="20"/></svg> Koneksi</div>
    <div class="value" id="wifi" style="font-size:16px;">--</div>
    <div class="sub-value" id="blynk">Blynk: --</div>
  </div>
</div>
</div>

<!-- Plant Selection Card -->
<div class="card">
<h2>
  <svg class="icon-md" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M11 20A7 7 0 0 1 9.8 6.1C15.5 5 17 4.48 19 2c1 2 2 4.18 2 8 0 5.5-4.78 10-10 10Z"/><path d="M2 21c0-3 1.85-5.36 5.08-6C9.5 14.52 12 13 13 12"/></svg>
  Pengaturan Tanaman
</h2>
<div class="plant-cards">
  <div class="plant-card" id="card0" onclick="selectPlant(0)">
    <div class="icon"><svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><line x1="18" y1="6" x2="6" y2="18"/><line x1="6" y1="6" x2="18" y2="18"/></svg></div>
    <div class="name">Tidak Ada</div>
  </div>
  <div class="plant-card" id="card1" onclick="selectPlant(1)">
    <div class="icon"><svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="13" r="9"/><path d="M12 4c-1.5 0-3-1.5-3-3"/></svg></div>
    <div class="name">Melon</div>
  </div>
  <div class="plant-card" id="card2" onclick="selectPlant(2)">
    <div class="icon"><svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M11 2c0 2.5-3.5 3-3.5 6 0 4 6 7.5 9.5 7.5 1.5 0 2.5-1 2.5-2 0-3-10-6.5-10-11.5"/></svg></div>
    <div class="name">Cabe</div>
  </div>
</div>

<div class="form-row">
  <div class="form-group">
    <label>Umur Tanaman (hari)</label>
    <input type="number" id="plantAge" min="1" max="90" value="1">
  </div>
  <div class="form-group">
    <label>Mode Operasi</label>
    <select id="modeSelect">
      <option value="1">Otomatis</option>
      <option value="0">Manual</option>
    </select>
  </div>
</div>

<div class="btn-group">
  <button class="btn btn-primary" onclick="saveSettings()">
    <svg class="icon-md" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg> 
    Simpan Pengaturan
  </button>
  <button class="btn btn-warning" onclick="syncTime()">
    <svg class="icon-md" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"/><polyline points="12 6 12 12 16 14"/></svg>
    Sync Jam RTC
  </button>
</div>
<div class="alert" id="alertBox"></div>
</div>

<!-- Plant Profile Table Card -->
<div class="card" id="profileCard" style="display:none;">
<h2>
  <svg class="icon-md" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M4 19.5v-15A2.5 2.5 0 0 1 6.5 2H20v20H6.5a2.5 2.5 0 0 1 0-5H20"/></svg>
  Profil Tanaman
</h2>
<div id="tableMelon" style="display:none;">
  <div class="profile-title" style="justify-content: space-between;">
    <div>🍈 Melon (~65 hari)</div>
    <button class="btn btn-sm btn-danger" onclick="stopSimulasi(1)" style="padding:4px 8px; min-width:auto;">⏹ Stop</button>
  </div>
  <div style="overflow-x:auto;">
    <table class="profile-table">
      <tr><th>Fase</th><th>Minggu</th><th>Jadwal</th><th>Threshold</th><th>Aksi</th></tr>
      <tr><td>Dormant</td><td>1-2</td><td>08:00, 16:00 (2 menit)</td><td>&lt; 35%</td><td><button class="btn btn-sm btn-primary" onclick="simulasi(1, 1)" style="padding:4px 8px; min-width:auto;">▶ Tes</button></td></tr>
      <tr><td>Vegetatif</td><td>3-4</td><td>07:00, 12:00, 17:00 (2-3 menit)</td><td>&lt; 35%</td><td><button class="btn btn-sm btn-primary" onclick="simulasi(1, 3)" style="padding:4px 8px; min-width:auto;">▶ Tes</button></td></tr>
      <tr><td>Generatif</td><td>5-7</td><td>06:00, 11:00, 15:00, 18:00 (2-3 menit)</td><td>&lt; 35%</td><td><button class="btn btn-sm btn-primary" onclick="simulasi(1, 5)" style="padding:4px 8px; min-width:auto;">▶ Tes</button></td></tr>
      <tr><td>Panen</td><td>8-9</td><td>07:00, 16:00 (2.5 menit)</td><td>&lt; 35%</td><td><button class="btn btn-sm btn-primary" onclick="simulasi(1, 8)" style="padding:4px 8px; min-width:auto;">▶ Tes</button></td></tr>
    </table>
  </div>
</div>
<div id="tableCabe" style="display:none;">
  <div class="profile-title" style="justify-content: space-between;">
    <div>🌶️ Cabe (~90 hari)</div>
    <button class="btn btn-sm btn-danger" onclick="stopSimulasi(2)" style="padding:4px 8px; min-width:auto;">⏹ Stop</button>
  </div>
  <div style="overflow-x:auto;">
    <table class="profile-table">
      <tr><th>Fase</th><th>Minggu</th><th>Jadwal</th><th>Threshold</th><th>Aksi</th></tr>
      <tr><td>Semai</td><td>1-2</td><td>07:30, 17:30 (1.5 menit)</td><td>&lt; 40%</td><td><button class="btn btn-sm btn-primary" onclick="simulasi(2, 1)" style="padding:4px 8px; min-width:auto;">▶ Tes</button></td></tr>
      <tr><td>Vegetatif</td><td>3-6</td><td>06:30, 11:00, 17:00 (1.5-2 menit)</td><td>&lt; 40%</td><td><button class="btn btn-sm btn-primary" onclick="simulasi(2, 3)" style="padding:4px 8px; min-width:auto;">▶ Tes</button></td></tr>
      <tr><td>Generatif</td><td>7-12</td><td>06:00, 10:30, 16:00, 19:00 (1.5-2.5 menit)</td><td>&lt; 40%</td><td><button class="btn btn-sm btn-primary" onclick="simulasi(2, 7)" style="padding:4px 8px; min-width:auto;">▶ Tes</button></td></tr>
      <tr><td>Panen</td><td>13</td><td>07:00, 16:30 (2 menit)</td><td>&lt; 40%</td><td><button class="btn btn-sm btn-primary" onclick="simulasi(2, 13)" style="padding:4px 8px; min-width:auto;">▶ Tes</button></td></tr>
    </table>
  </div>
</div>
</div>

<!-- Pump Control Card -->
<div class="card">
<h2>
  <svg class="icon-md" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"/><path d="M12 16v-4"/><path d="M12 8h.01"/></svg>
  Kontrol Manual
</h2>
<div class="btn-group">
  <button class="btn btn-success" onclick="testPump1()">Pompa 1 (ON)</button>
  <button class="btn btn-success" onclick="testPump2()">Pompa 2 (ON)</button>
  <button class="btn btn-danger" onclick="stopPumps()">
    <svg class="icon-md" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="3" width="18" height="18" rx="2" ry="2"/></svg>
    STOP Semua
  </button>
</div>
</div>

<!-- Schedule Card -->
<div class="card">
<h2>
  <svg class="icon-md" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="4" width="18" height="18" rx="2" ry="2"/><line x1="16" y1="2" x2="16" y2="6"/><line x1="8" y1="2" x2="8" y2="6"/><line x1="3" y1="10" x2="21" y2="10"/></svg>
  Penjadwalan
</h2>
<div class="form-row">
  <div class="form-group">
    <label>Jam (0-23)</label>
    <input type="number" id="schedHour" min="0" max="23" value="8">
  </div>
  <div class="form-group">
    <label>Menit (0-59)</label>
    <input type="number" id="schedMin" min="0" max="59" value="0">
  </div>
  <div class="form-group">
    <label>Durasi (detik)</label>
    <input type="number" id="schedDur" min="5" max="60" value="10">
  </div>
</div>
<div class="btn-group">
  <button class="btn btn-primary" onclick="saveSchedule()">
    <svg class="icon-md" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg>
    Simpan Jadwal
  </button>
</div>
<div class="sched-list" id="customScheduleList">
  <!-- Dynamic content will be loaded here -->
</div>
</div>

<div class="footer">Smart Irrigation System | Modern UI Edition</div>
</div>

<script>
let currentPlant = 0;
let initialLoad = true;

function showAlert(msg, type) {
  var alert = document.getElementById('alertBox');
  alert.textContent = msg;
  alert.className = 'alert ' + type;
  alert.style.display = 'block';
  setTimeout(() => alert.style.display = 'none', 3000);
}

function selectPlant(plant) {
  currentPlant = plant;
  document.querySelectorAll('.plant-card').forEach(c => c.classList.remove('selected'));
  document.getElementById('card' + plant).classList.add('selected');
}

function updateStatus() {
  fetch('/status').then(r => r.json()).then(data => {
    document.getElementById('time').innerHTML = data.time;
    document.getElementById('date').innerHTML = data.date;
    
    if (data.plant === '-' || data.plant === 'Tidak Ada') {
      document.getElementById('plant').innerHTML = '<span style="color:#64748b; font-size:14px; font-weight:500;">Tidak Ada Tanaman</span>';
    } else {
      document.getElementById('plant').innerHTML = 
        '<div style="display:flex; flex-wrap:wrap; gap:6px; margin-top:2px;">' +
        '<span style="background:#10B981; color:#fff; padding:2px 8px; border-radius:12px; font-size:13px; font-weight:600; box-shadow: 0 2px 4px rgba(16,185,129,0.2);">' + data.plant + '</span>' +
        '<span style="background:#3B82F6; color:#fff; padding:2px 8px; border-radius:12px; font-size:13px; font-weight:600; box-shadow: 0 2px 4px rgba(59,130,246,0.2);">Hari ' + data.plantAge + '</span>' +
        '<span style="background:#F59E0B; color:#fff; padding:2px 8px; border-radius:12px; font-size:13px; font-weight:600; box-shadow: 0 2px 4px rgba(245,158,11,0.2);">Mg ' + data.week + '</span>' +
        '<span style="background:#8B5CF6; color:#fff; padding:2px 8px; border-radius:12px; font-size:13px; font-weight:600; box-shadow: 0 2px 4px rgba(139,92,246,0.2);">' + data.phase + '</span>' +
        '</div>';
    }
    document.getElementById('phase').innerHTML = '';
    
    document.getElementById('moisture').innerHTML = data.moisture + '%';
    document.getElementById('mode').innerHTML = data.mode ? 'AUTO' : 'MANUAL';

    var p1 = document.getElementById('pump1');
    p1.innerHTML = data.pump1 ? 'ON <span class="pumping">●</span>' : 'OFF';
    p1.className = 'value ' + (data.pump1 ? 'on' : 'off');

    var p2 = document.getElementById('pump2');
    p2.innerHTML = data.pump2 ? 'ON <span class="pumping">●</span>' : 'OFF';
    p2.className = 'value ' + (data.pump2 ? 'on' : 'off');
    
    var wifiEl = document.getElementById('wifi');
    wifiEl.innerHTML = data.wifi ? 'WiFi ON' : 'WiFi OFF';
    wifiEl.className = 'value ' + (data.wifi ? 'on' : 'off');
    
    var blynkEl = document.getElementById('blynk');
    blynkEl.innerHTML = 'Blynk: ' + (data.blynk ? 'ON' : 'OFF') + ' | ' + data.ip;

    if (initialLoad) {
      document.getElementById('plantAge').value = data.plantAge;
      document.getElementById('modeSelect').value = data.mode ? 1 : 0;
      currentPlant = data.plantType;
      selectPlant(currentPlant);
      initialLoad = false;
    }

    // Render tables
    var pCard = document.getElementById('profileCard');
    if (data.plantType == 1 || data.plantType == 2) {
      pCard.style.display = 'block';
      document.getElementById('tableMelon').style.display = (data.plantType == 1) ? 'block' : 'none';
      document.getElementById('tableCabe').style.display = (data.plantType == 2) ? 'block' : 'none';
    } else {
      pCard.style.display = 'none';
    }

    // Render schedule list
    var schedList = document.getElementById('customScheduleList');
    if (data.useCustom) {
      if (data.schedules && data.schedules.length > 0) {
        var html = '<div class="profile-title" style="margin-top:0;">Daftar Jadwal Aktif</div>';
        data.schedules.forEach((sch, idx) => {
          var hh = sch.h < 10 ? '0'+sch.h : sch.h;
          var mm = sch.m < 10 ? '0'+sch.m : sch.m;
          html += '<div class="sched-item">';
          html += '<div class="sched-info">⏱️ ' + hh + ':' + mm + ' <span style="color:var(--text-muted); font-size:13px; font-weight:normal;">(' + sch.d + ' detik)</span></div>';
          html += '<div class="sched-actions">';
          html += '<button class="btn btn-sm btn-danger" onclick="deleteSchedule('+idx+')">Hapus</button>';
          html += '</div></div>';
        });
        html += '<button class="btn btn-sm btn-warning" style="margin-top:8px;" onclick="toggleCustom(0)">Matikan Jadwal Kustom</button>';
        schedList.innerHTML = html;
      } else {
        schedList.innerHTML = '<div style="color:var(--text-muted); font-size:14px; text-align:center;">Jadwal kustom kosong, menggunakan jadwal default.</div>';
      }
    } else {
      schedList.innerHTML = '<div style="color:var(--text-muted); font-size:14px; text-align:center;">Menggunakan jadwal otomatis dari Profil Tanaman.</div>';
    }
  }).catch(e => console.log('Error fetching status'));
}

function deleteSchedule(idx) {
  fetch('/control?cmd=schedule&action=delete&idx=' + idx)
  .then(() => { showAlert('Jadwal dihapus!', 'error'); updateStatus(); });
}

function toggleCustom(val) {
  fetch('/control?cmd=schedule&action=toggle&val=' + val)
  .then(() => { showAlert('Pengaturan jadwal diubah', 'success'); updateStatus(); });
}

function saveSettings() {
  var age = document.getElementById('plantAge').value;
  var mode = document.getElementById('modeSelect').value;
  fetch('/control?cmd=settings&plant=' + currentPlant + '&age=' + age + '&mode=' + mode)
  .then(() => { 
    showAlert('Pengaturan berhasil disimpan!', 'success'); 
    initialLoad = true; // Force reload forms
    updateStatus(); 
  });
}

function testPump1() { fetch('/control?cmd=pump1').then(() => { showAlert('Pompa 1 dinyalakan!', 'success'); updateStatus(); }); }
function testPump2() { fetch('/control?cmd=pump2').then(() => { showAlert('Pompa 2 dinyalakan!', 'success'); updateStatus(); }); }
function stopPumps() { fetch('/control?cmd=stop').then(() => { showAlert('Semua pompa dihentikan!', 'error'); updateStatus(); }); }

function saveSchedule() {
  var h = document.getElementById('schedHour').value;
  var m = document.getElementById('schedMin').value;
  var d = document.getElementById('schedDur').value;
  fetch('/control?cmd=schedule&action=add&hour=' + h + '&min=' + m + '&dur=' + d)
  .then(() => { showAlert('Jadwal ditambahkan!', 'success'); updateStatus(); });
}

function simulasi(plant, week) {
  var age = (week * 7) - 6;
  fetch('/control?cmd=settings&plant=' + plant + '&age=' + age + '&mode=1')
  .then(() => {
    showAlert('Simulasi Minggu ke-' + week + ' diaktifkan!', 'success');
    initialLoad = true;
    updateStatus();
  });
}

function stopSimulasi(plant) {
  fetch('/control?cmd=settings&plant=' + plant + '&age=1&mode=1')
  .then(() => {
    showAlert('Simulasi dihentikan (Kembali ke Hari 1)', 'error');
    initialLoad = true;
    updateStatus();
  });
}

function syncTime() {
  showAlert('Menyinkronkan waktu...', 'success');
  fetch('/control?cmd=synctime')
  .then(() => { showAlert('Waktu RTC tersinkronisasi!', 'success'); });
}

setInterval(updateStatus, 1000);
updateStatus();
</script>
</body>
</html>
)=====";

// ============================================
// DRIVER RTC DS3231 VIA I2C (Tanpa Library)
// ============================================
#define RTC_ADDR 0x68

class SimpleRTC {
public:
    int year, month, day, hour, minute, second;

    bool begin() {
        Wire.beginTransmission(RTC_ADDR);
        return (Wire.endTransmission() == 0);
    }

    void getTime() {
        Wire.beginTransmission(RTC_ADDR);
        Wire.write(0);  // Register address
        Wire.endTransmission();

        Wire.requestFrom(RTC_ADDR, 7);
        if (Wire.available() >= 7) {
            second = bcdToDec(Wire.read() & 0x7F);
            minute = bcdToDec(Wire.read());
            hour = bcdToDec(Wire.read());
            Wire.read();  // Day, skip
            day = bcdToDec(Wire.read());
            month = bcdToDec(Wire.read());
            year = bcdToDec(Wire.read()) + 2000;
        }
    }

    bool lostPower() {
        Wire.beginTransmission(RTC_ADDR);
        Wire.write(0x0F);  // Status register
        Wire.endTransmission();
        Wire.requestFrom(RTC_ADDR, 1);
        if (Wire.available()) {
            return (Wire.read() & 0x80) != 0;  // OSF bit
        }
        return false;
    }

    void adjust(int yr, int mon, int dy, int hr, int mn, int sc) {
        Wire.beginTransmission(RTC_ADDR);
        Wire.write(0);  // Start at register 0
        Wire.write(decToBcd(sc));
        Wire.write(decToBcd(mn));
        Wire.write(decToBcd(hr));
        Wire.write(0);   // Day
        Wire.write(decToBcd(dy));
        Wire.write(decToBcd(mon));
        Wire.write(decToBcd(yr - 2000));
        Wire.endTransmission();
    }

private:
    uint8_t bcdToDec(uint8_t bcd) {
        return ((bcd / 16) * 10) + (bcd % 16);
    }

    uint8_t decToBcd(uint8_t dec) {
        return ((dec / 10) * 16) + (dec % 10);
    }
};

// ============================================
// KONFIGURASI - SESUAIKAN DENGAN DATA ANDA
// ============================================

// WiFi Credentials
#define WIFI_SSID         "iPhone Akmal"
#define WIFI_PASSWORD     "12345678"

// Blynk Authentication Token
#define BLYNK_AUTH_TOKEN  "ElCFaWJO1Tvd6KTEzHsZFkRihHxMTecU"

// ============================================
// KONFIGURASI PIN GPIO
// ============================================

// Pin Relay (2 Channel)
#define RELAY_POMPA_1     26   // GPIO 26 - Pompa utama (Melon)
#define RELAY_POMPA_2     27   // GPIO 27 - Pompa cadangan (Cabe)

// Logika Relay (Active Low)
#define RELAY_ON          LOW
#define RELAY_OFF         HIGH

// Pin Sensor Soil Moisture (ADC)
#define SOIL_MOISTURE_PIN 34   // GPIO 34 - ADC1_6

// Pin I2C untuk LCD dan RTC (SHARED)
#define I2C_SDA           21   // GPIO 21 - SDA
#define I2C_SCL           22   // GPIO 22 - SCL

// ============================================
// KONFIGURASI LCD
// ============================================
#define LCD_ADDR          0x27   // Address LCD I2C (biasanya 0x27 atau 0x3F)
#define LCD_COLS          16     // Kolom LCD
#define LCD_ROWS          2      // Baris LCD

// ============================================
// KONFIGURASI THRESHOLD KELembadan TANAH
// ============================================

// Threshold untuk Melon (butuh air lebih banyak)
#define MELON_THRESHOLD_MIN   35
#define MELON_THRESHOLD_MAX   70

// Threshold untuk Cabe (butuh air sedang-konsisten)
#define CABE_THRESHOLD_MIN    40
#define CABE_THRESHOLD_MAX    75

// ============================================
// KONFIGURASI SENSOR
// ============================================
#define SOIL_SENSOR_READ_INTERVAL  1000
#define SOIL_READINGS_COUNT        10
#define SOIL_DRY_VALUE             4095   // ADC saat tanah kering
#define SOIL_WET_VALUE             0      // ADC saat tanah basah

// ============================================
// OBJEK GLOBAL
// ============================================
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
SimpleRTC rtc;  // RTC DS3231 object

// ============================================
// DEKLARASI FUNGSI (FORWARD DECLARATIONS)
// ============================================
void updateSoilThreshold();
void updatePlantInfo();
void updateLCD();
void controlPump(uint8_t pumpNum, bool on);
void updateSoilStatus();

// ============================================
// STATUS SISTEM
// ============================================
PlantType selectedPlant = PLANT_NONE;
int plantAge = 1;
bool autoMode = true;
bool pump1State = false;    // Status Pompa 1 (Melon)
bool pump2State = false;    // Status Pompa 2 (Cabe)
unsigned long pump1StartTime = 0;
unsigned long pump2StartTime = 0;
int pump1Duration = 0;
int pump2Duration = 0;

// Status sensor
int soilMoisturePercent = 0;
int soilThresholdMin = 40;
int soilThresholdMax = 70;
bool soilIsDry = false;
bool soilIsTooWet = false;

// Status WiFi & Blynk
bool wifiConnected = false;
bool blynkConnected = false;

// === CUSTOM SCHEDULE (V12, V13, V14, V15) ===
#define MAX_CUSTOM_SCHEDULES 5
IrrigationSchedule customSchedules[MAX_CUSTOM_SCHEDULES];
int customScheduleCount = 0;
bool useCustomSchedule = true;

// Variabel sementara untuk input dari Blynk
int blynkCustomHour = 8;
int blynkCustomMinute = 0;
int blynkCustomDuration = 10;

// === PREFERENCES ===
Preferences preferences;

void savePreferences() {
    preferences.begin("iot_irr", false);
    preferences.putUInt("plantType", (uint32_t)selectedPlant);
    preferences.putUInt("plantAge", plantAge);
    preferences.putBool("autoMode", autoMode);
    preferences.putBool("useCustom", useCustomSchedule);
    preferences.putUInt("schedCount", customScheduleCount);
    
    for (int i = 0; i < MAX_CUSTOM_SCHEDULES; i++) {
        String keyH = "sch_h_" + String(i);
        String keyM = "sch_m_" + String(i);
        String keyD = "sch_d_" + String(i);
        if (i < customScheduleCount) {
            preferences.putUInt(keyH.c_str(), customSchedules[i].jam);
            preferences.putUInt(keyM.c_str(), customSchedules[i].menit);
            preferences.putUInt(keyD.c_str(), customSchedules[i].durasiDetik);
        }
    }
    preferences.end();
    Serial.println("[PREFS] Pengaturan disimpan.");
}

void loadPreferences() {
    preferences.begin("iot_irr", true);
    if (preferences.isKey("plantType")) {
        selectedPlant = (PlantType)preferences.getUInt("plantType", 0);
        plantAge = preferences.getUInt("plantAge", 1);
        autoMode = preferences.getBool("autoMode", true);
        useCustomSchedule = preferences.getBool("useCustom", true);
        customScheduleCount = preferences.getUInt("schedCount", 0);
        
        for (int i = 0; i < customScheduleCount; i++) {
            String keyH = "sch_h_" + String(i);
            String keyM = "sch_m_" + String(i);
            String keyD = "sch_d_" + String(i);
            customSchedules[i].jam = preferences.getUInt(keyH.c_str(), 8);
            customSchedules[i].menit = preferences.getUInt(keyM.c_str(), 0);
            customSchedules[i].durasiDetik = preferences.getUInt(keyD.c_str(), 10);
        }
        Serial.println("[PREFS] Pengaturan dimuat.");
    } else {
        Serial.println("[PREFS] Tidak ada data tersimpan, pakai default.");
    }
    preferences.end();
}

// Buffer
char plantInfoBuffer[64];
char lcdBuffer[17];

// ============================================
// INISIALISASI BLYNK
// ============================================
BLYNK_WRITE(V0)  // Pilihan Tanaman
{
    int plantCode = param.asInt();
    selectedPlant = (PlantType)plantCode;

    Serial.print("[BLYNK] Tanaman dipilih: ");
    Serial.println(getPlantName(selectedPlant));

    updateSoilThreshold();
    updatePlantInfo();
    updateLCD();
    savePreferences();

    Blynk.virtualWrite(V10, plantInfoBuffer);
}

BLYNK_WRITE(V1)  // Umur Tanaman
{
    plantAge = param.asInt();

    if (selectedPlant == PLANT_MELON && plantAge > MELON_HARI_TANAMAN) {
        plantAge = MELON_HARI_TANAMAN;
    } else if (selectedPlant == PLANT_CABE && plantAge > CABE_HARI_TANAMAN) {
        plantAge = CABE_HARI_TANAMAN;
    }

    Serial.print("[BLYNK] Umur tanaman: ");
    Serial.print(plantAge);
    Serial.print(" hari - Fase: ");
    Serial.println(getPlantPhase(plantAge, selectedPlant));

    updatePlantInfo();
    updateLCD();
    Blynk.virtualWrite(V10, plantInfoBuffer);
}

BLYNK_WRITE(V3)  // Mode Auto/Manual
{
    autoMode = (param.asInt() == 1);
    Serial.print("[BLYNK] Mode: ");
    Serial.println(autoMode ? "Otomatis" : "Manual");
    updateLCD();
}

BLYNK_WRITE(V4)  // Kontrol Manual Pompa 1
{
    if (!autoMode) {
        controlPump(1, param.asInt() == 1);
    }
}

BLYNK_WRITE(V6)  // Kontrol Manual Pompa 2
{
    if (!autoMode) {
        controlPump(2, param.asInt() == 1);
    }
}

BLYNK_WRITE(V11)  // TOMBOL TEST POMPA (DEMO)
{
    int testTrigger = param.asInt();

    if (testTrigger == 1) {
        Serial.println("========================================");
        Serial.println("[DEMO] TOMBOL TEST DITEKAN!");
        Serial.println("========================================");

        // Cek tanaman yang dipilih
        if (selectedPlant == PLANT_MELON) {
            Serial.println("[DEMO] Menyalakan POMPA 1 (Melon)");
            pump1Duration = 10;  // 10 detik untuk demo
            controlPump(1, true);
        } else if (selectedPlant == PLANT_CABE) {
            Serial.println("[DEMO] Menyalakan POMPA 2 (Cabe)");
            pump2Duration = 10;  // 10 detik untuk demo
            controlPump(2, true);
        } else {
            Serial.println("[DEMO] ERROR: Pilih tanaman dulu (V0)");
            // Nyalakan pompa 1 untuk demo
            pump1Duration = 10;
            controlPump(1, true);
        }
    }
}

BLYNK_WRITE(V12)  // SET JAM PENJADWALAN
{
    blynkCustomHour = param.asInt();
    Serial.print("[SCHEDULE] Jam diset (Blynk): ");
    Serial.println(blynkCustomHour);
}

BLYNK_WRITE(V13)  // SET MENIT PENJADWALAN
{
    blynkCustomMinute = param.asInt();
    Serial.print("[SCHEDULE] Menit diset (Blynk): ");
    Serial.println(blynkCustomMinute);
}

BLYNK_WRITE(V14)  // SET DURASI PENYIRAMAN
{
    blynkCustomDuration = param.asInt();
    if (blynkCustomDuration < 5) blynkCustomDuration = 5;
    if (blynkCustomDuration > 60) blynkCustomDuration = 60;
    Serial.print("[SCHEDULE] Durasi diset (Blynk): ");
    Serial.print(blynkCustomDuration);
    Serial.println(" detik");
}

BLYNK_WRITE(V15)  // SIMPAN & APPLY JADWAL
{
    if (param.asInt() == 1) {
        useCustomSchedule = true;
        if (customScheduleCount < MAX_CUSTOM_SCHEDULES) {
            customSchedules[customScheduleCount].jam = blynkCustomHour;
            customSchedules[customScheduleCount].menit = blynkCustomMinute;
            customSchedules[customScheduleCount].durasiDetik = blynkCustomDuration;
            customScheduleCount++;
            savePreferences();
        }
        Serial.println("========================================");
        Serial.println("[SCHEDULE] JADWAL BARU DISIMPAN DARI BLYNK!");
        Serial.println("========================================");
        Blynk.virtualWrite(V8, "Jadwal disimpan!");
        delay(500);
    }
}

// ============================================
// FUNGSI UPDATE THRESHOLD
// ============================================
void updateSoilThreshold() {
    if (selectedPlant == PLANT_MELON) {
        soilThresholdMin = MELON_THRESHOLD_MIN;
        soilThresholdMax = MELON_THRESHOLD_MAX;
    } else if (selectedPlant == PLANT_CABE) {
        soilThresholdMin = CABE_THRESHOLD_MIN;
        soilThresholdMax = CABE_THRESHOLD_MAX;
    } else {
        soilThresholdMin = 40;
        soilThresholdMax = 70;
    }
}

// ============================================
// FUNGSI BACA SENSOR SOIL MOISTURE
// ============================================
int readSoilMoisture() {
    int readings[SOIL_READINGS_COUNT];
    int total = 0;

    for (int i = 0; i < SOIL_READINGS_COUNT; i++) {
        readings[i] = analogRead(SOIL_MOISTURE_PIN);
        delay(10);
    }

    for (int i = 0; i < SOIL_READINGS_COUNT; i++) {
        total += readings[i];
    }

    int avgRawValue = total / SOIL_READINGS_COUNT;

    int moisturePercent = map(avgRawValue,
                              SOIL_DRY_VALUE,
                              SOIL_WET_VALUE,
                              0,
                              100);

    moisturePercent = constrain(moisturePercent, 0, 100);
    return moisturePercent;
}

// ============================================
// FUNGSI UPDATE STATUS TANAH
// ============================================
void updateSoilStatus() {
    soilMoisturePercent = readSoilMoisture();
    soilIsDry = (soilMoisturePercent < soilThresholdMin);
    soilIsTooWet = (soilMoisturePercent > soilThresholdMax);

    Blynk.virtualWrite(V7, soilMoisturePercent);
}

// ============================================
// FUNGSI KONTROL POMPA
// ============================================
void controlPump(uint8_t pumpNum, bool on) {
    if (pumpNum == 1) {
        if (on) {
            digitalWrite(RELAY_POMPA_1, RELAY_ON);
            pump1State = true;
            pump1StartTime = millis();
            Serial.println("[POMPA 1] HIDUP");
            Blynk.virtualWrite(V2, 1);
        } else {
            digitalWrite(RELAY_POMPA_1, RELAY_OFF);
            pump1State = false;
            Serial.println("[POMPA 1] MATI");
            Blynk.virtualWrite(V2, 0);
        }
    } else if (pumpNum == 2) {
        if (on) {
            digitalWrite(RELAY_POMPA_2, RELAY_ON);
            pump2State = true;
            pump2StartTime = millis();
            Serial.println("[POMPA 2] HIDUP");
            Blynk.virtualWrite(V5, 1);
        } else {
            digitalWrite(RELAY_POMPA_2, RELAY_OFF);
            pump2State = false;
            Serial.println("[POMPA 2] MATI");
            Blynk.virtualWrite(V5, 0);
        }
    }
    updateLCD();
}

// ============================================
// FUNGSI UPDATE INFO TANAMAN
// ============================================
void updatePlantInfo() {
    if (selectedPlant == PLANT_NONE) {
        sprintf(plantInfoBuffer, "Tidak Ada Tanaman");
        return;
    }

    const char* plantName = getPlantName(selectedPlant);
    const char* phase = getPlantPhase(plantAge, selectedPlant);
    int minggu = getMingguKe(plantAge);

    sprintf(plantInfoBuffer, "%s|Hari-%d|Mg-%d|%s",
            plantName, plantAge, minggu, phase);
}

// ============================================
// FUNGSI MENDAPATKAN JADWAL PENYIRAMAN
// ============================================
void getCurrentSchedule(const IrrigationSchedule*& schedule, int& count) {
    if (useCustomSchedule && customScheduleCount > 0) {
        // Pakai daftar jadwal CUSTOM
        schedule = customSchedules;
        count = customScheduleCount;
        return;
    }

    if (selectedPlant == PLANT_MELON) {
        schedule = getMelonSchedule(plantAge, count);
    } else if (selectedPlant == PLANT_CABE) {
        schedule = getCabeSchedule(plantAge, count);
    } else {
        schedule = NULL;
        count = 0;
    }
}

// ============================================
// FUNGSI LCD DISPLAY
// ============================================
void updateLCD() {
    static unsigned long lastLCDUpdate = 0;
    if (millis() - lastLCDUpdate < 1000) return;  // Update LCD setiap 1 detik
    lastLCDUpdate = millis();

    lcd.clear();

    // Baris 1: Info Tanaman
    if (selectedPlant == PLANT_NONE) {
        lcd.print("Smart Irrigation");
    } else {
        sprintf(lcdBuffer, "%s Minggu-%d", getPlantName(selectedPlant), getMingguKe(plantAge));
        lcd.print(lcdBuffer);
    }

    // Baris 2: Info Lain
    lcd.setCursor(0, 1);

    if (pump1State || pump2State) {
        // Sedang menyiram
        if (pump1State) {
            lcd.print("P1:ON ");
        } else {
            lcd.print("P1:OFF ");
        }
        if (pump2State) {
            lcd.print("P2:ON");
        } else {
            lcd.print("P2:OFF");
        }
    } else {
        // Status normal
        sprintf(lcdBuffer, "Soil:%d%% %s",
                soilMoisturePercent,
                autoMode ? "AUTO" : "MANUAL");
        lcd.print(lcdBuffer);
    }
}

// ============================================
// FUNGSI UPDATE WAKTU RTC KE BLYNK
// ============================================
void updateBlynkTime() {
    rtc.getTime();
    char timeBuffer[32];
    sprintf(timeBuffer, "%02d/%02d/%04d %02d:%02d",
            rtc.day, rtc.month, rtc.year,
            rtc.hour, rtc.minute);

    Blynk.virtualWrite(V9, timeBuffer);
}

// ============================================
// FUNGSI AMBIL JADWAL BERDASARKAN WAKTU
// ============================================
bool isScheduledTime(const IrrigationSchedule& sched) {
    rtc.getTime();

    int scheduleTimeMinutes = sched.jam * 60 + sched.menit;
    int currentTimeMinutes = rtc.hour * 60 + rtc.minute;

    // Cek jika waktu sekarang cocok (tepat)
    return (currentTimeMinutes == scheduleTimeMinutes);
}

void checkPumpTimeouts() {
    // Cek Pompa 1 (Melon)
    if (pump1State) {
        unsigned long elapsed = millis() - pump1StartTime;
        if (elapsed >= (unsigned long)pump1Duration * 1000) {
            controlPump(1, false);
            Serial.println("[POMPA 1] Timeout / Selesai");
        }
    }

    // Cek Pompa 2 (Cabe)
    if (pump2State) {
        unsigned long elapsed = millis() - pump2StartTime;
        if (elapsed >= (unsigned long)pump2Duration * 1000) {
            controlPump(2, false);
            Serial.println("[POMPA 2] Timeout / Selesai");
        }
    }
}

// ============================================
// FUNGSI UTAMA PENYIRAMAN OTOMATIS
// ============================================
void runAutomaticIrrigation() {
    checkPumpTimeouts();

    if (!autoMode || selectedPlant == PLANT_NONE) {
        return;
    }

    const IrrigationSchedule* schedule;
    int scheduleCount;
    getCurrentSchedule(schedule, scheduleCount);

    if (schedule == NULL || scheduleCount == 0) {
        return;
    }

    // Cek jadwal penyiraman
    static unsigned long lastIrrigation[4] = {0, 0, 0, 0};
    static int lastCheckedDay = -1;

    rtc.getTime();
    int currentDay = rtc.day;

    if (currentDay != lastCheckedDay) {
        memset(lastIrrigation, 0, sizeof(lastIrrigation));
        lastCheckedDay = currentDay;
    }

    unsigned long currentMillis = millis();

    for (int i = 0; i < scheduleCount; i++) {
        const IrrigationSchedule& sched = schedule[i];

        int scheduleTimeMinutes = sched.jam * 60 + sched.menit;
        int currentTimeMinutes = rtc.hour * 60 + rtc.minute;

        if (currentTimeMinutes == scheduleTimeMinutes) {
            if (currentMillis - lastIrrigation[i] > 120000) {
                lastIrrigation[i] = currentMillis;

                // Cek kelembaban tanah
                int currentMoisture = readSoilMoisture();

                Serial.print("[KEL] Tanah: ");
                Serial.print(currentMoisture);
                Serial.print("% | Threshold: ");
                Serial.print(soilThresholdMin);
                Serial.print("-");
                Serial.print(soilThresholdMax);
                Serial.println("%");

                const char* plantName = getPlantName(selectedPlant);

                if (currentMoisture >= soilThresholdMin) {
                    // Tanah masih cukup lembab - skip
                    Serial.print("[SKIP] ");
                    Serial.print(plantName);
                    Serial.println(" - Tanah masih lembab (>= min), penyiraman dibatalkan!");
                } else {
                    // Tanah kering - siram full
                    Serial.print("[START] ");
                    Serial.print(plantName);
                    Serial.print(" - Menyiram (");
                    Serial.print(sched.durasiDetik);
                    Serial.println(" detik)");

                    if (selectedPlant == PLANT_MELON) {
                        pump1Duration = sched.durasiDetik;
                        controlPump(1, true);
                    } else if (selectedPlant == PLANT_CABE) {
                        pump2Duration = sched.durasiDetik;
                        controlPump(2, true);
                    }
                }
            }
        }
    }
}

// ============================================
// FUNGSI SYNC NTP
// ============================================
void syncNTPtoRTC() {
    configTime(7 * 3600, 0, "id.pool.ntp.org", "time.google.com");
    Serial.print("[NTP] Menunggu waktu dari internet");
    int retry = 0;
    // Tunggu maksimal 15 detik
    while (time(nullptr) < 100000 && retry < 30) {
        delay(500);
        Serial.print(".");
        retry++;
    }
    Serial.println();
    
    time_t now = time(nullptr);
    if (now > 100000) {
        struct tm* timeinfo = localtime(&now);
        int sec = timeinfo->tm_sec;
        int minute_temp = timeinfo->tm_min;
        int hour_temp = timeinfo->tm_hour;
        int day_temp = timeinfo->tm_mday;
        int month_temp = timeinfo->tm_mon + 1;
        int year_temp = timeinfo->tm_year + 1900;

        rtc.adjust(year_temp, month_temp, day_temp, hour_temp, minute_temp, sec);
        Serial.println("[RTC] Waktu di-sync dari NTP!");
        Serial.printf("[RTC] Baru: %02d:%02d:%02d %02d/%02d/%04d\n",
                    hour_temp, minute_temp, sec, day_temp, month_temp, year_temp);
    } else {
        Serial.println("[NTP] Gagal mendapatkan waktu.");
    }
}

// ============================================
// WEB SERVER HANDLERS
// ============================================
void handleRoot() {
    server.send_P(200, "text/html", HTML_PAGE);
}

void handleStatus() {
    rtc.getTime();
    char timeStr[16];
    sprintf(timeStr, "%02d:%02d:%02d", rtc.hour, rtc.minute, rtc.second);
    char dateStr[16];
    sprintf(dateStr, "%02d/%02d/%04d", rtc.day, rtc.month, rtc.year);
    
    String json = "{";
    json += "\"time\":\"" + String(timeStr) + "\",";
    json += "\"date\":\"" + String(dateStr) + "\",";
    json += "\"plant\":\"" + String(getPlantName(selectedPlant)) + "\",";
    json += "\"plantType\":" + String((int)selectedPlant) + ",";
    json += "\"plantAge\":" + String(plantAge) + ",";
    json += "\"phase\":\"" + String(selectedPlant == PLANT_NONE ? "-" : getPlantPhase(plantAge, selectedPlant)) + "\",";
    json += "\"week\":" + String(selectedPlant == PLANT_NONE ? 0 : getMingguKe(plantAge)) + ",";
    json += "\"moisture\":" + String(soilMoisturePercent) + ",";
    json += "\"pump1\":" + String(pump1State ? "true" : "false") + ",";
    json += "\"pump2\":" + String(pump2State ? "true" : "false") + ",";
    json += "\"mode\":" + String(autoMode ? "true" : "false") + ",";
    json += "\"useCustom\":" + String(useCustomSchedule ? "true" : "false") + ",";
    json += "\"schedules\":[";
    for(int i = 0; i < customScheduleCount; i++) {
        json += "{\"h\":" + String(customSchedules[i].jam) + ",";
        json += "\"m\":" + String(customSchedules[i].menit) + ",";
        json += "\"d\":" + String(customSchedules[i].durasiDetik) + "}";
        if(i < customScheduleCount - 1) json += ",";
    }
    json += "],";
    json += "\"wifi\":" + String(wifiConnected ? "true" : "false") + ",";
    json += "\"blynk\":" + String(blynkConnected ? "true" : "false") + ",";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\"";
    json += "}";
    server.send(200, "application/json", json);
}

void handleControl() {
    String cmd = server.arg("cmd");

    if (cmd == "pump1") {
        controlPump(1, true);
        pump1Duration = 15;
    } else if (cmd == "pump2") {
        controlPump(2, true);
        pump2Duration = 15;
    } else if (cmd == "stop") {
        controlPump(1, false);
        controlPump(2, false);
    } else if (cmd == "settings") {
        selectedPlant = (PlantType)server.arg("plant").toInt();
        plantAge = server.arg("age").toInt();
        autoMode = (server.arg("mode").toInt() == 1);
        updateSoilThreshold();
        updatePlantInfo();
        updateLCD();
        savePreferences();
        
        // SINKRONKAN KE BLYNK
        if (blynkConnected) {
            Blynk.virtualWrite(V0, (int)selectedPlant);
            Blynk.virtualWrite(V1, plantAge);
            Blynk.virtualWrite(V10, plantInfoBuffer);
            // Mode auto tidak ada pin khusus di blynk sebelumnya, abaikan jika tidak perlu
        }
    } else if (cmd == "schedule") {
        String action = server.arg("action");
        if (action == "add") {
            if (customScheduleCount < MAX_CUSTOM_SCHEDULES) {
                customSchedules[customScheduleCount].jam = server.arg("hour").toInt();
                customSchedules[customScheduleCount].menit = server.arg("min").toInt();
                customSchedules[customScheduleCount].durasiDetik = server.arg("dur").toInt();
                customScheduleCount++;
                useCustomSchedule = true;
                savePreferences();
                Serial.println("[WEB] Jadwal ditambahkan");
            }
        } else if (action == "delete") {
            int idx = server.arg("idx").toInt();
            if (idx >= 0 && idx < customScheduleCount) {
                for (int i = idx; i < customScheduleCount - 1; i++) {
                    customSchedules[i] = customSchedules[i+1];
                }
                customScheduleCount--;
                if (customScheduleCount == 0) useCustomSchedule = false;
                savePreferences();
                Serial.println("[WEB] Jadwal dihapus");
            }
        } else if (action == "toggle") {
            useCustomSchedule = (server.arg("val").toInt() == 1);
            savePreferences();
        }
    } else if (cmd == "synctime") {
        syncNTPtoRTC();
    }

    server.send(200, "text/plain", "OK");
}

void setupWebServer() {
    server.on("/", handleRoot);
    server.on("/status", handleStatus);
    server.on("/control", handleControl);
    server.begin();
    Serial.println("[WEB] Server started!");
}

// ============================================
// FUNGSI CEK KONEKSI & RECONNECT
// ============================================
void checkConnections() {
    // Cek WiFi
    if (WiFi.status() != WL_CONNECTED && wifiConnected) {
        wifiConnected = false;
        blynkConnected = false;
        Serial.println("[WIFI] Koneksi terputus!");
        lcd.clear();
        lcd.print("WiFi OFFLINE");
        lcd.setCursor(0, 1);
        lcd.print("Reconnecting...");
    }

    // Reconnect WiFi jika needed
    if (!wifiConnected) {
        WiFi.disconnect();
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        unsigned long startAttempt = millis();
        while (WiFi.status() != WL_CONNECTED &&
               millis() - startAttempt < 10000) {
            delay(500);
            Serial.print(".");
        }

        if (WiFi.status() == WL_CONNECTED) {
            wifiConnected = true;
            Serial.println("\n[WIFI] Terhubung!");
            Serial.print("[WIFI] IP: ");
            Serial.println(WiFi.localIP());

            // Start Web Server
            setupWebServer();

            Blynk.config(BLYNK_AUTH_TOKEN);
            Blynk.connect();
            blynkConnected = true;
            Serial.println("[BLYNK] Terhubung!");
        }
    }
}

// ============================================
// TIMER
// ============================================
BlynkTimer statusTimer;
BlynkTimer soilTimer;
BlynkTimer lcdUpdateTimer;
BlynkTimer rtcSyncTimer;

void sendPeriodicStatus() {
    // Status pompa
    const char* pumpStatus = "OFF";
    if (pump1State) pumpStatus = "P1 ON";
    else if (pump2State) pumpStatus = "P2 ON";

    char statusBuffer[64];
    sprintf(statusBuffer, "Soil:%d%%|%s|%s",
            soilMoisturePercent,
            pumpStatus,
            autoMode ? "AUTO" : "MANUAL");

    Blynk.virtualWrite(V8, statusBuffer);

    // Print ke Serial
    rtc.getTime();
    Serial.println("\n========== STATUS ==========");
    Serial.print("Waktu RTC: ");
    Serial.printf("%02d:%02d:%02d\n", rtc.hour, rtc.minute, rtc.second);
    Serial.print("Tanaman: ");
    Serial.println(getPlantName(selectedPlant));
    Serial.print("Umur: ");
    Serial.print(plantAge);
    Serial.print(" hari (");
    Serial.print(getPlantPhase(plantAge, selectedPlant));
    Serial.println(")");
    Serial.print("Kelembaban: ");
    Serial.print(soilMoisturePercent);
    Serial.println("%");
    Serial.print("Pompa 1: ");
    Serial.println(pump1State ? "HIDUP" : "MATI");
    Serial.print("Pompa 2: ");
    Serial.println(pump2State ? "HIDUP" : "MATI");
    Serial.print("Mode: ");
    Serial.println(autoMode ? "Otomatis" : "Manual");
    Serial.print("WiFi: ");
    Serial.println(wifiConnected ? "Terhubung" : "Terputus");
    Serial.println("============================\n");
}

BLYNK_CONNECTED() {
    // Sinkronkan nilai tersimpan di hardware ke aplikasi Blynk saat terhubung
    Blynk.virtualWrite(V0, (int)selectedPlant);
    Blynk.virtualWrite(V1, plantAge);
    Blynk.virtualWrite(V10, plantInfoBuffer);
}

void readSoilSensor() {
    updateSoilStatus();
}

void updateLCDDisplay() {
    updateLCD();
}

void syncRTCTime() {
    updateBlynkTime();
}

// ============================================
// SETUP
// ============================================
void setup()
{
    Serial.begin(115200);
    
    // Muat pengaturan dari EEPROM/Preferences
    loadPreferences();

    Serial.println("\n====================================");
    Serial.println("  SMART IRRIGATION SYSTEM v3.0");
    Serial.println("  + LCD 16x2 + RTC DS3231");
    Serial.println("====================================\n");

    // Inisialisasi Wire (I2C)
    Wire.begin(I2C_SDA, I2C_SCL);
    Serial.println("[I2C] SDA=G21, SCL=G22");

    // Konfigurasi Pin Output & Matikan Pompa (Pastikan Mati saat Boot)
    // Untuk Active Low Relay, set HIGH agar relay mati
    pinMode(RELAY_POMPA_1, OUTPUT);
    digitalWrite(RELAY_POMPA_1, RELAY_OFF);
    pinMode(RELAY_POMPA_2, OUTPUT);
    digitalWrite(RELAY_POMPA_2, RELAY_OFF);
    pinMode(SOIL_MOISTURE_PIN, INPUT);

    Serial.println("[RELAY] Pompa dimatikan saat startup");

    // Inisialisasi LCD
    lcd.init();
    lcd.backlight();
    lcd.print("Smart Irrigation");
    lcd.setCursor(0, 1);
    lcd.print("Init System...");
    Serial.println("[LCD] Inisialisasi selesai");

    // Inisialisasi RTC
    if (rtc.begin()) {
        Serial.println("[RTC] DS3231 terdeteksi");

        // Cek jika RTC lost power (kembali ke default)
        if (rtc.lostPower()) {
            Serial.println("[RTC] RTC lost power, set ke waktu kompilasi");
            // Parse __DATE__ dan __TIME__ macro
            // Format: "Jun 27 2026" dan "12:34:56"
            rtc.adjust(2026, 6, 27, 12, 0, 0);  // Default: contoh
        }

        rtc.getTime();
        Serial.print("[RTC] Waktu saat ini: ");
        Serial.printf("%02d:%02d:%02d %02d/%02d/%04d\n",
                      rtc.hour, rtc.minute, rtc.second,
                      rtc.day, rtc.month, rtc.year);
    } else {
        Serial.println("[RTC] ERROR - DS3231 tidak terdeteksi!");
        lcd.clear();
        lcd.print("RTC ERROR!");
    }

    // Baca sensor pertama kali
    soilMoisturePercent = readSoilMoisture();
    Serial.print("[SOIL] Kelembaban awal: ");
    Serial.print(soilMoisturePercent);
    Serial.println("%");

    // Koneksi WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("[WIFI] Menghubungkan");

    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 30) {
        delay(500);
        Serial.print(".");
        wifiAttempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
        Serial.println("\n[WIFI] Terhubung!");
        Serial.print("[WIFI] IP: ");
        Serial.println(WiFi.localIP());

        // Start Web Server
        setupWebServer();
        
        // Sinkronisasi otomatis ke RTC
        syncNTPtoRTC();
    } else {
        Serial.println("\n[WIFI] Gagal - Mode Offline");
    }

    // Koneksi Blynk
    Blynk.config(BLYNK_AUTH_TOKEN);
    if (wifiConnected) {
        if (Blynk.connect()) {
            blynkConnected = true;
            Serial.println("[BLYNK] Terhubung!");
        } else {
            Serial.println("[BLYNK] Gagal terhubung");
        }
    }

    // Setup Timer
    statusTimer.setInterval(60000L, sendPeriodicStatus);   // Status setiap 60 detik
    soilTimer.setInterval(10000L, readSoilSensor);         // Baca sensor setiap 10 detik
    lcdUpdateTimer.setInterval(1000L, updateLCDDisplay);   // Update LCD setiap 1 detik
    rtcSyncTimer.setInterval(60000L, syncRTCTime);         // Sync RTC ke Blynk setiap 60 detik

    // Inisialisasi
    updateSoilThreshold();
    updatePlantInfo();
    updateLCD();

    Serial.println("\n====================================");
    Serial.println("  Sistem siap digunakan!");
    Serial.println("====================================\n");

    lcd.clear();
    lcd.print("System Ready!");
    delay(1000);
}

// ============================================
// LOOP UTAMA
// ============================================
void loop()
{
    // Cek koneksi WiFi & Blynk
    checkConnections();

    // Jalankan Web Server
    server.handleClient();

    // Jalankan Blynk
    if (wifiConnected && blynkConnected) {
        Blynk.run();
    }

    // Jalankan semua timer
    statusTimer.run();
    soilTimer.run();
    lcdUpdateTimer.run();
    rtcSyncTimer.run();

    // Jalankan penyiraman otomatis
    runAutomaticIrrigation();
}