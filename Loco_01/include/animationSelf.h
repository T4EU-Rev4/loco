#ifndef animationSelf_h
#define animationSelf_h


#include <animationBasic.h>

/**
 * @brief This class is the base class for all animations, running on its own controller
 *        and do not need any support from the ESP
 * 
 */

class TAnimationSelf : public TAnimationBasic
{
  private:

  protected:
    //virtual void animate();             //this is the method where the real animation take place
    //virtual void beforeStart();         //will be called once before animation starts
    //virtual void afterStop();           //will be calls once after aniimation stops

    /* This class is intended to be used with animations using their own microcontroller
       The method command just sends the two bytes to the mircrocontroller.
       There is no interaction between ESP and microcontroller needed.
    */

  public:
    virtual void trigger();      //call on a regular base to keep the animation running, not needed here
    virtual void command( uint8_t cmd, uint8_t val );
};


#endif