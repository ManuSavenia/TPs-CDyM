#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#define F_CPU 16000000UL // Definimos la frecuencia del reloj a 16MHz
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "rgb.h"

#define BAUD_RATE 103

void print_welcome_msg(){
    char message[] = "Bienvenido\r\nComandos: \r\n"
                     "1: Rojo\r\n"
                     "2: Verde\r\n"
                     "3: Azul\r\n"
                     "4: Cian\r\n"
                     "5: Amarillo\r\n"
                     "6: Magenta\r\n"
                     "7: Blanco\r\n"
                     "8: Negro\r\n\0";
    uart_send_string(message);
}

volatile uint8_t rx_buffer[BUFFER_SIZE];
volatile uint8_t cmd_index = 0;
volatile uint8_t rx_flag = 0;
uint8_t adc_value = 0;
char buffer[128];

void parse_command(char *cmd){
    if ((strlen(cmd) > 1) || (cmd[0] < '1') || (cmd[0] > '8')){
        uart_send_string("Comando no reconocido\r\n");
        return;
    }else {
        uint8_t index = cmd[0] - '1'; // Convertir el comando a un índice
        pwm_set_color(colors[index]);
        sprintf(buffer, "Color cambiado a %s\r\n", color_names[index]);
        uart_send_string(buffer);
    }
    rx_flag = 0;
}

int main(void){
    UART_init(BAUD_RATE);               // Inicializa UART con la velocidad de baudios definida
    adc_init();
    pwm_init();

    _delay_ms(100);
    print_welcome_msg();                // Imprime mensaje de bienvenida
    sei();
    while (1){
        if (rx_flag){ //comando recibido
            parse_command((char *)rx_buffer); // procesa el comando recibido
            rx_flag = 0;
        }

        adc_value = adc_read();            // lee el valor del ADC
        pwm_change_opacity(adc_value);     // cambia la opacidad del color actual basado en el valor del ADC
        pwm_update();
    }
    return 0;
}