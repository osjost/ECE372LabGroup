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

#define MAX_OCR_VAL 1023

/*
 * Define a set of states that can be used in the state machine using an enum.
 */

enum stateEnum {wait_press, debounce_press, wait_release, debounce_release};
volatile stateEnum state = wait_press; //Initialize the state to waiting for button press


//Tracks the current value displayed on the LEDS
unsigned int currentVal = 0;
unsigned int currentSpeedMultiplier = 1; //1 for normal, 2 for double speed

volatile float currentVoltage = 0;

int main(){
  //enable interrupts
  sei();
  //init needed components
  initSwitchPB3();
  initTimer0();
  initTimer1();
  initLCD();
  initPWMTimer3();
  initializeADC();

 while (1) {


  //outer state machine for button logic

  //Get current ADC voltage
  currentVoltage = (((ADCH << 8) | ADCL) / MAX_OCR_VAL) * 5.0;
  //HANDLE MOTOR CONTROL LOGIC
  if (currentVoltage < 2.5) {
    //Set desired motor speed
    changeDutyCycle((currentVoltage / 2.5) * MAX_OCR_VAL);
    //Set correct motor direction. IDK if forward corresponds to CW or CCW, we can flip later if wrong
    motorDirection(1);
  }
  else if (currentVoltage == 2.5) {
    //Turn motor off
    changeDutyCycle(0);
  }
  else {
    //Set desired motor speed
    changeDutyCycle(((currentVoltage - 2.5) / 2.5) * MAX_OCR_VAL);
    //Set correct motor direction. IDK if backward corresponds to CW or CCW, we can flip later if wrong
    motorDirection(2);
  }

//   switch(state) {
//     case wait_press:
//       break;
//     case debounce_press:
//       delayUs(1); //Adds delay to account for debounce period
//       state = wait_release;
//       break;
//     case wait_release: 
//      break;
//     case debounce_release: //Add delay to account for debounce period
//       delayUs(1);
//       state = wait_press;
//       break;
//   }
//  }
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/
// register that corresponds to port b, which is the port our button is on
// ISR(PCINT0_vect){ //On interrupt, advance state machine
//   if (state == wait_press){
//     state = debounce_press;
//   }
//   else if (state == wait_release) {
//     state = debounce_release;
//   }
}
