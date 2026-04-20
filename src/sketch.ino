#include <Arduino.h>
#include <DHT.h>

/*
=========================================================
  FARMTECH SOLUTIONS - FASE 2
  Sistema de Irrigação Inteligente
  Cultura: Cana-de-açúcar

  Integração manual com API meteorológica (Python):
  - Se chuva prevista >= 10 mm → bloquear irrigação
  - Atualizar variável: bloquearPorChuva
=========================================================
*/

// ===================== DHT =====================
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===================== PINOS =====================
#define BTN_N 18
#define BTN_P 19
#define BTN_K 21

#define LDR_PIN 34
#define RELE 25

// ===================== ESTADO DOS BOTÕES (TOGGLE) =====================
bool estadoN = false;
bool estadoP = false;
bool estadoK = false;

bool ultimoN = HIGH;
bool ultimoP = HIGH;
bool ultimoK = HIGH;

// ===================== CONFIGURAÇÃO CHUVA (MANUAL) =====================
bool bloquearPorChuva = true;  // <-- ALTERAR MANUALMENTE

// ===================== PARÂMETROS AGRÍCOLAS =====================
const float PH_MIN = 5.5;
const float PH_MAX = 6.5;
const float UMIDADE_MINIMA = 60.0;

void setup() {

  Serial.begin(115200);

  pinMode(BTN_N, INPUT_PULLUP);
  pinMode(BTN_P, INPUT_PULLUP);
  pinMode(BTN_K, INPUT_PULLUP);

  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, LOW);

  dht.begin();

  Serial.println("=== SISTEMA INICIADO ===");
}

void loop() {

  // ===================== TOGGLE DOS BOTÕES =====================
  bool leituraN = digitalRead(BTN_N);
  bool leituraP = digitalRead(BTN_P);
  bool leituraK = digitalRead(BTN_K);

  if (ultimoN == HIGH && leituraN == LOW) {
    estadoN = !estadoN;
    delay(200);
  }

  if (ultimoP == HIGH && leituraP == LOW) {
    estadoP = !estadoP;
    delay(200);
  }

  if (ultimoK == HIGH && leituraK == LOW) {
    estadoK = !estadoK;
    delay(200);
  }

  ultimoN = leituraN;
  ultimoP = leituraP;
  ultimoK = leituraK;

  // ===================== LEITURA DO LDR (Simulando pH) =====================
  int ldrValue = analogRead(LDR_PIN);
  float pH = (ldrValue / 4095.0) * 14.0;

  // ===================== LEITURA DA UMIDADE =====================
  float umidade = dht.readHumidity();

  if (isnan(umidade)) {
    Serial.println("Erro ao ler DHT22!");
    delay(2000);
    return;
  }

  // ===================== DEBUG =====================
  Serial.println("\n------ STATUS DO SISTEMA ------");
  Serial.print("Nitrogenio (N): "); Serial.println(estadoN ? "OK" : "BAIXO");
  Serial.print("Fosforo (P): "); Serial.println(estadoP ? "OK" : "BAIXO");
  Serial.print("Potassio (K): "); Serial.println(estadoK ? "OK" : "BAIXO");
  Serial.print("pH do Solo: "); Serial.println(pH);
  Serial.print("Umidade do Solo (%): "); Serial.println(umidade);
  Serial.print("Bloqueio por Chuva: "); Serial.println(bloquearPorChuva ? "SIM" : "NAO");

  // ===================== LÓGICA AGRÍCOLA =====================
  bool soloSeco = (umidade < UMIDADE_MINIMA);
  bool phInadequado = (pH < PH_MIN || pH > PH_MAX);
  bool nutrientesBaixos = (!estadoN || !estadoP || !estadoK);

  bool precisaIrrigar = false;

  if (bloquearPorChuva) {

    precisaIrrigar = false;
    Serial.println("Motivo: Chuva prevista >= 10mm (Irrigacao Bloqueada)");

  } else {

    if (soloSeco) {
      precisaIrrigar = true;
      Serial.println("Motivo: Solo seco");
    }

    if (phInadequado) {
      precisaIrrigar = true;
      Serial.println("Motivo: pH fora da faixa ideal");
    }

    if (nutrientesBaixos) {
      precisaIrrigar = true;
      Serial.println("Motivo: NPK abaixo do ideal");
    }
  }

  // ===================== CONTROLE DO RELÉ =====================
  if (precisaIrrigar) {
    digitalWrite(RELE, HIGH);
    Serial.println(">>> IRRIGACAO LIGADA");
  } else {
    digitalWrite(RELE, LOW);
    Serial.println(">>> IRRIGACAO DESLIGADA");
  }

  delay(2000);
}