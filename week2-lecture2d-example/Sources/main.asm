;*****************************************************************
;* This stationery serves as the framework for a                 *
;* user application (single file, absolute assembly application) *
;* For a more comprehensive program that                         *
;* demonstrates the more advanced functionality of this          *
;* processor, please see the demonstration applications          *
;* located in the examples subdirectory of the                   *
;* Freescale CodeWarrior for the HC12 Program directory          *
;*****************************************************************

; export symbols
            XDEF Entry, _Startup            ; export 'Entry' symbol
            ABSENTRY Entry        ; for absolute assembly: mark this as application entry point



; Include derivative-specific definitions 
		INCLUDE 'derivative.inc' 

ROMStart    EQU  $4000  ; absolute address to place my code/constant data

; variable/data section

            ORG RAMStart


; code section
            ORG   ROMStart


Entry:
_Startup:
            LDS   #RAMEnd+1       ; initialize the stack pointer

            CLI                     ; enable interrupts

; look in the documentation for each instruction
main:
            LDAA  #6
            STAA  $1000
            LDAA  #20

; how many times will this loop
innerLoop:
            SUBA  $1000
            BGT   innerLoop
            END

; bonus question: how long will this program take to complete 
; from the first to last instruction
;  20 - 6 = 14 (first loop)
;  14 - 6 = 8 (second loop)
;  8 - 6 = 2 (third loop)
;  2 - 6 = -4 (fourth loop)

;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector
