#include "accelerometer.h"
#include "i2c.h"
#include <avr/io.h>

#define wait_for_completion while(!(TWCR & (1 << TWINT)));
#define WRITE_BIT 0;
#define READ_BIT 1;

//accelerometer data
#define MPU_XOUT_H 0x3B
#define MPU_XOUT_L 0x3C
#define MPU_YOUT_H 0x3D
#define MPU_YOUT_L 0x3E
#define MPU_ZOUT_H 0x3F
#define MPU_ZOUT_L 0x40
#define MPU_WHO_AM_I 0x75        //I2C addy
#define MPU_PWR_MANAGEMENT_1_REG 0x6B  
#define MPU_PWR_CONFIG  0x09 //sets internal accel clock to X-gyro for improved stability (datasheet recommended) 
                             //disables temp sensor (not needed for this lab)
                             //wakes up device
#define MPU_PWR_RESET 0x70  //resets all registers in accel to defaults
#define MPU_PWR_MANAGEMENT_2 0x6C

void initAccelerometer() {

    StartI2C_Trans(MPU_WHO_AM_I);

    write(MPU_PWR_MANAGEMENT_1_REG);

    write(MPU_PWR_CONFIG);


}

void write(unsigned char data){
  // this function loads the data passed into the I2C data register and transmits
  TWDR = data; //load data into TWDR register
  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action:  clear flag and enable TWI
  wait_for_completion;
}
