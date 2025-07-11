#ifndef RGB_H_
#define RGB_H_

#include <avr/io.h>

typedef struct{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgb_t;

extern const rgb_t colors[];
extern const char const *color_names[];

#endif /* RGB_H_ */