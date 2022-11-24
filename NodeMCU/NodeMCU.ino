#include <ArduinoOTA.h>
#include "ota_setup.h"
#include "comunication.h"
#include "io.h"

void setup_io_pins(void);
String read_analog_input(void);
int get_digital_io_address(int index);

void setup() {
  ota_setup();        // Configuração de atualização via OTA
  setup_io_pins();    // Define pinos da GPIO como entrada e acende o led
}

void loop() {
  // Faz a leitura
  String leitura = Serial.readString();

  if (leitura.length() > 0) {
    int comando = leitura.charAt(0);
    int endereco = leitura.charAt(1);

    switch (comando) {
      case GET_DIGITAL_INPUT_VALUE: // Le o valor de um sensor digital
        {
          int res = read_digital_input(endereco);
          res != -1
            ? send_response('0', res + '0') // retorna valor com sucesso
            : send_error(31);                // retorna erro de port inexistente
        }
        break;

      case GET_NUMBER_OF_SENSORS: // obtem Numero de sensores
        send_response('!', NELEMS(digital) + '0');
        break;

      case NODE_MCU_ON_LED_BUILTIN: // Ligar o led
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);
        send_response(NODE_MCU_ON_LED_BUILTIN, '1'); // led ligado com sucesso
        break;

      case NODE_MCU_OFF_LED_BUILTIN: //  desligar o led
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        send_response(NODE_MCU_OFF_LED_BUILTIN, '1');
        break;

      case GET_ANALOG_INPUT_VALUE: // Fazer a leitura do sensor analogico
        {
          String res = read_analog_input();
          send_response(res.charAt(0), res.charAt(1));
        }
        break;

      // Obtem o endereço do sensor
      // onde o valor do endereço deve estar entre 1 e o valor retornado
      // pela função GET_NUMBER_OF_SENSORS
      case GET_SENSOR_ADDRESS: 
        {
          int res = get_digital_io_address(endereco);
          res != -1
            ? send_response('S', res) // resposta
            : send_error('S'); // erro
        }
        break;

      case GET_SENSOR_NAME: // Obter o nome do sensor
        {
          char *name = get_digital_io_name(endereco);
          name != NULL
            ? send_response(name[0], name[1])
            : send_error('N');
        }
        break;

      case GET_NODE_MCU_STATUS: // Obter o status da placa
        send_response(NODE_MCU_STATUS_OK, NODE_MCU_STATUS_OK);
        break;
      
      // Comando não reconhecido
      default:
        send_error(NODE_MCU_STATUS_OK);
        break;
    }
  }

  handle_ota();
}

/**
 * Define os pinos da GPIO (onde podem ser conectados sensores digitais)
 * como modo de entrada e liga o led
 */
void setup_io_pins() {
  pinMode(16, INPUT);
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  pinMode(0, INPUT);
  pinMode(2, INPUT);
  pinMode(14, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(15, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
 }


/**
 * @brief Lê o valor de uma entrada digital, dado o
 * endereço da entrada digita.
 * 
 * @param io_address id da entrada digital
 * @return int Se sucesso, retorna 0 ou 1, dependendo do valor liso.
 * Em caso de erro. retorna -1
 */
int read_digital_input(unsigned char io_address) {
  struct IO sensor = find_digital_IO_by_id(io_address);
  if (sensor.id == (unsigned char) -1) {
    return -1; // nao existe sensor com o id informado
  } else {
    pinMode((uint8_t) sensor.id, INPUT);
    return digitalRead((unsigned char) sensor.id); 
  }
}

/**
 * @brief Lê o valor da entrada analogica
 * 
 * @return String valor lido codificado em string
 */
String read_analog_input(void){
  int value = analogRead(analogic.id);
  char first_half = value >> 8;
  char second_half = value;
  //Serial.printf("%c%c", first_half, second_half);
  String res = "00";
  res.setCharAt(0, first_half);
  res.setCharAt(1, second_half);
  return res;
}

/**
 * @brief Obtem o id/endereço de uma das entradas digitais
 * dado seu index. (valor detro do range retornado por GET_NUMBER_OF_SENSORS comand)
 * 
 * @param index index da entrada de acordo com o valor de digitais
 * @return unsigned char io address/id
 */
int get_digital_io_address(int index) {
  struct IO sensor = find_digital_IO_by_index(index - 1);
  if (sensor.id == (unsigned char) -1) {
    return -1;
  } else {
    return sensor.id;
  }
}

/**
 * @brief Envia resposta atravésda UART
 * 
 * @param code codigo de resposta
 * @param value valor da resposta
 */
void send_response(unsigned char code, unsigned char value) {
  Serial.write(code);
  Serial.write(value);
}

/**
 * @brief Envia resposta de erro
 * 
 * @param specific_error_code codigo de erro específico ao 
 * procedimento que gerou o erro
 */
void send_error(unsigned char specific_error_code) {
  send_response(NODE_MCU_STATUS_ERROR, specific_error_code);
}

/**
 * @brief Get the digital io name
 * 
 * @param io_id id/endereço da entrada digital
 * @return char* nome da entrada digital
 */
char* get_digital_io_name(unsigned char io_id) {
  struct IO sensor = find_digital_IO_by_id(io_id);
  if (sensor.id == (unsigned char) -1) {
    return 0;
  } else {
    return sensor.name;
  }
}
