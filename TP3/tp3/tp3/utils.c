#include "utils.h"

// Función para hardcodear una fecha inicial
void get_initial_date(date_t *date) {
    date->year = 25;
    date->month = 6;
    date->day = 23;
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

uint8_t parse_date(char date_str[], date_t *parsed_date) {
    // Validar longitud mínima esperada
    size_t len = strlen(date_str);
    if (len < 17 || len > 19)
        return 0;

    // Verificar separadores exactos en posiciones esperadas
    if (date_str[2] != '/' || date_str[5] != '/' ||
        date_str[8] != ' ' || date_str[11] != ':' || date_str[14] != ':') {
        return 0;
    }

    // Parsear usando sscanf
    if (sscanf(date_str, "%2hhu/%2hhu/%2hhu %2hhu:%2hhu:%2hhu",
               &d, &m, &y, &h, &min, &s) != 6){
        return 0;
    }

    // Validar rangos básicos
    if (d == 0 || d > 31)
        return 0;
    if (m == 0 || m > 12)
        return 0;
    if (h > 23)
        return 0;
    if (min > 59)
        return 0;
    if (s > 59)
        return 0;

    // Guardar en estructura
    parsed_date->day = d;
    parsed_date->month = m;
    parsed_date->year = y;
    parsed_date->hours = h;
    parsed_date->minutes = min;
    parsed_date->seconds = s;

    return 1;
}

uint8_t parse_time(char date_str[], date_t *parsed_time)
{
    // Validar longitud mínima esperada
    size_t len = strlen(date_str);
    if (len != 9)
        return 0;

    // Verificar separadores exactos en posiciones esperadas
    if (date_str[2] != ':' || date_str[5] != ':')
    {
        return 0;
    }

    // Parsear usando sscanf
    if (sscanf(date_str, "%2hhu:%2hhu:%2hhu",
               &h, &min, &s) != 3)
    {
        return 0;
    }

    // Validar rangos básicos
    if (h > 23)
        return 0;
    if (min > 59)
        return 0;
    if (s > 59)
        return 0;

    // Guardar en estructura
    parsed_time->hours = h;
    parsed_time->minutes = min;
    parsed_time->seconds = s;

    return 1;
}