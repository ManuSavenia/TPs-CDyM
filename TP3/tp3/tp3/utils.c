#include "utils.h"

// Función para hardcodear una fecha inicial
void get_initial_date(date_t *date) {
    date->year = 25;
    date->month = 6;
    date->day = 14;
    date->hours = 16;
    date->minutes = 0; // Minuto 0
    date->seconds = 0; // Segundo 0
}

uint8_t check_alarm(date_t *current_date, date_t *alarm_date) {
    // Compara la fecha y hora actual con la alarma
    if (current_date->year == alarm_date->year &&
        current_date->month == alarm_date->month &&
        current_date->day == alarm_date->day &&
        current_date->hours == alarm_date->hours &&
        current_date->minutes == alarm_date->minutes &&
        current_date->seconds == alarm_date->seconds) {
        return 1; // Alarma activada
    }
    return 0; // Alarma no activada
}

uint8_t is_valid_date(const char *str, date_t *date) {
    if (strlen(str) != 17) return 0;

    // validar formato de separadores
    if (str[2] != '/' || str[5] != '/' || str[8] != ' ' ||
        str[11] != ':' || str[14] != ':') {
        return 0;
    }

    for (int i = 0; i < 17; i++) {
        if (i == 2 || i == 5 || i == 8 || i == 11 || i == 14)
            continue;
        if (!isdigit((unsigned char)str[i]))
            return 0;
    }

    uint8_t day   = (str[0] - '0') * 10 + (str[1] - '0');
    uint8_t month = (str[3] - '0') * 10 + (str[4] - '0');
    uint8_t year  = (str[6] - '0') * 10 + (str[7] - '0');
    uint8_t hour  = (str[9]  - '0') * 10 + (str[10] - '0');
    uint8_t min   = (str[12] - '0') * 10 + (str[13] - '0');
    uint8_t sec   = (str[15] - '0') * 10 + (str[16] - '0');

    // validar rangos
    if (day < 1 || day > 31 || month < 1 || month > 12 ||
        hour > 23 || min > 59 || sec > 59) {
        return 0;
    }

    // Validar días del mes
    const int days_in_month[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (day > days_in_month[month - 1]) {
        return 0;
    }

    date->day = (uint8_t)day;
    date->month = (uint8_t)month;
    date->year = (uint8_t)year;
    date->hours = (uint8_t)hour;
    date->minutes = (uint8_t)min;
    date->seconds = (uint8_t)sec;

    return 1;
}

uint8_t is_valid_time(const char *str, date_t *result, const date_t *current_date) {
    if (strlen(str) != 8) return 0;

    // validar formato HH:MM:SS
    if (str[2] != ':' || str[5] != ':')
        return 0;

    for (int i = 0; i < 8; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit((unsigned char)str[i]))
            return 0;
    }

    uint8_t hour = (str[0] - '0') * 10 + (str[1] - '0');
    uint8_t min  = (str[3] - '0') * 10 + (str[4] - '0');
    uint8_t sec  = (str[6] - '0') * 10 + (str[7] - '0');

    if (hour > 23 || min > 59 || sec > 59)
        return 0;

    result->year = current_date->year;
    result->month = current_date->month;
    result->day = current_date->day;

    result->hours = (uint8_t)hour;
    result->minutes = (uint8_t)min;
    result->seconds = (uint8_t)sec;

    return 1;
}