#ifndef wagon_h
#define wagon_h

#include "Arduino.h"
#include "I2Cdevice.h"

#define NR_OF_WAGONS  25

enum CountryCode {
  L1,
  IT, NO, IR, HR, LT, DE,
  DK, PT, CH, X1, X2, NL,
  TR, AT, PL, FR, SK, SI,
  BE, SE, RO, X3, X4, X5
};


const char countries[NR_OF_WAGONS][3] = { 
    "L1",
    "IT","NO","IR","HR","LT","DE",
    "DK","PT","CH","X1","X2","NL",
    "TR","AT","PL","FR","SK","SI",
    "BE","SE","RO","X3","X4","X5"
};

//prototype definitions
uint16_t convertCountryCode( const char *code );    
void wagon_setup();

struct TWagon {
  TWagon( uint8_t address, uint16_t wag ) : device(address), wagon( wag ) {}
  I2CDevice  device;
  uint16_t   wagon;         //we derive a 16bit value from the 2-Letter Code of the Wagon
  uint8_t    isAvail;       //1=was found at the I2C bus diring scan
                            //0=was not found -> not reachable
  void begin() {
    device.begin();
  }
 
  void begin( uint8_t sda, uint8_t scl ) {
    device.begin( sda, scl );
  }
} ;


typedef  TWagon*  pWagon;





#endif