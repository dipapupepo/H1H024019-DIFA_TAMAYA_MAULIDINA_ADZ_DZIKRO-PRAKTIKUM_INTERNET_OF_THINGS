# Percobaan 5B – Akuisisi, Analisis, dan Visualisasi Data IoT

## A. Deskripsi Praktikum

Percobaan 5B membahas pengolahan dan analisis data IoT pada sisi komputer menggunakan Python. Data suhu dikirimkan oleh NodeMCU ESP8266 melalui protokol MQTT, kemudian diterima oleh program Python untuk disimpan dalam file CSV, dianalisis menggunakan statistik dan metode z-score, serta divisualisasikan dalam bentuk grafik. Program juga dimodifikasi dengan menambahkan moving average menggunakan jendela lima data untuk membandingkan hasil deteksi anomali sebelum dan sesudah filtering.

## B. Tujuan Praktikum

1. Memahami proses akuisisi data IoT menggunakan protokol MQTT.
2. Menyimpan data suhu beserta waktu penerimaannya ke dalam file CSV.
3. Menghitung rata-rata, simpangan baku, nilai minimum, dan maksimum.
4. Mendeteksi anomali menggunakan metode z-score.
5. Memvisualisasikan data suhu dan hasil deteksi anomali.
6. Membandingkan hasil deteksi anomali sebelum dan sesudah penerapan moving average.

## C. Alat dan Bahan

- NodeMCU ESP8266
- Kabel USB
- Laptop atau komputer
- Arduino IDE
- Python
- Broker MQTT `broker.hivemq.com`
- Pustaka Python `paho-mqtt`, `pandas`, dan `matplotlib`
- File program ESP8266 dan Python
- Data suhu simulasi sebagai pengganti sensor DHT22

## D. Alur Kerja Program

Percobaan ini terdiri dari empat tahap utama.

1. **Akuisisi data MQTT**

   NodeMCU ESP8266 menghasilkan data suhu simulasi dan mengirimkannya secara berkala dalam format JSON melalui broker MQTT. Program Python melakukan subscribe ke topic `unsoed/tkomputer/iot/modul5/suhu` untuk menerima data tersebut.

2. **Penyimpanan data ke CSV**

   Setiap data suhu yang valid disimpan bersama waktu penerimaannya. Setelah 30 sampel terkumpul, data dimasukkan ke dalam DataFrame menggunakan pandas dan disimpan ke file `data_suhu.csv`.

3. **Analisis statistik dan deteksi anomali**

   Program menghitung rata-rata, simpangan baku, suhu minimum, dan suhu maksimum. Z-score digunakan untuk mengetahui seberapa jauh nilai suhu dari rata-rata. Data ditandai sebagai anomali apabila nilai absolut z-score lebih besar dari 2.

4. **Visualisasi dan modifikasi**

   Data suhu divisualisasikan menggunakan matplotlib. Pada program modifikasi, moving average dengan jendela lima data diterapkan sebelum perhitungan z-score diulang. Hasilnya digunakan untuk membandingkan jumlah anomali sebelum dan sesudah filtering.

## E. Penjelasan Program

### 1. Program ESP8266

Program `modul5_analisis_data.py.ino` digunakan untuk mengirimkan data suhu simulasi melalui MQTT.

- `bacaSuhuMock()` menghasilkan nilai suhu simulasi dengan fluktuasi kecil dan lonjakan sesekali.
- `mqtt.setServer(broker, port)` menentukan alamat broker MQTT yang digunakan.
- `sambungMQTT()` menangani koneksi ulang ke broker ketika koneksi belum tersedia.
- `mqtt.publish(topik, pesan)` mengirimkan data suhu dalam format JSON ke topic yang telah ditentukan.
- `mqtt.loop()` menjaga komunikasi MQTT tetap diproses.
- Data dikirim setiap lima detik agar program Python dapat menerima sampel secara berkala.

### 2. Program Python

Program `modul5_analisis_data.py` menjalankan proses pengumpulan dan analisis data.

