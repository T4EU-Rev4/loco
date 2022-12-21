#ifndef animationDirect_h
#define animationDirect_h

#include <animationESP.h>

/**
 * @brief This animations interprete cmd as the pin number and val as the status 
 *          to witch the pin should be set.
 * 
 */

class TAnimationDirect : public TAnimationESP
{
  private:

  protected:
    //virtual void animate();             //this is the method where the real animation take place
    //virtual void beforeStart();         //will be called once before animation starts
    //virtual void afterStop();           //will be calls once after aniimation stops

    /* We do not need this methods here, because this class will directly set
       the bits of a port expander according to the command pin and status
       There is no other animation in the time between 2 of those commands.
    */

  public:

    virtual void command( uint8_t cmd, uint8_t val );
};


#endif