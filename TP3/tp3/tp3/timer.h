#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

void timer1_init(void);
void timer1_disable(void);

extern volatile uint8_t timer_flag; // Flag para indicar que ha pasado 1 segundo

#endif