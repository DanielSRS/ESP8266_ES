#include <ArduinoOTA.h>
#include "ota_setup.h"
#include "comunication.h"
#include "io.h"

void setup_io_pins(void);

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

    // Le o valor de um sensor digital
    if (comando == GET_DIGITAL_VALUE) {
      struct IO sensor = find_digital_IO_by_id(endereco);
      if (sensor.id == (uint8_t) -1) {
        Serial.printf("%c%c", 31, 31);
      } else {
        pinMode((uint8_t) sensor.id, INPUT);
        int val = digitalRead((uint8_t) sensor.id);
        Serial.printf("%c%c", '0', val + '0');

        Serial.printf("\nEndereço %i\n", endereco);  
        Serial.printf("\nEndereço %s\n", sensor.name);  
      }
    } 
    
    // Numero de sensores
    else if (comando == GET_NUMBER_OF_SENSORS) {
      char res = NELEMS(digital);
      Serial.printf("%c%c", '!', res + '0');
    }
    
    // Ligar ou desligar o led
    else if (comando == TURN_NODE_LED_ON_OR_OFF) {
      char* res = (char*) "LE";
      if (endereco == '0') {
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.print("LZ");
      } else if (endereco == '1') {
        // acende o led
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);
        Serial.print("LL");
      } else {
        Serial.print(res);
      }
    } 
    
    // Fazer a leitura do sensor analogico
    else if (comando == GET_ANALOGIC_SENSOR_VALUE) {
      int value = analogRead(analogic.id);
      char first_half = value >> 8;
      char second_half = value;
      //Serial.printf("%i", value);
      Serial.printf("%c%c", first_half, second_half);
    } 
    
    // Obtem o endereço do sensor
    // onde o valor do endereço deve estar entre 1 e o valor retornado
    // pela função GET_NUMBER_OF_SENSORS
    else if (comando == GET_SENSOR_ADDRESS) {
      struct IO sensor = find_digital_IO_by_index(endereco - 1);
      if (sensor.id == (uint8_t) -1) {
        Serial.printf("%c%c", 31, 31);
      } else {
        uint8_t val = sensor.id;
        Serial.printf("%c%c", 'S', val);
       
      }
    }

    // Obter o nome do sensor
    else if (comando == GET_SENSOR_NAME) {
      struct IO sensor = find_digital_IO_by_id(endereco);
      if (sensor.id == (uint8_t) -1) {
        Serial.print("ER");
      } else {
        Serial.printf(sensor.name);
      }
    }

    // Obter o estado da placa
    else if (comando == NODEMCU_STATUS) {
      Serial.printf("OK");
    }

    //Erro
    else {
      Serial.printf("EE");
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
