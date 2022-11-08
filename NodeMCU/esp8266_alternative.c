#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
//#define STASSID "INTELBRAS"
//#define STAPSK  "Pbl-Sistemas-Digitais"
#define STASSID "Fortaleza Digital"
#define STAPSK  "marinfinito314"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// Nome do ESP na rede
const char* host = "ESP-10.0.0.109";

// Definições de rede
IPAddress local_IP(10, 0, 0, 109);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 0, 0);


void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  // Configuração do IP fixo no roteador, se não conectado, imprime mensagem de falha
  //if (!WiFi.config(local_IP, gateway, subnet)) {
  //  Serial.println("STA Failed to configure");
  //}

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
   ArduinoOTA.setHostname(host);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Definindo pinos como entrada
  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);
}

int counter = 0;

struct IO {
  char* name;
  uint8_t id;
};

struct IO analogic = { .name = "A0", .id = A0};
struct IO digital[] = {{ .name = "D0", .id = 16},
                        { .name = "D1", .id = 5},
                        { .name = "D2", .id = 4},
                        { .name = "D3", .id = 0},
                        { .name = "D4", .id = 2},
                        { .name = "D5", .id = 14},
                        { .name = "D6", .id = 12},
                        { .name = "D7", .id = 13},
                        { .name = "D8", .id = 15}};

struct IO find_digital_IO_by_id(int id) {
  for (int index = 0; index < 9; index++) {
    struct IO io = digital[index];
    if (io.id == id) {
      return io;
    }
  }
  return { .name = "ER", .id = -1};
}

struct IO find_digital_IO_by_index(int in) {
  for (int index = in; index < 9; index++) {
    struct IO io = digital[index];
    if (in == index) {
      return io;
    }
  }
  return { .name = "ER", .id = -1};
}

/**
 * Tabela de comandos aceitos entre a SBC e o nodeMCU
 */
#define GET_NUMBER_OF_SENSORS '!' //0x15
#define GET_SENSOR_ADDRESS 'S' //0x14
#define GET_SENSOR_NAME 'N' //0x14
#define NODEMCU_STATUS 'O' // 0x03
#define GET_ANALOGIC_SENSOR_VALUE 'A' //0x04
#define GET_DIGITAL_VALUE 'D' //0x05
#define TURN_NODE_LED_ON_OR_OFF 'L' //0x06
#define GET_SENSOR_ID_AT 'P'

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

void loop() {
  // Faz a leitura
  String leitura = Serial.readString();
  // Debug
  //Serial.printf("\nLido: %s\n", Serial.readString());
  if (leitura.length() > 0) {
    int comando = leitura.charAt(0);
    int endereco = leitura.charAt(1);

    // Le o valor de um sensor digital
    if (comando == GET_DIGITAL_VALUE) {
      struct IO sensor = find_digital_IO_by_id(endereco);
      if (sensor.id == (uint8_t) -1) {
        Serial.print("ER");
      } else {
        int val = digitalRead(sensor.id);
        Serial.printf("%c%c", '0', val + '0');        
      }
    } 
    
    // Numero de sensores
    else if (comando == GET_NUMBER_OF_SENSORS) {
      char res = NELEMS(digital);
      Serial.printf("%c", res + '0');
    }
    
    // Ligar ou desligar o led
    else if (comando == TURN_NODE_LED_ON_OR_OFF) {
      char* res = "ER";
      if (endereco == '0') {
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.print("OK");
      } else if (endereco == '1') {
        // acende o led
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);
        Serial.print("OK");
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
        Serial.print("ER");
      } else {
        uint8_t val = sensor.id;
        Serial.printf("%c%c", '0', val);
        //Serial.printf("\n%i\n", val);        
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
      Serial.print("EE");
    }

    Serial.printf("\ncomando: %i\n", comando);
    Serial.printf("endereco: %i\n", endereco);
  }
  // Limpa para que não haja ruido na proxima leitura
  //Serial.readString();

  ArduinoOTA.handle();
}
