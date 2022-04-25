#ifndef ACCELEROMETER_HEADER
#define ACCELEROMETER_HEADER


// data structures containing the raw values
typedef struct AccelRaw {
  int x;
  int y;
  int z;
} AccelRaw;

// data structures containing the raw values
typedef struct AccelScaled {
  float x;
  float y;
  float z;
} AccelScaled;

// function to convert between raw and scaled acceleration
//  note: this function requires verification and calibration
void convertUnits(AccelRaw *raw_data, AccelScaled *scaled_data);

#endif