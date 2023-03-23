// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPB3(){
    // using the development board header pin 50

    // set direction for input
    DDRB &= ~(1 << PORTB3);

    // enable the pullup resistor for stable input
    PORTB |= (1 << PORTB3);

    // enable the interrupt on the switch. From lecture 11 ISR-2
    // enable PCINT 0 - 7
    PCICR |= (1 << PCIE0); 
    // enable PCINT3 since we're using switch 3
    PCMSK0 |= (1 << PCINT3); 
}
