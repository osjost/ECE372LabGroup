// Authors: Oliver Sjostrom
// Net ID: oliversjost
// Date: 5/10/2023
// Assignment: Final Project

#include "buzzer.h"
#include "pwm.h"
#include <util/delay.h>
#include "timer.h"

void chirp() {
    setVolume(100);
    delayMs(100);
    setVolume(0);
}

void stop_chirp() {
    //changeDutyCycle(0);
}
void beep_beep() {
    setVolume(100);
    delayMs(50);
    setVolume(0);

    delayMs(100);

    setVolume(100);
    delayMs(50);
    setVolume(0);

}