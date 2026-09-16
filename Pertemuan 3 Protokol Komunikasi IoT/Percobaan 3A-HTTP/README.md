# Percobaan 3A – Komunikasi Data Menggunakan HTTP

## A. Deskripsi Percobaan

Percobaan 3A dilakukan untuk memahami dan mengimplementasikan pengiriman data dari NodeMCU ESP8266 ke server menggunakan protokol HTTP dengan metode POST dalam format JSON.

Pada praktikum ini digunakan NodeMCU ESP8266 sebagai pengganti ESP32 yang tercantum pada modul. Oleh karena itu, library yang digunakan disesuaikan menjadi `ESP8266WiFi.h` dan `ESP8266HTTPClient.h`. Selain itu, karena endpoint yang digunakan adalah HTTPS, ditambahkan library `WiFiClientSecure.h`.

Data yang dikirim berupa suhu, kelembaban, dan pada program modifikasi ditambahkan waktu menggunakan `millis()`. Data dikirim ke endpoint `httpbin.org/post`, kemudian server akan mengembalikan data tersebut sebagai response sehingga dapat digunakan untuk memverifikasi data yang dikirim.

## B. Tujuan Percobaan

1. Memahami konsep komunikasi menggunakan protokol HTTP pada sistem IoT.
2. Mengimplementasikan pengiriman data menggunakan metode HTTP POST.
3. Mengirimkan data dalam format JSON dari NodeMCU ke server.
4. Menampilkan response code dan response body dari server.
5. Menambahkan data waktu menggunakan `millis()` pada data JSON.

## C. Alat dan Bahan

- NodeMCU ESP8266
- Kabel USB
- Laptop/PC
- Arduino IDE
- Jaringan WiFi yang terhubung ke internet
- Library ArduinoJson
- Library ESP8266WiFi
- Library ESP8266HTTPClient
- Library WiFiClientSecure
- Endpoint pengujian `httpbin.org/post`

## D. Konfigurasi Rangkaian

Gambar menunjukkan NodeMCU yang terhubung dengan laptop melalui kabel USB. Seluruh pertukaran data dilakukan melalui jaringan WiFi sehingga tidak diperlukan rangkaian sensor tambahan.

### Diagram Rangkaian

```text
Laptop/PC
    |
    | Kabel USB
    |
    v
NodeMCU ESP8266
    |
    | WiFi
    v
Jaringan Internet
    |
    v
httpbin.org/post
```

### Dokumentasi Rangkaian

![Rangkaian HTTP dan MQTT](rangkaian%20http%26mqtt.jpeg)

## E. Program

Program utama yang digunakan:

```cpp
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";
const char* serverUrl = "https://httpbin.org/post";

// Client untuk koneksi HTTPS
WiFiClientSecure clientInsecure;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  // Tidak melakukan verifikasi sertifikat SSL
  clientInsecure.setInsecure();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    // Koneksi HTTPS menggunakan WiFiClientSecure
    http.begin(clientInsecure, serverUrl);

    http.addHeader("Content-Type", "application/json");

    // Membuat objek data sensor dalam format JSON
    JsonDocument doc;
    doc["suhu"] = 28.5;
    doc["kelembaban"] = 65.0;

    // Menambahkan waktu sejak ESP8266 dinyalakan
    doc["waktu"] = millis();

    String requestBody;
    serializeJson(doc, requestBody);

    Serial.print("Mengirim data: ");
    Serial.println(requestBody);

    // Mengirim data melalui HTTP POST
    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0) {
      Serial.print("Kode Response HTTP: ");
      Serial.println(httpResponseCode);

      Serial.println("Isi Response:");
      Serial.println(http.getString());
    } else {
      Serial.print("Pengiriman gagal, kode error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  delay(10000);
}
```

## F. Library dan Dependencies

### 1. ESP8266WiFi

```cpp
#include <ESP8266WiFi.h>
```

Digunakan untuk menghubungkan NodeMCU ESP8266 ke jaringan WiFi.

### 2. WiFiClientSecure

```cpp
#include <WiFiClientSecure.h>
```

Digunakan untuk membuat koneksi HTTPS dengan server.

### 3. ESP8266HTTPClient

```cpp
#include <ESP8266HTTPClient.h>
```

