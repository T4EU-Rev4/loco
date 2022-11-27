#include <animationKnightRider.h>

void TAnimationKnightRider::setToDevice() {
    _portA = (data & 0xFF);
    _portB = (data >> 8) & 0xFF;
    TAnimationBasic::animate();  
}

void TAnimationKnightRider::animate() {             //this is the method where the real animation take place
    if ( _running ) {
      if (up) {
        data = data << 1;
        if (data & 0x8000) { up = false; }
      }  else {
        data = data >> 1;
        if (data & 1) { up = true; }
      }   
      setToDevice();
      //Serial.print(".");                    //uncomment to see, if animation is called regularly
    }
}

void TAnimationKnightRider::beforeStart(){         //will be called once before animation starts
    data = 1;
    up = true;
    setToDevice();
}

void TAnimationKnightRider::afterStop(){           //will be calls once after aniimation stops
    data = 0;
    setToDevice();             
}
