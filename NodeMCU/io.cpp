/*
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
*/
#include <ArduinoOTA.h> // uint8_t
#include "io.h"

/**
 * @brief Obtem as informações de uma porta digita
 * dado o seu id
 * 
 * @param id endereço da porta
 * @return struct IO Informações da porta como nome.
 */
struct IO find_digital_IO_by_id(int id) {
  for (int index = 0; index < 9; index++) {
    struct IO io = digital[index];
    if (io.id == id) {
      return io;
    }
  }
  return { .name = (char*) "ER", .id = (uint8_t) -1};
}

/**
 * @brief Obtem as informações de uma porta digita
 * dado o seu index, onde index é a ordem da lista
 * digital[]
 * 
 * @param id index da porta
 * @return struct IO Informações da porta como nome.
 * @see digital[]
 */
struct IO find_digital_IO_by_index(int in) {
  for (int index = in; index < 9; index++) {
    struct IO io = digital[index];
    if (in == index) {
      return io;
    }
  }
  return { .name = (char*) "ER", .id = (uint8_t) -1};
}
