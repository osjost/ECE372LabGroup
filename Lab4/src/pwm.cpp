#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

void changeDutyCycle(float dutyCycle) { //Pass in the percent duty cycle you want. Must be between 0.0 and 1.0, 1.0 being the highest supported.
 OCR1A = dutyCycle * 1023;
}

void initPWMTimer1() {
    //Init timer to development pin 11
    DDRB |= (1 << DDB5);
    TCCR1A |= (1 << COM1A1)|(1 << WGM11)|(1 << WGM10);
    TCCR1B |= (1 << WGM12)|(1 << CS10);
    changeDutyCycle(.50);
}

