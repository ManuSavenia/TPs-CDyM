/*
 * TP1_MS.c
 *
 * Created: 29/03/2025 04:46:28 p. m.
 * Author : manus
 */
#include <avr/io.h>      // Registros del microcontrolador
#define F_CPU 16000000UL // Defino la frecuencia de oscilador en 8MHz
#include <util/delay.h>  // Retardos por software
/* Función main */
int main(void)
{
    /* Setup */
    DDRD = 0xFF;                // Configuro Puerto D como salida
    DDRC &= ~(1 << PORTC0);     // Configuro bit0 puerto C como entrada
    PORTC |= (1 << PORTC0);     // Habilito Pull-Up en bit0 puerto C
    int state = 1;              // Variable para almacenar el estado del programa
    int par = 0;                // Variable para almacenar el estado del par/impar
    uint8_t valor = 0b00000001; // Variable para almacenar el valor de 8 bits (caso 2)
    /* Loop */
    while (1)
    {
        switch (state)
        {
        case 1:
            if (!par)
            {
                PORTD = 0b10101010; // Escribo Port D con patrón de bits
                _delay_ms(100);     // Delay de 100 ms
                PORTD = 0x00;       // Escribo Port D con todos 0
                _delay_ms(100);     // Delay de 100 ms
                par = 1;            // Cambio el estado de par/impar
            }
            else
            {
                PORTD = 0b01010101; // Escribo Port D con otro patrón de bits
                _delay_ms(100);     // Delay de 100 ms
                PORTD = 0x00;       // Escribo Port D con todos 0
                _delay_ms(100);     // Delay de 100 ms
                par = 0;            // Cambio el estado de par/impar
            }
            break;
        case 2:
            PORTD = valor;           // Escribo Port D con el valor de 8 bits
            _delay_ms(100);          // Delay de 100 ms
            PORTD = 0x00;            // Escribo Port D con todos 0
            _delay_ms(100);          // Delay de 100 ms
            valor = valor << 1;      // Desplazo el valor a la izquierda
            if (valor == 0b00000000) // Si el valor es 0, reinicio
            {
                valor = 0b00000001; // Reinicio el valor
            }

            break;
        case 3:
            
            break;
        }
        /* Si el botón es presionado, cambio de estado */
        if (!(PINC & (1 << PINC0)))
        {
            state++;
            if (state > 3)
            {
                state = 1; // Reinicio el estado si supera el máximo
            }
        }
    }
    /* Punto de finalización del programa (NO se debe llegar a este lugar) */
    return 0;
}