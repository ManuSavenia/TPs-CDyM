#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include "rgb.h"


void pwm_init(void);

void pwm_set_color(rgb_t color);
    
void pwm_change_opacity(uint8_t opacity);

void pwm_update(void);

#endif /* PWM_H_ */