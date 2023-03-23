// Author: Oliver Sjostrom
// Net ID: oliversjost
// Date: 3/16/2023
// Assignment:     Lab 3
//----------------------------------------------------------------------//
#include "lcd.h"
#include "timer.h"
#include <util/delay.h>

/*
 * Initializes all pins related to the LCD to be outputs
 */
void initLCDPins(){
  //Initilize data pins
  DDRA |= (1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3);
  //Intialize enable and RS pin
  DDRB |= (1 << DDB4) | (1 << DDB6);
}

// Library for custom display characters

//Used https://omerk.github.io/lcdchargen/ to generate maps
unsigned char smileyFace[8] = {
	0b11011,
	0b11011,
	0b00000,
	0b10001,
	0b10001,
	0b01110,
	0b00000,
	0b00000
};

unsigned char frownFace[8] = {
	0b11011,
	0b11011,
	0b00000,
	0b00000,
	0b01110,
	0b10001,
	0b00000,
	0b00000
};

unsigned char star[8] = {
	0b00100,
	0b01110,
	0b01110,
	0b11111,
	0b01110,
	0b01110,
	0b00100,
	0b00000
};

/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *      a. data is an unsigned char which has 8 bits. Therefore, you
 *         need assign the bottom 4 bits of "data" to the appropriate bits in
 *         PORTA
 *  2. This is a "command" signal, meaning RS should be Low
 *  3. Assert high on enable pin, delay, and asset low on enable pin
 *  4. delay the provided number in MICROseconds.
 */

/*
Bit mask with Portn (bottom 4 bits of data assigned to portn [3:0])
Set RS pin to low
Set enable to high
Delay 1 us
Set enable to low
Delay speciified amount so driver can interface with LCD
*/
//Taken from lecture notes 2-21-23
void fourBitCommandWithDelay(unsigned char data, unsigned int delay){
  PORTA = (PORTA & 0xF0) | (data & 0x0F);
   //Set RS pin to low
  PORTB &= ~(1<<PB6);
  PORTB |= (1<<PB4); //EnabPORTB &= ~(1<<PB6)le pin to high
  delayUs(1);
  PORTB &= ~(1<<PB4);//Enable pin to low
  delayUs(delay);
}
/* Similar to fourBitCommandWithDelay except that now all eight bits of command are
 * sent.
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *       a. unlike fourBitCommandWithDelay, you need to send the TOP four bits of
 *          "command" first. These should be assigned to appropriate bits in PORTA
 * 2. This is a command signal, meaning RS should be low
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of command to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay the provided number in MICROseconds.
 */
//Taken from lecture notes 2-21-23
void eightBitCommandWithDelay(unsigned char command, unsigned int delay){
 //Bit mask upper bits
 PORTA = (PORTA & 0xF0) | (command >> 4 & 0x0F);
 //RS pin low
 PORTB &= ~(1<<PB6);
 //Enable pin high
 PORTB |= (1<<PB4);
 //delay
 delayUs(1);
 //Enable low
 PORTB &= ~(1<<PB4);
 //Put lower y bits in port A
 PORTA = (PORTA & 0xF0) | (command & 0x0F);
 //Enable pin high
 PORTB |= (1 << PB4);
 //Delay
 delayUs(1);
 //Enable pin low
 PORTB &= ~(1<<PB4);
 //Execute delay
 delayUs(delay);
}

/* Similar to eightBitCommandWithDelay except that now RS should be high
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 * 2. This is a "data" signal, meaning RS should be high
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of character to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay is always 46 MICROseconds for a character write
 */
void writeCharacter(unsigned char character){
  //Write RS to high
  PORTB |= (1 << PORTB6);
  //Write top 4 bits
  PORTA = (PORTA & 0xf0) | (character >> 4 & 0x0f);
  //Assert high enable
  PORTB |= (1 << PORTB4);
  delayUs(1);
  //Deassert high enable
  PORTB &= ~(1 << PORTB4);
  //Write bottom 4 bits
  PORTA = (PORTA & 0xf0) | (character & 0x0f);
  //Assert high enable
  PORTB |= (1 << PORTB4);
  delayUs(1);
  //Deassert high enable
  PORTB &= ~(1 << PORTB4);
  delayUs(46);
}




/*
 * Writes a provided string such as "Hello!" to the LCD screen. You should
 * remember that a c string always ends with the '\0' character and
 * that this should just call writeCharacter multiple times.
 */
//Taken from lecture 15
void writeString(const char *string){
  while (*string != '\0') {
    writeCharacter(*string);
    string++;
  }
}

/*
 * This moves the LCD cursor to a specific place on the screen.
 * This can be done using the eightBitCommandWithDelay with correct arguments
 */
//From lecture 15 slide 102
void moveCursor(unsigned char x, unsigned char y){
	char data = 0x80 | (x << 6) | y;
  eightBitCommandWithDelay(data, 46);
}


/* This is the procedure outline on the LCD datasheet page 4 out of 9.
 * This should be the last function you write as it largely depends on all other
 * functions working.
 */
//Commands from LCD command website given in lecture links
void initLCDProcedure(){
  // Delay 15 milliseconds
  delayMs(15);
  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
  fourBitCommandWithDelay(0b0011, 4100);
  // Write 0b0011 to DB[7:4] and delay 100 microseconds
  fourBitCommandWithDelay(0b0011, 100);

  // The data sheet does not make this clear, but at this point you are issuing
  // commands in two sets of four bits. You must delay after each command
  // (which is the second set of four bits) an amount specified on page 3 of
  // the data sheet.
  // write 0b0011 to DB[7:4] and 100us delay
  fourBitCommandWithDelay(0b0011, 100);

  // write 0b0010 to DB[7:4] and 100us delay.
  fourBitCommandWithDelay(0b0010, 100);

  // Function set in the command table with 53us delay
  eightBitCommandWithDelay(0b00101000,53);

  // Display off in the command table with 53us delay
   eightBitCommandWithDelay(0b00001000,53);

  // Clear display in the command table. Remember the delay is longer!!!
  eightBitCommandWithDelay(0b00000001, 200);

  // Entry Mode Set in the command table.
  eightBitCommandWithDelay(0b00000110, 53);

  // Display ON/OFF Control in the command table. (Yes, this is not specified),
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)
  eightBitCommandWithDelay(0b00001101, 53);

}

/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}

//Writes the custom character into ram
//Pass in array pointer to unsigned char array of bit map
void createCustomCharacter(unsigned char* customCharacterIn) {
  for (unsigned int i = 0; i < 8; i++) {
    writeCharacter(customCharacterIn[i]);
  }
}

//Function called to add all custom characters into ram. With chip limitations maximum of 8 possible
void addCustomCharacters(){
  //Set write commands to write to GCRAM
  eightBitCommandWithDelay(0b01000000,53); 
  //Write RS high for subequent write commands
  //Call write custom character for each added character
  createCustomCharacter(smileyFace);
  createCustomCharacter(frownFace);
  createCustomCharacter(star);
  //Write RS low for other stuff to work
  //Flip mode back to 
  
}

