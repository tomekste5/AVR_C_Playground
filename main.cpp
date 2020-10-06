#include <Serial.h>
#include <avr/eeprom.h>
struct test
{
	uint8_t t;
	uint8_t x;
};


int main(void)
{
	Serial serial;
	serial.begin();

	test tesst;
	test testRead;

	tesst.t = 5;
	tesst.x = 9;

	eeprom_write_block((void *) &tesst,(void*) 0,sizeof(tesst));
	eeprom_read_block((void *) &testRead,(void *) 0,sizeof(testRead));


	char my_char = testRead.t + '0';
	serial.printCharln(my_char);
	char my_char2 = testRead.x + '0';
	serial.printCharln(my_char2);

	/*DDRB = 0xff;
	PORTB = 0xff;*/

	while (1 == 0)
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

	}
	
}

