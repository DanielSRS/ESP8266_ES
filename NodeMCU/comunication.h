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
#define GET_DIGITAL_INPUT_VALUE 'D' //0x05
#define GET_ANALOG_INPUT_VALUE 'A' //0x04
#define GET_NODE_MCU_STATUS 'O' //0x03
#define NODE_MCU_ON_LED_BUILTIN 0x06
#define NODE_MCU_OFF_LED_BUILTIN 0x09

// Resonse Command
#define NODE_MCU_STATUS_ERROR 0x1F
#define NODE_MCU_STATUS_OK 0x00
#define ANALOG_LEVEL 0x01
#define DIGITAL_LEVEL 0x02

#define TURN_NODE_LED_ON_OR_OFF 'L' //0x06

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

#endif
