#ifndef wagon_h
#define wagon_h

#include "Arduino.h"
#include "I2Cdevice.h"

#define NR_OF_WAGONS  6

#define cDK 0
#define cDE 1
#define cIT 2
#define cHR 3
#define cLT 4
#define cIR 5

const char countries[NR_OF_WAGONS][3] = { "DK","DE","IT","HR","LT","IR"};

//prototype definitions
uint16_t convertCountryCode( const char *code );    
void wagon_setup();

struct TWagon {
  TWagon( uint8_t address, uint16_t wag ) : device(address), wagon( wag ) {}
  I2CDevice  device;
  uint16_t   wagon;         //we derive a 16bit value from the 2-Letter Code of the Wagon

  void begin() {
    device.begin();
  }
 
 void begin( uint8_t sda, uint8_t scl ) {
    device.begin( sda, scl );
 }
} ;


typedef  TWagon*  pWagon;





#endif