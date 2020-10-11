#include "util.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include <util/setbaud.h>

#include "pwm.h"

void analogWrite(int pin,int val){
    DDRB |= (1<<pin);
	TCCR1A = (1<<WGM10) | (1<<COM1A1); 
	TCCR1B = (1<<CS11) | (1<<CS10); 
	OCR1A = val-1; 
}