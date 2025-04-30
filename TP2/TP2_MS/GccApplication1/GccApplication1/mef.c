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

char* get_random_word(const char *dict[], uint8_t size) {
    return (char *)dict[rand() % size];
}

typedef enum {INICIO, INICIO_JUEGO, JUEGO, PERDISTE, GANASTE} estado_t;

const char *dict[] = {
	"Arbol", "Boton", "CDyMC", "ClavE", "Facil", "Gafas", "Hojas", "LiBro", "Lanza", "Nieve",
	"PeRro", "PecES", "PiAno", "PrYKe", "RUEDa", "SERIE", "SalUd", "Salud", "Silla", "Tecla",
	"Valor", "Verde", "YnHRz", "hARdD", "silla"
};

estado_t curr_state;
uint8_t input_char, hp, pass_index, first, sec_to_win;
char *password, guess[6];

void init_mef(){
    curr_state = INICIO;
}

void update_mef(volatile uint8_t *seg, uint8_t key){
    switch(curr_state) {
        case INICIO:
            LCDstring((uint8_t *)"Bienvenido!!", 12);
            LCDGotoXY(0, 1);
            LCDstring((uint8_t *)"Presione *", 10);
            if (key == CHAR_INICIO){
                strcpy(guess, "*****");
                first = 1;
                hp = 3;
                pass_index = 0;
                password = get_random_word(dict, NUM_WORDS);
                curr_state = INICIO_JUEGO;
            }
            break;

        case INICIO_JUEGO:
            // mostrar password por dos segundos
            if (first){
                LCDclr();
                LCDGotoXY(0, 0);
                LCDstring((uint8_t *)password, 5);
                first = 0;
                *seg = 0;
            }            
            //comenzar juego
            if (*seg == 2){
                curr_state = JUEGO;
                LCDclr();
                pass_index = 0;
                input_char = 0;
                seg = 0;
                LCDGotoXY(0, 0);
                LCDstring((uint8_t *)guess, 5); // Mostrar la palabra oculta
                first = 1;
            }
            break;

        case JUEGO:
            if (key != CHAR_CONFIRMAR){
                if (key < '9' && key > '0'){
                    input_char = input_char * 10 + key - '0';
                }
            }
            else{
                if (input_char == password[pass_index]){
                    guess[pass_index] = password[pass_index];
                    pass_index++;
                    LCDGotoXY(pass_index, 0);
                    LCDsendChar(password[pass_index]);
                    input_char = 0;
                }
                else{
                    hp--;
                    if (hp == 0){
                        curr_state = PERDISTE;
                        *seg = 0;
                    }
                }
                if (pass_index == 5){
                    curr_state = GANASTE;
                    sec_to_win = *seg;
                    *seg = 0;
                }
            }
            break;

        case PERDISTE:
            if (first){
                LCDclr();
                LCDstring((uint8_t *)"Derrota!", 10);
                LCDGotoXY(0, 1);
                LCDstring((uint8_t *)password, 5);
                first = 0;
            }
            if (*seg == 5){
                curr_state = INICIO;
                *seg = 0;
            }
            break;

        case GANASTE:
            if (first){
                LCDclr();
                LCDstring((uint8_t *)"SAPEEE", 7);
                LCDGotoXY(0, 1);
                char message[20];
                sprintf(message, "Ganaste en %d segs", sec_to_win);
                LCDstring((uint8_t *)message, 20);
                first = 0;
            }
            if (*seg == 5){
                curr_state = INICIO;
                *seg = 0;
            }
            break;
    }
}