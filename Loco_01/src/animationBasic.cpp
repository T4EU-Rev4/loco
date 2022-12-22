#include <animationBasic.h>


TAnimationBasic::TAnimationBasic() {
    _wagon = NULL;
    _running = false;
    _portA = 0;
    _portB = 0;
    _value = 0;
}

void TAnimationBasic::setWagon( pWagon w ) {
    this->_wagon = w;
}

pWagon TAnimationBasic::getWagon() {
  return( _wagon );
}
    

void TAnimationBasic::animate() {       //call this method from any overloaded version to send the data over the bus
    if ( _wagon != NULL)  {
        _wagon->device.set( PCF_PORT_A, _portA );
        _wagon->device.set( PCF_PORT_B, _portB );
        _wagon->device.write();
        //Serial.println(".");
    }
}

void TAnimationBasic::trigger() {

}

void TAnimationBasic::command( uint8_t cmd, uint8_t val ) {

}