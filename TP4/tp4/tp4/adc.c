#include "adc.h"

static uint8_t aux;		// guardar valor del ADC

void adc_init(void){
	DDRC &= ~(1 << PORTC3); // pin PC3 como entrada
	DIDR0 |= (1 << ADC3D);  // pin ADC3 deshabilitado digitalmente
	ADMUX |= (1 << MUX0) | (1 << MUX1) | (1 << REFS0) | (1 << ADLAR); 	// seteamos canal 3 para que trabaje con vcc como referencia y salida justificada a la izquierda
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN); // prescaler a 128 (ADPS2, ADPS1, ADPS0) y habilita el ADC (ADEN).
	ADCSRA |= (1<<ADSC); 	//iniciar conversion
}

uint8_t adc_read(void){
	while(!(ADCSRA & (1 << ADIF))){}    // espera que termine la conversion

	aux = ADCH;
	ADCSRA |= (1<<ADSC);			    // empezar nueva conversion

	return aux;
}