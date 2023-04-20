// Author: Oliver Sjostrom
// Net ID: oliversjost
// Date: 3/16/2023
// Assignment:     Lab 3

#include "timer.h"
//You many use any timer you wish for the microsecond delay and the millisecond delay


/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
// For accuracy t 1us, 
//

// 

void initTimer1(){
    TCCR1A &= ~(1 << WGM00); // Set timer to be in CTC mode (Page 128 on Data Sheet)
    TCCR1A |= (1 << WGM01);
    TCCR1B &= ~(1 << WGM02);

    TCCR1B |= (1 << CS01) | (1 << CS00); // Sets the prescaler to 64
    //Set output compare registers to proper values
    OCR1A = 249; //Set count to 250
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayMs(unsigned int delay){
    unsigned int delayCnt = 0;
    TCNT1 = 0; //starting the timer at 0 instead of some random junk number
    TIFR1 |= (1 << OCF1A); // set compare flag to start timer
    while (delayCnt < delay) {
        if (TIFR1 & (1 << OCF1A)) { //increment only while the flag is set
            delayCnt++;
            TIFR1 |= (1 << OCF1A); //re-start timer. will go to 0 before reaching the if statement above
        }
    }
}