# Sonda Meteorológica – Telemetria (Protótipo)

Protótipo de sonda meteorológica baseada em **Arduino Nano**, desenvolvida para coleta e transmissão de dados ambientais e de localização.

## Funcionalidades

- Leitura de GPS (NEO-6M)
- Medição de pressão e altitude (BMP280)
- Transmissão de dados via LoRa (UART)
- Comunicação serial para depuração

## Status

Protótipo / Em desenvolvimento

## Hardware

- Arduino Nano
- GPS NEO-6M
- Módulo LoRa (UART)
- BMP280

## Dependências 

- Adafruit BMP280 Library - v3.0.0
- TinyGPSPlus - v1.0.3
- SoftwareSerial (incluído no core)
- Wire (incluído no core)

## Ambiente de desenvolvimento

- Arduino IDE 2.3.7
- Arduino AVR Core
- Placa alvo: Arduino Nano

## Observações

Projeto experimental voltado para testes e aprendizado em sistemas de telemetria.
