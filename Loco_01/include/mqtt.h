#ifndef mqtt_h
#define mqtt_h

#include "Arduino.h"
#include "wagon.h"

#define NR_OF_TOPICS    NR_OF_WAGONS+1

typedef void (*mqtt_callback_t) (uint16_t adr, uint8_t cmd, uint8_t val);		

//prototypes
void mqtt_setup();
void mqtt_loop();

void mqtt_register_Callback(  mqtt_callback_t cbf );


struct TTopic {

  TTopic( char *t, uint16_t a ) : topic( t ), wagon( a ) {}

  char    *topic;      //the topic, we want to register
  uint16_t wagon;      //the decimal representation of the 2 letter county code
 
} ;






#endif