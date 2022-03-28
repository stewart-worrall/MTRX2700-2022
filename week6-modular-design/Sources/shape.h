#ifndef MODULAR_EXAMPLE_SHAPE_H
#define MODULAR_EXAMPLE_SHAPE_H


enum {
  SHAPE_SPHERE,
  SHAPE_CUBE,
  SHAPE_CONE
};
  
  
#define true 1
#define false 0


struct shape {
  int dimension_1;
  int dimension_2;
  int type; // from the enum
  float location;
  unsigned char is_initialised;
};


struct shape generate_shape(int type, int dimension_1, int dimension_2);

int initialise_shape(struct shape *_shape, int type, int dimension_1, int dimension_2);

// Assumes you have a valid char buffer at address string
void shape_string(struct shape *_shape, char *string);

#endif //MODULAR_EXAMPLE_SHAPE_H
