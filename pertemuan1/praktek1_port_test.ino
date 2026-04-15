//Testing Port wemos 
//by : nyoman yudi kurniawan
//untuk : TRSE Undiksha 2026
//www.aisi555.com


// Kofigurasi Pin (aktif HIGH , sesuaikan dengan pin yang digunakan)
#define MAJU_PIN    D1
#define MUNDUR_PIN  D2
#define KANAN_PIN   D5
#define KIRI_PIN    D6





void setup() {
  pinMode(MAJU_PIN, OUTPUT);
  pinMode(MUNDUR_PIN, OUTPUT);
  pinMode(KANAN_PIN, OUTPUT);
  pinMode(KIRI_PIN, OUTPUT);
}

void loop() {


 //Maju
  digitalWrite(MAJU_PIN, HIGH);
  digitalWrite(MUNDUR_PIN, LOW);
  digitalWrite(KANAN_PIN, LOW);
  digitalWrite(KIRI_PIN, LOW);

  delay(500);
 
 //Mundur
  digitalWrite(MAJU_PIN, LOW);
  digitalWrite(MUNDUR_PIN, HIGH);
  digitalWrite(KANAN_PIN, LOW);
  digitalWrite(KIRI_PIN, LOW);

  delay(500);
 
 //kanan
  digitalWrite(MAJU_PIN, LOW);
  digitalWrite(MUNDUR_PIN, LOW);
  digitalWrite(KANAN_PIN, HIGH);
  digitalWrite(KIRI_PIN, LOW);

  delay(500);

 //kiri
  digitalWrite(MAJU_PIN, LOW);
  digitalWrite(MUNDUR_PIN, LOW);
  digitalWrite(KANAN_PIN, LOW);
  digitalWrite(KIRI_PIN, HIGH);

  delay(500);
}
