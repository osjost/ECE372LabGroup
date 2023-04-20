#include "i2c.h"
#include <avr/io.h>
#include "Arduino.h"

#define wait_for_completion while(!(TWCR & (1 << TWINT)));
#define WRITE_BIT 0;
#define READ_BIT 1;

void initI2C() {
  PRR0 &= ~(1 << PRTWI);
  TWSR &= ~((1 << TWPS0) | (1 << TWPS1)); //set prescaler to 1
  
  TWBR = 3; //setting TWBR to 3 to satisfy SCL freq requirement

  TWCR |= (1 << TWINT) | (1 << TWEN);
}

void StartI2C_Trans(unsigned char SLA) {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); //clear TWINT, start condition, and enable
  wait_for_completion;
   

  TWDR = (SLA << 1); //write bit is 0, so shift left sets write bit to 0
  TWCR = (1 << TWINT) | (1 << TWEN); // clear flag and enable TWI
  wait_for_completion;
 
}

void StopI2C_Trans() {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void Read_from(unsigned char SLA, unsigned char MEMADDRESS) {
  //StartI2C_Trans(SLA);

   signed int x=1;
    write(MEMADDRESS); //nternal mem address
  

  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); //restart for read mode
  wait_for_completion;
//   Serial.println(TWSR & (0xF8),HEX); // print out status register to verify transmission
//   Serial.flush();

  TWDR = (SLA << 1) | READ_BIT;         //7bit addy with read bit
  TWCR = (1 << TWINT) | (1 << TWEN);
  wait_for_completion;
//   Serial.println(TWSR & (0xF8),HEX); // print out status register to verify transmission
//   Serial.flush();



  TWCR = (1 << TWINT) | (1 << TWEN) | (1<< TWEA); // trigger with master sending ack
  wait_for_completion;
  x = TWDR;     //lower bit for x
  TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// trigger with master sending ack
  wait_for_completion;
  x = (TWDR << 8) | x;  // high byte for x axis

  Serial.println("x = ");
  Serial.print(x);



//   TWCR = (1 << TWINT) | (1 << TWEN) | (1<<TWEA); // trigger with master sending ack
//   wait_for_completion;
//   y = TWDR;     //lower bit for y
//   TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// trigger with master sending ack
//   wait_for_completion;
//   y = (TWDR << 8) | y;  // high byte for y axis

//   Serial.println("y = ");
//   Serial.print(y);

  


//   TWCR = (1 << TWINT) | (1 << TWEN) | (1<<TWEA); // trigger with master sending ack
//   wait_for_completion;
//   z = TWDR;     //lower bit for z
//   TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// trigger with master sending ack
//   wait_for_completion;
//   z = (TWDR << 8) | z;  // high byte for z axis

//   Serial.println("z = ");
//   Serial.print(z);


//    Serial.println(TWSR & (0xF8),HEX); // print out status register to verify transmission
//    Serial.flush();

}

void write(unsigned char data){
  // this function loads the data passed into the I2C data register and transmits
  TWDR = data; //load data into TWDR register
  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action:  clear flag and enable TWI
  wait_for_completion;
}

unsigned char Read_data() {
  return(TWDR);
}
