# Módulo de Amplificação e Aquisição de Sinais com AD620

Este projeto consiste em um módulo de condicionamento, amplificação e aquisição de sinais analógicos de baixa amplitude (como ponte de extensômetros, termopares ou sensores biomédicos) utilizando o **Amplificador de Instrumentação AD620**, gerador de fonte simétrica **ICL7660** e um microcontrolador **ATmega328P** (Arduino Uno) com comunicação serial USART.

---

## 📌 Sumário

* [Visão Geral](https://www.google.com/search?q=%23-vis%C3%A3o-geral)
* [Arquitetura de Hardware](https://www.google.com/search?q=%23-arquitetura-de-hardware)
* [Amplificador de Instrumentação (AD620)](https://www.google.com/search?q=%231-amplificador-de-instrumenta%C3%A7%C3%A3o-ad620)
* [Gerador de Tensão Simétrica (ICL7660)](https://www.google.com/search?q=%232-gerador-de-tens%C3%A3o-sim%C3%A9trica-icl7660)
* [Interface de Sinal e Acondicionamento](https://www.google.com/search?q=%233-interface-de-sinal-e-acondicionamento)
* [Processamento e Comunicação (MCU / USB-UART)](https://www.google.com/search?q=%234-processamento-e-comunica%C3%A7%C3%A3o-mcu--usb-uart)


* [Cálculos Teóricos de Ganho e Offset](https://www.google.com/search?q=%23-c%C3%A1lculos-te%C3%B3ricos-de-ganho-e-offset)
* [Firmware](https://www.google.com/search?q=%23-firmware)
* [Estrutura do Código](https://www.google.com/search?q=%23estrutura-do-c%C3%B3digo)
* [Protocolo de Comunicação Serial](https://www.google.com/search?q=%23protocolo-de-comunica%C3%A7%C3%A3o-serial)


* [Lista de Componentes (BOM)](https://www.google.com/search?q=%23-lista-de-componentes-bom)
* [Como Executar o Projeto](https://www.google.com/search?q=%23-como-executar-o-projeto)
* [Autor e Licença](https://www.google.com/search?q=%23-autor-e-licen%C3%A7a)

---

## 🔍 Visão Geral

Sinais analógicos provenientes de sensores diferenciais frequentemente possuem amplitudes muito reduzidas (mV) e necessitam de alta rejeição em modo comum (CMRR), além de condicionamento de tensão antes de serem lidos por um conversor analógico-digital (ADC) de microcontrolador.

O projeto resolve esses desafios combinando:

1. **Ganho Ajustável e Rejeição de Ruído:** Amplificador AD620 configurado com ganho $G \approx 34$.
2. **Alimentação Simétrica ($\pm 5\text{V}$):** O ICL7660 gera um trilho de $-5\text{V}$ a partir da alimentação externa ($+5\text{V}$), permitindo excursão de sinal negativa sem saturação.
3. **Offset Contínuo de Tensão:** Injeção de offset de $\approx 0{,}5\text{V}$ no pino `REF` do AD620 para conformidade com a faixa de leitura do ADC ($0 - 5\text{V}$).
4. **Digitalização e Transmissão:** O conversor ADC interno do ATmega328P amostra o sinal amplificado e o envia periodicamente em formato ASCII via UART (9600 baud rate).

---

## 🛠️ Arquitetura de Hardware

O circuito está dividido em três blocos funcionais principais conforme demonstrado no esquemático elétrico:

### 1. Amplificador de Instrumentação (AD620)

* **CI:** AD620 (U4)
* **Função:** Amplificação diferencial de baixo ruído e alto CMRR.
* **Resistor de Ganho ($R_G$):** $R_{10} = 1{,}5\text{ k}\Omega$.
* **Tensão de Referência (Offset):** Divisor com $R_{13} = 9{,}1\text{ k}\Omega$ e $R_{14} = 1{,}0\text{ k}\Omega$, fornecendo $V_{ref} \approx 0{,}5\text{V}$ ao pino 5.

### 2. Gerador de Tensão Simétrica (ICL7660)

* **CI:** ICL7660 (U3)
* **Função:** Inversor de tensão por capacitores comutados (*switched-capacitor*).
* **Entrada:** $+5\text{V}$ (Alimentação externa).
* **Saída:** $-5\text{V}$ para alimentar o trilho negativo ($V_-$) do AD620.
* **Capacitores de Filtro e Carga:** $C_1, C_5, C_6 = 100\text{ nF}$ e $C_2, C_3, C_4 = 10\mu\text{F}$.

### 3. Interface de Sinal e Acondicionamento

* **Rede de Entrada / Ponte de Teste:** Resistores $R_8 = 10\text{ k}\Omega$, $R_9 = 10\text{ k}\Omega$, $R_{11} = 10\text{ k}\Omega$, $R_{12} = 9{,}1\text{ k}\Omega$ e trimpot $RV_3 = 1\text{ k}\Omega$ para simulação/ajuste do sinal diferencial (`signal+` e `signal-`).

### 4. Processamento e Comunicação (MCU / USB-UART)

* **Microcontrolador:** ATmega328P (placa Arduino Uno R3 - A1).
* **Pino de Leitura ADC:** Canal `A0` (pino 9 da placa).
* **Conversor USB-Serial:** CH340C (U1) para interface com computador via conector USB Type-B (Integrado ao arduino).

---

## 📐 Cálculos Teóricos de Ganho e Offset

### 1. Ganho de Amplificação ($G$)

Pelo datasheet do AD620, o ganho é dado por:


$$G = \frac{49{,}4\text{ k}\Omega}{R_G} + 1$$

Substituindo $R_G = R_{10} = 1{,}5\text{ k}\Omega$:


$$G = \frac{49400}{1500} + 1 = 32{,}93 + 1 \approx 34$$

### 2. Tensão de Offset ($V_{offset}$)

O divisor de tensão ligado ao pino `REF` define o deslocamento de DC de saída:


$$V_{offset} = V_{CC} \times \left( \frac{R_{14}}{R_{13} + R_{14}} \right)$$

$$V_{offset} = 5\text{V} \times \left( \frac{1{,}0\text{ k}\Omega}{9{,}1\text{ k}\Omega + 1{,}0\text{ k}\Omega} \right) = 5 \times \frac{1}{10{,}1} \approx 0{,}495\text{V} \approx 0{,}5\text{V}$$

### 3. Equação Geral da Saída ($V_{output}$)

$$V_{output} = (V_{signal+} - V_{signal-}) \times G + V_{offset}$$

$$V_{output} = (V_{signal+} - V_{signal-}) \times 34 + 0{,}5\text{V}$$

**Exemplo Prático (presente no esquemático):**
Para um sinal diferencial de entrada $\Delta V_{in} = 0{,}1\text{V}$:


$$V_{output} = (0{,}1\text{V}) \times 34 + 0{,}5\text{V} = 3{,}4\text{V} + 0{,}5\text{V} = 3{,}9\text{V}$$

---

## 💻 Firmware

O código-fonte foi desenvolvido em linguagem **C puro** para a arquitetura AVR, sem dependências do framework Arduino (utilizando manipulação de registradores e drivers modulares de USART e ADC).

### Código-Fonte Principal (`main.c`)


### Protocolo de Comunicação Serial

* **Baud Rate:** 9600 bps
* **Data Bits:** 8 bits
* **Paridade:** Nenhuma
* **Stop Bits:** 1
* **Formato do Pacote:** String de 5 caracteres numéricos preenchida com zeros à esquerda, finalizada pelo caractere `0x0A` (`\n`).
* *Exemplo de saída:* `00798\n` (Valor bruto do ADC de 10 bits correspondente a $\approx 3{,}9\text{V}$).

---

## 📦 Lista de Componentes (BOM)

| Componente | Designador | Descrição | Qtd |
| --- | --- | --- | --- |
| **AD620** | U4 | Amplificador de Instrumentação DIP-8 / SOIC-8 | 1 |
| **ICL7660** | U3 | Conversor de Tensão Inversor de Carga Comutada | 1 |
| **CH340C** | U1 | Conversor USB para Serial UART | 1 |
| **ATmega328P / Arduino Uno** | A1 | Microcontrolador de 8-bits AVR | 1 |
| **Resistor 1.5 kΩ** | R10 | Resistor de ajuste de ganho ($R_G$), 1/4W 1% | 1 |
| **Resistor 1.0 kΩ** | R14 | Resistor do divisor de offset, 1/4W 1% | 1 |
| **Resistor 9.1 kΩ** | R12, R13 | Resistores de condicionamento/offset, 1/4W 1% | 2 |
| **Resistor 10 kΩ** | R8, R9, R11 | Resistores de ponte de entrada, 1/4W 1% | 3 |
| **Trimpot 1 kΩ** | RV3 | Potenciômetro multivoltas para ajuste fino | 1 |
| **Capacitor 100 nF** | C1, C5, C6 | Capacitor Cerâmico Desacoplamento / Filtro | 3 |
| **Capacitor Eletrolítico 10 µF** | C2, C3, C4 | Capacitor Eletrolítico da Fonte Simétrica | 3 |
| **Conector USB** | J1 | Conector USB Type-B / Micro USB | 1 |

---

## 🚀 Como Executar o Projeto

### Pré-requisitos

* **Compilador:** `avr-gcc`
* **Hardware:** Protoboard com os componentes listados
* **Software Monitor Serial:** Termite, PuTTY, Serial Plotter, Arduino Serial Monitor ou script Python

### Compilação e Gravação (via Terminal / Makefile)

O projeto pode ser compilado e gravado através da Arduino IDE, makefile, diretamente pelo gravador `avrdude` ou pelo Platform IO.

---

## 👨‍💻 Autor e Licença

**Desenvolvido por:** Gabriel Sirtori Corrêa

**Projeto:** Simple Signal Acquisition with AD620 Instrumentation Amplifier

**Software EDA:** KiCad 10.0.0

Licenciado sob a [MIT License](https://www.google.com/search?q=LICENSE).
