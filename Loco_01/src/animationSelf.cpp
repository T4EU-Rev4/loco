#include <animationSelf.h>



void TAnimationSelf::command( uint8_t cmd, uint8_t val ) {
  _running = false;  
  _portA = cmd;
  _portB = val;
  TAnimationBasic::animate();
}


void TAnimationSelf::trigger() {

}