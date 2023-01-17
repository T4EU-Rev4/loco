#ifndef wagon_h
#define wagon_h

#include "Arduino.h"
#include "I2Cdevice.h"

/**
 * @brief You can select different type of trains by uncomment one of the following symbols
 *        Always uncomment only one symbol and recompile the project.
 *        You can define more symbols for more trains as you like.
 *        Just search for one of the existing symbols and append a new section there wirh the 
 *        changes you need.
 *        1. Add a new symbol here
 *        2. Change file wagon.cpp according to the existing examples
 *        3. change file main.cpp according to the existing examples
 */

#define TRAIN_GERMANY   
//#define TRAIN_NORWAY    



//-- no changes neccesary after this line ---------

#define NR_OF_WAGONS  25

enum CountryCode {
  L1,
  IT, NO, IR, HR, LT, DE,
  DK, PT, CH, X1, X2, NL,
  TR, AT, PL, FR, SK, SI,
  BE, SE, RO, X3, X4, LX
};


const char countries[NR_OF_WAGONS][3] = { 
    "L1",
    "IT","NO","IR","HR","LT","DE",
    "DK","PT","CH","X1","X2","NL",
    "TR","AT","PL","FR","SK","SI",
    "BE","SE","RO","X3","X4","LX"
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