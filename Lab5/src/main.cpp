// Author: Oliver Sjostrom
// Net ID: oliversjost
// Date: 4/27/2023
// Assignment:     Lab 5
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
#include "i2c.h"
#include "accelerometer.h"


#define MAX_OCR_VAL 1023

#define X_THRESHOLD_VAL //to do
#define Y_THRESHOLD_VAL //to do

#define Z_THRESHOLD_VAL //to do
#define Y_GYRO_THRESHOLD_VAL //to do
#define Z_GYRO_THRESHOLD_VAL //to do

//accelerometer data
#define MPU_XOUT_H 0x3B
#define MPU_XOUT_L 0x3C
#define MPU_YOUT_H 0x3D
#define MPU_YOUT_L 0x3E
#define MPU_ZOUT_H 0x3F
#define MPU_ZOUT_L 0x40
#define MPU_WHO_AM_I 0x68        //I2C addy
#define MPU_PWR_MANAGEMENT_1_REG 0x6B  
#define MPU_PWR_CONFIG  0x09 //sets internal accel clock to X-gyro for improved stability (datasheet recommended) 
                             //disables temp sensor (not needed for this lab)
                             //wakes up device
#define MPU_PWR_RESET 0x70  //resets all registers in accel to defaults
#define MPU_PWR_MANAGEMENT_2 0x6C


//gyro data
#define MPU_GYRO_XOUT_H 0x43
#define MPU_GYRO_XOUT_L 0x44
#define MPU_GYRO_YOUT_H 0x45
#define MPU_GYRO_YOUT_L 0x46
#define MPU_GYRO_ZOUT_H 0x47
#define MPU_GYRO_ZOUT_L 0x48


/*
 * Define a set of states that can be used in the state machine using an enum.
 */

enum stateEnum {wait_press, debounce_press, wait_release, debounce_release};
volatile stateEnum state = wait_press; //Initialize the state to waiting for button press

enum accelerometerEnum{above_threshold, below_threshold}; //tracks the current accelerometer state
accelerometerEnum accelerometerState = below_threshold;

volatile bool chirpOn = false;

signed int x = 0;
signed int y = 0;
signed int z = 0;
signed int yGyro = 0;
signed int zGyro = 0;
signed int xGyro = 0;

bool reEnable = true;

int main() {
  //Initilizations. Remember to add the missing ones later!
  Serial.begin(9600);
  sei();
  initTimer1();
  SPI_MASTER_Init();
  display_init();
  initPWMTimer3();
  initSwitchPB3();
  initI2C();
  initAccelerometer();

  Serial.println("Start");

  while(true) {
    StartI2C_Trans(MPU_WHO_AM_I); //establish accel slave i2c address
    Read_from(MPU_WHO_AM_I, MPU_XOUT_L);
    x = Read_data();
    Read_from(MPU_WHO_AM_I, MPU_XOUT_H);
    x = (Read_data() << 8 | x);

    Read_from(MPU_WHO_AM_I, MPU_YOUT_L);
    y = Read_data();
    Read_from(MPU_WHO_AM_I, MPU_YOUT_H);
    y = (Read_data() << 8 | y);

    Read_from(MPU_WHO_AM_I, MPU_ZOUT_L);
    z = Read_data();
    Read_from(MPU_WHO_AM_I, MPU_ZOUT_H);
    z = (Read_data() << 8 | z);

    Read_from(MPU_WHO_AM_I, MPU_GYRO_XOUT_L);
    signed int a = Read_data();
    Read_from(MPU_WHO_AM_I, MPU_GYRO_XOUT_H);
    xGyro = (Read_data() << 8 | a);

    Read_from(MPU_WHO_AM_I, MPU_GYRO_YOUT_L);
    signed int b = Read_data();
    Read_from(MPU_WHO_AM_I, MPU_GYRO_YOUT_H);
    yGyro = (Read_data() << 8 | b);

    Read_from(MPU_WHO_AM_I, MPU_GYRO_ZOUT_L);
    signed int c = Read_data();
    Read_from(MPU_WHO_AM_I, MPU_GYRO_ZOUT_H);
    zGyro = (Read_data() << 8 | c);

    //print accelerometer values
    Serial.println("X: " + String(x) + " Y: " + String(y) + " Z:" + String(z));

    //Check threshold conditions
    if (!(z > 5000)) { //add more thresholds later once we expierment a bit, just || stuff
      accelerometerState = above_threshold;
    }
    else{
      reEnable = true;
      accelerometerState = below_threshold;
    }
    //State machines for switch and accelerometer
    switch(state) {
      case wait_press:
        if(chirpOn) {
          setVolume(40);
          delayMs(40);
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
        setVolume(0);
        delayMs(40);
        state = wait_press;
        break;
    }

    switch (accelerometerState) {
      case above_threshold:
        if (reEnable) {
          chirpOn = true;
        }
        write_frowney();
        reEnable = false;
        break;
      case below_threshold:
        reEnable = true;
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