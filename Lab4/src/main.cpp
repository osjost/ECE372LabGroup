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

#define SHORT_DELAY 100

/*
 * Define a set of states that can be used in the state machine using an enum.
 */

enum stateEnum {wait_press, debounce_press, wait_release, debounce_release};
volatile stateEnum state = wait_press; //Initialize the state to waiting for button press


//Tracks the current value displayed on the LEDS
unsigned int currentVal = 0;
unsigned int currentSpeedMultiplier = 1; //1 for normal, 2 for double speed

int main(){
  //enable interrupts
  sei();

  //init used pins
  initLED();
  initSwitchPB3();
  initTimer0();
  initTimer1();
  initLCD();
  moveCursor(0,0);
  writeString("Blinking rate =");
  moveCursor(1,0);
  writeString("Fast");

  //init and draw the custom characters
  addCustomCharacters();
  moveCursor(1,5);
  writeCharacter(0);
  moveCursor(1,6);
  writeCharacter(1);
  moveCursor(1,7);
  writeCharacter(2);
  
  /*
  * Implement a state machine in the while loop which achieves the assignment
  * requirements.
  */
 while (1) {
  switch(state) {
    case wait_press:
      break;
    case debounce_press:
      delayUs(1); //Adds delay to account for debounce period
      state = wait_release;
      break;
    case wait_release: 
     break;
    case debounce_release: //Add delay to account for debounce period
      delayUs(1);

      if (currentSpeedMultiplier == 1) { //Actual control logic for LED speed
        currentSpeedMultiplier = 2;
          moveCursor(1,0);
          writeString("Slow");
          moveCursor(1,7);
      } 
      else{
        currentSpeedMultiplier = 1;
          moveCursor(1,0);
          writeString("Fast");
      }
      state = wait_press;
      break;
  }
  //Logic for incrementing and toggling LEDS
  if (currentVal <= 15) {
    currentVal++;
  }
  else {
    currentVal = 0;
  }
  turnOnLEDWithChar((char)currentVal);
  delayMs(SHORT_DELAY * currentSpeedMultiplier);
 }
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/
// register that corresponds to port b, which is the port our button is on
ISR(PCINT0_vect){ //On interrupt, advance state machine
  if (state == wait_press){
    state = debounce_press;
  }
  else if (state == wait_release) {
    state = debounce_release;
  }
}