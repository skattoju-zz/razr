/*
  Simple I2C communication test with the Sparkfun Sensor Stick as the slave device.
*/

/* based on : http://elinux.org/ECE597_Project_Adding_Sense_to_Beagle#Detail_of_ADXL345.c */

#include "overo-i2c.h"
#include "sensors.h"

// Reads x, y and z gyroscope registers
void Read_Gyro()
{
  unsigned char buff[6];
  unsigned char reg = GYRO_DATAREG;
  int i = 0;
  
  // Select device
  i2cSetAddress(GYRO_ADDRESS);
  
  // Read 6 bytes
  while(i<6&&reg<GYRO_DATAREG+6){
    
    buff[i] = i2cReadByte(reg);
    printf("gyro[%x]:%x ",reg,buff[i]);
    //usleep(5000);
    reg++;
    i++;
  }
  
  if (i == 6)  // All bytes received?
  {
    
    gyro[0] = -1 * ((((int) buff[2]) << 8) | buff[3]);    // X axis (internal sensor -y axis)
    gyro[1] = -1 * ((((int) buff[0]) << 8) | buff[1]);    // Y axis (internal sensor -x axis)
    gyro[2] = -1 * ((((int) buff[4]) << 8) | buff[5]);    // Z axis (internal sensor -z axis)
    
    printf("gyro: %f %f %f\n",gyro[0],gyro[1],gyro[2]);
  
  }
  else
  {
    printf("!ERR: reading gyroscope");
  }
}

void Read_Magn()
{
  
  unsigned char buff[6];
  unsigned char reg = MAGN_DATAREG;
  int i = 0;
  
  // Select device
  i2cSetAddress(MAGN_ADDRESS);
   
  // Read 6 bytes
  while(i<6&&reg<MAGN_DATAREG+6){
    
    buff[i] = i2cReadByte(reg);
    printf("magn[%x]:%x ",reg,buff[i]);
    //usleep(5000);
    reg++;
    i++;
    
  }
  
  if (i == 6)  // All bytes received?
  {
    // MSB byte first, then LSB; Y and Z reversed: X, Z, Y
    magnetom[0] = ((((int) buff[0]) << 8) | buff[1]);         // X axis (internal sensor x axis)
    magnetom[1] = -1*((((int) buff[4]) << 8) | buff[5]);  // Y axis (internal sensor -y axis)
    magnetom[2] = -1*((((int) buff[2]) << 8) | buff[3]);  // Z axis (internal sensor -z axis)
    
    printf("magn: %f %f %f\n",magnetom[0],magnetom[1],magnetom[2]);
  }
  else
  {
      printf("!ERR: reading magnetometer");
  }
}

// Reads x, y and z accelerometer registers
void Read_Accel()
{
  unsigned char reg = ACCEL_DATAREG;
  unsigned char buff[6];
  int i = 0;
  
  //select device
  i2cSetAddress(ACCEL_ADDRESS);
  
  //Read 6 bytes
  while(i<6&&reg<ACCEL_DATAREG+6)
  {
    buff[i] = i2cReadByte(reg);  // Read one byte
    printf("accl[%x]:%x ",reg,buff[i]);
    //usleep(5000);
    reg++;
    i++;
  }
  
  if (i == 6)  // All bytes received?
  {
    // No multiply by -1 for coordinate system transformation here, because of double negation:
    // We want the gravity vector, which is negated acceleration vector.
    accel[0] = ((((int) buff[3]) << 8) | buff[2]);  // X axis (internal sensor y axis)
    accel[1] = ((((int) buff[1]) << 8) | buff[0]);  // Y axis (internal sensor x axis)
    accel[2] = ((((int) buff[5]) << 8) | buff[4]);  // Z axis (internal sensor z axis)
    
    printf("accl: %f %f %f\n",accel[0],accel[1],accel[2]);
  }
  else
  {
    printf("!ERR: reading accelerometer");
  }
}

void accel_init(){
  
  i2cSetAddress(ACCEL_ADDRESS);
  // Power register
  // Measurement mode
  i2cWriteByte(0x2D,0x08);  
  
  // Data format register
  // Set to full resolution
  i2cWriteByte(0x31,0x08);  
  
  // Because our main loop runs at 50Hz we adjust the output data rate to 50Hz (25Hz bandwidth)
  // Rate
  // Set to 50Hz, normal operation
  i2cWriteByte(0x2C,0x0C);
}

void magn_init(){
  
  //select device
  i2cSetAddress(MAGN_ADDRESS);
  
  // Set continuous mode (default 10Hz)
  i2cWriteByte(0x02,0x00); 

  // Set 75Hz & Average over 8 samples
  i2cWriteByte(0x00,0b01111000);

}

void gyro_init(){
  
  // Power up reset defaults
  i2cSetAddress(GYRO_ADDRESS);
  i2cWriteByte(0x3E,0x80);
  
  // Select full-scale range of the gyro sensors
  // Set LP filter bandwidth to 42Hz
  // DLPF_CFG = 3, FS_SEL = 3
  i2cWriteByte(0x16,0x1B);
  
  // Set sample rate to 50Hz
  //  SMPLRT_DIV = 1 (500Hz)
  i2cWriteByte(0x15,0x01);

  // Set clock to PLL with z gyro reference
  i2cWriteByte(0x3E,0x00);
 
}

int main(int argc, char **argv)
{ 
    i2cInit(I2C_BUS_DEVICE);
        
    accel_init();
    gyro_init();
    magn_init();
        
    int i = 0;
    while(i<10){      
      Read_Accel();
      Read_Gyro();
      Read_Magn();
      //usleep(40000);
      //i++;
    }
    
    i2cDestroy();

    return 0;
}