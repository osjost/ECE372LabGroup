// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PD0 and sets it into input mode
 */
void initSwitchPD0(){
    // using the development board header pin 

    // set direction for input
    DDRD &= ~(1 << PORTD0);

    // enable the pullup resistor for stable input
    PORTD |= (1 << PORTD0);


    //enable interrupts for portd0 // int0
    enableINT0Interrupt();

    //enable int0 for any logical change triggering interrupt
    EICRA &= ~(1<<ISC01);
    EICRA |=  (1<<ISC00);
    
    
}
void enableINT0Interrupt() {
      //enable interrupts for portd0 // int0
    EIMSK |=  (1<<INT0);
}
void disableINT0Interrupt(){
  //disable interrupts for portd0 // int0
    EIMSK &=  ~(1<<INT0);
}
