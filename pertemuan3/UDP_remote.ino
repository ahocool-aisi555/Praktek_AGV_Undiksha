#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


#define MAJU_PIN    D1
#define MUNDUR_PIN  D2
#define KANAN_PIN   D5
#define KIRI_PIN    D6

// Kredensial & Konfigurasi
const char* ssid = "PraktikumMikro";
const char* password = "pr4ktikum";
IPAddress local_IP(192, 168, 0, 10);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiUDP Udp;
unsigned int localPort = 7788;
char packetBuffer[10]; // Buffer kecil saja karena hanya kirim 1 karakter

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi Pin Motor (Sesuaikan dengan pin yang Anda gunakan sebelumnya)
  pinMode(MAJU_PIN, OUTPUT);
  pinMode(MUNDUR_PIN, OUTPUT);
  pinMode(KANAN_PIN, OUTPUT);
  pinMode(KIRI_PIN, OUTPUT);

  berhenti();


  // Memaksa mode ke Station saja (mematikan AP)
  WiFi.mode(WIFI_STA);
  
  // Memutuskan koneksi dan menghapus kredensial yang tersimpan di flash
  WiFi.disconnect(true); 
  
  // Mematikan SoftAP secara permanen
  WiFi.softAPdisconnect(true);

  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  
  Udp.begin(localPort);
  Serial.println("\nAGV Ready. Menunggu perintah UDP...");
}


void maju(){

  digitalWrite(MAJU_PIN, HIGH);
  digitalWrite(MUNDUR_PIN, LOW);
  digitalWrite(KANAN_PIN, LOW);
  digitalWrite(KIRI_PIN, LOW);

}

void mundur(){

  digitalWrite(MAJU_PIN, LOW);
  digitalWrite(MUNDUR_PIN, HIGH);
  digitalWrite(KANAN_PIN, LOW);
  digitalWrite(KIRI_PIN, LOW);

}

 void kanan(){
  digitalWrite(MAJU_PIN, HIGH);
  digitalWrite(MUNDUR_PIN, LOW);
  digitalWrite(KANAN_PIN, HIGH);
  digitalWrite(KIRI_PIN, LOW);

 }

 void kiri() {
  digitalWrite(MAJU_PIN, HIGH);
  digitalWrite(MUNDUR_PIN, LOW);
  digitalWrite(KANAN_PIN, LOW);
  digitalWrite(KIRI_PIN, HIGH);


 }

 void berhenti(){

  digitalWrite(MAJU_PIN, LOW);
  digitalWrite(MUNDUR_PIN, LOW);
  digitalWrite(KANAN_PIN, LOW);
  digitalWrite(KIRI_PIN, LOW);
}


void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, 10);
    if (len > 0) {
      char command = packetBuffer[0]; // Ambil karakter pertama
      eksekusiPerintah(command);
    }
  }
}

void eksekusiPerintah(char cmd) {
  switch (cmd) {
    case '1':
      Serial.println("Gerak: MAJU");
      maju();
      break;
    case '2':
      Serial.println("Gerak: MUNDUR");
      mundur();
      break;
    case '3':
      Serial.println("Gerak: KIRI");
      kiri();
      break;
    case '4':
      Serial.println("Gerak: KANAN");
      kanan();
      break;
    case '0':
      Serial.println("Gerak: BERHENTI");
      berhenti();
      break;
    default:
      return; // Abaikan jika karakter tidak dikenal
  }
  
  // Delay sesuai keinginan lama pencet tombol
  delay(400);

  // berhenti dan tidak disesuaikan kebutuhan remote  
  berhenti(); 
}
