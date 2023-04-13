// Description: This file implements turnOnLED and initLED to control
// the LEDs.
//----------------------------------------------------------------------//


#include <avr/io.h>
#include <util/delay.h>
#include "led.h"


///TESTST

/* Initialize PD0, PD1, PD2, and PD3 to outputs
*/
void initLED(){
    DDRD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD2) | (1 << DDD3);
}

/* This must be one line of code.
* In this function you will be giving a number "num" which will be represented
* in binary by four LEDs. You must effectively assign the lowest four bits of
* "num" to the appropriate bits of PORTA.
*/
void turnOnLEDWithChar(unsigned char num){
  PORTD = num & 0xFF; 
}
