#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "display.h"

enum sensor_type { Undefined, Analogic, Digital };

/**
 * @brief Informações sobre um sensor
 *
 */
struct sensor_data {
    char name[17];
    //char default_name[2];
    //char description[255];
    int value;
    int id;
    enum sensor_type type;
};

typedef struct sensor_data Sensor;

/**
 * @brief Tabela de comandos
 *
 */
#define GET_NODE_MCU_STATUS 0x03
#define GET_ANALOG_INPUT_VALUE 0x04
#define GET_DIGITAL_INPUT_VALUE 0x05
#define NODE_MCU_ON_LED_BUILTIN 0x06
#define NODE_MCU_OFF_LED_BUILTIN 0x09

// Address Sensor
#define ADDRESS_DIGITAL_SENSOR_1 60
#define ADDRESS_DIGITAL_SENSOR_2 104

#define TRUE 1

int uart0_filestream = -1;

/**
 * @brief Realiza a configuração da uart
 *
 */
void uart_configure()
{
  // Abre o arquivo da uart
  uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (uart0_filestream == -1)
    {
        printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
    }

    struct termios options;

    // opções de configuração da
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

/**
 * @brief Faz a leitura do buffer com as informações lidas pela uart
 *
 * @param l Variável de destino da string
 */
void serialReadBytes(unsigned char l[100])
{
    static unsigned char rx_buffer[100];
    char response_command = -1;

    if (uart0_filestream != -1)
    {

        int rx_length = read(uart0_filestream, (void *)rx_buffer, 100); // Filestream, buffer to store in, number of bytes to read (max)

        if (rx_length > 0)
        {
            rx_buffer[rx_length] = '\0';
            //printf("%s", rx_buffer);
            for (int i = 0; i < 100; i++) {
                l[i] = rx_buffer[i];
            }
        }
    }
}

/**
 * @brief Envia string de forma seria pela uart
 *
 * @param tx_string String a ser enviada
 * @return int Valor de sucesso do envio. retorna -1 se erro
 */
int uart_send_string(char* tx_string) {
  if (uart0_filestream != -1)
          return write(uart0_filestream, tx_string, strlen(tx_string));           //Filestream, bytes to write, number of bytes to write
  return -1;
}

/**
 * @brief Dado os dois bytes da resposta de um comando, retorna um numero inteiro.
 * Como todos os comandos (enviados, ou recebidos) tem dois bytes, não é possível
 * ler valores maiores que 255 (base 10).
 *
 * Como o valor de leitura do sensor analógico pode retornar valores até 1024, a
 * node MCU quebra esse valor em doys bytes no comando de resposta, e esse valor
 * é remontado por essa função;
 *
 * @param a primeiro byte de resposta
 * @param b segundo byte de resposta
 * @return int (um numero inteiro com tamanho de dois bytes)
 */
int command_to_int(char a, char b) {
    int val = a;
    val = val << 8;
    return val | b;
}

/**
 * @brief Get the Substring object
 *
 * @param dst Destination variable
 * @param src String to get a substring
 * @param start Initial position of the substring
 * @param end Ending position of the substring
 * @return char* Substring
 */
char *getSubstring(char *dst, const char *src, size_t start, size_t end)
{
    return strncpy(dst, src + start, end);
}

/**
 * @brief Separa um argumento lido em 3 substrings
 *
 * @param str string lida
 * @param a priemira substring de tamanho 10
 * @param b segunda substring de tamanho 10
 * @param c terceira substring de tamanho 10
 */
void separate_string_in_3(char *str, char a[10], char b[10], char c[10]) {
    char *ptr = strtok(str, ".");
    int index = 0;

        while(ptr != NULL && index < 3)
        {
                //printf("%s\n", ptr);
                //res[i] = ptr;
                //strcpy(res[index], ptr);
                if (index == 0) {
                    for (int i = 0; i < strlen(ptr) && i < 10; i++) {
                    //printf("%c", ptr[i]);
                    a[i] = ptr[i];
                }
                a[10] = '\0';
                }
                if (index == 1) {
                    for (int i = 0; i < strlen(ptr) && i < 10; i++) {
                    //printf("%c", ptr[i]);
                    b[i] = ptr[i];
                }
                b[10] = '\0';
                }
                if (index == 2) {
                    for (int i = 0; i < strlen(ptr) && i < 10; i++) {
                    //printf("%c", ptr[i]);
                    c[i] = ptr[i];
                }
                c[10] = '\0';

                }
                //printf("\n");
                ptr = strtok(NULL, ".");
                index++;
        }
}

/**
 * @brief Escreve uma string no display
 *
 * @param s string a ser escrita no display
 */
void write_string(char* s) {
  for (int i = 0; i < strlen(s) && i < 16; i++) {
    write_char(s[i]);
  }
}

void print_sensor_to_console(char *sensor_name, int sensor_value) {
  char str[16];
  sprintf(str, "%9s: %4i", sensor_name, sensor_value);
  clear_display();
  //str[17] = '\0';
  write_string((char*) str);
  printf("%9s: %4i\n", sensor_name, sensor_value);
}

/**
 * @brief Realiza a leitura de sensores de forma automatizada atraves de comunicação serial
 * da raspberry pi
 *
 * @param argc Numero de argumentos da linha de comando recebidos
 * @param argv Lista de argumentos recebidos
 * @return int Retorna 0 se executado com sucesso. Qualquer outro valor em caso de execução incorreta
 */
int main(int argc, char *argv[]) {
    // Configuração inicial
    init_display(); // configura o display
    init_display();
    clear_display(); // Limpa o conteudo do diaplay
    write_string("Iniciando");

    uart_configure(); // configura a uart
    int input = 0;
    char cmd[2] = "OO";  // Envio de comandos
    unsigned char ler[100]; // Leitura de respostas

    Sensor analogico;
    Sensor digital[31];
    /**
     * @brief Quantidade de sensores digitais selecionados
     * 
     */
    int digitalQtd = 0;
    /**
     * Se não tiver argumentos, encerra o programa
     */
    if (argc < 2) {
        printf("Uso inválido. Não há argumentos\n");
        return 0;
    }

    /**
     * Percorre a lista de argumentos para definir os sensores
     */
    for (int index = 1; index < argc; index++) {
        /**
         * @brief Se adicionado um sensor analogico
         */
        if (strcmp(argv[index], "-analogic") == 0) {
          printf("Há um sensor na porta analogica\n");
          char *j = "Analogico";
          for (int i = 0; i < strlen(j) && i < 10; i++) {
            //printf("%c", ptr[i]);
            analogico.name[i] = j[i];
          }
          analogico.name[10] = '\0';
          analogico.value = 0;
          analogico.id = 'A';
          analogico.type = Analogic;
        }
        char str[2];

        /**
         * Se inserido uma porta digital
         */
        int isDigital = strcmp(getSubstring(str, argv[index], 0, 2), "-d");
        if (isDigital == 0) {
            //char name[10];
            if (digitalQtd >= 31) {
                printf("Atingido maximo de sensores digitais!\n");
                continue;
            }
            printf("Há um sensor na porta digital\n");
            char arr[3][11] = {"", "", ""};
            separate_string_in_3(argv[index], arr[0], arr[1], arr[2]);

            /**
             * @brief Os parametros para adiconar um sensor digital não
             * podem ser vazios. O nome e o endereço são obrigatórios
             */
            if (strlen(arr[0]) == 0 || strlen(arr[1]) == 0 || strlen(arr[2]) == 0) {
                printf("\nComando ilegal!!!\n\n");
                return 0;
            }

            /**
             * O endereço do sensor nunca é zero
             */
            int address = atoi(arr[2]);
            if (address == 0) {
                printf("Endereço invalido para o sensor!!!\n");
                return 0;
            }

            /**
             * @brief Salvando o nome do sensor
             */
            for (int i = 0; i < strlen(arr[1]) && i < 10; i++) {
              //printf("%c", ptr[i]);
              digital[digitalQtd].name[i] = arr[1][i];
            }
            int end = 10;
            if (strlen(arr[1]) < 10)
              end = strlen(arr[1]);
            digital[digitalQtd].name[end] = '\0';
            
            /**
             * @brief Definido as outras propriedades do sensor
             * 
             */
            digital[digitalQtd].value = 0;
            digital[digitalQtd].id = atoi(arr[2]);
            digital[digitalQtd].type = Digital;


            //printf("cmd: %s\n", digital[0].name);
            printf("Nome: %s\n", digital[digitalQtd].name);
            printf("Endereço: %i\n", digital[digitalQtd].id);

            /**
             * Limpando substrings
             */
            for (int i = 0; i < 11; i++) {
                    arr[0][i] = '\0';
                    arr[1][i] = '\0';
                    arr[2][i] = '\0';
                }

                //printf("nome %s", arr[1]);
            digitalQtd++;
        }

        /**
         * @brief Se listando os sensores disponíveis
         */
        if (strcmp(argv[index], "-l") == 0) {
          char command[2] = "OO";  // Envio de comandos
          unsigned char read[100]; // Leitura de respostas
          printf("\nPegando o numero de portas digitais disponíveis: \n");
          uart_send_string("!!");
          sleep(3);
          serialReadBytes(read);
          if (read[0] != '!' || strlen(read) < 2) {
            printf("Erro na leitura do numero de portas disponíveis\n");
            return 0;
          }
          /** Numero de portas digitais disponíveis */
          int max_digital = read[1] - '0';
          printf("Portas digitais disponíveis: %i\n\n", max_digital);
          printf("%10s: %8s\n", "Nome", "Endereço\n");
          for (int p = 1; p <= max_digital; p++) {
            command[0] = 'S';           // Obetem o endereço do sensor
            command[1] = (char) p;  // na posição n°
            uart_send_string( (char*) command);
            printf("Obter endereço comando: %s\n", (char*) command);
            sleep(3);
            serialReadBytes(read);

            /** Se problema com a node, encerra a leitura*/
            if (read[0] == 31) {
              printf("NodeMCU com problema!!!\n\n");
              clear_display();
              write_string("NodeMCU com erro");
              return 0;
            }

            unsigned char sensor_address = read[1]; //    endereço do sensor

            /** Obtendo o nome */
            command[0] = 'N';
            command[1] = (char) sensor_address;
            uart_send_string((char*) command);
            sleep(3);
            serialReadBytes(read);

            command[0] = read[0];
            command[1] = read[1];

            // exibe a informação
            printf("%10s: %2i\n", command, sensor_address);
          }
        }

    }

    printf("Iniciando leitura\n\n");

    while(1) {
      /**
       * Lê o sensor analogico
       */
      if (analogico.type == Analogic) { // Se existe um sensor analogico
        printf("\nPegando o valor analogico: \n");
        uart_send_string("AA");
        sleep(3);
        serialReadBytes(ler); // lê resposta
        analogico.value = command_to_int(ler[0], ler[1]);
        print_sensor_to_console(analogico.name, analogico.value);
      }

      /**
       * Lê o sensor analogico
       */
      for (int i = 0; i < digitalQtd; i++) {
        cmd[0] = 'D';
        cmd[1] = (char) digital[i].id;
        uart_send_string((char*) cmd);
        sleep(3);
        serialReadBytes(ler); // lê resposta

        if (ler[0] == 31) {
          printf("NodeMCU com problema. Endereço do sensor é inválido!\n");
          clear_display();
          write_string("NodeMCU com erro");
        } else {
          digital[i].value = atoi(ler);
          print_sensor_to_console(digital[i].name, digital[i].value);
        }
      }
    }
}
