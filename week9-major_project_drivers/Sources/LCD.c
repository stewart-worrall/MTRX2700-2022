#include "LCD.h"


void Init_LCD(void){
    DDRK = 0xFF;   
    COMWRT4(0x33);   //reset sequence provided by data sheet
    MSDelay(1);
    COMWRT4(0x32);   //reset sequence provided by data sheet
    MSDelay(1);
    COMWRT4(0x28);   //Function set to four bit data length
                                     //2 line, 5 x 7 dot format
    MSDelay(1);
    COMWRT4(0x06);  //entry mode set, increment, no shift
    MSDelay(1);
    COMWRT4(0x0E);  //Display set, disp on, cursor on, blink off
    MSDelay(1);
    COMWRT4(0x01);  //Clear display
    MSDelay(1);
    COMWRT4(0x80);  //set start posistion, home position
    MSDelay(1);
}



void writeStringLCD(unsigned char * message){
    
    int i=0;  //initalise i =0
        
    while (message[i] != 0x00) {    //loop while thers charactes left in the string
        DATWRT4(message[i++]);      //print the charactes to the LCD
        MSDelay (25);               //delay for a short period of thime
    }
}



void writeSIntLCD(int number){
    int integer, fraction;          //initalise intergers
    unsigned char str[8],str2[8];   //define chars arrays for storing the strings
    
    sprintf(str, "%d", (number)/10);//store the interger value of the number in string 1 
    writeStringLCD(str);            //write the number to the LCD
    writeStringLCD(".");            //write decimal point to LCD
    if (number < 0){                //check if number is negitive
        number*=-1;                 //times by negitive one to prevent a negitive being printed in the decimal
           
    }
    sprintf(str2, "%u", (number)%10); //store the decimal part of the number in sting2
    writeStringLCD(str2);             //print the string on the LCD
  
}



void writeStringAndSIntLCD(unsigned char * message, int number ){
  COMWRT4(0x01);           //move the curose to the top left corrner of the LCD
  writeStringLCD(message); //print message to LCD
  writeSIntLCD(number);    //write number straight after the string
  COMWRT4(0x80);           //set start posistion, home position
  MSDelay(1);              //delay for a short period of time
}


void writeTwoStringsSndTwoSIn(unsigned char * message1, int number1, unsigned char * message2, int number2){
  
  writeStringLCD(message1);   //print message to LCD
  writeSIntLCD(number1);      //print number to LCD
  
  COMWRT4(0xC0);              //set start posistion, home position
  MSDelay(1);                 //delay for a short period of time
  
  writeStringLCD(message2);   //print message to LCD
  writeSIntLCD(number2);      //print number to LCD
  
   COMWRT4(0x80);             //set start posistion, home position
  MSDelay(1);                 //delay for a short period of time
}



void COMWRT4(unsigned char command)
  {
        unsigned char x;
        
        x = (command & 0xF0) >> 2;         //shift high nibble to center of byte for Pk5-Pk2
      LCD_DATA =LCD_DATA & ~0x3C;          //clear bits Pk5-Pk2
        LCD_DATA = LCD_DATA | x;          //sends high nibble to PORTK
        MSDelay(1);
        LCD_CTRL = LCD_CTRL & ~RS;         //set RS to command (RS=0)
        MSDelay(1);
        LCD_CTRL = LCD_CTRL | EN;          //rais enable
        MSDelay(5);
        LCD_CTRL = LCD_CTRL & ~EN;         //Drop enable to capture command
        MSDelay(15);                       //wait
        x = (command & 0x0F)<< 2;          // shift low nibble to center of byte for Pk5-Pk2
        LCD_DATA =LCD_DATA & ~0x3C;         //clear bits Pk5-Pk2
        LCD_DATA =LCD_DATA | x;             //send low nibble to PORTK
        LCD_CTRL = LCD_CTRL | EN;          //rais enable
        MSDelay(5);
        LCD_CTRL = LCD_CTRL & ~EN;         //drop enable to capture command
        MSDelay(15);
  }

         void DATWRT4(unsigned char data)
  {
  unsigned char x;
       
        
        
        x = (data & 0xF0) >> 2;
        LCD_DATA =LCD_DATA & ~0x3C;                     
        LCD_DATA = LCD_DATA | x;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL | RS;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL | EN;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL & ~EN;
        MSDelay(5);
       
        x = (data & 0x0F)<< 2;
        LCD_DATA =LCD_DATA & ~0x3C;                     
        LCD_DATA = LCD_DATA | x;
        LCD_CTRL = LCD_CTRL | EN;
        MSDelay(1);
        LCD_CTRL = LCD_CTRL & ~EN;
        MSDelay(15);
  }


 void MSDelay(unsigned int itime)
  {
    unsigned int i; unsigned int j;
    for(i=0;i<itime;i++)         //cycle through loop incrementing i till it reaches itime 
      for(j=0;j<1000;j++);
 }                               //cycle through loop incrementing j till it reaches itime
