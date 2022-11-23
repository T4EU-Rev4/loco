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
    _invert = true;     //default behavior of PCF8575

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
    }
    I2CDev_Counter++;    
}


void I2CDevice::invertBeforeTransmission( bool inv ) {
    _invert = inv;
}

void I2CDevice::set( uint8_t index, uint8_t value ) {
    if (index >= I2CDev_NrOfPorts) { index = I2CDev_NrOfPorts-1; }
    _ports[ index ] = value;
}

uint8_t I2CDevice::get( uint8_t index ) {
    if (index >= I2CDev_NrOfPorts) { index = I2CDev_NrOfPorts-1; }
    return ( _ports[ index ] );
}


 void I2CDevice::write( ){
    uint8_t x;
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

}

