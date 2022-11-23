#ifndef COMUNICATION_H
#define COMUNICATION_H

/**
 * Tabela de comandos aceitos entre a SBC e o nodeMCU
 */
#define GET_NUMBER_OF_SENSORS '!'
#define GET_SENSOR_ADDRESS 'S'
#define GET_SENSOR_NAME 'N'
#define GET_SENSOR_ID_AT 'P'

// Request Command
#define GET_NODE_MCU_STATUS 0x03
#define GET_ANALOG_INPUT_VALUE 0x04
#define GET_DIGITAL_INPUT_VALUE 0x05
#define NODE_MCU_ON_LED_BUILTIN 0x06
#define NODE_MCU_OFF_LED_BUILTIN 0x09

// Resonse Command
#define NODE_MCU_STATUS_ERROR 0x1F
#define NODE_MCU_STATUS_OK 0x00
#define ANALOG_LEVEL 0x01
#define DIGITAL_LEVEL 0x02

int command_to_int(char a, char b);

#endif