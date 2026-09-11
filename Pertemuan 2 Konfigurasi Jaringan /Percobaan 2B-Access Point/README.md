# Percobaan 2B – Konfigurasi Mode Access Point (AP)

## A. Tujuan Percobaan

Percobaan 2B dilakukan untuk membuat NodeMCU ESP8266 menjadi Access Point, sehingga perangkat lain dapat menemukan dan terhubung langsung ke jaringan yang dibuat oleh mikrokontroler.

Tujuan dari percobaan ini adalah:
1. Memahami konsep Access Point pada perangkat IoT.
2. Mengatur NodeMCU ESP8266 agar dapat membuat jaringan WiFi sendiri.
3. Mengetahui cara menghubungkan perangkat lain ke jaringan yang dibuat NodeMCU.
4. Mengetahui alamat IP Access Point.
5. Mengetahui jumlah perangkat yang terhubung ke Access Point.

---

## B. Gambar Rangkaian

Rangkaian terdiri dari NodeMCU ESP8266 yang terhubung ke komputer menggunakan kabel USB. LED pada board digunakan sebagai indikator.

### Konfigurasi Rangkaian

| No | Komponen | Keterangan |
|---|---|---|
| 1 | NodeMCU ESP8266 | Board utama |
| 2 | Kabel USB | Menghubungkan NodeMCU dengan komputer |
| 3 | LED | Indikator pada board |

### Diagram Rangkaian

```text
Komputer
   │
   │ Kabel USB
   │
   ▼
NodeMCU ESP8266
   │
   └── LED indikator pada board
```

### Dokumentasi Rangkaian

![Rangkaian Access Point](Documentation/rangkaian%20ap.jpeg)

---

## C. Penjelasan Program

Program yang digunakan pada percobaan ini adalah:

```cpp
#include <ESP8266WiFi.h>

const char* ap_ssid = "ESP32_AccessPoint";
const char* ap_password = "12345678"; // minimal 8 karakter

void setup() {
  Serial.begin(115200);

  // Set mode WiFi menjadi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);

  IPAddress apIP = WiFi.softAPIP();

  Serial.println("Access Point aktif!");
  Serial.print("SSID : ");
  Serial.println(ap_ssid);
  Serial.print("IP Address : ");
  Serial.println(apIP);
}

void loop() {
  // Menampilkan jumlah perangkat yang terhubung setiap 5 detik
  int jumlahClient = WiFi.softAPgetStationNum();

  Serial.print("Jumlah perangkat terhubung: ");
  Serial.println(jumlahClient);

  delay(5000);
}
```

### Alur Program

Pada percobaan ini NodeMCU digunakan sebagai Access Point, sehingga NodeMCU membuat jaringan WiFi sendiri yang dapat dihubungkan oleh perangkat lain.

SSID dan password Access Point ditentukan pada variabel `ap_ssid` dan `ap_password`. Pada `setup()`, mode WiFi diatur menggunakan `WiFi.mode(WIFI_AP)`, kemudian `WiFi.softAP()` digunakan untuk membuat jaringan.

Setelah Access Point aktif, alamat IP-nya ditampilkan menggunakan `WiFi.softAPIP()`. Pada bagian `loop()`, `WiFi.softAPgetStationNum()` digunakan untuk menghitung jumlah perangkat yang sedang terhubung ke Access Point.

Hasil jumlah perangkat yang terhubung ditampilkan pada Serial Monitor setiap 5 detik.

---

## D. Hasil Pengamatan

### 1. Parameter Access Point

| No | Parameter | Nilai |
|---|---|---|
| 1 | SSID Access Point | ESP32_AccessPoint |
| 2 | Password | 12345678 |
| 3 | IP Address default | 192.168.4.1 |

Berdasarkan hasil pengamatan pada Serial Monitor, jumlah perangkat yang terhubung mengalami perubahan sebagai berikut:

| Pengamatan | Jumlah Perangkat |
|---|---:|
| 1 | 1 |
| 2 | 1 |
| 3 | 2 |
| 4 | 2 |
| 5 | 2 |
| 6 | 3 |
| 7 | 4 |
| 8 | 4 |
| 9 | 4 |

Hasil tersebut menunjukkan bahwa Access Point berhasil dibuat dan dapat menerima koneksi dari perangkat lain. Jumlah perangkat yang terhubung bertambah dari 1 hingga 4 perangkat, sehingga fungsi `WiFi.softAPgetStationNum()` dapat digunakan untuk memantau jumlah client yang terhubung.

### Hasil Serial Monitor

![Hasil Percobaan Access Point](Documentation/serial%20monitor%20AP.jpeg)

---

## E. Pertanyaan Praktikum

### 1. Mengapa alamat IP default Access Point pada ESP32 umumnya 192.168.4.1?

Alamat tersebut merupakan alamat jaringan lokal yang secara default digunakan pada konfigurasi Access Point ESP32. Alamat tersebut digunakan sebagai gateway bagi perangkat-perangkat yang terhubung ke jaringan Access Point yang dibuat oleh ESP32.

Pada konfigurasi Access Point, NodeMCU menggunakan alamat tersebut sebagai alamat IP perangkat yang membuat jaringan.

### 2. Apa perbedaan mendasar antara mode Station dan Access Point?

Perbedaan mendasar antara Station dan Access Point terletak pada peran perangkat dalam jaringan WiFi.

