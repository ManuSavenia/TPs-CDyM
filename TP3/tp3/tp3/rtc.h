#ifndef RTC_H
#define RTC_H

#define DS3231_ADDRESS 0x68                             // 0b1101000, direccion I2C del DS3231
#define DS3231_READ_MODE (DS3231_ADDRESS << 1) | 0x01   // Enviamos un 1 adicional para indicar lectura
#define DS3231_WRITE_MODE (DS3231_ADDRESS << 1)         // Enviamos un 0 adicional para indicar escritura
#define DS3231_CONTROL_REG 0x0E                         // Registro de control del DS3231
#define DS3231_SECONDS_REG 0x00                         // Registro de segundos
#define DS3231_ALARM1_REG 0x07                          // Registro de alarma 1

#define SECONDS_MASK    0b01111111 // Mascara para los segundos (0-59)
#define MINUTES_MASK    0b01111111 // Mascara para los minutos (0-59)
#define HOURS_MASK      0b00111111 // Mascara para las horas (0-23), usa formato 24 horas

#define DAYS_MASK       0b00111111 // Mascara para los dias (1-31)
#define MONTHS_MASK     0b00011111 // Mascara para los meses (1-12)
#define YEARS_MASK      0b11111111 // Mascara para los años (0-99)

#endif // RTC_H

//pin ~INT = salida alarma