#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lcd.h"
#include "keypad.h"
#include "timer.h"

#define CHAR_INICIO '*'
#define CHAR_CONFIRMAR '#'
#define NUM_WORDS 25

char *get_random_word(const char *dict[], uint8_t size)
{
    return (char *)dict[rand() % size];
}

typedef enum
{
    INICIO,
    INICIO_JUEGO,
    JUEGO,
    PERDISTE,
    GANASTE
} estado_t;

const char *dict[] = {
    "Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro", "Lanza", "Nieve",
    "PeRro", "PecES", "PiAno", "PrYKe", "RUEDa", "SERIE", "SalUd", "Salud", "Silla", "Tecla",
    "Valor", "Verde", "YnHRz", "hARdD", "silla"};

estado_t curr_state;
uint8_t input_char, hp, pass_index = 0, first, sec_to_win, Ftime = 1;
char *password, guess[6];
char life[7];

void init_mef()
{
    curr_state = INICIO;
}

void update_mef(volatile uint16_t *seg, uint8_t key)
{
    switch (curr_state)
    {
    case INICIO:
        if (Ftime)
        {
            LCDclr();
            _delay_ms(10);
            LCDstring("Bienvenido!!", 12);
            LCDGotoXY(0, 1);
            LCDstring("Presione *", 10);
            Ftime = 0;
        }
        else
        {
            if (key == '*')
            {
                strcpy(guess, "*****");
                first = 1;
                hp = 3;
                pass_index = 0;
                password = get_random_word(dict, NUM_WORDS);
                curr_state = INICIO_JUEGO;
                Ftime = 1;
            }
        }
        break;

    case INICIO_JUEGO:
        // mostrar password por dos segundos
        if (first)
        {
            LCDclr();
            _delay_ms(160);
            LCDGotoXY(0, 0);
            LCDstring(password, 5);
            first = 0;
            *seg = 0;
        }
        // comenzar juego
        if (*seg == 2)
        {
            curr_state = JUEGO;
            LCDclr();
            pass_index = 0;
            input_char = 0;
            strcpy(life, "<3<3<3");
            seg = 0;
            LCDGotoXY(0, 0);
            LCDstring((uint8_t *)guess, 5); // Mostrar la palabra oculta
            first = 1;
        }
        break;

    case JUEGO:
        if (key != CHAR_CONFIRMAR)
        {
            if (key <= '9' && key >= '0')
            {
                input_char = input_char * 10 + key - '0';
            }
        }
        else
        {
            if (input_char == password[pass_index])
            {
                guess[pass_index] = password[pass_index];
                LCDGotoXY(pass_index, 0);
                LCDsendChar(password[pass_index]);
                pass_index++;
                input_char = 0;
            }
            else
            {
                hp--;
                if (hp == 0)
                {
                    curr_state = PERDISTE;
                    *seg = 0;
                }

                if (hp == 2)
                {
                    life[0] = ' ';
                    life[1] = ' ';
                }
                else if (hp == 1)
                {
                    life[2] = ' ';
                    life[3] = ' ';
                }
                else if (hp == 0)
                {
                    life[4] = ' ';
                    life[5] = ' ';
                }
                input_char = 0;
            }
            if (pass_index == 5)
            {
                curr_state = GANASTE;
                sec_to_win = *seg;
                *seg = 0;
            }
        }
        LCDGotoXY(9, 1);
        LCDstring(life, 6); // Mostrar vidas restantes
        break;

    case PERDISTE:
        if (first)
        {
            LCDclr();
            _delay_ms(160);
            LCDstring("Derrota!", 8);
            LCDGotoXY(0, 1);
            LCDstring(password, 5);
            first = 0;
            *seg = 0;
        }
        else
        {
            if (*seg == 5)
            {
                curr_state = INICIO;
                *seg = 0;
            }
        }
        break;

    case GANASTE:
        if (first)
        {
            LCDclr();
            _delay_ms(160);
            LCDstring("Victoria!", 9);
            LCDGotoXY(0, 1);
            char message[20];
            sprintf(message, "Tiempo: %ds", sec_to_win);
            LCDstring(message, 11);
            first = 0;
            *seg = 0;
        }
        else
        {
            if (*seg == 5)
            {
                curr_state = INICIO;
                *seg = 0;
            }
        }
        break;
    }
}