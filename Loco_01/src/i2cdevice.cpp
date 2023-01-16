#include "I2Cdevice.h"
#include "Wire.h"

uint8_t I2CDevice::I2CDev_Counter = 0;

I2CDevice::I2CDevice( uint8_t addr ){
	_wire = &Wire;
	_addr = addr;
    _sda  = SDA;
    _scl  = SCL;
    for ( uint8_t i=0; i<I2CDev_NrOfPorts; i++) {
      _ports[ i ] = 0;    
    }
    _invert = true;     //default behavior of PCF8575 lib
    _auto   = true;     //default behavior of PCF8575 lib

};


void I2CDevice::begin( uint8_t sda, uint8_t scl ) {
    _sda  = sda;
    _scl  = scl;
    if (I2CDev_Counter == 0) {
        _wire->begin( _sda, _scl ); 
    }
    I2CDev_Counter++; 
}



void I2CDevice::begin(){ 
    if (I2CDev_Counter == 0) {
        _wire->begin(); 
        //Serial.println("I2CDevice::begin()");
    }
    I2CDev_Counter++;    
}


void I2CDevice::invertBeforeTransmission( bool inv ) {
    _invert = inv;
}

void I2CDevice::autoWrite( bool doAuto ) {
    _auto = doAuto;
}

void I2CDevice::set( uint8_t index, uint8_t value ) {
    if (index >= I2CDev_NrOfPorts) { index = I2CDev_NrOfPorts-1; }
    _ports[ index ] = value;
    if ( _auto ) {
        write();
    }
}

uint8_t I2CDevice::get( uint8_t index ) {
    if (index >= I2CDev_NrOfPorts) { index = I2CDev_NrOfPorts-1; }
    return ( _ports[ index ] );
}


 void I2CDevice::write( ){
    uint8_t x;
    //Serial.println( _addr );
    _wire->beginTransmission( _addr );  // transmit to device _addr
    for (uint8_t i=0; i<I2CDev_NrOfPorts; i++ ) {
        if (_invert) {
            x = _ports[ i ] ^ 0xFF;
        } else {
            x = _ports[ i ];
        }
        _wire->write( x );           // send Byte a
    }
    _wire->endTransmission();    // stop transmitting 
 }



void I2CDevice::pinMode(uint8_t pin, uint8_t mode){ 
    //just for compatibility
    //not needed here
}


void I2CDevice::digitalWrite(uint8_t pin, uint8_t value) {
    uint8_t index  = pin / 8;
    uint8_t bitpos = pin % 8;
    if (index < I2CDev_NrOfPorts) {
        if (value == HIGH) {
            _ports[ index ] |= (1 << bitpos);    //set the corresponding bit
        } else {
            _ports[ index ] &= ~(1 << bitpos);   //clear the corresponding bit
        }
        if ( _auto ) {
           write();
        }
    }
}


void I2CDevice::scan() {

  Serial.println ("I2C Scanning ...");
  byte count = 0;

  Wire.begin();
  for (byte i = 8; i < 120; i++) {
    if ( i != EXCLUDE_FROM_SCAN) {
        Wire.beginTransmission (i);          // Begin I2C transmission Address (i)
        if (Wire.endTransmission () == 0) {  // Receive 0 = success (ACK response) 
        Serial.print ("Found address: ");
        Serial.print (" (0x");
        Serial.print (i, HEX);     // PCF8574 7 bit address
        Serial.println ("), ");
        count++;
        }
    }
  }
  Serial.print ("\nFound ");      
  Serial.print (count, DEC);        // numbers of devices
  Serial.println (" device(s).");
}

