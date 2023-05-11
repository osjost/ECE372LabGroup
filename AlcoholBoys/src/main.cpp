
// Authors: Oliver Sjostrom, Tom Gentrup, Nabor Rios
// Net ID: oliversjost , tgentrup, riosnabor
// Date: 5/10/2023
// Assignment: Final Project
//Description: Code for joining an MQ3 Gas sensor to detect the alcohol content of a user. Gas sensor will output an analog 
//             voltage to a ADS1115 16-bit conversion register which will convert it to a 16bit digital value. The ADS115 utilizes
//             I2C communication with the Arduino SCL/SDA ports to receive the digital values. Digital values are averaged,
//             and an equation we formed from our knowledge of BAC, the sensor datasheet, and unit conversion, displays
//             the measured BAC onto an LCD. This breathalyzer can accurately detect whether or not user has had alcohol, and a
//             reasonable estimate of BAC.

#include <Arduino.h>
#include <avr/io.h>
#include "switch.h"
#include "timer.h"
#include "lcd.h"
#include "pwm.h"
#include "i2c.h"
#include "ad1115.h"
#include "buzzer.h"
#include <math.h>

#define MAX_OCR_VAL 1023
#define ALC_THRESHOLD 0.08      //legal limit in most states (AZ is zero tolerance)
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
  initPWMTimer3();
  initTimer0();
  initTimer1();
  initSwitchPB3();
  initLCD();
  initI2C();
  initAD1115();
  
  moveCursor(0, 0); // moves the cursor to 0,0 position
  writeString("Breathalyzer     ");

  //print statements for debugging purposes
  //Serial.begin(9600);
  //Serial.println("Begin");
  double result = 0;

  while(true){
      resultADC = Read_from_16bit(ADD1115_ADDY, CONVERSION_reg);
      // Serial.println(resultADC);  //raw reading from the conversion register
      Serial.println(resultADC);
      //delay for debugging, making output human readable
      //delayMs(1000);
    
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
      delayMs(5000); //wait 5s
  

      //clear display
      moveCursor(0,0);
      writeString("                                           ");
      moveCursor(1,0);
      writeString("                                           ");

      
      moveCursor(0,0);
      writeString("Hold for 5s    ");
      chirp();
    
      //five second delay to allow to blow into sensor
      delayMs(5000);
      
      
      //clear display
      moveCursor(0,0);
      writeString("                                           ");
      moveCursor(1,0);
      writeString("                                           ");

      moveCursor(0,0);
      writeString("BAC: ");
      chirp();

      //next 20 times samples are recorded and take average, store in result adc
      resultADC = sample_average();

      //Take sensor reading
      //Serial.println("result average ADC");
      //Serial.println(resultADC);  //raw reading from the conversion register


      result = (resultADC / 32768) * 5;  //insert the conversion equation 
      result = exp((result + 1.1401 )/ 0.888 ) / 10000.0; //BAC conversion
      //Serial.println("result");
      //Serial.println(result);

            

      if (resultADC > ALC_THRESHOLD) {      //display the result from the conversion and push to LCD
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

//takes 20 reads from the ADS component and returns average
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