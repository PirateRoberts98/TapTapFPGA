/*--------------------------------------------
File: thermistor.c
Description:  thermistor Module
---------------------------------------------*/

#include <stdtypes.h>
#include "mc9s12dg256.h"
#include "thermistor.h"


//global variable declation
int convertedtemp;
int numofinteruptions = 0;
/*---------------------------------------------
Function: initThermistor
Description: initializes hardware for the 
             thermistor.
-----------------------------------------------*/
void initThermistor(void) 
{ 
	TIOS_IOS3 = 1;  //enable channel 3
	TIE |= 0b00001000; //enable interrupt on channel 3
	ATD0CTL2 = 0b11000010;// enable interruptions, fast clear,
  wait20us();//wait 20microseconds to allow for stablilization
	ATD0CTL3 = 0b00001000;//single conversion no FIFO
	ATD0CTL4 = 0b00010111;// for conversion use 10-bit resolution 500khz clock cylce
	TC3 = TCNT + 7500 ; //causes interruption every 10 ms    
	asm cli;
}


/*---------------------------------------------
Function: getTemp
Description: Va chercher la valeur convertit et le rend à un chiffre entre 0 et 700
-----------------------------------------------*/
int getTemp(void) 
{
  int localconverted;
  float temperature_actual;
  
  localconverted = convertedtemp; // use local variable to prevent change on other functions
  temperature_actual = (float)(localconverted * 4.88) ;   // calculate for value between 0 and 700
  
  
  
return (int)temperature_actual;	    
	    	   	  
}


void interrupt VectorNumber_Vtimch3 tco3_isr(void) {
 
  if (numofinteruptions < 10) {  //wait for 100 ms 
   numofinteruptions++;   //num of inerruptions increses every 20ms.
  } else {
       ATD0CTL5 = 0b10000101; //conversion on pin PAD05
       numofinteruptions = 0;//reset number of interruptions every 100ms
  }
  TC3 = TCNT + 7500; //prepare for next interruption  

} 

void interrupt VectorNumber_Vatd0 atd_isr(void) { 
      convertedtemp = ATD0DR0; //assigns value from the ATD to the convertedTEmp global variable.
      
}


void wait20us(void)
{
//TSCR1 = 0x90;
//enable TCNT, fast timer flag clear
//TSCR2 = 0;
// TCNT
//prescaler
//to 1
TIOS |= 0x01;
// enable OC0
TC0 = TCNT+ 480;
// wait for 20 us
while(!(TFLG1 & 0x01) ) /*wait*/;
}

