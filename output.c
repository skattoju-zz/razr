/* This file is part of the Razor AHRS Firmware */

#include <stdio.h>

extern float accel[3];  // Actually stores the NEGATED acceleration (equals gravity, if board not moving).
extern float accel_min[3];
extern float accel_max[3];

extern float magnetom[3];
extern float magnetom_min[3];
extern float magnetom_max[3];

extern float gyro[3];
extern float gyro_average[3];
extern int gyro_num_samples = 0;

extern int output_mode;

extern float yaw,pitch,roll;

extern OUTPUT__MODE_ANGLES_BINARY;
extern OUTPUT__MODE_ANGLES_TEXT;

#define TO_RAD(x) (x * 0.01745329252)  // *pi/180
#define TO_DEG(x) (x * 57.2957795131)  // *180/pi

// Output angles: yaw, pitch, roll
void output_angles()
{
  if (output_mode == OUTPUT__MODE_ANGLES_BINARY)
  {
    float ypr[3];  
    ypr[0] = TO_DEG(yaw);
    ypr[1] = TO_DEG(pitch);
    ypr[2] = TO_DEG(roll);
    int k;
    for(k = 0; k<12; k++){
      printf("%c",ypr[k]);  // No new-line
    }
  }
  else if (output_mode == OUTPUT__MODE_ANGLES_TEXT)
  {
      printf("#YPR=");
      printf("%d",TO_DEG(yaw)); printf(",");
      printf("%d",TO_DEG(pitch)); printf(",");
      printf("%d",TO_DEG(roll)); printf("\n");
  }
}

void output_calibration(int calibration_sensor)
{
  if (calibration_sensor == 0)  // Accelerometer
  {
    // Output MIN/MAX values
    printf("accel x,y,z (min/max) = ");
    int i;
    for (i = 0; i < 3; i++) {
      if (accel[i] < accel_min[i]) accel_min[i] = accel[i];
      if (accel[i] > accel_max[i]) accel_max[i] = accel[i];
      printf("%f",accel_min[i]);
      printf("/");
      printf("%f",accel_max[i]);
      if (i < 2) printf("  ");
      else printf("\n");
    }
  }
  else if (calibration_sensor == 1)  // Magnetometer
  {
    // Output MIN/MAX values
    printf("magn x,y,z (min/max) = ");
    int i;
    for (i = 0; i < 3; i++) {
      if (magnetom[i] < magnetom_min[i]) magnetom_min[i] = magnetom[i];
      if (magnetom[i] > magnetom_max[i]) magnetom_max[i] = magnetom[i];
      printf("%f",magnetom_min[i]);
      printf("/");
      printf("%f",magnetom_max[i]);
      if (i < 2) printf("  ");
      else printf("\n");
    }
  }
  else if (calibration_sensor == 2)  // Gyroscope
  {
    // Average gyro values
    int a;
    for (a = 0; a < 3; a++)
      gyro_average[a] += gyro[a];
    gyro_num_samples++;
      
    // Output current and averaged gyroscope values
    printf("gyro x,y,z (current/average) = ");
    int b;
    for (b = 0; b < 3; b++) {
      printf("%f",gyro[b]);
      printf("/");
      printf("%f",gyro_average[b] / (float) gyro_num_samples);
      if (b < 2) printf("  ");
      else printf("\n");
    }
  }
}

void output_sensors()
{
  printf("#ACC=");
  printf("%f",accel[0]); 
  printf(",");
  printf("%f",accel[1]); 
  printf(",");
  printf("%f",accel[2]); 
  printf("\n");

  printf("#MAG=");
  printf("%f",magnetom[0]); 
  printf(",");
  printf("%f",magnetom[1]); 
  printf(",");
  printf("%f",magnetom[2]); 
  printf("\n");

  printf("#GYR=");
  printf("%f",gyro[0]); 
  printf(",");
  printf("%f",gyro[1]); 
  printf(",");
  printf("%f",gyro[2]); 
  printf("\n");
}

