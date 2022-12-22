#ifndef MOTOR_h
#define MOTOR_h
#include "Arduino.h"

enum TDirection  {  //Controls the H-bridge
    mdFree,         //Motor is free 
    mdLeft,         //turns left, if enabled of PWM-Signal is applied    
    mdRight,        //turns right, if enabled of PWM-Signal is applied    
    mdBrake         //Motor    
};



class Motor
{
  public:

    Motor( void );
    Motor( uint8_t en1, uint8_t en2, uint8_t enable );

    virtual void begin();
    virtual void direction( TDirection dir );
    virtual void speed( uint8_t speed );        //0..15
    virtual void speed( uint8_t speedstart, uint8_t speedmax );        //0..15
    virtual void trigger();                     //used for ramp up / down
    
  private:

    uint8_t _en1;       
    uint8_t _en2;
    uint8_t _enable;    //typically a PWM-Signal
    TDirection _dir;
    uint8_t _speed;

    void stop();
    void left();
    void right();
    void brake();
};


#endif