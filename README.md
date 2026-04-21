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
- bloquearPorChuva == false

### Regra lógica:

IRRIGAR =
(umidade < 60)
AND (ph >= 5.5 AND ph <= 6.5)
AND (N_ok == true)
AND (P_ok == true)
AND (K_ok == true)
AND (bloquearPorChuva == false)


---

## 📋 6. Resumo da Decisão

| Situação | Irrigar? |
|----------|----------|
| Umidade ≥ 60% | Não |
| Umidade < 60% e pH fora da faixa | Não |
| Umidade < 60% e NPK inadequado | Não |
| Previsão de chuva ≥ 10mm | Não |
| Umidade < 60% + pH ideal + NPK ideal | Sim |

---

## 🎯 7. Objetivo do Sistema

- Evitar desperdício de água
- Melhorar eficiência agrícola
- Simular um sistema real de agricultura de precisão
- Aplicar conceitos de IoT e automação

---

## 🖥️ 8. Explicação do Código

O código do ESP32 é dividido em quatro blocos principais:

### 1️⃣ Leitura dos Sensores

* `analogRead()` para LDR (pH)
* `dht.readHumidity()` para umidade
* `digitalRead()` para botões NPK

### 2️⃣ Conversão de pH

O valor analógico do LDR (0–4095) é convertido para escala 0–14:

```cpp
ph = (ldrValue / 4095.0) * 14.0;
3️⃣ Sistema de Toggle com Debounce

Os botões utilizam:

INPUT_PULLUP
Detecção de borda (HIGH → LOW)
Controle de debounce com millis()

Isso evita múltiplos acionamentos por ruído elétrico.

4️⃣ Tomada de Decisão

Um bloco condicional avalia todas as variáveis e:

Liga o relé (irrigação ON)
Desliga o relé (irrigação OFF)
Exibe resultado no Serial Monitor

Além disso, existe a variável:

bool bloquearPorChuva = false;

Essa variável é atualizada manualmente com base no resultado da API meteorológica.

---

## 🌧 9. IR ALÉM – Integração com API Meteorológica

Foi implementada uma camada adicional de decisão baseada em previsão de chuva utilizando Python e API OpenWeather.

🔹 Objetivo

Evitar irrigação quando houver previsão significativa de chuva nas próximas 24 horas.

🔹 Funcionamento do Script Python
O usuário informa a cidade.
O script consulta a API OpenWeather (forecast).
Soma a precipitação prevista nas próximas 24 horas.
Aplica a regra:
Se chuva ≥ 10 mm → BLOQUEAR_IRRIGACAO
Se chuva < 10 mm → IRRIGACAO_NORMAL
🔹 Exemplo de Saída
Chuva prevista (24h): 12.4 mm
DECISAO: BLOQUEAR_IRRIGACAO

ou

Chuva prevista (24h): 2.1 mm
DECISAO: IRRIGACAO_NORMAL
🔄 Integração com o ESP32

Como o ESP32 está em simulação (Wokwi), ele não acessa a internet diretamente.

Portanto, a integração é feita manualmente:

No sketch.ino:

bool bloquearPorChuva = false;

Se o Python retornar:

BLOQUEAR_IRRIGACAO → alterar para:
bool bloquearPorChuva = true;
IRRIGACAO_NORMAL → manter:
bool bloquearPorChuva = false;
🔐 Prioridade da Regra

Se bloquearPorChuva == true, o sistema NÃO irriga, independentemente das demais condições.

Isso adiciona uma camada climática inteligente ao sistema.

---

## 🔬 10. IR ALÉM – Modelo Estatístico em R

Além da lógica embarcada, foi desenvolvido um modelo estatístico preditivo em R utilizando regressão linear.

📊 Dados Simulados
umidade <- c(50, 45, 75, 85, 55, 35, 65)
ph <- c(5.0, 6.5, 7.0, 6.3, 5.4, 4.7, 7.3)
nutrientes <- c(1, 0, 1, 1, 0, 1, 1)
irrigar <- c(1, 1, 0, 0, 1, 1, 0)
📈 Modelo Estatístico
modelo <- lm(irrigar ~ umidade + ph + nutrientes)
summary(modelo)
🔮 Simulação de Nova Leitura
novo <- data.frame(
  umidade = 80,
  ph = 7.0,
  nutrientes = 1
)


resultado <- predict(modelo, novo)


if (resultado > 0.5) {
  cat("Irrigar\n")
} else {
  cat("Não irrigar\n")
}
🎯 Importância

Demonstra aplicação de:

Estatística aplicada
Modelo preditivo
Agricultura orientada por dados
Conceitos iniciais de Machine Learning

---

---

## 🔌 11. Circuito

O circuito foi montado utilizando os seguintes componentes:

* ESP32 DevKit V4  
* 3 botões (GPIO 18, 19 e 21)  
* Sensor DHT22  
* LDR (simulando pH)  
* Módulo relé  

### 📍 Conexões principais

| Componente | GPIO ESP32 |
|------------|------------|
| Botão N    | 18 |
| Botão P    | 19 |
| Botão K    | 21 |
| DHT22      | 4  |
| LDR (AO)   | 34 |
| Relé (IN)  | 25 |

Os botões utilizam `INPUT_PULLUP`, sendo acionados em nível lógico LOW (pressionado).

---

## 🚿 12. Funcionamento do Sistema

Fluxo operacional:

1. Sistema inicializa
2. Sensores são lidos continuamente
3. Estados dos nutrientes (NPK) são verificados via toggle
4. Previsão climática é considerada (variável manual)
5. Lógica de decisão é processada
6. Relé é acionado ou desligado
7. Status é exibido no Monitor Serial

O sistema opera em tempo real e reage imediatamente a alterações nos sensores ou botões.

---

## 💻 13. Ambiente de Desenvolvimento

O projeto foi desenvolvido utilizando:

* VSCode  
* PlatformIO  
* Wokwi  
* Linguagem C++ (ESP32)  
* Linguagem Python (API meteorológica)  
* Linguagem R (modelo estatístico)

### 🔧 PlatformIO

O uso do PlatformIO permitiu:

* Compilação local do firmware
* Organização estruturada do projeto
* Gerenciamento automático de dependências
* Ambiente profissional para desenvolvimento embarcado

### 🌐 Wokwi

O Wokwi foi utilizado para:

* Simulação do circuito eletrônico
* Testes rápidos de lógica
* Visualização do comportamento dos sensores

---

## 🎯 14. Objetivos Técnicos Atingidos

✔ Simulação de sensores agrícolas  
✔ Implementação de lógica condicional embarcada  
✔ Sistema de debounce com `millis()`  
✔ Controle de relé (atuador)  
✔ Integração manual com API meteorológica  
✔ Modelagem estatística em R  
✔ Separação entre camada embarcada e camada analítica  
✔ Aplicação prática de IoT na agricultura  

---

## 🔮 15. Melhorias Futuras

Possíveis evoluções do projeto:

* Integração automática da API via WiFi no ESP32
* Comunicação MQTT
* Dashboard web para monitoramento remoto
* Armazenamento de dados em banco de dados
* Sensor real de pH
* Modelo de Machine Learning embarcado
* Sistema autônomo com previsão climática automática
* Aplicativo mobile para monitoramento

---

## 👥 Integrantes

- Ricardo Colpani Sprocati de Oliveira
- Nome 2

---

## 📹 Vídeo Demonstrativo

Link do vídeo no YouTube (não listado):

https://www.youtube.com/watch?v=PolIm6TFaq4
