#ifndef animationBasic_h
#define animationBasic_h

#include <wagon.h>

#define PCF_PORT_A  0
#define PCF_PORT_B  1

class TAnimationBasic
{
  private:
    pWagon  _wagon;

  protected:
    bool    _running;           //true, if animation via call to start() is running,  false otherwise
    uint8_t _portA;             //the two bytes for the port expander PCF8575
    uint8_t _portB;             //available in derived classes
    uint8_t _value;             //parameter that comes with the start() 

    virtual void animate();             //this is the method where the real animation take place
    virtual void beforeStart();         //will be called once before animation starts
    virtual void afterStop();           //will be calls once after aniimation stops

  public:
    TAnimationBasic();
    void setWagon( pWagon w );   //Connect this animation with a specific wagon
    pWagon getWagon();

    virtual void trigger();               //call on a regular base to keep the animation running

    virtual void start( uint8_t val );
    virtual void stop();    

    virtual void command( uint8_t cmd, uint8_t val );
};



// struct TAnimation {
//   TAnimationBasic*  animation;
//   uint16_t          wagon;         
// };



#endif