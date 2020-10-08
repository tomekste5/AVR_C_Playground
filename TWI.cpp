

#include <TWI.h>
#include <stdlib.h>

void * operator new(size_t size)
{
  return malloc(size);
}

void sentAddr(unsigned char addr){
    TWDR = addr;
    TWCR = (1<<TWINT) | (1<<TWEN);

    while ((TWCR & (1<< TWINT)));
}
void TWI::read(unsigned char bytes[], unsigned char addr, int size){
    addr |= 0x01;
    sentAddr(addr);
    for(int i = 0; i < size - 1; i++){
        bytes[i] = TWDR;
        TWCR = (1<<TWINT) | (1<<TWEN);

        while ((TWCR & (1<< TWINT)));
    }
    bytes[size-1] = TWDR;

    TWCR = (1<<TWINT) | (0<<TWEN);

    stop();
}

void TWI::write(unsigned char data){
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);

    while ((TWCR & (1<< TWINT)));
}

void TWI::init(){

  TWBR = TWBR0;                                  // Set bit rate register (Baud rate). Defined in header file.Driver presumes prescaler to be 00.    //FIXME                            
  TWDR = 0xFF;                                      // Default content = SDA released.
  TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins.
         (0<<TWIE)|(0<<TWINT)|                      // Disable Interrupt.
         (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests.
         (0<<TWWC);                                 // 
}

void TWI::start(){
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

    while  (!(TWCR & (1<<TWINT)));
}
void TWI::stop(){
    while ((TWCR & (1<< TWINT)));

    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}