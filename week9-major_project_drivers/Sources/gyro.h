#ifndef GYRO_HEADER
#define GYRO_HEADER

typedef struct GyroRaw {
  int x;
  int y;
  int z;
} GyroRaw;

typedef struct GyroScaled {
  float x;
  float y;
  float z;
} GyroScaled;

// NOTE: some function is required to convert between raw and 
//       scaled values. Also, calibration and removing the bias is
//       needed

#endif