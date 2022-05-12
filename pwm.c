#include "util.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include <util/setbaud.h>
#include <avr/wdt.h>
#include "time.h"

#include "pwm.h"

    uint8_t highTime = 0;
    uint8_t lowTime = 0;
    uint8_t tempTime;
    int high = 0;

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

ISR(INT0_vect){
	cli();
		if(high == 0){
			tempTime = millis();
			high = 1;
			EICRA = (1<<3) | (0<<2) | (1<<1) | (0<<0);
		}else if(high == 1){
			wdt_reset();
			high = 2;
			highTime = millis() - tempTime;
			EICRA = (1<<3) | (1<<2) | (1<<1) | (1<<0);
			tempTime = millis();
		}else{
			lowTime = (millis() - tempTime);
			high = 0;
			EIMSK = 0x00;
			EICRA = 0x00;
		}
	sei();
}

ISR(INT1_vect){
	cli();
		
		if(high == 0){
			tempTime = millis();
			EICRA = (1<<3) | (0<<2) | (1<<1) | (1<<0);
			high++;
		}else if(high == 1){
			wdt_reset();
			highTime = millis() - tempTime;
			EICRA = (1<<3) | (1<<2) | (1<<1) | (1<<0);
			tempTime = millis();
			high++;
		}else{
			lowTime = (millis() - tempTime);
			high = 0;
			EIMSK = 0x00;
			EICRA = 0x00;
		}
	sei();
}


unsigned long pulseIn(uint8_t pin)
{
	SREG |= (1<<7);
	EIMSK |= 0x03;   //Enabl External Interrupts INT0 and INT1 should be 0x03
	EICRA = (1<<3) | (1<<2) | (1<<1) | (1<<0);  

	while (lowTime == 0);
	
	SREG |= (0<<7);

	return	highTime / (lowTime+highTime);
	
}