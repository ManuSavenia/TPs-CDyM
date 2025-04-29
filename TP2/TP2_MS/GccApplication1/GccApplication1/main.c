#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <stdint.h>
#include "KEYpad.h"
#include "TIMER.h"

#define WORDS_SIZE 25
#define PASSWORD_LENGTH 5
#define MAX_ERRORS 3
#define TECLA_INICIO '*'
#define TECLA_CONFIRMAR '#'

const char KeyMap[16] = {
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
	'*', '0', '#', 'D'};

// estados de la MEF
typedef enum
{
	INICIO,
	INICIO_JUEGO,
	JUEGO,
	PERDISTE,
	GANASTE
} estado_t;

// obtener una palabra random
char *get_random_word(char *words[], uint8_t size)
{
	uint8_t pos = rand() % size;
	return words[pos];
}

estado_t estado_actual = INICIO;
uint8_t contador_ticks_estado = 0; // Cuenta ticks de 2 segundos
uint8_t mostrar_palabra = 1;
uint8_t pos_password = 0;	  // indice actual de la contrasenia a ingresar
uint8_t numero_ingresado = 0; // numero ingresado por el usuario
uint8_t vidas = 3;
char caracter;
char *curr_word; // palabra a adivinar
char *curr_word; // palabra a adivinar


const char *diccionario[] = {
	"Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro", "Lanza", "Nieve",
	"PeRro", "PecES", "PiAno", "PrYKe", "RUEDa", "SERIE", "SalUd", "Salud", "Silla", "Tecla",
	"Valor", "Verde", "YnHRz", "hARdD", "silla"};

uint8_t es_letra(char input)
{
	return (input > "1" && input < "9");
}

void update_input_number(uint8_t *numero_ingresado, uint8_t key)
{
	if (key < 16)
	{
		char letra = KeyMap[key];
		if (letra != TECLA_CONFIRMAR)
		{
			// asumimos que sólo se permiten números
			if (letra >= '0' && letra <= '9')
			{
				*numero_ingresado = (*numero_ingresado * 10) + (letra - '0');
			}
		}
	}
}

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
					// pantalla bienvenido
					LCDclr();
					_delay_ms(2);
					LCDstring((uint8_t *)"Bienvenido!!", 12);
					LCDGotoXY(0, 1);
					LCDstring((uint8_t *)"Presione *", 10);

					// indice actual de la contrasenia a ingresar

					pos_password = 0;
					vidas = 3;
				}

				if (KEYPAD_Scan(&key))
				{
					if (key < 16 && KeyMap[key] == TECLA_INICIO)
					{
						estado_actual = INICIO_JUEGO;
						contador_ticks_estado = 0;
					}
				}

				if (contador_ticks_estado >= 20) // 2 segundos (20 ticks de 100ms)
					contador_ticks_estado = 0;	 // reiniciar ventana
				break;

			case INICIO_JUEGO:
				if (contador_ticks_estado == 1 && mostrar_palabra)
				{
					LCDclr();
					_delay_ms(50);
					curr_word = get_random_word(diccionario, WORDS_SIZE);
					LCDstring((uint8_t *)curr_word, 5); // Mostrarla
					mostrar_palabra = 0;
				}

				if (contador_ticks_estado >= 20 && !mostrar_palabra)
				{			  // Despues de 2 segundos
					LCDclr(); // Borrar palabra
					_delay_ms(50);
					contador_ticks_estado = 0;
					caracter = curr_word[pos_password]; // Obtener el caracter a ingresar
					estado_actual = JUEGO;				// Cambiar a estado de juego
				}
				break;

			case JUEGO:
				if (pos_password < 5)
				{
					if (KEYPAD_Scan(&key))
					{
						if (key < 16){
							if (KeyMap[key] != TECLA_CONFIRMAR){
								if (pos_password )
								update_input_number(&numero_ingresado, key);
							}
						}
						if (key < 16 && KeyMap[key] == TECLA_CONFIRMAR)
						{
							// letra incorrecta
							if (numero_ingresado != caracter)
							{
								vidas--;
								if (vidas == 0)
								{
									estado_actual = PERDISTE;
									contador_ticks_estado = 0;
								}
							}
							else
							// letra correcta
							{
								if (pos_password == 5)
								{
									estado_actual = GANASTE;
									contador_ticks_estado = 0;
								}
							}
							pos_password++;
							numero_ingresado = 0; // Reiniciar el ingreso de caracteres
							contador_ticks_estado = 0;
							LCDGotoXY(pos_password + 1, 0);
							LCDsendChar(caracter);				// Mostrar el caracter a ingresar
							caracter = curr_word[pos_password]; // Obtener el caracter a ingresar
						}
						else
						{
							update_input_number(&numero_ingresado, key);
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
				pos_password = 0;
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
				pos_password = 0;
				break;
			}
		}
	}
}