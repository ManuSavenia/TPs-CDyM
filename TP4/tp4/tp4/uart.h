#ifndef _UART_H
#define _UART_H

#include <stdint.h>

#define BUFFER_SIZE 256 
extern volatile uint8_t rx_buffer[BUFFER_SIZE];
extern volatile uint8_t cmd_index;
extern volatile uint8_t rx_flag;

void UART_init(uint8_t baud_rate);
void uart_send_string(const char *str);

#endif