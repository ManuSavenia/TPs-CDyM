#ifndef KEYPAD_H
#define KEYPAD_H

#include <avr/io.h>

// Funciones para el teclado matricial
uint8_t KeypadUpdate(void);

// Escanea el teclado y detecta una tecla nueva presionada
uint8_t KEYPAD_Scan(uint8_t *pkey);

// inicializa el teclado
void keypad_init(void);

#endif // KEYPAD_H