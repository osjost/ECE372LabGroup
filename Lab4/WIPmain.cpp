#include <avr/io.h>
#include <avr/interrupt.h>
#include "switch.h"
#include "timer.h"

typedef enum stateType_enum {
  wait_press, debounce_press, wait_release, debounce_release
} stateType;

volatile stateType theState = wait_press;

///////////////////////////////////////////////////////////////////

int main(){
   sei();

   while(1) {
	

	switch(theState) {
	  case wait_press:
		break;
	  case debounce_press:
		cli();
		/* delaySec(10); */
		sei();
		theState = wait_release;
		break;
	  case wait_release:
		break;
	  case debounce_release:
		cli();
		/* delaySec(10) */;
		sei();
		theState = wait_press;
		break;
	}
   }

   return(0);
}

ISR(INT0_vector) {
   if(theState == wait_press) {
	theState = debounce_press;	
   }
   else if (theState == wait_release) {
	theState = debounce_release;
   }
}
