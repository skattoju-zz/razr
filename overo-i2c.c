/*
  Simple I2C communication test with an Arduino as the slave device.
*/

#include "overo-i2c.h"

typedef struct i2c_data_s{
  
  int fh;
  char buff[ARDUINO_I2C_BUFFER_LIMIT + 4];
  int len, sent, rcvd;

} i2c_data_t;

static i2c_data_t i2c_data;

void i2cWriteByte(unsigned char reg, unsigned char byte){
    
    //buffer
    char buff[2];
    buff[0] = reg;
    buff[1] = byte;
    
    //send
    i2c_data.sent = write(i2c_data.fh, buff, 2);
    
    //verify sent
    if (i2c_data.sent != 2) {
      perror("write");
      //printf("& the buffer is messed up\n");
    }
}

void i2cWriteAddress(unsigned char reg){    

    //send
    i2c_data.sent = write(i2c_data.fh, &reg, 1);
    
    //verify sent
    if (i2c_data.sent != 1) {
      perror("write");
      //printf("& the buffer is messed up\n");
    }
}

char i2cReadByte(unsigned char reg){
  
  unsigned char buff;
  
  //select register
  i2cWriteAddress(reg);

  //clear buffer
  memset(&buff, 0, sizeof(buff));
  
  //read one byte
  i2c_data.rcvd = read(i2c_data.fh, &buff, 1);

  if (i2c_data.rcvd != 1){
    printf("ERR!: Recived Error");
    return 0;
  }
  
  return buff;
}

void i2cSetAddress(int address){
  
  //set address
  if (ioctl(i2c_data.fh, I2C_SLAVE, address) < 0) {
    perror("ioctl");
   // return 1;
  }
  
}

void i2cInit(char* i2c_device){
  
  //open device
  i2c_data.fh = open(i2c_device, O_RDWR);
  if (i2c_data.fh < 0) {
    perror("open");
  }
  
}

void i2cDestroy(){
  close(i2c_data.fh);
}

void i2cReciveBytes(int bytes){

  //clear buffer
  memset(i2c_data.buff, 0, sizeof(i2c_data.buff));
  
  //read	
  i2c_data.rcvd = read(i2c_data.fh, i2c_data.buff, bytes);
  
  while (i2c_data.rcvd < bytes) {
    
    usleep(5000);	
    i2c_data.len = read(i2c_data.fh, i2c_data.buff + i2c_data.rcvd, bytes - i2c_data.rcvd);

    if (i2c_data.len <= 0) {
      if (i2c_data.len < 0)
	perror("read");
      break;
    }
    
    i2c_data.rcvd += i2c_data.len;
  }

  if (i2c_data.rcvd <= bytes){
    printf("ERR!: Recived Error");
  }
  
}

void i2cSend(char* msg){
  
  //copy stuff to send into buffer
  if (sizeof(msg) > 1) {

      memset(i2c_data.buff, 0, sizeof(i2c_data.buff));
      strncpy(i2c_data.buff, msg, ARDUINO_I2C_BUFFER_LIMIT);
  }
  else {
    strcpy(i2c_data.buff,"hello");
  }
  
  //send
  i2c_data.len = strlen(i2c_data.buff);
  i2c_data.sent = write(i2c_data.fh, i2c_data.buff, i2c_data.len);

  if (i2c_data.sent != i2c_data.len) {
    perror("write");
    //return 1;
  }

}

void i2cRecive(){

  //clear buffer
  memset(i2c_data.buff, 0, sizeof(i2c_data.buff));
  
  //read	
  i2c_data.rcvd = read(i2c_data.fh, i2c_data.buff, i2c_data.sent);
  while (i2c_data.rcvd < i2c_data.sent) {
    
    usleep(50000);	
    i2c_data.len = read(i2c_data.fh, i2c_data.buff + i2c_data.rcvd, i2c_data.sent - i2c_data.rcvd);

    if (i2c_data.len <= 0) {
      if (i2c_data.len < 0)
	perror("read");
      break;
    }
    
    i2c_data.rcvd += i2c_data.len;
  }
  
  if (i2c_data.rcvd <= 0){
    printf("ERR!: Recived Error");
  }
  
}
