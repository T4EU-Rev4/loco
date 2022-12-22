#ifndef animationESP_h
#define animationESP_h


#include <animationBasic.h>

/**
 * @brief This class is the base class for all type of animations, running on the ESP itself.
 * 
 */

class TAnimationESP : public TAnimationBasic
{
  private:

  protected:
    virtual void beforeStart();         //will be called once before animation starts
    virtual void afterStop();           //will be calls once after aniimation stops

  public:
    virtual void trigger();      
    virtual void start( uint8_t val );
    virtual void stop();    

    virtual void command( uint8_t cmd, uint8_t val );
};









#endif