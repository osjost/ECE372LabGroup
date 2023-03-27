#include "motor.h"
#include <avr/io.h>
#include <util/delay.h>

void initMotorPins() {

    //need two directional pins and one speed pin
    //speed pin will come from potentiometer
    //only init the directional pins that come from the board

    //in1 (PORTG1  // header 40) and in2 (PORTG0   //  header 41)

    DDRG |= (1<<DDG1) | (1<<DDG0);

    //enable  PG0 header 39
    DDRG |= (1<< DDG0);
}

void motorDirection(unsigned int direction) {
    //in1 in2
    // 1 1 -> OFF
    // 1 0 -> FORWARD
    // 0 1 -> BACKWARD
    // 0 0 -> OFF

    //direction will be value 0 1 2
    // off forward backward respectively

    

    switch (direction) {
        case 0: //off
        PORTG &= ~(1<<PORTG1);
        PORTG &= ~(1<<PORTG0);
        break;
        case 1: //forward
        PORTG |= (1<<PORTG1);
        PORTG &= ~(1<<PORTG0);
        break;
        case 2: //backward
        PORTG &= ~(1<<PORTG1);
        PORTG |= (1<<PORTG0);
        break;
        
    }


}
