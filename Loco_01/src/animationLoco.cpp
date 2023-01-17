#include <animationLoco.h>


TAnimationLoco::TAnimationLoco( uint8_t en1, uint8_t en2, uint8_t enable ) {
    _motor = Motor( en1, en2, enable );
    _motor.begin();
    _ramp = 0;
    _rampDelay = RAMP_DELAY;
    _acc = accUp;          
}

void TAnimationLoco::setSpeed() {
    //Serial.println( _ramp );
    _motor.speed( _ramp );
    if ( _acc == accUp) {   //speed up
        if (_ramp < MAX_SPEED) {
            _ramp++;
        }
    } else {                //slow down
        if (_ramp > 0) {
            _ramp--;
        } else {
            _running = false;
        }
    }
}

/**
 * @brief Method is called every 100ms by timer
 *        Calls method setSpeed() after every RAMP_DELAY calls
 *        Used to acclerate the motor up and down
 */
void TAnimationLoco::trigger(){      //call on a regular base to keep the animation running
    if (_running) {
      _rampDelay--;
      if (_rampDelay == 0) {
        _rampDelay = RAMP_DELAY;
        setSpeed();
      }      
    } else {
        _rampDelay = RAMP_DELAY;
    }
}

/**
 * @brief 
 * 
 * @param cmd   1=Left
 *              3=Right
 * @param val   1=On
 *              0=Off
 */
void TAnimationLoco::command( uint8_t cmd, uint8_t val ) {
    TDirection dir;
    switch( cmd ) {
        case  0: val = 0;          //Stop animation
                 break;
        case  1: dir = mdLeft;     //start animation
                 val=1;
                 break;
        case  3: dir = mdRight;     break;
        default: dir = mdFree;
    }

    if (dir != mdFree) {
        if (val !=0 ) {
            _running = true;
            _ramp = 0;
            _acc = accUp;
            _motor.direction( dir );
        } else {
            _acc = accDown;     //stopps automatically when reaching 0
        }
    }
    


}