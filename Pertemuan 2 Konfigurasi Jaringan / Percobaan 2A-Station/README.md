# Percobaan 2A – Konfigurasi Mode Station (STA)

## A. Tujuan Percobaan

Percobaan 2A dilakukan untuk memahami konfigurasi jaringan WiFi pada mikrokontroler menggunakan mode Station (STA). Pada mode ini, NodeMCU ESP8266 berperan sebagai client yang terhubung ke jaringan WiFi yang sudah tersedia.

Tujuan dari percobaan ini adalah:
1. Memahami cara kerja komunikasi jaringan WiFi pada perangkat IoT.
2. Memahami konfigurasi mode Station (STA) pada mikrokontroler.
3. Menghubungkan NodeMCU ESP8266 ke jaringan WiFi yang tersedia.
4. Membaca parameter jaringan seperti IP Address, MAC Address, dan RSSI.
5. Mengetahui status koneksi WiFi melalui Serial Monitor.

Pada pelaksanaan praktikum digunakan NodeMCU ESP8266 sebagai pengganti ESP32 yang tercantum pada modul karena perangkat yang tersedia di laboratorium adalah NodeMCU ESP8266.

---

## B. Gambar Rangkaian

Rangkaian pada percobaan 2A terdiri dari NodeMCU ESP8266 yang terhubung dengan komputer menggunakan kabel USB dan LED indikator yang digunakan untuk menunjukkan status koneksi WiFi.

### Konfigurasi Rangkaian

| No | Komponen | Pin NodeMCU ESP8266 |
|---|---|---|
| 1 | LED indikator | GPIO 4 |
| 2 | Kabel USB | USB NodeMCU |
| 3 | NodeMCU ESP8266 | Board utama |

### Diagram Rangkaian

```text
NodeMCU ESP8266

GPIO 4 ───── Resistor 220Ω ───── LED
GND    ────────────────────────── LED
```

### Dokumentasi Rangkaian

![Rangkaian Station](rangkaian-sta.jpeg)

---

## C. Penjelasan Program

Program yang digunakan pada percobaan ini adalah:

```cpp
#include <ESP8266WiFi.h>

const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";

const int ledPin = 4; // LED indikator status koneksi

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Set mode WiFi menjadi Station
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Jika berhasil terhubung
  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());

  Serial.print("RSSI (dBm) : ");
  Serial.println(WiFi.RSSI());

  digitalWrite(ledPin, HIGH); // nyalakan LED sebagai indikator
}

void loop() {
  // Cek status koneksi setiap 5 detik
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status: Terhubung");
  } else {
    Serial.println("Status: Terputus");
    digitalWrite(ledPin, LOW);
  }

  delay(5000);
}
```

### Alur Program

Program diawali dengan `#include <ESP8266WiFi.h>` karena praktikum menggunakan NodeMCU ESP8266. Kemudian SSID dan password WiFi disimpan dalam variabel `ssid` dan `password`. Pin LED juga ditentukan sebagai indikator status koneksi.

Pada `setup()`, Serial Monitor diaktifkan dan NodeMCU diatur menjadi Station menggunakan `WiFi.mode(WIFI_STA)`. Setelah itu, `WiFi.begin(ssid, password)` digunakan untuk menghubungkan NodeMCU ke jaringan WiFi.

Program akan terus mengecek status koneksi menggunakan `WiFi.status()` sampai berhasil terhubung. Jika koneksi berhasil, program menampilkan IP Address, MAC Address, dan RSSI pada Serial Monitor, kemudian LED dinyalakan sebagai indikator.

Pada bagian `loop()`, status koneksi dicek setiap 5 detik. Jika masih terhubung akan muncul `Status: Terhubung`, sedangkan jika koneksi terputus akan muncul `Status: Terputus` dan LED akan dimatikan.

---

## D. Hasil Pengamatan

### 1. Data Hasil Pengamatan

| No | Waktu (detik) | Status Koneksi | IP Address | MAC Address | RSSI (dBm) | Status LED |
|---|---:|---|---|---|---:|---|
| 1 | 0 | Menghubungkan | - | - | - | Mati |
| 2 | 5 | Terhubung | 10.22.73.171 | [MAC Address] | -64 | Menyala |
| 3 | 10 | Terhubung | 10.22.73.171 | [MAC Address] | -64 | Menyala |
| 4 | 15 | Terhubung | 10.22.73.171 | [MAC Address] | -64 | Menyala |
| 5 | 20 | Terhubung | 10.22.73.171 | [MAC Address] | -64 | Menyala |
| 6 | 25 | Terhubung | 10.22.73.171 | [MAC Address] | -64 | Menyala |
| 7 | 30 | Terhubung | 10.22.73.171 | [MAC Address] | -64 | Menyala |
| 8 | 35 | Terhubung | 10.22.73.171 | [MAC Address] | -64 | Menyala |
| 9 | 40 | Terhubung | 10.22.73.171 | [MAC Address] | -64 | Menyala |
| 10 | 45 | Terhubung | 10.22.73.171 | [MAC Address] | -64 | Menyala |

IP, MAC, dan RSSI tidak berubah karena pada program nilainya hanya ditampilkan ketika koneksi pertama kali berhasil pada `setup()`. Oleh karena itu, pengulangan nilai pada tabel masih menunjukkan kondisi koneksi yang sama.

### 2. Pengujian Mode STA