- **`on_connect()`** digunakan untuk berlangganan ke topic MQTT setelah koneksi berhasil.
- **`on_message()`** memproses pesan yang masuk, mengubah JSON menjadi objek Python, mengambil nilai suhu, dan mencatat waktu penerimaan.
- **`try-except`** menangani pesan yang tidak valid agar kesalahan pengolahan satu pesan tidak langsung menghentikan keseluruhan program.
- **`pd.DataFrame(data)`** mengubah kumpulan data menjadi tabel untuk dianalisis.
- **`to_csv()`** menyimpan data ke file CSV.
- **Perhitungan z-score** digunakan untuk menentukan apakah suatu nilai menyimpang jauh dari rata-rata.
- **Matplotlib** digunakan untuk menggambarkan perubahan suhu dan menandai data yang terdeteksi sebagai anomali.

### 3. Program Modifikasi `modif5B.py`

Program modifikasi menambahkan proses moving average sebelum deteksi anomali dihitung kembali.

```python
df["suhu_ma5"] = df["suhu"].rolling(
    window=5,
    min_periods=1
).mean()
```

Kode tersebut menghitung rata-rata bergerak dari maksimal lima data suhu terakhir. Parameter `min_periods=1` memungkinkan perhitungan dimulai sejak data pertama meskipun jumlah data belum mencapai lima.

Setelah itu, program menghitung rata-rata dan simpangan baku dari kolom `suhu_ma5`, kemudian menentukan z-score dan status anomali yang baru. Jumlah anomali sebelum dan sesudah moving average ditampilkan untuk dibandingkan.

Program modifikasi juga menyimpan hasil analisis ke `data_suhu.csv` dan menghasilkan grafik perbandingan suhu asli dengan moving average dalam file `grafik_suhu.png`.

## F. Hasil Pengamatan

Berdasarkan salah satu hasil pengujian, program Python berhasil menerima 30 data suhu dari ESP8266 melalui MQTT. Data berhasil disimpan ke file CSV beserta waktu penerimaannya.

Hasil analisis pada pengujian tersebut menunjukkan:

| Parameter | Hasil |
|---|---:|
| Jumlah data | 30 |
| Rata-rata suhu | 28,78°C |
| Simpangan baku | 2,43 |
| Suhu minimum | 27,05°C |
| Suhu maksimum | 36,40°C |
| Anomali sebelum moving average | 3 data |

Grafik menunjukkan adanya lonjakan suhu yang menyimpang dari pola data lainnya. Titik yang memenuhi kriteria z-score ditandai sebagai anomali sehingga perubahan suhu lebih mudah diamati.

Pada program modifikasi, moving average menghasilkan data yang lebih halus. Namun, jumlah anomali tidak selalu berkurang. Pada salah satu pengujian modifikasi, jumlah anomali meningkat dari satu menjadi tiga setelah filtering. Hal ini terjadi karena pengaruh lonjakan suhu dapat menyebar ke beberapa titik berdekatan akibat proses perataan.

Jumlah anomali dapat berbeda antarpengujian karena data berasal dari fungsi simulasi yang menghasilkan variasi acak.

## G. Dokumentasi

### Gambar Rangkaian

![Rangkaian Percobaan 5B](Documentation/rangkaian.jpeg)

### Hasil Percobaan

![Hasil Percobaan 5B](Documentation/percobaan%205B.png)

### Dokumentasi Pengecekan

![Pengecekan Percobaan 5B](Documentation/cek%20ke%20percobaan%204.png)

## H. Pertanyaan Praktikum

### 1. Apa fungsi blok `try-except` pada fungsi `on_message()` dan apa yang terjadi jika dihilangkan?

Blok `try-except` digunakan untuk menangani kesalahan ketika pesan MQTT tidak dapat diurai sebagai JSON yang valid atau tidak memiliki data suhu yang dibutuhkan. Jika terjadi kesalahan, program menampilkan pesan bahwa data tidak valid dan mengabaikannya. Tanpa blok tersebut, pesan yang salah format dapat menimbulkan exception yang menghentikan proses pengumpulan data.

### 2. Apa arti z-score dan mengapa batas `|z| > 2` digunakan untuk menentukan anomali?

