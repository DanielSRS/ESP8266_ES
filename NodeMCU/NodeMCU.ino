#include <ArduinoOTA.h>
#include "ota_setup.h"
#include "comunication.h"
#include "io.h"

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
