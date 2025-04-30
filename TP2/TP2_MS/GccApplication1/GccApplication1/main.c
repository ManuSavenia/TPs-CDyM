#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include "mef.h"

volatile uint8_t tick = 0,seg = 0;

int main(void)
{
	init_mef();
	timer0_init();
	LCD_Init();
	keypad_init();
	// srand(time(NULL));
	_delay_ms(100);
	uint8_t key;

	while (1){
		if (!KEYPAD_Scan(&key)){
			key = 0xFF;
		}
		update_mef(&seg, key); // Actualiza el estado de la MEF
		_delay_ms(50);
	}
	return 0;
}

ISR(TIMER0_COMPA_vect)
{
	tick++;
	if (tick == 100){
		tick = 0;
		seg++;
	}
}