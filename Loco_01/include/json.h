#ifndef json_h
#define json_h

#include <Arduino.h>

#define NR_OF_MSGTYPES      4

enum jsMsgTyp {
    mt_boot,
    mt_topic,
    mt_device,
    mt_handshake
};

#define NR_OF_PARAMS        8       //How many elements are in the following enum

enum jsParam {      //List of the valid JSON parameters
    Country,   
    Sender,   
    Client,
    Topic,
    Wagon,
    Action,
    Pin,            //deprecated
    Status          //deprecated
};

#define BUFFERSIZE  256

extern String Location;

extern String MAC;
extern String TopicPrefix;
extern String TopicSend;
extern String TopicRec0;
extern String TopicRec1;    
extern char   toPublish[BUFFERSIZE];

extern boolean handshakeRequest;
extern uint16_t handshakeCount;

//Prototypes
void json_setMAC( String mac );
void json_createServerMsg( enum jsMsgTyp msgTyp );
void json_mqtt_callback(char *topic, byte *payload, unsigned int length);



#endif