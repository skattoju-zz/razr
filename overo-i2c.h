/*
  Overi2c.h - I2C library for userspace programs accessing the i2c bus.
  Copyright (c) 2011 Siddhartha Kattoju. 
  
  Adapted from:
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef overoi2c_h
#define overoi2c_h

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

#define ARDUINO_I2C_ADDRESS 0x10
#define ARDUINO_I2C_BUFFER_LIMIT 32
#define I2C_BUS_DEVICE "/dev/i2c-3"

//limit defined by arduino wire library.
#define BUFFER_LENGTH 32

void i2cWriteByte(unsigned char reg,unsigned char byte);
char i2cReadByte(unsigned char reg);
void i2cSetAddress(int address);
void i2cInit(char* i2c_device);
void i2cDestroy();

//blerg
void i2cSend(char* msg);
void i2cRecive();
void i2cReciveBytes(int bytes);

#endif

