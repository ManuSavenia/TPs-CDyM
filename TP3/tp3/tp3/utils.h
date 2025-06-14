#ifndef UTILS_H
#define UTILS_H

#include "timer.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "rtc.h"

void get_initial_date(date_t *date);
uint8_t check_alarm(date_t *current_date, date_t *alarm_date);
uint8_t is_valid_date(const char *str, date_t *date);
uint8_t is_valid_time(const char *str, date_t *result, const date_t *current_date);

uint8_t d, m, y, h, min, s; // Variables para parsear fecha y hora

#endif // UTILS_H