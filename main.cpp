#include "util.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include <util/setbaud.h>

#include "Serial.h"
#include "TWI.h"

unsigned char array[15] = "12345678912345";
TWI twi; 
Serial ssserial;
int main(void)
{
	ssserial.begin();

	twi.init();

	twi.start();
	twi.write(0x68);
	twi.write(0x00);
	twi.stop();

	twi.start();
	twi.write(0x68);
	twi.write(0x1c);
	twi.write(0x10);
	twi.stop();
	
	twi.start();
	twi.write(0x68);
	twi.write(0x1b);
	twi.write(0x08);
	twi.stop();

	/*twi.start();
	twi.read(array,0x68,14);*/
	

	//uint16_t acc_x = (array[0] << 8) | array[1];
	
	//serial.println((unsigned char*) "Vertical acceleration");
	//serial.println((unsigned char*) &acc_x);



	/*while (1 == 0)
	{
		while (serial.available() < 2);

		char my_char = serial.available() + '0';
		serial.printCharln(my_char);

		unsigned char c1 = serial.read();
		unsigned char c2 = serial.read();

		serial.printChar(c1);//FIXME
		serial.printCharln(c2);

		char my_cha2r = serial.available() + '0';
		serial.printCharln(my_cha2r);

	}*/
	_delay_ms(200);
	_delay_ms(200);
	_delay_ms(200);
	_delay_ms(200);
	_delay_ms(200);
	_delay_ms(200);
	
}
