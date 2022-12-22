#ifndef animationLoco_h
#define animationLoso_h

#include <animationBasic.h>
#include "Motor.h"

#define RAMP_DELAY  3
#define MAX_SPEED   15

enum Accelerate {
    accUp,
    accDown
};

class TAnimationLoco : public TAnimationBasic
{
  private:
    uint8_t    _ramp;
    uint8_t    _rampDelay;
    Accelerate _acc;
    Motor      _motor;

    void setSpeed(); 
  protected:

  public:
    TAnimationLoco( uint8_t en1, uint8_t en2, uint8_t enable );

    virtual void trigger();      //call on a regular base to keep the animation running
    virtual void command( uint8_t cmd, uint8_t val );

};




#endif