| No | Kondisi | SSID | Password | Status Koneksi | Output Serial Monitor | Keterangan |
|---|---|---|---|---|---|---|
| 1 | Kredensial benar | SSID yang digunakan | Password yang digunakan | Berhasil | WiFi berhasil terhubung dan informasi IP, MAC, RSSI ditampilkan | NodeMCU berhasil terhubung ke jaringan |
| 2 | Password salah | SSID yang digunakan | Password salah | Gagal | Muncul titik-titik (`.....`) secara terus-menerus | Password tidak sesuai sehingga NodeMCU tidak berhasil terhubung |
| 3 | SSID salah | SSID salah | Password yang digunakan | Gagal | Muncul titik-titik (`.....`) secara terus-menerus | SSID tidak ditemukan sehingga NodeMCU tidak berhasil terhubung |

### Hasil Serial Monitor

![Hasil Serial Monitor](bisa.jpeg)

---

## E. Pertanyaan Praktikum

### 1. Flowchart proses koneksi ESP32/NodeMCU ke jaringan WiFi

```text
          ┌─────────────┐
          │    Mulai    │
          └──────┬──────┘
                 │
                 ▼
       ┌──────────────────┐
       │ Inisialisasi     │
       │ Serial dan LED   │
       └────────┬─────────┘
                │
                ▼
       ┌──────────────────┐
       │ Mode WiFi STA    │
       └────────┬─────────┘
                │
                ▼
       ┌──────────────────┐
       │ Hubungkan ke     │
       │ jaringan WiFi    │
       └────────┬─────────┘
                │
                ▼
       ┌──────────────────┐
       │ WiFi terhubung?  │
       └───────┬────┬─────┘
               │Ya  │Tidak
               │    │
               ▼    ▼
       ┌───────────────┐
       │ Tampilkan IP, │
       │ MAC dan RSSI  │
       └───────┬───────┘
               │
               ▼
       ┌───────────────┐
       │ LED menyala   │
       └───────┬───────┘
               │
               ▼
       ┌───────────────┐
       │ Cek status    │
       │ WiFi          │
       └───────┬───────┘
               │
               ▼
          Tunggu 5 detik
               │
               └──────► Cek kembali
```

### 2. Apa fungsi perintah `WiFi.mode(WIFI_STA)`?

`WiFi.mode(WIFI_STA)` dipakai untuk mengatur modul WiFi agar bekerja dalam mode Station. Dalam mode ini, NodeMCU atau ESP32 berperan sebagai client yang terhubung ke jaringan WiFi yang sudah tersedia, misalnya router atau hotspot smartphone.

### 3. Apa yang terjadi apabila SSID atau password yang dimasukkan salah?

Apabila SSID salah atau jaringan dengan nama tersebut tidak tersedia, perangkat tidak dapat menemukan jaringan WiFi yang dituju sehingga proses koneksi tidak berhasil. Apabila SSID benar tetapi password salah, perangkat dapat menemukan jaringan tersebut tetapi gagal melakukan proses autentikasi.

Akibatnya perangkat tetap tidak terhubung ke jaringan dan pada Serial Monitor akan muncul titik-titik secara terus-menerus selama proses koneksi.

### 4. Modifikasi program agar melakukan reconnect otomatis

Program dapat dimodifikasi dengan menambahkan proses untuk menghubungkan kembali NodeMCU ketika koneksi WiFi terputus.

```cpp
#include <ESP8266WiFi.h>

const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";

const int ledPin = 4;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  digitalWrite(ledPin, HIGH);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status: Terhubung");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("Status: Terputus");
    digitalWrite(ledPin, LOW);

    // Mencoba menghubungkan kembali
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    Serial.println("Mencoba menghubungkan kembali...");
  }

  delay(5000);
}
```

Pada modifikasi ini ditambahkan proses untuk menghubungkan kembali NodeMCU ketika koneksi WiFi terputus. Jika `WiFi.status()` tidak menunjukkan `WL_CONNECTED`, program menjalankan `WiFi.disconnect()` kemudian `WiFi.begin()` kembali menggunakan SSID dan password yang sama.

Dengan begitu, NodeMCU tidak hanya mendeteksi koneksi yang terputus, tetapi juga otomatis mencoba terhubung kembali tanpa perlu menjalankan program dari awal.

---

## F. Catatan Kendala

Kendala yang dialami pada percobaan 2A adalah saat melakukan compile dan upload program menggunakan Arduino IDE. Pada awal percobaan, proses upload mengalami error dan program tidak dapat dijalankan pada NodeMCU ESP8266.

Percobaan sudah dilakukan menggunakan dua laptop, tetapi kendala yang sama masih terjadi. Setelah diperiksa, kemungkinan masalah disebabkan oleh port perangkat yang tidak dapat dipilih atau tidak terdeteksi dengan baik pada Arduino IDE. Selain itu, hanya tersedia satu port yang dapat digunakan untuk menghubungkan perangkat, sehingga proses compile dan upload program menjadi terhambat.

---

## G. Dokumentasi Praktikum

### Perangkaian Hardware

![Rangkaian NodeMCU ESP8266](rangkaian-sta.jpeg)

### Hasil Serial Monitor

![Hasil Pengamatan Serial Monitor](bisa.jpeg)

---

## H. Kesimpulan

Pada percobaan 2A telah dilakukan konfigurasi NodeMCU ESP8266 menggunakan mode Station (STA). NodeMCU berhasil terhubung ke jaringan WiFi dan dapat menampilkan IP Address, MAC Address, serta nilai RSSI melalui Serial Monitor.

Berdasarkan hasil pengamatan, NodeMCU memperoleh IP Address `10.22.73.171` dan nilai RSSI sebesar `-64 dBm`. LED indikator juga menyala ketika koneksi berhasil. Pengujian dengan SSID dan password yang salah menunjukkan bahwa NodeMCU tidak dapat terhubung ke jaringan dan proses koneksi terus berlangsung.

Selain itu, program dimodifikasi dengan menambahkan mekanisme reconnect otomatis sehingga NodeMCU dapat mencoba terhubung kembali ketika koneksi WiFi terputus.
