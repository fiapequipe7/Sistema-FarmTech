
"""
===========================================================
    FARMTECH SOLUTIONS - FASE 2
    Integração com API Meteorológica (OpenWeather)

    Objetivo:
    Consultar a previsão de chuva para as próximas 24 horas
    e decidir se a irrigação deve ser bloqueada para
    a cultura de cana-de-açúcar.

    Regra Agronômica:
    - Se chuva prevista >= 10 mm → BLOQUEAR_IRRIGACAO
    - Caso contrário → IRRIGACAO_NORMAL

    Autor: Equipe FarmTech
    ===========================================================
"""

import requests
from typing import Dict
import unicodedata


# ==============================
# CONFIGURAÇÕES
# ==============================

LIMITE_CHUVA_MM = 10  # limite técnico para cana-de-açúcar (mm em 24h)

API_KEY = "35b59346da2c1ad9d401a6f402bbd7c1"


# ==============================
# FUNÇÕES
# ==============================


def normalizar_texto(texto: str) -> str:
    """
    Remove acentos, converte para minúsculo e remove espaços extras.
    """
    texto = unicodedata.normalize("NFD", texto)
    texto = texto.encode("ascii", "ignore").decode("utf-8")
    return texto.strip().lower()


def obter_dados_climaticos(cidade: str, api_key: str) -> dict:
    """
    Consulta API OpenWeather e valida cidade de forma rigorosa.
    """

    if not api_key:
        raise ValueError(
            "API_KEY não configurada. "
            "Defina OPENWEATHER_API_KEY."
        )

    # Força país Brasil para evitar ambiguidade
    url = (
        "https://api.openweathermap.org/data/2.5/forecast"
        f"?q={cidade},BR&appid={api_key}&units=metric"
    )

    try:
        response = requests.get(url, timeout=10)

        if response.status_code == 404:
            raise ValueError("Cidade não encontrada.")

        if response.status_code == 401:
            raise PermissionError("API_KEY inválida ou não autorizada.")

        if response.status_code == 429:
            raise RuntimeError("Limite de requisições da API excedido.")

        response.raise_for_status()

    except requests.exceptions.Timeout:
        raise TimeoutError("Tempo de conexão excedido.")

    except requests.exceptions.ConnectionError:
        raise ConnectionError("Falha de conexão com a internet.")

    except requests.exceptions.RequestException as e:
        raise RuntimeError(f"Erro inesperado na requisição: {e}")

    dados = response.json()

    cidade_retornada = dados.get("city", {}).get("name", "")
    
    # Normalização forte
    cidade_digitada_norm = normalizar_texto(cidade)
    cidade_retornada_norm = normalizar_texto(cidade_retornada)

    if cidade_digitada_norm != cidade_retornada_norm:
        raise ValueError(
            f"Cidade inválida. API retornou '{cidade_retornada}', "
            f"que não corresponde exatamente a '{cidade}'."
        )

    return dados

def calcular_chuva_24h(dados: Dict) -> float:
    """
    Soma o volume de chuva previsto para as próximas 24h.

    A API fornece dados a cada 3h.
    8 blocos representam 24 horas.

    :param dados: JSON retornado pela API
    :return: Volume total de chuva (mm)
    """

    chuva_total = 0.0
    previsoes = dados.get("list", [])[:8]

    for previsao in previsoes:
        if "rain" in previsao:
            chuva_total += previsao["rain"].get("3h", 0.0)

    return chuva_total


def decidir_irrigacao(chuva_mm: float, limite_mm: float) -> str:
    """
    Aplica regra agronômica para bloqueio da irrigação.

    :param chuva_mm: Volume previsto de chuva
    :param limite_mm: Limite adotado
    :return: Decisão final
    """

    if chuva_mm >= limite_mm:
        return "BLOQUEAR_IRRIGACAO"
    return "IRRIGACAO_NORMAL"


# ==============================
# EXECUÇÃO PRINCIPAL
# ==============================

def main() -> None:

    print("=========================================")
    print(" SISTEMA DE PREVISAO METEOROLOGICA")
    print(" Cultura: Cana-de-acucar")
    print("=========================================")

    cidade = input(
        "Digite o nome da cidade para consulta meteorológica: "
    ).strip()

    # Validação básica da entrada
    if not cidade:
        print("Erro: Nome da cidade não pode ser vazio.")
        return

    try:
        dados = obter_dados_climaticos(cidade, API_KEY)
        chuva_total = calcular_chuva_24h(dados)
        decisao = decidir_irrigacao(chuva_total, LIMITE_CHUVA_MM)

        print("\n----------- RESULTADO -----------")
        print(f"Cidade consultada: {cidade}")
        print(f"Chuva prevista (24h): {chuva_total:.2f} mm")
        print(f"Limite adotado: {LIMITE_CHUVA_MM} mm")
        print(f"DECISAO: {decisao}")
        print("---------------------------------")

    except Exception as erro:
        print("\nErro ao consultar dados meteorológicos:")
        print(f"Detalhe: {erro}")


if __name__ == "__main__":
    main()