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

#define X_THRESHOLD_VAL //to do
#define Y_THRESHOLD_VAL //to do
#define Z_THRESHOLD_VAL //to do
#define Y_GYRO_THRESHOLD_VAL //to do
#define Z_GYRO_THRESHOLD_VAL //to do


/*
 * Define a set of states that can be used in the state machine using an enum.
 */

enum stateEnum {wait_press, debounce_press, wait_release, debounce_release};
volatile stateEnum state = wait_press; //Initialize the state to waiting for button press

enum accelerometerEnum{above_threshold, below_threshold}; //tracks the current accelerometer state
accelerometerEnum accelerometerState = below_threshold;

volatile bool chirpOn = false;

int x = 0;
int y = 0;
int z = 0;
int yGyro = 0;
int zGyro = 0;

int main() {
  //Initilizations. Remember to add the missing ones later!
  Serial.begin(9600);
  sei();
  initTimer1();
  SPI_MASTER_Init();
  display_init();
  initPWMTimer3();
  initSwitchPB3();

  while(true) {
    x = 30; //update accelerometer position. add the function calls later
    y = 30;
    z = 30;
    yGyro = 30;
    zGyro = 30;
    //print accelerometer values
    Serial.println("X: " + String(x) + " Y: " + String(y) + " Z:" + String(z));

    //Check threshold conditions
    if (x > 31) { //add more thresholds later once we expierment a bit, just || stuff
      chirpOn = true;
      accelerometerState = above_threshold;
    }
    else {
      accelerometerState = below_threshold;
    }
    //State machines for switch and accelerometer
    switch(state) {
      case wait_press:
        if(chirpOn) {
          chirp(); //hangs here forever, is a chirp on then off?
        }
        break;
      case debounce_press:
        delayMs(1); //Adds delay to account for debounce period
        state = wait_release;
        break;
      case wait_release: 
      break;
      case debounce_release: //Add delay to account for debounce period
        delayMs(1);
        state = wait_press;
        stop_chirp();
        Serial.println("here");
        break;
    }

    switch (accelerometerState) {
      case above_threshold:
        write_frowney();
        break;
      case below_threshold:
        write_smiley();
        break;
    }
  }
}

ISR(PCINT0_vect){ //On interrupt, advance state machine
  if (state == wait_press){
    chirpOn = false;
    state = debounce_press;
  }
  else if (state == wait_release) {
    state = debounce_release;
  }
}