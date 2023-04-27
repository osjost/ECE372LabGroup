// Author: Oliver Sjostrom
// Net ID: oliversjost
// Date: 4/27/2023
// Assignment:     Lab 5

#include "PWM.h"
#include "timer.h"

//Initialize timer for PWM
void initPWMTimer3()
{
    //set header pin 5 to output
    DDRE |= (1 << DDE3);
    TCCR3A |= (1 << COM3A1) | (1 << WGM31) | (1 << WGM30);
    TCCR3A &= ~(1 << COM3A0);
    TCCR3B |= (1 << WGM32);
    TCCR3B &= ~(1 << WGM33);
    TCCR3B |= (1 << CS31);
    TCCR3B &= ~((1 << CS32) | (1 << CS30));
    OCR3A = 0;
}

//Set volume level, 0 for no sound
void setVolume(int result)
{
    OCR3A = result;
}