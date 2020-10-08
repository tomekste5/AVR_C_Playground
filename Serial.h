#include "util.h"

#include <avr/interrupt.h>

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

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
