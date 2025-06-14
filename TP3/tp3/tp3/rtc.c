#include "rtc.h"

uint8_t bcd_to_int(uint8_t bcd) {
    return (((bcd >> 4) & 0x0F) * 10) + (bcd & 0x0F);
}

uint8_t int_to_bcd(uint8_t value) {
    return (((value / 10) << 4) & 0xF0) | (value % 10);
}

void rtc_init(void) {
    rtc_disable_alarm(); // Desactivar alarma al iniciar
}

void rtc_disable_alarm(void) {
    i2c_start();
    i2c_write(DS3231_WRITE_MODE);
    i2c_write(DS3231_CONTROL_REG); // Apuntar al registro de control

    i2c_write(0x00); // Desactivar alarma 1 y 2
    i2c_stop();
}

void rtc_read_time(date_t *date) {
    i2c_start();
    i2c_write(DS3231_WRITE_MODE);
    i2c_write(DS3231_SECONDS_REG);      // Setear el registro de inicio en segundos

    i2c_stop();
    
    i2c_start();
    i2c_write(DS3231_READ_MODE);

    date->seconds = bcd_to_int(i2c_read(0) & SECONDS_MASK);
    date->minutes = bcd_to_int(i2c_read(0) & MINUTES_MASK);
    date->hours = bcd_to_int(i2c_read(0) & HOURS_MASK);

    i2c_read(0);

    date->day = bcd_to_int(i2c_read(0) & DAYS_MASK);
    date->month = bcd_to_int(i2c_read(0) & MONTHS_MASK); // Es necesario para ignorar el bit de century
    date->year = bcd_to_int(i2c_read(1) & YEARS_MASK);

    i2c_stop();
}

void rtc_set_time(date_t *date)
{
    i2c_start();
    i2c_write(DS3231_WRITE_MODE);
    i2c_write(DS3231_SECONDS_REG); // Apuntar al primer registro (segundos)

    i2c_write(int_to_bcd(date->seconds));
    i2c_write(int_to_bcd(date->minutes));
    i2c_write(int_to_bcd(date->hours));
    i2c_write(0); // Día de la semana (lo ignoramos o seteamos a 1)

    i2c_write(int_to_bcd(date->day));
    i2c_write(int_to_bcd(date->month));
    i2c_write(int_to_bcd(date->year));

    i2c_stop();
}