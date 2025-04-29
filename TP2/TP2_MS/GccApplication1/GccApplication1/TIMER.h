#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
void timer0_init(void);
extern volatile uint8_t tick; // Flag de "tick" de la MEF
#endif                        // KEYPAD_H