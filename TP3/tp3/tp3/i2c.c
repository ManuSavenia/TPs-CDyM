#include "i2c.h"

void i2c_init(void){
   TWSR = 0x00;
   TWBR = 72;           //frecuencia de 100KHz del SCL
   TWCR = (1 << TWEN);  //Habilitamos el I2C
}

void i2c_start(void){
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);
}

void i2c_write(uint8_t data){
    TWDR = data;                        //Cargamos el dato a enviar
    TWCR = (1 << TWINT) | (1 << TWEN);  //Iniciamos la transmision
    while ((TWCR & (1 << TWINT)) == 0); //Esperamos a que se complete
}

uint8_t i2c_read(uint8_t isLast){
    if (isLast)
        TWCR = (1 << TWINT) | (1 << TWEN); // Enviar Nack
    else
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Enviar Ack

    while ((TWCR & (1 << TWINT)) == 0);
    return TWDR; 
}

void i2c_stop(void){
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}