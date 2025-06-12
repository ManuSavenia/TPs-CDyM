#include "rtc.h"
#include "i2c.h"
#include <avr/io.h>

typedef struct {
    uint8_t year;   // 0-99 
    uint8_t month;  // 1-12
    uint8_t day;    // 1-31
    uint8_t hours;  // 0-23
    uint8_t minutes; // 0-59
    uint8_t seconds; // 0-59
} date_t;

uint8_t bcd_to_int(uint8_t bcd) {
    return (((bcd >> 4) & 0x0F) * 10) + (bcd & 0x0F);
}

uint8_t int_to_bcd(uint8_t value) {
    return (((value / 10) << 4) & 0xF0) | (value % 10);
}

void rtc_init(void) {
    rtc_disable_alarm(); // Desactivar alarma al iniciar
}

void rtc_disable_alarm(void){
    i2c_start();
    i2c_write(DS3231_WRITE_MODE);
    i2c_write(DS3231_CONTROL_REG);
    i2s_write(0x00); // Desactivar alarma 1 y 2, limpiar el registro de control

    i2c_stop();
}

void rtc_set_alarm(date_t *date) {

    i2c_start();
    i2c_write(DS3231_WRITE_MODE);
    i2c_write(DS3231_ALARM1_REG); // Setear el registro de alarma 1
    i2c_write(int_to_bcd(date->seconds) & SECONDS_MASK); // Setear segundos
    i2c_write(int_to_bcd(date->minutes) & MINUTES_MASK); // Setear minutos
    i2c_write(int_to_bcd(date->hours) & HOURS_MASK);     // Setear horas
    i2c_write(0x00);
    i2c_write(int_to_bcd(date->day) & DAYS_MASK); // Setear dia

    i2c_stop();

    i2c_start();
    i2c_write(DS3231_WRITE_MODE);
    i2c_write(DS3231_CONTROL_REG);
    i2c_write(0x01); // Activar alarma 1 (bit 0)

    i2c_stop();

}

void rtc_get_time(date_t *date) {
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