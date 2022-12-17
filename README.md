<h1 align="center">Sistema de Sensoriamento Genérico</h1> 

![GitHub Org's stars](https://img.shields.io/github/stars/DanielSRS?style=social)

O problema propõe desenvolver um Sistema de Sensoriamento Genérico para um processador ARMV6L. Esse sistema deve servir para a criação de um ecossistema de internet das coisas (IOT). O sistema será capaz de controlar o acionamento de um conjunto variável de sensores e monitorar seu funcionamento de forma automatizada. Para isso, o sistema conta com dois microcontroladores, uma Raspberry Pi Zero W e uma ESP8266, estes se comunicam através do protocolo UART(Universal Asynchronous Receiver Transmitter). Donde o NodeMCU é responsável por obter e retornar os valores dos sensores, quando requisitados via comunicação serial pela Raspberry.

Seções
=================

<!--ts-->
   * [Diagrama do projeto](#diagrama-do-projeto)
   * [Comunicação UART](#comunicação-uart)
   * [Comunicação entre os dispositivos](#comunicação-entre-os-dispositivos)
   * [Microcontroladores Utilizados](#microcontroladores-utilizados)
      * [Raspberry Pi Zero](#raspberry-pi-zero)
      * [NodeMCU Esp8266](#nodemcu-esp8266)
   * [Tests](#tests)
<!--te-->


Diagrama do projeto
===================
![diagram](/assets/digram_system.png)

Comunicação UART
================
O protocolo de comunicação utilizado é o UART (Universal Asynchronous Receiver/Transmitter ou Receptor/Transmissor Assíncrono Universal), que define um conjunto de regras para a troca de dados seriais entre dois dispositivos. A comunicação utiliza dois fios conectados entre o transmissor e o receptor para transmitir e receber informações entre ambas as direções.

Uma vantagem do UART é que ele é assíncrono, de forma que o transmissor e o receptor não precisam compartilhar um sinal de clock comum, com isso, ambas as extremidades devem transmitir ao mesmo tempo e em velocidade predefinida para poder ter a mesma temporização de bits, essa taxa é denominada taxa de baud. A taxa de baud utilizada no projeto foi 115200. Além de ter a mesma taxa de bauds, ambos os lados de uma conexão UART também têm que usar a mesma estrutura de frames e parâmetros.

A estrutura de frames da UART consiste em um bit inicial, um bit final, um bit de paridade e bits de dados. O bit inicial define o início da comunicação através da mudança de estado do sinal lógico, ele é seguido dos bits de dados, que nessa solução são 8 bits (1 byte), seguido do bit final, que determina a finalização da comunicação. O bit de paridade é enviando antes do bit final, servindo para determinar se existem erros nos dados transmitidos.

Comunicação entre os dispositivos
=================================
A Raspberry é responsável por iniciar a comunicação via serial, enviando comandos para o NodeMCU, este irá enviar os dados de resposta de acordo com o comando de requisição. Os comandos de requisição e resposta estão na imagem abaixo.

![Tabelas de comandos](/assets/Screenshot%20from%202022-11-18%2009-49-08.png)

Os dados de resposta enviados pelo NodeMCU foram exibidos em um display LCD 16x2.

Microcontroladores Utilizados
=============================

Raspberry Pi Zero
-----------------

NodeMCU Esp8266
-----------------



## :hammer: Ferramentas Utilizadas

Para o processo de desenvolvimento do sistema foram utilizadas as seguintes ferramentas:

#### [Visual Studio Code](https://code.visualstudio.com/) 
O Visual Studio Code é um editor de texto conhecido por ser um editor de código aberto muito intuitivo. Além disso, ele também é muito popular por ser multiplataforma e estar disponível para os principais sistemas operacionais, que são Linux, Mac e Windows.

#### [Arduino IDE](https://www.arduino.cc/en/software)
É uma plataforma eletrônica de código aberto baseada em hardware e software fáceis de usar. Nesta plataforma é possível realizar o envio do código desenvolvido na linguagem C, para a NodeMCU esp8266 através da comunicação sem fio usando a biblioteca  Arduino OTA.

#### [GNU Makefile](https://www.gnu.org/software/make/manual/make.html)
O programa GNU make determina quais partes de um grande programa devem ser compiladas ou recompiladas. Além disso, ele executa os programas necessários para fazer essas tarefas. Para orientar o programa make é necessário um arquivo Makefile que dita todas as regras de compilação GNU.

#### [GDB](https://www.onlinegdb.com/)
É o depurador de nível de fonte GNU que é padrão em sistemas linux (e muitos outros unix). O propósito de um depurador como o GDB é permitir ver o que está acontecendo “dentro” de outro programa enquanto ele é executado, ou o que outro programa estava fazendo no momento em que travou. Ele pode ser usado tanto para programas escritos em linguagens de alto nível como C e C++ quanto para programas de código assembly.


## :hammer: Compilar e executar 

#### Compilar código C

```
  gcc arquivo.c
```

#### Executar código

```
  ./arquivo
```
#### Buildar Makefile

```
  cd src/
  make all
```

#### Executar Makefile

```
  cd src/
  make run
```

## :computer: Arquitetura do Computador

- Sistema Operacional: Raspbian

- Arquitetura:  ARMV6L

- Ordem de armazenamento de byte: Little Endian (menor para o MSB)

- CPU: 1

- Memórioa: 512MB de memória LPDDR2 SDRAM;

- VID: ARM

- Modelo: 7


Nome do Modelo: ARM1176


## :pencil: Informações do NodeMCU

- NodeMCU ESP8266 V3 CH340

- Programação via Lua

- 13 pinos GPIO, 10 canais PWM, I2C, SPI, ADC, UART e 1-Wire.

- 5 conexões TCP/IP

## Testes

## Autores

| [<img src="https://avatars.githubusercontent.com/u/38389307?v=4" width=115><br><sub>Alexandre Silva Caribé</sub>](https://github.com/AlexandreCaribe) |  [<img src="https://avatars.githubusercontent.com/u/39845798?v=4" width=115><br><sub>Daniel Santa Rosa Santos</sub>](https://github.com/DanielSRS) |  [<img src="https://avatars.githubusercontent.com/u/88436328?v=4" width=115><br><sub>Joanderson Santos</sub>](https://github.com/Joanderson90) |
| :---: | :---: | :---: |


## Materiais de referência
[Raspberry Pi Documentation](https://www.raspberrypi.com/documentation/computers/raspberry-pi.html#raspberry-pi-zero-w)

[Display LCD HD44780U](https://www.google.com/url?sa=t&source=web&rct=j&url=https://www.sparkfun.com/datasheets/LCD/HD44780.pdf&ved=2ahUKEwjso46tlqn6AhVGL7kGHSe6BMEQFnoECGIQAQ&usg=AOvVaw076YT-P88DM3oFFvTDUv43)

[BCM2835 ARM Peripherals](https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf)

[ESP8266 Arduino Core Documentation](https://readthedocs.org/projects/arduino-esp8266/downloads/pdf/latest/)

[Documentação de Referência da Linguagem Arduino](https://www.arduino.cc/reference/pt/)

[ESP8266WiFi library](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)

[ESP8266mDNS library](https://www.arduino.cc/reference/en/libraries/esp8266_mdns/)

[WifiUdp library](https://www.arduino.cc/reference/en/libraries/wifi/wifiudp/)

[ArduinoOTA](https://www.arduino.cc/reference/en/libraries/arduinoota/)

[SoftwareSerial Library](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwjSluW4ypz7AhU_kZUCHbP9C5kQFnoECAoQAQ&url=https%3A%2F%2Fwww.arduino.cc%2Fen%2FReference%2FsoftwareSerial&usg=AOvVaw2kUbQNvvMDReS_1LIPB82g)

[HardwareSerial Library](https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/HardwareSerial.h)

[Termios Library](https://pubs.opengroup.org/onlinepubs/7908799/xsh/termios.h.html)

