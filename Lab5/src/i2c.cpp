#include "i2c.h"
#include <avr/io.h>
#include "Arduino.h"

#define wait_for_completion while(!(TWCR & (1 << TWINT)));


void initI2C() {
  PRR0 &= ~(1<<PRTWI);  // init I2C module
  TWSR |= (1 << TWPS0);  // prescaler = 1
  TWSR &= ~(1 << TWPS1); // prescaler = 1 
  TWBR = 0xC6; // bit rate generator = 10k  (TWBR = 198)
  TWCR |= (1 << TWINT )| (1 << TWEN); // enable two wire interface
}

 
void StartI2C_Trans(unsigned char SLA) {
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // Clear TWINT, initiate start condition, initiate enable
  wait_for_completion;
  TWDR = (SLA << 1); // Set two wire data register to the SLA + write bit
  TWCR = (1<<TWINT)|(1<<TWEN);  // Trigger action: Clear TWINT and initiate enable
  wait_for_completion;
}

void StopI2C_Trans() {
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //trigger action+stop condition
}

void write(unsigned char data){
  TWDR = data; //Set two wire data register equal to incoming data
  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action
  wait_for_completion;
}

void Read_from(unsigned char SLA, unsigned char MEMADDRESS){
  StartI2C_Trans(SLA);
  //Serial.println("Start Finish");

  write(MEMADDRESS);  
  //Serial.println("Write Finish");

  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // Clear TWINT, initiate start condition, initiate enable
  wait_for_completion;
  //Serial.println("Clear Finish");
  TWDR = (SLA << 1) | 0x01; // Set two wire data register to the SLA + read bit
  TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// Trigger action + master acknowledge bit
  wait_for_completion;
  //Serial.println("Set Finish");

  TWCR = (1<< TWINT) | (1 << TWEN);  // Trigger action
  wait_for_completion;
  //Serial.println("Trigger Finish");

  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Trigger action + stop condition
  //Serial.println("Read Finish");
}

  
unsigned char Read_data() 
{
  return TWDR;
}
