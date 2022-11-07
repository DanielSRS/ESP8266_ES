
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <gpio.h>
#include <uart.h>

#ifndef STASSID
#define STASSID "INTELBRAS"
#define STAPSK "Pbl-Sistemas-Digitais"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

// Nome do ESP na rede
const char *host = "ESP-10.0.0.109";

// Definições de rede
IPAddress local_IP(10, 0, 0, 109);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 0, 0);

#define BUF_SIZE (2048)
#define BAUDUART0 115200

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

int address_sensor = 0;

char *data = (char *)malloc(BUF_SIZE);

uart_t *uart0;

enum TEST_RESQUEST_RESPONSE
{
    READ_DIGITAL = 'A',
    READ_ANALOG = 'B',
    NODE_STATUS = 'C',
    NODE_ON_LED = 'D',
    NODE_OFF_LED = 'E'
};

void ota_setup()
{

    // Serial.begin(115200);
    Serial.println("Booting");

    // Configuração do IP fixo no roteador, se não conectado, imprime mensagem de falha
    if (!WiFi.config(local_IP, gateway, subnet))
    {
        Serial.println("STA Failed to configure");
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
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

    ArduinoOTA.onStart([]()
                       {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type); });
    ArduinoOTA.onEnd([]()
                     { Serial.println("\nEnd"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
    ArduinoOTA.onError([](ota_error_t error)
                       {
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
    } });
    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void gpioSetup()
{

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, GPIO_IN_ADDRESS);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, GPIO_IN_ADDRESS);
    gpio_init();
}

void setup()
{
    gpioSetup();

    uart0 = uart_init(UART0, BAUDUART0, UART_8N1, 0, 1, 10, 0);

    ota_setup();
}

void loop()
{
    ArduinoOTA.handle();

    delay(1000);

    if (uart_rx_available(uart0) > 0)
    {
        uart_read(uart0, data, 2);

        switch (*data)
        {

        case NODE_MCU_ON_LED_BUILTIN:
            GPIO_OUTPUT_SET(LED_BUILTIN, 0);
            uart_flush(uart0);
            break;

        case NODE_MCU_OFF_LED_BUILTIN:
            GPIO_OUTPUT_SET(LED_BUILTIN, 1);
            uart_flush(uart0);
            break;

        case GET_NODE_MCU_STATUS:
            ets_uart_printf("Node status: ");
            uart_write_char(uart0, NODE_MCU_STATUS_OK);
            uart_flush(uart0);
            break;

        case READ_ANALOG:
            ets_uart_printf("Analog Sensor [1]: ");
            uart_write_char(uart0, ANALOG_LEVEL);
            uart_flush(uart0);
            break;

        case GET_DIGITAL_INPUT_VALUE:
            address_sensor = data[1];

            if (address_sensor == GPIO_PIN_ADDR(16))
            {
                ets_uart_printf("Digital Sensor [1]: ");
                uart_write_char(uart0, DIGITAL_LEVEL + GPIO_INPUT_GET(16));
            }

            else if (address_sensor == GPIO_PIN_ADDR(5))
            {
                ets_uart_printf("Digital Sensor [2]: ");
                uart_write_char(uart0, DIGITAL_LEVEL + GPIO_INPUT_GET(5));
            }

            else
            {
                ets_uart_printf("Invalid sensor address!");
            }

            uart_flush(uart0);

            break;
        }
    }
}