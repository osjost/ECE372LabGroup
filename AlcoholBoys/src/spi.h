#ifndef SPI_H
#define SPI_H

void SPI_MASTER_Init();
void write_execute(unsigned char CMD, unsigned char data);
void write_smiley();
void write_frowney();
void display_init();

#endif