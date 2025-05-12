#ifndef TIMER_H
#define TIMER_H

extern volatile uint8_t flag;
extern volatile uint8_t seg;
extern volatile uint8_t tick;

void timer0_init();
#endif                        // TIMER_H