
// Authors: Nabor Rios
// Net ID: riosnabor
// Date: 5/10/2023
// Assignment: Final Project
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
// Requirements:
//----------------------------------------------------------------------//
#include "ad1115.h"
#include <avr/io.h>
#include <Arduino.h>
#include "i2c.h"

#define wait_for_completion while(!(TWCR & (1 << TWINT)));
#define WRITE_BIT 0;
#define READ_BIT 1;

#define ADD1115_ADDY 0b1001000


void initAD1115() {         //sets the module to continous conversion mode

    StartI2C_Trans(ADD1115_ADDY);

    write(0b00000001);      //points to the config register of module

    write(0b10000100); //msbyte of config register to be written. 

    write(0b10000011); //lsbyte of config register to be written

}