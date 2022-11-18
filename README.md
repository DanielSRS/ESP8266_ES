<h1 align="center">Sistema de Sensoriamento Genérico</h1> 

![GitHub Org's stars](https://img.shields.io/github/stars/DanielSRS?style=social)

O problema propõe desenvolver um Sistema de Sensoriamento Genérico para um processador ARMV6L. Esse sistema deve servir para a criação de um ecossistema de internet das coisas (IOT). O sistema será capaz de controlar o acionamento de um conjunto váriavel de sensores e monitorar seu funcionamento de forma automatizada.
Para isso, o sistema conta com dois microcontroladores, uma Raspberry Pi Zero W e um NodeMCU, estes se comunicam através do protocolo UART(Universal Asynchronous Receiver Transmitter). Donde o NodeMCU é responsável por obter e retornar os valores dos sensores, quando requisitados via comunicação serial pela Raspberry.

## :bookmark_tabs: Diagrama do projeto
![diagram](https://github.com/DanielSRS/ESP8266_ES/blob/main/digram_system.png)

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

## :heavy_check_mark: Softwares Utilizados

- Linux

- Nano

- Visual Studio Code

- GCC (GNU Compiler Collection)

- Git

- Arduino IDE

## :computer: Arquitetura do Computador

Sistema Operacional: Raspbian

Arquitetura:  ARMV6L

Ordem de armazenamento de byte: Little Endian (menor para o MSB)

CPU: 1

VID: ARM

Modelo: 7

Nome do Modelo: ARM1176


## :pencil: Informações do NodeMCU

NodeMCU V3 CH340

Programação via Lua

11 pinos de I/O e conversor analógico/digital

5 conexões TCP/IP


## Testes

## Autores

| [<img src="https://avatars.githubusercontent.com/u/38389307?v=4" width=115><br><sub>Alexandre Silva Caribé</sub>](https://github.com/AlexandreCaribe) |  [<img src="https://avatars.githubusercontent.com/u/39845798?v=4" width=115><br><sub>Daniel Santa Rosa Santos</sub>](https://github.com/DanielSRS) |  [<img src="https://avatars.githubusercontent.com/u/88436328?v=4" width=115><br><sub>Joanderson Santos</sub>](https://github.com/Joanderson90) |
| :---: | :---: | :---: |
