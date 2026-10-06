# Percobaan 4B – Pertukaran Data Dua Arah

## A. Deskripsi

Percobaan 4B merupakan pengembangan dari Percobaan 4A dengan menerapkan komunikasi data dua arah menggunakan MQTT. Pada percobaan ini, NodeMCU ESP8266 dapat berperan sebagai **publisher** untuk mengirim data sensor dan sebagai **subscriber** untuk menerima perintah kendali.

Data suhu dari sensor DHT11 dikirim ke broker MQTT secara berkala, sedangkan perintah `ON` dan `OFF` diterima dari MQTT Explorer untuk mengendalikan LED.

Dengan demikian, perangkat dapat melakukan publish dan subscribe secara bersamaan atau disebut juga komunikasi **full duplex**.

---

## B. Tujuan

Tujuan dari percobaan ini adalah:

1. Memahami konsep pertukaran data dua arah pada sistem IoT.
2. Mengimplementasikan publish dan subscribe secara bersamaan.
3. Mengirim data sensor melalui MQTT.
4. Menerima perintah kendali aktuator melalui MQTT.
5. Menerapkan mekanisme non-blocking menggunakan `millis()`.
6. Menganalisis proses pertukaran data dua arah pada sistem IoT.

---

## C. Alat dan Bahan

- NodeMCU ESP8266
- Sensor DHT11
- LED
- Resistor 220 Ohm
- Breadboard
- Kabel jumper
- Kabel USB
- Laptop/PC
- Arduino IDE
- Jaringan WiFi
- MQTT Explorer
- Broker MQTT `broker.hivemq.com`

### Library yang digunakan

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
```

---

## D. Konfigurasi Rangkaian

Pada percobaan ini NodeMCU ESP8266 digunakan sebagai perangkat utama untuk membaca sensor dan melakukan komunikasi MQTT.

| Komponen | Pin NodeMCU ESP8266 |
|---|---|
| DHT11 VCC | 3.3V |
| DHT11 DATA | D2 |
| DHT11 GND | GND |
| LED | D5 |
| GND LED | GND |

Sensor DHT11 digunakan untuk membaca suhu, sedangkan LED digunakan sebagai aktuator yang menerima perintah dari MQTT.

### Dokumentasi Rangkaian

![Rangkaian Percobaan 4B](Documentation/rangkaian%204b.jpeg)

---

## E. Program

Program digunakan untuk mengirim data suhu dari DHT11 dan menerima perintah kendali LED melalui MQTT.

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

const char* topicData =
  "unsoed/tk245004/kelompokDD/data";

const char* topicPerintah =
  "unsoed/tk245004/kelompokDD/perintah";

#define DHTPIN D2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int ledPin = D5;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long waktuTerakhirPublish = 0;
const long intervalPublish = 5000;

void callback(char* topic, byte* payload, unsigned int length) {

  String pesan;

  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  Serial.print("Pesan diterima [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(pesan);

  JsonDocument doc;

  DeserializationError error =
    deserializeJson(doc, pesan);

  if (error) {
    Serial.print("Gagal parsing JSON: ");
    Serial.println(error.c_str());
    return;
  }

  const char* perintah = doc["perintah"];

  if (String(perintah) == "ON") {

    digitalWrite(ledPin, HIGH);

    Serial.println("Perintah diterima -> Aktuator: ON");

  } else if (String(perintah) == "OFF") {

    digitalWrite(ledPin, LOW);

    Serial.println("Perintah diterima -> Aktuator: OFF");
  }
}

void hubungkanWiFi() {

  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void hubungkanMQTT() {

  while (!client.connected()) {

    Serial.print("Menghubungkan ke broker MQTT...");

    String clientId =
      "ESP8266Client-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {

      Serial.println("berhasil terhubung!");

      client.subscribe(topicPerintah);

      Serial.print("Subscribe ke topic: ");
      Serial.println(topicPerintah);

    } else {

      Serial.print("gagal, rc=");
      Serial.print(client.state());

      Serial.println(" coba lagi dalam 2 detik");

      delay(2000);
    }
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, LOW);

  dht.begin();

  hubungkanWiFi();

  client.setServer(mqttServer, mqttPort);

  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    hubungkanMQTT();
  }

  client.loop();

  if (millis() - waktuTerakhirPublish >= intervalPublish) {

    waktuTerakhirPublish = millis();

    float suhu = dht.readTemperature();

    if (!isnan(suhu)) {

      JsonDocument doc;

      doc["suhu"] = suhu;

      char buffer[128];

      serializeJson(doc, buffer);

      client.publish(topicData, buffer);

      Serial.print("Data terkirim: ");
      Serial.println(buffer);

    } else {

      Serial.println("Gagal membaca sensor DHT11.");
    }
  }
}
```

