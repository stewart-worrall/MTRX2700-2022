; timer overflow example

; export symbols
            XDEF Entry, _Startup  ; export 'Entry' symbol
            ABSENTRY Entry        ; for absolute assembly: mark this as application entry point
                          

; Include derivative-specific definitions 
		INCLUDE 'derivative.inc' 

ROMStart    EQU  $4000  ; absolute address to place my code/constant data


; define variables in RAM
            ORG RAMStart
counter     DS.B 1
LED_value   DS.W 1


; ROM is where the code goes
            ORG   ROMStart

Entry:
_Startup:
            LDS   #RAMEnd+1       ; initialize the stack pointer

            ; disable interrupts and perform initialisation
            SEI                   
            
            ; clear the variables defined in RAM
            movb  #0, LED_value
            movb  #0, counter
            
            ; enable the counter (TEN = 1)
            movb  #mTSCR1_TEN, TSCR1
            
            ; prescaler to 0, set timer overflow interrupt (TOI)
            movb  #mTSCR2_TOI, TSCR2  
            
            ; reset timer overflow flag by writing 1 to TOF (allow it to trigger)
            bset  TFLG2, #mTFLG2_TOF 
            
            ; enable interrupts now the initialisation is complete
            CLI                      
            
            
            
mainLoop:
            ; loop forever (could do other low priority tasks here)
            BRA  *
            
            
            
timer_overflow:
            ; increment the value in the 8 bit counter variable
            inc  counter
            
            ; branch if the counter overflows (counter overflows after 255)
            bvs counter_overflow    
            
continue:
            ; inspect the counter time (TCNT)
            ; this is the current value of the counter, it should be very small
            ; because this interrupt is triggered when it overflows
            ldx TCNT  
            
            ; reset the timer overflow flag so it will work again next time
            bset  TFLG2, #mTFLG2_TOF 
            
            ; return from the interrupt
            rti


counter_overflow:
            ; the counter has overflowed, toggle the LED value
            ldaa LED_value ; load the current LED_value
            coma           ; invert the value
            staa LED_value ; store over LED_value (toggle)
            
            ; return to the calling function       
            bra continue
            
            
;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector

            ORG   $FFDE
            DC.W  timer_overflow  ; timer overflow 
