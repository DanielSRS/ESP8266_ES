#ifndef SERIAL_H
#define SERIAL_H

void uart_configure();
void serialReadBytes(unsigned char l[100]);
int uart_send_string(char* tx_string);

#endif