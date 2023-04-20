#include "buzzer.h"
#include "pwm.h"
#include <util/delay.h>
#include "timer.h"

void chirp() {
    changeDutyCycle(1023);
}

void stop_chirp() {
    changeDutyCycle(0);
}
