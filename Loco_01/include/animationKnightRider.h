#ifndef animationKnightRider_h
#define animationKnightRider_h

#include <animationBasic.h>

/**
 * @brief This animation creates one bit running trough all the 16 LEDs and all the way back.
 *        Like the classic Knight Rider...
 * 
 */


class TAnimationKnightRider : public TAnimationBasic
{
  private:
    uint16_t data;
    bool up;
    void setToDevice();
  protected:
    virtual void animate();             //this is the method where the real animation take place
    virtual void beforeStart();         //will be called once before animation starts
    virtual void afterStop();           //will be calls once after aniimation stops

  public:

};


#endif