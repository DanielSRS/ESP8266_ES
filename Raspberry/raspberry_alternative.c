#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <string.h>

/**
 * Tipo do sensor: Analogico ou digital
 */
enum sensor_type { Analogic, Digital };

/**
 * Obtem o nome do tipo do sensor: Analogico ou digital 
 */
const char* get_sensor_type_name(enum sensor_type type) {
   switch (type) 
   {
      case Analogic: return "Analogico";
      case Digital: return "Digital";
   }
}

/**
 * Informações de um sensor
 */
struct sensor_data {
    char* name;
    int value;
    int id;
    enum sensor_type type;
};

/**
 *  Trasporta uma mensagem com informações de um sensor para uma
 * determinada saída
 */
void message_transporter(struct sensor_data sensor, void (*fn)(struct sensor_data)) {
    (*fn)(sensor);
}

/**
 *  Trasporta um mensagem para a saida padrão
 */
void console_transporter(struct sensor_data sensor_data) {
    printf("%s\n%i\n%s\n", sensor_data.name, sensor_data.value, get_sensor_type_name(sensor_data.type));
}

/**
 * Configuração basica para uso da uart
 */
int setup_uart() {
    //-------------------------
	//----- SETUP USART 0 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
	int uart0_filestream = -1;
	
	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}
	
	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B1152000 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
	
	return uart0_filestream;
}

/**
 * Envia uma string usando a uart
 */
int uart_send_string(int uart0_filestream, char* tx_string)
{
	if (uart0_filestream != -1)
		return write(uart0_filestream, tx_string, strlen(tx_string));		//Filestream, bytes to write, number of bytes to write
	return -1;
}

/**
 * Lê uma string da uart
 */
void read_uart_string(int uart0_filestream) {
    //----- CHECK FOR ANY RX BYTES -----
	if (uart0_filestream != -1)
	{
		// Read up to 255 characters from the port if they are there
		unsigned char rx_buffer[256];
		int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)
		if (rx_length < 0)
		{
			//An error occured (will occur if there are no bytes)
			printf("Não há bytes");
		}
		else if (rx_length == 0)
		{
			//No data waiting
			printf("Não há dados");
		}
		else
		{
			//Bytes received
			rx_buffer[rx_length] = '\0';
			printf("%i bytes read : %s\n", rx_length, rx_buffer);
		}
	}
}


/**
 * Tabela de comandos aceitos entre a SBC e o nodeMCU
 */
#define GET_NUMBER_OF_SENSORS 0x15
#define GET_SENSOR_ADDRESS 0x14
#define NODEMCU_STATUS 0x03
#define GET_ANALOGIC_SENSOR_VALUE 0x04
#define GET_DIGITAL_SENSOR_VALUE 0x05
#define TURN_NODE_LED_ON_OR_OFF 0x06

/**
 * Envia um comando através da uart
 */
void send_commnad(int uart0_filestream, int arg1, int arg2) {
    char command[3] = "\0";
    command[0] = (char) arg1;
    command[0] = (char) arg2;
    uart_send_string(uart0_filestream, &command);
}

/**
 * Obtem o numero de sensores do node mcu
 */
int get_number_of_sensors(int uart0_filestream) {
    send_commnad(uart0_filestream, GET_NUMBER_OF_SENSORS, 0);
}

/**
 * Obtem o endereço de um sensor da node mcu dado o numero do sensor
 * (valor referente ao retorno de get_number_of_sensors())
 * @see{get_number_of_sensors}
 */
int get_sensor_address(int uart0_filestream, int sensor_number) {
    send_commnad(uart0_filestream, GET_SENSOR_ADDRESS, sensor_number);
}

int main() {
    int analogic_sensor_id = -1;
    int digital_sensors_ids[31];
    int uart0_filestream = setup_uart();
    
    if (uart0_filestream == -1) {
        printf("Failed to setup uart\n\n");
    }
    
    struct sensor_data sen;
    sen.name = "Daniel";
    sen.value = 50;
    sen.type = Analogic;
    
    message_transporter(sen, console_transporter);

	//----- CLOSE THE UART -----
	close(uart0_filestream);
    return 0;
}