Digunakan untuk melakukan komunikasi HTTP dari NodeMCU ke server.

### 4. ArduinoJson

```cpp
#include <ArduinoJson.h>
```

Digunakan untuk membuat dan mengubah data menjadi format JSON.

## G. Penjelasan Program

Pada bagian `setup()`, NodeMCU menyambungkan diri ke jaringan WiFi terlebih dahulu karena komunikasi HTTP membutuhkan koneksi jaringan.

```cpp
WiFi.begin(ssid, password);
```

Digunakan untuk memulai koneksi ke jaringan WiFi.

```cpp
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
```

Program akan terus menunggu sampai NodeMCU berhasil terhubung ke WiFi.

Setelah terhubung, program mengaktifkan koneksi HTTPS:

```cpp
clientInsecure.setInsecure();
```

Perintah tersebut membuat ESP8266 tidak melakukan verifikasi sertifikat SSL. Pengaturan ini digunakan untuk kebutuhan praktikum dan tidak disarankan untuk sistem produksi.

Pada bagian `loop()`, status WiFi diperiksa terlebih dahulu.

```cpp
if (WiFi.status() == WL_CONNECTED)
```

Jika perangkat masih terhubung, program membuat objek HTTP:

```cpp
HTTPClient http;
```

Kemudian koneksi ke endpoint dilakukan menggunakan:

```cpp
http.begin(clientInsecure, serverUrl);
```

Header HTTP diatur sebagai JSON menggunakan:

```cpp
http.addHeader("Content-Type", "application/json");
```

Data suhu dan kelembaban dimasukkan ke dalam objek JSON:

```cpp
JsonDocument doc;
doc["suhu"] = 28.5;
doc["kelembaban"] = 65.0;
```

Pada program modifikasi ditambahkan data waktu:

```cpp
doc["waktu"] = millis();
```

`millis()` digunakan untuk mendapatkan waktu dalam milidetik sejak NodeMCU mulai dijalankan.

Selanjutnya objek JSON diubah menjadi teks menggunakan:

```cpp
serializeJson(doc, requestBody);
```

Data tersebut kemudian dikirim ke server menggunakan metode POST:

```cpp
int httpResponseCode = http.POST(requestBody);
```

Jika pengiriman berhasil, response code dan response body ditampilkan pada Serial Monitor.

```cpp
Serial.println(httpResponseCode);
Serial.println(http.getString());
```

Setelah proses selesai, koneksi HTTP ditutup menggunakan:

```cpp
http.end();
```

Program kemudian menunggu 10 detik sebelum melakukan pengiriman berikutnya.

## H. Hasil Pengamatan

Sesuai spesifikasi, NodeMCU berhasil terhubung ke WiFi dan mengirimkan data JSON ke server melalui HTTP POST.

Dari 10 kali pengiriman, 9 pengiriman berhasil dengan response code `200`. Isi response dari `httpbin.org` juga meng-echo data yang dikirim, yaitu suhu, kelembaban, dan waktu, sehingga data dapat diverifikasi telah diterima dengan benar.

Satu kali pengiriman, yaitu data ke-4, mengalami kegagalan dengan kode error `-1`. Error tersebut menunjukkan koneksi ke server terputus sesaat. Kemungkinan penyebabnya adalah jaringan WiFi yang sempat tidak stabil. Pengiriman berikutnya kembali berhasil tanpa perubahan program.

### Hasil Serial Monitor

![Serial Monitor HTTP](serial%20monitor%20http.jpeg)

## I. Pertanyaan Praktikum

### 1. Flowchart proses pengiriman data melalui HTTP POST

```text
          Mulai
            |
            v
     Inisialisasi Serial
            |
            v
     Hubungkan ke WiFi
            |
            v
     WiFi terhubung?
       /          \
     Tidak        Ya
       |           |
       |           v
       |     Membuat JSON
       |           |
       |           v
       |     Serialize JSON
       |           |
       |           v
       |      HTTP POST
       |           |
       |           v
       |   Cek Response Code
       |       /       \
       |    Berhasil   Gagal
       |       |         |
       |       v         v
       |    Tampilkan  Tampilkan
       |    Response   Error
       |       \         /
       |        \       /
       |         v     v
       |        Tunggu
       |       10 detik
       |          |
       └──────────┘
```

