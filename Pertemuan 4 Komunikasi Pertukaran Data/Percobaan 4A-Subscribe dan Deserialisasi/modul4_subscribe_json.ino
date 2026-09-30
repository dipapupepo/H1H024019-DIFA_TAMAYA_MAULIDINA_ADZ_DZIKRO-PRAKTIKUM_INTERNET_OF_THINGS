#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

const char* ssid = "Kelompok1ShiftB";
const char* password = "12345678";

const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

// Topic untuk mengirim data suhu
const char* topicData =
  "unsoed/tk245004/kelompokDD/data";

// Topic untuk menerima perintah LED
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

  // Mengubah payload MQTT menjadi String
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  Serial.print("Pesan diterima [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(pesan);

  // Deserialisasi JSON
  JsonDocument doc;

  DeserializationError error =
    deserializeJson(doc, pesan);

  if (error) {
    Serial.print("Gagal parsing JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Mengambil nilai perintah
  const char* perintah = doc["perintah"];

  // Mengendalikan LED
  if (String(perintah) == "ON") {

    digitalWrite(ledPin, HIGH);

    Serial.println("Perintah diterima -> Aktuator: ON");

  }
  else if (String(perintah) == "OFF") {

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

      // Subscribe topic perintah
      client.subscribe(topicPerintah);

      Serial.print("Subscribe ke topic: ");
      Serial.println(topicPerintah);

    }
    else {

      Serial.print("gagal, rc=");
      Serial.print(client.state());

      Serial.println(" coba lagi dalam 2 detik");

      delay(2000);
    }
  }
}

void setup() {

  Serial.begin(115200);

  // LED sebagai OUTPUT
  pinMode(ledPin, OUTPUT);

  // LED awal mati
  digitalWrite(ledPin, LOW);

  // Memulai DHT11
  dht.begin();

  // Hubungkan WiFi
  hubungkanWiFi();

  // Konfigurasi MQTT
  client.setServer(mqttServer, mqttPort);

  // Daftarkan callback
  client.setCallback(callback);
}


void loop() {

  // Jika MQTT terputus, hubungkan kembali
  if (!client.connected()) {
    hubungkanMQTT();
  }

  // Memproses pesan MQTT
  client.loop();


  if (millis() - waktuTerakhirPublish >= intervalPublish) {

    waktuTerakhirPublish = millis();

    // Membaca suhu DHT11
    float suhu = dht.readTemperature();

    // Memastikan pembacaan berhasil
    if (!isnan(suhu)) {

      // Membuat JSON
      JsonDocument doc;

      doc["suhu"] = suhu;

      // Menyimpan JSON ke buffer
      char buffer[128];

      serializeJson(doc, buffer);

      // Publish data suhu
      client.publish(topicData, buffer);

      // Menampilkan data di Serial Monitor
      Serial.print("Data terkirim: ");
      Serial.println(buffer);

    }
    else {

      Serial.println("Gagal membaca sensor DHT11.");

    }
  }
}