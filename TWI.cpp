

#include <TWI.h>
#include <stdlib.h>
#include "Serial.h"
Serial sserial;

void sentAddr(unsigned char addr){
    TWDR = addr;
    TWCR = (1<<TWINT) | (1<<TWEN);

    while ((TWCR & (1<< TWINT)));
    if ((TWSR & 0xF8)!= TW_MT_SLA_ACK){
        sserial.println((unsigned char*) "could not sent sentAddr");
    }

}
void TWI::read(unsigned char bytes[], unsigned char addr, int size){
    addr |= 0x01;
    sentAddr(addr);
    for(int i = 0; i < size - 2; i++){
        bytes[i] = TWDR;
        TWCR = (1<<TWINT) | (1<<TWEN);

        while ((TWCR & (1<< TWINT)));
    }
    bytes[size-2] = TWDR;
    bytes[size-1] = '\n';
    TWCR = (1<<TWINT) | (0<<TWEN);

    stop();
}

void TWI::write(unsigned char data){
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);

    while ((TWCR & (1<< TWINT)));
     if ((TWSR & 0xF8)!= TW_MT_DATA_ACK){
        sserial.println((unsigned char*) "Transmitting data was not sucessfull!");
    }
}

void TWI::init(){
    TWAR=0x00;
    TWBR=0x00;
    TWCR=0x04;

}

void TWI::start(){
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    while  (!(TWSR & (1<<TWINT))); 

    if ((TWSR & 0xF8)!= TW_START){
        sserial.println((unsigned char*) "could not sent start bit");
    }
}
void TWI::stop(){
    while ((TWCR & (1<< TWINT)));

    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}