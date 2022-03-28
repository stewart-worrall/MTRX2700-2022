#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 16

// a struct used as a data type with a set of character buffers
struct data_type {
  int first_parameter;
  int some_other_parameter;
  char buffer_1[BUFFER_SIZE];
  char buffer_2[BUFFER_SIZE];
  char buffer_3[BUFFER_SIZE];
  int last_parameter;
};


// goes through the string and adds one to the ASCII value of each character
void some_function(char *string_pointer) {
  while (*string_pointer != 0x00){
    *string_pointer = *string_pointer + 1;
    string_pointer++;
  }
}

// fills in some data to a struct
void some_other_function(struct data_type *data_pointer) {
  int counter = 0;
  data_pointer->first_parameter += 1;
  (*data_pointer).last_parameter += 1;

  for (counter = 0; counter < BUFFER_SIZE; counter++) {
    data_pointer->buffer_1[counter] = counter;
    data_pointer->buffer_2[counter] = counter;
    data_pointer->buffer_3[counter] = counter;
  }
}


// a recursive function that will eventually overflow the stack
void a_recursive_function(int counter) {
  volatile struct data_type my_data;
  my_data.first_parameter = counter;
  my_data.last_parameter = counter;
  
  some_other_function(&my_data);
  
  a_recursive_function(counter++);
}


void main(void) {
  // string buffer that is allocated to the stack (part of the stack frame)
  char string_on_stack[64];
  
  // make a struct of type data_type on the stack (part of the stack frame)
  struct data_type my_data_struct;
  
  // pointers to areas of memory, the pointers are stored on the stack
  char *pointer_to_stack = &(string_on_stack[0]);
  char *pointer_to_heap = 0x00;

  // make pointers to the structs
  struct data_type *pointer_struct_on_stack = &my_data_struct;
  struct data_type *pointer_struct_on_heap = 0x00;
  
  my_data_struct.first_parameter = 0xAAAA;
  my_data_struct.last_parameter = 0xBBBB;
  

  EnableInterrupts;

  // The malloc functions runs an algorithm that allocates some 
  //  memory in the area called the "heap" and returns the pointer
  //  to the start of this memory
  pointer_to_heap = (char*)malloc(64);
  
  // malloc can allocate memory on the heap for any data type
  pointer_struct_on_heap = (struct data_type*)malloc(1);
  
  // can access members of the struct using either -> or by
  //  dereferencing the variable with a *
  pointer_struct_on_heap->first_parameter = 0xAAAA;
  (*pointer_struct_on_heap).last_parameter = 0xBBBB;
    
  // The copy function can put a string in a buffer, which can be either
  //  the stack or the heap
  strcpy(pointer_to_stack, "This is a string that goes on the stack");
  strcpy(pointer_to_heap, "This is a string on the heap, with a pointer on the stack");
  
  // can send the pointer to the string to a function
  some_function(pointer_to_stack);
  some_function(pointer_to_heap);

  // can use a function on the data struct using the pointer,
  //  or by sending the address of the stack variable
  //  the first two lines do the same thing
  some_other_function(&my_data_struct);
  some_other_function(pointer_struct_on_stack);
  some_other_function(pointer_struct_on_heap);
    
  a_recursive_function(1);

  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
