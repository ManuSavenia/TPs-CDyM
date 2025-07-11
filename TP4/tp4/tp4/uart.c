#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD_RATE_CONFIG 103

volatile char tx_buffer[BUFFER_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;
volatile uint8_t tx_busy = 0;

char buffer[BUFFER_SIZE];

void UART_init(uint8_t baud_rate){
    UCSR0B = 0;
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
    UBRR0L = baud_rate;
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Enable RX, TX, RX interrupt
}

ISR(USART_RX_vect){
    uint8_t received_byte = UDR0;
    if ((received_byte == '\b' || received_byte == 0x7F) && cmd_index > 0){
        cmd_index--;
    }
    else if (received_byte == '\r' || received_byte == '\n'){
        rx_buffer[cmd_index] = '\0';
        cmd_index = 0;
        rx_flag = 1;
    }
    else if (cmd_index < BUFFER_SIZE - 1){
        rx_buffer[cmd_index++] = received_byte;
    }
}

void uart_send_char(uint8_t ch){
    uint8_t next_head = (tx_head + 1) % BUFFER_SIZE;

    while (next_head == tx_tail)
        ;

    tx_buffer[tx_head] = ch;
    tx_head = next_head;

    if (!tx_busy){
        tx_busy = 1;
        UDR0 = tx_buffer[tx_tail];
        tx_tail = (tx_tail + 1) % BUFFER_SIZE;
        UCSR0B |= (1 << TXCIE0);
    }
}

void uart_send_string(const char *str){
    while (*str) {
        uart_send_char(*str++);
    }
}

ISR(USART_TX_vect){
    if (tx_tail == tx_head){
        tx_busy = 0;
        UCSR0B &= ~(1 << TXCIE0);
        return;
    }

    UDR0 = tx_buffer[tx_tail];
    tx_tail = (tx_tail + 1) % BUFFER_SIZE;
}