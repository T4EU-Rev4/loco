#ifndef _STEPPER_
#define _STEPPER_


#define SM_TURN_LEFT   LOW
#define SM_TURN_RIGHT  HIGH
#define SM_ENABLED     LOW
#define SM_DISABLED    HIGH

#define SM_MAXLEFT     -1300
#define SM_MAXRIGHT     1300

#define SM_DELAY_US_AFTER_HIGH     100
#define SM_DELAY_US_AFTER_LOW      100
#define SM_DELAY_US_BETWEEN_STEPS  1500
#define SM_DELAY_US_BETWEEN_STEPS_CENTERING  3000

#define SM_CAMPOS_OFFSET    0     //20
#define SM_CAMPOS_STEP      160   //259       //exact value is 259.2

#define SM_BORDER_STEPS   3

enum SM_Direction {
    sm_Right,
    sm_Left
};




class Stepper
{
  public:
    Stepper( uint8_t pul, uint8_t dir );
    Stepper( uint8_t pul, uint8_t dir, uint8_t en );
    Stepper( uint8_t pul, uint8_t dir, uint8_t en, uint8_t sync );

    void begin();     //initializr the used pins and internal data
    void clock();     //called every 1ms from main
    void home();      //search the sync hole and move to its center
    void setDirection( enum SM_Direction dir ); 
    uint8_t error();  
    bool notch();
    void moveTo( uint8_t pos );

    //not implemented yet
    void setSpeed( uint8_t speed );   
    void turn( int16_t steps );
    void turnLeft( int16_t steps );
    void turnRight( int16_t steps );

   
  private:
    uint8_t _pulse;
    uint8_t _direction;
    uint8_t _enable;
    uint8_t _sync;
    uint8_t _speed;
    uint8_t _clockDiv;
    bool    _useEnable;
    bool    _useSync;
    int16_t _position;        //act. position of the axis
    uint8_t _error;           //0=Ok  1=Sync not found  2=No sync avail  3=

    void stepIt();
    void doSteps( uint16_t steps );
    void center();
    int16_t destPost( uint8_t pos );
};

#endif	//_STEPPER_