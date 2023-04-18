#include "i2c.h"
#include <avr/io.h>
#include "Arduino.h"

#define wait_for_completion while(!(TWCR & (1 << TWINT)));
#define WRITE_BIT 0;
#define READ_BIT 1;

void initI2C() {
  PRR0 &= ~(1 << PRTWI);
  TWSR &= ~((1 << TWPS0) | (1 << TWPS1)); //set prescaler to 1
  
  TWBR = 8; //setting TWBR to 8 to satisfy SCL freq requirement

  TWCR |= (1 << TWINT) | (1 << TWEN);
}

void StartI2C_Trans(unsigned char SLA) {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); //clear TWINT, start condition, and enable
  wait_for_completion;
  TWDR = (SLA << 1); //write bit is 0, so shift left sets write bit to 0
  TWCR = (1 << TWINT) | (1 << TWEN); // clear falg and enable TWI
  wait_for_completion;
}

void StopI2C_Trans() {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

Read_from(unsigned char SLA, unsigned char MEMADDRESS) {
  StartI2C_Trans(SLA);

  TWDR = MEMADDRESS;
  TWCR = (1 << TWINT) | (1 << TWEN);
  wait_for_completion;

  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  wait_for_completion;

  TWDR = (1 << TWINT) | 0x01;
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
  wait_for_completion;
  TWCR = (1 << TWINT) | (1 << TWEN);
  wait_for_completion;
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

  
}

unsigned char Read_data() {
  return(TWDR);
}
