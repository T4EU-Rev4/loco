#include <animationESP.h>


void TAnimationESP::trigger() {
    if ( getWagon() != NULL) {
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

void TAnimationESP::beforeStart() {

}

void TAnimationESP::afterStop() {
    
}


void TAnimationESP::start( uint8_t val ) {
    if ( getWagon() != NULL) {
      if (!_running) {
        _value = val * 10;  //because trigger() is called every 100ms 
        beforeStart();
        _running = true;
      }    
    }
}

void TAnimationESP::stop()  {
    if ( getWagon() != NULL) {
      if (_running) {
        _running = false;
        afterStop();
      }    
        
    }
} 


void TAnimationESP::command( uint8_t cmd, uint8_t val ) {
  switch (cmd)  {
    case 1: start( val );
            break;
    case 3: stop();
            break;  
    default: ;   //command( cmd, val );
             break;
  }
}