Pada mode Station (STA), NodeMCU atau ESP32 berperan sebagai client yang terhubung ke jaringan WiFi yang sudah tersedia, misalnya router atau hotspot.

Sedangkan pada mode Access Point (AP), NodeMCU atau ESP32 berperan sebagai penyedia jaringan WiFi yang dapat digunakan oleh perangkat lain untuk terhubung, seperti smartphone atau laptop.

Jadi, secara sederhana, mode Station digunakan ketika perangkat ingin bergabung ke jaringan yang sudah ada, sedangkan mode Access Point digunakan ketika perangkat menjadi sumber atau penyedia jaringan bagi perangkat lainnya.

### 3. Apa risiko keamanan apabila password Access Point tidak diberikan atau terlalu sederhana?

Apabila Access Point tidak menggunakan password atau menggunakan password yang terlalu sederhana, jaringan akan lebih mudah diakses oleh pihak yang tidak memiliki izin.

Hal tersebut dapat menyebabkan:
- Perangkat asing dapat terhubung ke jaringan.
- Penggunaan sumber daya jaringan tanpa izin.
- Potensi akses terhadap layanan atau perangkat IoT.
- Gangguan terhadap komunikasi antarperangkat.

Oleh karena itu, Access Point sebaiknya menggunakan password yang cukup kuat dan tidak mudah ditebak.

### 4. Modifikasi program agar ESP32 berjalan dalam mode AP+STA

Program dimodifikasi menggunakan mode `WIFI_AP_STA` agar NodeMCU dapat berperan sebagai Station sekaligus Access Point.

```cpp
#include <ESP8266WiFi.h>

const char* sta_ssid = "NAMA_WIFI_ANDA";
const char* sta_password = "PASSWORD_WIFI_ANDA";

const char* ap_ssid = "ESP32_AccessPoint";
const char* ap_password = "12345678";

void setup() {
  Serial.begin(115200);

  // Menggunakan mode AP + Station
  WiFi.mode(WIFI_AP_STA);

  // Menghubungkan perangkat ke WiFi utama
  WiFi.begin(sta_ssid, sta_password);

  Serial.print("Menghubungkan ke WiFi utama");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi utama terhubung!");
  Serial.print("IP Station : ");
  Serial.println(WiFi.localIP());

  // Membuat Access Point
  WiFi.softAP(ap_ssid, ap_password);

  Serial.println("Access Point aktif!");
  Serial.print("SSID AP : ");
  Serial.println(ap_ssid);
  Serial.print("IP AP : ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  Serial.print("Client AP terhubung: ");
  Serial.println(WiFi.softAPgetStationNum());

  delay(5000);
}
```

Pada modifikasi ini digunakan `WIFI_AP_STA` sehingga NodeMCU dapat menjalankan dua fungsi sekaligus, yaitu sebagai Station dan Access Point.

NodeMCU terlebih dahulu terhubung ke WiFi utama menggunakan `WiFi.begin()`, kemudian membuat jaringan sendiri menggunakan `WiFi.softAP()`.

Dengan mode ini, NodeMCU bisa tetap terhubung ke jaringan utama sekaligus menyediakan jaringan yang dapat digunakan perangkat lain.

Mode tersebut dapat dimanfaatkan pada perangkat IoT yang membutuhkan koneksi ke jaringan utama tetapi juga membutuhkan Access Point untuk proses konfigurasi.

---

## F. Catatan Kendala

Pada percobaan 2B, seluruh rangkaian kegiatan dan pengujian dapat dilaksanakan dengan baik tanpa adanya kendala.

Penggunaan board NodeMCU ESP8266 yang berbeda dari modul, yaitu ESP32, dapat terakomodasi dengan melakukan penyesuaian library program menjadi `ESP8266WiFi.h`.

Nama Access Point pada program tetap disesuaikan menggunakan `ESP32_AccessPoint` untuk memenuhi ketentuan modul.

Berdasarkan hasil pengujian, Access Point berhasil menerima koneksi dari beberapa perangkat dan Serial Monitor menunjukkan peningkatan jumlah client yang terhubung hingga 4 perangkat. Hal ini membuktikan bahwa konfigurasi Access Point telah berfungsi dengan baik.

---

## G. Dokumentasi Praktikum

### Perangkaian Hardware

![Rangkaian NodeMCU ESP8266](Documentation/rangkaian%20ap.jpeg)

### Hasil Percobaan Access Point

![Hasil Serial Monitor Access Point](Documentation/serial%20monitor%20AP.jpeg)

## H. Kesimpulan

Pada percobaan 2B telah dilakukan konfigurasi NodeMCU ESP8266 sebagai Access Point. NodeMCU berhasil membuat jaringan WiFi sendiri dengan SSID `ESP32_AccessPoint` dan dapat diakses oleh perangkat lain.

Berdasarkan hasil pengamatan, alamat IP default Access Point adalah `192.168.4.1`. Jumlah perangkat yang terhubung mengalami peningkatan dari 1 hingga 4 perangkat.

Percobaan ini menunjukkan bahwa NodeMCU dapat digunakan sebagai penyedia jaringan WiFi bagi perangkat lain. Selain itu, dengan menggunakan mode `WIFI_AP_STA`, NodeMCU dapat menjalankan fungsi Station dan Access Point secara bersamaan sehingga dapat dimanfaatkan dalam proses konfigurasi maupun komunikasi pada sistem IoT.
