# Sistema-FarmTech

# 🌱 FarmTech Solutions – Fase 2
## Sistema de Irrigação Inteligente – Cana-de-Açúcar

---

## 📌 1. Sobre o Projeto

Este projeto simula um sistema de irrigação inteligente utilizando o ESP32 na plataforma Wokwi.

O sistema monitora:

- Umidade do solo
- pH do solo
- Níveis de Nitrogênio (N), Fósforo (P) e Potássio (K)

Com base nesses dados, o sistema decide automaticamente se deve ligar ou desligar a irrigação (representada por um relé).

---

## 🌾 2. Cultura Escolhida

A cultura escolhida foi **cana-de-açúcar**, devido à sua grande importância econômica no Brasil e à sua alta necessidade de controle hídrico e nutricional.

---

## 📊 3. Parâmetros Utilizados

### 🔹 pH do Solo
- Ideal: entre **5,5 e 6,5**

### 🔹 Umidade do Solo
- Ideal: entre **60% e 80%**
- Abaixo de 60% → solo seco
- Acima de 80% → risco de encharcamento

### 🔹 Nutrientes (NPK)
Os nutrientes são simulados por botões:

- Botão pressionado → nutriente adequado
- Botão não pressionado → nutriente inadequado

Para condição ideal:
- N adequado
- P adequado
- K adequado

---

## 🔌 4. Sensores Simulados

Como o Wokwi não possui sensores agrícolas específicos, utilizamos:

| Elemento Real | Simulação |
|--------------|-----------|
| Nitrogênio (N) | Botão |
| Fósforo (P) | Botão |
| Potássio (K) | Botão |
| pH do solo | LDR |
| Umidade do solo | DHT22 |
| Bomba d’água | Relé |

---

## 🧠 5. Lógica de Irrigação

A irrigação será ativada somente quando:

- Umidade < 60%
- pH entre 5,5 e 6,5
- N adequado
- P adequado
- K adequado

### Regra lógica:

IRRIGAR =
(umidade < 60)
AND (ph >= 5.5 AND ph <= 6.5)
AND (N_ok == true)
AND (P_ok == true)
AND (K_ok == true)


---

## 📋 6. Resumo da Decisão

| Situação | Irrigar? |
|----------|----------|
| Umidade ≥ 60% | Não |
| Umidade < 60% e pH fora da faixa | Não |
| Umidade < 60% e NPK inadequado | Não |
| Umidade < 60% + pH ideal + NPK ideal | Sim |

---

## 🎯 7. Objetivo do Sistema

- Evitar desperdício de água
- Melhorar eficiência agrícola
- Simular um sistema real de agricultura de precisão
- Aplicar conceitos de IoT e automação

---

## 👥 Integrantes

- Nome 1
- Nome 2

---

## 📹 Vídeo Demonstrativo

Link do vídeo no YouTube (não listado):

(Adicionar link aqui)