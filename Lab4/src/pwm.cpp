#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
#include <Arduino.h>

void changeDutyCycle(unsigned char ADChigh, unsigned char ADClow) { //Pass in the high and low adc registers
    OCR3A = (ADChigh << 8) | ADClow; // Calculate OCR3A based on PWM period. Hardcoded for now, later change to use ADC registers
}

void initPWMTimer3() {
    //Timer 3A, on pin 5 on the board
    DDRE |= (1 << DDE3);
    TCCR3A |= (1 << COM3A1)|(1 << WGM31)|(1 << WGM30);
    TCCR3B |= (1 << WGM32)|(1 << CS30);
    changeDutyCycle(0x00 , 0x00); // default PWM to off
}