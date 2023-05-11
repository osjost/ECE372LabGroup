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
#include "switch.h"
#include "timer.h"
#include "lcd.h"
#include "pwm.h"
#include "adc.h"
#include "i2c.h"
#include "ad1115.h"
#include "buzzer.h"
#include <math.h>

#define MAX_OCR_VAL 1023
#define ALC_THRESHOLD 0.08
#define ADD1115_ADDY 0b1001000    //7bit address if ADDR pin is grounded  
#define CONVERSION_reg  0b00000000  //points to covnersion
#define CONFIG_reg  0b00000001    //points to config


/*
 * Define a set of states that can be used in the state machine using an enum.
 */

enum stateEnum {wait_press, debounce_press, wait_release, debounce_release};
volatile stateEnum state = wait_press; //Initialize the state to waiting for button press

double resultADC;
int sample_Average[20];

double sample_average ();

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
  writeString("Breathalyzer     ");

  Serial.begin(9600);
  Serial.println("Begin");
  double result = 0;

  while(true){
      resultADC = Read_from_16bit(ADD1115_ADDY, CONVERSION_reg);
      // Serial.println(resultADC);  //raw reading from the conversion register
      //result = ((double) resultADC / 32768) * 5;  //insert the conversion equation 
      Serial.println(resultADC);
      delayMs(1000);
    
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
      
      //Beep twice to indicate start heating up period (7 seconds)
      beep_beep();

      moveCursor(0,0);
      writeString("Heating up...  ");
      delayMs(5000); //wait 7s
  

      //clear display
      moveCursor(0,0);
      writeString("                                           ");
      moveCursor(1,0);
      writeString("                                           ");

      
      moveCursor(0,0);
      writeString("Hold for 5s    ");
      chirp();
    

      delayMs(5000);
      
      
      //clear display
      moveCursor(0,0);
      writeString("                                           ");
      moveCursor(1,0);
      writeString("                                           ");

      moveCursor(0,0);
      writeString("BAC: ");
      chirp();

      //sample 20 times and take average
      resultADC = sample_average();

      //Take sensor reading
      //resultADC = Read_from_16bit(ADD1115_ADDY, CONVERSION_reg);
      Serial.println("result average ADC");
      Serial.println(resultADC);  //raw reading from the conversion register
      //result = (resultADC / 32768) * 5;  //insert the conversion equation 
      //result = exp((result + 1.1401 )/ 0.888 ) / 10000.0; //BAC conversion
      //Serial.println("result");
      //Serial.println(result);


      
        
        // if (analogVoltage >= 3.8){
        //   ppm = 725 * analogVoltage - 2545;
        // }
        // else if (analogVoltage >= 3.25){
        //   ppm = 163.636 * analogVoltage - 412;
        // }
        // else if (analogVoltage >= 2.2){
        //   ppm = 66.6667 * analogVoltage - 97;
        // }
        // else if (analogVoltage >= 1.5){
        //   ppm = 71.4286 * analogVoltage - 107;
        // }
        // else {
        //   ppm = 0;
        // }
        // bac = ppm;
        
        // displayValue(bac);    // Display current digital result
        // delayS(1);            
      

      if (result > ALC_THRESHOLD) {
        String temp = String(result, 2);
        temp+= " NO!  ";
        Serial.println(temp);
        writeString(temp.c_str());
       }
      else {
        String temp = String(result, 2);
        temp+= " YES! ";
        Serial.println(temp);
        Serial.println(temp.c_str());
        writeString(temp.c_str());
        }
        
      

      state = wait_press;
      break;
  }
  }
}

double sample_average () {
  double ave = 0;

for (int i = 0; i< 20; i++) {

  ave  += Read_from_16bit(ADD1115_ADDY, CONVERSION_reg);
  delayUs(1);
}

double returnVal = ave / 20.0;

return (returnVal);
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
