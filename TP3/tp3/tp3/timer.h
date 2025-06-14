#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>

void timer_init(void);

extern volatile uint8_t timer_flag; // Flag para indicar que ha pasado 1 segundo

#endif