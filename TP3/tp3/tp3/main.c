#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#define F_CPU 16000000UL // Definimos la frecuencia del reloj a 16MHz
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "i2c.h"
#include "uart.h"
#include "rtc.h"
#include "timer.h"

#define BAUD_RATE 103

void print_welcome_msg(){
    char message[] = "Bienvenido\r\nCOMANDOS:\r\n - ON\r\n - OFF\r\n - SET TIME\r\n - SET ALARM\r\n";
    // char message[] = "Holaa\r\n";
    uart_send_string(message);
}

volatile uint8_t command_buffer[CMD_BUFFER_SIZE];
volatile uint8_t cmd_index;
volatile uint8_t command_ready;

// volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
// volatile uint8_t rx_index;
// volatile uint8_t rx_flag;

date_t current_date; // Estructura para almacenar la fecha y hora actual
date_t alarm_date; // Estructura para almacenar la fecha y hora de la alarma

volatile uint8_t flag_on = 0;
volatile uint8_t flag_alarm = 0;

volatile uint8_t alarm_counter = 0;

volatile uint8_t timer_flag = 0;

volatile uint8_t WAITING_SET_TIME = 0; // Variable para esperar el tiempo de respuesta del usuario
volatile uint8_t WAITING_SET_ALARM = 0; // Variable para esperar el tiempo de respuesta del usuario
volatile uint8_t ok = 0;

char buffer[128];

// posibles comandos
typedef enum {
    COMMAND_OFF = 0,
    COMMAND_ON,
    COMMAND_SET_TIME,
    COMMAND_SET_ALARM,
    INVALID
} command_t;

command_t command_type;

void parse_command(const char *cmd){
    if (strcmp(cmd, "ON") == 0)
        command_type = COMMAND_ON;
    else if (strcmp(cmd, "OFF") == 0)
        command_type = COMMAND_OFF;
    else if (strcmp(cmd, "SET TIME") == 0)
        command_type = COMMAND_SET_TIME;
    else if (strcmp(cmd, "SET ALARM") == 0)
        command_type = COMMAND_SET_ALARM;
    else{
        uart_send_string("Comando desconocido\r\n");
        command_type = INVALID; 
    }
}

// int main(void){
//     uart_init();
//     print_welcome_msg(); // Imprime mensaje de bienvenida
//     i2c_init(); // Inicializa I2C
//     // rtc_init(); // Inicializa el RTC
//     _delay_ms(50);
//     get_initial_date(&current_date);
//     rtc_set_time(&current_date); // Configura el RTC con la fecha inicial
//     // timer1_init();
//     sei();

//     while (1){
//         // se ejecuta cada 1 segundo
//         if (timer_flag){
//             rtc_read_time(&current_date);  // leer el tiempo del DS3231
//             if (alarm_counter != 0) {
//                 alarm_counter--;
//                 uart_send_string_nonblocking("ALARMA\r\n");
//             }
//             else{
//                 // si esta el modo ON, enviar la hora actual
//                 if (flag_on && !WAITING_SET_TIME && !WAITING_SET_ALARM) {
//                     sprintf(buffer, "FECHA: %02d/%02d/%02d HORA:%02d:%02d:%02d\r\n",
//                             current_date.day, current_date.month, current_date.year,
//                             current_date.hours, current_date.minutes, current_date.seconds);
//                     uart_send_string_nonblocking(buffer);
//                 }
//                 // si la alarma esta activa, verificar si se debe activar
//                 if (flag_alarm) {
//                     if (check_alarm(&current_date, &alarm_date)) {
//                         flag_alarm = 0; // Desactiva la alarma
//                         alarm_counter = 5;
//                     }
//                 }
//             }
//         }

//         // se ejecuta cada vez que se recibe un comando por UART
//         if (rx_flag){
//             rx_flag = 0;

//             if (WAITING_SET_TIME){
//                 //validar fecha, hora y actualizar RTC                
//                 ok = parse_date((char *)rx_buffer, &current_date);
//                 if (ok == 1){
//                     rtc_set_time(&current_date); // Actualiza el RTC con la nueva fecha y hora
//                     uart_send_string_nonblocking("Fecha y hora actualizadas correctamente.\r\n");
//                     WAITING_SET_TIME = 0; // Resetea el flag de espera
//                 } else {
//                     uart_send_string_nonblocking("Fecha y hora no validas. Formato esperado: DD/MM/YY HH:MM:SS\r\n");
//                 }
//                 continue;
//             }

//             if (WAITING_SET_ALARM){
//                 //validar hora y actualizar alarma 
//                 ok = parse_time((char *)rx_buffer, &alarm_date);
//                 if (ok){
//                     alarm_date.day = current_date.day; // Mantiene el mismo dia
//                     alarm_date.month = current_date.month; // Mantiene el mismo mes
//                     alarm_date.year = current_date.year; // Mantiene el mismo año
//                     WAITING_SET_ALARM = 0;
//                     flag_alarm = 1;
//                 }
//                 else{
//                     uart_send_string_nonblocking("Hora de alarma no valida. Formato esperado: HH:MM:SS\r\n");
//                 }
//                 continue;
//            }

//             parse_command((char *)rx_buffer); // Procesa el comando recibido

//             if (command_type == INVALID) {
//                 uart_send_string_nonblocking("Comando no reconocido.\r\n");
//                 continue;
//             }
//             else if (command_type == COMMAND_OFF){
//                 if (flag_on) {
//                     flag_on = 0; // Desactiva el modo ON
//                     uart_send_string_nonblocking("MODO OFF activado.\r\n");
//                 } else {
//                     uart_send_string_nonblocking("El sistema ya esta en modo OFF.\r\n");
//                 }
//             }
//             else if (command_type == COMMAND_ON){
//                 if (flag_on) {
//                     uart_send_string_nonblocking("El sistema ya esta en modo ON.\r\n");
//                 } else {
//                     flag_on = 1; // Activa el modo ON
//                     uart_send_string_nonblocking("MODO ON activado.\r\n");
//                 }
//             }
//             else if (command_type == COMMAND_SET_TIME){
//                 flag_on = 0;
//                 WAITING_SET_TIME = 1;
//                 uart_send_string_nonblocking("Ingrese fecha y hora (DD/MM/YY HH:MM:SS):\r\n");
//             }
//             else if (command_type == COMMAND_SET_ALARM){
//                 flag_on = 0;
//                 WAITING_SET_ALARM = 1;
//                 uart_send_string_nonblocking("Ingrese hora de alarma (HH:MM:SS):\r\n");
//             }
//         }
//     }
// }

int main(void){
    // UART_init();
    // print_welcome_msg(); // Imprime mensaje de bienvenida
    // _delay_ms(50);
    // sei();
    UART_init();
    print_welcome_msg(); // Imprime mensaje de bienvenida
    i2c_init(); // Inicializa I2C
    rtc_init(); // Inicializa el RTC
    _delay_ms(50);
    get_initial_date(&current_date);
    rtc_set_time(&current_date); // Configura el RTC con la fecha inicial
    timer1_init();
    sei();
    while (1){
        _delay_ms(1000);
        uart_send_string("Tick\r\n");
    }
}