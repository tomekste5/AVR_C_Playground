#ifdef __cplusplus
extern "C"{
#endif
     void analogWrite(int, int);
     uint16_t analogRead(uint8_t);
     unsigned long pulseIn(uint8_t);

     #include "util.h"

    #include <avr/io.h>
    #include <util/delay.h>
    #include <stdint.h>

    #include <util/setbaud.h>
#ifdef __cplusplus
}
#endif