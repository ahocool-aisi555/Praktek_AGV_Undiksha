//AGV Web Remote Mode AP
//by : nyoman yudi kurniawan
//untuk : TRSE Undiksha 2026
//www.aisi555.com

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Mode Access Point
const char* ssid = "MyAGV"; 

// Pin kendali (aktif HIGH , sesuaikan dengan pin yang digunakan)
#define MAJU_PIN    D1
#define MUNDUR_PIN  D2
#define KANAN_PIN   D5
#define KIRI_PIN    D6

ESP8266WebServer server(80);

String urutanInput = "";
volatile bool jalankan = false;
volatile bool henti = false;

void setup() {
  pinMode(MAJU_PIN, OUTPUT);
  pinMode(MUNDUR_PIN, OUTPUT);
  pinMode(KANAN_PIN, OUTPUT);
  pinMode(KIRI_PIN, OUTPUT);

  berhenti(); 

  Serial.begin(115200);

  WiFi.mode(WIFI_AP); //mode AP
  WiFi.softAP(ssid); //Tanpa password
  
  server.on("/", handleRoot);
  server.on("/kirim", HTTP_POST, handleKirim);
  server.on("/jalankan", HTTP_GET, handleJalankan);
  server.on("/berhenti", HTTP_GET, handleBerhenti);
  // Route tambahan untuk kontrol tombol instan
  server.on("/manual", HTTP_GET, handleManual);

  server.begin();
  Serial.println("Web server siap.");
}

void loop() {
  server.handleClient();

  if (jalankan && !henti) {
    jalankanUrutan(urutanInput);
    jalankan = false;
  }

  if (henti) {
    berhenti();
    henti = false;
  }
}

// --- Fungsi Gerakan ---
void maju(){

  digitalWrite(MAJU_PIN, HIGH);
  digitalWrite(MUNDUR_PIN, LOW);
  digitalWrite(KANAN_PIN, LOW);
  digitalWrite(KIRI_PIN, LOW);
  delay(500);
}

void mundur(){

  digitalWrite(MAJU_PIN, LOW);
  digitalWrite(MUNDUR_PIN, HIGH);
  digitalWrite(KANAN_PIN, LOW);
  digitalWrite(KIRI_PIN, LOW);

  delay(500);
}

 void kanan(){
  digitalWrite(MAJU_PIN, HIGH);
  digitalWrite(MUNDUR_PIN, LOW);
  digitalWrite(KANAN_PIN, HIGH);
  digitalWrite(KIRI_PIN, LOW);

  delay(500);

 }

 void kiri() {
  digitalWrite(MAJU_PIN, HIGH);
  digitalWrite(MUNDUR_PIN, LOW);
  digitalWrite(KANAN_PIN, LOW);
  digitalWrite(KIRI_PIN, HIGH);

  delay(500);

 }


 void berhenti(){

  digitalWrite(MAJU_PIN, LOW);
  digitalWrite(MUNDUR_PIN, LOW);
  digitalWrite(KANAN_PIN, LOW);
  digitalWrite(KIRI_PIN, LOW);
}

void eksekusiPerintah(int cmd) {
  if (henti) return;
  switch (cmd) {
    case 0: berhenti(); break;
    case 1: maju(); break;
    case 2: mundur(); break;
    case 3: kiri(); break;
    case 4: kanan(); break;
    default: berhenti(); break;
  }
}

void jalankanUrutan(String urutan) {
  if (urutan == "") return;
  int start = 0;
  int end = urutan.indexOf(',');
  while (end != -1) {
    eksekusiPerintah(urutan.substring(start, end).toInt());
    delay(500); // Jeda antar urutan
    start = end + 1;
    end = urutan.indexOf(',', start);
  }
  eksekusiPerintah(urutan.substring(start).toInt());
  delay(500);
  berhenti();
}

// --- Web Handlers ---
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>AGV Web Console</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: sans-serif; text-align: center; background-color: #2c3e50; color: white; margin: 0; padding: 20px; }
    .container { max-width: 400px; margin: auto; }
    /* Layout Tombol Panah */
    .d-pad { display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px; margin: 20px auto; width: 240px; }
    .btn-remote { width: 70px; height: 70px; font-size: 24px; border: none; border-radius: 10px; cursor: pointer; background: #34495e; color: white; touch-action: manipulation; }
    .btn-remote:active { background: #1abc9c; }
    .up { grid-column: 2; }
    .left { grid-column: 1; }
    .berhenti { grid-column: 2; background: #e74c3c; }
    .right { grid-column: 3; }
    .down { grid-column: 2; }
    
    textarea { width: 100%; margin-top: 20px; border-radius: 5px; padding: 10px; background: #ecf0f1; }
    .action-btn { padding: 10px 20px; margin: 10px; border-radius: 5px; border: none; font-weight: bold; cursor: pointer; }
    #status { margin-top: 15px; font-style: italic; color: #bdc3c7; }
  </style>
</head>
<body>
  <div class="container">
    <h1>AGV Console</h1>
    
    <div class="d-pad">
      <button class="btn-remote up" onclick="manual(1)">&#9650;</button>
      <button class="btn-remote left" onclick="manual(3)">&#9664;</button>
      <button class="btn-remote stop" onclick="manual(0)">&#9632;</button>
      <button class="btn-remote right" onclick="manual(4)">&#9654;</button>
      <button class="btn-remote down" onclick="manual(2)">&#9660;</button>
    </div>

    <hr>
    
    <textarea id="urutan" rows="3" placeholder="Petunjuk: 0(stop), 1(maju), 2(mundur), 3(kiri), 4(kanan). Contoh: 1,3,4,2"></textarea><br>
    <button class="action-btn" style="background:#2ecc71" onclick="kirim()">Jalankan Urutan</button>
    <button class="action-btn" style="background:#e74c3c" onclick="berhentiSemua()">Emergency berhenti</button>
    
    <div id="status">Status: Siap</div>
  </div>

  <script>
    function manual(id) {
      fetch('/manual?id=' + id);
      document.getElementById('status').innerText = 'Manual: ' + id;
    }
    function kirim() {
      const seq = document.getElementById('urutan').value;
      fetch('/kirim', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'urutan=' + encodeURIComponent(seq)
      }).then(() => fetch('/jalankan'));
    }
    function berhentiSemua() {
      fetch('/berhenti');
      document.getElementById('status').innerText = 'Status: Berhenti';
    }
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleManual() {
  if (server.hasArg("id")) {
    int cmd = server.arg("id").toInt();
    eksekusiPerintah(cmd);
    server.send(200, "text/plain", "OK");
  }
}

void handleKirim() {
  if (server.hasArg("urutan")) {
    urutanInput = server.arg("urutan");
    urutanInput.replace(" ", "");
    Serial.println("Sequence: " + urutanInput);
  }
  server.send(200, "text/plain", "OK");
}

void handleJalankan() { jalankan = true; server.send(200, "text/plain", "OK"); }
void handleBerhenti() { henti = true; server.send(200, "text/plain", "OK"); }
