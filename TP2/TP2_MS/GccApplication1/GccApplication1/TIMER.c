#include <avr/io.h>
#include <avr/interrupt.h>
#include "TIMER.h"
volatile uint8_t tick = 0;

void timer0_init(void)
{
    // Configurar Timer0 para interrupciones periodicas cada 10 ms
    TCCR0A = (1 << WGM01);              // Modo CTC
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
    OCR0A = 249;                        // Para 1 ms con 16MHz y prescaler 64
    TIMSK0 |= (1 << OCIE0A);            // Habilitar interrupcion de comparacion
}

ISR(TIMER0_COMPA_vect)
{
    static uint8_t contador_ticks = 0;
    contador_ticks++;

    if (contador_ticks >= 200)
    {
        // 10 ms * 10 = 2000 ms{
        contador_ticks = 0;
        tick = 1; // Se cumple un "tick" de la MEF cada 2000 ms
        // cada 2 seg
    }
}