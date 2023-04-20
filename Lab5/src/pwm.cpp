#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
#include <Arduino.h>

void changeDutyCycle(unsigned int OCRval) { //Pass in the desired OCR value. 1023 for max output, 0 for no output.
    OCR3A = OCRval; // Calculate OCR3A based on PWM period. Hardcoded for now, later change to use ADC registers
}

void initPWMTimer3() {
    //Timer 3A, on pin 5 on the board
    DDRE |= (1 << DDE3);
    TCCR3A |= (1 << COM3A1)|(1 << WGM31)|(1 << WGM30);
    TCCR3B |= (1 << WGM32)|(1 << CS30);
    changeDutyCycle(0); // default PWM to off
}