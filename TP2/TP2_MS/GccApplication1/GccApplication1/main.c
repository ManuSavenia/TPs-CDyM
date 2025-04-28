#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <stdint.h>

volatile uint8_t tick = 0; // Flag de "tick" de la MEF

void timer0_init(void)
{
	// Configurar Timer0 para interrupciones periódicas cada 10 ms
	TCCR0A = (1 << WGM01);              // Modo CTC
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
	OCR0A = 249;                        // Para 1 ms con 16MHz y prescaler 64
	TIMSK0 |= (1 << OCIE0A);            // Habilitar interrupción de comparación
}

ISR(TIMER0_COMPA_vect)
{
	static uint8_t contador_ticks = 0;
	contador_ticks++;

	if (contador_ticks >= 200) // 10 ms * 10 = 100 ms
	{
		contador_ticks = 0;
		tick = 1; // Se cumple un "tick" de la MEF cada 100 ms
	}
}

uint8_t KeypadUpdate(void)
{
	uint8_t fila, col;

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

		// Escaneo las columnas
		if (!(PIND & (1 << PD3)))
		return (fila * 4 + 0); // Columna 0
		if (!(PIND & (1 << PD5)))
		return (fila * 4 + 1); // Columna 1
		if (!(PIND & (1 << PD4)))
		return (fila * 4 + 2); // Columna 2
		if (!(PIND & (1 << PD2)))
		return (fila * 4 + 3); // Columna 3

		// Vuelvo a poner la fila en alto antes de la siguiente iteración
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

	return 0xFF; // No se presionó tecla
}

// Escanea el teclado y detecta una tecla nueva presionada
uint8_t KEYPAD_Scan(uint8_t *pkey)
{
	static uint8_t Old_key = 0xFF, Last_valid_key = 0xFF;
	uint8_t Key;

	Key = KeypadUpdate(); // Esta función la tenés que implementar o conectar

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
	PORTD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);
}

const char KeyMap[16] = {
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
'*', '0', '#', 'D'};

// Estados de la MEF
typedef enum
{
	INICIO,
	JUEGO,
	PERDISTE,
	GANASTE
} estado_t;

estado_t estado_actual = INICIO;
uint8_t contador_ticks_estado = 0; // Cuenta ticks de 2 segundos
uint8_t mostrar_palabra = 1;
uint8_t palabra_visible = 1;
uint8_t i; // palabra aleatoria
int vidas = 3;
const char *diccionario[] = {
	"Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro", "Lanza", "Nieve",
	"PeRro", "PecES", "PiAno", "PrYKe", "RUEDa", "SERIE", "SalUd", "Salud", "Silla", "Tecla",
"Valor", "Verde", "YnHRz", "hARdD", "silla"};

int main(void)
{
	timer0_init();
	sei();
	LCD_Init();
	_delay_ms(200);
	keypad_init();

	uint8_t key;

	while (1)
	{
		if (tick)
		{
			tick = 0; // Reiniciar flag
			contador_ticks_estado++;

			switch (estado_actual)
			{
				case INICIO:
				if (contador_ticks_estado == 1)
				{
					LCDclr();
					_delay_ms(2);
					LCDstring((uint8_t *)"Bienvenido!!", 12);
					LCDGotoXY(0, 1);
					LCDstring((uint8_t *)"Presione *", 10);
				}

				if (KEYPAD_Scan(&key))
				{
					if (key < 16 && KeyMap[key] == '*')
					{
						estado_actual = JUEGO;
						contador_ticks_estado = 0;
					}
				}

				if (contador_ticks_estado >= 20) // 2 segundos (20 ticks de 100ms)
				{
					contador_ticks_estado = 0; // reiniciar ventana
				}
				break;

				case JUEGO:
				if (contador_ticks_estado == 1 && mostrar_palabra)
				{
					LCDclr();
					_delay_ms(2);
					i = rand() % 25;                         // Elegir palabra
					LCDstring((uint8_t *)diccionario[i], 5); // Mostrarla
				}

				if (contador_ticks_estado >= 20 && palabra_visible) // Después de 2 segundos
				{
					LCDclr(); // Borrar palabra
					palabra_visible = 0;
					contador_ticks_estado = 0;
				}

				if (!palabra_visible) // Si ya no está visible
				{
					if (KEYPAD_Scan(&key))
					{
						if (key < 16 && KeyMap[key] == '*') // Simular respuesta correcta
						{
							estado_actual = GANASTE;
							contador_ticks_estado = 0;
						}
						else
						{
							vidas--;
							if (vidas == 0)
							{
								estado_actual = PERDISTE;
								contador_ticks_estado = 0;
							}
						}
					}
				}
				break;

				case PERDISTE:
				if (contador_ticks_estado == 1)
				{
					LCDclr();
					_delay_ms(2);
					LCDstring((uint8_t *)"Perdiste", 8);
				}
				if (contador_ticks_estado >= 20)
				{
					estado_actual = INICIO;
					contador_ticks_estado = 0;
				}
				break;

				case GANASTE:
				if (contador_ticks_estado == 1)
				{
					LCDclr();
					_delay_ms(2);
					LCDstring((uint8_t *)"Victoria", 8);
				}
				if (contador_ticks_estado >= 20)
				{
					estado_actual = INICIO;
					contador_ticks_estado = 0;
				}
				break;
			}
		}
	}
}