### 2. Apa fungsi `http.addHeader("Content-Type", "application/json")`?

Perintah tersebut menambahkan informasi pada header HTTP untuk memberi tahu server bahwa data yang dikirim pada body request berformat JSON.

Tanpa header tersebut, server dapat salah menafsirkan data yang masuk. Pada percobaan ini, response dari `httpbin.org` berhasil memisahkan data ke dalam field `json`, sehingga menunjukkan bahwa data JSON berhasil diproses dengan benar.

### 3. Apa arti kode response HTTP 200 dan contoh kode lainnya?

Kode `200 (OK)` berarti request berhasil diproses oleh server dan server memberikan response sesuai permintaan.

Contoh kode lainnya:
- `400 (Bad Request)` berarti format request dari client salah.
- `404 (Not Found)` berarti endpoint yang dituju tidak ditemukan.
- `500 (Internal Server Error)` berarti terjadi kesalahan pada sisi server.

Selain itu, terdapat kode negatif seperti `-1` pada library HTTPClient. Kode tersebut bukan kode HTTP resmi, tetapi menunjukkan bahwa koneksi ke server gagal terjalin.

### 4. Modifikasi program dengan menambahkan data waktu menggunakan `millis()`

Modifikasi dilakukan dengan menambahkan:

```cpp
doc["waktu"] = millis();
```

Baris tersebut menambahkan data waktu dalam milidetik sejak NodeMCU dinyalakan ke dalam objek JSON.

Karena endpoint menggunakan HTTPS, pada program juga ditambahkan:

```cpp
#include <WiFiClientSecure.h>
WiFiClientSecure clientInsecure;
```

Kemudian:

```cpp
clientInsecure.setInsecure();
```

digunakan agar ESP8266 tidak melakukan verifikasi sertifikat SSL.

Pemanggilan koneksi HTTP juga diubah menjadi:

```cpp
http.begin(clientInsecure, serverUrl);
```

sehingga request dapat dikirim melalui koneksi HTTPS.

## J. Catatan Kendala

Kendala terbesar pada percobaan 3A adalah terdapat beberapa kesalahan pada kode dan library yang kurang lengkap sehingga program mengalami error saat dijalankan. Kode kemudian diperbaiki dan library yang diperlukan ditambahkan secara manual.

Setelah program berhasil di-compile dan di-upload, Serial Monitor tidak menampilkan output karena port tidak terbaca di Arduino IDE. Percobaan dilakukan menggunakan dua laptop dan sempat mengganti board ESP, tetapi masalah tetap terjadi.

Masalah akhirnya teratasi setelah mengganti kabel USB dengan kabel milik kelompok lain. Hal tersebut menunjukkan bahwa kabel USB sebelumnya hanya berfungsi untuk mengisi daya tanpa jalur data.

Selain itu, terdapat satu kali kegagalan pengiriman data dengan kode error `-1` karena koneksi ke server sempat terputus akibat jaringan WiFi lab yang kurang stabil. Pengiriman berikutnya berhasil kembali tanpa perubahan program.

Kendala lainnya adalah nama file sketch masih menggunakan nama bawaan Arduino IDE, sehingga perlu diganti menjadi `modul3_komunikasi_http` sesuai ketentuan modul.

## K. Dokumentasi Praktikum

### Perangkaian Hardware

![Rangkaian HTTP dan MQTT](rangkaian%20http%26mqtt.jpeg)

### Hasil Serial Monitor

![Serial Monitor HTTP](serial%20monitor%20http.jpeg)

## L. Kesimpulan

Percobaan 3A berhasil menerapkan komunikasi data menggunakan protokol HTTP POST pada NodeMCU ESP8266. Data suhu, kelembaban, dan waktu berhasil dikemas dalam format JSON dan dikirimkan ke server `httpbin.org`.

Dari 10 kali pengiriman, 9 pengiriman berhasil mendapatkan response code `200`, sedangkan satu pengiriman mengalami error `-1` akibat koneksi yang sempat terputus. Percobaan ini menunjukkan bahwa HTTP dapat digunakan untuk mengirimkan data IoT ke server menggunakan format JSON.
