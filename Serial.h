#define __AVR_ATmega328P__

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define BAUD 9600
#define F_CPU 16000000UL
#include <util/setbaud.h>

class Serial{
    public:
        unsigned char read();
        void begin();
        void print(unsigned char[]);
        void printChar(unsigned char);
        void println(unsigned char[]);
        void printCharln(unsigned char);
        uint8_t available();
        void close();
};
