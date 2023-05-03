#ifndef main_h
#define main_h

#include "Arduino.h"
#include "I2Cdevice.h"

#define LED1    25
#define LED2    26

//define SDA     22
//define SCL     21

#define DEV_0   0x20
#define DEV_1   0x21
#define DEV_2   0x22
#define DEV_3   0x23
#define DEV_4   0x24
#define DEV_5   0x25
#define DEV_6   0x26
#define DEV_7   0x27 

#define DEV     DEV_0

#define NR_OF_PORTS 2

#define BUTTON1     0
#define BUTTON2     2
// #define BUTTON3     16
// #define BUTTON4     17     

void mqtt_MessageReceived( uint16_t adr, uint8_t cmd, uint8_t val );


#endif