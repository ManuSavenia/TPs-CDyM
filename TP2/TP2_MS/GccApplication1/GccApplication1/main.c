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


int main(void)
{
	uint8_t key;
	timer0_init();
	LCD_Init();
	keypad_init();
	init_mef();
	_delay_ms(100); // pequeño delay para que todo se inicialice correctamente

	while (1)
	{
		if (flag){
			if (!KEYPAD_Scan(&key)){
				key = 0xFF;
			}
			update_mef(key);
		}
	}

	return 0;
}