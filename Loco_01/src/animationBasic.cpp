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


void TAnimationBasic::beforeStart() {

}        

void TAnimationBasic::animate() {       //call this method from any overloaded version to send the data over the bus
    if ( _wagon != NULL)  {
        _wagon->device.set( PCF_PORT_A, _portA );
        _wagon->device.set( PCF_PORT_B, _portB );
        _wagon->device.write();
        //Serial.println(".");
    }
}

void TAnimationBasic::afterStop() {
    
}


void TAnimationBasic::trigger() {
    if ( _wagon != NULL) {
        if ( _running ) { 
          if ( _value > 0 ) {
            _value--;
            if ( _value == 0 ) {
              stop();
            }
          }
          animate();  
        }
    }
}

void TAnimationBasic::start( uint8_t val ) {
    if ( _wagon != NULL) {
      if (!_running) {
        _value = val * 10;  //because trigger() is called every 100ms 
        beforeStart();
        _running = true;
      }    
    }
}

void TAnimationBasic::stop()  {
    if ( _wagon != NULL) {
      if (_running) {
        _running = false;
        afterStop();
      }    
        
    }
} 


void TAnimationBasic::command( uint8_t cmd, uint8_t val ) {
  _running = false;  
  _portA = cmd;
  _portB = val;
  TAnimationBasic::animate();
}