
#ifndef __AHT10_H__
#define __AHT10_H__

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// Sensor address
#define AHT10_ADDR       0x38

// commands
#define AHT10_INIT_CMD   0xE1
#define AHT10_SOFT_RESET 0xBA
#define AHT10_TRIG_MEAS  0xAC
#define AHT10_DAT1_CMD   0x33
#define AHT10_DAT2_CMD   0x00

// General
#define HUMIDITY         0
#define TEMPERATURE      1

// APIs
void  aht10_init(const char* device);
float read_temperature();
float read_humidity();
void  delay(long ms);

#endif /* __AHT10_H__ */
