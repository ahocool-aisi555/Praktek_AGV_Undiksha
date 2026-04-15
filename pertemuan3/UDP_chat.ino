//UDP chat Room
//by : nyoman yudi kurniawan
//untuk : TRSE Undiksha 2026
//www.aisi555.com

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Konfigurasi WiFi
const char* ssid = "PraktikumMikro";
const char* password = "pr4ktikum";

// Konfigurasi IP Statis
IPAddress local_IP(192, 168, 0, 10); //Ubah IP paling belakang agar tidak bentrok dengan teman
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// Konfigurasi UDP
WiFiUDP Udp;
unsigned int localPort = 7788;      // Port untuk menerima
unsigned int targetPort = 7789;     // Port untuk mengirim (broadcast)
char packetBuffer[255];             // Buffer untuk menyimpan pesan masuk

void setup() {
  Serial.begin(115200);
  
  // Setting IP Statis
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Memulai UDP di port 7788
  Udp.begin(localPort);
  Serial.printf("Mendengarkan di port %d\n", localPort);
}

void loop() {
  // 1. Menerima Data dari Port 7788
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.printf("Diterima: %s\n", packetBuffer);
  }

  // 2. Mengirim Data via Konsol (Serial Monitor) ke Port 7789 (Broadcast)
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    
    // Alamat Broadcast (255) agar diterima semua perangkat di subnet yang sama
    Udp.beginPacket(IPAddress(192, 168, 0, 255), targetPort);
    Udp.write(message.c_str());
    Udp.endPacket();
    
    Serial.printf("Terkirim ke broadcast: %s\n", message.c_str());
  }
}
