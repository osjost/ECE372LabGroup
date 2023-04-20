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
#include "spi.h"
#include "buzzer.h"

#define MAX_OCR_VAL 1023

/*
 * Define a set of states that can be used in the state machine using an enum.
 */

enum stateEnum {wait_press, debounce_press, wait_release, debounce_release};
volatile stateEnum state = wait_press; //Initialize the state to waiting for button press

volatile bool chirpOn = true; //0 f

int main() {
  Serial.begin(9600);
  sei();
  initTimer0();
  SPI_MASTER_Init();
  display_init();
  initPWMTimer3();
  initSwitchPB3();

  while(true) {
    while(chirpOn) {
      chirp();
    }
  }
}

ISR(PCINT0_vect){ //On interrupt, advance state machine
  chirpOn = false;
}