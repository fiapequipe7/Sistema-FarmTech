import requests

API_KEY = "35b59346da2c1ad9d401a6f402bbd7c1"
cidade = "Florianopolis"

url = f"https://api.openweathermap.org/data/2.5/forecast?q={cidade}&appid={API_KEY}&units=metric"

resposta = requests.get(url)
dados = resposta.json()

chuva_total = 0

# Cada item representa previsão de 3 em 3 horas
for previsao in dados["list"][:8]:  # próximas 24h (8 blocos de 3h)
    if "rain" in previsao:
        chuva_total += previsao["rain"].get("3h", 0)

print(f"Chuva prevista próximas 24h: {chuva_total} mm")

if chuva_total >= 10:
    print("BLOQUEAR_IRRIGACAO")
else:
    print("IRRIGACAO_NORMAL")
