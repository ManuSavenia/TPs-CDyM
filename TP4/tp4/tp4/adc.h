#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void adc_init(void);
uint8_t adc_read(void);

#endif /* ADC_H_ */