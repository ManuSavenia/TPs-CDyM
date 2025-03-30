/*
 * TP1_MS.c
 *
 * Created: 29/03/2025 04:46:28 p. m.
 * Author : Manuel Savenia, Cabral Ramiro
 */
#include <avr/io.h>
#define F_CPU 16000000UL // Frecuencia de oscilador en 8MHz
#include <util/delay.h>
#include <stdint.h>

int main(void){
    /* Setup */
	// bit en 0: entrada, 1: salida
    DDRD = 0xFF;                            // Puerto D como salida
    DDRB &= (1 << PORTB3 | 1 << PORTB4);    // Bit 3 y 4 puerto C como salida
    DDRC &= ~(1 << PORTC0 | 1 << PORTC1);	// Bit 0 y 1 puerto C como entrada
    PORTC |= (1 << PORTC0 | 1 << PORTC1);	// Resistencias Pull-Up en bit 0 y 1 puerto C
    uint8_t state = 1;                      // Variable para almacenar el estado del programa
    uint8_t first = 1;                      // Variable para almacenar el estado de cada secuencia
    uint8_t c0_active = 1;                  // Variable para almacenar el estado del pulsador en C0
	
    /* Loop */
    while (1){

        if (c0_active && (PINC & (1 << PINC0))){
            c0_active = 0;
        }

        // si se presiono el pulsador en C0, cambiamos de estado
        if (!c0_active && !(PINC & (1 << PINC0))){
            state = (state == 3) ? 1 : state + 1;
            first = 1;
            c0_active = 1;
        }

        //pulsador en el pin C1
        if ((PINC & (1 << PINC1))){
            PORTB &= ~(1 << PORTB3 | 1 << PORTB4);
        } else{
            PORTB = (PORTB & ~(1 << PORTB4 | 1 << PORTB3)) | (state << PORTB3);
        }

        switch (state){
            case 1:
                PORTD = first ? 0b10101010 : ~PORTD;
                break;
            case 2:
                PORTD = (first || PORTD & (1 << PORTD7)) ? 0b00000001 : PORTD << 1;
                break;
            case 3:
                PORTD = first ? 0b00000001 : PORTD ^ 0b10000001;
                break;
        }
        _delay_ms(100);

        first = 0; //ya no estamos en la primera iteracion de la secuencia
    }
    /* Punto de finalización del programa (NO se debe llegar a este lugar) */
    return 0;
}