---

## F. Penjelasan Program

Pada Percobaan 4B terdapat dua proses utama, yaitu proses publish data sensor dan proses subscribe perintah kendali.

Data sensor dikirim melalui topic:

```text
unsoed/tk245004/kelompokDD/data
```

Sedangkan perintah kendali diterima melalui:

```text
unsoed/tk245004/kelompokDD/perintah
```

Pada proses publish, sensor DHT11 membaca suhu menggunakan:

```cpp
float suhu = dht.readTemperature();
```

Data tersebut kemudian dimasukkan ke dalam objek JSON:

```cpp
doc["suhu"] = suhu;
```

Setelah itu JSON diubah menjadi teks menggunakan `serializeJson()` dan dikirim melalui `client.publish()`.

Sementara itu, proses subscribe tetap menggunakan fungsi `callback()`. Ketika pesan masuk, payload dibaca kemudian dilakukan deserialisasi menggunakan `deserializeJson()`.

Untuk mengatur waktu pengiriman data digunakan `millis()`:

```cpp
if (millis() - waktuTerakhirPublish >= intervalPublish)
```

Interval publish ditentukan selama 5 detik.

Pendekatan ini digunakan agar program tidak berhenti terlalu lama seperti ketika menggunakan `delay()`. Dengan begitu, `client.loop()` tetap dapat dijalankan dan perangkat tetap dapat menerima pesan MQTT.

---

## G. Hasil Pengamatan

Pada pengujian, NodeMCU berhasil melakukan publish data suhu secara berkala dan menerima perintah dari MQTT Explorer.

Contoh data sensor yang dikirim:

```json
{
  "suhu": 27.1
}
```

Contoh perintah yang diterima:

```json
{
  "perintah": "ON"
}
```

dan:

```json
{
  "perintah": "OFF"
}
```

Hasil pengamatan menunjukkan bahwa data sensor tetap dapat dikirim ketika perangkat menerima perintah kendali. LED berubah sesuai dengan perintah yang dikirim melalui topic perintah.

| No | Proses | Hasil |
|---|---|---|
| 1 | NodeMCU terhubung ke WiFi | Berhasil |
| 2 | NodeMCU terhubung ke broker MQTT | Berhasil |
| 3 | Subscribe topic perintah | Berhasil |
| 4 | Publish data suhu | Berhasil |
| 5 | Menerima perintah `ON` | LED menyala |
| 6 | Menerima perintah `OFF` | LED mati |
| 7 | Publish dan subscribe berjalan bersamaan | Berhasil |

### Dokumentasi Serial Monitor

![Serial Monitor Percobaan 4B](Documentation/serial%20monitor%204b.jpeg)

---

## H. Pertanyaan Praktikum

### 1. Mengapa `delay()` yang lama sebaiknya dihindari?

`delay()` akan menghentikan sementara proses program. Jika digunakan terlalu lama, fungsi `client.loop()` juga tidak dapat berjalan dengan baik sehingga perangkat menjadi kurang responsif dalam menerima pesan MQTT.

Karena komunikasi dua arah membutuhkan proses publish dan subscribe secara bersamaan, penggunaan `millis()` lebih sesuai karena program tetap dapat menjalankan proses lainnya.

### 2. Bagaimana mekanisme non-blocking menggunakan `millis()`?

Program menyimpan waktu terakhir data dikirim pada variabel `waktuTerakhirPublish`. Nilai tersebut dibandingkan dengan `millis()` saat ini.

```cpp
if (millis() - waktuTerakhirPublish >= intervalPublish)
```

Jika selisih waktunya sudah mencapai 5 detik, data sensor akan dipublish. Jika belum, program tetap melanjutkan proses lainnya tanpa berhenti.

### 3. Apa yang terjadi jika `client.loop()` jarang dipanggil?

