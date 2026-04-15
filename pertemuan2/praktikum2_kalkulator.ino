//Membuat kalkulator di wemos mode AP
//by : nyoman yudi kurniawan
//untuk : TRSE Undiksha 2026
//www.aisi555.com

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Konfigurasi Access Point
const char* ssid = "Praktek_Kalkulator_ESP";

ESP8266WebServer server(80);

// Variabel untuk menyimpan hasil
float hasil = 0;
String operasi = "";

void setup() {
  Serial.begin(115200);

  // 1. Setting sebagai Access Point
  WiFi.softAP(ssid);
  Serial.println("\nAccess Point Aktif!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // 2. Routing
  server.on("/", handleRoot);
  server.on("/hitung", HTTP_GET, handleHitung);

  server.begin();
}

void loop() {
  server.handleClient();
}

// Tampilan Web (HTML)
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP8266 Kalkulator</title>
  <style>
    body { font-family: sans-serif; text-align: center; padding: 20px; background: #f4f4f4; }
    .box { background: white; padding: 20px; border-radius: 10px; display: inline-block; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }
    input { width: 60px; padding: 10px; margin: 5px; font-size: 18px; text-align: center; }
    select { padding: 10px; font-size: 18px; }
    button { padding: 10px 20px; font-size: 18px; background: #3498db; color: white; border: none; border-radius: 5px; cursor: pointer; }
    h2 { color: #2c3e50; }
  </style>
</head>
<body>
  <div class="box">
    <h2>Kalkulator Web Server</h2>
    <form action="/hitung">
      <input type="number" name="a" required step="any">
      <select name="op">
        <option value="tambah">+</option>
        <option value="kurang">-</option>
        <option value="kali">x</option>
        <option value="bagi">/</option>
      </select>
      <input type="number" name="b" required step="any">
      <br><br>
      <button type="submit">Hitung</button>
    </form>
    <hr>
    <h3>Hasil: )rawliteral" + String(hasil) + R"rawliteral(</h3>
  </div>
  <p>Data diproses oleh ESP8266</p>
</body>
</html>
)rawliteral";
  server.send(200, "text/html", html);
}

// Logika Perhitungan di Sisi Server
void handleHitung() {
  if (server.hasArg("a") && server.hasArg("b") && server.hasArg("op")) {
    float a = server.arg("a").toFloat();
    float b = server.arg("b").toFloat();
    String op = server.arg("op");

    if (op == "tambah") hasil = a + b;
    else if (op == "kurang") hasil = a - b;
    else if (op == "kali") hasil = a * b;
    else if (op == "bagi") hasil = (b != 0) ? a / b : 0;
  }
  
  // Setelah menghitung, arahkan kembali ke halaman utama untuk melihat hasil
  server.sendHeader("Location", "/");
  server.send(303);
}
