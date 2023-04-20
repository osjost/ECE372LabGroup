#include "buzzer.h"
#include "pwm.h"
#include <util/delay.h>
#include "timer.h"

void chirp() {
    changeDutyCycle(1000);
    delayMs(500);
    changeDutyCycle(0);
}
