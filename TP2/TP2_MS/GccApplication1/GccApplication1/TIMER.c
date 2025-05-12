#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "mef.h"

void timer0_init()
{
    // Configurar Timer0 para interrupciones periodicas cada 10 ms
    TCCR0A = (1 << WGM01);              // Modo CTC
    TCCR0B = (1 << CS02) | (1 << CS00); // Prescaler 1024
    OCR0A = 155;                        // Para 10 ms con 16MHz y prescaler 1024
    TIMSK0 |= (1 << OCIE0A);            // Habilitar interrupcion de comparacion
    sei();
}

ISR(TIMER0_COMPA_vect)
{
	tick++;
	flag ^= 1;
	if (tick == 100)
	{
		tick = 0;
		seg++;
	}
}