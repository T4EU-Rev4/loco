#ifndef httpServer_h
#define httpServer_h

#include "Arduino.h"

#define HTTP_SERVER_PORT     80

void    http_Setup();

boolean http_NewValueAvail();
void    http_Clear();
String  http_getWagon();
uint8_t http_getCmd();
uint8_t http_getValue();


#endif