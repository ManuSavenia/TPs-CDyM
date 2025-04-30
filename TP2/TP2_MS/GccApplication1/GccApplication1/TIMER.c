#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

void timer0_init()
{
    // Configurar Timer0 para interrupciones periodicas cada 10 ms
    TCCR0A = (1 << WGM01);              // Modo CTC
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
    OCR0A = 249;                        // Para 1 ms con 16MHz y prescaler 64
    TIMSK0 |= (1 << OCIE0A);            // Habilitar interrupcion de comparacion
    sei();
}