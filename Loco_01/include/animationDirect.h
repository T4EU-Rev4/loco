#ifndef animationDirect_h
#define animationDirect_h

#include <animationBasic.h>

/**
 * @brief This animations interprete cmd as the pin number and val as the status 
 *          to witch the pin should be set.
 * 
 */

class TAnimationDirect : public TAnimationBasic
{
  private:

  protected:
    virtual void animate();             //this is the method where the real animation take place
    virtual void beforeStart();         //will be called once before animation starts
    virtual void afterStop();           //will be calls once after aniimation stops

  public:

    virtual void command( uint8_t cmd, uint8_t val );
};


#endif