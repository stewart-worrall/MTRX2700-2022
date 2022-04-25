#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdio.h>  
  
#define LCD_DATA PORTK
#define LCD_CTRL PORTK
#define RS 0x01
#define EN 0x02

/*! \brief initalses the LCD screen and set the cursor to the first row firt column
        

*/
void Init_LCD(void);

/*! \brief moves the printing location of the LCD.
    \param Position to move the cursor to in Hexadecimal
      

*/
void COMWRT4(unsigned char);

/*!
	\brief Write a byte of data to the location of the cursor on the LCD.
*/


void DATWRT4(unsigned char);

/*! \brief delays the program by a given amount of time.
    \param the scaling value for the delay
*/
void MSDelay(unsigned int);




 /*! \brief Write a given string to the LCD screen. Writes the data byte-wise until a null character is written. 
    \param message The string to be written
*/
void writeStringLCD(unsigned char * message);
/*! \brief Write a given integer to the LCD screen. 
    \param number the number to be written
*/
void writeSIntLCD(int number);



/*! \brief Write a given string and signed integer to the LCd screen. The string will be written first, followed immediately with the integer.
    \param message The string to be written
    \param number the number to be written
      

*/
void writeStringAndSIntLCD(unsigned char * message, int number );

 /*! \brief Write a  2 given string and 2 signed integer to the LCd screen. 
 The string will be written first, followed immediately with a integer this process then repeats for for the second string and message on the 2nd line of the LCD.
    \param message1 The first string to be written to the LCD
    \param number1 The first number to be written to the LCD
    \param message2 The second string to be written to the LCD
	\param number2 The second number to be written to the LCD
*/
void writeTwoStringsSndTwoSInt(unsigned char * message1, int number1, unsigned char * message2, int number2);