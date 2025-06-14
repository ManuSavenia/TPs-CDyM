#ifndef _UART_H
#define _UART_H

#include <stdint.h>

#define CMD_BUFFER_SIZE 128 
extern volatile uint8_t command_buffer[CMD_BUFFER_SIZE];
extern volatile uint8_t cmd_index;
extern volatile uint8_t command_ready;

/**
 * @brief Inicializa el módulo UART para la comunicación serie.
 *
 * Configura los parámetros necesarios para habilitar la transmisión y recepción
 * de datos a través de la interfaz UART.
 */
void UART_init();

#endif