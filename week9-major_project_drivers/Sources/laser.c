#include "derivative.h"      /* derivative-specific definitions */

#include "laser.h"

// variables for reading the laser pulse timing
unsigned long riseEdge = 0;
unsigned long fallEdge = 0;
unsigned long lastLaserSample = 0;

void laserInit(void){
                    
  TSCR1_TEN = 1;   // enable the timers (this might happen more than once)

  TIE_C1I = 1;     // enable interrupts for channel 1                         
  TIOS_IOS1 = 0;   // Set Laser ch1 to input capture
  
  TCTL4_EDG1A = 1;
  TCTL4_EDG1B = 1; // trigger on both rising and falling edge interrupts

  TFLG1 |= TFLG1_C1F_MASK; // Reset flag
}

void GetLatestLaserSample(unsigned long *sample) {
  *sample = lastLaserSample;
}


#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
__interrupt void TC1_ISR(void) { 
   
  // if captured a rising edge (lidar input is in port T bit 1)
  if (PTT_PTT1) { 
    riseEdge = (unsigned long)TC1;
  } 
  else {
    fallEdge = (unsigned long)TC1;

    if (fallEdge < riseEdge) {
        fallEdge = (unsigned long)65536 + (unsigned long)fallEdge;
    }

    lastLaserSample = fallEdge - riseEdge; // Calculate the period of the PWM wave
  }
  
  TFLG1 |= TFLG1_C1F_MASK; // Reset flag
}
