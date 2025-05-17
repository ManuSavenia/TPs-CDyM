#include <avr/io.h>
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "keypad.h"

const char KeyMap[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'};

uint8_t KeypadUpdate(void)
{
    uint8_t fila;

    // Primero, ponemos todas las filas en alto (salida '1')
    PORTB |= (1 << PB0) | (1 << PB3) | (1 << PB4);
    PORTD |= (1 << PD7);

    for (fila = 0; fila < 4; fila++)
    {
        // Apago la fila actual (salida '0')
        switch (fila)
        {
        case 0:
            PORTB &= ~(1 << PB4);
            break;
        case 1:
            PORTB &= ~(1 << PB3);
            break;
        case 2:
            PORTB &= ~(1 << PB0);
            break;
        case 3:
            PORTD &= ~(1 << PD7);
            break;
        }
		_delay_us(10); // Espera un poco para estabilizar la señal
        // Escaneo las columnas
        if (!(PIND & (1 << PD3)))
            return (fila * 4 + 0); // Columna 0
        if (!(PIND & (1 << PD5)))
            return (fila * 4 + 1); // Columna 1
        if (!(PIND & (1 << PD4)))
            return (fila * 4 + 2); // Columna 2
        if (!(PIND & (1 << PD2)))
            return (fila * 4 + 3); // Columna 3

        // Vuelvo a poner la fila en alto antes de la siguiente iteracion
        switch (fila)
        {
        case 0:
            PORTB |= (1 << PB4);
            break;
        case 1:
            PORTB |= (1 << PB3);
            break;
        case 2:
            PORTB |= (1 << PB0);
            break;
        case 3:
            PORTD |= (1 << PD7);
            break;
        }
    }
    return 0xFF; // No se presiono tecla
}

uint8_t KEYPAD_Scan(uint8_t *pkey)
{
    static uint8_t Old_key = 0xFF, Last_valid_key = 0xFF;
    uint8_t Key;

    Key = KeyMap[KeypadUpdate()]; // Esta funcion la tenos que implementar o conectar

    if (Key == 0xFF)
    {
        Old_key = 0xFF;
        Last_valid_key = 0xFF;
        return 0;
    }

    if (Key == Old_key)
    {
        if (Key != Last_valid_key)
        {
            *pkey = Key;
            Last_valid_key = Key;
            return 1;
        }
    }

    Old_key = Key;
    return 0;
}

void keypad_init(void)
{
    // Filas como salidas
    DDRB |= (1 << PB0) | (1 << PB3) | (1 << PB4);
    DDRD |= (1 << PD7);

    // Columnas como entradas con pull-up
    DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5));
    PORTB |= (1 << PB0) | (1 << PB3) | (1 << PB4);
    PORTD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);
}