Jika `client.loop()` jarang dipanggil, proses komunikasi dengan broker dapat terganggu. Pesan yang masuk juga dapat terlambat diproses dan koneksi MQTT dapat terputus karena perangkat tidak cukup sering berkomunikasi dengan broker.

### 4. Apa yang terjadi jika koneksi ke broker terputus?

Program akan memeriksa kondisi koneksi melalui:

```cpp
if (!client.connected())
```

Jika koneksi terputus, fungsi `hubungkanMQTT()` dipanggil untuk mencoba melakukan koneksi kembali.

Program akan terus mencoba sampai koneksi berhasil. Setelah berhasil, ESP8266 kembali melakukan subscribe ke topic perintah.

### 5. Modifikasi untuk menambahkan topic kedua untuk aktuator buzzer

Topic kedua dapat digunakan untuk menerima perintah buzzer, misalnya:

```cpp
const char* topicPerintahBuzzer =
  "unsoed/tk245004/kelompokDD/perintah_buzzer";

const int buzzerPin = D6;
```

Topic tersebut kemudian di-subscribe:

```cpp
client.subscribe(topicPerintah);
client.subscribe(topicPerintahBuzzer);
```

Pada fungsi `callback()`, topic dapat diperiksa untuk menentukan aktuator yang dikendalikan.

```cpp
if (String(topic) == topicPerintah) {

  if (String(perintah) == "ON") {
    digitalWrite(ledPin, HIGH);
  } else if (String(perintah) == "OFF") {
    digitalWrite(ledPin, LOW);
  }

} else if (String(topic) == topicPerintahBuzzer) {

  if (String(perintah) == "ON") {
    digitalWrite(buzzerPin, HIGH);
  } else if (String(perintah) == "OFF") {
    digitalWrite(buzzerPin, LOW);
  }
}
```

Dengan demikian, LED dan buzzer dapat dikendalikan melalui topic yang berbeda.

---

## I. Analisis

Pada Percobaan 4B, NodeMCU ESP8266 dapat menjalankan dua fungsi komunikasi sekaligus, yaitu mengirim data sensor dan menerima perintah kendali.

Data suhu dikirim secara berkala menggunakan mekanisme publish. Sementara itu, perintah kendali diterima menggunakan mekanisme subscribe.

Penggunaan dua topic membuat jenis data dapat dibedakan. Topic `data` digunakan untuk data sensor, sedangkan topic `perintah` digunakan untuk perintah aktuator.

Penggunaan `millis()` juga membuat proses pengiriman data tidak menghambat `client.loop()`. Hal ini penting karena `client.loop()` diperlukan untuk memproses pesan masuk dan menjaga koneksi MQTT.

Dari percobaan ini dapat dilihat bahwa komunikasi dua arah lebih interaktif dibandingkan komunikasi satu arah karena perangkat dapat mengirim informasi sekaligus menerima perintah.

---

## J. Catatan Kendala

Kendala yang dialami pada Percobaan 4B adalah sensor DHT11 mengalami kerusakan di tengah pengujian. Setelah beberapa kali digunakan, sensor tidak lagi memberikan pembacaan suhu yang normal sehingga Serial Monitor menampilkan:

```text
Gagal membaca sensor DHT11.
```

Akibatnya, pengujian pembacaan suhu tidak dapat dilanjutkan secara penuh menggunakan sensor tersebut.

Meskipun demikian, proses komunikasi MQTT, publish-subscribe, penerimaan perintah, dan pengendalian LED masih dapat diamati. Data hasil pengamatan sensor yang digunakan merupakan data ketika DHT11 masih dapat membaca suhu dengan normal.

---

## K. Kesimpulan

Percobaan 4B berhasil menerapkan komunikasi data dua arah menggunakan MQTT pada NodeMCU ESP8266. Perangkat dapat melakukan publish data suhu dari sensor DHT11 sekaligus menerima perintah melalui mekanisme subscribe untuk mengendalikan LED.

Penggunaan `millis()` memungkinkan proses pengiriman data dilakukan secara berkala tanpa menghentikan proses `client.loop()`. Dengan demikian, perangkat tetap dapat menerima perintah ketika proses publish data sensor berlangsung.

Percobaan ini menunjukkan penerapan komunikasi bidirectional atau full duplex pada sistem IoT, yaitu perangkat dapat mengirim data dan menerima perintah dalam satu sistem komunikasi.
