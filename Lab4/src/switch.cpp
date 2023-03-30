// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include <avr/io.h>
#include "switch.h"


// initialize switch to use external interrupt.  We'll use INT0 that corresponds to header pin 21 on dev board

void initswitch() {

  //set data direction as input
  DDRD &= ~(1<<DDD0); 
  //enable pullup resistor
  PORTD |= (1 << PORTD0);  
  
  //enable interrupts for INT0 
  EICRA &=  ~( 1 << ISC01);
  EICRA |=  ( 1 << ISC00);

  //enable external interrupt mask reg for INT0
  EIMSK |= ( 1 << INT0);
}
