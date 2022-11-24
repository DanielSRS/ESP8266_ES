#include <stdio.h>            // printf, 
#include <stdlib.h>           // atoi
#include <unistd.h>           // sleep
#include <string.h>           // strncpy, strtok, strlen, strcmp, 
#include "display.h"          // write_char, init_display, clear_display
#include "sensor.h"           // Sensor, print_sensor_to_console, 
#include "serial.h"           // uart_configure, uart_send_string, serialReadBytes
#include "comunication.h"     // command_to_int


/**
 * @brief Get the Substring object
 *
 * @param dst Destination variable
 * @param src String to get a substring
 * @param start Initial position of the substring
 * @param end Ending position of the substring
 * @return char* Substring
 */
char *getSubstring(char *dst, const char *src, size_t start, size_t end) {
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
        int isDigital = strcmp(getSubstring(str, argv[index], 0, 2), "-d");
        /**
         * Se inserido uma porta digital
         */
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
            //digital[0].name = arr[1];
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
        /*if (strcmp(argv[index], "-l") == 0) {
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
          /*int max_digital = read[1] - '0';
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
            /*if (read[0] == 31) {
              printf("NodeMCU com problema!!!\n\n");
              clear_display();
              write_string("NodeMCU com erro");
              return 0;
            }

            unsigned char sensor_address = read[1]; //    endereço do sensor

            /** Obtendo o nome */
            /*command[0] = 'N';
            command[1] = (char) sensor_address;
            uart_send_string((char*) command);
            sleep(3);
            serialReadBytes(read);

            command[0] = read[0];
            command[1] = read[1];

            // exibe a informação
            printf("%10s: %2i\n", command, sensor_address);
          }
        }*/

    }

    uart_configure(); // configura a uart
    int input = 0;
    char cmd[2] = "OO";  // Envio de comandos
    unsigned char ler[100]; // Leitura de respostas

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
