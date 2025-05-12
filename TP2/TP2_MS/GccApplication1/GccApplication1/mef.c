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
    DERROTA,
    VICTORIA
} estado_t;

const char *dict[] = {
    "Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro", "Lanza", "Nieve",
    "PeRro", "PecES", "PiAno", "PrYKe", "RUEDa", "SERIE", "SalUd", "Salud", "Silla", "Tecla",
    "Valor", "Verde", "YnHRz", "hARdD", "silla"};

estado_t curr_state;
uint8_t input_char, hp, pass_index = 0, first, sec_to_win, firstExec = 1;
uint8_t firstRun = 1;
volatile uint8_t seg = 0, flag = 0, tick = 0;
char *password, guess[6];
char life[7];

void init_mef()
{
    curr_state = INICIO;
}

void update_mef(uint8_t key)
{
    flag = 0;
    switch (curr_state)
    {
    case INICIO:
        if (firstExec)
        {
            LCDclr();
            _delay_ms(10);
            LCDstring("Bienvenido!!", 12);
            LCDGotoXY(0, 1);
            LCDstring("Presione *", 10);
            firstExec = 0;
        }
        else
        {
            if (key == CHAR_INICIO)
            {
                // obtener semilla random, solo la primera ejecucion
                if (firstRun){
                    srand(seg * 100 + tick);
                    firstRun = 0;
                }
                strcpy(guess, "*****");
                first = 1;
                hp = 3;
                pass_index = 0;
                curr_state = INICIO_JUEGO;
                firstExec = 1;
                password = get_random_word(dict, NUM_WORDS);
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
            seg = 0;
        }
        // comenzar juego
        if (seg == 2)
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
                    curr_state = DERROTA;
                    seg = 0;
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
                curr_state = VICTORIA;
                sec_to_win = seg;
                seg = 0;
            }
        }
        LCDGotoXY(9, 1);
        LCDstring(life, 6); // Mostrar vidas restantes
        break;

    case DERROTA:
        if (first)
        {
            LCDclr();
            _delay_ms(160);
            LCDstring("Derrota!", 8);
            LCDGotoXY(0, 1);
            LCDstring(password, 5);
            first = 0;
            seg = 0;
        }
        else
        {
            if (seg == 5)
            {
                curr_state = INICIO;
                seg = 0;
            }
        }
        break;

    case VICTORIA:
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
            seg = 0;
        }
        else
        {
            if (seg == 5)
            {
                curr_state = INICIO;
                seg = 0;
            }
        }
        break;
    }
}