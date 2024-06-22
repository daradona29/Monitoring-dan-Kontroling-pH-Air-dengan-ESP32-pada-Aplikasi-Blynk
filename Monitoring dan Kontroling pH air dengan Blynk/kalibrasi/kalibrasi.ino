const int ph_Pin = 32; // Gunakan pin yang sesuai untuk pembacaan analog pada ESP32
float TeganganPh4 = 0.0;
float TeganganPh7 = 0.0;

// Nilai pH untuk larutan buffer
const float pH4 = 4.0;
const float pH7 = 7.0;

void setup() {
  Serial.begin(115200); // Baud rate yang lebih tinggi untuk ESP32
  analogReadResolution(12); // Mengatur resolusi ADC ke 12-bit
  analogSetAttenuation(ADC_11db); // Mengatur attenuasi ADC

  Serial.println("Ketik 'calibrate' untuk memulai kalibrasi.");
  Serial.println("Gunakan 'ph4' untuk kalibrasi pada larutan pH 4.");
  Serial.println("Gunakan 'ph7' untuk kalibrasi pada larutan pH 7.");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readString();

    if (input.startsWith("calibrate")) {
      kalibrasi();
    }
  }

  if (TeganganPh4 != 0.0 && TeganganPh7 != 0.0) {
    hitungPH();
  }
  delay(1000); // Tunggu 1 detik sebelum pembacaan berikutnya
}

void kalibrasi() {
  Serial.println("Mulai kalibrasi...");
  Serial.println("Celupkan sensor ke larutan pH 4 dan ketik 'ph4'.");
  delay(5000); // Tunggu 5 detik untuk memberikan waktu mencelupkan sensor

  while (!Serial.available());
  String input = Serial.readString();
  if (input.startsWith("ph4")) {
    TeganganPh4 = bacaTegangan();
    Serial.print("Kalibrasi pH 4 selesai. Tegangan: ");
    Serial.println(TeganganPh4, 3);
  }

  Serial.println("Celupkan sensor ke larutan pH 7 dan ketik 'ph7'.");
  delay(5000); // Tunggu 5 detik untuk memberikan waktu mencelupkan sensor

  while (!Serial.available());
  input = Serial.readString();
  if (input.startsWith("ph7")) {
    TeganganPh7 = bacaTegangan();
    Serial.print("Kalibrasi pH 7 selesai. Tegangan: ");
    Serial.println(TeganganPh7, 3);
  }

  Serial.println("Kalibrasi selesai.");
}

float bacaTegangan() {
  int nilai_analog = analogRead(ph_Pin);
  return 3.3 / 4095.0 * nilai_analog;
}

void hitungPH() {
  int nilai_analog_PH = analogRead(ph_Pin);
  float TeganganPh = 3.3 / 4095.0 * nilai_analog_PH;

  Serial.print("Nilai ADC Ph: ");
  Serial.println(nilai_analog_PH);
  Serial.print("TeganganPh: ");
  Serial.println(TeganganPh, 3);

  float PH_step = (pH7 - pH4) / (TeganganPh7 - TeganganPh4);
  float Po = pH7 + ((TeganganPh - TeganganPh7) * PH_step);

  Serial.print("Nilai PH Cairan: ");
  Serial.println(Po, 2);
}
