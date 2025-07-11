#include "pwm.h"

static rgb_t curr_color = {0, 0, 0};    //color actual, default = white
uint8_t pwm_red = 0;                    //valor top del pwm rojo (por software)

void pwm_init(void){
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);  // oc1a y oc1b en modo fast pwm 8 bit
    TCCR1B = (1 << WGM12) | (1 << CS12);                    // prescaler 256, modo fast pwm 8 bit

    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB5); // PB1, PB2 y PB5 como salidas
}

void pwm_set_color(rgb_t color){
    curr_color.red = color.red;
    curr_color.green = color.green;
    curr_color.blue = color.blue;
}

//opacity : 0-255
void pwm_change_opacity(uint8_t opacity){
    // OCR1A = opacity * curr_color.blue;
    // OCR1B = opacity * curr_color.green;
    // pwm_red = opacity * curr_color.red;

    OCR1A = (255 - opacity) * curr_color.blue;
    OCR1B = (255 - opacity) * curr_color.green;
    pwm_red = (255 - opacity) * curr_color.red;
}

void pwm_update(void){
    PORTB = (TCNT1 > pwm_red) ? (PORTB & ~(1 << PB5)) : (PORTB | (1 << PB5)); // actualizar el estado del rojo
}