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
/*
#include <Arduino.h>
#include <avr/io.h>

#include "timer.h"
#include "lcd.h"
#include "pwm.h"
#include "adc.h"

#include "spi.h"
#include "buzzer.h"
#include "i2c.h"

#include "ad1115.h"


//adds1115 has a four different slave address
//connect addr pin to GND for a slave addy of 1001000



#define ADD1115_ADDY 0b1001000    //7bit address if ADDR pin is grounded  
#define CONVERSION_reg  0b00000000  //points to covnersion
#define CONFIG_reg  0b00000001    //points to config



int main() {

  Serial.begin(9600);
 
  sei();

  initTimer1();
  initPWMTimer3();
  initI2C();

  Serial.print("init done\n");

  initAD1115();     //sets up module into continous conversion mode

  // DDRF &= ~(1<<PF0);
  // PORTF |= (1<<PF0);

 

//  int conversion_data_H;
//  int conversion_data_L;
//  int result;
  //float analog_in;
  
  while (true) {


    //Read_from(ADD1115_ADDY, CONVERSION_reg);


    //conversion_data_H = Read_data();

    Read_from_16bit(ADD1115_ADDY, CONVERSION_reg);

    //conversion_data_L = Read_data();

    //analog_in = analogRead(A0);

    
     //result = (conversion_data_H << 8 | conversion_data_L);

    //Serial.println(result);



    

  }

}

*/
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
#include "i2c.h"
#include "ad1115.h"

#define MAX_OCR_VAL 1023
#define ALC_THRESHOLD 99
#define ADD1115_ADDY 0b1001000    //7bit address if ADDR pin is grounded  
#define CONVERSION_reg  0b00000000  //points to covnersion
#define CONFIG_reg  0b00000001    //points to config


/*
 * Define a set of states that can be used in the state machine using an enum.
 */

enum stateEnum {wait_press, debounce_press, wait_release, debounce_release};
volatile stateEnum state = wait_press; //Initialize the state to waiting for button press

int resultADC;

int main() {
  sei();
  initializeADC();
  initPWMTimer3();
  initTimer0();
  initTimer1();
  initSwitchPB3();
  initLCD();
  initI2C();
  initAD1115();
  
  moveCursor(0, 0); // moves the cursor to 0,0 position
  writeString("BAC");

  Serial.begin(9600);
  Serial.println("Begin");
  double result = 0;

  while(true){
    
  switch(state) {
    case wait_press:
      break;
    case debounce_press:
      delayUs(1);
      state = wait_release;
      break;
    case wait_release: 
     break;
    case debounce_release:
      delayUs(1);
      //Beep to indicate start reading
      setVolume(40);
      delayMs(300);
      setVolume(0);

      //Delay 3 seconds for person to breathe
      delayMs(7000);

      //Beep to indicate reading done
      setVolume(40);
      delayMs(300);
      setVolume(0);

      //Take sensor reading
      resultADC = Read_from_16bit(ADD1115_ADDY, CONVERSION_reg);
      Serial.println(resultADC);
      result = ((double) resultADC / 32768) * 5;
      Serial.println(result);

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

