#ifndef UTILS_H
#define UTILS_H

#include "timer.h"
#include <avr/io.h>
#include "rtc.h"

void get_initial_date(date_t *date);
uint8_t check_alarm(date_t *current_date, date_t *alarm_date);
uint8_t parse_data(char date_str[], date_t *parsed_date);
uint8_t parse_time(char time_str[], date_t *parsed_time);

uint8_t d, m, y, h, min, s; // Variables para parsear fecha y hora

#endif // UTILS_H