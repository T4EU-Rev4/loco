#ifndef animationUpDown_h
#define animationUpDown_h

#include <animationESP.h>

/**
 * @brief This animation counts port A from 0 to 255 upwards,
 *        while port B counts from 255 to 0 downwards.
 * 
 */

class TAnimationUpDown : public TAnimationESP
{
  private:

  protected:
    virtual void animate();             //this is the method where the real animation take place
    virtual void beforeStart();         //will be called once before animation starts
    virtual void afterStop();           //will be calls once after aniimation stops

  public:

};


#endif