# Percobaan 4A – Subscribe dan Deserialisasi Data JSON

## A. Deskripsi

Percobaan 4A membahas mekanisme **subscribe pada MQTT** dan proses **deserialisasi data JSON** pada ESP8266. Pada percobaan ini, NodeMCU ESP8266 digunakan untuk menerima perintah dari MQTT Explorer melalui topic tertentu. Perintah yang diterima berupa data JSON dengan nilai `ON` atau `OFF`, kemudian diproses untuk mengendalikan LED sebagai aktuator.

Pada modul digunakan ESP8266 sebagai perangkat IoT. Dalam pelaksanaan praktikum, NodeMCU ESP8266 digunakan sebagai board yang tersedia di laboratorium.

---

## B. Tujuan

Tujuan dari percobaan ini adalah:

1. Memahami mekanisme subscribe pada protokol MQTT.
2. Memahami proses deserialisasi data JSON pada ESP8266.
3. Mengimplementasikan penerimaan perintah kendali melalui MQTT.
4. Mengendalikan aktuator berdasarkan perintah yang diterima.
5. Mengamati proses penerimaan dan pemrosesan pesan MQTT.

---

## C. Alat dan Bahan

- NodeMCU ESP8266
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

Pada percobaan ini NodeMCU ESP8266 digunakan sebagai perangkat utama. LED digunakan sebagai aktuator yang dikendalikan berdasarkan perintah MQTT.

| Komponen | Pin NodeMCU ESP8266 |
|---|---|
| LED | D5 |
| GND LED | GND |

### Dokumentasi Rangkaian

![Rangkaian Percobaan 4A](Documentation/rangkaian%204a.jpeg)

---

## E. Program

Program digunakan untuk menghubungkan NodeMCU ESP8266 ke WiFi dan broker MQTT, melakukan subscribe pada topic perintah, kemudian memproses pesan JSON yang diterima untuk mengendalikan LED.

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

Program diawali dengan library yang digunakan untuk koneksi WiFi, komunikasi MQTT, pemrosesan JSON, dan sensor DHT11.

Pada bagian `setup()`, NodeMCU mengatur pin LED sebagai output, melakukan inisialisasi sensor, menghubungkan perangkat ke WiFi, mengatur broker MQTT, dan menentukan fungsi `callback()`.

Setelah berhasil terhubung ke broker, program menjalankan:

```cpp
client.subscribe(topicPerintah);
```

Perintah tersebut membuat NodeMCU mendaftarkan diri sebagai subscriber pada topic yang digunakan untuk menerima perintah.

Ketika pesan baru masuk, fungsi `callback()` dipanggil secara otomatis. Payload yang diterima diubah menjadi String, kemudian diproses menggunakan:

```cpp
deserializeJson(doc, pesan);
```

Jika JSON valid, nilai `perintah` diambil dari objek JSON. Apabila nilainya `ON`, LED dinyalakan. Apabila nilainya `OFF`, LED dimatikan.

Fungsi `client.loop()` dijalankan secara berkala pada `loop()` agar perangkat tetap dapat menerima pesan dan menjaga koneksi dengan broker MQTT.

---

## G. Hasil Pengamatan

Pengujian dilakukan dengan mengirimkan pesan JSON melalui MQTT Explorer.

Pesan untuk menyalakan LED:

```json
{
  "perintah": "ON"
}
```

Hasil pada Serial Monitor:

```text
Pesan diterima [unsoed/tk245004/kelompokDD/perintah]: {"perintah":"ON"}
Perintah diterima -> Aktuator: ON
```

Kemudian dikirim pesan:

```json
{
  "perintah": "OFF"
}
```

Hasil pada Serial Monitor:

```text
Pesan diterima [unsoed/tk245004/kelompokDD/perintah]: {"perintah":"OFF"}
Perintah diterima -> Aktuator: OFF
```

Dari hasil pengujian, LED berhasil merespons perintah yang dikirim melalui MQTT Explorer. Data JSON juga berhasil diproses oleh ESP8266 tanpa mengalami error parsing.

### Dokumentasi Serial Monitor

![Serial Monitor Percobaan 4A](Documentation/serial%20monitor%204a.jpeg)

---

## H. Pertanyaan Praktikum

### 1. Flowchart proses penerimaan dan pemrosesan pesan pada fungsi callback

```text
Pesan MQTT masuk
        ↓
Fungsi callback() dipanggil
        ↓
Payload dibaca
        ↓
Payload diubah menjadi String
        ↓
Deserialisasi JSON
        ↓
Apakah JSON valid?
     /       \
   Tidak      Ya
    ↓          ↓
Tampilkan    Ambil nilai
error        "perintah"
    ↓          ↓
 Return    Cek ON / OFF
             /     \
           ON       OFF
            ↓        ↓
        LED ON     LED OFF
```

### 2. Apa yang terjadi jika pesan yang dipublikasikan bukan JSON yang valid?

Jika pesan yang diterima bukan JSON yang valid, fungsi `deserializeJson()` akan menghasilkan error. Program kemudian menampilkan pesan kegagalan parsing pada Serial Monitor dan menghentikan proses pada fungsi `callback()`.

Dengan demikian, perintah tidak akan diproses dan kondisi LED tetap seperti sebelumnya.

### 3. Mengapa `client.subscribe()` dipanggil di dalam `hubungkanMQTT()`?

`client.subscribe()` dipanggil setelah koneksi ke broker berhasil karena subscription perlu dilakukan kembali ketika terjadi reconnect.

Jika koneksi MQTT terputus kemudian tersambung kembali, perangkat perlu melakukan subscribe kembali agar tetap menerima pesan dari topic yang digunakan.

### 4. Modifikasi program untuk mengatur intensitas LED menggunakan PWM

Data JSON dapat ditambahkan dengan nilai intensitas, contohnya:

```json
{
  "perintah": "ON",
  "intensitas": 200
}
```

Bagian program dapat diubah menjadi:

```cpp
int intensitas = doc["intensitas"] | 255;

if (String(perintah) == "ON") {

  analogWrite(ledPin, intensitas);

  Serial.print("Aktuator: ON, intensitas: ");
  Serial.println(intensitas);

} else if (String(perintah) == "OFF") {

  analogWrite(ledPin, 0);

  Serial.println("Aktuator: OFF");
}
```

Nilai `intensitas` digunakan untuk mengatur tingkat kecerahan LED menggunakan PWM. Nilai 255 digunakan sebagai nilai default apabila intensitas tidak dikirim.

---

## I. Catatan Kendala

Kendala pada Percobaan 4A adalah board NodeMCU ESP8266 yang digunakan sebelumnya mengalami masalah dan tidak dapat digunakan. Board kemudian diganti dengan unit lain agar percobaan dapat dilanjutkan.

Selain itu, karena hardware yang digunakan adalah NodeMCU ESP8266, konfigurasi pin disesuaikan dengan board. LED menggunakan pin D5.

Setelah board diganti dan konfigurasi pin disesuaikan, program dapat dijalankan dan LED berhasil merespons perintah `ON` dan `OFF` dari MQTT Explorer.

---

## J. Kesimpulan

Percobaan 4A berhasil menerapkan mekanisme subscribe MQTT dan deserialisasi data JSON pada NodeMCU ESP8266. Perintah yang dikirim melalui MQTT Explorer dapat diterima dan diproses oleh ESP8266 untuk mengendalikan LED.

Percobaan ini menunjukkan bahwa perangkat IoT tidak hanya dapat mengirim data, tetapi juga dapat menerima perintah dari perangkat atau aplikasi lain melalui mekanisme subscribe MQTT.
