#ifndef animationBlink_h
#define animationBlink_h

#include <animationBasic.h>

/**
 * @brief This animations will toggle between the lower and the higher nibble of port a and port b.
 * 
 */

class TAnimationBlink : public TAnimationBasic
{
  private:

  protected:
    virtual void animate();             //this is the method where the real animation take place
    virtual void beforeStart();         //will be called once before animation starts
    virtual void afterStop();           //will be calls once after aniimation stops

  public:

};


#endif