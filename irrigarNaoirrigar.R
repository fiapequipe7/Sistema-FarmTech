# Dados simulados
umidade <- c(50, 45, 75, 85, 55, 35, 65)
ph <- c(5.0, 6.5, 7.0, 6.3, 5.4, 4.7, 7.3)
nutrientes <- c(1, 0, 1, 1, 0, 1, 1) # 1 = bom, 0 = ruim

# 1 = irrigar, 0 = não irrigar
irrigar <- c(1, 1, 0, 0, 1, 1, 0)


#modelo estatistico

modelo <- lm(irrigar ~ umidade + ph + nutrientes)

#mostrar analise
summary(modelo)

#simulando leitura do esp 32
novo <- data.frame(
    umidade=80,
    ph = 7.0,
    nutrientes = 1
)

#previsão
resultado <- predict(modelo, novo)


#resultado
cat("Resultado:\n")
cat("Valor previsto", resultado, "\n")

if (resultado > 0.5){
  cat("Irrigar\n")
}else{
    cat("Não irrigar\n")
  }

