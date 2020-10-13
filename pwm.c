#include "util.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <interrupt.h>

#include <util/setbaud.h>
#include "time.h"

#include "pwm.h"

void analogWrite(int pin,int val){
    DDRB |= (1<<pin);
	TCCR1A = (1<<WGM10) | (1<<COM1A1); 
	TCCR1B = (1<<CS11) | (1<<CS10); 
	OCR1A = val-1; 
}
uint16_t analogRead(uint8_t pin){
	ADCInit();

	ADMUX = (ADMUX & ~(0x1F)) | (pin & 0x1F);
  	ADCSRA |= (1<<ADSC);          
 	while (ADCSRA & (1<<ADSC) );
  	uint16_t result = ADCW;
	ADCSRA |= (0<<ADEN); //because of powere comsumption
	return result; 

}
void ADCInit(){
	
  ADMUX = (1<<REFS0);    //ref voltage

  ADCSRA = (1<<ADPS1) | (1<<ADPS0);     //prescaler
  ADCSRA |= (1<<ADEN);                 //ADC aktivieren

  

  ADCSRA |= (1<<ADSC);         //messung starten            
  while (ADCSRA & (1<<ADSC) ) {         
  }
  (void) ADCW;

}
uint8_t highTime = 0;
uint8_t lowTime = 0;
uint8_t tempTime;
int high = 0;

ISR(INT0_vect){
		if(high == 0){
			tempTime = millis();
			EICRA = (1<<3) | (0<<2);
			high++;
		}else if(high == 1){
			highTime = millis() - tempTime;
			EICRA = (1<<3) | (1<<2);
			tempTime = millis();
			high++;
		}else{
			lowTime = (millis() - tempTime);
			high = 0;
			EIMSK = 0x00;
		}
}


unsigned long pulseIn(uint8_t pin)
{
	EIMSK |= (1<<pin);   //Enable External Interrupts INT0 and INT1 should be 0x03
	EICRA = (1<<3) | (1<<2);  

	while (lowTime == 0);

	return	highTime / (lowTime+highTime);
	
}