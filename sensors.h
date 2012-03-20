#ifndef SENSORS_H
#define SENSORS_H

// Sensor I2C addresses
#define ACCEL_ADDRESS ((int) 0x53) // 0x53 = 0xA6 / 2
#define MAGN_ADDRESS  ((int) 0x1E) // 0x1E = 0x3C / 2
#define GYRO_ADDRESS  ((int) 0x68) // 0x68 = 0xD0 / 2

// Sensor data registers
#define ACCEL_DATAREG	0x32
#define MAGN_DATAREG	0x03 
#define GYRO_DATAREG	0x1D

float gyro[3];
float accel[3];
float magnetom[3];

void accel_init();
void gyro_init();
void magn_init();
    
void Read_Accel();
void Read_Gyro();
void Read_Magn();

#endif