#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include <stdio.h> 
#include <stdlib.h>

// #defines for the shape and movement modules
#include "shape.h"
#include "movement.h"

// #define for the serial port
#include "serial.h"

const int NUMBER_OF_SHAPES = 16;


void main(void) {
  
  int counter = 0;
  char string_buffer[64];

  struct shape shape_1 = generate_shape(SHAPE_SPHERE, 5, 5);
  struct shape shape_2;                                    
  struct shape *shape_3;                                    
  
  struct shape shape_array[16];

  SerialInitialise(BAUD_9600, &SCI0);
  SerialInitialise(BAUD_9600, &SCI1);
  
  shape_string(&shape_1, &string_buffer[0]);
  SerialOutputString(&string_buffer[0], &SCI0);

  shape_string(&shape_2, &string_buffer[0]);
  SerialOutputString(&string_buffer[0], &SCI0);

  shape_3 = (struct shape*)malloc(sizeof(struct shape));
  shape_string(shape_3, &string_buffer[0]);
  SerialOutputString(&string_buffer[0], &SCI0);

  *shape_3 = generate_shape(SHAPE_CONE, 6,3);
  shape_string(shape_3, &string_buffer[0]);
  SerialOutputString(&string_buffer[0], &SCI0);


	EnableInterrupts;

  for (counter = 0; counter < NUMBER_OF_SHAPES; counter++) {
    if (initialise_shape(&shape_array[counter], counter % 3, 3, 4)) {
      shape_string(&shape_array[counter], &string_buffer[0]);
      SerialOutputString(&string_buffer[0], &SCI1);
    } else {
      shape_string(&shape_array[counter], &string_buffer[0]);
      SerialOutputString(&string_buffer[0], &SCI1);
    }
  }

  // slide the shapes
  sprintf(&string_buffer[0], "sliding the shapes by 1.5 units\n");
  SerialOutputString(&string_buffer[0], &SCI1);

  for (counter = 0; counter < NUMBER_OF_SHAPES; counter++) {
    slide_shape(&shape_array[counter], 1.5);
    shape_string(&shape_array[counter], &string_buffer[0]);
    SerialOutputString(&string_buffer[0], &SCI1);  
  }

  // roll the shapes
  sprintf(&string_buffer[0], "rolling the shapes by 2.5 units\n");
  SerialOutputString(&string_buffer[0], &SCI1);
  for (counter = 0; counter < NUMBER_OF_SHAPES; counter++) {
    roll_shape(&shape_array[counter], 2.5);
    shape_string(&shape_array[counter], &string_buffer[0]);
    SerialOutputString(&string_buffer[0], &SCI1);
  }


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  
  
  /* please make sure that you never leave main */
}
