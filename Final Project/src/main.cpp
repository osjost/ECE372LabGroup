// Author: Oliver Sjostrom
// Net ID: oliversjost
// Date: 3/16/2023
// Assignment:     Lab 3
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
// Requirements:
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/io.h>
#include "led.h"
#include "switch.h"
#include "timer.h"
#include "lcd.h"
#include "pwm.h"
#include "adc.h"
#include "motor.h"
#include "shiftRegister.h"

#define MAX_OCR_VAL 1023
#define ALC_THRESHOLD 99

/*
 * Define a set of states that can be used in the state machine using an enum.
 */

enum stateEnum {wait_press, debounce_press, wait_release, debounce_release};
volatile stateEnum state = wait_press; //Initialize the state to waiting for button press

int main() {
  sei();
  initializeADC();
  initPWMTimer3();
  initTimer0();
  initTimer1();
  initSwitchPB3();
  initLCD();
  moveCursor(0, 0); // moves the cursor to 0,0 position
  writeString("BAC");

  Serial.begin(9600);
  Serial.println("Begin");
  double result = 0;

  while(true){
    
  switch(state) {
    case wait_press:
    Serial.println("wait_press");
      break;
    case debounce_press:
      delayUs(1);
      Serial.println("debounce_press");
      state = wait_release;
      break;
    case wait_release: 
    Serial.println("wait_release");
     break;
    case debounce_release:
    Serial.println("debounce_release");
      delayUs(1);
      //Beep to indicate start reading
      setVolume(40);
      delayMs(300);
      setVolume(0);

      //Delay 3 seconds for person to breathe
      delayMs(3000);

      //Beep to indicate reading done
      setVolume(40);
      delayMs(300);
      setVolume(0);

      //Take sensor reading
      result = 999.9999;
      // result = ADCL;
      // result = (((unsigned int) ADCH) << 8) + result;
      // voltage = result * (5/1024.0);

      if (result > ALC_THRESHOLD) {
        String temp = String(result, 4);
        temp+= " NO!";
        moveCursor(1, 0);
        writeString(temp.c_str());
      }
      else {
        String temp = String(result, 4);
        temp+= " YES!";
        moveCursor(1, 0);
        writeString(temp.c_str());
      }

      state = wait_press;
      break;
  }
  }
}

  // register that corresponds to port b, which is the port our button is on
ISR(PCINT0_vect){ //On interrupt, advance state machine
  if (state == wait_press){
    state = debounce_press;
  }
  else if (state == wait_release) {
    state = debounce_release;
  }
}

