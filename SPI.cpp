#include "util.h"
#include <avr\io.h>

#include "SPI.h"

void SPI::init(){
    DDRB = (1<<5) | (1<<3);
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPIE);
}
void SPI::write(unsigned char msg){
    SPDR = msg;
    while (!(SPSR & (1<<SPIF)));
    
}
ISR(SPI_STC_vect){
    
}
char SPI(void)
{
/* Wait for reception complete */
while(!(SPSR & (1<<SPIF)))
;
/* Return Data Register */
return SPDR;
}