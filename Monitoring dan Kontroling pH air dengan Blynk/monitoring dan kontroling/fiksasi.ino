#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL65wAKJC0N"
#define BLYNK_TEMPLATE_NAME "Monitoring dan Kontroling pH Akuarium"
#define BLYNK_AUTH_TOKEN "FB6TY0aSiO65VW4qELQtnjQ4OyannV_a"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


// WiFi credentials
char ssid[] = "PERSEBAYASBY";
char pass[] = "bondowani123";

const int ph_Pin = 32; // Pin untuk pembacaan pH
const int relay1_Pin = 25; // Pin untuk mengontrol relay 1
const int relay2_Pin = 26; // Pin untuk mengontrol relay 2

float TeganganPh4 = 3.295; // Nilai tegangan saat kalibrasi pH 4
float TeganganPh7 = 3.208; // Nilai tegangan saat kalibrasi pH 7

// Nilai pH untuk larutan buffer
const float pH4 = 4.0;
const float pH7 = 7.0;

BlynkTimer timer;

// Fungsi untuk mengontrol selenoid valve secara manual
BLYNK_WRITE(V1) {
  int relay1_state = param.asInt();
  digitalWrite(relay1_Pin, relay1_state);
}

BLYNK_WRITE(V2) {
  int relay2_state = param.asInt();
  digitalWrite(relay2_Pin, relay2_state);
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  pinMode(relay1_Pin, OUTPUT);
  pinMode(relay2_Pin, OUTPUT);
  digitalWrite(relay1_Pin, LOW);
  digitalWrite(relay2_Pin, LOW);

  WiFi.begin(ssid, pass);
  int wifi_ctr=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup Blynk timer to call the function every second
  timer.setInterval(1000L, sendPhToBlynk);
}

void loop() {
  Blynk.run();
  timer.run(); // Menjalankan timer Blynk
}

float bacaTegangan() {
  int nilai_analog = analogRead(ph_Pin);
  return 3.3 / 4095.0 * nilai_analog;
}

void sendPhToBlynk() {
  int nilai_analog_PH = analogRead(ph_Pin);
  float TeganganPh = 3.3 / 4095.0 * nilai_analog_PH;
  float PH_step = (pH7 - pH4) / (TeganganPh7 - TeganganPh4);
  float Po = pH7 + ((TeganganPh - TeganganPh7) * PH_step);

  Blynk.virtualWrite(V0, Po); // Send pH value to Blynk app, adjust V0 to the virtual pin used in the app

  Serial.print("Nilai ADC Ph: ");
  Serial.println(nilai_analog_PH);
  Serial.print("TeganganPh: ");
  Serial.println(TeganganPh, 3);
  Serial.print("Nilai PH Cairan: ");
  Serial.println(Po, 2);
}
