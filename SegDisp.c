/*--------------------------------------------
File: SegDisp.c
Description:  Segment Display Module
              Uses Timer Channel 1
---------------------------------------------*/

#include "mc9s12dg256.h"
#include "SegDisp.h"

#define NUMDISPS 4  // number of displays
#define SPACE ' '   // The space character
#define BLANK 0xFF  // disables displays
#define NUMFLASH 10 // Number of flashes
#define delaydisp 3750	// For 50 ms delay (1 1/3 microSec/tick).
#define HALFSEC 500  // 500 ms = 1/2 sec


// Global Variables
byte codes[NUMDISPS];  // ASCII codes for each display

struct ascii_to_code 
{
   byte ascii;
   byte code;  
}; 


//Tableau pour faire la conversion
#define NUMCHS  18 // number of entries in dispTbl
struct ascii_to_code dispTbl[NUMCHS] = 
{          //gfedcba  <- display segments
    { '0',0b00111111 },//0
    { '1',0b00000110 },//1
    { '2',0b01011011 },//2
    { '3',0b01001111 },//3
    { '4',0b01100110 },//4
    { '5',0b01101101 },//5
    { '6',0b01111101 },//6
    { '7',0b00000111 },//7
    { '8',0b01111111 },//8
    { '9',0b01101111 },//9
    { '*',0b01000110 },//*
    { '#',0b01110000 },//#
    { 'a',0b01110111 },//a
    { 'b',0b01111100 },//b
    { 'c',0b00111001 },//c
    { 'd',0b01011110 },//d
    { 'e',0b01111001 },//e
    { ' ',0b00000000 }  // space
};

byte enableCodes[NUMDISPS]= {  
     0b00001110,	// display 0
     0b00001101,	// display 1
     0b00001011,	// display 2
     0b00000111	  // display 3
};

// Prototypes for internal functions

byte getCode(byte ch);

/*---------------------------------------------
Function: initDisp
Description: Initialiser les composantes pour l'afficheur 7-segments
-----------------------------------------------*/
void initDisp(void) 
{
  // Sets up Port B and P to control displays
  DDRB = 0xFF;  // direction  output for PORT B
  DDRP |= 0x0F;  // set output direction for  PORT P bits 0 to 3
  PTP |= 0x0F;   // disable all displays
  clearDisp();  // Clears displays 
	TIOS |= 0b00000010; // Set ouput compare for TC1
	TIE |= 0b00000010; // Enable interrupt on TC1
	
	TC1 = TCNT + delaydisp; // enables timeout on channel 1		;  
}

/*---------------------------------------------
Function: clearDisp
Description: Clears all displays.
-----------------------------------------------*/
void clearDisp(void) 
{
   int i;
   for(i=0 ; i<NUMDISPS ; i++) //loops over all displays
      codes[i] = 0; //sets global array to 0;
}

/*---------------------------------------------
Function: setCharDisplay
Description: Receives an ASCII character (ch)
             and translates
             it to the corresponding code to 
             display on 7-segment display.  Code
             is stored in appropriate element of
             codes for identified display (dispNum).
-----------------------------------------------*/
void setCharDisplay(char ch, byte dispNum) 
{
  byte code;

  code = getCode(ch);
  codes[dispNum] = code | (codes[dispNum]&0x80);//store the code at the locaiton of the dispNum
                          // to preserve decimal points
}

/*---------------------------------------------
Function: getCode
Description: Translates an ASCII character code
             (ch) to a 7-segment display code. 
             Returns 0 (blank) if character is
             not in the table;
-----------------------------------------------*/
byte getCode(byte ch) 
{
   byte code = 0;
   byte i;
   for(i=0 ; i < NUMCHS && code==0 ; i++)
     if(ch == dispTbl[i].ascii)    //look in the table to match the value read
        code = dispTbl[i].code;    //store translated code of matched value
   return(code);  
}

/*---------------------------------------------
Function: turnOnDP
Description: turns on the decimal point in the display. 
-----------------------------------------------*/
void turnOnDP(int dNum) 
{
    codes[dNum] = codes[dNum] | 0x80;  // set last to 1 to tun on the decimal point  
}

/*---------------------------------------------
Function: turnOffDP
Description: Éteint le point décimal
-----------------------------------------------*/
void turnOffDP(int dNum) 
{
    codes[dNum] = codes[dNum] & 0x7f;  // set the last bit to 0 to turn off the decimal point
}


/*-------------------------------------------------
Interrupt: disp_isr
Description:interrupt service is called every 50ms. 
---------------------------------------------------*/
void interrupt VectorNumber_Vtimch1 disp_isr(void)
{
  static byte dNum = 0;  //    store between interruptions
  byte enable;
  
  PORTB = codes[dNum];
  enable = PTP;  //  search current values
  enable &= 0xF0; //  clears lower 4 bits
  PTP = enable | enableCodes[dNum]; // set lower four bits
  dNum++;
  dNum = dNum%NUMDISPS;
	// intialize next interuption
	TC1 = TC1 + delaydisp;
}



