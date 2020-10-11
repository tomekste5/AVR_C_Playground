

#include <TWI.h>
#include <stdlib.h>
#include "Serial.h"
Serial sserial;

unsigned char writeBuffer[80];
unsigned char *readBuffer;

uint8_t indexWrite = 0;
uint8_t indexRead = 0;
uint8_t requestedBytes = 0;

bool finishFLag = 0;

void shiftBuffer(){
    for(int i = 1; i < 80; i++){
        writeBuffer[i-1] = writeBuffer[i];
        indexWrite--;
    }
}

void sentAddr(unsigned char addr){
    TWDR = addr;
    TWCR = (1<<TWINT) | (1<<TWEN);
}

void TWI::stop(){
    while ((TWCR & (1<< TWINT)));
    if(finishFLag == 1){
        TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
        finishFLag = 0;
    }
}
void TWI::read(unsigned char bytes[], unsigned char addr, int size){
    readBuffer = bytes;
    requestedBytes = size;
    TWCR |= (1<<TWIE);
    addr |= 0x01;
    sentAddr(addr);
    
}
ISR(TWI_vect, ISR_BLOCK){
    if ((TWSR & 0xF8) == TW_MT_DATA_ACK)
    {
        if(indexWrite > 0){
            finishFLag = 0;
            TWDR = writeBuffer[0];
            shiftBuffer();
            TWCR = (1<<TWEN) | (1<<TWINT);
        }else{
            finishFLag = 1;
            TWCR |= (0<<TWIE);
        }
    }
    
    else if((TWSR & 0xF8) == TW_MR_SLA_ACK | (TWSR & 0xF8) == TW_MR_DATA_ACK){//FIXME
       if(indexRead < requestedBytes){ 
           readBuffer[indexRead] = TWDR;
           TWCR |= (1<<TWEN) | (1<<TWEA);
           indexRead++;
        }else if(indexRead == (requestedBytes-1)){
            readBuffer[indexRead] = TWDR;
            TWCR |= (1<<TWEN) | (0<<TWIE) | (0<<TWEA);
            indexRead++;
            readBuffer[indexRead] = '\n'; //DONT NEED THAT
            
            TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);

        }
    }
}

void TWI::write(unsigned char data){
    writeBuffer[indexWrite] = data;
    indexWrite++;
    TWCR |= (1<< TWIE);
    TWCR |= (0<<TWINT);
}

void TWI::init(){
    DDRC |= (1<<PC5) | (1<<PC4);
    PORTC|= (1<<PC5) | (1<<PC4);

	TWBR =  0;
    // Prescaler to 64
    // Enable TWI
    	
    TWSR = 0;   // TWPS = 0 => prescale value = 1 
	    TWCR = (1 << TWEN) |                               // Enable TWI interface and release TWI pins.
	       (0 << TWIE) | (0 << TWINT) |                // Disable TWI interupt.
	       (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | // No signal requests.
	       (0 << TWWC);

}

void TWI::start(){
    TWCR &= ~((1 << TWSTO) | (1 << TWEN));
    TWCR |= (1 << TWEN);

    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    while  (!(TWSR & (1<<TWINT))); 

    if (((TWSR & 0xF8)!= TW_START) | ((TWSR & 0xF8) != TW_REP_START)){
        sserial.println((unsigned char*) "could not sent start bit");
    }
}