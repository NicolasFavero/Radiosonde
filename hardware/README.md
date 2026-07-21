# Hardware

Esta pasta contém os esquemáticos, layouts e demais arquivos relacionados à PCB da **Radiossonda Experimental v2.0**.

A placa foi projetada para oferecer uma plataforma compacta e flexível, permitindo a utilização de diferentes módulos de rádio através da mesma PCB.

---

# Microcontrolador

- ESP32-S3 Supermini

---

# Comunicação por Rádio

A PCB suporta diferentes módulos de rádio.

## Instalação direta

- RF96W;
- RFM95;
- RFM96;
- Outros módulos compatíveis com a mesma pinagem.

## Instalação alternativa

Através da configuração de pads de solda, a PCB também pode utilizar um dos seguintes módulos:

- Módulos baseados no **Semtech LLCC68** (como os modelos da NiceRF);
- **EBYTE E07-M1101D** (formato ESP-01);
- RFM69W.

Essa abordagem permite reutilizar a mesma PCB para diferentes tecnologias e bandas de operação.

---

# Alimentação

A placa possui:

- Conversor Buck-Boost integrado;
- Alimentação por baterias Li-Ion/LiPo de **1S até 3S**;
- Conector dedicado para bateria.

---

# Sensores e Expansões

A PCB disponibiliza:

- Barramento I²C para conexão de sensores, como o BMP280;
- Suporte para módulo GPS;
- Suporte opcional ao conversor **MAX31865** para sensores RTD;
- Conector I²C para expansão de outros periféricos.

Os módulos GPS e MAX31865 podem ser montados na região lateral da placa, permanecendo parcialmente externos ao contorno principal da PCB para otimizar o aproveitamento de espaço.

---

# Antenas

A placa possui suporte para até **2 conectores SMA**, permitindo diferentes configurações de antenas conforme a aplicação.

---

# Estrutura

```text
Hardware/
└── PCB/
```