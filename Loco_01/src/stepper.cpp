#include <Arduino.h>
#include "stepper.h"



Stepper::Stepper( uint8_t pul, uint8_t dir ){
    _pulse = pul;
    _direction = dir;
    _enable = 0;
    _useEnable = false;
    _sync = 0;
    _useSync = false;
    _speed = 0;
    _clockDiv = 0;      
    _error = 0;
};

Stepper::Stepper( uint8_t pul, uint8_t dir, uint8_t en  ){
    _pulse = pul;
    _direction = dir;
    _enable = en;
    _useEnable = true;
    _sync = 0;
    _useSync = false;    
    _speed = 0;
    _clockDiv = 0;    
    _error = 0;
};

    
Stepper::Stepper( uint8_t pul, uint8_t dir, uint8_t en, uint8_t sync ){
    _pulse = pul;
    _direction = dir;
    _enable = en;
    _useEnable = true;
    _sync = sync;
    _useSync = true;    
    _speed = 0;
    _clockDiv = 0;
    _error = 0;
    
};    


void Stepper::begin() {
    pinMode( _pulse, OUTPUT );
    pinMode( _direction, OUTPUT );
    digitalWrite( _pulse, LOW );
    digitalWrite( _direction, SM_TURN_LEFT );
    if (_useEnable) {
        pinMode( _enable, OUTPUT );
        digitalWrite( _enable, SM_DISABLED );
    }
    if (_useSync) {
        pinMode( _sync, INPUT_PULLUP );
    }
    _error = 0;
}; 

//1ms-Takt vom Timer
void Stepper::clock() {
    if (_clockDiv > 0) {
        _clockDiv--;
    } else {
       // stepIt();
        _clockDiv = _speed;
    }
}; 
     
bool Stepper::notch() {
    return (digitalRead( _sync ) == 0);
};

uint8_t Stepper::error(){
    return _error;
};

void Stepper::setDirection( enum SM_Direction dir ){
    switch( dir ) {
        case sm_Right: digitalWrite( _direction, SM_TURN_RIGHT ); break;
        case sm_Left:  digitalWrite( _direction, SM_TURN_LEFT ); break;
        default: ;
    }
 }; 

void Stepper::stepIt(){
    digitalWrite( _pulse, HIGH );
    delayMicroseconds( SM_DELAY_US_AFTER_HIGH );
    digitalWrite( _pulse, LOW );
    delayMicroseconds( SM_DELAY_US_AFTER_LOW );
};

void Stepper::doSteps( uint16_t steps ){
    while( steps > 0 )  {
        stepIt();
        steps--;
        delayMicroseconds( SM_DELAY_US_BETWEEN_STEPS );
    } 
}

void Stepper::center(){
    Serial.println("Centering...");

    uint16_t step, stepL, stepR;    
    if (  (notch() && (_error == 0)) ){
        step = 0;
        stepL = 0;
        stepR = 0;
        setDirection( sm_Right );
        while ( notch() ) {
            stepIt();
            step++;
            delayMicroseconds( SM_DELAY_US_BETWEEN_STEPS_CENTERING );
        }
        stepR=step;
        setDirection( sm_Left );
        while (step > 0) {
            stepIt();
            step--;
            delayMicroseconds( SM_DELAY_US_BETWEEN_STEPS_CENTERING );
        }
        while ( notch() ) {
            stepIt();
            step++;
            delayMicroseconds( SM_DELAY_US_BETWEEN_STEPS_CENTERING );
        }
        Serial.print("Left:  "); Serial.println( stepL );
        Serial.print("Right: "); Serial.println( stepR );
        stepL = step;
        step = ( stepR + stepL ) >> 2;      //half of the hole
        setDirection( sm_Right );
        while (step > 0) {
            stepIt();
            step--;
        }
        _position = 0;
        Serial.println("Centering done.");
    }
}

