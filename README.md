<h1 align="center">Sistema de Sensoriamento Genérico</h1> 

![GitHub Org's stars](https://img.shields.io/github/stars/DanielSRS?style=social)

Foi proposto o desenvolvimento de um Sistema de Sensoriamento Genérico para um processador da arquitetura ARMV6L. O sistema deveria ser capaz de controlar o acionamento de um conjunto variável de sensores e monitorar seu funcionamento de forma automatizada. Para isso, o sistema contou com dois microcontroladores, uma Raspberry Pi Zero W, que desempenhou o papel de SBC (Single Board Computer) e uma NodeMCU ESP8266, estes deveriam comunicar-se através do protocolo UART(Universal Asynchronous Receiver Transmitter). Donde, a NodeMCU deveria ser responsável por obter e retornar os valores dos sensores, quando estes fossem requisitados via comunicação serial pela Raspberry. Além disso, o sistema deveria servir para a criação de um ecossistema de internet das coisas (IOT).

Seções
=================

<!--ts-->
   * [Diagrama do projeto](#diagrama-do-projeto)
   * [Comunicação UART](#comunicação-uart)
   * [Comunicação entre os dispositivos](#comunicação-entre-os-dispositivos)
   * [Microcontroladores Utilizados](#microcontroladores-utilizados)
      * [Raspberry Pi Zero W](#raspberry-pi-zero-w)
      * [NodeMCU Esp8266](#nodemcu-esp8266)
   * [Desenvolvimento do projeto](#desenvolvimento-do-projeto)
      * [Raspberry](#raspberry)
      * [Esp8266](#esp8266)
   * [Ferramentas Utilizadas](#ferramentas-utilizadas)
   * [Rodando o projeto](#rodando-o-projeto)
    
   * [Testes Realizados](#testes-realizados)
      * [Leitura dos sensores digitais](#leitura-dos-sensores-digitais)
      * [Leitura do sensor analógico](#leitura-do-sensor-analógico)
      * [Ligar e desligar o LED built-in da NodeMCU](#ligar-e-desligar-o-led-built-in-da-nodemcu)
      * [Obtenção do status da NodeMCU](#obtenção-do-status-da-nodemcu)
      * [Vídeo dos testes realizados da leitura dos sensores](#vídeo-dos-testes-realizados-da-leitura-dos-sensores)
<!--te-->


Diagrama do projeto
===================
![diagram](/assets/digram_system.png)

Comunicação UART
================
O protocolo de comunicação serial adotado foi o UART (Universal Asynchronous Receiver/Transmitter ou Receptor/Transmissor Assíncrono Universal), que define um conjunto de regras para a troca de dados seriais entre dois dispositivos. A comunicação utiliza dois fios conectados entre o transmissor e o receptor para transmitir e receber informações entre ambas as direções. Uma vantagem do UART é que ele é assíncrono, de forma que o transmissor e o receptor não precisam compartilhar um sinal de clock comum, com isso, ambas as extremidades devem transmitir ao mesmo tempo e em velocidade predefinida para poder ter a mesma temporização de bits, essa taxa é denominada taxa de baud, ou seja, taxa de transmissão de dados. A taxa de baud utilizada no projeto foi 115200. Além de ter a mesma taxa de bauds, ambos os lados de uma conexão UART também têm que usar a mesma estrutura de frames e parâmetros. A estrutura de frames da UART consiste em um bit inicial, um bit final, um bit de paridade e bits de dados. O bit inicial define o início da comunicação através da mudança de estado do sinal lógico, ele é seguido dos bits de dados, que nesta solução são 8 bits (1 byte), seguido do bit final, que determina a finalização da comunicação. O bit de paridade é enviado antes do bit final, servindo para determinar se existem erros nos dados transmitidos.


Comunicação entre os dispositivos
=================================
A Raspberry foi responsável por iniciar a comunicação serial, enviando comandos para a NodeMCU, esta deveria enviar os dados de resposta de acordo com o comando de requisição. Os comandos de requisição e resposta estão na imagem abaixo.

![Tabelas de comandos](/assets/Screenshot%20from%202022-11-18%2009-49-08.png)

Os dados de resposta enviados pelo NodeMCU foram exibidos em um display LCD 16x2.

Microcontroladores Utilizados
=============================

Raspberry Pi Zero W
-----------------
![Raspberry Pi Zero W](/assets/raspberry.jpg)

## :computer: Detalhes da arquitetura

- Sistema Operacional: Raspbian

- Arquitetura:  ARMV6L

- Ordem de armazenamento de byte: Little Endian (menor para o MSB)

- Processador single-core de 1 GHz

- GPIO 40-Pinos

- Memórioa: 512MB de memória LPDDR2 SDRAM;

- VID: ARM

- Modelo: 7

- Nome do Modelo: ARM1176

NodeMCU Esp8266
-----------------
![Raspberry Pi Zero W](/assets/node-mcu-esp8266.jpg)

## :computer: Detalhes da arquitetura

- NodeMCU ESP8266 V3 CH340

- Programação via Lua

- 13 pinos GPIO, 10 canais PWM, I2C, SPI, ADC, UART e 1-Wire.

- 5 conexões TCP/IP


Desenvolvimento do projeto
=============================


Raspberry
---------
O script de desenvolvimento da Rapsberry está localizado no arquivo [main.c](/Raspberry/main.c).
Como dito nas seções anteriores a Rapsberry foi responsável por iniciar a comunicação serial, além de exibir as informações recebidas pela NodeMCU em um display lcd.
Para a utilização do protocolo uart na Rapsberry foi utilizada a biblioteca [Termios (termios.h)](https://pubs.opengroup.org/onlinepubs/7908799/xsh/termios.h.html), esta define a interface de E/S da placa, trabalhando com arquivos do sistema operacional para efetuar a comunicação UART pelos pinos Tx e Rx. 
Portanto, essa biblioteca foi utilizada para configurar, enviar e receber mensagens através do protocolo UART.


## Decisões de projeto
- Velocidade de comuniação serial (Baud Rate) adotada = 115200bps


Esp8266
-------

Ferramentas Utilizadas
======================

Para o processo de desenvolvimento do sistema foram utilizadas as seguintes ferramentas:

#### [Visual Studio Code](https://code.visualstudio.com/) 
O Visual Studio Code é um editor de texto conhecido por ser um editor de código aberto muito intuitivo. Além disso, ele também é muito popular por ser multiplataforma e estar disponível para os principais sistemas operacionais, que são Linux, Mac e Windows.

#### [Arduino IDE](https://www.arduino.cc/en/software)
É uma plataforma eletrônica de código aberto baseada em hardware e software fáceis de usar. Nesta plataforma é possível realizar o envio do código desenvolvido na linguagem C, para a NodeMCU esp8266 através da comunicação sem fio usando a biblioteca  Arduino OTA.

#### [GNU Makefile](https://www.gnu.org/software/make/manual/make.html)
O programa GNU make determina quais partes de um grande programa devem ser compiladas ou recompiladas. Além disso, ele executa os programas necessários para fazer essas tarefas. Para orientar o programa make é necessário um arquivo Makefile que dita todas as regras de compilação GNU.

#### [GDB](https://www.onlinegdb.com/)
É o depurador de nível de fonte GNU que é padrão em sistemas linux (e muitos outros unix). O propósito de um depurador como o GDB é permitir ver o que está acontecendo “dentro” de outro programa enquanto ele é executado, ou o que outro programa estava fazendo no momento em que travou. Ele pode ser usado tanto para programas escritos em linguagens de alto nível como C e C++ quanto para programas de código assembly.

Rodando o Projeto
=================

#### Compilar código C

```
  gcc arquivo.c
```

#### Executar código

```
  ./arquivo
```
#### Processo de build do Makefile

```
  cd src/
  make all
```

#### Executar Makefile

```
  cd src/
  make run
```

Testes Realizados
=================
Para realização dos testes foi utilizado o kit de desenvolvimento na imagem abaixo:
![Kit de desenvolvimento utilizado](/assets/kit_desenvolvimento.jpg)

Leitura dos sensores digitais
-------------------------
Para realização do teste dos sensores digitais foram utilizados dois botões, estes quando pressionados representaram a alteração dos valores digitais dos sensores. 

Leitura do sensor analógico
---------------------------
Para realização do teste do sensor analógico foi utilizado um potenciômetro, este representou a alteração do valor analógico, quando utilizado. Podendo ter um valor de 0 a 1024.

Ligar e desligar o LED built-in da NodeMCU
------------------------------------------
Foi realizado o teste para ligar e desligar o LED built-in da NodeMCU. Teste esse que funcionou como esperado, isto é, o led built-in da NodeMCU  foi aceso e apagado quando requisitado por comando através da Raspberry.

Obtenção do status da NodeMCU
-----------------------------
Foi realizado para obter o status da NodeMCU. Status estes podendo ser OK, ou seja tudo ok com a NodeMCU, ou status de ERRO, ou seja a NodeMCU apresentou algum erro no seu funcionamento.

Vídeo dos testes realizados da leitura dos sensores
===================================================
https://user-images.githubusercontent.com/88436328/208267483-5b4e35fe-75b3-4232-baff-982cb9c1761f.mp4



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

