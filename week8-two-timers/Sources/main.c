#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "serial.h"


char *serial_1 = "AaA\n";
char *serial_2 = "BbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbB\n";

int flag_next_message = 0;

void main(void) {
  
  // set up the timers  
  TSCR1_TEN = 1;
  TSCR2 = 0x07;   // prescaler 128
  
  TIOS_IOS6 = 1;
  TCTL1_OL6 = 1;
  TIE_C6I = 1;

  TIOS_IOS7 = 1;
  TCTL1_OL7 = 1;
  TIE_C7I = 1;

  SerialInitialise(BAUD_9600, &SCI1);  

	EnableInterrupts;
	

  for(;;) {

    while (!flag_next_message) {
      _FEED_COP();
    }
    
    flag_next_message = 0;

    SerialOutputString(serial_2, &SCI1);

    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}




// the interrupt for timer 6 which is used for a iic timeout
#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
__interrupt void TC6_ISR(void) { 
    
  TC6 =TCNT + 19000;   // interrupt at 100Hz assuming prescaler =1
  TFLG1=TFLG1 | TFLG1_C6F_MASK;

  SerialOutputString(serial_1, &SCI1);
 
}

// the interrupt for timer 7 which is used for a iic timeout
#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
__interrupt void TC7_ISR(void) { 
   
  TC7 =TCNT + 12000;   // interrupt at 100Hz assuming prescaler =1
  TFLG1=TFLG1 | TFLG1_C7F_MASK;

  //SerialOutputString(serial_2, &SCI1);
  
  flag_next_message = 1;
}


// register the timer callback with the TIMER 6 ISR vector
typedef void (*near tIsrFunc)(void);

const tIsrFunc _vect_6 @Vtimch6 = TC6_ISR;
const tIsrFunc _vect_7 @Vtimch7 = TC7_ISR;