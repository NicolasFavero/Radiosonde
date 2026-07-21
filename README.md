# Radiossonda Experimental (v2.0)

## Sobre o projeto

A **versão 2.0** consiste no desenvolvimento de uma plataforma embarcada para uma **radiossonda experimental**, destinada à aquisição e transmissão de dados ambientais durante voos em balões de alta altitude.

O sistema é baseado em um **ESP32-S3 Supermini** e possui uma arquitetura modular, permitindo a utilização de diferentes módulos de rádio através da mesma PCB, tornando a plataforma flexível para diferentes aplicações e faixas de frequência.

Além da telemetria, a placa oferece interfaces para sensores e módulos opcionais, permitindo sua adaptação para diferentes experimentos científicos e educacionais.

---

# Objetivos

Desenvolver uma plataforma compacta para aquisição e transmissão de dados ambientais, integrando:

- Localização por GPS;
- Medição de pressão e altitude;
- Comunicação via rádio;
- Alimentação para diferentes configurações de bateria;
- Arquitetura preparada para futuras expansões.

---

# Funcionalidades

## Comunicação

- Suporte para múltiplas famílias de rádios;
- Comunicação LoRa e FSK;
- Compatibilidade com diferentes módulos sem necessidade de uma nova PCB.

## Sensoriamento

- Interface para módulo GPS;
- Barramento I²C para sensores adicionais;
- Suporte opcional ao conversor MAX31865 para sensores RTD.

## Alimentação

- Alimentação por baterias Li-Ion/LiPo de **1S até 3S**;
- Conversor Buck-Boost integrado;
- Conector dedicado para bateria.

---

# Status do projeto

🚧 **Em desenvolvimento**

O hardware e o firmware encontram-se em constante evolução, podendo receber novos sensores, módulos de rádio e funcionalidades.

---

# Hardware

- ESP32-S3 Supermini;
- Conversor Buck-Boost;
- Conector para bateria;
- Barramento I²C;
- Suporte para módulo GPS;
- Suporte opcional ao MAX31865;
- Até dois conectores SMA;
- PCB dedicada.

---

# Software

- C++;
- PlatformIO;
- Arduino Framework.

---

# Ambiente de Desenvolvimento

- IDE: PlatformIO;
- Framework: Arduino;
- Placa alvo: ESP32-S3.

---

# Estrutura do Projeto

```text
Firmware/
├── include/
├── lib/
├── src/
└── platformio.ini

Hardware/
├── PCB/
└── Documentação/
```

---

# Aplicação

Projeto desenvolvido para fins educacionais e experimentais, servindo como plataforma para estudos de telemetria, sensoriamento ambiental, comunicação por rádio e sistemas embarcados aplicados a radiossondas.