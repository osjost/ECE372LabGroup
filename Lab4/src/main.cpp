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
  initMotorPins();
  initSwitchPD0();

  Serial.begin(9600);
  unsigned int result = 0;
  double voltage = 0;
  int cycleVal = 0;
  while(1){
    turnOffDisplay();

  switch(state) {
    case wait_press:
        result = ADCL;
        result = (((unsigned int) ADCH) << 8) + result;
        voltage = result * (5/1024.0);
        Serial.println(voltage,2);

        if (voltage <= 2.5) {
          //Set desired motor speed
          cycleVal = (1 - (voltage / 2.5)) * MAX_OCR_VAL;
          Serial.println(cycleVal);
          changeDutyCycle(cycleVal);
          //Set correct motor direction. IDK if forward corresponds to CW or CCW, we can flip later if wrong
          motorDirection(1);
        }
        else {
          //Set desired motor speed
          cycleVal = ((voltage - 2.5) / 2.5) * MAX_OCR_VAL;
          Serial.println(cycleVal);
          changeDutyCycle(cycleVal);
          //Set correct motor direction. IDK if backward corresponds to CW or CCW, we can flip later if wrong
          motorDirection(2);
        }
      break;
    case debounce_press:
      delayUs(1); //Adds delay to account for debounce period
      Serial.println("debounce_press");
      state = wait_release;
      break;
    case wait_release: 
      Serial.println("wait_release");
     break;
    case debounce_release: //Add delay to account for debounce period
      Serial.println("debounce_release");
      delayUs(1);
     disableINT0Interrupt();
      state = wait_press;
      motorDirection(0);
     tenSecTimerCountdownDisplay();
      enableINT0Interrupt();
      break;
  }
  }
}

  // register that corresponds to port b, which is the port our button is on
ISR(INT0_vect){ //On interrupt, advance state machine
  
  if (state == wait_press){
    state = debounce_press;
  }
  else if (state == wait_release) {
    state = debounce_release;
  }
  else if( state == debounce_press || debounce_release) {
    return;
  }
}

