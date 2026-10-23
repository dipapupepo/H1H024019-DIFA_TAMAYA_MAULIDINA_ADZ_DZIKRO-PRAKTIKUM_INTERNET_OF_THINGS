#include <math.h>

// Jumlah data untuk moving average
const int N = 5;

// Tempat menyimpan 5 data suhu terakhir
float buffer[N];
int indeks = 0;
int jumlah = 0;

// Variabel suhu minimum dan maksimum
float suhuMin = 1000.0;
float suhuMax = -1000.0;

// Mock function pengganti sensor DHT22
float bacaSuhuMock() {
    static unsigned long jumlahBaca = 0;
    jumlahBaca++;

    // Suhu dasar berubah secara bertahap
    float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);

    // Noise kecil untuk mensimulasikan fluktuasi sensor
    float noise = random(-5, 6) * 0.1;

    float suhu = suhuDasar + noise;

    // Lonjakan suhu setiap 25 pembacaan
    if (jumlahBaca % 25 == 0) {
        suhu += 8.0;
    }

    return suhu;
}

// Fungsi menghitung moving average
float hitungMovingAverage(float nilaiBaru) {
    buffer[indeks] = nilaiBaru;

    // Berpindah ke posisi penyimpanan berikutnya
    indeks = (indeks + 1) % N;

    // Menghitung jumlah data yang sudah masuk
    if (jumlah < N) {
        jumlah++;
    }

    float total = 0;

    // Menjumlahkan data yang tersimpan
    for (int i = 0; i < jumlah; i++) {
        total += buffer[i];
    }

    // Mengembalikan nilai rata-rata
    return total / jumlah;
}

void setup() {
    // Membuka komunikasi dengan Serial Monitor
    Serial.begin(115200);

    // Mengacak nilai noise saat program dimulai
    randomSeed(micros());

    Serial.println("Percobaan 5A: Edge Processing");
    Serial.println("ESP8266 - Suhu Simulasi");
}

void loop() {
    // Mengambil suhu buatan sebagai pengganti DHT22
    float suhu = bacaSuhuMock();

    // Menghaluskan suhu menggunakan moving average
    float suhuFilter = hitungMovingAverage(suhu);

    // Memperbarui suhu minimum
    if (suhu < suhuMin) {
        suhuMin = suhu;
    }

    // Memperbarui suhu maksimum
    if (suhu > suhuMax) {
        suhuMax = suhu;
    }

    // Menampilkan semua hasil ke Serial Monitor
    Serial.print("Mentah: ");
    Serial.print(suhu, 2);

    Serial.print(" C | Filter: ");
    Serial.print(suhuFilter, 2);

    Serial.print(" C | Min: ");
    Serial.print(suhuMin, 2);

    Serial.print(" C | Max: ");
    Serial.print(suhuMax, 2);

    Serial.println(" C");

    // Membaca data setiap 2 detik
    delay(2000);
}