# Makefile for razr stuff
# Based on the Makefile for the overo-arduino-i2c test program

ifeq ($(strip $(OETMP)),)
	OETMP = $(HOME)/overo-oe/tmp
endif


TOOLDIR = $(OETMP)/sysroots/`uname -m`-linux/usr/armv7a/bin
STAGEDIR = ${OETMP}/sysroots/armv7a-angstrom-linux-gnueabi/usr

CC = ${TOOLDIR}/arm-angstrom-linux-gnueabi-gcc
CFLAGS = -Wall

LIBDIR = $(STAGEDIR)/lib
INCDIR = $(STAGEDIR)/include

TARGET = razr

$(TARGET) :
	$(CC) $(CFLAGS) -I $(INCDIR) -I . -L $(LIBDIR) Razor_AHRS.c dcm.c math.c output.c compass.c sensors.c overo-i2c.c -o $(TARGET)

test:
	gcc Razor_AHRS.c dcm.c math.c output.c compass.c sensors.c overo-i2c.c -o test

install:
	scp $(TARGET) root@192.168.23.3:/home/root

clean :
	rm -f $(TARGET)
