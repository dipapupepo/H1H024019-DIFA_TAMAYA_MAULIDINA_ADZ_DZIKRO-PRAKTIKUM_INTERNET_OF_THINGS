#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Ganti dengan Wi-Fi kamu
const char* ssid = "dips!";
const char* password = "12345678";

// Broker MQTT
const char* broker = "broker.hivemq.com";
const int port = 1883;

// Topic harus sama persis dengan topic Python
const char* topik = "unsoed/tkomputer/iot/modul5/suhu";

WiFiClient espClient;
PubSubClient mqtt(espClient);

// Membuat data suhu simulasi, tanpa sensor DHT
float bacaSuhuMock() {
  float suhu = 28.0 + random(-100, 101) / 100.0;

  // Sesekali menambahkan lonjakan suhu
  if (random(0, 20) == 0) {
    suhu += 8.0;
  }

  return suhu;
}

void sambungMQTT() {
  while (!mqtt.connected()) {
    String id = "esp-modul5-" + String(random(0xffff), HEX);

    Serial.print("Menghubungkan ke MQTT... ");

    if (mqtt.connect(id.c_str())) {
      Serial.println("berhasil!");
    } else {
      Serial.print("Gagal, kode: ");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(micros());

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi terhubung!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  mqtt.setServer(broker, port);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(500);
    return;
  }

  if (!mqtt.connected()) {
    sambungMQTT();
  }

  mqtt.loop();

  float suhu = bacaSuhuMock();

  char pesan[32];
  snprintf(pesan, sizeof(pesan), "{\"suhu\":%.2f}", suhu);

  if (mqtt.publish(topik, pesan)) {
    Serial.print("Data terkirim: ");
    Serial.println(pesan);
  } else {
    Serial.println("Pengiriman data gagal");
  }

  delay(5000);
}