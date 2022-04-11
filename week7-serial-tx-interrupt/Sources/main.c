#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdio.h>   

// NOTE: these are stored as pointers because they 
//       are const values so we can't store them directly
//       in the struct
typedef struct SerialPort { 
  byte *BaudHigh;
  byte *BaudLow;
  byte *ControlRegister1;
  byte *ControlRegister2;
  byte *DataRegister;
  byte *StatusRegister;
} SerialPort;

 
char *string_1 = "polling is not great\r\n";
char *string_2 = "interrupts are the best\r\n";
char *current_character = 0x00;
 
// instantiate the serial port parameters
//   note: the complexity is hidden in the c file
SerialPort SCI1 = {&SCI1BDH, &SCI1BDL, &SCI1CR1, &SCI1CR2, &SCI1DRL, &SCI1SR1};


// InitialiseSerial - Initialise the serial port SCI1
// Input: baudRate is tha baud rate in bits/sec
void SerialInitialiseBasic(SerialPort *serial_port) {
  
  *(serial_port->BaudHigh)=0;
  *(serial_port->BaudLow)=156; 
  *(serial_port->ControlRegister2) = SCI1CR2_RE_MASK|SCI1CR2_TE_MASK|SCI1CR2_TCIE_MASK; 
  *(serial_port->ControlRegister1) = 0x00;
}   

        
void SerialOutputChar(char data, SerialPort *serial_port) {  

  int wait_counter = 0;
  while((*(serial_port->StatusRegister) & SCI1SR1_TDRE_MASK) == 0){
     if (wait_counter < 0xFE)
       wait_counter++;
  }
  
  *(serial_port->DataRegister) = data;
}


void SerialOutputString(char *pt, SerialPort *serial_port) {
  while(*pt) {
    SerialOutputChar(*pt, serial_port);
    pt++;
  }            
}

  
interrupt VectorNumber_Vsci1 void SerialInterruptHandler(){

  if (*(SCI1.StatusRegister) & SCI1SR1_TDRE_MASK && *current_character != 0x00) {
    SerialOutputChar(*(current_character++), &SCI1);
  } 
  else if (*current_character == 0x00){
    
    // string is finished, stop the transmit interrupt from firing
    *(SCI1.ControlRegister2) &= ~SCI1CR2_TCIE_MASK;
  }
}      



void main(void){  
  
  SerialInitialiseBasic(&SCI1);  

  EnableInterrupts
                                                
  while(1){

    current_character = &string_2[0];
    
    // enable the transmit mask
    *(SCI1.ControlRegister2) |= SCI1CR2_TCIE_MASK;
    
    // interrupts are enabled, only send the first char then the interrupts will send the rest one at a time
    SerialOutputChar(*(current_character++), &SCI1);
    
    while (*current_character != 0x00) {
      // waiting in here until the string has completed sending
    }
  }  
}

