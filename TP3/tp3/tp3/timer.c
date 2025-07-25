#include "timer.h"

void timer_init(void) {
    // Timer1 en modo CTC 
    // TCCR1A = 0;
    TCCR1B |= (1 << WGM12); // Modo CTC, TOP = OCR1A

    // 16MHz / 1024 = 15.625kHz -> 1s = 15,625 ciclos
    OCR1A = 15624;

    // Prescaler = 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Habilita interrupción por comparación en OCR1A
    TIMSK1 |= (1 << OCIE1A);
}

// ISR de Timer1 – ejecutado cada 1 segundo
ISR(TIMER1_COMPA_vect) {
    timer_flag = 1;
    // uart_send_string("Timer interrupt triggered\r\n");
}