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

    TCCR1B |=  (0<<CS02) | (1 << CS01) | (0 << CS00); // Sets the prescaler to 8
    //Set output compare registers to proper values
    OCR1AH = 0;
    OCR1AL = 2;
    
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayUs(unsigned int delay){
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

// Code taken from lecture timers_0, some values just tweaked 

//Used the lecture equation to caclulate the value of OCR0A and prescaler.
//We need the flag to be raised every millisecond
//Uses a 16mHz clock, using prescaler of 1024, count would need to be 15.625, which isn't a whole number.
//So, instead use a prescaler of 64, count then needs to be 250
void initTimer0(){
    
    TCCR0A &= ~(1 << WGM00); // Set timer to be in CTC mode (Page 128 on Data Sheet)
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);
    //Tweaked prescaler to use correct value to get 1 milliscond and not 10 milliseconds
    TCCR0B |= (1 << CS01) | (1 << CS00); // Sets the prescaler to 64
    //Tweaked count register
    OCR0A = 249; //Set count to 250
}

/* This delays the program an amount of milliseconds specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-200 milliseconds
*/
void delayMs(unsigned int delay){ // Code taken from lecture timers_0
    unsigned int delayCnt = 0;
    TCNT0 = 0; //starting the timer at 0 instead of some random junk number
    TIFR0 |= (1 << OCF0A); // set compare flag to start timer
    while (delayCnt < delay) {
        if (TIFR0 & (1 << OCF0A)) { //increment only while the flag is set (timer reachedmaxval = OCR0A)
            delayCnt++;
            TIFR0 |= (1 << OCF0A); //re-start timer. will go to 0 before reaching the if statement above
        }
    }
}