#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdint.h>

extern void init_display(void);
extern void write_char(int c);

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

// Address Sensor
#define ADDRESS_DIGITAL_SENSOR_1 0x3C
#define ADDRESS_DIGITAL_SENSOR_2 0x68

#define TRUE 1

int uart0_filestream = -1;

void delay(int sec)
{
    int millis = sec * 1000;
    clock_t start_time = clock();

    while (clock() < start_time + millis)
    {
    }
}

void uart_configure()
{
    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (uart0_filestream == -1)
    {
        printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
    }

    struct termios options;

    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B1152000 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void serial_write_char(const unsigned char c)
{
    write(uart0_filestream, &c, 1);
}

unsigned char serial_read()
{
    uint8_t value;

    if (read(uart0_filestream, &value, 1) != 1)
        return -1;

    return ((int)value) & 0xFF;
}

void serial_read_bytes()
{
    static unsigned char rx_buffer[100];
    char response_command = -1;

    if (uart0_filestream != -1)
    {

        int rx_length = read(uart0_filestream, (void *)rx_buffer, 100); // Filestream, buffer to store in, number of bytes to read (max)

        if (rx_length > 0)
        {
            rx_buffer[rx_length] = '\0';
            printf("Value RX: %s", rx_buffer[0]);

            switch (rx_buffer[0])
            {

            case NODE_MCU_STATUS_OK:
                printf("Status NodeMCU Ok!\n");
                break;

            case NODE_MCU_STATUS_ERROR:
                printf("NodeMCU Error!\n");
                break;

            case DIGITAL_LEVEL:
                printf("Level DC Sensor: %d\n", rx_buffer[1]);
                break;

            case ANALOG_LEVEL:
                printf("Analog Sensor value: %d\n", rx_buffer[1]);
                break;

            default:
                printf("Valeu RX not mapped: %s", rx_buffer[0]);
                break;
            }
        }

        else if (rx_length < 0)
            printf("Error to read data!");

        else
            printf("RX empty!");
    }
}

void main()
{

    uart_configure();
    int input = 0;

    while (TRUE)
    {

        printf("Commands:\n");
        printf("1 = Get status NodeMcu:\n");
        printf("2 = Get value analog sensor:\n");
        printf("3 = Get value digital sensor:\n");
        printf("4 = Turn On led NodeMcu\n");
        printf("5 = Turn Off led do NodeMcu\n");
        printf("6 = Exit\n");
        printf("\n");

        scanf("%d", &input);

        switch (input)
        {
        case 1:
            serial_write_char(GET_NODE_MCU_STATUS);
            printf("Send request to get NodeMCU status.\n");

            break;
        case 2:
            serial_write_char(GET_ANALOG_INPUT_VALUE);
            printf("Send request to get analog sensor value.\n");
            delay(2);
            serial_read_bytes();

            break;
        case 3:
            serial_write_char(GET_DIGITAL_INPUT_VALUE + ADDRESS_DIGITAL_SENSOR_1);
            printf("Send request to get digital sensor 1 value.\n");
            delay(2);
            serial_read_bytes();

            break;
        case 4:
            serial_write_char(NODE_MCU_ON_LED_BUILTIN);
            printf("Send request to turn on NodeMCU led.\n");

            break;
        case 5:
            serial_write_char(NODE_MCU_OFF_LED_BUILTIN);
            printf("Send request to turn off NodeMCU led.\n");

            break;

        case 6:
            exit(0);
            break;
        }
    }
}