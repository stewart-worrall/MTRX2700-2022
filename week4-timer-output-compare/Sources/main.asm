; *************************************************************
; Output compare timer based interrupts
; driving the speaker, and port B
;
; Stewart Worrall 3/2021
;  updated 3/2022


; export symbols
            XDEF Entry, _Startup            ; export 'Entry' symbol
            ABSENTRY Entry        ; for absolute assembly: mark this as application entry point



; Include derivative-specific definitions 
		INCLUDE 'derivative.inc' 


ROMStart    EQU  $4000  ; absolute address to place my code/constant data

; variable/data section

 ifdef _HCS12_SERIALMON
            ORG $3FFF - (RAMEnd - RAMStart)
 else
            ORG RAMStart
 endif

;* Global variables
Counter     DS.B 1    ; a counter to indicate how many times the interrupt is called
Period      DS.W 1    ; the current offset (5.333 microseconds * period)
Notes       DS.W 1    ; a pointer to the delays we want to use

; uncomment the next line for fast tones/counting
FastNotes       DC.W   $5D, $BB, $176, $2EC
; uncomment the next line for slow tones/counting
SlowNotes       DC.W   $2DC8, $16E4, $0b72, $05b9



; code section
            ORG   ROMStart


USING_HARDWARE equ 1 

Entry:
_Startup:
            ; remap the RAM &amp; EEPROM here. See EB386.pdf
 ifdef USING_HARDWARE
            ; set registers at $0000
            CLR   $11                  ; INITRG= $0
            ; set ram to end at $3FFF
            LDAB  #$39
            STAB  $10                  ; INITRM= $39

            ; set eeprom to end at $0FFF
            LDAA  #$9
            STAA  $12                  ; INITEE= $9


            LDS   #$3FFF+1        ; See EB386.pdf, initialize the stack pointer
            movb  #0, DDRH      ; set port H as output

            ; load the fast counter values for speaker tones
            ldd   #FastNotes
           
 else
            LDS   #RAMEnd+1       ; initialize the stack pointer
            movb  #$FF, DDRH      ; set port H as input

            ; load slow notes for visualisation in the simulator
            ldd   #SlowNotes
            
 endif

            CLI                     ; enable interrupts
mainLoop:
            
            
            std   Notes
            


; ------------------------------------
; Main program
           
; Configure registers

            sei                       ; disable all interrupt

            movb        #0, Counter   ; reset the counter to 0
            movw        #0, Period    ; initalise the period to a default value

            ; select channel 5 for Output compare
            movb        #mTIOS_IOS5, TIOS
            
            ; enable timers  #$90 does not req Flag cleared
            movb        #mTSCR1_TEN, TSCR1
            
            ; prescaler  div 128 (bits set to 111)
            movb        #mTSCR2_PR0 | mTSCR2_PR1 | mTSCR2_PR2, TSCR2    
            
            bset        TIE, mTIE_C5I  ; enable Timer Interrupt 5
            cli                        ; enable all interrupts

            bset        DDRT, mDDRT_DDRT5  ; enable PT5 to be output
            bsr         led_enable

; Infinite Loop

loop:       bra  *


; ------------------------------------
; ISR: Output compare event. Set time for NEXT event
; Increment Global Variable and sent to LED (PORTB)

timer_isr:
                  
            ldaa  PTH         ; load the state of the buttons 
            anda  #%00000011  ; only interested in the first two 
                              ; ([PH0 PH1] 00 = 0,01 = 1,10 = 2,11 = 3)

            lsla              ; we want to look for the memory address
                              ; corresponding to the word (2 bytes)
                              ; indicated by the button values
                              ; this needs to be multiplied by 2 to
                              ; point to the correct offset
                              ; notes, notes+2, notes+4, notes+6                                                  
            
            ldy Notes
            ldx a, y
            stx Period
            
            ldd         TCNT      ; get current timer count
            addd Period           ; add the period before the next event
            std         TC5       ; reload the output compare register for channel 5
            
            ; load/increment/store the counter, also store in PORTB
            ldaa        Counter
            inca        
            staa        Counter    
            staa        PORTB     

            ; toggle the bit for PT5
            ldab  PTT
            eorb  #%00100000
            stab  PTT
            
            ; set the flag to 1 for channel 5 to reset the interrupt
            ; Could be avoided with bset TSCR1, #$90
            bset        TFLG1, #mTFLG1_C5F   
            
            rti


led_enable:
            movb  #$ff, DDRB    ; port B as output
            movb  #$ff, DDRJ    ; port J as output                
            movb  #$00, PTJ     ; port J to 0 (enables LEDs)
            clr    PORTB        ; port B to 0 (turns off all LEDs)
            rts                     
            
            
;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector
            
; ISR configuration: Timer 5 
            org         $ffe4
            DC.W        timer_isr

