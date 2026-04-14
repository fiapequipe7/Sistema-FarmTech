#include <DHT.h>

// ===== DHT =====
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== PINOS =====
#define BTN_N 14
#define BTN_P 27
#define BTN_K 26

#define LDR_PIN 34
#define RELE 25

void setup() {
  Serial.begin(115200);

  // Botões (INPUT_PULLUP)
  pinMode(BTN_N, INPUT_PULLUP);
  pinMode(BTN_P, INPUT_PULLUP);
  pinMode(BTN_K, INPUT_PULLUP);

  // Relé
  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, LOW); // começa desligado

  dht.begin();
}

void loop() {

  // ===== LEITURA DOS BOTÕES =====
  bool N = !digitalRead(BTN_N); // invertido
  bool P = !digitalRead(BTN_P);
  bool K = !digitalRead(BTN_K);

  // ===== LEITURA DO LDR (pH) =====
  int ldrValue = analogRead(LDR_PIN);

  // Converte para escala de pH (0 a 14)
  float pH = (ldrValue / 4095.0) * 14.0;

  // ===== LEITURA DHT22 =====
  float umidade = dht.readHumidity();

  // ===== TRATAMENTO DE ERRO =====
  if (isnan(umidade)) {
    Serial.println("Erro ao ler DHT22!");
    return;
  }

  // ===== DEBUG =====
  Serial.println("------ SISTEMA ------");
  Serial.print("Nitrogenio (N): "); Serial.println(N ? "OK" : "BAIXO");
  Serial.print("Fosforo (P): "); Serial.println(P ? "OK" : "BAIXO");
  Serial.print("Potassio (K): "); Serial.println(K ? "OK" : "BAIXO");
  Serial.print("pH: "); Serial.println(pH);
  Serial.print("Umidade: "); Serial.println(umidade);

  // ===== LÓGICA DA IRRIGAÇÃO =====
  bool precisaIrrigar = false;

  // Regra 1: solo seco
  if (umidade < 60) {
    precisaIrrigar = true;
    Serial.println("Motivo: Solo seco");
  }

  // Regra 2: pH fora do ideal
  if (pH < 5.5 || pH > 6.5) {
    precisaIrrigar = true;
    Serial.println("Motivo: pH inadequado");
  }

  // Regra 3: nutrientes baixos
  if (!N || !P || !K) {
    if (N) 
    precisaIrrigar = true;
    Serial.println("Motivo: Nutrientes baixos");
  }
  


  // ===== CONTROLE DO RELÉ =====
  if (precisaIrrigar) {
    digitalWrite(RELE, HIGH);
    Serial.println(">>> IRRIGAÇÃO LIGADA");
  } else {
    digitalWrite(RELE, LOW);
    Serial.println(">>> IRRIGAÇÃO DESLIGADA");
  }

  delay(2000);

}