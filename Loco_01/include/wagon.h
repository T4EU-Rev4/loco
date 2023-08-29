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
//#define TRAIN_ITALY
// #define TRAIN_NORWAY    



//-- no changes neccesary after this line ---------
//-- the structure is defined for the whole train. We only use
//-- the first 10 wagons at the showcase.

#define NR_OF_WAGONS    25
#define SHOWCASE_LIMIT  10

enum CountryCode {
  DE, DK, IS, SK, RO, 
  FR, AT, HR, BE, CH,

  L1, IT, NO, IR, LT, 
  PT, TR, PL, NL, SE,
  SI, LX, X1, X2, X3
};


const char countries[NR_OF_WAGONS][3] = { 
  "DE", "DK", "IS", "SK", "RO", 
  "FR", "AT", "HR", "BE", "CH",

  "L1", "IT", "NO", "IR", "LT", 
  "PT", "TR", "PL", "NL", "SE",
  "SI", "LX", "X1", "X2", "X3"
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