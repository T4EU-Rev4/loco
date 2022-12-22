#include "motor.h"

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;

uint8_t speed2pwm[16] = {0,185,190,195,200,205,210,215,220,225,230,235,240,245,250,255};

Motor::Motor(  ) {
    _en1 = 18;       
    _en2 = 19;
    _enable = 5;        //typically a PWM-Signal
    _speed = 0;
    _dir = mdFree;      //
}

Motor::Motor( uint8_t en1, uint8_t en2, uint8_t enable ) {
    _en1 = en1;       
    _en2 = en2;
    _enable = enable;    //typically a PWM-Signal
    _speed = 0;
    _dir = mdFree;       //

}


void Motor::begin() {
  //ESP32:
  // sets the pins as outputs:
  pinMode(_en1, OUTPUT);
  pinMode(_en2, OUTPUT);
  pinMode(_enable, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin( _enable, pwmChannel );
}
void Motor::stop(){
  digitalWrite(_en1, LOW);
  digitalWrite(_en2, LOW); 
  _dir = mdFree;
}

void Motor::left(){
  digitalWrite(_en1, LOW);
  digitalWrite(_en2, HIGH); 
  _dir = mdLeft;
}

void Motor::right(){
  digitalWrite(_en1, HIGH);
  digitalWrite(_en2, LOW); 
  _dir = mdRight;
}

void Motor::brake(){
  digitalWrite(_en1, HIGH);
  digitalWrite(_en2, HIGH); 
  _dir = mdBrake;
}

void Motor::direction( TDirection dir ) {
    if (_dir != dir ) {
      if (_dir != mdFree) {
        stop();
      }
      switch (dir) {
        case mdFree : stop();         
                      break;
        case mdLeft : left();         
                      break;
        case mdRight: right();         
                      break;
        case mdBrake: brake();         
                      break;
      }
    }
}

void Motor::speed( uint8_t speed ) {   
    speed = speed & 0x0F;      //delimit to values between 0..15
    if ( _speed != speed ) {
        _speed = speed;     
        ledcWrite(pwmChannel, speed2pwm[ _speed ]);   
        Serial.println( speed2pwm[ _speed ] );
    }
}

//0..15
void Motor::speed( uint8_t speedstart, uint8_t speedmax ) {

}    

void Motor::trigger() {     //used for ramp up / down

}                     
    