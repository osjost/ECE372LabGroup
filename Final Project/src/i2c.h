#ifndef I2C_H
#define I2C_H

void initI2C();

void StartI2C_Trans(unsigned char SLA);

void StopI2C_Trans();

void Read_from(unsigned char SLA, unsigned char MEMADDRESS);

int Read_from_16bit(unsigned char SLA, unsigned char MEMADDRESS);

unsigned char Read_data();

void write(unsigned char);

#endif
