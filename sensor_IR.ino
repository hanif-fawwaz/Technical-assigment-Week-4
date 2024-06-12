#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "Drone";          // Ganti dengan SSID WiFi Anda
const char* password = "1234567890";  // Ganti dengan password WiFi Anda

const char* serverName = "http://192.168.137.1:5000/IR/data";


// Pin untuk sensor inframerah
const int irSensorPin = 34; // GPIO34 (ADC1)

// Ambang batas untuk mendeteksi keberadaan buah
const int threshold = 500; // Sesuaikan ambang batas ini berdasarkan pengujian sensor Anda

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");


void setup() {
  Serial.begin(115200);

  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { // Cek koneksi WiFi
    
    // Memperbarui waktu dari NTP server
    timeClient.update();
    String formattedTime = timeClient.getFormattedTime();



    // Membaca nilai dari sensor inframerah
    int irValue = analogRead(irSensorPin);
    Serial.print("IR Sensor Value: ");
    Serial.println(irValue);

    // Menentukan apakah ada buah atau tidak berdasarkan nilai sensor
    String fruitPresence = (irValue < threshold) ? "Ya ada buah" : "Tidak ada buah";

    // Mengirim data ke server
    HTTPClient http;

    Serial.print("Connecting to ");
    Serial.println(serverName);
    http.begin(serverName); // Memulai koneksi ke server
    http.addHeader("Content-Type", "application/json"); // Menambahkan header

    // Data JSON yang akan dikirim
    String httpRequestData = "{\"sensor\":"+ String(irValue) + ",\"Ada Buah ?  \":  \"" + fruitPresence + "\", \"waktu\":\"" + formattedTime + "\"}";

    Serial.print("Sending data: ");
    Serial.println(httpRequestData);

    int httpResponseCode = http.POST(httpRequestData); // Mengirim data

    http.end(); // Mengakhiri koneksi
  } else {
    Serial.println("Error in WiFi connection");
  }

  delay(10000); // Mengirim data setiap 10 detik
}


