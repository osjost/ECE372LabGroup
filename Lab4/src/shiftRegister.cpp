#include "shiftregister.h"
#include <avr/io.h>
#include <Arduino.h>
#include "timer.h"

void initShiftRegister() {

    //header 8 h5, 9 h6, 10 b4
    //data latch clock respectively
    
    DDRH |= (1<<DDH5) | (1<<DDH6);
    DDRB |= (1<<DDB4);
}

void shiftRegister(unsigned int num) {

    PORTH &= ~(1<<PORTH6); //latch low to receive data

    shiftOut(8, 10, LSBFIRST, num);

    PORTH |= (1<<PORTH6); //display
}

void tenSecTimerCountdownDisplay() {
    //use shift register and timer0 to count down from 9 to 0 and display on 7 segment display.
    int datArray[16] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 246, 238, 62, 156, 122, 158, 142};

    for(int i = 9; i>=0; i--) {

    PORTH &= ~(1<<PORTH6); //latch low to receive data
    shiftOut(8, 10, LSBFIRST, datArray[i]); //find correct number
    PORTH |= (1<<PORTH6); //display
    
    delayMs(1000);

    }

}
