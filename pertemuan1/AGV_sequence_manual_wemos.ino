//Kontrol AGV secara manual lewat koding langsung
//by : nyoman yudi kurniawan
//untuk : TRSE Undiksha 2026
//www.aisi555.com



// Pin kendali (aktif HIGH , sesuaikan dengan pin yang digunakan)
#define MAJU_PIN    D5
#define MUNDUR_PIN  D1
#define KANAN_PIN   D6
#define KIRI_PIN    D2


void setup() {
  pinMode(MAJU_PIN, OUTPUT);
  pinMode(MUNDUR_PIN, OUTPUT);
  pinMode(KANAN_PIN, OUTPUT);
  pinMode(KIRI_PIN, OUTPUT);
}


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

 void mundur_kiri(){

  digitalWrite(MAJU_PIN, LOW);
  digitalWrite(MUNDUR_PIN, HIGH);
  digitalWrite(KANAN_PIN, HIGH);
  digitalWrite(KIRI_PIN, LOW);

  delay(500);
}

 void mundur_kanan(){

  digitalWrite(MAJU_PIN, LOW);
  digitalWrite(MUNDUR_PIN, HIGH);
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

void loop() {

 //sequence urutan pergerakan  RC Truck
 

  maju();
  maju();
  kiri();
  maju();
  kanan();
  mundur();
  mundur_kanan();
  mundur_kiri();
  mundur();
  mundur();
  mundur();
  berhenti();
  delay(10000);

}