//sucht den Referenzpunkt
void Stepper::home(){
    uint8_t border = SM_BORDER_STEPS;
    int16_t step = 0;
    if (_useSync) {
        Serial.println("Search home position...");
        _error = 0;
        if (!(notch())) {
            setDirection( sm_Right );
            while( ((!notch() || (border > 0) ) && (step < SM_MAXRIGHT)) ) {
                stepIt();
                step++;
                if (notch()) {
                    border--;
                } else {
                    border = SM_BORDER_STEPS;
                }
                delayMicroseconds( SM_DELAY_US_BETWEEN_STEPS );
            }
            Serial.println("Search right done");
            if (!(notch())) {   //Not found, turn back
                setDirection( sm_Left );
                while( step > 0 )  {
                    stepIt();
                    step--;
                    delayMicroseconds( SM_DELAY_US_BETWEEN_STEPS );
                }   
                Serial.println("Returning back from Search right");
            }
            step = 0;
            border = SM_BORDER_STEPS;
            //We should now be at the position where we started from
            Serial.print("Notch: ");   Serial.println( notch() );
            Serial.print("Step: ");   Serial.println( step );

            setDirection( sm_Left );
            while( (!notch()  || border > 0) && (step > SM_MAXLEFT))  {
                stepIt();
                step--;
                if (notch()) {
                    border--;
                } else {
                    border = SM_BORDER_STEPS;
                }                
                delayMicroseconds( SM_DELAY_US_BETWEEN_STEPS );
            }
            Serial.print("step: "); Serial.println(step);
            Serial.println("Search left done");
            if (!(notch())) {   //Not found, turn back
                setDirection( sm_Right );
                while( step < 0 )  {
                    stepIt();
                    step++;
                    delayMicroseconds( SM_DELAY_US_BETWEEN_STEPS );
                } 
                _error = 1;
                Serial.println("Returning back from Search right");
                Serial.println("Error: Sync not found") ; 
            }
        }
        Serial.print("Error: ");   Serial.println( _error );
        center();

    } else {
        _error = 2;
    }
};   

void Stepper::setSpeed( uint8_t speed ) {
    _speed = speed;
    _clockDiv = speed;
}; 

void Stepper::turn( int16_t steps ){
    if (steps < 0) {
        setDirection( sm_Left );
    } else {
        setDirection( sm_Right );
    }
    doSteps( abs( steps ) );
};


void Stepper::turnLeft( int16_t steps ) {
    setDirection( sm_Left );
    doSteps( abs( steps ) );
}; 

void Stepper::turnRight( int16_t steps ) {
    setDirection( sm_Right );
    doSteps( abs( steps ) );
}; 


/*
    NEMA17  -> 200 Schritte pro Umdrehung
            -> Treiber auf 1/4 - Schritt Betrieb eingestellt
            -> 800 Schritte pro Umdrehung
            -> 80 Schritte pro Position

    pos == dest   ->  no movement

    //destination in right half
    dest >= 0   &&  pos < 0        ->  dist = abs(pos) + dest  e.g.    260 = |-130| + 130    ->  turn right
    dest >= 0   &&  pos < dest     ->  dist = dest - pos       e.g.     65 = 194 - 130       ->  turn right  
    dest >= 0   &&  pos > dest     ->  dist = dest - pos       e.g.    -64 = 130 - 194       ->  turn left   

    //destination in left half
    dest < 0   &&  pos > 0         ->  dist = dest - pos       e.g.   -260 = -130 - 130      ->  turn left
    dest < 0   &&  pos < dest      ->  dist = dest - pos       e.g.     64 = -130 - -194     ->  turn right  
    dest < 0   &&  pos > dest      ->  dist = dest - pos       e.g.    -64 = -194 - -130     ->  turn left   
*/

void Stepper::moveTo( uint8_t pos ){ 
    int16_t distance = 0;
    int16_t dest = destPost( pos );        //the target position
    Serial.print( "dest: ");  Serial.println(dest);
    if (_position != dest) {                //if we are not already there...
        if (dest >= 0) {        //Destination in the right half
            if ( _position < 0) {
                distance = abs(_position) + dest ;
                Serial.print( "turn 1: ");  Serial.println( distance );
            }  else {
                distance = dest - _position;
                Serial.print( "turn 2: ");  Serial.println( distance );
            }  
        } else {                //Destination in the left half
            distance = dest - _position;
            Serial.print( "turn 3: ");  Serial.println( distance );
        }
        turn( distance );
        _position = _position + distance;
    }
};



int16_t Stepper::destPost( uint8_t pos ) {
    int16_t value;
    switch (pos ) {
        case 0 : value =  SM_CAMPOS_OFFSET; break; 
        case 1 : value =  1*SM_CAMPOS_STEP + SM_CAMPOS_OFFSET; break; 
        case 2 : value =  2*SM_CAMPOS_STEP + SM_CAMPOS_OFFSET; break; 
        case 3 : value =  3*SM_CAMPOS_STEP + SM_CAMPOS_OFFSET; break; 
        case 4 : value =  4*SM_CAMPOS_STEP + SM_CAMPOS_OFFSET; break; 
        case 5 : value =  5*SM_CAMPOS_STEP + SM_CAMPOS_OFFSET; break; 
        case 6 : value = -4*SM_CAMPOS_STEP + SM_CAMPOS_OFFSET; break; 
        case 7 : value = -3*SM_CAMPOS_STEP + SM_CAMPOS_OFFSET; break; 
        case 8 : value = -2*SM_CAMPOS_STEP + SM_CAMPOS_OFFSET; break; 
        case 9 : value = -1*SM_CAMPOS_STEP + SM_CAMPOS_OFFSET; break; 
        default: value = SM_CAMPOS_OFFSET;
    }
    return value;
}