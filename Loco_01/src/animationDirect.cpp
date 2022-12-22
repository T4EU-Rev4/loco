#include <animationDirect.h>
#include <animationBasic.h>


// void TAnimationDirect::animate() {             //this is the method where the real animation take place
//     if ( _running ) {
//       _portA ^= 0xFF;
//       _portB ^= 0xFF;
//       TAnimationBasic::animate();             //call method of parent class to to transfer via i2c bus
//       //Serial.print(".");                    //uncomment to see, if animation is called regularly
//     }
// }

// void TAnimationDirect::beforeStart(){         //will be called once before animation starts
//     _portA = 0x00;
//     _portB = 0x00;
//     TAnimationBasic::animate();  
// }

// void TAnimationDirect::afterStop(){           //will be calls once after aniimation stops
//     _portA = 0;
//     _portB = 0;
//     TAnimationBasic::animate();              
// }




void TAnimationDirect::command( uint8_t cmd, uint8_t val ) {
  _running = false;  
  cmd = cmd && 0x0F;  //limit to the range from 0..15
  uint16_t x = (1 << cmd);  //pit pattern, we want to set

  if (val > 0) {
    //sett bit
    _portB |= (x >> 8);   //Port B 0 high byte PB0 - PB7 = x.8 - x.15
    _portA |= (x);        //Port A 0 low byte  PA0 - PA7 = x.0 - x.7
  }  else {
    //clear bit
    _portB &= ~(x >> 8);   //Port B 0 high byte PB0 - PB7 = x.8 - x.15
    _portA &= ~(x);        //Port A 0 low byte  PA0 - PA7 = x.0 - x.7

  }
  TAnimationBasic::animate();
}