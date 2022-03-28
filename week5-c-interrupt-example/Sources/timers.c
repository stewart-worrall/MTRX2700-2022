#include "timers.h"

// include the register/pin definitions
#include "derivative.h"      /* derivative-specific definitions */


// set up the timers for channel 7 to use the interrupt
void Init_TC7 (void) {

  TSCR1_TEN = 1;    // enable the timer
  TSCR1_TFFCA = 1;  // set the fast flag clear (read TCNT to re-enable interrupt)
  
  TSCR2_PR2 = 1;    // prescaler 100
  
  TIOS_IOS7 = 1;    // Output compare for ch7
  TCTL1_OL7 = 1;    // Output mode for ch7
  
  TIE_C7I = 1;      // enable interrupt for ch7
}



// look at the isr_vectors.c for where this function is 
//  added to the ISR vector table
#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
__interrupt void TC7_ISR(void) { 
  
  // add to the current timer to make the next interrupt in 
  //  62500 * 41ns * prescaler
  TC7 = TCNT + (word)62500;    // 12 Hhz and PT7
  PORTB ^= 0xF0;        // High nibble LED toggle
}
