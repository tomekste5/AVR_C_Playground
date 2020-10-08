#define __AVR_ATmega328P__

#include <util/twi.h>

class TWI{
    public:
        void init();
        void start();
        void read(unsigned char[], unsigned char, int);
        void write(unsigned char);
        void stop();
};