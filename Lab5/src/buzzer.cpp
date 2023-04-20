#include "buzzer.h"
#include "pwm.h"
#include <util/delay.h>
#include "timer.h"

void chirp() {
    changeDutyCycle(1000);
    delayMs(200);
    changeDutyCycle(0);
    delayMs(200);
}

void stop_chirp() {
    changeDutyCycle(0);
}
