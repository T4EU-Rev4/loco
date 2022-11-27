#include <animationBlink.h>


void TAnimationBlink::animate() {             //this is the method where the real animation take place
    if ( _running ) {
      _portA ^= 0xFF;
      _portB ^= 0xFF;
      TAnimationBasic::animate();             //call method of parent class to to transfer via i2c bus
      //Serial.print(".");                    //uncomment to see, if animation is called regularly
    }
}

void TAnimationBlink::beforeStart(){         //will be called once before animation starts
    _portA = 0x0F;
    _portB = 0xF0;
    TAnimationBasic::animate();  
}

void TAnimationBlink::afterStop(){           //will be calls once after aniimation stops
    _portA = 0;
    _portB = 0;
    TAnimationBasic::animate();              
}
