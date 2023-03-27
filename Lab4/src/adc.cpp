// Author: Thomas Gentrup
// Net ID:tgentrup
// Date: 3/26/23
// Assignment: Lab 4
//
// Description:  Set the ADC bits to initiate ADC sampling on free-running mode
//----------------------------------------------------------------------//

#include "ADC.h"
#include <avr/io.h>

void initializeADC(){
   //set voltage reference to 5V
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);

   //set to right adjusted
	ADMUX &= ~(1 << ADLAR);

   //set as single-ended input using ADC0 as input
	ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3) | (1 << MUX4) | (1 << MUX5));

   //set Auto-trigger with free-running fode
	ADCSRB &= ~((1 << ADTS0) | (1 << ADST1) | (1 << ADST2));

   //enable ADC and auto-triggering
	ADCSRBA |= ((1 << ADEN) | (1 << ADATE));

   //set ADC clock frequency using prescaler
   //borrowing Dr. Hetherington's prescaler from Digital_Voltmeter_ADC...
   //setting PS to 128, => sampling rate = 9615 KHz
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

   //disable ADC0 pin as digital input
	DIDR0 |= (1 << ADC0D);

   //start 1st conversion to start free running conversions
	ADCSRA |= (1 << ADSC);
}
