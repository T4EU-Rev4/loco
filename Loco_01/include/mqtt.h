#ifndef mqtt_h
#define mqtt_h

#include "Arduino.h"
#include "wagon.h"

#define NR_OF_TOPICS    NR_OF_WAGONS+1

typedef void (*mqtt_callback_t) (uint16_t adr, uint8_t cmd, uint8_t val);		

enum MQTT_State {
  MQTT_None,
  MQTT_Connecting,
  MQTT_Connected,
  MQTT_Run
};

extern MQTT_State mqttState;

//prototypes
void mqtt_setup();
void mqtt_loop();

void mqtt_register_Callback(  mqtt_callback_t cbf );
void mqtt_enterState( MQTT_State mqs ) ;
void mqtt_Received( uint16_t adr, uint8_t cmd, uint8_t val );

struct TTopic {

  TTopic( char *t, uint16_t a ) : topic( t ), wagon( a ) {}

  char    *topic;      //the topic, we want to register
  uint16_t wagon;      //the decimal representation of the 2 letter county code
 
} ;






#endif