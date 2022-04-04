#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


// callback for when there is a new light sensor reading
void LightReading(unsigned char value) {

  volatile int test = 0;
  test = value;  
  
  // the light sensor range seems to be up to 0x1F, so show the 4 most significant bits of that (bits 4,3,2,1)
  //  in the least significant bits of PORTB
  PORTB |= (value >> 1) & 0x0F;
}


// callback for when there is a new potentiometer reading
void PotReading(unsigned char value) {

  volatile int test = 0;
  test = value;  

  // show the 4 most significant bits of the potentiometer in the 4 most significant bits of PORT B
  PORTB |= (value) & 0xF0;
}

 


void main(void) {
  
  // set up the timers  
  TSCR1_TEN = 1;
  TSCR2 = 0x07;   // prescaler 128
  
  TIOS_IOS6 = 1;
  TCTL1_OL6 = 1;
  TIE_C6I = 1;

  // set up the ADC
  ATD0CTL2 = 0x80;     //Turn on ADC,..No Interrupt
 
  ATD0CTL3 = 0x08;  //one conversion, no FIFO
  ATD0CTL4 = 0xEB;  //8-bit resolu, 16-clock for 2nd phase,
      
  // set up the LED interface
  DDRB = 0xFF;    //PORTB as output
  DDRJ = 0xFF;    //PTJ as output for Dragon12+ LEDs
  PTJ = 0x0;        //Allow the LEDs to dsiplay data on PORTB pins

  // register the different ADC channels and include a callback function for each
  RegisterADC(0x87, &PotReading); // pot is on Channel 7 (right justified, unsigned,single-conver,one channel only) 
  RegisterADC(0x84, &LightReading); // light sensor is on Channel 4 (right justified, unsigned,single-conver,one channel only) 

	EnableInterrupts;

    for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}


