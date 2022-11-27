#include <animationUpDown.h>


void TAnimationUpDown::animate() {             //this is the method where the real animation take place
    if ( _running ) {
      _portA++;
      _portB--;
      TAnimationBasic::animate();             //call method of parent class to to transfer via i2c bus
      //Serial.print(".");                    //uncomment to see, if animation is called regularly
    }
}

void TAnimationUpDown::beforeStart(){         //will be called once before animation starts
    _portA = 0;
    _portB = 255;
    TAnimationBasic::animate();  
}

void TAnimationUpDown::afterStop(){           //will be calls once after aniimation stops
    _portA = 0;
    _portB = 0;
    TAnimationBasic::animate();              
}