Z-score menunjukkan seberapa jauh suatu nilai data dari rata-rata dalam satuan simpangan baku. Pada praktikum ini, data dengan nilai absolut z-score lebih besar dari 2 ditandai sebagai anomali karena nilainya berada lebih dari dua simpangan baku dari rata-rata. Batas tersebut merupakan kriteria yang digunakan dalam praktikum, bukan jaminan bahwa setiap data yang ditandai pasti merupakan kesalahan sensor.

### 3. Apa kelemahan metode z-score ketika data memiliki tren naik atau turun yang tajam?

Z-score menggunakan rata-rata dan simpangan baku dari keseluruhan data sehingga kurang sesuai apabila data memiliki tren yang berubah secara tajam. Nilai yang sebenarnya normal pada suatu periode dapat dianggap anomali karena berbeda jauh dari rata-rata keseluruhan. Sebaliknya, simpangan baku yang besar akibat tren dapat menyebabkan lonjakan tertentu tidak terdeteksi.

### 4. Bagaimana pengaruh moving average terhadap deteksi anomali?

Moving average menghitung rata-rata dari maksimal lima data terakhir sehingga dapat menghaluskan fluktuasi suhu. Namun, lonjakan suhu dapat memengaruhi beberapa nilai rata-rata yang berurutan. Ketika z-score dihitung kembali dari hasil moving average, jumlah anomali dapat berkurang, tetap, atau bertambah bergantung pada pola data yang diterima. Oleh karena itu, hasil kedua metode perlu dibandingkan untuk mengetahui pengaruh filtering terhadap deteksi anomali.

## I. Analisis

Pengolahan data di sisi komputer memungkinkan analisis yang lebih kompleks dibandingkan pengolahan langsung pada ESP8266. Pada percobaan 5A, perangkat melakukan moving average, pencatatan nilai minimum dan maksimum, serta perhitungan simpangan baku dari buffer. Pada percobaan 5B, Python dapat mengumpulkan data historis, menyimpannya dalam CSV, menghitung statistik, mendeteksi anomali, dan membuat visualisasi.

Penggunaan MQTT memungkinkan data dari perangkat dikirimkan ke komputer melalui broker. Sementara itu, penyimpanan CSV membantu proses dokumentasi dan analisis ulang. Moving average dapat membantu mengurangi fluktuasi, tetapi tidak selalu meningkatkan hasil deteksi anomali karena lonjakan dapat memengaruhi beberapa titik setelah proses perataan.

Hasil analisis dan deteksi anomali dapat digunakan sebagai informasi pendukung untuk meningkatkan keandalan sistem IoT. Data yang terdeteksi menyimpang dapat diperiksa lebih lanjut sebelum digunakan sebagai dasar pengambilan keputusan otomatis.

## J. Catatan Kendala

Kendala utama pada praktikum adalah sensor DHT22 tidak tersedia sehingga data suhu yang dikirim ESP8266 berasal dari fungsi simulasi, bukan dari pengukuran sensor fisik. Data simulasi tetap dibuat memiliki fluktuasi, noise kecil, dan lonjakan suhu agar proses akuisisi, analisis statistik, dan deteksi anomali dapat diuji.

Di luar keterbatasan tersebut, proses penerimaan data melalui MQTT, penyimpanan ke CSV, perhitungan statistik, dan pembuatan grafik dapat dijalankan sesuai tujuan praktikum.

## K. Kesimpulan

1. Program Python berhasil menerima data suhu dari ESP8266 melalui protokol MQTT dan menyimpannya ke file CSV beserta waktu penerimaan.
2. Analisis statistik berhasil digunakan untuk menghitung rata-rata, simpangan baku, nilai minimum, dan maksimum suhu.
3. Metode z-score dapat digunakan untuk menandai data yang menyimpang berdasarkan batas `|z| > 2`.
4. Moving average dengan jendela lima data menghasilkan data yang lebih halus, tetapi tidak selalu mengurangi jumlah anomali yang terdeteksi.
5. Pengolahan di sisi komputer memungkinkan analisis dan visualisasi data IoT yang lebih kompleks, sedangkan pengolahan di sisi perangkat sesuai untuk perhitungan sederhana secara langsung.
